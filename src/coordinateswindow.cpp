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
#include "myFiles.h"

#include "coordinateswindow.h"

////@begin XPM images
////@end XPM images

/*!
 * CoordinatesWindow type definition
 */

IMPLEMENT_DYNAMIC_CLASS( CoordinatesWindow, wxFrame )

/*!
 * CoordinatesWindow event table definition
 */
enum MMP_EventID {
	MMP_SHRINK10=wxID_HIGHEST+1,
	MMP_ENLARGE10,
	MMP_SHOWMODE,
	MMP_PREVMODE,
	MMP_NEXTMODE,
	MMP_SHOWAXIS,
	MMP_CENTER,
	MMP_ROTATESUBMENU,
	MMP_ROTATETOXAXIS,
	MMP_ROTATETOYAXIS,
	MMP_ROTATETOZAXIS,
	MMP_ROTATE180HOR,
	MMP_ROTATE180VER,
	MMP_ROTATEPRINC,
	MMP_ROTATEOTHER,
	MMP_CONVERTTOBOHR,
	MMP_CONVERTTOANGSTROMS,
	MMP_INVERTNORMALMODE,
	MMP_COPYCOORDS,
	MMP_BONDSWINDOW,
	MMP_COORDSWINDOW,
	
	Number_MMP_Ids
};

BEGIN_EVENT_TABLE( CoordinatesWindow, wxFrame )

////@begin CoordinatesWindow event table entries
    EVT_CLOSE( CoordinatesWindow::OnCloseWindow )

    EVT_MENU( wxID_CLOSE, CoordinatesWindow::OnCloseClick )

    EVT_UPDATE_UI( wxID_UNDO, CoordinatesWindow::OnUndoUpdate )

    EVT_UPDATE_UI( wxID_CUT, CoordinatesWindow::OnCutUpdate )

    EVT_UPDATE_UI( wxID_COPY, CoordinatesWindow::OnCopyUpdate )

    EVT_MENU( MMP_COPYCOORDSITEM, CoordinatesWindow::OnMmpCopycoordsitemClick )
    EVT_UPDATE_UI( MMP_COPYCOORDSITEM, CoordinatesWindow::OnMmpCopycoordsitemUpdate )

    EVT_UPDATE_UI( wxID_PASTE, CoordinatesWindow::OnPasteUpdate )

    EVT_UPDATE_UI( wxID_CLEAR, CoordinatesWindow::OnClearUpdate )

    EVT_MENU( wxID_SELECTALL, CoordinatesWindow::OnSelectallClick )
    EVT_UPDATE_UI( wxID_SELECTALL, CoordinatesWindow::OnSelectallUpdate )

    EVT_MENU( ID_STICKMENU, CoordinatesWindow::OnStickmenuClick )
    EVT_UPDATE_UI( ID_STICKMENU, CoordinatesWindow::OnStickmenuUpdate )

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

	EVT_ACTIVATE(CoordinatesWindow::OnActivate)
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
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
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
    CoordinatesWindow* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(wxID_NEW, _("&New\tCtrl+N"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(wxID_OPEN, _("&Open ...\tCtrl+O"), _T(""), wxITEM_NORMAL);
    itemMenu3->Append(wxID_CLOSE, _("&Close\tCtrl+W"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("File"));
    wxMenu* itemMenu7 = new wxMenu;
    itemMenu7->Append(wxID_UNDO, _("&Undo\tCtrl+Z"), _T(""), wxITEM_NORMAL);
    itemMenu7->Enable(wxID_UNDO, false);
    itemMenu7->AppendSeparator();
    itemMenu7->Append(wxID_CUT, _("Cu&t\tCtrl+X"), _T(""), wxITEM_NORMAL);
    itemMenu7->Enable(wxID_CUT, false);
    itemMenu7->Append(wxID_COPY, _("&Copy\tCtrl+C"), _T(""), wxITEM_NORMAL);
    itemMenu7->Enable(wxID_COPY, false);
    itemMenu7->Append(MMP_COPYCOORDSITEM, _("Copy Coordinates"), _("Copy the full set of coordinates with the current coordinate type."), wxITEM_NORMAL);
    itemMenu7->Enable(MMP_COPYCOORDSITEM, false);
    itemMenu7->Append(wxID_PASTE, _("&Paste\tCtrl+V"), _T(""), wxITEM_NORMAL);
    itemMenu7->Enable(wxID_PASTE, false);
    itemMenu7->Append(wxID_CLEAR, _("&Delete\tDel"), _T(""), wxITEM_NORMAL);
    itemMenu7->Enable(wxID_CLEAR, false);
    itemMenu7->AppendSeparator();
    itemMenu7->Append(wxID_SELECTALL, _("&Select all\tCtrl+A"), _T(""), wxITEM_NORMAL);
    itemMenu7->Enable(wxID_SELECTALL, false);
    menuBar->Append(itemMenu7, _("Edit"));
    wxMenu* itemMenu17 = new wxMenu;
    itemMenu17->Append(ID_STICKMENU, _("Use Coordinates for Reference"), _("Makes the current rotated coordinates the reference coordinates."), wxITEM_NORMAL);
    menuBar->Append(itemMenu17, _("Coordinates"));
    itemFrame1->SetMenuBar(menuBar);

    wxPanel* itemPanel19 = new wxPanel( itemFrame1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );

    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxVERTICAL);
    itemPanel19->SetSizer(itemBoxSizer20);

    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer20->Add(itemBoxSizer21, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton22 = new wxButton( itemPanel19, wxID_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        itemButton22->SetToolTip(_("Add a new atom to the list of coordinates."));
    itemBoxSizer21->Add(itemButton22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    deleteButton = new wxButton( itemPanel19, wxID_DELETE, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        deleteButton->SetToolTip(_("Delete the selected atoms."));
    itemBoxSizer21->Add(deleteButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    BondButton = new wxButton( itemPanel19, ID_BONDBUTTON, _("Bond"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        BondButton->SetToolTip(_("Apply the default bonding criteria."));
    itemBoxSizer21->Add(BondButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton25 = new wxButton( itemPanel19, ID_STICKBUTTON, _("Stick"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        itemButton25->SetToolTip(_("Use the current screen rotation as the reference frame."));
    itemBoxSizer21->Add(itemButton25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText26 = new wxStaticText( itemPanel19, wxID_STATIC, _("Coord. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer21->Add(itemStaticText26, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString coordTypeChoiceStrings[] = {
        _("Cartesian"),
        _("Z-Matrix")
    };
    coordTypeChoice = new wxChoice( itemPanel19, ID_COORDCHOICE1, wxDefaultPosition, wxDefaultSize, 2, coordTypeChoiceStrings, 0 );
    if (ShowToolTips())
        coordTypeChoice->SetToolTip(_("Changes the displayed coordinate type."));
    itemBoxSizer21->Add(coordTypeChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    coordGrid = new wxGrid( itemPanel19, ID_COORDGRID, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
    coordGrid->SetDefaultColSize(50);
    coordGrid->SetDefaultRowSize(25);
    coordGrid->SetColLabelSize(25);
    coordGrid->SetRowLabelSize(50);
    coordGrid->CreateGrid(5, 5, wxGrid::wxGridSelectRows);
    itemBoxSizer20->Add(coordGrid, 1, wxGROW|wxALL, 0);

////@end CoordinatesWindow content construction
	coordGrid->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
	SetupGridColumns();
	FrameChanged();
	UpdateControls();
    Centre();
	wxSize s(50, 150);
	coordGrid->SetMinSize(s);
	itemPanel19->Fit();
	Fit();
}

/*!
 * Should we show tooltips?
 */

bool CoordinatesWindow::ShowToolTips()
{
    return true;
}

void CoordinatesWindow::UpdateControls(void) {
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	bool selectionActive = false;
	for (long i=0; i<natoms; i++) {
		if (lFrame->GetAtomSelectState(i)) {
			selectionActive = true;
		}
	}
	deleteButton->Enable(selectionActive);
	coordTypeChoice->SetSelection(CoordType);
}
void CoordinatesWindow::SetupGridColumns(void) {
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	bool temp = false;
	if (natoms>0) temp = lFrame->GetAtomSelectState(0);
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
	if (natoms>0) {
		coordGrid->ClearSelection();
		lFrame->SetAtomSelectState(0, temp);
	}
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
			if (lFrame->GetAtomSelectState(i)) coordGrid->SelectRow(i, true);
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
					if (i>0) {
						buf.Printf("%d", mInts->GetConnection(i,0)+1);
						coordGrid->SetCellValue(i, 1, buf);
						buf.Printf("%f", mInts->GetValue(i,0));
						coordGrid->SetCellValue(i, 2, buf);
						if (i>1) {
							buf.Printf("%d", mInts->GetConnection(i,1)+1);
							coordGrid->SetCellValue(i, 3, buf);
							buf.Printf("%.2f", mInts->GetValue(i,1));
							coordGrid->SetCellValue(i, 4, buf);
							if (i>2) {
								buf.Printf("%d", mInts->GetConnection(i,2)+1);
								coordGrid->SetCellValue(i, 5, buf);
								buf.Printf("%.2f", mInts->GetValue(i,2));
								coordGrid->SetCellValue(i, 6, buf);
							} else {
								for (int j=5; j<7; j++)
									coordGrid->SetReadOnly(i, j, true);
							}
						} else {
							for (int j=3; j<7; j++)
								coordGrid->SetReadOnly(i, j, true);
						}
					} else {
						for (int j=1; j<7; j++)
							coordGrid->SetReadOnly(i, j, true);
					}
				}
			}
		}
	}
}

void CoordinatesWindow::SizeCols(wxSize & s) {
	int width = s.GetWidth() - 66;	//subtract off the row labels and scroll
	if (CoordType == 0) {
		int a = (int) (width/6.0);
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
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	for (int i=0; i<natoms; i++) lFrame->SetAtomSelectState(i, false);
	MainData->NewAtom();
	if (CoordType == 1) {
		Internals * internals = MainData->GetInternalCoordinates();
		if (internals) {
			MOPacInternals * mInts = internals->GetMOPacStyle();
			if (mInts)
				mInts->AddAtom(MainData);
		}
	}
	lFrame->SetAtomSelectState(natoms, true);
	Parent->FrameChanged();
	FrameChanged();
	coordGrid->SelectRow(natoms, true);
	coordGrid->SetGridCursor(natoms, 0);
	coordGrid->MakeCellVisible(natoms, 0);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE
 */

void CoordinatesWindow::OnDeleteClick( wxCommandEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	for (int i=(natoms-1); i>=0; i--) {
		if (lFrame->GetAtomSelectState(i)) {
			lFrame->DeleteAtom(i);
			coordGrid->DeleteRows(i, 1, true);
		}
	}
	coordGrid->ClearSelection();
	UpdateControls();
	Parent->ResetModel(false);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BONDBUTTON
 */

void CoordinatesWindow::OnBondbuttonClick( wxCommandEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	lFrame->SetBonds(Prefs, true);
	Parent->BondsChanged();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STICKBUTTON
 */

void CoordinatesWindow::OnStickbuttonClick( wxCommandEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	MainData->StickCoordinates();
	Parent->AtomsChanged();
	FrameChanged();
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
					UpdateControls();
					return;
				}
			}
			MOPacInternals * mInts = internals->GetMOPacStyle();
			if (!mInts) {
				internals->CreateMOPacInternals(3*MainData->GetMaximumAtomCount());
				mInts = internals->GetMOPacStyle();
				if (!mInts) {
					CoordType = 0;
					UpdateControls();
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
	int row = event.GetRow();
	int col = event.GetCol();
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	wxString val = coordGrid->GetCellValue(row, col);
	bool Changed = false;
	if (col == 0) {	//Atomic label
		long atomnum;
		if (val.ToLong(&atomnum)) {	//number instead of symbol
			if ((atomnum < 1)||(atomnum>107)) atomnum = -1;//Not a correct Atomic #
		} else {
			atomnum = ::SetAtomType((const unsigned char *)val.ToAscii());//Change the symbol to Atomic #
			if ((atomnum < 1)||(atomnum>107)) atomnum = -1;//Not a correct Atomic label
		}
		if ((atomnum > -1)&&(atomnum!=lFrame->GetAtomType(row))) {//A atom type was found so change the type
			lFrame->SetAtomType(row, atomnum);
			Changed = true;
		}
		Prefs->GetAtomLabel(lFrame->GetAtomType(row)-1, val);
		coordGrid->SetCellValue(row, 0, val);
	} else {
		if (CoordType == 0) {	//X, Y, Z cartesians
			double fval;
			if (val.ToDouble(&fval)) {
				CPoint3D pos;
				lFrame->GetAtomPosition(row, pos);
				if ((col == 1)&&(fval != pos.x)) {
					pos.x = fval;
					Changed = true;
				} else if ((col==2)&&(fval != pos.y)) {
					pos.y = fval;
					Changed = true;
				} else if ((col == 3)&&(fval != pos.z)) {
					pos.z = fval;
					Changed = true;
				}
				if (Changed) lFrame->SetAtomPosition(row, pos);
			} else {	//invalid text, just veto the change
				event.Veto();
				return;
			}
		} else { //z-matrix
			long newid;
			double fval;
			bool goodVal;
			MoleculeData * MainData = Parent->GetData();
			Internals * internals = MainData->GetInternalCoordinates();
			MOPacInternals * mInts = internals->GetMOPacStyle();
			if (col & 1) { //odd numbered columns contain the integer connection ids
				goodVal = val.ToLong(&newid);
				newid --;
					//test the value to see if it is in range
				if (goodVal && ((newid < 0)||(newid>row))) goodVal = false;
			} else {	//even numbered columns contain floats for the length and angles
				goodVal = val.ToDouble(&fval);
			}
			if (goodVal) {
				switch (col) {
					case 1:
						if (newid != mInts->GetConnection(row, 0)) {
							//We need to make sure that the 3 reference atom ids are unique
							//If we are already using the requested id, swap them
							if (newid == mInts->GetConnection(row, 1))
								mInts->SetConnection(row, 1, mInts->GetConnection(row, 0));
							else if (newid == mInts->GetConnection(row, 2))
								mInts->SetConnection(row, 2, mInts->GetConnection(row, 0));
							mInts->SetConnection(row, 0, newid);
							Changed = true;
						}
						break;
					case 2:
						if (fval != mInts->GetValue(row, 0)) {
							mInts->SetValue(row, 0, fval);
							Changed = true;
						}
						break;
					case 3:
						if (newid != mInts->GetConnection(row, 1)) {
							//We need to make sure that the 3 reference atom ids are unique
							//If we are already using the requested id, swap them
							if (newid == mInts->GetConnection(row, 0))
								mInts->SetConnection(row, 0, mInts->GetConnection(row, 1));
							else if (newid == mInts->GetConnection(row, 2))
								mInts->SetConnection(row, 2, mInts->GetConnection(row, 1));
							mInts->SetConnection(row, 1, newid);
							Changed = true;
						}
						break;
					case 4:
						if (fval != mInts->GetValue(row, 1)) {
							mInts->SetValue(row, 1, fval);
							Changed = true;
						}
						break;
					case 5:
						if (newid != mInts->GetConnection(row, 2)) {
							//We need to make sure that the 3 reference atom ids are unique
							//If we are already using the requested id, swap them
							if (newid == mInts->GetConnection(row, 0))
								mInts->SetConnection(row, 0, mInts->GetConnection(row, 2));
							else if (newid == mInts->GetConnection(row, 1))
								mInts->SetConnection(row, 1, mInts->GetConnection(row, 2));
							mInts->SetConnection(row, 2, newid);
							Changed = true;
						}
						break;
					case 6:
						if (fval != mInts->GetValue(row, 2)) {
							mInts->SetValue(row, 2, fval);
							Changed = true;
						}
						break;
				}
			}
			if (!goodVal) {
				event.Veto();
				return;
			}
			if (Changed) {
				if (col & 1) { //if a connection id was changed we need to regenerate the internals
					mInts->CartesiansToInternals(MainData);
						//Need to update the length and angles for this row
					if (row>0) {
						val.Printf("%d", mInts->GetConnection(row,0)+1);
						coordGrid->SetCellValue(row, 1, val);
						val.Printf("%f", mInts->GetValue(row,0));
						coordGrid->SetCellValue(row, 2, val);
						if (row>1) {
							val.Printf("%d", mInts->GetConnection(row,1)+1);
							coordGrid->SetCellValue(row, 3, val);
							val.Printf("%.2f", mInts->GetValue(row,1));
							coordGrid->SetCellValue(row, 4, val);
							if (row>2) {
								val.Printf("%d", mInts->GetConnection(row,2)+1);
								coordGrid->SetCellValue(row, 5, val);
								val.Printf("%.2f", mInts->GetValue(row,2));
								coordGrid->SetCellValue(row, 6, val);
							}
						}
					}
					
					// everything is now up to date, but we don't need to update the molecule display
					Changed = false;
				} else {	//a change of a bond length or angle actually moves the atoms
					mInts->InternalsToCartesians(MainData, Prefs, row);
					MainData->ResetRotation();
				}
			}
		}
	}

	if (Changed)
		Parent->ResetModel(false);
	event.Skip();
}

/*!
 * wxEVT_GRID_SELECT_CELL event handler for ID_GRID
 */

void CoordinatesWindow::OnSelectCell( wxGridEvent& event )
{
	int row = event.GetRow();
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	//we seem to only get selection events and not also deselection events
	//so first clear off the list of selected cells
	for (int i=0; i<natoms; i++) {
		lFrame->SetAtomSelectState(i, false);
	}
	if ((row>=0)&&(row<natoms)) {
		lFrame->SetAtomSelectState(row, event.Selecting());
	}
	UpdateControls();
	event.Skip();
}

/*!
 * wxEVT_GRID_RANGE_SELECT event handler for ID_GRID
 */

void CoordinatesWindow::OnRangeSelect( wxGridRangeSelectEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	//we seem to only get selection events and not also deselection events
	//so first clear off the list of selected cells
	for (int i=0; i<natoms; i++) lFrame->SetAtomSelectState(i, false);
	if(event.Selecting()) {
		for (int i=event.GetTopRow(); i<=event.GetBottomRow(); i++) {
			lFrame->SetAtomSelectState(i, true);
		}
	}
	
	UpdateControls();
	event.Skip();
}

/*!
 * wxEVT_SIZE event handler for ID_GRID
 */

void CoordinatesWindow::OnSize( wxSizeEvent& event )
{
	wxSize s = event.GetSize();
	if (coordGrid) SizeCols(s);
    event.Skip();
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_DIALOG
 */

void CoordinatesWindow::OnCloseWindow( wxCloseEvent& event )
{
	Parent->CloseCoordsWindow();
}
/*!
 * wxEVT_UPDATE_UI event handler for wxID_COPY
 */

void CoordinatesWindow::OnCopyUpdate( wxUpdateUIEvent& event )
{
	event.Enable(false);
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_CLOSE
 */

void CoordinatesWindow::OnCloseClick( wxCommandEvent& event )
{
	Parent->CloseCoordsWindow();
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_PASTE
 */

void CoordinatesWindow::OnPasteUpdate( wxUpdateUIEvent& event )
{
	event.Enable(false);
}

bool CoordinatesWindow::ProcessEvent(wxEvent& event) 
{
	static wxEvent* s_lastEvent = NULL; 
	// Check for infinite recursion 
	if (& event == s_lastEvent) 
		return false; 
	if (event.IsCommandEvent() && 
		!event.IsKindOf(CLASSINFO(wxChildFocusEvent)) && 
		!event.IsKindOf(CLASSINFO(wxContextMenuEvent))) 
	{ 
		s_lastEvent = & event; 
		wxControl *focusWin = wxDynamicCast(FindFocus(), wxControl);        
		bool success = false; 
		if (focusWin) 
			success = focusWin->GetEventHandler() 
				->ProcessEvent(event); 
		if (!success)
			success = wxFrame::ProcessEvent(event);
		s_lastEvent = NULL;
		return success;
	} 
	else 
	{ 
		return wxFrame::ProcessEvent(event); 
	} 
} 

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for MMP_COPYCOORDSITEM
 */

void CoordinatesWindow::OnMmpCopycoordsitemClick( wxCommandEvent& event )
{
	Parent->CopyCoordinates(CoordType);
}


/*!
 * wxEVT_UPDATE_UI event handler for wxID_UNDO
 */

void CoordinatesWindow::OnUndoUpdate( wxUpdateUIEvent& event )
{
	event.Enable(false);
}

/*!
 * wxEVT_UPDATE_UI event handler for wxID_CUT
 */

void CoordinatesWindow::OnCutUpdate( wxUpdateUIEvent& event )
{
	event.Enable(false);
}

/*!
 * wxEVT_UPDATE_UI event handler for wxID_CLEAR
 */

void CoordinatesWindow::OnClearUpdate( wxUpdateUIEvent& event )
{
	event.Enable(coordGrid->IsSelection());
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_SELECTALL
 */

void CoordinatesWindow::OnSelectallClick( wxCommandEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	//we seem to only get selection events and not also deselection events
	//so first clear off the list of selected cells
	for (int i=0; i<natoms; i++) {
		lFrame->SetAtomSelectState(i, true);
		coordGrid->SelectRow(i, true);
	}
	
	UpdateControls();
}

/*!
 * wxEVT_UPDATE_UI event handler for wxID_SELECTALL
 */

void CoordinatesWindow::OnSelectallUpdate( wxUpdateUIEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	event.Enable((natoms>0));
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_STICKMENU
 */

void CoordinatesWindow::OnStickmenuClick( wxCommandEvent& event )
{
	OnStickbuttonClick(event);
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_STICKMENU
 */

void CoordinatesWindow::OnStickmenuUpdate( wxUpdateUIEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	event.Enable((natoms>0));
}
/*!
 * wxEVT_UPDATE_UI event handler for MMP_COPYCOORDSITEM
 */

void CoordinatesWindow::OnMmpCopycoordsitemUpdate( wxUpdateUIEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	event.Enable((natoms>0));
}

void CoordinatesWindow::OnActivate(wxActivateEvent & event) {
	if (event.GetActive()) {
		Parent->StopAnimations();
	}
	event.Skip();
}
