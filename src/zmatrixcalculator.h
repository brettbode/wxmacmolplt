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
#define SYMBOL_ZMATRIXCALCULATOR_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ZMATRIXCALCULATOR_TITLE _("Z-Matrix Calculator")
#define SYMBOL_ZMATRIXCALCULATOR_IDNAME ID_MYDIALOG6
#define SYMBOL_ZMATRIXCALCULATOR_SIZE wxSize(400, 300)
#define SYMBOL_ZMATRIXCALCULATOR_POSITION wxDefaultPosition
#define ID_ATOM1EDIT 10192
#define ID_ATOM2EDIT 10193
#define ID_ATOM3EDIT 10194
#define ID_ATOM4EDIT 10195
#define ID_BONDLENGTHEDIT 10196
#define ID_BONDANGLEEDIT 10197
#define ID_DIHEDRALANGLEEDIT 10198
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

class ZMatrixCalculator: public wxDialog
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

    /// wxEVT_CLOSE_WINDOW event handler for ID_MYDIALOG6
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM1EDIT
    void OnAtom1editUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM2EDIT
    void OnAtom2editUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM3EDIT
    void OnAtom3editUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM4EDIT
    void OnAtom4editUpdated( wxCommandEvent& event );

////@end ZMatrixCalculator event handler declarations

////@begin ZMatrixCalculator member function declarations

    wxString GetAtom1() const { return atom1 ; }
    void SetAtom1(wxString value) { atom1 = value ; }

    wxString GetAtom2() const { return atom2 ; }
    void SetAtom2(wxString value) { atom2 = value ; }

    wxString GetAtom3() const { return atom3 ; }
    void SetAtom3(wxString value) { atom3 = value ; }

    wxString GetAtom4() const { return atom4 ; }
    void SetAtom4(wxString value) { atom4 = value ; }

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
    wxString atom1;
    wxString atom2;
    wxString atom3;
    wxString atom4;
////@end ZMatrixCalculator member variables
};

#endif
    // _ZMATRIXCALCULATOR_H_
