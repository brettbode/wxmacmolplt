/////////////////////////////////////////////////////////////////////////////
// Name:        surfaceswindow.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Wed 19 Sep 13:37:29 2007
// RCS-ID:      
// Copyright:   (c) 2007 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SURFACESWINDOW_H_
#define _SURFACESWINDOW_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "surfaceswindow.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/notebook.h"
#include <wx/choicebk.h>
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxChoicebook;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_FRAME 10067
#define ID_PANEL2 10225
#define ID_SURFTITLE 10086
#define ID_VISIBLECHECK 10087
#define ID_ALLFRAMECHECK 10088
#define ID_SURFLISTBOOK 10069
#define SYMBOL_SURFACESWINDOW_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SURFACESWINDOW_TITLE _("Surfaces")
#define SYMBOL_SURFACESWINDOW_IDNAME ID_FRAME
#define SYMBOL_SURFACESWINDOW_SIZE wxDefaultSize
#define SYMBOL_SURFACESWINDOW_POSITION wxDefaultPosition
////@end control identifiers


/**
 * SufacesWindow is the main window managing the creation and editing of surfaces. All
 * surfaces are pages in a note book with subclasses handling the controls for each page.
 */

class SurfacesWindow: public wxFrame
{    
	DECLARE_CLASS( SurfacesWindow )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	SurfacesWindow();
	SurfacesWindow( MolDisplayWin* parent, wxWindowID id = SYMBOL_SURFACESWINDOW_IDNAME, const wxString& caption = SYMBOL_SURFACESWINDOW_TITLE, const wxPoint& pos = SYMBOL_SURFACESWINDOW_POSITION, const wxSize& size = SYMBOL_SURFACESWINDOW_SIZE, long style = SYMBOL_SURFACESWINDOW_STYLE );

	bool Create( MolDisplayWin* parent, wxWindowID id = SYMBOL_SURFACESWINDOW_IDNAME, const wxString& caption = SYMBOL_SURFACESWINDOW_TITLE, const wxPoint& pos = SYMBOL_SURFACESWINDOW_POSITION, const wxSize& size = SYMBOL_SURFACESWINDOW_SIZE, long style = SYMBOL_SURFACESWINDOW_STYLE );

	/// Destructor
	~SurfacesWindow();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();
	
	///Creates a surfacepane of the desired type.
	void addNewPane(int type);

////@begin SurfacesWindow event handler declarations

	void OnChangeTitle(wxCommandEvent& event);

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD
	void OnAddClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE
	void OnDeleteClick( wxCommandEvent& event );

////@end SurfacesWindow event handler declarations

	/// Event handler for page changes in the choice book.
	/// DialogBlocks doesn't know about ChoiceBookEvents so I need this outside of its code.
	void OnSurflistbookPageChanged( wxChoicebookEvent& event );

	/// Change the window title when the file name changes
	void UpdateWindowTitle(void);

	/// Menu handler for the close menu item
	void OnCloseEvent( wxCommandEvent& event );

////@begin SurfacesWindow member function declarations

	MolDisplayWin * GetParent() const { return Parent ; }
	void SetParent(MolDisplayWin * value) { Parent = value ; }

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
	
////@end SurfacesWindow member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

	void Reset(void);	///< Call to rebuild the dialog
	
	MoleculeData * GetMoleculeData(void) const {return Parent->GetData();};
	WinPrefs * GetPrefs(void) const {return Parent->GetPrefs();};
	/** Call when the active surface has changed to cause the
	surface title to update and the main display to redraw.
	*/
	void SurfaceUpdated(void);
	
////@begin SurfacesWindow member variables
	wxChoicebook* book;
	wxButton* mDeleteButton;
private:
	MolDisplayWin * Parent;
////@end SurfacesWindow member variables
    MoleculeData* mData;
    WinPrefs* mPrefs;
	bool	setValueCalled;	///< Flag to enable a workaround for setvalue generating a text event.
	
	///Display a dialog for the user to select the type of a new surface.
	int selectSurfaceType();
};

#endif
	// _SURFACESWINDOW_H_
