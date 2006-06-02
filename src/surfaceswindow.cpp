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

    EVT_BUTTON( wxID_ADD, SurfacesWindow::OnAddClick )

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
    listBook = NULL;
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

    listBook = new wxListbook( itemFrame1, ID_SURFLISTBOOK, wxDefaultPosition, wxDefaultSize, wxLB_BOTTOM|wxSUNKEN_BORDER );

    itemBoxSizer5->Add(listBook, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer5->Add(itemBoxSizer7, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton8 = new wxButton( itemFrame1, wxID_DELETE, _("&Delete"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        itemButton8->SetToolTip(_("Delete the selected surface"));
    itemBoxSizer7->Add(itemButton8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton9 = new wxButton( itemFrame1, wxID_ADD, _("Add"), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        itemButton9->SetToolTip(_("Add a new surface"));
    itemBoxSizer7->Add(itemButton9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end SurfacesWindow content construction
	Orbital3D * temp = new Orbital3D(this);
	listBook->AddPage(temp, wxT("test orb"), false);
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
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE in SurfacesWindow.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_DELETE in SurfacesWindow. 
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD
 */

void SurfacesWindow::OnAddClick( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD in SurfacesWindow.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ADD in SurfacesWindow. 
}


