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

    EVT_CHOICE( ID_EXE_CHOICE, InputBuilderWindow::OnExeChoiceSelected )

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
    dftFuncChoice = NULL;
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

////@begin InputBuilderWindow creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end InputBuilderWindow creation

    TmpInputRec = new InputData(((MolDisplayWin *)parent)->GetData()->GetInputData());

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
    itemFlexGridSizer39->Add(mchargeText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText42 = new wxStaticText( itemPanel26, wxID_STATIC, _("Multiplicity:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer39->Add(itemStaticText42, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    multText = new wxTextCtrl( itemPanel26, ID_MULT_TEXTCTRL, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
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
    itemFlexGridSizer45->Add(scfIterText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer50Static = new wxStaticBox(itemPanel26, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer50 = new wxStaticBoxSizer(itemStaticBoxSizer50Static, wxVERTICAL);
    itemBoxSizer36->Add(itemStaticBoxSizer50, 0, wxALIGN_TOP|wxALL, 5);
    mp2Check = new wxCheckBox( itemPanel26, ID_MP2_CHECKBOX, _("Use MP2"), wxDefaultPosition, wxDefaultSize, 0 );
    mp2Check->SetValue(false);
    itemStaticBoxSizer50->Add(mp2Check, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    dftCheck = new wxCheckBox( itemPanel26, ID_DFT_CHECKBOX, _("Use DFT"), wxDefaultPosition, wxDefaultSize, 0 );
    dftCheck->SetValue(false);
    itemStaticBoxSizer50->Add(dftCheck, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer53 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer50->Add(itemFlexGridSizer53, 0, wxGROW, 5);
    ciLabel = new wxStaticText( itemPanel26, wxID_STATIC, _("CI:"), wxDefaultPosition, wxDefaultSize, 0 );
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
        zmatrixVarsText->SetToolTip(_("If set and a set of internal coordinates are defined a $ZMAT group will be punched out."));
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
    itemBoxSizer79->Add(plBalanceRadio, 0, wxALIGN_LEFT|wxALL, 5);

    externDataRepCheck = new wxCheckBox( itemPanel78, ID_EXTERNDATAREP_CHECKBOX, _("Use External Data Representation for messages"), wxDefaultPosition, wxDefaultSize, 0 );
    externDataRepCheck->SetValue(false);
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

    wxString dftFuncChoiceStrings[] = {
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
    dftFuncChoice = new wxUglyChoice( itemPanel93, ID_FUNCTIONAL_CHOICE, wxDefaultPosition, wxDefaultSize, 21, dftFuncChoiceStrings, 0 );
    dftFuncChoice->SetStringSelection(_("Slater exchange"));
    itemFlexGridSizer95->Add(dftFuncChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel93, _("DFT"));

    wxPanel* itemPanel100 = new wxPanel( listBook, ID_IBMOGUESSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer101 = new wxBoxSizer(wxVERTICAL);
    itemPanel100->SetSizer(itemBoxSizer101);

    wxBoxSizer* itemBoxSizer102 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer101->Add(itemBoxSizer102, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText103 = new wxStaticText( itemPanel100, wxID_STATIC, _("Initial Guess:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer102->Add(itemStaticText103, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString initGuessChoiceStrings[] = {
        _("Huckel"),
        _("HCore"),
        _("MO Read ($VEC)"),
        _("MO Saved (DICTNRY)"),
        _("Skip")
    };
    initGuessChoice = new wxUglyChoice( itemPanel100, ID_INITGUESS_CHOICE, wxDefaultPosition, wxSize(300, -1), 5, initGuessChoiceStrings, 0 );
    initGuessChoice->SetStringSelection(_("Huckel"));
    itemBoxSizer102->Add(initGuessChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer105 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer101->Add(itemBoxSizer105, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText106 = new wxStaticText( itemPanel100, wxID_STATIC, _("$VEC source:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer105->Add(itemStaticText106, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString* mMOSourceChoiceStrings = NULL;
    mMOSourceChoice = new wxUglyChoice( itemPanel100, ID_MOGUESS_VECSOURCE_CHOICE, wxDefaultPosition, wxSize(300, -1), 0, mMOSourceChoiceStrings, 0 );
    itemBoxSizer105->Add(mMOSourceChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer108 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer101->Add(itemBoxSizer108, 0, wxALIGN_LEFT|wxALL, 5);
    wxStaticText* itemStaticText109 = new wxStaticText( itemPanel100, wxID_STATIC, _("Number of Orbitals to read in:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer108->Add(itemStaticText109, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mVecOrbCountText = new wxTextCtrl( itemPanel100, ID_MOGUESS_VECORBCOUNT_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer108->Add(mVecOrbCountText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    printInitGuessCheck = new wxCheckBox( itemPanel100, ID_PRINTINITGUESS_CHECKBOX, _("Print the initial guess"), wxDefaultPosition, wxDefaultSize, 0 );
    printInitGuessCheck->SetValue(false);
    itemBoxSizer101->Add(printInitGuessCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    rotOrbitalsCheck = new wxCheckBox( itemPanel100, ID_ROTORBITALS_CHECKBOX, _("Rotate alpha and beta orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    rotOrbitalsCheck->SetValue(false);
    rotOrbitalsCheck->Enable(false);
    itemBoxSizer101->Add(rotOrbitalsCheck, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel100, _("MO Guess"));

    wxPanel* itemPanel113 = new wxPanel( listBook, ID_IBHESSOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer114 = new wxBoxSizer(wxVERTICAL);
    itemPanel113->SetSizer(itemBoxSizer114);

    wxBoxSizer* itemBoxSizer115 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer114->Add(itemBoxSizer115, 0, wxALIGN_LEFT, 5);
    wxString itemRadioBox116Strings[] = {
        _("Analytic"),
        _("Numeric")
    };
    wxRadioBox* itemRadioBox116 = new wxRadioBox( itemPanel113, ID_RADIOBOX, _("Method"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox116Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox116->SetSelection(0);
    itemBoxSizer115->Add(itemRadioBox116, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer117 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer115->Add(itemBoxSizer117, 0, wxALIGN_TOP|wxALL, 5);
    wxStaticText* itemStaticText118 = new wxStaticText( itemPanel113, wxID_STATIC, _("Displacement size (bohrs):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer117->Add(itemStaticText118, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl119 = new wxTextCtrl( itemPanel113, ID_TEXTCTRL15, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer117->Add(itemTextCtrl119, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox120 = new wxCheckBox( itemPanel113, ID_CHECKBOX20, _("Double differenced hessian"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox120->SetValue(false);
    itemBoxSizer114->Add(itemCheckBox120, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox121 = new wxCheckBox( itemPanel113, ID_CHECKBOX21, _("Purify hessian"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox121->SetValue(false);
    itemBoxSizer114->Add(itemCheckBox121, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox122 = new wxCheckBox( itemPanel113, ID_CHECKBOX22, _("Print internal force constants"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox122->SetValue(false);
    itemBoxSizer114->Add(itemCheckBox122, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox123 = new wxCheckBox( itemPanel113, ID_CHECKBOX23, _("Vibrational Analysis"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox123->SetValue(false);
    itemBoxSizer114->Add(itemCheckBox123, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer124 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer114->Add(itemBoxSizer124, 0, wxGROW, 5);
    wxStaticText* itemStaticText125 = new wxStaticText( itemPanel113, wxID_STATIC, _("Frequency scale factor:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer124->Add(itemStaticText125, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl126 = new wxTextCtrl( itemPanel113, ID_TEXTCTRL14, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer124->Add(itemTextCtrl126, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel113, _("Hess. Options"));

    wxPanel* itemPanel127 = new wxPanel( listBook, ID_IBMISCPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer128 = new wxBoxSizer(wxVERTICAL);
    itemPanel127->SetSizer(itemBoxSizer128);

    wxStaticText* itemStaticText129 = new wxStaticText( itemPanel127, wxID_STATIC, _("GAMESS interface to other codes."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer128->Add(itemStaticText129, 0, wxALIGN_LEFT|wxALL|wxADJUST_MINSIZE, 5);

    wxStaticBox* itemStaticBoxSizer130Static = new wxStaticBox(itemPanel127, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer130 = new wxStaticBoxSizer(itemStaticBoxSizer130Static, wxVERTICAL);
    itemBoxSizer128->Add(itemStaticBoxSizer130, 0, wxALIGN_LEFT|wxALL, 5);
    mMolPltCheck = new wxCheckBox( itemPanel127, ID_MOLPLT_CHECK, _("MolPlt"), wxDefaultPosition, wxDefaultSize, 0 );
    mMolPltCheck->SetValue(false);
    itemStaticBoxSizer130->Add(mMolPltCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    mPltOrbCheck = new wxCheckBox( itemPanel127, ID_PLTORB_CHECK, _("PltOrb"), wxDefaultPosition, wxDefaultSize, 0 );
    mPltOrbCheck->SetValue(false);
    itemStaticBoxSizer130->Add(mPltOrbCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    aimpacCheck = new wxCheckBox( itemPanel127, ID_AIMPAC_CHECK, _("Atoms In Molecules (AIMPAC)"), wxDefaultPosition, wxDefaultSize, 0 );
    aimpacCheck->SetValue(false);
    itemStaticBoxSizer130->Add(aimpacCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    rpacCheck = new wxCheckBox( itemPanel127, ID_RPAC_CHECK, _("RPAC"), wxDefaultPosition, wxDefaultSize, 0 );
    rpacCheck->SetValue(false);
    itemStaticBoxSizer130->Add(rpacCheck, 0, wxALIGN_LEFT|wxALL, 5);

    wxString mMiscProgRadioStrings[] = {
        _("None"),
        _("Hondo 8.2"),
        _("MELDF"),
        _("GAMESS (UK version)"),
        _("Gaussian 9x"),
        _("All")
    };
    mMiscProgRadio = new wxRadioBox( itemPanel127, ID_MISC_PROG_RADIO, _("These Force a Check Run Type"), wxDefaultPosition, wxDefaultSize, 6, mMiscProgRadioStrings, 1, wxRA_SPECIFY_COLS );
    mMiscProgRadio->SetSelection(0);
    itemBoxSizer128->Add(mMiscProgRadio, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel127, _("Misc. Prefs"));

    wxPanel* itemPanel136 = new wxPanel( listBook, ID_IBMP2PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer137 = new wxBoxSizer(wxVERTICAL);
    itemPanel136->SetSizer(itemBoxSizer137);

    wxFlexGridSizer* itemFlexGridSizer138 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer137->Add(itemFlexGridSizer138, 0, wxGROW, 5);
    wxStaticText* itemStaticText139 = new wxStaticText( itemPanel136, wxID_STATIC, _("# of core electrons:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemStaticText139, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl140 = new wxTextCtrl( itemPanel136, ID_TEXTCTRL5, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemTextCtrl140, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText141 = new wxStaticText( itemPanel136, wxID_STATIC, _("Memory (in words):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemStaticText141, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl142 = new wxTextCtrl( itemPanel136, ID_TEXTCTRL7, _("All"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemTextCtrl142, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText143 = new wxStaticText( itemPanel136, wxID_STATIC, _("Integral retention cutoff:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemStaticText143, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl144 = new wxTextCtrl( itemPanel136, ID_TEXTCTRL8, _("1e-09"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemTextCtrl144, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox145 = new wxCheckBox( itemPanel136, ID_CHECKBOX3, _("Use Localized Orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox145->SetValue(false);
    itemBoxSizer137->Add(itemCheckBox145, 0, wxGROW|wxALL, 5);

    wxString itemRadioBox146Strings[] = {
        _("Segmented transformation"),
        _("Two phase bin sort")
    };
    wxRadioBox* itemRadioBox146 = new wxRadioBox( itemPanel136, ID_RADIOBOX4, _("Transformation method"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox146Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox146->SetSelection(0);
    itemBoxSizer137->Add(itemRadioBox146, 0, wxALIGN_LEFT|wxALL, 5);

    wxString itemRadioBox147Strings[] = {
        _("Duplicated on each node"),
        _("Distributed across all nodes")
    };
    wxRadioBox* itemRadioBox147 = new wxRadioBox( itemPanel136, ID_RADIOBOX5, _("AO integral storage"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox147Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox147->SetSelection(0);
    itemBoxSizer137->Add(itemRadioBox147, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel136, _("MP2 Options"));

    wxPanel* itemPanel148 = new wxPanel( listBook, ID_IBSCBOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer149 = new wxBoxSizer(wxVERTICAL);
    itemPanel148->SetSizer(itemBoxSizer149);

    wxCheckBox* itemCheckBox150 = new wxCheckBox( itemPanel148, ID_CHECKBOX17, _("Direct SCF"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox150->SetValue(false);
    itemBoxSizer149->Add(itemCheckBox150, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox151 = new wxCheckBox( itemPanel148, ID_CHECKBOX18, _("Compute only change in Fock Matrix"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox151->SetValue(true);
    itemCheckBox151->Enable(false);
    itemBoxSizer149->Add(itemCheckBox151, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox152 = new wxCheckBox( itemPanel148, ID_CHECKBOX19, _("Generate UHF Natural Orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox152->SetValue(false);
    itemCheckBox152->Enable(false);
    itemBoxSizer149->Add(itemCheckBox152, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer153 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer149->Add(itemBoxSizer153, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText154 = new wxStaticText( itemPanel148, wxID_STATIC, _("SCF convergence criteria:  10^"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer153->Add(itemStaticText154, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxSpinCtrl* itemSpinCtrl155 = new wxSpinCtrl( itemPanel148, ID_SPINCTRL2, _T("5"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 2147483647, 5 );
    itemBoxSizer153->Add(itemSpinCtrl155, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel148, _("SCF Options"));

    wxPanel* itemPanel156 = new wxPanel( listBook, ID_IBSTATPOINTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer157 = new wxBoxSizer(wxVERTICAL);
    itemPanel156->SetSizer(itemBoxSizer157);

    wxFlexGridSizer* itemFlexGridSizer158 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer157->Add(itemFlexGridSizer158, 0, wxGROW, 5);
    wxStaticText* itemStaticText159 = new wxStaticText( itemPanel156, wxID_STATIC, _("Max. number of steps:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer158->Add(itemStaticText159, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl160 = new wxTextCtrl( itemPanel156, ID_TEXTCTRL16, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer158->Add(itemTextCtrl160, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText161 = new wxStaticText( itemPanel156, wxID_STATIC, _("Gradient convergance criteria:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer158->Add(itemStaticText161, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl162 = new wxTextCtrl( itemPanel156, ID_TEXTCTRL17, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer158->Add(itemTextCtrl162, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer163 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer157->Add(itemBoxSizer163, 0, wxGROW, 5);
    wxStaticText* itemStaticText164 = new wxStaticText( itemPanel156, wxID_STATIC, _("Optimization method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer163->Add(itemStaticText164, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice165Strings = NULL;
    wxUglyChoice* itemChoice165 = new wxUglyChoice( itemPanel156, ID_CHOICE23, wxDefaultPosition, wxDefaultSize, 0, itemChoice165Strings, 0 );
    itemBoxSizer163->Add(itemChoice165, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer166 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer157->Add(itemBoxSizer166, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer167 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer166->Add(itemFlexGridSizer167, 0, wxGROW, 5);
    wxStaticText* itemStaticText168 = new wxStaticText( itemPanel156, wxID_STATIC, _("Initial step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer167->Add(itemStaticText168, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl169 = new wxTextCtrl( itemPanel156, ID_TEXTCTRL18, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer167->Add(itemTextCtrl169, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText170 = new wxStaticText( itemPanel156, wxID_STATIC, _("Min. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer167->Add(itemStaticText170, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl171 = new wxTextCtrl( itemPanel156, ID_TEXTCTRL19, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer167->Add(itemTextCtrl171, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText172 = new wxStaticText( itemPanel156, wxID_STATIC, _("Max. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer167->Add(itemStaticText172, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl173 = new wxTextCtrl( itemPanel156, ID_TEXTCTRL20, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer167->Add(itemTextCtrl173, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer174 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer166->Add(itemBoxSizer174, 0, wxGROW, 5);
    wxCheckBox* itemCheckBox175 = new wxCheckBox( itemPanel156, ID_CHECKBOX25, _("Update Step Size"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox175->SetValue(false);
    itemBoxSizer174->Add(itemCheckBox175, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox176 = new wxCheckBox( itemPanel156, ID_CHECKBOX26, _("Stationary Pt."), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox176->SetValue(false);
    itemBoxSizer174->Add(itemCheckBox176, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer177 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer157->Add(itemBoxSizer177, 0, wxGROW, 5);
    wxString itemRadioBox178Strings[] = {
        _("Guess (+ definite)"),
        _("Read (from $HESS)"),
        _("Calculate")
    };
    wxRadioBox* itemRadioBox178 = new wxRadioBox( itemPanel156, ID_RADIOBOX3, _("Initial Hessian"), wxDefaultPosition, wxDefaultSize, 3, itemRadioBox178Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox178->SetSelection(0);
    itemBoxSizer177->Add(itemRadioBox178, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText179 = new wxStaticText( itemPanel156, wxID_STATIC, _("Recalc. Hess every"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer177->Add(itemStaticText179, 0, wxALIGN_TOP|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    wxTextCtrl* itemTextCtrl180 = new wxTextCtrl( itemPanel156, ID_TEXTCTRL21, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer177->Add(itemTextCtrl180, 0, wxALIGN_TOP|wxALL, 5);

    wxStaticText* itemStaticText181 = new wxStaticText( itemPanel156, wxID_STATIC, _("steps."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer177->Add(itemStaticText181, 0, wxALIGN_TOP|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    wxCheckBox* itemCheckBox182 = new wxCheckBox( itemPanel156, ID_CHECKBOX24, _("Print Orbs at each iteration"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox182->SetValue(false);
    itemBoxSizer157->Add(itemCheckBox182, 0, wxGROW|wxALL, 5);

    listBook->AddPage(itemPanel156, _("Stat. Point"));

    wxPanel* itemPanel183 = new wxPanel( listBook, ID_SUMMARYPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer184 = new wxBoxSizer(wxVERTICAL);
    itemPanel183->SetSizer(itemBoxSizer184);

    wxBoxSizer* itemBoxSizer185 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer184->Add(itemBoxSizer185, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText186 = new wxStaticText( itemPanel183, wxID_STATIC, _("Title:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer185->Add(itemStaticText186, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mTitleText = new wxTextCtrl( itemPanel183, ID_SUMMARY_TITLE, _T(""), wxDefaultPosition, wxSize(400, -1), 0 );
    mTitleText->Enable(false);
    itemBoxSizer185->Add(mTitleText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer188 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer184->Add(itemBoxSizer188, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText189 = new wxStaticText( itemPanel183, wxID_STATIC, _("Basis Set:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer188->Add(itemStaticText189, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mBasisSetText = new wxTextCtrl( itemPanel183, ID_SUMMARY_BASISSET, _T(""), wxDefaultPosition, wxSize(400, -1), 0 );
    mBasisSetText->Enable(false);
    itemBoxSizer188->Add(mBasisSetText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer191 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer184->Add(itemBoxSizer191, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText192 = new wxStaticText( itemPanel183, wxID_STATIC, _("SCF Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer191->Add(itemStaticText192, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mSCFTypeText = new wxTextCtrl( itemPanel183, ID_SUMMARY_SCFTYPE, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    mSCFTypeText->Enable(false);
    itemBoxSizer191->Add(mSCFTypeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer194 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer184->Add(itemBoxSizer194, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText195 = new wxStaticText( itemPanel183, wxID_STATIC, _("Run Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer194->Add(itemStaticText195, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mRunTypeText = new wxTextCtrl( itemPanel183, ID_SUMMARY_RUNTYPE, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    mRunTypeText->Enable(false);
    itemBoxSizer194->Add(mRunTypeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer197 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer184->Add(itemBoxSizer197, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText198 = new wxStaticText( itemPanel183, wxID_STATIC, _("Molecular Point Group:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer197->Add(itemStaticText198, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mPointGroupText = new wxTextCtrl( itemPanel183, ID_SUMMARY_PG, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    mPointGroupText->Enable(false);
    itemBoxSizer197->Add(mPointGroupText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer200 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer184->Add(itemBoxSizer200, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText201 = new wxStaticText( itemPanel183, wxID_STATIC, _("Electron Correlation"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer200->Add(itemStaticText201, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mElectronCorr = new wxTextCtrl( itemPanel183, ID_SUMMARY_ELEC, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    mElectronCorr->Enable(false);
    itemBoxSizer200->Add(mElectronCorr, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel183, _("Summary"));

    itemBoxSizer4->Add(listBook, 1, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer203 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer4->Add(itemBoxSizer203, 0, wxGROW, 5);

    defaultsBtn = new wxButton( itemPanel3, ID_DEFAULTSBUTTON, _("Use Defaults"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer203->Add(defaultsBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    revertBtn = new wxButton( itemPanel3, ID_REVERTBUTTON, _("Revert"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer203->Add(revertBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    writeBtn = new wxButton( itemPanel3, ID_WRITEFILEBUTTON, _("Write File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer203->Add(writeBtn, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer207 = new wxStdDialogButtonSizer;

    itemBoxSizer203->Add(itemStdDialogButtonSizer207, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* itemButton208 = new wxButton( itemPanel3, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer207->AddButton(itemButton208);

    wxButton* itemButton209 = new wxButton( itemPanel3, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer207->AddButton(itemButton209);

    itemStdDialogButtonSizer207->Realize();

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
    scfIterText->SetValue(wxString::Format("%d", itemValue));
    
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
    mchargeText->SetValue(wxString::Format("%hd", TmpInputRec->Control->GetCharge()));
    
    // multText
    itemValue = TmpInputRec->Control->GetMultiplicity();
    if(itemValue <= 0) {
        if(NumElectrons & 1) itemValue == 2;
        else itemValue = 1;
    }
    multText->SetValue(wxString::Format("%d", itemValue));
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
	zvars.Printf("%d", TmpInputRec->Data->GetNumZVar());
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
    
    wxString time;
	time.Printf("%.2f", TmpInputRec->System->GetConvertedTime());
	timeLimitText->SetValue(time);
	
	timeLimitUnitChoice->SetSelection(TmpInputRec->System->GetTimeUnits()-1);
	
	wxString mem;
	mem.Printf("%.2f", TmpInputRec->System->GetConvertedMem());
	memoryText->SetValue(mem);
	
	memoryUnitChoice->SetSelection(TmpInputRec->System->GetMemUnits()-1);
	
	diagChoice->SetSelection(TmpInputRec->System->GetDiag());
	
	coreCheck->SetValue(TmpInputRec->System->GetCoreFlag());
	
	plBalanceRadio->SetSelection(TmpInputRec->System->GetBalanceType());
	
	externDataRepCheck->SetValue(TmpInputRec->System->GetXDR());
}

void InputBuilderWindow::SetupDFTItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
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
		noText.Printf("%d", numOrbs);
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
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
}

void InputBuilderWindow::SetupSCFOptsItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
}

void InputBuilderWindow::SetupStatPointItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
}

void InputBuilderWindow::SetupSummaryItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	
	if (TmpInputRec->Data->GetTitle()) {
		mTitleText->SetValue(wxString(TmpInputRec->Data->GetTitle(), wxConvUTF8));
	} else mTitleText->Clear();
	if (TmpInputRec->Basis->GetBasis()) {
		wxString temp(TmpInputRec->Basis->GetBasisText(), wxConvUTF8);
		if ((TmpInputRec->Basis->GetBasis()>3)&&(TmpInputRec->Basis->GetBasis()<6)) {
			temp.Printf("%d-%s", TmpInputRec->Basis->GetNumGauss(), TmpInputRec->Basis->GetBasisText());
		} else if (TmpInputRec->Basis->GetBasis()==3) {
			temp.Printf("%s-%dG", TmpInputRec->Basis->GetBasisText(), TmpInputRec->Basis->GetNumGauss());
		}
		if (TmpInputRec->Basis->GetNumDFuncs()) {
			wxString t;
			t.Printf(", # D funcs = %d", TmpInputRec->Basis->GetNumDFuncs());
			temp.Append(t);
		}
		if (TmpInputRec->Basis->GetNumPFuncs()) {
			wxString t;
			t.Printf(", # P funcs = %d", TmpInputRec->Basis->GetNumPFuncs());
			temp.Append(t);
		}
		if (TmpInputRec->Basis->GetNumFFuncs()) {
			wxString t;
			t.Printf(", # F funcs = %d", TmpInputRec->Basis->GetNumFFuncs());
			temp.Append(t);
		}
		if (TmpInputRec->Basis->GetPolar() > GAMESS_BS_No_Polarization) {
			wxString t;
			t.Printf(", Polar = %s", TmpInputRec->Basis->GetPolarText());
			temp.Append(t);
		}
		mBasisSetText->SetValue(temp);
	} else mBasisSetText->Clear();
	
	mRunTypeText->SetValue(ControlGroup::GetGAMESSRunText(TmpInputRec->Control->GetRunType()));

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
					temp.Printf("%d", TmpInputRec->Data->GetPointGroupOrder());
					pg.append(temp);
				} else {
					pg.append((char)pgt[i]);
				}
				i++;
			}
		} else pg = wxString(pgt, wxConvUTF8);
		mPointGroupText->SetValue(pg);
	}
	wxString eclevel;
	if (TmpInputRec->Control->GetMPLevel() == 2)
		eclevel.Printf("MP2");
	else if (TmpInputRec->Control->GetCCType())
		eclevel.Printf(TmpInputRec->Control->GetGAMESSCCType(TmpInputRec->Control->GetCCType()));
	else if (TmpInputRec->Control->GetCIType())
		eclevel.Printf(TmpInputRec->Control->GetCIType(TmpInputRec->Control->GetCIType()));
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
	TmpInputRec->Data->SetTitle(temp.c_str());
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
	time.Printf("%.2f", TmpInputRec->System->GetConvertedTime());
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
	mem.Printf("%.2f", TmpInputRec->System->GetConvertedMem());
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


