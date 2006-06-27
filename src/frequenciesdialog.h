/////////////////////////////////////////////////////////////////////////////
// Name:        frequenciesdialog.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Wed 31 May 2006 12:01:12 PM CDT
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _FREQUENCIESDIALOG_H_
#define _FREQUENCIESDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "frequenciesdialog.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
////@end includes
#include "wxMolGraph.h"

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
#define ID_FREQDIALOG 10054
#define SYMBOL_FREQUENCIESDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_FREQUENCIESDIALOG_TITLE _("Frequencies")
#define SYMBOL_FREQUENCIESDIALOG_IDNAME ID_FREQDIALOG
#define SYMBOL_FREQUENCIESDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_FREQUENCIESDIALOG_POSITION wxDefaultPosition
#define ID_PREVMODE 10084
#define ID_NEXTMODE 10085
#define ID_FREQLISTBOX 10143
#define ID_CUSTOM 10066
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * FrequenciesDialog class declaration
 */

class FrequenciesDialog: public wxFrame
{    
    DECLARE_CLASS( FrequenciesDialog )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    FrequenciesDialog( );
    FrequenciesDialog( wxWindow* parent, wxWindowID id = SYMBOL_FREQUENCIESDIALOG_IDNAME, const wxString& caption = SYMBOL_FREQUENCIESDIALOG_TITLE, const wxPoint& pos = SYMBOL_FREQUENCIESDIALOG_POSITION, const wxSize& size = SYMBOL_FREQUENCIESDIALOG_SIZE, long style = SYMBOL_FREQUENCIESDIALOG_STYLE );

    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FREQUENCIESDIALOG_IDNAME, const wxString& caption = SYMBOL_FREQUENCIESDIALOG_TITLE, const wxPoint& pos = SYMBOL_FREQUENCIESDIALOG_POSITION, const wxSize& size = SYMBOL_FREQUENCIESDIALOG_SIZE, long style = SYMBOL_FREQUENCIESDIALOG_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin FrequenciesDialog event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_FREQDIALOG
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_SIZE event handler for ID_FREQDIALOG
    void OnSize( wxSizeEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_COPY
    void OnCopyClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_PREVMODE
    void OnPrevmodeClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEXTMODE
    void OnNextmodeClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_FREQLISTBOX
    void OnFreqlistboxSelected( wxCommandEvent& event );

////@end FrequenciesDialog event handler declarations
	/// wxEVT_GRAPH_CLICK event handler for ID_CUSTOM
	void OnCustomGraphClick( wxCommandEvent& event );

////@begin FrequenciesDialog member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end FrequenciesDialog member function declarations

    void FrameChanged(void);
    void ModeChanged(void);
    void RegenData(void);

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin FrequenciesDialog member variables
    wxListBox* mFreqListBox;
    wxMolGraph* fGraph;
////@end FrequenciesDialog member variables
};

#endif
    // _FREQUENCIESDIALOG_H_
