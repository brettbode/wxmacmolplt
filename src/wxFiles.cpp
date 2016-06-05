/*
 *  (c) 2006 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* Routines to manage the open/save file interfaces

	Created by Brett Bode 3/2006
*/

#include "Globals.h"
#include "GlobalExceptions.h"
#include "Progress.h"
#include "MoleculeData.h"
#include "MolDisplayWin.h"
#include "BFiles.h"
#include "myFiles.h"
#include "InputData.h"
#include "Prefs.h"
#include <wx/wfstream.h>
#include <wx/anidecod.h>

extern WinPrefs *	gPreferences;

	//Prompt for a filename and then write out a valid input file for GAMESS
long InputData::WriteInputFile(MoleculeData * lData, MolDisplayWin * owner) {

    wxString filePath = wxFileSelector(wxT("Save As"), wxT(""), wxT(""), wxT(""),
                              wxT("GAMESS input files (*.inp)|*.inp"),
#if wxCHECK_VERSION(2,9,0)
                              wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
#else
                              wxSAVE | wxOVERWRITE_PROMPT,
#endif
							  owner);
	
    if(!filePath.IsEmpty()) {
		return WriteInputFile(filePath, lData, owner);
	}
	return -1;
}

long InputData::WriteInputFile(const wxString &filePath, MoleculeData * lData, MolDisplayWin * owner) {

    FILE *currFile = NULL;
    BufferFile *buffer = NULL;
	
    if((currFile = fopen(filePath.mb_str(wxConvUTF8), "wb")) == NULL) {
        MessageAlert("Unable to open the file for output.");
        return 0;
    }
    try {
		lData->PruneUnusedFragments();
        buffer = new BufferFile(currFile, true);
		long BasisTest=0;
		buffer->WriteLine("!   File created by MacMolPlt " wxMacMolPlt_VERSION, true);
		if (Control) Control->WriteToFile(buffer, this, lData->GetNumElectrons());
		DFT.WriteToFile(buffer, this);
		if (System) System->WriteToFile(buffer);
		if (Basis && (Control->GetRunType() != G3MP2)) BasisTest = Basis->WriteToFile(buffer, lData);
		if (Guess) Guess->WriteToFile(buffer, this, lData);
		if (SCF) SCF->WriteToFile(buffer, this);
		if (MP2) MP2->WriteToFile(buffer, this);
		if (StatPt) StatPt->WriteToFile(buffer, this);
		if (Hessian) Hessian->WriteToFile(buffer, this);
		FMO.WriteToFile(buffer, lData);
		if (Data) Data->WriteToFile(buffer, lData, owner->GetPrefs(), BasisTest);
		if (Guess) Guess->WriteVecGroup(buffer, lData);
    }
    catch (MemoryError) {
        MessageAlert("Not enough memory to open the file for writing.");
    }
	if(buffer) {
		delete buffer;
	}
	fclose(currFile);
	return 1;
}

	//Prompt for the dat file name and return a buffer to it
BufferFile * OpenDatFile(void) {
	BufferFile * Buffer = NULL;

    wxString filename = wxFileSelector(wxT("Choose a GAMESS .DAT file corresponding to this log file."));
    if (!filename.empty()) {
        FILE * myfile = fopen(filename.mb_str(wxConvUTF8), "rb");
        if (myfile == NULL) {
            MessageAlert("Unable to open the selected file!");
        } else {
			Buffer = new BufferFile(myfile, false);
		}
	}
				
	return Buffer;
}

#if wxCHECK_VERSION(2,9,0)
#include <wx/quantize.h>
void MolDisplayWin::WriteGIFMovie(wxString & filepath) {
	wxImageArray images;
	long AnimateTime = 10*Prefs->GetAnimateTime();
	
	for(int i = 1; i <= MainData->NumFrames; ++i) {
//		if(!ProgressInd->UpdateProgress((float)i)) {
//			goto CLEANUP;
//		}
		
		MainData->SetCurrentFrame(i);
		
		Surface *temp = MainData->cFrame->SurfaceList;
		while(temp) {
			temp->RotateEvent(MainData);
			temp = temp->GetNextSurface();
		}
		MainData->ResetRotation();
		ReleaseLists();
		DrawGL();
		
		wxImage frame = glCanvas->getImage(0, 0);
		wxQuantize::Quantize(frame, frame);
		if (frame.HasAlpha()) {
			frame.ConvertAlphaToMask();
		}
		//Hmm does any memory need to be freed up later?
		images.push_back(frame);
	}
	
	if(!filepath.Lower().Matches(wxT("*.gif"))) {
		
		filepath.Append(wxT(".gif"));
	}
	wxFileOutputStream outFile(filepath);
	if (outFile.IsOk()) {
//	bool wxGIFHandler::SaveAnimation(const wxImageArray& images,
//									 wxOutputStream *stream, bool verbose, int delayMilliSecs)
		wxGIFHandler * gfhandler = (wxGIFHandler *) wxImage::FindHandler(wxBITMAP_TYPE_GIF);
		gfhandler->SaveAnimation(images, &outFile, true, AnimateTime);
	} else {
		wxLogMessage(_("Unable to open the requested file. Aborted!"));
	}
}
#endif
