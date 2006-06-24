/*
 Implement preferences...    -Song Li
*/

//Make some useful preferences related definitions

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "PrefsPanes.h"
#endif

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
#define kEPrefsPane		4
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
#define ID_OUTLINE_BONDS 5006
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
#define ID_ACTIVATE_3D_MODE 5032
#define ID_TOTAL_ENERGY 5033
#define ID_MP2_ENERGY 5034
#define ID_POTENTIAL_ENERGY 5035
#define ID_UNIT_RADIOBOX 5036
#define ID_MISC_RADIOBOX 5037

#include "Globals.h"
#include "GlobalExceptions.h"
#include "MyTypes.h"
#include "Prefs.h"
#include "PrefsPanes.h"
#include "colorArea.h"
#include <stdio.h>
#include <string.h>
#include <wx/colordlg.h>

using namespace std;

extern short		gAppResRef;

BEGIN_EVENT_TABLE(BondPrefsPane, wxPanel)
  EVT_CHOICE (ID_BOND_MODE_CHOICE, BondPrefsPane::OnChoice)
  EVT_SLIDER (ID_NORMAL_MODE_SCALING_SLIDER, BondPrefsPane::OnSliderUpdate)
  EVT_CHECKBOX (ID_SHOW_NORMAL_MODE_ANIMATION, BondPrefsPane::OnToggleAnim)
  EVT_CHECKBOX (ID_SHOW_NORMAL_MODE_ROTATION, BondPrefsPane::OnToggleRotation)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(DisplayPrefsPane, wxPanel)
  EVT_RADIOBOX (ID_DISPLAY_MODE_RADIOBOX, DisplayPrefsPane::OnRadio)
  EVT_CHECKBOX (ID_COLOR_BONDS_BY_ATOM_COLOR, DisplayPrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_OUTLINE_BONDS, DisplayPrefsPane::OnCheckBox)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(EnergyPrefsPane, wxPanel)
  EVT_SLIDER (ID_PLOT_BALL_SIZE_SLIDER, EnergyPrefsPane::OnSliderUpdate)
  EVT_RADIOBOX (ID_UNIT_RADIOBOX, EnergyPrefsPane::OnRadio)
  EVT_RADIOBOX (ID_MISC_RADIOBOX, EnergyPrefsPane::OnRadio)
  EVT_CHECKBOX (ID_TOTAL_ENERGY, EnergyPrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_MP2_ENERGY, EnergyPrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_POTENTIAL_ENERGY, EnergyPrefsPane::OnCheckBox)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(FilePrefsPane, wxPanel)
  EVT_CHECKBOX (ID_AUTO_BOND_PASTE_OPEN, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_GUESS_BOND_ORDER, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_PREVENT_HH_BONDS, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_LOOK_HYDROGEN_BONDS, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_SHOW_ANGLES_WHILE_ROTATING, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_USE_MAC_EOL_CHAR, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_PROMPT_SAVE_FILE, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_CREATE_CUSTOM_FILE_ICON, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_CHANGE_FILE_CREATOR, FilePrefsPane::OnCheckBox)
  EVT_SLIDER (ID_AUTO_BOND_TOLERANCE_SLIDER, FilePrefsPane::OnSliderUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(ScalingPrefsPane, wxPanel)
  EVT_SLIDER (ID_ATOM_SIZE_SLIDER, ScalingPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_DEPTH_CUEING_SLIDER, ScalingPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_ANIM_QUALITY_SLIDER, ScalingPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_FRAME_DELAY_SLIDER, ScalingPrefsPane::OnSliderUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(StereoPrefsPane, wxPanel)
  EVT_CHECKBOX (ID_STEREO_ACTIVE, StereoPrefsPane::OnCheckBox)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(SurfacePrefsPane, wxPanel)
  EVT_SLIDER (ID_NUM_GRID_POINT_SLIDER, SurfacePrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_3D_GRID_SIZE_SLIDER, SurfacePrefsPane::OnSliderUpdate)
  EVT_CHECKBOX (ID_SHOW_ZERO_VALUE_CONTOUR, SurfacePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_SHOW_ATOM_POSITION, SurfacePrefsPane::OnCheckBox)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(QD3DPrefsPane, wxPanel)
  EVT_SLIDER (ID_BOND_SIZE_SLIDER, QD3DPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_DISPLAY_QUALITY_SLIDER, QD3DPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_FILL_LIGHT_BRIGHTNESS_SLIDER, QD3DPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_POINT_LIGHT_BRIGHTNESS_SLIDER, QD3DPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_LINE_WIDTH_SLIDER, QD3DPrefsPane::OnSliderUpdate)
  EVT_CHECKBOX (ID_ACTIVATE_3D_MODE, QD3DPrefsPane::OnCheckBox)
END_EVENT_TABLE()

PrefsPane::PrefsPane(MolDisplayWin* targetWindow, WinPrefs* targetPrefs, short PaneID, Boolean GlobalPrefs) 
{ 
  isGlobalPrefs = GlobalPrefs;

  //mTargetPrefs = new WinPrefs;
  mTargetPrefs = targetPrefs;
}

PrefsPane::~PrefsPane() 
{ 
  //delete mTargetPrefs; 
}

AtomPrefsPane::AtomPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean Global) 
  : PrefsPane(targetWindow, targetPrefs, kAtomPrefsPane, Global) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER|wxScrolledWindowStyle );

  mLabels[0] = wxString("Atom");
  mLabels[1] = wxString("#");
  mLabels[2] = wxString("size(pm)");
  mLabels[3] = wxString("mass");
  mLabels[4] = wxString("Color");

  mMainSizer = new wxFlexGridSizer(kMaxAtomTypes, NUM_ATOM_LABELS, 3, 10);

  SetSizer(mMainSizer);

  SetScrollRate( 10, 10 );
  SetVirtualSize( 500, 1000 );
  FitInside();
}

AtomPrefsPane::~AtomPrefsPane()
{
  for ( int i = 0; i < kMaxAtomTypes; i++)
    {
      delete mEleNames[i];
      delete mEleSizes[i];
      delete mEleMasses[i];
      delete mColorArea[i];
    }
}

void AtomPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) 
{
  for ( int i = 0; i < NUM_ATOM_LABELS; i++)
    mMainSizer->Add(new wxStaticText(
                            this,
                            wxID_ANY,
                            mLabels[i],
                            wxDefaultPosition,
                            wxDefaultSize,
                            wxALIGN_CENTER
                           ),
                       0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  //first row

  for ( int i = 0; i < kMaxAtomTypes; i++)
    {
      wxString tmp;

      mTargetPrefs->GetAtomLabel(i, tmp);
      mEleNames[i] = new wxTextCtrl( this, wxID_ANY, tmp, wxDefaultPosition, wxSize(30, 20));
      mMainSizer->Add(mEleNames[i], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

      tmp.Printf("%d", i+1);
      mMainSizer->Add(new wxStaticText(
                            this,
                            wxID_ANY,
                            tmp,
                            wxDefaultPosition,
                            wxDefaultSize,
                            wxALIGN_CENTER
                           ),
                       0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

      tmp.Printf("%d",mTargetPrefs->GetAtomSize(i));
      mEleSizes[i] = new wxTextCtrl( this, wxID_ANY, tmp, wxDefaultPosition, wxSize(40, 20));
      mMainSizer->Add(mEleSizes[i], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

      tmp.Printf("%.2f",mTargetPrefs->GetAtomMass(i));
      mEleMasses[i] = new wxTextCtrl( this, wxID_ANY, tmp, wxDefaultPosition, wxSize(50, 20));
      mMainSizer->Add(mEleMasses[i], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

      mColorArea[i] = new colorArea(this, i);
      mColorArea[i]->draw(mTargetPrefs->GetAtomColorLoc(i));
      mMainSizer->Add(mColorArea[i], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
    }

  mMainSizer->Layout();
}

void AtomPrefsPane::saveToTempPrefs()
{
  long size;
  double mass;

  for ( int i = 0; i < kMaxAtomTypes; i++)
    {
      mTargetPrefs->SetAtomLabel(i, mEleNames[i]->GetValue());
      (mEleSizes[i]->GetValue()).ToLong(&size);
      mTargetPrefs->SetAtomSize(i, size);
      (mEleMasses[i]->GetValue()).ToDouble(&mass);
      mTargetPrefs->SetAtomMass(i, mass);
      mTargetPrefs->SetAtomColor(i, WX2RGB(mColorArea[i]->getColor()));
    }
}

BondPrefsPane::BondPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean Global)
	: PrefsPane(targetWindow, targetPrefs, kBondPrefsPane, Global) 
{
	Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

	mMainSizer = new wxBoxSizer(wxVERTICAL);
	mUpperSizer = new wxBoxSizer(wxVERTICAL);
	mSldSizer = new wxBoxSizer(wxHORIZONTAL);
	mBox = new wxStaticBoxSizer(wxHORIZONTAL, this);
	mInnerSizer = new wxFlexGridSizer(3, 3, 0, 0);
	mLowerSizer = new wxBoxSizer(wxVERTICAL);

	SetSizer(mMainSizer);
}

BondPrefsPane::~BondPrefsPane()
{
  delete mColorArea;
  delete mPatternArea;
  delete mNormColorArea;
  delete mNormPatternArea;
}

void BondPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) 
{
  mChkAnim = new wxCheckBox(this, ID_SHOW_NORMAL_MODE_ANIMATION, _T("Show Normal Mode During Animation"), wxDefaultPosition);
  mChkAnim->SetValue(mTargetPrefs->GetAnimateMode());
  mChkRotation = new wxCheckBox(this, ID_SHOW_NORMAL_MODE_ROTATION, _T("Show Normal Mode During Rotation"), wxDefaultPosition);
  mChkRotation->SetValue(mTargetPrefs->GetRotateMode());

  mSldScale = new wxSlider( this, ID_NORMAL_MODE_SCALING_SLIDER, 
			    (int)(mTargetPrefs->GetVectorScale()*10 - 1), 0, 25,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord));

  mUpperSizer->Add(mChkAnim, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mUpperSizer->Add(mChkRotation, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mSldSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Normal Mode Scaling:")) ,0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSldSizer->Add(mSldScale, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mUpperSizer->Add(mSldSizer);

  wxString choices[] = { _T("Hydrogen Bonds"),
		       _T("Single Bonds"),
		       _T("Double Bonds"),
		       _T("Triple Bonds")
  };

  mBondChoice = new wxChoice(this, ID_BOND_MODE_CHOICE, wxPoint(10,10), wxSize(150,wxDefaultCoord), 4, choices);
  mBondChoice->SetSelection(1);
  mChoiceId = 1;

  mInnerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("")), 0, wxALIGN_RIGHT | wxALL, 3);
  mInnerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Color")), 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mInnerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Pattern")), 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mInnerSizer->Add(mBondChoice, 0, wxALIGN_RIGHT | wxALL, 3);

  mColorArea = new colorArea(this, 1);
  mColorArea->draw(mTargetPrefs->GetBondColorLoc(1));
  mPatternArea = new colorArea(this, 2);
  mPatternArea->draw(mTargetPrefs->GetBondColorLoc(1));
  //!!! don't know where to get patterns, use color 

  mNormColorArea = new colorArea(this, 3);
  mNormColorArea->draw(mTargetPrefs->GetVectorColorLoc());
  mNormPatternArea = new colorArea(this, 4);
  mNormPatternArea->draw(mTargetPrefs->GetVectorColorLoc());

  mInnerSizer->Add(mColorArea, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mInnerSizer->Add(mPatternArea, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

  mInnerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Normal Modes:")), 0, wxALIGN_RIGHT | wxALL, 3);
  mInnerSizer->Add(mNormColorArea, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mInnerSizer->Add(mNormPatternArea, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

  mBox->Add(mInnerSizer);
  mLowerSizer->Add(mBox);

  mMainSizer->Add(mUpperSizer, 0, wxALIGN_CENTER | wxALL, 3);
  mMainSizer->Add(mLowerSizer, 0, wxALIGN_CENTER | wxALL, 3);
}

void BondPrefsPane::saveToTempPrefs()
{
  mTargetPrefs->SetBondColor(mChoiceId, WX2RGB(mColorArea->getColor()));
  mTargetPrefs->SetVectorColor(WX2RGB(mNormColorArea->getColor()));
}

void BondPrefsPane::OnSliderUpdate( wxCommandEvent &WXUNUSED(event) )
{
  mTargetPrefs->SetVectorScale((float)(0.1*(mSldScale->GetValue()+1)));
}

void BondPrefsPane::OnToggleAnim(wxCommandEvent& WXUNUSED(event))
{
  mTargetPrefs->SetAnimateMode(mChkAnim->GetValue());
}

void BondPrefsPane::OnToggleRotation(wxCommandEvent& WXUNUSED(event))
{
  mTargetPrefs->SetRotateMode(mChkRotation->GetValue());
}

void BondPrefsPane::OnChoice( wxCommandEvent &event )
{
  //int localId = -1;

  mTargetPrefs->SetBondColor(mChoiceId, WX2RGB(mColorArea->getColor())); //save the old setting first

  wxString localStr = event.GetString();

  if ( localStr.Cmp(_T("Hydrogen Bonds")) == 0)
    mChoiceId = 0;
  else if ( localStr.Cmp(_T("Single Bonds")) == 0)
    mChoiceId = 1;
  else if ( localStr.Cmp(_T("Double Bonds")) == 0)
    mChoiceId = 2;
  else if ( localStr.Cmp(_T("Triple Bonds")) == 0)
    mChoiceId = 3;

  mColorArea->draw(mTargetPrefs->GetBondColorLoc(mChoiceId));
  mPatternArea->draw(mTargetPrefs->GetBondColorLoc(mChoiceId));
}


DisplayPrefsPane::DisplayPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean Global)
	: PrefsPane(targetWindow, targetPrefs, kDisplayPrefsPane, Global) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER);

  mMainSizer = new wxBoxSizer(wxVERTICAL);
  mUpperSizer = new wxBoxSizer(wxHORIZONTAL);
  mLowerSizer = new wxBoxSizer(wxVERTICAL);

  wxString choices[] = {_T("Ball and Stick"), _T("WireFrame (Bonds only)")};
  
  mRdoBox = new wxRadioBox( this, ID_DISPLAY_MODE_RADIOBOX, _T("Display Mode"), wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices), choices, 1, wxRA_SPECIFY_COLS );
  mChkColor = new wxCheckBox(this, ID_COLOR_BONDS_BY_ATOM_COLOR, _T("Color bonds by atom color"), wxDefaultPosition);
  mChkOutline = new wxCheckBox(this, ID_OUTLINE_BONDS, _T("Outline Bonds"), wxDefaultPosition);

  SetSizer(mMainSizer);
}

#include <iostream>
void DisplayPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) 
{
  if ( mTargetPrefs->DrawBallnStick() && !mTargetPrefs->DrawWireFrame())
    mRdoBox->SetSelection(0);
  else if ( !mTargetPrefs->DrawBallnStick() && mTargetPrefs->DrawWireFrame())
    mRdoBox->SetSelection(1);
  else
    cout<<"Something wrong! Check preference setting!"<<endl;

  mUpperSizer->Add(mRdoBox, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

  mChkColor->SetValue(mTargetPrefs->ColorBondHalves());
  mChkOutline->SetValue(mTargetPrefs->OutLineBonds());
  mLowerSizer->Add(mChkColor, 1, wxALIGN_LEFT | wxALL, 3);
  mLowerSizer->Add(mChkOutline, 1, wxALIGN_LEFT | wxALL, 3);

  mMainSizer->Add(mUpperSizer, 0, wxALIGN_CENTER | wxALL, 3);
  mMainSizer->Add(mLowerSizer, 0, wxALIGN_CENTER | wxALL, 3);
}

void DisplayPrefsPane::saveToTempPrefs()
{

}

void DisplayPrefsPane::OnRadio( wxCommandEvent &event )
{
  wxString tmpStr = event.GetString();

  if (tmpStr.Cmp(_T("Ball and Stick")) == 0)
    {
      mTargetPrefs->DrawBallnStick(true);
      mTargetPrefs->DrawWireFrame(false);
    }
  else if (tmpStr.Cmp(_T("WireFrame (Bonds only)")) == 0)
    {
      mTargetPrefs->DrawWireFrame(true);
      mTargetPrefs->DrawBallnStick(false);
    }
}

void DisplayPrefsPane::OnCheckBox( wxCommandEvent &event)
{
  if (event.GetId() == ID_COLOR_BONDS_BY_ATOM_COLOR)
    mTargetPrefs->ColorBondHalves(mChkColor->GetValue());
  else if (event.GetId() == ID_OUTLINE_BONDS)
    mTargetPrefs->OutLineBonds(mChkOutline->GetValue());
}


EnergyPrefsPane::EnergyPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, targetPrefs, kEPrefsPane, GlobalPrefs) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

  mMainSizer = new wxBoxSizer(wxVERTICAL);
  mUpperSizer = new wxBoxSizer(wxHORIZONTAL);

  if (PrefsAreGlobal())
    mLowerSizer = new wxGridSizer(4,2);
  else
    mLowerSizer = new wxGridSizer(6,2);

  if (!PrefsAreGlobal())
    {
      mBottomSizer = new wxBoxSizer(wxHORIZONTAL);
      mLeftBottomSizer = new wxBoxSizer(wxVERTICAL);
    }

  mRightBottomSizer = NULL;
  mRight1BottomSizer = NULL;
  mRight2BottomSizer = NULL;
  mAtomText[0] = NULL;
  mAtomText[1] = NULL;
  mAtomText[2] = NULL;

  SetSizer(mMainSizer);

}

EnergyPrefsPane::~EnergyPrefsPane()
{
  delete mEColor;
  delete mMPColor;
  delete mPEColor;
  delete mOtherColor;
}

void EnergyPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) 
{
  EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();
  GraphOptions * lPOpts = mTargetPrefs->GetGraphOptions();
  //Setup the slider for the E plot point size

  mSldBallSize = new wxSlider( this, ID_PLOT_BALL_SIZE_SLIDER, 
			    (int)(lEOpts->GetEPlotPointSize()), 1, 20,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord),
                             wxSL_AUTOTICKS | wxSL_LABELS);

  wxString tmp;
  tmp.Printf("%d", lEOpts->GetNumDigits());
  mNumDigitsArea = new wxTextCtrl( this, wxID_ANY, tmp, wxDefaultPosition, wxDefaultSize);

  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, wxString("E. Plot Ball Size:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mUpperSizer->Add(mSldBallSize, 0, wxALIGN_LEFT | wxALL, 3);
  mUpperSizer->Add(30, 30);
  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, wxString("# digits")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mUpperSizer->Add(mNumDigitsArea, 0, wxALIGN_CENTER | wxALL, 3);

  mEColor = new colorArea(this, 0);
  mEColor->draw(lEOpts->GetTEColor());
  mMPColor = new colorArea(this, 1);
  mMPColor->draw(lEOpts->GetMPColor());
  mPEColor = new colorArea(this, 2);
  mPEColor->draw(lEOpts->GetPEColor());
  mOtherColor = new colorArea(this, 3);
  mOtherColor->draw(lEOpts->GetKEColor());

  mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("E Color")), 0, wxALIGN_RIGHT | wxALL, 3);
  mLowerSizer->Add(mEColor, 0, wxALIGN_LEFT | wxALL, 3);
  mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("MP Color")), 0, wxALIGN_RIGHT | wxALL, 3);
  mLowerSizer->Add(mMPColor, 0, wxALIGN_LEFT | wxALL, 3);

  if (!PrefsAreGlobal())
    {
      mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Y1 zero pt ")), 0, wxALIGN_RIGHT | wxALL, 3);
      tmp.Printf("%.4f",lEOpts->GetY1Zero());
      mY1pt = new wxTextCtrl(this, wxID_ANY, tmp);
      mLowerSizer->Add(mY1pt, 0, wxALIGN_RIGHT | wxALL, 3);
    }

  mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("PE Color")), 0, wxALIGN_RIGHT | wxALL, 3);
  mLowerSizer->Add(mPEColor, 0, wxALIGN_LEFT | wxALL, 3);
  mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Other Color")), 0, wxALIGN_RIGHT | wxALL, 3);
  mLowerSizer->Add(mOtherColor, 0, wxALIGN_LEFT | wxALL, 3);

  if (!PrefsAreGlobal())
    {
      mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Y2 zero pt ")), 0, wxALIGN_RIGHT | wxALL, 3);
      tmp.Printf("%.4f",lEOpts->GetY2Zero());
      mY2pt = new wxTextCtrl(this, wxID_ANY, tmp);
      mLowerSizer->Add(mY2pt, 0, wxALIGN_RIGHT | wxALL, 3);
    }

  if (!PrefsAreGlobal())
    {
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
  
      mRdoUnit = new wxRadioBox( this, ID_UNIT_RADIOBOX, _T("Unit"), wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices), choices, 1, wxRA_SPECIFY_COLS );

      if (lunit == kDefault)
	mRdoUnit->SetSelection(0);
      else if (lunit == kKCalPerMole)
	mRdoUnit->SetSelection(1);

      wxString choices2[] = {_T("None"), _T("Kinetic Energy"), _T("RMS Grad"), _T("Max. Grad"), _T("Bond Length"), _T("Bond Angle") };
  
      mRdoMisc = new wxRadioBox( this, ID_MISC_RADIOBOX, _T(""), wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices2), choices2, 1, wxRA_SPECIFY_COLS );

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

      if (lPOpts->PlotBondAngle())
	{
	  mRightBottomSizer->Show(mRight1BottomSizer, true, true);
	  mRightBottomSizer->Show(mRight2BottomSizer, true, true);
	  mRightBottomSizer->Layout();
	}

      if (lPOpts->PlotBondLength())
	{
	  mRightBottomSizer->Show(mRight1BottomSizer, true, true);
	  mRightBottomSizer->Layout();
	}

    }

  mMainSizer->Add(mUpperSizer);
  mMainSizer->Add(mLowerSizer);

  if (!PrefsAreGlobal())
    mMainSizer->Add(mBottomSizer, 0, wxALL, 10);

}

void EnergyPrefsPane::setHiddenCtrls()
{
  GraphOptions * lPOpts = mTargetPrefs->GetGraphOptions();

  if (!mRightBottomSizer)
    mRightBottomSizer = new wxBoxSizer(wxVERTICAL);

  if (!mRight1BottomSizer)
    mRight1BottomSizer = new wxGridSizer(2,3);

  if (!mRight2BottomSizer)
    mRight2BottomSizer = new wxGridSizer(2,1);

  wxString tmp;
  if (!mAtomText[0])
    {
      tmp.Printf("%d", lPOpts->Get1stAtom()+1);
      mAtomText[0] = new wxTextCtrl(this, wxID_ANY, tmp);
    }
  if (!mAtomText[1])
    {
      tmp.Printf("%d", lPOpts->Get2ndAtom()+1);
      mAtomText[1] = new wxTextCtrl(this, wxID_ANY, tmp);
    }

  mRight1BottomSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Atom 1:")), 0, wxALIGN_CENTER | wxALIGN_RIGHT | wxALL, 3);
  mRight1BottomSizer->Add(mAtomText[0], wxALIGN_CENTER | wxALL, 3);
  mRight1BottomSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Atom 2:")), 0, wxALIGN_CENTER | wxALIGN_RIGHT | wxALL, 3);
  mRight1BottomSizer->Add(mAtomText[1], wxALIGN_CENTER | wxALL, 3);

  if (!mAtomText[2])
    {
      tmp.Printf("%d", lPOpts->Get3rdAtom()+1);
      mAtomText[2] = new wxTextCtrl(this, wxID_ANY, tmp);
    }
  mRight2BottomSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Atom 3:")), 0, wxALIGN_CENTER | wxALIGN_RIGHT | wxALL, 3);
  mRight2BottomSizer->Add(mAtomText[2], wxALIGN_CENTER | wxALL, 3);
	  
  mRightBottomSizer->Add(mRight1BottomSizer);
  mRightBottomSizer->Add(mRight2BottomSizer);
  mBottomSizer->Add(mRightBottomSizer);
}

void EnergyPrefsPane::saveToTempPrefs()
{
  EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();
  GraphOptions * lPOpts = mTargetPrefs->GetGraphOptions();

  long localLong;
  double localDouble;

  (mNumDigitsArea->GetValue()).ToLong(&localLong);
  lEOpts->SetNumDigits(localLong);

  lEOpts->SetTEColor(WX2RGB(mEColor->getColor()));
  lEOpts->SetMPColor(WX2RGB(mMPColor->getColor()));
  lEOpts->SetKEColor(WX2RGB(mOtherColor->getColor()));
  lEOpts->SetPEColor(WX2RGB(mPEColor->getColor()));

  if (!PrefsAreGlobal())
    {
      (mY1pt->GetValue()).ToDouble(&localDouble);
      lEOpts->SetY1Zero(localDouble);
      (mY2pt->GetValue()).ToDouble(&localDouble);
      lEOpts->SetY2Zero(localDouble);

      if (lPOpts->PlotBondLength() || lPOpts->PlotBondAngle())
	{
	  (mAtomText[0]->GetValue()).ToLong(&localLong);
	  lPOpts->Set1stAtom(localLong-1);
	  (mAtomText[1]->GetValue()).ToLong(&localLong);
	  lPOpts->Set2ndAtom(localLong-1);

	  if (lPOpts->PlotBondAngle())
	    {
	      (mAtomText[2]->GetValue()).ToLong(&localLong);
	      lPOpts->Set3rdAtom(localLong-1);
	    }
	}
    }
}

void EnergyPrefsPane::OnSliderUpdate( wxCommandEvent &WXUNUSED(event) )
{
  EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();
  lEOpts->SetEPlotPointSize(mSldBallSize->GetValue());
}

void EnergyPrefsPane::OnCheckBox( wxCommandEvent& event)
{
  int id = event.GetId();

  EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();

  if ( id == ID_TOTAL_ENERGY )
    lEOpts->SetPlotEnergy(mTotalEny->GetValue());
  if ( id == ID_MP2_ENERGY )
    lEOpts->SetPlotMPEnergy(mMP2Eny->GetValue());
  if ( id == ID_POTENTIAL_ENERGY )
    lEOpts->SetPlotPEnergy(mPotEny->GetValue());
}

void EnergyPrefsPane::OnRadio( wxCommandEvent& event )
{
  //int id = event.GetId();

  wxString tmpStr = event.GetString();

  mRightBottomSizer->Hide(mRight1BottomSizer);
  mRightBottomSizer->Hide(mRight2BottomSizer);

  if ( tmpStr.Cmp(_T("Bond Length")) == 0 || tmpStr.Cmp(_T("Bond Angle")) == 0)
    {
      mRightBottomSizer->Show(mRight1BottomSizer, true, true);

      if (tmpStr.Cmp(_T("Bond Angle")) == 0 )
	mRightBottomSizer->Show(mRight2BottomSizer, true, true);

      mRightBottomSizer->Layout();
    }

  EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();
  GraphOptions * lPOpts = mTargetPrefs->GetGraphOptions();

  if (event.GetId() == ID_UNIT_RADIOBOX)
    {
      EnergyUnit lunit;

      if (tmpStr.Cmp(_T("Default")) == 0)
	lunit = kDefault;
      else if (tmpStr.Cmp(_T("kcal/mol")) == 0)
	lunit = kKCalPerMole;

      lEOpts->SetDisplayUnits(lunit);
    }

  if (event.GetId() == ID_MISC_RADIOBOX)
    {
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
	: PrefsPane(targetWindow, targetPrefs, kFilePrefsPane, GlobalPrefs) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

  mMainSizer = new wxBoxSizer(wxVERTICAL);
  mUpperSizer = new wxBoxSizer(wxVERTICAL);
  mSecondSizer = new wxFlexGridSizer(2,3);
  mMiddleSizer = new wxFlexGridSizer(2,3);
  mLowerSizer = new wxBoxSizer(wxHORIZONTAL);
  mBottomSizer = new wxBoxSizer(wxHORIZONTAL);

  SetSizer(mMainSizer);
}

void FilePrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) 
{
  mChkBox[0] = new wxCheckBox(this, ID_AUTO_BOND_PASTE_OPEN, _T("Auto-Bond on Paste and File Open"));
  mChkBox[0]->SetValue(mTargetPrefs->GetAutoBond());

  mChkBox[1] = new wxCheckBox(this, ID_GUESS_BOND_ORDER, _T("Guess bond order"));
  mChkBox[1]->SetValue(mTargetPrefs->DetermineBondOrder());

  mChkBox[2] = new wxCheckBox(this, ID_PREVENT_HH_BONDS, _T("Prevent H-H bonds in Auto-Bond"));
  mChkBox[2]->SetValue(mTargetPrefs->GetHHBondFlag());

  mChkBox[3] = new wxCheckBox(this, ID_LOOK_HYDROGEN_BONDS, _T("Look for hydrogen bonds"));
  mChkBox[3]->SetValue(mTargetPrefs->AllowHydrogenBonds());
	
  mChkBox[4] = new wxCheckBox(this, ID_SHOW_ANGLES_WHILE_ROTATING, _T("Show angles while rotating"));
  mChkBox[4]->SetValue(mTargetPrefs->GetShowAngles());

#ifdef __WXMAC__
if (PrefsAreGlobal())
    {
      mChkBox[5] = new wxCheckBox(this, ID_USE_MAC_EOL_CHAR, _T("Use Mac EOL chars"));
      mChkBox[5]->SetValue(mTargetPrefs->NativeEOLChar());
    }
#endif

  mChkBox[6] = new wxCheckBox(this, ID_PROMPT_SAVE_FILE, _T("Prompt to save file"));
  mChkBox[6]->SetValue(mTargetPrefs->GetPrompt4Save());


  mChkBox[7] = new wxCheckBox(this, ID_CREATE_CUSTOM_FILE_ICON, _T("create custom file icon"));
  mChkBox[7]->SetValue(mTargetPrefs->CreateCustomIcon());

#ifdef __WXMAC__
  if (PrefsAreGlobal())
    {
      mChkBox[8] = new wxCheckBox(this, ID_CHANGE_FILE_CREATOR, _T("Change file creator type to wxMacMolPlt"));
      mChkBox[8]->SetValue(mTargetPrefs->ChangeFileType());
    }
#endif

  mUpperSizer->Add(mChkBox[0], 0, wxALIGN_LEFT | wxALL, 3);

  mSecondSizer->Add(30, 30);
  mSecondSizer->Add(mChkBox[1], 0, wxALIGN_LEFT | wxALL, 3);
  mSecondSizer->Add(30, 30);
  mSecondSizer->Add(mChkBox[2], 0, wxALIGN_LEFT | wxALL, 3);
  mSecondSizer->Add(30, 30);
  mSecondSizer->Add(mChkBox[3], 0, wxALIGN_LEFT | wxALL, 3);

  mMiddleSizer->Add(mChkBox[4], 0, wxALIGN_LEFT | wxALL, 3);

#ifdef __WXMAC__
  if (PrefsAreGlobal())
    mMiddleSizer->Add(mChkBox[5], 0, wxALIGN_LEFT | wxALL, 3);
#endif

  mMiddleSizer->Add(mChkBox[6], 0, wxALIGN_LEFT | wxALL, 3);
  mMiddleSizer->Add(mChkBox[7], 0, wxALIGN_LEFT | wxALL, 3);

#ifdef __WXMAC__
  if (PrefsAreGlobal())
    mMiddleSizer->Add(mChkBox[8], 0, wxALIGN_LEFT | wxALL, 3);
#endif

  mSldTol = new wxSlider( this, ID_AUTO_BOND_TOLERANCE_SLIDER, 
			  (int)(mTargetPrefs->GetAutoBondScale()*10000+0.5), 
			  50, 150, wxDefaultPosition, 
			  wxSize(155,wxDefaultCoord));

  mLowerSizer->Add(new wxStaticText(this, wxID_ANY, _T("Auto-Bond Tolerance:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mLowerSizer->Add(mSldTol, 0, wxALIGN_LEFT | wxALL, 3);

  wxString tmp;
  tmp.Printf("%d", mTargetPrefs->GetDRCSkip());
  mPointSkip = new wxTextCtrl(this, wxID_ANY, tmp);

  mBottomSizer->Add(new wxStaticText(this, wxID_ANY, _T("When reading a DRC file how many\n points should be skipped between\n points read in? (0 reads every point)")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mBottomSizer->Add(mPointSkip, 0, wxALIGN_CENTER | wxALL, 3);

  mMainSizer->Add(mUpperSizer);
  mMainSizer->Add(mSecondSizer);
  mMainSizer->Add(mMiddleSizer);
  mMainSizer->Add(mLowerSizer);
  mMainSizer->Add(mBottomSizer);

}

void FilePrefsPane::saveToTempPrefs()
{
  long v;
  (mPointSkip->GetValue()).ToLong(&v);
  mTargetPrefs->SetDRCSkip(v);
}

void FilePrefsPane::OnCheckBox( wxCommandEvent &event)
{
  if (event.GetId() == ID_AUTO_BOND_PASTE_OPEN)
    {
      Boolean mainStatus = mChkBox[0]->GetValue();
      mTargetPrefs->SetAutoBond(mainStatus);

      if (!mainStatus)
	{
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

void FilePrefsPane::OnSliderUpdate( wxCommandEvent &WXUNUSED(event) )
{
  mTargetPrefs->SetAutoBondScale((float)(mSldTol->GetValue())/10000);
}


ScalingPrefsPane::ScalingPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, targetPrefs, kScalingPrefsPane, GlobalPrefs) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

  mMainSizer = new wxFlexGridSizer(2,4);

  SetSizer(mMainSizer);
}

void ScalingPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) 
{
  mSld[0] = new wxSlider( this, ID_ATOM_SIZE_SLIDER, 
			  (int)(mTargetPrefs->GetAtomScale()*10000+0.5), 
			  0, 250, wxDefaultPosition, 
			  wxSize(155,wxDefaultCoord));

  mSld[1] = new wxSlider( this, ID_DEPTH_CUEING_SLIDER, 
			  (int)(mTargetPrefs->GetZScale()*100+0.5), 
			  0, 70, wxDefaultPosition, 
			  wxSize(155,wxDefaultCoord));

  mSld[2] = new wxSlider( this, ID_ANIM_QUALITY_SLIDER, 
			  (int)(mTargetPrefs->GetAnimationSpeed()-1), 
			  0, 15, wxDefaultPosition, 
			  wxSize(155,wxDefaultCoord));

  mSld[3] = new wxSlider( this, ID_FRAME_DELAY_SLIDER, 
			  (int)(mTargetPrefs->GetAnimateTime()), 
			  0, 120, wxDefaultPosition, 
			  wxSize(155,wxDefaultCoord));

  mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Atom Size:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mMainSizer->Add(mSld[0], 0, wxALIGN_LEFT | wxALL, 3);
  mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Depth Cueing:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mMainSizer->Add(mSld[1], 0, wxALIGN_LEFT | wxALL, 3);
  mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Mode Animation Quality:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mMainSizer->Add(mSld[2], 0, wxALIGN_LEFT | wxALL, 3);
  mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Frame Delay:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mMainSizer->Add(mSld[3], 0, wxALIGN_LEFT | wxALL, 3);
}

void ScalingPrefsPane::saveToTempPrefs()
{

}

void ScalingPrefsPane::OnSliderUpdate( wxCommandEvent &event )
{
  int id = event.GetId();

  if (id == ID_ATOM_SIZE_SLIDER)
    mTargetPrefs->SetAtomScale((float)(mSld[0]->GetValue())/10000);
  if (id == ID_DEPTH_CUEING_SLIDER)
    mTargetPrefs->SetZScale((float)(mSld[1]->GetValue())/100);
  if (id == ID_ANIM_QUALITY_SLIDER)
    mTargetPrefs->SetAnimationSpeed(mSld[2]->GetValue()+1);
  if (id == ID_FRAME_DELAY_SLIDER)
    mTargetPrefs->SetAnimateTime(mSld[3]->GetValue());
}

StereoPrefsPane::StereoPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, targetPrefs, kStereoPrefsPane, GlobalPrefs) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

  mMainSizer = new wxBoxSizer(wxVERTICAL);
  mMiddleSizer = new wxBoxSizer(wxHORIZONTAL);

  SetSizer(mMainSizer);
}

void StereoPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) 
{
  mChkActive = new wxCheckBox(this, ID_STEREO_ACTIVE, _T("Stereo Active"), wxDefaultPosition);
  mChkActive->SetValue(mTargetPrefs->UseStereo());
  mMainSizer->Add(mChkActive, 0, wxALIGN_LEFT | wxALL, 10);
  mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Enter the number of degrees of offset between the stereo images.")), 0, wxALIGN_LEFT | wxALL, 10);

  mMiddleSizer->Add(new wxStaticText(this, wxID_ANY, _T("Offset (degrees) ")), 0, wxALIGN_LEFT | wxALL, 10);

  wxString tmp;
  tmp.Printf("%d", mTargetPrefs->GetStereoOffset());
  mOffDegree = new wxTextCtrl( this, wxID_ANY, tmp);

  mMiddleSizer->Add(mOffDegree, 0, wxALIGN_LEFT | wxALL, 3);

  mMainSizer->Add(mMiddleSizer, 0, wxALIGN_LEFT | wxLEFT, 10);
  mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Hint: Enter a + number for cross-eyed viewing\n or a - number for straight on viewing.")), 0, wxALIGN_LEFT | wxLEFT, 10);

}

void StereoPrefsPane::saveToTempPrefs()
{
  long v;
  (mOffDegree->GetValue()).ToLong(&v);
  mTargetPrefs->SetStereoOffset(v);
}

void StereoPrefsPane::OnCheckBox( wxCommandEvent& WXUNUSED(event))
{
  mTargetPrefs->UseStereo(mChkActive->GetValue());
}


SurfacePrefsPane::SurfacePrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, targetPrefs, kSurfacePrefsPane, GlobalPrefs) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

  mMainSizer = new wxBoxSizer(wxHORIZONTAL);
  mLeftSizer = new wxBoxSizer(wxVERTICAL);
  mRightSizer = new wxBoxSizer(wxVERTICAL);
  mLeftMidSizer = new wxGridSizer(2,2);
  mRightUpperSizer = new wxBoxSizer(wxHORIZONTAL);
  mRightMidSizer = new wxFlexGridSizer(2,2);

  SetSizer(mMainSizer);
}

SurfacePrefsPane::~SurfacePrefsPane()
{
  delete mSurfColor[0];
  delete mSurfColor[1];
}

void SurfacePrefsPane::SetupPaneItems( MolDisplayWin* targetWindow) 
{
  SurfaceOptions * lSOpts = mTargetPrefs->GetSurfaceOptions();

  mLeftSizer->Add(new wxStaticText(this, wxID_ANY, _T("Surface Colors:")), 0, wxALIGN_CENTER | wxALL, 3);

  mLeftMidSizer->Add(new wxStaticText(this, wxID_ANY, _T("+")), 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_BOTTOM | wxALL, 1);
  mLeftMidSizer->Add(new wxStaticText(this, wxID_ANY, _T("-")), 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_BOTTOM | wxALL, 1);

  RGBColor* tmpColor = new RGBColor();

  mSurfColor[0] = new colorArea(this, 0);
  lSOpts->GetPosColor(tmpColor);
  mSurfColor[0]->draw(tmpColor);
  mSurfColor[1] = new colorArea(this, 1);
  lSOpts->GetNegColor(tmpColor);
  mSurfColor[1]->draw(tmpColor);

  mLeftMidSizer->Add(mSurfColor[0], 0, wxALIGN_TOP | wxALL, 1);
  mLeftMidSizer->Add(mSurfColor[1], 0, wxALIGN_TOP | wxALL, 1);

  mLeftSizer->Add(mLeftMidSizer, 0, wxALIGN_CENTER | wxALL, 3);
  mLeftSizer->Add(10,10);

  mLeftSizer->Add(new wxStaticText(this, wxID_ANY, _T("Def. # of Grid Points")), 0, wxALIGN_LEFT | wxALL, 3);

  mSldGridPoint = new wxSlider( this, ID_NUM_GRID_POINT_SLIDER, 
			    lSOpts->GetNumGridPoints(), 10, 150,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord),
                             wxSL_AUTOTICKS | wxSL_LABELS);

  mLeftSizer->Add(mSldGridPoint, 0, wxALIGN_CENTER | wxALL, 3);

  mRightSizer->Add(new wxStaticText(this, wxID_ANY, _T("Specific to 3D surfaces:")), 0, wxALIGN_LEFT | wxALL, 3);

  mRightUpperSizer->Add(new wxStaticText(this, wxID_ANY, _T("Grid Size:")), 0, wxALIGN_CENTER | wxALL, 3);

  mSldGridSize = new wxSlider( this, ID_3D_GRID_SIZE_SLIDER, 
			    (int)(100*(lSOpts->GetGridSize())+0.5), 0, 300,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord),
                             wxSL_AUTOTICKS | wxSL_LABELS);
  mRightUpperSizer->Add(mSldGridSize, 0, wxALIGN_CENTER | wxALL, 3);
  mRightSizer->Add(mRightUpperSizer);

  mRightSizer->Add(new wxStaticText(this, wxID_ANY, _T("Specific to 2D maps:")), 0, wxALIGN_LEFT | wxALL, 3);

  mRightMidSizer->Add(new wxStaticText(this, wxID_ANY, _T("# of Contours:")), 0, wxALIGN_CENTER | wxALL, 3);
  
  wxString tmp;
  tmp.Printf("%d", lSOpts->GetNumContours());
  mNumContour = new wxTextCtrl(this, wxID_ANY, tmp);
  mRightMidSizer->Add(mNumContour, 0, wxALIGN_CENTER | wxALL, 3);

 mRightMidSizer->Add(new wxStaticText(this, wxID_ANY, _T("Max contour value")), 0, wxALIGN_CENTER | wxALL, 3);

  tmp.Printf("%.3f", lSOpts->GetMaxContour());
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

void SurfacePrefsPane::saveToTempPrefs()
{
  SurfaceOptions * lSOpts = mTargetPrefs->GetSurfaceOptions();

  long lv;
  double dv;

  (mNumContour->GetValue()).ToLong(&lv);
  lSOpts->SetNumContours(lv);
  (mMaxContourValue->GetValue()).ToDouble(&dv);
  lSOpts->SetMaxContour(dv);

  lSOpts->SetPosColor(WX2RGB(mSurfColor[0]->getColor()));
  lSOpts->SetNegColor(WX2RGB(mSurfColor[1]->getColor()));
}

void SurfacePrefsPane::OnSliderUpdate( wxCommandEvent& event)
{
  SurfaceOptions * lSOpts = mTargetPrefs->GetSurfaceOptions();
  int id = event.GetId();

  if ( id == ID_NUM_GRID_POINT_SLIDER )
    lSOpts->SetNumGridPoints(mSldGridPoint->GetValue());
  if ( id == ID_3D_GRID_SIZE_SLIDER )
    lSOpts->SetGridSize((float)(mSldGridSize->GetValue()/100));
}

void SurfacePrefsPane::OnCheckBox( wxCommandEvent& event)
{
  SurfaceOptions * lSOpts = mTargetPrefs->GetSurfaceOptions();
  int id = event.GetId();

  if ( id == ID_SHOW_ZERO_VALUE_CONTOUR )
    lSOpts->SetShowZeroContour(mZeroContour->GetValue());
  if ( id == ID_SHOW_ATOM_POSITION )
    lSOpts->Set2DHashMarks(mAtomPosition->GetValue());
}


QD3DPrefsPane::QD3DPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, WinPrefs* targetPrefs, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, targetPrefs, kQD3DPrefsPane, GlobalPrefs) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

  mMainSizer = new wxBoxSizer(wxVERTICAL);
  mUpperSizer = new wxGridSizer(2,5);
  mLowerSizer = new wxBoxSizer(wxHORIZONTAL);

  SetSizer(mMainSizer);
}

void QD3DPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow) 
{
  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, _T("Bond Size:")), 0, wxALIGN_CENTER | wxALL, 3);
  mSld[0] = new wxSlider( this, ID_BOND_SIZE_SLIDER, 
			    (int)(mTargetPrefs->GetQD3DBondWidth()*500+0.5), 1, 100,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord));
  mUpperSizer->Add(mSld[0], 0, wxALIGN_CENTER | wxALL, 3);

  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, _T("Display Quality:")), 0, wxALIGN_CENTER | wxALL, 3);
  mSld[1] = new wxSlider( this, ID_DISPLAY_QUALITY_SLIDER, 
			    (int)(mTargetPrefs->GetQD3DAtomQuality()+0.5), 2, 40,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord));
  mUpperSizer->Add(mSld[1], 0, wxALIGN_CENTER | wxALL, 3);

  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, _T("Fill Light Brightness:")), 0, wxALIGN_CENTER | wxALL, 3);
  mSld[2] = new wxSlider( this, ID_FILL_LIGHT_BRIGHTNESS_SLIDER, 
		       (int)(mTargetPrefs->GetQD3DFillBrightness()*100+0.5), 0, 100,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord));
  mUpperSizer->Add(mSld[2], 0, wxALIGN_CENTER | wxALL, 3);

  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, _T("Point Light Brightness:")), 0, wxALIGN_CENTER | wxALL, 3);
  mSld[3] = new wxSlider( this, ID_POINT_LIGHT_BRIGHTNESS_SLIDER, 
		      (int)(mTargetPrefs->GetQD3DPointBrightness()*100+0.5), 0, 100,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord));
  mUpperSizer->Add(mSld[3], 0, wxALIGN_CENTER | wxALL, 3);

  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, _T("High-Resolution Line Width:")), 0, wxALIGN_CENTER | wxALL, 3);
  mTargetPrefs->CylindersForLines(true);
  mSld[4] = new wxSlider( this, ID_LINE_WIDTH_SLIDER, 
		      (int)(mTargetPrefs->GetQD3DLineWidth()*10000+0.5), 0, 200,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord));
  mTargetPrefs->CylindersForLines(false);
  mUpperSizer->Add(mSld[4], 0, wxALIGN_CENTER | wxALL, 3);

  mMainSizer->Add(mUpperSizer, 0, wxALIGN_CENTER | wxALL, 3 );

  /*
  if (PrefsAreGlobal())
    {
      mChk3D = new wxCheckBox( this, ID_ACTIVATE_3D_MODE, _T("Activate 3D mode by default"));
      mChk3D->SetValue(mTargetPrefs->Default3DOn());
      mMainSizer->Add(mChk3D, 0, wxALIGN_CENTER | wxALL, 3);
    }
  */ //There is no 2-D mode for now
	
  mLowerSizer->Add(new wxStaticText(this, wxID_ANY, _T("Background color:")), 0, wxALIGN_CENTER | wxALL, 3);
  mBackgrdColor = new colorArea(this, 0);
  mBackgrdColor->draw(mTargetPrefs->GetBackgroundColorLoc());
  mLowerSizer->Add(mBackgrdColor, 0, wxALIGN_CENTER | wxALL, 3);

  mMainSizer->Add(mLowerSizer, 0, wxALIGN_CENTER | wxALL, 3);

}

void QD3DPrefsPane::saveToTempPrefs()
{
  mTargetPrefs->SetBackgroundColorLoc(WX2RGB(mBackgrdColor->getColor()));
}

void QD3DPrefsPane::OnCheckBox( wxCommandEvent& WXUNUSED(event))
{
  mTargetPrefs->Default3DOn(mChk3D->GetValue());
}

void QD3DPrefsPane::OnSliderUpdate( wxCommandEvent &event )
{
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
}
