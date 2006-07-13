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
class wxSpinCtrl;
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
#define ID_PANEL6 10230
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
#define ID_TIMELIMIT_SPINCTRL 10164
#define ID_TIMELIMITUNITS_CHOICE 10125
#define ID_MEMORY_SPINCTRL 10124
#define ID_MEMORYUNITS_CHOICE 10127
#define ID_DIAGONALIZATION_CHOICE 10128
#define ID_CORE_CHECKBOX 10129
#define ID_PLBALANCE_RADIOBOX 10130
#define ID_EXTERNDATAREP_CHECKBOX 10131
#define ID_IBDFTPANEL 10181
#define ID_DFTMETHOD_CHOICE 10183
#define ID_FUNCTIONAL_CHOICE 10184
#define ID_IBMOGUESSPANEL 10095
#define ID_INITGUESS_CHOICE 10132
#define ID_PRINTINITGUESS_CHECKBOX 10133
#define ID_ROTORBITALS_CHECKBOX 10134
#define ID_IBHESSOPTSPANEL 10144
#define ID_RADIOBOX 10145
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
#define ID_SUMMARYPANEL 10229
#define ID_DEFAULTSBUTTON 10228
#define ID_REVERTBUTTON 10090
#define ID_WRITEFILEBUTTON 10073
////@end control identifiers


enum {
    BASIS_PANE,
    CONTROL_PANE,
    DATA_PANE,
    SYSTEM_PANE,
    DFT_PANE,
    MOGUESS_PANE,
    HESSOPTS_PANE,
    MISCPREFS_PANE,
    MP2OPTS_PANE,
    SCFOPTS_PANE,
    STATPOINT_PANE,
    SUMMARY_PANE,
    NUM_PANES
};


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

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_COORD_CHOICE
    void OnCoordChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_UNIT_CHOICE
    void OnUnitChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_POINTGROUP_CHOICE
    void OnPointgroupChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ORDER_CHOICE
    void OnOrderChoiceSelected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SYMMETRY_CHECKBOX
    void OnSymmetryCheckboxClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DEFAULTSBUTTON
    void OnDefaultsbuttonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_REVERTBUTTON
    void OnRevertbuttonClick( wxCommandEvent& event );

    /// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_WRITEFILEBUTTON
    void OnWritefilebuttonClick( wxCommandEvent& event );

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
    void SetupBasisItems();
    void SetupControlItems();
    void SetupDataItems();
    void SetupSystemItems();
    void SetupDFTItems();
    void SetupMOGuessItems();
    void SetupHessOptsItems();
    void SetupMiscPrefsItems();
    void SetupMP2OptsItems();
    void SetupSCFOptsItems();
    void SetupStatPointItems();
    void SetupSummaryItems();

////@begin InputBuilderWindow member variables
    wxListbook* listBook;
    wxUglyChoice* basisChoice;
    wxStaticText* ecpTypeLabel;
    wxUglyChoice* ecpTypeChoice;
    wxUglyChoice* numDChoice;
    wxUglyChoice* numFChoice;
    wxUglyChoice* numPChoice;
    wxStaticText* polarLabel;
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
    wxStaticText* ciLabel;
    wxUglyChoice* ciChoice;
    wxStaticText* ccLabel;
    wxUglyChoice* ccChoice;
    wxTextCtrl* titleText;
    wxUglyChoice* coordTypeChoice;
    wxUglyChoice* unitChoice;
    wxTextCtrl* zmatrixVarsText;
    wxUglyChoice* pointGroupChoice;
    wxUglyChoice* paxisOrderChoice;
    wxCheckBox* symmetryCheck;
    wxSpinCtrl* timeLimitSpin;
    wxUglyChoice* timeLimitUnitChoice;
    wxSpinCtrl* memorySpin;
    wxUglyChoice* memoryUnitChoice;
    wxUglyChoice* diagChoice;
    wxCheckBox* coreCheck;
    wxRadioBox* plBalanceRadio;
    wxCheckBox* externDataRepCheck;
    wxUglyChoice* dftMethodChoice;
    wxUglyChoice* dftFuncChoice;
    wxUglyChoice* initGuessChoice;
    wxCheckBox* printInitGuessCheck;
    wxCheckBox* rotOrbitalsCheck;
    wxCheckBox* aimpacCheck;
    wxCheckBox* rpacCheck;
    wxButton* defaultsBtn;
    wxButton* revertBtn;
    wxButton* writeBtn;
////@end InputBuilderWindow member variables

    InputData * TmpInputRec;
    
    wxWindow *tab[NUM_PANES];
    wxString  tabText[NUM_PANES];
    bool visibleTab[NUM_PANES];
    
    void setPaneVisible(int pane, bool visible);
    int getCurrentPane();
};

#endif
    // _INPUTBUILDER_H_
