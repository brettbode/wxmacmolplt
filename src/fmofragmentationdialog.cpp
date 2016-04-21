/////////////////////////////////////////////////////////////////////////////
// Name:        fmofragmentationdialog.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Wed 23 Dec 11:01:48 2009
// RCS-ID:      
// Copyright:   (c) 2008 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "fmofragmentationdialog.h"
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

#include "fmofragmentationdialog.h"

////@begin XPM images
////@end XPM images


/*!
 * FMOFragmentationDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( FMOFragmentationDialog, wxDialog )


/*!
 * FMOFragmentationDialog event table definition
 */

BEGIN_EVENT_TABLE( FMOFragmentationDialog, wxDialog )

////@begin FMOFragmentationDialog event table entries
	EVT_TEXT( ID_NUMFRAGEDIT, FMOFragmentationDialog::OnNumFragEditTextUpdated )
	EVT_BUTTON( wxID_OK, FMOFragmentationDialog::OnFMOFragButtonClick )
////@end FMOFragmentationDialog event table entries

END_EVENT_TABLE()


/*!
 * FMOFragmentationDialog constructors
 */

FMOFragmentationDialog::FMOFragmentationDialog()
{
	Init();
}

FMOFragmentationDialog::FMOFragmentationDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create(parent, id, caption, pos, size, style);
}


/*!
 * FMOFragmentationDialog creator
 */

bool FMOFragmentationDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin FMOFragmentationDialog creation
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end FMOFragmentationDialog creation
	return true;
}


/*!
 * FMOFragmentationDialog destructor
 */

FMOFragmentationDialog::~FMOFragmentationDialog()
{
////@begin FMOFragmentationDialog destruction
////@end FMOFragmentationDialog destruction
}


/*!
 * Member initialisation
 */

void FMOFragmentationDialog::Init()
{
////@begin FMOFragmentationDialog member initialisation
	mNumFragEdit = NULL;
////@end FMOFragmentationDialog member initialisation
}


/*!
 * Control creation for FMOFragmentationDialog
 */

void FMOFragmentationDialog::CreateControls()
{    
////@begin FMOFragmentationDialog content construction
	FMOFragmentationDialog* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Clicking Ok will initialize the FMO fragment definition list by dividing the system up into the non-bonded (based on the bonding shown) pieces, optionally combining the n nearest non-bonded pieces into each FMO fragment."), wxDefaultPosition, wxDefaultSize, 0 );
	itemStaticText3->Wrap(450);
	itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("# non-bonded molecules per FMO fragment"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mNumFragEdit = new wxTextCtrl( itemDialog1, ID_NUMFRAGEDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer4->Add(mNumFragEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer7, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxButton* itemButton8 = new wxButton( itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer7->Add(itemButton8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton9 = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	itemButton9->SetDefault();
	itemBoxSizer7->Add(itemButton9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	// Set validators
	mNumFragEdit->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
////@end FMOFragmentationDialog content construction
	mNumFragEdit->SetValue(wxT("1"));
}


/*!
 * Should we show tooltips?
 */

bool FMOFragmentationDialog::ShowToolTips()
{
	return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap FMOFragmentationDialog::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin FMOFragmentationDialog bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end FMOFragmentationDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon FMOFragmentationDialog::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin FMOFragmentationDialog icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end FMOFragmentationDialog icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY
 */

void FMOFragmentationDialog::OnFMOFragButtonClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY in FMOFragmentationDialog.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY in FMOFragmentationDialog. 
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_NUMFRAGEDIT
 */

void FMOFragmentationDialog::OnNumFragEditTextUpdated( wxCommandEvent& event )
{
	wxString textVal = mNumFragEdit->GetValue();
	long test = 1;
	if (! textVal.ToLong(&test)) {
		textVal.Printf(wxT("%ld"), test);
		mNumFragEdit->SetValue(textVal);
	}
	event.Skip();
}

int FMOFragmentationDialog::GetMolCount(void) {
	wxString textVal = mNumFragEdit->GetValue();
	long test = 1;
	textVal.ToLong(&test);
	return test;
}

