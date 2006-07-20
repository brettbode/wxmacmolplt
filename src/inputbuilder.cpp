/////////////////////////////////////////////////////////////////////////////
// Name:        inputbuilder.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Mon 12 Jun 2006 09:37:31 AM CDT
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "inputbuilder.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/listctrl.h>
#include <wx/listbook.h>

////@begin includes
////@end includes

#include "Globals.h"
#include "MolDisplayWin.h"
#include "AtomTypeList.h"
#include "inputbuilder.h"
#include "Frame.h"

////@begin XPM images
////@end XPM images

/*!
 * InputBuilderWindow type definition
 */

IMPLEMENT_CLASS( InputBuilderWindow, wxFrame )

/*!
 * InputBuilderWindow event table definition
 */

BEGIN_EVENT_TABLE( InputBuilderWindow, wxFrame )

////@begin InputBuilderWindow event table entries
    EVT_CLOSE( InputBuilderWindow::OnCloseWindow )

    EVT_LISTBOOK_PAGE_CHANGED( ID_IBNOTEBOOK, InputBuilderWindow::OnIblistbookPageChanged )

    EVT_CHOICE( ID_BASIS_CHOICE, InputBuilderWindow::OnBasisChoiceSelected )

    EVT_CHOICE( ID_ECPT_CHOICE, InputBuilderWindow::OnEcptChoiceSelected )

    EVT_CHOICE( ID_NUMD_CHOICE, InputBuilderWindow::OnNumdChoiceSelected )

    EVT_CHOICE( ID_NUMF_CHOICE, InputBuilderWindow::OnNumfChoiceSelected )

    EVT_CHOICE( ID_NUMP_CHOICE, InputBuilderWindow::OnNumpChoiceSelected )

    EVT_CHOICE( ID_POLAR_CHOICE, InputBuilderWindow::OnPolarChoiceSelected )

    EVT_CHECKBOX( ID_DIFFL_CHECKBOX, InputBuilderWindow::OnDifflCheckboxClick )

    EVT_CHECKBOX( ID_DIFFS_CHECKBOX, InputBuilderWindow::OnDiffsCheckboxClick )

    EVT_CHOICE( ID_RUN_CHOICE, InputBuilderWindow::OnRunChoiceSelected )

    EVT_CHOICE( ID_SCF_CHOICE, InputBuilderWindow::OnScfChoiceSelected )

    EVT_CHOICE( ID_LOCAL_CHOICE, InputBuilderWindow::OnLocalChoiceSelected )

    EVT_TEXT( ID_MCHARGE_TEXTCTRL, InputBuilderWindow::OnMchargeTextctrlUpdated )

    EVT_TEXT( ID_MULT_TEXTCTRL, InputBuilderWindow::OnMultTextctrlUpdated )

    EVT_CHOICE( ID_EXE_CHOICE, InputBuilderWindow::OnExeChoiceSelected )

    EVT_TEXT( ID_MAXIT_TEXTCTRL, InputBuilderWindow::OnMaxitTextctrlUpdated )

    EVT_CHECKBOX( ID_MP2_CHECKBOX, InputBuilderWindow::OnMp2CheckboxClick )

    EVT_CHECKBOX( ID_DFT_CHECKBOX, InputBuilderWindow::OnDftCheckboxClick )

    EVT_CHOICE( ID_CI_CHOICE, InputBuilderWindow::OnCiChoiceSelected )

    EVT_CHOICE( ID_CC_CHOICE, InputBuilderWindow::OnCcChoiceSelected )

    EVT_TEXT( ID_TITLE_TEXTCTRL, InputBuilderWindow::OnTitleTextctrlUpdated )

    EVT_CHOICE( ID_COORD_CHOICE, InputBuilderWindow::OnCoordChoiceSelected )

    EVT_CHOICE( ID_UNIT_CHOICE, InputBuilderWindow::OnUnitChoiceSelected )

    EVT_TEXT( ID_ZMAT_VARS_TEXT, InputBuilderWindow::OnZmatVarsTextUpdated )

    EVT_CHOICE( ID_POINTGROUP_CHOICE, InputBuilderWindow::OnPointgroupChoiceSelected )

    EVT_CHOICE( ID_ORDER_CHOICE, InputBuilderWindow::OnOrderChoiceSelected )

    EVT_CHECKBOX( ID_SYMMETRY_CHECKBOX, InputBuilderWindow::OnSymmetryCheckboxClick )

    EVT_TEXT( ID_TIMELIMIT_TEXT, InputBuilderWindow::OnTimelimitTextUpdated )

    EVT_CHOICE( ID_TIMELIMITUNITS_CHOICE, InputBuilderWindow::OnTimelimitunitsChoiceSelected )

    EVT_TEXT( ID_MEMORY_TEXT, InputBuilderWindow::OnMemoryTextUpdated )

    EVT_CHOICE( ID_MEMORYUNITS_CHOICE, InputBuilderWindow::OnMemoryunitsChoiceSelected )

    EVT_CHOICE( ID_DIAGONALIZATION_CHOICE, InputBuilderWindow::OnDiagonalizationChoiceSelected )

    EVT_CHECKBOX( ID_CORE_CHECKBOX, InputBuilderWindow::OnCoreCheckboxClick )

    EVT_RADIOBOX( ID_PLBALANCE_RADIOBOX, InputBuilderWindow::OnPlbalanceRadioboxSelected )

    EVT_CHECKBOX( ID_EXTERNDATAREP_CHECKBOX, InputBuilderWindow::OnExterndatarepCheckboxClick )

    EVT_CHOICE( ID_DFTMETHOD_CHOICE, InputBuilderWindow::OnDftmethodChoiceSelected )

    EVT_CHOICE( ID_GRID_FUNCTIONAL_CHOICE, InputBuilderWindow::OnGridFunctionalChoiceSelected )

    EVT_CHOICE( ID_GRIDFREE_FUNCTIONAL_CHOICE, InputBuilderWindow::OnGridfreeFunctionalChoiceSelected )

    EVT_CHOICE( ID_INITGUESS_CHOICE, InputBuilderWindow::OnInitguessChoiceSelected )

    EVT_CHOICE( ID_MOGUESS_VECSOURCE_CHOICE, InputBuilderWindow::OnMoguessVecsourceChoiceSelected )

    EVT_TEXT( ID_MOGUESS_VECORBCOUNT_TEXT, InputBuilderWindow::OnMoguessVecorbcountTextUpdated )

    EVT_CHECKBOX( ID_PRINTINITGUESS_CHECKBOX, InputBuilderWindow::OnPrintinitguessCheckboxClick )

    EVT_CHECKBOX( ID_ROTORBITALS_CHECKBOX, InputBuilderWindow::OnRotorbitalsCheckboxClick )

    EVT_CHECKBOX( ID_MOLPLT_CHECK, InputBuilderWindow::OnMolpltCheckClick )

    EVT_CHECKBOX( ID_PLTORB_CHECK, InputBuilderWindow::OnPltorbCheckClick )

    EVT_CHECKBOX( ID_AIMPAC_CHECK, InputBuilderWindow::OnAimpacCheckClick )

    EVT_CHECKBOX( ID_RPAC_CHECK, InputBuilderWindow::OnRpacCheckClick )

    EVT_RADIOBOX( ID_MISC_PROG_RADIO, InputBuilderWindow::OnMiscProgRadioSelected )

    EVT_TEXT( ID_MP2_CORE_EDIT, InputBuilderWindow::OnMp2CoreEditUpdated )

    EVT_TEXT( ID_MP2MEM_EDIT, InputBuilderWindow::OnMp2memEditUpdated )

    EVT_TEXT( ID_MP2INTCUTOFF_EDIT, InputBuilderWindow::OnMp2intcutoffEditUpdated )

    EVT_CHECKBOX( ID_LMOMP2_CHECK, InputBuilderWindow::OnLmomp2CheckClick )

    EVT_CHECKBOX( ID_MP2PROP_CHECK, InputBuilderWindow::OnMp2propCheckClick )

    EVT_RADIOBOX( ID_MP2TRANS_RADIO, InputBuilderWindow::OnMp2transRadioSelected )

    EVT_RADIOBOX( ID_MP2AOSTORAGE_RADIO, InputBuilderWindow::OnMp2aostorageRadioSelected )

    EVT_CHECKBOX( ID_DIRECTSCF_CHECK, InputBuilderWindow::OnDirectscfCheckClick )

    EVT_CHECKBOX( ID_FDIFF_CHECK, InputBuilderWindow::OnFdiffCheckClick )

    EVT_CHECKBOX( ID_UHF_NO_CHECK, InputBuilderWindow::OnUhfNoCheckClick )

    EVT_SPINCTRL( ID_SCF_CONV_SPIN, InputBuilderWindow::OnScfConvSpinUpdated )

    EVT_TEXT( ID_MAXSTEPCOUNT_EDIT, InputBuilderWindow::OnMaxstepcountEditUpdated )

    EVT_TEXT( ID_GRADCONVCRITERIA_EDIT, InputBuilderWindow::OnGradconvcriteriaEditUpdated )

    EVT_CHOICE( ID_OPTMETHOD_CHOICE, InputBuilderWindow::OnOptmethodChoiceSelected )

    EVT_TEXT( ID_INITSTEPSIZE_EDIT, InputBuilderWindow::OnInitstepsizeEditUpdated )

    EVT_TEXT( ID_MINSTEPSIZE_EDIT, InputBuilderWindow::OnMinstepsizeEditUpdated )

    EVT_TEXT( ID_MAXSTEPSIZE_CHECK, InputBuilderWindow::OnMaxstepsizeEditUpdated )

    EVT_CHECKBOX( ID_UPDATESTEPSIZE_CHECK, InputBuilderWindow::OnUpdatestepsizeCheckClick )

    EVT_CHECKBOX( ID_STATPT_CHECK, InputBuilderWindow::OnStatptCheckClick )

    EVT_TEXT( ID_STATPT_JUMP_EDIT, InputBuilderWindow::OnStatptJumpEditUpdated )

    EVT_RADIOBOX( ID_INIT_HESS_RADIO, InputBuilderWindow::OnInitHessRadioSelected )

    EVT_TEXT( ID_HESS_RECALC_EDIT, InputBuilderWindow::OnHessRecalcEditUpdated )

    EVT_TEXT( ID_STATPT_MODE_EDIT, InputBuilderWindow::OnStatptModeEditUpdated )

    EVT_CHECKBOX( ID_PRINTORBS_CHECK, InputBuilderWindow::OnPrintorbsCheckClick )

    EVT_BUTTON( ID_DEFAULTSBUTTON, InputBuilderWindow::OnDefaultsbuttonClick )

    EVT_BUTTON( ID_REVERTBUTTON, InputBuilderWindow::OnRevertbuttonClick )

    EVT_BUTTON( ID_WRITEFILEBUTTON, InputBuilderWindow::OnWritefilebuttonClick )

    EVT_BUTTON( wxID_CANCEL, InputBuilderWindow::OnCancelClick )

    EVT_BUTTON( wxID_OK, InputBuilderWindow::OnOkClick )

////@end InputBuilderWindow event table entries

END_EVENT_TABLE()

/*!
 * InputBuilderWindow constructors
 */

InputBuilderWindow::InputBuilderWindow( )
{
}

InputBuilderWindow::InputBuilderWindow( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create( parent, id, caption, pos, size, style );
}

InputBuilderWindow::~InputBuilderWindow() {
    //temp
    //tabMOGuess->Destroy();
    //tabHessOpts->Destroy();
    //tabMiscPrefs->Destroy();
    //tabSCFOpts->Destroy();
    //tabStatPoint->Destroy();
    
    delete TmpInputRec;
}

/*!
 * InputBuilder creator
 */

bool InputBuilderWindow::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin InputBuilderWindow member initialisation
    listBook = NULL;
    basisChoice = NULL;
    ecpTypeLabel = NULL;
    ecpTypeChoice = NULL;
    numDChoice = NULL;
    numFChoice = NULL;
    numPChoice = NULL;
    polarLabel = NULL;
    polarChoice = NULL;
    diffuseLCheck = NULL;
    diffuseSCheck = NULL;
    runChoice = NULL;
    scfChoice = NULL;
    localChoice = NULL;
    mchargeText = NULL;
    multText = NULL;
    exeChoice = NULL;
    scfIterText = NULL;
    mp2Check = NULL;
    dftCheck = NULL;
    ciLabel = NULL;
    ciChoice = NULL;
    ccLabel = NULL;
    ccChoice = NULL;
    titleText = NULL;
    coordTypeChoice = NULL;
    unitChoice = NULL;
    zmatrixVarsText = NULL;
    pointGroupChoice = NULL;
    paxisOrderChoice = NULL;
    symmetryCheck = NULL;
    timeLimitText = NULL;
    timeLimitUnitChoice = NULL;
    memoryText = NULL;
    memoryUnitChoice = NULL;
    diagChoice = NULL;
    coreCheck = NULL;
    plBalanceRadio = NULL;
    externDataRepCheck = NULL;
    dftMethodChoice = NULL;
    funcSizer = NULL;
    dftGridFuncChoice = NULL;
    dftGridFreeFuncChoice = NULL;
    initGuessChoice = NULL;
    mMOSourceChoice = NULL;
    mVecOrbCountText = NULL;
    printInitGuessCheck = NULL;
    rotOrbitalsCheck = NULL;
    mMolPltCheck = NULL;
    mPltOrbCheck = NULL;
    aimpacCheck = NULL;
    rpacCheck = NULL;
    mMiscProgRadio = NULL;
    mMP2CoreEleEdit = NULL;
    mMP2MemEdit = NULL;
    mMP2IntCutoffEdit = NULL;
    mLMOMP2Check = NULL;
    mMP2PropCheck = NULL;
    mMP2TransRadio = NULL;
    mMP2AOStorageRadio = NULL;
    mDirectSCFCheck = NULL;
    mFDiffCheck = NULL;
    mUHFNOCheck = NULL;
    mSCFConvSpin = NULL;
    mMaxStepCountEdit = NULL;
    mGradConvEdit = NULL;
    mOptMethodChoice = NULL;
    mInitStepSizeEdit = NULL;
    mMinStepSizeEdit = NULL;
    mMaxStepSizeEdit = NULL;
    mUpdateStepSizeCheck = NULL;
    mIsStatPtCheck = NULL;
    mStatPtJumpSizeEdit = NULL;
    mInitHessRadio = NULL;
    mHessRecalcEdit = NULL;
    mStatPtModeEdit = NULL;
    mPrintOrbsCheck = NULL;
    mTitleText = NULL;
    mBasisSetText = NULL;
    mSCFTypeText = NULL;
    mRunTypeText = NULL;
    mPointGroupText = NULL;
    mElectronCorr = NULL;
    defaultsBtn = NULL;
    revertBtn = NULL;
    writeBtn = NULL;
////@end InputBuilderWindow member initialisation

	TmpInputRec = new InputData(((MolDisplayWin *)parent)->GetData()->GetInputData());

////@begin InputBuilderWindow creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end InputBuilderWindow creation

    SetupItems();
    
    return true;
}

/*!
 * Control creation for InputBuilder
 */

void InputBuilderWindow::CreateControls()
{    
////@begin InputBuilderWindow content construction
    InputBuilderWindow* itemFrame1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer2);

    wxPanel* itemPanel3 = new wxPanel( itemFrame1, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    itemBoxSizer2->Add(itemPanel3, 1, wxGROW, 0);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemPanel3->SetSizer(itemBoxSizer4);

    listBook = new wxListbook( itemPanel3, ID_IBNOTEBOOK, wxDefaultPosition, wxDefaultSize, wxLB_LEFT|wxLB_DEFAULT|wxSUNKEN_BORDER );

    wxPanel* itemPanel6 = new wxPanel( listBook, ID_IBBASISPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
    itemPanel6->SetSizer(itemBoxSizer7);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer7->Add(itemBoxSizer8, 0, wxGROW, 5);
    wxStaticText* itemStaticText9 = new wxStaticText( itemPanel6, wxID_STATIC, _("Basis Set:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    itemBoxSizer8->Add(itemStaticText9, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString basisChoiceStrings[] = {
        _("MINI"),
        _("MIDI"),
        _("STO-2G"),
        _("STO-3G"),
        _("STO-4G"),
        _("STO-5G"),
        _("STO-6G"),
        _("3-21G"),
        _("6-21G"),
        _("4-31G"),
        _("5-31G"),
        _("6-31G"),
        _("6-311G"),
        _("Double Zeta Valance"),
        _("Dunning/Hay DZ"),
        _("Binning/Curtiss DZ"),
        _("Triple Zeta Valence"),
        _("McLean/Chandler"),
        _T(""),
        _("SBKJC Valance"),
        _("Hay/Wadt Valance"),
        _T(""),
        _("MNDO"),
        _("AM1"),
        _("PM3")
    };
    basisChoice = new wxUglyChoice( itemPanel6, ID_BASIS_CHOICE, wxDefaultPosition, wxDefaultSize, 25, basisChoiceStrings, 0 );
    basisChoice->SetStringSelection(_("MINI"));
    itemBoxSizer8->Add(basisChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer8->Add(5, 5, 0, wxGROW|wxALL, 5);

    ecpTypeLabel = new wxStaticText( itemPanel6, wxID_STATIC, _("ECP Type:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    ecpTypeLabel->Enable(false);
    itemBoxSizer8->Add(ecpTypeLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxString ecpTypeChoiceStrings[] = {
        _("None"),
        _("Read"),
        _("SBKJC"),
        _("Hay-Wadt")
    };
    ecpTypeChoice = new wxUglyChoice( itemPanel6, ID_ECPT_CHOICE, wxDefaultPosition, wxDefaultSize, 4, ecpTypeChoiceStrings, 0 );
    ecpTypeChoice->SetStringSelection(_("None"));
    ecpTypeChoice->Enable(false);
    itemBoxSizer8->Add(ecpTypeChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer14 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer7->Add(itemFlexGridSizer14, 0, wxGROW, 5);
    wxStaticText* itemStaticText15 = new wxStaticText( itemPanel6, wxID_STATIC, _("# D heavy atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(itemStaticText15, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString numDChoiceStrings[] = {
        _("0"),
        _("1"),
        _("2"),
        _("3")
    };
    numDChoice = new wxUglyChoice( itemPanel6, ID_NUMD_CHOICE, wxDefaultPosition, wxDefaultSize, 4, numDChoiceStrings, 0 );
    numDChoice->SetStringSelection(_("0"));
    itemFlexGridSizer14->Add(numDChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemPanel6, wxID_STATIC, _("# F heavy atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(itemStaticText17, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString numFChoiceStrings[] = {
        _("0"),
        _("1"),
        _("2"),
        _("3")
    };
    numFChoice = new wxUglyChoice( itemPanel6, ID_NUMF_CHOICE, wxDefaultPosition, wxDefaultSize, 4, numFChoiceStrings, 0 );
    numFChoice->SetStringSelection(_("0"));
    itemFlexGridSizer14->Add(numFChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText19 = new wxStaticText( itemPanel6, wxID_STATIC, _("# light atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer14->Add(itemStaticText19, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString numPChoiceStrings[] = {
        _("0"),
        _("1"),
        _("2"),
        _("3")
    };
    numPChoice = new wxUglyChoice( itemPanel6, ID_NUMP_CHOICE, wxDefaultPosition, wxDefaultSize, 4, numPChoiceStrings, 0 );
    numPChoice->SetStringSelection(_("0"));
    itemFlexGridSizer14->Add(numPChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer7->Add(itemBoxSizer21, 0, wxGROW, 5);
    polarLabel = new wxStaticText( itemPanel6, wxID_STATIC, _("Polar:"), wxDefaultPosition, wxDefaultSize, 0 );
    polarLabel->Enable(false);
    itemBoxSizer21->Add(polarLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString polarChoiceStrings[] = {
        _("Pople"),
        _("Pople N311"),
        _("Dunning"),
        _("Huzinaga"),
        _("Hondo7")
    };
    polarChoice = new wxUglyChoice( itemPanel6, ID_POLAR_CHOICE, wxDefaultPosition, wxDefaultSize, 5, polarChoiceStrings, 0 );
    polarChoice->SetStringSelection(_("Huzinaga"));
    polarChoice->Enable(false);
    itemBoxSizer21->Add(polarChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    diffuseLCheck = new wxCheckBox( itemPanel6, ID_DIFFL_CHECKBOX, _("Diffuse L-shell on heavy atoms"), wxDefaultPosition, wxDefaultSize, 0 );
    diffuseLCheck->SetValue(false);
    itemBoxSizer7->Add(diffuseLCheck, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    diffuseSCheck = new wxCheckBox( itemPanel6, ID_DIFFS_CHECKBOX, _("Diffuse s-shell on hydrogens"), wxDefaultPosition, wxDefaultSize, 0 );
    diffuseSCheck->SetValue(false);
    itemBoxSizer7->Add(diffuseSCheck, 0, wxGROW|wxALL, 5);

    listBook->AddPage(itemPanel6, _("Basis"));

    wxPanel* itemPanel26 = new wxPanel( listBook, ID_IBCONTROLPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxVERTICAL);
    itemPanel26->SetSizer(itemBoxSizer27);

    wxStaticBox* itemStaticBoxSizer28Static = new wxStaticBox(itemPanel26, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer28 = new wxStaticBoxSizer(itemStaticBoxSizer28Static, wxVERTICAL);
    itemBoxSizer27->Add(itemStaticBoxSizer28, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer29 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer28->Add(itemFlexGridSizer29, 0, wxGROW, 5);
    wxStaticText* itemStaticText30 = new wxStaticText( itemPanel26, wxID_STATIC, _("Run Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer29->Add(itemStaticText30, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString runChoiceStrings[] = {
        _("Energy"),
        _("Gradient"),
        _("Hessian"),
        _("Optimization"),
        _("Trudge"),
        _("Saddle Point"),
        _("IRC"),
        _("Gradient Extremal"),
        _("DRC"),
        _("Energy Surface"),
        _("Properties"),
        _("Morokuma"),
        _("Radiative Transition mom."),
        _("Spin Orbit"),
        _("Finite Electric Field"),
        _("TDHF"),
        _("Global Optimization"),
        _("VSCF"),
        _("FMO Optimization"),
        _("Raman Intensities"),
        _("NMR"),
        _("Make EFP")
    };
    runChoice = new wxUglyChoice( itemPanel26, ID_RUN_CHOICE, wxDefaultPosition, wxDefaultSize, 22, runChoiceStrings, 0 );
    runChoice->SetStringSelection(_("Energy"));
    itemFlexGridSizer29->Add(runChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText32 = new wxStaticText( itemPanel26, wxID_STATIC, _("SCF Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer29->Add(itemStaticText32, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString scfChoiceStrings[] = {
        _("RHF"),
        _("UHF"),
        _("ROHF"),
        _("GVB"),
        _("MCSCF"),
        _("None (CI)")
    };
    scfChoice = new wxUglyChoice( itemPanel26, ID_SCF_CHOICE, wxDefaultPosition, wxDefaultSize, 6, scfChoiceStrings, 0 );
    scfChoice->SetStringSelection(_("RHF"));
    itemFlexGridSizer29->Add(scfChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText34 = new wxStaticText( itemPanel26, wxID_STATIC, _("Localization Method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer29->Add(itemStaticText34, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString localChoiceStrings[] = {
        _("None"),
        _("Foster-Boys"),
        _("Edmiston-Ruedenberg"),
        _("Pipek-Mezey")
    };
    localChoice = new wxUglyChoice( itemPanel26, ID_LOCAL_CHOICE, wxDefaultPosition, wxDefaultSize, 4, localChoiceStrings, 0 );
    localChoice->SetStringSelection(_("None"));
    itemFlexGridSizer29->Add(localChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer36 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer27->Add(itemBoxSizer36, 0, wxGROW, 5);
    wxBoxSizer* itemBoxSizer37 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer36->Add(itemBoxSizer37, 0, wxGROW, 5);
    wxStaticBox* itemStaticBoxSizer38Static = new wxStaticBox(itemPanel26, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer38 = new wxStaticBoxSizer(itemStaticBoxSizer38Static, wxVERTICAL);
    itemBoxSizer37->Add(itemStaticBoxSizer38, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer39 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer38->Add(itemFlexGridSizer39, 0, wxGROW, 5);
    wxStaticText* itemStaticText40 = new wxStaticText( itemPanel26, wxID_STATIC, _("Molecule Charge:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer39->Add(itemStaticText40, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    mchargeText = new wxTextCtrl( itemPanel26, ID_MCHARGE_TEXTCTRL, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mchargeText->SetToolTip(_("$CONTRL:ICHARG - Enter an integer value for the molecular charge."));
    itemFlexGridSizer39->Add(mchargeText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText42 = new wxStaticText( itemPanel26, wxID_STATIC, _("Multiplicity:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer39->Add(itemStaticText42, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    multText = new wxTextCtrl( itemPanel26, ID_MULT_TEXTCTRL, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        multText->SetToolTip(_("$CONTRL:MULT - Enter an integer value for the spin state."));
    itemFlexGridSizer39->Add(multText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer44Static = new wxStaticBox(itemPanel26, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer44 = new wxStaticBoxSizer(itemStaticBoxSizer44Static, wxVERTICAL);
    itemBoxSizer37->Add(itemStaticBoxSizer44, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer45 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer44->Add(itemFlexGridSizer45, 0, wxGROW, 5);
    wxStaticText* itemStaticText46 = new wxStaticText( itemPanel26, wxID_STATIC, _("Exe. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer45->Add(itemStaticText46, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString exeChoiceStrings[] = {
        _("Normal Run"),
        _("Check"),
        _("Debug"),
        _("Other...")
    };
    exeChoice = new wxUglyChoice( itemPanel26, ID_EXE_CHOICE, wxDefaultPosition, wxDefaultSize, 4, exeChoiceStrings, 0 );
    exeChoice->SetStringSelection(_("Normal Run"));
    itemFlexGridSizer45->Add(exeChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText48 = new wxStaticText( itemPanel26, wxID_STATIC, _("Max # SCF Iterations:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer45->Add(itemStaticText48, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    scfIterText = new wxTextCtrl( itemPanel26, ID_MAXIT_TEXTCTRL, _("30"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        scfIterText->SetToolTip(_("$CONTRL:MAXIT - Enter the maximum number of SCF iterations. If the wavefunction is not converged at this point the run will be aborted."));
    itemFlexGridSizer45->Add(scfIterText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer50Static = new wxStaticBox(itemPanel26, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer50 = new wxStaticBoxSizer(itemStaticBoxSizer50Static, wxVERTICAL);
    itemBoxSizer36->Add(itemStaticBoxSizer50, 0, wxALIGN_TOP|wxALL, 5);
    mp2Check = new wxCheckBox( itemPanel26, ID_MP2_CHECKBOX, _("Use MP2"), wxDefaultPosition, wxDefaultSize, 0 );
    mp2Check->SetValue(false);
    if (ShowToolTips())
        mp2Check->SetToolTip(_("$CONTRL:MPLEVL - Click to use 2nd order Moller-Plesset perturbation theory. Implemented for RHF, UHF, ROHF and MCSCF energies and RHF, UHF, and ROHF gradients."));
    itemStaticBoxSizer50->Add(mp2Check, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    dftCheck = new wxCheckBox( itemPanel26, ID_DFT_CHECKBOX, _("Use DFT"), wxDefaultPosition, wxDefaultSize, 0 );
    dftCheck->SetValue(false);
    itemStaticBoxSizer50->Add(dftCheck, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer53 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer50->Add(itemFlexGridSizer53, 0, wxGROW, 5);
    ciLabel = new wxStaticText( itemPanel26, wxID_STATIC, _("CI:"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        ciLabel->SetToolTip(_("$CONTRL:CITYP Choose the type of CI to perform on top of the base wavefunction or on the supplied $VEC group for SCFTYP=NONE."));
    itemFlexGridSizer53->Add(ciLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString ciChoiceStrings[] = {
        _("None"),
        _("GUGA"),
        _("Ames Lab. Determinant"),
        _("Occupation Restricted Multiple Active Space"),
        _("CI Singles"),
        _("Full Second Order CI"),
        _("General CI")
    };
    ciChoice = new wxUglyChoice( itemPanel26, ID_CI_CHOICE, wxDefaultPosition, wxDefaultSize, 7, ciChoiceStrings, 0 );
    ciChoice->SetStringSelection(_("None"));
    itemFlexGridSizer53->Add(ciChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    ccLabel = new wxStaticText( itemPanel26, wxID_STATIC, _("CC:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer53->Add(ccLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString ccChoiceStrings[] = {
        _("None"),
        _("LCCD: linearized CC"),
        _("CCD: CC with doubles"),
        _("CCSD: CC with singles and doubles"),
        _("CCSD(T)"),
        _("R-CC"),
        _("CR-CC"),
        _("EOM-CCSD"),
        _("CR-EOM")
    };
    ccChoice = new wxUglyChoice( itemPanel26, ID_CC_CHOICE, wxDefaultPosition, wxDefaultSize, 9, ccChoiceStrings, 0 );
    ccChoice->SetStringSelection(_("None"));
    itemFlexGridSizer53->Add(ccChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel26, _("Control"));

    wxPanel* itemPanel58 = new wxPanel( listBook, ID_IBDATAPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer59 = new wxBoxSizer(wxVERTICAL);
    itemPanel58->SetSizer(itemBoxSizer59);

    wxBoxSizer* itemBoxSizer60 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer59->Add(itemBoxSizer60, 0, wxALIGN_LEFT, 5);
    titleText = new wxTextCtrl( itemPanel58, ID_TITLE_TEXTCTRL, _("Title"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        titleText->SetToolTip(_("$DATA - You may enter a one line title which may help you identify this input deck in the future."));
    itemBoxSizer60->Add(titleText, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer62Static = new wxStaticBox(itemPanel58, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer62 = new wxStaticBoxSizer(itemStaticBoxSizer62Static, wxVERTICAL);
    itemBoxSizer60->Add(itemStaticBoxSizer62, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer63 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer62->Add(itemFlexGridSizer63, 0, wxGROW, 5);
    wxStaticText* itemStaticText64 = new wxStaticText( itemPanel58, wxID_STATIC, _("Coord. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer63->Add(itemStaticText64, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString coordTypeChoiceStrings[] = {
        _("Unique cartesian Coords."),
        _("Hilderbrant internals"),
        _("Cartesian coordinates"),
        _("Z-Matrix"),
        _("MOPAC Z-Matrix")
    };
    coordTypeChoice = new wxUglyChoice( itemPanel58, ID_COORD_CHOICE, wxDefaultPosition, wxDefaultSize, 5, coordTypeChoiceStrings, 0 );
    coordTypeChoice->SetStringSelection(_("Unique cartesian Coords."));
    itemFlexGridSizer63->Add(coordTypeChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText66 = new wxStaticText( itemPanel58, wxID_STATIC, _("Units:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer63->Add(itemStaticText66, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString unitChoiceStrings[] = {
        _("Angstroms"),
        _("Bohr")
    };
    unitChoice = new wxUglyChoice( itemPanel58, ID_UNIT_CHOICE, wxDefaultPosition, wxDefaultSize, 2, unitChoiceStrings, 0 );
    unitChoice->SetStringSelection(_("Angstroms"));
    itemFlexGridSizer63->Add(unitChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer62->Add(itemBoxSizer68, 0, wxGROW, 5);
    wxStaticText* itemStaticText69 = new wxStaticText( itemPanel58, wxID_STATIC, _("# of Z-Matrix Variables:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer68->Add(itemStaticText69, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    zmatrixVarsText = new wxTextCtrl( itemPanel58, ID_ZMAT_VARS_TEXT, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        zmatrixVarsText->SetToolTip(_("$CONTRL:NZVAR - Enter an integer number representing the number of internal coordinates for your molecule. Normally this will be 3N-6 (3N-5 for linear molecules) where N is the number of atoms. A value of 0 selects cartesian coordinates. If set and a set of internal coordinates are defined a $ZMAT group will be punched out."));
    itemBoxSizer68->Add(zmatrixVarsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer71Static = new wxStaticBox(itemPanel58, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer71 = new wxStaticBoxSizer(itemStaticBoxSizer71Static, wxVERTICAL);
    itemBoxSizer60->Add(itemStaticBoxSizer71, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer72 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer71->Add(itemFlexGridSizer72, 0, wxGROW, 5);
    wxStaticText* itemStaticText73 = new wxStaticText( itemPanel58, wxID_STATIC, _("Point Group:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer72->Add(itemStaticText73, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString pointGroupChoiceStrings[] = {
        _("C1"),
        _("CS"),
        _("CI"),
        _("CnH"),
        _("CnV"),
        _("Cn"),
        _("S2n"),
        _("DnD"),
        _("DnH"),
        _("Dn"),
        _("TD"),
        _("TH"),
        _("T"),
        _("OH"),
        _("O")
    };
    pointGroupChoice = new wxUglyChoice( itemPanel58, ID_POINTGROUP_CHOICE, wxDefaultPosition, wxDefaultSize, 15, pointGroupChoiceStrings, 0 );
    itemFlexGridSizer72->Add(pointGroupChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText75 = new wxStaticText( itemPanel58, wxID_STATIC, _("Order of Principle Axis:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText75->Enable(false);
    itemFlexGridSizer72->Add(itemStaticText75, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString paxisOrderChoiceStrings[] = {
        _("2"),
        _("3"),
        _("4")
    };
    paxisOrderChoice = new wxUglyChoice( itemPanel58, ID_ORDER_CHOICE, wxDefaultPosition, wxDefaultSize, 3, paxisOrderChoiceStrings, 0 );
    if (ShowToolTips())
        paxisOrderChoice->SetToolTip(_("Replaces the 'n' above."));
    paxisOrderChoice->Enable(false);
    itemFlexGridSizer72->Add(paxisOrderChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    symmetryCheck = new wxCheckBox( itemPanel58, ID_SYMMETRY_CHECKBOX, _("Use Symmetry During Calculation"), wxDefaultPosition, wxDefaultSize, 0 );
    symmetryCheck->SetValue(true);
    if (ShowToolTips())
        symmetryCheck->SetToolTip(_("$CONTRL:NOSYM - When checked symmetry will be used as much as possible in the caluclation of integrals, gradients, etc. (This is the normal setting)"));
    itemStaticBoxSizer71->Add(symmetryCheck, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel58, _("Data"));

    wxPanel* itemPanel78 = new wxPanel( listBook, ID_IBSYSTEMPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer79 = new wxBoxSizer(wxVERTICAL);
    itemPanel78->SetSizer(itemBoxSizer79);

    wxFlexGridSizer* itemFlexGridSizer80 = new wxFlexGridSizer(0, 3, 0, 0);
    itemBoxSizer79->Add(itemFlexGridSizer80, 0, wxGROW, 5);
    wxStaticText* itemStaticText81 = new wxStaticText( itemPanel78, wxID_STATIC, _("Time Limit:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer80->Add(itemStaticText81, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    timeLimitText = new wxTextCtrl( itemPanel78, ID_TIMELIMIT_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        timeLimitText->SetToolTip(_("$SYSTEM:TIMLIM - Enter a value for the time limit. When the time limit is reached GAMESS will stop the run. The number entered here will have the units given at the right."));
    itemFlexGridSizer80->Add(timeLimitText, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxString timeLimitUnitChoiceStrings[] = {
        _("Seconds"),
        _("Minutes"),
        _("Hours"),
        _("Days"),
        _("Weeks"),
        _("Years"),
        _("Millenia")
    };
    timeLimitUnitChoice = new wxUglyChoice( itemPanel78, ID_TIMELIMITUNITS_CHOICE, wxDefaultPosition, wxDefaultSize, 7, timeLimitUnitChoiceStrings, 0 );
    timeLimitUnitChoice->SetStringSelection(_("Minutes"));
    itemFlexGridSizer80->Add(timeLimitUnitChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText84 = new wxStaticText( itemPanel78, wxID_STATIC, _("Memory:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer80->Add(itemStaticText84, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    memoryText = new wxTextCtrl( itemPanel78, ID_MEMORY_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        memoryText->SetToolTip(_("$SYSTEM:MEMORY - Enter the amount of memory (in the units at the right) that GAMESS will request for its dynamic memory pool. You should not normally request more memory than the RAM size."));
    itemFlexGridSizer80->Add(memoryText, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxString memoryUnitChoiceStrings[] = {
        _("Words"),
        _("Bytes"),
        _("MegaWords"),
        _("MegaBytes")
    };
    memoryUnitChoice = new wxUglyChoice( itemPanel78, ID_MEMORYUNITS_CHOICE, wxDefaultPosition, wxDefaultSize, 4, memoryUnitChoiceStrings, 0 );
    memoryUnitChoice->SetStringSelection(_("Words"));
    itemFlexGridSizer80->Add(memoryUnitChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer87 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer79->Add(itemBoxSizer87, 0, wxGROW, 5);
    wxStaticText* itemStaticText88 = new wxStaticText( itemPanel78, wxID_STATIC, _("Diagonalization Method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer87->Add(itemStaticText88, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString diagChoiceStrings[] = {
        _("default"),
        _("EVVRSP"),
        _("GIVEIS"),
        _("JACOBI")
    };
    diagChoice = new wxUglyChoice( itemPanel78, ID_DIAGONALIZATION_CHOICE, wxDefaultPosition, wxDefaultSize, 4, diagChoiceStrings, 0 );
    diagChoice->SetStringSelection(_("default"));
    itemBoxSizer87->Add(diagChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    coreCheck = new wxCheckBox( itemPanel78, ID_CORE_CHECKBOX, _("Produce \"core\" file upon abort"), wxDefaultPosition, wxDefaultSize, 0 );
    coreCheck->SetValue(false);
    itemBoxSizer79->Add(coreCheck, 0, wxALIGN_LEFT|wxALL, 5);

    wxString plBalanceRadioStrings[] = {
        _("Loop"),
        _("Next Value")
    };
    plBalanceRadio = new wxRadioBox( itemPanel78, ID_PLBALANCE_RADIOBOX, _("Parallel Load Balance Type"), wxDefaultPosition, wxDefaultSize, 2, plBalanceRadioStrings, 1, wxRA_SPECIFY_COLS );
    plBalanceRadio->SetSelection(0);
    if (ShowToolTips())
        plBalanceRadio->SetToolTip(_("$SYSTEM:BALTYP - Select the type of load balancing to use. Only applicable to runs on parallel computers."));
    itemBoxSizer79->Add(plBalanceRadio, 0, wxALIGN_LEFT|wxALL, 5);

    externDataRepCheck = new wxCheckBox( itemPanel78, ID_EXTERNDATAREP_CHECKBOX, _("Use External Data Representation for messages"), wxDefaultPosition, wxDefaultSize, 0 );
    externDataRepCheck->SetValue(false);
    if (ShowToolTips())
        externDataRepCheck->SetToolTip(_("$SYSTEM:XDR= Click to cause messages to be converted into a generic data format to pass between nodes from different vendors."));
    itemBoxSizer79->Add(externDataRepCheck, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel78, _("System"));

    wxPanel* itemPanel93 = new wxPanel( listBook, ID_IBDFTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer94 = new wxBoxSizer(wxVERTICAL);
    itemPanel93->SetSizer(itemBoxSizer94);

    wxFlexGridSizer* itemFlexGridSizer95 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer94->Add(itemFlexGridSizer95, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText96 = new wxStaticText( itemPanel93, wxID_STATIC, _("Method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer95->Add(itemStaticText96, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString dftMethodChoiceStrings[] = {
        _("Grid"),
        _("Grid-Free")
    };
    dftMethodChoice = new wxUglyChoice( itemPanel93, ID_DFTMETHOD_CHOICE, wxDefaultPosition, wxDefaultSize, 2, dftMethodChoiceStrings, 0 );
    dftMethodChoice->SetStringSelection(_("Grid"));
    itemFlexGridSizer95->Add(dftMethodChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText98 = new wxStaticText( itemPanel93, wxID_STATIC, _("DFT Functional:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer95->Add(itemStaticText98, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    funcSizer = new wxBoxSizer(wxVERTICAL);
    itemFlexGridSizer95->Add(funcSizer, 1, wxGROW|wxGROW, 5);
    wxString dftGridFuncChoiceStrings[] = {
        _("Slater exchange"),
        _("Becke 1988 exchange"),
        _("VWN: Vosko-Wilk-Nusair (VWN5) correlation"),
        _("LYP: Lee-Yang-Parr correlation"),
        _("SVWN: Slater exhange + VWN correlation"),
        _("BVWN: BECKE exchange + VWN5 correlation"),
        _("BLYP: BECKE exchange + LYP correlation"),
        _("B3LYP"),
        _("Gill 1996 exchange"),
        _("Perdew-Burke-Ernzerhof (PBE) exchange"),
        _("OP: One-parameter Progressive correlation"),
        _("SLYP: SLATER + Lee-Yang-Parr (LYP) correlation"),
        _("SOP: SLATER + OP correlation"),
        _("BOP: BECKE exchange + OP correlation"),
        _("BVWN: GILL exchange + VWN5 correlation"),
        _("GLYP: GILL exchange + LYP correlation"),
        _("GOP: GILL exchange + OP correlation"),
        _("PBEVWN: PBE exchange + VWN6 correlation"),
        _("PBELYP: PBE exchange +LYP correlation"),
        _("PBEOP: PBE exchange + OP correlation"),
        _("BHHLYP: HF and BECKE exchange + LYP correlation")
    };
    dftGridFuncChoice = new wxUglyChoice( itemPanel93, ID_GRID_FUNCTIONAL_CHOICE, wxDefaultPosition, wxDefaultSize, 21, dftGridFuncChoiceStrings, 0 );
    dftGridFuncChoice->SetStringSelection(_("Slater exchange"));
    funcSizer->Add(dftGridFuncChoice, 0, wxALIGN_LEFT|wxALL, 5);

    wxString dftGridFreeFuncChoiceStrings[] = {
        _("Slater exchange"),
        _("Becke's 1988 exchange"),
        _("VWN: Vosko/Wilke/Nusair corr"),
        _("LYP: Lee/Yang/Parr corr\nSVWN: Slater exchange + VWN corr"),
        _("BVWN: Becke exchange + VWN corr"),
        _("BLYP: Becke exchange + LYP corr"),
        _("B3LYP: hybridized HF/Becke/LYP using VWN"),
        _("X-Alpha exchange"),
        _("Depristo/Kress exchange"),
        _("CAMA: Handy et al's mods to Becke exchange"),
        _("HALF: 50-50 mix of Becke and HF exchange"),
        _("PWLOC: Perdew/Wang local corr"),
        _("BPWLOC: Becke exchange "),
        _(" Perdew/Wang corr"),
        _("CAMB: CAMA exchange + Cambridge corr"),
        _("XVWN: Xalpha exchange + VWN corr"),
        _("XPWLOC: Xalpha exchange + Perdew/Wang corr"),
        _("SPWLOC: Slater exchange + PWLOC corr"),
        _("WIGNER: Wigner exchange + corr"),
        _("WS: Wigner scaled exchange + corr"),
        _("WIGEXP: Wigner exponential exchange + corr")
    };
    dftGridFreeFuncChoice = new wxUglyChoice( itemPanel93, ID_GRIDFREE_FUNCTIONAL_CHOICE, wxDefaultPosition, wxDefaultSize, 21, dftGridFreeFuncChoiceStrings, 0 );
    dftGridFreeFuncChoice->SetStringSelection(_("Slater exchange"));
    funcSizer->Add(dftGridFreeFuncChoice, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    listBook->AddPage(itemPanel93, _("DFT"));

    wxPanel* itemPanel102 = new wxPanel( listBook, ID_IBMOGUESSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer103 = new wxBoxSizer(wxVERTICAL);
    itemPanel102->SetSizer(itemBoxSizer103);

    wxBoxSizer* itemBoxSizer104 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer103->Add(itemBoxSizer104, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText105 = new wxStaticText( itemPanel102, wxID_STATIC, _("Initial Guess:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer104->Add(itemStaticText105, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString initGuessChoiceStrings[] = {
        _("Huckel"),
        _("HCore"),
        _("MO Read ($VEC)"),
        _("MO Saved (DICTNRY)"),
        _("Skip")
    };
    initGuessChoice = new wxUglyChoice( itemPanel102, ID_INITGUESS_CHOICE, wxDefaultPosition, wxSize(300, -1), 5, initGuessChoiceStrings, 0 );
    initGuessChoice->SetStringSelection(_("Huckel"));
    itemBoxSizer104->Add(initGuessChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer107 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer103->Add(itemBoxSizer107, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText108 = new wxStaticText( itemPanel102, wxID_STATIC, _("$VEC source:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer107->Add(itemStaticText108, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString* mMOSourceChoiceStrings = NULL;
    mMOSourceChoice = new wxUglyChoice( itemPanel102, ID_MOGUESS_VECSOURCE_CHOICE, wxDefaultPosition, wxSize(300, -1), 0, mMOSourceChoiceStrings, 0 );
    if (ShowToolTips())
        mMOSourceChoice->SetToolTip(_("test tip"));
    itemBoxSizer107->Add(mMOSourceChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer110 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer103->Add(itemBoxSizer110, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText111 = new wxStaticText( itemPanel102, wxID_STATIC, _("Number of Orbitals to read in:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer110->Add(itemStaticText111, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mVecOrbCountText = new wxTextCtrl( itemPanel102, ID_MOGUESS_VECORBCOUNT_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mVecOrbCountText->SetToolTip(_("$GUESS:NORB - Enter an integer number of orbitals to read in from the $VEC group. The default is to read in the number of occupied orbitals for RHF, UHF, ROHF, and GVB, but you must provide a value for CI and MCSCF runs."));
    itemBoxSizer110->Add(mVecOrbCountText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    printInitGuessCheck = new wxCheckBox( itemPanel102, ID_PRINTINITGUESS_CHECKBOX, _("Print the initial guess"), wxDefaultPosition, wxDefaultSize, 0 );
    printInitGuessCheck->SetValue(false);
    if (ShowToolTips())
        printInitGuessCheck->SetToolTip(_("\"$GUESS:PRTMO - When checked GAMESS will write out the initial set of MO vectors to the log file."));
    itemBoxSizer103->Add(printInitGuessCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    rotOrbitalsCheck = new wxCheckBox( itemPanel102, ID_ROTORBITALS_CHECKBOX, _("Rotate alpha and beta orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    rotOrbitalsCheck->SetValue(false);
    if (ShowToolTips())
        rotOrbitalsCheck->SetToolTip(_("$GUESS:MIX - Click to rotate the alpha and beta HOMO and LUMO orbitals such that the alpha and beta orbital spaces are in-equivalent. Only applicable to UHF runs."));
    rotOrbitalsCheck->Enable(false);
    itemBoxSizer103->Add(rotOrbitalsCheck, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel102, _("MO Guess"));

    wxPanel* itemPanel115 = new wxPanel( listBook, ID_IBHESSOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer116 = new wxBoxSizer(wxVERTICAL);
    itemPanel115->SetSizer(itemBoxSizer116);

    wxBoxSizer* itemBoxSizer117 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer116->Add(itemBoxSizer117, 0, wxALIGN_LEFT, 5);
    wxString itemRadioBox118Strings[] = {
        _("Analytic"),
        _("Numeric")
    };
    wxRadioBox* itemRadioBox118 = new wxRadioBox( itemPanel115, ID_RADIOBOX, _("Method"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox118Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox118->SetSelection(0);
    itemBoxSizer117->Add(itemRadioBox118, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer119 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer117->Add(itemBoxSizer119, 0, wxALIGN_TOP|wxALL, 5);
    wxStaticText* itemStaticText120 = new wxStaticText( itemPanel115, wxID_STATIC, _("Displacement size (bohrs):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer119->Add(itemStaticText120, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl121 = new wxTextCtrl( itemPanel115, ID_TEXTCTRL15, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer119->Add(itemTextCtrl121, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox122 = new wxCheckBox( itemPanel115, ID_CHECKBOX20, _("Double differenced hessian"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox122->SetValue(false);
    itemBoxSizer116->Add(itemCheckBox122, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox123 = new wxCheckBox( itemPanel115, ID_CHECKBOX21, _("Purify hessian"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox123->SetValue(false);
    itemBoxSizer116->Add(itemCheckBox123, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox124 = new wxCheckBox( itemPanel115, ID_CHECKBOX22, _("Print internal force constants"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox124->SetValue(false);
    itemBoxSizer116->Add(itemCheckBox124, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox125 = new wxCheckBox( itemPanel115, ID_CHECKBOX23, _("Vibrational Analysis"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox125->SetValue(false);
    itemBoxSizer116->Add(itemCheckBox125, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer126 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer116->Add(itemBoxSizer126, 0, wxGROW, 5);
    wxStaticText* itemStaticText127 = new wxStaticText( itemPanel115, wxID_STATIC, _("Frequency scale factor:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer126->Add(itemStaticText127, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl128 = new wxTextCtrl( itemPanel115, ID_TEXTCTRL14, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer126->Add(itemTextCtrl128, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel115, _("Hess. Options"));

    wxPanel* itemPanel129 = new wxPanel( listBook, ID_IBMISCPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer130 = new wxBoxSizer(wxVERTICAL);
    itemPanel129->SetSizer(itemBoxSizer130);

    wxStaticText* itemStaticText131 = new wxStaticText( itemPanel129, wxID_STATIC, _("GAMESS interface to other codes."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer130->Add(itemStaticText131, 0, wxALIGN_LEFT|wxALL|wxADJUST_MINSIZE, 5);

    wxStaticBox* itemStaticBoxSizer132Static = new wxStaticBox(itemPanel129, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer132 = new wxStaticBoxSizer(itemStaticBoxSizer132Static, wxVERTICAL);
    itemBoxSizer130->Add(itemStaticBoxSizer132, 0, wxALIGN_LEFT|wxALL, 5);
    mMolPltCheck = new wxCheckBox( itemPanel129, ID_MOLPLT_CHECK, _("MolPlt"), wxDefaultPosition, wxDefaultSize, 0 );
    mMolPltCheck->SetValue(false);
    if (ShowToolTips())
        mMolPltCheck->SetToolTip(_("$CONTRL:MOLPLT=.FALSE. - Click to punch an input deck (to the .dat file) for the MolPlt X-windows program. Note MacMolPlt can also read these files."));
    itemStaticBoxSizer132->Add(mMolPltCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    mPltOrbCheck = new wxCheckBox( itemPanel129, ID_PLTORB_CHECK, _("PltOrb"), wxDefaultPosition, wxDefaultSize, 0 );
    mPltOrbCheck->SetValue(false);
    itemStaticBoxSizer132->Add(mPltOrbCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    aimpacCheck = new wxCheckBox( itemPanel129, ID_AIMPAC_CHECK, _("Atoms In Molecules (AIMPAC)"), wxDefaultPosition, wxDefaultSize, 0 );
    aimpacCheck->SetValue(false);
    itemStaticBoxSizer132->Add(aimpacCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    rpacCheck = new wxCheckBox( itemPanel129, ID_RPAC_CHECK, _("RPAC"), wxDefaultPosition, wxDefaultSize, 0 );
    rpacCheck->SetValue(false);
    itemStaticBoxSizer132->Add(rpacCheck, 0, wxALIGN_LEFT|wxALL, 5);

    wxString mMiscProgRadioStrings[] = {
        _("None"),
        _("Hondo 8.2"),
        _("MELDF"),
        _("GAMESS (UK version)"),
        _("Gaussian 9x"),
        _("All")
    };
    mMiscProgRadio = new wxRadioBox( itemPanel129, ID_MISC_PROG_RADIO, _("These Force a Check Run Type"), wxDefaultPosition, wxDefaultSize, 6, mMiscProgRadioStrings, 1, wxRA_SPECIFY_COLS );
    mMiscProgRadio->SetSelection(0);
    if (ShowToolTips())
        mMiscProgRadio->SetToolTip(_("$CONTRL:FRIEND - Click to punch input decks for the programs listed in this box."));
    itemBoxSizer130->Add(mMiscProgRadio, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel129, _("Misc. Prefs"));

    wxPanel* itemPanel138 = new wxPanel( listBook, ID_IBMP2PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer139 = new wxBoxSizer(wxVERTICAL);
    itemPanel138->SetSizer(itemBoxSizer139);

    wxFlexGridSizer* itemFlexGridSizer140 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer139->Add(itemFlexGridSizer140, 0, wxGROW, 5);
    wxStaticText* itemStaticText141 = new wxStaticText( itemPanel138, wxID_STATIC, _("# of core electrons:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer140->Add(itemStaticText141, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    mMP2CoreEleEdit = new wxTextCtrl( itemPanel138, ID_MP2_CORE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mMP2CoreEleEdit->SetToolTip(_("$MP2:NACORE - Enter an integer value for the number of electrons to leave out of the MP2 calculation."));
    itemFlexGridSizer140->Add(mMP2CoreEleEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText143 = new wxStaticText( itemPanel138, wxID_STATIC, _("Memory (in words):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer140->Add(itemStaticText143, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    mMP2MemEdit = new wxTextCtrl( itemPanel138, ID_MP2MEM_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mMP2MemEdit->SetToolTip(_("$MP2:NWORD - Enter an integer number for the number of words of memory to use in the MP2 calculation. A value of 0 uses all available memory."));
    itemFlexGridSizer140->Add(mMP2MemEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText145 = new wxStaticText( itemPanel138, wxID_STATIC, _("Integral retention cutoff:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer140->Add(itemStaticText145, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    mMP2IntCutoffEdit = new wxTextCtrl( itemPanel138, ID_MP2INTCUTOFF_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    mMP2IntCutoffEdit->SetHelpText(_("$MP2:CUTOFF - Enter a floating point value for the integral retention cutoff. (default is 1.0e-9)"));
    if (ShowToolTips())
        mMP2IntCutoffEdit->SetToolTip(_("$MP2:CUTOFF - Enter a floating point value for the integral retention cutoff. (default is 1.0e-9)"));
    itemFlexGridSizer140->Add(mMP2IntCutoffEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mLMOMP2Check = new wxCheckBox( itemPanel138, ID_LMOMP2_CHECK, _("Use Localized Orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    mLMOMP2Check->SetValue(false);
    itemBoxSizer139->Add(mLMOMP2Check, 0, wxGROW|wxALL, 5);

    mMP2PropCheck = new wxCheckBox( itemPanel138, ID_MP2PROP_CHECK, _("Compute MP2 properties"), wxDefaultPosition, wxDefaultSize, 0 );
    mMP2PropCheck->SetValue(false);
    if (ShowToolTips())
        mMP2PropCheck->SetToolTip(_("$MP2:MP2PRP - a flag to turn on property computation for MP2 energy jobs. This is much more expensive than a plain energy. Properties are always computed during gradient runs where they are almost free."));
    itemBoxSizer139->Add(mMP2PropCheck, 0, wxALIGN_LEFT|wxALL, 5);

    wxString mMP2TransRadioStrings[] = {
        _("Segmented transformation"),
        _("Two phase bin sort")
    };
    mMP2TransRadio = new wxRadioBox( itemPanel138, ID_MP2TRANS_RADIO, _("Transformation method"), wxDefaultPosition, wxDefaultSize, 2, mMP2TransRadioStrings, 1, wxRA_SPECIFY_COLS );
    mMP2TransRadio->SetSelection(0);
    if (ShowToolTips())
        mMP2TransRadio->SetToolTip(_("$MP2:Method - The Two phase bin sort is a more conventional method requiring more disk space, but less memory. It is required when using localized orbitals."));
    itemBoxSizer139->Add(mMP2TransRadio, 0, wxALIGN_LEFT|wxALL, 5);

    wxString mMP2AOStorageRadioStrings[] = {
        _("Duplicated on each node"),
        _("Distributed across all nodes")
    };
    mMP2AOStorageRadio = new wxRadioBox( itemPanel138, ID_MP2AOSTORAGE_RADIO, _("AO integral storage"), wxDefaultPosition, wxDefaultSize, 2, mMP2AOStorageRadioStrings, 1, wxRA_SPECIFY_COLS );
    mMP2AOStorageRadio->SetSelection(0);
    if (ShowToolTips())
        mMP2AOStorageRadio->SetToolTip(_("$MP2:AOINTS -Choose the 2nd to distribute AO integrals over all nodes. This requires less disk space, but should only be used on parallel computers with high speed communications."));
    itemBoxSizer139->Add(mMP2AOStorageRadio, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel138, _("MP2 Options"));

    wxPanel* itemPanel151 = new wxPanel( listBook, ID_IBSCBOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer152 = new wxBoxSizer(wxVERTICAL);
    itemPanel151->SetSizer(itemBoxSizer152);

    mDirectSCFCheck = new wxCheckBox( itemPanel151, ID_DIRECTSCF_CHECK, _("Direct SCF"), wxDefaultPosition, wxDefaultSize, 0 );
    mDirectSCFCheck->SetValue(false);
    if (ShowToolTips())
        mDirectSCFCheck->SetToolTip(_("$SCF:DIRSCF - Direct SCF will be used to calculate the AO integrals. This will also activate direct MP2 if MP2 is requested."));
    itemBoxSizer152->Add(mDirectSCFCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    mFDiffCheck = new wxCheckBox( itemPanel151, ID_FDIFF_CHECK, _("Compute only change in Fock Matrix"), wxDefaultPosition, wxDefaultSize, 0 );
    mFDiffCheck->SetValue(false);
    if (ShowToolTips())
        mFDiffCheck->SetToolTip(_("$SCF:FDIFF - When checked only the change since the previous iteration in the Fock matrices will be computed. This saves a large amount of CPU time, but may not fully converge cases with a lot of diffuse functions."));
    mFDiffCheck->Enable(false);
    itemBoxSizer152->Add(mFDiffCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    mUHFNOCheck = new wxCheckBox( itemPanel151, ID_UHF_NO_CHECK, _("Generate UHF Natural Orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    mUHFNOCheck->SetValue(false);
    if (ShowToolTips())
        mUHFNOCheck->SetToolTip(_("$SCF:UHFNOS - Click to generate the natural orbitals of the UHF function. This option only applies to UHF wavefunctions."));
    mUHFNOCheck->Enable(false);
    itemBoxSizer152->Add(mUHFNOCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer156 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer152->Add(itemBoxSizer156, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText157 = new wxStaticText( itemPanel151, wxID_STATIC, _("SCF convergence criteria:  10^"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer156->Add(itemStaticText157, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    mSCFConvSpin = new wxSpinCtrl( itemPanel151, ID_SCF_CONV_SPIN, _T("5"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 2147483647, 5 );
    if (ShowToolTips())
        mSCFConvSpin->SetToolTip(_("$SCF:NCONV - Enter an integer value to use as the power (in 10**(-n)) for the SCF density convergance criteria. Setting this to less than 5 (the default) will give questionable gradients."));
    itemBoxSizer156->Add(mSCFConvSpin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel151, _("SCF Options"));

    wxPanel* itemPanel159 = new wxPanel( listBook, ID_IBSTATPOINTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer160 = new wxBoxSizer(wxVERTICAL);
    itemPanel159->SetSizer(itemBoxSizer160);

    wxFlexGridSizer* itemFlexGridSizer161 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer160->Add(itemFlexGridSizer161, 0, wxGROW, 5);
    wxStaticText* itemStaticText162 = new wxStaticText( itemPanel159, wxID_STATIC, _("Max. number of steps:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer161->Add(itemStaticText162, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    mMaxStepCountEdit = new wxTextCtrl( itemPanel159, ID_MAXSTEPCOUNT_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mMaxStepCountEdit->SetToolTip(_("$STATPT:NSTEP=An integer representing the maximum number of steps to take before cleanly stopping the run (after punching out all needed restart data). Most such runs can be restarted."));
    itemFlexGridSizer161->Add(mMaxStepCountEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText164 = new wxStaticText( itemPanel159, wxID_STATIC, _("Gradient convergance criteria:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer161->Add(itemStaticText164, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    mGradConvEdit = new wxTextCtrl( itemPanel159, ID_GRADCONVCRITERIA_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mGradConvEdit->SetToolTip(_("$STATPT:OPTTOL=a floating point value representing the convergance criteria for the gradient (in Hartree/Bohr)."));
    itemFlexGridSizer161->Add(mGradConvEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer166 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer160->Add(itemBoxSizer166, 0, wxGROW, 5);
    wxStaticText* itemStaticText167 = new wxStaticText( itemPanel159, wxID_STATIC, _("Optimization method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer166->Add(itemStaticText167, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString mOptMethodChoiceStrings[] = {
        _("Newton-Raphson"),
        _("Rational Function Optimization"),
        _("Quadratic Approximation"),
        _("Schlegel (quasi-NR)"),
        _("Constrained Optimization")
    };
    mOptMethodChoice = new wxUglyChoice( itemPanel159, ID_OPTMETHOD_CHOICE, wxDefaultPosition, wxDefaultSize, 5, mOptMethodChoiceStrings, 0 );
    if (ShowToolTips())
        mOptMethodChoice->SetToolTip(_("$STATPT:METHOD The algorithm used to drive the gradient to zero."));
    itemBoxSizer166->Add(mOptMethodChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer169 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer160->Add(itemBoxSizer169, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer170 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer169->Add(itemFlexGridSizer170, 0, wxGROW, 5);
    wxStaticText* itemStaticText171 = new wxStaticText( itemPanel159, wxID_STATIC, _("Initial step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer170->Add(itemStaticText171, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    mInitStepSizeEdit = new wxTextCtrl( itemPanel159, ID_INITSTEPSIZE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mInitStepSizeEdit->SetToolTip(_("$STATPT:DXMAX = a floating point value representing the initial size of the step"));
    itemFlexGridSizer170->Add(mInitStepSizeEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText173 = new wxStaticText( itemPanel159, wxID_STATIC, _("Min. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer170->Add(itemStaticText173, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    mMinStepSizeEdit = new wxTextCtrl( itemPanel159, ID_MINSTEPSIZE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mMinStepSizeEdit->SetToolTip(_("$STATPT:TRMIN= the minimum size of the step."));
    itemFlexGridSizer170->Add(mMinStepSizeEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText175 = new wxStaticText( itemPanel159, wxID_STATIC, _("Max. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer170->Add(itemStaticText175, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    mMaxStepSizeEdit = new wxTextCtrl( itemPanel159, ID_MAXSTEPSIZE_CHECK, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mMaxStepSizeEdit->SetToolTip(_("$STATPT:TRMAX= the maximum size of the step."));
    itemFlexGridSizer170->Add(mMaxStepSizeEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer177 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer169->Add(itemBoxSizer177, 0, wxGROW, 5);
    mUpdateStepSizeCheck = new wxCheckBox( itemPanel159, ID_UPDATESTEPSIZE_CHECK, _("Update Step Size"), wxDefaultPosition, wxDefaultSize, 0 );
    mUpdateStepSizeCheck->SetValue(false);
    if (ShowToolTips())
        mUpdateStepSizeCheck->SetToolTip(_("$STATPT:TRUPD The step size will be allowed to change within the range given at the left."));
    itemBoxSizer177->Add(mUpdateStepSizeCheck, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    mIsStatPtCheck = new wxCheckBox( itemPanel159, ID_STATPT_CHECK, _("Stationary Pt."), wxDefaultPosition, wxDefaultSize, 0 );
    mIsStatPtCheck->SetValue(false);
    if (ShowToolTips())
        mIsStatPtCheck->SetToolTip(_("$STATPT:STPT If checked the initial geometry will be considered a stationary point and the jump away from the initial geometry will be given by the step size below."));
    itemBoxSizer177->Add(mIsStatPtCheck, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer180 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer177->Add(itemBoxSizer180, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
    wxStaticText* itemStaticText181 = new wxStaticText( itemPanel159, wxID_STATIC, _("Jump Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer180->Add(itemStaticText181, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mStatPtJumpSizeEdit = new wxTextCtrl( itemPanel159, ID_STATPT_JUMP_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mStatPtJumpSizeEdit->SetToolTip(_("$STATPT:STSTEP= the size of the step taken away from stationary points.\n"));
    itemBoxSizer180->Add(mStatPtJumpSizeEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer183 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer160->Add(itemBoxSizer183, 0, wxGROW, 5);
    wxString mInitHessRadioStrings[] = {
        _("Guess (+ definite)"),
        _("Read (from $HESS)"),
        _("Calculate")
    };
    mInitHessRadio = new wxRadioBox( itemPanel159, ID_INIT_HESS_RADIO, _("Initial Hessian"), wxDefaultPosition, wxDefaultSize, 3, mInitHessRadioStrings, 1, wxRA_SPECIFY_COLS );
    mInitHessRadio->SetSelection(0);
    if (ShowToolTips())
        mInitHessRadio->SetToolTip(_("$STATPT:HESS=The source of the initial hessian matrix."));
    itemBoxSizer183->Add(mInitHessRadio, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer185 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer183->Add(itemBoxSizer185, 0, wxALIGN_TOP|wxALL, 0);
    wxBoxSizer* itemBoxSizer186 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer185->Add(itemBoxSizer186, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
    wxStaticText* itemStaticText187 = new wxStaticText( itemPanel159, wxID_STATIC, _("Recalc. Hess every"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer186->Add(itemStaticText187, 0, wxALIGN_TOP|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    mHessRecalcEdit = new wxTextCtrl( itemPanel159, ID_HESS_RECALC_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mHessRecalcEdit->SetToolTip(_("$STATPT:IHREP=an integer representing how often the hessian will be fully recalculated. A value of zero is normal and means never fully recalculate the hessian. The hessian will be updated at every step."));
    itemBoxSizer186->Add(mHessRecalcEdit, 0, wxALIGN_TOP|wxALL, 5);

    wxStaticText* itemStaticText189 = new wxStaticText( itemPanel159, wxID_STATIC, _("steps."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer186->Add(itemStaticText189, 0, wxALIGN_TOP|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    wxBoxSizer* itemBoxSizer190 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer185->Add(itemBoxSizer190, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
    wxStaticText* itemStaticText191 = new wxStaticText( itemPanel159, wxID_STATIC, _("Follow which mode?"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer190->Add(itemStaticText191, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mStatPtModeEdit = new wxTextCtrl( itemPanel159, ID_STATPT_MODE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mStatPtModeEdit->SetToolTip(_("$STATPT:IFOLOW Only applicable to SADPOINT runs and indicates which mode to maximize."));
    itemBoxSizer190->Add(mStatPtModeEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    mPrintOrbsCheck = new wxCheckBox( itemPanel159, ID_PRINTORBS_CHECK, _("Print Orbs at each iteration"), wxDefaultPosition, wxDefaultSize, 0 );
    mPrintOrbsCheck->SetValue(false);
    if (ShowToolTips())
        mPrintOrbsCheck->SetToolTip(_("If checked the optimized orbitals will be output in the log file for each geometry. Otherwise the default is to output them for the 1st and last geometry."));
    itemBoxSizer160->Add(mPrintOrbsCheck, 0, wxGROW|wxALL, 5);

    listBook->AddPage(itemPanel159, _("Stat. Point"));

    wxPanel* itemPanel194 = new wxPanel( listBook, ID_SUMMARYPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer195 = new wxBoxSizer(wxVERTICAL);
    itemPanel194->SetSizer(itemBoxSizer195);

    wxBoxSizer* itemBoxSizer196 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer195->Add(itemBoxSizer196, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText197 = new wxStaticText( itemPanel194, wxID_STATIC, _("Title:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer196->Add(itemStaticText197, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mTitleText = new wxTextCtrl( itemPanel194, ID_SUMMARY_TITLE, _T(""), wxDefaultPosition, wxSize(400, -1), 0 );
    mTitleText->Enable(false);
    itemBoxSizer196->Add(mTitleText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer199 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer195->Add(itemBoxSizer199, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText200 = new wxStaticText( itemPanel194, wxID_STATIC, _("Basis Set:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer199->Add(itemStaticText200, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mBasisSetText = new wxTextCtrl( itemPanel194, ID_SUMMARY_BASISSET, _T(""), wxDefaultPosition, wxSize(400, -1), 0 );
    mBasisSetText->Enable(false);
    itemBoxSizer199->Add(mBasisSetText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer202 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer195->Add(itemBoxSizer202, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText203 = new wxStaticText( itemPanel194, wxID_STATIC, _("SCF Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer202->Add(itemStaticText203, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mSCFTypeText = new wxTextCtrl( itemPanel194, ID_SUMMARY_SCFTYPE, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    mSCFTypeText->Enable(false);
    itemBoxSizer202->Add(mSCFTypeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer205 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer195->Add(itemBoxSizer205, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText206 = new wxStaticText( itemPanel194, wxID_STATIC, _("Run Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer205->Add(itemStaticText206, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mRunTypeText = new wxTextCtrl( itemPanel194, ID_SUMMARY_RUNTYPE, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    mRunTypeText->Enable(false);
    itemBoxSizer205->Add(mRunTypeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer208 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer195->Add(itemBoxSizer208, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText209 = new wxStaticText( itemPanel194, wxID_STATIC, _("Molecular Point Group:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer208->Add(itemStaticText209, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mPointGroupText = new wxTextCtrl( itemPanel194, ID_SUMMARY_PG, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    mPointGroupText->Enable(false);
    itemBoxSizer208->Add(mPointGroupText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer211 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer195->Add(itemBoxSizer211, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText212 = new wxStaticText( itemPanel194, wxID_STATIC, _("Electron Correlation"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer211->Add(itemStaticText212, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mElectronCorr = new wxTextCtrl( itemPanel194, ID_SUMMARY_ELEC, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    mElectronCorr->Enable(false);
    itemBoxSizer211->Add(mElectronCorr, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel194, _("Summary"));

    itemBoxSizer4->Add(listBook, 1, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer214 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer4->Add(itemBoxSizer214, 0, wxGROW, 5);

    defaultsBtn = new wxButton( itemPanel3, ID_DEFAULTSBUTTON, _("Use Defaults"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer214->Add(defaultsBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    revertBtn = new wxButton( itemPanel3, ID_REVERTBUTTON, _("Revert"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer214->Add(revertBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    writeBtn = new wxButton( itemPanel3, ID_WRITEFILEBUTTON, _("Write File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer214->Add(writeBtn, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer218 = new wxStdDialogButtonSizer;

    itemBoxSizer214->Add(itemStdDialogButtonSizer218, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* itemButton219 = new wxButton( itemPanel3, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer218->AddButton(itemButton219);

    wxButton* itemButton220 = new wxButton( itemPanel3, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer218->AddButton(itemButton220);

    itemStdDialogButtonSizer218->Realize();

////@end InputBuilderWindow content construction

    tabText[0] = wxT("Basis");
    tabText[1] = wxT("Control");
    tabText[2] = wxT("Data");
    tabText[3] = wxT("System");
    tabText[4] = wxT("DFT");
    tabText[5] = wxT("MO Guess");
    tabText[6] = wxT("Hess. Options");
    tabText[7] = wxT("Misc. Prefs");
    tabText[8] = wxT("MP2 Options");
    tabText[9] = wxT("SCF Options");
    tabText[10] = wxT("Stat. Point");
    tabText[11] = wxT("Summary");

    for(int i = NUM_PANES - 1; i >= 0; i--) {
        tab[i] = listBook->GetPage(i);
        listBook->RemovePage(i);
        visibleTab[i] = false;
    }
    
    //wxListView * t = listBook->GetListView();
	//t->SetWindowStyle(wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_NO_HEADER);
	//t->SetWindowStyle(wxLC_LIST|wxLC_SINGLE_SEL);

    setPaneVisible(BASIS_PANE,   true);
    setPaneVisible(CONTROL_PANE, true);
    setPaneVisible(DATA_PANE,    true);
	setPaneVisible(MOGUESS_PANE, true);
    setPaneVisible(SYSTEM_PANE,  true);
    //if(TmpInputRec->Control->UseDFT()) setPaneVisible(DFT_PANE, true);
	if(TmpInputRec->Control->GetSCFType() <= 4) setPaneVisible(SCFOPTS_PANE, true);
    setPaneVisible(MISCPREFS_PANE, true);
    setPaneVisible(SUMMARY_PANE, true);

    listBook->SetSelection(1);
}

/*!
 * Should we show tooltips?
 */

bool InputBuilderWindow::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap InputBuilderWindow::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin InputBuilderWindow bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end InputBuilderWindow bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon InputBuilderWindow::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin InputBuilderWindow icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end InputBuilderWindow icon retrieval
}
/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_INPUTBUILDERWINDOW
 */

void InputBuilderWindow::OnCloseWindow( wxCloseEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    parent->CloseInputBuilderWindow();
}


void InputBuilderWindow::SetupItems() {
    SetupBasisItems();
    SetupControlItems();
    SetupDataItems();
    SetupSystemItems();
    SetupDFTItems();
    SetupMOGuessItems();
    SetupHessOptsItems();
    SetupMiscPrefsItems();
    SetupMP2OptsItems();
    SetupSCFOptsItems();
    SetupStatPointItems();
    SetupSummaryItems();

	setPaneVisible(STATPOINT_PANE, ((TmpInputRec->Control->GetRunType() == 4)||(TmpInputRec->Control->GetRunType() == 6)));
	setPaneVisible(MP2OPTS_PANE, (TmpInputRec->Control->GetMPLevel() == 2));
}

void InputBuilderWindow::SetupBasisItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    short BasisValue = TmpInputRec->Basis->GetBasis();
    short NumGauss = TmpInputRec->Basis->GetNumGauss();
    int itemValue = 0;
    int testValue = 0;
    
    // basisChoice
    itemValue = BasisValue;
    if(itemValue == 0) itemValue = 1;
    else if(itemValue == 3) itemValue = NumGauss + 1;
    else if(itemValue == 4) {
        itemValue += 4;
        if(NumGauss == 6) itemValue++;
    }
    else if(itemValue == 5) itemValue = NumGauss + 6;
    else if(itemValue > 5) itemValue += 7;
    basisChoice->SetSelection(itemValue - 1);
    CheckBasisMenu();
    
    // ecpTypeChoice
    if(BasisValue == 12 || BasisValue == 13) {
        ecpTypeChoice->Enable(true);
        ecpTypeLabel->Enable(true);
        itemValue = TmpInputRec->Basis->GetECPPotential();
        if(itemValue == 0) {
            if(BasisValue == 12) itemValue = 2;
            else itemValue = 3;
        }
        ecpTypeChoice->SetSelection(itemValue);
    }
    else {
        ecpTypeChoice->Enable(false);
        ecpTypeLabel->Enable(false);
    }
    
    // polarChoice
    testValue = TmpInputRec->Basis->GetNumPFuncs() +
                TmpInputRec->Basis->GetNumDFuncs() +
                TmpInputRec->Basis->GetNumFFuncs();
    if(testValue) {
        polarChoice->Enable(true);
        polarLabel->Enable(true);
        itemValue = TmpInputRec->Basis->GetPolar();
        if(itemValue == 0) {
            if(BasisValue == 6 || BasisValue == 11) itemValue = 2;
            else if(BasisValue == 7 || BasisValue == 8) itemValue = 3;
            else if(BasisValue < 3) itemValue = 4;
            else if(BasisValue == 10) itemValue = 5;
            else itemValue = 1;
        }
        polarChoice->SetSelection(itemValue - 1);
    }
    else {
        polarChoice->Enable(false);
        polarLabel->Enable(false);
    }
    
    // diffuseLCheck
    diffuseLCheck->SetValue(TmpInputRec->Basis->GetDiffuseSP());
    
    // diffuseSCheck
    diffuseSCheck->SetValue(TmpInputRec->Basis->GetDiffuseS());
    
    // numDChoice
    numDChoice->SetSelection(TmpInputRec->Basis->GetNumDFuncs());
    
    // numFChoice
    numFChoice->SetSelection(TmpInputRec->Basis->GetNumFFuncs());
    
    // numPChoice
    numPChoice->SetSelection(TmpInputRec->Basis->GetNumPFuncs());
}

void InputBuilderWindow::SetupControlItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
        
    short mp2 = TmpInputRec->Control->GetMPLevel();
    bool dft = TmpInputRec->Control->UseDFT();
    short ci = TmpInputRec->Control->GetCIType();
    CCRunType cc = TmpInputRec->Control->GetCCType();
    long scft = TmpInputRec->Control->GetSCFType();
    long NumElectrons = parent->GetData()->GetNumElectrons();
    
    int itemValue = 0;
    
    itemValue = TmpInputRec->Control->GetRunType();
    if(itemValue == 0) itemValue = 1;
    runChoice->SetSelection(itemValue - 1);
    
    if(scft == 0) {
        if(NumElectrons & 1) {
            scft = 3;
        }
        else {
            scft = 1;
        }
    }
    scfChoice->SetSelection(scft - 1);

    // mp2Check
    if(ci || cc || dft || (mp2 < 0)) {
        mp2Check->SetValue(false);
        mp2Check->Enable(false);
    }
    else {
        mp2Check->Enable(true);
        if(mp2 < 0) mp2 = 0;
        if(mp2 == 2) mp2 = 1;
        mp2Check->SetValue(mp2);
    }
    
    // dftCheck
    if(ci || cc || (mp2 > 0) || (scft > 3)) {
        dftCheck->SetValue(false);
        dftCheck->Enable(false);
    }
    else {
        dftCheck->Enable(true);
        dftCheck->SetValue(dft);
        if(dft) setPaneVisible(DFT_PANE, true);
    }
    
    // ciChoice
    if((mp2 > 0) || dft || cc || scft == 2) {
        ciChoice->SetSelection(0);
        ciChoice->Enable(false);
        ciLabel->Enable(false);
    }
    else {
        ciChoice->Enable(true);
        ciLabel->Enable(true);
        ciChoice->SetSelection(ci);
    }
    
    // ccChoice
    if((mp2 > 0) || dft || ci || scft > 1) {
        ccChoice->SetSelection(0);
        ccChoice->Enable(false);
        ccLabel->Enable(false);
    }
    else {
        ccChoice->Enable(true);
        ccLabel->Enable(true);
        ccChoice->SetSelection(cc);
    }
    
    // scfIterText
    itemValue = TmpInputRec->Control->GetMaxIt();
    if(itemValue <= 0) itemValue = 30;
    scfIterText->SetValue(wxString::Format(wxT("%d"), itemValue));
    
    // exeChoice
    exeChoice->SetSelection(TmpInputRec->Control->GetExeType());
    if(TmpInputRec->Control->GetFriend() != Friend_None) {
		//The friend keyword choices force a check run type
        exeChoice->Enable(false);
		exeChoice->SetSelection(1);
    }
    else {
        exeChoice->Enable(true);
    }
    
    // mchargeText
    mchargeText->SetValue(wxString::Format(wxT("%hd"), TmpInputRec->Control->GetCharge()));
    
    // multText
    itemValue = TmpInputRec->Control->GetMultiplicity();
    if(itemValue <= 0) {
        if(NumElectrons & 1) itemValue == 2;
        else itemValue = 1;
    }
    multText->SetValue(wxString::Format(wxT("%d"), itemValue));
    
    // localChoice
    localChoice->SetSelection(TmpInputRec->Control->GetLocal() - 1);
}

void InputBuilderWindow::SetupDataItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    int itemValue;
	
	//Title
	if (TmpInputRec->Data->GetTitle())
		titleText->SetValue(wxString(TmpInputRec->Data->GetTitle(), wxConvUTF8));
	else
		titleText->SetValue(wxString(_T("Title")));
    
    // coordTypeChoice
    itemValue = TmpInputRec->Data->GetCoordType();
    if(itemValue == 0) itemValue = 1;
    coordTypeChoice->SetSelection(itemValue - 1);
    
    // unitChoice
    unitChoice->SetSelection(TmpInputRec->Data->GetUnits());
	
	//# Z-Matrix vars
	wxString zvars;
	zvars.Printf(wxT("%d"), TmpInputRec->Data->GetNumZVar());
	zmatrixVarsText->SetValue(zvars);
    
		//Point Group
	itemValue = TmpInputRec->Data->GetPointGroup();
	if (itemValue == 0) itemValue = 1;
	pointGroupChoice->SetSelection(itemValue-1);

		//Point group order
	SetupPointGroupOrder();
    
    // symmetryCheck
    symmetryCheck->SetValue(TmpInputRec->Data->GetUseSym());
}

void InputBuilderWindow::SetupPointGroupOrder(void) {
	//Point group order - only applicable to certain point groups
	int itemValue = TmpInputRec->Data->GetPointGroup();
	if (itemValue == 0) itemValue = 1;
	if ((itemValue>3)&&(itemValue<11)) {
		paxisOrderChoice->Enable(true);
		itemValue = TmpInputRec->Data->GetPointGroupOrder()-1;
		if (itemValue <= 0) {
			itemValue = 1;
			TmpInputRec->Data->SetPointGroupOrder(2);
		}
		paxisOrderChoice->SetSelection(itemValue-1);
	} else paxisOrderChoice->Enable(false);
}

void InputBuilderWindow::SetupSystemItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    
    // timeLimitText
    wxString time;
	time.Printf(wxT("%.2f"), TmpInputRec->System->GetConvertedTime());
	timeLimitText->SetValue(time);
	
    // timeLimitUnitChoice
	timeLimitUnitChoice->SetSelection(TmpInputRec->System->GetTimeUnits() - 1);
	
    // memoryText
	wxString mem;
	mem.Printf(wxT("%.2f"), TmpInputRec->System->GetConvertedMem());
	memoryText->SetValue(mem);
	
    // memoryUnitChoice
	memoryUnitChoice->SetSelection(TmpInputRec->System->GetMemUnits() - 1);
	
    // diagChoice
	diagChoice->SetSelection(TmpInputRec->System->GetDiag());
	
    // coreCheck
	coreCheck->SetValue(TmpInputRec->System->GetCoreFlag());
	
    // plBalanceRadio
	plBalanceRadio->SetSelection(TmpInputRec->System->GetBalanceType());
	
    // externDataRepCheck
	externDataRepCheck->SetValue(TmpInputRec->System->GetXDR());
}

void InputBuilderWindow::SetupDFTItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();

    if(TmpInputRec->DFT == NULL) {
        TmpInputRec->DFT = new DFTGroup;
    }
    
    // dftMethodChoice
    bool grid = TmpInputRec->DFT->MethodGrid();
    dftMethodChoice->SetSelection(grid ? 0 : 1);
    
    // dftGridFuncChoice and dftGridFreeFuncChoice
    if(grid) {
        dftGridFreeFuncChoice->Show(false);
        dftGridFuncChoice->Show(true);
        dftGridFuncChoice->SetSelection(TmpInputRec->DFT->GetFunctional() - 1);
        funcSizer->Layout();
    }
    else {
        dftGridFuncChoice->Show(false);
        dftGridFreeFuncChoice->Show(true);
        dftGridFreeFuncChoice->SetSelection(TmpInputRec->DFT->GetFunctional() - 1);
        funcSizer->Layout();
    }
}

void InputBuilderWindow::SetupMOGuessItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();

	long GuessType = TmpInputRec->Guess->GetGuess();
	MoleculeData * MainData = parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();
	if (GuessType == 0) {
		GuessType = 1;
		if (Orbs->size() > 0) {GuessType = 3; TmpInputRec->Guess->SetGuess(GuessType);}
	}
	initGuessChoice->SetSelection(GuessType-1);
	
	if (GuessType == 3) { //only applicable to MO read type of guess
		mMOSourceChoice->Enable(true);
		mMOSourceChoice->Clear();
		mVecOrbCountText->Enable(true);
		mMOSourceChoice->Append(wxString(_("By Hand Later")));
		short tempVec = TmpInputRec->Guess->GetVecSource();
		if (Orbs->size() > 0) {
			if ((tempVec<=0)||(tempVec>Orbs->size() + 2)) tempVec = 2;
			std::vector<OrbitalRec *>::const_iterator OrbSet = Orbs->begin();
			while (OrbSet != Orbs->end()) {	//Build the popup menu
				switch ((*OrbSet)->getOrbitalType()) {
					case OptimizedOrbital:
						if ((*OrbSet)->getOrbitalWavefunctionType() == MCSCF)
							mMOSourceChoice->Append(wxString(_("MCSCF Optimized Orbitals")));
						else
							mMOSourceChoice->Append(wxString(_("Molecular EigenVectors")));
						break;
					case NaturalOrbital:
						switch ((*OrbSet)->getOrbitalWavefunctionType()) {
							case UHF:
								mMOSourceChoice->Append(wxString(_("UHF Natural Orbitals")));
								break;
							case GVB:
								mMOSourceChoice->Append(wxString(_("GVB GI Orbitals")));
								break;
							case MCSCF:
								mMOSourceChoice->Append(wxString(_("MCSCF Natural Orbitals")));
								break;
							case CI:
								mMOSourceChoice->Append(wxString(_("CI Natural Orbitals")));
								break;
							case RHFMP2:
								mMOSourceChoice->Append(wxString(_("RMP2 Natural Orbitals")));
								break;
							default:
								mMOSourceChoice->Append(wxString(_("Natural Orbitals")));
								break;
						}
						break;
					case LocalizedOrbital:
						mMOSourceChoice->Append(wxString(_("Localized Orbitals")));
						break;
					case OrientedLocalizedOrbital:
						mMOSourceChoice->Append(wxString(_("Oriented Localized Orbitals")));
						break;
					default:
						mMOSourceChoice->Append(wxString(_("Molecular Orbitals")));
				}
				OrbSet++;
			}
		} else {	//No orbs so the only choice is by hand later
			tempVec = 1;
		}
		mMOSourceChoice->SetSelection(tempVec-1);

		int numOrbs = TmpInputRec->Guess->GetNumOrbs();
		if (!numOrbs) {	//Obtain the # of orbs if from a local set
			if (Orbs->size() > 0) {
				if (tempVec > 1) {
					OrbitalRec * OrbSet = (*Orbs)[tempVec-2];
					numOrbs = OrbSet->getNumOccupiedAlphaOrbitals();
					if (numOrbs <= 0) numOrbs = OrbSet->getNumAlphaOrbitals();
				}
			}
		}
		wxString noText;
		noText.Printf(wxT("%d"), numOrbs);
		mVecOrbCountText->SetValue(noText);
	} else {
		mMOSourceChoice->Enable(false);
		mVecOrbCountText->Enable(false);
	}
	
	printInitGuessCheck->SetValue(TmpInputRec->Guess->GetPrintMO());
	
	rotOrbitalsCheck->SetValue(TmpInputRec->Guess->GetMix());
	rotOrbitalsCheck->Enable(((TmpInputRec->Control->GetSCFType() == 2)&&
							  ((TmpInputRec->Control->GetMultiplicity() == 1)||
							   (TmpInputRec->Control->GetMultiplicity() == 0))));
}

void InputBuilderWindow::SetupHessOptsItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
}

void InputBuilderWindow::SetupMiscPrefsItems() {
	mMolPltCheck->SetValue(TmpInputRec->Control->GetMolPlot());
	mPltOrbCheck->SetValue(TmpInputRec->Control->GetPlotOrb());
	
	//aimpac and rpac require normal run type, but the others below require a check run
	if ((TmpInputRec->Control->GetExeType()!=1)&&(TmpInputRec->Control->GetFriend()==Friend_None)) {
		aimpacCheck->Enable(true);
		aimpacCheck->SetValue(TmpInputRec->Control->GetAIMPAC());
		
		rpacCheck->Enable(true);
		rpacCheck->SetValue(TmpInputRec->Control->GetRPAC());
	} else {
		aimpacCheck->Enable(false);
		aimpacCheck->SetValue(false);
		
		rpacCheck->Enable(false);
		rpacCheck->SetValue(false);
	}
	
	mMiscProgRadio->SetSelection(TmpInputRec->Control->GetFriend());
}

void InputBuilderWindow::SetupMP2OptsItems() {
	if (!TmpInputRec->MP2) TmpInputRec->MP2 = new MP2Group;
	
	wxString temp;
	//# core electrons
	temp.Printf(wxT("%ld"), TmpInputRec->MP2->GetNumCoreElectrons());
	mMP2CoreEleEdit->SetValue(temp);
	//memory
	long memVal = TmpInputRec->MP2->GetMemory();
	if (memVal == 0)
		temp.Printf(wxT("All"));
	else
		temp.Printf(wxT("%d"), memVal);
	mMP2MemEdit->SetValue(temp);
	//integral cutoff
	double	tempf = TmpInputRec->MP2->GetIntCutoff();
	if (tempf == 0.0) tempf = 1.0e-9;
	temp.Printf(wxT("%g"), tempf);
	mMP2IntCutoffEdit->SetValue(temp);
	//LMOMP2
	mLMOMP2Check->Enable((1>=TmpInputRec->Control->GetSCFType()));
	mLMOMP2Check->SetValue(TmpInputRec->MP2->GetLMOMP2());
	//MP2 Prop
	mMP2PropCheck->Enable((Energy==TmpInputRec->Control->GetRunType()));
	mMP2PropCheck->SetValue(TmpInputRec->MP2->GetMP2Prop());
	//Trans. method
	if (TmpInputRec->MP2->GetLMOMP2()) {	//LMOMP2 forces method = 3
		mMP2TransRadio->Enable(false);
		mMP2TransRadio->SetSelection(1);
	} else {
		mMP2TransRadio->Enable(true);
		short itemValue = TmpInputRec->MP2->GetMethod();
		if (itemValue <= 2) itemValue = 0;
		else itemValue = 1;
		mMP2TransRadio->SetSelection(itemValue);
	}
	//AO dist.
	short itemValue = TmpInputRec->MP2->GetAOIntMethod();
	if ((itemValue == 0)||(itemValue == 2)) itemValue = 1;
	else itemValue = 0;
	mMP2AOStorageRadio->SetSelection(itemValue);
}

void InputBuilderWindow::SetupSCFOptsItems() {
	if (!TmpInputRec->SCF) TmpInputRec->SCF = new SCFGroup;
	mDirectSCFCheck->SetValue(TmpInputRec->SCF->GetDirectSCF());
	if ((TmpInputRec->SCF->GetDirectSCF() && (3>=TmpInputRec->Control->GetSCFType()))) {
		mFDiffCheck->SetValue(TmpInputRec->SCF->GetFockDiff());
		mFDiffCheck->Enable(true);
	} else {
		mFDiffCheck->SetValue(false);
		mFDiffCheck->Enable(false);
	}
	if (TmpInputRec->Control->GetSCFType() == GAMESS_UHF) {
		mUHFNOCheck->SetValue(TmpInputRec->SCF->GetUHFNO());
		mUHFNOCheck->Enable(true);
	} else {
		mUHFNOCheck->SetValue(false);
		mUHFNOCheck->Enable(false);
	}

	int conv = TmpInputRec->SCF->GetConvergance();
	if (conv == 0) conv = 5;
	mSCFConvSpin->SetValue(conv);
}

void InputBuilderWindow::SetupStatPointItems() {
	short RunType = TmpInputRec->Control->GetRunType();

	wxString temp;
	//max steps
	temp.Printf(wxT("%d"), TmpInputRec->StatPt->GetMaxSteps());
	mMaxStepCountEdit->SetValue(temp);
	
	//conv. criteria
	temp.Printf(wxT("%g"), TmpInputRec->StatPt->GetOptConvergance());
	mGradConvEdit->SetValue(temp);
	
	//method
	short OptMethod = TmpInputRec->StatPt->GetMethod();
	mOptMethodChoice->SetSelection(OptMethod-1);
	
	//init step size
	if (OptMethod == 1) {
		mInitStepSizeEdit->Enable(false);
	} else {
		mInitStepSizeEdit->Enable(true);
		float tempf = TmpInputRec->StatPt->GetInitRadius();
		if (tempf == 0.0) {
			if (OptMethod == 5) tempf = 0.1;
			else if (RunType == 6) tempf = 0.2;
			else tempf = 0.3;
		}
		temp.Printf(wxT("%g"), tempf);
		mInitStepSizeEdit->SetValue(temp);
	}
	//Min step size
	temp.Printf(wxT("%g"), TmpInputRec->StatPt->GetMinRadius());
	mMinStepSizeEdit->SetValue(temp);
	//max step size
	float tempf = TmpInputRec->StatPt->GetMaxRadius();
	if (tempf == 0.0) {
		if (RunType == 6) tempf = 0.3;
		else tempf = 0.5;
	}
	temp.Printf(wxT("%g"), tempf);
	mMaxStepSizeEdit->SetValue(temp);
	
	//hess recalc interval
	temp.Printf(wxT("%d"), TmpInputRec->StatPt->GetHessRecalcInterval());
	mHessRecalcEdit->SetValue(temp);
	
	//Init Hess
	short HessType = TmpInputRec->StatPt->GetHessMethod();
	if (!HessType) {
		if (RunType == 4) HessType = 1;
		else HessType = 2;
	}
	mInitHessRadio->SetSelection(HessType-1);
	//step size update radius
	mUpdateStepSizeCheck->SetValue(TmpInputRec->StatPt->GetRadiusUpdate());
	//Statpt check
	mIsStatPtCheck->SetValue(TmpInputRec->StatPt->GetStatPoint());
	//Orbs print check
	mPrintOrbsCheck->SetValue(TmpInputRec->StatPt->AlwaysPrintOrbs());

	//statpt jump off size
	if (TmpInputRec->StatPt->GetStatPoint()) {
		mStatPtJumpSizeEdit->Enable(true);
		temp.Printf(wxT("%g"), TmpInputRec->StatPt->GetStatJump());
		mStatPtJumpSizeEdit->SetValue(temp);
	} else {
		mStatPtJumpSizeEdit->Enable(false);
	}
	//mode to follow
	if (RunType == 6) {
		mStatPtModeEdit->Enable(true);
		temp.Printf(wxT("%ld"), TmpInputRec->StatPt->GetModeFollow());
		mStatPtModeEdit->SetValue(temp);
	} else
		mStatPtModeEdit->Enable(false);
}

void InputBuilderWindow::SetupSummaryItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	
	if (TmpInputRec->Data->GetTitle()) {
		mTitleText->SetValue(wxString(TmpInputRec->Data->GetTitle(), wxConvUTF8));
	} else mTitleText->Clear();
	if (TmpInputRec->Basis->GetBasis()) {
		wxString temp(TmpInputRec->Basis->GetBasisText(), wxConvUTF8);
		if ((TmpInputRec->Basis->GetBasis()>3)&&(TmpInputRec->Basis->GetBasis()<6)) {
			temp.Printf(wxT("%d-%s"), TmpInputRec->Basis->GetNumGauss(), TmpInputRec->Basis->GetBasisText());
		} else if (TmpInputRec->Basis->GetBasis()==3) {
			temp.Printf(wxT("%s-%dG"), TmpInputRec->Basis->GetBasisText(), TmpInputRec->Basis->GetNumGauss());
		}
		if (TmpInputRec->Basis->GetNumDFuncs()) {
			wxString t;
			t.Printf(wxT(", # D funcs = %d"), TmpInputRec->Basis->GetNumDFuncs());
			temp.Append(t);
		}
		if (TmpInputRec->Basis->GetNumPFuncs()) {
			wxString t;
			t.Printf(wxT(", # P funcs = %d"), TmpInputRec->Basis->GetNumPFuncs());
			temp.Append(t);
		}
		if (TmpInputRec->Basis->GetNumFFuncs()) {
			wxString t;
			t.Printf(wxT(", # F funcs = %d"), TmpInputRec->Basis->GetNumFFuncs());
			temp.Append(t);
		}
		if (TmpInputRec->Basis->GetPolar() > GAMESS_BS_No_Polarization) {
			wxString t;
			t.Printf(wxT(", Polar = %s"), TmpInputRec->Basis->GetPolarText());
			temp.Append(t);
		}
		mBasisSetText->SetValue(temp);
	} else mBasisSetText->Clear();
	
    wxString temp(ControlGroup::GetGAMESSRunText(TmpInputRec->Control->GetRunType()), wxConvUTF8);
	mRunTypeText->SetValue(temp);

	if (TmpInputRec->Control->GetSCFType()) {
		mSCFTypeText->SetValue(wxString(TmpInputRec->Control->GetSCFTypeText(), wxConvUTF8));
	} else mSCFTypeText->Clear();
	{
		wxString pg;
		const char * pgt = TmpInputRec->Data->GetPointGroupText();
		if (TmpInputRec->Data->GetPointGroupOrder() > 0) {
			int i=0;
			while (pgt[i]) {
				if (pgt[i] == 'N') {
					wxString temp;
					temp.Printf(wxT("%d"), TmpInputRec->Data->GetPointGroupOrder());
					pg.Append(temp);
				} else {
					pg.Append((char)(pgt[i]), 1);
				}
				i++;
			}
		} else pg = wxString(pgt, wxConvUTF8);
		mPointGroupText->SetValue(pg);
	}
	wxString eclevel;
	if (TmpInputRec->Control->GetMPLevel() == 2)
		eclevel.Printf(wxT("MP2"));
	else if (TmpInputRec->Control->GetCCType())
		eclevel.Printf((wxChar*)TmpInputRec->Control->GetGAMESSCCType(TmpInputRec->Control->GetCCType()));
	else if (TmpInputRec->Control->GetCIType())
		eclevel.Printf((wxChar*)TmpInputRec->Control->GetCIType(TmpInputRec->Control->GetCIType()));
	mElectronCorr->SetValue(eclevel);
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_BASIS_CHOICE
 */

void InputBuilderWindow::OnBasisChoiceSelected( wxCommandEvent& event )
{
    short BasisValue = 0;
    short NumGauss   = 0;
    int   itemValue  = basisChoice->GetSelection();
    
    if(itemValue < 2) {
        BasisValue = itemValue + 1;
    }
    else if(itemValue < 7) {
        BasisValue = 3;
        NumGauss = itemValue;
    }
    else if(itemValue < 9) {
        BasisValue = 4;
        NumGauss = ((itemValue==7) ? 3 : 6);
    }
    else if(itemValue < 12) {
        BasisValue = 5;
        NumGauss = itemValue - 5;
    }
    else if(itemValue < 18) {
        BasisValue = itemValue - 6;
        if(itemValue == 12) NumGauss = 6;
    }
    else {
        BasisValue = itemValue - 6;
    }
    
    TmpInputRec->Basis->SetBasis(BasisValue);
    TmpInputRec->Basis->SetNumGauss(NumGauss);
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ECPT_CHOICE
 */

void InputBuilderWindow::OnEcptChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetECPPotential(ecpTypeChoice->GetSelection());
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_POLAR_CHOICE
 */

void InputBuilderWindow::OnPolarChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetPolar((GAMESS_BS_Polarization)(polarChoice->GetSelection() + 1));
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DIFFL_CHECKBOX
 */

void InputBuilderWindow::OnDifflCheckboxClick( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetDiffuseSP(event.IsChecked());
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DIFFS_CHECKBOX
 */

void InputBuilderWindow::OnDiffsCheckboxClick( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetDiffuseS(event.IsChecked());
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_NUMD_CHOICE
 */

void InputBuilderWindow::OnNumdChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetNumDFuncs(numDChoice->GetSelection());
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_NUMF_CHOICE
 */

void InputBuilderWindow::OnNumfChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetNumFFuncs(numFChoice->GetSelection());
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_NUMP_CHOICE
 */

void InputBuilderWindow::OnNumpChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetNumPFuncs(numPChoice->GetSelection());
    SetupItems();
}
/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_RUN_CHOICE
 */

void InputBuilderWindow::OnRunChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetRunType((TypeOfRun)(runChoice->GetSelection() + 1));
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_SCF_CHOICE
 */

void InputBuilderWindow::OnScfChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetSCFType((GAMESS_SCFType)(scfChoice->GetSelection() + 1));
	setPaneVisible(SCFOPTS_PANE, (TmpInputRec->Control->GetSCFType() <= 4));
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_LOCAL_CHOICE
 */

void InputBuilderWindow::OnLocalChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetLocal((GAMESS_Localization)(localChoice->GetSelection() + 1));
    SetupItems();
}
/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_MP2_CHECKBOX
 */

void InputBuilderWindow::OnMp2CheckboxClick( wxCommandEvent& event )
{
    if(event.IsChecked()) {
        TmpInputRec->Control->SetMPLevel(2);
    }
    else {
        TmpInputRec->Control->SetMPLevel(0);
    }
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DFT_CHECKBOX
 */

void InputBuilderWindow::OnDftCheckboxClick( wxCommandEvent& event )
{
    TmpInputRec->Control->UseDFT(event.IsChecked());
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CI_CHOICE
 */

void InputBuilderWindow::OnCiChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetCIType((CIRunType)(ciChoice->GetSelection()));
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CC_CHOICE
 */

void InputBuilderWindow::OnCcChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetCCType((CCRunType)(ccChoice->GetSelection()));
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_EXE_CHOICE
 */

void InputBuilderWindow::OnExeChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetExeType(exeChoice->GetSelection());
	SetupMiscPrefsItems();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void InputBuilderWindow::OnCancelClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    parent->CloseInputBuilderWindow();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void InputBuilderWindow::OnOkClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    parent->GetData()->SetInputData(TmpInputRec);
    parent->CloseInputBuilderWindow();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DEFAULTSBUTTON
 */

void InputBuilderWindow::OnDefaultsbuttonClick( wxCommandEvent& event )
{
    switch(getCurrentPane()) {
        case BASIS_PANE:
            TmpInputRec->Basis->InitData();
            break;
        case CONTROL_PANE:
            TmpInputRec->Control->InitControlPaneData();
            break;
        case DATA_PANE:
            TmpInputRec->Data->InitData();
            break;
        case SYSTEM_PANE:
            TmpInputRec->System->InitData();
            break;
        case DFT_PANE:
            TmpInputRec->DFT->InitData();
            break;
        case MOGUESS_PANE:
            TmpInputRec->Guess->InitData();
            break;
        case HESSOPTS_PANE:
            TmpInputRec->Hessian->InitData();
            break;
        case MISCPREFS_PANE:
            TmpInputRec->Control->InitProgPaneData();
            break;
        case MP2OPTS_PANE:
            TmpInputRec->MP2->InitData();
            break;
        case SCFOPTS_PANE:
            TmpInputRec->SCF->InitData();
            break;
        case STATPOINT_PANE:
            TmpInputRec->StatPt->InitData();
            break;
        default:
            // NO-OP
            break;
    }
    
    SetupItems();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_REVERTBUTTON
 */

void InputBuilderWindow::OnRevertbuttonClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    InputData *OldData = ((MolDisplayWin *)parent)->GetData()->GetInputData();
    
    switch(getCurrentPane()) {
        case BASIS_PANE:
            *(TmpInputRec->Basis) = *(OldData->Basis);
            break;
        case CONTROL_PANE:
            TmpInputRec->Control->RevertControlPane(OldData->Control);
            break;
        case DATA_PANE:
            if(TmpInputRec->Data) delete TmpInputRec->Data;
            TmpInputRec->Data = new DataGroup(OldData->Data);
            break;
        case SYSTEM_PANE:
            *(TmpInputRec->System) = *(OldData->System);
            break;
        case DFT_PANE:
            if(OldData->DFT) {
                *(TmpInputRec->DFT) = *(OldData->DFT);
            }
            else {
                TmpInputRec->DFT->InitData();
            }
            break;
        case MOGUESS_PANE:
            if(OldData->Guess) {
                *(TmpInputRec->Guess) = *(OldData->Guess);
            }
            else {
                TmpInputRec->Guess->InitData();
            }
            break;
        case HESSOPTS_PANE:
            if(OldData->Hessian) {
                *(TmpInputRec->Hessian) = *(OldData->Hessian);
            }
            else {
                TmpInputRec->Hessian->InitData();
            }
            break;
        case MISCPREFS_PANE:
            TmpInputRec->Control->RevertProgPane(OldData->Control);
            break;
        case MP2OPTS_PANE:
            if(OldData->MP2) {
                *(TmpInputRec->MP2) = *(OldData->MP2);
            }
            else {
                TmpInputRec->MP2->InitData();
            }
            break;
        case SCFOPTS_PANE:
            if(OldData->SCF) {
                *(TmpInputRec->SCF) = *(OldData->SCF);
            }
            else {
                TmpInputRec->SCF->InitData();
            }
            break;
        case STATPOINT_PANE:
            if(OldData->StatPt) {
                *(TmpInputRec->StatPt) = *(OldData->StatPt);
            }
            else {
                TmpInputRec->StatPt->InitData();
            }
            break;
        default:
            // NO-OP
            break;
    }
    
    SetupItems();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_WRITEFILEBUTTON
 */

void InputBuilderWindow::OnWritefilebuttonClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    parent->GetData()->SetInputData(TmpInputRec);
    parent->GetData()->WriteInputFile(parent);
}


void InputBuilderWindow::setPaneVisible(int pane, bool visible) {
    int currPos = pane;
    int i = 0;
    
    if(visibleTab[pane] == visible) return;
    
    for(i = 0; i < pane; i++) {
        if(!visibleTab[i]) {
            currPos--;
        }
    }
    
    if(visible) {
        visibleTab[pane] = listBook->InsertPage(currPos, tab[pane], tabText[pane]);
    }
    else {
        visibleTab[pane] = !(listBook->RemovePage(currPos));
    }
}

int InputBuilderWindow::getCurrentPane() {
    getPaneAtPosition(listBook->GetSelection());
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_COORD_CHOICE
 */

void InputBuilderWindow::OnCoordChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Data->SetCoordType((CoordinateType)(coordTypeChoice->GetSelection() + 1));
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_UNIT_CHOICE
 */

void InputBuilderWindow::OnUnitChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Data->SetUnits(unitChoice->GetSelection());
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_POINTGROUP_CHOICE
 */

void InputBuilderWindow::OnPointgroupChoiceSelected( wxCommandEvent& event )
{
	TmpInputRec->Data->SetPointGroup((GAMESSPointGroup)(pointGroupChoice->GetSelection()+1));
	SetupPointGroupOrder();
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ORDER_CHOICE
 */

void InputBuilderWindow::OnOrderChoiceSelected( wxCommandEvent& event )
{
	TmpInputRec->Data->SetPointGroupOrder(paxisOrderChoice->GetSelection()+2);
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SYMMETRY_CHECKBOX
 */

void InputBuilderWindow::OnSymmetryCheckboxClick( wxCommandEvent& event )
{
    TmpInputRec->Data->SetUseSym(event.IsChecked());
}


/*!
 * wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED event handler for ID_IBNOTEBOOK
 */

void InputBuilderWindow::OnIblistbookPageChanged( wxListbookEvent& event )
{
	switch (getPaneAtPosition(event.GetSelection())) {
		case SUMMARY_PANE:
			SetupSummaryItems();
			break;
	}
    event.Skip();
}


int InputBuilderWindow::getPaneAtPosition(int pos) {
    int currPane = pos;
    int i = 0;
    
    for(i = 0; i <= currPane; i++) {
        if(!visibleTab[i]) {
            currPane++;
        }
    }
    
    return currPane;
}

#define kIBBasisMenuItem   2
#define kNumBasisMenuItems 23
void InputBuilderWindow::CheckBasisMenu(void) {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    AtomTypeList * list = parent->GetData()->GetAtomTypes();
    int i = 0;
    bool states[kNumBasisMenuItems];
    
    if(list) {
        for(i = 0; i < kNumBasisMenuItems; i++) states[i] = true;
        list->Sort();
        long MaxType = list->GetMaxAtomType();
        if(MaxType > 86) {
            for(i = 0; i < kNumBasisMenuItems; i++) states[i] = false;
        }
        else if(MaxType > 54) {
            for(i = 0; i < kNumBasisMenuItems; i++) states[i] = false;
            states[0] = true;
            states[18] = true;
        }
        else if(MaxType > 18) {
            states[8] = false;
            states[12] = false;
            states[14] = false;
            states[17] = false;
            states[20] = false;
            states[21] = false;
            states[22] = false;
            if(list->TransitionMetalInList()) {
                states[9] = false;
                states[10] = false;
                states[11] = false;
                states[13] = false;
                states[15] = false;
                states[19] = false;
            }
            if(list->IsAtomInList(19) || list->IsAtomInList(20)) {
                states[9] = false;
                states[10] = false;
                states[11] = false;
                states[13] = false;
                states[15] = false;
            }
            if(MaxType > 30) states[16] = false;
        }
        else if(MaxType > 10) {
            states[10] = false;
            states[12] = false;
            states[15] = false;
            if(list->IsAtomInList(11) ||
               list->IsAtomInList(12) ||
               list->IsAtomInList(13) ||
               list->IsAtomInList(14) ||
               list->IsAtomInList(18)) states[9] = false;
            if(list->IsAtomInList(11) ||
               list->IsAtomInList(12) ||
               list->IsAtomInList(18)) {
                states[13] = false;
                states[14] = false;
            }
        }
        else {
            states[15] = false;
            if(list->IsAtomInList(3) ||
               list->IsAtomInList(4) ||
               list->IsAtomInList(5) ||
               list->IsAtomInList(10)) states[10] = false;
        }
        
        if(list->IsAtomInList(2)) {
            states[13] = false;
            states[14] = false;
            states[16] = false;
        }
        
        delete list;
        
        for(i = 0; i < kNumBasisMenuItems; i++) {
            basisChoice->SetEnabled(i, states[i]);
        }
    }
}
/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ZMAT_VARS_TEXT
 */

void InputBuilderWindow::OnZmatVarsTextUpdated( wxCommandEvent& event )
{
	wxString temp = zmatrixVarsText->GetValue();
	long t;
	if (temp.ToLong(&t)) {
		TmpInputRec->Data->SetNumZVar(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TITLE_TEXTCTRL
 */

void InputBuilderWindow::OnTitleTextctrlUpdated( wxCommandEvent& event )
{
	wxString temp = titleText->GetValue();
	TmpInputRec->Data->SetTitle((const char *)temp.c_str());
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CORE_CHECKBOX
 */

void InputBuilderWindow::OnCoreCheckboxClick( wxCommandEvent& event )
{
	TmpInputRec->System->SetCoreFlag(coreCheck->GetValue());
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TIMELIMIT_TEXT
 */

void InputBuilderWindow::OnTimelimitTextUpdated( wxCommandEvent& event )
{
	wxString temp = timeLimitText->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		TmpInputRec->System->SetConvertedTime(t);
	}

    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TIMELIMITUNITS_CHOICE
 */

void InputBuilderWindow::OnTimelimitunitsChoiceSelected( wxCommandEvent& event )
{
	TmpInputRec->System->SetTimeUnits((TimeUnit) (timeLimitUnitChoice->GetSelection()+1));
    wxString time;
	time.Printf(wxT("%.2f"), TmpInputRec->System->GetConvertedTime());
	timeLimitText->SetValue(time);

    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MEMORY_TEXT
 */

void InputBuilderWindow::OnMemoryTextUpdated( wxCommandEvent& event )
{
	wxString temp = memoryText->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		TmpInputRec->System->SetConvertedMem(t);
	}

    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MEMORYUNITS_CHOICE
 */

void InputBuilderWindow::OnMemoryunitsChoiceSelected( wxCommandEvent& event )
{
	TmpInputRec->System->SetMemUnits((MemoryUnit) (memoryUnitChoice->GetSelection()+1));
	wxString mem;
	mem.Printf(wxT("%.2f"), TmpInputRec->System->GetConvertedMem());
	memoryText->SetValue(mem);

    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_DIAGONALIZATION_CHOICE
 */

void InputBuilderWindow::OnDiagonalizationChoiceSelected( wxCommandEvent& event )
{
	TmpInputRec->System->SetDiag(diagChoice->GetSelection());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_PLBALANCE_RADIOBOX
 */

void InputBuilderWindow::OnPlbalanceRadioboxSelected( wxCommandEvent& event )
{
	TmpInputRec->System->SetBalanceType(plBalanceRadio->GetSelection());
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_EXTERNDATAREP_CHECKBOX
 */

void InputBuilderWindow::OnExterndatarepCheckboxClick( wxCommandEvent& event )
{
	TmpInputRec->System->SetXDR(externDataRepCheck->GetValue());
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_INITGUESS_CHOICE
 */

void InputBuilderWindow::OnInitguessChoiceSelected( wxCommandEvent& event )
{
	int s = initGuessChoice->GetSelection() + 1;
	if (s != TmpInputRec->Guess->GetGuess()) {
		TmpInputRec->Guess->SetGuess(s);
		SetupMOGuessItems();
	}
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PRINTINITGUESS_CHECKBOX
 */

void InputBuilderWindow::OnPrintinitguessCheckboxClick( wxCommandEvent& event )
{
	TmpInputRec->Guess->SetPrintMO(printInitGuessCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_ROTORBITALS_CHECKBOX
 */

void InputBuilderWindow::OnRotorbitalsCheckboxClick( wxCommandEvent& event )
{
	TmpInputRec->Guess->SetMix(rotOrbitalsCheck->GetValue());
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MOGUESS_VECSOURCE_CHOICE
 */

void InputBuilderWindow::OnMoguessVecsourceChoiceSelected( wxCommandEvent& event )
{
	int s = mMOSourceChoice->GetSelection() + 1;
	if (s != TmpInputRec->Guess->GetVecSource()) {
		TmpInputRec->Guess->SetVecSource(s);
		SetupMOGuessItems();
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MOGUESS_VECORBCOUNT_TEXT
 */

void InputBuilderWindow::OnMoguessVecorbcountTextUpdated( wxCommandEvent& event )
{
	long t;
	wxString noTemp = mVecOrbCountText->GetValue();
	if (noTemp.ToLong(&t)) {
		TmpInputRec->Guess->SetNumOrbs(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_MOLPLT_CHECK
 */

void InputBuilderWindow::OnMolpltCheckClick( wxCommandEvent& event )
{
	TmpInputRec->Control->SetMolPlot(mMolPltCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PLTORB_CHECK
 */

void InputBuilderWindow::OnPltorbCheckClick( wxCommandEvent& event )
{
	TmpInputRec->Control->SetPlotOrb(mPltOrbCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_AIMPAC_CHECK
 */

void InputBuilderWindow::OnAimpacCheckClick( wxCommandEvent& event )
{
	TmpInputRec->Control->SetAIMPAC(aimpacCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_RPAC_CHECK
 */

void InputBuilderWindow::OnRpacCheckClick( wxCommandEvent& event )
{
	TmpInputRec->Control->SetRPAC(rpacCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_MISC_PROG_RADIO
 */

void InputBuilderWindow::OnMiscProgRadioSelected( wxCommandEvent& event )
{
	int val = mMiscProgRadio->GetSelection();
	if (val != TmpInputRec->Control->GetFriend()) {
		TmpInputRec->Control->SetFriend((FriendType) val);
		SetupMiscPrefsItems();
		SetupControlItems();
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DIRECTSCF_CHECK
 */

void InputBuilderWindow::OnDirectscfCheckClick( wxCommandEvent& event )
{
	TmpInputRec->SCF->SetDirectSCF(mDirectSCFCheck->GetValue());
	SetupSCFOptsItems();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_FDIFF_CHECK
 */

void InputBuilderWindow::OnFdiffCheckClick( wxCommandEvent& event )
{
	TmpInputRec->SCF->SetFockDiff(mFDiffCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_UHF_NO_CHECK
 */

void InputBuilderWindow::OnUhfNoCheckClick( wxCommandEvent& event )
{
	TmpInputRec->SCF->SetUHFNO(mUHFNOCheck->GetValue());
 //   event.Skip();
}

/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SCF_CONV_SPIN
 */

void InputBuilderWindow::OnScfConvSpinUpdated( wxSpinEvent& event )
{
	TmpInputRec->SCF->SetConvergance(mSCFConvSpin->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_INIT_HESS_RADIO
 */

void InputBuilderWindow::OnInitHessRadioSelected( wxCommandEvent& event )
{
	int val = mInitHessRadio->GetSelection() + 1;
	if (val != TmpInputRec->StatPt->GetHessMethod()) {
		TmpInputRec->StatPt->SetHessMethod(val);
	}
	SetupStatPointItems();
	event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PRINTORBS_CHECK
 */

void InputBuilderWindow::OnPrintorbsCheckClick( wxCommandEvent& event )
{
	TmpInputRec->StatPt->SetAlwaysPrintOrbs(mPrintOrbsCheck->GetValue());
 //   event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_HESS_RECALC_EDIT
 */

void InputBuilderWindow::OnHessRecalcEditUpdated( wxCommandEvent& event )
{
	wxString temp = mHessRecalcEdit->GetValue();
	long t;
	if (temp.ToLong(&t)) {
		TmpInputRec->StatPt->SetHessRecalcInterval(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MAXSTEPCOUNT_EDIT
 */

void InputBuilderWindow::OnMaxstepcountEditUpdated( wxCommandEvent& event )
{
	wxString temp = mMaxStepCountEdit->GetValue();
	long t;
	if (temp.ToLong(&t)) {
		TmpInputRec->StatPt->SetMaxSteps(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_GRADCONVCRITERIA_EDIT
 */

void InputBuilderWindow::OnGradconvcriteriaEditUpdated( wxCommandEvent& event )
{
	wxString temp = mGradConvEdit->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		TmpInputRec->StatPt->SetOptConvergance(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_OPTMETHOD_CHOICE
 */

void InputBuilderWindow::OnOptmethodChoiceSelected( wxCommandEvent& event )
{
	TmpInputRec->StatPt->SetMethod(mOptMethodChoice->GetSelection()+1);
	SetupStatPointItems();
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_INITSTEPSIZE_EDIT
 */

void InputBuilderWindow::OnInitstepsizeEditUpdated( wxCommandEvent& event )
{
	wxString temp = mInitStepSizeEdit->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		TmpInputRec->StatPt->SetInitRadius(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_UPDATESTEPSIZE_CHECK
 */

void InputBuilderWindow::OnUpdatestepsizeCheckClick( wxCommandEvent& event )
{
	TmpInputRec->StatPt->SetRadiusUpdate(mUpdateStepSizeCheck->GetValue());
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MINSTEPSIZE_EDIT
 */

void InputBuilderWindow::OnMinstepsizeEditUpdated( wxCommandEvent& event )
{
	wxString temp = mMinStepSizeEdit->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		TmpInputRec->StatPt->SetMinRadius(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MAXSTEPSIZE_Edit
 */

void InputBuilderWindow::OnMaxstepsizeEditUpdated( wxCommandEvent& event )
{
	wxString temp = mMaxStepSizeEdit->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		TmpInputRec->StatPt->SetMaxRadius(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_STATPT_CHECK
 */

void InputBuilderWindow::OnStatptCheckClick( wxCommandEvent& event )
{
	TmpInputRec->StatPt->SetStatPoint(mIsStatPtCheck->GetValue());
	SetupStatPointItems();
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_STATPT_MODE_EDIT
 */

void InputBuilderWindow::OnStatptModeEditUpdated( wxCommandEvent& event )
{
	wxString temp = mStatPtModeEdit->GetValue();
	long t;
	if (temp.ToLong(&t)) {
		TmpInputRec->StatPt->SetModeFollow(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_STATPT_JUMP_EDIT
 */

void InputBuilderWindow::OnStatptJumpEditUpdated( wxCommandEvent& event )
{
	wxString temp = mStatPtJumpSizeEdit->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		TmpInputRec->StatPt->SetStatJump(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MCHARGE_TEXTCTRL
 */

void InputBuilderWindow::OnMchargeTextctrlUpdated( wxCommandEvent& event )
{
    wxString tmpStr = mchargeText->GetValue();
    long val = 0;
    if(tmpStr.ToLong(&val)) {
        TmpInputRec->Control->SetCharge((short)val);
    }
    
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MULT_TEXTCTRL
 */

void InputBuilderWindow::OnMultTextctrlUpdated( wxCommandEvent& event )
{
    wxString tmpStr = multText->GetValue();
    long val = 0;
    if(tmpStr.ToLong(&val)) {
        TmpInputRec->Control->SetMultiplicity((short)val);
    }
    
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MAXIT_TEXTCTRL
 */

void InputBuilderWindow::OnMaxitTextctrlUpdated( wxCommandEvent& event )
{
    wxString tmpStr = scfIterText->GetValue();
    long val = 0;
    if(tmpStr.ToLong(&val)) {
        TmpInputRec->Control->SetMaxIt((short)val);
    }
    
    event.Skip();
}
/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MP2_CORE_EDIT
 */

void InputBuilderWindow::OnMp2CoreEditUpdated( wxCommandEvent& event )
{
    wxString tmpStr = mMP2CoreEleEdit->GetValue();
    long val;
    if(tmpStr.ToLong(&val)) {
        TmpInputRec->MP2->SetNumCoreElectrons(val);
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MP2MEM_EDIT
 */

void InputBuilderWindow::OnMp2memEditUpdated( wxCommandEvent& event )
{
    wxString tmpStr = mMP2MemEdit->GetValue();
    long val;
    if(tmpStr.ToLong(&val)) {
        TmpInputRec->MP2->SetMemory(val);
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MP2INTCUTOFF_EDIT
 */

void InputBuilderWindow::OnMp2intcutoffEditUpdated( wxCommandEvent& event )
{
    wxString tmpStr = mMP2IntCutoffEdit->GetValue();
    double val;
    if(tmpStr.ToDouble(&val)) {
        TmpInputRec->MP2->SetIntCutoff(val);
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_LMOMP2_CHECK
 */

void InputBuilderWindow::OnLmomp2CheckClick( wxCommandEvent& event )
{
	TmpInputRec->MP2->SetLMOMP2(mLMOMP2Check->GetValue());
	SetupMP2OptsItems();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_MP2PROP_CHECK
 */

void InputBuilderWindow::OnMp2propCheckClick( wxCommandEvent& event )
{
	TmpInputRec->MP2->SetMP2Prop(mMP2PropCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_MP2TRANS_RADIO
 */

void InputBuilderWindow::OnMp2transRadioSelected( wxCommandEvent& event )
{
	TmpInputRec->MP2->SetMethod(mMP2TransRadio->GetSelection()+1);
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_MP2AOSTORAGE_RADIO
 */

void InputBuilderWindow::OnMp2aostorageRadioSelected( wxCommandEvent& event )
{
	TmpInputRec->MP2->SetAOIntMethod(mMP2AOStorageRadio->GetSelection()+1);
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_DFTMETHOD_CHOICE
 */

void InputBuilderWindow::OnDftmethodChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->DFT->SetMethodGrid(!(dftMethodChoice->GetSelection()));
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_FUNCTIONAL_CHOICE
 */

void InputBuilderWindow::OnGridFunctionalChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->DFT->SetFunctional(dftGridFuncChoice->GetSelection() + 1);
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_GRIDFREE_FUNCTIONAL_CHOICE
 */

void InputBuilderWindow::OnGridfreeFunctionalChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->DFT->SetFunctional(dftGridFreeFuncChoice->GetSelection() + 1);
}

