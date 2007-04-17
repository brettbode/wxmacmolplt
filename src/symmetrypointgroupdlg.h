/////////////////////////////////////////////////////////////////////////////
// Name:        symmetrypointgroupdlg.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Sat  7 Apr 09:51:08 2007
// RCS-ID:      
// Copyright:   (c) 2007 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SYMMETRYPOINTGROUPDLG_H_
#define _SYMMETRYPOINTGROUPDLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "symmetrypointgroupdlg.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/listctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxListCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_SYMMETRYPOINTGROUPDLG 10251
#define ID_LISTCTRL1 10253
#define ID_SETBUTTON 10254
#define SYMBOL_SYMMETRYPOINTGROUPDLG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_SYMMETRYPOINTGROUPDLG_TITLE _("Symmetry Point Group")
#define SYMBOL_SYMMETRYPOINTGROUPDLG_IDNAME ID_SYMMETRYPOINTGROUPDLG
#define SYMBOL_SYMMETRYPOINTGROUPDLG_SIZE wxSize(400, 300)
#define SYMBOL_SYMMETRYPOINTGROUPDLG_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * SymmetryPointGroupDlg class declaration
 */

class SymmetryPointGroupDlg: public wxDialog
{    
	DECLARE_DYNAMIC_CLASS( SymmetryPointGroupDlg )
	DECLARE_EVENT_TABLE()

public:
	/// Constructors
	SymmetryPointGroupDlg();
	SymmetryPointGroupDlg( MolDisplayWin* parent, wxWindowID id = SYMBOL_SYMMETRYPOINTGROUPDLG_IDNAME, const wxString& caption = SYMBOL_SYMMETRYPOINTGROUPDLG_TITLE, const wxPoint& pos = SYMBOL_SYMMETRYPOINTGROUPDLG_POSITION, const wxSize& size = SYMBOL_SYMMETRYPOINTGROUPDLG_SIZE, long style = SYMBOL_SYMMETRYPOINTGROUPDLG_STYLE );

	/// Creation
	bool Create( MolDisplayWin* parent, wxWindowID id = SYMBOL_SYMMETRYPOINTGROUPDLG_IDNAME, const wxString& caption = SYMBOL_SYMMETRYPOINTGROUPDLG_TITLE, const wxPoint& pos = SYMBOL_SYMMETRYPOINTGROUPDLG_POSITION, const wxSize& size = SYMBOL_SYMMETRYPOINTGROUPDLG_SIZE, long style = SYMBOL_SYMMETRYPOINTGROUPDLG_STYLE );

	/// Destructor
	~SymmetryPointGroupDlg();

	/// Initialises member variables
	void Init();

	/// Creates the controls and sizers
	void CreateControls();

////@begin SymmetryPointGroupDlg event handler declarations

////@end SymmetryPointGroupDlg event handler declarations

////@begin SymmetryPointGroupDlg member function declarations

	MolDisplayWin * GetParent() const { return Parent ; }
	void SetParent(MolDisplayWin * value) { Parent = value ; }

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end SymmetryPointGroupDlg member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();

////@begin SymmetryPointGroupDlg member variables
	wxStaticText* mHighestPGText;
	wxStaticText* mAbelianPGText;
	wxListCtrl* mPGListCntl;
	MolDisplayWin * Parent;
////@end SymmetryPointGroupDlg member variables
};

#endif
	// _SYMMETRYPOINTGROUPDLG_H_
