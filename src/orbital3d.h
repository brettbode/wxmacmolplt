/////////////////////////////////////////////////////////////////////////////
// Name:        orbital3d.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri  2 Jun 10:08:18 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _ORBITAL3D_H_
#define _ORBITAL3D_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "orbital3d.h"
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
#define ID_MYDIALOG4 10070
#define SYMBOL_ORBITAL3D_STYLE wxSUNKEN_BORDER
#define SYMBOL_ORBITAL3D_TITLE _("3D Orbital")
#define SYMBOL_ORBITAL3D_IDNAME ID_MYDIALOG4
#define SYMBOL_ORBITAL3D_SIZE wxSize(400, 300)
#define SYMBOL_ORBITAL3D_POSITION wxDefaultPosition
#define ID_TEXTCTRL5 10071
#define ID_CHECKBOX3 10072
#define ID_CHECKBOX4 10073
#define ID_CHOICE1 10074
#define ID_CHECKBOX5 10075
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * Orbital3D class declaration
 */

class Orbital3D: public wxPanel
{    
    DECLARE_DYNAMIC_CLASS( Orbital3D )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    Orbital3D( );
    Orbital3D( wxWindow* parent, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin Orbital3D event handler declarations

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL5
    void OnTextctrl5Updated( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX3
    void OnCheckbox3Click( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX4
    void OnCheckbox4Click( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE1
    void OnChoice1Selected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX5
    void OnCheckbox5Click( wxCommandEvent& event );

////@end Orbital3D event handler declarations

////@begin Orbital3D member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end Orbital3D member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin Orbital3D member variables
    wxTextCtrl* titleEdit;
    wxCheckBox* visCheck;
    wxCheckBox* allFrameCheck;
    wxChoice* orbSetChoice;
    wxCheckBox* rPhaseCheck;
////@end Orbital3D member variables
};

#endif
    // _ORBITAL3D_H_
