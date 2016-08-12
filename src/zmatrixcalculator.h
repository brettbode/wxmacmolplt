/////////////////////////////////////////////////////////////////////////////
// Name:        zmatrixcalculator.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri 30 Jun 10:43:34 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _ZMATRIXCALCULATOR_H_
#define _ZMATRIXCALCULATOR_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "zmatrixcalculator.h"
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
#define ID_MYDIALOG6 10191
#define ID_ATOM1EDIT 10192
#define ID_ATOM2EDIT 10193
#define ID_ATOM3EDIT 10194
#define ID_ATOM4EDIT 10195
#define ID_BONDLENGTHEDIT 10196
#define ID_BONDANGLEEDIT 10197
#define ID_DIHEDRALANGLEEDIT 10198
#define ID_PANEL7 10199
#define SYMBOL_ZMATRIXCALCULATOR_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_ZMATRIXCALCULATOR_TITLE _("Z-Matrix Calculator")
#define SYMBOL_ZMATRIXCALCULATOR_IDNAME ID_MYDIALOG6
#define SYMBOL_ZMATRIXCALCULATOR_SIZE wxSize(400, 300)
#define SYMBOL_ZMATRIXCALCULATOR_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * ZMatrixCalculator class declaration
 */

class ZMatrixCalculator: public wxFrame
{    
    DECLARE_DYNAMIC_CLASS( ZMatrixCalculator )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ZMatrixCalculator( );
    ZMatrixCalculator( wxWindow* parent, wxWindowID id = SYMBOL_ZMATRIXCALCULATOR_IDNAME, const wxString& caption = SYMBOL_ZMATRIXCALCULATOR_TITLE, const wxPoint& pos = SYMBOL_ZMATRIXCALCULATOR_POSITION, const wxSize& size = SYMBOL_ZMATRIXCALCULATOR_SIZE, long style = SYMBOL_ZMATRIXCALCULATOR_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ZMATRIXCALCULATOR_IDNAME, const wxString& caption = SYMBOL_ZMATRIXCALCULATOR_TITLE, const wxPoint& pos = SYMBOL_ZMATRIXCALCULATOR_POSITION, const wxSize& size = SYMBOL_ZMATRIXCALCULATOR_SIZE, long style = SYMBOL_ZMATRIXCALCULATOR_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin ZMatrixCalculator event handler declarations

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM1EDIT
	void OnAtom1editUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM2EDIT
	void OnAtom2editUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM3EDIT
	void OnAtom3editUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM4EDIT
	void OnAtom4editUpdated( wxCommandEvent& event );

////@end ZMatrixCalculator event handler declarations
	/// Menu handler for the close menu item
	void OnCloseEvent( wxCommandEvent& event );
	/// wxEVT_CLOSE_WINDOW event handler for ZMatrixCalculator
	void OnCloseWindow( wxCloseEvent& event );

////@begin ZMatrixCalculator member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end ZMatrixCalculator member function declarations
	
	void UpdateValues(void);

    /// Should we show tooltips?
    static bool ShowToolTips();

	/// Change the window title when the file name changes
	void UpdateWindowTitle(void);
	
////@begin ZMatrixCalculator member variables
	wxTextCtrl* mAtom1Edit;
	wxTextCtrl* mAtom2Edit;
	wxTextCtrl* mAtom3Edit;
	wxTextCtrl* mAtom4Edit;
	wxTextCtrl* mBondLength;
	wxTextCtrl* mBondAngle;
	wxTextCtrl* mDihedralAngle;
////@end ZMatrixCalculator member variables
};

#endif
    // _ZMATRIXCALCULATOR_H_
