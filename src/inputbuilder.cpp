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
    dftChoice = NULL;
    ciChoice = NULL;
    ccChoice = NULL;
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

    wxString* polarChoiceStrings = NULL;
    polarChoice = new wxUglyChoice( itemPanel4, ID_POLAR_CHOICE, wxDefaultPosition, wxDefaultSize, 0, polarChoiceStrings, 0 );
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

    dftChoice = new wxCheckBox( itemPanel24, ID_DFT_CHECKBOX, _("Use DFT"), wxDefaultPosition, wxDefaultSize, 0 );
    dftChoice->SetValue(false);
    itemStaticBoxSizer48->Add(dftChoice, 0, wxGROW|wxALL, 5);

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

    wxPanel* itemPanel91 = new wxPanel( listBook, ID_IBMOGUESSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer92 = new wxBoxSizer(wxVERTICAL);
    itemPanel91->SetSizer(itemBoxSizer92);

    wxBoxSizer* itemBoxSizer93 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer92->Add(itemBoxSizer93, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText94 = new wxStaticText( itemPanel91, wxID_STATIC, _("Initial Guess:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer93->Add(itemStaticText94, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString itemChoice95Strings[] = {
        _("Huckel"),
        _("HCore"),
        _("MO Read ($VEC)"),
        _("MO Saved (DICTNRY)"),
        _("Skip")
    };
    wxUglyChoice* itemChoice95 = new wxUglyChoice( itemPanel91, ID_CHOICE22, wxDefaultPosition, wxDefaultSize, 5, itemChoice95Strings, 0 );
    itemChoice95->SetStringSelection(_("Huckel"));
    itemBoxSizer93->Add(itemChoice95, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox96 = new wxCheckBox( itemPanel91, ID_CHECKBOX11, _("Print the initial guess"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox96->SetValue(false);
    itemBoxSizer92->Add(itemCheckBox96, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox97 = new wxCheckBox( itemPanel91, ID_CHECKBOX12, _("Rotate alpha and beta orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox97->SetValue(false);
    itemCheckBox97->Enable(false);
    itemBoxSizer92->Add(itemCheckBox97, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel91, _("MO Guess"));

    wxPanel* itemPanel98 = new wxPanel( listBook, ID_IBHESSOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer99 = new wxBoxSizer(wxVERTICAL);
    itemPanel98->SetSizer(itemBoxSizer99);

    wxBoxSizer* itemBoxSizer100 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer99->Add(itemBoxSizer100, 0, wxALIGN_LEFT, 5);
    wxString itemRadioBox101Strings[] = {
        _("Analytic"),
        _("Numeric")
    };
    wxRadioBox* itemRadioBox101 = new wxRadioBox( itemPanel98, ID_RADIOBOX2, _("Method"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox101Strings, 1, wxRA_SPECIFY_COLS );
    itemBoxSizer100->Add(itemRadioBox101, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer102 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer100->Add(itemBoxSizer102, 0, wxALIGN_TOP|wxALL, 5);
    wxStaticText* itemStaticText103 = new wxStaticText( itemPanel98, wxID_STATIC, _("Displacement size (bohrs):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer102->Add(itemStaticText103, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl104 = new wxTextCtrl( itemPanel98, ID_TEXTCTRL15, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer102->Add(itemTextCtrl104, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox105 = new wxCheckBox( itemPanel98, ID_CHECKBOX20, _("Double differenced hessian"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox105->SetValue(false);
    itemBoxSizer99->Add(itemCheckBox105, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox106 = new wxCheckBox( itemPanel98, ID_CHECKBOX21, _("Purify hessian"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox106->SetValue(false);
    itemBoxSizer99->Add(itemCheckBox106, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox107 = new wxCheckBox( itemPanel98, ID_CHECKBOX22, _("Print internal force constants"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox107->SetValue(false);
    itemBoxSizer99->Add(itemCheckBox107, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox108 = new wxCheckBox( itemPanel98, ID_CHECKBOX23, _("Vibrational Analysis"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox108->SetValue(false);
    itemBoxSizer99->Add(itemCheckBox108, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer109 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer99->Add(itemBoxSizer109, 0, wxGROW, 5);
    wxStaticText* itemStaticText110 = new wxStaticText( itemPanel98, wxID_STATIC, _("Frequency scale factor:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer109->Add(itemStaticText110, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl111 = new wxTextCtrl( itemPanel98, ID_TEXTCTRL14, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer109->Add(itemTextCtrl111, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel98, _("Hess. Options"));

    wxPanel* itemPanel112 = new wxPanel( listBook, ID_IBMISCPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer113 = new wxBoxSizer(wxVERTICAL);
    itemPanel112->SetSizer(itemBoxSizer113);

    wxStaticBox* itemStaticBoxSizer114Static = new wxStaticBox(itemPanel112, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer114 = new wxStaticBoxSizer(itemStaticBoxSizer114Static, wxVERTICAL);
    itemBoxSizer113->Add(itemStaticBoxSizer114, 0, wxALIGN_LEFT|wxALL, 5);
    wxCheckBox* itemCheckBox115 = new wxCheckBox( itemPanel112, ID_CHECKBOX13, _("MolPlt"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox115->SetValue(false);
    itemStaticBoxSizer114->Add(itemCheckBox115, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox116 = new wxCheckBox( itemPanel112, ID_CHECKBOX14, _("PltOrb"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox116->SetValue(false);
    itemStaticBoxSizer114->Add(itemCheckBox116, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox117 = new wxCheckBox( itemPanel112, ID_CHECKBOX15, _("Atoms In Molecules (AIMPAC)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox117->SetValue(false);
    itemStaticBoxSizer114->Add(itemCheckBox117, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox118 = new wxCheckBox( itemPanel112, ID_CHECKBOX16, _("RPAC"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox118->SetValue(false);
    itemStaticBoxSizer114->Add(itemCheckBox118, 0, wxALIGN_LEFT|wxALL, 5);

    wxString itemRadioBox119Strings[] = {
        _("None"),
        _("Hondo 8.2"),
        _("MELDF"),
        _("GAMESS (UK version)"),
        _("Gaussian 9x"),
        _("All")
    };
    wxRadioBox* itemRadioBox119 = new wxRadioBox( itemPanel112, ID_RADIOBOX1, _("Forces Check Run Type"), wxDefaultPosition, wxDefaultSize, 6, itemRadioBox119Strings, 1, wxRA_SPECIFY_COLS );
    itemBoxSizer113->Add(itemRadioBox119, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel112, _("Misc. Prefs"));

    wxPanel* itemPanel120 = new wxPanel( listBook, ID_IBSCBOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer121 = new wxBoxSizer(wxVERTICAL);
    itemPanel120->SetSizer(itemBoxSizer121);

    wxCheckBox* itemCheckBox122 = new wxCheckBox( itemPanel120, ID_CHECKBOX17, _("Direct SCF"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox122->SetValue(false);
    itemBoxSizer121->Add(itemCheckBox122, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox123 = new wxCheckBox( itemPanel120, ID_CHECKBOX18, _("Compute only change in Fock Matrix"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox123->SetValue(true);
    itemCheckBox123->Enable(false);
    itemBoxSizer121->Add(itemCheckBox123, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox124 = new wxCheckBox( itemPanel120, ID_CHECKBOX19, _("Generate UHF Natural Orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox124->SetValue(false);
    itemCheckBox124->Enable(false);
    itemBoxSizer121->Add(itemCheckBox124, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer125 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer121->Add(itemBoxSizer125, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText126 = new wxStaticText( itemPanel120, wxID_STATIC, _("SCF convergence criteria:  10^"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer125->Add(itemStaticText126, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxSpinCtrl* itemSpinCtrl127 = new wxSpinCtrl( itemPanel120, ID_SPINCTRL2, _T("5"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 2147483647, 5 );
    itemBoxSizer125->Add(itemSpinCtrl127, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel120, _("SCF Options"));

    wxPanel* itemPanel128 = new wxPanel( listBook, ID_IBSTATPOINTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer129 = new wxBoxSizer(wxVERTICAL);
    itemPanel128->SetSizer(itemBoxSizer129);

    wxFlexGridSizer* itemFlexGridSizer130 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer129->Add(itemFlexGridSizer130, 0, wxGROW, 5);
    wxStaticText* itemStaticText131 = new wxStaticText( itemPanel128, wxID_STATIC, _("Max. number of steps:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(itemStaticText131, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl132 = new wxTextCtrl( itemPanel128, ID_TEXTCTRL16, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(itemTextCtrl132, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText133 = new wxStaticText( itemPanel128, wxID_STATIC, _("Gradient convergance criteria:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(itemStaticText133, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl134 = new wxTextCtrl( itemPanel128, ID_TEXTCTRL17, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer130->Add(itemTextCtrl134, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer135 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer129->Add(itemBoxSizer135, 0, wxGROW, 5);
    wxStaticText* itemStaticText136 = new wxStaticText( itemPanel128, wxID_STATIC, _("Optimization method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer135->Add(itemStaticText136, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice137Strings = NULL;
    wxUglyChoice* itemChoice137 = new wxUglyChoice( itemPanel128, ID_CHOICE23, wxDefaultPosition, wxDefaultSize, 0, itemChoice137Strings, 0 );
    itemBoxSizer135->Add(itemChoice137, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer138 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer129->Add(itemBoxSizer138, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer139 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer138->Add(itemFlexGridSizer139, 0, wxGROW, 5);
    wxStaticText* itemStaticText140 = new wxStaticText( itemPanel128, wxID_STATIC, _("Initial step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer139->Add(itemStaticText140, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl141 = new wxTextCtrl( itemPanel128, ID_TEXTCTRL18, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer139->Add(itemTextCtrl141, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText142 = new wxStaticText( itemPanel128, wxID_STATIC, _("Min. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer139->Add(itemStaticText142, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl143 = new wxTextCtrl( itemPanel128, ID_TEXTCTRL19, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer139->Add(itemTextCtrl143, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText144 = new wxStaticText( itemPanel128, wxID_STATIC, _("Max. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer139->Add(itemStaticText144, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl145 = new wxTextCtrl( itemPanel128, ID_TEXTCTRL20, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer139->Add(itemTextCtrl145, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer146 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer138->Add(itemBoxSizer146, 0, wxGROW, 5);
    wxCheckBox* itemCheckBox147 = new wxCheckBox( itemPanel128, ID_CHECKBOX25, _("Update Step Size"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox147->SetValue(false);
    itemBoxSizer146->Add(itemCheckBox147, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox148 = new wxCheckBox( itemPanel128, ID_CHECKBOX26, _("Stationary Pt."), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox148->SetValue(false);
    itemBoxSizer146->Add(itemCheckBox148, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer149 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer129->Add(itemBoxSizer149, 0, wxGROW, 5);
    wxString itemRadioBox150Strings[] = {
        _("Guess (+ definite)"),
        _("Read (from $HESS)"),
        _("Calculate")
    };
    wxRadioBox* itemRadioBox150 = new wxRadioBox( itemPanel128, ID_RADIOBOX3, _("Radiobox"), wxDefaultPosition, wxDefaultSize, 3, itemRadioBox150Strings, 1, wxRA_SPECIFY_COLS );
    itemBoxSizer149->Add(itemRadioBox150, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText151 = new wxStaticText( itemPanel128, wxID_STATIC, _("Recalc. Hess every"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer149->Add(itemStaticText151, 0, wxALIGN_TOP|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    wxTextCtrl* itemTextCtrl152 = new wxTextCtrl( itemPanel128, ID_TEXTCTRL21, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer149->Add(itemTextCtrl152, 0, wxALIGN_TOP|wxALL, 5);

    wxStaticText* itemStaticText153 = new wxStaticText( itemPanel128, wxID_STATIC, _("steps."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer149->Add(itemStaticText153, 0, wxALIGN_TOP|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    wxCheckBox* itemCheckBox154 = new wxCheckBox( itemPanel128, ID_CHECKBOX24, _("Print Orbs at each iteration"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox154->SetValue(false);
    itemBoxSizer129->Add(itemCheckBox154, 0, wxGROW|wxALL, 5);

    listBook->AddPage(itemPanel128, _("Stat. Point"));

    itemBoxSizer2->Add(listBook, 1, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer155 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer155, 0, wxGROW, 5);

    wxButton* itemButton156 = new wxButton( itemFrame1, ID_BUTTON, _("Write File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer155->Add(itemButton156, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton157 = new wxButton( itemFrame1, ID_BUTTON1, _("Summary"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer155->Add(itemButton157, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer158 = new wxStdDialogButtonSizer;

    itemBoxSizer155->Add(itemStdDialogButtonSizer158, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* itemButton159 = new wxButton( itemFrame1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer158->AddButton(itemButton159);

    wxButton* itemButton160 = new wxButton( itemFrame1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer158->AddButton(itemButton160);

    itemStdDialogButtonSizer158->Realize();

////@end InputBuilderWindow content construction
    wxListView * t = listBook->GetListView();
	//t->SetWindowStyle(wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_NO_HEADER);
	t->SetWindowStyle(wxLC_LIST|wxLC_SINGLE_SEL);
    
    /* Make sure all tabs get displayed properly */
    wxWindow *tempPane = NULL;
    
    tempPane = listBook->GetPage(1);
    listBook->RemovePage(1);
    listBook->InsertPage(1, tempPane, _("Basis"));
    
    tempPane = listBook->GetPage(2);
    listBook->RemovePage(2);
    listBook->InsertPage(2, tempPane, _("Control"));
    
    tempPane = listBook->GetPage(3);
    listBook->RemovePage(3);
    listBook->InsertPage(3, tempPane, _("Data"));
    
    tempPane = listBook->GetPage(4);
    listBook->RemovePage(4);
    listBook->InsertPage(4, tempPane, _("System"));

    tempPane = NULL;
    
    /* Remove optional pages and store them for later */
    tabStatPoint = listBook->GetPage(8);
    listBook->RemovePage(8);
    tabSCFOpts   = listBook->GetPage(7);
    listBook->RemovePage(7);
    tabMiscPrefs = listBook->GetPage(6);
    listBook->RemovePage(6);
    tabHessOpts  = listBook->GetPage(5);
    listBook->RemovePage(5);
    tabMOGuess   = listBook->GetPage(4);
    listBook->RemovePage(4);
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
    //SetupItems();
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
    TmpInputRec->Basis->SetNumDFuncs(numDChoice->GetSelection() + 1);
    //SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_NUMF_CHOICE
 */

void InputBuilderWindow::OnNumfChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetNumFFuncs(numFChoice->GetSelection() + 1);
    //SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_NUMP_CHOICE
 */

void InputBuilderWindow::OnNumpChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetNumPFuncs(numPChoice->GetSelection() + 1);
    //SetupItems();
}
/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_RUN_CHOICE
 */

void InputBuilderWindow::OnRunChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetRunType((TypeOfRun)(runChoice->GetSelection() + 1));
    //SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_SCF_CHOICE
 */

void InputBuilderWindow::OnScfChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetSCFType((GAMESS_SCFType)(scfChoice->GetSelection() + 1));
    //SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_LOCAL_CHOICE
 */

void InputBuilderWindow::OnLocalChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetLocal((GAMESS_Localization)(localChoice->GetSelection() + 1));
    //SetupItems();
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
    //SetupItems();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DFT_CHECKBOX
 */

void InputBuilderWindow::OnDftCheckboxClick( wxCommandEvent& event )
{
    TmpInputRec->Control->UseDFT(event.IsChecked());
    //SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CI_CHOICE
 */

void InputBuilderWindow::OnCiChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetCIType((CIRunType)(ciChoice->GetSelection() + 1));
    //SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CC_CHOICE
 */

void InputBuilderWindow::OnCcChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetCCType((CCRunType)(ccChoice->GetSelection() + 1));
    //SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_EXE_CHOICE
 */

void InputBuilderWindow::OnExeChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetExeType(exeChoice->GetSelection() + 1);
}


