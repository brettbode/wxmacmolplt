/////////////////////////////////////////////////////////////////////////////
// Name:        orbital3d.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri  2 Jun 10:08:18 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "surfaceDlg.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "Globals.h"
#include "SurfaceTypes.h"
#include "surfaceDlg.h"
#include "colorArea.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "BasisSet.h"
#include "Prefs.h"
#include "Progress.h"
#include "MolDisplayWin.h"
#include "surfaceswindow.h"
#include "setscreenplane.h"

#include <wx/config.h>

////@begin XPM images
////@end XPM images

using namespace std;
#include <iostream>

#define CONTOUR_WIDTH 200

#if 0
IMPLEMENT_CLASS(BaseSurfacePane, wxPanel)
IMPLEMENT_CLASS(Surface2DPane, BaseSurfacePane)
IMPLEMENT_CLASS(Surface3DPane, BaseSurfacePane)
IMPLEMENT_CLASS(Orbital2DSurfPane, wxPanel)
IMPLEMENT_CLASS(Orbital3DSurfPane, wxPanel)
IMPLEMENT_CLASS(General3DSurfPane, wxPanel)
IMPLEMENT_CLASS(General2DSurfPane, wxPanel)
IMPLEMENT_CLASS(TEDensity2DSurfPane, wxPanel)
IMPLEMENT_CLASS(TEDensity3DSurfPane, wxPanel)
IMPLEMENT_CLASS(MEP2DSurfPane, wxPanel)
IMPLEMENT_CLASS(MEP3DSurfPane, wxPanel)

IMPLEMENT_CLASS(Surface2DParamDlg, wxDialog)
IMPLEMENT_CLASS(Surface3DParamDlg, wxDialog)
IMPLEMENT_DYNAMIC_CLASS(FormattedListBox, wxHtmlListBox)
#endif

BEGIN_EVENT_TABLE(FormattedListBox, wxHtmlListBox)
	EVT_SIZE(FormattedListBox::OnResize)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(BaseSurfacePane, wxPanel)
	EVT_CHECKBOX(ID_ALLFRAMECHECK, BaseSurfacePane::OnCheckAllFrames)
	EVT_TEXT(ID_SURFTITLE, BaseSurfacePane::OnChangeTitle)
	EVT_CHECKBOX(ID_VISIBLECHECK, BaseSurfacePane::OnCheckVisible)
	EVT_SLIDER(ID_GRID_POINT_SLIDER, BaseSurfacePane::OnGridPointSld)
	EVT_BUTTON(ID_SURFACE_EXPORT_BUT, BaseSurfacePane::OnExport)
	EVT_CHOICE(ID_ORB_CHOICE, BaseSurfacePane::OnOrbSetChoice)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(Surface3DPane, BaseSurfacePane)
	EVT_RADIOBOX(ID_3D_RADIOBOX, Surface3DPane::On3DRadioBox)
	EVT_CHECKBOX(ID_SMOOTH_CHECKBOX, Surface3DPane::OnSmoothCheck)
	EVT_TEXT_ENTER(ID_CONTOUR_VALUE_SLIDER, Surface3DPane::OnContourValueEnter)
	EVT_BUTTON(ID_SET_PARAM_BUT, Surface3DPane::OnSetParam)
	EVT_BUTTON(ID_FREE_MEM_BUT, Surface3DPane::OnFreeMem)
	EVT_COMMAND_ENTER(ID_3D_COLOR_POSITIVE, Surface3DPane::OnPosColorChange)
	EVT_COMMAND_ENTER(ID_3D_COLOR_NEGATIVE, Surface3DPane::OnNegColorChange)
	EVT_SPINCTRL(ID_TRANSPARENCY, Surface3DPane::OnTransparencyChange)
	EVT_IDLE(Surface3DPane::OnIdle)
	EVT_SLIDER(ID_GRID_SIZE_SLIDER, Surface3DPane::OnGridSizeSld)
	EVT_SLIDER(ID_CONTOUR_VALUE_SLIDER, Surface3DPane::OnContourValueSld)
	EVT_TEXT_ENTER(ID_CONTOUR_VALUE_EDIT, Surface3DPane::OnContourValueEnter)
	EVT_CHECKBOX(ID_3D_COLOR_SURF_CHECK, Surface3DPane::OnUseMEPCheck)
	EVT_CHECKBOX(ID_USERGB_COLOR_CHECK, Surface3DPane::OnRGBColorCheck)
	EVT_CHECKBOX(ID_INVERT_RGB_CHECK, Surface3DPane::OnInvertRGBCheck)
	EVT_TEXT(ID_3D_MAX_MAP_EDIT, Surface3DPane::OnMaxMEPValueText)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(Surface1DPane, BaseSurfacePane)
	EVT_COMMAND_ENTER(ID_2D_COLOR_POSITIVE, Surface1DPane::OnPosColorChange)
	EVT_COMMAND_ENTER(ID_2D_COLOR_NEGATIVE, Surface1DPane::OnNegColorChange)
	EVT_TEXT(ID_SCALE_EDIT, Surface1DPane::OnChangeScale)
	EVT_TEXT(ID_3D_MAX_MAP_EDIT, Surface1DPane::OnChangeClamp)
	EVT_TEXT/*_ENTER*/(ID_ENDPT1_XBOX, Surface1DPane::OnChangeEndpoint)
	EVT_TEXT/*_ENTER*/(ID_ENDPT1_YBOX, Surface1DPane::OnChangeEndpoint)
	EVT_TEXT/*_ENTER*/(ID_ENDPT1_ZBOX, Surface1DPane::OnChangeEndpoint)
	EVT_TEXT/*_ENTER*/(ID_ENDPT2_XBOX, Surface1DPane::OnChangeEndpoint)
	EVT_TEXT/*_ENTER*/(ID_ENDPT2_YBOX, Surface1DPane::OnChangeEndpoint)
	EVT_TEXT/*_ENTER*/(ID_ENDPT2_ZBOX, Surface1DPane::OnChangeEndpoint)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(Surface2DPane, BaseSurfacePane)
	EVT_CHECKBOX(ID_USE_PLANE_CHECKBOX, Surface2DPane::OnUsePlaneChk)
	EVT_CHECKBOX(ID_SHOW_ZERO_CHECKBOX, Surface2DPane::OnShowZeroChk)
	EVT_CHECKBOX(ID_DASH_CHECKBOX, Surface2DPane::OnDashChk)
	EVT_BUTTON(ID_SET_PARAM_BUT, Surface2DPane::OnSetParam)
	EVT_BUTTON(ID_SET_PLANE_BUT, Surface2DPane::OnSetPlane)
	EVT_COMMAND_ENTER(ID_2D_COLOR_POSITIVE, Surface2DPane::OnPosColorChange)
	EVT_COMMAND_ENTER(ID_2D_COLOR_NEGATIVE, Surface2DPane::OnNegColorChange)
	EVT_CHECKBOX(ID_DISPLAY_PLANE_CHECKBOX, Surface2DPane::OnDisplayPlaneChk)
	EVT_TEXT(ID_CONTOUR_VALUE_EDIT, Surface2DPane::OnContourValueText)
	EVT_TEXT(ID_NUM_CONTOUR_TEXT, Surface2DPane::OnNumContoursText)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(Orbital2DSurfPane, Surface2DPane)
	EVT_CHOICE(ID_ORB_FORMAT_CHOICE,  Orbital2DSurfPane::OnOrbFormatChoice)
	EVT_CHOICE(ID_ORB_CHOICE, Orbital2DSurfPane::OnOrbSetChoice)
	EVT_LISTBOX(ID_ATOM_LIST, Orbital2DSurfPane::OnMOList)
	EVT_LISTBOX(ID_ORB_COEF, Orbital2DSurfPane::OnCoefList)
	EVT_CHECKBOX(ID_REVERSE_PHASE_CHECKBOX, Orbital2DSurfPane::OnReversePhase)
	EVT_BUTTON(ID_SURFACE_UPDATE_BUT, Orbital2DSurfPane::OnUpdate)
	EVT_CHECKBOX(ID_SPH_HARMONICS_CHECKBOX, Orbital2DSurfPane::OnSphHarmonicChk)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(Orbital3DSurfPane, Surface3DPane)
	EVT_CHOICE(ID_ORB_FORMAT_CHOICE,  Orbital3DSurfPane::OnOrbFormatChoice)
	EVT_CHOICE(ID_ORB_CHOICE, Orbital3DSurfPane::OnOrbSetChoice)
	EVT_LISTBOX(ID_ATOM_LIST, Orbital3DSurfPane::OnMOList)
	EVT_LISTBOX(ID_ORB_COEF, Orbital3DSurfPane::OnCoefList)
	EVT_CHECKBOX(ID_SPH_HARMONICS_CHECKBOX, Orbital3DSurfPane::OnSphHarmonicChk)
	EVT_CHECKBOX(ID_REVERSE_PHASE_CHECKBOX, Orbital3DSurfPane::OnReversePhase)
	EVT_SLIDER(ID_CONTOUR_VALUE_SLIDER, Orbital3DSurfPane::OnContourValueSld)
	EVT_SLIDER(ID_GRID_SIZE_SLIDER, Orbital3DSurfPane::OnGridSizeSld)
	EVT_BUTTON(ID_SURFACE_UPDATE_BUT, Orbital3DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(General3DSurfPane, Surface3DPane)
	EVT_BUTTON(ID_GENFILEBUTTON, General3DSurfPane::OnFileButton)
	EVT_CHECKBOX(ID_GENMULTCHECK, General3DSurfPane::OnMultCheck)
	EVT_CHECKBOX(ID_GENSQUARECHECK, General3DSurfPane::OnSquareCheck)
	EVT_CHECKBOX(ID_GENCONPOSNEGCHECK, General3DSurfPane::OnContourPosNegCheck)
	EVT_SLIDER(ID_CONTOUR_VALUE_SLIDER, General3DSurfPane::OnContourValueSld)
	EVT_TEXT(ID_GENMULTEDIT, General3DSurfPane::OnMultValueEdit)
	EVT_BUTTON(ID_SURFACE_UPDATE_BUT, General3DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(General2DSurfPane, Surface2DPane)
	EVT_BUTTON(ID_GENFILEBUTTON, General2DSurfPane::OnFileButton)
	EVT_CHECKBOX(ID_GENMULTCHECK, General2DSurfPane::OnMultCheck)
	EVT_CHECKBOX(ID_GENSQUARECHECK, General2DSurfPane::OnSquareCheck)
	EVT_TEXT(ID_GENMULTEDIT, General2DSurfPane::OnMultValueEnter)
	EVT_BUTTON(ID_SURFACE_UPDATE_BUT, General2DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(TEDensity1DSurfPane, Surface1DPane)
	EVT_BUTTON(ID_SURFACE_UPDATE_BUT, TEDensity1DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(TEDensity2DSurfPane, Surface2DPane)
	EVT_BUTTON(ID_SURFACE_UPDATE_BUT, TEDensity2DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(TEDensity3DSurfPane, Surface3DPane)
	EVT_BUTTON(ID_SURFACE_UPDATE_BUT, TEDensity3DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MEP2DSurfPane, Surface2DPane)
	EVT_BUTTON(ID_SURFACE_UPDATE_BUT, MEP2DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MEP3DSurfPane, Surface3DPane)
	EVT_BUTTON(ID_SURFACE_UPDATE_BUT, MEP3DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(Surface2DParamDlg, wxDialog)
  EVT_BUTTON(wxID_OK, Surface2DParamDlg::OnClose)
  EVT_BUTTON(ID_COPY_ALL, Surface2DParamDlg::OnCopyAll)
  EVT_BUTTON(ID_PASTE_ALL, Surface2DParamDlg::OnPasteAll)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(Surface3DParamDlg, wxDialog)
  EVT_BUTTON(wxID_OK, Surface3DParamDlg::OnClose)
  EVT_BUTTON(ID_COPY_ALL, Surface3DParamDlg::OnCopyAll)
  EVT_BUTTON(ID_PASTE_ALL, Surface3DParamDlg::OnPasteAll)
END_EVENT_TABLE()
/*!
 * Base class of any Panel
 */

BaseSurfacePane::BaseSurfacePane(wxWindow* parent, Surface* target, SurfacesWindow* p, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) {
	mTarget = target;
	owner = p;
	//mData = data;

	Visible = target->GetVisibility();
	AllFrames = (target->GetSurfaceID() != 0);
	UpdateTest = false;

	Create(parent, id, pos, size, style);
}

BaseSurfacePane::~BaseSurfacePane() {

#if 0
	// This should be fixed by wxWidgets r53773.

	// The default button apparently needs to be manually unset.  Otherwise
	// calling wxButton::SetDefault later will try to operate on the previous
	// default and now deleted button.
    wxTopLevelWindow * const
        tlw = wxDynamicCast(wxGetTopLevelParent(this), wxTopLevelWindow);

	if (tlw) {
		tlw->SetDefaultItem(NULL);
	}
#endif

}

bool BaseSurfacePane::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) {
	wxPanel::Create(parent, id, pos, size, style);

	CreateControls();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();

	return true;
}

/*
 * In base classes we just create objects
 * leave the adding operation to the final classes
 */

void BaseSurfacePane::CreateControls() {

	mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *box_sizer = new wxBoxSizer(wxHORIZONTAL);

	box_sizer->Add(new wxStaticText(this, wxID_ANY, _T("Name:")), 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxALIGN_RIGHT, 5);

	wxString tmp_str(GetTargetSurface()->GetLabel(), wxConvUTF8);
	surfTitleEdit = new wxTextCtrl(this, ID_SURFTITLE, tmp_str, wxDefaultPosition, wxSize(250, -1), 0);
	if (SurfacesWindow::ShowToolTips())
		surfTitleEdit->SetToolTip(_("You may change the label to anything you like."));
	box_sizer->Add(surfTitleEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	visibleCheck = new wxCheckBox(this, ID_VISIBLECHECK, _("Visible"), wxDefaultPosition, wxDefaultSize, 0);
	visibleCheck->SetValue(Visible);
	if (SurfacesWindow::ShowToolTips())
		visibleCheck->SetToolTip(_("Uncheck to hide the surface without deleting it."));
	box_sizer->Add(visibleCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	allFrameCheck = new wxCheckBox(this, ID_ALLFRAMECHECK, _("All Frames"), wxDefaultPosition, wxDefaultSize, 0);
	allFrameCheck->SetValue(AllFrames);
	if (SurfacesWindow::ShowToolTips())
		allFrameCheck->SetToolTip(_("Check to apply this surface to all frames."));
	box_sizer->Add(allFrameCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mainSizer->Add(box_sizer, 0, wxALIGN_CENTER);

	SetSizer(mainSizer);
}

void BaseSurfacePane::OnCheckAllFrames(wxCommandEvent& event) {

	setAllFrames(event.GetSelection());

}

void BaseSurfacePane::OnCheckVisible(wxCommandEvent& event) {

	SetVisibility(event.GetSelection());

}

void BaseSurfacePane::OnChangeTitle(wxCommandEvent& event) {

	wxString newLabel(event.GetString());
	wxString test(mTarget->GetLabel(), wxConvUTF8);
	if (newLabel.IsEmpty()) {
		mTarget->SetLabel(NULL);
		newLabel = wxString(mTarget->GetLabel(), wxConvUTF8);
	} else if (newLabel.Cmp(test) != 0) {
		mTarget->SetLabel((const char *)newLabel.mb_str(wxConvUTF8));
	}

	event.Skip();

}

const wxString BaseSurfacePane::GetSurfaceName() const {

	return surfTitleEdit->GetValue();

}

void BaseSurfacePane::PageIsNowActive(void) {
	if (mUpdateBut) mUpdateBut->SetDefault();
}

void BaseSurfacePane::OnGridPointSld(wxCommandEvent &event) {
	NumGridPoints = mNumGridPntSld->GetValue();
	SwitchFixGrid = true;

	setUpdateButton();
}

void BaseSurfacePane::OnExport(wxCommandEvent &event) {
	FILE *currFile = NULL;
	BufferFile *buffer = NULL;
	wxString dropFilter;
	wxFileName fileName;
	wxString filePath;
	// get filename from filePath 
	if(mTarget->GetDimensions() == 3)
		dropFilter = wxT("text file (*.txt)|*.txt|CCP4 file (*.CCP4)|*.ccp4|CNS electron density map (*.CNS)|*.cns");
	else
		dropFilter = wxT("text file (*.txt)|*.txt");
	
	wxFileDialog wfd(owner, wxT("Export As"), wxT(""), wxT(""), dropFilter,
#if wxCHECK_VERSION(2,9,0)
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT
#else
			wxSAVE | wxOVERWRITE_PROMPT
#endif 
);

	if (wfd.ShowModal()==wxID_OK) {
		filePath = wfd.GetPath();
	}

	// Generalize out
	if (!filePath.IsEmpty()) {
		if((currFile = fopen(filePath.mb_str(wxConvUTF8), "wb")) == NULL) {
			MessageAlert("Unable to open the file for output.");
			return;
		}
		try {
			buffer = new BufferFile(currFile, true);
			int filterIndex = 0;
				//Looks like under wx 2.9 cocoa if there is only one choice the index doesn't work.
			if(mTarget->GetDimensions() == 3) filterIndex = wfd.GetFilterIndex();
			switch (filterIndex) {
				case 0:
					// our txt file
					mTarget->Export(buffer, Surface::TXTFILE);
				break;
				case 1:
					// ccp4 file
					mTarget->Export(buffer, Surface::CCP4FILE);
				break;
				case 2:
					// cns file
					mTarget->Export(buffer, Surface::CNSFILE);
				break;
			}
		}
		catch (...) {
			MessageAlert("Error attempting to write file.");
		}
		if(buffer) {
			delete buffer;
		}
		fclose(currFile);
	}
}

//Call to change the visibilty for the active surface
void BaseSurfacePane::SetVisibility(bool state) {
	Visible = state;

	visibleCheck->SetValue((int) state);
	setUpdateButton();
}

void BaseSurfacePane::setAllFrames(bool state) {
	AllFrames = state;

	allFrameCheck->SetValue((int) state);
	setUpdateButton();
}

void BaseSurfacePane::SetUpdateTest(bool test) {
	UpdateTest = test;

	if (test)
		mUpdateBut->Enable();
	else
		mUpdateBut->Disable();

}

void BaseSurfacePane::setUpdateButton() {
	if (UpdateNeeded())
		mUpdateBut->Enable();
	else
		mUpdateBut->Disable();
}

OrbSurfacePane::OrbSurfacePane(OrbSurfBase* target, SurfacesWindow* o) {
	mTarget = target;
	myowner = o;

	//initialize parameters
	TargetSet = mTarget->GetTargetSet();
	OrbOptions = mTarget->GetOptions();
	OrbColumnEnergyOrOccupation = mTarget->GetOrbOccDisplay();
	PlotOrb = target->GetTargetOrb();
	SphericalHarmonics = target->UseSphericalHarmonics();
	PhaseChange = target->GetPhaseChange();

	if ((TargetSet<0)&&(!(OrbOptions&1))) {	//default to something sensible
		MoleculeData * mData = myowner->GetMoleculeData();
		Frame * lFrame = mData->GetCurrentFramePtr();
		const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();

		if (Orbs->size() > 0) {
			TargetSet = 0;	//just default to the first set...
		} else
			OrbOptions = 1;	//No MO's so default to AO's
	}

	coefIsEnabled = false;
	if (OrbOptions&1) coefIsEnabled = true;
}

OrbSurfacePane::~OrbSurfacePane() {

}

bool OrbSurfacePane::UpdateEvent() {
	//user clicked update button, so push the data to the surface

	bool updateGrid = false;
	if (PlotOrb != mTarget->GetTargetOrb()) {
		mTarget->SetTargetOrb(PlotOrb);
		updateGrid = true;
	}
	if (TargetSet != mTarget->GetTargetSet()) {
		mTarget->SetTargetSet(TargetSet);
		updateGrid = true;
	}
	if (OrbOptions != mTarget->GetOptions()) {
		mTarget->SetOptions(OrbOptions);
		updateGrid = true;
	}
	if (OrbColumnEnergyOrOccupation != mTarget->GetOrbOccDisplay()) {
		//This is just a display flag for the MO column so no
		//grid or contour updates are needed.
		mTarget->SetOrbOccDisplay(OrbColumnEnergyOrOccupation);
	}
	if (SphericalHarmonics != mTarget->UseSphericalHarmonics()) {
		mTarget->UseSphericalHarmonics(SphericalHarmonics);
		updateGrid = true;
	}

	return updateGrid;
}

int OrbSurfacePane::getOrbSetForOrbPane(vector<wxString>& choice) {
	MoleculeData * mData = myowner->GetMoleculeData();
	Frame * lFrame = mData->GetCurrentFramePtr();
	const vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();

	short item, numitems;

	choice.push_back(_T("Atomic Orbitals"));
	item = 1; numitems = 1;

	if (Orbs->size() > 0) {
		std::vector<OrbitalRec *>::const_iterator OrbSet = Orbs->begin();
		long	OrbSetCount = 0;
		while (OrbSet != Orbs->end()) {
			if (TargetSet == OrbSetCount) item = numitems + 1;

			if (((*OrbSet)->getOrbitalWavefunctionType() == UHF)&&
					(!((*OrbSet)->getOrbitalType() == NaturalOrbital))) {
				if ((*OrbSet)->getOrbitalType() == LocalizedOrbital) {
					choice.push_back(_T("Alpha Spin Localized Orbitals"));
					choice.push_back(_T("Beta Spin Localized Orbitals"));
				} 
				else if ((*OrbSet)->getOrbitalType() == LocalizedOrbital) {
					choice.push_back(_T("Alpha Spin Oriented Localized Orbitals"));
					choice.push_back(_T("Beta Spin Oriented Localized Orbitals"));
				} 
				else if ((*OrbSet)->getOrbitalType() == GuessOrbital) {
					choice.push_back(_T("Alpha Spin Initial Guess Orbitals"));
					choice.push_back(_T("Beta Spin Initial Guess Orbitals"));
				} 
				else {
					choice.push_back(_T("Alpha Spin Orbitals"));
					choice.push_back(_T("Beta Spin Orbitals"));
				}
				numitems++;	//increment count for two sets, other increment is done below

				if ((TargetSet == OrbSetCount)&&(OrbOptions & 16)) 
					item = numitems+1;	//target beta set
			} 
			else {
				choice.push_back(wxString((*OrbSet)->getOrbitalTypeText(), wxConvUTF8));
			}
			numitems++;
			OrbSetCount++;
			OrbSet++;
		}
	}
	if (OrbOptions&1) item = 1;	//AOs

	return item;
}

void OrbSurfacePane::makeMOList() {
	MoleculeData * mData = myowner->GetMoleculeData();
	Frame * lFrame = mData->GetCurrentFramePtr();
	const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();

	mMOList->Clear();

	wxString tmpStr;

		//validate Frame and orbitals and skip if AOs are chosen
	if (lFrame && (Orbs->size() > 0) && !(OrbOptions&1)) {
		OrbitalRec * lMOs = NULL;
		//defaultOrb will be a bit sloppy and set more often to position the list in the active space
		//HOMOOrb will be set when there is a clear Orb (RHF, UHF,...)
		long defaultOrb = -1, HOMOOrb = -1;

		if ((TargetSet < Orbs->size())&&(TargetSet >= 0)) 
			lMOs = (*Orbs)[TargetSet];

		if (lMOs) {
			bool Alpha = !(OrbOptions & 16);
			long	NumMOs;
			char * SymLabel;
			float * Energy;
			float * OccNum;

			if (Alpha) {
				NumMOs=lMOs->NumAlphaOrbs;
				SymLabel = lMOs->SymType;
				Energy = lMOs->Energy;
				OccNum = lMOs->OrbOccupation;
				defaultOrb = lMOs->getNumOccupiedAlphaOrbitals()-1;
			} else {
				NumMOs=lMOs->NumBetaOrbs;
				SymLabel = lMOs->SymTypeB;
				Energy = lMOs->EnergyB;
				OccNum = lMOs->OrbOccupationB;
				defaultOrb = lMOs->getNumOccupiedBetaOrbitals()-1;
			}

			char* oneSymLabel;
			if ((lMOs->getOrbitalWavefunctionType() == RHF)|| (lMOs->getOrbitalWavefunctionType() == UHF)||
				(lMOs->getOrbitalWavefunctionType() == ROHF)) HOMOOrb = defaultOrb;
			if (OccNum) {
				long i=0;
				while ((OccNum[i] > 0.85)&&(i<NumMOs)) {
					defaultOrb = i;
					i++;
				}
			}

			for (int theCell = 0; theCell < NumMOs; theCell++) {
				if (theCell != HOMOOrb)
					tmpStr.Printf(wxT("<table width=\"100%%\" cellspacing=\"1\" cellpadding=\"0\" border=\"0\"><tr><td width=\"20%%\" align=\"right\">%d&nbsp;</td>"), theCell+1);
				else
					tmpStr.Printf(wxT("<table style=\"background:#80BFFF\" width=\"100%%\" cellspacing=\"1\" cellpadding=\"0\" border=\"0\"><tr><td width=\"20%%\" align=\"right\">%d&nbsp;</td>"), theCell+1);

				tmpStr.Append(wxT("<td width=\"40%%\">"));
				if (SymLabel) {	//Add the symetry of the orb, if known

					oneSymLabel = &(SymLabel[theCell*5]);	
					//offset to the label for this orb

					for (int ichar=0; ichar<4; ichar++) {
						if ((oneSymLabel[ichar])=='\0') break;

						if ((oneSymLabel[ichar]>='0')&&(oneSymLabel[ichar]<='9')) {
							tmpStr.Append(_T("<sub>"));
							tmpStr.Append(oneSymLabel[ichar]);
							tmpStr.Append(_T("</sub>"));
						} else if ((oneSymLabel[ichar]=='U')||(oneSymLabel[ichar]=='u')) {
							tmpStr.Append(_T("<sub>u</sub>"));
						} else if ((oneSymLabel[ichar]=='G')||(oneSymLabel[ichar]=='g')) {
							tmpStr.Append(_T("<sub>g</sub>"));
						} else {
							if (oneSymLabel[ichar] == '\'' && ichar < 3 &&
								oneSymLabel[ichar + 1] == '\'') {
								tmpStr.Append(_T("<sup>\"</sup>"));
								++ichar;
							} else if (oneSymLabel[ichar] == '\'') {
								tmpStr.Append(_T("<sup>"));
								tmpStr.Append(oneSymLabel[ichar]);
								tmpStr.Append(_T("</sup>"));
							} else {
								tmpStr.Append(oneSymLabel[ichar]);
							}
						}
					}
				}
				tmpStr.Append(wxT("&nbsp;</td>"));

				wxString temp;
				tmpStr.Append(wxT("<td width=\"40%%\" align=\"right\">"));
				if (OrbColumnEnergyOrOccupation) {	//orb occupation selected

					if (OccNum)
						temp.Printf(wxT("%.3f"), OccNum[theCell]);
					else {	
						//attempt to set the occupation based on the wavefunction type
						if (lMOs->getOrbitalWavefunctionType() == RHF) {
							if (theCell<lMOs->getNumOccupiedAlphaOrbitals()) 
								temp.Printf(wxT("2"));
							else 
								temp.Printf(wxT("0"));
						} else if (lMOs->getOrbitalWavefunctionType() == ROHF) {
							temp.Printf(wxT("0"));
							if (theCell<lMOs->getNumOccupiedBetaOrbitals()) 
								temp.Printf(wxT("2")); // TODO Append?
							else if (theCell<lMOs->getNumOccupiedAlphaOrbitals()) 
								temp.Printf(wxT("1"));
						} else if (lMOs->getOrbitalWavefunctionType() == UHF) {
							temp.Printf(wxT("0"));
							if (Alpha) {
								if (theCell<lMOs->getNumOccupiedAlphaOrbitals()) 
									temp.Printf(wxT("1"));
							} else if (theCell<lMOs->getNumOccupiedBetaOrbitals()) 
								temp.Printf(wxT("1"));
						} else {	//MCSCF or CI occupations can't be guessed
							temp.Printf(wxT("??"));
						}
					}
				} else if (Energy) {	//punch out the orb energy
					temp.Printf(wxT("%.3f"), Energy[theCell]);
				}

				tmpStr.Append(temp);
				tmpStr.Append(wxT("&nbsp;</td></tr></table>"));
				mMOList->Append(tmpStr);
			}
		}
		mMOList->DoneAppending();
		if (defaultOrb >= 0) mMOList->SetSelection(defaultOrb);
	}
}

void OrbSurfacePane::makeAOList() {

	wxString aChoice;
	int percents[4];
	int i;

	MoleculeData * mData = myowner->GetMoleculeData();
	BasisSet * BasisPtr = mData->GetBasisSet();

	mOrbCoef->Clear();

	if (BasisPtr) {
		long NumBasisFuncs = BasisPtr->GetNumBasisFuncs(SphericalHarmonics);

		for (int theCell = 0; theCell < NumBasisFuncs; theCell++) {
			long ifunc = 0, iatom=0;
			Frame * lFrame = mData->GetCurrentFramePtr();

			if (PlotOrb >= 0) {
				percents[0] = 10;
				percents[1] = 15;
				percents[2] = 25;
				percents[3] = 50;
			} else {
				percents[0] = 30;
				percents[1] = 30;
				percents[2] = 40;
				percents[3] = 0;
			}

			while (ifunc<=theCell) {
				long minshell = BasisPtr->BasisMap[2*iatom];
				long maxshell = BasisPtr->BasisMap[2*iatom+1];

				for (long ishell=minshell;ishell<=maxshell;ishell++) {
					long jfunc = ifunc+(BasisPtr->Shells[ishell]).GetNumFuncs(SphericalHarmonics);
					if (theCell<jfunc) { //Found the right shell, now pick out the right function

						char label[63];
						wxString tmpStr;
						aChoice.Printf(wxT("<table width=\"100%%\" cellspacing=\"1\" cellpadding=\"0\" border=\"0\"><tr>")); 
						int nchar = 0;
						//punch out the atom # and symbol if this is the 1st function for this atom

						if ((ishell==minshell)&&(theCell==ifunc)) {
							sprintf(label, "%ld", iatom+1);
							tmpStr.Printf(wxT("<td width=\"%d%%\">%ld&nbsp;</td>"), percents[0], iatom + 1);
							aChoice.Append(tmpStr);

							tmpStr.Printf(_T("<td width=\"%d%%\">"), percents[1]);
							aChoice.Append(tmpStr);
							WinPrefs * mPrefs = myowner->GetPrefs();
							if (mPrefs) {
								long AtomType = lFrame->GetAtomType(iatom)-1;
								mPrefs->GetAtomLabel(AtomType, tmpStr);
								tmpStr.Trim();
								aChoice.Append(tmpStr);
							}
							aChoice.Append(_T("</td>"));
						} else {
							tmpStr.Printf(_T("<td width=\"%d%%\">&nbsp;</td><td width=\"%d%%\">&nbsp;</td>"),
										  percents[0], percents[1]);
							aChoice.Append(tmpStr);
						}

						jfunc = theCell-ifunc;
						BasisPtr->Shells[ishell].GetLabel(label, jfunc, SphericalHarmonics);
						nchar = strlen(label);

						tmpStr.Printf(_T("<td width=\"%d%%\">"), percents[2]);
						aChoice.Append(tmpStr);

						wxString ltemp(label, wxConvUTF8);
						aChoice.Append(ltemp[0]);
						aChoice.Append(wxT("<sub>"));
						wxString sub = ltemp.Mid(1);

						for (i = 0; i < sub.length(); ++i) {
							if (isdigit(sub[i]) && i != 0 && (isalpha(sub[i - 1]) || isdigit(sub[i - 1]))) {
								aChoice.Append(_T("<sup><font size=\"-1\">"));
								aChoice.Append(sub[i]);
								aChoice.Append(_T("</font></sup>"));
							} else {
								aChoice.Append(sub[i]);
							}
						}

						aChoice.Append(wxT("</sub></td>"));

						/* for (int i = 0; i < 6-nchar; i++) */
							/* aChoice.Append(' '); */

						/* if (nchar <= 2) */
							/* aChoice.Append(' '); //compensate one space */

						ifunc = theCell+1;
						ishell=maxshell;
					} else {
						ifunc = jfunc;
					}
				}
				iatom++;
			}

			if (PlotOrb>=0) { //Is an MO selected?
				aChoice.Append(wxT("<td width=\"40%%\" align=\"right\">"));
				if (!(OrbOptions&1)) { //If not displaying AO's
					const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();
					if (Orbs->size() > 0) {
						OrbitalRec *MOs = NULL;
						if ((TargetSet < Orbs->size())&&(TargetSet >= 0)) 
							MOs = (*Orbs)[TargetSet];

						if (MOs) {
							float * aVector=NULL;

							if (OrbOptions & 16) {
								if (PlotOrb<MOs->NumBetaOrbs)
									aVector = &(MOs->VectorsB[NumBasisFuncs*PlotOrb]);
							}
							else {
								if (PlotOrb<MOs->NumAlphaOrbs)
									aVector = &(MOs->Vectors[NumBasisFuncs*PlotOrb]);
							}

							if (aVector) {
								wxString label;

								label.Printf(wxT("%.3f"), aVector[theCell]);//prepare the coef for printing
								aChoice.Append(label);
							}
						}
					}
				}
				aChoice.Append(wxT("&nbsp;</td>"));
			}
			aChoice.Append(wxT("</tr></table>"));
			mOrbCoef->Append(aChoice);
			/* choice.push_back(aChoice); */
		}
		mOrbCoef->DoneAppending();
	}
}

void OrbSurfacePane::setFlagOnOrbFormatChange(int itemtype) {
	if (OrbColumnEnergyOrOccupation) OrbColumnEnergyOrOccupation = false;
	if (itemtype) OrbColumnEnergyOrOccupation = true;
}

int OrbSurfacePane::orbSetChangeEvt(int item, SurfacesWindow * owner) {
	int itemtype=0;

	if (item <= 1) {
		if (!(OrbOptions&1)) {
			OrbOptions = 1; itemtype=1;
		}
	}

	else {
		short numitems=1;
		OrbOptions = 0;	//turn off AOs and alpha/beta flags
		MoleculeData * mData = owner->GetMoleculeData();
		Frame *lFrame = mData->GetCurrentFramePtr();

		const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();

		if (Orbs->size() > 0) {
			vector<OrbitalRec *>::const_iterator OrbSet = Orbs->begin();
			long OrbSetCount = 0;

			while (OrbSet != Orbs->end()) {
				numitems++;
				if (numitems == item) {
					TargetSet = OrbSetCount;
					itemtype = 1;
					break;
				}

				if (((*OrbSet)->getOrbitalWavefunctionType() == UHF)&&
						(!((*OrbSet)->getOrbitalType() == NaturalOrbital))) {
					numitems++; //Extra increment for the beta set
					if (numitems == item) {
						TargetSet = OrbSetCount;
						OrbOptions = 16; //beta set selected
						itemtype = 1;
						break;
					}
				}
				OrbSetCount++;
				OrbSet++;
			}
		}
	}

	return itemtype;
}

/*
 * General 1D surface dialog class
 */

Surface1DPane::Surface1DPane(wxWindow* parent, Surf1DBase* target, 
		SurfacesWindow* Owner, wxWindowID id, 
		const wxPoint& pos, const wxSize& size, 
		long style) 
	: BaseSurfacePane(parent, target, Owner, id, pos, size, style),
	  endpt1_xbox(NULL),
	  endpt1_ybox(NULL),
	  endpt1_zbox(NULL),
	  endpt2_xbox(NULL),
	  endpt2_ybox(NULL),
	  endpt2_zbox(NULL) {
	mOrbColor1 = NULL;
	mOrbColor2 = NULL;

	mTarget = target;
}

Surface1DPane::~Surface1DPane() {
	delete mOrbColor1;
	if (mOrbColor2) delete mOrbColor2;
}

void Surface1DPane::OnPosColorChange(wxCommandEvent & event) {
	mOrbColor1->getColor(&PosColor);
	setUpdateButton();
}

void Surface1DPane::OnNegColorChange(wxCommandEvent & event) {
	mOrbColor2->getColor(&NegColor);
	setUpdateButton();
}

void Surface1DPane::OnChangeClamp(wxCommandEvent &event) {

	double dval;

	if (clamp_box->GetValue().ToDouble(&dval)) {
		MaxContourValue = (float) dval;
		setUpdateButton();
	}

}

void Surface1DPane::OnChangeScale(wxCommandEvent &event) {

	double dval;

	if (scale_box->GetValue().ToDouble(&dval)) {
		Scale = (float) dval;
		setUpdateButton();
	}

}

void Surface1DPane::OnChangeEndpoint(wxCommandEvent &event) {
	bool ok;
	bool any_ok;
	double dval;
	float fval;
#define UPDATE_ENDPOINT(control, coord) \
	if (control) { \
		ok = control->GetValue().ToDouble(&dval); \
		fval = (float) dval; \
		if (ok) { \
			coord = fval; \
			any_ok = true; \
		} \
	}

	UPDATE_ENDPOINT(endpt1_xbox, Start.x);
	UPDATE_ENDPOINT(endpt1_ybox, Start.y);
	UPDATE_ENDPOINT(endpt1_zbox, Start.z);
	UPDATE_ENDPOINT(endpt2_xbox, End.x);
	UPDATE_ENDPOINT(endpt2_ybox, End.y);
	UPDATE_ENDPOINT(endpt2_zbox, End.z);

#undef UPDATE_ENDPOINT

	if (any_ok) {
		setUpdateButton();
	}
}

/* if the focus in the panel is changed between main panel and 
   two wxTextCtrls, read the content of two wxTextCtrls. */

void Surface1DPane::OnIdle(wxIdleEvent& WXUNUSED(event)) {
	static wxWindow *s_windowFocus = (wxWindow *)NULL;
	wxWindow *focus = wxWindow::FindFocus();

	if (focus && (focus != s_windowFocus)) {
		s_windowFocus = focus;
		wxString className = s_windowFocus->GetClassInfo()->GetClassName();

		if (className.Cmp(_T("wxTextCtrl")) == 0 
				|| className.Cmp(_T("Orbital2DSurfPane")) == 0) {
			setUpdateButton();
		}
	}
}

/*
 * General 2D surface dialog class
 */

Surface2DPane::Surface2DPane(wxWindow* parent, Surf2DBase* target, 
		SurfacesWindow* Owner, wxWindowID id, 
		const wxPoint& pos, const wxSize& size, 
		long style) 
	: BaseSurfacePane(parent, target, Owner, id, pos, size, style) {
	mOrbColor1 = NULL;
	mOrbColor2 = NULL;

	mTarget = target;
}

Surface2DPane::~Surface2DPane() {
}

void Surface2DPane::OnPosColorChange(wxCommandEvent & event) {
	mOrbColor1->getColor(&PosColor);
	setUpdateButton();
}
void Surface2DPane::OnNegColorChange(wxCommandEvent & event) {
	mOrbColor2->getColor(&NegColor);
	setUpdateButton();
}

/* if the focus in the panel is changed between main panel and 
   two wxTextCtrls, read the content of two wxTextCtrls. */

void Surface2DPane::OnIdle(wxIdleEvent& WXUNUSED(event)) {
	static wxWindow *s_windowFocus = (wxWindow *)NULL;
	wxWindow *focus = wxWindow::FindFocus();

	if (focus && (focus != s_windowFocus)) {
		s_windowFocus = focus;
		wxString className = s_windowFocus->GetClassInfo()->GetClassName();

		if (className.Cmp(_T("wxTextCtrl")) == 0 
				|| className.Cmp(_T("Orbital2DSurfPane")) == 0) {
			long t;
			if ((mNumContourText->GetValue()).ToLong(&t))
				NumContours = t;
			double d;
			if ((mContourValText->GetValue()).ToDouble(&d))
				MaxContourValue = d;
			setUpdateButton();
		}
	}
}

void Surface2DPane::OnDashChk(wxCommandEvent& event) {
	DashLines = mDashCheck->GetValue();
	setUpdateButton();
}

void Surface2DPane::OnShowZeroChk(wxCommandEvent &event) {
	ShowZeroContour = mShowZeroCheck->GetValue();
	setUpdateButton();
}
void Surface2DPane::OnDisplayPlaneChk(wxCommandEvent &event) {
	DisplayPlane = mDisplayPlaneCheck->GetValue();
	setUpdateButton();
}

void Surface2DPane::OnSetPlane(wxCommandEvent &event) {
	SetScreenPlane * temp = new SetScreenPlane(owner->GetParent());
	temp->Show();
}

void Surface2DPane::OnUsePlaneChk(wxCommandEvent &event) {
	UseScreenPlane = mUsePlaneChk->GetValue();
	setUpdateButton();
}

void Surface2DPane::OnSetParam(wxCommandEvent &event) {
	Surface2DParamDlg paramDlg(this, mTarget);
	paramDlg.ShowModal();
}

void Surface2DPane::OnContourValueText(wxCommandEvent& event) {
	//Contour Value edit text has changed, convert the text
	double newVal=0.0;

	wxString tmpStr = mContourValText->GetValue();
	if (tmpStr.ToDouble(&newVal)) {
		if (newVal > 0.0) MaxContourValue = newVal;
	}
	setUpdateButton();
}

void Surface2DPane::SetContourValueText(void) {
	wxString tmpStr;
	tmpStr.Printf(wxT("%.4f"), MaxContourValue);
	mContourValText->SetValue(tmpStr);
}

void Surface2DPane::OnNumContoursText(wxCommandEvent& event) {
	//Num contours edit text has changed, convert the text
	long newVal=-1;

	wxString tmpStr = mNumContourText->GetValue();
	if (tmpStr.ToLong(&newVal)) {
		if (newVal > 0) NumContours = newVal;
	}

	setUpdateButton();
}

void Surface2DPane::SetNumContoursText(void) {
	wxString tmpStr;
	tmpStr.Printf(wxT("%ld"), NumContours);
	mNumContourText->SetValue(tmpStr);
}

/*
 * General 3D surface dialog class
 */

Surface3DPane::Surface3DPane(wxWindow* parent, Surf3DBase* target, 
		SurfacesWindow* Owner, wxWindowID id,
		const wxPoint& pos, const wxSize& size, 
		long style) 
: BaseSurfacePane(parent, target, Owner, id, pos, size, style) {
	mTarget = target;
	UseSolidSurface = mTarget->SolidSurface();
	UseNormals = mTarget->UseSurfaceNormals();
}

Surface3DPane::~Surface3DPane() {
}

void Surface3DPane::setContourValueSld() {
	mContourValSld->SetValue(ContourValue);
}

void Surface3DPane::refreshMEPControls() {

	MoleculeData * data = owner->GetMoleculeData();
	if (data->MEPCalculationPossible()) {
		mColorSurfCheck->SetValue(UseMEP);
		mUseRGBColorCheck->SetValue(UseRGBSurfaceColor);
		if (UseMEP) {
			mUseRGBColorCheck->Enable();
			if (UseRGBSurfaceColor) mInvertRGBCheck->Enable();
			else mInvertRGBCheck->Disable();
			mMaxMapEdit->Enable();
		} else {
			mUseRGBColorCheck->Disable();
			mInvertRGBCheck->Disable();
			mMaxMapEdit->Disable();
		}
		SetMaxMEPValueText();
	} else {
		UseMEP = false;
		mColorSurfCheck->Disable();
		mUseRGBColorCheck->Disable();
		mInvertRGBCheck->Disable();
		mMaxMapEdit->Disable();
	}

}

void Surface3DPane::On3DRadioBox(wxCommandEvent& event) {
	UseSolidSurface = 1-m3DRdoBox->GetSelection();

	if (UseSolidSurface)
		mSmoothChkBox->Enable();
	else
		mSmoothChkBox->Disable();

	setUpdateButton();
}

void Surface3DPane::OnSmoothCheck(wxCommandEvent& event) {
	UseNormals = mSmoothChkBox->GetValue();

	setUpdateButton();
}

void Surface3DPane::changeContourValue() {
	float newVal;

	if (mTarget->GetGridMax() > 0.000001){
		newVal = mContourValSld->GetValue();
		if (newVal < 0.0) newVal *= -1.0;
		if (newVal > mTarget->GetGridMax()) 
			newVal = mTarget->GetGridMax();

		ContourValue = newVal;
		setUpdateButton();
	}
}

void Surface3DPane::OnContourValueEnter(wxCommandEvent& event) {
	OnUpdate(event);
}

void Surface3DPane::SetContourMaxValueText(void) {
	mContourValSld->SetMax(mTarget->GetGridMax());
}

void Surface3DPane::OnContourValueSld(wxCommandEvent &event) {
	ContourValue = mContourValSld->GetValue();
	setUpdateButton();
}

void Surface3DPane::OnGridSizeSld(wxCommandEvent &event) {
	GridSize = 0.01 * mGridSizeSld->GetValue();
	setUpdateButton();
}

void Surface3DPane::OnIdle(wxIdleEvent& WXUNUSED(event)) {
	static wxWindow *s_windowFocus = (wxWindow *)NULL;
	wxWindow *focus = wxWindow::FindFocus();

	if (focus && (focus != s_windowFocus)) {
		s_windowFocus = focus;
		wxString className = s_windowFocus->GetClassInfo()->GetClassName();

		if (className.Cmp(_T("wxTextCtrl")) == 0 
				|| className.Cmp(_T("Orbital3DSurfPane")) == 0) {
			changeContourValue();
			setUpdateButton();
		}
	}

}
void Surface3DPane::OnUseMEPCheck(wxCommandEvent &event) {
	UseMEP = mColorSurfCheck->GetValue();
	refreshControls();
	setUpdateButton();
}
void Surface3DPane::OnRGBColorCheck(wxCommandEvent &event) {
	UseRGBSurfaceColor = mUseRGBColorCheck->GetValue();
	refreshControls();
	setUpdateButton();
}
void Surface3DPane::OnInvertRGBCheck(wxCommandEvent &event) {
	InvertRGBSurfaceColor = mInvertRGBCheck->GetValue();
	//	refreshControls();
	setUpdateButton();
}
void Surface3DPane::OnMaxMEPValueText(wxCommandEvent& event) {
	double newVal=0.0;
	wxString temp = mMaxMapEdit->GetValue();

	if (temp.ToDouble(&newVal)) {
		MaxMEPValue = newVal;
	}
	setUpdateButton();
}
void Surface3DPane::SetMaxMEPValueText() {
	wxString temp;
	temp.Printf(wxT("%.4f"), MaxMEPValue);
	mMaxMapEdit->SetValue(temp);
}
void Surface3DPane::OnFreeMem(wxCommandEvent& event) {
	mTarget->FreeGrid();
	mFreeMemBut->Disable();
}

void Surface3DPane::OnPosColorChange(wxCommandEvent & event) {
	mOrbColor1->getColor(&PosColor);
	setUpdateButton();
}
void Surface3DPane::OnNegColorChange(wxCommandEvent & event) {
	mOrbColor2->getColor(&NegColor);
	setUpdateButton();
}
void Surface3DPane::OnTransparencyChange(wxSpinEvent & event) {
	Transparency = event.GetPosition();
	setUpdateButton();
}

void Surface3DPane::OnSetParam(wxCommandEvent &event) {
	Surface3DParamDlg paramDlg(this, mTarget);
	paramDlg.ShowModal();
}

/*!
 * Orbital2D class
 */

Orbital2DSurfPane::Orbital2DSurfPane(wxWindow* parent, Orb2DSurface* target, 
		SurfacesWindow* o, wxWindowID id, 
		const wxPoint& pos, const wxSize& size, 
		long style) 
: Surface2DPane(parent, target, o, id, pos, size, style), OrbSurfacePane(target, o) {
	mTarget = target;

	TargetToPane();
	CreateControls();
}

Orbital2DSurfPane::~Orbital2DSurfPane() {

}

void Orbital2DSurfPane::CreateControls() {
	//Stuff from the base class creation
	wxBoxSizer * upperSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * middleSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * leftMiddleSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * upperLeftMiddleSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * lowerLeftMiddleSizer = new wxBoxSizer(wxHORIZONTAL);
	wxFlexGridSizer * rightMiddleSizer = new wxFlexGridSizer(2,2,0,0);
	wxBoxSizer * leftBottomSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * rightBottomSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * label0 = new wxStaticText(this, wxID_ANY,
			_T("Select Orbital Set:"),
			wxDefaultPosition,
			wxDefaultSize);

	wxStaticText * label1 = new wxStaticText(this, wxID_ANY,
			_T("Number of\n Grid Points:"),
			wxDefaultPosition,
			wxDefaultSize);

	mNumGridPntSld = new wxSlider(this, ID_GRID_POINT_SLIDER, 
			0, 10, 150,
			wxDefaultPosition, wxSize(155,wxDefaultCoord),
			wxSL_AUTOTICKS | wxSL_LABELS);
	mNumGridPntSld->SetToolTip(_("Sets the resolution of the grid. Fewer points is faster while more points produces a high-resolution result."));

	mSetParamBut = new wxButton(this, ID_SET_PARAM_BUT, wxT("Set Parameters"), wxPoint(450, 160));
	mExportBut = new wxButton(this, ID_SURFACE_EXPORT_BUT, wxT("Export"), wxPoint(450, 160));

	if (mTarget->ExportPossible())
		mExportBut->Enable();
	else
		mExportBut->Disable();

	mUpdateBut = new wxButton(this, ID_SURFACE_UPDATE_BUT, wxT("Update"), wxPoint(450, 160));
	upperSizer->Add(label0, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	wxStaticText* lNumContourLabel = new wxStaticText(this, wxID_ANY,
			_T("Max # of contours:"),
			wxDefaultPosition,
			wxDefaultSize);
	wxStaticText* lContourValLabel = new wxStaticText(this, wxID_ANY,
			_T("Max contour value:"),
			wxDefaultPosition,
			wxDefaultSize);

	mNumContourText = new wxTextCtrl(this, ID_NUM_CONTOUR_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	mNumContourText->SetToolTip(_("Sets the number of contours between zero and the max value."));

	mContourValText = new wxTextCtrl(this, ID_CONTOUR_VALUE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	mContourValText->SetToolTip(_("The maximum value for a contour. The contour spacing is this value divided by the number of contours."));

	mShowZeroCheck = new wxCheckBox(this, ID_SHOW_ZERO_CHECKBOX, _T("Show zero contour"), wxPoint(340,130), wxDefaultSize);

	mDashCheck = new wxCheckBox(this, ID_DASH_CHECKBOX, _T("Dash - contour"), wxPoint(340,130), wxDefaultSize);

	wxButton* lSetPlaneBut = new wxButton(this, ID_SET_PLANE_BUT, wxT("Set Plane"));

	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);

	mOrbColor1 = new colorArea(this, ID_2D_COLOR_POSITIVE, &PosColor);
	mOrbColor2 = new colorArea(this, ID_2D_COLOR_NEGATIVE, &NegColor);

	vector<wxString> choices;
	int itemSelect = getOrbSetForOrbPane(choices) - 1;

	mOrbSetChoice = new wxChoice(this, ID_ORB_CHOICE, wxPoint(10,10), wxSize(200,wxDefaultCoord), choices.size(), &choices.front());
	mOrbSetChoice->SetSelection(itemSelect);
	mOrbSetChoice->SetToolTip(_("Select the set of molecular orbitals or atomic orbitals to view."));
	upperSizer->Add(mOrbSetChoice, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	mSphHarmonicsChk = new wxCheckBox(this, ID_SPH_HARMONICS_CHECKBOX, _T("Spherical Harmonics"), wxDefaultPosition, wxDefaultSize);
	mSphHarmonicsChk->SetToolTip(_("Use spherical harmonics for the AOs. Only available for AOs."));

	wxString choices1[] = {_T("Energy"), _T("Occupation #")};
	mOrbFormatChoice = new wxChoice(this, ID_ORB_FORMAT_CHOICE, wxDefaultPosition, wxSize(120,wxDefaultCoord), 2, choices1);
	mOrbFormatChoice->SetSelection(0);
	mOrbFormatChoice->SetToolTip(_("The MO list can display either the orbital energy or the occupation number."));

	mMOList = new FormattedListBox(this, ID_ATOM_LIST, wxLB_SINGLE | wxLB_ALWAYS_SB);
	makeMOList();
	if (PlotOrb < 0) { // If there is no previous selection & we have MOs pull the default out of the MOList
		if (!(OrbOptions&1)) {//MOs
			PlotOrb = mMOList->GetSelection();
		}
	}

	mOrbCoef = new FormattedListBox(this, ID_ORB_COEF, wxLB_SINGLE | wxLB_ALWAYS_SB);
	makeAOList();

	mSphHarmonicsChk->SetValue(SphericalHarmonics);
	upperLeftMiddleSizer->Add(mSphHarmonicsChk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	mSphHarmonicsChk->Enable(coefIsEnabled);

	lowerLeftMiddleSizer->Add(label1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	lowerLeftMiddleSizer->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	mNumGridPntSld->SetValue(NumGridPoints);

	leftMiddleSizer->Add(upperLeftMiddleSizer, 0, wxALL, 3);
	leftMiddleSizer->Add(lowerLeftMiddleSizer, 0, wxALL, 3);

	wxString tmpStr;

	rightMiddleSizer->Add(lNumContourLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

	tmpStr.Printf(wxT("%ld"), NumContours);
	mNumContourText->SetValue(tmpStr);
	rightMiddleSizer->Add(mNumContourText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

	rightMiddleSizer->Add(lContourValLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

	tmpStr.Printf(wxT("%.2f"), MaxContourValue);
	mContourValText->SetValue(tmpStr);
	rightMiddleSizer->Add(mContourValText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

	wxBoxSizer * SubLeftBot1Sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * SubLeftBot2Sizer = new wxBoxSizer(wxVERTICAL);

	SubLeftBot1Sizer->Add(new wxStaticText(this, wxID_ANY,
				_T("Select Orb:"),
				wxDefaultPosition,
				wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	SubLeftBot1Sizer->Add(mOrbFormatChoice, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	SubLeftBot1Sizer->Add(mMOList, 1, wxGROW | wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALL, 3);

	SubLeftBot2Sizer->Add(new wxStaticText(this, wxID_ANY,
				_T("Orbital vector: \nAtom Orbital Coef"),
				wxDefaultPosition,
				wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
	SubLeftBot2Sizer->Add(mOrbCoef, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALL, 3);

	leftBottomSizer->Add(SubLeftBot1Sizer, 1, wxEXPAND | wxALL, 5);
	leftBottomSizer->Add(SubLeftBot2Sizer, 1, wxEXPAND | wxALL, 5);

	wxBoxSizer * SubRightBot1Sizer = new wxBoxSizer(wxVERTICAL);
	mUsePlaneChk = new wxCheckBox(this, ID_USE_PLANE_CHECKBOX, _T("Use plane of screen"), wxDefaultPosition);
	mUsePlaneChk->SetToolTip(_("When checked the plotting plane is fixed to the plane of the screen and recomputed during rotation."));
	mUsePlaneChk->SetValue(UseScreenPlane);
	mDisplayPlaneCheck = new wxCheckBox(this, ID_DISPLAY_PLANE_CHECKBOX, _("Display Plotting Plane"), wxDefaultPosition, wxDefaultSize, 0);
	mDisplayPlaneCheck->SetValue(DisplayPlane);
	mDisplayPlaneCheck->SetToolTip(_("Display a translucent plane with border to indicate the plotting plane"));

	mRevPhaseChk = new wxCheckBox(this, ID_REVERSE_PHASE_CHECKBOX, _T("Reverse Phase"), wxDefaultPosition);
	mRevPhaseChk->SetToolTip(_("The phase of the orbital is arbitrary. Check the box to flip the coloring."));
	mRevPhaseChk->SetValue(PhaseChange);
	mDashCheck->SetValue(DashLines);
	mShowZeroCheck->SetValue(ShowZeroContour);

	SubRightBot1Sizer->Add(mUsePlaneChk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	SubRightBot1Sizer->Add(mDisplayPlaneCheck, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	SubRightBot1Sizer->Add(mShowZeroCheck, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	SubRightBot1Sizer->Add(mDashCheck, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	SubRightBot1Sizer->Add(mRevPhaseChk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	wxBoxSizer * SubRightBot2Sizer = new wxBoxSizer(wxHORIZONTAL);
	SubRightBot2Sizer->Add(new wxStaticText(this, wxID_ANY,
				_T("Orbital\n Colors:"),
				wxDefaultPosition,
				wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 8);
	SubRightBot2Sizer->Add(mOrbColor1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 8);
	SubRightBot2Sizer->Add(mOrbColor2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 8);

	wxGridSizer * SubRightBot3Sizer = new wxGridSizer(2,2,0,0);
	SubRightBot3Sizer->Add(mSetParamBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	SubRightBot3Sizer->Add(lSetPlaneBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	SubRightBot3Sizer->Add(mExportBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	SubRightBot3Sizer->Add(mUpdateBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	mUpdateBut->SetDefault();

	rightBottomSizer->Add(SubRightBot1Sizer);
	rightBottomSizer->Add(SubRightBot2Sizer);
	rightBottomSizer->Add(SubRightBot3Sizer);

	middleSizer->Add(leftMiddleSizer, 0, wxALL, 10);
	middleSizer->Add(rightMiddleSizer, 0, wxALL, 10);
	bottomSizer->Add(leftBottomSizer, 1, wxEXPAND | wxALL, 3);
	bottomSizer->Add(rightBottomSizer, 0, wxALL, 3);
	mainSizer->Add(upperSizer, 0, wxALIGN_CENTER_HORIZONTAL);
	mainSizer->Add(middleSizer, 0, wxALIGN_CENTER_HORIZONTAL);
	mainSizer->Add(bottomSizer, 1, wxEXPAND, wxALIGN_CENTER_HORIZONTAL);

	if (PlotOrb >= 0) {
		if (OrbOptions&1)	//AOs
			mOrbCoef->SetSelection(PlotOrb);
		else //MOs
			mMOList->SetSelection(PlotOrb);
	}
}

void Orbital2DSurfPane::TargetToPane(void) {
	NumGridPoints = mTarget->GetNumGridPoints();
	NumContours = mTarget->GetNumContours();
	MaxContourValue = mTarget->GetMaxValue();
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	ShowZeroContour = mTarget->GetShowZeroContour();
	UseScreenPlane = mTarget->GetRotate2DMap();
	Visible = mTarget->GetVisibility();
	DashLines = mTarget->GetDashLine();
	DisplayPlane = mTarget->ShowPlottingPlane();
	UpdateTest = false;
}

bool Orbital2DSurfPane::UpdateNeeded(void) {
	bool result = false;

	if (PlotOrb >= 0) {	//Don't update unless a valid orbital is chosen
		if (PlotOrb != mTarget->GetTargetOrb()) result=true;
		if (Visible != mTarget->GetVisibility()) result = true;
		if (AllFrames != (mTarget->GetSurfaceID() != 0)) result = true;
		if (TargetSet != mTarget->GetTargetSet()) result = true;
		if (OrbOptions != mTarget->GetOptions()) result = true;
		if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
		if (NumContours != mTarget->GetNumContours()) result = true;
		if (MaxContourValue != mTarget->GetMaxValue()) result = true;
		if (ShowZeroContour != mTarget->GetShowZeroContour()) result = true;
		if (UseScreenPlane != mTarget->GetRotate2DMap()) result = true;
		if (DashLines != mTarget->GetDashLine()) result = true;
		if (PhaseChange != mTarget->GetPhaseChange()) result = true;
		if (DisplayPlane != mTarget->ShowPlottingPlane()) result = true;
		if (SphericalHarmonics != mTarget->UseSphericalHarmonics()) result = true;

		if (!result) {
			RGBColor	testColor;
			mTarget->GetPosColor(&testColor);
			if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)
					||(PosColor.blue!=testColor.blue)) result=true;
			mTarget->GetNegColor(&testColor);
			if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)||
					(NegColor.blue!=testColor.blue)) result=true;
		}
	}
	return result;
}

void Orbital2DSurfPane::OnUpdate(wxCommandEvent &event) {
	SetNumContoursText();
	SetContourValueText();

	bool updateGrid = UpdateTest;
	MoleculeData * data = owner->GetMoleculeData();

	if (NumGridPoints != mTarget->GetNumGridPoints()) {
		updateGrid = true;
		mTarget->SetNumGridPoints(NumGridPoints);
	}

	mTarget->SetVisibility(Visible);
	mTarget->SetNumContours(NumContours);
	mTarget->SetMaxValue(MaxContourValue);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetNegColor(&NegColor);
	mTarget->SetShowZeroContour(ShowZeroContour);
	mTarget->ShowPlottingPlane(DisplayPlane);
	if (UseScreenPlane && !mTarget->GetRotate2DMap()) updateGrid = true;
	mTarget->SetRotate2DMap(UseScreenPlane);
	mTarget->SetDashLine(DashLines);
	if (SphericalHarmonics != mTarget->UseSphericalHarmonics()) {
		mTarget->UseSphericalHarmonics(SphericalHarmonics);
		updateGrid = true;
	}
	if (PhaseChange != mTarget->GetPhaseChange()) {
		updateGrid = true;
		mTarget->SetPhaseChange(PhaseChange);
	}
	if (OrbSurfacePane::UpdateEvent()) updateGrid = true;
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) {	//update all frames
		long	SurfaceID;
		Frame *	lFrame = data->GetFirstFrame();
		if (AllFrames) {	//adding the surface to all frames
			SurfaceID = mTarget->SetSurfaceID();
			while (lFrame) {
				if (lFrame != data->GetCurrentFramePtr()) {
					Orb2DSurface * NewSurface = new Orb2DSurface(mTarget);
					lFrame->AppendSurface(NewSurface);
				}
				lFrame = lFrame->GetNextFrame();
			}
		} 
		else 
		{			//deleting the surface from other frames
			SurfaceID = mTarget->GetSurfaceID();
			mTarget->SetSurfaceID(0);	//Unmark this frames surface so it doesn't get deleted
			while (lFrame) {
				lFrame->DeleteSurfaceWithID(SurfaceID);
				lFrame = lFrame->GetNextFrame();
			}
		}
	} 
	else if (AllFrames) {
		long SurfaceID = mTarget->GetSurfaceID();
		Frame * lFrame = data->GetFirstFrame();
		while (lFrame) {
			if (lFrame != data->GetCurrentFramePtr()) {
				Surface * temp = lFrame->GetSurfaceWithID(SurfaceID);
				Orb2DSurface * lSurf = NULL;
				if (temp)
					if (temp->GetSurfaceType() == kOrb2DType)
						lSurf = (Orb2DSurface *) temp;
				if (lSurf) {
					lSurf->UpdateData(mTarget);
					if (updateGrid) mTarget->FreeGrid();
				}
			}
			lFrame = lFrame->GetNextFrame();
		}
	}
	if (updateGrid) mTarget->FreeGrid();
	UpdateTest = false;
	mExportBut->Enable();

	mUpdateBut->Disable();

	owner->SurfaceUpdated();
}

void Orbital2DSurfPane::refreshControls() {
	wxString tmpStr;

	mNumGridPntSld->SetValue(NumGridPoints);
	mSphHarmonicsChk->SetValue(SphericalHarmonics); 

	SetNumContoursText();
	SetContourValueText();

	mUsePlaneChk->SetValue(UseScreenPlane);
	mRevPhaseChk->SetValue(PhaseChange);
	mDashCheck->SetValue(DashLines);
	mShowZeroCheck->SetValue(ShowZeroContour);
	mDisplayPlaneCheck->SetValue(DisplayPlane);

	mOrbColor1->setColor(&PosColor);
	mOrbColor2->setColor(&NegColor);
}

void Orbital2DSurfPane::OnOrbSetChoice(wxCommandEvent &event) {
	int item = mOrbSetChoice->GetSelection()+1;
	int itemtype = orbSetChangeEvt(item, owner);

	if (itemtype) {	//TargetSet has been choosen
		PlotOrb = -1;

		if (!(OrbOptions&1)) {	//An MO set is selected
			mSphHarmonicsChk->Enable(false);
			SphericalHarmonics = false;
			mOrbFormatChoice->Enable(true);
		} else {	//Must be looking for AOs
			mSphHarmonicsChk->Enable(true);
			mSphHarmonicsChk->SetValue(SphericalHarmonics);
			mOrbFormatChoice->Enable(false);
		}
	}

	if (item <= 1) {
		mMOList->Clear();
		PlotOrb = -1;
		coefIsEnabled = true;
	} else {
		coefIsEnabled = false;
		makeMOList();
	}
	makeAOList();

	if (!coefIsEnabled) {
		mMOList->SetFocus();
	} else {
		mOrbCoef->SetFocus();
	}
}

void Orbital2DSurfPane::OnCoefList(wxCommandEvent &event) {

	OrbSurfacePane::OnAtomicOrbitalChoice(event);
	if (coefIsEnabled) {
		if (OrbOptions&1)
			PlotOrb = mOrbCoef->GetSelection();

		setUpdateButton();
	}

}

void Orbital2DSurfPane::OnMOList(wxCommandEvent &event) {
	PlotOrb = event.GetSelection();

	makeAOList();

	setUpdateButton();  
}

void Orbital2DSurfPane::OnOrbFormatChoice(wxCommandEvent &event) {
	int itemtype = mOrbFormatChoice->GetSelection();
	setFlagOnOrbFormatChange(itemtype);

	makeMOList();
}

void Orbital2DSurfPane::OnReversePhase(wxCommandEvent &event) {
	PhaseChange = mRevPhaseChk->GetValue();

	setUpdateButton();
}

void Orbital2DSurfPane::OnSphHarmonicChk(wxCommandEvent &event) {
	SphericalHarmonics = mSphHarmonicsChk->GetValue();
	mMOList->Clear();
	PlotOrb = -1;
	/* coefIsEnabled = true; */

	makeAOList();
}

/*!
 * Orbital3D class
 */

Orbital3DSurfPane::Orbital3DSurfPane(wxWindow* parent, Orb3DSurface* target, 
		SurfacesWindow* o, wxWindowID id,
		const wxPoint& pos, const wxSize& size, 
		long style) 
: Surface3DPane(parent, target, o, id, pos, size, style), OrbSurfacePane(target, o) {
	mTarget = target;

	TargetToPane();
	CreateControls();
	refreshControls();
}

Orbital3DSurfPane::~Orbital3DSurfPane() {

}

void Orbital3DSurfPane::TargetToPane(void) {
	NumGridPoints = mTarget->GetNumGridPoints();
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	Transparency = mTarget->GetTransparency();
	GridSize = mTarget->GetGridSize();
	ContourValue = mTarget->GetContourValue();
	UseSolidSurface = mTarget->SolidSurface();
	UseNormals = mTarget->UseSurfaceNormals();
	SphericalHarmonics = mTarget->UseSphericalHarmonics();
	UpdateTest = false;
	SwitchFixGrid = false;
}

void Orbital3DSurfPane::refreshControls() {
	float GridMax = mTarget->GetGridMax();

	mNumGridPntSld->SetValue(NumGridPoints);
	mGridSizeSld->SetValue((short)sqrt((5000*5000)*(mTarget->GetGridSize()-.01)/(30-.01)));
	mContourValSld->SetValue(ContourValue);
	mContourValSld->SetMax(GridMax);
	m3DRdoBox->SetSelection(1-UseSolidSurface);
	mSmoothChkBox->SetValue(UseNormals);
	mSphHarmonicsChk->SetValue(SphericalHarmonics);

	if (UseSolidSurface)
		mSmoothChkBox->Enable();
	else
		mSmoothChkBox->Disable();

	mOrbColor1->setColor(&PosColor);
	mOrbColor2->setColor(&NegColor);
	mTransparency->SetValue(Transparency);
}

/*!
 * Control creation for Orbital3D
 */

void Orbital3DSurfPane::CreateControls() {
	//stuff from the base class creator
	wxBoxSizer * upperSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * middleSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * leftMiddleSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * upperLeftMiddleSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * lowerLeftMiddleSizer = new wxBoxSizer(wxHORIZONTAL);
	wxFlexGridSizer * rightMiddleSizer = new wxFlexGridSizer(2,2,0,0);
	wxBoxSizer * leftBottomSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * rightBottomSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText * label0 = new wxStaticText(this, wxID_ANY,
			_T("Select Orbital Set:"),
			wxDefaultPosition,
			wxDefaultSize);

	wxStaticText * label1 = new wxStaticText(this, wxID_ANY,
			_T("Number of \nGrid Points:"),
			wxDefaultPosition,
			wxDefaultSize);

	mNumGridPntSld = new wxSlider(this, ID_GRID_POINT_SLIDER, 
			0, 10, 150,
			wxDefaultPosition, wxSize(155,wxDefaultCoord),
			wxSL_AUTOTICKS | wxSL_LABELS);
	mNumGridPntSld->SetToolTip(_("Sets the resolution of the grid. Fewer points is faster while more points produces a high-resolution result."));

	mSetParamBut = new wxButton(this, ID_SET_PARAM_BUT, wxT("Set Parameters"), wxPoint(450, 160));
	mExportBut = new wxButton(this, ID_SURFACE_EXPORT_BUT, wxT("Export"), wxPoint(450, 160));

	if (mTarget->ExportPossible())
		mExportBut->Enable();
	else
		mExportBut->Disable();

	mUpdateBut = new wxButton(this, ID_SURFACE_UPDATE_BUT, wxT("Update"), wxPoint(450, 160));

	float GridMax = mTarget->GetGridMax();

	wxStaticText * label2 = new wxStaticText(this, wxID_ANY,
			_T("Grid Size:"),
			wxDefaultPosition,
			wxDefaultSize);
	wxStaticText * label3 = new wxStaticText(this, wxID_ANY,
			_T("Contour Value:"),
			wxDefaultPosition,
			wxDefaultSize);

	mGridSizeSld = new wxSlider(this, ID_GRID_SIZE_SLIDER,
								(short) sqrt((5000*5000)*(mTarget->GetGridSize()-.01)/(30-.01)), 1, 5000,
								wxDefaultPosition, wxSize(CONTOUR_WIDTH,wxDefaultCoord));
	mGridSizeSld->SetToolTip(_("Adjusts the volume over which the grid is computed. Adjust to the right if your surface appears clipped."));

	mContourValSld = new FloatSlider(this, ID_CONTOUR_VALUE_SLIDER,
									 mTarget->GetContourValue() /
									 	((fabs(GridMax)>=0.001)?GridMax:0.25),
									 0.0f, 100.0f, FloatSlider::POW, wxSize(CONTOUR_WIDTH, wxDefaultCoord));
	mContourValSld->SetToolTip(_("Sets the value for the isosurface. Move to the right to produce a surface closer to the nucleus."));

	wxStaticText * label4 = new wxStaticText(this, wxID_ANY,
			_T("Transparency:"),
			wxDefaultPosition,
			wxDefaultSize);

	Transparency = mTarget->GetTransparency();
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);

	mOrbColor1 = new colorArea(this, ID_3D_COLOR_POSITIVE, &PosColor);
	mOrbColor2 = new colorArea(this, ID_3D_COLOR_NEGATIVE, &NegColor);
	mTransparency = new wxSpinCtrl(this, ID_TRANSPARENCY, _T(""),
			wxDefaultPosition, wxDefaultSize,
			wxSP_ARROW_KEYS, 0, 100, 0);
	mTransparency->SetValue(Transparency);

	wxString radiochoices[] = {_T("Solid"), _T("Wire Frame")};
	m3DRdoBox = new wxRadioBox(this, ID_3D_RADIOBOX, _T(""), wxDefaultPosition, wxDefaultSize, WXSIZEOF(radiochoices), radiochoices, 1, wxRA_SPECIFY_ROWS);
	m3DRdoBox->SetSelection(1-UseSolidSurface);

	mSmoothChkBox = new wxCheckBox(this, ID_SMOOTH_CHECKBOX, _T("Smooth"), wxPoint(340,130), wxDefaultSize);
	mSmoothChkBox->SetValue(UseNormals);

	if (UseSolidSurface)
		mSmoothChkBox->Enable();
	else
		mSmoothChkBox->Disable();

	mFreeMemBut = new wxButton(this, ID_FREE_MEM_BUT, wxT("Free Mem"), wxPoint(450, 160));

	if (mTarget->GridAvailable())
		mFreeMemBut->Enable();
	else
		mFreeMemBut->Disable();

	upperSizer->Add(label0, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	vector<wxString> choices;
	int itemSelect = getOrbSetForOrbPane(choices) - 1;

	mOrbSetChoice = new wxChoice(this, ID_ORB_CHOICE, wxPoint(10,10), wxSize(200,wxDefaultCoord), choices.size(), &choices.front());
	mOrbSetChoice->SetSelection(itemSelect);
	upperSizer->Add(mOrbSetChoice, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	mOrbSetChoice->SetToolTip(_("Select the set of molecular orbitals or atomic orbitals to view."));

	upperSizer->Add(10,10);

	mRevPhaseChk = new wxCheckBox(this, ID_REVERSE_PHASE_CHECKBOX, _T("Reverse Phase"), wxDefaultPosition);
	mRevPhaseChk->SetValue(PhaseChange);
	mRevPhaseChk->SetToolTip(_("The phase of the orbital is arbitrary. Check the box to flip the coloring."));

	upperSizer->Add(mRevPhaseChk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	mSphHarmonicsChk = new wxCheckBox(this, ID_SPH_HARMONICS_CHECKBOX, _T("Spherical Harmonics"), wxDefaultPosition, wxDefaultSize);
	mSphHarmonicsChk->Enable(coefIsEnabled);
	mSphHarmonicsChk->SetToolTip(_("Use spherical harmonics for the AOs. Only available for AOs."));

	wxString choices1[] = {_T("Energy"), _T("Occupation #")};
	mOrbFormatChoice = new wxChoice(this, ID_ORB_FORMAT_CHOICE, wxDefaultPosition, wxSize(120,wxDefaultCoord), 2, choices1);
	mOrbFormatChoice->SetSelection(0);
	mRevPhaseChk->SetToolTip(_("When viewing molecular orbitals you may choose to include either the energy or the occupation number in the list."));

	mMOList = new FormattedListBox(this, ID_ATOM_LIST, wxLB_SINGLE);
	makeMOList();
	if (PlotOrb < 0) { // If there is no previous selection & we have MOs pull the default out of the MOList
		if (!(OrbOptions&1)) {//MOs
			PlotOrb = mMOList->GetSelection();
		}
	}

	mOrbCoef = new FormattedListBox(this, ID_ORB_COEF, wxLB_SINGLE);
	makeAOList();

	mSphHarmonicsChk->SetValue(SphericalHarmonics);
	upperLeftMiddleSizer->Add(mSphHarmonicsChk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	lowerLeftMiddleSizer->Add(label1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	lowerLeftMiddleSizer->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	mNumGridPntSld->SetValue(NumGridPoints);

	leftMiddleSizer->Add(upperLeftMiddleSizer, 0, wxALL, 3);
	leftMiddleSizer->Add(lowerLeftMiddleSizer, 0, wxALL, 3);

	rightMiddleSizer->Add(label2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
	rightMiddleSizer->Add(mGridSizeSld, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
	rightMiddleSizer->Add(label3, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
	rightMiddleSizer->Add(mContourValSld, 0, wxFIXED_MINSIZE | wxALIGN_CENTER_VERTICAL | wxALL, 10);

	wxBoxSizer * SubLeftBot1Sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * SubLeftBot2Sizer = new wxBoxSizer(wxVERTICAL);

	SubLeftBot1Sizer->Add(new wxStaticText(this, wxID_ANY,
				_T("Select Orb:"),
				wxDefaultPosition,
				wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	SubLeftBot1Sizer->Add(mOrbFormatChoice, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	SubLeftBot1Sizer->Add(mMOList, 1, wxGROW | wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALL, 3);

	SubLeftBot2Sizer->Add(new wxStaticText(this, wxID_ANY,
				_T("Orbital vector: \nAtom Orbital Coef"),
				wxDefaultPosition,
				wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
	SubLeftBot2Sizer->Add(mOrbCoef, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALL, 3);

	leftBottomSizer->Add(SubLeftBot1Sizer, 1, wxEXPAND | wxALL, 5);
	leftBottomSizer->Add(SubLeftBot2Sizer, 1, wxEXPAND | wxALL, 5);

	wxBoxSizer * SubRightBot1Sizer = new wxBoxSizer(wxHORIZONTAL);
	SubRightBot1Sizer->Add(new wxStaticText(this, wxID_ANY,
				_T("Orbital Colors:"),
				wxDefaultPosition,
				wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	SubRightBot1Sizer->Add(mOrbColor1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	SubRightBot1Sizer->Add(mOrbColor2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	wxBoxSizer * SubRightBot2Sizer = new wxBoxSizer(wxHORIZONTAL);
	SubRightBot2Sizer->Add(label4, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	SubRightBot2Sizer->Add(mTransparency, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	wxBoxSizer * SubRightBot3Sizer = new wxBoxSizer(wxHORIZONTAL);
	SubRightBot3Sizer->Add(m3DRdoBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	wxBoxSizer * SubRightBot4Sizer = new wxBoxSizer(wxHORIZONTAL);
	SubRightBot4Sizer->Add(mSmoothChkBox,0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	wxGridSizer * SubRightBot5Sizer = new wxGridSizer(2,2,0,0);
	SubRightBot5Sizer->Add(mSetParamBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	SubRightBot5Sizer->Add(mExportBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	SubRightBot5Sizer->Add(mFreeMemBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	SubRightBot5Sizer->Add(mUpdateBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	mUpdateBut->SetDefault();

	rightBottomSizer->Add(20, 20);
	rightBottomSizer->Add(SubRightBot1Sizer);
	rightBottomSizer->Add(SubRightBot2Sizer);
	rightBottomSizer->Add(SubRightBot3Sizer);
	rightBottomSizer->Add(SubRightBot4Sizer);
	rightBottomSizer->Add(SubRightBot5Sizer);

	middleSizer->Add(leftMiddleSizer, 0, wxALL | wxALIGN_CENTER, 10);
	middleSizer->Add(45,10);
	middleSizer->Add(rightMiddleSizer, 0, wxALL | wxALIGN_CENTER, 10);
	bottomSizer->Add(leftBottomSizer, 1, wxEXPAND | wxALL | wxALIGN_CENTER, 3);
	bottomSizer->Add(rightBottomSizer, 0, wxALL | wxALIGN_TOP, 3);
	mainSizer->Add(upperSizer, 0, wxALIGN_CENTER);
	mainSizer->Add(middleSizer, 0, wxALIGN_CENTER);
	mainSizer->Add(bottomSizer, 1, wxEXPAND | wxALIGN_CENTER);

	if (PlotOrb >= 0) {
		if (OrbOptions&1) {//AOs
			if (mOrbCoef->GetItemCount()) {
				mOrbCoef->SetSelection(PlotOrb);
			}
		} else { //MOs
			if (mMOList->GetItemCount()) {
				mMOList->SetSelection(PlotOrb);
			}
		}
	}
}

bool Orbital3DSurfPane::UpdateNeeded(void) {
	bool result = false;

	ContourValue = mContourValSld->GetValue();

	if (PlotOrb >= 0) {	//Don't update unless a valid orbital is chosen
		if (PlotOrb != mTarget->GetTargetOrb()) result=true;
		if (Visible != mTarget->GetVisibility()) result = true;
		if (AllFrames != (mTarget->GetSurfaceID() != 0)) result = true;
		if (TargetSet != mTarget->GetTargetSet()) result = true;
		if (OrbOptions != mTarget->GetOptions()) result = true;
		if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
		if (ContourValue != mTarget->GetContourValue()) result = true;
		if (GridSize != mTarget->GetGridSize()) result = true;
		if (UseSolidSurface != mTarget->SolidSurface()) result = true;
		if (UseNormals != mTarget->UseSurfaceNormals()) result = true;
		if (PhaseChange != mTarget->GetPhaseChange()) result = true;
		if (SphericalHarmonics != mTarget->UseSphericalHarmonics()) result = true;

		if (!result) {
			RGBColor	testColor;
			mTarget->GetPosColor(&testColor);
			if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)
					||(PosColor.blue!=testColor.blue)) 
				result=true;

			mTarget->GetNegColor(&testColor);
			if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)
					||(NegColor.blue!=testColor.blue)) 
				result=true;

			unsigned char test_transp = mTarget->GetTransparency();
			if (test_transp != Transparency) 
				result=true;
		}
	}
	return result;
}

void Orbital3DSurfPane::OnOrbFormatChoice(wxCommandEvent &event) {
	int itemtype = mOrbFormatChoice->GetSelection();
	setFlagOnOrbFormatChange(itemtype);

	makeMOList();
}

void Orbital3DSurfPane::OnOrbSetChoice(wxCommandEvent &event) {
	int item = mOrbSetChoice->GetSelection()+1;
	int itemtype = orbSetChangeEvt(item, owner);

	if (itemtype) {	//TargetSet has been choosen
		PlotOrb = -1;

		if (!(OrbOptions&1)) {	//An MO set is selected
			mSphHarmonicsChk->Enable(false);
			SphericalHarmonics = false;
			mOrbFormatChoice->Enable(true);
		} else {	//Must be looking for AOs
			mSphHarmonicsChk->Enable(true);
			mSphHarmonicsChk->SetValue(SphericalHarmonics);
			mOrbFormatChoice->Enable(false);
		}
	}

	if (item <= 1) {
		mMOList->Clear();
		PlotOrb = -1;
		coefIsEnabled = true;
	} else {
		coefIsEnabled = false;
		makeMOList();
	}
	makeAOList();

	if (!coefIsEnabled) {
		mMOList->SetFocus();
	} else {
		mOrbCoef->SetFocus();
	}
}

void Orbital3DSurfPane::OnCoefList(wxCommandEvent &event) {

	OrbSurfacePane::OnAtomicOrbitalChoice(event);
	if (coefIsEnabled) {
		if (OrbOptions&1)
			PlotOrb = mOrbCoef->GetSelection();

		setUpdateButton();
	}

}

void Orbital3DSurfPane::OnMOList(wxCommandEvent &event) {
	PlotOrb = event.GetSelection();

	makeAOList();

	setUpdateButton();  
}

void Orbital3DSurfPane::OnSphHarmonicChk(wxCommandEvent &event) {
	SphericalHarmonics = mSphHarmonicsChk->GetValue();
	mMOList->Clear();
	PlotOrb = -1;
	/* coefIsEnabled = true; */

	makeAOList();
}

void Orbital3DSurfPane::OnReversePhase(wxCommandEvent &event) {
	PhaseChange = mRevPhaseChk->GetValue();

	setUpdateButton();
}

void Orbital3DSurfPane::OnContourValueSld(wxCommandEvent &event) {
	ContourValue = mContourValSld->GetValue();

	setUpdateButton();
}

void Orbital3DSurfPane::OnGridSizeSld(wxCommandEvent &event) {
	GridSize = (30-.01) * (mGridSizeSld->GetValue()*mGridSizeSld->GetValue())/((5000*5000))+.01;
	SwitchFixGrid = true;

	setUpdateButton();
}

void Orbital3DSurfPane::OnUpdate(wxCommandEvent &event) {
	bool updateGrid=UpdateTest;
	bool updateContour=false;

	if (PlotOrb >= 0) {	//Don't update unless a valid orbital is chosen
		if (TargetSet != mTarget->GetTargetSet()) updateGrid = true;
		if (PlotOrb != mTarget->GetTargetOrb()) updateGrid=true;
		if (SphericalHarmonics != mTarget->UseSphericalHarmonics()) updateGrid = true;
		if (OrbOptions != mTarget->GetOptions()) updateGrid = true;
		if (NumGridPoints != mTarget->GetNumGridPoints()) updateGrid = true;
		if (ContourValue != mTarget->GetContourValue()) updateContour = true;
		if (GridSize != mTarget->GetGridSize()) updateGrid = true;
		if (PhaseChange != mTarget->GetPhaseChange()) updateGrid = true;
	}

	if (SwitchFixGrid) {
		mTarget->SetFixGrid(false);
		SwitchFixGrid = false;
		updateGrid = true;
	}

	if (Visible && !mTarget->ContourAvail()) updateContour = true;
	//test to see if grid or contour must calculated anyway
	if (updateContour && ! mTarget->GridAvailable()) updateGrid = true;
	if (updateGrid) updateContour = true;

	mTarget->SetVisibility(Visible);
	mTarget->SolidSurface(UseSolidSurface);
	mTarget->SetNumGridPoints(NumGridPoints);
	mTarget->SetContourValue(ContourValue);
	mTarget->SetGridSize(GridSize);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetNegColor(&NegColor);
	mTarget->SetTransparency(Transparency);
	mTarget->SetPhaseChange(PhaseChange);
	mTarget->UseSurfaceNormals(UseNormals);
	mTarget->UseSphericalHarmonics(SphericalHarmonics);

	OrbSurfacePane::UpdateEvent();

	MoleculeData * mData = owner->GetMoleculeData();

	//update this surface's data on all frames if necessary
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) {	//update all frames
		long	SurfaceID;
		Frame *	lFrame = mData->GetFirstFrame();
		updateGrid = updateContour = true;
		if (AllFrames) {	//adding the surface to all frames
			SurfaceID = mTarget->SetSurfaceID();
			while (lFrame) {
				if (lFrame != mData->GetCurrentFramePtr()) {
					Orb3DSurface * NewSurface = new Orb3DSurface(mTarget);
					lFrame->AppendSurface(NewSurface);
				}
				lFrame = lFrame->GetNextFrame();
			}
		} else {			//deleting the surface from other frames
			SurfaceID = mTarget->GetSurfaceID();
			mTarget->SetSurfaceID(0);	//Unmark this frames surface so it doesn't get deleted
			while (lFrame) {
				lFrame->DeleteSurfaceWithID(SurfaceID);
				lFrame = lFrame->GetNextFrame();
			}
		}
	}

	Progress * lProgress = new Progress();

	if (AllFrames) {	//compute the contour for each frame, no grid is kept
		long SurfaceID = mTarget->GetSurfaceID();
		long CurrentFrame = mData->GetCurrentFrame();
		long NumFrames = mData->GetNumFrames();

		for (int i=0; i<NumFrames; i++) {
			Orb3DSurface * lSurf;
			lSurf = NULL;
			mData->SetCurrentFrame(i+1);
			Frame * lFrame = mData->GetCurrentFramePtr();

			if (CurrentFrame != mData->GetCurrentFrame()) {
				Surface * temp = lFrame->GetSurfaceWithID(SurfaceID);
				//Confirm that the surface is the correct type

				if (temp)
					if (temp->GetSurfaceType() == kOrb3DType) {
						lSurf = (Orb3DSurface *) temp;
					}

				if (lSurf) lSurf->UpdateData(mTarget);
			} else lSurf = mTarget;

			if (lSurf) {
				if (Visible) {
					lSurf->SetTargetSet(mTarget->GetTargetSet());
					lProgress->ChangeText("Calculating 3D Grid...");
					lProgress->SetBaseValue(100*i/NumFrames);
					lProgress->SetScaleFactor((float) 0.9/NumFrames);
					if (updateGrid) {
						lSurf->CalculateMOGrid(mData, lProgress);
					}
					lProgress->ChangeText("Contouring grid...");
					lProgress->SetBaseValue((long)(100*i/NumFrames + 90.0/NumFrames));
					lProgress->SetScaleFactor((float) 0.1/NumFrames);
					if (updateContour) {
						lSurf->Contour3DGrid(lProgress);
					}
					lSurf->FreeGrid();
				} else {
					if (updateGrid) lSurf->FreeGrid();
					if (updateContour) lSurf->FreeContour();
				}
			}
		}
		mData->SetCurrentFrame(CurrentFrame);
	} else {	//simply update this surface
		if (Visible) {
			try {
				lProgress->ChangeText("Calculating 3D Grid...");
				lProgress->SetScaleFactor(0.9);
				if (updateGrid) mTarget->CalculateMOGrid(mData, lProgress);
				lProgress->ChangeText("Contouring grid...");
				lProgress->SetBaseValue(90);
				lProgress->SetScaleFactor(0.1);
				if (updateContour) mTarget->Contour3DGrid(lProgress);
			}
			catch (bad_alloc & ba) {
				MessageAlert("Error: Unable to allocate enough memory to update surface.");
			}
			catch (...) {
				MessageAlert("Error: unknown exception occured while attempting to update surface.");
			}
		} else {
			if (updateGrid) mTarget->FreeGrid();
			if (updateContour) mTarget->FreeContour();
		}
	}
	if (lProgress) delete lProgress;

	if (mTarget->GridAvailable())
		mFreeMemBut->Enable();
	else
		mFreeMemBut->Disable();

	float GridMax = mTarget->GetGridMax();

	//Setup the contour value and grid max text items
	mContourValSld->SetMax(GridMax);
	setContourValueSld();

	UpdateTest = false;

	if (mTarget->ExportPossible())
		mExportBut->Enable();
	else
		mExportBut->Disable();

	mUpdateBut->Disable();

	owner->SurfaceUpdated();

	if (!coefIsEnabled)
		mMOList->SetFocus();
	else
		mOrbCoef->SetFocus();
}

/*!
 * Get bitmap resources
 */

wxBitmap Orbital3DSurfPane::GetBitmapResource(const wxString& name) {
	// Bitmap retrieval
	////@begin Orbital3D bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
	////@end Orbital3D bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Orbital3DSurfPane::GetIconResource(const wxString& name) {
	// Icon retrieval
	////@begin Orbital3D icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
	////@end Orbital3D icon retrieval
}

/*!
 * General3DSurfPane class
 */

General3DSurfPane::General3DSurfPane(wxWindow* parent, General3DSurface* target, 
		SurfacesWindow* o, wxWindowID id,
		const wxPoint& pos, const wxSize& size, 
		long style) 
: Surface3DPane(parent, target, o, id, pos, size, style) {
	mTarget = target;
	MultValue = -1.0;
	useMultValue = false;
	squareValues = false;

	TargetToPane();
	CreateControls();
	refreshControls();
}

General3DSurfPane::~General3DSurfPane() {

}

void General3DSurfPane::TargetToPane(void) {
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	Transparency = mTarget->GetTransparency();
	ContourValue = mTarget->GetContourValue();
	UseSolidSurface = mTarget->SolidSurface();
	UseNormals = mTarget->UseSurfaceNormals();
	ContourPosNeg = mTarget->ContourBothPosNeg();
	UseMEP = mTarget->ColorByValue();
	UseRGBSurfaceColor = mTarget->UseRGBColoration();
	InvertRGBSurfaceColor = mTarget->InvertRGBColoration();
	MaxMEPValue = mTarget->GetMaxSurfaceValue();
	UpdateTest = false;
}

void General3DSurfPane::refreshControls() {
	float GridMax = mTarget->GetGridMax();
	float GridMin = mTarget->GetGridMin();
	if (ContourPosNeg) GridMin = 0.0;
	float Range = GridMax - GridMin;
	if (fabs(Range) < ContourValue) Range = ContourValue;

	mContourValSld->SetValue(ContourValue);
	wxString tmpStr;
	mContourValSld->SetMax(GridMax);
	mContourValSld->SetMin(GridMin);

	tmpStr.Printf(wxT("%.4f"), MultValue);
	mGenMultValue->SetValue(tmpStr);
	mMultCheck->SetValue(useMultValue);
	mSquareCheck->SetValue(squareValues);

	m3DRdoBox->SetSelection(1-UseSolidSurface);
	mSmoothChkBox->SetValue(UseNormals);

	if (UseSolidSurface)
		mSmoothChkBox->Enable();
	else
		mSmoothChkBox->Disable();

	mOrbColor1->setColor(&PosColor);
	mOrbColor2->setColor(&NegColor);
	mTransparency->SetValue(Transparency);

	mGenContourPosNegCheck->SetValue(ContourPosNeg);
	refreshMEPControls();

	// We make dual contouring and MEP coloring mutually exclusive.
	mColorSurfCheck->Enable(!ContourPosNeg);
	mUseRGBColorCheck->Enable(mUseRGBColorCheck->IsEnabled() && !ContourPosNeg);
	mInvertRGBCheck->Enable(mInvertRGBCheck->IsEnabled() && !ContourPosNeg);
	mMaxMapEdit->Enable(mMaxMapEdit->IsEnabled() && !ContourPosNeg);
	mGenContourPosNegCheck->Enable(!UseMEP);
}

/*!
 * Control creation General3D
 */

void General3DSurfPane::CreateControls() {    
	General3DSurfPane * Gen3DPanel = this;	//this is here just to match the code below from DialogBlocks

	wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText6 = new wxStaticText(Gen3DPanel, wxID_STATIC, _("Choose 3D grid file:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer5->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton7 = new wxButton(Gen3DPanel, ID_GENFILEBUTTON, _("Choose File..."), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer5->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	mMultCheck = new wxCheckBox(Gen3DPanel, ID_GENMULTCHECK, _("Multiply by:"), wxDefaultPosition, wxDefaultSize, 0);
	mMultCheck->SetValue(false);
	mMultCheck->SetToolTip(_("check to multiply the grid values by the value to the left as they are read in from file."));
	itemBoxSizer8->Add(mMultCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mGenMultValue = new wxTextCtrl(Gen3DPanel, ID_GENMULTEDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	mGenMultValue->SetToolTip(_("Enter a value to multiply the grid by as it is read in from file."));
	itemBoxSizer8->Add(mGenMultValue, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mSquareCheck = new wxCheckBox(Gen3DPanel, ID_GENSQUARECHECK, _("Square values as read in"), wxDefaultPosition, wxDefaultSize, 0);
	mSquareCheck->SetValue(false);
	mMultCheck->SetToolTip(_("check to square the grid values as they are read in from file."));
	mainSizer->Add(mSquareCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer12->Add(itemBoxSizer13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText* itemStaticText14 = new wxStaticText(Gen3DPanel, wxID_STATIC, _("Contour Value:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer13->Add(itemStaticText14, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mContourValSld = new FloatSlider(Gen3DPanel, ID_CONTOUR_VALUE_SLIDER, 0.0f, 0.0f, 100.0f, FloatSlider::POW, wxSize(CONTOUR_WIDTH, wxDefaultCoord));
	itemBoxSizer12->Add(mContourValSld, 0, wxFIXED_MINSIZE | wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer21, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer21->Add(itemBoxSizer22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	wxStaticText* itemStaticText23 = new wxStaticText(Gen3DPanel, wxID_STATIC, _("Positive Color:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer22->Add(itemStaticText23, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	// Create MEP box.
	wxStaticBoxSizer *mep_sizer = new wxStaticBoxSizer(wxHORIZONTAL, Gen3DPanel, wxT("MEP"));
	mainSizer->Add(mep_sizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	wxBoxSizer* itemBoxSizer112 = new wxBoxSizer(wxVERTICAL);
	mep_sizer->Add(itemBoxSizer112, 0);
	mColorSurfCheck = new wxCheckBox(Gen3DPanel, ID_3D_COLOR_SURF_CHECK, _("Colorize using the surface MEP value"), wxDefaultPosition, wxDefaultSize, 0);
	mColorSurfCheck->SetValue(false);
	mColorSurfCheck->SetToolTip(_("check to colorize the isosurface with the MEP value. The molecule must have appropriate orbitals available to produce a MEP computation."));
	itemBoxSizer112->Add(mColorSurfCheck, 0);

	mUseRGBColorCheck = new wxCheckBox(Gen3DPanel, ID_USERGB_COLOR_CHECK, _("Use RGB surface coloration"), wxDefaultPosition, wxDefaultSize, 0);
	mUseRGBColorCheck->SetToolTip(_("Uses blue for attractive, green for neutral, and red of repulsive to a + charge"));
	mUseRGBColorCheck->SetValue(false);
	itemBoxSizer112->Add(mUseRGBColorCheck, 0);

	mInvertRGBCheck = new wxCheckBox(Gen3DPanel, ID_INVERT_RGB_CHECK, _("Invert color map"), wxDefaultPosition, wxDefaultSize, 0);
	mInvertRGBCheck->SetValue(false);
	mInvertRGBCheck->SetToolTip(_("Flips the RGB mapping so that red is attractive, blue replusive to a + charge"));
	itemBoxSizer112->Add(mInvertRGBCheck, 0);

	mep_sizer->AddSpacer(10);

	wxBoxSizer* itemBoxSizer115 = new wxBoxSizer(wxVERTICAL);
	mep_sizer->Add(itemBoxSizer115, 0, wxALIGN_CENTER_VERTICAL);
	wxStaticText* itemStaticText116 = new wxStaticText(Gen3DPanel, wxID_STATIC, _("Max. value to map"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer115->Add(itemStaticText116, 0);

	mMaxMapEdit = new wxTextCtrl(Gen3DPanel, ID_3D_MAX_MAP_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0);
	mMaxMapEdit->SetToolTip(_("Set the maximum value for the color map. Values large than the max map to the same color."));
	itemBoxSizer115->Add(mMaxMapEdit, 0, wxALIGN_CENTER_HORIZONTAL);

	Transparency = mTarget->GetTransparency();
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);

	mOrbColor1 = new colorArea(Gen3DPanel, ID_3D_COLOR_POSITIVE, &PosColor);
	itemBoxSizer22->Add(mOrbColor1, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer21->Add(itemBoxSizer25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	wxStaticText* itemStaticText26 = new wxStaticText(Gen3DPanel, wxID_STATIC, _("Negative Color:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer25->Add(itemStaticText26, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mOrbColor2 = new colorArea(Gen3DPanel, ID_3D_COLOR_NEGATIVE, &NegColor);
	itemBoxSizer25->Add(mOrbColor2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer21->Add(itemBoxSizer28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	wxStaticText* itemStaticText29 = new wxStaticText(Gen3DPanel, wxID_STATIC, _("Transparency:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer28->Add(itemStaticText29, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mTransparency = new wxSpinCtrl(this, ID_TRANSPARENCY, _T(""),
			wxDefaultPosition, wxDefaultSize,
			wxSP_ARROW_KEYS, 0, 100, 0);
	mTransparency->SetValue(Transparency);
	itemBoxSizer28->Add(mTransparency, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer31 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer31, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	wxString m3DRdoBoxStrings[] = {
		_("&Solid"),
		_("&WireFrame")
	};
	m3DRdoBox = new wxRadioBox(Gen3DPanel, ID_3D_RADIOBOX, _("Surface Display"), wxDefaultPosition, wxDefaultSize, 2, m3DRdoBoxStrings, 1, wxRA_SPECIFY_ROWS);
	m3DRdoBox->SetSelection(0);
	itemBoxSizer31->Add(m3DRdoBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mSmoothChkBox = new wxCheckBox(Gen3DPanel, ID_SMOOTH_CHECKBOX, _("Smooth"), wxDefaultPosition, wxDefaultSize, 0);
	mSmoothChkBox->SetValue(false);
	mSmoothChkBox->SetToolTip(_("Check to use surface normals to smooth out the surface of the contour."));
	itemBoxSizer31->Add(mSmoothChkBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mGenContourPosNegCheck = new wxCheckBox(Gen3DPanel, ID_GENCONPOSNEGCHECK, _("Contour +/- values"), wxDefaultPosition, wxDefaultSize, 0);
	mGenContourPosNegCheck->SetValue(false);
	mGenContourPosNegCheck->SetToolTip(_("Check to produce both + and - contours with the magnitude given above."));
	mainSizer->Add(mGenContourPosNegCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer35, 0, wxALIGN_RIGHT|wxALL, 5);
	mFreeMemBut = new wxButton(Gen3DPanel, ID_FREE_MEM_BUT, _("Free Mem"), wxDefaultPosition, wxDefaultSize, 0);

	if (mTarget->GridAvailable())
		mFreeMemBut->Enable();
	else
		mFreeMemBut->Disable();

	mFreeMemBut->SetToolTip(_("Click to free the memory used by the 3D grid."));
	itemBoxSizer35->Add(mFreeMemBut, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mExportBut = new wxButton(Gen3DPanel, ID_SURFACE_EXPORT_BUT, _("Export..."), wxDefaultPosition, wxDefaultSize, 0);
	mExportBut->SetToolTip(_("Click to export the selected surface to a text file."));
	itemBoxSizer35->Add(mExportBut, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mUpdateBut = new wxButton(Gen3DPanel, ID_SURFACE_UPDATE_BUT, _("Update"), wxDefaultPosition, wxDefaultSize, 0);

	if (mTarget->ExportPossible())
		mExportBut->Enable();
	else
		mExportBut->Disable();

	mUpdateBut->SetDefault();
	mUpdateBut->SetToolTip(_("Click to apply your changes to the molecule display."));
	itemBoxSizer35->Add(mUpdateBut, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
}

bool General3DSurfPane::UpdateNeeded(void) {
	bool result = UpdateTest;

	if (mTarget->GridAvailable()) {	//Don't update unless a valid 3D grid has been read in from file
		if (Visible != mTarget->GetVisibility()) result = true;
		if (ContourValue != mTarget->GetContourValue()) result = true;
		if (UseSolidSurface != mTarget->SolidSurface()) result = true;
		if (UseMEP != mTarget->ColorByValue()) result = true;
		if (UseNormals != mTarget->UseSurfaceNormals()) result = true;
		if (UseRGBSurfaceColor != mTarget->UseRGBColoration()) result = true;
		if (InvertRGBSurfaceColor != mTarget->InvertRGBColoration()) result = true;
		if (MaxMEPValue != mTarget->GetMaxSurfaceValue()) result = true;
		//		if (UseColorByValue != mTarget->ColorByValue()) result = true;
		if (ContourPosNeg != mTarget->ContourBothPosNeg()) result = true;
		if (UseNormals != mTarget->UseSurfaceNormals()) result = true;
		if (!result) {
			RGBColor	testColor;
			mTarget->GetPosColor(&testColor);
			if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)||
					(PosColor.blue!=testColor.blue)) result=true;
			mTarget->GetNegColor(&testColor);
			if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)||
					(NegColor.blue!=testColor.blue)) result=true;
			unsigned char test_transp = mTarget->GetTransparency();
			if (test_transp != Transparency) 
				result=true;
		}
	}
	return result;
}
void General3DSurfPane::OnUpdate(wxCommandEvent &event) {
	SetMaxMEPValueText();

	bool updateMEP=false;
	bool updateContour = UpdateTest || ! mTarget->ContourAvail();
	if (Visible) {	//update the contour if contour value has changed
		if (ContourValue != mTarget->GetContourValue()) updateContour = true;
	}
	mTarget->SetVisibility(Visible);
	if (UseMEP && !mTarget->ColorByValue()) updateMEP = true;
	if (updateMEP && !mTarget->ContourAvail()) updateContour = true;
	mTarget->SolidSurface(UseSolidSurface);
	if (ContourPosNeg != mTarget->ContourBothPosNeg()) updateContour = true;
	mTarget->SetContourBothPosNeg(ContourPosNeg);
	if (updateContour && UseMEP) updateMEP = true;
	//	mTarget->SetColorByValue(UseColorByValue);
	mTarget->SetColorByValue(UseMEP);
	mTarget->SetContourValue(ContourValue);
	mTarget->SetMaxSurfaceValue(MaxMEPValue);
	mTarget->UseRGBColoration(UseRGBSurfaceColor);
	mTarget->InvertRGBColoration(InvertRGBSurfaceColor);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetNegColor(&NegColor);
	mTarget->SetTransparency(Transparency);
	mTarget->UseSurfaceNormals(UseNormals);

	MoleculeData * data = owner->GetMoleculeData();
	float MEPScale = 1.0;
	if (updateMEP) MEPScale = 0.5;
	Progress * lProgress = new Progress();
	if (updateContour && mTarget->GridAvailable()) {
		lProgress->ChangeText("Contouring grid...");
		lProgress->SetBaseValue((long)(90*MEPScale));
		lProgress->SetScaleFactor(0.1*MEPScale);
		mTarget->Contour3DGrid(lProgress);
	}
	if (updateMEP && data->MEPCalculationPossible()) {
		lProgress->ChangeText("Calculating MEP values...");
		lProgress->SetBaseValue(50);
		lProgress->SetScaleFactor(0.5);
		owner->GetParent()->Freeze();
		mTarget->CalculateSurfaceValues(data, lProgress);
		owner->GetParent()->Thaw();
	}
	delete lProgress;
	//Setup the contour value and grid max text items
	float GridMax = mTarget->GetGridMax();
	float GridMin = mTarget->GetGridMin();
	if (ContourPosNeg) GridMin = 0.0;
	float Range = GridMax - GridMin;
	if (fabs(Range) < ContourValue) Range = ContourValue;

	mContourValSld->SetValue(ContourValue);

	UpdateTest = false;
	setUpdateButton();
	owner->SurfaceUpdated();
}
void General3DSurfPane::OnMultCheck(wxCommandEvent &event) {
	useMultValue = mMultCheck->GetValue();
	setUpdateButton();
}
void General3DSurfPane::OnSquareCheck(wxCommandEvent &event) {
	squareValues = mSquareCheck->GetValue();
	setUpdateButton();
}
void General3DSurfPane::OnContourPosNegCheck(wxCommandEvent &event) {
	ContourPosNeg = mGenContourPosNegCheck->GetValue();
	refreshControls();
	setUpdateButton();
}
void General3DSurfPane::OnContourValueSld(wxCommandEvent &event) {
	float GridMax = mTarget->GetGridMax();
	float GridMin = mTarget->GetGridMin();
	if (ContourPosNeg) GridMin = 0.0;
	float Range = GridMax - GridMin;
	if (fabs(Range)< ContourValue) Range = ContourValue;

	ContourValue = mContourValSld->GetValue();

	setUpdateButton();
}
void General3DSurfPane::OnMultValueEdit(wxCommandEvent& event) {
	double newVal=0.0;
	wxString temp = mGenMultValue->GetValue();

	if (temp.ToDouble(&newVal)) {
		MultValue = newVal;
	}
	setUpdateButton();
}
void General3DSurfPane::OnFileButton(wxCommandEvent& event) {
	mTarget->ReadGrid(squareValues, useMultValue, MultValue);
	UpdateTest = true;
	refreshControls();
	setUpdateButton();
}

/*!
 * Get bitmap resources
 */

wxBitmap General3DSurfPane::GetBitmapResource(const wxString& name) {
	// Bitmap retrieval
	////@begin Orbital3D bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
	////@end Orbital3D bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon General3DSurfPane::GetIconResource(const wxString& name) {
	// Icon retrieval
	////@begin Orbital3D icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
	////@end Orbital3D icon retrieval
}
/*!
 * General2DSurfPane class
 */

General2DSurfPane::General2DSurfPane(wxWindow* parent, General2DSurface* target, 
		SurfacesWindow* o, wxWindowID id,
		const wxPoint& pos, const wxSize& size, 
		long style) 
: Surface2DPane(parent, target, o, id, pos, size, style) {
	mTarget = target;
	MultValue = -1.0;
	useMultValue = false;
	squareValues = false;

	TargetToPane();
	CreateControls();
	refreshControls();
}

General2DSurfPane::~General2DSurfPane() {

}

void General2DSurfPane::CreateControls() {
	General2DSurfPane * Gen2DPanel = this;	//this is here just to match the code below from DialogBlocks

	wxBoxSizer* itemBoxSizer41 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer41, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText42 = new wxStaticText(Gen2DPanel, wxID_STATIC, _("Choose 2D grid file:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer41->Add(itemStaticText42, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton43 = new wxButton(Gen2DPanel, ID_GENFILEBUTTON, _("Choose File..."), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer41->Add(itemButton43, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer44 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer44, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	mMultCheck = new wxCheckBox(Gen2DPanel, ID_GENMULTCHECK, _("Multiply by:"), wxDefaultPosition, wxDefaultSize, 0);
	mMultCheck->SetValue(false);
	if (ShowToolTips())
		mMultCheck->SetToolTip(_("check to multiply the grid values by the value to the left as they are read in from file."));
	itemBoxSizer44->Add(mMultCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mGenMultValue = new wxTextCtrl(Gen2DPanel, ID_GENMULTEDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (ShowToolTips())
		mGenMultValue->SetToolTip(_("Enter a value to multiply the grid by as it is read in from file."));
	itemBoxSizer44->Add(mGenMultValue, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mSquareCheck = new wxCheckBox(Gen2DPanel, ID_GENSQUARECHECK, _("Square Grid values as read in"), wxDefaultPosition, wxDefaultSize, 0);
	mSquareCheck->SetValue(false);
	if (ShowToolTips())
		mSquareCheck->SetToolTip(_("Check to square the grid values as they are read in from the file."));
	mainSizer->Add(mSquareCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer48 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer48, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText49 = new wxStaticText(Gen2DPanel, wxID_STATIC, _("Max # of contours:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer48->Add(itemStaticText49, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mNumContourText = new wxTextCtrl(Gen2DPanel, ID_NUM_CONTOUR_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (ShowToolTips())
		mNumContourText->SetToolTip(_("The maximum number of contours to create."));
	itemBoxSizer48->Add(mNumContourText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer51 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer51, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText52 = new wxStaticText(Gen2DPanel, wxID_STATIC, _("Max contour value:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer51->Add(itemStaticText52, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mContourValText = new wxTextCtrl(Gen2DPanel, ID_CONTOUR_VALUE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (ShowToolTips())
		mContourValText->SetToolTip(_("The maximum value to contour. The contour spacing is this value divided by the number of contours."));
	itemBoxSizer51->Add(mContourValText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer54 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer54, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	wxBoxSizer* itemBoxSizer55 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer54->Add(itemBoxSizer55, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	wxStaticText* itemStaticText56 = new wxStaticText(Gen2DPanel, wxID_STATIC, _("Positive Color:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer55->Add(itemStaticText56, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mOrbColor1 = new colorArea(Gen2DPanel, ID_2D_COLOR_POSITIVE, &PosColor);
	itemBoxSizer55->Add(mOrbColor1, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer58 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer54->Add(itemBoxSizer58, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	wxStaticText* itemStaticText59 = new wxStaticText(Gen2DPanel, wxID_STATIC, _("Negative Color:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer58->Add(itemStaticText59, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mOrbColor2 = new colorArea(Gen2DPanel, ID_2D_COLOR_NEGATIVE, &NegColor);
	itemBoxSizer58->Add(mOrbColor2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mShowZeroCheck = new wxCheckBox(Gen2DPanel, ID_SHOW_ZERO_CHECKBOX, _("Show zero value contour"), wxDefaultPosition, wxDefaultSize, 0);
	mShowZeroCheck->SetValue(false);
	if (ShowToolTips())
		mShowZeroCheck->SetToolTip(_("Check to produce a gray contour where the plane changes sign."));
	mainSizer->Add(mShowZeroCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mDisplayPlaneCheck = new wxCheckBox(Gen2DPanel, ID_DISPLAY_PLANE_CHECKBOX, _("Display Plotting Plane"), wxDefaultPosition, wxDefaultSize, 0);
	mDisplayPlaneCheck->SetValue(false);
	if (ShowToolTips())
		mDisplayPlaneCheck->SetToolTip(_("Display a translucent plane with border to indicate the plotting plane"));
	mainSizer->Add(mDisplayPlaneCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mDashCheck = new wxCheckBox(Gen2DPanel, ID_DASH_CHECKBOX, _("Dash negative contours"), wxDefaultPosition, wxDefaultSize, 0);
	mDashCheck->SetValue(false);
	if (ShowToolTips())
		mDashCheck->SetToolTip(_("Produces dashed negative contour lines for easier viewing in B & W."));
	mainSizer->Add(mDashCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer63 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer63, 0, wxALIGN_RIGHT|wxALL, 5);
	mExportBut = new wxButton(Gen2DPanel, ID_SURFACE_EXPORT_BUT, _("Export..."), wxDefaultPosition, wxDefaultSize, 0);

	if (mTarget->ExportPossible())
		mExportBut->Enable();
	else
		mExportBut->Disable();

	if (ShowToolTips())
		mExportBut->SetToolTip(_("Click to export the selected surface to a text file."));
	itemBoxSizer63->Add(mExportBut, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mUpdateBut = new wxButton(Gen2DPanel, ID_SURFACE_UPDATE_BUT, _("Update"), wxDefaultPosition, wxDefaultSize, 0);
	mUpdateBut->SetDefault();
	if (ShowToolTips())
		mUpdateBut->SetToolTip(_("Click to apply your changes to the molecule display."));
	itemBoxSizer63->Add(mUpdateBut, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
}
void General2DSurfPane::TargetToPane(void) {
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	MaxContourValue = mTarget->GetMaxValue();
	NumContours = mTarget->GetNumContours();
	DashLines = mTarget->GetDashLine();
	ShowZeroContour = mTarget->GetShowZeroContour();
	DisplayPlane = mTarget->ShowPlottingPlane();
	UpdateTest = false;
}
bool General2DSurfPane::UpdateNeeded(void) {
	bool result = UpdateTest;

	if (mTarget->GridAvailable()) {	//Don't update unless a valid 2D grid has been read in from file
		if (Visible != mTarget->GetVisibility()) result = true;
		if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
		if (NumContours != mTarget->GetNumContours()) result = true;
		if (MaxContourValue != mTarget->GetMaxValue()) result = true;
		if (ShowZeroContour != mTarget->GetShowZeroContour()) result = true;
		if (DashLines != mTarget->GetDashLine()) result = true;
		if (DisplayPlane != mTarget->ShowPlottingPlane()) result = true;
		if (!result) {
			RGBColor	testColor;
			mTarget->GetPosColor(&testColor);
			if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)||
					(PosColor.blue!=testColor.blue)) result=true;
			mTarget->GetNegColor(&testColor);
			if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)||
					(NegColor.blue!=testColor.blue)) result=true;
		}
	}
	return result;
}
void General2DSurfPane::OnUpdate(wxCommandEvent &event) {
	SetNumContoursText();
	SetContourValueText();

	mTarget->SetVisibility(Visible);
	mTarget->SetNumContours(NumContours);
	mTarget->SetMaxValue(MaxContourValue);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetNegColor(&NegColor);
	mTarget->SetShowZeroContour(ShowZeroContour);
	mTarget->SetDashLine(DashLines);
	mTarget->ShowPlottingPlane(DisplayPlane);

	UpdateTest = false;
	setUpdateButton();
	owner->SurfaceUpdated();
}

void General2DSurfPane::refreshControls() {
	wxString tmpStr;
	tmpStr.Printf(wxT("%.2f"), MaxContourValue);
	mContourValText->SetValue(tmpStr);
	tmpStr.Printf(wxT("%ld"), NumContours);
	mNumContourText->SetValue(tmpStr);

	SetMultValue();
	mMultCheck->SetValue(useMultValue);
	mSquareCheck->SetValue(squareValues);

	mOrbColor1->setColor(&PosColor);
	mOrbColor2->setColor(&NegColor);

	mShowZeroCheck->SetValue(ShowZeroContour);
	mDashCheck->SetValue(DashLines);
	mDisplayPlaneCheck->SetValue(DisplayPlane);
}
void General2DSurfPane::SetMultValue(void) {
	wxString temp;
	temp.Printf(wxT("%.4f"), MultValue);
	mGenMultValue->SetValue(temp);
}

void General2DSurfPane::OnFileButton(wxCommandEvent& event) {
	//Push out the current validated MultValue before we use it
	SetMultValue();

	mTarget->ReadGrid(squareValues, useMultValue, MultValue);
	UpdateTest = true;
	refreshControls();
	setUpdateButton();
}

void General2DSurfPane::OnMultCheck(wxCommandEvent &event) {
	useMultValue = mMultCheck->GetValue();
	setUpdateButton();
}
void General2DSurfPane::OnSquareCheck(wxCommandEvent &event) {
	squareValues = mSquareCheck->GetValue();
	setUpdateButton();
}
void General2DSurfPane::OnMultValueEnter(wxCommandEvent& event) {
	double newVal=0.0;
	wxString temp = mGenMultValue->GetValue();

	if (temp.ToDouble(&newVal)) {
		MultValue = newVal;
	}
	setUpdateButton();
}

/*!
 * Get bitmap resources
 */

wxBitmap General2DSurfPane::GetBitmapResource(const wxString& name) {
	// Bitmap retrieval
	////@begin Orbital3D bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
	////@end Orbital3D bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon General2DSurfPane::GetIconResource(const wxString& name) {
	// Icon retrieval
	////@begin Orbital3D icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
	////@end Orbital3D icon retrieval
}

/*!
 * TEDensity2DSurfPane class
 */

TEDensity2DSurfPane::TEDensity2DSurfPane(wxWindow* parent, TEDensity2DSurface* target, 
		SurfacesWindow* o, wxWindowID id,
		const wxPoint& pos, const wxSize& size, 
		long style) 
: Surface2DPane(parent, target, o, id, pos, size, style) {
	mTarget = target;

	TargetToPane();
	CreateControls();
	BuildOrbSetPopup();
	refreshControls();
}

TEDensity2DSurfPane::~TEDensity2DSurfPane() {

}

void BaseSurfacePane::BuildOrbSetPopup(void) {
	MoleculeData * mData = owner->GetMoleculeData();
	Frame * lFrame = mData->GetCurrentFramePtr();
	const vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();

	short item, numitems;

	item = 0; numitems = 0;

	if (Orbs->size() > 0) {
		std::vector<OrbitalRec *>::const_iterator OrbSet = Orbs->begin();
		unsigned long	OrbSetCount = 0;
		while (OrbSet != Orbs->end()) {
			if ((*OrbSet)->TotalDensityPossible()) {
				mOrbSetChoice->Append(wxString((*OrbSet)->getOrbitalTypeText(), wxConvUTF8));
			}
			if (TargetOrbSet < 0) {
				if (((*OrbSet)->getOrbitalType() != GuessOrbital) ||
						((OrbSetCount+1)>=Orbs->size()))
					TargetOrbSet = numitems;	//default to the first valid set
			}
			if (TargetOrbSet == OrbSetCount) item = numitems+1;
			numitems++;
			OrbSetCount++;
			OrbSet++;
		}
	}
}
void BaseSurfacePane::OnOrbSetChoice(wxCommandEvent &event) {
	TargetOrbSet = mOrbSetChoice->GetSelection();
	setUpdateButton();
}	

TEDensity1DSurfPane::TEDensity1DSurfPane(wxWindow* parent, TEDensity1DSurface* target, 
		SurfacesWindow* o, wxWindowID id,
		const wxPoint& pos, const wxSize& size, 
		long style) 
	: Surface1DPane(parent, target, o, id, pos, size, style) {
	mTarget = target;

	TargetToPane();
	CreateControls();
	BuildOrbSetPopup();
	refreshControls();
}

TEDensity1DSurfPane::~TEDensity1DSurfPane() {

}

void TEDensity1DSurfPane::CreateControls() {
	TEDensity1DSurfPane * TED1DPANEL = this;	//this is here just to match the code below from DialogBlocks

	wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer68, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText69 = new wxStaticText(TED1DPANEL, wxID_STATIC, _("Select Orbital Set:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer68->Add(itemStaticText69, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxString* mOrbSetChoiceStrings = NULL;
	mOrbSetChoice = new wxChoice(TED1DPANEL, ID_ORB_CHOICE, wxDefaultPosition, wxDefaultSize, 0, mOrbSetChoiceStrings, 0);
	if (ShowToolTips())
		mOrbSetChoice->SetToolTip(_("Choose the set of vectors to use for producing the TED surface."));
	itemBoxSizer68->Add(mOrbSetChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer71 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer71, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText72 = new wxStaticText(TED1DPANEL, wxID_STATIC, _("Number of grid points:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer71->Add(itemStaticText72, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mNumGridPntSld = new wxSlider(TED1DPANEL, ID_GRID_POINT_SLIDER, 0, 0, 100, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_LABELS);
	itemBoxSizer71->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer81;
	/* itemBoxSizer81 = new wxBoxSizer(wxHORIZONTAL); */
	/* mainSizer->Add(itemBoxSizer81, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5); */
	/* wxStaticText* itemStaticText82 = new wxStaticText(TED1DPANEL, wxID_STATIC, _("Contour color:"), wxDefaultPosition, wxDefaultSize, 0); */
	/* itemBoxSizer81->Add(itemStaticText82, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5); */

	/* mOrbColor1 = new colorArea(TED1DPANEL, ID_2D_COLOR_POSITIVE, &PosColor); */
	/* itemBoxSizer81->Add(mOrbColor1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5); */

	itemBoxSizer81 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer81, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	itemBoxSizer81->Add(new wxStaticText(TED1DPANEL, wxID_STATIC, _("Clamp Value")));
	clamp_box = new wxTextCtrl(TED1DPANEL, ID_3D_MAX_MAP_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	itemBoxSizer81->Add(clamp_box, 0, wxALIGN_CENTER_HORIZONTAL);

	itemBoxSizer81 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer81, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	itemBoxSizer81->Add(new wxStaticText(TED1DPANEL, wxID_STATIC, _("Scale Factor")));
	scale_box = new wxTextCtrl(TED1DPANEL, ID_SCALE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	itemBoxSizer81->Add(scale_box, 0, wxALIGN_CENTER_HORIZONTAL);

	wxFlexGridSizer* flex_sizer = new wxFlexGridSizer(2, 4, 0, 0);
	mainSizer->Add(flex_sizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	flex_sizer->Add(new wxStaticText(TED1DPANEL, wxID_STATIC, _("Endpoint 1:")),
					wxSizerFlags().Align(wxALIGN_CENTER_VERTICAL));
	endpt1_xbox = new wxTextCtrl(TED1DPANEL, ID_ENDPT1_XBOX, wxT(""),
								 /* wxString::Format("%f", mTarget->Start.x), */
								 wxDefaultPosition, wxDefaultSize,
								 wxTE_PROCESS_ENTER,
								 wxTextValidator(wxFILTER_NUMERIC,
												 &endpt1_xstr));
	flex_sizer->Add(endpt1_xbox);
	endpt1_ybox = new wxTextCtrl(TED1DPANEL, ID_ENDPT1_YBOX, wxT(""),
								 /* wxString::Format("%f", mTarget->Start.y), */
								 wxDefaultPosition, wxDefaultSize,
								 wxTE_PROCESS_ENTER,
								 wxTextValidator(wxFILTER_NUMERIC,
												 &endpt1_ystr));
	flex_sizer->Add(endpt1_ybox);
	endpt1_zbox = new wxTextCtrl(TED1DPANEL, ID_ENDPT1_ZBOX, wxT(""),
								 /* wxString::Format("%f", mTarget->Start.z), */
								 wxDefaultPosition, wxDefaultSize,
								 wxTE_PROCESS_ENTER,
								 wxTextValidator(wxFILTER_NUMERIC,
												 &endpt1_zstr));
	flex_sizer->Add(endpt1_zbox);
	flex_sizer->Add(new wxStaticText(TED1DPANEL, wxID_STATIC, _("Endpoint 2:")),
					wxSizerFlags().Align(wxALIGN_CENTER_VERTICAL));
	endpt2_xbox = new wxTextCtrl(TED1DPANEL, ID_ENDPT2_XBOX, wxT(""),
								 /* wxString::Format("%f", mTarget->End.x), */
								 wxDefaultPosition, wxDefaultSize,
								 wxTE_PROCESS_ENTER,
								 wxTextValidator(wxFILTER_NUMERIC,
												 &endpt2_xstr));
	flex_sizer->Add(endpt2_xbox);
	endpt2_ybox = new wxTextCtrl(TED1DPANEL, ID_ENDPT2_YBOX, wxT(""),
								 /* wxString::Format("%f", mTarget->End.y), */
								 wxDefaultPosition, wxDefaultSize,
								 wxTE_PROCESS_ENTER,
								 wxTextValidator(wxFILTER_NUMERIC,
												 &endpt2_ystr));
	flex_sizer->Add(endpt2_ybox);
	endpt2_zbox = new wxTextCtrl(TED1DPANEL, ID_ENDPT2_ZBOX, wxT(""),
								 /* wxString::Format("%f", mTarget->End.z), */
								 wxDefaultPosition, wxDefaultSize,
								 wxTE_PROCESS_ENTER,
								 wxTextValidator(wxFILTER_NUMERIC,
												 &endpt2_zstr));
	flex_sizer->Add(endpt2_zbox);

	wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer84, 0, wxALIGN_CENTER|wxALL, 5);
	wxBoxSizer* itemBoxSizer85 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer84->Add(itemBoxSizer85, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxBoxSizer* itemBoxSizer88 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer84->Add(itemBoxSizer88, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mExportBut = new wxButton(TED1DPANEL, ID_SURFACE_EXPORT_BUT, _("Export..."), wxDefaultPosition, wxDefaultSize, 0);

	if (mTarget->ExportPossible())
		mExportBut->Enable();
	else
		mExportBut->Disable();

	itemBoxSizer88->Add(mExportBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mUpdateBut = new wxButton(TED1DPANEL, ID_SURFACE_UPDATE_BUT, _("Update"), wxDefaultPosition, wxDefaultSize, 0);
	mUpdateBut->SetDefault();
	itemBoxSizer88->Add(mUpdateBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
}

void TEDensity1DSurfPane::TargetToPane(void) {
	TargetOrbSet = mTarget->getTargetOrbSet();
	NumGridPoints = mTarget->GetNumGridPoints();
	mTarget->GetPosColor(&PosColor);
	MaxContourValue = mTarget->GetMaxValue();
	Scale = mTarget->GetScale();
	Visible = mTarget->GetVisibility();
	AllFrames = (mTarget->GetSurfaceID() != 0);
	Start = mTarget->Start;
	End = mTarget->End;
	UpdateTest = false;
}

bool TEDensity1DSurfPane::UpdateNeeded(void) {
	bool result = UpdateTest;

	if (Visible != mTarget->GetVisibility()) result = true;
	if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
	if (MaxContourValue != mTarget->GetMaxValue()) result = true;
	if (Scale != mTarget->GetScale()) result = true;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) result = true;
	if (!result) {
		RGBColor	testColor;
		mTarget->GetPosColor(&testColor);
		if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)||
				(PosColor.blue!=testColor.blue)) result=true;
	}

#define CHECK_ENDPOINT(coordinate) \
	if (coordinate != mTarget->coordinate) { \
		result = true; \
	}

	CHECK_ENDPOINT(Start.x)
	CHECK_ENDPOINT(Start.y)
	CHECK_ENDPOINT(Start.z)
	CHECK_ENDPOINT(End.x)
	CHECK_ENDPOINT(End.y)
	CHECK_ENDPOINT(End.z)

#undef CHECK_ENDPOINT

	return result;
}

void TEDensity1DSurfPane::OnUpdate(wxCommandEvent &event) {

	bool updateGrid = UpdateTest;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) {
		mTarget->setTargetOrbSet(TargetOrbSet);
		updateGrid = true;
	}
	if (NumGridPoints != mTarget->GetNumGridPoints()) {
		mTarget->SetNumGridPoints(NumGridPoints);
		updateGrid = true;
	}

	mTarget->SetVisibility(Visible);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetMaxValue(MaxContourValue);
	mTarget->SetScale(Scale);

#define CHECK_ENDPOINT(coordinate) \
	if (coordinate != mTarget->coordinate) { \
		mTarget->coordinate = coordinate; \
		updateGrid = true; \
	}

	CHECK_ENDPOINT(Start.x)
	CHECK_ENDPOINT(Start.y)
	CHECK_ENDPOINT(Start.z)
	CHECK_ENDPOINT(End.x)
	CHECK_ENDPOINT(End.y)
	CHECK_ENDPOINT(End.z)

#undef CHECK_ENDPOINT

	MoleculeData * mData = owner->GetMoleculeData();
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) {	//update all frames
		long SurfaceID;
		Frame *	lFrame = mData->GetFirstFrame();
		if (AllFrames) {	//adding the surface to all frames
			SurfaceID = mTarget->SetSurfaceID();
			while (lFrame) {
				if (lFrame != mData->GetCurrentFramePtr()) {
					TEDensity1DSurface * NewSurface = new TEDensity1DSurface(mTarget);
					lFrame->AppendSurface(NewSurface);
				}
				lFrame = lFrame->GetNextFrame();
			}
		} else {			//deleting the surface from other frames
			SurfaceID = mTarget->GetSurfaceID();
			mTarget->SetSurfaceID(0);	//Unmark this frames surface so it doesn't get deleted
			while (lFrame) {
				lFrame->DeleteSurfaceWithID(SurfaceID);
				lFrame = lFrame->GetNextFrame();
			}
		}
	} else if (AllFrames) {
		long SurfaceID = mTarget->GetSurfaceID();
		Frame * lFrame = mData->GetFirstFrame();
		while (lFrame) {
			if (lFrame != mData->GetCurrentFramePtr()) {
				Surface * temp = lFrame->GetSurfaceWithID(SurfaceID);
				TEDensity1DSurface * lSurf = NULL;
				if (temp)
					if (temp->GetSurfaceType() == kTotalDensity1D)
						lSurf = (TEDensity1DSurface *) temp;
				if (lSurf) {
					lSurf->UpdateData(mTarget);
					if (updateGrid) lSurf->FreeGrid();
				}
			}
			lFrame = lFrame->GetNextFrame();
		}
	}	//if the grid needs updating, release the current grid to mark it for recalc.
	if (updateGrid) mTarget->FreeGrid();
	UpdateTest = false;
	setUpdateButton();
	mExportBut->Enable();
	owner->SurfaceUpdated();
}

void TEDensity1DSurfPane::refreshControls() {
	mOrbSetChoice->SetSelection(TargetOrbSet);
	/* mOrbColor1->setColor(&PosColor); */
	mNumGridPntSld->SetValue(NumGridPoints);

	clamp_box->SetValue(wxString::Format(wxT("%f"), mTarget->GetMaxValue()));
	scale_box->SetValue(wxString::Format(wxT("%f"), mTarget->GetScale()));
	endpt1_xbox->SetValue(wxString::Format(wxT("%f"), mTarget->Start.x));
	endpt1_ybox->SetValue(wxString::Format(wxT("%f"), mTarget->Start.y));
	endpt1_zbox->SetValue(wxString::Format(wxT("%f"), mTarget->Start.z));
	endpt2_xbox->SetValue(wxString::Format(wxT("%f"), mTarget->End.x));
	endpt2_ybox->SetValue(wxString::Format(wxT("%f"), mTarget->End.y));
	endpt2_zbox->SetValue(wxString::Format(wxT("%f"), mTarget->End.z));
}
/*!
 * Get bitmap resources
 */

wxBitmap TEDensity1DSurfPane::GetBitmapResource(const wxString& name) {
	// Bitmap retrieval
	////@begin Orbital3D bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
	////@end Orbital3D bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon TEDensity1DSurfPane::GetIconResource(const wxString& name) {
	// Icon retrieval
	////@begin Orbital3D icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
	////@end Orbital3D icon retrieval
}

void TEDensity2DSurfPane::CreateControls() {
	TEDensity2DSurfPane * TED2DPANEL = this;	//this is here just to match the code below from DialogBlocks

	wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer68, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText69 = new wxStaticText(TED2DPANEL, wxID_STATIC, _("Select Orbital Set:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer68->Add(itemStaticText69, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxString* mOrbSetChoiceStrings = NULL;
	mOrbSetChoice = new wxChoice(TED2DPANEL, ID_ORB_CHOICE, wxDefaultPosition, wxDefaultSize, 0, mOrbSetChoiceStrings, 0);
	if (ShowToolTips())
		mOrbSetChoice->SetToolTip(_("Choose the set of vectors to use for producing the TED surface."));
	itemBoxSizer68->Add(mOrbSetChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer71 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer71, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText72 = new wxStaticText(TED2DPANEL, wxID_STATIC, _("Number of grid points:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer71->Add(itemStaticText72, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mNumGridPntSld = new wxSlider(TED2DPANEL, ID_GRID_POINT_SLIDER, 0, 0, 100, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_LABELS);
	itemBoxSizer71->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mNumGridPntSld->SetToolTip(_("Sets the resolution of the grid. Fewer points is faster while more points produces a high-resolution result."));

	wxBoxSizer* itemBoxSizer74 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer74, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText75 = new wxStaticText(TED2DPANEL, wxID_STATIC, _("Max. # of contours:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer74->Add(itemStaticText75, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mNumContourText = new wxTextCtrl(TED2DPANEL, ID_NUM_CONTOUR_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	itemBoxSizer74->Add(mNumContourText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mNumContourText->SetToolTip(_("Sets the number of contours between zero and the max value."));

	wxBoxSizer* itemBoxSizer77 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer77, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText78 = new wxStaticText(TED2DPANEL, wxID_STATIC, _("Max. contour value:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer77->Add(itemStaticText78, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mContourValText = new wxTextCtrl(TED2DPANEL, ID_CONTOUR_VALUE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	itemBoxSizer77->Add(mContourValText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mContourValText->SetToolTip(_("The maximum value for a contour. The contour spacing is this value divided by the number of contours."));

	mUsePlaneChk = new wxCheckBox(TED2DPANEL, ID_USE_PLANE_CHECKBOX, _("Use plane of screen"), wxDefaultPosition, wxDefaultSize, 0);
	mUsePlaneChk->SetValue(false);
	mainSizer->Add(mUsePlaneChk, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	mUsePlaneChk->SetToolTip(_("When checked the plotting plane is fixed to the plane of the screen and recomputed during rotation."));

	mDisplayPlaneCheck = new wxCheckBox(TED2DPANEL, ID_DISPLAY_PLANE_CHECKBOX, _("Display Plotting Plane"), wxDefaultPosition, wxDefaultSize, 0);
	mDisplayPlaneCheck->SetValue(false);
	if (ShowToolTips())
		mDisplayPlaneCheck->SetToolTip(_("Display a translucent plane with border to indicate the plotting plane."));
	mainSizer->Add(mDisplayPlaneCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer81 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer81, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText82 = new wxStaticText(TED2DPANEL, wxID_STATIC, _("Contour color:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer81->Add(itemStaticText82, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mOrbColor1 = new colorArea(TED2DPANEL, ID_2D_COLOR_POSITIVE, &PosColor);
	itemBoxSizer81->Add(mOrbColor1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer84, 0, wxALIGN_CENTER|wxALL, 5);
	wxBoxSizer* itemBoxSizer85 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer84->Add(itemBoxSizer85, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mSetParamBut = new wxButton(TED2DPANEL, ID_SET_PARAM_BUT, _("Parameters..."), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer85->Add(mSetParamBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxButton* lSetPlaneBut = new wxButton(TED2DPANEL, ID_SET_PLANE_BUT, _("Set Plane..."), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer85->Add(lSetPlaneBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer88 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer84->Add(itemBoxSizer88, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mExportBut = new wxButton(TED2DPANEL, ID_SURFACE_EXPORT_BUT, _("Export..."), wxDefaultPosition, wxDefaultSize, 0);

	if (mTarget->ExportPossible())
		mExportBut->Enable();
	else
		mExportBut->Disable();

	itemBoxSizer88->Add(mExportBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mUpdateBut = new wxButton(TED2DPANEL, ID_SURFACE_UPDATE_BUT, _("Update"), wxDefaultPosition, wxDefaultSize, 0);
	mUpdateBut->SetDefault();
	itemBoxSizer88->Add(mUpdateBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
}

void TEDensity2DSurfPane::TargetToPane(void) {
	TargetOrbSet = mTarget->getTargetOrbSet();
	NumGridPoints = mTarget->GetNumGridPoints();
	mTarget->GetPosColor(&PosColor);
	MaxContourValue = mTarget->GetMaxValue();
	NumContours = mTarget->GetNumContours();
	UseScreenPlane = mTarget->GetRotate2DMap();
	DisplayPlane = mTarget->ShowPlottingPlane();
	Visible = mTarget->GetVisibility();
	AllFrames = (mTarget->GetSurfaceID() != 0);
	UpdateTest = false;
}

bool TEDensity2DSurfPane::UpdateNeeded(void) {
	bool result = UpdateTest;

	if (Visible != mTarget->GetVisibility()) result = true;
	if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
	if (NumContours != mTarget->GetNumContours()) result = true;
	if (MaxContourValue != mTarget->GetMaxValue()) result = true;
	if (UseScreenPlane != mTarget->GetRotate2DMap()) result = true;
	if (DisplayPlane != mTarget->ShowPlottingPlane()) result = true;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) result = true;
	if (!result) {
		RGBColor	testColor;
		mTarget->GetPosColor(&testColor);
		if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)||
				(PosColor.blue!=testColor.blue)) result=true;
	}
	return result;
}

void TEDensity2DSurfPane::OnUpdate(wxCommandEvent &event) {
	SetNumContoursText();
	SetContourValueText();

	bool	updateGrid = UpdateTest;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) {
		mTarget->setTargetOrbSet(TargetOrbSet);
		updateGrid = true;
	}
	if (NumGridPoints != mTarget->GetNumGridPoints()) {
		mTarget->SetNumGridPoints(NumGridPoints);
		updateGrid = true;
	}
	mTarget->SetVisibility(Visible);
	mTarget->SetNumContours(NumContours);
	mTarget->SetMaxValue(MaxContourValue);
	mTarget->SetPosColor(&PosColor);
	if (UseScreenPlane && !mTarget->GetRotate2DMap()) updateGrid = true;
	mTarget->SetRotate2DMap(UseScreenPlane);
	mTarget->ShowPlottingPlane(DisplayPlane);

	MoleculeData * mData = owner->GetMoleculeData();
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) {	//update all frames
		long	SurfaceID;
		Frame *	lFrame = mData->GetFirstFrame();
		if (AllFrames) {	//adding the surface to all frames
			SurfaceID = mTarget->SetSurfaceID();
			while (lFrame) {
				if (lFrame != mData->GetCurrentFramePtr()) {
					TEDensity2DSurface * NewSurface = new TEDensity2DSurface(mTarget);
					lFrame->AppendSurface(NewSurface);
				}
				lFrame = lFrame->GetNextFrame();
			}
		} else {			//deleting the surface from other frames
			SurfaceID = mTarget->GetSurfaceID();
			mTarget->SetSurfaceID(0);	//Unmark this frames surface so it doesn't get deleted
			while (lFrame) {
				lFrame->DeleteSurfaceWithID(SurfaceID);
				lFrame = lFrame->GetNextFrame();
			}
		}
	} else if (AllFrames) {
		long SurfaceID = mTarget->GetSurfaceID();
		Frame * lFrame = mData->GetFirstFrame();
		while (lFrame) {
			if (lFrame != mData->GetCurrentFramePtr()) {
				Surface * temp = lFrame->GetSurfaceWithID(SurfaceID);
				TEDensity2DSurface * lSurf = NULL;
				if (temp)
					if (temp->GetSurfaceType() == kTotalDensity2D)
						lSurf = (TEDensity2DSurface *) temp;
				if (lSurf) {
					lSurf->UpdateData(mTarget);
					if (updateGrid) lSurf->FreeGrid();
				}
			}
			lFrame = lFrame->GetNextFrame();
		}
	}	//if the grid needs updating, release the current grid to mark it for recalc.
	if (updateGrid) mTarget->FreeGrid();
	UpdateTest = false;
	setUpdateButton();
	mExportBut->Enable();
	owner->SurfaceUpdated();
}

void TEDensity2DSurfPane::refreshControls() {
	mOrbSetChoice->SetSelection(TargetOrbSet);
	SetNumContoursText();
	SetContourValueText();

	mOrbColor1->setColor(&PosColor);

	mNumGridPntSld->SetValue(NumGridPoints);

	mUsePlaneChk->SetValue(UseScreenPlane);
	mDisplayPlaneCheck->SetValue(DisplayPlane);
}
/*!
 * Get bitmap resources
 */

wxBitmap TEDensity2DSurfPane::GetBitmapResource(const wxString& name) {
	// Bitmap retrieval
	////@begin Orbital3D bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
	////@end Orbital3D bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon TEDensity2DSurfPane::GetIconResource(const wxString& name) {
	// Icon retrieval
	////@begin Orbital3D icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
	////@end Orbital3D icon retrieval
}

/*!
 * TEDensity3DSurfPane class
 */

TEDensity3DSurfPane::TEDensity3DSurfPane(wxWindow* parent, TEDensity3DSurface* target, 
		SurfacesWindow* o, wxWindowID id,
		const wxPoint& pos, const wxSize& size, 
		long style) 
: Surface3DPane(parent, target, o, id, pos, size, style) {
	mTarget = target;

	TargetToPane();
	CreateControls();
	BuildOrbSetPopup();
	refreshControls();
}

TEDensity3DSurfPane::~TEDensity3DSurfPane() {

}

void TEDensity3DSurfPane::TargetToPane(void) {
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	Transparency = mTarget->GetTransparency();
	ContourValue = mTarget->GetContourValue();
	UseSolidSurface = mTarget->SolidSurface();
	UseNormals = mTarget->UseSurfaceNormals();
	NumGridPoints = mTarget->GetNumGridPoints();
	TargetOrbSet = mTarget->getTargetOrbSet();
	GridSize = mTarget->GetGridSize();
	Visible = mTarget->GetVisibility();
	AllFrames = (mTarget->GetSurfaceID() != 0);
	UseMEP = mTarget->ColorByValue();
	UseRGBSurfaceColor = mTarget->UseRGBColoration();
	InvertRGBSurfaceColor = mTarget->InvertRGBColoration();
	MaxMEPValue = mTarget->GetMaxSurfaceValue();
	UpdateTest = false;
}

void TEDensity3DSurfPane::refreshControls() {
	mOrbSetChoice->SetSelection(TargetOrbSet);
	SetContourMaxValueText();
	setContourValueSld();

	mOrbColor1->setColor(&PosColor);
	mOrbColor2->setColor(&NegColor);
	mTransparency->SetValue(Transparency);

	refreshMEPControls();
	mNumGridPntSld->SetValue(NumGridPoints);
	mGridSizeSld->SetValue((short)(100*GridSize));

	m3DRdoBox->SetSelection(1-UseSolidSurface);
	mSmoothChkBox->SetValue(UseNormals);

	if (UseSolidSurface)
		mSmoothChkBox->Enable();
	else
		mSmoothChkBox->Disable();
}
bool TEDensity3DSurfPane::UpdateNeeded(void) {
	bool result = UpdateTest;

	if (Visible != mTarget->GetVisibility()) result = true;
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) result = true;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) result = true;
	if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
	if (ContourValue != mTarget->GetContourValue()) result = true;
	if (GridSize != mTarget->GetGridSize()) result = true;
	if (UseSolidSurface != mTarget->SolidSurface()) result = true;
	if (UseMEP != mTarget->ColorByValue()) result = true;
	if (UseNormals != mTarget->UseSurfaceNormals()) result = true;
	if (UseRGBSurfaceColor != mTarget->UseRGBColoration()) result = true;
	if (InvertRGBSurfaceColor != mTarget->InvertRGBColoration()) result = true;
	if (MaxMEPValue != mTarget->GetMaxSurfaceValue()) result = true;
	if (!result) {
		RGBColor	testColor;
		mTarget->GetPosColor(&testColor);
		if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)||
				(PosColor.blue!=testColor.blue)) result=true;
		mTarget->GetNegColor(&testColor);
		if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)||
				(NegColor.blue!=testColor.blue)) result=true;
		unsigned char test_transp = mTarget->GetTransparency();
		if (test_transp != Transparency) 
			result=true;
	}
	return result;
}
void TEDensity3DSurfPane::OnUpdate(wxCommandEvent &event) {
	SetMaxMEPValueText();

	//only update the grid if needed
	bool updateGrid=UpdateTest, updateContour=false, updateMEP=false;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) {
		updateGrid = true;
		mTarget->SetFixGrid(false);
	}
	if (NumGridPoints != mTarget->GetNumGridPoints()) {
		updateGrid = true;
		mTarget->SetFixGrid(false);
	}
	if (ContourValue != mTarget->GetContourValue()) updateContour = true;
	if (GridSize != mTarget->GetGridSize()) {
		updateGrid = true;
		mTarget->SetFixGrid(false);
	}
	if (Visible && !mTarget->ContourAvail()) updateContour = true;
	if (UseMEP && !mTarget->ColorByValue()) updateMEP = true;
	if (updateMEP && !mTarget->ContourAvail()) updateContour = true;
	//test to see if grid/contour must be updated anyway...
	if (updateContour && ! mTarget->GridAvailable()) updateGrid = true;
	if (updateGrid) updateContour = true;
	if (updateContour && UseMEP) updateMEP = true;
	mTarget->SetVisibility(Visible);
	mTarget->SolidSurface(UseSolidSurface);
	mTarget->setTargetOrbSet(TargetOrbSet);
	mTarget->SetNumGridPoints(NumGridPoints);
	mTarget->SetContourValue(ContourValue);
	mTarget->SetMaxSurfaceValue(MaxMEPValue);
	mTarget->SetGridSize(GridSize);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetNegColor(&NegColor);
	mTarget->SetTransparency(Transparency);
	mTarget->SetColorByValue(UseMEP);
	mTarget->UseSurfaceNormals(UseNormals);
	mTarget->UseRGBColoration(UseRGBSurfaceColor);
	mTarget->InvertRGBColoration(InvertRGBSurfaceColor);
	MoleculeData * data = owner->GetMoleculeData();
	WinPrefs * Prefs = owner->GetPrefs();
	//update this surface's data on all frames if necessary
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) {	//update all frames
		long	SurfaceID;
		Frame *	lFrame = data->GetFirstFrame();
		updateGrid = updateContour = true;
		if (AllFrames) {	//adding the surface to all frames
			SurfaceID = mTarget->SetSurfaceID();
			while (lFrame) {
				if (lFrame != data->GetCurrentFramePtr()) {
					TEDensity3DSurface * NewSurface = new TEDensity3DSurface(Prefs);
					if (NewSurface) {
						NewSurface->SetSurfaceID(SurfaceID);
						lFrame->AppendSurface(NewSurface);
					}
				}
				lFrame = lFrame->GetNextFrame();
			}
		} else {			//deleting the surface from other frames
			SurfaceID = mTarget->GetSurfaceID();
			mTarget->SetSurfaceID(0);	//Unmark this frames surface so it doesn't get deleted
			while (lFrame) {
				lFrame->DeleteSurfaceWithID(SurfaceID);
				lFrame = lFrame->GetNextFrame();
			}
		}
	}
	Progress * lProgress = new Progress();
	//	if (!lProgress) throw MemoryError();
	if (AllFrames) {	//compute the contour for each frame, no grid is kept
		long SurfaceID = mTarget->GetSurfaceID();
		long CurrentFrame = data->GetCurrentFrame();
		long NumFrames = data->GetNumFrames();
		for (int i=0; i<NumFrames; i++) {
			TEDensity3DSurface * lSurf;
			lSurf = NULL;
			data->SetCurrentFrame(i+1);
			Frame * lFrame = data->GetCurrentFramePtr();
			if (CurrentFrame != data->GetCurrentFrame()) {
				Surface * temp = lFrame->GetSurfaceWithID(SurfaceID);
				//Confirm that the surface is the correct type
				if (temp)
					if (temp->GetSurfaceType() == kTotalDensity3D)
						lSurf = (TEDensity3DSurface *) temp;
				if (lSurf) lSurf->UpdateData(mTarget);
			} else lSurf = mTarget;
			if (lSurf) {
				if (!UseMEP) mTarget->FreeList();
				if (Visible) {
					float MEPScale = 1.0;
					if (updateMEP) MEPScale = 0.5;
					lProgress->ChangeText("Calculating 3D Grid...");
					lProgress->SetBaseValue((long)(100*i*MEPScale/NumFrames));
					lProgress->SetScaleFactor((float) 0.9*MEPScale/NumFrames);
					if (updateGrid) lSurf->CalculateMOGrid(data, lProgress);
					lProgress->ChangeText("Contouring grid...");
					lProgress->SetBaseValue((long)(100*i/NumFrames + 90.0*MEPScale/NumFrames));
					lProgress->SetScaleFactor((float) 0.1*MEPScale/NumFrames);
					if (updateContour) lSurf->Contour3DGrid(lProgress);
					if (updateMEP && data->MEPCalculationPossible()) {
						lProgress->ChangeText("Calculating MEP values...");
						lProgress->SetBaseValue(100*i/NumFrames + 50/NumFrames);
						lProgress->SetScaleFactor(0.5/NumFrames);
						owner->GetParent()->Freeze();
						mTarget->CalculateSurfaceValues(data, lProgress);
						owner->GetParent()->Thaw();
					}
					lSurf->FreeGrid();
				} else {
					if (updateContour) lSurf->FreeContour();
					if (updateMEP) lSurf->FreeList();
				}
			}
		}
		data->SetCurrentFrame(CurrentFrame);
	} else {	//simply update this surface
		if (!UseMEP) mTarget->FreeList();
		if (Visible) {
			try {
				float MEPScale = 1.0;
				if (updateMEP) MEPScale = 0.5;
				if (updateGrid) {
					lProgress->ChangeText("Calculating 3D Grid...");
					lProgress->SetScaleFactor(0.9*MEPScale);
					mTarget->CalculateMOGrid(data, lProgress);
				}
				if (updateContour) {
					lProgress->ChangeText("Contouring grid...");
					lProgress->SetBaseValue((long)(90*MEPScale));
					lProgress->SetScaleFactor(0.1*MEPScale);
					mTarget->Contour3DGrid(lProgress);
				}
				if (updateMEP && data->MEPCalculationPossible()) {
					lProgress->ChangeText("Calculating MEP values...");
					lProgress->SetBaseValue(50);
					lProgress->SetScaleFactor(0.5);
					owner->GetParent()->Freeze();
					mTarget->CalculateSurfaceValues(data, lProgress);
					owner->GetParent()->Thaw();
				}
			}
			catch (bad_alloc & ba) {
				MessageAlert("Error: Unable to allocate enough memory to update surface.");
			}
			catch (...) {
				MessageAlert("Error: unknown exception occured while attempting to update surface.");
			}
		} else {
			//always free the grid for invisable surfaces since they can be big
			mTarget->FreeGrid();
			//free the contour so that it will be updated when it is made visible
			if (updateContour) mTarget->FreeContour();
			if (updateMEP) mTarget->FreeList();
		}
	}
	if (lProgress) delete lProgress;

	if (mTarget->GridAvailable())
		mFreeMemBut->Enable();
	else
		mFreeMemBut->Disable();

	SetContourMaxValueText();
	setContourValueSld();
	UpdateTest = false;
	setUpdateButton();

	if (mTarget->ExportPossible())
		mExportBut->Enable();
	else
		mExportBut->Disable();

	owner->SurfaceUpdated();
}
void TEDensity3DSurfPane::CreateControls() {
	TEDensity3DSurfPane * TED3DPanel = this;	//this is here just to match the code below from DialogBlocks

	wxBoxSizer* itemBoxSizer93 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer93, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText94 = new wxStaticText(TED3DPanel, wxID_STATIC, _("Select Orbital Set:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer93->Add(itemStaticText94, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxString* mOrbSetChoiceStrings = NULL;
	mOrbSetChoice = new wxChoice(TED3DPanel, ID_ORB_CHOICE, wxDefaultPosition, wxDefaultSize, 0, mOrbSetChoiceStrings, 0);
	if (ShowToolTips())
		mOrbSetChoice->SetToolTip(_("Choose the set of vectors to use for producing the TED surface."));
	itemBoxSizer93->Add(mOrbSetChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer96 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer96, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
	wxStaticText* itemStaticText97 = new wxStaticText(TED3DPanel, wxID_STATIC, _("Number of grid points:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer96->Add(itemStaticText97, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mNumGridPntSld = new wxSlider(TED3DPanel, ID_GRID_POINT_SLIDER, 0, 10, 150, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_LABELS);
	itemBoxSizer96->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mNumGridPntSld->SetToolTip(_("Sets the resolution of the grid. Fewer points is faster while more points produces a high-resolution result."));

	wxBoxSizer* itemBoxSizer99 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer99, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
	wxStaticText* itemStaticText100 = new wxStaticText(TED3DPanel, wxID_STATIC, _("Grid Size:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer99->Add(itemStaticText100, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mGridSizeSld = new wxSlider(TED3DPanel, ID_GRID_SIZE_SLIDER, 0, 1, 300, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_HORIZONTAL);
	itemBoxSizer99->Add(mGridSizeSld, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mGridSizeSld->SetToolTip(_("Adjusts the volume over which the grid is computed. Adjust to the right if your surface appears clipped."));

	wxBoxSizer* itemBoxSizer102 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer102, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
	wxBoxSizer* itemBoxSizer103 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer102->Add(itemBoxSizer103, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText* itemStaticText104 = new wxStaticText(TED3DPanel, wxID_STATIC, _("Contour Value:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer103->Add(itemStaticText104, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mContourValSld = new FloatSlider(TED3DPanel, ID_CONTOUR_VALUE_SLIDER, 0.0f, 0.0f, 100.0f, FloatSlider::POW, wxSize(CONTOUR_WIDTH, wxDefaultCoord));
	itemBoxSizer102->Add(mContourValSld, 0, wxFIXED_MINSIZE | wxALIGN_CENTER_VERTICAL|wxALL, 3);
	mContourValSld->SetToolTip(_("Sets the value for the isosurface. Move to the right to produce a surface closer to the nucleus."));

	wxStaticBoxSizer *mep_sizer = new wxStaticBoxSizer(wxHORIZONTAL, TED3DPanel, wxT("MEP"));
	mainSizer->Add(mep_sizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	wxBoxSizer* itemBoxSizer112 = new wxBoxSizer(wxVERTICAL);
	mep_sizer->Add(itemBoxSizer112, 0);
	mColorSurfCheck = new wxCheckBox(TED3DPanel, ID_3D_COLOR_SURF_CHECK, _("Colorize using the surface MEP value"), wxDefaultPosition, wxDefaultSize, 0);
	mColorSurfCheck->SetValue(false);
	itemBoxSizer112->Add(mColorSurfCheck, 0);

	mUseRGBColorCheck = new wxCheckBox(TED3DPanel, ID_USERGB_COLOR_CHECK, _("Use RGB surface coloration"), wxDefaultPosition, wxDefaultSize, 0);
	if (ShowToolTips())
		mUseRGBColorCheck->SetToolTip(_("Uses blue for attractive, green for neutral, and red of repulsive to a + charge"));
	mUseRGBColorCheck->SetValue(false);
	itemBoxSizer112->Add(mUseRGBColorCheck, 0);

	mInvertRGBCheck = new wxCheckBox(TED3DPanel, ID_INVERT_RGB_CHECK, _("Invert color map"), wxDefaultPosition, wxDefaultSize, 0);
	mInvertRGBCheck->SetValue(false);
	if (ShowToolTips())
		mInvertRGBCheck->SetToolTip(_("Flips the RGB mapping so that red is attractive, blue replusive to a + charge"));
	itemBoxSizer112->Add(mInvertRGBCheck, 0);

	mep_sizer->AddSpacer(10);

	wxBoxSizer* itemBoxSizer115 = new wxBoxSizer(wxVERTICAL);
	mep_sizer->Add(itemBoxSizer115, 0, wxALIGN_CENTER_VERTICAL);
	wxStaticText* itemStaticText116 = new wxStaticText(TED3DPanel, wxID_STATIC, _("Max. value to map"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer115->Add(itemStaticText116, 0);

	mMaxMapEdit = new wxTextCtrl(TED3DPanel, ID_3D_MAX_MAP_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer115->Add(mMaxMapEdit, 0, wxALIGN_CENTER_HORIZONTAL);

	wxBoxSizer* itemBoxSizer118 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer118, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
	wxBoxSizer* itemBoxSizer119 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer118->Add(itemBoxSizer119, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxBoxSizer* itemBoxSizer120 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer119->Add(itemBoxSizer120, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	wxStaticText* itemStaticText121 = new wxStaticText(TED3DPanel, wxID_STATIC, _("Surface Color:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer120->Add(itemStaticText121, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mOrbColor1 = new colorArea(TED3DPanel, ID_3D_COLOR_POSITIVE, &PosColor);
	itemBoxSizer120->Add(mOrbColor1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mOrbColor2 = new colorArea(TED3DPanel, ID_3D_COLOR_NEGATIVE, &NegColor);
	itemBoxSizer120->Add(mOrbColor2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer124 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer119->Add(itemBoxSizer124, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	wxStaticText* itemStaticText125 = new wxStaticText(TED3DPanel, wxID_STATIC, _("Transparency:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer124->Add(itemStaticText125, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mTransparency = new wxSpinCtrl(this, ID_TRANSPARENCY, _T(""),
			wxDefaultPosition, wxDefaultSize,
			wxSP_ARROW_KEYS, 0, 100, 0);
	mTransparency->SetValue(Transparency);
	itemBoxSizer124->Add(mTransparency, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer127 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer119->Add(itemBoxSizer127, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	wxString m3DRdoBoxStrings[] = {
		_("&Solid"),
		_("&WireFrame")
	};
	m3DRdoBox = new wxRadioBox(TED3DPanel, ID_3D_RADIOBOX, _("Surface Display"), wxDefaultPosition, wxDefaultSize, 2, m3DRdoBoxStrings, 1, wxRA_SPECIFY_ROWS);
	m3DRdoBox->SetSelection(0);
	itemBoxSizer127->Add(m3DRdoBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mSmoothChkBox = new wxCheckBox(TED3DPanel, ID_SMOOTH_CHECKBOX, _("Smooth"), wxDefaultPosition, wxDefaultSize, 0);
	mSmoothChkBox->SetValue(false);
	if (ShowToolTips())
		mSmoothChkBox->SetToolTip(_("Check to use surface normals to smooth out the surface of the contour."));
	itemBoxSizer127->Add(mSmoothChkBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer130 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer118->Add(itemBoxSizer130, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mSetParamBut = new wxButton(TED3DPanel, ID_SET_PARAM_BUT, _("Parameters..."), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer130->Add(mSetParamBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mFreeMemBut = new wxButton(TED3DPanel, ID_FREE_MEM_BUT, _("Free Mem"), wxDefaultPosition, wxDefaultSize, 0);

	if (mTarget->GridAvailable())
		mFreeMemBut->Enable();
	else
		mFreeMemBut->Disable();

	if (ShowToolTips())
		mFreeMemBut->SetToolTip(_("Click to free the memory used by the 3D grid."));
	itemBoxSizer130->Add(mFreeMemBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mExportBut = new wxButton(TED3DPanel, ID_SURFACE_EXPORT_BUT, _("Export..."), wxDefaultPosition, wxDefaultSize, 0);

	if (mTarget->ExportPossible())
		mExportBut->Enable();
	else
		mExportBut->Disable();

	itemBoxSizer130->Add(mExportBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mUpdateBut = new wxButton(TED3DPanel, ID_SURFACE_UPDATE_BUT, _("Update"), wxDefaultPosition, wxDefaultSize, 0);
	mUpdateBut->SetDefault();
	itemBoxSizer130->Add(mUpdateBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
}
/*!
 * Get bitmap resources
 */

wxBitmap TEDensity3DSurfPane::GetBitmapResource(const wxString& name) {
	// Bitmap retrieval
	////@begin Orbital3D bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
	////@end Orbital3D bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon TEDensity3DSurfPane::GetIconResource(const wxString& name) {
	// Icon retrieval
	////@begin Orbital3D icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
	////@end Orbital3D icon retrieval
}
/*!
 * TEDensity2DSurfPane class
 */

MEP2DSurfPane::MEP2DSurfPane(wxWindow* parent, MEP2DSurface* target, 
		SurfacesWindow* o, wxWindowID id,
		const wxPoint& pos, const wxSize& size, 
		long style) 
: Surface2DPane(parent, target, o, id, pos, size, style) {
	mTarget = target;

	TargetToPane();
	CreateControls();
	BuildOrbSetPopup();
	refreshControls();
}

MEP2DSurfPane::~MEP2DSurfPane() {

}
void MEP2DSurfPane::CreateControls() {
	MEP2DSurfPane * MEP2DPanel = this;	//this is here just to match the code below from DialogBlocks

	wxBoxSizer* itemBoxSizer137 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer137, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText138 = new wxStaticText(MEP2DPanel, wxID_STATIC, _("Select Orbital Set:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer137->Add(itemStaticText138, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxString* mOrbSetChoiceStrings = NULL;
	mOrbSetChoice = new wxChoice(MEP2DPanel, ID_ORB_CHOICE, wxDefaultPosition, wxDefaultSize, 0, mOrbSetChoiceStrings, 0);
	if (ShowToolTips())
		mOrbSetChoice->SetToolTip(_("Choose the set of vectors to use for producing the MEP surface."));
	itemBoxSizer137->Add(mOrbSetChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer140 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer140, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText141 = new wxStaticText(MEP2DPanel, wxID_STATIC, _("Number of grid points:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer140->Add(itemStaticText141, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mNumGridPntSld = new wxSlider(MEP2DPanel, ID_GRID_POINT_SLIDER, 0, 0, 100, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_LABELS);
	itemBoxSizer140->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mNumGridPntSld->SetToolTip(_("Sets the resolution of the grid. Fewer points is faster while more points produces a high-resolution result."));

	wxBoxSizer* itemBoxSizer143 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer143, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText144 = new wxStaticText(MEP2DPanel, wxID_STATIC, _("Max. # of contours:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer143->Add(itemStaticText144, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mNumContourText = new wxTextCtrl(MEP2DPanel, ID_NUM_CONTOUR_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	itemBoxSizer143->Add(mNumContourText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mNumContourText->SetToolTip(_("Sets the number of contours between zero and the max value."));

	wxBoxSizer* itemBoxSizer146 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer146, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText147 = new wxStaticText(MEP2DPanel, wxID_STATIC, _("Max. contour value:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer146->Add(itemStaticText147, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mContourValText = new wxTextCtrl(MEP2DPanel, ID_CONTOUR_VALUE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	itemBoxSizer146->Add(mContourValText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mContourValText->SetToolTip(_("The maximum value for a contour. The contour spacing is this value divided by the number of contours."));

	mUsePlaneChk = new wxCheckBox(MEP2DPanel, ID_USE_PLANE_CHECKBOX, _("Use plane of screen"), wxDefaultPosition, wxDefaultSize, 0);
	mUsePlaneChk->SetValue(false);
	mainSizer->Add(mUsePlaneChk, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mDisplayPlaneCheck = new wxCheckBox(MEP2DPanel, ID_DISPLAY_PLANE_CHECKBOX, _("Display Plotting Plane"), wxDefaultPosition, wxDefaultSize, 0);
	mDisplayPlaneCheck->SetValue(false);
	if (ShowToolTips())
		mDisplayPlaneCheck->SetToolTip(_("Display a translucent plane with border to indicate the plotting plane"));
	mainSizer->Add(mDisplayPlaneCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mShowZeroCheck = new wxCheckBox(MEP2DPanel, ID_SHOW_ZERO_CHECKBOX, _("Show zero value contour"), wxDefaultPosition, wxDefaultSize, 0);
	mShowZeroCheck->SetValue(false);
	if (ShowToolTips())
		mShowZeroCheck->SetToolTip(_("Check to produce a gray contour where the plane changes sign."));
	mainSizer->Add(mShowZeroCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mDashCheck = new wxCheckBox(MEP2DPanel, ID_DASH_CHECKBOX, _("Dash negative contours"), wxDefaultPosition, wxDefaultSize, 0);
	mDashCheck->SetValue(false);
	if (ShowToolTips())
		mDashCheck->SetToolTip(_("Produces dashed negative contour lines for easier viewing in B & W."));
	mainSizer->Add(mDashCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer152 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer152, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	wxBoxSizer* itemBoxSizer153 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer152->Add(itemBoxSizer153, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	wxStaticText* itemStaticText154 = new wxStaticText(MEP2DPanel, wxID_STATIC, _("Positive Color:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer153->Add(itemStaticText154, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mOrbColor1 = new colorArea(MEP2DPanel, ID_2D_COLOR_POSITIVE, &PosColor);
	itemBoxSizer153->Add(mOrbColor1, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer156 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer152->Add(itemBoxSizer156, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
	wxStaticText* itemStaticText157 = new wxStaticText(MEP2DPanel, wxID_STATIC, _("Negative Color:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer156->Add(itemStaticText157, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mOrbColor2 = new colorArea(MEP2DPanel, ID_2D_COLOR_NEGATIVE, &NegColor);
	itemBoxSizer156->Add(mOrbColor2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer159 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer159, 0, wxALIGN_CENTER|wxALL, 5);
	wxBoxSizer* itemBoxSizer160 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer159->Add(itemBoxSizer160, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mSetParamBut = new wxButton(MEP2DPanel, ID_SET_PARAM_BUT, _("Parameters..."), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer160->Add(mSetParamBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxButton* lSetPlaneBut = new wxButton(MEP2DPanel, ID_SET_PLANE_BUT, _("Set Plane..."), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer160->Add(lSetPlaneBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer163 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer159->Add(itemBoxSizer163, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mExportBut = new wxButton(MEP2DPanel, ID_SURFACE_EXPORT_BUT, _("Export..."), wxDefaultPosition, wxDefaultSize, 0);

	if (mTarget->ExportPossible())
		mExportBut->Enable();
	else
		mExportBut->Disable();

	itemBoxSizer163->Add(mExportBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mUpdateBut = new wxButton(MEP2DPanel, ID_SURFACE_UPDATE_BUT, _("Update"), wxDefaultPosition, wxDefaultSize, 0);
	mUpdateBut->SetDefault();
	itemBoxSizer163->Add(mUpdateBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
}
void MEP2DSurfPane::TargetToPane(void) {
	TargetOrbSet = mTarget->getTargetOrbSet();
	NumGridPoints = mTarget->GetNumGridPoints();
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	MaxContourValue = mTarget->GetMaxValue();
	NumContours = mTarget->GetNumContours();
	UseScreenPlane = mTarget->GetRotate2DMap();
	DashLines = mTarget->GetDashLine();
	ShowZeroContour = mTarget->GetShowZeroContour();
	DisplayPlane = mTarget->ShowPlottingPlane();
	Visible = mTarget->GetVisibility();
	AllFrames = (mTarget->GetSurfaceID() != 0);
	UpdateTest = false;
}
bool MEP2DSurfPane::UpdateNeeded(void) {
	bool result = UpdateTest;

	if (Visible != mTarget->GetVisibility()) result = true;
	if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
	if (NumContours != mTarget->GetNumContours()) result = true;
	if (MaxContourValue != mTarget->GetMaxValue()) result = true;
	if (UseScreenPlane != mTarget->GetRotate2DMap()) result = true;
	if (ShowZeroContour != mTarget->GetShowZeroContour()) result = true;
	if (DisplayPlane != mTarget->ShowPlottingPlane()) result = true;
	if (DashLines != mTarget->GetDashLine()) result = true;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) result = true;
	if (!result) {
		RGBColor	testColor;
		mTarget->GetPosColor(&testColor);
		if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)||
				(PosColor.blue!=testColor.blue)) result=true;
		mTarget->GetNegColor(&testColor);
		if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)||
				(NegColor.blue!=testColor.blue)) result=true;
	}
	return result;
}
void MEP2DSurfPane::OnUpdate(wxCommandEvent &event) {
	SetNumContoursText();
	SetContourValueText();

	bool	updateGrid = UpdateTest;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) {
		mTarget->setTargetOrbSet(TargetOrbSet);
		updateGrid = true;
	}
	if (NumGridPoints != mTarget->GetNumGridPoints()) {
		mTarget->SetNumGridPoints(NumGridPoints);
		updateGrid = true;
	}
	mTarget->SetVisibility(Visible);
	mTarget->SetNumContours(NumContours);
	mTarget->SetMaxValue(MaxContourValue);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetNegColor(&NegColor);
	mTarget->SetShowZeroContour(ShowZeroContour);
	mTarget->SetDashLine(DashLines);
	mTarget->ShowPlottingPlane(DisplayPlane);
	if (UseScreenPlane && !mTarget->GetRotate2DMap()) updateGrid = true;
	mTarget->SetRotate2DMap(UseScreenPlane);

	MoleculeData * mData = owner->GetMoleculeData();
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) {	//update all frames
		long	SurfaceID;
		Frame *	lFrame = mData->GetFirstFrame();
		if (AllFrames) {	//adding the surface to all frames
			SurfaceID = mTarget->SetSurfaceID();
			while (lFrame) {
				if (lFrame != mData->GetCurrentFramePtr()) {
					try {
						MEP2DSurface * NewSurface = new MEP2DSurface(mTarget);
						lFrame->AppendSurface(NewSurface);
					} catch (...) {
					}
				}
				lFrame = lFrame->GetNextFrame();
			}
		} else {			//deleting the surface from other frames
			SurfaceID = mTarget->GetSurfaceID();
			mTarget->SetSurfaceID(0);	//Unmark this frames surface so it doesn't get deleted
			while (lFrame) {
				lFrame->DeleteSurfaceWithID(SurfaceID);
				lFrame = lFrame->GetNextFrame();
			}
		}
	} else if (AllFrames) {
		long SurfaceID = mTarget->GetSurfaceID();
		Frame * lFrame = mData->GetFirstFrame();
		while (lFrame) {
			if (lFrame != mData->GetCurrentFramePtr()) {
				Surface * temp = lFrame->GetSurfaceWithID(SurfaceID);
				MEP2DSurface * lSurf = NULL;
				if (temp)
					if (temp->GetSurfaceType() == kMEP2D)
						lSurf = (MEP2DSurface *) temp;
				if (lSurf) {
					lSurf->UpdateData(mTarget);
					if (updateGrid) lSurf->FreeGrid();
				}
			}
			lFrame = lFrame->GetNextFrame();
		}
	}
	if (updateGrid) mTarget->FreeGrid();
	UpdateTest = false;
	setUpdateButton();
	mExportBut->Enable();
	owner->SurfaceUpdated();
}
void MEP2DSurfPane::refreshControls() {
	mOrbSetChoice->SetSelection(TargetOrbSet);
	SetNumContoursText();
	SetContourValueText();

	mOrbColor1->setColor(&PosColor);
	mOrbColor2->setColor(&NegColor);

	mNumGridPntSld->SetValue(NumGridPoints);

	mUsePlaneChk->SetValue(UseScreenPlane);
	mShowZeroCheck->SetValue(ShowZeroContour);
	mDashCheck->SetValue(DashLines);
	mDisplayPlaneCheck->SetValue(DisplayPlane);
}
/*!
 * Get bitmap resources
 */

wxBitmap MEP2DSurfPane::GetBitmapResource(const wxString& name) {
	// Bitmap retrieval
	////@begin bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
	////@end bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon MEP2DSurfPane::GetIconResource(const wxString& name) {
	// Icon retrieval
	////@begin icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
	////@end icon retrieval
}

/*!
 * MEP3DSurfPane class
 */

MEP3DSurfPane::MEP3DSurfPane(wxWindow* parent, MEP3DSurface* target, 
		SurfacesWindow* o, wxWindowID id,
		const wxPoint& pos, const wxSize& size, 
		long style) 
: Surface3DPane(parent, target, o, id, pos, size, style) {
	mTarget = target;

	TargetToPane();
	CreateControls();
	BuildOrbSetPopup();
	refreshControls();
}

MEP3DSurfPane::~MEP3DSurfPane() {

}
void MEP3DSurfPane::TargetToPane(void) {
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	Transparency = mTarget->GetTransparency();
	ContourValue = mTarget->GetContourValue();
	UseSolidSurface = mTarget->SolidSurface();
	UseNormals = mTarget->UseSurfaceNormals();
	NumGridPoints = mTarget->GetNumGridPoints();
	TargetOrbSet = mTarget->getTargetOrbSet();
	GridSize = mTarget->GetGridSize();
	Visible = mTarget->GetVisibility();
	AllFrames = (mTarget->GetSurfaceID() != 0);
	UpdateTest = false;
}
void MEP3DSurfPane::refreshControls() {
	mOrbSetChoice->SetSelection(TargetOrbSet);
	SetContourMaxValueText();
	setContourValueSld();

	mOrbColor1->setColor(&PosColor);
	mOrbColor2->setColor(&NegColor);
	mTransparency->SetValue(Transparency);

	mNumGridPntSld->SetValue(NumGridPoints);
	mGridSizeSld->SetValue((short)(100*GridSize));

	m3DRdoBox->SetSelection(1-UseSolidSurface);
	mSmoothChkBox->SetValue(UseNormals);

	if (UseSolidSurface)
		mSmoothChkBox->Enable();
	else
		mSmoothChkBox->Disable();
}
bool MEP3DSurfPane::UpdateNeeded(void) {
	bool result = UpdateTest;

	if (Visible != mTarget->GetVisibility()) result = true;
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) result = true;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) result = true;
	if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
	if (ContourValue != mTarget->GetContourValue()) result = true;
	if (GridSize != mTarget->GetGridSize()) result = true;
	if (UseSolidSurface != mTarget->SolidSurface()) result = true;
	if (UseNormals != mTarget->UseSurfaceNormals()) result = true;
	if (!result) {
		RGBColor	testColor;
		mTarget->GetPosColor(&testColor);
		if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)||
				(PosColor.blue!=testColor.blue)) result=true;
		mTarget->GetNegColor(&testColor);
		if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)||
				(NegColor.blue!=testColor.blue)) result=true;
		unsigned char test_transp = mTarget->GetTransparency();
		if (test_transp != Transparency) 
			result=true;
	}
	return result;
}
void MEP3DSurfPane::OnUpdate(wxCommandEvent &event) {
	//only update the grid if needed
	bool updateGrid=UpdateTest, updateContour=false;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) {
		updateGrid = true;
		mTarget->SetFixGrid(false);
	}
	if (NumGridPoints != mTarget->GetNumGridPoints()) {
		updateGrid = true;
		mTarget->SetFixGrid(false);
	}
	if (ContourValue != mTarget->GetContourValue()) updateContour = true;
	if (GridSize != mTarget->GetGridSize()) {
		updateGrid = true;
		mTarget->SetFixGrid(false);
	}
	if (Visible && !mTarget->ContourAvail()) updateContour = true;
	//test to see if grid/contour must be updated anyway...
	if (updateContour && ! mTarget->GridAvailable()) updateGrid = true;
	if (updateGrid) updateContour = true;
	mTarget->SetVisibility(Visible);
	mTarget->SolidSurface(UseSolidSurface);
	mTarget->setTargetOrbSet(TargetOrbSet);
	mTarget->SetNumGridPoints(NumGridPoints);
	mTarget->SetContourValue(ContourValue);
	mTarget->SetGridSize(GridSize);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetNegColor(&NegColor);
	mTarget->SetTransparency(Transparency);
	mTarget->UseSurfaceNormals(UseNormals);
	MoleculeData * data = owner->GetMoleculeData();
	WinPrefs * Prefs = owner->GetPrefs();
	//update this surface's data on all frames if necessary
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) {	//update all frames
		long	SurfaceID;
		Frame *	lFrame = data->GetFirstFrame();
		updateGrid = updateContour = true;
		if (AllFrames) {	//adding the surface to all frames
			SurfaceID = mTarget->SetSurfaceID();
			while (lFrame) {
				if (lFrame != data->GetCurrentFramePtr()) {
					MEP3DSurface * NewSurface = new MEP3DSurface(Prefs);
					if (NewSurface) {
						NewSurface->SetSurfaceID(SurfaceID);
						lFrame->AppendSurface(NewSurface);
					}
				}
				lFrame = lFrame->GetNextFrame();
			}
		} else {			//deleting the surface from other frames
			SurfaceID = mTarget->GetSurfaceID();
			mTarget->SetSurfaceID(0);	//Unmark this frames surface so it doesn't get deleted
			while (lFrame) {
				lFrame->DeleteSurfaceWithID(SurfaceID);
				lFrame = lFrame->GetNextFrame();
			}
		}
	}
	Progress * lProgress = new Progress();
	if (AllFrames) {	//compute the contour for each frame, no grid is kept
		long SurfaceID = mTarget->GetSurfaceID();
		long CurrentFrame = data->GetCurrentFrame();
		long NumFrames = data->GetNumFrames();
		for (int i=0; i<NumFrames; i++) {
			MEP3DSurface * lSurf;
			lSurf = NULL;
			data->SetCurrentFrame(i+1);
			Frame * lFrame = data->GetCurrentFramePtr();
			if (CurrentFrame != data->GetCurrentFrame()) {
				Surface * temp = lFrame->GetSurfaceWithID(SurfaceID);
				//Confirm that the surface is the correct type
				if (temp)
					if (temp->GetSurfaceType() == kMEP3D)
						lSurf = (MEP3DSurface *) temp;
				if (lSurf) lSurf->UpdateData(mTarget);
			} else lSurf = mTarget;
			if (lSurf) {
				if (Visible) {
					lProgress->ChangeText("Calculating 3D Grid...");
					lProgress->SetBaseValue(100*i/NumFrames);
					lProgress->SetScaleFactor((float) 0.9/NumFrames);
					if (updateGrid) {
						try {
							lSurf->CalculateMEPGrid(data, lProgress);
						} catch (...) {
						}
					}
					lProgress->ChangeText("Contouring grid...");
					lProgress->SetBaseValue((long)(100*i/NumFrames + 90.0/NumFrames));
					lProgress->SetScaleFactor((float) 0.1/NumFrames);
					if (updateContour) lSurf->Contour3DGrid(lProgress);
					lSurf->FreeGrid();
				} else {
					if (updateContour) lSurf->FreeContour();
				}
			}
		}
		data->SetCurrentFrame(CurrentFrame);
	} else {	//simply update this surface
		if (Visible) {
			lProgress->ChangeText("Calculating 3D Grid...");
			lProgress->SetScaleFactor(0.95);
			try {
				if (updateGrid) mTarget->CalculateMEPGrid(data, lProgress);
				lProgress->ChangeText("Contouring grid...");
				lProgress->SetBaseValue(95);
				lProgress->SetScaleFactor(0.05);
				if (updateContour) mTarget->Contour3DGrid(lProgress);
			} catch (...) {
				MessageAlert("MEP Calculation failed.");
			}
		} else {
			mTarget->FreeGrid();
			if (updateContour) mTarget->FreeContour();
		}
	}
	if (lProgress) delete lProgress;

	if (mTarget->GridAvailable())
		mFreeMemBut->Enable();
	else
		mFreeMemBut->Disable();

	SetContourMaxValueText();
	setContourValueSld();
	UpdateTest = false;
	setUpdateButton();

	if (mTarget->ExportPossible())
		mExportBut->Enable();
	else
		mExportBut->Disable();

	owner->SurfaceUpdated();
}

void MEP3DSurfPane::CreateControls() {
	MEP3DSurfPane * MEP3DPanel = this;	//this is here just to match the code below from DialogBlocks

	wxBoxSizer* itemBoxSizer168 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer168, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText169 = new wxStaticText(MEP3DPanel, wxID_STATIC, _("Select Orbital Set:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer168->Add(itemStaticText169, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxString* mOrbSetChoiceStrings = NULL;
	mOrbSetChoice = new wxChoice(MEP3DPanel, ID_ORB_CHOICE, wxDefaultPosition, wxDefaultSize, 0, mOrbSetChoiceStrings, 0);
	if (ShowToolTips())
		mOrbSetChoice->SetToolTip(_("Choose the set of vectors to use for producing the MEP surface."));
	itemBoxSizer168->Add(mOrbSetChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer171 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer171, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
	wxStaticText* itemStaticText172 = new wxStaticText(MEP3DPanel, wxID_STATIC, _("Number of grid points:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer171->Add(itemStaticText172, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mNumGridPntSld = new wxSlider(MEP3DPanel, ID_GRID_POINT_SLIDER, 0, 10, 150, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_LABELS);
	itemBoxSizer171->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mNumGridPntSld->SetToolTip(_("Sets the resolution of the grid. Fewer points is faster while more points produces a high-resolution result."));

	wxBoxSizer* itemBoxSizer174 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer174, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
	wxStaticText* itemStaticText175 = new wxStaticText(MEP3DPanel, wxID_STATIC, _("Grid Size:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer174->Add(itemStaticText175, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mGridSizeSld = new wxSlider(MEP3DPanel, ID_GRID_SIZE_SLIDER, 0, 1, 300, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_HORIZONTAL);
	itemBoxSizer174->Add(mGridSizeSld, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mGridSizeSld->SetToolTip(_("Adjusts the volume over which the grid is computed. Adjust to the right if your surface appears clipped."));

	wxBoxSizer* itemBoxSizer177 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer177, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
	wxBoxSizer* itemBoxSizer178 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer177->Add(itemBoxSizer178, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
	wxStaticText* itemStaticText179 = new wxStaticText(MEP3DPanel, wxID_STATIC, _("Contour Value:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer178->Add(itemStaticText179, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mContourValSld = new FloatSlider(MEP3DPanel, ID_CONTOUR_VALUE_SLIDER, 0.0f, 0.0f, 100.0f, FloatSlider::POW, wxSize(CONTOUR_WIDTH, wxDefaultCoord));
	itemBoxSizer177->Add(mContourValSld, 0, wxFIXED_MINSIZE | wxALIGN_CENTER_VERTICAL|wxALL, 3);
	mContourValSld->SetToolTip(_("Sets the value for the isosurface. Move to the right to produce a surface closer to the nucleus."));

	wxBoxSizer* itemBoxSizer186 = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(itemBoxSizer186, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
	wxBoxSizer* itemBoxSizer187 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer186->Add(itemBoxSizer187, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxBoxSizer* itemBoxSizer188 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer187->Add(itemBoxSizer188, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	wxStaticText* itemStaticText189 = new wxStaticText(MEP3DPanel, wxID_STATIC, _("Surface Color:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer188->Add(itemStaticText189, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mOrbColor1 = new colorArea(MEP3DPanel, ID_3D_COLOR_POSITIVE, &PosColor);
	itemBoxSizer188->Add(mOrbColor1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mOrbColor2 = new colorArea(MEP3DPanel, ID_3D_COLOR_NEGATIVE, &NegColor);
	itemBoxSizer188->Add(mOrbColor2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer192 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer187->Add(itemBoxSizer192, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	wxStaticText* itemStaticText193 = new wxStaticText(MEP3DPanel, wxID_STATIC, _("Transparency:"), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer192->Add(itemStaticText193, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mTransparency = new wxSpinCtrl(MEP3DPanel, ID_TRANSPARENCY, _T(""),
			wxDefaultPosition, wxDefaultSize,
			wxSP_ARROW_KEYS, 0, 100, 0);
	mTransparency->SetValue(Transparency);
	itemBoxSizer192->Add(mTransparency, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer195 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer187->Add(itemBoxSizer195, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	wxString m3DRdoBoxStrings[] = {
		_("&Solid"),
		_("&WireFrame")
	};
	m3DRdoBox = new wxRadioBox(MEP3DPanel, ID_3D_RADIOBOX, _("Surface Display"), wxDefaultPosition, wxDefaultSize, 2, m3DRdoBoxStrings, 1, wxRA_SPECIFY_ROWS);
	m3DRdoBox->SetSelection(0);
	itemBoxSizer195->Add(m3DRdoBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mSmoothChkBox = new wxCheckBox(MEP3DPanel, ID_SMOOTH_CHECKBOX, _("Smooth"), wxDefaultPosition, wxDefaultSize, 0);
	mSmoothChkBox->SetValue(false);
	if (ShowToolTips())
		mSmoothChkBox->SetToolTip(_("Check to use surface normals to smooth out the surface of the contour."));
	itemBoxSizer195->Add(mSmoothChkBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer198 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer186->Add(itemBoxSizer198, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mSetParamBut = new wxButton(MEP3DPanel, ID_SET_PARAM_BUT, _("Parameters..."), wxDefaultPosition, wxDefaultSize, 0);
	itemBoxSizer198->Add(mSetParamBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mFreeMemBut = new wxButton(MEP3DPanel, ID_FREE_MEM_BUT, _("Free Mem"), wxDefaultPosition, wxDefaultSize, 0);

	if (mTarget->GridAvailable())
		mFreeMemBut->Enable();
	else
		mFreeMemBut->Disable();

	if (ShowToolTips())
		mFreeMemBut->SetToolTip(_("Click to free the memory used by the 3D grid."));
	itemBoxSizer198->Add(mFreeMemBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mExportBut = new wxButton(MEP3DPanel, ID_SURFACE_EXPORT_BUT, _("Export..."), wxDefaultPosition, wxDefaultSize, 0);

	if (mTarget->ExportPossible())
		mExportBut->Enable();
	else
		mExportBut->Disable();

	itemBoxSizer198->Add(mExportBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mUpdateBut = new wxButton(MEP3DPanel, ID_SURFACE_UPDATE_BUT, _("Update"), wxDefaultPosition, wxDefaultSize, 0);
	mUpdateBut->SetDefault();
	itemBoxSizer198->Add(mUpdateBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
}

Surface2DParamDlg::Surface2DParamDlg(BaseSurfacePane * parent, Surf2DBase * targetSurface, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style) {
	mParent = parent;
	mTargetSurf = targetSurface;

	Create(id, caption, pos, size, style);

}

bool Surface2DParamDlg::Create(wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style) {
	wxDialog::Create(mParent, id, caption, pos, size, style);

	createControls();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();

	return true;
}

void Surface2DParamDlg::createControls() {
	wxString tmpStr;
	CPoint3D tempPt;
	mTargetSurf->GetOrigin(&tempPt);

	wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer * firstTierSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * secondTierSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * thirdTierSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * fourthTierSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * fifthTierSizer = new wxBoxSizer(wxHORIZONTAL);

	firstTierSizer->Add(new wxStaticText(this, wxID_ANY,
				_T("Number of grid points:"),
				wxDefaultPosition, wxDefaultSize), 
			0, wxALIGN_LEFT | wxALL, 3);

	numGridPoint = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%ld"), mTargetSurf->GetNumGridPoints());
	numGridPoint->SetValue(tmpStr);
	firstTierSizer->Add(numGridPoint, 0, wxALL, 5);
	numGridPoint->SetToolTip(_("The dimension of the 2D of grid."));

	mainSizer->Add(firstTierSizer);

	mTargetSurf->GetOrigin(&tempPt);

	mainSizer->Add(new wxStaticText(this, wxID_ANY,
				_T("Origin (x, y, z):"),
				wxDefaultPosition, wxDefaultSize), 
			0, wxALIGN_LEFT | wxALL, 3);

	originText1 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), tempPt.x);
	originText1->SetValue(tmpStr);
	originText1->SetToolTip(_("The x coordinate for the origin point of the grid."));

	originText2 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), tempPt.y);
	originText2->SetValue(tmpStr);
	originText2->SetToolTip(_("The y coordinate for the origin point of the grid."));

	originText3 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), tempPt.z);
	originText3->SetValue(tmpStr);
	originText3->SetToolTip(_("The z coordinate for the origin point of the grid."));

	secondTierSizer->Add(originText1, 0, wxALL, 5);
	secondTierSizer->Add(originText2, 0, wxALL, 5);
	secondTierSizer->Add(originText3, 0, wxALL, 5);

	mainSizer->Add(secondTierSizer);

	mTargetSurf->GetXIncrement(&tempPt);

	mainSizer->Add(new wxStaticText(this, wxID_ANY,
				_T("Vector defining axis 1:"),
				wxDefaultPosition, wxDefaultSize), 
			0, wxALIGN_LEFT | wxALL, 3);

	vectorAxis1x = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), tempPt.x);
	vectorAxis1x->SetValue(tmpStr);
	vectorAxis1x->SetToolTip(_("The x coordinate for vector 1 defining the grid. The length of the vector provides the increment between points."));

	vectorAxis1y = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), tempPt.y);
	vectorAxis1y->SetValue(tmpStr);
	vectorAxis1y->SetToolTip(_("The y coordinate for vector 1 defining the grid. The length of the vector provides the increment between points."));

	vectorAxis1z = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), tempPt.z);
	vectorAxis1z->SetValue(tmpStr);
	vectorAxis1z->SetToolTip(_("The z coordinate for vector 1 defining the grid. The length of the vector provides the increment between points."));

	thirdTierSizer->Add(vectorAxis1x, 0, wxALL, 5);
	thirdTierSizer->Add(vectorAxis1y, 0, wxALL, 5);
	thirdTierSizer->Add(vectorAxis1z, 0, wxALL, 5);

	mTargetSurf->GetYIncrement(&tempPt);

	mainSizer->Add(thirdTierSizer);

	mainSizer->Add(new wxStaticText(this, wxID_ANY,
				_T("Vector defining axis 2:"),
				wxDefaultPosition, wxDefaultSize), 
			0, wxALIGN_LEFT | wxALL, 3);

	vectorAxis2x = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), tempPt.x);
	vectorAxis2x->SetValue(tmpStr);
	vectorAxis2x->SetToolTip(_("The x coordinate for vector 2 defining the grid. The length of the vector provides the increment between points."));

	vectorAxis2y = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), tempPt.y);
	vectorAxis2y->SetValue(tmpStr);
	vectorAxis2y->SetToolTip(_("The y coordinate for vector 2 defining the grid. The length of the vector provides the increment between points."));

	vectorAxis2z = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), tempPt.z);
	vectorAxis2z->SetValue(tmpStr);
	vectorAxis2z->SetToolTip(_("The z coordinate for vector 2 defining the grid. The length of the vector provides the increment between points."));

	fourthTierSizer->Add(vectorAxis2x, 0, wxALL, 5);
	fourthTierSizer->Add(vectorAxis2y, 0, wxALL, 5);
	fourthTierSizer->Add(vectorAxis2z, 0, wxALL, 5);

	mainSizer->Add(fourthTierSizer);

	wxButton * okButton = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize);
	okButton->SetToolTip(_("Click Ok to apply the current parameter values to the surface and close the dialog."));
	wxButton * cancelButton = new wxButton(this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize);
	cancelButton->SetToolTip(_("Click Cancel to abandon changes and close the dialog."));
	wxButton * copyAllButton = new wxButton(this, ID_COPY_ALL, _("Cop&y All"), wxDefaultPosition, wxDefaultSize);
	copyAllButton->SetToolTip(_("Copy the current values to the clipboard."));
	wxButton * pasteAllButton = new wxButton(this, ID_PASTE_ALL, _("&Paste All"), wxDefaultPosition, wxDefaultSize);
	pasteAllButton->SetToolTip(_("Paste a set of parameters values from the clipboard."));

	fifthTierSizer->Add(copyAllButton , 0, wxALL, 5);
	fifthTierSizer->Add(pasteAllButton , 0, wxALL, 5);
	fifthTierSizer->Add(cancelButton , 0, wxALL, 5);
	fifthTierSizer->Add(okButton , 0, wxALL, 5);

	mainSizer->Add(fifthTierSizer);

	SetSizer(mainSizer);
}

void Surface2DParamDlg::OnClose(wxCommandEvent &event) {
	wxString tmpStr;
	long tempLong;
	CPoint3D tempPt;
	double	tempFlt;

	tmpStr = numGridPoint->GetValue();
	if (tmpStr.ToLong(&tempLong))
		mTargetSurf->SetNumGridPoints(tempLong);

	tempPt.x = tempPt.y = tempPt.z = 0.0;
	tmpStr = originText1->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.x = tempFlt;
	tmpStr = originText2->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.y = tempFlt;
	tmpStr = originText3->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.z = tempFlt;

	mTargetSurf->SetOrigin(&tempPt);

	tempPt.x = 0.1;
	tempPt.y = tempPt.z = 0.0;
	tmpStr = vectorAxis1x->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.x = tempFlt;
	tmpStr = vectorAxis1y->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.y = tempFlt;
	tmpStr = vectorAxis1z->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.z = tempFlt;

	mTargetSurf->SetXIncrement(&tempPt);

	tempPt.x = tempPt.z = 0.0;
	tempPt.y = 0.1;
	tmpStr = vectorAxis2x->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.x = tempFlt;
	tmpStr = vectorAxis2y->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.y = tempFlt;
	tmpStr = vectorAxis2z->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.z = tempFlt;

	mTargetSurf->SetYIncrement(&tempPt);

	mTargetSurf->SetRotate2DMap(false);

	mParent->TargetToPane();
	mParent->refreshControls();
	mParent->SetUpdateTest(true);
	//Pass the event up the chain to allow the dialog to be properly cleaned up
	event.Skip();
}

/*!!! Use wxWidgets' config class to implement copyAll and pasteAll
  instead of operating on a file directly
  */

void Surface2DParamDlg::OnCopyAll(wxCommandEvent &event) {
	wxConfigBase *pConfig = wxConfigBase::Get();

	pConfig->Write(_T("/Parameters/Surface2D/NumGridPoints"), numGridPoint->GetValue());

	pConfig->Write(_T("/Parameters/Surface2D/OriginX"), originText1->GetValue());
	pConfig->Write(_T("/Parameters/Surface2D/OriginY"), originText2->GetValue());
	pConfig->Write(_T("/Parameters/Surface2D/OriginZ"), originText3->GetValue());

	pConfig->Write(_T("/Parameters/Surface2D/vectorAxis1x"), vectorAxis1x->GetValue());
	pConfig->Write(_T("/Parameters/Surface2D/vectorAxis1y"), vectorAxis1y->GetValue());
	pConfig->Write(_T("/Parameters/Surface2D/vectorAxis1z"), vectorAxis1z->GetValue());

	pConfig->Write(_T("/Parameters/Surface2D/vectorAxis2x"), vectorAxis2x->GetValue());
	pConfig->Write(_T("/Parameters/Surface2D/vectorAxis2y"), vectorAxis2y->GetValue());
	pConfig->Write(_T("/Parameters/Surface2D/vectorAxis2z"), vectorAxis2z->GetValue());

}

void Surface2DParamDlg::OnPasteAll(wxCommandEvent &event) {
	wxConfigBase *pConfig = wxConfigBase::Get();

	pConfig->SetPath(_T("/Parameters/Surface2D"));

	numGridPoint->SetValue(pConfig->Read(_T("NumGridPoints")));

	originText1->SetValue(pConfig->Read(_T("OriginX")));
	originText2->SetValue(pConfig->Read(_T("OriginY")));
	originText3->SetValue(pConfig->Read(_T("OriginZ")));

	vectorAxis1x->SetValue(pConfig->Read(_T("vectorAxis1x")));
	vectorAxis1y->SetValue(pConfig->Read(_T("vectorAxis1y")));
	vectorAxis1z->SetValue(pConfig->Read(_T("vectorAxis1z")));

	vectorAxis2x->SetValue(pConfig->Read(_T("vectorAxis2x")));
	vectorAxis2y->SetValue(pConfig->Read(_T("vectorAxis2y")));
	vectorAxis2z->SetValue(pConfig->Read(_T("vectorAxis2z")));
}

Surface3DParamDlg::Surface3DParamDlg(BaseSurfacePane * parent, Surf3DBase * targetSurface, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style) {
	mParent = parent;
	mTargetSurf = targetSurface;

	Create(id, caption, pos, size, style);

}

bool Surface3DParamDlg::Create(wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style) {
	wxDialog::Create(mParent, id, caption, pos, size, style);

	createControls();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();

	return true;
}

void Surface3DParamDlg::createControls() {
	wxString tmpStr;
	CPoint3D tempPt;

	wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);

	mainSizer->Add(new wxStaticText(this, wxID_ANY,
				_T("Number of grid points (x, y, z):"),
				wxDefaultPosition, wxDefaultSize), 
			0, wxALIGN_LEFT | wxALL, 3);

	wxBoxSizer * firstTierSizer = new wxBoxSizer(wxHORIZONTAL);
	numGridPoint1 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%ld"), mTargetSurf->GetNumXGridPoints());
	numGridPoint1->SetValue(tmpStr);
	numGridPoint1->SetToolTip(_("The X dimension of the 3D rectangular grid."));

	numGridPoint2 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%ld"), mTargetSurf->GetNumYGridPoints());
	numGridPoint2->SetValue(tmpStr);
	numGridPoint2->SetToolTip(_("The Y dimension of the 3D rectangular grid."));

	numGridPoint3 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%ld"), mTargetSurf->GetNumZGridPoints());
	numGridPoint3->SetValue(tmpStr);
	numGridPoint3->SetToolTip(_("The Z dimension of the 3D rectangular grid."));

	firstTierSizer->Add(numGridPoint1, 0, wxALL, 5);
	firstTierSizer->Add(numGridPoint2, 0, wxALL, 5);
	firstTierSizer->Add(numGridPoint3, 0, wxALL, 5);

	mainSizer->Add(firstTierSizer);

	mainSizer->Add(new wxStaticText(this, wxID_ANY,
				_T("Origin (x, y, z):"),
				wxDefaultPosition, wxDefaultSize), 
			0, wxALIGN_LEFT | wxALL, 3);

	wxBoxSizer * secondTierSizer = new wxBoxSizer(wxHORIZONTAL);

	mTargetSurf->GetOrigin(&tempPt);

	originText1 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), tempPt.x);
	originText1->SetValue(tmpStr);
	originText1->SetToolTip(_("The x coordinate for the origin point of the grid."));

	originText2 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), tempPt.y);
	originText2->SetValue(tmpStr);
	originText2->SetToolTip(_("The y coordinate for the origin point of the grid."));

	originText3 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), tempPt.z);
	originText3->SetValue(tmpStr);
	originText3->SetToolTip(_("The z coordinate for the origin point of the grid."));

	secondTierSizer->Add(originText1, 0, wxALL, 5);
	secondTierSizer->Add(originText2, 0, wxALL, 5);
	secondTierSizer->Add(originText3, 0, wxALL, 5);

	mainSizer->Add(secondTierSizer);

	mainSizer->Add(new wxStaticText(this, wxID_ANY,
				_T("Grid increment (x, y, z):"),
				wxDefaultPosition, wxDefaultSize), 
			0, wxALIGN_LEFT | wxALL, 3);

	wxBoxSizer * thirdTierSizer = new wxBoxSizer(wxHORIZONTAL);
	gridIncText1 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), mTargetSurf->GetXGridInc());
	gridIncText1->SetValue(tmpStr);
	gridIncText1->SetToolTip(_("The increment in the X direction for the 3D grid."));

	gridIncText2 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), mTargetSurf->GetYGridInc());
	gridIncText2->SetValue(tmpStr);
	gridIncText2->SetToolTip(_("The increment in the Y direction for the 3D grid."));

	gridIncText3 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25), 0, wxTextValidator(wxFILTER_NUMERIC));
	tmpStr.Printf(wxT("%f"), mTargetSurf->GetZGridInc());
	gridIncText3->SetValue(tmpStr);
	gridIncText3->SetToolTip(_("The increment in the Z direction for the 3D grid."));

	thirdTierSizer->Add(gridIncText1, 0, wxALL, 5);
	thirdTierSizer->Add(gridIncText2, 0, wxALL, 5);
	thirdTierSizer->Add(gridIncText3, 0, wxALL, 5);

	mainSizer->Add(thirdTierSizer);

	wxBoxSizer * fourthTierSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton * okButton = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize);
	okButton->SetToolTip(_("Click Ok to apply the current parameter values to the surface and close the dialog."));
	wxButton * cancelButton = new wxButton(this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize);
	cancelButton->SetToolTip(_("Click Cancel to abandon changes and close the dialog."));
	wxButton * copyAllButton = new wxButton(this, ID_COPY_ALL, _("Cop&y All"), wxDefaultPosition, wxDefaultSize);
	copyAllButton->SetToolTip(_("Copy the current values to the clipboard."));
	wxButton * pasteAllButton = new wxButton(this, ID_PASTE_ALL, _("&Paste All"), wxDefaultPosition, wxDefaultSize);
	pasteAllButton->SetToolTip(_("Paste a set of parameters values from the clipboard."));

	fourthTierSizer->Add(copyAllButton , 0, wxALL, 5);
	fourthTierSizer->Add(pasteAllButton , 0, wxALL, 5);
	fourthTierSizer->Add(cancelButton , 0, wxALL, 5);
	fourthTierSizer->Add(okButton , 0, wxALL, 5);

	mainSizer->Add(fourthTierSizer);

	SetSizer(mainSizer);
}

void Surface3DParamDlg::OnClose(wxCommandEvent &event) {
	wxString tmpStr;
	long tempLong;
	CPoint3D tempPt;
	double	tempFlt;

	long origXGridpts = mTargetSurf->GetNumXGridPoints();
	long origYGridpts = mTargetSurf->GetNumYGridPoints();
	long origZGridpts = mTargetSurf->GetNumZGridPoints();
	tmpStr = numGridPoint1->GetValue();
	if (tmpStr.ToLong(&tempLong))
		mTargetSurf->SetNumXGridPoints(tempLong);

	tmpStr = numGridPoint2->GetValue();
	if (tmpStr.ToLong(&tempLong))
		mTargetSurf->SetNumYGridPoints(tempLong);

	tmpStr = numGridPoint3->GetValue();
	if (tmpStr.ToLong(&tempLong))
		mTargetSurf->SetNumZGridPoints(tempLong);
 
	//Since the number of grid points is cubic it gets big fast. Test to make sure the
	//user entered values that can be managed.
	long testpts = mTargetSurf->GetNumXGridPoints()*mTargetSurf->GetNumYGridPoints()*
				mTargetSurf->GetNumZGridPoints();
	if (testpts < 0) {	//overflow
		MessageAlert("Error: The specified number of grid points exceeds what is possible on this machine.");
		mTargetSurf->SetNumXGridPoints(origXGridpts);
		mTargetSurf->SetNumYGridPoints(origYGridpts);
		mTargetSurf->SetNumZGridPoints(origZGridpts);
	} else if (testpts > 200*200*200) {	//this is normally a "big" size
		MessageAlert("Warning: the selected number of grid points maybe excessive!");
	}
	tempPt.x = tempPt.y = tempPt.z = 0.0;
	tmpStr = originText1->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.x = tempFlt;
	tmpStr = originText2->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.y = tempFlt;
	tmpStr = originText3->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.z = tempFlt;

	mTargetSurf->SetOrigin(&tempPt);

	tmpStr = gridIncText1->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		mTargetSurf->SetXGridInc(tempFlt);

	tmpStr = gridIncText2->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		mTargetSurf->SetYGridInc(tempFlt);			

	tmpStr = gridIncText3->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		mTargetSurf->SetZGridInc(tempFlt);

	mTargetSurf->SetFixGrid(true);

	mParent->TargetToPane();
	mParent->refreshControls();
	mParent->SetUpdateTest(true);

	event.Skip();
}

/*!!! Use wxWidgets' config class to implement copyAll and pasteAll
  instead of operating on a file directly
  */

void Surface3DParamDlg::OnCopyAll(wxCommandEvent &event) {
	wxConfigBase *pConfig = wxConfigBase::Get();

	pConfig->Write(_T("/Parameters/Surface3D/NumGridPointsX"), numGridPoint1->GetValue());
	pConfig->Write(_T("/Parameters/Surface3D/NumGridPointsY"), numGridPoint2->GetValue());
	pConfig->Write(_T("/Parameters/Surface3D/NumGridPointsZ"), numGridPoint3->GetValue());

	pConfig->Write(_T("/Parameters/Surface3D/OriginX"), originText1->GetValue());
	pConfig->Write(_T("/Parameters/Surface3D/OriginY"), originText2->GetValue());
	pConfig->Write(_T("/Parameters/Surface3D/OriginZ"), originText3->GetValue());

	pConfig->Write(_T("/Parameters/Surface3D/XInc"), gridIncText1->GetValue());
	pConfig->Write(_T("/Parameters/Surface3D/YInc"), gridIncText2->GetValue());
	pConfig->Write(_T("/Parameters/Surface3D/ZInc"), gridIncText3->GetValue());

}

void Surface3DParamDlg::OnPasteAll(wxCommandEvent &event) {
	wxConfigBase *pConfig = wxConfigBase::Get();

	pConfig->SetPath(_T("/Parameters/Surface3D"));

	numGridPoint1->SetValue(pConfig->Read(_T("NumGridPointsX")));
	numGridPoint2->SetValue(pConfig->Read(_T("NumGridPointsY")));
	numGridPoint3->SetValue(pConfig->Read(_T("NumGridPointsZ")));

	originText1->SetValue(pConfig->Read(_T("OriginX")));
	originText2->SetValue(pConfig->Read(_T("OriginY")));
	originText3->SetValue(pConfig->Read(_T("OriginZ")));

	gridIncText1->SetValue(pConfig->Read(_T("XInc")));
	gridIncText2->SetValue(pConfig->Read(_T("YInc")));
	gridIncText3->SetValue(pConfig->Read(_T("ZInc")));
}

void OrbSurfacePane::OnAtomicOrbitalChoice(wxCommandEvent& event) {

	if (!coefIsEnabled) {
		mOrbCoef->SetSelection(wxNOT_FOUND);
	}

}
