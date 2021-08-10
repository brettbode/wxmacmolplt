/////////////////////////////////////////////////////////////////////////////
// Name:        inputfileeditor.cpp
// Purpose:     
// Author:      Brett Bode
// Created:     Sun 11 Sep 09:11:25 2016
// Copyright:   (c) 2008-2009 Iowa State University
// Licence:     See the LICENSE file in the top level directory
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "inputfileeditor.h"
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

#include "inputfileeditor.h"

////@begin XPM images
////@end XPM images


/*!
 * InputFileEditor type definition
 */

IMPLEMENT_DYNAMIC_CLASS( InputFileEditor, wxDialog )


/*!
 * InputFileEditor event table definition
 */

BEGIN_EVENT_TABLE( InputFileEditor, wxDialog )

////@begin InputFileEditor event table entries
	EVT_BUTTON( wxID_CANCEL, InputFileEditor::OnCancelClick )
	EVT_BUTTON( wxID_SAVEAS, InputFileEditor::OnSaveasClick )
////@end InputFileEditor event table entries

END_EVENT_TABLE()


/*!
 * InputFileEditor constructors
 */

InputFileEditor::InputFileEditor()
{
	Init();
}

InputFileEditor::InputFileEditor( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create(parent, id, caption, pos, size, style);
}


/*!
 * InputFileEditor creator
 */

bool InputFileEditor::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin InputFileEditor creation
	SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end InputFileEditor creation
	return true;
}


/*!
 * InputFileEditor destructor
 */

InputFileEditor::~InputFileEditor()
{
////@begin InputFileEditor destruction
////@end InputFileEditor destruction
}


/*!
 * Member initialisation
 */

void InputFileEditor::Init()
{
////@begin InputFileEditor member initialisation
	mFileEditor = NULL;
////@end InputFileEditor member initialisation
}


/*!
 * Control creation for InputFileEditor
 */

void InputFileEditor::CreateControls()
{    
////@begin InputFileEditor content construction
	InputFileEditor* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Edit the GAMESS input file below as you wish and hit Save to write to disk or Cancel to abort."), wxDefaultPosition, wxDefaultSize, 0 );
	itemStaticText3->Wrap(500);
	itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mFileEditor = new wxTextCtrl( itemDialog1, ID_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxSize(600, 400), wxTE_MULTILINE );
	itemBoxSizer2->Add(mFileEditor, 1, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_RIGHT|wxALL, 5);

	wxButton* itemButton6 = new wxButton( itemDialog1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer5->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton7 = new wxButton( itemDialog1, wxID_SAVEAS, _("Save &As..."), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer5->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end InputFileEditor content construction
}


/*!
 * Should we show tooltips?
 */

bool InputFileEditor::ShowToolTips()
{
	return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap InputFileEditor::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin InputFileEditor bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end InputFileEditor bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon InputFileEditor::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin InputFileEditor icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end InputFileEditor icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_SAVEAS
 * Open a save as dialog to get a filename, rename the temp file to the new name and exit.
 */

void InputFileEditor::OnSaveasClick( wxCommandEvent& event )
{
	wxString filePath = wxFileSelector(wxT("Save As"), wxT(""), wxT(""), wxT(""),
									   wxT("GAMESS input files (*.inp)|*.inp"),
#if wxCHECK_VERSION(2,9,0)
									   wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
#else
									   wxSAVE | wxOVERWRITE_PROMPT,
#endif
									   this);
	
	if(!filePath.IsEmpty()) {
		mFileEditor->SaveFile();
		if (wxRenameFile(mFileName, filePath, true)) {
			Destroy();
		} else {
			wxLogMessage(_("Unable to save to the target file name. Please try again."));
		}
	} else	//If the file path is empty leave the dialog open
		event.Skip();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void InputFileEditor::OnCancelClick( wxCommandEvent& /*event */)
{
	// Hitting cancel should discard the temporary file and close the dialog
	wxRemoveFile(mFileName);
	Destroy();
}

