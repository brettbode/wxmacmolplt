/////////////////////////////////////////////////////////////////////////////
// Name:        inputbuilder.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Mon 12 Jun 2006 09:37:31 AM CDT
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _INPUTBUILDER_H_
#define _INPUTBUILDER_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "inputbuilder.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/notebook.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxListbook;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_FRAME 10071
#define SYMBOL_INPUTBUILDER_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_INPUTBUILDER_TITLE _("Input Builder")
#define SYMBOL_INPUTBUILDER_IDNAME ID_FRAME
#define SYMBOL_INPUTBUILDER_SIZE wxDefaultSize
#define SYMBOL_INPUTBUILDER_POSITION wxDefaultPosition
#define ID_NOTEBOOK 10072
#define ID_PANEL 10091
#define ID_CHOICE3 10098
#define ID_CHOICE4 10099
#define ID_CHOICE5 10100
#define ID_CHOICE6 10101
#define ID_CHOICE7 10102
#define ID_CHOICE8 10103
#define ID_CHECKBOX3 10104
#define ID_CHECKBOX4 10105
#define ID_PANEL1 10092
#define ID_CHOICE9 10106
#define ID_CHOICE10 10107
#define ID_CHOICE11 10108
#define ID_TEXTCTRL5 10109
#define ID_TEXTCTRL7 10110
#define ID_CHOICE12 10111
#define ID_TEXTCTRL8 10112
#define ID_CHECKBOX6 10113
#define ID_CHECKBOX7 10114
#define ID_CHOICE13 10115
#define ID_CHOICE14 10116
#define ID_PANEL2 10093
#define ID_TEXTCTRL9 10117
#define ID_CHOICE15 10118
#define ID_CHOICE16 10119
#define ID_TEXTCTRL10 10123
#define ID_CHOICE17 10120
#define ID_CHOICE18 10121
#define ID_CHECKBOX8 10122
#define ID_PANEL3 10094
#define ID_TEXTCTRL11 10124
#define ID_CHOICE19 10125
#define ID_TEXTCTRL12 10126
#define ID_CHOICE20 10127
#define ID_CHOICE21 10128
#define ID_CHECKBOX9 10129
#define ID_RADIOBOX 10130
#define ID_CHECKBOX10 10131
#define ID_PANEL4 10095
#define ID_CHOICE22 10132
#define ID_CHECKBOX11 10133
#define ID_CHECKBOX12 10134
#define ID_PANEL5 10096
#define ID_CHECKBOX13 10136
#define ID_CHECKBOX14 10137
#define ID_CHECKBOX15 10138
#define ID_CHECKBOX16 10139
#define ID_RADIOBOX1 10135
#define ID_PANEL6 10097
#define ID_CHECKBOX17 10140
#define ID_CHECKBOX18 10141
#define ID_CHECKBOX19 10142
#define ID_TEXTCTRL13 10143
#define ID_BUTTON 10073
#define ID_BUTTON1 10090
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * InputBuilder class declaration
 */

class InputBuilder: public wxFrame
{    
    DECLARE_CLASS( InputBuilder )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    InputBuilder( );
    InputBuilder( wxWindow* parent, wxWindowID id = SYMBOL_INPUTBUILDER_IDNAME, const wxString& caption = SYMBOL_INPUTBUILDER_TITLE, const wxPoint& pos = SYMBOL_INPUTBUILDER_POSITION, const wxSize& size = SYMBOL_INPUTBUILDER_SIZE, long style = SYMBOL_INPUTBUILDER_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_INPUTBUILDER_IDNAME, const wxString& caption = SYMBOL_INPUTBUILDER_TITLE, const wxPoint& pos = SYMBOL_INPUTBUILDER_POSITION, const wxSize& size = SYMBOL_INPUTBUILDER_SIZE, long style = SYMBOL_INPUTBUILDER_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin InputBuilder event handler declarations

////@end InputBuilder event handler declarations

////@begin InputBuilder member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end InputBuilder member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin InputBuilder member variables
    wxListbook* listBook;
////@end InputBuilder member variables
};

#endif
    // _INPUTBUILDER_H_
