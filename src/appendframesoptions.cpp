/////////////////////////////////////////////////////////////////////////////
// Name:        appendframesoptions.cpp
// Purpose:     Manages the addition of additional gemotries from other files.
// Author:      Brett Bode
// Created:     Thu  4 May 12:46:21 2006
// Copyright:   (c) 2006 Iowa State University
// Licence:     See the LICENSE file in the top level directory
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "appendframesoptions.h"
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

#include "appendframesoptions.h"

////@begin XPM images
////@end XPM images

/*!
 * AppendFramesOptions type definition
 */

IMPLEMENT_DYNAMIC_CLASS( AppendFramesOptions, wxDialog )

/*!
 * AppendFramesOptions event table definition
 */

BEGIN_EVENT_TABLE( AppendFramesOptions, wxDialog )

////@begin AppendFramesOptions event table entries
	EVT_BUTTON( wxID_OK, AppendFramesOptions::OnOkClick )
////@end AppendFramesOptions event table entries

END_EVENT_TABLE()

/*!
 * AppendFramesOptions constructors
 */

AppendFramesOptions::AppendFramesOptions( )
{
}

AppendFramesOptions::AppendFramesOptions( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * AppendFramesOptions creator
 */

bool AppendFramesOptions::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin AppendFramesOptions member initialisation
	flip = false;
	offset = 0.0;
	negativeCheck = NULL;
	offsetEdit = NULL;
	skipEdit = NULL;
////@end AppendFramesOptions member initialisation

////@begin AppendFramesOptions creation
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end AppendFramesOptions creation
    return true;
}

/*!
 * Control creation for AppendFramesOptions
 */

void AppendFramesOptions::CreateControls()
{    
////@begin AppendFramesOptions content construction
	AppendFramesOptions* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Options to apply to the geometry points\nto be read in from the selected file."), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	negativeCheck = new wxCheckBox( itemDialog1, ID_NEGATIVECHECKBOX, _("Make these points negative"), wxDefaultPosition, wxDefaultSize, 0 );
	negativeCheck->SetValue(false);
	if (AppendFramesOptions::ShowToolTips())
		negativeCheck->SetToolTip(_("Invert the sign of IRC or time values to determine the order of the geometries"));
	itemBoxSizer2->Add(negativeCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("IRC or time coordinate offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer5->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	offsetEdit = new wxTextCtrl( itemDialog1, ID_OFFSETTEXTCTL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	if (AppendFramesOptions::ShowToolTips())
		offsetEdit->SetToolTip(_("This value will be added to the IRC or DRP time to determine the order frames will be inserted."));
	itemBoxSizer5->Add(offsetEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("# of points to skip between\npoints read in:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	itemBoxSizer8->Add(itemStaticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	skipEdit = new wxTextCtrl( itemDialog1, ID_SKIPTEXTCTL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	if (AppendFramesOptions::ShowToolTips())
		skipEdit->SetToolTip(_("Enter 0 to read every geometry, 1 to read every other geometry, ..."));
	itemBoxSizer8->Add(skipEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer11, 0, wxALIGN_RIGHT|wxALL, 5);

	wxButton* itemButton12 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer11->Add(itemButton12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton13 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer11->Add(itemButton13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	// Set validators
	offsetEdit->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	skipEdit->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
////@end AppendFramesOptions content construction
}

/*!
 * Should we show tooltips?
 */

bool AppendFramesOptions::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap AppendFramesOptions::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin AppendFramesOptions bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end AppendFramesOptions bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon AppendFramesOptions::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin AppendFramesOptions icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end AppendFramesOptions icon retrieval
}
void AppendFramesOptions::SetupItems(void) {
    negativeCheck->SetValue(flip);
	wxString buf;
	buf.Printf(wxT("%lf"), offset);
    offsetEdit->SetValue(buf);
	buf.Printf(wxT("%ld"), skip);
    skipEdit->SetValue(buf);
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void AppendFramesOptions::OnOkClick( wxCommandEvent& event )
{
	flip = negativeCheck->IsChecked();
	bool good = true;
	wxString oval = offsetEdit->GetValue();
	if (! oval.ToDouble(&offset)) {
		oval.Printf(wxT("%lf"), offset);
		offsetEdit->SetValue(oval);
		good = false;
	}
	wxString sval = skipEdit->GetValue();
	if (! sval.ToLong(&skip)) {
		sval.Printf(wxT("%ld"), skip);
		skipEdit->SetValue(sval);
		good = false;
	}
	
	if (good) event.Skip();
}


