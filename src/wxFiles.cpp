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
                              wxSAVE | wxOVERWRITE_PROMPT, owner);
	
    if(!filePath.IsEmpty()) {
        if((currFile = fopen(filePath.mb_str(wxConvUTF8), "wb")) == NULL) {
            MessageAlert("Unable to open the file for output.");
            return 0;
        }
        try {
            buffer = new BufferFile(currFile, true);
			long BasisTest=0;
			buffer->WriteLine("!   File created by MacMolPlt 6", true);
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

#ifdef __WXMAC__
#ifdef __MAC_USE_QUICKTIME__
#include <wx/mac/carbon/private.h>
#include <Movies.h>

typedef struct qtData {
	Media theMedia;
	ImageDescriptionHandle imageDesc;
	ImageSequence seqID;
	Rect DisplayRect;
};

void MolDisplayWin::WriteMovie(wxString & filepath) {
	//Create a QuickTime movie using the standard animation codecs with normal quality, and 
	//temporal compression. The final file is flattened for cross platform compatability
//	void MolDisplayWin::WriteMovie(FSSpec * targetFile, short type, bool IncludeEPlot) {

	FSRef mFSRef;
	OSErr myErr = myErr;
	FSSpec targetSpec;
	const char * t = filepath.mb_str(wxConvUTF8);
	OSStatus s = FSPathMakeRef((const UInt8 *) t, &mFSRef, false);
	FSCatalogInfoBitmap fields = kFSCatInfoFinderInfo;
	FSCatalogInfo info;
	if (s == noErr) {
		myErr = FSGetCatalogInfo(&mFSRef, fields, &info, NULL, &targetSpec, NULL);
	}
	if (myErr != noErr) return;
	
	Movie	theMovie = NULL;
		
	FSSpec tempSpec = targetSpec;
	strcpy((char *) &(tempSpec.name[1]), "MacMolPlt8933tempMovie");
	tempSpec.name[0] = 22;

	BeginOperation();
	ProgressInd->ChangeText("Creating movieÉ");
		
	myErr = EnterMovies();	//initialize the quicktime manager
	if (myErr != noErr) {
		MessageAlert("Error initializing QuickTime!");
		return;
	}
	bool IncludeEPlot = false;
		//Create the movie file and initialize file data
		//Use Quicktime creator code 'TVOD' instead of simpletext 'ttxt'
	short	resRefNum = 0;
	short	resId = 0;
	myErr = CreateMovieFile(&tempSpec, 'TVOD', smCurrentScript, createMovieFileDeleteCurFile,
							&resRefNum, &theMovie);
	if (myErr != noErr) {
		MessageAlert("Error creating movie file!");
	} else {
		int width, height;
		glCanvas->GetClientSize(&width, &height);
		Rect lDisplayRect={0,0,0,0};
		lDisplayRect.right = width;
		lDisplayRect.bottom = height;
		WindowRef * TempWindow;
		s = CreateNewWindow(kDocumentWindowClass, kWindowNoAttributes, &lDisplayRect, TempWindow);
		if (s != noErr) {
		}
//			WindowPtr	ScreenWindow = thisWindow;
//			WindowPtr	TempWindow = GetNewCWindow(windowID, NULL, (WindowPtr) -1);
//			if (!TempWindow) throw MemoryError();
//			thisWindow = TempWindow;
//			Rect	EPlotRect, SavedERect;
//			EPlotRect = lDisplayRect;
//			EPlotRect.left = EPlotRect.right;
//			EPlotRect.right = EPlotRect.left + (EPlotRect.bottom - EPlotRect.top);
//			Boolean KillEPlot = false;
			//If we are including an energy plot add space for it here
//			if (IncludeEPlot && (type == kQTFrameAnim)) {
//				if (!EPlotWin) {
//					new EnergyPlotWin(this);	//There must be an open energy plot
//					KillEPlot = true;
//					if (!EPlotWin) IncludeEPlot = false;
//				} else EPlotWin->GetWindowRect(&SavedERect);
//				lDisplayRect.right += EPlotRect.bottom - EPlotRect.top;
//				if (EPlotWin) {
//					EPlotWin->HideWindow();
//					EPlotWin->SetWindowRect(&EPlotRect);
//					EPlotWin->Reset(1);
//				}
//			}
//			short dh, dv;
//			dh = lDisplayRect.right - lDisplayRect.left;
//			dv = lDisplayRect.bottom - lDisplayRect.top;
//			::SizeWindow(TempWindow, dh, dv, true);	//Now correct size (but still hidden)
													//Create the video track
			Track theTrack = NewMovieTrack (theMovie, FixRatio(width,1),
											FixRatio(height,1), kNoVolume);
			if ((noErr == GetMoviesError())&&theTrack) {
				Media theMedia = NewTrackMedia (theTrack, VideoMediaType,
												60, // Video Time Scale
												NULL, 0);
				if ((noErr == GetMoviesError())&&theMedia) {
					myErr = BeginMediaEdits (theMedia);
					if (myErr == noErr) {
						//create the actual movie frames
						GWorldPtr	lgWorld=NULL;
						Rect		gRect = lDisplayRect;
						LocalToGlobal ((Point *) &(gRect.top));
						LocalToGlobal ((Point *) &(gRect.bottom));
						
						if (! NewGWorld (&lgWorld, 0, &gRect, (CTabHandle) NULL, (GDHandle) NULL,
										 (GWorldFlags) (pixPurge + useTempMem))) {
							long MaxCompressedSize;
							ImageSequence seqID;
							ImageDescriptionHandle imageDesc = (ImageDescriptionHandle)NewHandle(4);
							PixMapHandle myPixMap = GetPortPixMap(lgWorld);
							LockPixels (myPixMap);
							myErr = CompressSequenceBegin(&seqID, myPixMap, NULL, &lDisplayRect, &lDisplayRect, 0,
														  'rle ', bestCompressionCodec, codecNormalQuality, codecNormalQuality, 2, NULL,
														  codecFlagUpdatePreviousComp, imageDesc);
							//						myErr = CompressSequenceBegin(&seqID, myPixMap, NULL, &lDisplayRect, &DisplayRect, 0,
							//								'rle ', bestCompressionCodec, codecNormalQuality, codecNormalQuality, 0, NULL,
							//								codecFlagUpdatePreviousComp, imageDesc);
							GetMaxCompressionSize (myPixMap, &lDisplayRect,
												   0, codecNormalQuality, 'rle ', (CompressorComponent) anyCodec, &MaxCompressedSize);
							Handle Buffer = TempNewHandle(MaxCompressedSize, &myErr);
							if (!Buffer)
								Buffer = NewHandle(MaxCompressedSize);
							if (Buffer) {
								qtData myqtData = {theMedia, imageDesc, seqID, lDisplayRect};
	//							if (type == kQTFrameAnim) {
									CreateFrameMovie(lgWorld, Buffer, myqtData, IncludeEPlot);
	//							} else {
	//								CreateModeMovie(lgWorld, Buffer, myqtData);
	//							}
								DisposeHandle(Buffer);
							}
							myErr = CDSequenceEnd(seqID);
							if (lgWorld != NULL) DisposeGWorld (lgWorld);
							if (imageDesc) DisposeHandle((Handle) imageDesc);
						}
						
						myErr = EndMediaEdits (theMedia);
					}
					myErr = InsertMediaIntoTrack (theTrack, 0,/* track start time */
						0,        /* media start time */
						GetMediaDuration (theMedia),
						FixRatio(1,1));
				}
				myErr = AddMovieResource (theMovie, resRefNum, &resId, NULL);
			}
		if (resRefNum) {
			//		short resID=0;
						//Create the actual file as a flat data fork so it can be placed on the www
			ProgressInd->ChangeText("Flattening movieÉ");
			FlattenMovie(theMovie, flattenAddMovieToDataFork, &targetSpec, 'TVOD', smCurrentScript, 
						 createMovieFileDeleteCurFile, &resId, NULL);
			CloseMovieFile (resRefNum);
		}
		DisposeMovie (theMovie);
		DeleteMovieFile (&tempSpec);	//delete the temp file after disposing of the movie
//		thisWindow = ScreenWindow;
//		DisposeWindow(TempWindow);
//		if (EPlotWin) {
//			if (KillEPlot) delete EPlotWin;
//			else {
//				EPlotWin->SetWindowRect(&SavedERect);
//				EPlotWin->Reset(1);
//				EPlotWin->ShowWindow();
//			}
//			::SelectWindow(thisWindow);
//		}
	}
	ExitMovies();	//Close out quicktime as we are done with it for now
	FinishOperation();
}
void MolDisplayWin::CreateFrameMovie(GWorldPtr lgWorld, Handle CompressedData,
									 const qtData & myqtData, bool IncludeEPlot) {
	OSErr	myErr;
	long AnimateTime = Prefs->GetAnimateTime();
	if (AnimateTime <= 0) AnimateTime = 1;
	long SavedFrameNum = MainData->GetCurrentFrame();
	Rect	lDisplayRect = myqtData.DisplayRect;
	Rect	EPlotRect;
	EPlotRect = myqtData.DisplayRect;;
	EPlotRect.left = EPlotRect.right;
	EPlotRect.right = EPlotRect.left + (EPlotRect.bottom - EPlotRect.top);
	if (IncludeEPlot) lDisplayRect.right += (EPlotRect.bottom - EPlotRect.top);
	ProgressInd->SetScaleFactor(100.0/((float) MainData->NumFrames));
	for (long i=1; i<=MainData->NumFrames; i++) {
		ProgressInd->UpdateProgress(i);
		MainData->SetCurrentFrame(i);
		//Check for and update any surfaces depending on the screen plane
		Surface * temp = MainData->cFrame->SurfaceList;
		while (temp) {
			temp->RotateEvent(MainData);
			temp = temp->GetNextSurface();
		}
		MainData->ResetRotation();
		UpdateGLModel();
		DrawGL();

		//	create a pict of the current molecule display
		wxImage mImage = glCanvas->getImage(0,0);
		wxBitmap * mBitmap = new wxBitmap(mImage);
		//The following utilizes a Mac specific internal pair of calls
		//to get a PicHandle from a wxBitmap.
		wxBitmapRefData * mBitmapData = mBitmap->GetBitmapData();
		PicHandle tempPict = mBitmapData->GetPictHandle();

		CGrafPtr	lSavedPort;
		GDHandle	lSavedGDH;
		long		dataSize;
		unsigned char similarity;
		
		if (tempPict) {	//Got a PICT, now draw it into the printer port
			HLock(CompressedData);
			Ptr compressedDataPtr = *CompressedData;
			GetGWorld (&lSavedPort, &lSavedGDH);
			SetGWorld (lgWorld, NULL);
			
			EraseRect(&lDisplayRect);
			DrawPicture(tempPict, &(lDisplayRect));
			
			delete mBitmap;
			
	//		if (IncludeEPlot) {
	//			tempPict = EPlotWin->CreatePICT(72);
	//			if (tempPict) {
	//				SetGWorld (lgWorld, NULL);
					
	//				EraseRect(&EPlotRect);
	//				DrawPicture(tempPict, &(EPlotRect));
	//				KillPicture(tempPict);
	//				delete mBitmap;
	//			}
	//		}
			
			SetGWorld (lSavedPort, lSavedGDH);
			
			PixMapHandle myPixMap = GetPortPixMap(lgWorld);
			myErr = CompressSequenceFrame(myqtData.seqID, myPixMap, &lDisplayRect,
										  codecFlagUpdatePreviousComp, compressedDataPtr, &dataSize, &similarity, NULL);
			
			TimeValue test;//I don't use this value, but Carbon requires it
				myErr = AddMediaSample(myqtData.theMedia, CompressedData, 0,    /* no offset in data */
					dataSize, AnimateTime,
					(SampleDescriptionHandle)myqtData.imageDesc, 1,    /* one sample */
					0,    /* self-contained samples */
					&test);
		}
	}
	MainData->SetCurrentFrame(SavedFrameNum);
	MainData->ResetRotation();
	UpdateGLModel();
	DrawGL();
}

#endif
#endif
