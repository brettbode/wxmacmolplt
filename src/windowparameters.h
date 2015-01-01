/////////////////////////////////////////////////////////////////////////////
// Name:        windowparameters.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Wed 28 Jun 13:01:36 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WINDOWPARAMETERS_H_
#define _WINDOWPARAMETERS_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "windowparameters.h"
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
#define ID_MYDIALOG5 10169
#define ID_XWINSIZEEDIT 10171
#define ID_YWINSIZE 10172
#define ID_UNITSRADIOBOX 10173
#define ID_WINSCALE 10174
#define ID_XWINCENTER 10175
#define ID_YWINCENTER 10176
#define ID_ZWINCENTER 10177
#define ID_PSIANGLEEDIT 10178
#define ID_PHIANGLEEDIT 10179
#define ID_THETAANGLEEDIT 10180
#define ID_SetDefaultButton 10273
#define SYMBOL_WINDOWPARAMETERS_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_WINDOWPARAMETERS_TITLE _("Manual window parameters")
#define SYMBOL_WINDOWPARAMETERS_IDNAME ID_MYDIALOG5
#define SYMBOL_WINDOWPARAMETERS_SIZE wxSize(400, 300)
#define SYMBOL_WINDOWPARAMETERS_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * windowparameters class declaration
 */

class windowparameters: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( windowparameters )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    windowparameters( );
    windowparameters( wxWindow* parent, wxWindowID id = SYMBOL_WINDOWPARAMETERS_IDNAME, const wxString& caption = SYMBOL_WINDOWPARAMETERS_TITLE, const wxPoint& pos = SYMBOL_WINDOWPARAMETERS_POSITION, const wxSize& size = SYMBOL_WINDOWPARAMETERS_SIZE, long style = SYMBOL_WINDOWPARAMETERS_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_WINDOWPARAMETERS_IDNAME, const wxString& caption = SYMBOL_WINDOWPARAMETERS_TITLE, const wxPoint& pos = SYMBOL_WINDOWPARAMETERS_POSITION, const wxSize& size = SYMBOL_WINDOWPARAMETERS_SIZE, long style = SYMBOL_WINDOWPARAMETERS_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin windowparameters event handler declarations

	/// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_UNITSRADIOBOX
	void OnUnitsradioboxSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SetDefaultButton
	void OnSetDefaultButtonClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_APPLY
	void OnApplyClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
	void OnOkClick( wxCommandEvent& event );

////@end windowparameters event handler declarations

////@begin windowparameters member function declarations

	wxString GetXCenter() const { return XCenter ; }
	void SetXCenter(wxString value) { XCenter = value ; }

	wxString GetYCenter() const { return YCenter ; }
	void SetYCenter(wxString value) { YCenter = value ; }

	wxString GetZCenter() const { return ZCenter ; }
	void SetZCenter(wxString value) { ZCenter = value ; }

	wxString GetPhi() const { return phi ; }
	void SetPhi(wxString value) { phi = value ; }

	wxString GetPsi() const { return psi ; }
	void SetPsi(wxString value) { psi = value ; }

	wxString GetScreenX() const { return screenX ; }
	void SetScreenX(wxString value) { screenX = value ; }

	wxString GetScreenY() const { return screenY ; }
	void SetScreenY(wxString value) { screenY = value ; }

	wxString GetTheta() const { return theta ; }
	void SetTheta(wxString value) { theta = value ; }

	wxString GetWindowScale() const { return windowScale ; }
	void SetWindowScale(wxString value) { windowScale = value ; }

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end windowparameters member function declarations
	
	bool Apply(void);

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin windowparameters member variables
	wxTextCtrl* mXWinSize;
	wxTextCtrl* mYWinSize;
	wxRadioBox* mUnitRadios;
	wxTextCtrl* mWindowScale;
	wxTextCtrl* mXWinCenter;
	wxTextCtrl* mYWinCenter;
	wxTextCtrl* mZWinCenter;
	wxTextCtrl* mPsiEdit;
	wxTextCtrl* mPhiEdit;
	wxTextCtrl* mThetaEdit;
	wxString XCenter;
	wxString YCenter;
	wxString ZCenter;
	wxString phi;
	wxString psi;
	wxString screenX;
	wxString screenY;
	wxString theta;
	wxString windowScale;
////@end windowparameters member variables
};

#endif
    // _WINDOWPARAMETERS_H_
