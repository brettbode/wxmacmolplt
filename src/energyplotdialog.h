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
#define ID_ENERGYPLOTDIALOG 10053
#define SYMBOL_ENERGYPLOTDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ENERGYPLOTDIALOG_TITLE _("Energy Plot")
#define SYMBOL_ENERGYPLOTDIALOG_IDNAME ID_ENERGYPLOTDIALOG
#define SYMBOL_ENERGYPLOTDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_ENERGYPLOTDIALOG_POSITION wxDefaultPosition
#define ID_EPGRAPH 10054
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

class EnergyPlotDialog: public wxDialog
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
////@end EnergyPlotDialog event handler declarations

////@begin EnergyPlotDialog member function declarations
    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end EnergyPlotDialog member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin EnergyPlotDialog member variables
    wxMolGraph* epGraph;
////@end EnergyPlotDialog member variables
};

#endif
    // _ENERGYPLOTDIALOG_H_
