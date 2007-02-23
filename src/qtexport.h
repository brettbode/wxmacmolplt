/////////////////////////////////////////////////////////////////////////////
// Name:        qtexport.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri 23 Feb 08:44:29 2007
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _QTEXPORT_H_
#define _QTEXPORT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "qtexport.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/statline.h"
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
#define ID_QTEXPORT 10246
#define ID_QT_MOVIE_RADIO 10247
#define ID_EPLOT_CHECK 10248
#define ID_QT_COMPRESSOR_CHOICE 10068
#define ID_KEY_FRAME_EDIT 10249
#define SYMBOL_QTEXPORT_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_QTEXPORT_TITLE _("Movie Options")
#define SYMBOL_QTEXPORT_IDNAME ID_QTEXPORT
#define SYMBOL_QTEXPORT_SIZE wxSize(400, 300)
#define SYMBOL_QTEXPORT_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * QTExport class declaration
 */

class QTExport: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( QTExport )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    QTExport();
    QTExport( wxWindow* parent, wxWindowID id = SYMBOL_QTEXPORT_IDNAME, const wxString& caption = SYMBOL_QTEXPORT_TITLE, const wxPoint& pos = SYMBOL_QTEXPORT_POSITION, const wxSize& size = SYMBOL_QTEXPORT_SIZE, long style = SYMBOL_QTEXPORT_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_QTEXPORT_IDNAME, const wxString& caption = SYMBOL_QTEXPORT_TITLE, const wxPoint& pos = SYMBOL_QTEXPORT_POSITION, const wxSize& size = SYMBOL_QTEXPORT_SIZE, long style = SYMBOL_QTEXPORT_STYLE );

    /// Destructor
    ~QTExport();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin QTExport event handler declarations

	/// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_QT_MOVIE_RADIO
	void OnQtMovieRadioSelected( wxCommandEvent& event );

////@end QTExport event handler declarations

////@begin QTExport member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end QTExport member function declarations
	
	void SetMovieChoice(int v);
	void EnableFrameMovie(bool v);
	void EnableModeMovie(bool b);
	
	int GetMovieChoice(void) const;
	bool AddEnergyPlot(void) const;
	
	int GetCompressorChoice(void) const;
	int GetKeyFrameRate(void) const;

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin QTExport member variables
	wxRadioBox* mMovieType;
	wxCheckBox* mEPlotCheck;
	wxChoice* mQTChoice;
	wxTextCtrl* mKeyFrameEdit;
////@end QTExport member variables
};

#endif
    // _QTEXPORT_H_
