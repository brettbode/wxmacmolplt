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

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes
#include <wx/choicebk.h>

#include "MolDisplayWin.h"
#include "Frame.h"

#include "SurfaceTypes.h"
#include "surfaceswindow.h"
#include "surfaceDlg.h"

#define ID_2D_ORBITAL_PANE 1
#define ID_3D_ORBITAL_PANE 2
#define ID_2D_TE_DENSITY_PANE 3
#define ID_3D_TE_DENSITY_PANE 4
#define ID_2D_ME_POTENTIAL_PANE 5
#define ID_3D_ME_POTENTIAL_PANE 6
#define ID_2D_FILE_PANE 7
#define ID_3D_FILE_PANE 8

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
	EVT_TEXT( ID_SURFTITLE, SurfacesWindow::OnSurftitleUpdated )

	EVT_CHECKBOX( ID_VISIBLECHECK, SurfacesWindow::OnVisiblecheckClick )

	EVT_CHECKBOX( ID_ALLFRAMECHECK, SurfacesWindow::OnAllframecheckClick )

	EVT_BUTTON( wxID_ADD, SurfacesWindow::OnAddClick )

	EVT_BUTTON( wxID_DELETE, SurfacesWindow::OnDeleteClick )

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
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end SurfacesWindow creation
	return true;
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
	surfTitleEdit = NULL;
	visibleCheck = NULL;
	allFrameCheck = NULL;
	book = NULL;
	mDeleteButton = NULL;
////@end SurfacesWindow member initialisation
	setValueCalled = false;
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

	wxPanel* itemPanel3 = new wxPanel( itemFrame1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
	itemBoxSizer2->Add(itemPanel3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);

	wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
	itemPanel3->SetSizer(itemBoxSizer4);

	wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer4->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	surfTitleEdit = new wxTextCtrl( itemPanel3, ID_SURFTITLE, _T(""), wxDefaultPosition, wxSize(250, -1), 0 );
	if (SurfacesWindow::ShowToolTips())
		surfTitleEdit->SetToolTip(_("You may change the label to anything you like."));
	itemBoxSizer5->Add(surfTitleEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	visibleCheck = new wxCheckBox( itemPanel3, ID_VISIBLECHECK, _("Visible"), wxDefaultPosition, wxDefaultSize, 0 );
	visibleCheck->SetValue(false);
	if (SurfacesWindow::ShowToolTips())
		visibleCheck->SetToolTip(_("Uncheck to hide the surface without deleting it."));
	itemBoxSizer5->Add(visibleCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	allFrameCheck = new wxCheckBox( itemPanel3, ID_ALLFRAMECHECK, _("All Frames"), wxDefaultPosition, wxDefaultSize, 0 );
	allFrameCheck->SetValue(false);
	if (SurfacesWindow::ShowToolTips())
		allFrameCheck->SetToolTip(_("Check to apply this surface to all frames."));
	itemBoxSizer5->Add(allFrameCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer4->Add(itemBoxSizer9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	book = new wxChoicebook( itemPanel3, ID_SURFLISTBOOK, wxDefaultPosition, wxDefaultSize, wxNB_TOP|wxSUNKEN_BORDER );

	itemBoxSizer9->Add(book, 4, wxALIGN_CENTER_VERTICAL|wxALL, 2);

	wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer4->Add(itemBoxSizer11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxButton* itemButton12 = new wxButton( itemPanel3, wxID_ADD, _("&Add..."), wxDefaultPosition, wxDefaultSize, 0 );
	if (SurfacesWindow::ShowToolTips())
		itemButton12->SetToolTip(_("Click to add a new surface."));
	itemBoxSizer11->Add(itemButton12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mDeleteButton = new wxButton( itemPanel3, wxID_DELETE, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
	if (SurfacesWindow::ShowToolTips())
		mDeleteButton->SetToolTip(_("Delete the selected surface"));
	itemBoxSizer11->Add(mDeleteButton, 0, wxALIGN_BOTTOM|wxALL, 5);

////@end SurfacesWindow content construction

	Frame * lFrame = mData->GetCurrentFramePtr();
	long NumSurfaces = lFrame->GetNumSurfaces();
	if (NumSurfaces == 0) {
		int type = selectSurfaceType();
		addNewPane(type);
	} else {
		Reset();
	}
}

void SurfacesWindow::addNewPane(int type) {
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
	
	if ( tempPane && newSurface) {
		Frame * lFrame = mData->GetCurrentFramePtr();
		lFrame->AppendSurface(newSurface);
		
		tempPane->SetVisibility(true);
		visibleCheck->SetValue(true);
		
		allFrameCheck->SetValue((newSurface->GetSurfaceID() != 0));	 
	} else {
		visibleCheck->Disable();
		allFrameCheck->Disable();
	}
	
	if ( type != -1) {
		wxString temp(newSurface->GetLabel(), wxConvUTF8);
		book->AddPage(tempPane, temp, true);
	}
	
	book->Fit();
	
	GetSizer()->SetSizeHints(this);
}
void SurfacesWindow::Reset(void) {
	//rebuild the list of surfaces
	int oldpage = book->GetSelection();
	book->DeleteAllPages();
	
	BaseSurfacePane* tempPane = NULL;
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	long NumSurfaces = lFrame->GetNumSurfaces();
	if (NumSurfaces > 0 ) {
		for (int i=0; i<NumSurfaces; i++) {
			Surface * lSurf = lFrame->GetSurface(i);
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
		wxString temp(_("No surface chosen."));
#if wxCHECK_VERSION(2, 8, 0)
		surfTitleEdit->ChangeValue(temp);
#else
		setValueCalled = true;
		surfTitleEdit->SetValue(temp);
#endif
	}
	book->Fit();
	
	GetSizer()->SetSizeHints(this);
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
	Parent->UpdateModelDisplay();
	if (book->GetPageCount() <= 0) {
		visibleCheck->Disable();
		allFrameCheck->Disable();
#if wxCHECK_VERSION(2, 8, 0)
		surfTitleEdit->ChangeValue(wxT(""));
#else
		setValueCalled = true;
		surfTitleEdit->SetValue(wxT(""));
#endif
	}
	book->Fit();
	
	GetSizer()->SetSizeHints(this);
	Fit();
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
void SurfacesWindow::OnSurflistbookPageChanged( wxChoicebookEvent& event )
{
	wxString temp(_("No surface chosen."));
	BaseSurfacePane* tempPane = (BaseSurfacePane * ) book->GetCurrentPage();
	if (tempPane) {
	//	GetSizer()->Fit(this);
	//	MainPanel->Fit();
		Fit();
		
		Surface * tempSurf = tempPane->GetTargetSurface();
		wxString t2(tempSurf->GetLabel(), wxConvUTF8);
		temp = t2;
		visibleCheck->SetValue(tempPane->GetVisibility());
		
		int surfType = tempSurf->GetSurfaceType();
		if ( surfType == kGeneral2DSurface || surfType == kGeneral3DSurface)
			allFrameCheck->Disable();
		else {
			allFrameCheck->Enable();
			allFrameCheck->SetValue(tempPane->GetAllFrames());
		}
		tempPane->PageIsNowActive();	//make sure the update button is the default
	}
#if wxCHECK_VERSION(2, 8, 0)
	surfTitleEdit->ChangeValue(temp);
#else
	setValueCalled = true;
	surfTitleEdit->SetValue(temp);
#endif
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_VISIBLECHECK
 */

void SurfacesWindow::OnVisiblecheckClick( wxCommandEvent& event )
{
	BaseSurfacePane* tempPane = (BaseSurfacePane * ) book->GetCurrentPage();
	tempPane->SetVisibility(event.GetSelection());
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_ALLFRAMECHECK
 */

void SurfacesWindow::OnAllframecheckClick( wxCommandEvent& event )
{
	BaseSurfacePane* tempPane = (BaseSurfacePane * ) book->GetCurrentPage();
	tempPane->setAllFrames(event.GetSelection());
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD
 */

void SurfacesWindow::OnAddClick( wxCommandEvent& event )
{
	int surfTypeId = selectSurfaceType();
	if (surfTypeId > 0) {
		addNewPane(surfTypeId);
		if (book->GetPageCount()> 0) {
			visibleCheck->Enable();
			//allFrameCheck->Enable();
		}
	}
}

void SurfacesWindow::SurfaceUpdated(void) {
	//Do we need to generate a frame changed event here???
	BaseSurfacePane* tempPane = (BaseSurfacePane * ) book->GetCurrentPage();
	if (tempPane) {
		Surface * tempSurf = tempPane->GetTargetSurface();
		wxString temp(tempSurf->GetLabel(), wxConvUTF8);
#if wxCHECK_VERSION(2, 8, 0)
		surfTitleEdit->ChangeValue(temp);
#else
		setValueCalled = true;
		surfTitleEdit->SetValue(temp);
#endif
		book->SetPageText(book->GetSelection(), temp);
		//The previous line does not immediately update the list text???
	} 
	Parent->UpdateModelDisplay();
}

int SurfacesWindow::selectSurfaceType()
{
	std::vector<wxString> allOptions;
	int numOptions = 2;
	
	if (mData->OrbSurfacePossible())
    {
		allOptions.push_back(_T("2D Orbital"));
		allOptions.push_back(_T("3D Orbital"));
		numOptions += 2;
    }
	
	if (mData->TotalDensityPossible())
    {
		allOptions.push_back(_T("2D Total Electron Density"));
		allOptions.push_back(_T("3D Total Electron Density"));
		allOptions.push_back(_T("2D Molecular Electrostatic Potential"));
		allOptions.push_back(_T("3D Molecular Electrostatic Potential"));
		numOptions += 4;
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
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_SURFTITLE
 */

void SurfacesWindow::OnSurftitleUpdated( wxCommandEvent& event )
{
	//Prior to wx v2.7 you had to call SetValue to change a textCtrl which generates
	//a TEXT_UPDATED event. To avoid treating these as actual edits I set the setValueCalled
	//flag before calling SetValue and then clear it here instead of looking at the value.
	if (!setValueCalled) {
		int id = book->GetSelection();
		wxString newLabel = surfTitleEdit->GetValue();
		
		BaseSurfacePane* tempPane = (BaseSurfacePane * ) book->GetCurrentPage();
		if (tempPane) {
			Surface * tempSurf = tempPane->GetTargetSurface();
			if (tempSurf) {
				wxString test(tempSurf->GetLabel(), wxConvUTF8);
				if (newLabel.IsEmpty()) {
					tempSurf->SetLabel(NULL);
					newLabel = wxString(tempSurf->GetLabel(), wxConvUTF8);
					book->SetPageText(book->GetSelection(), newLabel);
				} else if (newLabel.Cmp(test) != 0) {
					tempSurf->SetLabel((const char *)newLabel.mb_str(wxConvUTF8));
					book->SetPageText(book->GetSelection(), newLabel);
				}
			}
		}
	} else
		setValueCalled = false;
	
}
