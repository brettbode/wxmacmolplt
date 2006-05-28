/////////////////////////////////////////////////////////////////////////////
// Name:        choosevecgroup.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri 26 May 08:35:16 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _CHOOSEVECGROUP_H_
#define _CHOOSEVECGROUP_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "choosevecgroup.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/grid.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxGrid;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_DIALOG 10055
#define SYMBOL_CHOOSEVECGROUP_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_CHOOSEVECGROUP_TITLE _("Choose $VEC group...")
#define SYMBOL_CHOOSEVECGROUP_IDNAME ID_DIALOG
#define SYMBOL_CHOOSEVECGROUP_SIZE wxSize(400, 300)
#define SYMBOL_CHOOSEVECGROUP_POSITION wxDefaultPosition
#define ID_GRID 10056
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * ChooseVECgroup class declaration
 */

class ChooseVECgroup: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( ChooseVECgroup )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    ChooseVECgroup( );
    ChooseVECgroup( wxWindow* parent, wxWindowID id = SYMBOL_CHOOSEVECGROUP_IDNAME, const wxString& caption = SYMBOL_CHOOSEVECGROUP_TITLE, const wxPoint& pos = SYMBOL_CHOOSEVECGROUP_POSITION, const wxSize& size = SYMBOL_CHOOSEVECGROUP_SIZE, long style = SYMBOL_CHOOSEVECGROUP_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_CHOOSEVECGROUP_IDNAME, const wxString& caption = SYMBOL_CHOOSEVECGROUP_TITLE, const wxPoint& pos = SYMBOL_CHOOSEVECGROUP_POSITION, const wxSize& size = SYMBOL_CHOOSEVECGROUP_SIZE, long style = SYMBOL_CHOOSEVECGROUP_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin ChooseVECgroup event handler declarations

    /// wxEVT_GRID_EDITOR_SHOWN event handler for ID_GRID
    void OnEditorShown( wxGridEvent& event );

    /// wxEVT_UPDATE_UI event handler for wxID_OK
    void OnOkUpdate( wxUpdateUIEvent& event );

////@end ChooseVECgroup event handler declarations

////@begin ChooseVECgroup member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end ChooseVECgroup member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();
	void SetBuffer(BufferFile * b);
	int GetTarget(void);

////@begin ChooseVECgroup member variables
    wxGrid* list;
////@end ChooseVECgroup member variables
};

#endif
    // _CHOOSEVECGROUP_H_
