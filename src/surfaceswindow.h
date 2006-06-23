/////////////////////////////////////////////////////////////////////////////
// Name:        surfaceswindow.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri  2 Jun 09:56:43 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
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
//#include "wx/notebook.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxListbook;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_FRAME 10067
#define SYMBOL_SURFACESWINDOW_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SURFACESWINDOW_TITLE _("Surfaces")
#define SYMBOL_SURFACESWINDOW_IDNAME ID_FRAME
#define SYMBOL_SURFACESWINDOW_SIZE wxSize(560,550)
#define SYMBOL_SURFACESWINDOW_POSITION wxDefaultPosition
#define ID_SURFTITLE 10086
#define ID_VISIBLECHECK 10087
#define ID_ALLFRAMECHECK 10088
#define ID_SURFLISTBOOK 10069
#define ID_ADDSURFCHOICE 10089
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif
  

/*!
 * SurfacesWindow class declaration
 */

class SurfacesWindow: public wxFrame
{    
    DECLARE_CLASS( SurfacesWindow )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    SurfacesWindow( );
    ~SurfacesWindow( );
    SurfacesWindow( MolDisplayWin* parent, wxWindowID id = SYMBOL_SURFACESWINDOW_IDNAME, const wxString& caption = SYMBOL_SURFACESWINDOW_TITLE, const wxPoint& pos = SYMBOL_SURFACESWINDOW_POSITION, const wxSize& size = SYMBOL_SURFACESWINDOW_SIZE, long style = SYMBOL_SURFACESWINDOW_STYLE );

    bool Create( MolDisplayWin* parent, wxWindowID id = SYMBOL_SURFACESWINDOW_IDNAME, const wxString& caption = SYMBOL_SURFACESWINDOW_TITLE, const wxPoint& pos = SYMBOL_SURFACESWINDOW_POSITION, const wxSize& size = SYMBOL_SURFACESWINDOW_SIZE, long style = SYMBOL_SURFACESWINDOW_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin SurfacesWindow event handler declarations
    void OnDeleteClick( wxCommandEvent& event );
    void OnAddClick( wxCommandEvent& event );
    void OnClose( wxCloseEvent& event );
////@end SurfacesWindow event handler declarations

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
	
    void Reset(void);	//Call to rebuild the dialog

 private:

    int selectSurfaceType();

    wxBoxSizer* mainSizer;
    wxBoxSizer* upperSizer;
    wxBoxSizer* middleSizer;
    wxBoxSizer* bottomSizer;
    wxTextCtrl* surfTitleEdit;
    wxCheckBox* visibleCheck;
    wxCheckBox* allFrameCheck;
    wxListbook* listBook;
    wxButton* addButton;
    wxButton* delButton;
    MolDisplayWin * Parent;

    wxString mCurrSurfStr;

    MoleculeData* mData;
    WinPrefs* mPrefs;
};

class SurfacePane : public wxPanel {
public:
	SurfacePane(wxWindow * parent, MolDisplayWin * owner, Surface * target);
protected:
	wxBoxSizer *	mMainSizer;
};

class TEDPane : public SurfacePane {
public:
	TEDPane(wxWindow * parent, MolDisplayWin * owner, Surface * target);
protected:
	wxBoxSizer *	mOrbSetSizer;
	wxChoice *		orbSetChoice;
};

class TEDensity2DPane : public TEDPane {
public:
	TEDensity2DPane(wxWindow * parent, MolDisplayWin * owner, TEDensity2DSurface * target);
private:
};

#endif
    // _SURFACESWINDOW_H_
