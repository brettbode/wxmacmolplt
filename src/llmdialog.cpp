/////////////////////////////////////////////////////////////////////////////
// Name:        llmdialog.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Tue  2 May 13:24:39 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "llmdialog.h"
#endif

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
#include "Globals.h"
#include "MolDisplayWin.h"
#include "Frame.h"
#include "Internals.h"

#include "llmdialog.h"

////@begin XPM images
////@end XPM images

/*!
 * LLMDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( LLMDialog, wxDialog )

/*!
 * LLMDialog event table definition
 */

BEGIN_EVENT_TABLE( LLMDialog, wxDialog )

////@begin LLMDialog event table entries
    EVT_TEXT( ID_TEXTCTRL, LLMDialog::OnTextctrlUpdated )

    EVT_CHECKBOX( ID_INTERNALSCHECKBOX, LLMDialog::OnInternalscheckboxClick )

    EVT_BUTTON( wxID_CANCEL, LLMDialog::OnCancelClick )

    EVT_BUTTON( wxID_OK, LLMDialog::OnOkClick )

////@end LLMDialog event table entries

END_EVENT_TABLE()

/*!
 * LLMDialog constructors
 */

LLMDialog::LLMDialog( )
{
}

LLMDialog::LLMDialog( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * LLMDialog creator
 */

bool LLMDialog::Create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin LLMDialog member initialisation
////@end LLMDialog member initialisation
	Parent = parent;

////@begin LLMDialog creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end LLMDialog creation
    return true;
}

/*!
 * Control creation for LLMDialog
 */

void LLMDialog::CreateControls()
{    
////@begin LLMDialog content construction
    LLMDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Linear Least Motion (LLM) path analysis"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Click the Create button to insert the\nnumber of frames given below along\nthe LLM path connecting the current\ngeometry with the next geometry."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl5 = new wxTextCtrl( itemDialog1, ID_TEXTCTRL, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        itemTextCtrl5->SetToolTip(_("Enter an integer for the number of intermediate geometries to create"));
    itemBoxSizer2->Add(itemTextCtrl5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxCheckBox* itemCheckBox6 = new wxCheckBox( itemDialog1, ID_INTERNALSCHECKBOX, _("Compute using internal coordinates"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox6->SetValue(false);
    if (ShowToolTips())
        itemCheckBox6->SetToolTip(_("Check to create the LLM path using the currently defined set of internal coordinates"));
    itemBoxSizer2->Add(itemCheckBox6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer7, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton8 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemButton8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton9 = new wxButton( itemDialog1, wxID_OK, _("Create LLM"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton9->SetDefault();
    itemBoxSizer7->Add(itemButton9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end LLMDialog content construction
	MoleculeData * MainData = Parent->GetData();
	itemCheckBox6->Enable(MainData->GetInternalCoordinates()!=NULL);
}

/*!
 * Should we show tooltips?
 */

bool LLMDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap LLMDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin LLMDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end LLMDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon LLMDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin LLMDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end LLMDialog icon retrieval
}
/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL
 */

void LLMDialog::OnTextctrlUpdated( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL in LLMDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL in LLMDialog. 
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_INTERNALSCHECKBOX
 */

void LLMDialog::OnInternalscheckboxClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_INTERNALSCHECKBOX in LLMDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_INTERNALSCHECKBOX in LLMDialog. 
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void LLMDialog::OnCancelClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL in LLMDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL in LLMDialog. 
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void LLMDialog::OnOkClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK in LLMDialog.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK in LLMDialog. 
}


