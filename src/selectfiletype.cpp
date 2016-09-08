/////////////////////////////////////////////////////////////////////////////
// Name:        selectfiletype.cpp
// Purpose:     
// Author:      Brett Bode
// Created:     Sat  3 Sep 20:35:29 2016
// Copyright:   (c) 2016 Iowa State University
// Licence:     See the LICENSE file in the top level directory
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "selectfiletype.h"
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

#include "selectfiletype.h"

////@begin XPM images
////@end XPM images


/*!
 * SelectFileType type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SelectFileType, wxDialog )


/*!
 * SelectFileType event table definition
 */

BEGIN_EVENT_TABLE( SelectFileType, wxDialog )

////@begin SelectFileType event table entries
	EVT_CHOICE( ID_FILETYPECHOICE, SelectFileType::OnFiletypechoiceSelected )
	EVT_UPDATE_UI( wxID_OK, SelectFileType::OnOkUpdate )
////@end SelectFileType event table entries

END_EVENT_TABLE()


/*!
 * SelectFileType constructors
 */

SelectFileType::SelectFileType()
{
	Init();
}

SelectFileType::SelectFileType( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create(parent, id, caption, pos, size, style);
}


/*!
 * SelectFileType creator
 */

bool SelectFileType::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SelectFileType creation
	SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end SelectFileType creation
	return true;
}


/*!
 * SelectFileType destructor
 */

SelectFileType::~SelectFileType()
{
////@begin SelectFileType destruction
////@end SelectFileType destruction
}


/*!
 * Member initialisation
 */

void SelectFileType::Init()
{
////@begin SelectFileType member initialisation
	mFileText = NULL;
	mFileType = NULL;
////@end SelectFileType member initialisation
}


/*!
 * Control creation for SelectFileType
 */

void SelectFileType::CreateControls()
{    
////@begin SelectFileType content construction
	SelectFileType* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Unable to automatically determine the file type. You may use the file type selector below to manually choose a file type and proceed to parse the file or cancel. All files should be plain ASCII text files!"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStaticText3->Wrap(500);
	itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mFileText = new wxTextCtrl( itemDialog1, ID_FILEDISPLAYTEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(-1, 300), wxTE_MULTILINE|wxTE_READONLY );
	itemBoxSizer2->Add(mFileText, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("File type:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer5->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString mFileTypeStrings;
	mFileTypeStrings.Add(_("Unknown"));
	mFileTypeStrings.Add(_("MolPlt File"));
	mFileTypeStrings.Add(_("GAMESS log File"));
	mFileTypeStrings.Add(_("GAMESS IRC File"));
	mFileTypeStrings.Add(_("GAMESS DRC File"));
	mFileTypeStrings.Add(_("GAMESS TRJ File"));
	mFileTypeStrings.Add(_("GAMESS Input File"));
	mFileTypeStrings.Add(_("XYZ File"));
	mFileTypeStrings.Add(_("PDB File"));
	mFileTypeStrings.Add(_("MDL Mol File"));
	mFileTypeStrings.Add(_("CML File"));
	mFileTypeStrings.Add(_("MolDen File"));
	mFileTypeStrings.Add(_("Molekel File"));
	mFileTypeStrings.Add(_("MOPAC arc File"));
	mFileTypeStrings.Add(_("MPOAC mop File"));
	mFileType = new wxChoice( itemDialog1, ID_FILETYPECHOICE, wxDefaultPosition, wxDefaultSize, mFileTypeStrings, 0 );
	itemBoxSizer5->Add(mFileType, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer8, 0, wxALIGN_RIGHT|wxALL, 5);

	wxButton* itemButton9 = new wxButton( itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer8->Add(itemButton9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton10 = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer8->Add(itemButton10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end SelectFileType content construction
}


/*!
 * Should we show tooltips?
 */

bool SelectFileType::ShowToolTips()
{
	return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SelectFileType::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin SelectFileType bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end SelectFileType bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SelectFileType::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin SelectFileType icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end SelectFileType icon retrieval
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE
 */

void SelectFileType::OnFiletypechoiceSelected( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE in SelectFileType.
	// Before editing this code, remove the block markers.
	event.Skip();
////@end wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE in SelectFileType. 
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void SelectFileType::OnOkUpdate( wxUpdateUIEvent& event )
{
	
	event.Enable((mFileType->GetSelection()>=1));
}

