/////////////////////////////////////////////////////////////////////////////
// Name:        swfexport.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Thu 12 Apr 2007 04:16:34 PM CDT
// RCS-ID:      
// Copyright:   (c) 2007 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SWFEXPORT_H_
#define _SWFEXPORT_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "swfexport.h"
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
#define ID_SWFEXPORT 10251
#define ID_SWF_MOVIE_RADIO 10252
#define ID_EPLOT_CHECK 10253
#define SYMBOL_SWFEXPORT_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SWFEXPORT_TITLE _("Movie Options")
#define SYMBOL_SWFEXPORT_IDNAME ID_SWFEXPORT
#define SYMBOL_SWFEXPORT_SIZE wxSize(400, 300)
#define SYMBOL_SWFEXPORT_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * SWFExport class declaration
 */

class SWFExport: public wxDialog
{    
	DECLARE_DYNAMIC_CLASS( SWFExport )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	SWFExport();
	SWFExport( wxWindow* parent, wxWindowID id = SYMBOL_SWFEXPORT_IDNAME, const wxString& caption = SYMBOL_SWFEXPORT_TITLE, const wxPoint& pos = SYMBOL_SWFEXPORT_POSITION, const wxSize& size = SYMBOL_SWFEXPORT_SIZE, long style = SYMBOL_SWFEXPORT_STYLE );

	/// Creation
	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_SWFEXPORT_IDNAME, const wxString& caption = SYMBOL_SWFEXPORT_TITLE, const wxPoint& pos = SYMBOL_SWFEXPORT_POSITION, const wxSize& size = SYMBOL_SWFEXPORT_SIZE, long style = SYMBOL_SWFEXPORT_STYLE );

	/// Destructor
	~SWFExport();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin SWFExport event handler declarations

	/// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_SWF_MOVIE_RADIO
	void OnSwfMovieRadioSelected( wxCommandEvent& event );

////@end SWFExport event handler declarations

////@begin SWFExport member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end SWFExport member function declarations

	void SetMovieChoice(int v);
	void EnableFrameMovie(bool v);
	void EnableModeMovie(bool b);

	int GetMovieChoice(void) const;
	bool AddEnergyPlot(void) const;

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin SWFExport member variables
	wxRadioBox* mMovieType;
	wxCheckBox* mEPlotCheck;
////@end SWFExport member variables
};

#endif
	// _SWFEXPORT_H_
