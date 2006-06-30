/////////////////////////////////////////////////////////////////////////////
// Name:        surfaceswindow.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri  2 Jun 09:56:43 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
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

#include <wx/listctrl.h>
#include <wx/listbook.h>

////@begin includes
////@end includes
#include "Globals.h"
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

using namespace std;

/*!
 * SurfacesWindow type definition
 */

IMPLEMENT_CLASS( SurfacesWindow, wxFrame )

/*!
 * SurfacesWindow event table definition
 */

BEGIN_EVENT_TABLE( SurfacesWindow, wxFrame )

////@begin SurfacesWindow event table entries
  EVT_BUTTON( wxID_DELETE, SurfacesWindow::OnDeleteClick )
  EVT_BUTTON( wxID_ADD, SurfacesWindow::OnAddClick )
  EVT_CLOSE( SurfacesWindow::OnClose )
////@end SurfacesWindow event table entries

END_EVENT_TABLE()


/*!
 * SurfacesWindow constructors
 */

SurfacesWindow::SurfacesWindow( )
{
}

SurfacesWindow::~SurfacesWindow()
{
  Parent->CloseSurfacesWindow();
}

SurfacesWindow::SurfacesWindow( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  mData = parent->GetData();
  Create( parent, id, caption, pos, size, style );
}

/*!
 * SurfacesWindow creator
 */

bool SurfacesWindow::Create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    surfTitleEdit = NULL;
    visibleCheck = NULL;
    allFrameCheck = NULL;
    listBook = NULL;

    Parent = parent;
    mPrefs = parent->GetPrefs();
////@begin SurfacesWindow creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end SurfacesWindow creation
    return true;
}

/*!
 * Control creation for SurfacesWindow
 */

void SurfacesWindow::CreateControls()
{    
////@begin SurfacesWindow content construction
  //SurfacesWindow* itemFrame1 = this;

    /*
    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    menuBar->Append(itemMenu3, _("File"));
    wxMenu* itemMenu4 = new wxMenu;
    menuBar->Append(itemMenu4, _("Edit"));
    itemFrame1->SetMenuBar(menuBar);
    */

    mainSizer = new wxBoxSizer(wxVERTICAL);
    upperSizer = new wxBoxSizer(wxHORIZONTAL);

    surfTitleEdit = new wxTextCtrl( this, ID_SURFTITLE, _T(""), wxDefaultPosition, wxSize(250, -1), 0 );
    upperSizer->Add(surfTitleEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    visibleCheck = new wxCheckBox( this, ID_VISIBLECHECK, _("Visible"), wxDefaultPosition, wxDefaultSize, 0 );
    upperSizer->Add(visibleCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    allFrameCheck = new wxCheckBox( this, ID_ALLFRAMECHECK, _("All Frames"), wxDefaultPosition, wxDefaultSize, 0 );
    upperSizer->Add(allFrameCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook = new wxListbook( this, ID_SURFLISTBOOK, wxDefaultPosition, wxDefaultSize, wxLB_BOTTOM|wxSUNKEN_BORDER );

    middleSizer = new wxBoxSizer(wxHORIZONTAL);
    middleSizer->Add(listBook, 4, wxGROW|wxALL, 2);

    addButton = new wxButton( this, wxID_ADD, _("&Add"), wxDefaultPosition, wxDefaultSize, 0 );
    delButton = new wxButton( this, wxID_DELETE, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );

    if (ShowToolTips())
      {
        addButton->SetToolTip(_("Add a surface"));
	delButton->SetToolTip(_("Delete the  selected surface"));
      }

    bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    bottomSizer->Add(addButton, 0, wxALIGN_BOTTOM|wxALL, 5);
    bottomSizer->Add(delButton, 0, wxALIGN_BOTTOM|wxALL, 5);

    mainSizer->Add(upperSizer, 0, wxALIGN_LEFT|wxALL, 5);
    mainSizer->Add(middleSizer, 0, wxALIGN_LEFT|wxALL, 5);
    mainSizer->Add(bottomSizer, 0, wxALIGN_LEFT|wxALL, 5);

    SetSizer(mainSizer);

    int type = selectSurfaceType();
    wxListView * t = listBook->GetListView();
    t->SetWindowStyle(wxLC_LIST);

    BaseSurfacePane* tempPane = NULL;
    Surface* newSurface = NULL;

    switch (type)
    {
    case ID_3D_ORBITAL_PANE:
      newSurface = new Orb3DSurface(mPrefs);
      tempPane = new Orbital3DSurfPane(listBook, dynamic_cast<Orb3DSurface*>(newSurface), mData, mPrefs );
      break;
    }

    if ( tempPane && newSurface)
      {
	tempPane->SetVisibility(true);
	visibleCheck->SetValue(true);
	allFrameCheck->SetValue((newSurface->GetSurfaceID() != 0));
      }
    else
      {
	visibleCheck->Disable();
	allFrameCheck->Disable();
      }

    if ( type != -1)
      listBook->AddPage(tempPane, mCurrSurfStr, false);
}

void SurfacesWindow::OnAddClick( wxCommandEvent& event )
{
  int surfTypeId = selectSurfaceType();
  BaseSurfacePane* tempPane;

  switch (surfTypeId)
    {
    case ID_3D_ORBITAL_PANE:
      Orb3DSurface* newSurface = new Orb3DSurface(mPrefs);
      tempPane = new Orbital3DSurfPane(listBook,newSurface, mData, mPrefs );
      break;
    }

  if ( surfTypeId != -1)
    listBook->AddPage(tempPane, mCurrSurfStr, false);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE
 */

void SurfacesWindow::OnDeleteClick( wxCommandEvent& event )
{
  Frame * lFrame = mData->GetCurrentFramePtr();

  int targetSurf = listBook->GetSelection();
  lFrame->DeleteSurface(targetSurf);
  listBook->DeletePage(targetSurf);
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ADDSURFCHOICE
 */

int SurfacesWindow::selectSurfaceType()
{
  vector<wxString> allOptions;
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

  if (dialog.ShowModal() == wxID_OK)
    mCurrSurfStr= dialog.GetStringSelection();

  if (mCurrSurfStr.Cmp("2D Orbital") == 0)
    return ID_2D_ORBITAL_PANE;
  if (mCurrSurfStr.Cmp("3D Orbital") == 0)
    return ID_3D_ORBITAL_PANE;
  if (mCurrSurfStr.Cmp("2D Total Electron Density") == 0)
    return ID_2D_TE_DENSITY_PANE;
  if (mCurrSurfStr.Cmp("3D Total Electron Density") == 0)
    return ID_3D_TE_DENSITY_PANE;
  if (mCurrSurfStr.Cmp("2D Molecular Electrostatic Potential") == 0)
    return ID_2D_ME_POTENTIAL_PANE;
  if (mCurrSurfStr.Cmp("3D Molecular Electrostatic Potential") == 0)
    return ID_3D_ME_POTENTIAL_PANE;
  if (mCurrSurfStr.Cmp("General 2D from File") == 0)
    return ID_2D_FILE_PANE;
  if (mCurrSurfStr.Cmp("General 3D from File") == 0)
    return ID_3D_FILE_PANE;

  return -1;

}

void SurfacesWindow::Reset(void) {
	//rebuild the list of surfaces
	int oldpage = listBook->GetSelection();
	listBook->DeleteAllPages();
	
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
}

void SurfacesWindow::OnClose( wxCloseEvent& event )
{
  Destroy();
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



