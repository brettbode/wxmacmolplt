/////////////////////////////////////////////////////////////////////////////
// Name:        setbondlength.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Wed  3 May 11:01:00 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "setbondlength.h"
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
////@end SetBondLength event table entries

END_EVENT_TABLE()

/*!
 * SetBondLength constructors
 */

SetBondLength::SetBondLength( )
{
}

SetBondLength::SetBondLength( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * SetBondLength creator
 */

bool SetBondLength::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SetBondLength member initialisation
    maxValueEdit = NULL;
////@end SetBondLength member initialisation

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
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    maxValueEdit = new wxTextCtrl( itemDialog1, ID_TEXTCTRL1, _("0.00"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        maxValueEdit->SetToolTip(_("Enter a value to apply a flat maximum distance, or 0 to ignore."));
    itemBoxSizer2->Add(maxValueEdit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_LEFT|wxALL, 5);

    wxCheckBox* itemCheckBox6 = new wxCheckBox( itemDialog1, ID_CHECKBOX, _("Keep old bonds"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox6->SetValue(false);
    if (ShowToolTips())
        itemCheckBox6->SetToolTip(_("When checked bonds are only appended the current set"));
    itemBoxSizer5->Add(itemCheckBox6, 0, wxALIGN_LEFT|wxALL, 5);

    wxCheckBox* itemCheckBox7 = new wxCheckBox( itemDialog1, ID_CHECKBOX1, _("Use \"normal\" bond length criteria"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox7->SetValue(false);
    if (ShowToolTips())
        itemCheckBox7->SetToolTip(_("Determines bonds based on the atomic radius of each atom."));
    itemBoxSizer5->Add(itemCheckBox7, 0, wxALIGN_LEFT|wxALL, 5);

    wxCheckBox* itemCheckBox8 = new wxCheckBox( itemDialog1, ID_CHECKBOX2, _("Guess Bond Order"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox8->SetValue(false);
    if (ShowToolTips())
        itemCheckBox8->SetToolTip(_("Assign double and triple bonds based on length."));
    itemBoxSizer5->Add(itemCheckBox8, 0, wxALIGN_LEFT|wxALL, 5);

////@end SetBondLength content construction
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
