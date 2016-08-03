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
#include "exportoptionsdialog.h"
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

//TODO: offer a normal mode animation
//		offer to add an energy plot to the frame animation
//		Add progress indicator
void MolDisplayWin::WriteGIFMovie(wxString & filepath, ExportOptionsDialog * dlg) {
	wxImageArray images;
	long AnimateTime = 10*Prefs->GetAnimateTime();
	ProgressInd = new Progress;
	if (!ProgressInd) throw MemoryError();
	ProgressInd->ChangeText("Creating Animated GIF");
	
	if (dlg->GetMovieChoice() == 0) {
		ProgressInd->SetScaleFactor(100.0f / MainData->NumFrames);
		for(int i = 1; i <= MainData->NumFrames; ++i) {
			if(!ProgressInd->UpdateProgress((float)i)) {
	//			goto myCLEANUP;
			}
			
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
	} else { //normal mode movie
		long width;
		long height;
		bool savedrawmode=false;
		long AnimateTime = 10*Prefs->GetAnimateTime();
		if(!MainData->cFrame->Vibs) {
			wxLogMessage(_("Error! No normal modes found when trying to create a normal mode animation. Aborted!"));
			return;
		}
		Frame * lFrame = MainData->cFrame;
		if (MainData->GetDrawMode()) {
			savedrawmode=true;
			if(!Prefs->GetAnimateMode()) MainData->SetDrawMode(false);
		}
		
		long cmode = (lFrame->NumAtoms) * (lFrame->Vibs->CurrentMode);
		long AnimationSpeed = Prefs->GetAnimationSpeed();
		float offsetFactor = 1.0f/(4.5f*AnimationSpeed);
		float VectorScale = Prefs->GetVectorScale();
		CPoint3D *ModeOffset = new CPoint3D[lFrame->NumAtoms];
		if(!ModeOffset) {
			wxLogMessage(_("Unable to allocate memory for ModeOffset. Aborted!"));
			return;
		}
		CPoint3D *SavedAtoms = new CPoint3D[lFrame->NumAtoms];
		if(!SavedAtoms) {
			wxLogMessage(_("Unable to allocate memory for SavedAtoms. Aborted!"));
			delete[] ModeOffset;
			return;
		}
		mpAtom *lAtoms = lFrame->Atoms;
		long iatm;
		for(iatm = 0; iatm < (lFrame->NumAtoms); ++iatm) {
			SavedAtoms[iatm] = lAtoms[iatm].Position;
			ModeOffset[iatm] = lFrame->Vibs->NormMode[iatm + cmode];
			ModeOffset[iatm] *= VectorScale;
		}
		MainData->ResetRotation();
		ReleaseLists();
		DrawGL();
		
		if(AnimateTime < 1) AnimateTime = 1;
		
		getCanvasSize(&width, &height);
		
		long npoint = 0;
		long inc = 1;
		
		ProgressInd->SetScaleFactor(100.0f / (float)(4 * AnimationSpeed));
		
		for(long i = 0; i < (4 * AnimationSpeed); i++) {
			if(!ProgressInd->UpdateProgress((float)i)) {
	//			goto myCLEANUP;
			}
			if((npoint == AnimationSpeed) || (npoint == -AnimationSpeed)) {
				inc *= -1;
				offsetFactor *= -1.0;
			}
			npoint += inc;
			
			wxImage frame = glCanvas->getImage(0, 0);
			wxQuantize::Quantize(frame, frame);
			if (frame.HasAlpha()) {
				frame.ConvertAlphaToMask();
			}
			//Hmm does any memory need to be freed up later?
			images.push_back(frame);
			
//			frameNumStr = wxString::Format(wxT("%d/%d"), i+1, (4 * AnimationSpeed));
//			frameNumText = new SWFText();
//			frameNumText->setFont(font);
//			frameNumText->setHeight(24);
//			frameNumText->setColor(0x00, 0x00, 0x00); /* TODO:  Get from wxMMP config */
//			frameNumText->moveTo(0,
//								 height - ((24.0 / 1024.0) * font->getAscent() + 3));
//			frameNumText->addString(frameNumStr.mb_str(wxConvUTF8));

			for (iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
				lAtoms[iatm].Position.x += offsetFactor * (ModeOffset[iatm].x);
				lAtoms[iatm].Position.y += offsetFactor * (ModeOffset[iatm].y);
				lAtoms[iatm].Position.z += offsetFactor * (ModeOffset[iatm].z);
			}
			
			MainData->ResetRotation();
			ReleaseLists();
			DrawGL();

//			wxImage frame = glCanvas->getImage(0, 0);
//			wxQuantize::Quantize(frame, frame);
//			if (frame.HasAlpha()) {
//				frame.ConvertAlphaToMask();
//			}
//			//Hmm does any memory need to be freed up later?
//			images.push_back(frame);
		}

		for (iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
			lAtoms[iatm].Position = SavedAtoms[iatm];
		}
		MainData->ResetRotation();
		
		MainData->SetDrawMode(savedrawmode);
		ReleaseLists();
		DrawGL();
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

myCLEANUP:
	delete ProgressInd;
	ProgressInd = NULL;
	
}
#endif
