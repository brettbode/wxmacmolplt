/*
 Implement preferences...    -Song Li
*/

//Make some useful preferences related definitions

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "PrefsPanes.h"
#endif
#include "Globals.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

//put ID definitions here later
#define kAtomPrefsPane		1
#define kBondPrefsPane		2
#define kDisplayPrefsPane	3
#define kEPrefsPane			4
#define kFontPrefsPane		5
#define kFilePrefsPane		6
#define kScalingPrefsPane	7
#define kStereoPrefsPane	8
#define kSurfacePrefsPane	9
#define kQD3DPrefsPane		10

#define ID_SHOW_NORMAL_MODE_ANIMATION 5000
#define ID_SHOW_NORMAL_MODE_ROTATION 5001
#define ID_NORMAL_MODE_SCALING_SLIDER 5002
#define ID_BOND_MODE_CHOICE 5003
#define ID_DISPLAY_MODE_RADIOBOX 5004
#define ID_COLOR_BONDS_BY_ATOM_COLOR 5005
#define ID_ATOM_LABELS_RADIO 5006
#define ID_PLOT_BALL_SIZE_SLIDER 5007
#define ID_AUTO_BOND_PASTE_OPEN 5008
#define ID_GUESS_BOND_ORDER 5009
#define ID_PREVENT_HH_BONDS 5010
#define ID_LOOK_HYDROGEN_BONDS 5011
#define ID_SHOW_ANGLES_WHILE_ROTATING 5012
#define ID_USE_MAC_EOL_CHAR 5013
#define ID_PROMPT_SAVE_FILE 5014
#define ID_CREATE_CUSTOM_FILE_ICON 5015
#define ID_CHANGE_FILE_CREATOR 5016
#define ID_AUTO_BOND_TOLERANCE_SLIDER 5017
#define ID_ATOM_SIZE_SLIDER 5018
#define ID_DEPTH_CUEING_SLIDER 5019
#define ID_ANIM_QUALITY_SLIDER 5020
#define ID_FRAME_DELAY_SLIDER 5021
#define ID_STEREO_ACTIVE 5022
#define ID_NUM_GRID_POINT_SLIDER 5023
#define ID_3D_GRID_SIZE_SLIDER 5024
#define ID_SHOW_ZERO_VALUE_CONTOUR 5025
#define ID_SHOW_ATOM_POSITION 5026
#define ID_BOND_SIZE_SLIDER 5027
#define ID_DISPLAY_QUALITY_SLIDER 5028
#define ID_FILL_LIGHT_BRIGHTNESS_SLIDER 5029
#define ID_POINT_LIGHT_BRIGHTNESS_SLIDER 5030
#define ID_LINE_WIDTH_SLIDER 5031
#define ID_3D_AUTO_ROTATE_CHECK 5032
#define ID_TOTAL_ENERGY 5033
#define ID_MP2_ENERGY 5034
#define ID_POTENTIAL_ENERGY 5035
#define ID_UNIT_RADIOBOX 5036
#define ID_MISC_RADIOBOX 5037
#define ID_DISPLAY_DEPTH_SLIDER	5038
#define ID_ATOM_LABEL_SLIDER	5039
#define ID_ANNOTATION_SLIDER	5040
#define ID_SHADER_CHOICE 5041
#define ID_3D_AUTOROTATIONX_SPIN 5042
#define ID_3D_AUTOROTATIONY_SPIN 5043

#include "GlobalExceptions.h"
#include "MyTypes.h"
#include "Prefs.h"
#include "PrefsPanes.h"
#include "colorArea.h"
#include "mmp_gl.h"
#include <stdio.h>
#include <string.h>
#include <wx/colordlg.h>

using namespace std;

extern short gAppResRef;

BEGIN_EVENT_TABLE(BondPrefsPane, wxPanel)
	EVT_CHOICE(ID_BOND_MODE_CHOICE, BondPrefsPane::OnChoice)
	EVT_SLIDER(ID_NORMAL_MODE_SCALING_SLIDER, BondPrefsPane::OnSliderUpdate)
	EVT_CHECKBOX(ID_SHOW_NORMAL_MODE_ANIMATION, BondPrefsPane::OnToggleAnim)
	EVT_CHECKBOX(ID_SHOW_NORMAL_MODE_ROTATION, BondPrefsPane::OnToggleRotation)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(DisplayPrefsPane, wxPanel)
	EVT_RADIOBOX(ID_DISPLAY_MODE_RADIOBOX, DisplayPrefsPane::OnRadio)
	EVT_CHECKBOX(ID_COLOR_BONDS_BY_ATOM_COLOR, DisplayPrefsPane::OnCheckBox)
	EVT_RADIOBOX(ID_ATOM_LABELS_RADIO, DisplayPrefsPane::OnLabelsRadio)
	EVT_SLIDER(ID_ATOM_LABEL_SLIDER, DisplayPrefsPane::OnAtomLabelSlider)
	EVT_SLIDER(ID_ANNOTATION_SLIDER, DisplayPrefsPane::OnAnnotationLabelSlider)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(EnergyPrefsPane, wxPanel)
	EVT_SLIDER(ID_PLOT_BALL_SIZE_SLIDER, EnergyPrefsPane::OnSliderUpdate)
	EVT_RADIOBOX(ID_UNIT_RADIOBOX, EnergyPrefsPane::OnRadio)
	EVT_RADIOBOX(ID_MISC_RADIOBOX, EnergyPrefsPane::OnRadio)
	EVT_CHECKBOX(ID_TOTAL_ENERGY, EnergyPrefsPane::OnCheckBox)
	EVT_CHECKBOX(ID_MP2_ENERGY, EnergyPrefsPane::OnCheckBox)
	EVT_CHECKBOX(ID_POTENTIAL_ENERGY, EnergyPrefsPane::OnCheckBox)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(FilePrefsPane, wxPanel)
	EVT_CHECKBOX(ID_AUTO_BOND_PASTE_OPEN, FilePrefsPane::OnCheckBox)
	EVT_CHECKBOX(ID_GUESS_BOND_ORDER, FilePrefsPane::OnCheckBox)
	EVT_CHECKBOX(ID_PREVENT_HH_BONDS, FilePrefsPane::OnCheckBox)
	EVT_CHECKBOX(ID_LOOK_HYDROGEN_BONDS, FilePrefsPane::OnCheckBox)
	EVT_CHECKBOX(ID_SHOW_ANGLES_WHILE_ROTATING, FilePrefsPane::OnCheckBox)
	EVT_CHECKBOX(ID_USE_MAC_EOL_CHAR, FilePrefsPane::OnCheckBox)
	EVT_CHECKBOX(ID_PROMPT_SAVE_FILE, FilePrefsPane::OnCheckBox)
	EVT_CHECKBOX(ID_CREATE_CUSTOM_FILE_ICON, FilePrefsPane::OnCheckBox)
	EVT_CHECKBOX(ID_CHANGE_FILE_CREATOR, FilePrefsPane::OnCheckBox)
	EVT_SLIDER(ID_AUTO_BOND_TOLERANCE_SLIDER, FilePrefsPane::OnSliderUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(ScalingPrefsPane, wxPanel)
	EVT_SLIDER(ID_ATOM_SIZE_SLIDER, ScalingPrefsPane::OnSliderUpdate)
	EVT_SLIDER(ID_ANIM_QUALITY_SLIDER, ScalingPrefsPane::OnSliderUpdate)
	EVT_SLIDER(ID_FRAME_DELAY_SLIDER, ScalingPrefsPane::OnSliderUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(StereoPrefsPane, wxPanel)
	EVT_CHECKBOX(ID_STEREO_ACTIVE, StereoPrefsPane::OnCheckBox)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(SurfacePrefsPane, wxPanel)
	EVT_SLIDER(ID_NUM_GRID_POINT_SLIDER, SurfacePrefsPane::OnSliderUpdate)
	EVT_SLIDER(ID_3D_GRID_SIZE_SLIDER, SurfacePrefsPane::OnSliderUpdate)
	EVT_CHECKBOX(ID_SHOW_ZERO_VALUE_CONTOUR, SurfacePrefsPane::OnCheckBox)
	EVT_CHECKBOX(ID_SHOW_ATOM_POSITION, SurfacePrefsPane::OnCheckBox)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(QD3DPrefsPane, wxPanel)
	EVT_SLIDER(ID_BOND_SIZE_SLIDER, QD3DPrefsPane::OnSliderUpdate)
	EVT_SLIDER(ID_DISPLAY_QUALITY_SLIDER, QD3DPrefsPane::OnSliderUpdate)
	EVT_SLIDER(ID_FILL_LIGHT_BRIGHTNESS_SLIDER, QD3DPrefsPane::OnSliderUpdate)
	EVT_SLIDER(ID_POINT_LIGHT_BRIGHTNESS_SLIDER, QD3DPrefsPane::OnSliderUpdate)
	EVT_SLIDER(ID_LINE_WIDTH_SLIDER, QD3DPrefsPane::OnSliderUpdate)
	EVT_SLIDER(ID_DEPTH_CUEING_SLIDER, QD3DPrefsPane::OnSliderUpdate)
	EVT_CHOICE(ID_SHADER_CHOICE, QD3DPrefsPane::OnShaderChoice)
	EVT_SPINCTRL(ID_3D_AUTOROTATIONX_SPIN, QD3DPrefsPane::OnSpinChange)
	EVT_SPINCTRL(ID_3D_AUTOROTATIONY_SPIN, QD3DPrefsPane::OnSpinChange)
END_EVENT_TABLE()

PrefsPane::PrefsPane(MolDisplayWin* targetWindow,
					 WinPrefs* targetPrefs,
					 short PaneID,
					 Boolean GlobalPrefs,
					 wxBookCtrlBase *parent)
	: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER) { 

	isGlobalPrefs = GlobalPrefs;

	mTargetPrefs = targetPrefs;
}

PrefsPane::~PrefsPane() { 
}

AtomPrefsPane::AtomPrefsPane(MolDisplayWin* targetWindow,
							 wxBookCtrlBase *parent, WinPrefs* targetPrefs,
							 Boolean Global) 
	: PrefsPane(targetWindow, targetPrefs, kAtomPrefsPane, Global, parent) {

	scroll_win = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition,
									  wxDefaultSize, wxVSCROLL);

	mLabels[0] = wxString(wxT("Atom"));
	mLabels[1] = wxString(wxT("#"));
	mLabels[2] = wxString(wxT("size(pm)"));
	mLabels[3] = wxString(wxT("mass"));
	mLabels[4] = wxString(wxT("Color"));
	mLabels[5] = wxString(wxT("Pattern"));

	mMainSizer = new wxFlexGridSizer(kMaxAtomTypes+1, NUM_ATOM_LABELS, 3, 10);

	wxFlexGridSizer *sizer = new wxFlexGridSizer(1, 1, 0, 0);
	sizer->AddGrowableCol(0);
	sizer->AddGrowableRow(0);
	sizer->Add(scroll_win, wxSizerFlags().Expand());
	SetSizerAndFit(sizer);

 	scroll_win->SetSizer(mMainSizer);
	scroll_win->SetScrollRate(10, 10);
	scroll_win->SetVirtualSize(500, 1000);
	mMainSizer->Layout();
	scroll_win->FitInside();

}

AtomPrefsPane::~AtomPrefsPane() {

	// These should be deleted by wx.
#if 0
  for (int i = 0; i < kMaxAtomTypes; i++) {
      delete mEleNames[i];
      delete mEleSizes[i];
      delete mEleMasses[i];
      delete mColorArea[i];
      delete mPatternArea[i];
    }
#endif

}

void AtomPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) {

	for (int i = 0; i < NUM_ATOM_LABELS; i++) {
		mMainSizer->Add(new wxStaticText(scroll_win, wxID_ANY, mLabels[i],
										 wxDefaultPosition, wxDefaultSize,
										 wxALIGN_CENTER),
						0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
	}

	for (int i = 0; i < kMaxAtomTypes; i++) {
		wxString tmp;
		mTargetPrefs->GetAtomLabel(i, tmp);
		mEleNames[i] = new wxTextCtrl(scroll_win, wxID_ANY, tmp, wxDefaultPosition, wxSize(30, 20));
		mMainSizer->Add(mEleNames[i], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
		tmp.Printf(wxT("%d"), i+1);
		mMainSizer->Add(new wxStaticText(scroll_win, wxID_ANY, tmp,
										 wxDefaultPosition, wxDefaultSize,
										 wxALIGN_CENTER),
						0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

		tmp.Printf(wxT("%ld"),mTargetPrefs->GetAtomSize(i));
		mEleSizes[i] = new wxTextCtrl(scroll_win, wxID_ANY, tmp, wxDefaultPosition, wxSize(40, 20));
		mMainSizer->Add(mEleSizes[i], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
		
		tmp.Printf(wxT("%.2f"),mTargetPrefs->GetAtomMass(i));
		mEleMasses[i] = new wxTextCtrl(scroll_win, wxID_ANY, tmp, wxDefaultPosition, wxSize(50, 20));
		mMainSizer->Add(mEleMasses[i], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
		
		mColorArea[i] = new colorArea(scroll_win, i, mTargetPrefs->GetAtomColorLoc(i));
		mMainSizer->Add(mColorArea[i], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
		
		mPatternArea[i] = new colorPatternArea(scroll_win, i, mTargetPrefs->GetAtomColorLoc(i), mTargetPrefs->GetAtomPattern(i));
		
		mMainSizer->Add(mPatternArea[i], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
		mColorArea[i]->setPeer(mPatternArea[i]);
		mPatternArea[i]->setPeer(mColorArea[i]); //need to synchronize color if needed
	}

}

void AtomPrefsPane::saveToTempPrefs() {
	long size;
	double mass;

	wxString tmp;

	for (int i = 0; i < kMaxAtomTypes; i++) {
		mTargetPrefs->SetAtomLabel(i, mEleNames[i]->GetValue());
		(mEleSizes[i]->GetValue()).ToLong(&size);
		mTargetPrefs->SetAtomSize(i, size);
		(mEleMasses[i]->GetValue()).ToDouble(&mass);
		mTargetPrefs->SetAtomMass(i, mass);
		mColorArea[i]->getColor(mTargetPrefs->GetAtomColorLoc(i));
		mTargetPrefs->SetAtomPattern(i, mPatternArea[i]->getPattern());
	}
}

BondPrefsPane::BondPrefsPane(MolDisplayWin* targetWindow,
							 wxBookCtrlBase *parent,
							 WinPrefs* targetPrefs,
							 Boolean Global)
	: PrefsPane(targetWindow, targetPrefs, kBondPrefsPane, Global, parent) {

	/* Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER); */

	mMainSizer = new wxBoxSizer(wxVERTICAL);
	mUpperSizer = new wxBoxSizer(wxVERTICAL);
	mSldSizer = new wxBoxSizer(wxHORIZONTAL);
	mBox = new wxStaticBoxSizer(wxHORIZONTAL, this);
	mInnerSizer = new wxFlexGridSizer(3, 2, 0, 0);
	mLowerSizer = new wxBoxSizer(wxVERTICAL);

	SetSizer(mMainSizer);
}

BondPrefsPane::~BondPrefsPane() {
	delete mColorArea;
	delete mNormColorArea;
}

void BondPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) {
	mChkAnim = new wxCheckBox(this, ID_SHOW_NORMAL_MODE_ANIMATION, _T("Show Normal Mode During Animation"), wxDefaultPosition);
	mChkAnim->SetValue(mTargetPrefs->GetAnimateMode());
	mChkRotation = new wxCheckBox(this, ID_SHOW_NORMAL_MODE_ROTATION, _T("Show Normal Mode During Rotation"), wxDefaultPosition);
	mChkRotation->SetValue(mTargetPrefs->GetRotateMode());

	mSldScale = new wxSlider(this, ID_NORMAL_MODE_SCALING_SLIDER, 
				(int)(mTargetPrefs->GetVectorScale()*10 - 1), 0, 25,
							 wxDefaultPosition, wxSize(155,wxDefaultCoord));

	mUpperSizer->Add(mChkAnim, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
	mUpperSizer->Add(mChkRotation, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
	mSldSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("Normal Mode Scaling:"))) ,0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	mSldSizer->Add(mSldScale, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
	mUpperSizer->Add(mSldSizer);

	wxString choices[] = { _T("Hydrogen Bonds"),
						_T("Single Bonds"),
						_T("Double Bonds"),
						_T("Triple Bonds") };

	mBondChoice = new wxChoice(this, ID_BOND_MODE_CHOICE, wxPoint(10,10), wxSize(150,wxDefaultCoord), 4, choices);
	mBondChoice->SetSelection(1);
	mChoiceId = 1;

	mInnerSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT(""))), 0, wxALIGN_RIGHT | wxALL, 3);
	mInnerSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("Color"))), 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
	mInnerSizer->Add(mBondChoice, 0, wxALIGN_RIGHT | wxALL, 3);

	mColorArea = new colorArea(this, 1, mTargetPrefs->GetBondColorLoc(1));

	mNormColorArea = new colorArea(this, 3, mTargetPrefs->GetVectorColorLoc());

	mInnerSizer->Add(mColorArea, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

	mInnerSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("Normal Modes:"))), 0, wxALIGN_RIGHT | wxALL, 3);
	mInnerSizer->Add(mNormColorArea, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

	mBox->Add(mInnerSizer);
	mLowerSizer->Add(mBox);

	mMainSizer->Add(mUpperSizer, 0, wxALIGN_CENTER | wxALL, 3);
	mMainSizer->Add(mLowerSizer, 0, wxALIGN_CENTER | wxALL, 3);
}

void BondPrefsPane::saveToTempPrefs() {
	mColorArea->getColor(mTargetPrefs->GetBondColorLoc(mChoiceId));
	mNormColorArea->getColor(mTargetPrefs->GetVectorColorLoc());
}

void BondPrefsPane::OnSliderUpdate(wxCommandEvent &WXUNUSED(event)) {
	mTargetPrefs->SetVectorScale((float)(0.1*(mSldScale->GetValue()+1)));
}

void BondPrefsPane::OnToggleAnim(wxCommandEvent& WXUNUSED(event)) {
	mTargetPrefs->SetAnimateMode(mChkAnim->GetValue());
}

void BondPrefsPane::OnToggleRotation(wxCommandEvent& WXUNUSED(event)) {
	mTargetPrefs->SetRotateMode(mChkRotation->GetValue());
}

void BondPrefsPane::OnChoice(wxCommandEvent &event) {
  //int localId = -1;

	mColorArea->getColor(mTargetPrefs->GetBondColorLoc(mChoiceId));

	wxString localStr = event.GetString();

	if (localStr.Cmp(_T("Hydrogen Bonds")) == 0)
		mChoiceId = 0;
	else if (localStr.Cmp(_T("Single Bonds")) == 0)
		mChoiceId = 1;
	else if (localStr.Cmp(_T("Double Bonds")) == 0)
		mChoiceId = 2;
	else if (localStr.Cmp(_T("Triple Bonds")) == 0)
		mChoiceId = 3;

	mColorArea->setColor(mTargetPrefs->GetBondColorLoc(mChoiceId));
}


DisplayPrefsPane::DisplayPrefsPane(MolDisplayWin* targetWindow,
								   wxBookCtrlBase *parent,
								   WinPrefs* targetPrefs,
								   Boolean Global)
	: PrefsPane(targetWindow, targetPrefs, kDisplayPrefsPane, Global, parent) {
	/* Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER); */

	mMainSizer = new wxBoxSizer(wxVERTICAL);

	wxString choices[] = {_T("Ball and Stick"), _T("WireFrame (Bonds only)")};

	mRdoBox = new wxRadioBox(this, ID_DISPLAY_MODE_RADIOBOX, _T("Display Mode"), wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices), choices, 0, wxRA_SPECIFY_ROWS);
	mChkColor = new wxCheckBox(this, ID_COLOR_BONDS_BY_ATOM_COLOR, _T("Color bonds by atom color"), wxDefaultPosition, wxDefaultSize, 0);

	wxString atomLabels[] = {_T("None"), _T("Atomic Symbols"), _T("Atom Numbers"), _T("Both Symbols and Numbers")};

	mAtomLabels = new wxRadioBox(this, ID_ATOM_LABELS_RADIO, _T("Atom Labels"), wxDefaultPosition, wxDefaultSize, WXSIZEOF(atomLabels), atomLabels, 0, wxRA_SPECIFY_ROWS);

	mMainSizer->Add(mRdoBox, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
	mMainSizer->Add(mChkColor, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	mMainSizer->Add(mAtomLabels, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

	wxGridSizer * lgridSizer = new wxGridSizer(2,2, 0,0);
	lgridSizer->Add(new wxStaticText(this, wxID_ANY, _("Size of atom labels:")), 0, wxALIGN_RIGHT | wxALL, 3);
	mAtomLabelSizeSlider = new wxSlider(this, ID_ATOM_LABEL_SLIDER, 
							(int)(mTargetPrefs->GetAtomLabelSize()*100), 25, 400,
							wxDefaultPosition, wxSize(155,wxDefaultCoord));
	mAtomLabelSizeSlider->SetToolTip(_("Changes the size of the atomic labels. Move left for smaller labels, right for larger ones."));
	lgridSizer->Add(mAtomLabelSizeSlider, 0, wxALIGN_CENTER | wxALL, 3);

	lgridSizer->Add(new wxStaticText(this, wxID_ANY, _("Size of annotation text:")), 0, wxALIGN_RIGHT | wxALL, 3);
	mAnnotationLabelSizeSlider = new wxSlider(this, ID_ANNOTATION_SLIDER, 
										 (int)(mTargetPrefs->GetAnnotationLabelSize()*100), 25, 400,
										 wxDefaultPosition, wxSize(155,wxDefaultCoord));
	mAnnotationLabelSizeSlider->SetToolTip(_("Changes the size of the annotation text. Move left for smaller labels, right for larger ones."));
	lgridSizer->Add(mAnnotationLabelSizeSlider, 0, wxALIGN_CENTER | wxALL, 3);
	mMainSizer->Add(lgridSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
	
	SetSizer(mMainSizer);
}

#include <iostream>
void DisplayPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) {
	if (mTargetPrefs->DrawBallnStick() && !mTargetPrefs->DrawWireFrame())
		mRdoBox->SetSelection(0);
	else if (!mTargetPrefs->DrawBallnStick() && mTargetPrefs->DrawWireFrame())
		mRdoBox->SetSelection(1);
	else
		cout<<"Something wrong! Check preference setting!"<<endl;


	mChkColor->SetValue(mTargetPrefs->ColorBondHalves());

	if (mTargetPrefs->ShowAtomicSymbolLabels() && mTargetPrefs->ShowAtomNumberLabels())
		mAtomLabels->SetSelection(3);
	else if (mTargetPrefs->ShowAtomNumberLabels())
		mAtomLabels->SetSelection(2);
	else if (mTargetPrefs->ShowAtomicSymbolLabels())
		mAtomLabels->SetSelection(1);
	else
		mAtomLabels->SetSelection(0);
	
	mAtomLabelSizeSlider->SetValue((int)(mTargetPrefs->GetAtomLabelSize()*100));
	mAnnotationLabelSizeSlider->SetValue((int)(mTargetPrefs->GetAnnotationLabelSize()*100));
}

void DisplayPrefsPane::saveToTempPrefs() {

}

void DisplayPrefsPane::OnRadio(wxCommandEvent &event) {
	wxString tmpStr = event.GetString();

	if (tmpStr.Cmp(_T("Ball and Stick")) == 0) {
		mTargetPrefs->DrawBallnStick(true);
		mTargetPrefs->DrawWireFrame(false);
	}
	else if (tmpStr.Cmp(_T("WireFrame (Bonds only)")) == 0) {
		mTargetPrefs->DrawWireFrame(true);
		mTargetPrefs->DrawBallnStick(false);
	}
}

void DisplayPrefsPane::OnLabelsRadio(wxCommandEvent &event) {
	switch (event.GetSelection()) {
		case 0:
			mTargetPrefs->ShowAtomicSymbolLabels(false);
			mTargetPrefs->ShowAtomNumberLabels(false);
			break;
		case 1:
			mTargetPrefs->ShowAtomicSymbolLabels(true);
			mTargetPrefs->ShowAtomNumberLabels(false);
			break;
		case 2:
			mTargetPrefs->ShowAtomicSymbolLabels(false);
			mTargetPrefs->ShowAtomNumberLabels(true);
			break;
		case 3:
			mTargetPrefs->ShowAtomicSymbolLabels(true);
			mTargetPrefs->ShowAtomNumberLabels(true);
			break;
	}
}

void DisplayPrefsPane::OnCheckBox(wxCommandEvent &event) {
	if (event.GetId() == ID_COLOR_BONDS_BY_ATOM_COLOR)
		mTargetPrefs->ColorBondHalves(mChkColor->GetValue());
}
void DisplayPrefsPane::OnAtomLabelSlider(wxCommandEvent &event) {
	mTargetPrefs->SetAtomLabelSize((float)(mAtomLabelSizeSlider->GetValue())/100.0);
}
void DisplayPrefsPane::OnAnnotationLabelSlider(wxCommandEvent &event) {
	mTargetPrefs->SetAnnotationLabelSize((float)(mAnnotationLabelSizeSlider->GetValue())/100.0);
}

EnergyPrefsPane::EnergyPrefsPane(MolDisplayWin* targetWindow,
								 wxBookCtrlBase *parent,
								 WinPrefs* targetPrefs,
								 Boolean GlobalPrefs)
	: PrefsPane(targetWindow, targetPrefs, kEPrefsPane, GlobalPrefs, parent) {
	/* Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER); */

	mMainSizer = new wxBoxSizer(wxVERTICAL);
	mUpperSizer = new wxBoxSizer(wxHORIZONTAL);

	if (PrefsAreGlobal())
		mLowerSizer = new wxGridSizer(4,2, 0,0);
	else
		mLowerSizer = new wxGridSizer(6,2, 0,0);

	if (!PrefsAreGlobal()) {
		mBottomSizer = new wxBoxSizer(wxHORIZONTAL);
		mLeftBottomSizer = new wxBoxSizer(wxVERTICAL);
	}

	mRightBottomSizer = NULL;
	mRight1BottomSizer = NULL;
	mRight2BottomSizer = NULL;
	mAtomText[0] = NULL;
	mAtomText[1] = NULL;
	mAtomText[2] = NULL;
	mY1pt = NULL;
	mY2pt = NULL;

	SetSizer(mMainSizer);

}

EnergyPrefsPane::~EnergyPrefsPane() {
	delete mEColor;
	delete mMPColor;
	delete mPEColor;
	delete mOtherColor;

	if (mY1pt) delete mY1pt;
	if (mY2pt) delete mY2pt;
}

void EnergyPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) {
	EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();
	GraphOptions * lPOpts = mTargetPrefs->GetGraphOptions();
	//Setup the slider for the E plot point size

	mSldBallSize = new wxSlider(this, ID_PLOT_BALL_SIZE_SLIDER, 
			(int)(lEOpts->GetEPlotPointSize()), 1, 20,
			wxDefaultPosition, wxSize(155,wxDefaultCoord),
			wxSL_AUTOTICKS | wxSL_LABELS);

	wxString tmp;
	tmp.Printf(wxT("%d"), lEOpts->GetNumDigits());
	mNumDigitsArea = new wxTextCtrl(this, wxID_ANY, tmp, wxDefaultPosition, wxDefaultSize);

	mUpperSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("E. Plot Ball Size:"))), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
	mUpperSizer->Add(mSldBallSize, 0, wxALIGN_LEFT | wxALL, 3);
	mUpperSizer->Add(30, 30);
	mUpperSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("# digits"))), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
	mUpperSizer->Add(mNumDigitsArea, 0, wxALIGN_CENTER | wxALL, 3);

	mEColor = new colorArea(this, 0, lEOpts->GetTEColor());
	mMPColor = new colorArea(this, 1, lEOpts->GetMPColor());
	mPEColor = new colorArea(this, 2, lEOpts->GetPEColor());
	mOtherColor = new colorArea(this, 3, lEOpts->GetKEColor());

	mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("E Color"))), 0, wxALIGN_RIGHT | wxALL, 3);
	mLowerSizer->Add(mEColor, 0, wxALIGN_LEFT | wxALL, 3);
	mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("MP Color"))), 0, wxALIGN_RIGHT | wxALL, 3);
	mLowerSizer->Add(mMPColor, 0, wxALIGN_LEFT | wxALL, 3);

	if (!PrefsAreGlobal()) {
		mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("Y1 zero pt "))), 0, wxALIGN_RIGHT | wxALL, 3);
		tmp.Printf(wxT("%.4f"),lEOpts->GetY1Zero());
		mY1pt = new wxTextCtrl(this, wxID_ANY, tmp);
		mLowerSizer->Add(mY1pt, 0, wxALIGN_RIGHT | wxALL, 3);
	}

	mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("PE Color"))), 0, wxALIGN_RIGHT | wxALL, 3);
	mLowerSizer->Add(mPEColor, 0, wxALIGN_LEFT | wxALL, 3);
	mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("Other Color"))), 0, wxALIGN_RIGHT | wxALL, 3);
	mLowerSizer->Add(mOtherColor, 0, wxALIGN_LEFT | wxALL, 3);

	if (!PrefsAreGlobal()) {
		mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("Y2 zero pt "))), 0, wxALIGN_RIGHT | wxALL, 3);
		tmp.Printf(wxT("%.4f"),lEOpts->GetY2Zero());
		mY2pt = new wxTextCtrl(this, wxID_ANY, tmp);
		mLowerSizer->Add(mY2pt, 0, wxALIGN_RIGHT | wxALL, 3);
	}

	if (!PrefsAreGlobal()) {
		mTotalEny = new wxCheckBox(this, ID_TOTAL_ENERGY, _T("Total Energy"));
		mTotalEny->SetValue(lEOpts->PlotEnergy());
		mMP2Eny = new wxCheckBox(this, ID_MP2_ENERGY, _T("MP2 Energy"));
		mMP2Eny->SetValue(lEOpts->PlotMPEnergy());
		mPotEny = new wxCheckBox(this, ID_POTENTIAL_ENERGY, _T("Potential Energy"));
		mPotEny->SetValue(lEOpts->PlotPEnergy());

		mLeftBottomSizer->Add(mTotalEny, 0, wxALIGN_LEFT | wxALL, 3);
		mLeftBottomSizer->Add(mMP2Eny, 0, wxALIGN_LEFT | wxALL, 3);
		mLeftBottomSizer->Add(mPotEny, 0, wxALIGN_LEFT | wxALL, 3);

		EnergyUnit lunit = lEOpts->GetDisplayUnits();

		wxString choices[] = {_T("Default"), _T("kcal/mol")};

		mRdoUnit = new wxRadioBox(this, ID_UNIT_RADIOBOX, _T("Unit"), wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices), choices, 1, wxRA_SPECIFY_COLS);

		if (lunit == kDefault)
			mRdoUnit->SetSelection(0);
		else if (lunit == kKCalPerMole)
			mRdoUnit->SetSelection(1);

		wxString choices2[] = {_T("None"), _T("Kinetic Energy"), _T("RMS Grad"), _T("Max. Grad"), _T("Bond Length"), _T("Bond Angle") };

		mRdoMisc = new wxRadioBox(this, ID_MISC_RADIOBOX, _T(""), wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices2), choices2, 1, wxRA_SPECIFY_COLS);

		if (lEOpts->PlotOther())
			mRdoMisc->SetSelection(0);
		if (lEOpts->PlotKEnergy())
			mRdoMisc->SetSelection(1);
		if (lPOpts->PlotRMSGradient())
			mRdoMisc->SetSelection(2);
		if (lPOpts->PlotMaxGradient())
			mRdoMisc->SetSelection(3);
		if (lPOpts->PlotBondLength())
			mRdoMisc->SetSelection(4);
		if (lPOpts->PlotBondAngle())
			mRdoMisc->SetSelection(5);

		mLeftBottomSizer->Add(mRdoUnit, 0, wxALIGN_CENTER | wxALL, 3);
		mBottomSizer->Add(mLeftBottomSizer, 0, wxALL, 10);
		mBottomSizer->Add(mRdoMisc, 0, wxALIGN_CENTER | wxALL, 10);

		setHiddenCtrls();

		mRightBottomSizer->Hide(mRight1BottomSizer);
		mRightBottomSizer->Hide(mRight2BottomSizer); 
		//initially hide them

		if (lPOpts->PlotBondAngle()) {
			mRightBottomSizer->Show(mRight1BottomSizer, true, true);
			mRightBottomSizer->Show(mRight2BottomSizer, true, true);
			mRightBottomSizer->Layout();
		}

		if (lPOpts->PlotBondLength()) {
			mRightBottomSizer->Show(mRight1BottomSizer, true, true);
			mRightBottomSizer->Layout();
		}

	}

	mMainSizer->Add(mUpperSizer);
	mMainSizer->Add(mLowerSizer);

	if (!PrefsAreGlobal())
		mMainSizer->Add(mBottomSizer, 0, wxALL, 10);

}

void EnergyPrefsPane::setHiddenCtrls() {
	GraphOptions * lPOpts = mTargetPrefs->GetGraphOptions();

	if (!mRightBottomSizer)
		mRightBottomSizer = new wxBoxSizer(wxVERTICAL);

	if (!mRight1BottomSizer)
		mRight1BottomSizer = new wxGridSizer(2,3, 0,0);

	if (!mRight2BottomSizer)
		mRight2BottomSizer = new wxGridSizer(2,1, 0,0);

	wxString tmp;
	if (!mAtomText[0]) {
		tmp.Printf(wxT("%ld"), lPOpts->Get1stAtom()+1);
		mAtomText[0] = new wxTextCtrl(this, wxID_ANY, tmp);
	}
	if (!mAtomText[1]) {
		tmp.Printf(wxT("%ld"), lPOpts->Get2ndAtom()+1);
		mAtomText[1] = new wxTextCtrl(this, wxID_ANY, tmp);
	}

	mRight1BottomSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("Atom 1:"))), 0, wxALIGN_CENTER | wxALIGN_RIGHT | wxALL, 3);
	mRight1BottomSizer->Add(mAtomText[0], wxALIGN_CENTER | wxALL, 3);
	mRight1BottomSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("Atom 2:"))), 0, wxALIGN_CENTER | wxALIGN_RIGHT | wxALL, 3);
	mRight1BottomSizer->Add(mAtomText[1], wxALIGN_CENTER | wxALL, 3);

	if (!mAtomText[2]) {
		tmp.Printf(wxT("%ld"), lPOpts->Get3rdAtom()+1);
		mAtomText[2] = new wxTextCtrl(this, wxID_ANY, tmp);
	}
	mRight2BottomSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("Atom 3:"))), 0, wxALIGN_CENTER | wxALIGN_RIGHT | wxALL, 3);
	mRight2BottomSizer->Add(mAtomText[2], wxALIGN_CENTER | wxALL, 3);

	mRightBottomSizer->Add(mRight1BottomSizer);
	mRightBottomSizer->Add(mRight2BottomSizer);
	mBottomSizer->Add(mRightBottomSizer);
}

void EnergyPrefsPane::saveToTempPrefs() {
	EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();
	GraphOptions * lPOpts = mTargetPrefs->GetGraphOptions();

	long localLong;
	double localDouble;

	(mNumDigitsArea->GetValue()).ToLong(&localLong);
	lEOpts->SetNumDigits(localLong);

	mEColor->getColor(lEOpts->GetTEColor());
	mMPColor->getColor(lEOpts->GetMPColor());
	mOtherColor->getColor(lEOpts->GetKEColor());
	mPEColor->getColor(lEOpts->GetPEColor());

	if (!PrefsAreGlobal()) {
		(mY1pt->GetValue()).ToDouble(&localDouble);
		lEOpts->SetY1Zero(localDouble);
		(mY2pt->GetValue()).ToDouble(&localDouble);
		lEOpts->SetY2Zero(localDouble);

		if (lPOpts->PlotBondLength() || lPOpts->PlotBondAngle()) {
			(mAtomText[0]->GetValue()).ToLong(&localLong);
			lPOpts->Set1stAtom(localLong-1);
			(mAtomText[1]->GetValue()).ToLong(&localLong);
			lPOpts->Set2ndAtom(localLong-1);

			if (lPOpts->PlotBondAngle()) {
				(mAtomText[2]->GetValue()).ToLong(&localLong);
				lPOpts->Set3rdAtom(localLong-1);
			}
		}
	}
}

void EnergyPrefsPane::OnSliderUpdate(wxCommandEvent &WXUNUSED(event)) {
	EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();
	lEOpts->SetEPlotPointSize(mSldBallSize->GetValue());
}

void EnergyPrefsPane::OnCheckBox(wxCommandEvent& event) {
	int id = event.GetId();

	EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();

	if (id == ID_TOTAL_ENERGY)
		lEOpts->SetPlotEnergy(mTotalEny->GetValue());
	if (id == ID_MP2_ENERGY)
		lEOpts->SetPlotMPEnergy(mMP2Eny->GetValue());
	if (id == ID_POTENTIAL_ENERGY)
		lEOpts->SetPlotPEnergy(mPotEny->GetValue());
}

void EnergyPrefsPane::OnRadio(wxCommandEvent& event) {
	//int id = event.GetId();

	wxString tmpStr = event.GetString();

	mRightBottomSizer->Hide(mRight1BottomSizer);
	mRightBottomSizer->Hide(mRight2BottomSizer);

	if (tmpStr.Cmp(_T("Bond Length")) == 0 || tmpStr.Cmp(_T("Bond Angle")) == 0) {
		mRightBottomSizer->Show(mRight1BottomSizer, true, true);

		if (tmpStr.Cmp(_T("Bond Angle")) == 0)
			mRightBottomSizer->Show(mRight2BottomSizer, true, true);

		mMainSizer->Layout();
	}

	EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();
	GraphOptions * lPOpts = mTargetPrefs->GetGraphOptions();

	if (event.GetId() == ID_UNIT_RADIOBOX) {
		EnergyUnit lunit;

		if (tmpStr.Cmp(_T("Default")) == 0)
			lunit = kDefault;
		else if (tmpStr.Cmp(_T("kcal/mol")) == 0)
			lunit = kKCalPerMole;

		lEOpts->SetDisplayUnits(lunit);
	}

	if (event.GetId() == ID_MISC_RADIOBOX) {
		lEOpts->SetPlotOther(false);
		lEOpts->SetPlotKEnergy(false);
		lPOpts->SetPlotRMSGradient(false);
		lPOpts->SetPlotMaxGradient(false);
		lPOpts->SetPlotBondLength(false);
		lPOpts->SetPlotBondAngle(false);
		//set everything to false first

		if (tmpStr.Cmp(_T("None")) == 0)
			lEOpts->SetPlotOther(true);
		else if (tmpStr.Cmp(_T("Kinetic Energy")) == 0)
			lEOpts->SetPlotKEnergy(true);
		else if (tmpStr.Cmp(_T("RMS Grad")) == 0)
			lPOpts->SetPlotRMSGradient(true);
		else if (tmpStr.Cmp(_T("Max. Grad")) == 0)
			lPOpts->SetPlotMaxGradient(true);
		else if (tmpStr.Cmp(_T("Bond Length")) == 0)
			lPOpts->SetPlotBondLength(true);
		else if (tmpStr.Cmp(_T("Bond Angle")) == 0)
			lPOpts->SetPlotBondAngle(true);
	}

}

FilePrefsPane::FilePrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, targetPrefs, kFilePrefsPane, GlobalPrefs, parent) {
	/* Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER); */

	mMainSizer = new wxBoxSizer(wxVERTICAL);

	SetSizer(mMainSizer);
}

void FilePrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) {
	wxBoxSizer* UpperSizer = new wxBoxSizer(wxVERTICAL);
	mMainSizer->Add(UpperSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mChkBox[0] = new wxCheckBox(this, ID_AUTO_BOND_PASTE_OPEN, _T("Auto-Bond on Paste and File Open"));
	mChkBox[0]->SetValue(mTargetPrefs->GetAutoBond());
	UpperSizer->Add(mChkBox[0], 0, wxALIGN_LEFT|wxALL, 5);
	
	mChkBox[1] = new wxCheckBox(this, ID_GUESS_BOND_ORDER, _T("Guess bond order"));
	mChkBox[1]->SetValue(mTargetPrefs->DetermineBondOrder());
	UpperSizer->Add(mChkBox[1], 0, wxALIGN_LEFT|wxALL, 5);

	mChkBox[2] = new wxCheckBox(this, ID_PREVENT_HH_BONDS, _T("Prevent H-H bonds in Auto-Bond"));
	mChkBox[2]->SetValue(mTargetPrefs->GetHHBondFlag());
	UpperSizer->Add(mChkBox[2], 0, wxALIGN_LEFT|wxALL, 5);

	mChkBox[3] = new wxCheckBox(this, ID_LOOK_HYDROGEN_BONDS, _T("Look for hydrogen bonds"));
	mChkBox[3]->SetValue(mTargetPrefs->AllowHydrogenBonds());
	UpperSizer->Add(mChkBox[3], 0, wxALIGN_LEFT|wxALL, 5);

	mChkBox[4] = new wxCheckBox(this, ID_SHOW_ANGLES_WHILE_ROTATING, _T("Show angles while rotating"));
	mChkBox[4]->SetValue(mTargetPrefs->GetShowAngles());
	UpperSizer->Add(mChkBox[4], 0, wxALIGN_LEFT|wxALL, 5);

#ifdef __WXMAC__
	if (PrefsAreGlobal()) {
		mChkBox[5] = new wxCheckBox(this, ID_USE_MAC_EOL_CHAR, _T("Use Mac EOL chars"));
		mChkBox[5]->SetValue(mTargetPrefs->NativeEOLChar());
		UpperSizer->Add(mChkBox[5], 0, wxALIGN_LEFT|wxALL, 5);
	}
#endif

	mChkBox[6] = new wxCheckBox(this, ID_PROMPT_SAVE_FILE, _T("Prompt to save file"));
	mChkBox[6]->SetValue(mTargetPrefs->GetPrompt4Save());
	UpperSizer->Add(mChkBox[6], 0, wxALIGN_LEFT|wxALL, 5);


	mChkBox[7] = new wxCheckBox(this, ID_CREATE_CUSTOM_FILE_ICON, _T("create custom file icon"));
	mChkBox[7]->SetValue(mTargetPrefs->CreateCustomIcon());
	UpperSizer->Add(mChkBox[7], 0, wxALIGN_LEFT|wxALL, 5);

#ifdef __WXMAC__
	if (PrefsAreGlobal()) {
		mChkBox[8] = new wxCheckBox(this, ID_CHANGE_FILE_CREATOR, _T("Change file creator type to wxMacMolPlt"));
		mChkBox[8]->SetValue(mTargetPrefs->ChangeFileType());
		UpperSizer->Add(mChkBox[8], 0, wxALIGN_LEFT|wxALL, 5);
	}
#endif

	mSldTol = new wxSlider(this, ID_AUTO_BOND_TOLERANCE_SLIDER, 
			(int)(mTargetPrefs->GetAutoBondScale()*10000+0.5), 
			50, 150, wxDefaultPosition, 
			wxSize(155,wxDefaultCoord));

	wxBoxSizer* LowerSizer = new wxBoxSizer(wxHORIZONTAL);
	mMainSizer->Add(LowerSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);
	LowerSizer->Add(new wxStaticText(this, wxID_ANY, _T("Auto-Bond Tolerance:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
	LowerSizer->Add(mSldTol, 0, wxALIGN_LEFT | wxALL, 3);

	wxBoxSizer* BottomSizer = new wxBoxSizer(wxHORIZONTAL);
	mMainSizer->Add(BottomSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 2);
	wxString tmp;
	tmp.Printf(wxT("%ld"), mTargetPrefs->GetDRCSkip());
	mPointSkip = new wxTextCtrl(this, wxID_ANY, tmp);

	BottomSizer->Add(new wxStaticText(this, wxID_ANY, _T("When reading a DRC file how many\n points should be skipped between\n points read in? (0 reads every point)")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
	BottomSizer->Add(mPointSkip, 0, wxALIGN_CENTER | wxALL, 3);
}

void FilePrefsPane::saveToTempPrefs() {
	long v;
	(mPointSkip->GetValue()).ToLong(&v);
	mTargetPrefs->SetDRCSkip(v);
}

void FilePrefsPane::OnCheckBox(wxCommandEvent &event) {
	if (event.GetId() == ID_AUTO_BOND_PASTE_OPEN) {
		Boolean mainStatus = mChkBox[0]->GetValue();
		mTargetPrefs->SetAutoBond(mainStatus);

		if (!mainStatus) {
			mChkBox[1]->Disable();
			mChkBox[2]->Disable();
			mChkBox[3]->Disable();
		}
		else
		{
			mChkBox[1]->Enable();
			mChkBox[2]->Enable();
			mChkBox[3]->Enable();
		}
	}
	else if (event.GetId() == ID_GUESS_BOND_ORDER)
		mTargetPrefs->DetermineBondOrder(mChkBox[1]->GetValue());
	else if (event.GetId() == ID_PREVENT_HH_BONDS)
		mTargetPrefs->SetHHBondFlag(mChkBox[2]->GetValue());
	else if (event.GetId() == ID_LOOK_HYDROGEN_BONDS)
		mTargetPrefs->AllowHydrogenBonds(mChkBox[3]->GetValue());
	else if (event.GetId() == ID_SHOW_ANGLES_WHILE_ROTATING)
		mTargetPrefs->SetShowAngles(mChkBox[4]->GetValue());
#ifdef __WXMAC__
	else if (event.GetId() == ID_USE_MAC_EOL_CHAR && PrefsAreGlobal())
		mTargetPrefs->NativeEOLChar(mChkBox[5]->GetValue());
#endif
	else if (event.GetId() == ID_PROMPT_SAVE_FILE)
		mTargetPrefs->SetPrompt4Save(mChkBox[6]->GetValue());
	else if (event.GetId() == ID_CREATE_CUSTOM_FILE_ICON)
		mTargetPrefs->CreateCustomIcon(mChkBox[7]->GetValue());
#ifdef __WXMAC__
	else if (event.GetId() == ID_CHANGE_FILE_CREATOR)
		mTargetPrefs->ChangeFileType(mChkBox[8]->GetValue());
#endif
}

void FilePrefsPane::OnSliderUpdate(wxCommandEvent &WXUNUSED(event)) {
  mTargetPrefs->SetAutoBondScale((float)(mSldTol->GetValue())/10000);
}


ScalingPrefsPane::ScalingPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, targetPrefs, kScalingPrefsPane, GlobalPrefs, parent) {

	wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
	SetSizer(itemBoxSizer4);
	mMainSizer = new wxFlexGridSizer(3, 2, 0,0);
	itemBoxSizer4->Add(mMainSizer, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

}

void ScalingPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) {
	mSld[0] = new wxSlider(this, ID_ATOM_SIZE_SLIDER, 
			(int)(mTargetPrefs->GetAtomScale()*10000+0.5), 
			0, 250, wxDefaultPosition, 
			wxSize(155,wxDefaultCoord));

	mSld[1] = new wxSlider(this, ID_ANIM_QUALITY_SLIDER, 
			(int)(mTargetPrefs->GetAnimationSpeed()-1), 
			0, 15, wxDefaultPosition, 
			wxSize(155,wxDefaultCoord));

	mSld[2] = new wxSlider(this, ID_FRAME_DELAY_SLIDER, 
			(int)(mTargetPrefs->GetAnimateTime()), 
			0, 120, wxDefaultPosition, 
			wxSize(155,wxDefaultCoord));

	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Atom Size:")), 0, wxALIGN_RIGHT | wxALL, 3);
	mMainSizer->Add(mSld[0], 0, wxALIGN_LEFT | wxALL, 3);
	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Mode Animation Quality:")), 0, wxALIGN_RIGHT | wxALL, 3);
	mMainSizer->Add(mSld[1], 0, wxALIGN_LEFT | wxALL, 3);
	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Frame Delay:")), 0, wxALIGN_RIGHT | wxALL, 3);
	mMainSizer->Add(mSld[2], 0, wxALIGN_LEFT | wxALL, 3);
}

void ScalingPrefsPane::saveToTempPrefs() {

}

void ScalingPrefsPane::OnSliderUpdate(wxCommandEvent &event) {
	int id = event.GetId();

	if (id == ID_ATOM_SIZE_SLIDER)
		mTargetPrefs->SetAtomScale((float)(mSld[0]->GetValue())/10000);
	if (id == ID_ANIM_QUALITY_SLIDER)
		mTargetPrefs->SetAnimationSpeed(mSld[1]->GetValue()+1);
	if (id == ID_FRAME_DELAY_SLIDER)
		mTargetPrefs->SetAnimateTime(mSld[2]->GetValue());
}

StereoPrefsPane::StereoPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, targetPrefs, kStereoPrefsPane, GlobalPrefs, parent) {

	mMainSizer = new wxBoxSizer(wxVERTICAL);
	mMiddleSizer = new wxBoxSizer(wxHORIZONTAL);

	SetSizer(mMainSizer);
	if (!GlobalPrefs) {
		Disable();
	}
}

void StereoPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) {

#if wxCHECK_VERSION(2,9,0)
	int attribs[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16,
					 WX_GL_STEREO, 0};
	if (!wxGLCanvas::IsDisplaySupported(attribs)) {
		Disable();
		mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("No stereo pixel format available.")), 0, wxALIGN_LEFT | wxALL, 10);
	}
#else
	Disable();
	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("wxWidgets 2.9 needed for stereo support.")), 0, wxALIGN_LEFT | wxALL, 10);
#endif

	mChkActive = new wxCheckBox(this, ID_STEREO_ACTIVE, _T("Stereo Active"), wxDefaultPosition);
	mChkActive->SetValue(mTargetPrefs->UseStereo());
	mMainSizer->Add(mChkActive, 0, wxALIGN_LEFT | wxALL, 10);
	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Enabling or disabling stereo affects new windows only.")), 0, wxALIGN_LEFT | wxALL, 10);
	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("The eye separation is determined by dividing the focal length\n")
				_T("by the eye separation factor. To minimize ghosting, either\n")
				_T("zoom out or increase the factor below.")), 0, wxALIGN_LEFT | wxALL, 10);

	mMiddleSizer->Add(new wxStaticText(this, wxID_ANY, _T("Factor ")), 0, wxALIGN_LEFT | wxALL, 10);

	wxString tmp;
	tmp.Printf(wxT("%d"), mTargetPrefs->GetStereoOffset());
	mOffDegree = new wxTextCtrl(this, wxID_ANY, tmp);
	mMiddleSizer->Add(mOffDegree, 0, wxALIGN_LEFT | wxALL, 3);

	mMainSizer->Add(mMiddleSizer, 0, wxALIGN_LEFT | wxLEFT, 10);

}

void StereoPrefsPane::saveToTempPrefs() {
	long v;
	mOffDegree->GetValue().ToLong(&v);
	mTargetPrefs->SetStereoOffset(v);
}

void StereoPrefsPane::OnCheckBox(wxCommandEvent& WXUNUSED(event)) {
	mTargetPrefs->UseStereo(mChkActive->GetValue());
}


SurfacePrefsPane::SurfacePrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, targetPrefs, kSurfacePrefsPane, GlobalPrefs, parent) {
  /* Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER); */

	mMainSizer = new wxBoxSizer(wxHORIZONTAL);
	mLeftSizer = new wxBoxSizer(wxVERTICAL);
	mRightSizer = new wxBoxSizer(wxVERTICAL);
	mLeftMidSizer = new wxGridSizer(2,2, 0,0);
	mRightUpperSizer = new wxBoxSizer(wxHORIZONTAL);
	mRightMidSizer = new wxFlexGridSizer(2,2, 0,0);

	SetSizer(mMainSizer);
}

SurfacePrefsPane::~SurfacePrefsPane() {
	delete mSurfColor[0];
	delete mSurfColor[1];
}

void SurfacePrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) {
	SurfaceOptions * lSOpts = mTargetPrefs->GetSurfaceOptions();

	mLeftSizer->Add(new wxStaticText(this, wxID_ANY, _T("Surface Colors:")), 0, wxALIGN_CENTER | wxALL, 3);

	mLeftMidSizer->Add(new wxStaticText(this, wxID_ANY, _T("+")), 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_BOTTOM | wxALL, 1);
	mLeftMidSizer->Add(new wxStaticText(this, wxID_ANY, _T("-")), 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_BOTTOM | wxALL, 1);

	RGBColor tmpColor;

	lSOpts->GetPosColor(&tmpColor);
	mSurfColor[0] = new colorArea(this, 0, &tmpColor);
	//  mSurfColor[0]->draw(tmpColor);
	lSOpts->GetNegColor(&tmpColor);
	mSurfColor[1] = new colorArea(this, 1, &tmpColor);
	//  mSurfColor[1]->draw(tmpColor);

	mLeftMidSizer->Add(mSurfColor[0], 0, wxALIGN_TOP | wxALL, 1);
	mLeftMidSizer->Add(mSurfColor[1], 0, wxALIGN_TOP | wxALL, 1);

	mLeftSizer->Add(mLeftMidSizer, 0, wxALIGN_CENTER | wxALL, 3);
	mLeftSizer->Add(10,10);

	mLeftSizer->Add(new wxStaticText(this, wxID_ANY, _T("Def. # of Grid Points")), 0, wxALIGN_LEFT | wxALL, 3);

	mSldGridPoint = new wxSlider(this, ID_NUM_GRID_POINT_SLIDER, 
			lSOpts->GetNumGridPoints(), 10, 150,
			wxDefaultPosition, wxSize(155,wxDefaultCoord),
			wxSL_AUTOTICKS | wxSL_LABELS);

	mLeftSizer->Add(mSldGridPoint, 0, wxALIGN_CENTER | wxALL, 3);

	mRightSizer->Add(new wxStaticText(this, wxID_ANY, _T("Specific to 3D surfaces:")), 0, wxALIGN_LEFT | wxALL, 3);

	mRightUpperSizer->Add(new wxStaticText(this, wxID_ANY, _T("Grid Size:")), 0, wxALIGN_CENTER | wxALL, 3);

	mSldGridSize = new wxSlider(this, ID_3D_GRID_SIZE_SLIDER, 
			(int)(100*(lSOpts->GetGridSize())+0.5), 0, 300,
			wxDefaultPosition, wxSize(155,wxDefaultCoord),
			wxSL_AUTOTICKS | wxSL_LABELS);
	mRightUpperSizer->Add(mSldGridSize, 0, wxALIGN_CENTER | wxALL, 3);
	mRightSizer->Add(mRightUpperSizer);

	mRightSizer->Add(new wxStaticText(this, wxID_ANY, _T("Specific to 2D maps:")), 0, wxALIGN_LEFT | wxALL, 3);

	mRightMidSizer->Add(new wxStaticText(this, wxID_ANY, _T("# of Contours:")), 0, wxALIGN_CENTER | wxALL, 3);

	wxString tmp;
	tmp.Printf(wxT("%ld"), lSOpts->GetNumContours());
	mNumContour = new wxTextCtrl(this, wxID_ANY, tmp);
	mRightMidSizer->Add(mNumContour, 0, wxALIGN_CENTER | wxALL, 3);

	mRightMidSizer->Add(new wxStaticText(this, wxID_ANY, _T("Max contour value")), 0, wxALIGN_CENTER | wxALL, 3);

	tmp.Printf(wxT("%.3f"), lSOpts->GetMaxContour());
	mMaxContourValue = new wxTextCtrl(this, wxID_ANY, tmp);
	mRightMidSizer->Add(mMaxContourValue, 0, wxALIGN_CENTER | wxALL, 3);			       
	mRightSizer->Add(mRightMidSizer);

	mZeroContour = new wxCheckBox(this, ID_SHOW_ZERO_VALUE_CONTOUR, _T("Show Zero-value contour"));
	mZeroContour->SetValue(lSOpts->GetShowZeroContour());
	mAtomPosition = new wxCheckBox(this, ID_SHOW_ATOM_POSITION, _T("Show atom positions"));
	mAtomPosition->SetValue(lSOpts->Get2DHashMarks());

	mRightSizer->Add(mZeroContour,  0, wxALIGN_LEFT | wxALL, 3);
	mRightSizer->Add(mAtomPosition,  0, wxALIGN_LEFT | wxALL, 3);

	mMainSizer->Add(mLeftSizer, wxALIGN_CENTER | wxALL, 10);
	mMainSizer->Add(mRightSizer, wxALIGN_CENTER | wxALL, 10);
}

void SurfacePrefsPane::saveToTempPrefs() {
	SurfaceOptions * lSOpts = mTargetPrefs->GetSurfaceOptions();

	long lv;
	double dv;

	(mNumContour->GetValue()).ToLong(&lv);
	lSOpts->SetNumContours(lv);
	(mMaxContourValue->GetValue()).ToDouble(&dv);
	lSOpts->SetMaxContour(dv);

	RGBColor tmpColor;
	mSurfColor[0]->getColor(&tmpColor);
	lSOpts->SetPosColor(tmpColor);
	mSurfColor[1]->getColor(&tmpColor);
	lSOpts->SetNegColor(tmpColor);
}

void SurfacePrefsPane::OnSliderUpdate(wxCommandEvent& event) {
	SurfaceOptions * lSOpts = mTargetPrefs->GetSurfaceOptions();
	int id = event.GetId();

	if (id == ID_NUM_GRID_POINT_SLIDER)
		lSOpts->SetNumGridPoints(mSldGridPoint->GetValue());
	if (id == ID_3D_GRID_SIZE_SLIDER)
		lSOpts->SetGridSize((float)(mSldGridSize->GetValue()/100));
}

void SurfacePrefsPane::OnCheckBox(wxCommandEvent& event) {
	SurfaceOptions * lSOpts = mTargetPrefs->GetSurfaceOptions();
	int id = event.GetId();

	if (id == ID_SHOW_ZERO_VALUE_CONTOUR)
		lSOpts->SetShowZeroContour(mZeroContour->GetValue());
	if (id == ID_SHOW_ATOM_POSITION)
		lSOpts->Set2DHashMarks(mAtomPosition->GetValue());
}


QD3DPrefsPane::QD3DPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, targetPrefs, kQD3DPrefsPane, GlobalPrefs, parent) {

	mMainSizer = new wxFlexGridSizer(11, 2, 5, 0);
	mMainSizer->AddGrowableCol(0);
	mMainSizer->AddGrowableCol(1);

	SetSizer(mMainSizer);

}

void QD3DPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) {

	int rflags = wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL;
	int lflags = wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT | wxALL;

	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _("Bond Size:")), 0,
			rflags, 3);
	mSld[0] = new wxSlider(this, ID_BOND_SIZE_SLIDER, 
			(int) (mTargetPrefs->GetQD3DBondWidth() * 500 + 0.5),
			1, 100, wxDefaultPosition,
			wxSize(155, wxDefaultCoord));
	mMainSizer->Add(mSld[0], 0, lflags, 3);

	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _("Display Quality:")),
			0, rflags, 3);
	mSld[1] = new wxSlider(this, ID_DISPLAY_QUALITY_SLIDER, 
			(int) (mTargetPrefs->GetQD3DAtomQuality() + 0.5),
			2, 40, wxDefaultPosition,
			wxSize(155, wxDefaultCoord));
	mMainSizer->Add(mSld[1], 0, lflags, 3);

	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _("Fill Light Brightness:")),
			0, rflags, 3);
	mSld[2] = new wxSlider(this, ID_FILL_LIGHT_BRIGHTNESS_SLIDER, 
			(int) (mTargetPrefs->GetQD3DFillBrightness() * 100 + 0.5),
			0, 100, wxDefaultPosition,
			wxSize(155, wxDefaultCoord));
	mMainSizer->Add(mSld[2], 0, lflags, 3);

	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _("Point Light Brightness:")),
			0, rflags, 3);
	mSld[3] = new wxSlider(this, ID_POINT_LIGHT_BRIGHTNESS_SLIDER, 
			(int) (mTargetPrefs->GetQD3DPointBrightness()*100+0.5),
			0, 100, wxDefaultPosition, wxSize(155, wxDefaultCoord));
	mMainSizer->Add(mSld[3], 0, lflags, 3);

	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _("High-Resolution Line Width:")),
			0, rflags, 3);
	mTargetPrefs->CylindersForLines(true);
	mSld[4] = new wxSlider(this, ID_LINE_WIDTH_SLIDER, 
			(int) (mTargetPrefs->GetQD3DLineWidth()*10000+0.5),
			0, 200, wxDefaultPosition, wxSize(155, wxDefaultCoord));
	mTargetPrefs->CylindersForLines(false);
	mMainSizer->Add(mSld[4], 0, lflags, 3);

	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _("3D Perspective:")),
			0, rflags, 3);
	mSld[5] = new wxSlider(this, ID_DEPTH_CUEING_SLIDER, 
			(int)(mTargetPrefs->GetGLFOV()), 
			0, 45, wxDefaultPosition, 
			wxSize(155, wxDefaultCoord));
	mMainSizer->Add(mSld[5], 0, lflags, 3);

	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _("Background color:")),
			0, rflags, 3);
	mBackgrdColor = new colorArea(this, 0, mTargetPrefs->GetBackgroundColorLoc());
	mMainSizer->Add(mBackgrdColor, 0, lflags, 3);

	if (GLEW_VERSION_2_0) {
		mMainSizer->Add(new wxStaticText(this, wxID_ANY,
					    _("Shader Mode:")), 0, rflags, 3);
		wxString choices[3] = {
			_("No shaders"),
			_("Smooth shading"),
			_("Smooth shading and shadows")
		};
		mShaderChooser = new wxChoice(this, ID_SHADER_CHOICE,
									  wxDefaultPosition, wxDefaultSize,
									  3, choices);
		mShaderChooser->SetSelection(mTargetPrefs->GetShaderMode());
		mMainSizer->Add(mShaderChooser, 0, lflags, 3);
	}
	mAutoRotateCheck = new wxCheckBox(this, ID_3D_AUTO_ROTATE_CHECK, _("Enable Auto Rotation"));
	mAutoRotateCheck->SetValue(mTargetPrefs->AutoRotationEnabled());
	mMainSizer->Add(new wxStaticText(this, wxID_ANY, wxT("")), 0, rflags, 3);
	mMainSizer->Add(mAutoRotateCheck, 0, lflags, 3);
	mAutoRotateCheck->SetToolTip(_("Check to enable drag and release in the main display to start auto-rotation"));

	int xv,yv,zv;
	mTargetPrefs->GetAutoRotationVector(xv, yv, zv);
	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _("Rotation speed in X")), 0, rflags, 3);
	wxSpinCtrl  * mspin = new wxSpinCtrl(this, ID_3D_AUTOROTATIONX_SPIN);
	mspin->SetRange(-100, 100);
	mspin->SetValue(xv);
	mMainSizer->Add(mspin, 0, lflags, 3);
	
	mMainSizer->Add(new wxStaticText(this, wxID_ANY, _("Rotation speed in Y")), 0, rflags, 3);
	mspin = new wxSpinCtrl(this, ID_3D_AUTOROTATIONY_SPIN);
	mspin->SetRange(-100, 100);
	mspin->SetValue(yv);
	mMainSizer->Add(mspin, 0, lflags, 3);
}

void QD3DPrefsPane::saveToTempPrefs() {
	mBackgrdColor->getColor(mTargetPrefs->GetBackgroundColorLoc());
	mTargetPrefs->EnableAutoRotation(mAutoRotateCheck->GetValue());
}

void QD3DPrefsPane::OnShaderChoice(wxCommandEvent& event) {
	mTargetPrefs->SetShaderMode(event.GetInt());
}

void QD3DPrefsPane::OnSliderUpdate(wxCommandEvent &event) {
	int id = event.GetId();

	if (id == ID_BOND_SIZE_SLIDER)
		mTargetPrefs->SetQD3DBondWidth((float)(mSld[0]->GetValue())/500);
	if (id == ID_DISPLAY_QUALITY_SLIDER)
		mTargetPrefs->SetQD3DAtomQuality(mSld[1]->GetValue());
	if (id == ID_FILL_LIGHT_BRIGHTNESS_SLIDER)
		mTargetPrefs->SetQD3DFillBrightness((float)(mSld[2]->GetValue())/100);
	if (id == ID_POINT_LIGHT_BRIGHTNESS_SLIDER)
		mTargetPrefs->SetQD3DPointBrightness((float)(mSld[3]->GetValue())/100);
	if (id == ID_LINE_WIDTH_SLIDER)
		mTargetPrefs->SetQD3DLineWidth((float)(mSld[4]->GetValue())/10000);
	if (id == ID_DEPTH_CUEING_SLIDER)
		mTargetPrefs->SetGLFOV((float)(mSld[5]->GetValue()));
}

void QD3DPrefsPane::OnSpinChange(wxSpinEvent & event) {
	int id = event.GetId();
	int val = event.GetPosition();
	
	int xv, yv, zv;
	mTargetPrefs->GetAutoRotationVector(xv, yv, zv);
	if (id == ID_3D_AUTOROTATIONX_SPIN) {
		xv = val;
	} else { //other option is Y
		yv = val;
	}
	mTargetPrefs->SetAutoRotationVector(xv, yv, zv);
}
