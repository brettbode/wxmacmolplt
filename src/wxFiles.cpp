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

extern WinPrefs *	gPreferences;

	//Prompt for a filename and then write out a valid input file for GAMESS
long InputData::WriteInputFile(MoleculeData * lData, MolDisplayWin * owner) {

    FILE *currFile = NULL;
    BufferFile *buffer = NULL;
	
    wxString filePath = wxFileSelector(wxT("Save As"), wxT(""), wxT(""), wxT(""),
                              wxT("GAMESS input files (*.inp)|*.inp"),
#if wxCHECK_VERSION(2,9,0)
                              wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
#else
                              wxSAVE | wxOVERWRITE_PROMPT,
#endif
							  owner);
	
    if(!filePath.IsEmpty()) {
        if((currFile = fopen(filePath.mb_str(wxConvUTF8), "wb")) == NULL) {
            MessageAlert("Unable to open the file for output.");
            return 0;
        }
        try {
            buffer = new BufferFile(currFile, true);
			long BasisTest=0;
			buffer->WriteLine("!   File created by MacMolPlt "wxMacMolPlt_VERSION, true);
			if (Control) Control->WriteToFile(buffer, this, lData->GetNumElectrons());
			if (DFT) DFT->WriteToFile(buffer, this);
			if (System) System->WriteToFile(buffer);
			if (Basis) BasisTest = Basis->WriteToFile(buffer, lData);
			if (Guess) Guess->WriteToFile(buffer, this, lData);
			if (SCF) SCF->WriteToFile(buffer, this);
			if (MP2) MP2->WriteToFile(buffer, this);
			if (StatPt) StatPt->WriteToFile(buffer, this);
			if (Hessian) Hessian->WriteToFile(buffer, this);
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
    }
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

