/////////////////////////////////////////////////////////////////////////////
// Name:        changeatomorderdialog.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Tue  2 Oct 11:11:35 2007
// RCS-ID:      
// Copyright:   (c) 2007 Iowa State University
/////////////////////////////////////////////////////////////////////////////

#ifndef _CHANGEATOMORDERDIALOG_H_
#define _CHANGEATOMORDERDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "changeatomorderdialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/valtext.h"
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
#define ID_CHANGEATOMORDERDIALOG 10261
#define ID_TEXTCTRL5 10262
#define SYMBOL_CHANGEATOMORDERDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_CHANGEATOMORDERDIALOG_TITLE _("Change Atom Order")
#define SYMBOL_CHANGEATOMORDERDIALOG_IDNAME ID_CHANGEATOMORDERDIALOG
#define SYMBOL_CHANGEATOMORDERDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_CHANGEATOMORDERDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * Present a dialog to obtain the desired index to move the selected atom to.
 */

class ChangeAtomOrderDialog: public wxDialog
{    
	DECLARE_DYNAMIC_CLASS( ChangeAtomOrderDialog )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	ChangeAtomOrderDialog();
	ChangeAtomOrderDialog( MolDisplayWin* parent, wxWindowID id = SYMBOL_CHANGEATOMORDERDIALOG_IDNAME, const wxString& caption = SYMBOL_CHANGEATOMORDERDIALOG_TITLE, const wxPoint& pos = SYMBOL_CHANGEATOMORDERDIALOG_POSITION, const wxSize& size = SYMBOL_CHANGEATOMORDERDIALOG_SIZE, long style = SYMBOL_CHANGEATOMORDERDIALOG_STYLE );

	/// Creation
	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CHANGEATOMORDERDIALOG_IDNAME, const wxString& caption = SYMBOL_CHANGEATOMORDERDIALOG_TITLE, const wxPoint& pos = SYMBOL_CHANGEATOMORDERDIALOG_POSITION, const wxSize& size = SYMBOL_CHANGEATOMORDERDIALOG_SIZE, long style = SYMBOL_CHANGEATOMORDERDIALOG_STYLE );

	/// Destructor
	~ChangeAtomOrderDialog();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin ChangeAtomOrderDialog event handler declarations

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL5
	void OnTextctrl5Updated( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
	void OnOkClick( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for wxID_OK
	void OnOkUpdate( wxUpdateUIEvent& event );

////@end ChangeAtomOrderDialog event handler declarations

////@begin ChangeAtomOrderDialog member function declarations

	long GetIndex() const { return index ; }
	void SetIndex(long value) { index = value ; }

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end ChangeAtomOrderDialog member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin ChangeAtomOrderDialog member variables
	wxTextCtrl* indexEdit;
private:
	long index;
////@end ChangeAtomOrderDialog member variables
	MolDisplayWin * Parent;
};

#endif
	// _CHANGEATOMORDERDIALOG_H_
