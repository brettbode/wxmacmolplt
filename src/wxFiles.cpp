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
#include "energyplotdialog.h"
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

void MolDisplayWin::WriteGIFMovie(wxString & filepath, ExportOptionsDialog * dlg) {
	wxImageArray images;
	long AnimateTime = 10*Prefs->GetAnimateTime();
	bool killEPlotWin=false, includeEP = false;
	int savedEPlotWidth, savedEPlotHeight;
	wxBitmap  *bmp=NULL, *ePlotbmp=NULL;
	wxMemoryDC memDC, ePlotmemDC;
	long saveFrame = MainData->GetCurrentFrame();

	try {
		ProgressInd = new Progress;
		if (!ProgressInd) throw MemoryError();
		ProgressInd->ChangeText("Creating Animated GIF... Patience...");
			//Setup a memory draw context to buffer each image at the full resolution
		bmp = new wxBitmap(dlg->getWidth(),
						   dlg->getHeight());
		memDC.SelectObject(*bmp);
			//Set the line width appropriate to the resolution (reset at the end)
		Prefs->SetLineWidth(dlg->getImageRatio());

		if (dlg->GetMovieChoice() == 0) {	//Multi-frame animation
			ProgressInd->SetScaleFactor(100.0f / MainData->NumFrames);
			includeEP = dlg->AddEnergyPlot();
			
			if(includeEP) {	//Add the energy plot to the movie
				if(!energyPlotWindow) {	//If the energy plot window is not open, temporarily open it
					energyPlotWindow = new EnergyPlotDialog(this);
					killEPlotWin = true;
				}
				else {
					energyPlotWindow->GetSize(&savedEPlotWidth, &savedEPlotHeight);
				}
				energyPlotWindow->SetSize(dlg->getHeight(), dlg->getHeight());
				energyPlotWindow->Show(true);
				energyPlotWindow->Update();
				//Setup a draw context for the extended image size including the energy plot
				ePlotbmp = new wxBitmap(dlg->getWidth()+dlg->getHeight(),
										dlg->getHeight());
				ePlotmemDC.SelectObject(*ePlotbmp);
			}

			for(int i = 1; i <= MainData->NumFrames; ++i) {
				if(!ProgressInd->UpdateProgress((float)i)) {
					throw UserCancel();
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
					//Generate the possibly hi-res image
				glCanvas->GenerateHiResImageForExport(&memDC);
				wxImage frame;
				
				if(dlg->AddEnergyPlot()) {
						//update the energy plot, copy it, then copy both bitmaps to the extended buffer
					energyPlotWindow->FrameChanged();
					wxBitmap *epBitmap = NULL;
					energyPlotWindow->CopyToBitMap(&epBitmap);
					if(epBitmap == NULL) {
						wxLogMessage(_("Error! Unable to copy energy plot. Aborted!"));
						throw UserCancel();
					}
					ePlotmemDC.Clear();
					ePlotmemDC.DrawBitmap(*bmp, 0, 0);
					ePlotmemDC.DrawBitmap(*epBitmap, dlg->getWidth(), 0);
					//finally create the image off of the extended bitmap.
					frame = ePlotbmp->ConvertToImage();
					delete epBitmap;
				} else	// or standard bitmap
					frame = bmp->ConvertToImage();
					//GIFs require this step to reduce the colors
				wxQuantize::Quantize(frame, frame);
				if (frame.HasAlpha()) {
					frame.ConvertAlphaToMask();
				}
				images.push_back(frame);
			}
		} else { //normal mode movie
			bool savedrawmode=false;
			long AnimateTime = 10*Prefs->GetAnimateTime();
			if(!MainData->cFrame->Vibs) {
				wxLogMessage(_("Error! No normal modes found when trying to create a normal mode animation. Aborted!"));
				throw UserCancel();
			}
			Frame * lFrame = MainData->cFrame;
			
			long cmode = (lFrame->NumAtoms) * (lFrame->Vibs->CurrentMode);
			long AnimationSpeed = Prefs->GetAnimationSpeed();
			float offsetFactor = 1.0f/(4.5f*AnimationSpeed);
			float VectorScale = Prefs->GetVectorScale();
			CPoint3D *ModeOffset = new CPoint3D[lFrame->NumAtoms];
			if(!ModeOffset) {
				wxLogMessage(_("Unable to allocate memory for ModeOffset. Aborted!"));
				throw UserCancel();
			}
			CPoint3D *SavedAtoms = new CPoint3D[lFrame->NumAtoms];
			if(!SavedAtoms) {
				wxLogMessage(_("Unable to allocate memory for SavedAtoms. Aborted!"));
				delete[] ModeOffset;
				throw UserCancel();
			}
			if (MainData->GetDrawMode()) {
				savedrawmode=true;
				if(!Prefs->GetAnimateMode()) MainData->SetDrawMode(false);
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
			
			long npoint = 0;
			long inc = 1;
			
			ProgressInd->SetScaleFactor(100.0f / (float)(4 * AnimationSpeed));
			
			for(long i = 0; i < (4 * AnimationSpeed); i++) {
				if(!ProgressInd->UpdateProgress((float)i)) {
					for (iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
						lAtoms[iatm].Position = SavedAtoms[iatm];
					}
					MainData->ResetRotation();
					
					MainData->SetDrawMode(savedrawmode);
					ReleaseLists();
					DrawGL();
					
					delete [] ModeOffset;
					delete [] SavedAtoms;

					throw UserCancel();
				}
				if((npoint == AnimationSpeed) || (npoint == -AnimationSpeed)) {
					inc *= -1;
					offsetFactor *= -1.0;
				}
				npoint += inc;
				
				glCanvas->GenerateHiResImageForExport(&memDC);
				wxImage frame = bmp->ConvertToImage();
				wxQuantize::Quantize(frame, frame);
				if (frame.HasAlpha()) {
					frame.ConvertAlphaToMask();
				}

				images.push_back(frame);
				
				for (iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
					lAtoms[iatm].Position.x += offsetFactor * (ModeOffset[iatm].x);
					lAtoms[iatm].Position.y += offsetFactor * (ModeOffset[iatm].y);
					lAtoms[iatm].Position.z += offsetFactor * (ModeOffset[iatm].z);
				}
				
				MainData->ResetRotation();
				ReleaseLists();
				DrawGL();
			}

			for (iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
				lAtoms[iatm].Position = SavedAtoms[iatm];
			}
			MainData->ResetRotation();
			
			MainData->SetDrawMode(savedrawmode);
			ReleaseLists();
			DrawGL();
			
			delete [] ModeOffset;
			delete [] SavedAtoms;
		}
		
		//Series of frames is complete, now save to the requested file.
		if(!filepath.Lower().Matches(wxT("*.gif"))) {
			
			filepath.Append(wxT(".gif"));
		}
		wxFileOutputStream outFile(filepath);
		if (outFile.IsOk()) {
			wxGIFHandler * gfhandler = (wxGIFHandler *) wxImage::FindHandler(wxBITMAP_TYPE_GIF);
			gfhandler->SaveAnimation(images, &outFile, true, AnimateTime);
		} else {
			wxLogMessage(_("Unable to open the requested file. Aborted!"));
		}
	}
	catch (UserCancel & p) {
		//User canceled, just cleanup and exit
	}

	//Cleanup any memory and reset the windows to their entry state
	delete ProgressInd;
	ProgressInd = NULL;
	Prefs->SetLineWidth(1);
	memDC.SelectObject(wxNullBitmap); // bmp has now been destroyed
	ePlotmemDC.SelectObject(wxNullBitmap);
	if(includeEP) {
		if(killEPlotWin) {
			delete energyPlotWindow;
			energyPlotWindow = NULL;
		}
		else {
			energyPlotWindow->SetSize(savedEPlotWidth, savedEPlotHeight);
			energyPlotWindow->FrameChanged();
		}
	}
	ChangeFrames(saveFrame);
	
}
#endif
