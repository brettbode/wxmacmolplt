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

#include "surfaceDlg.h"
#include "colorArea.h"

////@begin XPM images
////@end XPM images

/*!
 * Orbital3D event table definition
 */

/*BEGIN_EVENT_TABLE( Orbital3DSurf, wxPanel )
    EVT_CHOICE( ID_CHOICE1, Orbital3DSurf::OnChoice1Selected )
    EVT_CHECKBOX( ID_CHECKBOX5, Orbital3DSurf::OnCheckbox5Click )
END_EVENT_TABLE()
*/

using namespace std;

IMPLEMENT_CLASS( BaseSurfacePane, wxPanel )
IMPLEMENT_CLASS( Surface3DPane, wxPanel )
IMPLEMENT_CLASS( Orbital3DSurf, wxPanel )

/*!
 * Base class of any Panel
 */

BaseSurfacePane::BaseSurfacePane( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, pos, size, style);
}

BaseSurfacePane::~BaseSurfacePane()
{

}

bool BaseSurfacePane::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
  wxPanel::Create( parent, id, pos, size, style );

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

void BaseSurfacePane::CreateControls()
{
  mainSizer = new wxBoxSizer(wxVERTICAL);
  upperSizer = new wxBoxSizer(wxHORIZONTAL);
  middleSizer = new wxBoxSizer(wxHORIZONTAL);
  bottomSizer = new wxBoxSizer(wxHORIZONTAL);
  leftMiddleSizer = new wxBoxSizer(wxHORIZONTAL);
  rightMiddleSizer = new wxFlexGridSizer(2,2,0,0);
  leftBottomSizer = new wxBoxSizer(wxHORIZONTAL);
  rightBottomSizer = new wxBoxSizer(wxVERTICAL);

  label0 = new wxStaticText( this, wxID_ANY,
                            _T("Select Orbital Set:"),
                            wxDefaultPosition,
                            wxDefaultSize);

  label1 = new wxStaticText( this, wxID_ANY,
                            _T("Number of\n Grid Points:"),
                            wxDefaultPosition,
                            wxDefaultSize);

  wxString choices[] = {_T("for test")};
  mOrbSetChoice = new wxChoice( this, ID_ORB_CHOICE, wxPoint(10,10), wxSize(120,wxDefaultCoord), 1, choices );

  mNumGridPntSld = new wxSlider( this, ID_GRID_POINT_SLIDER, 0, 0, 200,
				 wxDefaultPosition, wxSize(155,wxDefaultCoord),
                             wxSL_AUTOTICKS | wxSL_LABELS);
  //range is set temporarily for testing

  mSetParamBut = new wxButton( this, ID_SET_PARAM_BUT, wxT("Set Parameters"), wxPoint(450, 160) );
  mExportBut = new wxButton( this, ID_SURFACE_EXPORT_BUT, wxT("Export"), wxPoint(450, 160) );
  mUpdateBut = new wxButton( this, ID_SURFACE_UPDATE_BUT, wxT("Update"), wxPoint(450, 160) );

  SetSizer(mainSizer);
}

/*
 * 3D surface dialog class
 */

Surface3DPane::Surface3DPane( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : BaseSurfacePane(parent, id, pos, size, style)
{
    CreateControls();
}

Surface3DPane::~Surface3DPane()
{
  delete mTransColor;
}

void Surface3DPane::CreateControls()
{
  label2 = new wxStaticText( this, wxID_ANY,
                            _T("Grid Size:"),
                            wxDefaultPosition,
                            wxDefaultSize);
  label3 = new wxStaticText( this, wxID_ANY,
                            _T("Contour Value:"),
                            wxDefaultPosition,
                            wxDefaultSize);

  mGridSizeSld = new wxSlider( this, ID_GRID_SIZE_SLIDER, 0, 0, 200,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord),
                             wxSL_AUTOTICKS | wxSL_LABELS);
  mContourValSld = new wxSlider( this, ID_CONTOUR_VALUE_SLIDER, 0, 0, 200,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord),
                             wxSL_AUTOTICKS | wxSL_LABELS);
  //range is set temporarily for testing;

  label4 = new wxStaticText( this, wxID_ANY,
                            _T("Transparency Color:"),
                            wxDefaultPosition,
                            wxDefaultSize);
  mTransColor = new colorArea(this, ID_TRANSPARENCY_COLOR);

  wxString choices[] = {_T("Solid"), _T("Wire Frame")};
  m3DRdoBox = new wxRadioBox( this, ID_3D_RADIOBOX, _T(""), wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices), choices, 1, wxRA_SPECIFY_ROWS );

  mSmoothChkBox = new wxCheckBox( this, ID_SMOOTH_CHECKBOX, _T("Smooth"), wxPoint(340,130), wxDefaultSize );

  mFreeMemBut = new wxButton( this, ID_FREE_MEM_BUT, wxT("Free Mem"), wxPoint(450, 160) );

}

/*!
 * Orbital3D constructors
 */

Orbital3DSurf::Orbital3DSurf( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : Surface3DPane(parent, id, pos, size, style)
{
    CreateControls();
}

Orbital3DSurf::~Orbital3DSurf()
{
  delete mOrbColor1;
  delete mOrbColor2;
}

/*!
 * Control creation for Orbital3D
 */

void Orbital3DSurf::CreateControls()
{    
    upperSizer->Add(label0, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    upperSizer->Add(mOrbSetChoice, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    upperSizer->Add(10,10);
    mRevPhaseChk = new wxCheckBox(this, ID_REVERSE_PHASE_CHECKBOX, _T("Reverse Phase"), wxDefaultPosition);
    upperSizer->Add(mRevPhaseChk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    leftMiddleSizer->Add(label1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    leftMiddleSizer->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    rightMiddleSizer->Add(label2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    rightMiddleSizer->Add(mGridSizeSld, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    rightMiddleSizer->Add(label3, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    rightMiddleSizer->Add(mContourValSld, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    mSubLeftBot1Sizer = new wxBoxSizer(wxVERTICAL);
    mSubLeftBot2Sizer = new wxBoxSizer(wxVERTICAL);
  
    mSubLeftBot1Sizer->Add(new wxStaticText(this, wxID_ANY,
                            _T("Select Orb:"),
                            wxDefaultPosition,
			    wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    wxString choices1[] = {_T("Energy"), _T("Occupation #")};
    mOrbFormatChoice = new wxChoice( this, ID_ORB_FORMAT_CHOICE, wxDefaultPosition, wxSize(120,wxDefaultCoord), 2, choices1 );
    mSubLeftBot1Sizer->Add(mOrbFormatChoice, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    wxString choices2[] = {_T("test")};
    mAtomList = new wxListBox( this, ID_ATOM_LIST,
                               wxDefaultPosition, wxSize(120,180),
                               1, choices2, wxLB_MULTIPLE |wxLB_ALWAYS_SB );

    mSubLeftBot1Sizer->Add(mAtomList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    mSubLeftBot2Sizer->Add(new wxStaticText(this, wxID_ANY,
                            _T("Orbital vector: \n Atom Orbital Coef"),
                            wxDefaultPosition,
                            wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    mOrbCoef = new wxTextCtrl( this, wxID_ANY, _T("Atom Orbital Coef."), wxPoint(20,160), wxSize(120,180) );
    mSubLeftBot2Sizer->Add(mOrbCoef, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    leftBottomSizer->Add(mSubLeftBot1Sizer, 0, wxALL, 5);
    leftBottomSizer->Add(mSubLeftBot2Sizer, 0, wxALL, 5);

    mSubRightBot1Sizer = new wxBoxSizer(wxHORIZONTAL);
    mSubRightBot1Sizer->Add(new wxStaticText(this, wxID_ANY,
                            _T("Orbital Colors:"),
                            wxDefaultPosition,
                            wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    /*!!! temporarily give a color for testing  */

    RGBColor tmpColor;
    tmpColor.red = 0.0;
    tmpColor.green = 0.0;
    tmpColor.blue = 0.0;

    mOrbColor1 = new colorArea(this, ID_ORB_COLOR_POSITIVE);
    mOrbColor1->draw(&tmpColor);
    mOrbColor2 = new colorArea(this, ID_ORB_COLOR_NEGATIVE);
    mOrbColor2->draw(&tmpColor);
    mSubRightBot1Sizer->Add(mOrbColor1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    mSubRightBot1Sizer->Add(mOrbColor2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    mSubRightBot2Sizer = new wxBoxSizer(wxHORIZONTAL);
    mSubRightBot2Sizer->Add(label4, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    mTransColor->draw(&tmpColor);
    mSubRightBot2Sizer->Add(mTransColor, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    mSubRightBot3Sizer = new wxBoxSizer(wxHORIZONTAL);
    mSubRightBot3Sizer->Add(m3DRdoBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    mSubRightBot4Sizer = new wxBoxSizer(wxHORIZONTAL);
    mSubRightBot4Sizer->Add(mSmoothChkBox,0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    mSubRightBot5Sizer = new wxGridSizer(2,2,0,0);
    mSubRightBot5Sizer->Add(mSetParamBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    mSubRightBot5Sizer->Add(mExportBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    mSubRightBot5Sizer->Add(mFreeMemBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    mSubRightBot5Sizer->Add(mUpdateBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    rightBottomSizer->Add(mSubRightBot1Sizer);
    rightBottomSizer->Add(mSubRightBot2Sizer);
    rightBottomSizer->Add(mSubRightBot3Sizer);
    rightBottomSizer->Add(mSubRightBot4Sizer);
    rightBottomSizer->Add(mSubRightBot5Sizer);

    middleSizer->Add(leftMiddleSizer, 0, wxALL, 10);
    middleSizer->Add(rightMiddleSizer, 0, wxALL, 10);
    bottomSizer->Add(leftBottomSizer, 0, wxALL, 3);
    bottomSizer->Add(rightBottomSizer, 0, wxALL, 3);
    mainSizer->Add(upperSizer);
    mainSizer->Add(middleSizer);
    mainSizer->Add(bottomSizer);
}

/*!
 * Should we show tooltips?
 */

bool Orbital3DSurf::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Orbital3DSurf::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin Orbital3D bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end Orbital3D bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Orbital3DSurf::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin Orbital3D icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end Orbital3D icon retrieval
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE1
 */

/*
void Orbital3DSurf::OnChoice1Selected( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE1 in Orbital3D.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE1 in Orbital3D. 
}
*/

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX5
 */
/*
void Orbital3DSurf::OnCheckbox5Click( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX5 in Orbital3D.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX5 in Orbital3D. 
}
*/

