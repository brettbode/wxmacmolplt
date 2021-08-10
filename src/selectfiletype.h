/////////////////////////////////////////////////////////////////////////////
// Name:        selectfiletype.h
// Purpose:     
// Author:      Brett Bode
// Created:     Sat  3 Sep 20:35:29 2016
// Copyright:   (c) 2016 Iowa State University
// Licence:     See the LICENSE file in the top level directory
/////////////////////////////////////////////////////////////////////////////

#ifndef _SELECTFILETYPE_H_
#define _SELECTFILETYPE_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "selectfiletype.h"
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
#define ID_SELECTFILETYPE 10000
#define ID_FILEDISPLAYTEXTCTRL 10001
#define ID_FILETYPECHOICE 10002
#define SYMBOL_SELECTFILETYPE_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_SELECTFILETYPE_TITLE _("SelectFileType")
#define SYMBOL_SELECTFILETYPE_IDNAME ID_SELECTFILETYPE
#define SYMBOL_SELECTFILETYPE_SIZE wxSize(400, 300)
#define SYMBOL_SELECTFILETYPE_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * SelectFileType class declaration
 */

class SelectFileType: public wxDialog
{    
	DECLARE_DYNAMIC_CLASS( SelectFileType )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	SelectFileType();
	SelectFileType( wxWindow* parent, wxWindowID id = SYMBOL_SELECTFILETYPE_IDNAME, const wxString& caption = SYMBOL_SELECTFILETYPE_TITLE, const wxPoint& pos = SYMBOL_SELECTFILETYPE_POSITION, const wxSize& size = SYMBOL_SELECTFILETYPE_SIZE, long style = SYMBOL_SELECTFILETYPE_STYLE );

	/// Creation
	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SELECTFILETYPE_IDNAME, const wxString& caption = SYMBOL_SELECTFILETYPE_TITLE, const wxPoint& pos = SYMBOL_SELECTFILETYPE_POSITION, const wxSize& size = SYMBOL_SELECTFILETYPE_SIZE, long style = SYMBOL_SELECTFILETYPE_STYLE );

	/// Destructor
	~SelectFileType();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();
	
	/// Set the file to display
	bool LoadFile(wxString & f) {return mFileText->LoadFile(f);};
	
	/// Grab the selected file type
	int GetFileType(void) const {return mFileType->GetSelection();};

////@begin SelectFileType event handler declarations

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_FILETYPECHOICE
	void OnFiletypechoiceSelected( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for wxID_OK
	void OnOkUpdate( wxUpdateUIEvent& event );

////@end SelectFileType event handler declarations

////@begin SelectFileType member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end SelectFileType member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin SelectFileType member variables
	wxTextCtrl* mFileText;
	wxChoice* mFileType;
////@end SelectFileType member variables
};

#endif
	// _SELECTFILETYPE_H_
