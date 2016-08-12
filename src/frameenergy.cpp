/////////////////////////////////////////////////////////////////////////////
// Name:        frameenergy.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Mon 29 May 21:14:10 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "frameenergy.h"
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

#include "frameenergy.h"

////@begin XPM images
////@end XPM images

/*!
 * FrameEnergy type definition
 */

IMPLEMENT_DYNAMIC_CLASS( FrameEnergy, wxDialog )

/*!
 * FrameEnergy event table definition
 */

BEGIN_EVENT_TABLE( FrameEnergy, wxDialog )

////@begin FrameEnergy event table entries
	EVT_BUTTON( wxID_OK, FrameEnergy::OnOkClick )
////@end FrameEnergy event table entries

END_EVENT_TABLE()

/*!
 * FrameEnergy constructors
 */

FrameEnergy::FrameEnergy( )
{
}

FrameEnergy::FrameEnergy( MolDisplayWin * parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * FrameEnergy creator
 */

bool FrameEnergy::Create( MolDisplayWin * parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin FrameEnergy member initialisation
	TEedit = NULL;
	MP2edit = NULL;
	KEedit = NULL;
	TimeEdit = NULL;
////@end FrameEnergy member initialisation
	Parent = parent;

////@begin FrameEnergy creation
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end FrameEnergy creation
    return true;
}

/*!
 * Control creation for FrameEnergy
 */

void FrameEnergy::CreateControls()
{    
////@begin FrameEnergy content construction
	FrameEnergy* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Total Energy for this frame:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	TEedit = new wxTextCtrl( itemDialog1, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0 );
	itemBoxSizer2->Add(TEedit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("MP2 Energy for this frame:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer2->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	MP2edit = new wxTextCtrl( itemDialog1, ID_MP2Text, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0 );
	itemBoxSizer2->Add(MP2edit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Kinetic Energy for this frame:\n(for DRP's only)"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer2->Add(itemStaticText7, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	KEedit = new wxTextCtrl( itemDialog1, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0 );
	itemBoxSizer2->Add(KEedit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("DRP time, IRC distance or other unique id:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer2->Add(itemStaticText9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	TimeEdit = new wxTextCtrl( itemDialog1, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0 );
	itemBoxSizer2->Add(TimeEdit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer11, 0, wxALIGN_RIGHT|wxALL, 5);

	wxButton* itemButton12 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer11->Add(itemButton12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton13 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	itemButton13->SetDefault();
	itemBoxSizer11->Add(itemButton13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	// Set validators
	TEedit->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	MP2edit->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	KEedit->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	TimeEdit->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
////@end FrameEnergy content construction
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	wxString temp;
	temp.Printf(wxT("%.8f"),lFrame->GetEnergy());
	TEedit->SetValue(temp);
	temp.Printf(wxT("%.8f"),lFrame->GetMP2Energy());
	MP2edit->SetValue(temp);
	temp.Printf(wxT("%.8f"),lFrame->GetKineticEnergy());
	KEedit->SetValue(temp);
	temp.Printf(wxT("%.8f"),lFrame->GetTime());
	TimeEdit->SetValue(temp);
	
	//convert the values when tabing and when OK is hit
	//Add MP2 E?
}

/*!
 * Should we show tooltips?
 */

bool FrameEnergy::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap FrameEnergy::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin FrameEnergy bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end FrameEnergy bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon FrameEnergy::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin FrameEnergy icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end FrameEnergy icon retrieval
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */
void FrameEnergy::OnOkClick( wxCommandEvent& event )
{
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	wxString temp = TEedit->GetValue();
	double dval = lFrame->GetEnergy();
	if (temp.ToDouble(&dval)) {
		lFrame->SetEnergy(dval);
	} else {
		temp.Printf(wxT("%.8f"),dval);
		TEedit->SetValue(temp);
		return;
	}
	wxString mp2 = MP2edit->GetValue();
	dval = lFrame->GetMP2Energy();
	if (mp2.ToDouble(&dval)) {
		lFrame->SetMP2Energy(dval);
	} else {
		temp.Printf(wxT("%.8f"),dval);
		MP2edit->SetValue(temp);
		return;
	}
	wxString ke = KEedit->GetValue();
	dval = lFrame->GetKineticEnergy();
	if (ke.ToDouble(&dval)) {
		lFrame->SetKineticEnergy(dval);
	} else {
		temp.Printf(wxT("%.8f"),dval);
		KEedit->SetValue(temp);
		return;
	}
	wxString time = TimeEdit->GetValue();
	dval = lFrame->GetTime();
	if (time.ToDouble(&dval)) {
		lFrame->SetTime(dval);
	} else {
		temp.Printf(wxT("%.8f"),lFrame->GetTime());
		TimeEdit->SetValue(temp);
		return;
	}

    event.Skip();
}

