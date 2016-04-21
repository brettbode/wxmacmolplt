/////////////////////////////////////////////////////////////////////////////
// Name:        fmofragmentationdialog.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Wed 23 Dec 11:01:48 2009
// RCS-ID:      
// Copyright:   (c) 2009 Iowa State University
/////////////////////////////////////////////////////////////////////////////

#ifndef _FMOFRAGMENTATIONDIALOG_H_
#define _FMOFRAGMENTATIONDIALOG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "fmofragmentationdialog.h"
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
#define ID_FMOFRAGMENTATIONDIALOG 10271
#define ID_NUMFRAGEDIT 10272
#define SYMBOL_FMOFRAGMENTATIONDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_FMOFRAGMENTATIONDIALOG_TITLE _("FMO Fragmentation")
#define SYMBOL_FMOFRAGMENTATIONDIALOG_IDNAME ID_FMOFRAGMENTATIONDIALOG
#define SYMBOL_FMOFRAGMENTATIONDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_FMOFRAGMENTATIONDIALOG_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * FMOFragmentationDialog class declaration
 */

class FMOFragmentationDialog: public wxDialog
{    
	DECLARE_DYNAMIC_CLASS( FMOFragmentationDialog )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	FMOFragmentationDialog();
	FMOFragmentationDialog( wxWindow* parent, wxWindowID id = SYMBOL_FMOFRAGMENTATIONDIALOG_IDNAME, const wxString& caption = SYMBOL_FMOFRAGMENTATIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_FMOFRAGMENTATIONDIALOG_POSITION, const wxSize& size = SYMBOL_FMOFRAGMENTATIONDIALOG_SIZE, long style = SYMBOL_FMOFRAGMENTATIONDIALOG_STYLE );

	/// Creation
	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FMOFRAGMENTATIONDIALOG_IDNAME, const wxString& caption = SYMBOL_FMOFRAGMENTATIONDIALOG_TITLE, const wxPoint& pos = SYMBOL_FMOFRAGMENTATIONDIALOG_POSITION, const wxSize& size = SYMBOL_FMOFRAGMENTATIONDIALOG_SIZE, long style = SYMBOL_FMOFRAGMENTATIONDIALOG_STYLE );

	/// Destructor
	~FMOFragmentationDialog();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin FMOFragmentationDialog event handler declarations

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_NUMFRAGEDIT
	void OnNumFragEditTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
	void OnFMOFragButtonClick( wxCommandEvent& event );

////@end FMOFragmentationDialog event handler declarations

////@begin FMOFragmentationDialog member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end FMOFragmentationDialog member function declarations
	
	/// Retreive the # non-bonded molecules per fragment
	int GetMolCount(void);

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin FMOFragmentationDialog member variables
	wxTextCtrl* mNumFragEdit;
////@end FMOFragmentationDialog member variables
};

#endif
	// _FMOFRAGMENTATIONDIALOG_H_
