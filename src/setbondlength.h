/////////////////////////////////////////////////////////////////////////////
// Name:        setbondlength.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Wed  3 May 11:01:00 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SETBONDLENGTH_H_
#define _SETBONDLENGTH_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "setbondlength.h"
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
#define ID_MYDIALOG 10032
#define SYMBOL_SETBONDLENGTH_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SETBONDLENGTH_TITLE _("Set Bond Length...")
#define SYMBOL_SETBONDLENGTH_IDNAME ID_MYDIALOG
#define SYMBOL_SETBONDLENGTH_SIZE wxSize(400, 300)
#define SYMBOL_SETBONDLENGTH_POSITION wxDefaultPosition
#define ID_TEXTCTRL1 10033
#define ID_CHECKBOX 10034
#define ID_CHECKBOX1 10035
#define ID_CHECKBOX2 10036
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * SetBondLength class declaration
 */

class SetBondLength: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( SetBondLength )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SetBondLength( );
    SetBondLength( wxWindow* parent, wxWindowID id = SYMBOL_SETBONDLENGTH_IDNAME, const wxString& caption = SYMBOL_SETBONDLENGTH_TITLE, const wxPoint& pos = SYMBOL_SETBONDLENGTH_POSITION, const wxSize& size = SYMBOL_SETBONDLENGTH_SIZE, long style = SYMBOL_SETBONDLENGTH_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SETBONDLENGTH_IDNAME, const wxString& caption = SYMBOL_SETBONDLENGTH_TITLE, const wxPoint& pos = SYMBOL_SETBONDLENGTH_POSITION, const wxSize& size = SYMBOL_SETBONDLENGTH_SIZE, long style = SYMBOL_SETBONDLENGTH_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin SetBondLength event handler declarations

////@end SetBondLength event handler declarations

////@begin SetBondLength member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SetBondLength member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin SetBondLength member variables
    wxTextCtrl* maxValueEdit;
////@end SetBondLength member variables
};

#endif
    // _SETBONDLENGTH_H_
