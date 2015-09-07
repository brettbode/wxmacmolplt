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
#include "wx/listbook.h"
#include "wx/grid.h"
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
class wxBoxSizer;
class wxGrid;
class wxSpinCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_INPUTBUILDERWINDOW 10071
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
#define ID_SPHERE_CHECK 10256
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
#define ID_ZMAT_VARS_TEXT 10123
#define ID_POINTGROUP_CHOICE 10120
#define ID_ORDER_CHOICE 10121
#define ID_SYMMETRY_CHECKBOX 10122
#define ID_IBSYSTEMPANEL 10094
#define ID_TIMELIMIT_TEXT 10237
#define ID_TIMELIMITUNITS_CHOICE 10125
#define ID_MEMORY_TEXT 10164
#define ID_MEMORYUNITS_CHOICE 10127
#define ID_MEMDDI_EDIT 10242
#define ID_MEMDDICHOICE 10000
#define ID_DIAGONALIZATION_CHOICE 10128
#define ID_CORE_CHECKBOX 10129
#define ID_PLBALANCE_RADIOBOX 10130
#define ID_EXTERNDATAREP_CHECKBOX 10131
#define ID_PARALL_CHECK 10243
#define ID_GVBPANEL 10002
#define ID_GVB_NCOEDIT 10003
#define ID_GVB_PAIRSEDIT 10004
#define ID_GVB_NSETOEDIT 10005
#define ID_GVB_NOEDIT 10006
#define ID_IBDFTPANEL 10181
#define ID_DFTMETHOD_CHOICE 10183
#define ID_GRID_FUNCTIONAL_CHOICE 10184
#define ID_GRIDFREE_FUNCTIONAL_CHOICE 10185
#define ID_FMOPANE 10265
#define ID_FMOACTIVECHECK 10267
#define ID_FMOFRAGCOUNT 10266
#define ID_FMOLIST 10268
#define ID_FMOFRAGBUTTON 10269
#define ID_IBMOGUESSPANEL 10095
#define ID_INITGUESS_CHOICE 10132
#define ID_MOGUESS_VECSOURCE_CHOICE 10124
#define ID_MOGUESS_VECORBCOUNT_TEXT 10238
#define ID_PRINTINITGUESS_CHECKBOX 10133
#define ID_ROTORBITALS_CHECKBOX 10134
#define ID_IBHESSOPTSPANEL 10144
#define ID_HESSMETHOD_RADIO 10145
#define ID_FORCEDISPLACE_EDIT 10151
#define ID_HESSDOUBLEDIFFCHECK 10146
#define ID_HESSPURIFYCHECK 10147
#define ID_HESSPRTINTFC_CHECK 10148
#define ID_HESSVIBANL_CHECK 10149
#define ID_HESSFREQSCALE_EDIT 10150
#define ID_IBMISCPANEL 10096
#define ID_MOLPLT_CHECK 10136
#define ID_PLTORB_CHECK 10137
#define ID_AIMPAC_CHECK 10138
#define ID_RPAC_CHECK 10139
#define ID_MISC_PROG_RADIO 10135
#define ID_IBMP2PANEL 10182
#define ID_MP2_CORE_EDIT 10185
#define ID_MP2MEM_EDIT 10186
#define ID_MP2INTCUTOFF_EDIT 10187
#define ID_LMOMP2_CHECK 10188
#define ID_MP2PROP_CHECK 10241
#define ID_MP2TRANS_RADIO 10189
#define ID_MP2AOSTORAGE_RADIO 10190
#define ID_IBSCBOPTSPANEL 10097
#define ID_DIRECTSCF_CHECK 10140
#define ID_FDIFF_CHECK 10141
#define ID_UHF_NO_CHECK 10142
#define ID_SCF_CONV_SPIN 10126
#define ID_IBSTATPOINTPANEL 10152
#define ID_MAXSTEPCOUNT_EDIT 10153
#define ID_GRADCONVCRITERIA_EDIT 10154
#define ID_OPTMETHOD_CHOICE 10155
#define ID_INITSTEPSIZE_EDIT 10159
#define ID_MINSTEPSIZE_EDIT 10160
#define ID_MAXSTEPSIZE_CHECK 10161
#define ID_UPDATESTEPSIZE_CHECK 10157
#define ID_STATPT_CHECK 10158
#define ID_STATPT_JUMP_EDIT 10239
#define ID_INIT_HESS_RADIO 10162
#define ID_HESS_RECALC_EDIT 10163
#define ID_STATPT_MODE_EDIT 10240
#define ID_PRINTORBS_CHECK 10156
#define ID_HESSEND_CHECK 10264
#define ID_SUMMARYPANEL 10229
#define ID_SUMMARY_TITLE 10231
#define ID_SUMMARY_BASISSET 10232
#define ID_SUMMARY_SCFTYPE 10233
#define ID_SUMMARY_RUNTYPE 10234
#define ID_SUMMARY_PG 10235
#define ID_SUMMARY_ELEC 10236
#define ID_DEFAULTSBUTTON 10228
#define ID_REVERTBUTTON 10090
#define ID_WRITEFILEBUTTON 10073
#define ID_ADVANCEDBUTTON 10270
#define SYMBOL_INPUTBUILDERWINDOW_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_INPUTBUILDERWINDOW_TITLE _("Input Builder")
#define SYMBOL_INPUTBUILDERWINDOW_IDNAME ID_INPUTBUILDERWINDOW
#define SYMBOL_INPUTBUILDERWINDOW_SIZE wxDefaultSize
#define SYMBOL_INPUTBUILDERWINDOW_POSITION wxDefaultPosition
////@end control identifiers
#define ID_ENQUEUEBUTTON 10273


enum {
    BASIS_PANE,
    CONTROL_PANE,
    DATA_PANE,
    SYSTEM_PANE,
	GVB_PANE,
    DFT_PANE,
	FMO_Pane,
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
	
	/// Saves the temporary data back to the main data structure
	void SaveData(void) const;

	bool ShowBasis() const;
	bool ShowStatPoint() const;
	bool ShowMP2Opts() const;
	bool ShowHessOpts() const;
	bool ShowSCFOpts() const;

////@begin InputBuilderWindow event handler declarations

	/// wxEVT_CLOSE_WINDOW event handler for ID_INPUTBUILDERWINDOW
	void OnCloseWindow( wxCloseEvent& event );

	/// wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED event handler for ID_IBNOTEBOOK
	void OnIblistbookPageChanged( wxListbookEvent& event );

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

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SPHERE_CHECK
	void OnSphereCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_RUN_CHOICE
	void OnRunChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_SCF_CHOICE
	void OnScfChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_LOCAL_CHOICE
	void OnLocalChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MCHARGE_TEXTCTRL
	void OnMchargeTextctrlUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MULT_TEXTCTRL
	void OnMultTextctrlUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_EXE_CHOICE
	void OnExeChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MAXIT_TEXTCTRL
	void OnMaxitTextctrlUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_MP2_CHECKBOX
	void OnMp2CheckboxClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DFT_CHECKBOX
	void OnDftCheckboxClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CI_CHOICE
	void OnCiChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CC_CHOICE
	void OnCcChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TITLE_TEXTCTRL
	void OnTitleTextctrlUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_COORD_CHOICE
	void OnCoordChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_UNIT_CHOICE
	void OnUnitChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ZMAT_VARS_TEXT
	void OnZmatVarsTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_POINTGROUP_CHOICE
	void OnPointgroupChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ORDER_CHOICE
	void OnOrderChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SYMMETRY_CHECKBOX
	void OnSymmetryCheckboxClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TIMELIMIT_TEXT
	void OnTimelimitTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TIMELIMITUNITS_CHOICE
	void OnTimelimitunitsChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MEMORY_TEXT
	void OnMemoryTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MEMORYUNITS_CHOICE
	void OnMemoryunitsChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MEMDDI_EDIT
	void OnMemddiEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MEMDDICHOICE
	void OnMemddichoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_DIAGONALIZATION_CHOICE
	void OnDiagonalizationChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CORE_CHECKBOX
	void OnCoreCheckboxClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_PLBALANCE_RADIOBOX
	void OnPlbalanceRadioboxSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_EXTERNDATAREP_CHECKBOX
	void OnExterndatarepCheckboxClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PARALL_CHECK
	void OnParallCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_GVB_NCOEDIT
	void OnGVBNCOEditTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_GVB_PAIRSEDIT
	void OnGVBPairsEditTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_GVB_NSETOEDIT
	void OnGVBNSetOeditTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_GVB_NOEDIT
	void OnGVBNOeditTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_DFTMETHOD_CHOICE
	void OnDftmethodChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_GRID_FUNCTIONAL_CHOICE
	void OnGridFunctionalChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_GRIDFREE_FUNCTIONAL_CHOICE
	void OnGridfreeFunctionalChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_FMOACTIVECHECK
	void OnFMOActiveCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_FMOFRAGCOUNT
	void OnFMOFragCountTextUpdated( wxCommandEvent& event );

	/// wxEVT_GRID_CELL_CHANGE event handler for ID_FMOLIST
	void OnFMOFragIdCellChange( wxGridEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_FMOFRAGBUTTON
	void OnFMOFragButtonClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_INITGUESS_CHOICE
	void OnInitguessChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MOGUESS_VECSOURCE_CHOICE
	void OnMoguessVecsourceChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MOGUESS_VECORBCOUNT_TEXT
	void OnMoguessVecorbcountTextUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PRINTINITGUESS_CHECKBOX
	void OnPrintinitguessCheckboxClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_ROTORBITALS_CHECKBOX
	void OnRotorbitalsCheckboxClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_HESSMETHOD_RADIO
	void OnHessmethodRadioSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_FORCEDISPLACE_EDIT
	void OnForcedisplaceEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HESSDOUBLEDIFFCHECK
	void OnHessdoublediffcheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HESSPURIFYCHECK
	void OnHesspurifycheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HESSPRTINTFC_CHECK
	void OnHessprtintfcCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HESSVIBANL_CHECK
	void OnHessvibanlCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_HESSFREQSCALE_EDIT
	void OnHessfreqscaleEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_MOLPLT_CHECK
	void OnMolpltCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PLTORB_CHECK
	void OnPltorbCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_AIMPAC_CHECK
	void OnAimpacCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_RPAC_CHECK
	void OnRpacCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_MISC_PROG_RADIO
	void OnMiscProgRadioSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MP2_CORE_EDIT
	void OnMp2CoreEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MP2MEM_EDIT
	void OnMp2memEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MP2INTCUTOFF_EDIT
	void OnMp2intcutoffEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_LMOMP2_CHECK
	void OnLmomp2CheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_MP2PROP_CHECK
	void OnMp2propCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_MP2TRANS_RADIO
	void OnMp2transRadioSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_MP2AOSTORAGE_RADIO
	void OnMp2aostorageRadioSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DIRECTSCF_CHECK
	void OnDirectscfCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_FDIFF_CHECK
	void OnFdiffCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_UHF_NO_CHECK
	void OnUhfNoCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SCF_CONV_SPIN
	void OnScfConvSpinUpdated( wxSpinEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MAXSTEPCOUNT_EDIT
	void OnMaxstepcountEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_GRADCONVCRITERIA_EDIT
	void OnGradconvcriteriaEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_OPTMETHOD_CHOICE
	void OnOptmethodChoiceSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_INITSTEPSIZE_EDIT
	void OnInitstepsizeEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MINSTEPSIZE_EDIT
	void OnMinstepsizeEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MAXSTEPSIZE_CHECK
	void OnMaxstepsizeEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_UPDATESTEPSIZE_CHECK
	void OnUpdatestepsizeCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_STATPT_CHECK
	void OnStatptCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_STATPT_JUMP_EDIT
	void OnStatptJumpEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_INIT_HESS_RADIO
	void OnInitHessRadioSelected( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_HESS_RECALC_EDIT
	void OnHessRecalcEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_TEXT_UPDATED event handler for ID_STATPT_MODE_EDIT
	void OnStatptModeEditUpdated( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PRINTORBS_CHECK
	void OnPrintorbsCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HESSEND_CHECK
	void OnHessendCheckClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DEFAULTSBUTTON
	void OnDefaultsbuttonClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_REVERTBUTTON
	void OnRevertbuttonClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_WRITEFILEBUTTON
	void OnWritefilebuttonClick( wxCommandEvent& event );

	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADVANCEDBUTTON
	void OnAdvancedButtonClicked( wxCommandEvent& event );

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

	/// Change the window title when the file name changes
	void UpdateWindowTitle(void);
	
	/// wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ENQUEUEBUTTON
	void OnEnqueueButtonClick( wxCommandEvent& event );
    
    void SetupItems();
    void SetupBasisItems();
    void SetupControlItems();
    void SetupDataItems();
    void SetupSystemItems();
	void SetupGVBItems();
	void SetupDFTItems();
	void SetupFMOItems();
    void SetupMOGuessItems();
    void SetupHessOptsItems();
    void SetupMiscPrefsItems();
    void SetupMP2OptsItems();
    void SetupSCFOptsItems();
    void SetupStatPointItems();
    void SetupSummaryItems();
	
	void SetupPointGroupOrder(void);
	///Call when the frame or atoms in it have changed in case the input builder needs to update
	void FrameChanged(void);
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
	wxCheckBox* spherHarmCheck;
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
	wxStaticText* paxisOrderText;
	wxUglyChoice* paxisOrderChoice;
	wxCheckBox* symmetryCheck;
	wxTextCtrl* timeLimitText;
	wxUglyChoice* timeLimitUnitChoice;
	wxTextCtrl* memoryText;
	wxUglyChoice* memoryUnitChoice;
	wxTextCtrl* memddiText;
	wxUglyChoice* memddiUnitChoice;
	wxUglyChoice* diagChoice;
	wxCheckBox* coreCheck;
	wxRadioBox* plBalanceRadio;
	wxCheckBox* externDataRepCheck;
	wxCheckBox* mSGParallCheck;
	wxTextCtrl* mGVB_NCOEdit;
	wxTextCtrl* mGVB_PairsEdit;
	wxTextCtrl* mGVB_NSETOEdit;
	wxStaticText* mGVB_NOStatic;
	wxTextCtrl* mGVB_NOEdit;
	wxUglyChoice* dftMethodChoice;
	wxBoxSizer* funcSizer;
	wxUglyChoice* dftGridFuncChoice;
	wxUglyChoice* dftGridFreeFuncChoice;
	wxCheckBox* mFMOCheck;
	wxTextCtrl* mNumFragsEdit;
	wxGrid* mFMOFragList;
	wxButton* mFMOGenFragsButton;
	wxUglyChoice* initGuessChoice;
	wxUglyChoice* mMOSourceChoice;
	wxTextCtrl* mVecOrbCountText;
	wxCheckBox* printInitGuessCheck;
	wxCheckBox* rotOrbitalsCheck;
	wxRadioBox* mHessMethodRadio;
	wxTextCtrl* mHESSDisplaceEdit;
	wxCheckBox* mHessDblDiffCheck;
	wxCheckBox* mHessPurifyCheck;
	wxCheckBox* mHessPrintIntFCCheck;
	wxCheckBox* mHessVibAnlCheck;
	wxTextCtrl* mHessFreqScaleEdit;
	wxCheckBox* mMolPltCheck;
	wxCheckBox* mPltOrbCheck;
	wxCheckBox* aimpacCheck;
	wxCheckBox* rpacCheck;
	wxRadioBox* mMiscProgRadio;
	wxTextCtrl* mMP2CoreEleEdit;
	wxTextCtrl* mMP2MemEdit;
	wxTextCtrl* mMP2IntCutoffEdit;
	wxCheckBox* mLMOMP2Check;
	wxCheckBox* mMP2PropCheck;
	wxRadioBox* mMP2TransRadio;
	wxRadioBox* mMP2AOStorageRadio;
	wxCheckBox* mDirectSCFCheck;
	wxCheckBox* mFDiffCheck;
	wxCheckBox* mUHFNOCheck;
	wxSpinCtrl* mSCFConvSpin;
	wxTextCtrl* mMaxStepCountEdit;
	wxTextCtrl* mGradConvEdit;
	wxUglyChoice* mOptMethodChoice;
	wxTextCtrl* mInitStepSizeEdit;
	wxTextCtrl* mMinStepSizeEdit;
	wxTextCtrl* mMaxStepSizeEdit;
	wxCheckBox* mUpdateStepSizeCheck;
	wxCheckBox* mIsStatPtCheck;
	wxTextCtrl* mStatPtJumpSizeEdit;
	wxRadioBox* mInitHessRadio;
	wxTextCtrl* mHessRecalcEdit;
	wxTextCtrl* mStatPtModeEdit;
	wxCheckBox* mPrintOrbsCheck;
	wxTextCtrl* mTitleText;
	wxTextCtrl* mBasisSetText;
	wxTextCtrl* mSCFTypeText;
	wxTextCtrl* mRunTypeText;
	wxTextCtrl* mPointGroupText;
	wxTextCtrl* mElectronCorr;
	wxButton* defaultsBtn;
	wxButton* revertBtn;
	wxButton* writeBtn;
	wxButton* advanced_button;
////@end InputBuilderWindow member variables

    InputData * TmpInputRec;
	wxBoxSizer *button_sizer;
	//local data for FMO
	std::vector<long> FMOFragmentIds;	///< Vector containing the FMO fragment mapping for each atom
    
    wxWindow *tab[NUM_PANES];
    wxString  tabText[NUM_PANES];
    bool visibleTab[NUM_PANES];
    
    void setPaneVisible(int pane, bool visible);
    int getCurrentPane();
    int getPaneAtPosition(int pos);
    
    void CheckBasisMenu(void);
};

#endif
    // _INPUTBUILDER_H_
