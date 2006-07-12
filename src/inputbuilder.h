/////////////////////////////////////////////////////////////////////////////
// Name:        inputbuilder.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Mon 12 Jun 2006 09:37:31 AM CDT
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _INPUTBUILDER_H_
#define _INPUTBUILDER_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "inputbuilder.h"
#endif

/*!
 * Includes
 */

////@begin includes
#include "wx/frame.h"
#include "wx/notebook.h"
#include "wx/spinctrl.h"
////@end includes

#include "wxUglyChoice.h"
#include "InputData.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxListbook;
class wxUglyChoice;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_INPUTBUILDERWINDOW 10071
#define SYMBOL_INPUTBUILDERWINDOW_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_INPUTBUILDERWINDOW_TITLE _("Input Builder")
#define SYMBOL_INPUTBUILDERWINDOW_IDNAME ID_INPUTBUILDERWINDOW
#define SYMBOL_INPUTBUILDERWINDOW_SIZE wxDefaultSize
#define SYMBOL_INPUTBUILDERWINDOW_POSITION wxDefaultPosition
#define ID_IBNOTEBOOK 10072
#define ID_IBBASISPANEL 10091
#define ID_BASIS_CHOICE 10098
#define ID_ECPT_CHOICE 10099
#define ID_NUMD_CHOICE 10100
#define ID_NUMF_CHOICE 10101
#define ID_NUMP_CHOICE 10102
#define ID_POLAR_CHOICE 10103
#define ID_DIFFL_CHECKBOX 10104
#define ID_DIFFS_CHECKBOX 10105
#define ID_IBCONTROLPANEL 10092
#define ID_RUN_CHOICE 10106
#define ID_SCF_CHOICE 10107
#define ID_LOCAL_CHOICE 10108
#define ID_MCHARGE_TEXTCTRL 10109
#define ID_MULT_TEXTCTRL 10110
#define ID_EXE_CHOICE 10111
#define ID_MAXIT_TEXTCTRL 10112
#define ID_MP2_CHECKBOX 10113
#define ID_DFT_CHECKBOX 10114
#define ID_CI_CHOICE 10115
#define ID_CC_CHOICE 10116
#define ID_IBDATAPANEL 10093
#define ID_TITLE_TEXTCTRL 10117
#define ID_COORD_CHOICE 10118
#define ID_UNIT_CHOICE 10119
#define ID_TEXTCTRL10 10123
#define ID_POINTGROUP_CHOICE 10120
#define ID_ORDER_CHOICE 10121
#define ID_SYMMETRY_CHECKBOX 10122
#define ID_IBSYSTEMPANEL 10094
#define ID_SPINCTRL 10164
#define ID_CHOICE19 10125
#define ID_SPINCTRL1 10124
#define ID_CHOICE20 10127
#define ID_CHOICE21 10128
#define ID_CHECKBOX9 10129
#define ID_RADIOBOX 10130
#define ID_CHECKBOX10 10131
#define ID_IBDFTPANEL 10181
#define ID_CHOICE3 10183
#define ID_CHOICE4 10184
#define ID_IBMOGUESSPANEL 10095
#define ID_CHOICE22 10132
#define ID_CHECKBOX11 10133
#define ID_CHECKBOX12 10134
#define ID_IBHESSOPTSPANEL 10144
#define ID_RADIOBOX2 10145
#define ID_TEXTCTRL15 10151
#define ID_CHECKBOX20 10146
#define ID_CHECKBOX21 10147
#define ID_CHECKBOX22 10148
#define ID_CHECKBOX23 10149
#define ID_TEXTCTRL14 10150
#define ID_IBMISCPANEL 10096
#define ID_CHECKBOX13 10136
#define ID_CHECKBOX14 10137
#define ID_CHECKBOX15 10138
#define ID_CHECKBOX16 10139
#define ID_RADIOBOX1 10135
#define ID_IBMP2PANEL 10182
#define ID_TEXTCTRL5 10185
#define ID_TEXTCTRL7 10186
#define ID_TEXTCTRL8 10187
#define ID_CHECKBOX3 10188
#define ID_RADIOBOX4 10189
#define ID_RADIOBOX5 10190
#define ID_IBSCBOPTSPANEL 10097
#define ID_CHECKBOX17 10140
#define ID_CHECKBOX18 10141
#define ID_CHECKBOX19 10142
#define ID_SPINCTRL2 10126
#define ID_IBSTATPOINTPANEL 10152
#define ID_TEXTCTRL16 10153
#define ID_TEXTCTRL17 10154
#define ID_CHOICE23 10155
#define ID_TEXTCTRL18 10159
#define ID_TEXTCTRL19 10160
#define ID_TEXTCTRL20 10161
#define ID_CHECKBOX25 10157
#define ID_CHECKBOX26 10158
#define ID_RADIOBOX3 10162
#define ID_TEXTCTRL21 10163
#define ID_CHECKBOX24 10156
#define ID_BUTTON 10073
#define ID_BUTTON1 10090
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * InputBuilderWindow class declaration
 */

class InputBuilderWindow: public wxFrame
{    
    DECLARE_CLASS( InputBuilderWindow )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    InputBuilderWindow( );
    InputBuilderWindow( wxWindow* parent, wxWindowID id = SYMBOL_INPUTBUILDERWINDOW_IDNAME, const wxString& caption = SYMBOL_INPUTBUILDERWINDOW_TITLE, const wxPoint& pos = SYMBOL_INPUTBUILDERWINDOW_POSITION, const wxSize& size = SYMBOL_INPUTBUILDERWINDOW_SIZE, long style = SYMBOL_INPUTBUILDERWINDOW_STYLE );
    
    ~InputBuilderWindow();
    
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_INPUTBUILDERWINDOW_IDNAME, const wxString& caption = SYMBOL_INPUTBUILDERWINDOW_TITLE, const wxPoint& pos = SYMBOL_INPUTBUILDERWINDOW_POSITION, const wxSize& size = SYMBOL_INPUTBUILDERWINDOW_SIZE, long style = SYMBOL_INPUTBUILDERWINDOW_STYLE );

    /// Creates the controls and sizers
    void CreateControls();

////@begin InputBuilderWindow event handler declarations

    /// wxEVT_CLOSE_WINDOW event handler for ID_INPUTBUILDERWINDOW
    void OnCloseWindow( wxCloseEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_BASIS_CHOICE
    void OnBasisChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ECPT_CHOICE
    void OnEcptChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_NUMD_CHOICE
    void OnNumdChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_NUMF_CHOICE
    void OnNumfChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_NUMP_CHOICE
    void OnNumpChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_POLAR_CHOICE
    void OnPolarChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DIFFL_CHECKBOX
    void OnDifflCheckboxClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DIFFS_CHECKBOX
    void OnDiffsCheckboxClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_RUN_CHOICE
    void OnRunChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_SCF_CHOICE
    void OnScfChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_LOCAL_CHOICE
    void OnLocalChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_EXE_CHOICE
    void OnExeChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_MP2_CHECKBOX
    void OnMp2CheckboxClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DFT_CHECKBOX
    void OnDftCheckboxClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CI_CHOICE
    void OnCiChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CC_CHOICE
    void OnCcChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
    void OnCancelClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
    void OnOkClick( wxCommandEvent& event );

////@end InputBuilderWindow event handler declarations

////@begin InputBuilderWindow member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end InputBuilderWindow member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();
    
    void SetupItems();

////@begin InputBuilderWindow member variables
    wxListbook* listBook;
    wxUglyChoice* basisChoice;
    wxUglyChoice* ecpTypeChoice;
    wxUglyChoice* numDChoice;
    wxUglyChoice* numFChoice;
    wxUglyChoice* numPChoice;
    wxUglyChoice* polarChoice;
    wxCheckBox* diffuseLCheck;
    wxCheckBox* diffuseSCheck;
    wxUglyChoice* runChoice;
    wxUglyChoice* scfChoice;
    wxUglyChoice* localChoice;
    wxTextCtrl* mchargeText;
    wxTextCtrl* multText;
    wxUglyChoice* exeChoice;
    wxTextCtrl* scfIterText;
    wxCheckBox* mp2Check;
    wxCheckBox* dftCheck;
    wxUglyChoice* ciChoice;
    wxUglyChoice* ccChoice;
    wxTextCtrl* titleText;
    wxUglyChoice* coordTypeChoice;
    wxUglyChoice* unitChoice;
    wxTextCtrl* zmatrixVarsText;
    wxUglyChoice* pointGroupChoice;
    wxUglyChoice* paxisOrderChoice;
    wxCheckBox* symmetryCheck;
    wxCheckBox* aimpacCheck;
    wxCheckBox* rpacCheck;
    wxWindow * tabMOGuess;
    wxWindow * tabHessOpts;
    wxWindow * tabMiscPrefs;
    wxWindow * tabSCFOpts;
    wxWindow * tabStatPoint;
    wxWindow * tabBasis;
    wxWindow * tabControl;
    wxWindow * tabData;
    wxWindow * tabSystem;
    wxWindow * tabDFT;
    wxWindow * tabMP2Opts;
////@end InputBuilderWindow member variables

    InputData * TmpInputRec;
};

#endif
    // _INPUTBUILDER_H_
