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

#include "surfaceswindow.h"
#include "orbital3d.h"

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
    EVT_BUTTON( wxID_DELETE, SurfacesWindow::OnDeleteClick )

    EVT_CHOICE( ID_ADDSURFCHOICE, SurfacesWindow::OnAddsurfchoiceSelected )

////@end SurfacesWindow event table entries

END_EVENT_TABLE()

/*!
 * SurfacesWindow constructors
 */

SurfacesWindow::SurfacesWindow( )
{
}

SurfacesWindow::SurfacesWindow( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create( parent, id, caption, pos, size, style );
}

/*!
 * SurfacesWindow creator
 */

bool SurfacesWindow::Create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SurfacesWindow member initialisation
    surfTitleEdit = NULL;
    visibleCheck = NULL;
    allFrameCheck = NULL;
    listBook = NULL;
    surfAddChoice = NULL;
////@end SurfacesWindow member initialisation
	Parent = parent;

////@begin SurfacesWindow creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
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
    SurfacesWindow* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    menuBar->Append(itemMenu3, _("File"));
    wxMenu* itemMenu4 = new wxMenu;
    menuBar->Append(itemMenu4, _("Edit"));
    itemFrame1->SetMenuBar(menuBar);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer5->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    surfTitleEdit = new wxTextCtrl( itemFrame1, ID_SURFTITLE, _T(""), wxDefaultPosition, wxSize(250, -1), 0 );
    itemBoxSizer6->Add(surfTitleEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    visibleCheck = new wxCheckBox( itemFrame1, ID_VISIBLECHECK, _("Visible"), wxDefaultPosition, wxDefaultSize, 0 );
    visibleCheck->SetValue(false);
    itemBoxSizer6->Add(visibleCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    allFrameCheck = new wxCheckBox( itemFrame1, ID_ALLFRAMECHECK, _("All Frames"), wxDefaultPosition, wxDefaultSize, 0 );
    allFrameCheck->SetValue(false);
    itemBoxSizer6->Add(allFrameCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    listBook = new wxListbook( itemFrame1, ID_SURFLISTBOOK, wxDefaultPosition, wxDefaultSize, wxLB_BOTTOM|wxSUNKEN_BORDER );

    itemBoxSizer5->Add(listBook, 4, wxGROW|wxALL, 2);

    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer5->Add(itemBoxSizer11, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton12 = new wxButton( itemFrame1, wxID_DELETE, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        itemButton12->SetToolTip(_("Delete the selected surface"));
    itemBoxSizer11->Add(itemButton12, 0, wxALIGN_BOTTOM|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemFrame1, wxID_STATIC, _("Add a surface of type:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer11->Add(itemStaticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString surfAddChoiceStrings[] = {
        _("2D Orbital"),
        _("3D Orbital"),
        _("2D Total Electron Density"),
        _("3D Total Electron Density"),
        _("2D Molecular Electrostatic Potential"),
        _("3D Molecular Electrostatic Potential"),
        _("General 2D from File"),
        _("General 3D from File")
    };
    surfAddChoice = new wxChoice( itemFrame1, ID_ADDSURFCHOICE, wxDefaultPosition, wxDefaultSize, 8, surfAddChoiceStrings, 0 );
    itemBoxSizer11->Add(surfAddChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end SurfacesWindow content construction
	wxListView * t = listBook->GetListView();
	t->SetWindowStyle(wxLC_LIST);
	Orbital3D * temp = new Orbital3D(listBook);
	listBook->AddPage(temp, wxT("test orb"), false);
	temp = new Orbital3D(listBook);
	listBook->AddPage(temp, wxT("test pane 2"), false);
}

void SurfacesWindow::Reset(void) {
	//rebuild the list of surfaces
	int oldpage = listBook->GetSelection();
	listBook->DeleteAllPages();
	
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
}

void SurfacesWindow::BuildAddChoice(void) {
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	
	surfAddChoice->Clear();
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
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE
 */

void SurfacesWindow::OnDeleteClick( wxCommandEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();

	int targetSurf = listBook->GetSelection();
	lFrame->DeleteSurface(targetSurf);
	listBook->DeletePage(targetSurf);
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ADDSURFCHOICE
 */

void SurfacesWindow::OnAddsurfchoiceSelected( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ADDSURFCHOICE in SurfacesWindow.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ADDSURFCHOICE in SurfacesWindow. 
}


