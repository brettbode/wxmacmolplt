/////////////////////////////////////////////////////////////////////////////
// Name:        windowparameters.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Wed 28 Jun 13:01:36 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "windowparameters.h"
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
#include "MolDisplayWin.h"
#include "windowparameters.h"

////@begin XPM images
////@end XPM images

/*!
 * windowparameters type definition
 */

IMPLEMENT_DYNAMIC_CLASS( windowparameters, wxDialog )

/*!
 * windowparameters event table definition
 */

BEGIN_EVENT_TABLE( windowparameters, wxDialog )

////@begin windowparameters event table entries
    EVT_RADIOBOX( ID_UNITSRADIOBOX, windowparameters::OnUnitsradioboxSelected )

    EVT_BUTTON( wxID_APPLY, windowparameters::OnApplyClick )

    EVT_BUTTON( wxID_OK, windowparameters::OnOkClick )

////@end windowparameters event table entries

END_EVENT_TABLE()

/*!
 * windowparameters constructors
 */

windowparameters::windowparameters( )
{
}

windowparameters::windowparameters( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * windowparameters creator
 */

bool windowparameters::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin windowparameters member initialisation
    mXWinSize = NULL;
    mYWinSize = NULL;
    mUnitRadios = NULL;
    mWindowScale = NULL;
    mXWinCenter = NULL;
    mYWinCenter = NULL;
    mZWinCenter = NULL;
    mPsiEdit = NULL;
    mPhiEdit = NULL;
    mThetaEdit = NULL;
////@end windowparameters member initialisation

////@begin windowparameters creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end windowparameters creation
    return true;
}

/*!
 * Control creation for windowparameters
 */

void windowparameters::CreateControls()
{    
////@begin windowparameters content construction
    windowparameters* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Window Size (screen space):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer4->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mXWinSize = new wxTextCtrl( itemDialog1, ID_XWINSIZEEDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(mXWinSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYWinSize = new wxTextCtrl( itemDialog1, ID_YWINSIZE, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(mYWinSize, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxString mUnitRadiosStrings[] = {
        _("&pixels"),
        _("&inches"),
        _("&cm")
    };
    mUnitRadios = new wxRadioBox( itemDialog1, ID_UNITSRADIOBOX, _("units"), wxDefaultPosition, wxDefaultSize, 3, mUnitRadiosStrings, 1, wxRA_SPECIFY_COLS );
    if (ShowToolTips())
        mUnitRadios->SetToolTip(_("units for the x and y values to the left"));
    itemBoxSizer3->Add(mUnitRadios, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer10, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, _("Window Scale:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemStaticText11, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mWindowScale = new wxTextCtrl( itemDialog1, ID_WINSCALE, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(mWindowScale, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemDialog1, wxID_STATIC, _("Window Center:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer14, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mXWinCenter = new wxTextCtrl( itemDialog1, ID_XWINCENTER, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer14->Add(mXWinCenter, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mYWinCenter = new wxTextCtrl( itemDialog1, ID_YWINCENTER, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer14->Add(mYWinCenter, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mZWinCenter = new wxTextCtrl( itemDialog1, ID_ZWINCENTER, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer14->Add(mZWinCenter, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText18 = new wxStaticText( itemDialog1, wxID_STATIC, _("Rotation Angles (degrees):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText18, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer19, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mPsiEdit = new wxTextCtrl( itemDialog1, ID_PSIANGLEEDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(mPsiEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mPhiEdit = new wxTextCtrl( itemDialog1, ID_PHIANGLEEDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(mPhiEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mThetaEdit = new wxTextCtrl( itemDialog1, ID_THETAANGLEEDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(mThetaEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer23, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton24 = new wxButton( itemDialog1, wxID_APPLY, _("&Apply"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer23->Add(itemButton24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton25 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer23->Add(itemButton25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton26 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer23->Add(itemButton26, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    // Set validators
    mXWinSize->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & screenX) );
    mYWinSize->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & screenY) );
    mWindowScale->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & windowScale) );
    mXWinCenter->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & XCenter) );
    mYWinCenter->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & YCenter) );
    mZWinCenter->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & ZCenter) );
    mPsiEdit->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & psi) );
    mPhiEdit->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & phi) );
    mThetaEdit->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & theta) );
////@end windowparameters content construction

	MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	MoleculeData * 	MainData = parent->GetData();
	
	int width, height;
	parent->GetClientSize(&width, &height);
	screenX.Printf(wxT("%d"), width);
	screenY.Printf(wxT("%d"), height);
	
	float scale = MainData->GetMoleculeSize();
	windowScale.Printf(wxT("%f"), scale);

	CPoint3D center;
	MainData->GetModelCenter(&center);
	XCenter.Printf(wxT("%f"), center.x);
	YCenter.Printf(wxT("%f"), center.y);
	ZCenter.Printf(wxT("%f"), center.z);

	float psi1, phi1, theta1;
	MainData->GetModelRotation(&psi1, &phi1, &theta1);
	psi.Printf(wxT("%.2f"), psi1);
	phi.Printf(wxT("%.2f"), phi1);
	theta.Printf(wxT("%.2f"), theta1);
}

/*!
 * Should we show tooltips?
 */

bool windowparameters::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap windowparameters::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin windowparameters bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end windowparameters bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon windowparameters::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin windowparameters icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end windowparameters icon retrieval
}
/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_RADIOBOX4
 */

void windowparameters::OnUnitsradioboxSelected( wxCommandEvent& event )
{
	MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	int width, height;
	wxString	t;
	parent->GetClientSize(&width, &height);
	switch (mUnitRadios->GetSelection()) {
		case 0:	//pixels
			screenX.Printf(wxT("%d"), width);
			screenY.Printf(wxT("%d"), height);
			break;
		case 1: //inches
			screenX.Printf(wxT("%f"), ((float)width/72.0));
			screenY.Printf(wxT("%f"), ((float)height/72.0));
			break;
		case 2:	//cm
			screenX.Printf(wxT("%f"), ((float)width/(72.0*0.3937)));
			screenY.Printf(wxT("%f"), ((float)height/(72.0*0.3937)));
			break;
	}
	t = screenX;
	mXWinSize->SetValue(t);
	t = screenY;
	mYWinSize->SetValue(t);
}

bool windowparameters::Apply(void) {
	bool result = true;
	
	MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	MoleculeData * 	MainData = parent->GetData();
	
	double UnitConversion = 1.0;
	switch (mUnitRadios->GetSelection()) {
		case 1:
			UnitConversion = 72.0;
			break;
		case 2:
			UnitConversion = 72.0*0.3937;
			break;
	}
	int width, height;
	parent->GetClientSize(&width, &height);
	double tw, th;
	if (!mXWinSize->GetValue().ToDouble(&tw)) {
		tw = width/UnitConversion;
		result = false;
	}
	if (tw <= 0) {
		tw = width/UnitConversion;
		result = false;
	}
	if (!mYWinSize->GetValue().ToDouble(&th)) {
		th = height/UnitConversion;
		result = false;
	}
	if (th <= 0) {
		th = height/UnitConversion;
		result = false;
	}
	double wScale;
	if (!mWindowScale->GetValue().ToDouble(&wScale)) {
		result = false;
		wScale = MainData->GetMoleculeSize();
	}
	if (wScale <= 0.0) {
		result = false;
		wScale = MainData->GetMoleculeSize();
	}
	double tc;
	CPoint3D center;
	MainData->GetModelCenter(&center);
	if (!mXWinCenter->GetValue().ToDouble(&tc)) {
		result = false;
		tc = center.x;
	}
	center.x = tc;
	if (!mYWinCenter->GetValue().ToDouble(&tc)) {
		result = false;
		tc = center.y;
	}
	center.y = tc;
	if (!mZWinCenter->GetValue().ToDouble(&tc)) {
		result = false;
		tc = center.z;
	}
	center.z = tc;
	
	double tpsi, tphi, ttheta;
	float psi1, phi1, theta1;
	MainData->GetModelRotation(&psi1, &phi1, &theta1);
	if (!mPsiEdit->GetValue().ToDouble(&tpsi)) {
		result = false;
		tpsi = psi1;
	}
	if (!mPhiEdit->GetValue().ToDouble(&tphi)) {
		result = false;
		tphi = phi1;
	}
	if (!mThetaEdit->GetValue().ToDouble(&ttheta)) {
		result = false;
		ttheta = theta1;
	}
	
	if (result) {	//everything looks good, apply it to the window and redraw
		tw *= UnitConversion;
		th *= UnitConversion;
		if ((tw!=width)||(th!=height)) {
			width = (int) tw;
			height = (int) th;
			parent->SetClientSize(width, height);
		}
		MainData->SetMoleculeSize(wScale);
		MainData->SetModelCenter(&center);
		MainData->SetModelRotation(tpsi,tphi,ttheta);

		parent->ResetModel(false);
	} else { //punch out the values we have and return false
		wxString temp, temp2;

		if (mUnitRadios->GetSelection() == 0) {
			int w = (int) tw;
			int h = (int) th;
			temp.Printf(wxT("%d"), w);
			temp2.Printf(wxT("%d"), h);
		} else {
			temp.Printf(wxT("%f"), tw);
			temp2.Printf(wxT("%f"), th);
		}
		mXWinSize->SetValue(temp);
		mYWinSize->SetValue(temp2);
		
		temp.Printf(wxT("%f"), wScale);
		mWindowScale->SetValue(temp);
		
		temp.Printf(wxT("%f"), center.x);
		mXWinCenter->SetValue(temp);
		temp.Printf(wxT("%f"), center.y);
		mYWinCenter->SetValue(temp);
		temp.Printf(wxT("%f"), center.z);
		mZWinCenter->SetValue(temp);
		
		temp.Printf(wxT("%.2f"), tpsi);
		mPsiEdit->SetValue(temp);
		temp.Printf(wxT("%.2f"), tphi);
		mPhiEdit->SetValue(temp);
		temp.Printf(wxT("%.2f"), ttheta);
		mThetaEdit->SetValue(temp);
	}
	
	return result;
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY
 */

void windowparameters::OnApplyClick( wxCommandEvent& event )
{
	Apply();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void windowparameters::OnOkClick( wxCommandEvent& event )
{
	if (!Apply()) {
//		event.veto();
		return;
	}
    event.Skip();
}
