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
#include "Frame.h"
#include "Internals.h"
#include "myFiles.h"

#include "coordinateswindow.h"
#include "changeatomorderdialog.h"

////@begin XPM images
////@end XPM images

/*!
 * CoordinatesWindow type definition
 */

IMPLEMENT_DYNAMIC_CLASS( CoordinatesWindow, wxFrame )

BEGIN_EVENT_TABLE( CoordinatesWindow, wxFrame )

////@begin CoordinatesWindow event table entries
	EVT_CLOSE( CoordinatesWindow::OnCloseWindow )

	EVT_MENU( wxID_CLOSE, CoordinatesWindow::OnCloseClick )

	EVT_UPDATE_UI( wxID_UNDO, CoordinatesWindow::OnUndoUpdate )

	EVT_UPDATE_UI( wxID_CUT, CoordinatesWindow::OnCutUpdate )

	EVT_UPDATE_UI( wxID_COPY, CoordinatesWindow::OnCopyUpdate )

	EVT_MENU( MMP_COPYCOORDSITEM, CoordinatesWindow::OnMmpCopycoordsitemClick )
	EVT_UPDATE_UI( MMP_COPYCOORDSITEM, CoordinatesWindow::OnMmpCopycoordsitemUpdate )
	EVT_UPDATE_UI( MMP_COPYNWCOORDS, CoordinatesWindow::OnCopyNWCoordsItemUpdate )

	EVT_UPDATE_UI( wxID_PASTE, CoordinatesWindow::OnPasteUpdate )

	EVT_UPDATE_UI( wxID_CLEAR, CoordinatesWindow::OnClearUpdate )

	EVT_MENU( wxID_SELECTALL, CoordinatesWindow::OnSelectallClick )
	EVT_UPDATE_UI( wxID_SELECTALL, CoordinatesWindow::OnSelectallUpdate )

	EVT_MENU( ID_STICKMENU, CoordinatesWindow::OnStickmenuClick )
	EVT_UPDATE_UI( ID_STICKMENU, CoordinatesWindow::OnStickmenuUpdate )

	EVT_MENU( ID_REORDERCOORDITEM, CoordinatesWindow::OnReordercoorditemClick )
	EVT_UPDATE_UI( ID_REORDERCOORDITEM, CoordinatesWindow::OnReordercoorditemUpdate )

	EVT_BUTTON( wxID_ADD, CoordinatesWindow::OnAddClick )

	EVT_BUTTON( wxID_DELETE, CoordinatesWindow::OnDeleteClick )

	EVT_BUTTON( ID_BONDBUTTON, CoordinatesWindow::OnBondbuttonClick )

	EVT_BUTTON( ID_STICKBUTTON, CoordinatesWindow::OnStickbuttonClick )

	EVT_CHOICE( ID_COORDCHOICE1, CoordinatesWindow::OnCoordchoice1Selected )

#if wxCHECK_VERSION(3, 0, 0)
	EVT_GRID_CELL_CHANGED( CoordinatesWindow::OnCellChange )
#else
	EVT_GRID_CELL_CHANGE( CoordinatesWindow::OnCellChange )
#endif
	EVT_GRID_SELECT_CELL( CoordinatesWindow::OnSelectCell )
	EVT_GRID_RANGE_SELECT( CoordinatesWindow::OnRangeSelect )

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
 * CoordinatesWindow destructors
 */

CoordinatesWindow::~CoordinatesWindow( )
{
//  Parent->SetHighliteMode(false);
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
	needClearAll = true;

////@begin CoordinatesWindow creation
//	I do not see why this was here.
//	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxFrame::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end CoordinatesWindow creation
	UpdateWindowTitle();

	return true;
}

/*!
 * Update the Window title in the event the file is saved
 */
void CoordinatesWindow::UpdateWindowTitle(void) {
	wxString foo = Parent->GetTitle();
	foo += wxString(_(" ")) + wxString(SYMBOL_COORDINATESWINDOW_TITLE);
	SetTitle(foo);
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
	wxMenu* lEditMenu = new wxMenu;
	lEditMenu->Append(wxID_UNDO, _("&Undo\tCtrl+Z"), _T(""), wxITEM_NORMAL);
	lEditMenu->Enable(wxID_UNDO, false);
	lEditMenu->AppendSeparator();
	lEditMenu->Append(wxID_CUT, _("Cu&t\tCtrl+X"), _T(""), wxITEM_NORMAL);
	lEditMenu->Enable(wxID_CUT, false);
	lEditMenu->Append(wxID_COPY, _("&Copy\tCtrl+C"), _T(""), wxITEM_NORMAL);
	lEditMenu->Enable(wxID_COPY, false);
	lEditMenu->Append(MMP_COPYCOORDSITEM, _("Copy Coordinates"), _("Copy the full set of coordinates with the current coordinate type."), wxITEM_NORMAL);
	lEditMenu->Enable(MMP_COPYCOORDSITEM, false);
	lEditMenu->Append(MMP_COPYNWCOORDS, wxT("Copy NWChem Coordinates"), wxT("Copy the current set of coordinates as plain text in NWChem style"));
	lEditMenu->Enable(MMP_COPYNWCOORDS, false);
	lEditMenu->Append(wxID_PASTE, _("&Paste\tCtrl+V"), _T(""), wxITEM_NORMAL);
	lEditMenu->Enable(wxID_PASTE, false);
	lEditMenu->Append(wxID_CLEAR, _("&Delete\tDel"), _T(""), wxITEM_NORMAL);
	lEditMenu->Enable(wxID_CLEAR, false);
	lEditMenu->AppendSeparator();
	lEditMenu->Append(wxID_SELECTALL, _("&Select all\tCtrl+A"), _T(""), wxITEM_NORMAL);
	lEditMenu->Enable(wxID_SELECTALL, false);
	lEditMenu->AppendSeparator();
	lEditMenu->Append(wxID_PREFERENCES, wxT("Global Pr&eferences"));
	menuBar->Append(lEditMenu, _("Edit"));

	wxMenu* itemMenu17 = new wxMenu;
	itemMenu17->Append(ID_STICKMENU, _("Use Coordinates for Reference"), _("Makes the current rotated coordinates the reference coordinates."), wxITEM_NORMAL);
	itemMenu17->Append(ID_REORDERCOORDITEM, _("&Change selected atoms order..."), _T(""), wxITEM_NORMAL);
	menuBar->Append(itemMenu17, _("Coordinates"));

	wxMenu * menuWindow = new wxMenu;
	menuWindow->Append(MMP_MOLECULEDISPLAYWINDOW, wxT("&Molecule Display"), _("The primary molecule display"));
	menuWindow->Append(MMP_BONDSWINDOW, wxT("&Bonds"), _("View/edit the bonding within the molecule"));
	menuWindow->Append(MMP_ENERGYPLOTWINDOW, wxT("&Energy Plot"), _("A plot of the energy for each geometry"));
	menuWindow->Append(MMP_FREQUENCIESWINDOW, wxT("&Frequencies"), _("Plot the vibrational frequencies"));
	menuWindow->Append(MMP_INPUTBUILDERWINDOW, wxT("&Input Builder"), _T("Generate a GAMESS input file"));
	menuWindow->Append(MMP_SURFACESWINDOW, wxT("&Surfaces"), _T("Add/Edit/Remove various surface types"));
	menuWindow->Append(MMP_ZMATRIXCALC, wxT("&Z-Matrix Calculator"), _("Compute bond lengths/angles or dihedrals between any set of atoms"));
	menuWindow->Append(MMP_LOCAL_PREFERENCES, wxT("Pr&eferences"), _T("Edit the preferences for this window"));
	menuBar->Append(menuWindow, wxT("&Subwindow"));
	
	wxMenu * menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, wxT("&About MacMolPlt..."), _T("Learn about MacMolPlt"));
	menuHelp->Append(wxID_HELP, wxT("&MacMolPlt Manual..."), _T("Brief documentation"));
	menuBar->Append(menuHelp, wxT("&Help"));

	itemFrame1->SetMenuBar(menuBar);

	wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxVERTICAL);
	itemFrame1->SetSizer(itemBoxSizer20);

	wxPanel* itemPanel21 = new wxPanel( itemFrame1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
	itemBoxSizer20->Add(itemPanel21, 1, wxGROW, 0);

	wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxVERTICAL);
	itemPanel21->SetSizer(itemBoxSizer22);

	wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer22->Add(itemBoxSizer23, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxButton* itemButton24 = new wxButton( itemPanel21, wxID_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
	if (CoordinatesWindow::ShowToolTips())
		itemButton24->SetToolTip(_("Add a new atom to the list of coordinates."));
	itemBoxSizer23->Add(itemButton24, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	deleteButton = new wxButton( itemPanel21, wxID_DELETE, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	if (CoordinatesWindow::ShowToolTips())
		deleteButton->SetToolTip(_("Delete the selected atoms."));
	itemBoxSizer23->Add(deleteButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	BondButton = new wxButton( itemPanel21, ID_BONDBUTTON, _("Bond"), wxDefaultPosition, wxDefaultSize, 0 );
	if (CoordinatesWindow::ShowToolTips())
		BondButton->SetToolTip(_("Apply the default bonding criteria."));
	itemBoxSizer23->Add(BondButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton27 = new wxButton( itemPanel21, ID_STICKBUTTON, _("Stick"), wxDefaultPosition, wxDefaultSize, 0 );
	if (CoordinatesWindow::ShowToolTips())
		itemButton27->SetToolTip(_("Use the current screen rotation as the reference frame."));
	itemBoxSizer23->Add(itemButton27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* itemStaticText28 = new wxStaticText( itemPanel21, wxID_STATIC, _("Coord. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer23->Add(itemStaticText28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString coordTypeChoiceStrings;
	coordTypeChoiceStrings.Add(_("Cartesian"));
	coordTypeChoiceStrings.Add(_("Z-Matrix"));
	coordTypeChoice = new wxChoice( itemPanel21, ID_COORDCHOICE1, wxDefaultPosition, wxDefaultSize, coordTypeChoiceStrings, 0 );
	if (CoordinatesWindow::ShowToolTips())
		coordTypeChoice->SetToolTip(_("Changes the displayed coordinate type."));
	itemBoxSizer23->Add(coordTypeChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	coordGrid = new wxGrid( itemPanel21, ID_COORDGRID, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL );
	if (CoordinatesWindow::ShowToolTips())
		coordGrid->SetToolTip(_("Double click on a field to edit"));
	coordGrid->SetDefaultColSize(50);
	coordGrid->SetDefaultRowSize(25);
	coordGrid->SetColLabelSize(25);
	coordGrid->SetRowLabelSize(50);
	coordGrid->CreateGrid(5, 5, wxGrid::wxGridSelectRows);
	itemBoxSizer22->Add(coordGrid, 1, wxGROW|wxALL, 0);

	wxStatusBar* itemStatusBar31 = new wxStatusBar( itemFrame1, ID_STATUSBAR1, wxST_SIZEGRIP|wxNO_BORDER );
	itemStatusBar31->SetFieldsCount(1);
	itemFrame1->SetStatusBar(itemStatusBar31);

	// Connect events and objects
	coordGrid->Connect(ID_COORDGRID, wxEVT_SIZE, wxSizeEventHandler(CoordinatesWindow::OnSize), NULL, this);
////@end CoordinatesWindow content construction
	coordGrid->SetDefaultCellAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
	SetupGridColumns();
	FrameChanged();
	UpdateControls();
    Centre();
	wxSize s(50, 150);
	coordGrid->SetMinSize(s);
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
		if (lFrame->GetAtomSelection(i)) {
			selectionActive = true;
		}
	}
	deleteButton->Enable(selectionActive);
	coordTypeChoice->SetSelection(CoordType);
}
void CoordinatesWindow::SetupGridColumns(void) {
	bool save = needClearAll;
	needClearAll = false;
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	bool temp = false;
	if (natoms>0) temp = lFrame->GetAtomSelection(0);
	coordGrid->BeginBatch();
	if (coordGrid->GetNumberRows() > 0)
		coordGrid->DeleteRows(0, coordGrid->GetNumberRows(), true);
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
		lFrame->SetAtomSelection(0, temp);
	}
	coordGrid->EndBatch();
	needClearAll = save;
}

void CoordinatesWindow::FrameChanged(void) {
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	bool save = needClearAll;
	needClearAll = false;
	coordGrid->BeginBatch();
	//clear off any extra rows
	if (coordGrid->GetNumberRows() > natoms)
		coordGrid->DeleteRows(0, coordGrid->GetNumberRows()-natoms, true);
	coordGrid->HideCellEditControl();
	if (natoms > 0) {
		if (coordGrid->GetNumberRows() < natoms)
			coordGrid->InsertRows(0, natoms - coordGrid->GetNumberRows(), true);
		coordGrid->ClearSelection();
		wxString buf;
		Internals * internals = NULL;
		MOPacInternals * mInts = NULL;
		//If using internal coordinates set them up and update the stored values.
		if (CoordType != 0) {
			internals = MainData->GetInternalCoordinates();
			if (internals)
				mInts = internals->GetMOPacStyle();
			if (mInts)
				mInts->CartesiansToInternals(MainData);
		}
		for (long i=0; i<natoms; i++) {
			buf.Printf(wxT("%ld"), (i+1));
			coordGrid->SetRowLabelValue(i, buf);
			Prefs->GetAtomLabel(lFrame->GetAtomType(i)-1, buf);
			coordGrid->SetCellValue(i, 0, buf);
			if (lFrame->GetAtomSelection(i)) coordGrid->SelectRow(i, true);
			if (CoordType == 0) {
				CPoint3D pos;
				lFrame->GetAtomPosition(i, pos);
				buf.Printf(wxT("%f"), pos.x);
				coordGrid->SetCellValue(i, 1, buf);
				buf.Printf(wxT("%f"), pos.y);
				coordGrid->SetCellValue(i, 2, buf);
				buf.Printf(wxT("%f"), pos.z);
				coordGrid->SetCellValue(i, 3, buf);
			} else {
				if (mInts) {
					if (i>0) {
						buf.Printf(wxT("%ld"), mInts->GetConnection(i,0)+1);
						coordGrid->SetCellValue(i, 1, buf);
		// These SetReadOnly to false calls seem to be both unnecessary and very costly
		//				coordGrid->SetReadOnly(i, 1, false);
						buf.Printf(wxT("%f"), mInts->GetValue(i,0));
						coordGrid->SetCellValue(i, 2, buf);
		//				coordGrid->SetReadOnly(i, 2, false);
						if (i>1) {
							buf.Printf(wxT("%ld"), mInts->GetConnection(i,1)+1);
							coordGrid->SetCellValue(i, 3, buf);
		//					coordGrid->SetReadOnly(i, 3, false);
							buf.Printf(wxT("%.2f"), mInts->GetValue(i,1));
							coordGrid->SetCellValue(i, 4, buf);
		//					coordGrid->SetReadOnly(i, 4, false);
							if (i>2) {
								buf.Printf(wxT("%ld"), mInts->GetConnection(i,2)+1);
								coordGrid->SetCellValue(i, 5, buf);
		//						coordGrid->SetReadOnly(i, 5, false);
								buf.Printf(wxT("%.2f"), mInts->GetValue(i,2));
								coordGrid->SetCellValue(i, 6, buf);
		//						coordGrid->SetReadOnly(i, 6, false);
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
	coordGrid->EndBatch();
	needClearAll = save;
}

void CoordinatesWindow::SizeCols(wxSize & s) {
	if (coordGrid->GetNumberCols()<=0) return;
	int width = s.GetWidth() - 66;	//subtract off the row labels and scroll
	if (CoordType == 0) {
		if (coordGrid->GetNumberCols()<4) return;
		int a = (int) (width/6.0);
		int b = (int) ((width-a)/3.0);
		coordGrid->SetColSize(0, a);
		coordGrid->SetColSize(1, b);
		coordGrid->SetColSize(2, b);
		coordGrid->SetColSize(3, width-(a+2*b));
	} else {
		//Apparently this is getting called while deleting rows before redoing the number of columns
		//So punt here if the number of columns doesn't match the expected number.
		if (coordGrid->GetNumberCols()<7) {
			return;
		}
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

void CoordinatesWindow::UpdateSelection(bool mode) {
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();

	needClearAll = mode;

	std::vector<int> selected_ids;
	int visibleRow = 0;

	for (long i=0; i<natoms; i++) 
	if (lFrame->GetAtomSelection(i)) {
		selected_ids.push_back(i);
		visibleRow = i;
	}
	//remember the selected atom before clearing selections

	if (natoms != coordGrid->GetNumberRows()) {
		FrameChanged();
		visibleRow = natoms - 1;
	}

	if (mode)
		coordGrid->ClearSelection();

	for (unsigned int i = 0; i < selected_ids.size(); ++i)
		coordGrid->SelectRow(selected_ids[i], true);

	Layout();
	coordGrid->MakeCellVisible(visibleRow, 0);

	needClearAll = true;
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
	Parent->CreateFrameSnapShot();
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	for (int i=0; i<natoms; i++) lFrame->SetAtomSelection(i, false);
	CPoint3D p = CPoint3D(0.0f, 0.0f, 0.0);
	MainData->NewAtom(1, p);
	lFrame->SetAtomSelection(natoms, true);
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
	Parent->CreateFrameSnapShot();
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	for (int i=(natoms-1); i>=0; i--) {
		if (lFrame->GetAtomSelection(i)) {
			int j = MainData->DeleteAtom(i);
			//restart the scan if more than 1 atom is removed.
			if ((j==0)&&(i!=0)) i = natoms-1;
		}
	}
	FrameChanged();
	UpdateControls();
	Parent->ResetModel(false);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_BONDBUTTON
 */

void CoordinatesWindow::OnBondbuttonClick( wxCommandEvent& event )
{
	Parent->CreateFrameSnapShot();
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	Progress lProg;
	lFrame->SetBonds(Prefs, true, &lProg);
	Parent->BondsChanged();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_STICKBUTTON
 */

void CoordinatesWindow::OnStickbuttonClick( wxCommandEvent& event )
{
	Parent->CreateFrameSnapShot();
	MoleculeData * MainData = Parent->GetData();
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
			} //else
			//This is unnecessary as it is aleady done in FrameChanged()
			//	mInts->CartesiansToInternals(MainData);
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
	wxString val = coordGrid->GetCellValue(row, col);
	bool Changed = false;
	if (col == 0) {	//Atomic label
		long atomnum;
		if (val.ToLong(&atomnum)) {	//number instead of symbol
			if ((atomnum < 1)||(atomnum>107)) atomnum = -1;//Not a correct Atomic #
		} else {
			//I am not quite following the docs on wxString coversion to ascii c str
			//ToAscii doesn't seem to convert to ascii in a unicode build
      //      const char *tmpChar = val.ToAscii();
			//mb_str is the recommend method and seems to work
			const char *  tmpChar = val.mb_str();
			atomnum = ::SetAtomType((const unsigned char *)tmpChar);//Change the symbol to Atomic #
			if ((atomnum < 1)||(atomnum>107)) atomnum = -1;//Not a correct Atomic label
		}
		if ((atomnum > -1)&&(atomnum!=lFrame->GetAtomType(row))) {//A atom type was found so change the type
			Parent->CreateFrameSnapShot();
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
				if (Changed) {
					Parent->CreateFrameSnapShot();
					lFrame->SetAtomPosition(row, pos);
				}
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
							Parent->CreateFrameSnapShot();
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
							Parent->CreateFrameSnapShot();
							mInts->SetValue(row, 0, fval);
							Changed = true;
						}
						break;
					case 3:
						if (newid != mInts->GetConnection(row, 1)) {
							Parent->CreateFrameSnapShot();
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
							Parent->CreateFrameSnapShot();
							mInts->SetValue(row, 1, fval);
							Changed = true;
						}
						break;
					case 5:
						if (newid != mInts->GetConnection(row, 2)) {
							Parent->CreateFrameSnapShot();
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
							Parent->CreateFrameSnapShot();
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
						val.Printf(wxT("%d"), mInts->GetConnection(row,0)+1);
						coordGrid->SetCellValue(row, 1, val);
						val.Printf(wxT("%f"), mInts->GetValue(row,0));
						coordGrid->SetCellValue(row, 2, val);
						if (row>1) {
							val.Printf(wxT("%d"), mInts->GetConnection(row,1)+1);
							coordGrid->SetCellValue(row, 3, val);
							val.Printf(wxT("%.2f"), mInts->GetValue(row,1));
							coordGrid->SetCellValue(row, 4, val);
							if (row>2) {
								val.Printf(wxT("%d"), mInts->GetConnection(row,2)+1);
								coordGrid->SetCellValue(row, 5, val);
								val.Printf(wxT("%.2f"), mInts->GetValue(row,2));
								coordGrid->SetCellValue(row, 6, val);
							}
						}
					}
					
					// everything is now up to date, but we don't need to update the molecule display
					Changed = false;
				} else {	//a change of a bond length or angle actually moves the atoms
					mInts->InternalsToCartesians(MainData, Prefs, row);
					MainData->RotateToPrincipleOrientation(Prefs);
					MainData->StickCoordinates();
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
void CoordinatesWindow::OnSelectCell( wxGridEvent& event ) {
	if (!needClearAll)
		return;

	int row = event.GetRow();
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	//we seem to only get selection events and not also deselection events
	//so first clear off the list of selected cells
	for (int i=0; i<natoms; i++) {
		lFrame->SetAtomSelection(i, false);
	}
	if ((row>=0)&&(row<natoms)) {
		lFrame->SetAtomSelection(row, event.Selecting());
	}
	UpdateControls();

	Parent->ReleaseLists();
	Parent->ResetView();

	event.Skip();
}

/*!
 * wxEVT_GRID_RANGE_SELECT event handler for ID_GRID
 */

void CoordinatesWindow::OnRangeSelect( wxGridRangeSelectEvent& event ) {
	if (!needClearAll)
		return;

	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	//we seem to only get selection events and not also deselection events
	//so first clear off the list of selected cells
	//for (int i=0; i<natoms; i++) lFrame->SetAtomSelection(i, false);

	for (int i=0; i<natoms; i++) 
		lFrame->SetAtomSelection(i, coordGrid->IsInSelection(i, 1));

	UpdateControls();

	Parent->ReleaseLists();
	Parent->ResetView();

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
		lFrame->SetAtomSelection(i, true);
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

void CoordinatesWindow::OnCopyNWCoordsItemUpdate( wxUpdateUIEvent& event )
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


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_REORDERCOORDITEM
 */

void CoordinatesWindow::OnReordercoorditemClick( wxCommandEvent& event )
{
	ChangeAtomOrderDialog * dlg = new ChangeAtomOrderDialog(Parent);
	dlg->ShowModal();
	dlg->Destroy();
}


/*!
 * wxEVT_UPDATE_UI event handler for ID_REORDERCOORDITEM
 */

void CoordinatesWindow::OnReordercoorditemUpdate( wxUpdateUIEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long natoms = lFrame->GetNumAtoms();
	event.Enable((natoms>0)&&coordGrid->IsSelection());
}

