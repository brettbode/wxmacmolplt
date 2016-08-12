/////////////////////////////////////////////////////////////////////////////
// Name:        frameenergy.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Mon 29 May 21:14:10 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _FRAMEENERGY_H_
#define _FRAMEENERGY_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "frameenergy.h"
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
#define ID_MYDIALOG3 10060
#define ID_TEXTCTRL2 10061
#define ID_MP2Text 10064
#define ID_TEXTCTRL3 10062
#define ID_TEXTCTRL4 10063
#define SYMBOL_FRAMEENERGY_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_FRAMEENERGY_TITLE _("Frame Energy")
#define SYMBOL_FRAMEENERGY_IDNAME ID_MYDIALOG3
#define SYMBOL_FRAMEENERGY_SIZE wxSize(400, 300)
#define SYMBOL_FRAMEENERGY_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * FrameEnergy class declaration
 */

class FrameEnergy: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( FrameEnergy )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    FrameEnergy( );
    FrameEnergy( MolDisplayWin * parent, wxWindowID id = SYMBOL_FRAMEENERGY_IDNAME, const wxString& caption = SYMBOL_FRAMEENERGY_TITLE, const wxPoint& pos = SYMBOL_FRAMEENERGY_POSITION, const wxSize& size = SYMBOL_FRAMEENERGY_SIZE, long style = SYMBOL_FRAMEENERGY_STYLE );

    /// Creation
    bool Create( MolDisplayWin * parent, wxWindowID id = SYMBOL_FRAMEENERGY_IDNAME, const wxString& caption = SYMBOL_FRAMEENERGY_TITLE, const wxPoint& pos = SYMBOL_FRAMEENERGY_POSITION, const wxSize& size = SYMBOL_FRAMEENERGY_SIZE, long style = SYMBOL_FRAMEENERGY_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin FrameEnergy event handler declarations

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
	void OnOkClick( wxCommandEvent& event );

////@end FrameEnergy event handler declarations

////@begin FrameEnergy member function declarations

	MolDisplayWin * GetParent() const { return Parent ; }
	void SetParent(MolDisplayWin * value) { Parent = value ; }

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end FrameEnergy member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin FrameEnergy member variables
	wxTextCtrl* TEedit;
	wxTextCtrl* MP2edit;
	wxTextCtrl* KEedit;
	wxTextCtrl* TimeEdit;
	MolDisplayWin * Parent;
////@end FrameEnergy member variables
};

#endif
    // _FRAMEENERGY_H_
