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
#include "Frame.h"

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

    EVT_GRID_SELECT_CELL( BondsDlg::OnSelectCell )
    EVT_GRID_EDITOR_HIDDEN( BondsDlg::OnEditorHidden )
    EVT_GRID_RANGE_SELECT( BondsDlg::OnRangeSelect )

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
    bondGrid = NULL;
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
    DeleteBtn->Enable(false);
    itemBoxSizer3->Add(DeleteBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Bond Type:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    itemStaticText6->Enable(false);
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
    BondOrderCtl->Enable(false);
    itemBoxSizer3->Add(BondOrderCtl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer8, 5, wxGROW, 5);

    bondGrid = new wxGrid( itemDialog1, ID_BONDGRID, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxVSCROLL );
    bondGrid->SetDefaultColSize(100);
    bondGrid->SetDefaultRowSize(25);
    bondGrid->SetColLabelSize(25);
    bondGrid->SetRowLabelSize(0);
    bondGrid->CreateGrid(5, 4, wxGrid::wxGridSelectRows);
    itemBoxSizer8->Add(bondGrid, 5, wxGROW|wxALL, 0);

////@end BondsDlg content construction
	 //Setup the columns to store integers and floats
	bondGrid->SetColFormatNumber(0);
	bondGrid->SetColFormatNumber(1);
	bondGrid->SetColFormatFloat(2, -1, 6);
	bondGrid->SetColLabelValue(0, _T("Atom 1"));
	bondGrid->SetColLabelValue(1, _T("Atom 2"));
	bondGrid->SetColLabelValue(2, _T("Length"));
	bondGrid->SetColLabelValue(3, _T("Type"));
	ResetList();
}

void BondsDlg::ResetList(void) {
		//Clear off the old rows
	bondGrid->DeleteRows(0, bondGrid->GetNumberRows(), true);
	//loop through the bonds and add each with SetItem(row, col, string)
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long nbonds = lFrame->GetNumBonds();
	//Add back the new ones
	bondGrid->InsertRows(0, nbonds, true);
	wxString buf;
	for (long i=0; i<nbonds; i++) {
		Bond * b = lFrame->GetBondLoc(i);
		buf.Printf("%d", b->Atom1);
		bondGrid->SetCellValue(i, 0, buf);
		buf.Printf("%d", b->Atom2);
		bondGrid->SetCellValue(i, 1, buf);
		buf.Printf("%f", lFrame->GetBondLength(i));
		bondGrid->SetCellValue(i, 2, buf);
		bondGrid->SetReadOnly(i, 2, true);
		switch (lFrame->GetBondOrder(i)) {
			case kHydrogenBond:
				buf.Printf("%s", _T("Hydrogen"));
				break;
			case kSingleBond:
				buf.Printf("%s", _T("Single"));
				break;
			case kDoubleBond:
				buf.Printf("%s", _T("Double"));
				break;
			case kTripleBond:
				buf.Printf("%s", _T("Triple"));
				break;
		}
		bondGrid->SetCellValue(i, 3, buf);
		bondGrid->SetReadOnly(i, 3, true);
		bondGrid->SetCellAlignment(wxALIGN_CENTRE, i, 3);
	}
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
 * wxEVT_GRID_SELECT_CELL event handler for ID_BONDGRID
 */

void BondsDlg::OnSelectCell( wxGridEvent& event )
{
////@begin wxEVT_GRID_SELECT_CELL event handler for ID_BONDGRID in BondsDlg.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_GRID_SELECT_CELL event handler for ID_BONDGRID in BondsDlg. 
}

/*!
 * wxEVT_GRID_EDITOR_HIDDEN event handler for ID_BONDGRID
 */

void BondsDlg::OnEditorHidden( wxGridEvent& event )
{
////@begin wxEVT_GRID_EDITOR_HIDDEN event handler for ID_BONDGRID in BondsDlg.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_GRID_EDITOR_HIDDEN event handler for ID_BONDGRID in BondsDlg. 
}


/*!
 * wxEVT_GRID_RANGE_SELECT event handler for ID_BONDGRID
 */

void BondsDlg::OnRangeSelect( wxGridRangeSelectEvent& event )
{
////@begin wxEVT_GRID_RANGE_SELECT event handler for ID_BONDGRID in BondsDlg.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_GRID_RANGE_SELECT event handler for ID_BONDGRID in BondsDlg. 
}


