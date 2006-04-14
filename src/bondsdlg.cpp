/////////////////////////////////////////////////////////////////////////////
// Name:        bondsdlg.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Thu 13 Apr 16:02:45 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "bondsdlg.h"
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

#include "bondsdlg.h"

////@begin XPM images
////@end XPM images

/*!
 * BondsDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( BondsDlg, wxDialog )

/*!
 * BondsDlg event table definition
 */

BEGIN_EVENT_TABLE( BondsDlg, wxDialog )

////@begin BondsDlg event table entries
    EVT_BUTTON( wxID_ADD, BondsDlg::OnAddClick )

    EVT_BUTTON( wxID_DELETE, BondsDlg::OnDeleteClick )

    EVT_CHOICE( ID_CHOICE, BondsDlg::OnChoiceSelected )

    EVT_LIST_ITEM_SELECTED( ID_LISTCTRL, BondsDlg::OnListctrlSelected )
    EVT_LIST_ITEM_DESELECTED( ID_LISTCTRL, BondsDlg::OnListctrlDeselected )
    EVT_LIST_BEGIN_LABEL_EDIT( ID_LISTCTRL, BondsDlg::OnListctrlBeginLabelEdit )
    EVT_LIST_END_LABEL_EDIT( ID_LISTCTRL, BondsDlg::OnListctrlEndLabelEdit )
    EVT_LIST_DELETE_ITEM( ID_LISTCTRL, BondsDlg::OnListctrlDeleteItem )

////@end BondsDlg event table entries

END_EVENT_TABLE()

/*!
 * BondsDlg constructors
 */

BondsDlg::BondsDlg( )
{
}

BondsDlg::BondsDlg( MolDisplayWin * parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * BondsDlg creator
 */

bool BondsDlg::Create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin BondsDlg member initialisation
    AddBtn = NULL;
    DeleteBtn = NULL;
    BondOrderCtl = NULL;
    bondList = NULL;
////@end BondsDlg member initialisation
    Parent = parent;

////@begin BondsDlg creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end BondsDlg creation
    return true;
}

/*!
 * Control creation for BondsDlg
 */

void BondsDlg::CreateControls()
{    
////@begin BondsDlg content construction
    BondsDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    AddBtn = new wxButton( itemDialog1, wxID_ADD, _("&Add"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        AddBtn->SetToolTip(_("Add a new bond"));
    itemBoxSizer3->Add(AddBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    DeleteBtn = new wxButton( itemDialog1, wxID_DELETE, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        DeleteBtn->SetToolTip(_("deletes the selected bond(s)"));
    itemBoxSizer3->Add(DeleteBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Bond Type:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    itemBoxSizer3->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString BondOrderCtlStrings[] = {
        _("Hydrogen Bond"),
        _("Single Bond"),
        _("Double Bond"),
        _("Triple Bond")
    };
    BondOrderCtl = new wxChoice( itemDialog1, ID_CHOICE, wxDefaultPosition, wxDefaultSize, 4, BondOrderCtlStrings, 0 );
    BondOrderCtl->SetStringSelection(_("Single Bond"));
    if (ShowToolTips())
        BondOrderCtl->SetToolTip(_("Bond Order"));
    itemBoxSizer3->Add(BondOrderCtl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    bondList = new wxListCtrl( itemDialog1, ID_LISTCTRL, wxDefaultPosition, wxSize(200, -1), wxLC_REPORT|wxLC_EDIT_LABELS|wxSUNKEN_BORDER );
    itemBoxSizer8->Add(bondList, 5, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end BondsDlg content construction
	//add the column headings here?
	wxListItem itemCol;
	itemCol.SetText(_T("Atom 1"));
	bondList->InsertColumn(0, itemCol);
	
	itemCol.SetText(_T("Atom 2"));
	bondList->InsertColumn(1, itemCol);

	itemCol.SetText(_T("Length"));
	bondList->InsertColumn(2, itemCol);

	itemCol.SetText(_T("Type"));
	bondList->InsertColumn(3, itemCol);
}

void BondsDlg::ResetList(void) {
	//loop through the bonds and add each with SetItem(row, col, string)
}

/*!
 * Should we show tooltips?
 */

bool BondsDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap BondsDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin BondsDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end BondsDlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon BondsDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin BondsDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end BondsDlg icon retrieval
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD
 */

void BondsDlg::OnAddClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD in BondsDlg.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD in BondsDlg. 
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE
 */

void BondsDlg::OnDeleteClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE in BondsDlg.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE in BondsDlg. 
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE
 */

void BondsDlg::OnChoiceSelected( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE in BondsDlg.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE in BondsDlg. 
}

/*!
 * wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL
 */

void BondsDlg::OnListctrlSelected( wxListEvent& event )
{
////@begin wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL in BondsDlg.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL in BondsDlg. 
}

/*!
 * wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_LISTCTRL
 */

void BondsDlg::OnListctrlDeselected( wxListEvent& event )
{
////@begin wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_LISTCTRL in BondsDlg.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_LIST_ITEM_DESELECTED event handler for ID_LISTCTRL in BondsDlg. 
}

/*!
 * wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT event handler for ID_LISTCTRL
 */

void BondsDlg::OnListctrlBeginLabelEdit( wxListEvent& event )
{
////@begin wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT event handler for ID_LISTCTRL in BondsDlg.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_LIST_BEGIN_LABEL_EDIT event handler for ID_LISTCTRL in BondsDlg. 
}

/*!
 * wxEVT_COMMAND_LIST_END_LABEL_EDIT event handler for ID_LISTCTRL
 */

void BondsDlg::OnListctrlEndLabelEdit( wxListEvent& event )
{
////@begin wxEVT_COMMAND_LIST_END_LABEL_EDIT event handler for ID_LISTCTRL in BondsDlg.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_LIST_END_LABEL_EDIT event handler for ID_LISTCTRL in BondsDlg. 
}

/*!
 * wxEVT_COMMAND_LIST_DELETE_ITEM event handler for ID_LISTCTRL
 */

void BondsDlg::OnListctrlDeleteItem( wxListEvent& event )
{
////@begin wxEVT_COMMAND_LIST_DELETE_ITEM event handler for ID_LISTCTRL in BondsDlg.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_LIST_DELETE_ITEM event handler for ID_LISTCTRL in BondsDlg. 
}


