/////////////////////////////////////////////////////////////////////////////
// Name:        setbondlength.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Wed  3 May 11:01:00 2006
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "setbondlength.h"
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

////@begin includes
////@end includes
#include "MolDisplayWin.h"
#include "Frame.h"

#include "setbondlength.h"

////@begin XPM images
////@end XPM images

/*!
 * SetBondLength type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SetBondLength, wxDialog )

/*!
 * SetBondLength event table definition
 */

BEGIN_EVENT_TABLE( SetBondLength, wxDialog )

////@begin SetBondLength event table entries
    EVT_TEXT( ID_MAXLENGTHEDIT, SetBondLength::OnMaxlengtheditUpdated )

    EVT_CHECKBOX( ID_NORMALCHECKBOX, SetBondLength::OnNormalcheckboxClick )

    EVT_BUTTON( wxID_OK, SetBondLength::OnOkClick )

////@end SetBondLength event table entries

END_EVENT_TABLE()

/*!
 * SetBondLength constructors
 */

SetBondLength::SetBondLength( )
{
}

SetBondLength::SetBondLength( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * SetBondLength creator
 */

bool SetBondLength::Create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SetBondLength member initialisation
    maxValueEdit = NULL;
    keepOldBondsCheck = NULL;
    normalBondCheck = NULL;
    normalText = NULL;
    normalTolSlider = NULL;
    guessBondCheck = NULL;
    hhCheck = NULL;
    hydrogenCheck = NULL;
////@end SetBondLength member initialisation
	Parent = parent;

////@begin SetBondLength creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end SetBondLength creation
    return true;
}

/*!
 * Control creation for SetBondLength
 */

void SetBondLength::CreateControls()
{    
////@begin SetBondLength content construction
    SetBondLength* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Maximum inter-atomic distance to be\nconsidered a bond:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    maxValueEdit = new wxTextCtrl( itemDialog1, ID_MAXLENGTHEDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        maxValueEdit->SetToolTip(_("Enter a value to apply a flat maximum distance, or 0 to ignore."));
    itemBoxSizer2->Add(maxValueEdit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_LEFT|wxALL, 5);

    keepOldBondsCheck = new wxCheckBox( itemDialog1, ID_KEEPOLDCHECKBOX, _("Keep old bonds"), wxDefaultPosition, wxDefaultSize, 0 );
    keepOldBondsCheck->SetValue(false);
    if (ShowToolTips())
        keepOldBondsCheck->SetToolTip(_("When checked bonds are only appended to the current set"));
    itemBoxSizer5->Add(keepOldBondsCheck, 0, wxALIGN_LEFT|wxALL, 5);

    normalBondCheck = new wxCheckBox( itemDialog1, ID_NORMALCHECKBOX, _("Use \"normal\" bond length criteria"), wxDefaultPosition, wxDefaultSize, 0 );
    normalBondCheck->SetValue(false);
    if (ShowToolTips())
        normalBondCheck->SetToolTip(_("Determines bonds based on the atomic radius of each atom."));
    itemBoxSizer5->Add(normalBondCheck, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer5->Add(itemBoxSizer8, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer8->Add(itemBoxSizer9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    normalText = new wxStaticText( itemDialog1, wxID_STATIC, _("\"normal\" bonding tolerance:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer9->Add(normalText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    normalTolSlider = new wxSlider( itemDialog1, ID_NORMALSLIDER, 100, 50, 150, wxDefaultPosition, wxSize(110, -1), wxSL_HORIZONTAL );
    if (ShowToolTips())
        normalTolSlider->SetToolTip(_("Move to the left for tighter tolerances - shorter bonds"));
    itemBoxSizer9->Add(normalTolSlider, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    guessBondCheck = new wxCheckBox( itemDialog1, ID_GUESSBONDCHECKBOX, _("Guess Bond Order"), wxDefaultPosition, wxDefaultSize, 0 );
    guessBondCheck->SetValue(false);
    if (ShowToolTips())
        guessBondCheck->SetToolTip(_("Assign double and triple bonds based on length."));
    itemBoxSizer8->Add(guessBondCheck, 0, wxALIGN_LEFT|wxALL, 5);

    hhCheck = new wxCheckBox( itemDialog1, ID_HHCHECKBOX, _("Allow H-H bonds"), wxDefaultPosition, wxDefaultSize, 0 );
    hhCheck->SetValue(false);
    itemBoxSizer8->Add(hhCheck, 0, wxALIGN_LEFT|wxALL, 5);

    hydrogenCheck = new wxCheckBox( itemDialog1, ID_HYDROGENCHECK, _("Assign Hydrogen Bonds"), wxDefaultPosition, wxDefaultSize, 0 );
    hydrogenCheck->SetValue(false);
    itemBoxSizer8->Add(hydrogenCheck, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer15, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton16 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer15->Add(itemButton16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton17 = new wxButton( itemDialog1, wxID_OK, _("&Set"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton17->SetDefault();
    itemBoxSizer15->Add(itemButton17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end SetBondLength content construction
	WinPrefs * Prefs = Parent->GetPrefs();
	bool AutoBond = Prefs->GetAutoBond();
	normalBondCheck->SetValue(AutoBond);
	normalText->Enable(AutoBond);
	normalTolSlider->Enable(AutoBond);
	hhCheck->Enable(AutoBond);
	guessBondCheck->Enable(AutoBond);
	hydrogenCheck->Enable(AutoBond);
	hhCheck->SetValue(! Prefs->GetHHBondFlag());
	guessBondCheck->SetValue(Prefs->DetermineBondOrder());
	hydrogenCheck->SetValue(Prefs->AllowHydrogenBonds());
	normalTolSlider->SetValue((int) (Prefs->GetAutoBondScale()*10000));
	wxString buf;
	buf.Printf(wxT("%.2f"),Prefs->GetMaxBondLength());
	maxValueEdit->SetValue(buf);
	// Set validator
	maxValueEdit->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
}

/*!
 * Should we show tooltips?
 */

bool SetBondLength::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SetBondLength::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin SetBondLength bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end SetBondLength bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SetBondLength::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin SetBondLength icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end SetBondLength icon retrieval
}
/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MAXLENGTHEDIT
 */

void SetBondLength::OnMaxlengtheditUpdated( wxCommandEvent& event )
{
	wxString textVal = maxValueEdit->GetValue();
	double val = 0.0;
	if (! textVal.ToDouble(&val)) {
		textVal.Printf(wxT("%.2f"), val);
		maxValueEdit->SetValue(textVal);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void SetBondLength::OnOkClick( wxCommandEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	WinPrefs * Prefs = Parent->GetPrefs();
	wxString textVal = maxValueEdit->GetValue();
	double val;
	if (textVal.ToDouble(&val)) {
		Prefs->SetMaxBondLength((float) val);
	}
	bool AutoBond = normalBondCheck->IsChecked();
	Prefs->SetAutoBond(AutoBond);
	if (AutoBond) {
		Prefs->SetAutoBondScale(0.0001f * (float) (normalTolSlider->GetValue()));
		Prefs->SetHHBondFlag(! hhCheck->IsChecked());
		Prefs->DetermineBondOrder(guessBondCheck->IsChecked());
		Prefs->AllowHydrogenBonds(hydrogenCheck->IsChecked());
	}
				
	Frame * lFrame = MainData->GetCurrentFramePtr();
	Progress lProg;
	lFrame->SetBonds(Prefs, keepOldBondsCheck->IsChecked(), &lProg);
	
	Parent->ResetAllWindows();
	
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_NORMALCHECKBOX
 */

void SetBondLength::OnNormalcheckboxClick( wxCommandEvent& event )
{
		//All we do here is enable/disable the dependant controls
	bool checked = normalBondCheck->IsChecked();
	normalText->Enable(checked);
	normalTolSlider->Enable(checked);
	hhCheck->Enable(checked);
	guessBondCheck->Enable(checked);
	hydrogenCheck->Enable(checked);
	
	event.Skip();
}


