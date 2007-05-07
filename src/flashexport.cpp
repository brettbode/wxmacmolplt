#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/mstream.h"

#include "Globals.h"
#include "swfexport.h"

#ifdef HAVE_LIBMING
#include <mingpp.h>
#endif

void SWFExport::SetMovieChoice(int v) {
	mMovieType->SetSelection(v);
}

void SWFExport::EnableFrameMovie(bool v) {
	if(v) {
		mMovieType->SetSelection(0);
		mMovieType->Enable(0, true);
		mEPlotCheck->Enable(true);
	}
	else {
		mMovieType->SetSelection(1);
		mMovieType->Enable(0, false);
		mEPlotCheck->Enable(false);
	}
}

void SWFExport::EnableModeMovie(bool b) {
	mMovieType->SetSelection(0);
	mMovieType->Enable(1, b);
}

int SWFExport::GetMovieChoice(void) const {
	return mMovieType->GetSelection();
}

bool SWFExport::AddEnergyPlot(void) const {
	return mEPlotCheck->IsChecked();
}

#ifdef HAVE_LIBMING
#include "MolDisplayWin.h"

void MolDisplayWin::WriteMovie(wxString & filepath) {
	SWFExport * SWFOptions = new SWFExport(this);

	if(SWFOptions->ShowModal() != wxID_OK) {
		SWFOptions->Destroy();
		return;
	}

	int movieType = SWFOptions->GetMovieChoice();

	if(movieType == 0) {
		CreateFrameMovie(filepath);
	}
	else {
		CreateModeMovie();
	}
}

void MolDisplayWin::CreateFrameMovie(wxString &filePath) {
    long SavedFrameNum = MainData->GetCurrentFrame();
    long i;
    SWFMovie *movie = new SWFMovie();
    SWFInput *in;
    SWFBitmap *bm;
    SWFDisplayItem *di = NULL;
    long width;
    long height;
    long AnimateTime = 10*Prefs->GetAnimateTime();

    if(AnimateTime < 1) AnimateTime = 1;

    getCanvasSize(&width, &height);

    movie->setBackground(0xFF, 0xFF, 0xFF);
    movie->setRate(1000.0/(double)AnimateTime);
    movie->setDimension(width, height);

    for(i = 1; i <= MainData->NumFrames; ++i) {
        MainData->SetCurrentFrame(i);

        Surface *temp = MainData->cFrame->SurfaceList;
        while(temp) {
            temp->RotateEvent(MainData);
            temp = temp->GetNextSurface();
        }
        MainData->ResetRotation();
        UpdateGLModel();
        DrawGL();

        wxImage frame = glCanvas->getImage(0, 0);
        wxMemoryOutputStream *memOutStream = new wxMemoryOutputStream();
        frame.SaveFile(*(wxOutputStream *)memOutStream, (int)wxBITMAP_TYPE_JPEG);
        long datLen = memOutStream->GetSize();
        char unsigned *jpegData = new unsigned char [datLen];
        memOutStream->CopyTo(jpegData, datLen);
        delete memOutStream;
        in = new SWFInput(jpegData, datLen);
        bm = new SWFBitmap(in);
        
        di = movie->add((SWFBlock *)bm);
        movie->nextFrame();
        movie->remove(di);
        //delete jpegData;
    }

    movie->save(filePath.mb_str(wxConvUTF8), 0);
    delete movie;
    MainData->SetCurrentFrame(SavedFrameNum);
}

void MolDisplayWin::CreateModeMovie() {
	bool savedrawmode=false;
    long i;
    SWFMovie *movie;
    SWFInput *in;
    SWFBitmap *bm;
    SWFDisplayItem *di = NULL;
    long width;
    long height;
    long AnimateTime = 10*Prefs->GetAnimateTime();

	if(!MainData->cFrame->Vibs) return;
	Frame * lFrame = MainData->cFrame;
	if (MainData->GetDrawMode()) {
		savedrawmode=true;
		if(!Prefs->GetAnimateMode()) MainData->SetDrawMode(false);
	}

	long cmode = (lFrame->NumAtoms) * (lFrame->Vibs->CurrentMode);
	long AnimationSpeed = Prefs->GetAnimationSpeed();
	float offsetFactor = 1.0/(4.5*AnimationSpeed);
	float VectorScale = Prefs->GetVectorScale();
	CPoint3D *ModeOffset = new CPoint3D[lFrame->NumAtoms];
	CPoint3D *SavedAtoms = new CPoint3D[lFrame->NumAtoms];
	if(!ModeOffset || !SavedAtoms) {
		return; //insufficient memory
	}
	mpAtom *lAtoms = lFrame->Atoms;
	long iatm;
	for(iatm = 0; iatm < (lFrame->NumAtoms); ++iatm) {
		SavedAtoms[iatm] = lAtoms[iatm].Position;
		ModeOffset[iatm] = lFrame->Vibs->NormMode[iatm + cmode];
		ModeOffset[iatm] *= VectorScale;
	}
	MainData->ResetRotation();
	UpdateGLModel();
	DrawGL();

    if(AnimateTime < 1) AnimateTime = 1;

    getCanvasSize(&width, &height);

    movie = new SWFMovie();
    movie->setBackground(0xFF, 0xFF, 0xFF);
    movie->setRate(1000.0/(double)AnimateTime);
    movie->setDimension(width, height);
}
#endif

