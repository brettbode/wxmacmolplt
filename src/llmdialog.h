/////////////////////////////////////////////////////////////////////////////
// Name:        llmdialog.h
// Purpose:     
// Author:      Brett Bode
// Created:     Tue  2 May 13:24:40 2006
// Copyright:   (c) 2006 Iowa State University
// Licence:     See the LICENSE file in the top level directory
/////////////////////////////////////////////////////////////////////////////

#ifndef _LLMDIALOG_H_
#define _LLMDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "llmdialog.h"
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
#define ID_LLMDIALOG 10029
#define ID_TEXTCTRL 10030
#define ID_INTERNALSCHECKBOX 10031
#define SYMBOL_LLMDIALOG_STYLE wxDEFAULT_DIALOG_STYLE|wxSYSTEM_MENU|wxSTAY_ON_TOP|wxCLOSE_BOX
#define SYMBOL_LLMDIALOG_TITLE _("LLMDialog")
#define SYMBOL_LLMDIALOG_IDNAME ID_LLMDIALOG
#define SYMBOL_LLMDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_LLMDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * LLMDialog class declaration
 */

class LLMDialog: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( LLMDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    LLMDialog( );
    LLMDialog( MolDisplayWin* parent, wxWindowID id = SYMBOL_LLMDIALOG_IDNAME, const wxString& caption = SYMBOL_LLMDIALOG_TITLE, const wxPoint& pos = SYMBOL_LLMDIALOG_POSITION, const wxSize& size = SYMBOL_LLMDIALOG_SIZE, long style = SYMBOL_LLMDIALOG_STYLE );

    /// Creation
    bool Create( MolDisplayWin* parent, wxWindowID id = SYMBOL_LLMDIALOG_IDNAME, const wxString& caption = SYMBOL_LLMDIALOG_TITLE, const wxPoint& pos = SYMBOL_LLMDIALOG_POSITION, const wxSize& size = SYMBOL_LLMDIALOG_SIZE, long style = SYMBOL_LLMDIALOG_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin LLMDialog event handler declarations

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
	void OnOkClick( wxCommandEvent& event );

////@end LLMDialog event handler declarations

////@begin LLMDialog member function declarations

	MolDisplayWin * GetParent() const { return Parent ; }
	void SetParent(MolDisplayWin * value) { Parent = value ; }

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end LLMDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin LLMDialog member variables
	wxTextCtrl* numPointsEdit;
	wxCheckBox* internalCheck;
	MolDisplayWin * Parent;
////@end LLMDialog member variables
};

#endif
    // _LLMDIALOG_H_
