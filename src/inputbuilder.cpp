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
#include "inputbuilder.h"

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

    EVT_CHOICE( ID_COORD_CHOICE, InputBuilderWindow::OnCoordChoiceSelected )

    EVT_CHOICE( ID_UNIT_CHOICE, InputBuilderWindow::OnUnitChoiceSelected )

    EVT_CHOICE( ID_POINTGROUP_CHOICE, InputBuilderWindow::OnPointgroupChoiceSelected )

    EVT_CHOICE( ID_ORDER_CHOICE, InputBuilderWindow::OnOrderChoiceSelected )

    EVT_CHECKBOX( ID_SYMMETRY_CHECKBOX, InputBuilderWindow::OnSymmetryCheckboxClick )

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
    timeLimitSpin = NULL;
    timeLimitUnitChoice = NULL;
    memorySpin = NULL;
    memoryUnitChoice = NULL;
    diagChoice = NULL;
    coreCheck = NULL;
    plBalanceRadio = NULL;
    externDataRepCheck = NULL;
    dftMethodChoice = NULL;
    dftFuncChoice = NULL;
    initGuessChoice = NULL;
    printInitGuessCheck = NULL;
    rotOrbitalsCheck = NULL;
    aimpacCheck = NULL;
    rpacCheck = NULL;
    defaultsBtn = NULL;
    revertBtn = NULL;
    writeBtn = NULL;
////@end InputBuilderWindow member initialisation

////@begin InputBuilderWindow creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
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

    wxPanel* itemPanel2 = new wxPanel( itemFrame1, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemPanel2->SetSizer(itemBoxSizer3);

    listBook = new wxListbook( itemPanel2, ID_IBNOTEBOOK, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxLB_LEFT );

    wxPanel* itemPanel5 = new wxPanel( listBook, ID_IBBASISPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxVERTICAL);
    itemPanel5->SetSizer(itemBoxSizer6);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer6->Add(itemBoxSizer7, 0, wxGROW, 5);
    wxStaticText* itemStaticText8 = new wxStaticText( itemPanel5, wxID_STATIC, _("Basis Set:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

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
    basisChoice = new wxUglyChoice( itemPanel5, ID_BASIS_CHOICE, wxDefaultPosition, wxDefaultSize, 25, basisChoiceStrings, 0 );
    basisChoice->SetStringSelection(_("MINI"));
    itemBoxSizer7->Add(basisChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer7->Add(5, 5, 0, wxGROW|wxALL, 5);

    ecpTypeLabel = new wxStaticText( itemPanel5, wxID_STATIC, _("ECP Type:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    ecpTypeLabel->Enable(false);
    itemBoxSizer7->Add(ecpTypeLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxString ecpTypeChoiceStrings[] = {
        _("None"),
        _("Read"),
        _("SBKJC"),
        _("Hay-Wadt")
    };
    ecpTypeChoice = new wxUglyChoice( itemPanel5, ID_ECPT_CHOICE, wxDefaultPosition, wxDefaultSize, 4, ecpTypeChoiceStrings, 0 );
    ecpTypeChoice->SetStringSelection(_("None"));
    ecpTypeChoice->Enable(false);
    itemBoxSizer7->Add(ecpTypeChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer13 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer6->Add(itemFlexGridSizer13, 0, wxGROW, 5);
    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel5, wxID_STATIC, _("# D heavy atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer13->Add(itemStaticText14, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString numDChoiceStrings[] = {
        _("0"),
        _("1"),
        _("2"),
        _("3")
    };
    numDChoice = new wxUglyChoice( itemPanel5, ID_NUMD_CHOICE, wxDefaultPosition, wxDefaultSize, 4, numDChoiceStrings, 0 );
    numDChoice->SetStringSelection(_("0"));
    itemFlexGridSizer13->Add(numDChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText16 = new wxStaticText( itemPanel5, wxID_STATIC, _("# F heavy atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer13->Add(itemStaticText16, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString numFChoiceStrings[] = {
        _("0"),
        _("1"),
        _("2"),
        _("3")
    };
    numFChoice = new wxUglyChoice( itemPanel5, ID_NUMF_CHOICE, wxDefaultPosition, wxDefaultSize, 4, numFChoiceStrings, 0 );
    numFChoice->SetStringSelection(_("0"));
    itemFlexGridSizer13->Add(numFChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText18 = new wxStaticText( itemPanel5, wxID_STATIC, _("# light atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer13->Add(itemStaticText18, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString numPChoiceStrings[] = {
        _("0"),
        _("1"),
        _("2"),
        _("3")
    };
    numPChoice = new wxUglyChoice( itemPanel5, ID_NUMP_CHOICE, wxDefaultPosition, wxDefaultSize, 4, numPChoiceStrings, 0 );
    numPChoice->SetStringSelection(_("0"));
    itemFlexGridSizer13->Add(numPChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer6->Add(itemBoxSizer20, 0, wxGROW, 5);
    polarLabel = new wxStaticText( itemPanel5, wxID_STATIC, _("Polar:"), wxDefaultPosition, wxDefaultSize, 0 );
    polarLabel->Enable(false);
    itemBoxSizer20->Add(polarLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString polarChoiceStrings[] = {
        _("Pople"),
        _("Pople N311"),
        _("Dunning"),
        _("Huzinaga"),
        _("Hondo7")
    };
    polarChoice = new wxUglyChoice( itemPanel5, ID_POLAR_CHOICE, wxDefaultPosition, wxDefaultSize, 5, polarChoiceStrings, 0 );
    polarChoice->SetStringSelection(_("Huzinaga"));
    polarChoice->Enable(false);
    itemBoxSizer20->Add(polarChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    diffuseLCheck = new wxCheckBox( itemPanel5, ID_DIFFL_CHECKBOX, _("Diffuse L-shell on heavy atoms"), wxDefaultPosition, wxDefaultSize, 0 );
    diffuseLCheck->SetValue(false);
    itemBoxSizer6->Add(diffuseLCheck, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    diffuseSCheck = new wxCheckBox( itemPanel5, ID_DIFFS_CHECKBOX, _("Diffuse s-shell on hydrogens"), wxDefaultPosition, wxDefaultSize, 0 );
    diffuseSCheck->SetValue(false);
    itemBoxSizer6->Add(diffuseSCheck, 0, wxGROW|wxALL, 5);

    listBook->AddPage(itemPanel5, _("Basis"));

    wxPanel* itemPanel25 = new wxPanel( listBook, ID_IBCONTROLPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxVERTICAL);
    itemPanel25->SetSizer(itemBoxSizer26);

    wxStaticBox* itemStaticBoxSizer27Static = new wxStaticBox(itemPanel25, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer27 = new wxStaticBoxSizer(itemStaticBoxSizer27Static, wxVERTICAL);
    itemBoxSizer26->Add(itemStaticBoxSizer27, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer28 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer27->Add(itemFlexGridSizer28, 0, wxGROW, 5);
    wxStaticText* itemStaticText29 = new wxStaticText( itemPanel25, wxID_STATIC, _("Run Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer28->Add(itemStaticText29, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

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
    runChoice = new wxUglyChoice( itemPanel25, ID_RUN_CHOICE, wxDefaultPosition, wxDefaultSize, 22, runChoiceStrings, 0 );
    runChoice->SetStringSelection(_("Energy"));
    itemFlexGridSizer28->Add(runChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText31 = new wxStaticText( itemPanel25, wxID_STATIC, _("SCF Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer28->Add(itemStaticText31, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString scfChoiceStrings[] = {
        _("RHF"),
        _("UHF"),
        _("ROHF"),
        _("GVB"),
        _("MCSCF"),
        _("None (CI)")
    };
    scfChoice = new wxUglyChoice( itemPanel25, ID_SCF_CHOICE, wxDefaultPosition, wxDefaultSize, 6, scfChoiceStrings, 0 );
    scfChoice->SetStringSelection(_("RHF"));
    itemFlexGridSizer28->Add(scfChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText33 = new wxStaticText( itemPanel25, wxID_STATIC, _("Localization Method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer28->Add(itemStaticText33, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString localChoiceStrings[] = {
        _("None"),
        _("Foster-Boys"),
        _("Edmiston-Ruedenberg"),
        _("Pipek-Mezey")
    };
    localChoice = new wxUglyChoice( itemPanel25, ID_LOCAL_CHOICE, wxDefaultPosition, wxDefaultSize, 4, localChoiceStrings, 0 );
    localChoice->SetStringSelection(_("None"));
    itemFlexGridSizer28->Add(localChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer26->Add(itemBoxSizer35, 0, wxGROW, 5);
    wxBoxSizer* itemBoxSizer36 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer35->Add(itemBoxSizer36, 0, wxGROW, 5);
    wxStaticBox* itemStaticBoxSizer37Static = new wxStaticBox(itemPanel25, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer37 = new wxStaticBoxSizer(itemStaticBoxSizer37Static, wxVERTICAL);
    itemBoxSizer36->Add(itemStaticBoxSizer37, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer38 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer37->Add(itemFlexGridSizer38, 0, wxGROW, 5);
    wxStaticText* itemStaticText39 = new wxStaticText( itemPanel25, wxID_STATIC, _("Molecule Charge:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer38->Add(itemStaticText39, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    mchargeText = new wxTextCtrl( itemPanel25, ID_MCHARGE_TEXTCTRL, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer38->Add(mchargeText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText41 = new wxStaticText( itemPanel25, wxID_STATIC, _("Multiplicity:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer38->Add(itemStaticText41, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    multText = new wxTextCtrl( itemPanel25, ID_MULT_TEXTCTRL, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer38->Add(multText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer43Static = new wxStaticBox(itemPanel25, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer43 = new wxStaticBoxSizer(itemStaticBoxSizer43Static, wxVERTICAL);
    itemBoxSizer36->Add(itemStaticBoxSizer43, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer44 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer43->Add(itemFlexGridSizer44, 0, wxGROW, 5);
    wxStaticText* itemStaticText45 = new wxStaticText( itemPanel25, wxID_STATIC, _("Exe. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer44->Add(itemStaticText45, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString exeChoiceStrings[] = {
        _("Normal Run"),
        _("Check"),
        _("Debug"),
        _("Other...")
    };
    exeChoice = new wxUglyChoice( itemPanel25, ID_EXE_CHOICE, wxDefaultPosition, wxDefaultSize, 4, exeChoiceStrings, 0 );
    exeChoice->SetStringSelection(_("Normal Run"));
    itemFlexGridSizer44->Add(exeChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText47 = new wxStaticText( itemPanel25, wxID_STATIC, _("Max # SCF Iterations:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer44->Add(itemStaticText47, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    scfIterText = new wxTextCtrl( itemPanel25, ID_MAXIT_TEXTCTRL, _("30"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer44->Add(scfIterText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer49Static = new wxStaticBox(itemPanel25, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer49 = new wxStaticBoxSizer(itemStaticBoxSizer49Static, wxVERTICAL);
    itemBoxSizer35->Add(itemStaticBoxSizer49, 0, wxALIGN_TOP|wxALL, 5);
    mp2Check = new wxCheckBox( itemPanel25, ID_MP2_CHECKBOX, _("Use MP2"), wxDefaultPosition, wxDefaultSize, 0 );
    mp2Check->SetValue(false);
    itemStaticBoxSizer49->Add(mp2Check, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    dftCheck = new wxCheckBox( itemPanel25, ID_DFT_CHECKBOX, _("Use DFT"), wxDefaultPosition, wxDefaultSize, 0 );
    dftCheck->SetValue(false);
    itemStaticBoxSizer49->Add(dftCheck, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer52 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer49->Add(itemFlexGridSizer52, 0, wxGROW, 5);
    ciLabel = new wxStaticText( itemPanel25, wxID_STATIC, _("CI:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer52->Add(ciLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString ciChoiceStrings[] = {
        _("None"),
        _("GUGA"),
        _("Ames Lab. Determinant"),
        _("Occupation Restricted Multiple Active Space"),
        _("CI Singles"),
        _("Full Second Order CI"),
        _("General CI")
    };
    ciChoice = new wxUglyChoice( itemPanel25, ID_CI_CHOICE, wxDefaultPosition, wxDefaultSize, 7, ciChoiceStrings, 0 );
    ciChoice->SetStringSelection(_("None"));
    itemFlexGridSizer52->Add(ciChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    ccLabel = new wxStaticText( itemPanel25, wxID_STATIC, _("CC:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer52->Add(ccLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

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
    ccChoice = new wxUglyChoice( itemPanel25, ID_CC_CHOICE, wxDefaultPosition, wxDefaultSize, 9, ccChoiceStrings, 0 );
    ccChoice->SetStringSelection(_("None"));
    itemFlexGridSizer52->Add(ccChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel25, _("Control"));

    wxPanel* itemPanel57 = new wxPanel( listBook, ID_IBDATAPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer58 = new wxBoxSizer(wxVERTICAL);
    itemPanel57->SetSizer(itemBoxSizer58);

    wxBoxSizer* itemBoxSizer59 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer58->Add(itemBoxSizer59, 0, wxALIGN_LEFT, 5);
    titleText = new wxTextCtrl( itemPanel57, ID_TITLE_TEXTCTRL, _("Title"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer59->Add(titleText, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer61Static = new wxStaticBox(itemPanel57, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer61 = new wxStaticBoxSizer(itemStaticBoxSizer61Static, wxVERTICAL);
    itemBoxSizer59->Add(itemStaticBoxSizer61, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer62 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer61->Add(itemFlexGridSizer62, 0, wxGROW, 5);
    wxStaticText* itemStaticText63 = new wxStaticText( itemPanel57, wxID_STATIC, _("Coord. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer62->Add(itemStaticText63, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString coordTypeChoiceStrings[] = {
        _("Unique cartesian Coords."),
        _("Hilderbrant internals"),
        _("Cartesian coordinates"),
        _("Z-Matrix"),
        _("MOPAC Z-Matrix")
    };
    coordTypeChoice = new wxUglyChoice( itemPanel57, ID_COORD_CHOICE, wxDefaultPosition, wxDefaultSize, 5, coordTypeChoiceStrings, 0 );
    coordTypeChoice->SetStringSelection(_("Unique cartesian Coords."));
    itemFlexGridSizer62->Add(coordTypeChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText65 = new wxStaticText( itemPanel57, wxID_STATIC, _("Units:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer62->Add(itemStaticText65, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString unitChoiceStrings[] = {
        _("Angstroms"),
        _("Bohr")
    };
    unitChoice = new wxUglyChoice( itemPanel57, ID_UNIT_CHOICE, wxDefaultPosition, wxDefaultSize, 2, unitChoiceStrings, 0 );
    unitChoice->SetStringSelection(_("Angstroms"));
    itemFlexGridSizer62->Add(unitChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer67 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer61->Add(itemBoxSizer67, 0, wxGROW, 5);
    wxStaticText* itemStaticText68 = new wxStaticText( itemPanel57, wxID_STATIC, _("# of Z-Matrix Variables:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer67->Add(itemStaticText68, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    zmatrixVarsText = new wxTextCtrl( itemPanel57, ID_TEXTCTRL10, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer67->Add(zmatrixVarsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer70Static = new wxStaticBox(itemPanel57, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer70 = new wxStaticBoxSizer(itemStaticBoxSizer70Static, wxVERTICAL);
    itemBoxSizer59->Add(itemStaticBoxSizer70, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer71 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer70->Add(itemFlexGridSizer71, 0, wxGROW, 5);
    wxStaticText* itemStaticText72 = new wxStaticText( itemPanel57, wxID_STATIC, _("Point Group:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer71->Add(itemStaticText72, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString* pointGroupChoiceStrings = NULL;
    pointGroupChoice = new wxUglyChoice( itemPanel57, ID_POINTGROUP_CHOICE, wxDefaultPosition, wxDefaultSize, 0, pointGroupChoiceStrings, 0 );
    itemFlexGridSizer71->Add(pointGroupChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText74 = new wxStaticText( itemPanel57, wxID_STATIC, _("Order of Principle Axis:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText74->Enable(false);
    itemFlexGridSizer71->Add(itemStaticText74, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* paxisOrderChoiceStrings = NULL;
    paxisOrderChoice = new wxUglyChoice( itemPanel57, ID_ORDER_CHOICE, wxDefaultPosition, wxDefaultSize, 0, paxisOrderChoiceStrings, 0 );
    paxisOrderChoice->Enable(false);
    itemFlexGridSizer71->Add(paxisOrderChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    symmetryCheck = new wxCheckBox( itemPanel57, ID_SYMMETRY_CHECKBOX, _("Use Symmetry During Calculation"), wxDefaultPosition, wxDefaultSize, 0 );
    symmetryCheck->SetValue(true);
    itemStaticBoxSizer70->Add(symmetryCheck, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel57, _("Data"));

    wxPanel* itemPanel77 = new wxPanel( listBook, ID_IBSYSTEMPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer78 = new wxBoxSizer(wxVERTICAL);
    itemPanel77->SetSizer(itemBoxSizer78);

    wxFlexGridSizer* itemFlexGridSizer79 = new wxFlexGridSizer(0, 3, 0, 0);
    itemBoxSizer78->Add(itemFlexGridSizer79, 0, wxGROW, 5);
    wxStaticText* itemStaticText80 = new wxStaticText( itemPanel77, wxID_STATIC, _("Time Limit:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer79->Add(itemStaticText80, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    timeLimitSpin = new wxSpinCtrl( itemPanel77, ID_TIMELIMIT_SPINCTRL, _T("525600"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 2147483647, 525600 );
    itemFlexGridSizer79->Add(timeLimitSpin, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxString timeLimitUnitChoiceStrings[] = {
        _("Seconds"),
        _("Minutes"),
        _("Hours"),
        _("Days"),
        _("Weeks"),
        _("Years"),
        _("Millenia")
    };
    timeLimitUnitChoice = new wxUglyChoice( itemPanel77, ID_TIMELIMITUNITS_CHOICE, wxDefaultPosition, wxDefaultSize, 7, timeLimitUnitChoiceStrings, 0 );
    timeLimitUnitChoice->SetStringSelection(_("Minutes"));
    itemFlexGridSizer79->Add(timeLimitUnitChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText83 = new wxStaticText( itemPanel77, wxID_STATIC, _("Memory:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer79->Add(itemStaticText83, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    memorySpin = new wxSpinCtrl( itemPanel77, ID_MEMORY_SPINCTRL, _T("1000000"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 2147483647, 1000000 );
    itemFlexGridSizer79->Add(memorySpin, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxString memoryUnitChoiceStrings[] = {
        _("Words"),
        _("Bytes"),
        _("MegaWords"),
        _("MegaBytes")
    };
    memoryUnitChoice = new wxUglyChoice( itemPanel77, ID_MEMORYUNITS_CHOICE, wxDefaultPosition, wxDefaultSize, 4, memoryUnitChoiceStrings, 0 );
    memoryUnitChoice->SetStringSelection(_("Words"));
    itemFlexGridSizer79->Add(memoryUnitChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer86 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer78->Add(itemBoxSizer86, 0, wxGROW, 5);
    wxStaticText* itemStaticText87 = new wxStaticText( itemPanel77, wxID_STATIC, _("Diagonalization Method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer86->Add(itemStaticText87, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString diagChoiceStrings[] = {
        _("default"),
        _("EVVRSP"),
        _("GIVEIS"),
        _("JACOBI")
    };
    diagChoice = new wxUglyChoice( itemPanel77, ID_DIAGONALIZATION_CHOICE, wxDefaultPosition, wxDefaultSize, 4, diagChoiceStrings, 0 );
    diagChoice->SetStringSelection(_("default"));
    itemBoxSizer86->Add(diagChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    coreCheck = new wxCheckBox( itemPanel77, ID_CORE_CHECKBOX, _("Produce \"core\" file upon abort"), wxDefaultPosition, wxDefaultSize, 0 );
    coreCheck->SetValue(false);
    itemBoxSizer78->Add(coreCheck, 0, wxALIGN_LEFT|wxALL, 5);

    wxString plBalanceRadioStrings[] = {
        _("Loop"),
        _("Next Value")
    };
    plBalanceRadio = new wxRadioBox( itemPanel77, ID_PLBALANCE_RADIOBOX, _("Parallel Load Balance Type"), wxDefaultPosition, wxDefaultSize, 2, plBalanceRadioStrings, 1, wxRA_SPECIFY_COLS );
    plBalanceRadio->SetSelection(0);
    itemBoxSizer78->Add(plBalanceRadio, 0, wxALIGN_LEFT|wxALL, 5);

    externDataRepCheck = new wxCheckBox( itemPanel77, ID_EXTERNDATAREP_CHECKBOX, _("Use External Data Representation for messages"), wxDefaultPosition, wxDefaultSize, 0 );
    externDataRepCheck->SetValue(false);
    itemBoxSizer78->Add(externDataRepCheck, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel77, _("System"));

    wxPanel* itemPanel92 = new wxPanel( listBook, ID_IBDFTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer93 = new wxBoxSizer(wxVERTICAL);
    itemPanel92->SetSizer(itemBoxSizer93);

    wxFlexGridSizer* itemFlexGridSizer94 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer93->Add(itemFlexGridSizer94, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText95 = new wxStaticText( itemPanel92, wxID_STATIC, _("Method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer94->Add(itemStaticText95, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString dftMethodChoiceStrings[] = {
        _("Grid"),
        _("Grid-Free")
    };
    dftMethodChoice = new wxUglyChoice( itemPanel92, ID_DFTMETHOD_CHOICE, wxDefaultPosition, wxDefaultSize, 2, dftMethodChoiceStrings, 0 );
    dftMethodChoice->SetStringSelection(_("Grid"));
    itemFlexGridSizer94->Add(dftMethodChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText97 = new wxStaticText( itemPanel92, wxID_STATIC, _("DFT Functional:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer94->Add(itemStaticText97, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

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
    dftFuncChoice = new wxUglyChoice( itemPanel92, ID_FUNCTIONAL_CHOICE, wxDefaultPosition, wxDefaultSize, 21, dftFuncChoiceStrings, 0 );
    dftFuncChoice->SetStringSelection(_("Slater exchange"));
    itemFlexGridSizer94->Add(dftFuncChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel92, _("DFT"));

    wxPanel* itemPanel99 = new wxPanel( listBook, ID_IBMOGUESSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer100 = new wxBoxSizer(wxVERTICAL);
    itemPanel99->SetSizer(itemBoxSizer100);

    wxBoxSizer* itemBoxSizer101 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer100->Add(itemBoxSizer101, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText102 = new wxStaticText( itemPanel99, wxID_STATIC, _("Initial Guess:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer101->Add(itemStaticText102, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString initGuessChoiceStrings[] = {
        _("Huckel"),
        _("HCore"),
        _("MO Read ($VEC)"),
        _("MO Saved (DICTNRY)"),
        _("Skip")
    };
    initGuessChoice = new wxUglyChoice( itemPanel99, ID_INITGUESS_CHOICE, wxDefaultPosition, wxDefaultSize, 5, initGuessChoiceStrings, 0 );
    initGuessChoice->SetStringSelection(_("Huckel"));
    itemBoxSizer101->Add(initGuessChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    printInitGuessCheck = new wxCheckBox( itemPanel99, ID_PRINTINITGUESS_CHECKBOX, _("Print the initial guess"), wxDefaultPosition, wxDefaultSize, 0 );
    printInitGuessCheck->SetValue(false);
    itemBoxSizer100->Add(printInitGuessCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    rotOrbitalsCheck = new wxCheckBox( itemPanel99, ID_ROTORBITALS_CHECKBOX, _("Rotate alpha and beta orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    rotOrbitalsCheck->SetValue(false);
    rotOrbitalsCheck->Enable(false);
    itemBoxSizer100->Add(rotOrbitalsCheck, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel99, _("MO Guess"));

    wxPanel* itemPanel106 = new wxPanel( listBook, ID_IBHESSOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer107 = new wxBoxSizer(wxVERTICAL);
    itemPanel106->SetSizer(itemBoxSizer107);

    wxBoxSizer* itemBoxSizer108 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer107->Add(itemBoxSizer108, 0, wxALIGN_LEFT, 5);
    wxString itemRadioBox109Strings[] = {
        _("Analytic"),
        _("Numeric")
    };
    wxRadioBox* itemRadioBox109 = new wxRadioBox( itemPanel106, ID_RADIOBOX, _("Method"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox109Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox109->SetSelection(0);
    itemBoxSizer108->Add(itemRadioBox109, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer110 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer108->Add(itemBoxSizer110, 0, wxALIGN_TOP|wxALL, 5);
    wxStaticText* itemStaticText111 = new wxStaticText( itemPanel106, wxID_STATIC, _("Displacement size (bohrs):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer110->Add(itemStaticText111, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl112 = new wxTextCtrl( itemPanel106, ID_TEXTCTRL15, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer110->Add(itemTextCtrl112, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox113 = new wxCheckBox( itemPanel106, ID_CHECKBOX20, _("Double differenced hessian"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox113->SetValue(false);
    itemBoxSizer107->Add(itemCheckBox113, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox114 = new wxCheckBox( itemPanel106, ID_CHECKBOX21, _("Purify hessian"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox114->SetValue(false);
    itemBoxSizer107->Add(itemCheckBox114, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox115 = new wxCheckBox( itemPanel106, ID_CHECKBOX22, _("Print internal force constants"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox115->SetValue(false);
    itemBoxSizer107->Add(itemCheckBox115, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox116 = new wxCheckBox( itemPanel106, ID_CHECKBOX23, _("Vibrational Analysis"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox116->SetValue(false);
    itemBoxSizer107->Add(itemCheckBox116, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer117 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer107->Add(itemBoxSizer117, 0, wxGROW, 5);
    wxStaticText* itemStaticText118 = new wxStaticText( itemPanel106, wxID_STATIC, _("Frequency scale factor:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer117->Add(itemStaticText118, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl119 = new wxTextCtrl( itemPanel106, ID_TEXTCTRL14, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer117->Add(itemTextCtrl119, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel106, _("Hess. Options"));

    wxPanel* itemPanel120 = new wxPanel( listBook, ID_IBMISCPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer121 = new wxBoxSizer(wxVERTICAL);
    itemPanel120->SetSizer(itemBoxSizer121);

    wxStaticBox* itemStaticBoxSizer122Static = new wxStaticBox(itemPanel120, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer122 = new wxStaticBoxSizer(itemStaticBoxSizer122Static, wxVERTICAL);
    itemBoxSizer121->Add(itemStaticBoxSizer122, 0, wxALIGN_LEFT|wxALL, 5);
    wxCheckBox* itemCheckBox123 = new wxCheckBox( itemPanel120, ID_CHECKBOX13, _("MolPlt"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox123->SetValue(false);
    itemStaticBoxSizer122->Add(itemCheckBox123, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox124 = new wxCheckBox( itemPanel120, ID_CHECKBOX14, _("PltOrb"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox124->SetValue(false);
    itemStaticBoxSizer122->Add(itemCheckBox124, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    aimpacCheck = new wxCheckBox( itemPanel120, ID_CHECKBOX15, _("Atoms In Molecules (AIMPAC)"), wxDefaultPosition, wxDefaultSize, 0 );
    aimpacCheck->SetValue(false);
    itemStaticBoxSizer122->Add(aimpacCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    rpacCheck = new wxCheckBox( itemPanel120, ID_CHECKBOX16, _("RPAC"), wxDefaultPosition, wxDefaultSize, 0 );
    rpacCheck->SetValue(false);
    itemStaticBoxSizer122->Add(rpacCheck, 0, wxALIGN_LEFT|wxALL, 5);

    wxString itemRadioBox127Strings[] = {
        _("None"),
        _("Hondo 8.2"),
        _("MELDF"),
        _("GAMESS (UK version)"),
        _("Gaussian 9x"),
        _("All")
    };
    wxRadioBox* itemRadioBox127 = new wxRadioBox( itemPanel120, ID_RADIOBOX1, _("Forces Check Run Type"), wxDefaultPosition, wxDefaultSize, 6, itemRadioBox127Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox127->SetSelection(0);
    itemBoxSizer121->Add(itemRadioBox127, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel120, _("Misc. Prefs"));

    wxPanel* itemPanel128 = new wxPanel( listBook, ID_IBMP2PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer129 = new wxBoxSizer(wxVERTICAL);
    itemPanel128->SetSizer(itemBoxSizer129);

    wxFlexGridSizer* itemFlexGridSizer130 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer129->Add(itemFlexGridSizer130, 0, wxGROW, 5);
    wxStaticText* itemStaticText131 = new wxStaticText( itemPanel128, wxID_STATIC, _("# of core electrons:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(itemStaticText131, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl132 = new wxTextCtrl( itemPanel128, ID_TEXTCTRL5, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(itemTextCtrl132, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText133 = new wxStaticText( itemPanel128, wxID_STATIC, _("Memory (in words):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(itemStaticText133, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl134 = new wxTextCtrl( itemPanel128, ID_TEXTCTRL7, _("All"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(itemTextCtrl134, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText135 = new wxStaticText( itemPanel128, wxID_STATIC, _("Integral retention cutoff:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(itemStaticText135, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl136 = new wxTextCtrl( itemPanel128, ID_TEXTCTRL8, _("1e-09"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(itemTextCtrl136, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox137 = new wxCheckBox( itemPanel128, ID_CHECKBOX3, _("Use Localized Orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox137->SetValue(false);
    itemBoxSizer129->Add(itemCheckBox137, 0, wxGROW|wxALL, 5);

    wxString itemRadioBox138Strings[] = {
        _("Segmented transformation"),
        _("Two phase bin sort")
    };
    wxRadioBox* itemRadioBox138 = new wxRadioBox( itemPanel128, ID_RADIOBOX4, _("Transformation method"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox138Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox138->SetSelection(0);
    itemBoxSizer129->Add(itemRadioBox138, 0, wxALIGN_LEFT|wxALL, 5);

    wxString itemRadioBox139Strings[] = {
        _("Duplicated on each node"),
        _("Distributed across all nodes")
    };
    wxRadioBox* itemRadioBox139 = new wxRadioBox( itemPanel128, ID_RADIOBOX5, _("AO integral storage"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox139Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox139->SetSelection(0);
    itemBoxSizer129->Add(itemRadioBox139, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel128, _("MP2 Options"));

    wxPanel* itemPanel140 = new wxPanel( listBook, ID_IBSCBOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer141 = new wxBoxSizer(wxVERTICAL);
    itemPanel140->SetSizer(itemBoxSizer141);

    wxCheckBox* itemCheckBox142 = new wxCheckBox( itemPanel140, ID_CHECKBOX17, _("Direct SCF"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox142->SetValue(false);
    itemBoxSizer141->Add(itemCheckBox142, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox143 = new wxCheckBox( itemPanel140, ID_CHECKBOX18, _("Compute only change in Fock Matrix"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox143->SetValue(true);
    itemCheckBox143->Enable(false);
    itemBoxSizer141->Add(itemCheckBox143, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox144 = new wxCheckBox( itemPanel140, ID_CHECKBOX19, _("Generate UHF Natural Orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox144->SetValue(false);
    itemCheckBox144->Enable(false);
    itemBoxSizer141->Add(itemCheckBox144, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer145 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer141->Add(itemBoxSizer145, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText146 = new wxStaticText( itemPanel140, wxID_STATIC, _("SCF convergence criteria:  10^"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer145->Add(itemStaticText146, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxSpinCtrl* itemSpinCtrl147 = new wxSpinCtrl( itemPanel140, ID_SPINCTRL2, _T("5"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 2147483647, 5 );
    itemBoxSizer145->Add(itemSpinCtrl147, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel140, _("SCF Options"));

    wxPanel* itemPanel148 = new wxPanel( listBook, ID_IBSTATPOINTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer149 = new wxBoxSizer(wxVERTICAL);
    itemPanel148->SetSizer(itemBoxSizer149);

    wxFlexGridSizer* itemFlexGridSizer150 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer149->Add(itemFlexGridSizer150, 0, wxGROW, 5);
    wxStaticText* itemStaticText151 = new wxStaticText( itemPanel148, wxID_STATIC, _("Max. number of steps:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer150->Add(itemStaticText151, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl152 = new wxTextCtrl( itemPanel148, ID_TEXTCTRL16, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer150->Add(itemTextCtrl152, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText153 = new wxStaticText( itemPanel148, wxID_STATIC, _("Gradient convergance criteria:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer150->Add(itemStaticText153, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl154 = new wxTextCtrl( itemPanel148, ID_TEXTCTRL17, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer150->Add(itemTextCtrl154, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer155 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer149->Add(itemBoxSizer155, 0, wxGROW, 5);
    wxStaticText* itemStaticText156 = new wxStaticText( itemPanel148, wxID_STATIC, _("Optimization method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer155->Add(itemStaticText156, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice157Strings = NULL;
    wxUglyChoice* itemChoice157 = new wxUglyChoice( itemPanel148, ID_CHOICE23, wxDefaultPosition, wxDefaultSize, 0, itemChoice157Strings, 0 );
    itemBoxSizer155->Add(itemChoice157, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer158 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer149->Add(itemBoxSizer158, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer159 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer158->Add(itemFlexGridSizer159, 0, wxGROW, 5);
    wxStaticText* itemStaticText160 = new wxStaticText( itemPanel148, wxID_STATIC, _("Initial step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer159->Add(itemStaticText160, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl161 = new wxTextCtrl( itemPanel148, ID_TEXTCTRL18, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer159->Add(itemTextCtrl161, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText162 = new wxStaticText( itemPanel148, wxID_STATIC, _("Min. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer159->Add(itemStaticText162, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl163 = new wxTextCtrl( itemPanel148, ID_TEXTCTRL19, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer159->Add(itemTextCtrl163, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText164 = new wxStaticText( itemPanel148, wxID_STATIC, _("Max. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer159->Add(itemStaticText164, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl165 = new wxTextCtrl( itemPanel148, ID_TEXTCTRL20, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer159->Add(itemTextCtrl165, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer166 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer158->Add(itemBoxSizer166, 0, wxGROW, 5);
    wxCheckBox* itemCheckBox167 = new wxCheckBox( itemPanel148, ID_CHECKBOX25, _("Update Step Size"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox167->SetValue(false);
    itemBoxSizer166->Add(itemCheckBox167, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox168 = new wxCheckBox( itemPanel148, ID_CHECKBOX26, _("Stationary Pt."), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox168->SetValue(false);
    itemBoxSizer166->Add(itemCheckBox168, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer169 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer149->Add(itemBoxSizer169, 0, wxGROW, 5);
    wxString itemRadioBox170Strings[] = {
        _("Guess (+ definite)"),
        _("Read (from $HESS)"),
        _("Calculate")
    };
    wxRadioBox* itemRadioBox170 = new wxRadioBox( itemPanel148, ID_RADIOBOX3, _("Initial Hessian"), wxDefaultPosition, wxDefaultSize, 3, itemRadioBox170Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox170->SetSelection(0);
    itemBoxSizer169->Add(itemRadioBox170, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText171 = new wxStaticText( itemPanel148, wxID_STATIC, _("Recalc. Hess every"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer169->Add(itemStaticText171, 0, wxALIGN_TOP|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    wxTextCtrl* itemTextCtrl172 = new wxTextCtrl( itemPanel148, ID_TEXTCTRL21, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer169->Add(itemTextCtrl172, 0, wxALIGN_TOP|wxALL, 5);

    wxStaticText* itemStaticText173 = new wxStaticText( itemPanel148, wxID_STATIC, _("steps."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer169->Add(itemStaticText173, 0, wxALIGN_TOP|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    wxCheckBox* itemCheckBox174 = new wxCheckBox( itemPanel148, ID_CHECKBOX24, _("Print Orbs at each iteration"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox174->SetValue(false);
    itemBoxSizer149->Add(itemCheckBox174, 0, wxGROW|wxALL, 5);

    listBook->AddPage(itemPanel148, _("Stat. Point"));

    wxPanel* itemPanel175 = new wxPanel( listBook, ID_SUMMARYPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

    listBook->AddPage(itemPanel175, _("Summary"));

    itemBoxSizer3->Add(listBook, 1, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer176 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer3->Add(itemBoxSizer176, 0, wxGROW, 5);

    defaultsBtn = new wxButton( itemPanel2, ID_DEFAULTSBUTTON, _("Use Defaults"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer176->Add(defaultsBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    revertBtn = new wxButton( itemPanel2, ID_REVERTBUTTON, _("Revert"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer176->Add(revertBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    writeBtn = new wxButton( itemPanel2, ID_WRITEFILEBUTTON, _("Write File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer176->Add(writeBtn, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer180 = new wxStdDialogButtonSizer;

    itemBoxSizer176->Add(itemStdDialogButtonSizer180, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* itemButton181 = new wxButton( itemPanel2, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer180->AddButton(itemButton181);

    wxButton* itemButton182 = new wxButton( itemPanel2, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer180->AddButton(itemButton182);

    itemStdDialogButtonSizer180->Realize();

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
    setPaneVisible(SYSTEM_PANE,  true);
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
    // TODO:  CheckBasisMenu();
    
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
    
    int itemValue = 0;
    
    itemValue = TmpInputRec->Control->GetRunType();
    if(itemValue == 0) itemValue = 1;
    runChoice->SetSelection(itemValue - 1);
    
    if(scft == 0) {
        if(parent->GetData()->GetNumElectrons() & 1) {
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
    
    // TODO:  scfIterText
    
    // exeChoice
    exeChoice->SetSelection(TmpInputRec->Control->GetExeType());
    if(TmpInputRec->Control->GetFriend()) {
        exeChoice->Enable(false);
    }
    else {
        exeChoice->Enable(true);
    }
    
    // TODO:  mchargeText
    // TODO:  multText
}

void InputBuilderWindow::SetupDataItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    int itemValue;
    
    // coordTypeChoice
    itemValue = TmpInputRec->Data->GetCoordType();
    if(itemValue == 0) itemValue = 1;
    coordTypeChoice->SetSelection(itemValue - 1);
    
    // unitChoice
    unitChoice->SetSelection(TmpInputRec->Data->GetUnits());
    
    // TODO:  pointGroupChoice
    // TODO:  paxisOrderChoice
    
    // symmetryCheck
    symmetryCheck->SetValue(TmpInputRec->Data->GetUseSym());
}

void InputBuilderWindow::SetupSystemItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    
    
}

void InputBuilderWindow::SetupDFTItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
}

void InputBuilderWindow::SetupMOGuessItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
}

void InputBuilderWindow::SetupHessOptsItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
}

void InputBuilderWindow::SetupMiscPrefsItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
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
    int currPane = listBook->GetSelection();
    int i = 0;
    
    for(i = 0; i <= currPane; i++) {
        if(!visibleTab[i]) {
            currPane++;
        }
    }
    
    return currPane;
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
////@begin wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_POINTGROUP_CHOICE in InputBuilderWindow.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_POINTGROUP_CHOICE in InputBuilderWindow. 
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ORDER_CHOICE
 */

void InputBuilderWindow::OnOrderChoiceSelected( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ORDER_CHOICE in InputBuilderWindow.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ORDER_CHOICE in InputBuilderWindow. 
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SYMMETRY_CHECKBOX
 */

void InputBuilderWindow::OnSymmetryCheckboxClick( wxCommandEvent& event )
{
    TmpInputRec->Data->SetUseSym(event.IsChecked());
}


