/////////////////////////////////////////////////////////////////////////////
// Name:        setbondlength.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Wed  3 May 11:01:00 2006
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
#define ID_MAXLENGTHEDIT 10033
#define ID_KEEPOLDCHECKBOX 10034
#define ID_NORMALCHECKBOX 10035
#define ID_NORMALSLIDER 10039
#define ID_GUESSBONDCHECKBOX 10036
#define ID_HHCHECKBOX 10037
#define ID_HYDROGENCHECK 10038
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
    SetBondLength( MolDisplayWin* parent, wxWindowID id = SYMBOL_SETBONDLENGTH_IDNAME, const wxString& caption = SYMBOL_SETBONDLENGTH_TITLE, const wxPoint& pos = SYMBOL_SETBONDLENGTH_POSITION, const wxSize& size = SYMBOL_SETBONDLENGTH_SIZE, long style = SYMBOL_SETBONDLENGTH_STYLE );

    /// Creation
    bool Create( MolDisplayWin* parent, wxWindowID id = SYMBOL_SETBONDLENGTH_IDNAME, const wxString& caption = SYMBOL_SETBONDLENGTH_TITLE, const wxPoint& pos = SYMBOL_SETBONDLENGTH_POSITION, const wxSize& size = SYMBOL_SETBONDLENGTH_SIZE, long style = SYMBOL_SETBONDLENGTH_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin SetBondLength event handler declarations

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MAXLENGTHEDIT
    void OnMaxlengtheditUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_NORMALCHECKBOX
    void OnNormalcheckboxClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end SetBondLength event handler declarations

////@begin SetBondLength member function declarations

    MolDisplayWin * GetParent() const { return Parent ; }
    void SetParent(MolDisplayWin * value) { Parent = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end SetBondLength member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin SetBondLength member variables
    wxTextCtrl* maxValueEdit;
    wxCheckBox* keepOldBondsCheck;
    wxCheckBox* normalBondCheck;
    wxStaticText* normalText;
    wxSlider* normalTolSlider;
    wxCheckBox* guessBondCheck;
    wxCheckBox* hhCheck;
    wxCheckBox* hydrogenCheck;
    MolDisplayWin * Parent;
////@end SetBondLength member variables
};

#endif
    // _SETBONDLENGTH_H_
