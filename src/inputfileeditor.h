/////////////////////////////////////////////////////////////////////////////
// Name:        inputfileeditor.h
// Purpose:     
// Author:      Brett Bode
// Created:     Sun 11 Sep 09:11:25 2016
// Copyright:   (c) 2008-2009 Iowa State University
// Licence:     See the LICENSE file in the top level directory
/////////////////////////////////////////////////////////////////////////////

#ifndef _INPUTFILEEDITOR_H_
#define _INPUTFILEEDITOR_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "inputfileeditor.h"
#endif

/*!
 * Includes
 */

////@begin includes
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_INPUTFILEEDITOR 10000
#define ID_TEXTCTRL 10001
#define SYMBOL_INPUTFILEEDITOR_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_INPUTFILEEDITOR_TITLE _("Input File Editor")
#define SYMBOL_INPUTFILEEDITOR_IDNAME ID_INPUTFILEEDITOR
#define SYMBOL_INPUTFILEEDITOR_SIZE wxSize(500, 500)
#define SYMBOL_INPUTFILEEDITOR_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * InputFileEditor class declaration
 */

class InputFileEditor: public wxDialog
{    
	DECLARE_DYNAMIC_CLASS( InputFileEditor )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	InputFileEditor();
	InputFileEditor( wxWindow* parent, wxWindowID id = SYMBOL_INPUTFILEEDITOR_IDNAME, const wxString& caption = SYMBOL_INPUTFILEEDITOR_TITLE, const wxPoint& pos = SYMBOL_INPUTFILEEDITOR_POSITION, const wxSize& size = SYMBOL_INPUTFILEEDITOR_SIZE, long style = SYMBOL_INPUTFILEEDITOR_STYLE );

	/// Creation
	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_INPUTFILEEDITOR_IDNAME, const wxString& caption = SYMBOL_INPUTFILEEDITOR_TITLE, const wxPoint& pos = SYMBOL_INPUTFILEEDITOR_POSITION, const wxSize& size = SYMBOL_INPUTFILEEDITOR_SIZE, long style = SYMBOL_INPUTFILEEDITOR_STYLE );

	/// Destructor
	~InputFileEditor();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

	/// Set the file to display
	bool LoadFile(wxString & f) {
		mFileName = f;
		return mFileEditor->LoadFile(mFileName);};

////@begin InputFileEditor event handler declarations

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
	void OnCancelClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_SAVEAS
	void OnSaveasClick( wxCommandEvent& event );

////@end InputFileEditor event handler declarations

////@begin InputFileEditor member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end InputFileEditor member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin InputFileEditor member variables
	wxTextCtrl* mFileEditor;
////@end InputFileEditor member variables
	/// Local copy of the temporary file name
	wxString	mFileName;
};

#endif
	// _INPUTFILEEDITOR_H_
