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
#include "wxMolGraph.h"
////@end includes
#include "wxMolGraph.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxMenu;
class wxMolGraph;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_FREQDIALOG 10054
#define ID_FREQCOPY 10170
#define ID_PREVMODE 10084
#define ID_NEXTMODE 10085
#define ID_MENUIRINTEN 10165
#define ID_MENURAMANINTEN 10166
#define ID_MENUZOOMIN 10167
#define ID_MENUZOOMOUT 10168
#define ID_PANEL4 10226
#define ID_FREQLISTBOX 10143
#define ID_CUSTOM 10066
#define SYMBOL_FREQUENCIESDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_FREQUENCIESDIALOG_TITLE _("Frequencies")
#define SYMBOL_FREQUENCIESDIALOG_IDNAME ID_FREQDIALOG
#define SYMBOL_FREQUENCIESDIALOG_SIZE wxDefaultSize
#define SYMBOL_FREQUENCIESDIALOG_POSITION wxDefaultPosition
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

	/// wxEVT_COMMAND_MENU_SELECTED event handler for wxID_CLOSE
	void OnCloseEvent( wxCommandEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for ID_FREQCOPY
	void OnFreqcopyClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for ID_PREVMODE
	void OnPrevmodeClick( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for ID_PREVMODE
	void OnPrevmodeUpdate( wxUpdateUIEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEXTMODE
	void OnNextmodeClick( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for ID_NEXTMODE
	void OnNextmodeUpdate( wxUpdateUIEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUIRINTEN
	void OnMenuirintenClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENURAMANINTEN
	void OnMenuramanintenClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUZOOMIN
	void OnMenuzoominClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUZOOMOUT
	void OnMenuzoomoutClick( wxCommandEvent& event );

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

	/// Change the window title when the file name changes
	void UpdateWindowTitle(void);
	
////@begin FrequenciesDialog member variables
	wxMenu* editMenu;
	wxListBox* mFreqListBox;
	wxMolGraph* fGraph;
////@end FrequenciesDialog member variables
};

#endif
    // _FREQUENCIESDIALOG_H_
