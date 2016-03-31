/////////////////////////////////////////////////////////////////////////////
// Name:        coordinateswindow.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri 21 Apr 12:23:58 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _COORDINATESWINDOW_H_
#define _COORDINATESWINDOW_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "coordinateswindow.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/grid.h"
#include "wx/statusbr.h"
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
#define ID_COORDINATESDIALOG 10021
#define MMP_COPYCOORDSITEM 10027
#define ID_STICKMENU 10028
#define ID_REORDERCOORDITEM 10089
#define ID_PANEL3 10222
#define ID_BONDBUTTON 10023
#define ID_STICKBUTTON 10024
#define ID_COORDCHOICE1 10025
#define ID_COORDGRID 10026
#define ID_STATUSBAR1 10250
#define SYMBOL_COORDINATESWINDOW_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_COORDINATESWINDOW_TITLE _("Coordinates")
#define SYMBOL_COORDINATESWINDOW_IDNAME ID_COORDINATESDIALOG
#define SYMBOL_COORDINATESWINDOW_SIZE wxSize(400, 300)
#define SYMBOL_COORDINATESWINDOW_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * CoordinatesWindow class declaration
 */

class CoordinatesWindow: public wxFrame
{    
    DECLARE_DYNAMIC_CLASS( CoordinatesWindow )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    CoordinatesWindow( );
    CoordinatesWindow( MolDisplayWin* parent, wxWindowID id = SYMBOL_COORDINATESWINDOW_IDNAME, const wxString& caption = SYMBOL_COORDINATESWINDOW_TITLE, const wxPoint& pos = SYMBOL_COORDINATESWINDOW_POSITION, const wxSize& size = SYMBOL_COORDINATESWINDOW_SIZE, long style = SYMBOL_COORDINATESWINDOW_STYLE );
    ~CoordinatesWindow( ); //destructor

    /// Creation
    bool Create( MolDisplayWin* parent, wxWindowID id = SYMBOL_COORDINATESWINDOW_IDNAME, const wxString& caption = SYMBOL_COORDINATESWINDOW_TITLE, const wxPoint& pos = SYMBOL_COORDINATESWINDOW_POSITION, const wxSize& size = SYMBOL_COORDINATESWINDOW_SIZE, long style = SYMBOL_COORDINATESWINDOW_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin CoordinatesWindow event handler declarations

	/// wxEVT_CLOSE_WINDOW event handler for ID_COORDINATESDIALOG
	void OnCloseWindow( wxCloseEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_CLOSE
	void OnCloseClick( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for wxID_UNDO
	void OnUndoUpdate( wxUpdateUIEvent& event );

	/// wxEVT_UPDATE_UI event handler for wxID_CUT
	void OnCutUpdate( wxUpdateUIEvent& event );

	/// wxEVT_UPDATE_UI event handler for wxID_COPY
	void OnCopyUpdate( wxUpdateUIEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for MMP_COPYCOORDSITEM
	void OnMmpCopycoordsitemClick( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for MMP_COPYCOORDSITEM
	void OnMmpCopycoordsitemUpdate( wxUpdateUIEvent& event );
	
	/// Update event handler for the copy NWChem style coordinates update menu item
	void OnCopyNWCoordsItemUpdate( wxUpdateUIEvent& event );

	/// wxEVT_UPDATE_UI event handler for wxID_PASTE
	void OnPasteUpdate( wxUpdateUIEvent& event );

	/// wxEVT_UPDATE_UI event handler for wxID_CLEAR
	void OnClearUpdate( wxUpdateUIEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SELECTALL
	void OnSelectallClick( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for wxID_SELECTALL
	void OnSelectallUpdate( wxUpdateUIEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for ID_STICKMENU
	void OnStickmenuClick( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for ID_STICKMENU
	void OnStickmenuUpdate( wxUpdateUIEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for ID_REORDERCOORDITEM
	void OnReordercoorditemClick( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for ID_REORDERCOORDITEM
	void OnReordercoorditemUpdate( wxUpdateUIEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD
	void OnAddClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE
	void OnDeleteClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BONDBUTTON
	void OnBondbuttonClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STICKBUTTON
	void OnStickbuttonClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_COORDCHOICE1
	void OnCoordchoice1Selected( wxCommandEvent& event );

	/// wxEVT_GRID_CELL_CHANGE event handler for ID_COORDGRID
	void OnCellChange( wxGridEvent& event );

	/// wxEVT_GRID_SELECT_CELL event handler for ID_COORDGRID
	void OnSelectCell( wxGridEvent& event );

	/// wxEVT_GRID_RANGE_SELECT event handler for ID_COORDGRID
	void OnRangeSelect( wxGridRangeSelectEvent& event );

	/// wxEVT_SIZE event handler for ID_COORDGRID
	void OnSize( wxSizeEvent& event );

////@end CoordinatesWindow event handler declarations
	
	void OnActivate(wxActivateEvent & event);

////@begin CoordinatesWindow member function declarations

	MolDisplayWin * GetParent() const { return Parent ; }
	void SetParent(MolDisplayWin * value) { Parent = value ; }

	int GetCoordType() const { return CoordType ; }
	void SetCoordType(int value) { CoordType = value ; }

	WinPrefs * GetPrefs() const { return Prefs ; }
	void SetPrefs(WinPrefs * value) { Prefs = value ; }

	bool GetNeedClearAll() const { return needClearAll ; }
	void SetNeedClearAll(bool value) { needClearAll = value ; }

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end CoordinatesWindow member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();
	
	void SetupGridColumns(void);
	void FrameChanged(void);
	void SizeCols(wxSize & s);
	void UpdateControls(void);
	bool ProcessEvent(wxEvent& event);
	void UpdateSelection(bool mode);
	void UpdateWindowTitle(void);
	
////@begin CoordinatesWindow member variables
	wxButton* deleteButton;
	wxButton* BondButton;
	wxChoice* coordTypeChoice;
	wxGrid* coordGrid;
	MolDisplayWin * Parent;
	int CoordType;
	WinPrefs * Prefs;
	bool needClearAll;
////@end CoordinatesWindow member variables
};

#endif
    // _COORDINATESWINDOW_H_
