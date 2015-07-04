/////////////////////////////////////////////////////////////////////////////
// Name:        energyplotdialog.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Tue 23 May 2006 01:34:03 PM CDT
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _ENERGYPLOTDIALOG_H_
#define _ENERGYPLOTDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "energyplotdialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wxMolGraph.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxMolGraph;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_EPLOTDIALOG 10065
#define ID_PANEL5 10227
#define ID_EPGRAPH 10000
#define SYMBOL_ENERGYPLOTDIALOG_STYLE wxDEFAULT_FRAME_STYLE|wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ENERGYPLOTDIALOG_TITLE _("Energy Plot")
#define SYMBOL_ENERGYPLOTDIALOG_IDNAME ID_EPLOTDIALOG
#define SYMBOL_ENERGYPLOTDIALOG_SIZE wxDefaultSize
#define SYMBOL_ENERGYPLOTDIALOG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * EnergyPlotDialog class declaration
 */

class EnergyPlotDialog: public wxFrame
{    
    DECLARE_DYNAMIC_CLASS( EnergyPlotDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    EnergyPlotDialog( );
    EnergyPlotDialog( wxWindow* parent, wxWindowID id = SYMBOL_ENERGYPLOTDIALOG_IDNAME, const wxString& caption = SYMBOL_ENERGYPLOTDIALOG_TITLE, const wxPoint& pos = SYMBOL_ENERGYPLOTDIALOG_POSITION, const wxSize& size = SYMBOL_ENERGYPLOTDIALOG_SIZE, long style = SYMBOL_ENERGYPLOTDIALOG_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ENERGYPLOTDIALOG_IDNAME, const wxString& caption = SYMBOL_ENERGYPLOTDIALOG_TITLE, const wxPoint& pos = SYMBOL_ENERGYPLOTDIALOG_POSITION, const wxSize& size = SYMBOL_ENERGYPLOTDIALOG_SIZE, long style = SYMBOL_ENERGYPLOTDIALOG_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin EnergyPlotDialog event handler declarations

	/// wxEVT_CLOSE_WINDOW event handler for ID_EPLOTDIALOG
	void OnCloseWindow( wxCloseEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_CLOSE
	void OnCloseEvent( wxCommandEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_COPY
	void OnCopyClick( wxCommandEvent& event );

////@end EnergyPlotDialog event handler declarations

    /// wxEVT_AXIS_DCLICK event handler for ID_EPGRAPH
	void OnEpgraphAxisDClick( wxCommandEvent& event );
	
    /// wxEVT_GRAPH_CLICK event handler for ID_EPGRAPH
	void OnEpgraphGraphClick( wxCommandEvent& event );
	
	/// Key event handler
	void KeyHandler(wxKeyEvent &event);

////@begin EnergyPlotDialog member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );

	/// Change the window title when the file name changes
	void UpdateWindowTitle(void);
	
////@end EnergyPlotDialog member function declarations

    void FrameChanged(void);
    void RegenData(void);
	void CopyToBitMap(wxBitmap ** target);

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin EnergyPlotDialog member variables
	wxMolGraph* epGraph;
////@end EnergyPlotDialog member variables
};

#endif
    // _ENERGYPLOTDIALOG_H_
