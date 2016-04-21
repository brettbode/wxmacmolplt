/////////////////////////////////////////////////////////////////////////////
// Name:        changeatomorderdialog.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Tue  2 Oct 11:11:35 2007
// RCS-ID:      
// Copyright:   (c) 2007 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "changeatomorderdialog.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "mmp_gl.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "MolDisplayWin.h"
#include "changeatomorderdialog.h"

////@begin XPM images
////@end XPM images


/*!
 * ChangeAtomOrderDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ChangeAtomOrderDialog, wxDialog )


/*!
 * ChangeAtomOrderDialog event table definition
 */

BEGIN_EVENT_TABLE( ChangeAtomOrderDialog, wxDialog )

////@begin ChangeAtomOrderDialog event table entries
	EVT_TEXT( ID_TEXTCTRL5, ChangeAtomOrderDialog::OnTextctrl5Updated )
	EVT_BUTTON( wxID_OK, ChangeAtomOrderDialog::OnOkClick )
	EVT_UPDATE_UI( wxID_OK, ChangeAtomOrderDialog::OnOkUpdate )
////@end ChangeAtomOrderDialog event table entries

END_EVENT_TABLE()


/*!
 * ChangeAtomOrderDialog constructors
 */

ChangeAtomOrderDialog::ChangeAtomOrderDialog()
{
	Init();
}

ChangeAtomOrderDialog::ChangeAtomOrderDialog( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Parent = parent;
	Create(parent, id, caption, pos, size, style);
}


/*!
 * ChangeAtomOrderDialog creator
 */

bool ChangeAtomOrderDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ChangeAtomOrderDialog creation
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end ChangeAtomOrderDialog creation
	return true;
}


/*!
 * ChangeAtomOrderDialog destructor
 */

ChangeAtomOrderDialog::~ChangeAtomOrderDialog()
{
////@begin ChangeAtomOrderDialog destruction
////@end ChangeAtomOrderDialog destruction
}


/*!
 * Member initialisation
 */

void ChangeAtomOrderDialog::Init()
{
////@begin ChangeAtomOrderDialog member initialisation
	index = -1;
	indexEdit = NULL;
////@end ChangeAtomOrderDialog member initialisation
}


/*!
 * Control creation for ChangeAtomOrderDialog
 */

void ChangeAtomOrderDialog::CreateControls()
{    
////@begin ChangeAtomOrderDialog content construction
	ChangeAtomOrderDialog* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Enter the position you would like the first selected atom to be inserted into the atom list:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStaticText3->Wrap(250);
	itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	indexEdit = new wxTextCtrl( itemDialog1, ID_TEXTCTRL5, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer2->Add(indexEdit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Any other selected atoms will be inserted in order after the first atom."), wxDefaultPosition, wxDefaultSize, 0 );
	itemStaticText5->Wrap(250);
	itemBoxSizer2->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_RIGHT|wxALL, 5);

	wxButton* itemButton7 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer6->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton8 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	itemButton8->SetDefault();
	itemBoxSizer6->Add(itemButton8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	// Set validators
	indexEdit->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
////@end ChangeAtomOrderDialog content construction
}


/*!
 * Should we show tooltips?
 */

bool ChangeAtomOrderDialog::ShowToolTips()
{
	return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ChangeAtomOrderDialog::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin ChangeAtomOrderDialog bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end ChangeAtomOrderDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ChangeAtomOrderDialog::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin ChangeAtomOrderDialog icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end ChangeAtomOrderDialog icon retrieval
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL5
 */

void ChangeAtomOrderDialog::OnTextctrl5Updated( wxCommandEvent& event )
{
	wxString textVal = indexEdit->GetValue();
	long test = 1;
	if (! textVal.ToLong(&test)) {
		textVal.Printf(wxT("%ld"), test);
		indexEdit->SetValue(textVal);
	}
	index = test;
    event.Skip();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void ChangeAtomOrderDialog::OnOkClick( wxCommandEvent& event )
{
	Parent->ChangeAtomOrder(index-1);
	event.Skip();
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void ChangeAtomOrderDialog::OnOkUpdate( wxUpdateUIEvent& event )
{
	event.Enable((index >= 0));
}

