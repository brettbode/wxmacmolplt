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
    ecpTypeChoice = NULL;
    numDChoice = NULL;
    numFChoice = NULL;
    numPChoice = NULL;
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
    ciChoice = NULL;
    ccChoice = NULL;
    titleText = NULL;
    coordTypeChoice = NULL;
    unitChoice = NULL;
    zmatrixVarsText = NULL;
    pointGroupChoice = NULL;
    paxisOrderChoice = NULL;
    symmetryCheck = NULL;
    aimpacCheck = NULL;
    rpacCheck = NULL;
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

    listBook = new wxListbook( itemFrame1, ID_IBNOTEBOOK, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxLB_LEFT );

    wxPanel* itemPanel4 = new wxPanel( listBook, ID_IBBASISPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemPanel4->SetSizer(itemBoxSizer5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer5->Add(itemBoxSizer6, 0, wxGROW, 5);
    wxStaticText* itemStaticText7 = new wxStaticText( itemPanel4, wxID_STATIC, _("Basis Set:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    itemBoxSizer6->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

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
    basisChoice = new wxUglyChoice( itemPanel4, ID_BASIS_CHOICE, wxDefaultPosition, wxDefaultSize, 25, basisChoiceStrings, 0 );
    basisChoice->SetStringSelection(_("MINI"));
    itemBoxSizer6->Add(basisChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer6->Add(5, 5, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemPanel4, wxID_STATIC, _("ECP Type:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    itemStaticText10->Enable(false);
    itemBoxSizer6->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxString ecpTypeChoiceStrings[] = {
        _("None"),
        _("Read"),
        _("SBKJC"),
        _("Hay-Wadt")
    };
    ecpTypeChoice = new wxUglyChoice( itemPanel4, ID_ECPT_CHOICE, wxDefaultPosition, wxDefaultSize, 4, ecpTypeChoiceStrings, 0 );
    ecpTypeChoice->SetStringSelection(_("None"));
    ecpTypeChoice->Enable(false);
    itemBoxSizer6->Add(ecpTypeChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer12 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer5->Add(itemFlexGridSizer12, 0, wxGROW, 5);
    wxStaticText* itemStaticText13 = new wxStaticText( itemPanel4, wxID_STATIC, _("# D heavy atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer12->Add(itemStaticText13, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString numDChoiceStrings[] = {
        _("0"),
        _("1"),
        _("2"),
        _("3")
    };
    numDChoice = new wxUglyChoice( itemPanel4, ID_NUMD_CHOICE, wxDefaultPosition, wxDefaultSize, 4, numDChoiceStrings, 0 );
    numDChoice->SetStringSelection(_("0"));
    itemFlexGridSizer12->Add(numDChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText15 = new wxStaticText( itemPanel4, wxID_STATIC, _("# F heavy atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer12->Add(itemStaticText15, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString numFChoiceStrings[] = {
        _("0"),
        _("1"),
        _("2"),
        _("3")
    };
    numFChoice = new wxUglyChoice( itemPanel4, ID_NUMF_CHOICE, wxDefaultPosition, wxDefaultSize, 4, numFChoiceStrings, 0 );
    numFChoice->SetStringSelection(_("0"));
    itemFlexGridSizer12->Add(numFChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemPanel4, wxID_STATIC, _("# light atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer12->Add(itemStaticText17, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString numPChoiceStrings[] = {
        _("0"),
        _("1"),
        _("2"),
        _("3")
    };
    numPChoice = new wxUglyChoice( itemPanel4, ID_NUMP_CHOICE, wxDefaultPosition, wxDefaultSize, 4, numPChoiceStrings, 0 );
    numPChoice->SetStringSelection(_("0"));
    itemFlexGridSizer12->Add(numPChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer5->Add(itemBoxSizer19, 0, wxGROW, 5);
    wxStaticText* itemStaticText20 = new wxStaticText( itemPanel4, wxID_STATIC, _("Polar:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText20->Enable(false);
    itemBoxSizer19->Add(itemStaticText20, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString polarChoiceStrings[] = {
        _("Pople"),
        _("Pople N311"),
        _("Dunning"),
        _("Huzinaga"),
        _("Hondo7")
    };
    polarChoice = new wxUglyChoice( itemPanel4, ID_POLAR_CHOICE, wxDefaultPosition, wxDefaultSize, 5, polarChoiceStrings, 0 );
    polarChoice->SetStringSelection(_("Huzinaga"));
    polarChoice->Enable(false);
    itemBoxSizer19->Add(polarChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    diffuseLCheck = new wxCheckBox( itemPanel4, ID_DIFFL_CHECKBOX, _("Diffuse L-shell on heavy atoms"), wxDefaultPosition, wxDefaultSize, 0 );
    diffuseLCheck->SetValue(false);
    itemBoxSizer5->Add(diffuseLCheck, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    diffuseSCheck = new wxCheckBox( itemPanel4, ID_DIFFS_CHECKBOX, _("Diffuse s-shell on hydrogens"), wxDefaultPosition, wxDefaultSize, 0 );
    diffuseSCheck->SetValue(false);
    itemBoxSizer5->Add(diffuseSCheck, 0, wxGROW|wxALL, 5);

    listBook->AddPage(itemPanel4, _("Basis"));

    wxPanel* itemPanel24 = new wxPanel( listBook, ID_IBCONTROLPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxVERTICAL);
    itemPanel24->SetSizer(itemBoxSizer25);

    wxStaticBox* itemStaticBoxSizer26Static = new wxStaticBox(itemPanel24, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer26 = new wxStaticBoxSizer(itemStaticBoxSizer26Static, wxVERTICAL);
    itemBoxSizer25->Add(itemStaticBoxSizer26, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer27 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer26->Add(itemFlexGridSizer27, 0, wxGROW, 5);
    wxStaticText* itemStaticText28 = new wxStaticText( itemPanel24, wxID_STATIC, _("Run Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer27->Add(itemStaticText28, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

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
    runChoice = new wxUglyChoice( itemPanel24, ID_RUN_CHOICE, wxDefaultPosition, wxDefaultSize, 22, runChoiceStrings, 0 );
    runChoice->SetStringSelection(_("Energy"));
    itemFlexGridSizer27->Add(runChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText30 = new wxStaticText( itemPanel24, wxID_STATIC, _("SCF Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer27->Add(itemStaticText30, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString scfChoiceStrings[] = {
        _("RHF"),
        _("UHF"),
        _("ROHF"),
        _("GVB"),
        _("MCSCF"),
        _("None (CI)")
    };
    scfChoice = new wxUglyChoice( itemPanel24, ID_SCF_CHOICE, wxDefaultPosition, wxDefaultSize, 6, scfChoiceStrings, 0 );
    scfChoice->SetStringSelection(_("RHF"));
    itemFlexGridSizer27->Add(scfChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText32 = new wxStaticText( itemPanel24, wxID_STATIC, _("Localization Method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer27->Add(itemStaticText32, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString localChoiceStrings[] = {
        _("None"),
        _("Foster-Boys"),
        _("Edmiston-Ruedenberg"),
        _("Pipek-Mezey")
    };
    localChoice = new wxUglyChoice( itemPanel24, ID_LOCAL_CHOICE, wxDefaultPosition, wxDefaultSize, 4, localChoiceStrings, 0 );
    localChoice->SetStringSelection(_("None"));
    itemFlexGridSizer27->Add(localChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer34 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer25->Add(itemBoxSizer34, 0, wxGROW, 5);
    wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer34->Add(itemBoxSizer35, 0, wxGROW, 5);
    wxStaticBox* itemStaticBoxSizer36Static = new wxStaticBox(itemPanel24, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer36 = new wxStaticBoxSizer(itemStaticBoxSizer36Static, wxVERTICAL);
    itemBoxSizer35->Add(itemStaticBoxSizer36, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer37 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer36->Add(itemFlexGridSizer37, 0, wxGROW, 5);
    wxStaticText* itemStaticText38 = new wxStaticText( itemPanel24, wxID_STATIC, _("Molecule Charge:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer37->Add(itemStaticText38, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    mchargeText = new wxTextCtrl( itemPanel24, ID_MCHARGE_TEXTCTRL, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer37->Add(mchargeText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText40 = new wxStaticText( itemPanel24, wxID_STATIC, _("Multiplicity:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer37->Add(itemStaticText40, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    multText = new wxTextCtrl( itemPanel24, ID_MULT_TEXTCTRL, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer37->Add(multText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer42Static = new wxStaticBox(itemPanel24, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer42 = new wxStaticBoxSizer(itemStaticBoxSizer42Static, wxVERTICAL);
    itemBoxSizer35->Add(itemStaticBoxSizer42, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer43 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer42->Add(itemFlexGridSizer43, 0, wxGROW, 5);
    wxStaticText* itemStaticText44 = new wxStaticText( itemPanel24, wxID_STATIC, _("Exe. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer43->Add(itemStaticText44, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString exeChoiceStrings[] = {
        _("Normal Run"),
        _("Check"),
        _("Debug"),
        _("Other...")
    };
    exeChoice = new wxUglyChoice( itemPanel24, ID_EXE_CHOICE, wxDefaultPosition, wxDefaultSize, 4, exeChoiceStrings, 0 );
    exeChoice->SetStringSelection(_("Normal Run"));
    itemFlexGridSizer43->Add(exeChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText46 = new wxStaticText( itemPanel24, wxID_STATIC, _("Max # SCF Iterations:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer43->Add(itemStaticText46, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    scfIterText = new wxTextCtrl( itemPanel24, ID_MAXIT_TEXTCTRL, _("30"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer43->Add(scfIterText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer48Static = new wxStaticBox(itemPanel24, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer48 = new wxStaticBoxSizer(itemStaticBoxSizer48Static, wxVERTICAL);
    itemBoxSizer34->Add(itemStaticBoxSizer48, 0, wxALIGN_TOP|wxALL, 5);
    mp2Check = new wxCheckBox( itemPanel24, ID_MP2_CHECKBOX, _("Use MP2"), wxDefaultPosition, wxDefaultSize, 0 );
    mp2Check->SetValue(false);
    itemStaticBoxSizer48->Add(mp2Check, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    dftCheck = new wxCheckBox( itemPanel24, ID_DFT_CHECKBOX, _("Use DFT"), wxDefaultPosition, wxDefaultSize, 0 );
    dftCheck->SetValue(false);
    itemStaticBoxSizer48->Add(dftCheck, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer51 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer48->Add(itemFlexGridSizer51, 0, wxGROW, 5);
    wxStaticText* itemStaticText52 = new wxStaticText( itemPanel24, wxID_STATIC, _("CI:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer51->Add(itemStaticText52, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString ciChoiceStrings[] = {
        _("None"),
        _("GUGA"),
        _("Ames Lab. Determinant"),
        _("Occupation Restricted Multiple Active Space"),
        _("CI Singles"),
        _("Full Second Order CI"),
        _("General CI")
    };
    ciChoice = new wxUglyChoice( itemPanel24, ID_CI_CHOICE, wxDefaultPosition, wxDefaultSize, 7, ciChoiceStrings, 0 );
    ciChoice->SetStringSelection(_("None"));
    itemFlexGridSizer51->Add(ciChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText54 = new wxStaticText( itemPanel24, wxID_STATIC, _("CC:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer51->Add(itemStaticText54, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

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
    ccChoice = new wxUglyChoice( itemPanel24, ID_CC_CHOICE, wxDefaultPosition, wxDefaultSize, 9, ccChoiceStrings, 0 );
    ccChoice->SetStringSelection(_("None"));
    itemFlexGridSizer51->Add(ccChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel24, _("Control"));

    wxPanel* itemPanel56 = new wxPanel( listBook, ID_IBDATAPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxVERTICAL);
    itemPanel56->SetSizer(itemBoxSizer57);

    wxBoxSizer* itemBoxSizer58 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer57->Add(itemBoxSizer58, 0, wxALIGN_LEFT, 5);
    titleText = new wxTextCtrl( itemPanel56, ID_TITLE_TEXTCTRL, _("Title"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer58->Add(titleText, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer60Static = new wxStaticBox(itemPanel56, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer60 = new wxStaticBoxSizer(itemStaticBoxSizer60Static, wxVERTICAL);
    itemBoxSizer58->Add(itemStaticBoxSizer60, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer61 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer60->Add(itemFlexGridSizer61, 0, wxGROW, 5);
    wxStaticText* itemStaticText62 = new wxStaticText( itemPanel56, wxID_STATIC, _("Coord. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer61->Add(itemStaticText62, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString coordTypeChoiceStrings[] = {
        _("Unique cartesian Coords."),
        _("Hilderbrant internals"),
        _("Cartesian coordinates"),
        _("Z-Matrix"),
        _("MOPAC Z-Matrix")
    };
    coordTypeChoice = new wxUglyChoice( itemPanel56, ID_COORD_CHOICE, wxDefaultPosition, wxDefaultSize, 5, coordTypeChoiceStrings, 0 );
    coordTypeChoice->SetStringSelection(_("Unique cartesian Coords."));
    itemFlexGridSizer61->Add(coordTypeChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText64 = new wxStaticText( itemPanel56, wxID_STATIC, _("Units:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer61->Add(itemStaticText64, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString unitChoiceStrings[] = {
        _("Angstroms"),
        _("Bohr")
    };
    unitChoice = new wxUglyChoice( itemPanel56, ID_UNIT_CHOICE, wxDefaultPosition, wxDefaultSize, 2, unitChoiceStrings, 0 );
    unitChoice->SetStringSelection(_("Angstroms"));
    itemFlexGridSizer61->Add(unitChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer66 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer60->Add(itemBoxSizer66, 0, wxGROW, 5);
    wxStaticText* itemStaticText67 = new wxStaticText( itemPanel56, wxID_STATIC, _("# of Z-Matrix Variables:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer66->Add(itemStaticText67, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    zmatrixVarsText = new wxTextCtrl( itemPanel56, ID_TEXTCTRL10, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer66->Add(zmatrixVarsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer69Static = new wxStaticBox(itemPanel56, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer69 = new wxStaticBoxSizer(itemStaticBoxSizer69Static, wxVERTICAL);
    itemBoxSizer58->Add(itemStaticBoxSizer69, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer70 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer69->Add(itemFlexGridSizer70, 0, wxGROW, 5);
    wxStaticText* itemStaticText71 = new wxStaticText( itemPanel56, wxID_STATIC, _("Point Group:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer70->Add(itemStaticText71, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString* pointGroupChoiceStrings = NULL;
    pointGroupChoice = new wxUglyChoice( itemPanel56, ID_POINTGROUP_CHOICE, wxDefaultPosition, wxDefaultSize, 0, pointGroupChoiceStrings, 0 );
    itemFlexGridSizer70->Add(pointGroupChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText73 = new wxStaticText( itemPanel56, wxID_STATIC, _("Order of Principle Axis:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText73->Enable(false);
    itemFlexGridSizer70->Add(itemStaticText73, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* paxisOrderChoiceStrings = NULL;
    paxisOrderChoice = new wxUglyChoice( itemPanel56, ID_ORDER_CHOICE, wxDefaultPosition, wxDefaultSize, 0, paxisOrderChoiceStrings, 0 );
    paxisOrderChoice->Enable(false);
    itemFlexGridSizer70->Add(paxisOrderChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    symmetryCheck = new wxCheckBox( itemPanel56, ID_SYMMETRY_CHECKBOX, _("Use Symmetry During Calculation"), wxDefaultPosition, wxDefaultSize, 0 );
    symmetryCheck->SetValue(true);
    itemStaticBoxSizer69->Add(symmetryCheck, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel56, _("Data"));

    wxPanel* itemPanel76 = new wxPanel( listBook, ID_IBSYSTEMPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer77 = new wxBoxSizer(wxVERTICAL);
    itemPanel76->SetSizer(itemBoxSizer77);

    wxFlexGridSizer* itemFlexGridSizer78 = new wxFlexGridSizer(0, 3, 0, 0);
    itemBoxSizer77->Add(itemFlexGridSizer78, 0, wxGROW, 5);
    wxStaticText* itemStaticText79 = new wxStaticText( itemPanel76, wxID_STATIC, _("Time Limit:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer78->Add(itemStaticText79, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxSpinCtrl* itemSpinCtrl80 = new wxSpinCtrl( itemPanel76, ID_SPINCTRL, _T("525600"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 2147483647, 525600 );
    itemFlexGridSizer78->Add(itemSpinCtrl80, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxString itemChoice81Strings[] = {
        _("Seconds"),
        _("Minutes"),
        _("Hours"),
        _("Days"),
        _("Weeks"),
        _("Years"),
        _("Millenia")
    };
    wxUglyChoice* itemChoice81 = new wxUglyChoice( itemPanel76, ID_CHOICE19, wxDefaultPosition, wxDefaultSize, 7, itemChoice81Strings, 0 );
    itemChoice81->SetStringSelection(_("Minutes"));
    itemFlexGridSizer78->Add(itemChoice81, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText82 = new wxStaticText( itemPanel76, wxID_STATIC, _("Memory:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer78->Add(itemStaticText82, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxSpinCtrl* itemSpinCtrl83 = new wxSpinCtrl( itemPanel76, ID_SPINCTRL1, _T("1000000"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 2147483647, 1000000 );
    itemFlexGridSizer78->Add(itemSpinCtrl83, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxString itemChoice84Strings[] = {
        _("Words"),
        _("Bytes"),
        _("MegaWords"),
        _("MegaBytes")
    };
    wxUglyChoice* itemChoice84 = new wxUglyChoice( itemPanel76, ID_CHOICE20, wxDefaultPosition, wxDefaultSize, 4, itemChoice84Strings, 0 );
    itemChoice84->SetStringSelection(_("Words"));
    itemFlexGridSizer78->Add(itemChoice84, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer85 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer77->Add(itemBoxSizer85, 0, wxGROW, 5);
    wxStaticText* itemStaticText86 = new wxStaticText( itemPanel76, wxID_STATIC, _("Diagonalization Method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer85->Add(itemStaticText86, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString itemChoice87Strings[] = {
        _("default"),
        _("EVVRSP"),
        _("GIVEIS"),
        _("JACOBI")
    };
    wxUglyChoice* itemChoice87 = new wxUglyChoice( itemPanel76, ID_CHOICE21, wxDefaultPosition, wxDefaultSize, 4, itemChoice87Strings, 0 );
    itemChoice87->SetStringSelection(_("default"));
    itemBoxSizer85->Add(itemChoice87, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox88 = new wxCheckBox( itemPanel76, ID_CHECKBOX9, _("Produce \"core\" file upon abort"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox88->SetValue(false);
    itemBoxSizer77->Add(itemCheckBox88, 0, wxALIGN_LEFT|wxALL, 5);

    wxString itemRadioBox89Strings[] = {
        _("Loop"),
        _("Next Value")
    };
    wxRadioBox* itemRadioBox89 = new wxRadioBox( itemPanel76, ID_RADIOBOX, _("Parallel Load Balance Type"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox89Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox89->SetSelection(0);
    itemBoxSizer77->Add(itemRadioBox89, 0, wxALIGN_LEFT|wxALL, 5);

    wxCheckBox* itemCheckBox90 = new wxCheckBox( itemPanel76, ID_CHECKBOX10, _("Use External Data Representation for messages"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox90->SetValue(false);
    itemBoxSizer77->Add(itemCheckBox90, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel76, _("System"));

    wxPanel* itemPanel91 = new wxPanel( listBook, ID_IBDFTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer92 = new wxBoxSizer(wxVERTICAL);
    itemPanel91->SetSizer(itemBoxSizer92);

    wxFlexGridSizer* itemFlexGridSizer93 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer92->Add(itemFlexGridSizer93, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText94 = new wxStaticText( itemPanel91, wxID_STATIC, _("Method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer93->Add(itemStaticText94, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString itemChoice95Strings[] = {
        _("Grid"),
        _("Grid-Free")
    };
    wxUglyChoice* itemChoice95 = new wxUglyChoice( itemPanel91, ID_CHOICE3, wxDefaultPosition, wxDefaultSize, 2, itemChoice95Strings, 0 );
    itemChoice95->SetStringSelection(_("Grid"));
    itemFlexGridSizer93->Add(itemChoice95, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText96 = new wxStaticText( itemPanel91, wxID_STATIC, _("DFT Functional:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer93->Add(itemStaticText96, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString itemChoice97Strings[] = {
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
    wxUglyChoice* itemChoice97 = new wxUglyChoice( itemPanel91, ID_CHOICE4, wxDefaultPosition, wxDefaultSize, 21, itemChoice97Strings, 0 );
    itemChoice97->SetStringSelection(_("Slater exchange"));
    itemFlexGridSizer93->Add(itemChoice97, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel91, _("DFT"));

    wxPanel* itemPanel98 = new wxPanel( listBook, ID_IBMOGUESSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer99 = new wxBoxSizer(wxVERTICAL);
    itemPanel98->SetSizer(itemBoxSizer99);

    wxBoxSizer* itemBoxSizer100 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer99->Add(itemBoxSizer100, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText101 = new wxStaticText( itemPanel98, wxID_STATIC, _("Initial Guess:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer100->Add(itemStaticText101, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString itemChoice102Strings[] = {
        _("Huckel"),
        _("HCore"),
        _("MO Read ($VEC)"),
        _("MO Saved (DICTNRY)"),
        _("Skip")
    };
    wxUglyChoice* itemChoice102 = new wxUglyChoice( itemPanel98, ID_CHOICE22, wxDefaultPosition, wxDefaultSize, 5, itemChoice102Strings, 0 );
    itemChoice102->SetStringSelection(_("Huckel"));
    itemBoxSizer100->Add(itemChoice102, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox103 = new wxCheckBox( itemPanel98, ID_CHECKBOX11, _("Print the initial guess"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox103->SetValue(false);
    itemBoxSizer99->Add(itemCheckBox103, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox104 = new wxCheckBox( itemPanel98, ID_CHECKBOX12, _("Rotate alpha and beta orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox104->SetValue(false);
    itemCheckBox104->Enable(false);
    itemBoxSizer99->Add(itemCheckBox104, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel98, _("MO Guess"));

    wxPanel* itemPanel105 = new wxPanel( listBook, ID_IBHESSOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer106 = new wxBoxSizer(wxVERTICAL);
    itemPanel105->SetSizer(itemBoxSizer106);

    wxBoxSizer* itemBoxSizer107 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer106->Add(itemBoxSizer107, 0, wxALIGN_LEFT, 5);
    wxString itemRadioBox108Strings[] = {
        _("Analytic"),
        _("Numeric")
    };
    wxRadioBox* itemRadioBox108 = new wxRadioBox( itemPanel105, ID_RADIOBOX2, _("Method"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox108Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox108->SetSelection(0);
    itemBoxSizer107->Add(itemRadioBox108, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer109 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer107->Add(itemBoxSizer109, 0, wxALIGN_TOP|wxALL, 5);
    wxStaticText* itemStaticText110 = new wxStaticText( itemPanel105, wxID_STATIC, _("Displacement size (bohrs):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer109->Add(itemStaticText110, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl111 = new wxTextCtrl( itemPanel105, ID_TEXTCTRL15, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer109->Add(itemTextCtrl111, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox112 = new wxCheckBox( itemPanel105, ID_CHECKBOX20, _("Double differenced hessian"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox112->SetValue(false);
    itemBoxSizer106->Add(itemCheckBox112, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox113 = new wxCheckBox( itemPanel105, ID_CHECKBOX21, _("Purify hessian"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox113->SetValue(false);
    itemBoxSizer106->Add(itemCheckBox113, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox114 = new wxCheckBox( itemPanel105, ID_CHECKBOX22, _("Print internal force constants"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox114->SetValue(false);
    itemBoxSizer106->Add(itemCheckBox114, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox115 = new wxCheckBox( itemPanel105, ID_CHECKBOX23, _("Vibrational Analysis"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox115->SetValue(false);
    itemBoxSizer106->Add(itemCheckBox115, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer116 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer106->Add(itemBoxSizer116, 0, wxGROW, 5);
    wxStaticText* itemStaticText117 = new wxStaticText( itemPanel105, wxID_STATIC, _("Frequency scale factor:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer116->Add(itemStaticText117, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl118 = new wxTextCtrl( itemPanel105, ID_TEXTCTRL14, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer116->Add(itemTextCtrl118, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel105, _("Hess. Options"));

    wxPanel* itemPanel119 = new wxPanel( listBook, ID_IBMISCPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer120 = new wxBoxSizer(wxVERTICAL);
    itemPanel119->SetSizer(itemBoxSizer120);

    wxStaticBox* itemStaticBoxSizer121Static = new wxStaticBox(itemPanel119, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer121 = new wxStaticBoxSizer(itemStaticBoxSizer121Static, wxVERTICAL);
    itemBoxSizer120->Add(itemStaticBoxSizer121, 0, wxALIGN_LEFT|wxALL, 5);
    wxCheckBox* itemCheckBox122 = new wxCheckBox( itemPanel119, ID_CHECKBOX13, _("MolPlt"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox122->SetValue(false);
    itemStaticBoxSizer121->Add(itemCheckBox122, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox123 = new wxCheckBox( itemPanel119, ID_CHECKBOX14, _("PltOrb"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox123->SetValue(false);
    itemStaticBoxSizer121->Add(itemCheckBox123, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    aimpacCheck = new wxCheckBox( itemPanel119, ID_CHECKBOX15, _("Atoms In Molecules (AIMPAC)"), wxDefaultPosition, wxDefaultSize, 0 );
    aimpacCheck->SetValue(false);
    itemStaticBoxSizer121->Add(aimpacCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    rpacCheck = new wxCheckBox( itemPanel119, ID_CHECKBOX16, _("RPAC"), wxDefaultPosition, wxDefaultSize, 0 );
    rpacCheck->SetValue(false);
    itemStaticBoxSizer121->Add(rpacCheck, 0, wxALIGN_LEFT|wxALL, 5);

    wxString itemRadioBox126Strings[] = {
        _("None"),
        _("Hondo 8.2"),
        _("MELDF"),
        _("GAMESS (UK version)"),
        _("Gaussian 9x"),
        _("All")
    };
    wxRadioBox* itemRadioBox126 = new wxRadioBox( itemPanel119, ID_RADIOBOX1, _("Forces Check Run Type"), wxDefaultPosition, wxDefaultSize, 6, itemRadioBox126Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox126->SetSelection(0);
    itemBoxSizer120->Add(itemRadioBox126, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel119, _("Misc. Prefs"));

    wxPanel* itemPanel127 = new wxPanel( listBook, ID_IBMP2PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer128 = new wxBoxSizer(wxVERTICAL);
    itemPanel127->SetSizer(itemBoxSizer128);

    wxFlexGridSizer* itemFlexGridSizer129 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer128->Add(itemFlexGridSizer129, 0, wxGROW, 5);
    wxStaticText* itemStaticText130 = new wxStaticText( itemPanel127, wxID_STATIC, _("# of core electrons:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer129->Add(itemStaticText130, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl131 = new wxTextCtrl( itemPanel127, ID_TEXTCTRL5, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer129->Add(itemTextCtrl131, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText132 = new wxStaticText( itemPanel127, wxID_STATIC, _("Memory (in words):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer129->Add(itemStaticText132, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl133 = new wxTextCtrl( itemPanel127, ID_TEXTCTRL7, _("All"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer129->Add(itemTextCtrl133, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText134 = new wxStaticText( itemPanel127, wxID_STATIC, _("Integral retention cutoff:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer129->Add(itemStaticText134, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl135 = new wxTextCtrl( itemPanel127, ID_TEXTCTRL8, _("1e-09"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer129->Add(itemTextCtrl135, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox136 = new wxCheckBox( itemPanel127, ID_CHECKBOX3, _("Use Localized Orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox136->SetValue(false);
    itemBoxSizer128->Add(itemCheckBox136, 0, wxGROW|wxALL, 5);

    wxString itemRadioBox137Strings[] = {
        _("Segmented transformation"),
        _("Two phase bin sort")
    };
    wxRadioBox* itemRadioBox137 = new wxRadioBox( itemPanel127, ID_RADIOBOX4, _("Transformation method"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox137Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox137->SetSelection(0);
    itemBoxSizer128->Add(itemRadioBox137, 0, wxALIGN_LEFT|wxALL, 5);

    wxString itemRadioBox138Strings[] = {
        _("Duplicated on each node"),
        _("Distributed across all nodes")
    };
    wxRadioBox* itemRadioBox138 = new wxRadioBox( itemPanel127, ID_RADIOBOX5, _("AO integral storage"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox138Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox138->SetSelection(0);
    itemBoxSizer128->Add(itemRadioBox138, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel127, _("MP2 Options"));

    wxPanel* itemPanel139 = new wxPanel( listBook, ID_IBSCBOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer140 = new wxBoxSizer(wxVERTICAL);
    itemPanel139->SetSizer(itemBoxSizer140);

    wxCheckBox* itemCheckBox141 = new wxCheckBox( itemPanel139, ID_CHECKBOX17, _("Direct SCF"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox141->SetValue(false);
    itemBoxSizer140->Add(itemCheckBox141, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox142 = new wxCheckBox( itemPanel139, ID_CHECKBOX18, _("Compute only change in Fock Matrix"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox142->SetValue(true);
    itemCheckBox142->Enable(false);
    itemBoxSizer140->Add(itemCheckBox142, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox143 = new wxCheckBox( itemPanel139, ID_CHECKBOX19, _("Generate UHF Natural Orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox143->SetValue(false);
    itemCheckBox143->Enable(false);
    itemBoxSizer140->Add(itemCheckBox143, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer144 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer140->Add(itemBoxSizer144, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText145 = new wxStaticText( itemPanel139, wxID_STATIC, _("SCF convergence criteria:  10^"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer144->Add(itemStaticText145, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxSpinCtrl* itemSpinCtrl146 = new wxSpinCtrl( itemPanel139, ID_SPINCTRL2, _T("5"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 2147483647, 5 );
    itemBoxSizer144->Add(itemSpinCtrl146, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel139, _("SCF Options"));

    wxPanel* itemPanel147 = new wxPanel( listBook, ID_IBSTATPOINTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer148 = new wxBoxSizer(wxVERTICAL);
    itemPanel147->SetSizer(itemBoxSizer148);

    wxFlexGridSizer* itemFlexGridSizer149 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer148->Add(itemFlexGridSizer149, 0, wxGROW, 5);
    wxStaticText* itemStaticText150 = new wxStaticText( itemPanel147, wxID_STATIC, _("Max. number of steps:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer149->Add(itemStaticText150, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl151 = new wxTextCtrl( itemPanel147, ID_TEXTCTRL16, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer149->Add(itemTextCtrl151, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText152 = new wxStaticText( itemPanel147, wxID_STATIC, _("Gradient convergance criteria:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer149->Add(itemStaticText152, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl153 = new wxTextCtrl( itemPanel147, ID_TEXTCTRL17, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer149->Add(itemTextCtrl153, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer154 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer148->Add(itemBoxSizer154, 0, wxGROW, 5);
    wxStaticText* itemStaticText155 = new wxStaticText( itemPanel147, wxID_STATIC, _("Optimization method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer154->Add(itemStaticText155, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice156Strings = NULL;
    wxUglyChoice* itemChoice156 = new wxUglyChoice( itemPanel147, ID_CHOICE23, wxDefaultPosition, wxDefaultSize, 0, itemChoice156Strings, 0 );
    itemBoxSizer154->Add(itemChoice156, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer157 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer148->Add(itemBoxSizer157, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer158 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer157->Add(itemFlexGridSizer158, 0, wxGROW, 5);
    wxStaticText* itemStaticText159 = new wxStaticText( itemPanel147, wxID_STATIC, _("Initial step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer158->Add(itemStaticText159, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl160 = new wxTextCtrl( itemPanel147, ID_TEXTCTRL18, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer158->Add(itemTextCtrl160, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText161 = new wxStaticText( itemPanel147, wxID_STATIC, _("Min. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer158->Add(itemStaticText161, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl162 = new wxTextCtrl( itemPanel147, ID_TEXTCTRL19, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer158->Add(itemTextCtrl162, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText163 = new wxStaticText( itemPanel147, wxID_STATIC, _("Max. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer158->Add(itemStaticText163, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl164 = new wxTextCtrl( itemPanel147, ID_TEXTCTRL20, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer158->Add(itemTextCtrl164, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer165 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer157->Add(itemBoxSizer165, 0, wxGROW, 5);
    wxCheckBox* itemCheckBox166 = new wxCheckBox( itemPanel147, ID_CHECKBOX25, _("Update Step Size"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox166->SetValue(false);
    itemBoxSizer165->Add(itemCheckBox166, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox167 = new wxCheckBox( itemPanel147, ID_CHECKBOX26, _("Stationary Pt."), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox167->SetValue(false);
    itemBoxSizer165->Add(itemCheckBox167, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer168 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer148->Add(itemBoxSizer168, 0, wxGROW, 5);
    wxString itemRadioBox169Strings[] = {
        _("Guess (+ definite)"),
        _("Read (from $HESS)"),
        _("Calculate")
    };
    wxRadioBox* itemRadioBox169 = new wxRadioBox( itemPanel147, ID_RADIOBOX3, _("Initial Hessian"), wxDefaultPosition, wxDefaultSize, 3, itemRadioBox169Strings, 1, wxRA_SPECIFY_COLS );
    itemRadioBox169->SetSelection(0);
    itemBoxSizer168->Add(itemRadioBox169, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText170 = new wxStaticText( itemPanel147, wxID_STATIC, _("Recalc. Hess every"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer168->Add(itemStaticText170, 0, wxALIGN_TOP|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    wxTextCtrl* itemTextCtrl171 = new wxTextCtrl( itemPanel147, ID_TEXTCTRL21, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer168->Add(itemTextCtrl171, 0, wxALIGN_TOP|wxALL, 5);

    wxStaticText* itemStaticText172 = new wxStaticText( itemPanel147, wxID_STATIC, _("steps."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer168->Add(itemStaticText172, 0, wxALIGN_TOP|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    wxCheckBox* itemCheckBox173 = new wxCheckBox( itemPanel147, ID_CHECKBOX24, _("Print Orbs at each iteration"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox173->SetValue(false);
    itemBoxSizer148->Add(itemCheckBox173, 0, wxGROW|wxALL, 5);

    listBook->AddPage(itemPanel147, _("Stat. Point"));

    wxPanel* itemPanel174 = new wxPanel( listBook, ID_SUMMARYPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

    listBook->AddPage(itemPanel174, _("Summary"));

    itemBoxSizer2->Add(listBook, 1, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer175 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer175, 0, wxGROW, 5);

    defaultsBtn = new wxButton( itemFrame1, ID_DEFAULTSBUTTON, _("Use Defaults"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer175->Add(defaultsBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    revertBtn = new wxButton( itemFrame1, ID_REVERTBUTTON, _("Revert"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer175->Add(revertBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    writeBtn = new wxButton( itemFrame1, ID_WRITEFILEBUTTON, _("Write File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer175->Add(writeBtn, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer179 = new wxStdDialogButtonSizer;

    itemBoxSizer175->Add(itemStdDialogButtonSizer179, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* itemButton180 = new wxButton( itemFrame1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer179->AddButton(itemButton180);

    wxButton* itemButton181 = new wxButton( itemFrame1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer179->AddButton(itemButton181);

    itemStdDialogButtonSizer179->Realize();

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
    
    // Enable/Disable ecpTypeChoice
    if(!(TmpInputRec->Basis->GetBasis() == 12 || TmpInputRec->Basis->GetBasis() == 13)) {
        ecpTypeChoice->Enable(false);
    }
    else {
        ecpTypeChoice->Enable(true);
    }
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
    runChoice->SetSelection(itemValue);
    
    if(scft == 0) {
        if(parent->GetData()->GetNumElectrons() & 1) {
            scft = 3;
        }
        else {
            scft = 1;
        }
    }
    scfChoice->SetSelection(scft - 1);

    // Enable/Disable mp2Check
    if(ci || cc || dft || (mp2 < 0)) {
        mp2Check->Enable(false);
    }
    else {
        mp2Check->Enable(true);
    }
    
    // Enable/Disable dftCheck
    if(ci || cc || (mp2 > 0) || (scft > 3)) {
        dftCheck->Enable(false);
    }
    else {
        dftCheck->Enable(true);
    }
    
    // Enable/Disable ciChoice
    if((mp2 > 0) || dft || cc || scft == 2) {
        ciChoice->Enable(false);
    }
    else {
        ciChoice->Enable(true);
    }
    
    // Enable/Disable ccChoice
    if((mp2 > 0) || dft || ci || scft > 1) {
        ccChoice->Enable(false);
    }
    else {
        ccChoice->Enable(true);
    }
    
    // Enable/Disable exeChoice
    if(TmpInputRec->Control->GetFriend()) {
        exeChoice->Enable(false);
    }
    else {
        exeChoice->Enable(true);
    }
}

void InputBuilderWindow::SetupDataItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
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
    TmpInputRec->Basis->SetPolar((GAMESS_BS_Polarization)(polarChoice->GetSelection()));
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
    TmpInputRec->Control->SetRunType((TypeOfRun)(runChoice->GetSelection()));
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
    TmpInputRec->Control->SetLocal((GAMESS_Localization)(localChoice->GetSelection()));
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

