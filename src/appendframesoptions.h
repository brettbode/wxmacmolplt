/////////////////////////////////////////////////////////////////////////////
// Name:        appendframesoptions.h
// Author:      Brett Bode
// Created:     Thu  4 May 12:46:21 2006
// Copyright:   (c) 2006 Iowa State University
// Licence:     See the LICENSE file in the top level directory
/////////////////////////////////////////////////////////////////////////////

#ifndef _APPENDFRAMESOPTIONS_H_
#define _APPENDFRAMESOPTIONS_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "appendframesoptions.h"
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
#define ID_APPENDFRAMEOPTIONDLG 10040
#define ID_NEGATIVECHECKBOX 10041
#define ID_OFFSETTEXTCTL 10042
#define ID_SKIPTEXTCTL 10043
#define SYMBOL_APPENDFRAMESOPTIONS_STYLE wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_APPENDFRAMESOPTIONS_TITLE _("Append Frames Options")
#define SYMBOL_APPENDFRAMESOPTIONS_IDNAME ID_APPENDFRAMEOPTIONDLG
#define SYMBOL_APPENDFRAMESOPTIONS_SIZE wxSize(400, 300)
#define SYMBOL_APPENDFRAMESOPTIONS_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * AppendFramesOptions class declaration
 */

class AppendFramesOptions: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( AppendFramesOptions )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    AppendFramesOptions( );
    AppendFramesOptions( wxWindow* parent, wxWindowID id = SYMBOL_APPENDFRAMESOPTIONS_IDNAME, const wxString& caption = SYMBOL_APPENDFRAMESOPTIONS_TITLE, const wxPoint& pos = SYMBOL_APPENDFRAMESOPTIONS_POSITION, const wxSize& size = SYMBOL_APPENDFRAMESOPTIONS_SIZE, long style = SYMBOL_APPENDFRAMESOPTIONS_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_APPENDFRAMESOPTIONS_IDNAME, const wxString& caption = SYMBOL_APPENDFRAMESOPTIONS_TITLE, const wxPoint& pos = SYMBOL_APPENDFRAMESOPTIONS_POSITION, const wxSize& size = SYMBOL_APPENDFRAMESOPTIONS_SIZE, long style = SYMBOL_APPENDFRAMESOPTIONS_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin AppendFramesOptions event handler declarations

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
	void OnOkClick( wxCommandEvent& event );

////@end AppendFramesOptions event handler declarations
	void SetupItems(void);

////@begin AppendFramesOptions member function declarations

	bool GetFlip() const { return flip ; }
	void SetFlip(bool value) { flip = value ; }

	double GetOffset() const { return offset ; }
	void SetOffset(double value) { offset = value ; }

	long GetSkip() const { return skip ; }
	void SetSkip(long value) { skip = value ; }

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource( const wxString& name );

	/// Retrieves icon resources
	wxIcon GetIconResource( const wxString& name );
////@end AppendFramesOptions member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin AppendFramesOptions member variables
	wxCheckBox* negativeCheck;
	wxTextCtrl* offsetEdit;
	wxTextCtrl* skipEdit;
	bool flip;
	double offset;
	long skip;
////@end AppendFramesOptions member variables
};

#endif
    // _APPENDFRAMESOPTIONS_H_
