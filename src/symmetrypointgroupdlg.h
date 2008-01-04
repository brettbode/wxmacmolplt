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
#define ID_LISTCTRL1 10255
#define ID_SLIDER1 10263
#define ID_SETBUTTON 10254
#define SYMBOL_SYMMETRYPOINTGROUPDLG_STYLE wxCAPTION|wxSYSTEM_MENU
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
 * SymmetryPointGroupDlg class declaration.
 * This class presents the user with a dialog listing the possible symmetry point
 * groups for the current set of coordinates. Upon dismissing the dialog the caller
 * can retrieve the selected point group.
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

	/// wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL1
	void OnListctrl1Selected( wxListEvent& event );

	/// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER1
	void OnSlider1Updated( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SETBUTTON
	void OnSetbuttonClick( wxCommandEvent& event );

	/// wxEVT_UPDATE_UI event handler for ID_SETBUTTON
	void OnSetbuttonUpdate( wxUpdateUIEvent& event );

////@end SymmetryPointGroupDlg event handler declarations

////@begin SymmetryPointGroupDlg member function declarations

	MolDisplayWin * GetParent() const { return Parent ; }
	void SetParent(MolDisplayWin * value) { Parent = value ; }

	long GetSelection() const { return selection ; }
	void SetSelection(long value) { selection = value ; }

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end SymmetryPointGroupDlg member function declarations

	/// Should we show tooltips?
	static bool ShowToolTips();
	
	/** Call to retrieve the selected point group.
	 @param pg The GAMESS point group
	 @param pgOrder The order of the principle axis.
	 */
	bool GetSelectedPointGroup(GAMESSPointGroup & pg, int & pgOrder) const;
	/// Call to retrieve the current tolerance value.
	double GetTolerance(void) const {return tolerance;};
	
private:
	/// Call to update the list of acceptable point groups
	void setup(void);

	bool pgFlags[kNumSymmetryPointGroups+2];	///< List of flags indicating an acceptable PG
	double tolerance;							///< How close due the coords have to be to be a match

////@begin SymmetryPointGroupDlg member variables
	wxStaticText* mHighestPGText;
	wxStaticText* mAbelianPGText;
	wxListCtrl* mPGListCntl;
	wxSlider* mSlider;
	MolDisplayWin * Parent;
public:
	long selection;
////@end SymmetryPointGroupDlg member variables
};

#endif
	// _SYMMETRYPOINTGROUPDLG_H_
