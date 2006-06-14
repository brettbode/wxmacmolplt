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
////@end InputBuilderWindow member initialisation

////@begin InputBuilderWindow creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end InputBuilderWindow creation
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

    wxString* itemChoice8Strings = NULL;
    wxChoice* itemChoice8 = new wxChoice( itemPanel4, ID_CHOICE3, wxDefaultPosition, wxDefaultSize, 0, itemChoice8Strings, 0 );
    itemBoxSizer6->Add(itemChoice8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemBoxSizer6->Add(5, 5, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText10 = new wxStaticText( itemPanel4, wxID_STATIC, _("ECP Type:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    itemBoxSizer6->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxString* itemChoice11Strings = NULL;
    wxChoice* itemChoice11 = new wxChoice( itemPanel4, ID_CHOICE4, wxDefaultPosition, wxDefaultSize, 0, itemChoice11Strings, 0 );
    itemBoxSizer6->Add(itemChoice11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer12 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer5->Add(itemFlexGridSizer12, 0, wxGROW, 5);
    wxStaticText* itemStaticText13 = new wxStaticText( itemPanel4, wxID_STATIC, _("# D heavy atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer12->Add(itemStaticText13, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString* itemChoice14Strings = NULL;
    wxChoice* itemChoice14 = new wxChoice( itemPanel4, ID_CHOICE5, wxDefaultPosition, wxDefaultSize, 0, itemChoice14Strings, 0 );
    itemFlexGridSizer12->Add(itemChoice14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText15 = new wxStaticText( itemPanel4, wxID_STATIC, _("# F heavy atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer12->Add(itemStaticText15, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString* itemChoice16Strings = NULL;
    wxChoice* itemChoice16 = new wxChoice( itemPanel4, ID_CHOICE6, wxDefaultPosition, wxDefaultSize, 0, itemChoice16Strings, 0 );
    itemFlexGridSizer12->Add(itemChoice16, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText17 = new wxStaticText( itemPanel4, wxID_STATIC, _("# light atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer12->Add(itemStaticText17, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice18Strings = NULL;
    wxChoice* itemChoice18 = new wxChoice( itemPanel4, ID_CHOICE7, wxDefaultPosition, wxDefaultSize, 0, itemChoice18Strings, 0 );
    itemFlexGridSizer12->Add(itemChoice18, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer5->Add(itemBoxSizer19, 0, wxGROW, 5);
    wxStaticText* itemStaticText20 = new wxStaticText( itemPanel4, wxID_STATIC, _("Polar:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemStaticText20, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice21Strings = NULL;
    wxChoice* itemChoice21 = new wxChoice( itemPanel4, ID_CHOICE8, wxDefaultPosition, wxDefaultSize, 0, itemChoice21Strings, 0 );
    itemBoxSizer19->Add(itemChoice21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox22 = new wxCheckBox( itemPanel4, ID_CHECKBOX3, _("Diffuse L-shell on heavy atoms"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox22->SetValue(false);
    itemBoxSizer5->Add(itemCheckBox22, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox23 = new wxCheckBox( itemPanel4, ID_CHECKBOX4, _("Diffuse s-shell on hydrogens"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox23->SetValue(false);
    itemBoxSizer5->Add(itemCheckBox23, 0, wxGROW|wxALL, 5);

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

    wxString* itemChoice29Strings = NULL;
    wxChoice* itemChoice29 = new wxChoice( itemPanel24, ID_CHOICE9, wxDefaultPosition, wxDefaultSize, 0, itemChoice29Strings, 0 );
    itemFlexGridSizer27->Add(itemChoice29, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText30 = new wxStaticText( itemPanel24, wxID_STATIC, _("SCF Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer27->Add(itemStaticText30, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString* itemChoice31Strings = NULL;
    wxChoice* itemChoice31 = new wxChoice( itemPanel24, ID_CHOICE10, wxDefaultPosition, wxDefaultSize, 0, itemChoice31Strings, 0 );
    itemFlexGridSizer27->Add(itemChoice31, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText32 = new wxStaticText( itemPanel24, wxID_STATIC, _("Localization Method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer27->Add(itemStaticText32, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice33Strings = NULL;
    wxChoice* itemChoice33 = new wxChoice( itemPanel24, ID_CHOICE11, wxDefaultPosition, wxDefaultSize, 0, itemChoice33Strings, 0 );
    itemFlexGridSizer27->Add(itemChoice33, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

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

    wxTextCtrl* itemTextCtrl39 = new wxTextCtrl( itemPanel24, ID_TEXTCTRL5, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer37->Add(itemTextCtrl39, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText40 = new wxStaticText( itemPanel24, wxID_STATIC, _("Multiplicity:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer37->Add(itemStaticText40, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl41 = new wxTextCtrl( itemPanel24, ID_TEXTCTRL7, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer37->Add(itemTextCtrl41, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer42Static = new wxStaticBox(itemPanel24, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer42 = new wxStaticBoxSizer(itemStaticBoxSizer42Static, wxVERTICAL);
    itemBoxSizer35->Add(itemStaticBoxSizer42, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer43 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer42->Add(itemFlexGridSizer43, 0, wxGROW, 5);
    wxStaticText* itemStaticText44 = new wxStaticText( itemPanel24, wxID_STATIC, _("Exe. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer43->Add(itemStaticText44, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString* itemChoice45Strings = NULL;
    wxChoice* itemChoice45 = new wxChoice( itemPanel24, ID_CHOICE12, wxDefaultPosition, wxDefaultSize, 0, itemChoice45Strings, 0 );
    itemFlexGridSizer43->Add(itemChoice45, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText46 = new wxStaticText( itemPanel24, wxID_STATIC, _("Max # SCF Iterations:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer43->Add(itemStaticText46, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl47 = new wxTextCtrl( itemPanel24, ID_TEXTCTRL8, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer43->Add(itemTextCtrl47, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer48Static = new wxStaticBox(itemPanel24, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer48 = new wxStaticBoxSizer(itemStaticBoxSizer48Static, wxVERTICAL);
    itemBoxSizer34->Add(itemStaticBoxSizer48, 0, wxALIGN_TOP|wxALL, 5);
    wxCheckBox* itemCheckBox49 = new wxCheckBox( itemPanel24, ID_CHECKBOX6, _("Use MP2"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox49->SetValue(false);
    itemStaticBoxSizer48->Add(itemCheckBox49, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox50 = new wxCheckBox( itemPanel24, ID_CHECKBOX7, _("Use DFT"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox50->SetValue(false);
    itemStaticBoxSizer48->Add(itemCheckBox50, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer51 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer48->Add(itemFlexGridSizer51, 0, wxGROW, 5);
    wxStaticText* itemStaticText52 = new wxStaticText( itemPanel24, wxID_STATIC, _("CI:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer51->Add(itemStaticText52, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString* itemChoice53Strings = NULL;
    wxChoice* itemChoice53 = new wxChoice( itemPanel24, ID_CHOICE13, wxDefaultPosition, wxDefaultSize, 0, itemChoice53Strings, 0 );
    itemFlexGridSizer51->Add(itemChoice53, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText54 = new wxStaticText( itemPanel24, wxID_STATIC, _("CC:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer51->Add(itemStaticText54, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice55Strings = NULL;
    wxChoice* itemChoice55 = new wxChoice( itemPanel24, ID_CHOICE14, wxDefaultPosition, wxDefaultSize, 0, itemChoice55Strings, 0 );
    itemFlexGridSizer51->Add(itemChoice55, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel24, _("Control"));

    wxPanel* itemPanel56 = new wxPanel( listBook, ID_IBDATAPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer57 = new wxBoxSizer(wxVERTICAL);
    itemPanel56->SetSizer(itemBoxSizer57);

    wxTextCtrl* itemTextCtrl58 = new wxTextCtrl( itemPanel56, ID_TEXTCTRL9, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer57->Add(itemTextCtrl58, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer59Static = new wxStaticBox(itemPanel56, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer59 = new wxStaticBoxSizer(itemStaticBoxSizer59Static, wxVERTICAL);
    itemBoxSizer57->Add(itemStaticBoxSizer59, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer60 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer59->Add(itemFlexGridSizer60, 0, wxGROW, 5);
    wxStaticText* itemStaticText61 = new wxStaticText( itemPanel56, wxID_STATIC, _("Coord. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer60->Add(itemStaticText61, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString* itemChoice62Strings = NULL;
    wxChoice* itemChoice62 = new wxChoice( itemPanel56, ID_CHOICE15, wxDefaultPosition, wxDefaultSize, 0, itemChoice62Strings, 0 );
    itemFlexGridSizer60->Add(itemChoice62, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText63 = new wxStaticText( itemPanel56, wxID_STATIC, _("Units:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer60->Add(itemStaticText63, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice64Strings = NULL;
    wxChoice* itemChoice64 = new wxChoice( itemPanel56, ID_CHOICE16, wxDefaultPosition, wxDefaultSize, 0, itemChoice64Strings, 0 );
    itemFlexGridSizer60->Add(itemChoice64, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer65 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer59->Add(itemBoxSizer65, 0, wxGROW, 5);
    wxStaticText* itemStaticText66 = new wxStaticText( itemPanel56, wxID_STATIC, _("# of Z-Matrix Variables:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer65->Add(itemStaticText66, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl67 = new wxTextCtrl( itemPanel56, ID_TEXTCTRL10, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer65->Add(itemTextCtrl67, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer68Static = new wxStaticBox(itemPanel56, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer68 = new wxStaticBoxSizer(itemStaticBoxSizer68Static, wxVERTICAL);
    itemBoxSizer57->Add(itemStaticBoxSizer68, 0, wxALIGN_LEFT|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer69 = new wxFlexGridSizer(0, 2, 0, 0);
    itemStaticBoxSizer68->Add(itemFlexGridSizer69, 0, wxGROW, 5);
    wxStaticText* itemStaticText70 = new wxStaticText( itemPanel56, wxID_STATIC, _("Point Group:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer69->Add(itemStaticText70, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxString* itemChoice71Strings = NULL;
    wxChoice* itemChoice71 = new wxChoice( itemPanel56, ID_CHOICE17, wxDefaultPosition, wxDefaultSize, 0, itemChoice71Strings, 0 );
    itemFlexGridSizer69->Add(itemChoice71, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText72 = new wxStaticText( itemPanel56, wxID_STATIC, _("Order of Principle Axis:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer69->Add(itemStaticText72, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice73Strings = NULL;
    wxChoice* itemChoice73 = new wxChoice( itemPanel56, ID_CHOICE18, wxDefaultPosition, wxDefaultSize, 0, itemChoice73Strings, 0 );
    itemFlexGridSizer69->Add(itemChoice73, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox74 = new wxCheckBox( itemPanel56, ID_CHECKBOX8, _("Use Symmetry During Calculation"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox74->SetValue(false);
    itemStaticBoxSizer68->Add(itemCheckBox74, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel56, _("Data"));

    wxPanel* itemPanel75 = new wxPanel( listBook, ID_IBSYSTEMPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer76 = new wxBoxSizer(wxVERTICAL);
    itemPanel75->SetSizer(itemBoxSizer76);

    wxFlexGridSizer* itemFlexGridSizer77 = new wxFlexGridSizer(0, 3, 0, 0);
    itemBoxSizer76->Add(itemFlexGridSizer77, 0, wxGROW, 5);
    wxStaticText* itemStaticText78 = new wxStaticText( itemPanel75, wxID_STATIC, _("Time Limit:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer77->Add(itemStaticText78, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl79 = new wxTextCtrl( itemPanel75, ID_TEXTCTRL11, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer77->Add(itemTextCtrl79, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

    wxString* itemChoice80Strings = NULL;
    wxChoice* itemChoice80 = new wxChoice( itemPanel75, ID_CHOICE19, wxDefaultPosition, wxDefaultSize, 0, itemChoice80Strings, 0 );
    itemFlexGridSizer77->Add(itemChoice80, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText81 = new wxStaticText( itemPanel75, wxID_STATIC, _("Memory:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer77->Add(itemStaticText81, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl82 = new wxTextCtrl( itemPanel75, ID_TEXTCTRL12, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer77->Add(itemTextCtrl82, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxString* itemChoice83Strings = NULL;
    wxChoice* itemChoice83 = new wxChoice( itemPanel75, ID_CHOICE20, wxDefaultPosition, wxDefaultSize, 0, itemChoice83Strings, 0 );
    itemFlexGridSizer77->Add(itemChoice83, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer76->Add(itemBoxSizer84, 0, wxGROW, 5);
    wxStaticText* itemStaticText85 = new wxStaticText( itemPanel75, wxID_STATIC, _("Diagonalization Method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer84->Add(itemStaticText85, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice86Strings = NULL;
    wxChoice* itemChoice86 = new wxChoice( itemPanel75, ID_CHOICE21, wxDefaultPosition, wxDefaultSize, 0, itemChoice86Strings, 0 );
    itemBoxSizer84->Add(itemChoice86, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox87 = new wxCheckBox( itemPanel75, ID_CHECKBOX9, _("Produce \"core\" file upon abort"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox87->SetValue(false);
    itemBoxSizer76->Add(itemCheckBox87, 0, wxALIGN_LEFT|wxALL, 5);

    wxString itemRadioBox88Strings[] = {
        _("Loop"),
        _("Next Value")
    };
    wxRadioBox* itemRadioBox88 = new wxRadioBox( itemPanel75, ID_RADIOBOX, _("Parallel Load Balance Type"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox88Strings, 1, wxRA_SPECIFY_COLS );
    itemBoxSizer76->Add(itemRadioBox88, 0, wxALIGN_LEFT|wxALL, 5);

    wxCheckBox* itemCheckBox89 = new wxCheckBox( itemPanel75, ID_CHECKBOX10, _("Use External Data Representation for messages"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox89->SetValue(false);
    itemBoxSizer76->Add(itemCheckBox89, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel75, _("System"));

    wxPanel* itemPanel90 = new wxPanel( listBook, ID_IBMOGUESSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer91 = new wxBoxSizer(wxVERTICAL);
    itemPanel90->SetSizer(itemBoxSizer91);

    wxBoxSizer* itemBoxSizer92 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer91->Add(itemBoxSizer92, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText93 = new wxStaticText( itemPanel90, wxID_STATIC, _("Initial Guess:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer92->Add(itemStaticText93, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice94Strings = NULL;
    wxChoice* itemChoice94 = new wxChoice( itemPanel90, ID_CHOICE22, wxDefaultPosition, wxDefaultSize, 0, itemChoice94Strings, 0 );
    itemBoxSizer92->Add(itemChoice94, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox95 = new wxCheckBox( itemPanel90, ID_CHECKBOX11, _("Print the initial guess"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox95->SetValue(false);
    itemBoxSizer91->Add(itemCheckBox95, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox96 = new wxCheckBox( itemPanel90, ID_CHECKBOX12, _("Rotate alpha and beta orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox96->SetValue(false);
    itemBoxSizer91->Add(itemCheckBox96, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel90, _("MO Guess"));

    wxPanel* itemPanel97 = new wxPanel( listBook, ID_IBHESSOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer98 = new wxBoxSizer(wxVERTICAL);
    itemPanel97->SetSizer(itemBoxSizer98);

    wxBoxSizer* itemBoxSizer99 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer98->Add(itemBoxSizer99, 0, wxALIGN_LEFT, 5);
    wxString itemRadioBox100Strings[] = {
        _("Analytic"),
        _("Numeric")
    };
    wxRadioBox* itemRadioBox100 = new wxRadioBox( itemPanel97, ID_RADIOBOX2, _("Method"), wxDefaultPosition, wxDefaultSize, 2, itemRadioBox100Strings, 1, wxRA_SPECIFY_COLS );
    itemBoxSizer99->Add(itemRadioBox100, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer101 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer99->Add(itemBoxSizer101, 0, wxALIGN_TOP|wxALL, 5);
    wxStaticText* itemStaticText102 = new wxStaticText( itemPanel97, wxID_STATIC, _("Displacement size (bohrs):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer101->Add(itemStaticText102, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl103 = new wxTextCtrl( itemPanel97, ID_TEXTCTRL15, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer101->Add(itemTextCtrl103, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxCheckBox* itemCheckBox104 = new wxCheckBox( itemPanel97, ID_CHECKBOX20, _("Double differenced hessian"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox104->SetValue(false);
    itemBoxSizer98->Add(itemCheckBox104, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox105 = new wxCheckBox( itemPanel97, ID_CHECKBOX21, _("Purify hessian"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox105->SetValue(false);
    itemBoxSizer98->Add(itemCheckBox105, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox106 = new wxCheckBox( itemPanel97, ID_CHECKBOX22, _("Print internal force constants"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox106->SetValue(false);
    itemBoxSizer98->Add(itemCheckBox106, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox107 = new wxCheckBox( itemPanel97, ID_CHECKBOX23, _("Vibrational Analysis"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox107->SetValue(false);
    itemBoxSizer98->Add(itemCheckBox107, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer108 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer98->Add(itemBoxSizer108, 0, wxGROW, 5);
    wxStaticText* itemStaticText109 = new wxStaticText( itemPanel97, wxID_STATIC, _("Frequency scale factor:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer108->Add(itemStaticText109, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl110 = new wxTextCtrl( itemPanel97, ID_TEXTCTRL14, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer108->Add(itemTextCtrl110, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook->AddPage(itemPanel97, _("Hess. Options"));

    wxPanel* itemPanel111 = new wxPanel( listBook, ID_IBMISCPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer112 = new wxBoxSizer(wxVERTICAL);
    itemPanel111->SetSizer(itemBoxSizer112);

    wxStaticBox* itemStaticBoxSizer113Static = new wxStaticBox(itemPanel111, wxID_ANY, _T(""));
    wxStaticBoxSizer* itemStaticBoxSizer113 = new wxStaticBoxSizer(itemStaticBoxSizer113Static, wxVERTICAL);
    itemBoxSizer112->Add(itemStaticBoxSizer113, 0, wxALIGN_LEFT|wxALL, 5);
    wxCheckBox* itemCheckBox114 = new wxCheckBox( itemPanel111, ID_CHECKBOX13, _("MolPlt"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox114->SetValue(false);
    itemStaticBoxSizer113->Add(itemCheckBox114, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox115 = new wxCheckBox( itemPanel111, ID_CHECKBOX14, _("PltOrb"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox115->SetValue(false);
    itemStaticBoxSizer113->Add(itemCheckBox115, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox116 = new wxCheckBox( itemPanel111, ID_CHECKBOX15, _("Atoms In Molecules (AIMPAC)"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox116->SetValue(false);
    itemStaticBoxSizer113->Add(itemCheckBox116, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox117 = new wxCheckBox( itemPanel111, ID_CHECKBOX16, _("RPAC"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox117->SetValue(false);
    itemStaticBoxSizer113->Add(itemCheckBox117, 0, wxALIGN_LEFT|wxALL, 5);

    wxString itemRadioBox118Strings[] = {
        _("None"),
        _("Hondo 8.2"),
        _("MELDF"),
        _("GAMESS (UK version)"),
        _("Gaussian 9x"),
        _("All")
    };
    wxRadioBox* itemRadioBox118 = new wxRadioBox( itemPanel111, ID_RADIOBOX1, _("Forces Check Run Type"), wxDefaultPosition, wxDefaultSize, 6, itemRadioBox118Strings, 1, wxRA_SPECIFY_COLS );
    itemBoxSizer112->Add(itemRadioBox118, 0, wxALIGN_LEFT|wxALL, 5);

    listBook->AddPage(itemPanel111, _("Misc. Prefs"));

    wxPanel* itemPanel119 = new wxPanel( listBook, ID_IBSCBOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer120 = new wxBoxSizer(wxVERTICAL);
    itemPanel119->SetSizer(itemBoxSizer120);

    wxCheckBox* itemCheckBox121 = new wxCheckBox( itemPanel119, ID_CHECKBOX17, _("Direct SCF"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox121->SetValue(false);
    itemBoxSizer120->Add(itemCheckBox121, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox122 = new wxCheckBox( itemPanel119, ID_CHECKBOX18, _("Compute only change in Fock Matrix"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox122->SetValue(false);
    itemBoxSizer120->Add(itemCheckBox122, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox123 = new wxCheckBox( itemPanel119, ID_CHECKBOX19, _("Generate UHF Natural Orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox123->SetValue(false);
    itemBoxSizer120->Add(itemCheckBox123, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxBoxSizer* itemBoxSizer124 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer120->Add(itemBoxSizer124, 0, wxALIGN_LEFT, 5);
    wxStaticText* itemStaticText125 = new wxStaticText( itemPanel119, wxID_STATIC, _("SCF convergence criteria:  10^"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer124->Add(itemStaticText125, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl126 = new wxTextCtrl( itemPanel119, ID_TEXTCTRL13, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer124->Add(itemTextCtrl126, 0, wxALIGN_TOP|wxRIGHT|wxTOP|wxBOTTOM, 5);

    listBook->AddPage(itemPanel119, _("SCF Options"));

    wxPanel* itemPanel127 = new wxPanel( listBook, ID_IBSTATPOINTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer128 = new wxBoxSizer(wxVERTICAL);
    itemPanel127->SetSizer(itemBoxSizer128);

    wxFlexGridSizer* itemFlexGridSizer129 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer128->Add(itemFlexGridSizer129, 0, wxGROW, 5);
    wxStaticText* itemStaticText130 = new wxStaticText( itemPanel127, wxID_STATIC, _("Max. number of steps:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer129->Add(itemStaticText130, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl131 = new wxTextCtrl( itemPanel127, ID_TEXTCTRL16, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer129->Add(itemTextCtrl131, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText132 = new wxStaticText( itemPanel127, wxID_STATIC, _("Gradient convergance criteria:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer129->Add(itemStaticText132, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl133 = new wxTextCtrl( itemPanel127, ID_TEXTCTRL17, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer129->Add(itemTextCtrl133, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer134 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer128->Add(itemBoxSizer134, 0, wxGROW, 5);
    wxStaticText* itemStaticText135 = new wxStaticText( itemPanel127, wxID_STATIC, _("Optimization method:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer134->Add(itemStaticText135, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxString* itemChoice136Strings = NULL;
    wxChoice* itemChoice136 = new wxChoice( itemPanel127, ID_CHOICE23, wxDefaultPosition, wxDefaultSize, 0, itemChoice136Strings, 0 );
    itemBoxSizer134->Add(itemChoice136, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer137 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer128->Add(itemBoxSizer137, 0, wxGROW|wxALL, 5);
    wxFlexGridSizer* itemFlexGridSizer138 = new wxFlexGridSizer(0, 2, 0, 0);
    itemBoxSizer137->Add(itemFlexGridSizer138, 0, wxGROW, 5);
    wxStaticText* itemStaticText139 = new wxStaticText( itemPanel127, wxID_STATIC, _("Initial step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemStaticText139, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl140 = new wxTextCtrl( itemPanel127, ID_TEXTCTRL18, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemTextCtrl140, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText141 = new wxStaticText( itemPanel127, wxID_STATIC, _("Min. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemStaticText141, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl142 = new wxTextCtrl( itemPanel127, ID_TEXTCTRL19, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemTextCtrl142, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

    wxStaticText* itemStaticText143 = new wxStaticText( itemPanel127, wxID_STATIC, _("Max. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemStaticText143, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl144 = new wxTextCtrl( itemPanel127, ID_TEXTCTRL20, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer138->Add(itemTextCtrl144, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer145 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer137->Add(itemBoxSizer145, 0, wxGROW, 5);
    wxCheckBox* itemCheckBox146 = new wxCheckBox( itemPanel127, ID_CHECKBOX25, _("Update Step Size"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox146->SetValue(false);
    itemBoxSizer145->Add(itemCheckBox146, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

    wxCheckBox* itemCheckBox147 = new wxCheckBox( itemPanel127, ID_CHECKBOX26, _("Stationary Pt."), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox147->SetValue(false);
    itemBoxSizer145->Add(itemCheckBox147, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer148 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer128->Add(itemBoxSizer148, 0, wxGROW, 5);
    wxString itemRadioBox149Strings[] = {
        _("Guess (+ definite)"),
        _("Read (from $HESS)"),
        _("Calculate")
    };
    wxRadioBox* itemRadioBox149 = new wxRadioBox( itemPanel127, ID_RADIOBOX3, _("Radiobox"), wxDefaultPosition, wxDefaultSize, 3, itemRadioBox149Strings, 1, wxRA_SPECIFY_COLS );
    itemBoxSizer148->Add(itemRadioBox149, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText150 = new wxStaticText( itemPanel127, wxID_STATIC, _("Recalc. Hess every"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer148->Add(itemStaticText150, 0, wxALIGN_TOP|wxLEFT|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    wxTextCtrl* itemTextCtrl151 = new wxTextCtrl( itemPanel127, ID_TEXTCTRL21, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer148->Add(itemTextCtrl151, 0, wxALIGN_TOP|wxALL, 5);

    wxStaticText* itemStaticText152 = new wxStaticText( itemPanel127, wxID_STATIC, _("steps."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer148->Add(itemStaticText152, 0, wxALIGN_TOP|wxTOP|wxBOTTOM|wxADJUST_MINSIZE, 10);

    wxCheckBox* itemCheckBox153 = new wxCheckBox( itemPanel127, ID_CHECKBOX24, _("Print Orbs at each iteration"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox153->SetValue(false);
    itemBoxSizer128->Add(itemCheckBox153, 0, wxGROW|wxALL, 5);

    listBook->AddPage(itemPanel127, _("Stat. Point"));

    itemBoxSizer2->Add(listBook, 1, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer154 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer154, 0, wxGROW, 5);

    wxButton* itemButton155 = new wxButton( itemFrame1, ID_BUTTON, _("Write File"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer154->Add(itemButton155, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

    wxButton* itemButton156 = new wxButton( itemFrame1, ID_BUTTON1, _("Summary"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer154->Add(itemButton156, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer157 = new wxStdDialogButtonSizer;

    itemBoxSizer154->Add(itemStdDialogButtonSizer157, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* itemButton158 = new wxButton( itemFrame1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer157->AddButton(itemButton158);

    wxButton* itemButton159 = new wxButton( itemFrame1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer157->AddButton(itemButton159);

    itemStdDialogButtonSizer157->Realize();

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


