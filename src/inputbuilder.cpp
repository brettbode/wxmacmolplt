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
    tabMOGuess->Destroy();
    tabHessOpts->Destroy();
    tabMiscPrefs->Destroy();
    tabSCFOpts->Destroy();
    tabStatPoint->Destroy();
    
    delete TmpInputRec;
}

/*!
 * InputBuilder creator
 */

bool InputBuilderWindow::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin InputBuilderWindow member initialisation
    tabMOGuess = NULL;
    tabHessOpts = NULL;
    tabMiscPrefs = NULL;
    tabSCFOpts = NULL;
    tabStatPoint = NULL;
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
    exeChoice = NULL;
    mp2Check = NULL;
    dftCheck = NULL;
    ciChoice = NULL;
    ccChoice = NULL;
    aimpacCheck = NULL;
    rpacCheck = NULL;
////@end InputBuilderWindow member initialisation

////@begin InputBuilderWindow creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end InputBuilderWindow creation

    TmpInputRec = new InputData(((MolDisplayWin *)parent)->GetData()->GetInputData());

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

    wxTextCtrl* itemTextCtrl39 = new wxTextCtrl( itemPanel24, ID_MCHARGE_TEXTCTRL, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer37->Add(itemTextCtrl39, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText40 = new wxStaticText( itemPanel24, wxID_STATIC, _("Multiplicity:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer37->Add(itemStaticText40, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl41 = new wxTextCtrl( itemPanel24, ID_MULT_TEXTCTRL, _("1"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer37->Add(itemTextCtrl41, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

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

    wxTextCtrl* itemTextCtrl47 = new wxTextCtrl( itemPanel24, ID_MAXIT_TEXTCTRL, _("30"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer43->Add(itemTextCtrl47, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

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
    wxTextCtrl* itemTextCtrl59 = new wxTextCtrl( itemPanel56, ID_TITLE_TEXTCTRL, _("Title"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer58->Add(itemTextCtrl59, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer60Static = new wxStaticBox(itemPanel56, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer60 = new wxStaticBoxSizer(itemStaticBoxSizer60Static, wxVERTICAL);
    itemBoxSizer58->Add(itemStaticBoxSizer60, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer61 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer60->Add(itemFlexGridSizer61, 0, wxGROW, 5);
    wxStaticText* itemStaticText62 = new wxStaticText( itemPanel56, wxID_STATIC, _("Coord. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer61->Add(itemStaticText62, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString itemChoice63Strings[] = {
        _("Unique cartesian Coords."),
        _("Hilderbrant internals"),
        _("Cartesian coordinates"),
        _("Z-Matrix"),
        _("MOPAC Z-Matrix")
    };
    wxUglyChoice* itemChoice63 = new wxUglyChoice( itemPanel56, ID_COORD_CHOICE, wxDefaultPosition, wxDefaultSize, 5, itemChoice63Strings, 0 );
    itemChoice63->SetStringSelection(_("Unique cartesian Coords."));
    itemFlexGridSizer61->Add(itemChoice63, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText64 = new wxStaticText( itemPanel56, wxID_STATIC, _("Units:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer61->Add(itemStaticText64, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString itemChoice65Strings[] = {
        _("Angstroms"),
        _("Bohr")
    };
    wxUglyChoice* itemChoice65 = new wxUglyChoice( itemPanel56, ID_UNIT_CHOICE, wxDefaultPosition, wxDefaultSize, 2, itemChoice65Strings, 0 );
    itemChoice65->SetStringSelection(_("Angstroms"));
    itemFlexGridSizer61->Add(itemChoice65, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer66 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer60->Add(itemBoxSizer66, 0, wxGROW, 5);
    wxStaticText* itemStaticText67 = new wxStaticText( itemPanel56, wxID_STATIC, _("# of Z-Matrix Variables:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer66->Add(itemStaticText67, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl68 = new wxTextCtrl( itemPanel56, ID_TEXTCTRL10, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer66->Add(itemTextCtrl68, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer69Static = new wxStaticBox(itemPanel56, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer69 = new wxStaticBoxSizer(itemStaticBoxSizer69Static, wxVERTICAL);
    itemBoxSizer58->Add(itemStaticBoxSizer69, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer70 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer69->Add(itemFlexGridSizer70, 0, wxGROW, 5);
    wxStaticText* itemStaticText71 = new wxStaticText( itemPanel56, wxID_STATIC, _("Point Group:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer70->Add(itemStaticText71, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString* itemChoice72Strings = NULL;
    wxUglyChoice* itemChoice72 = new wxUglyChoice( itemPanel56, ID_POINTGROUP_CHOICE, wxDefaultPosition, wxDefaultSize, 0, itemChoice72Strings, 0 );
    itemFlexGridSizer70->Add(itemChoice72, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText73 = new wxStaticText( itemPanel56, wxID_STATIC, _("Order of Principle Axis:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText73->Enable(false);
    itemFlexGridSizer70->Add(itemStaticText73, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice74Strings = NULL;
    wxUglyChoice* itemChoice74 = new wxUglyChoice( itemPanel56, ID_ORDER_CHOICE, wxDefaultPosition, wxDefaultSize, 0, itemChoice74Strings, 0 );
    itemChoice74->Enable(false);
    itemFlexGridSizer70->Add(itemChoice74, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox75 = new wxCheckBox( itemPanel56, ID_SYMMETRY_CHECKBOX, _("Use Symmetry During Calculation"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox75->SetValue(true);
    itemStaticBoxSizer69->Add(itemCheckBox75, 0, wxALIGN_LEFT|wxALL, 5);

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
    itemBoxSizer120->Add(itemRadioBox126, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel119, _("Misc. Prefs"));

    wxPanel* itemPanel127 = new wxPanel( listBook, ID_IBMP2PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

    listBook->AddPage(itemPanel127, _("MP2 Options"));

    wxPanel* itemPanel128 = new wxPanel( listBook, ID_IBSCBOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer129 = new wxBoxSizer(wxVERTICAL);
    itemPanel128->SetSizer(itemBoxSizer129);

    wxCheckBox* itemCheckBox130 = new wxCheckBox( itemPanel128, ID_CHECKBOX17, _("Direct SCF"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox130->SetValue(false);
    itemBoxSizer129->Add(itemCheckBox130, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox131 = new wxCheckBox( itemPanel128, ID_CHECKBOX18, _("Compute only change in Fock Matrix"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox131->SetValue(true);
    itemCheckBox131->Enable(false);
    itemBoxSizer129->Add(itemCheckBox131, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox132 = new wxCheckBox( itemPanel128, ID_CHECKBOX19, _("Generate UHF Natural Orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox132->SetValue(false);
    itemCheckBox132->Enable(false);
    itemBoxSizer129->Add(itemCheckBox132, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer133 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer129->Add(itemBoxSizer133, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText134 = new wxStaticText( itemPanel128, wxID_STATIC, _("SCF convergence criteria:  10^"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer133->Add(itemStaticText134, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxSpinCtrl* itemSpinCtrl135 = new wxSpinCtrl( itemPanel128, ID_SPINCTRL2, _T("5"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 2147483647, 5 );
    itemBoxSizer133->Add(itemSpinCtrl135, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel128, _("SCF Options"));

    wxPanel* itemPanel136 = new wxPanel( listBook, ID_IBSTATPOINTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer137 = new wxBoxSizer(wxVERTICAL);
    itemPanel136->SetSizer(itemBoxSizer137);

    wxFlexGridSizer* itemFlexGridSizer138 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer137->Add(itemFlexGridSizer138, 0, wxGROW, 5);
    wxStaticText* itemStaticText139 = new wxStaticText( itemPanel136, wxID_STATIC, _("Max. number of steps:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemStaticText139, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl140 = new wxTextCtrl( itemPanel136, ID_TEXTCTRL16, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemTextCtrl140, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText141 = new wxStaticText( itemPanel136, wxID_STATIC, _("Gradient convergance criteria:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemStaticText141, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl142 = new wxTextCtrl( itemPanel136, ID_TEXTCTRL17, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemTextCtrl142, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer143 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer137->Add(itemBoxSizer143, 0, wxGROW, 5);
    wxStaticText* itemStaticText144 = new wxStaticText( itemPanel136, wxID_STATIC, _("Optimization method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer143->Add(itemStaticText144, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice145Strings = NULL;
    wxUglyChoice* itemChoice145 = new wxUglyChoice( itemPanel136, ID_CHOICE23, wxDefaultPosition, wxDefaultSize, 0, itemChoice145Strings, 0 );
    itemBoxSizer143->Add(itemChoice145, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer146 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer137->Add(itemBoxSizer146, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer147 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer146->Add(itemFlexGridSizer147, 0, wxGROW, 5);
    wxStaticText* itemStaticText148 = new wxStaticText( itemPanel136, wxID_STATIC, _("Initial step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer147->Add(itemStaticText148, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl149 = new wxTextCtrl( itemPanel136, ID_TEXTCTRL18, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer147->Add(itemTextCtrl149, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText150 = new wxStaticText( itemPanel136, wxID_STATIC, _("Min. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer147->Add(itemStaticText150, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl151 = new wxTextCtrl( itemPanel136, ID_TEXTCTRL19, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer147->Add(itemTextCtrl151, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText152 = new wxStaticText( itemPanel136, wxID_STATIC, _("Max. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer147->Add(itemStaticText152, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl153 = new wxTextCtrl( itemPanel136, ID_TEXTCTRL20, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer147->Add(itemTextCtrl153, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer154 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer146->Add(itemBoxSizer154, 0, wxGROW, 5);
    wxCheckBox* itemCheckBox155 = new wxCheckBox( itemPanel136, ID_CHECKBOX25, _("Update Step Size"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox155->SetValue(false);
    itemBoxSizer154->Add(itemCheckBox155, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox156 = new wxCheckBox( itemPanel136, ID_CHECKBOX26, _("Stationary Pt."), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox156->SetValue(false);
    itemBoxSizer154->Add(itemCheckBox156, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer157 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer137->Add(itemBoxSizer157, 0, wxGROW, 5);
    wxString itemRadioBox158Strings[] = {
        _("Guess (+ definite)"),
        _("Read (from $HESS)"),
        _("Calculate")
    };
    wxRadioBox* itemRadioBox158 = new wxRadioBox( itemPanel136, ID_RADIOBOX3, _("Initial Hessian"), wxDefaultPosition, wxDefaultSize, 3, itemRadioBox158Strings, 1, wxRA_SPECIFY_COLS );
    itemBoxSizer157->Add(itemRadioBox158, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText159 = new wxStaticText( itemPanel136, wxID_STATIC, _("Recalc. Hess every"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer157->Add(itemStaticText159, 0, wxALIGN_TOP|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    wxTextCtrl* itemTextCtrl160 = new wxTextCtrl( itemPanel136, ID_TEXTCTRL21, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer157->Add(itemTextCtrl160, 0, wxALIGN_TOP|wxALL, 5);

    wxStaticText* itemStaticText161 = new wxStaticText( itemPanel136, wxID_STATIC, _("steps."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer157->Add(itemStaticText161, 0, wxALIGN_TOP|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    wxCheckBox* itemCheckBox162 = new wxCheckBox( itemPanel136, ID_CHECKBOX24, _("Print Orbs at each iteration"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox162->SetValue(false);
    itemBoxSizer137->Add(itemCheckBox162, 0, wxGROW|wxALL, 5);

    listBook->AddPage(itemPanel136, _("Stat. Point"));

    itemBoxSizer2->Add(listBook, 1, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer163 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer163, 0, wxGROW, 5);

    wxButton* itemButton164 = new wxButton( itemFrame1, ID_BUTTON, _("Write File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer163->Add(itemButton164, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton165 = new wxButton( itemFrame1, ID_BUTTON1, _("Summary"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer163->Add(itemButton165, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer166 = new wxStdDialogButtonSizer;

    itemBoxSizer163->Add(itemStdDialogButtonSizer166, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* itemButton167 = new wxButton( itemFrame1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer166->AddButton(itemButton167);

    wxButton* itemButton168 = new wxButton( itemFrame1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer166->AddButton(itemButton168);

    itemStdDialogButtonSizer166->Realize();

////@end InputBuilderWindow content construction
    tabStatPoint = listBook->GetPage(10);
    listBook->RemovePage(10);
    
    tabSCFOpts = listBook->GetPage(9);
    listBook->RemovePage(9);
    
    tabMP2Opts = listBook->GetPage(8);
    listBook->RemovePage(8);
    
    tabMiscPrefs = listBook->GetPage(7);
    listBook->RemovePage(7);
    
    tabHessOpts  = listBook->GetPage(6);
    listBook->RemovePage(6);
    
    tabMOGuess = listBook->GetPage(5);
    listBook->RemovePage(5);
        
    tabDFT = listBook->GetPage(4);
    listBook->RemovePage(4);
    
    tabSystem = listBook->GetPage(3);
    listBook->RemovePage(3);
    
    tabData = listBook->GetPage(2);
    listBook->RemovePage(2);
    
    tabControl = listBook->GetPage(1);
    listBook->RemovePage(1);
    
    tabBasis = listBook->GetPage(0);
    listBook->RemovePage(0);
    
    wxListView * t = listBook->GetListView();
	//t->SetWindowStyle(wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_NO_HEADER);
	t->SetWindowStyle(wxLC_LIST|wxLC_SINGLE_SEL);
    
    listBook->AddPage(tabBasis, _("Basis"));
    listBook->AddPage(tabControl, _("Control"));
    listBook->AddPage(tabData, _("Data"));
    listBook->AddPage(tabSystem, _("System"));
    listBook->AddPage(tabDFT, _("DFT"));
    listBook->AddPage(tabMOGuess, _("MO Guess"));
    listBook->AddPage(tabHessOpts, _("Hess. Options"));
    listBook->AddPage(tabMiscPrefs, _("Misc. Prefs"));
    listBook->AddPage(tabMP2Opts, _("MP2 Options"));
    listBook->AddPage(tabSCFOpts, _("SCF Options"));
    listBook->AddPage(tabStatPoint, _("Stat. Point"));
    
    listBook->SetSelection(1);
    
    //listBook->RemovePage(10);
    //listBook->RemovePage(9);
    //listBook->RemovePage(8);
    //listBook->RemovePage(7);
    //listBook->RemovePage(6);
    //listBook->RemovePage(5);
    //listBook->RemovePage(4);
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
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    
    short mp2 = TmpInputRec->Control->GetMPLevel();
    bool dft = TmpInputRec->Control->UseDFT();
    short ci = TmpInputRec->Control->GetCIType();
    CCRunType cc = TmpInputRec->Control->GetCCType();
    long scft = TmpInputRec->Control->GetSCFType();
    
    if(scft == 0) {
        if(parent->GetData()->GetNumElectrons() & 1) {
            scft = 3;
        }
        else {
            scft = 1;
        }
    }

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
    if(!TmpInputRec->Control->GetFriend()) {
        exeChoice->Enable(false);
    }
    else {
        exeChoice->Enable(true);
    }

    // Enable/Disable ecpTypeChoice
    if(!(TmpInputRec->Basis->GetBasis() == 12 || TmpInputRec->Basis->GetBasis() == 13)) {
        ecpTypeChoice->Enable(false);
    }
    else {
        ecpTypeChoice->Enable(true);
    }
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
    else if(itemValue < 19) {
        BasisValue = itemValue - 6;
        if(itemValue == 12) NumGauss = 6;
    }
    else if(itemValue < 21) {
        BasisValue = itemValue - 7;
    }
    else {
        BasisValue = itemValue - 8;
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
    TmpInputRec->Control->SetSCFType((GAMESS_SCFType)(scfChoice->GetSelection()));
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


