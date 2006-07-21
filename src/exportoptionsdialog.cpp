/////////////////////////////////////////////////////////////////////////////
// Name:        exportoptionsdialog.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Thu 11 May 2006 02:46:50 PM CDT
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), Thu 11 May 2006 02:46:50 PM CDT

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "exportoptionsdialog.h"
#endif
#include "Globals.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes
#include "MolDisplayWin.h"

#include "exportoptionsdialog.h"

////@begin XPM images
////@end XPM images

/*!
 * ExportOptionsDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ExportOptionsDialog, wxDialog )

/*!
 * ExportOptionsDialog event table definition
 */

BEGIN_EVENT_TABLE( ExportOptionsDialog, wxDialog )

////@begin ExportOptionsDialog event table entries
    EVT_CHOICE( ID_RES_CHOICE, ExportOptionsDialog::OnResChoiceSelected )

    EVT_SPINCTRL( ID_RESWIDTHSPIN, ExportOptionsDialog::OnReswidthspinUpdated )

    EVT_SPINCTRL( ID_RESHEIGHTSPIN, ExportOptionsDialog::OnResheightspinUpdated )

    EVT_BUTTON( wxID_OK, ExportOptionsDialog::OnOkClick )

////@end ExportOptionsDialog event table entries

END_EVENT_TABLE()

/*!
 * ExportOptionsDialog constructors
 */

ExportOptionsDialog::ExportOptionsDialog( )
{
}

ExportOptionsDialog::ExportOptionsDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * ExportOptionsDialog creator
 */

bool ExportOptionsDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ExportOptionsDialog member initialisation
    panelRes = NULL;
    resChoice = NULL;
    txt0 = NULL;
    resWidthSpin = NULL;
    txt1 = NULL;
    txt2 = NULL;
    resHeightSpin = NULL;
    txt3 = NULL;
////@end ExportOptionsDialog member initialisation
    width = 0;
    height = 0;
    H2Wratio = 1.0;
    filetype = 0;

    ((MolDisplayWin *)parent)->getCanvasSize(&screenWidth, &screenHeight);
    width = screenWidth;
    height = screenHeight;
    H2Wratio = (double)height / (double)width;

////@begin ExportOptionsDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end ExporresWidthTexttOptionsDialog creation
    resWidthSpin->SetValue(width);
    resHeightSpin->SetValue(height);
    return true;
}

/*!
 * Control creation for ExportOptionsDialog
 */

void ExportOptionsDialog::CreateControls()
{
////@begin ExportOptionsDialog content construction
    ExportOptionsDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    panelRes = new wxPanel( itemDialog1, ID_RES_PANEL, wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
    itemBoxSizer2->Add(panelRes, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    panelRes->SetSizer(itemBoxSizer4);

    wxString resChoiceStrings[] = {
        _("72dpi (Screen)"),
        _("300dpi (Print)"),
        _("Custom")
    };
    resChoice = new wxChoice( panelRes, ID_RES_CHOICE, wxDefaultPosition, wxDefaultSize, 3, resChoiceStrings, 0 );
    resChoice->SetStringSelection(_("72dpi (Screen)"));
    itemBoxSizer4->Add(resChoice, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer4->Add(itemBoxSizer6, 0, wxALIGN_RIGHT, 5);

    txt0 = new wxStaticText( panelRes, wxID_STATIC, _("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(txt0, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM|wxADJUST_MINSIZE, 5);

    resWidthSpin = new wxSpinCtrl( panelRes, ID_RESWIDTHSPIN, _T("300"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 999999999, 300 );
    itemBoxSizer6->Add(resWidthSpin, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxBOTTOM, 5);

    txt1 = new wxStaticText( panelRes, wxID_STATIC, _("pixels"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer6->Add(txt1, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer4->Add(itemBoxSizer10, 0, wxALIGN_RIGHT, 5);

    txt2 = new wxStaticText( panelRes, wxID_STATIC, _("Height:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(txt2, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM|wxADJUST_MINSIZE, 5);

    resHeightSpin = new wxSpinCtrl( panelRes, ID_RESHEIGHTSPIN, _T("300"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 999999999, 300 );
    itemBoxSizer10->Add(resHeightSpin, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxBOTTOM, 5);

    txt3 = new wxStaticText( panelRes, wxID_STATIC, _("pixels"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(txt3, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxBOTTOM|wxADJUST_MINSIZE, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer14 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer14, 0, wxALIGN_RIGHT|wxRIGHT|wxTOP|wxBOTTOM, 5);
    wxButton* itemButton15 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer14->AddButton(itemButton15);

    wxButton* itemButton16 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer14->AddButton(itemButton16);

    itemStdDialogButtonSizer14->Realize();

////@end ExportOptionsDialog content construction
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void ExportOptionsDialog::OnOkClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK in ExportOptionsDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK in ExportOptionsDialog. 
}


/*!
 * Should we show tooltips?
 */

bool ExportOptionsDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ExportOptionsDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ExportOptionsDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ExportOptionsDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ExportOptionsDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ExportOptionsDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ExportOptionsDialog icon retrieval
}



/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_RES_CHOICE
 */

void ExportOptionsDialog::OnResChoiceSelected( wxCommandEvent& event )
{
    switch(resChoice->GetSelection()) {
        case 0:
            width = screenWidth;
            height = screenHeight;
            resWidthSpin->SetValue(width);
            resHeightSpin->SetValue(height);
            break;
        case 1:
            width = screenWidth * 300 / 72;
            height = screenHeight * 300 / 72;
            resWidthSpin->SetValue(width);
            resHeightSpin->SetValue(height);
            break;
        case 2:
            // TODO:  Nothing?
            break;
    }
}

/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_RESWIDTHSPIN
 */

void ExportOptionsDialog::OnReswidthspinUpdated( wxSpinEvent& event )
{
    long tempWidth = 0;

    resChoice->SetSelection(2);
    tempWidth = resWidthSpin->GetValue();
    if(height != -1) {
        width = -1;
        height = (long)(tempWidth * H2Wratio);
        resHeightSpin->SetValue(height);
    }
    width = tempWidth;
}

/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_RESHEIGHTSPIN
 */

void ExportOptionsDialog::OnResheightspinUpdated( wxSpinEvent& event )
{
    long tempHeight = 0;

    resChoice->SetSelection(2);
    tempHeight = resHeightSpin->GetValue();
    if(width != -1) {
        height = -1;
        width = (long)(tempHeight / H2Wratio);
        resWidthSpin->SetValue(width);
    }
    height = tempHeight;
}


