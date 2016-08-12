
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
#include <wx/stdpaths.h>
#include "MolDisplayWin.h"
#include "energyplotdialog.h"
#include "Progress.h"

#include <mingpp.h>

void MolDisplayWin::WriteFlashMovie(wxString & filepath) {
	SWFExport * SWFOptions = new SWFExport(this);
	
	if(SWFOptions->ShowModal() != wxID_OK) {
		SWFOptions->Destroy();
		return;
	}
	
	int movieType = SWFOptions->GetMovieChoice();
	bool includeEP = SWFOptions->AddEnergyPlot();
	
	BeginOperation();
	ProgressInd->ChangeText("Creating movie...");
	
	if(movieType == 0) {
		CreateFrameMovie(filepath, includeEP);
	}
	else {
		CreateModeMovie(filepath);
	}
	
	FinishOperation();
}

void MolDisplayWin::CreateFrameMovie(wxString &filePath,
									 const bool includeEP) {
	wxMessageDialog *sizeDlg = NULL;
	wxString title;
	wxString msg;
	long SavedFrameNum = MainData->GetCurrentFrame();
	long i;
	SWFMovie *movie = new SWFMovie();
	SWFInput *in = NULL;
	SWFBitmap *bm = NULL;
	SWFDisplayItem *di = NULL;
	SWFDisplayItem *textDI = NULL;
	SWFDisplayItem *di2 = NULL;
	long width;
	long height;
	long AnimateTime = 10*Prefs->GetAnimateTime();
	bool killEPlotWin = false;
	int savedEPlotWidth, savedEPlotHeight;
	wxStandardPathsBase & gStdPaths = wxStandardPaths::Get();
#if wxCHECK_VERSION(2, 8, 0)
	wxString fontPath = gStdPaths.GetResourcesDir();
#else
	wxString fontPath = gStdPaths.GetDataDir();
#ifdef __WXMAC__
	//wxWidgets has a funny idea of where the resources are stored. It locates them as "SharedSupport"
	//but xcode is putting them in Resources.
	fontPath.Remove(fontPath.Length() - 13);
	fontPath += wxT("Resources");
#endif
#endif
#ifdef __WXMSW__
	fontPath += wxT("\\BsVeraSans.fdb");
#else
	fontPath += wxT("/BsVeraSans.fdb");
#endif
	FILE *fontFile = fopen(fontPath.mb_str(wxConvUTF8), "rb");
	if(fontFile == NULL) {
		delete movie;
		wxLogMessage(_("Unable to locate the SWF font file. Aborted!"));
		wxString fp = _("Looking for : ");
		fp += fontPath;
		wxLogMessage(fp);
		return;
	}
	SWFFont *font = new SWFFont(fontFile);
	SWFText *frameNumText = NULL;
	wxString frameNumStr;
	
	if(AnimateTime < 1) AnimateTime = 1;
	
	getCanvasSize(&width, &height);
	
	if(includeEP) {
		width += height;
		if(!energyPlotWindow) {
			energyPlotWindow = new EnergyPlotDialog(this);
			killEPlotWin = true;
		}
		else {
			energyPlotWindow->GetSize(&savedEPlotWidth, &savedEPlotHeight);
		}
		energyPlotWindow->SetSize(height, height);
		energyPlotWindow->Show(true);
		energyPlotWindow->Update();
	}
	
	movie->setBackground(0xFF, 0xFF, 0xFF);
	movie->setRate(1000.0/(double)AnimateTime);
	movie->setDimension(width, height);
	
	ProgressInd->SetScaleFactor(100.0 / (float)(MainData->NumFrames));
	
	for(i = 1; i <= MainData->NumFrames; ++i) {
		if(!ProgressInd->UpdateProgress((float)i)) {
			goto CLEANUP;
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
		wxMemoryOutputStream *memOutStream = new wxMemoryOutputStream();
		frame.SaveFile(*(wxOutputStream *)memOutStream, wxBITMAP_TYPE_JPEG);
		long datLen = memOutStream->GetSize();
		char unsigned *jpegData = new unsigned char [datLen];
		memOutStream->CopyTo(jpegData, datLen);
		delete memOutStream;
		in = new SWFInput(jpegData, datLen);
		bm = new SWFBitmap(in);
		di = movie->add((SWFBlock *)bm);
		
		if(includeEP) {
			energyPlotWindow->FrameChanged();
			wxBitmap *epBitmap = NULL;
			energyPlotWindow->CopyToBitMap(&epBitmap);
			if(epBitmap != NULL) {
				memOutStream = new wxMemoryOutputStream();
				frame = epBitmap->ConvertToImage();
				frame.SaveFile(*(wxOutputStream *)memOutStream,
							   wxBITMAP_TYPE_JPEG);
				datLen = memOutStream->GetSize();
				char unsigned *epJpegData = new unsigned char [datLen];
				memOutStream->CopyTo(epJpegData, datLen);
				delete memOutStream;
				in = new SWFInput(epJpegData, datLen);
				bm = new SWFBitmap(in);
				di2 = movie->add((SWFBlock *)bm);
				di2->moveTo(width - height, 0);
			}
			delete epBitmap;
		}
		
		frameNumStr = wxString::Format(wxT("%ld/%ld"), i, MainData->NumFrames);
		frameNumText = new SWFText();
		frameNumText->setFont(font);
		frameNumText->setHeight(24);
		frameNumText->setColor(0x00, 0x00, 0x00); /* TODO:  Get from wxMMP config */
		frameNumText->moveTo(0,
							 height - ((24.0 / 1024.0) * font->getAscent() + 3));
		frameNumText->addString(frameNumStr.mb_str(wxConvUTF8));
		textDI = movie->add((SWFBlock *)frameNumText);
		
		movie->nextFrame();
		movie->remove(di);
		movie->remove(textDI);
		if(includeEP) {
			movie->remove(di2);
		}
	}
	
	movie->save(filePath.mb_str(wxConvUTF8), 0);
	MainData->SetCurrentFrame(SavedFrameNum);
	
	/* TODO:  Make sure EpWin is hidden */
	
	msg = wxString::Format(wxT("Optimal viewing dimensions are %ld pixels wide by %ld pixels high."), width, height);
	title = wxString(wxT("Flash Video Info"));
	sizeDlg = new wxMessageDialog(this, msg, title, wxOK);
	sizeDlg->ShowModal();
	delete sizeDlg;
	
CLEANUP:
	delete movie;
	delete font;
	fclose(fontFile);
	
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
}

void MolDisplayWin::CreateModeMovie(wxString &filePath) {
	wxString msg;
	wxString title;
	wxMessageDialog *sizeDlg = NULL;
	bool savedrawmode=false;
	//long i;
	SWFMovie *movie = NULL;
	SWFInput *in = NULL;
	SWFBitmap *bm = NULL;
	SWFDisplayItem *di = NULL;
	SWFDisplayItem *textDI = NULL;
	long width;
	long height;
	long AnimateTime = 10*Prefs->GetAnimateTime();
	wxStandardPathsBase & gStdPaths = wxStandardPaths::Get();
#if wxCHECK_VERSION(2, 8, 0)
	wxString fontPath = gStdPaths.GetResourcesDir();
#else
	wxString fontPath = gStdPaths.GetDataDir();
#ifdef __WXMAC__
	//wxWidgets has a funny idea of where the resources are stored. It locates them as "SharedSupport"
	//but xcode is putting them in Resources.
	fontPath.Remove(fontPath.Length() - 13);
	fontPath += wxT("Resources");
#endif
#endif
#ifdef __WXMSW__
	fontPath += wxT("\\BsVeraSans.fdb");
#else
	fontPath += wxT("/BsVeraSans.fdb");
#endif
	FILE *fontFile = NULL;
	SWFFont *font = NULL;
	SWFText *frameNumText = NULL;
	wxString frameNumStr;
	
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
	float offsetFactor = 1.0/(4.5*AnimationSpeed);
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
	
	fontFile = fopen(fontPath.mb_str(wxConvUTF8), "rb");
	if(fontFile == NULL) {
		wxLogMessage(_("Unable to open font file. Aborted!"));
		/* OR, just disable drawing text */
		return;
	}
	font = new SWFFont(fontFile);
	
	movie = new SWFMovie();
	movie->setBackground(0xFF, 0xFF, 0xFF);
	movie->setRate(1000.0/(double)AnimateTime);
	movie->setDimension(width, height);
	
	long npoint = 0;
	long inc = 1;
	
	ProgressInd->SetScaleFactor(100.0 / (float)(4 * AnimationSpeed));
	
	for(long i = 0; i < (4 * AnimationSpeed); i++) {
		if(!ProgressInd->UpdateProgress((float)i)) {
			goto CLEANUP;
		}
		if((npoint == AnimationSpeed) || (npoint == -AnimationSpeed)) {
			inc *= -1;
			offsetFactor *= -1.0;
		}
		npoint += inc;
		
		wxImage mImage = glCanvas->getImage(0, 0);
		wxMemoryOutputStream *memOutStream = new wxMemoryOutputStream();
		mImage.SaveFile(*(wxOutputStream *)memOutStream, wxBITMAP_TYPE_JPEG);
		long datLen = memOutStream->GetSize();
		char unsigned *jpegData = new unsigned char [datLen];
		memOutStream->CopyTo(jpegData, datLen);
		delete memOutStream;
		in = new SWFInput(jpegData, datLen);
		bm = new SWFBitmap(in);
		
		frameNumStr = wxString::Format(wxT("%d/%d"), i+1, (4 * AnimationSpeed));
		frameNumText = new SWFText();
		frameNumText->setFont(font);
		frameNumText->setHeight(24);
		frameNumText->setColor(0x00, 0x00, 0x00); /* TODO:  Get from wxMMP config */
		frameNumText->moveTo(0,
							 height - ((24.0 / 1024.0) * font->getAscent() + 3));
		frameNumText->addString(frameNumStr.mb_str(wxConvUTF8));
		
		di = movie->add((SWFBlock *)bm);
		textDI = movie->add((SWFBlock *)frameNumText);
		
		movie->nextFrame();
		movie->remove(di);
		movie->remove(textDI);
		
		for (iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
			lAtoms[iatm].Position.x += offsetFactor * (ModeOffset[iatm].x);
			lAtoms[iatm].Position.y += offsetFactor * (ModeOffset[iatm].y);
			lAtoms[iatm].Position.z += offsetFactor * (ModeOffset[iatm].z);
		}
		
		MainData->ResetRotation();
		ReleaseLists();
		DrawGL();
	}
	
	movie->save(filePath.mb_str(wxConvUTF8), 0);
	
	for (iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
		lAtoms[iatm].Position = SavedAtoms[iatm];
	}
	MainData->ResetRotation();
	
	MainData->SetDrawMode(savedrawmode);
	ReleaseLists();
	DrawGL();
	
	msg = wxString::Format(wxT("Optimal viewing dimensions are %ld pixels wide by %ld pixels high."), width, height);
	title = wxString(wxT("Flash Video Info"));
	sizeDlg = new wxMessageDialog(this, msg, title, wxOK);
	sizeDlg->ShowModal();
	delete sizeDlg;
	
CLEANUP:
	delete movie;
	delete font;
	fclose(fontFile);
	if (ModeOffset) delete [] ModeOffset;
	if (SavedAtoms) delete [] SavedAtoms;
}
#endif
