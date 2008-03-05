/////////////////////////////////////////////////////////////////////////////
// Name:        coordinateoffset.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Sat 27 May 21:58:09 2006
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "coordinateoffset.h"
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
#include "coordinateoffset.h"

////@begin XPM images
////@end XPM images

/*!
 * CoordinateOffset type definition
 */

IMPLEMENT_DYNAMIC_CLASS( CoordinateOffset, wxDialog )

/*!
 * CoordinateOffset event table definition
 */

BEGIN_EVENT_TABLE( CoordinateOffset, wxDialog )

////@begin CoordinateOffset event table entries
    EVT_SLIDER( ID_SLIDER, CoordinateOffset::OnSliderUpdated )

    EVT_TEXT( ID_TEXTCTRL1, CoordinateOffset::OnTextctrl1Updated )

    EVT_BUTTON( wxID_OK, CoordinateOffset::OnOkClick )

////@end CoordinateOffset event table entries

END_EVENT_TABLE()

/*!
 * CoordinateOffset constructors
 */

CoordinateOffset::CoordinateOffset( )
{
}

CoordinateOffset::CoordinateOffset( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}
CoordinateOffset::~CoordinateOffset(void) {
	if (Parent) {
		MoleculeData * MainData = Parent->GetData();
		Frame * lFrame = MainData->GetCurrentFramePtr();
		if (lFrame->GetNumAtoms() == origCoords.size()) {
			for (long i=0; i<lFrame->GetNumAtoms(); i++) {
				lFrame->SetAtomPosition(i, origCoords[i]);
			}
			Parent->ResetModel(false);
		}
	}
}

/*!
 * CoordinateOffset creator
 */

bool CoordinateOffset::Create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin CoordinateOffset member initialisation
    Parent = NULL;
    slider = NULL;
    editField = NULL;
////@end CoordinateOffset member initialisation
	Parent = parent;

////@begin CoordinateOffset creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end CoordinateOffset creation
	MoleculeData * MainData = Parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	origCoords.reserve(lFrame->GetNumAtoms());
	CPoint3D temp;
	for (long i=0; i<lFrame->GetNumAtoms(); i++) {
		lFrame->GetAtomPosition(i, temp);
		origCoords.push_back(temp);
	}
    return true;
}

/*!
 * Control creation for CoordinateOffset
 */

void CoordinateOffset::CreateControls()
{    
////@begin CoordinateOffset content construction
    CoordinateOffset* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Move the slider to offset the coordinates\nalong the chosen normal mode. Note the offset\nwill be performed in cartesian space rather than\nmass-weighted cartesian space as shown."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    slider = new wxSlider( itemDialog1, ID_SLIDER, 0, -100, 100, wxDefaultPosition, wxSize(100, 20), wxSL_HORIZONTAL );
    if (ShowToolTips())
        slider->SetToolTip(_("Adjust the slider to adjust the magnitude of the offset"));
    itemBoxSizer4->Add(slider, 1, wxALIGN_CENTER_VERTICAL|wxALL | wxEXPAND, 5);

    editField = new wxTextCtrl(itemDialog1, ID_TEXTCTRL1, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC, &valid_string));
    if (ShowToolTips())
        editField->SetToolTip(_("Type in a specific value for the offset percentage"));
    itemBoxSizer4->Add(editField, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	editField->SetValue(_("0.0"));

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("%"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxButton* itemButton9 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton9->SetDefault();
    itemBoxSizer8->Add(itemButton9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton10 = new wxButton( itemDialog1, wxID_OK, _("Copy Coordinates"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton10->SetDefault();
    itemBoxSizer8->Add(itemButton10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end CoordinateOffset content construction
}

/*!
 * Should we show tooltips?
 */

bool CoordinateOffset::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap CoordinateOffset::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin CoordinateOffset bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end CoordinateOffset bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon CoordinateOffset::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin CoordinateOffset icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end CoordinateOffset icon retrieval
}
/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL1
 */
void CoordinateOffset::OnTextctrl1Updated( wxCommandEvent& event )
{
	wxString textVal = editField->GetValue();
	double val = 0.0;
	if (textVal.ToDouble(&val)) {
		slider->SetValue((int)val);
		ApplyOffset(val);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER
 */

void CoordinateOffset::OnSliderUpdated( wxCommandEvent& event )
{
	int sval = slider->GetValue();
	wxString eval;
	eval.Printf(wxT("%d"), sval);
	editField->SetValue(eval);
	ApplyOffset(sval);
    event.Skip();
}

void CoordinateOffset::ApplyOffset(float offset) {
	if (origCoords.size() > 0) {
		offset /= 100.0;
		MoleculeData * MainData = Parent->GetData();
		Frame *	lFrame = MainData->GetCurrentFramePtr();
		VibRec * vibs = lFrame->GetFrequencies();
		long cmode = lFrame->GetNumAtoms() * vibs->GetCurrentMode();
		WinPrefs * Prefs = Parent->GetPrefs();
		float VectorScale = Prefs->GetVectorScale();
	
		CPoint3D temp, temp2;
		for (long iatom=0; iatom < lFrame->GetNumAtoms(); iatom++) {
				//The modes are stored mass weighted. We want to remove that here.
			float lmass = 1.0/Prefs->GetSqrtAtomMass(lFrame->GetAtomType(iatom)-1);
			vibs->GetModeOffset(iatom+cmode, temp2);
			temp.x = origCoords[iatom].x + offset*VectorScale*temp2.x*lmass;
			temp.y = origCoords[iatom].y + offset*VectorScale*temp2.y*lmass;
			temp.z = origCoords[iatom].z + offset*VectorScale*temp2.z*lmass;
			lFrame->SetAtomPosition(iatom, temp);
		}
		Parent->ResetModel(false);
	}
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void CoordinateOffset::OnOkClick( wxCommandEvent& event )
{
	Parent->CopyCoordinates(0);
    event.Skip();
}
