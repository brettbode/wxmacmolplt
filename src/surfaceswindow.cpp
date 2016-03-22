/////////////////////////////////////////////////////////////////////////////
// Name:        surfaceswindow.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Wed 19 Sep 13:37:29 2007
// RCS-ID:      
// Copyright:   (c) 2007 Iowa State University
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "surfaceswindow.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "mmp_gl.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "MolDisplayWin.h"
#include "Frame.h"

#include "SurfaceTypes.h"
#include "surfaceswindow.h"
#include "surfaceDlg.h"

#define ID_2D_ORBITAL_PANE 1
#define ID_3D_ORBITAL_PANE 2
#define ID_1D_TE_DENSITY_PANE 3
#define ID_2D_TE_DENSITY_PANE 4
#define ID_3D_TE_DENSITY_PANE 5
#define ID_2D_ME_POTENTIAL_PANE 6
#define ID_3D_ME_POTENTIAL_PANE 7
#define ID_2D_FILE_PANE 8
#define ID_3D_FILE_PANE 9

////@begin XPM images
////@end XPM images


/*!
 * SurfacesWindow type definition
 */

IMPLEMENT_CLASS( SurfacesWindow, wxFrame )


/*!
 * SurfacesWindow event table definition
 */

BEGIN_EVENT_TABLE( SurfacesWindow, wxFrame )

////@begin SurfacesWindow event table entries

	EVT_TEXT(ID_SURFTITLE, SurfacesWindow::OnChangeTitle)
	EVT_BUTTON( wxID_ADD, SurfacesWindow::OnAddClick )
	EVT_BUTTON( wxID_DELETE, SurfacesWindow::OnDeleteClick )
	EVT_MENU( wxID_CLOSE, SurfacesWindow::OnCloseEvent )

////@end SurfacesWindow event table entries

	EVT_CHOICEBOOK_PAGE_CHANGED(ID_SURFLISTBOOK, SurfacesWindow::OnSurflistbookPageChanged )

END_EVENT_TABLE()


/*!
 * SurfacesWindow constructors
 */

SurfacesWindow::SurfacesWindow()
{
	Init();
}

SurfacesWindow::SurfacesWindow( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
    Parent = parent;
    mPrefs = parent->GetPrefs();
	mData = parent->GetData();
	Create( parent, id, caption, pos, size, style );
}


/*!
 * SurfacesWindow creator
 */

bool SurfacesWindow::Create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SurfacesWindow creation
	wxFrame::Create( parent, id, caption, pos, size, style );

	CreateControls();
	/* if (GetSizer())  */
	/* {  */
		/* GetSizer()->SetSizeHints(this);  */
	/* }  */
	Centre();
////@end SurfacesWindow creation
	UpdateWindowTitle();
	return true;
}

/*!
 * Update the Window title in the event the file is saved
 */
void SurfacesWindow::UpdateWindowTitle(void) {
	wxString foo = Parent->GetTitle();
	foo += wxString(_(" ")) + wxString(SYMBOL_SURFACESWINDOW_TITLE);
	SetTitle(foo);
}


/*!
 * SurfacesWindow destructor
 */

SurfacesWindow::~SurfacesWindow()
{
	Parent->CloseSurfacesWindow();
}


/*!
 * Member initialisation
 */

void SurfacesWindow::Init()
{
////@begin SurfacesWindow member initialisation
	book = NULL;
	mDeleteButton = NULL;
////@end SurfacesWindow member initialisation
}


/*!
 * Control creation for SurfacesWindow
 */

void SurfacesWindow::CreateControls()
{    
////@begin SurfacesWindow content construction
	SurfacesWindow* itemFrame1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemFrame1->SetSizer(itemBoxSizer2);

	// wxPanel* itemPanel3 = new wxPanel( itemFrame1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL ); 
	// itemBoxSizer2->Add(itemPanel3, 1, wxALIGN_CENTER_HORIZONTAL | wxALL | wxEXPAND, 0); 

	// wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL); 
	// itemPanel3->SetSizer(itemBoxSizer4); 

	book = new wxChoicebook(this, ID_SURFLISTBOOK, wxDefaultPosition, wxDefaultSize, wxNB_TOP); // | wxSUNKEN_BORDER);

	itemBoxSizer2->Add(book, 2, wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 2);

	wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);

	wxButton* itemButton12 = new wxButton( this, wxID_ADD, _("&Add..."), wxDefaultPosition, wxDefaultSize, 0 );
	if (SurfacesWindow::ShowToolTips())
		itemButton12->SetToolTip(_("Click to add a new surface."));
	itemBoxSizer11->Add(itemButton12, 0, wxALIGN_BOTTOM|wxALL, 5);

	mDeleteButton = new wxButton( this, wxID_DELETE, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	if (SurfacesWindow::ShowToolTips())
		mDeleteButton->SetToolTip(_("Delete the selected surface"));
	itemBoxSizer11->Add(mDeleteButton, 0, wxALIGN_BOTTOM|wxALL, 5);
	itemBoxSizer2->Add(itemBoxSizer11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

////@end SurfacesWindow content construction

	wxMenuBar* menuBar = new wxMenuBar;
	wxMenu* lFileMenu = new wxMenu;
	lFileMenu->Append(wxID_NEW, wxT("&New\tCtrl+N"), _("Open a new empty window"));
	lFileMenu->Append(wxID_OPEN, wxT("&Open ...\tCtrl+O"), wxT("Open a file into a new window"));
	lFileMenu->Append(wxID_CLOSE, _("&Close Window\tCtrl+W"), wxEmptyString, wxITEM_NORMAL);
	lFileMenu->AppendSeparator();
	lFileMenu->Append(wxID_EXIT, _("&Quit\tCtrl+Q"), wxEmptyString, wxITEM_NORMAL);
	menuBar->Append(lFileMenu, _("&File"));
	wxMenu* lEditMenu = new wxMenu;
	lEditMenu->Append(wxID_UNDO, _("&Undo\tCtrl+Z"), _T(""), wxITEM_NORMAL);
	lEditMenu->AppendSeparator();
	lEditMenu->Append(wxID_COPY, _("&Copy\tCtrl+C"), wxEmptyString, wxITEM_NORMAL);
	lEditMenu->Append(wxID_PASTE, _("&Paste\tCtrl+V"), _T(""), wxITEM_NORMAL);
	lEditMenu->AppendSeparator();
	lEditMenu->Append(wxID_SELECTALL, _("&Select all\tCtrl+A"), _T(""), wxITEM_NORMAL);
	menuBar->Append(lEditMenu, _("&Edit"));
	lEditMenu->AppendSeparator();
	lEditMenu->Append(wxID_PREFERENCES, wxT("Global Pr&eferences"));

	wxMenu * menuWindow = new wxMenu;
	menuWindow->Append(MMP_MOLECULEDISPLAYWINDOW, wxT("&Molecule Display"), _("The primary molecule display"));
	menuWindow->Append(MMP_BONDSWINDOW, wxT("&Bonds"), _("View/edit the bonding within the molecule"));
	menuWindow->Append(MMP_COORDSWINDOW, wxT("&Coordinates"), _("View/edit cartesian or internal coordinates"));
	menuWindow->Append(MMP_ENERGYPLOTWINDOW, wxT("&Energy Plot"), _("A plot of the energy for each geometry"));
	menuWindow->Append(MMP_FREQUENCIESWINDOW, wxT("&Frequencies"), _("Plot the vibrational frequencies"));
	menuWindow->Append(MMP_INPUTBUILDERWINDOW, wxT("&Input Builder"), _T("Generate a GAMESS input file"));
	menuWindow->Append(MMP_ZMATRIXCALC, wxT("&Z-Matrix Calculator"), _("Compute bond lengths/angles or dihedrals between any set of atoms"));
	menuWindow->Append(MMP_LOCAL_PREFERENCES, wxT("Pr&eferences"), _T("Edit the preferences for this window"));
	menuBar->Append(menuWindow, wxT("&Subwindow"));

	wxMenu * menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, wxT("&About MacMolPlt..."), _T("Learn about MacMolPlt"));
	menuHelp->Append(wxID_HELP, wxT("&MacMolPlt Manual..."), _T("Brief documentation"));
	menuBar->Append(menuHelp, wxT("&Help"));
	itemFrame1->SetMenuBar(menuBar);
	
	Frame * lFrame = mData->GetCurrentFramePtr();
	long NumSurfaces = lFrame->GetNumSurfaces();
	if (NumSurfaces == 0) {
		int type = selectSurfaceType();
		if (type == -1) {
			book->Disable();
			book->AddPage(new wxPanel(book), _("No surfaces defined"), true);
		} else {
			addNewPane(type);
		}
	} else {
		Reset();
	}
}

void SurfacesWindow::OnChangeTitle(wxCommandEvent& event) {

	BaseSurfacePane* tempPane = (BaseSurfacePane *) book->GetCurrentPage();
	if (tempPane) {
		book->SetPageText(book->GetSelection(), tempPane->GetSurfaceName());
	} 
	
}

void SurfacesWindow::addNewPane(int type) {

	if (!book->IsEnabled()) {
		book->DeletePage(0);
		book->Enable();
	}

	BaseSurfacePane* tempPane = NULL;
	Surface* newSurface = NULL;
	
	switch (type) {
		case ID_2D_ORBITAL_PANE:
			newSurface = new Orb2DSurface(mPrefs);
			tempPane = new Orbital2DSurfPane(book, dynamic_cast<Orb2DSurface*>(newSurface), this);
			break;
			
		case ID_3D_ORBITAL_PANE:
			newSurface = new Orb3DSurface(mPrefs);
			tempPane = new Orbital3DSurfPane(book, dynamic_cast<Orb3DSurface*>(newSurface), this);
			break;

		case ID_1D_TE_DENSITY_PANE:
			newSurface = new TEDensity1DSurface(mPrefs);
			tempPane = new TEDensity1DSurfPane(book, dynamic_cast<TEDensity1DSurface*>(newSurface), this);
			break;
			
		case ID_2D_TE_DENSITY_PANE:
			newSurface = new TEDensity2DSurface(mPrefs);
			tempPane = new TEDensity2DSurfPane(book, dynamic_cast<TEDensity2DSurface*>(newSurface), this);
			break;
			
		case ID_3D_TE_DENSITY_PANE:
			newSurface = new TEDensity3DSurface(mPrefs);
			tempPane = new TEDensity3DSurfPane(book, dynamic_cast<TEDensity3DSurface*>(newSurface), this);
			break;
			
		case ID_2D_ME_POTENTIAL_PANE:
			newSurface = new MEP2DSurface(mPrefs);
			tempPane = new MEP2DSurfPane(book, dynamic_cast<MEP2DSurface*>(newSurface), this);
			break;
			
		case ID_3D_ME_POTENTIAL_PANE:
			newSurface = new MEP3DSurface(mPrefs);
			tempPane = new MEP3DSurfPane(book, dynamic_cast<MEP3DSurface*>(newSurface), this);
			break;
			
		case ID_2D_FILE_PANE:
			newSurface = new General2DSurface(mPrefs);
			tempPane = new General2DSurfPane(book, dynamic_cast<General2DSurface*>(newSurface), this);
			break;
			
		case ID_3D_FILE_PANE:
			newSurface = new General3DSurface(mPrefs);
			tempPane = new General3DSurfPane(book, dynamic_cast<General3DSurface*>(newSurface), this);
			break;
	}
	
	if (tempPane && newSurface) {
		Frame * lFrame = mData->GetCurrentFramePtr();
		lFrame->AppendSurface(newSurface);
		
		tempPane->SetVisibility(true);
		tempPane->setAllFrames(newSurface->GetSurfaceID() != 0);
	}
	
	if (type != -1) {
		wxString temp(newSurface->GetLabel(), wxConvUTF8);
		book->AddPage(tempPane, temp, true);
	}
	
	Fit();
	SetMinSize(GetBestSize());
	SetSize(GetMinSize());
	// GetSizer()->SetSizeHints(this); 
}

void SurfacesWindow::Reset(void) {
	//rebuild the list of surfaces
//	int oldpage = book->GetSelection();

	if (!book->IsEnabled()) {
		book->DeletePage(0);
		book->Enable();
	}

#if wxCHECK_VERSION(2,9,0)
	book->DeleteAllPages();
#else
	size_t npages = book->GetPageCount();
	for (size_t i = 0; i < npages; ++i) {
		book->DeletePage(0);
	}
#endif
	
	BaseSurfacePane *tempPane = NULL;
	MoleculeData *MainData = Parent->GetData();
	Frame *lFrame = MainData->GetCurrentFramePtr();
	long NumSurfaces = lFrame->GetNumSurfaces();
	if (NumSurfaces > 0) {
		for (int i=0; i<NumSurfaces; i++) {
			Surface *lSurf = lFrame->GetSurface(i);
			if (lSurf) {
				switch (lSurf->GetSurfaceType()) {
					case kOrb3DType:
						tempPane = new Orbital3DSurfPane(book, dynamic_cast<Orb3DSurface*>(lSurf), this);
						break;
					case kOrb2DType:
						tempPane = new Orbital2DSurfPane(book, dynamic_cast<Orb2DSurface*>(lSurf), this);
						break;
					case kTotalDensity3D:
						tempPane = new TEDensity3DSurfPane(book, dynamic_cast<TEDensity3DSurface*>(lSurf), this);
						break;
					case kTotalDensity2D:
						tempPane = new TEDensity2DSurfPane(book, dynamic_cast<TEDensity2DSurface*>(lSurf), this);
						break;
					case kTotalDensity1D:
						tempPane = new TEDensity1DSurfPane(book, dynamic_cast<TEDensity1DSurface*>(lSurf), this);
						break;
					case kMEP2D:
						tempPane = new MEP2DSurfPane(book, dynamic_cast<MEP2DSurface*>(lSurf), this);
						break;
					case kMEP3D:
						tempPane = new MEP3DSurfPane(book, dynamic_cast<MEP3DSurface*>(lSurf), this);
						break;
					case kGeneral2DSurface:
						tempPane = new General2DSurfPane(book, dynamic_cast<General2DSurface*>(lSurf), this);
						break;
					case kGeneral3DSurface:
						tempPane = new General3DSurfPane(book, dynamic_cast<General3DSurface*>(lSurf), this);
						break;
				}
				wxString temp(lSurf->GetLabel(), wxConvUTF8);
				tempPane->Fit();
				book->AddPage(tempPane, temp, true);
			}
		}
	} else {
		book->Disable();
		book->AddPage(new wxPanel(book), _("No surfaces defined"), true);
	}

	Fit();
	SetMinSize(GetBestSize());
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE
 */

void SurfacesWindow::OnDeleteClick( wxCommandEvent& event ) {
	Frame * lFrame = mData->GetCurrentFramePtr();
	
	int targetSurf = book->GetSelection();
	if (targetSurf == wxNOT_FOUND) {
		return;
	}

	lFrame->DeleteSurface(targetSurf);
	book->DeletePage(targetSurf);
	if (!book->GetPageCount()) {
		book->Disable();
		book->AddPage(new wxPanel(book), _("No surfaces defined"), true);
	}
	Parent->UpdateModelDisplay();
	
	Fit();
	SetMinSize(GetBestSize());
	SetSize(GetMinSize());
	// GetSizer()->SetSizeHints(this); 
}


/*!
 * Should we show tooltips?
 */

bool SurfacesWindow::ShowToolTips()
{
	return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SurfacesWindow::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin SurfacesWindow bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end SurfacesWindow bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SurfacesWindow::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin SurfacesWindow icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end SurfacesWindow icon retrieval
}


/*!
 * wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED event handler for ID_SURFLISTBOOK
 */
void SurfacesWindow::OnSurflistbookPageChanged(wxChoicebookEvent& event) {

	BaseSurfacePane* tempPane = dynamic_cast<BaseSurfacePane *>(book->GetCurrentPage());
	if (tempPane) {
		tempPane->PageIsNowActive();	//make sure the update button is the default
	}

	// Fit(); 
	// SetMinSize(GetBestSize()); 

}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD
 */

void SurfacesWindow::OnAddClick( wxCommandEvent& event )
{
	int surfTypeId = selectSurfaceType();
	if (surfTypeId > 0) {
		addNewPane(surfTypeId);
	}
}

void SurfacesWindow::SurfaceUpdated(void) {
	//Do we need to generate a frame changed event here???
	BaseSurfacePane* tempPane = (BaseSurfacePane * ) book->GetCurrentPage();
	if (tempPane) {
		book->SetPageText(book->GetSelection(), tempPane->GetSurfaceName());
		//The previous line does not immediately update the list text???
	} 
	Parent->UpdateModelDisplay();
}

int SurfacesWindow::selectSurfaceType() {
	std::vector<wxString> allOptions;
	int numOptions = 2;

	if (mData->OrbSurfacePossible()) {
		allOptions.push_back(_T("2D Orbital"));
		allOptions.push_back(_T("3D Orbital"));
		numOptions += 2;
	}

	if (mData->TotalDensityPossible()) {
		allOptions.push_back(_T("1D Total Electron Density"));
		allOptions.push_back(_T("2D Total Electron Density"));
		allOptions.push_back(_T("3D Total Electron Density"));
		numOptions += 3;
	}

	if (mData->MEPCalculationPossible()) {
		allOptions.push_back(_T("2D Molecular Electrostatic Potential"));
		allOptions.push_back(_T("3D Molecular Electrostatic Potential"));
		numOptions += 2;
	}
	
	allOptions.push_back(_T("General 2D from File"));
	allOptions.push_back(_T("General 3D from File"));
	
	wxSingleChoiceDialog dialog(this, _T("Choose the type of surface to create:"),
								_T("Surface Type"),
								numOptions, &allOptions.front());
	
	dialog.SetSelection(0);
	
	wxString temp;
	if (dialog.ShowModal() == wxID_OK)
		temp= dialog.GetStringSelection();
	
	if (temp.Cmp(wxT("2D Orbital")) == 0)
		return ID_2D_ORBITAL_PANE;
	if (temp.Cmp(wxT("3D Orbital")) == 0)
		return ID_3D_ORBITAL_PANE;
	if (temp.Cmp(wxT("1D Total Electron Density")) == 0)
		return ID_1D_TE_DENSITY_PANE;
	if (temp.Cmp(wxT("2D Total Electron Density")) == 0)
		return ID_2D_TE_DENSITY_PANE;
	if (temp.Cmp(wxT("3D Total Electron Density")) == 0)
		return ID_3D_TE_DENSITY_PANE;
	if (temp.Cmp(wxT("2D Molecular Electrostatic Potential")) == 0)
		return ID_2D_ME_POTENTIAL_PANE;
	if (temp.Cmp(wxT("3D Molecular Electrostatic Potential")) == 0)
		return ID_3D_ME_POTENTIAL_PANE;
	if (temp.Cmp(wxT("General 2D from File")) == 0)
		return ID_2D_FILE_PANE;
	if (temp.Cmp(wxT("General 3D from File")) == 0)
		return ID_3D_FILE_PANE;
	
	return -1;
	
}

/*!
 * wxEVT_CLOSE menu event handler for SurfacesWindow
 */

void SurfacesWindow::OnCloseEvent( wxCommandEvent& /* event */ )
{
	MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	parent->CloseSurfacesWindow();
}

