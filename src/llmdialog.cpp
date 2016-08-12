/////////////////////////////////////////////////////////////////////////////
// Name:        llmdialog.cpp
// Purpose:     
// Author:      Brett Bode
// Created:     Tue  2 May 13:24:39 2006
// Copyright:   (c) 2006 Iowa State University
// Licence:     See the LICENSE file in the top level directory
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "llmdialog.h"
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

#include "llmdialog.h"

////@begin XPM images
////@end XPM images

/*!
 * LLMDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( LLMDialog, wxDialog )

/*!
 * LLMDialog event table definition
 */

BEGIN_EVENT_TABLE( LLMDialog, wxDialog )

////@begin LLMDialog event table entries
	EVT_BUTTON( wxID_OK, LLMDialog::OnOkClick )
////@end LLMDialog event table entries

END_EVENT_TABLE()

/*!
 * LLMDialog constructors
 */

LLMDialog::LLMDialog( )
{
}

LLMDialog::LLMDialog( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * LLMDialog creator
 */

bool LLMDialog::Create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin LLMDialog member initialisation
	numPointsEdit = NULL;
	internalCheck = NULL;
////@end LLMDialog member initialisation
	Parent = parent;

////@begin LLMDialog creation
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end LLMDialog creation
    return true;
}

/*!
 * Control creation for LLMDialog
 */

void LLMDialog::CreateControls()
{    
////@begin LLMDialog content construction
	LLMDialog* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Linear Least Motion (LLM) path analysis"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Click the Create button to insert the\nnumber of frames given below along\nthe LLM path connecting the current\ngeometry with the next geometry."), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer2->Add(itemStaticText4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	numPointsEdit = new wxTextCtrl( itemDialog1, ID_TEXTCTRL, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
	if (LLMDialog::ShowToolTips())
		numPointsEdit->SetToolTip(_("Enter an integer for the number of intermediate geometries to create"));
	itemBoxSizer2->Add(numPointsEdit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	internalCheck = new wxCheckBox( itemDialog1, ID_INTERNALSCHECKBOX, _("Compute using internal coordinates"), wxDefaultPosition, wxDefaultSize, 0 );
	internalCheck->SetValue(false);
	if (LLMDialog::ShowToolTips())
		internalCheck->SetToolTip(_("Check to create the LLM path using the currently defined set of internal coordinates"));
	itemBoxSizer2->Add(internalCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer7, 0, wxALIGN_RIGHT|wxALL, 5);

	wxButton* itemButton8 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer7->Add(itemButton8, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton9 = new wxButton( itemDialog1, wxID_OK, _("Create LLM"), wxDefaultPosition, wxDefaultSize, 0 );
	itemButton9->SetDefault();
	itemBoxSizer7->Add(itemButton9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end LLMDialog content construction
	MoleculeData * MainData = Parent->GetData();
	internalCheck->Enable(MainData->GetInternalCoordinates()!=NULL);
	numPointsEdit->SetValue(wxT("0"));

	// Set validators
#if wxCHECK_VERSION(3, 0, 0)
	numPointsEdit->SetValidator( wxTextValidator(wxFILTER_DIGITS) );
#else
	numPointsEdit->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
#endif
}

/*!
 * Should we show tooltips?
 */

bool LLMDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap LLMDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin LLMDialog bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end LLMDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon LLMDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin LLMDialog icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end LLMDialog icon retrieval
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void LLMDialog::OnOkClick( wxCommandEvent& event )
{
	wxString textVal = numPointsEdit->GetValue();
	long numPoints=0;
	if (textVal.ToLong(&numPoints)) {
		if (numPoints > 0) {
			MoleculeData * MainData = Parent->GetData();
			WinPrefs * Prefs = 	Prefs = Parent->GetPrefs();
			if (internalCheck->IsChecked())
				MainData->CreateInternalLLM(numPoints, Prefs);
			else
				MainData->CreateLLM(numPoints, Prefs);
			Parent->ResetAllWindows();
		}
	}
    event.Skip();
}
