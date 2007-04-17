/////////////////////////////////////////////////////////////////////////////
// Name:        symmetrypointgroupdlg.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Sat  7 Apr 09:51:08 2007
// RCS-ID:      
// Copyright:   (c) 2007 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "symmetrypointgroupdlg.h"
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

#include "MolDisplayWin.h"
#include "symmetrypointgroupdlg.h"

////@begin XPM images
////@end XPM images

/*!
 * SymmetryPointGroupDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SymmetryPointGroupDlg, wxDialog )

/*!
 * SymmetryPointGroupDlg event table definition
 */

BEGIN_EVENT_TABLE( SymmetryPointGroupDlg, wxDialog )

////@begin SymmetryPointGroupDlg event table entries
////@end SymmetryPointGroupDlg event table entries

END_EVENT_TABLE()

/*!
 * SymmetryPointGroupDlg constructors
 */

SymmetryPointGroupDlg::SymmetryPointGroupDlg()
{
	Init();
}

SymmetryPointGroupDlg::SymmetryPointGroupDlg( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create(parent, id, caption, pos, size, style);
}

/*!
 * SymmetryPointGroupDlg creator
 */

bool SymmetryPointGroupDlg::Create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SymmetryPointGroupDlg creation
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end SymmetryPointGroupDlg creation
	return true;
}

/*!
 * SymmetryPointGroupDlg destructor
 */

SymmetryPointGroupDlg::~SymmetryPointGroupDlg()
{
////@begin SymmetryPointGroupDlg destruction
////@end SymmetryPointGroupDlg destruction
}

/*!
 * Member initialisation 
 */

void SymmetryPointGroupDlg::Init()
{
////@begin SymmetryPointGroupDlg member initialisation
	Parent = NULL;
	mHighestPGText = NULL;
	mAbelianPGText = NULL;
	mPGListCntl = NULL;
////@end SymmetryPointGroupDlg member initialisation
}

/*!
 * Control creation for SymmetryPointGroupDlg
 */

void SymmetryPointGroupDlg::CreateControls()
{    
////@begin SymmetryPointGroupDlg content construction
	SymmetryPointGroupDlg* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("The highest point group of the molecule is:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	mHighestPGText = new wxStaticText( itemDialog1, wxID_STATIC, _("C1"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer3->Add(mHighestPGText, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("The highest abelian point group is:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer6->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	mAbelianPGText = new wxStaticText( itemDialog1, wxID_STATIC, _("C1"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer6->Add(mAbelianPGText, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("This is the list of possible point groups for this geometry:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStaticText10->Wrap(100);
	itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

	mPGListCntl = new wxListCtrl( itemDialog1, ID_LISTCTRL1, wxDefaultPosition, wxSize(100, 100), 0 );
	itemBoxSizer9->Add(mPGListCntl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer12, 0, wxALIGN_RIGHT|wxALL, 5);

	wxButton* itemButton13 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer12->Add(itemButton13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton14 = new wxButton( itemDialog1, ID_SETBUTTON, _("&Set to selected"), wxDefaultPosition, wxDefaultSize, 0 );
	itemButton14->SetDefault();
	itemBoxSizer12->Add(itemButton14, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end SymmetryPointGroupDlg content construction
}

/*!
 * Should we show tooltips?
 */

bool SymmetryPointGroupDlg::ShowToolTips()
{
	return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SymmetryPointGroupDlg::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin SymmetryPointGroupDlg bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end SymmetryPointGroupDlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SymmetryPointGroupDlg::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin SymmetryPointGroupDlg icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end SymmetryPointGroupDlg icon retrieval
}
