/////////////////////////////////////////////////////////////////////////////
// Name:        coordinateoffset.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Sat 27 May 21:58:09 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _COORDINATEOFFSET_H_
#define _COORDINATEOFFSET_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "coordinateoffset.h"
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
#define ID_MYDIALOG2 10057
#define ID_SLIDER 10058
#define ID_TEXTCTRL1 10059
#define SYMBOL_COORDINATEOFFSET_STYLE wxCAPTION|wxSYSTEM_MENU
#define SYMBOL_COORDINATEOFFSET_TITLE _("Coordinate Offset")
#define SYMBOL_COORDINATEOFFSET_IDNAME ID_MYDIALOG2
#define SYMBOL_COORDINATEOFFSET_SIZE wxSize(400, 300)
#define SYMBOL_COORDINATEOFFSET_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * CoordinateOffset class declaration
 */

class CoordinateOffset: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( CoordinateOffset )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CoordinateOffset( );
    CoordinateOffset( MolDisplayWin* parent, wxWindowID id = SYMBOL_COORDINATEOFFSET_IDNAME, const wxString& caption = SYMBOL_COORDINATEOFFSET_TITLE, const wxPoint& pos = SYMBOL_COORDINATEOFFSET_POSITION, const wxSize& size = SYMBOL_COORDINATEOFFSET_SIZE, long style = SYMBOL_COORDINATEOFFSET_STYLE );

	~CoordinateOffset(void);
    /// Creation
    bool Create( MolDisplayWin* parent, wxWindowID id = SYMBOL_COORDINATEOFFSET_IDNAME, const wxString& caption = SYMBOL_COORDINATEOFFSET_TITLE, const wxPoint& pos = SYMBOL_COORDINATEOFFSET_POSITION, const wxSize& size = SYMBOL_COORDINATEOFFSET_SIZE, long style = SYMBOL_COORDINATEOFFSET_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin CoordinateOffset event handler declarations

    /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER
    void OnSliderUpdated( wxCommandEvent& event );

    /// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL1
    void OnTextctrl1Updated( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end CoordinateOffset event handler declarations

////@begin CoordinateOffset member function declarations

    MolDisplayWin * GetParent() const { return Parent ; }
    void SetParent(MolDisplayWin * value) { Parent = value ; }

    std::vector<CPoint3D> GetOrigCoords() const { return origCoords ; }
    void SetOrigCoords(std::vector<CPoint3D> value) { origCoords = value ; }

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end CoordinateOffset member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();
	
	void ApplyOffset(float offset);

////@begin CoordinateOffset member variables
    wxSlider* slider;
    wxTextCtrl* editField;
    MolDisplayWin * Parent;
    std::vector<CPoint3D> origCoords;
	wxString valid_string;
////@end CoordinateOffset member variables
};

#endif
    // _COORDINATEOFFSET_H_
