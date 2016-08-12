/////////////////////////////////////////////////////////////////////////////
// Name:        swfexport-g.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Thu 12 Apr 2007 04:16:34 PM CDT
// RCS-ID:      
// Copyright:   (c) 2007 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "swfexport.h"
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

#include "swfexport.h"

////@begin XPM images
////@end XPM images

/*!
 * SWFExport type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SWFExport, wxDialog )

/*!
 * SWFExport event table definition
 */

BEGIN_EVENT_TABLE( SWFExport, wxDialog )

////@begin SWFExport event table entries
	EVT_RADIOBOX( ID_SWF_MOVIE_RADIO, SWFExport::OnSwfMovieRadioSelected )
////@end SWFExport event table entries

END_EVENT_TABLE()

/*!
 * SWFExport constructors
 */

SWFExport::SWFExport()
{
	Init();
}

SWFExport::SWFExport( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	Init();
	Create(parent, id, caption, pos, size, style);
}

/*!
 * SWFExport creator
 */

bool SWFExport::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SWFExport creation
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end SWFExport creation
	return true;
}

/*!
 * SWFExport destructor
 */

SWFExport::~SWFExport()
{
////@begin SWFExport destruction
////@end SWFExport destruction
}

/*!
 * Member initialisation 
 */

void SWFExport::Init()
{
////@begin SWFExport member initialisation
	mMovieType = NULL;
	mEPlotCheck = NULL;
////@end SWFExport member initialisation
}

/*!
 * Control creation for SWFExport
 */

void SWFExport::CreateControls()
{    
////@begin SWFExport content construction
	SWFExport* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxArrayString mMovieTypeStrings;
	mMovieTypeStrings.Add(_("&Frame Animation"));
	mMovieTypeStrings.Add(_("&Normal Mode animation"));
	mMovieType = new wxRadioBox( itemDialog1, ID_SWF_MOVIE_RADIO, _("Animation Type"), wxDefaultPosition, wxDefaultSize, mMovieTypeStrings, 0, wxRA_SPECIFY_ROWS );
	mMovieType->SetSelection(0);
	itemBoxSizer2->Add(mMovieType, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mEPlotCheck = new wxCheckBox( itemDialog1, ID_EPLOT_CHECK, _("Include Energy Plot"), wxDefaultPosition, wxDefaultSize, 0 );
	mEPlotCheck->SetValue(false);
	if (SWFExport::ShowToolTips())
		mEPlotCheck->SetToolTip(_("click to display the energy plot next to the animation for the frame animation."));
	itemBoxSizer2->Add(mEPlotCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_RIGHT|wxALL, 5);

	wxButton* itemButton6 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer5->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton7 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer5->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end SWFExport content construction
}

/*!
 * Should we show tooltips?
 */

bool SWFExport::ShowToolTips()
{
	return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SWFExport::GetBitmapResource( const wxString& name )
{
	// Bitmap retrieval
////@begin SWFExport bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end SWFExport bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SWFExport::GetIconResource( const wxString& name )
{
	// Icon retrieval
////@begin SWFExport icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end SWFExport icon retrieval
}

/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_SWF_MOVIE_RADIO
 */

void SWFExport::OnSwfMovieRadioSelected( wxCommandEvent& event )
{
    if(event.GetInt() == 0) {
        mEPlotCheck->Enable(true);
    }
    else {
        mEPlotCheck->SetValue(false);
        mEPlotCheck->Enable(false);
    }
}

