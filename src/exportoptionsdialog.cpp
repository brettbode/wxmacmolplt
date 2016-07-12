/////////////////////////////////////////////////////////////////////////////
// Name:        exportoptionsdialog.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Thu 11 May 2006 02:46:50 PM CDT
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

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
	EVT_CHECKBOX( ID_CHECKBOX3, ExportOptionsDialog::OnCheckbox3Click )
	EVT_RADIOBOX( ID_ANIMATIONRADIO, ExportOptionsDialog::OnAnimationradioSelected )
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
	resTBGCheck = NULL;
	mMovieType = NULL;
	mEPlotCheck = NULL;
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

	wxArrayString resChoiceStrings;
	resChoiceStrings.Add(_("72dpi (Screen)"));
	resChoiceStrings.Add(_("300dpi (Print)"));
	resChoiceStrings.Add(_("Custom"));
	resChoice = new wxChoice( panelRes, ID_RES_CHOICE, wxDefaultPosition, wxDefaultSize, resChoiceStrings, 0 );
	resChoice->SetStringSelection(_("72dpi (Screen)"));
	itemBoxSizer4->Add(resChoice, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer4->Add(itemBoxSizer6, 0, wxALIGN_RIGHT, 5);

	txt0 = new wxStaticText( panelRes, wxID_STATIC, _("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer6->Add(txt0, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	resWidthSpin = new wxSpinCtrl( panelRes, ID_RESWIDTHSPIN, wxT("300"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 999999999, 300 );
	itemBoxSizer6->Add(resWidthSpin, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxBOTTOM, 5);

	txt1 = new wxStaticText( panelRes, wxID_STATIC, _("pixels"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer6->Add(txt1, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxBOTTOM, 5);

	wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer4->Add(itemBoxSizer10, 0, wxALIGN_RIGHT, 5);

	txt2 = new wxStaticText( panelRes, wxID_STATIC, _("Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer10->Add(txt2, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

	resHeightSpin = new wxSpinCtrl( panelRes, ID_RESHEIGHTSPIN, wxT("300"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 999999999, 300 );
	itemBoxSizer10->Add(resHeightSpin, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxBOTTOM, 5);

	txt3 = new wxStaticText( panelRes, wxID_STATIC, _("pixels"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer10->Add(txt3, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxBOTTOM, 5);

	resTBGCheck = new wxCheckBox( panelRes, ID_CHECKBOX3, _("Enable background transparency"), wxDefaultPosition, wxDefaultSize, 0 );
	resTBGCheck->SetValue(false);
	resTBGCheck->Enable(false);
	itemBoxSizer4->Add(resTBGCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer2->Add(itemBoxSizer15, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxArrayString mMovieTypeStrings;
	mMovieTypeStrings.Add(_("&Frame Animation"));
	mMovieTypeStrings.Add(_("&Normal Mode Animation"));
	mMovieType = new wxRadioBox( itemDialog1, ID_ANIMATIONRADIO, _("Animation Type"), wxDefaultPosition, wxDefaultSize, mMovieTypeStrings, 1, wxRA_SPECIFY_ROWS );
	mMovieType->SetSelection(0);
	itemBoxSizer15->Add(mMovieType, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mEPlotCheck = new wxCheckBox( itemDialog1, ID_ENERGYPLOTCHECK, _("Include Energy Plot"), wxDefaultPosition, wxDefaultSize, 0 );
	mEPlotCheck->SetValue(false);
	itemBoxSizer15->Add(mEPlotCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStdDialogButtonSizer* itemStdDialogButtonSizer18 = new wxStdDialogButtonSizer;

	itemBoxSizer2->Add(itemStdDialogButtonSizer18, 0, wxALIGN_RIGHT|wxRIGHT|wxTOP|wxBOTTOM, 5);
	wxButton* itemButton19 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	itemButton19->SetDefault();
	itemStdDialogButtonSizer18->AddButton(itemButton19);

	wxButton* itemButton20 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStdDialogButtonSizer18->AddButton(itemButton20);

	itemStdDialogButtonSizer18->Realize();

////@end ExportOptionsDialog content construction
}

void ExportOptionsDialog::setFileType(int ft) {
    filetype = ft;
    if((filetype == wxBITMAP_TYPE_PNG)
#if wxCHECK_VERSION(2,9,0)
	   || (filetype == wxBITMAP_TYPE_GIF) || (filetype == wxBITMAP_TYPE_TIFF)
#endif
	   ) {
        resTBGCheck->Enable(true);
    }
    else {
        resTBGCheck->Enable(false);
    }
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



/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX3
 */

void ExportOptionsDialog::OnCheckbox3Click( wxCommandEvent& event )
{
    transparency = resTBGCheck->GetValue();
}

/*!
 * Compute and return the ratio in size between the target output image and the screen image
 */

int ExportOptionsDialog::getImageRatio() const
{
    int result;
	result = width/screenWidth;
	if (result*screenWidth < width) result++;
	if (result < 1) result = 1;
	return result;
}


/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_ANIMATIONRADIO
 */

void ExportOptionsDialog::OnAnimationradioSelected( wxCommandEvent& event )
{
	if(event.GetInt() == 0) {
		mEPlotCheck->Enable(true);
	}
	else {
		mEPlotCheck->SetValue(false);
		mEPlotCheck->Enable(false);
	}
}

void ExportOptionsDialog::SetMovieChoice(int v) {
	mMovieType->SetSelection(v);
}

void ExportOptionsDialog::EnableFrameMovie(bool v) {
	if(v) {
		mMovieType->SetSelection(0);
		mMovieType->Enable(0, true);
		mEPlotCheck->Enable(true);
	}
	else {
		mMovieType->SetSelection(1);
		mMovieType->Enable(0, false);
		mEPlotCheck->Enable(false);
	}
}

void ExportOptionsDialog::EnableModeMovie(bool b) {
	mMovieType->SetSelection(0);
	mMovieType->Enable(1, b);
}

int ExportOptionsDialog::GetMovieChoice(void) const {
	return mMovieType->GetSelection();
}

bool ExportOptionsDialog::AddEnergyPlot(void) const {
	return mEPlotCheck->IsChecked();
}


