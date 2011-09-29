/////////////////////////////////////////////////////////////////////////////
// Name:        qtexport.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri 23 Feb 08:44:29 2007
// RCS-ID:      
// Copyright:   (c) 2007 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "qtexport.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "Globals.h"
#include "qtexport.h"
#include "energyplotdialog.h"

////@begin XPM images
////@end XPM images

/*!
 * QTExport type definition
 */

IMPLEMENT_DYNAMIC_CLASS( QTExport, wxDialog )

/*!
 * QTExport event table definition
 */

BEGIN_EVENT_TABLE( QTExport, wxDialog )

////@begin QTExport event table entries
	EVT_RADIOBOX( ID_QT_MOVIE_RADIO, QTExport::OnQtMovieRadioSelected )

////@end QTExport event table entries

END_EVENT_TABLE()

/*!
 * QTExport constructors
 */

QTExport::QTExport()
{
    Init();
}

QTExport::QTExport( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

/*!
 * QTExport creator
 */

bool QTExport::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin QTExport creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end QTExport creation
    return true;
}

/*!
 * QTExport destructor
 */

QTExport::~QTExport()
{
////@begin QTExport destruction
////@end QTExport destruction
}

/*!
 * Member initialisation 
 */

void QTExport::Init()
{
////@begin QTExport member initialisation
    mMovieType = NULL;
    mEPlotCheck = NULL;
    mQTChoice = NULL;
    mKeyFrameEdit = NULL;
////@end QTExport member initialisation
}
/*!
 * Control creation for QTExport
 */

void QTExport::CreateControls()
{    
////@begin QTExport content construction
    QTExport* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxString mMovieTypeStrings[] = {
        _("&Frame Animation"),
        _("&Normal Mode animation")
    };
    mMovieType = new wxRadioBox( itemDialog1, ID_QT_MOVIE_RADIO, _("Type of Movie"), wxDefaultPosition, wxDefaultSize, 2, mMovieTypeStrings, 0, wxRA_SPECIFY_ROWS );
    mMovieType->SetSelection(0);
    itemBoxSizer2->Add(mMovieType, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mEPlotCheck = new wxCheckBox( itemDialog1, ID_EPLOT_CHECK, _("Include Energy Plot"), wxDefaultPosition, wxDefaultSize, 0 );
    mEPlotCheck->SetValue(false);
    if (ShowToolTips())
        mEPlotCheck->SetToolTip(_("click to display the energy plot next to the animation for the frame animation."));
    itemBoxSizer2->Add(mEPlotCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticLine* itemStaticLine5 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine5, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("QuickTime Compressor:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxString mQTChoiceStrings[] = {
        _("Cinepak"),
        _("Graphics"),
        _("Animation"),
        _("MPEG4")
    };
    mQTChoice = new wxChoice( itemDialog1, ID_QT_COMPRESSOR_CHOICE, wxDefaultPosition, wxDefaultSize, 4, mQTChoiceStrings, 0 );
    itemBoxSizer6->Add(mQTChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("Key frame rate:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mKeyFrameEdit = new wxTextCtrl( itemDialog1, ID_KEY_FRAME_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mKeyFrameEdit->SetToolTip(_("0 or 1 will produce a key frame every frame, higher values will result in smaller files, but lower quality."));
    itemBoxSizer9->Add(mKeyFrameEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer12, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton13 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemButton13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton14 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton14->SetDefault();
    itemBoxSizer12->Add(itemButton14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end QTExport content construction
	mQTChoice->SetSelection(3);
	mKeyFrameEdit->SetValue(_T("2"));
}

/*!
* wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_QT_MOVIE_RADIO
 */

void QTExport::OnQtMovieRadioSelected( wxCommandEvent& event )
{
	mEPlotCheck->Enable(mMovieType->GetSelection() == 0);
	event.Skip();
}

/*!
 * Should we show tooltips?
 */

bool QTExport::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap QTExport::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin QTExport bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end QTExport bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon QTExport::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin QTExport icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end QTExport icon retrieval
}

void QTExport::EnableFrameMovie(bool state) {
	if (state) {
		mMovieType->SetSelection(0);
		mMovieType->Enable(0, true);
		mEPlotCheck->Enable(true);
	} else {
		mMovieType->SetSelection(1);
		mMovieType->Enable(0, false);
		mEPlotCheck->Enable(false);
	}
}
void QTExport::SetMovieChoice(int v) {
	mMovieType->SetSelection(v);
}
void QTExport::EnableModeMovie(bool state) {
	mMovieType->SetSelection(0);
	mMovieType->Enable(1, state);
}

int QTExport::GetMovieChoice(void) const {
	return mMovieType->GetSelection();
}
bool QTExport::AddEnergyPlot(void) const {
	return mEPlotCheck->IsChecked();
}
int QTExport::GetCompressorChoice(void) const {
	return mQTChoice->GetSelection();
}
int QTExport::GetKeyFrameRate(void) const {
	long result = 0;
	wxString val = mKeyFrameEdit->GetValue();
	val.ToLong(&result);
	return result;
}

#ifdef __MAC_USE_QUICKTIME__
#include "GlobalExceptions.h"
#include "Progress.h"
#include "MoleculeData.h"
#include "MolDisplayWin.h"
#include <QuickTime/Movies.h>

#if wxCHECK_VERSION(2, 9, 0)
#include <wx/osx/private.h>
//This is a gross hack to get access to the picHandle in the wxBitmapRefData class
//that is no longer provided in the wx headers
class wxBitmapRefData {
public:
	PicHandle     GetPictHandle();
};
#else
#include <wx/mac/carbon/private.h>
#endif

typedef struct qtData {
	Media theMedia;
	ImageDescriptionHandle imageDesc;
	ImageSequence seqID;
	Rect DisplayRect;
	Rect FullRect;
};

void MolDisplayWin::WriteQTMovie(wxString & filepath) {
	//Create a QuickTime movie using the standard animation codecs with normal quality, and 
	//temporal compression. The final file is flattened for cross platform compatability
	
	QTExport * QTOptions = new QTExport(this);
	//setup controls for the current data
	if (MainData->GetNumFrames() > 1) { //default to frame animation
		QTOptions->SetMovieChoice(0);
	} else {
		QTOptions->EnableFrameMovie(false);
	}
	if (MainData->cFrame->GetNumberNormalModes() <= 0) {
		QTOptions->EnableModeMovie(false);
	}
	
	if (QTOptions->ShowModal() != wxID_OK) {
		//user cancelled the operation
		QTOptions->Destroy();
		return;
	}
	//retrieve the value of each option
	int MovieType = QTOptions->GetMovieChoice();
	bool IncludeEPlot = QTOptions->AddEnergyPlot();
	int compressorChoice = QTOptions->GetCompressorChoice();
	int keyFrameRate = QTOptions->GetKeyFrameRate();
	if (keyFrameRate < 0) keyFrameRate = 0;
	
	QTOptions->Destroy();
	
	CodecType mCodec;
	switch (compressorChoice) {
		case 0:
			mCodec = kCinepakCodecType;
			break;
		case 1:
			mCodec = kGraphicsCodecType;
			break;
		case 2:
			mCodec = kAnimationCodecType;
			break;
		case 3:
		default:
			mCodec = kMPEG4VisualCodecType;
	}

	OSStatus s;
	OSErr myErr = myErr;
	FSSpec targetSpec;
	//ugh I need to get an FSSpec to hand to quicktime, but these calls only seem to work if
	//the file already exists...
	const char * t = filepath.mb_str(wxConvUTF8);
	FILE * temp = fopen(t, "wb");
	fclose(temp);

#ifdef __WXOSX_COCOA__
	//This function is not found in the wxCocoa implementation, it is probably possible to work around it
	//Otherwise the code appears to link and run currently. However, it is probably better to redue the
	//code to use the Cocoa qtKit framework rather than the old Carbon QT library.
	
	//This path is not tested as the current Cocoa code does not properly support the extended save dialog.
	//	void wxMacFilename2FSSpec( const wxString& path , FSSpec *spec )
	{
		OSStatus err = noErr;
		FSRef fsRef;
		wxMacPathToFSRef( filepath , &fsRef );
		err = FSGetCatalogInfo(&fsRef, kFSCatInfoNone, NULL, NULL, &targetSpec, NULL);
		verify_noerr( err );
	}
#else
	wxMacFilename2FSSpec(filepath, &targetSpec);
#endif
	
	Movie	theMovie = NULL;
		
	FSSpec tempSpec = targetSpec;
	strcpy((char *) &(tempSpec.name[1]), "MacMolPlt8933tempMovie");
	tempSpec.name[0] = 22;

	BeginOperation();
	ProgressInd->ChangeText("Creating movie...");
		
	myErr = EnterMovies();	//initialize the quicktime manager
	if (myErr != noErr) {
		FinishOperation();
		MessageAlert("Error initializing QuickTime!");
		return;
	}
		//Create the movie file and initialize file data
		//Use Quicktime creator code 'TVOD' instead of simpletext 'ttxt'
	short	resRefNum = 0;
	short	resId = 0;
	myErr = CreateMovieFile(&tempSpec, 'TVOD', smCurrentScript, createMovieFileDeleteCurFile,
							&resRefNum, &theMovie);
	if (myErr != noErr) {
		MessageAlert("Error creating movie file!");
	} else {
		bool KillEPlot = false;
		int width, height, savedEPlotWidth, savedEPlotHeight;
		glCanvas->GetClientSize(&width, &height);
		Rect lDisplayRect={0,0,0,0};
		lDisplayRect.right = width;
		lDisplayRect.bottom = height;
		Rect		gRect = lDisplayRect;

		Rect EPlotRect = lDisplayRect;
		//If we are including an energy plot add space for it here
		if (IncludeEPlot && (MovieType == 0)) {
			EPlotRect.left = EPlotRect.right;
			EPlotRect.right = EPlotRect.left + height;
			if (!energyPlotWindow) {
				energyPlotWindow = new EnergyPlotDialog(this);
				KillEPlot = true;
			} else {
				energyPlotWindow->GetSize(&savedEPlotWidth, &savedEPlotHeight);
			}
			gRect.right += height;
			width += height;
			energyPlotWindow->Show(false);
			energyPlotWindow->SetSize(height, height);
			energyPlotWindow->Update();	//This is needed to initialise the window if we just created it
		}

		LocalToGlobal ((Point *) &(gRect.top));
		LocalToGlobal ((Point *) &(gRect.bottom));
		WindowRef TempWindow;
		s = CreateNewWindow(kDocumentWindowClass, kWindowNoAttributes, &gRect, &TempWindow);
		if (s == noErr) {
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
						
						if (! NewGWorld (&lgWorld, 0, &gRect, (CTabHandle) NULL, (GDHandle) NULL,
										 (GWorldFlags) (pixPurge + useTempMem))) {
							long MaxCompressedSize;
							ImageSequence seqID;
							ImageDescriptionHandle imageDesc = (ImageDescriptionHandle)NewHandle(4);
							PixMapHandle myPixMap = GetPortPixMap(lgWorld);
							LockPixels (myPixMap);
							myErr = CompressSequenceBegin(&seqID, myPixMap, NULL, &gRect, &gRect, 0,
														  mCodec, bestCompressionCodec, codecNormalQuality, codecNormalQuality, keyFrameRate, NULL,
														  codecFlagUpdatePreviousComp, imageDesc);
							GetMaxCompressionSize (myPixMap, &gRect,
												   0, codecNormalQuality, mCodec, (CompressorComponent) anyCodec, &MaxCompressedSize);
							Handle Buffer = TempNewHandle(MaxCompressedSize, &myErr);
							if (!Buffer)
								Buffer = NewHandle(MaxCompressedSize);
							if (Buffer) {
								qtData myqtData = {theMedia, imageDesc, seqID, lDisplayRect, gRect};
								if (MovieType == 0) {
									CreateFrameMovie(lgWorld, Buffer, myqtData, IncludeEPlot);
								} else {
									CreateModeMovie(lgWorld, Buffer, myqtData);
								}
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
						//Create the actual file as a flat data fork so it can be placed on the www
				ProgressInd->ChangeText("Flattening movieÉ");
				FlattenMovie(theMovie, flattenAddMovieToDataFork, &targetSpec, 'TVOD', smCurrentScript, 
							 createMovieFileDeleteCurFile, &resId, NULL);
				CloseMovieFile (resRefNum);
			}
			DisposeWindow(TempWindow);
		}
		DisposeMovie (theMovie);
		DeleteMovieFile (&tempSpec);	//delete the temp file after disposing of the movie

		if (energyPlotWindow) {
			if (KillEPlot) {
				delete energyPlotWindow;
				energyPlotWindow = NULL;
			} else {
				energyPlotWindow->SetSize(savedEPlotWidth, savedEPlotHeight);
				energyPlotWindow->FrameChanged();
				energyPlotWindow->Show(true);
			}
		}
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
	EPlotRect = myqtData.DisplayRect;
	EPlotRect.left = EPlotRect.right;
	EPlotRect.right = myqtData.FullRect.right;
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
		ReleaseLists();
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
		
		if (tempPict) {	//Got a PICT, draw it into the GWorld
			HLock(CompressedData);
			Ptr compressedDataPtr = *CompressedData;
			GetGWorld (&lSavedPort, &lSavedGDH);
			SetGWorld (lgWorld, NULL);
			
			EraseRect(&lDisplayRect);
			DrawPicture(tempPict, &(lDisplayRect));
			
			delete mBitmap;
			
			if (IncludeEPlot) {
				energyPlotWindow->FrameChanged();
				wxBitmap * ePlotBitMap = NULL;
				energyPlotWindow->CopyToBitMap(&ePlotBitMap);
				if (ePlotBitMap) {
					wxBitmapRefData * mBitmapData = mBitmap->GetBitmapData();
					PicHandle tempPict = mBitmapData->GetPictHandle();
					if (tempPict) {
						SetGWorld (lgWorld, NULL);
						
						EraseRect(&EPlotRect);
						DrawPicture(tempPict, &(EPlotRect));
					}
					delete ePlotBitMap;
				}
			}
			
			SetGWorld (lSavedPort, lSavedGDH);
			
			PixMapHandle myPixMap = GetPortPixMap(lgWorld);
			myErr = CompressSequenceFrame(myqtData.seqID, myPixMap, &(myqtData.FullRect),
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
	ReleaseLists();
	DrawGL();
}
void MolDisplayWin::CreateModeMovie(GWorldPtr lgWorld, Handle CompressedData,
									const qtData & myqtData) {
	float		offsetFactor;
	bool		savedrawmode=false;
	Rect		lDisplayRect = myqtData.DisplayRect;
	
	if (!MainData->cFrame->Vibs) return;
	Frame * lFrame = MainData->cFrame;
	if (MainData->GetDrawMode()) {
		savedrawmode = true;
		if (!Prefs->GetAnimateMode()) MainData->SetDrawMode(false);
	}
	long cmode = (lFrame->NumAtoms)*(lFrame->Vibs->CurrentMode);
	offsetFactor = 1.0/(4.5*Prefs->GetAnimationSpeed());
	float	VectorScale = Prefs->GetVectorScale();
	long AnimationSpeed = Prefs->GetAnimationSpeed();
	CPoint3D * ModeOffset = new CPoint3D[lFrame->NumAtoms];
	CPoint3D * SavedAtoms = new CPoint3D[lFrame->NumAtoms];
	if (!ModeOffset || !SavedAtoms) {
		return;	//insufficient memory
	}
	mpAtom * lAtoms = lFrame->Atoms;
	long iatm;
	for (iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
		SavedAtoms[iatm] = lAtoms[iatm].Position;
		ModeOffset[iatm] = lFrame->Vibs->NormMode[iatm+cmode];
		ModeOffset[iatm] *= VectorScale;
	}
	MainData->ResetRotation();
	ReleaseLists();
	DrawGL();

	OSErr	myErr;
	long npoint = 0;
	long inc = 1;
	ProgressInd->SetScaleFactor(100.0/((float) 4*AnimationSpeed));
	for (long i=0; i<4*AnimationSpeed; i++) {
		ProgressInd->UpdateProgress(i);
		if ((npoint==AnimationSpeed)||(npoint==-AnimationSpeed)) {
			inc *= -1;
			offsetFactor *= -1.0;
		}
		npoint += inc;

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
		
		if (tempPict) {	//Got a PICT, draw it into the GWorld
			HLock(CompressedData);
			Ptr compressedDataPtr = *CompressedData;

			GetGWorld (&lSavedPort, &lSavedGDH);
			SetGWorld (lgWorld, NULL);
			
			EraseRect(&lDisplayRect);
			DrawPicture(tempPict, &(lDisplayRect));

			delete mBitmap;
			
			SetGWorld (lSavedPort, lSavedGDH);
			
			PixMapHandle myPixMap = GetPortPixMap(lgWorld);
			myErr = CompressSequenceFrame(myqtData.seqID, myPixMap, &lDisplayRect,
										  codecFlagUpdatePreviousComp, compressedDataPtr, &dataSize, &similarity, NULL);
			
			TimeValue test;//I don't use this value, but Carbon requires it
			myErr = AddMediaSample(myqtData.theMedia, CompressedData, 0,    /* no offset in data */
					dataSize, 1,
					(SampleDescriptionHandle)myqtData.imageDesc, 1,    /* one sample */
					0,    /* self-contained samples */
					&test);
		}
		for (iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
			lAtoms[iatm].Position.x += offsetFactor*(ModeOffset[iatm].x);
			lAtoms[iatm].Position.y += offsetFactor*(ModeOffset[iatm].y);
			lAtoms[iatm].Position.z += offsetFactor*(ModeOffset[iatm].z);
		}

		MainData->ResetRotation();
		ReleaseLists();
		DrawGL();
	}
	for (iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
		lAtoms[iatm].Position = SavedAtoms[iatm];	
	}
	MainData->ResetRotation();

	if (ModeOffset) delete [] ModeOffset;
	if (SavedAtoms) delete [] SavedAtoms;
	MainData->SetDrawMode(savedrawmode);
	ReleaseLists();
	DrawGL();
}

#endif
