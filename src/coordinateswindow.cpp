/////////////////////////////////////////////////////////////////////////////
// Name:        coordinateswindow.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri 21 Apr 12:23:58 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "coordinateswindow.h"
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

#include "coordinateswindow.h"

////@begin XPM images
////@end XPM images

/*!
 * CoordinatesWindow type definition
 */

IMPLEMENT_DYNAMIC_CLASS( CoordinatesWindow, wxDialog )

/*!
 * CoordinatesWindow event table definition
 */

BEGIN_EVENT_TABLE( CoordinatesWindow, wxDialog )

////@begin CoordinatesWindow event table entries
    EVT_CLOSE( CoordinatesWindow::OnCloseWindow )

    EVT_BUTTON( wxID_ADD, CoordinatesWindow::OnAddClick )

    EVT_BUTTON( wxID_DELETE, CoordinatesWindow::OnDeleteClick )

    EVT_BUTTON( ID_BONDBUTTON, CoordinatesWindow::OnBondbuttonClick )

    EVT_BUTTON( ID_STICKBUTTON, CoordinatesWindow::OnStickbuttonClick )

    EVT_CHOICE( ID_COORDCHOICE1, CoordinatesWindow::OnCoordchoice1Selected )

    EVT_GRID_CELL_CHANGE( CoordinatesWindow::OnCellChange )
    EVT_GRID_SELECT_CELL( CoordinatesWindow::OnSelectCell )
    EVT_GRID_RANGE_SELECT( CoordinatesWindow::OnRangeSelect )
    EVT_SIZE( CoordinatesWindow::OnSize )

////@end CoordinatesWindow event table entries

END_EVENT_TABLE()

/*!
 * CoordinatesWindow constructors
 */

CoordinatesWindow::CoordinatesWindow( )
{
}

CoordinatesWindow::CoordinatesWindow( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * CoordinatesWindow creator
 */

bool CoordinatesWindow::Create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin CoordinatesWindow member initialisation
    CoordType = 0;
    deleteButton = NULL;
    BondButton = NULL;
    coordTypeChoice = NULL;
    coordGrid = NULL;
////@end CoordinatesWindow member initialisation
	Parent = parent;
	Prefs = Parent->GetPrefs();

////@begin CoordinatesWindow creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end CoordinatesWindow creation
    return true;
}

/*!
 * Control creation for CoordinatesWindow
 */

void CoordinatesWindow::CreateControls()
{    
////@begin CoordinatesWindow content construction
    CoordinatesWindow* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton4 = new wxButton( itemDialog1, wxID_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemButton4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    deleteButton = new wxButton( itemDialog1, wxID_DELETE, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(deleteButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    BondButton = new wxButton( itemDialog1, ID_BONDBUTTON, _("Bond"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(BondButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, ID_STICKBUTTON, _("Stick"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Coord. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText8, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString coordTypeChoiceStrings[] = {
        _("Cartesian"),
        _("Z-Matrix")
    };
    coordTypeChoice = new wxChoice( itemDialog1, ID_COORDCHOICE1, wxDefaultPosition, wxDefaultSize, 2, coordTypeChoiceStrings, 0 );
    itemBoxSizer3->Add(coordTypeChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    coordGrid = new wxGrid( itemDialog1, ID_COORDGRID, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    coordGrid->SetDefaultColSize(50);
    coordGrid->SetDefaultRowSize(25);
    coordGrid->SetColLabelSize(25);
    coordGrid->SetRowLabelSize(50);
    coordGrid->CreateGrid(5, 5, wxGrid::wxGridSelectRows);
    itemBoxSizer2->Add(coordGrid, 1, wxGROW|wxALL, 0);

////@end CoordinatesWindow content construction
	coordTypeChoice->SetSelection(CoordType);
	SetupGridColumns();
	FrameChanged();
}

/*!
 * Should we show tooltips?
 */

bool CoordinatesWindow::ShowToolTips()
{
    return true;
}

void CoordinatesWindow::SetupGridColumns(void) {
	coordGrid->DeleteCols(0, coordGrid->GetNumberCols(), true);
	if (CoordType == 0) {
		coordGrid->InsertCols(0, 4, true);
		coordGrid->SetColLabelValue(0, _T("Type"));
		coordGrid->SetColLabelValue(1, _T("X"));
		coordGrid->SetColLabelValue(2, _T("Y"));
		coordGrid->SetColLabelValue(3, _T("Z"));
	} else {
		coordGrid->InsertCols(0, 7, true);
		coordGrid->SetColLabelValue(0, _T("Type"));
		coordGrid->SetColLabelValue(1, _T("atm 1"));
		coordGrid->SetColLabelValue(2, _T("Length"));
		coordGrid->SetColLabelValue(3, _T("atm 2"));
		coordGrid->SetColLabelValue(4, _T("Angle"));
		coordGrid->SetColLabelValue(5, _T("atm 3"));
		coordGrid->SetColLabelValue(6, _T("Dihedral"));
	}
	wxSize s = GetSize();
	SizeCols(s);
}

void CoordinatesWindow::FrameChanged(void) {
		//clear off the old rows
	coordGrid->DeleteRows(0, coordGrid->GetNumberRows(), true);
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	if (natoms > 0) {
		bool temp = lFrame->GetAtomSelectState(0);
		coordGrid->InsertRows(0, natoms, true);
		coordGrid->HideCellEditControl();
		coordGrid->ClearSelection();
		lFrame->SetAtomSelectState(0, temp);
		wxString buf;
		for (long i=0; i<natoms; i++) {
			Prefs->GetAtomLabel(lFrame->GetAtomType(i)-1, buf);
			coordGrid->SetCellValue(i, 0, buf);
			if (CoordType == 0) {
				CPoint3D pos;
				lFrame->GetAtomPosition(i, pos);
				buf.Printf("%f", pos.x);
				coordGrid->SetCellValue(i, 1, buf);
				buf.Printf("%f", pos.y);
				coordGrid->SetCellValue(i, 2, buf);
				buf.Printf("%f", pos.z);
				coordGrid->SetCellValue(i, 3, buf);
			} else {
				Internals * internals = MainData->GetInternalCoordinates();
				MOPacInternals * mInts = NULL;
				if (internals)
					mInts = internals->GetMOPacStyle();
				if (mInts) {
					buf.Printf("%d", mInts->GetConnection(i,0)+1);
					coordGrid->SetCellValue(i, 1, buf);
					buf.Printf("%f", mInts->GetValue(i,0));
					coordGrid->SetCellValue(i, 2, buf);
					buf.Printf("%d", mInts->GetConnection(i,1)+1);
					coordGrid->SetCellValue(i, 3, buf);
					buf.Printf("%.2f", mInts->GetValue(i,1));
					coordGrid->SetCellValue(i, 4, buf);
					buf.Printf("%d", mInts->GetConnection(i,2)+1);
					coordGrid->SetCellValue(i, 5, buf);
					buf.Printf("%.2f", mInts->GetValue(i,2));
					coordGrid->SetCellValue(i, 6, buf);
				}
			}
		}
	}
}

void CoordinatesWindow::SizeCols(wxSize & s) {
	int width = s.GetWidth() - 66;	//subtract off the row labels and scroll
	if (CoordType == 0) {
		int a = (int) (width/5.0);
		int b = (int) ((width-a)/3.0);
		coordGrid->SetColSize(0, a);
		coordGrid->SetColSize(1, b);
		coordGrid->SetColSize(2, b);
		coordGrid->SetColSize(3, width-(a+2*b));
	} else {
		int a = (width/3)/4;
		int b = (2*width/3)/3;
		coordGrid->SetColSize(0, a);
		coordGrid->SetColSize(1, a);
		coordGrid->SetColSize(2, b);
		coordGrid->SetColSize(3, a);
		coordGrid->SetColSize(4, b);
		coordGrid->SetColSize(5, a);
		coordGrid->SetColSize(6, (width-4*a-2*b));
	}
		
}
/*!
 * Get bitmap resources
 */

wxBitmap CoordinatesWindow::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin CoordinatesWindow bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end CoordinatesWindow bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon CoordinatesWindow::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin CoordinatesWindow icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end CoordinatesWindow icon retrieval
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD
 */

void CoordinatesWindow::OnAddClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD in CoordinatesWindow.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD in CoordinatesWindow. 
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE
 */

void CoordinatesWindow::OnDeleteClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE in CoordinatesWindow.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE in CoordinatesWindow. 
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BONDBUTTON
 */

void CoordinatesWindow::OnBondbuttonClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BONDBUTTON in CoordinatesWindow.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BONDBUTTON in CoordinatesWindow. 
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STICKBUTTON
 */

void CoordinatesWindow::OnStickbuttonClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STICKBUTTON in CoordinatesWindow.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STICKBUTTON in CoordinatesWindow. 
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_COORDCHOICE1
 */

void CoordinatesWindow::OnCoordchoice1Selected( wxCommandEvent& event )
{
	int temp = coordTypeChoice->GetSelection();
	if (temp != CoordType) {
		CoordType = temp;
		if (CoordType == 1) {
			MoleculeData * MainData = Parent->GetData();
			Internals * internals = MainData->GetInternalCoordinates();
			if (!internals) {
				MainData->InitializeInternals();
				internals = MainData->GetInternalCoordinates();
				if (!internals) {
					CoordType = 0;
	//				UpdateControls();
					return;
				}
			}
			MOPacInternals * mInts = internals->GetMOPacStyle();
			if (!mInts) {
				internals->CreateMOPacInternals(3*MainData->GetMaximumAtomCount());
				mInts = internals->GetMOPacStyle();
				if (mInts) {
					CoordType = 0;
	//				UpdateControls();
					return;
				}
				mInts->GuessInit(MainData);
			} else 
				mInts->CartesiansToInternals(MainData);
		}
		SetupGridColumns();
		FrameChanged();
	}
}

/*!
 * wxEVT_GRID_CELL_CHANGE event handler for ID_GRID
 */

void CoordinatesWindow::OnCellChange( wxGridEvent& event )
{
////@begin wxEVT_GRID_CELL_CHANGE event handler for ID_GRID in CoordinatesWindow.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_GRID_CELL_CHANGE event handler for ID_GRID in CoordinatesWindow. 
}

/*!
 * wxEVT_GRID_SELECT_CELL event handler for ID_GRID
 */

void CoordinatesWindow::OnSelectCell( wxGridEvent& event )
{
////@begin wxEVT_GRID_SELECT_CELL event handler for ID_GRID in CoordinatesWindow.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_GRID_SELECT_CELL event handler for ID_GRID in CoordinatesWindow. 
}

/*!
 * wxEVT_GRID_RANGE_SELECT event handler for ID_GRID
 */

void CoordinatesWindow::OnRangeSelect( wxGridRangeSelectEvent& event )
{
////@begin wxEVT_GRID_RANGE_SELECT event handler for ID_GRID in CoordinatesWindow.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_GRID_RANGE_SELECT event handler for ID_GRID in CoordinatesWindow. 
}

/*!
 * wxEVT_SIZE event handler for ID_GRID
 */

void CoordinatesWindow::OnSize( wxSizeEvent& event )
{
	wxSize s = event.GetSize();
	SizeCols(s);
    event.Skip();
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_DIALOG
 */

void CoordinatesWindow::OnCloseWindow( wxCloseEvent& event )
{
	Parent->CloseCoordsWindow();
}


