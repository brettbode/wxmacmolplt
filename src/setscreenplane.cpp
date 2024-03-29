/////////////////////////////////////////////////////////////////////////////
// Name:        setscreenplane.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Tue 11 Apr 15:33:38 2006
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks, Tue 11 Apr 15:33:38 2006

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "setscreenplane.h"
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
#include "MyTypes.h"
#include "Math3D.h"
#include "MolDisplayWin.h"
#include "Frame.h"

#include "setscreenplane.h"

////@begin XPM images
////@end XPM images

/*!
 * SetScreenPlane type definition
 */

IMPLEMENT_DYNAMIC_CLASS( SetScreenPlane, wxDialog )

/*!
 * SetScreenPlane event table definition
 */

BEGIN_EVENT_TABLE( SetScreenPlane, wxDialog )

////@begin SetScreenPlane event table entries
	EVT_TEXT( ID_ATOM1, SetScreenPlane::OnAtom1Updated )
	EVT_TEXT( ID_PT1X, SetScreenPlane::OnPt1xUpdated )
	EVT_TEXT( ID_PT1Y, SetScreenPlane::OnPt1yUpdated )
	EVT_TEXT( ID_PT1Z, SetScreenPlane::OnPt1zUpdated )
	EVT_TEXT( ID_ATOM2, SetScreenPlane::OnAtom2Updated )
	EVT_TEXT( ID_PT2X, SetScreenPlane::OnPt2xUpdated )
	EVT_TEXT( ID_PT2Y, SetScreenPlane::OnPt2yUpdated )
	EVT_TEXT( ID_PT2Z, SetScreenPlane::OnPt2zUpdated )
	EVT_TEXT( ID_ATOM3, SetScreenPlane::OnAtom3Updated )
	EVT_TEXT( ID_PT3X, SetScreenPlane::OnPt3xUpdated )
	EVT_TEXT( ID_PT3Y, SetScreenPlane::OnPt3yUpdated )
	EVT_TEXT( ID_PT3Z, SetScreenPlane::OnPt3zUpdated )
	EVT_BUTTON( ID_XY, SetScreenPlane::OnXyClick )
	EVT_BUTTON( ID_YX, SetScreenPlane::OnYxClick )
	EVT_BUTTON( ID_XZ, SetScreenPlane::OnXzClick )
	EVT_BUTTON( ID_ZX, SetScreenPlane::OnZxClick )
	EVT_BUTTON( ID_YZ, SetScreenPlane::OnYzClick )
	EVT_BUTTON( ID_ZY, SetScreenPlane::OnZyClick )
	EVT_BUTTON( wxID_OK, SetScreenPlane::OnOkClick )
////@end SetScreenPlane event table entries

END_EVENT_TABLE()

/*!
 * SetScreenPlane constructors
 */

SetScreenPlane::SetScreenPlane( )
{
}

SetScreenPlane::SetScreenPlane( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * SetScreenPlane creator
 */

bool SetScreenPlane::Create( MolDisplayWin* p, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin SetScreenPlane member initialisation
	atom1 = -1;
	atom2 = -1;
	atom3 = -1;
	Atom1Ctl = NULL;
	Pt1XCtl = NULL;
	Pt1YCtl = NULL;
	Pt1ZCtl = NULL;
	Atom2Ctl = NULL;
	Pt2XCtl = NULL;
	Pt2YCtl = NULL;
	Pt2ZCtl = NULL;
	Atom3Ctl = NULL;
	Pt3XCtl = NULL;
	Pt3YCtl = NULL;
	Pt3ZCtl = NULL;
////@end SetScreenPlane member initialisation
    parent = p;

////@begin SetScreenPlane creation
	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	wxDialog::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end SetScreenPlane creation
	Fit();
	MoleculeData * MainData = parent->GetData();
	CPoint3D inPoint, outPoint;
	Matrix4D lRot, lInverse;
	MainData->GetRotationMatrix(lRot);
	InverseMatrix(lRot, lInverse);
	inPoint.x = inPoint.y = inPoint.z = 0.0;
	BackRotate3DPt(lInverse, inPoint, &(PlanePts[0]));
	inPoint.x = 1.0;
	BackRotate3DPt(lInverse, inPoint, &(PlanePts[1]));
	inPoint.x = 0.0;
	inPoint.y = 1.0;
	BackRotate3DPt(lInverse, inPoint, &(PlanePts[2]));
	setPlaneValues();
	
    return true;
}

/*!
 * Control creation for SetScreenPlane
 */

void SetScreenPlane::CreateControls()
{    
////@begin SetScreenPlane content construction
	SetScreenPlane* itemDialog1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemDialog1->SetSizer(itemBoxSizer2);

	wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("Enter 3 points to define the screen plane:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxFlexGridSizer* itemFlexGridSizer4 = new wxFlexGridSizer(3, 6, 0, 0);
	itemBoxSizer2->Add(itemFlexGridSizer4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Atom 1"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	Atom1Ctl = new wxTextCtrl( itemDialog1, ID_ATOM1, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
	if (SetScreenPlane::ShowToolTips())
		Atom1Ctl->SetToolTip(_("Enter the index for an atom to define point 1."));
	itemFlexGridSizer4->Add(Atom1Ctl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("or point 1"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer4->Add(itemStaticText7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	Pt1XCtl = new wxTextCtrl( itemDialog1, ID_PT1X, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 0 );
	if (SetScreenPlane::ShowToolTips())
		Pt1XCtl->SetToolTip(_("The x-coordinate for point 1."));
	itemFlexGridSizer4->Add(Pt1XCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	Pt1YCtl = new wxTextCtrl( itemDialog1, ID_PT1Y, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 0 );
	if (SetScreenPlane::ShowToolTips())
		Pt1YCtl->SetToolTip(_("The y-coordinate for point 1."));
	itemFlexGridSizer4->Add(Pt1YCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	Pt1ZCtl = new wxTextCtrl( itemDialog1, ID_PT1Z, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 0 );
	if (SetScreenPlane::ShowToolTips())
		Pt1ZCtl->SetToolTip(_("The z-coordinate for point 1."));
	itemFlexGridSizer4->Add(Pt1ZCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, _("Atom 2"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer4->Add(itemStaticText11, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	Atom2Ctl = new wxTextCtrl( itemDialog1, ID_ATOM2, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
	if (SetScreenPlane::ShowToolTips())
		Atom2Ctl->SetToolTip(_("Enter the index for an atom to define point 2."));
	itemFlexGridSizer4->Add(Atom2Ctl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* itemStaticText13 = new wxStaticText( itemDialog1, wxID_STATIC, _("or point 2"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer4->Add(itemStaticText13, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	Pt2XCtl = new wxTextCtrl( itemDialog1, ID_PT2X, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 0 );
	if (SetScreenPlane::ShowToolTips())
		Pt2XCtl->SetToolTip(_("The x-coordinate for point 2."));
	itemFlexGridSizer4->Add(Pt2XCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	Pt2YCtl = new wxTextCtrl( itemDialog1, ID_PT2Y, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 0 );
	if (SetScreenPlane::ShowToolTips())
		Pt2YCtl->SetToolTip(_("The y-coordinate for point 2."));
	itemFlexGridSizer4->Add(Pt2YCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	Pt2ZCtl = new wxTextCtrl( itemDialog1, ID_PT2Z, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 0 );
	if (SetScreenPlane::ShowToolTips())
		Pt2ZCtl->SetToolTip(_("The z-coordinate for point 2."));
	itemFlexGridSizer4->Add(Pt2ZCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* itemStaticText17 = new wxStaticText( itemDialog1, wxID_STATIC, _("Atom 3"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer4->Add(itemStaticText17, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	Atom3Ctl = new wxTextCtrl( itemDialog1, ID_ATOM3, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 0 );
	if (SetScreenPlane::ShowToolTips())
		Atom3Ctl->SetToolTip(_("Enter the index for an atom to define point 3."));
	itemFlexGridSizer4->Add(Atom3Ctl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* itemStaticText19 = new wxStaticText( itemDialog1, wxID_STATIC, _("or point 3"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer4->Add(itemStaticText19, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	Pt3XCtl = new wxTextCtrl( itemDialog1, ID_PT3X, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 0 );
	if (SetScreenPlane::ShowToolTips())
		Pt3XCtl->SetToolTip(_("The x-coordinate for point 3."));
	itemFlexGridSizer4->Add(Pt3XCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	Pt3YCtl = new wxTextCtrl( itemDialog1, ID_PT3Y, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 0 );
	if (SetScreenPlane::ShowToolTips())
		Pt3YCtl->SetToolTip(_("The y-coordinate for point 3."));
	itemFlexGridSizer4->Add(Pt3YCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	Pt3ZCtl = new wxTextCtrl( itemDialog1, ID_PT3Z, wxEmptyString, wxDefaultPosition, wxSize(100, -1), 0 );
	if (SetScreenPlane::ShowToolTips())
		Pt3ZCtl->SetToolTip(_("The z-coordinate for point 3."));
	itemFlexGridSizer4->Add(Pt3ZCtl, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* itemStaticText23 = new wxStaticText( itemDialog1, wxID_STATIC, _("or choose a symbolic plane"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer2->Add(itemStaticText23, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer24, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxButton* itemButton25 = new wxButton( itemDialog1, ID_XY, _("XY"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer24->Add(itemButton25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton26 = new wxButton( itemDialog1, ID_YX, _("YX"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer24->Add(itemButton26, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton27 = new wxButton( itemDialog1, ID_XZ, _("XZ"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer24->Add(itemButton27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton28 = new wxButton( itemDialog1, ID_ZX, _("ZX"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer24->Add(itemButton28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton29 = new wxButton( itemDialog1, ID_YZ, _("YZ"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer24->Add(itemButton29, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton30 = new wxButton( itemDialog1, ID_ZY, _("ZY"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer24->Add(itemButton30, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer31 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer31, 0, wxALIGN_RIGHT|wxALL, 5);

	wxButton* itemButton32 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer31->Add(itemButton32, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton33 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer31->Add(itemButton33, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	// Set validators
	Atom1Ctl->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	Pt1XCtl->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	Pt1YCtl->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	Pt1ZCtl->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	Atom2Ctl->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	Pt2XCtl->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	Pt2YCtl->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	Pt2ZCtl->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	Atom3Ctl->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	Pt3XCtl->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	Pt3YCtl->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
	Pt3ZCtl->SetValidator( wxTextValidator(wxFILTER_NUMERIC) );
////@end SetScreenPlane content construction
	//Adjust the tab order for the controls so that the three atom index controls are in order
	//followed by the three x,y,z block.
	Atom2Ctl->MoveAfterInTabOrder(Atom1Ctl);
	Atom3Ctl->MoveAfterInTabOrder(Atom2Ctl);
}

/*!
 * Should we show tooltips?
 */

bool SetScreenPlane::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap SetScreenPlane::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin SetScreenPlane bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end SetScreenPlane bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon SetScreenPlane::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin SetScreenPlane icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end SetScreenPlane icon retrieval
}

void SetScreenPlane::setPlaneValues(void) {
	wxString field;
	if (atom1 < 0) Atom1Ctl->Clear();
	if (atom2 < 0) Atom2Ctl->Clear();
	if (atom3 < 0) Atom3Ctl->Clear();
	
	field.Printf(wxT("%f"), PlanePts[0].x);
	Pt1XCtl->SetValue(field);
	field.Printf(wxT("%f"), PlanePts[0].y);
	Pt1YCtl->SetValue(field);
	field.Printf(wxT("%f"), PlanePts[0].z);
	Pt1ZCtl->SetValue(field);
	field.Printf(wxT("%f"), PlanePts[1].x);
	Pt2XCtl->SetValue(field);
	field.Printf(wxT("%f"), PlanePts[1].y);
	Pt2YCtl->SetValue(field);
	field.Printf(wxT("%f"), PlanePts[1].z);
	Pt2ZCtl->SetValue(field);
	field.Printf(wxT("%f"), PlanePts[2].x);
	Pt3XCtl->SetValue(field);
	field.Printf(wxT("%f"), PlanePts[2].y);
	Pt3YCtl->SetValue(field);
	field.Printf(wxT("%f"), PlanePts[2].z);
	Pt3ZCtl->SetValue(field);
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void SetScreenPlane::OnOkClick( wxCommandEvent& event )
{
	(void)event;
	MoleculeData * MainData = parent->GetData();
	if (MainData->SetScreenPlane(PlanePts))
		parent->ResetModel(false);
	Destroy();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM1
 */

void SetScreenPlane::OnAtom1Updated( wxCommandEvent& event )
{
	(void)event;
	if ((Atom1Ctl->GetLineLength(0)>0)&&Atom1Ctl->IsModified()) {
		bool validAtom = false;
		wxString text = Atom1Ctl->GetValue();
		unsigned long temp;
		if (text.ToULong(&temp)) {
			temp --;	//convert to zero indexing
			MoleculeData * MainData = parent->GetData();
			Frame * lFrame = MainData->GetCurrentFramePtr();
			if (temp < lFrame->GetNumAtoms()) {
				CPoint3D tp;
				if (lFrame->GetAtomPosition(temp, tp)) {
					validAtom = true;
					atom1 = temp;
					PlanePts[0] = tp;
				}
			}
		}
		if (validAtom)
			setPlaneValues();
		else {
			if (atom1 >= 0) {
				wxString atm;
				atm.Printf(wxT("%d"), atom1);
				Atom1Ctl->SetValue(atm);
			} else
				Atom1Ctl->Clear();
		}
	}
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_PT1X
 */

void SetScreenPlane::OnPt1xUpdated( wxCommandEvent& event )
{
	(void)event;
	if (Pt1XCtl->IsModified()) {
		bool valid = false;
		wxString text = Pt1XCtl->GetValue();
		double temp;
		if (text.ToDouble(&temp)) {
			PlanePts[0].x = temp;
			valid = true;
		}
		if (valid) {
			if (atom1 >= 0) {
				Atom1Ctl->Clear();
				atom1 = -1;
			}
 		}
	}
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_PT1Y
 */

void SetScreenPlane::OnPt1yUpdated( wxCommandEvent& event )
{
	(void)event;
	if (Pt1YCtl->IsModified()) {
		bool valid = false;
		wxString text = Pt1YCtl->GetValue();
		double temp;
		if (text.ToDouble(&temp)) {
			PlanePts[0].y = temp;
			valid = true;
		}
		if (valid) {
			if (atom1 >= 0) {
				Atom1Ctl->Clear();
				atom1 = -1;
			}
 		}
	}
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_PT1Z
 */

void SetScreenPlane::OnPt1zUpdated( wxCommandEvent& event )
{
	(void)event;
	if (Pt1ZCtl->IsModified()) {
		bool valid = false;
		wxString text = Pt1ZCtl->GetValue();
		double temp;
		if (text.ToDouble(&temp)) {
			PlanePts[0].z = temp;
			valid = true;
		}
		if (valid) {
			if (atom1 >= 0) {
				Atom1Ctl->Clear();
				atom1 = -1;
			}
 		}
	}
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM2
 */

void SetScreenPlane::OnAtom2Updated( wxCommandEvent& event )
{
	(void)event;
	if ((Atom2Ctl->GetLineLength(0)>0)&&Atom2Ctl->IsModified()) {
		bool validAtom = false;
		wxString text = Atom2Ctl->GetValue();
		unsigned long temp;
		if (text.ToULong(&temp)) {
			temp --;	//convert to zero indexing
			MoleculeData * MainData = parent->GetData();
			Frame * lFrame = MainData->GetCurrentFramePtr();
			if (temp < lFrame->GetNumAtoms()) {
				CPoint3D tp;
				if (lFrame->GetAtomPosition(temp, tp)) {
					validAtom = true;
					atom2 = temp;
					PlanePts[1] = tp;
				}
			}
		}
		if (validAtom)
			setPlaneValues();
		else {
			if (atom2 >= 0) {
				wxString atm;
				atm.Printf(wxT("%d"), atom2);
				Atom2Ctl->SetValue(atm);
			} else
				Atom2Ctl->Clear();
		}
	}
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_PT2X
 */

void SetScreenPlane::OnPt2xUpdated( wxCommandEvent& event )
{
	(void)event;
	if (Pt2XCtl->IsModified()) {
		bool valid = false;
		wxString text = Pt2XCtl->GetValue();
		double temp;
		if (text.ToDouble(&temp)) {
			PlanePts[1].x = temp;
			valid = true;
		}
		if (valid) {
			if (atom2 >= 0) {
				Atom2Ctl->Clear();
				atom2 = -1;
			}
 		}
	}
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_PT2Y
 */

void SetScreenPlane::OnPt2yUpdated( wxCommandEvent& event )
{
	(void)event;
	if (Pt2YCtl->IsModified()) {
		bool valid = false;
		wxString text = Pt2YCtl->GetValue();
		double temp;
		if (text.ToDouble(&temp)) {
			PlanePts[1].y = temp;
			valid = true;
		}
		if (valid) {
			if (atom2 >= 0) {
				Atom2Ctl->Clear();
				atom2 = -1;
			}
 		}
	}
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_PT2Z
 */

void SetScreenPlane::OnPt2zUpdated( wxCommandEvent& event )
{
	(void)event;
	if (Pt2ZCtl->IsModified()) {
		bool valid = false;
		wxString text = Pt2ZCtl->GetValue();
		double temp;
		if (text.ToDouble(&temp)) {
			PlanePts[1].z = temp;
			valid = true;
		}
		if (valid) {
			if (atom2 >= 0) {
				Atom2Ctl->Clear();
				atom2 = -1;
			}
 		}
	}
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM3
 */

void SetScreenPlane::OnAtom3Updated( wxCommandEvent& event )
{
	(void)event;
	if ((Atom3Ctl->GetLineLength(0)>0)&&Atom3Ctl->IsModified()) {
		bool validAtom = false;
		wxString text = Atom3Ctl->GetValue();
		unsigned long temp;
		if (text.ToULong(&temp)) {
			temp --;	//convert to zero indexing
			MoleculeData * MainData = parent->GetData();
			Frame * lFrame = MainData->GetCurrentFramePtr();
			if (temp < lFrame->GetNumAtoms()) {
				CPoint3D tp;
				if (lFrame->GetAtomPosition(temp, tp)) {
					validAtom = true;
					atom3 = temp;
					PlanePts[2] = tp;
				}
			}
		}
		if (validAtom)
			setPlaneValues();
		else {
			if (atom3 >= 0) {
				wxString atm;
				atm.Printf(wxT("%d"), atom3);
				Atom3Ctl->SetValue(atm);
			} else
				Atom3Ctl->Clear();
		}
	}
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_PT3X
 */

void SetScreenPlane::OnPt3xUpdated( wxCommandEvent& event )
{
	(void)event;
	if (Pt3XCtl->IsModified()) {
		bool valid = false;
		wxString text = Pt3XCtl->GetValue();
		double temp;
		if (text.ToDouble(&temp)) {
			PlanePts[2].x = temp;
			valid = true;
		}
		if (valid) {
			if (atom3 >= 0) {
				Atom3Ctl->Clear();
				atom3 = -1;
			}
 		}
	}
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_PT3Y
 */

void SetScreenPlane::OnPt3yUpdated( wxCommandEvent& event )
{
	(void)event;
	if (Pt3YCtl->IsModified()) {
		bool valid = false;
		wxString text = Pt3YCtl->GetValue();
		double temp;
		if (text.ToDouble(&temp)) {
			PlanePts[2].y = temp;
			valid = true;
		}
		if (valid) {
			if (atom3 >= 0) {
				Atom3Ctl->Clear();
				atom3 = -1;
			}
 		}
	}
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_PT3Z
 */

void SetScreenPlane::OnPt3zUpdated( wxCommandEvent& event )
{
	(void)event;
	if (Pt3ZCtl->IsModified()) {
		bool valid = false;
		wxString text = Pt3ZCtl->GetValue();
		double temp;
		if (text.ToDouble(&temp)) {
			PlanePts[2].z = temp;
			valid = true;
		}
		if (valid) {
			if (atom3 >= 0) {
				Atom3Ctl->Clear();
				atom3 = -1;
			}
 		}
	}
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_XY
 */

void SetScreenPlane::OnXyClick( wxCommandEvent& event )
{
	(void)event;
	atom1 = atom2 = atom3 = -1;
	for (int i=0; i<3; i++)
		PlanePts[i].x = PlanePts[i].y = PlanePts[i].z = 0.0;
	
	PlanePts[1].x = 1.0;
	PlanePts[2].y = 1.0;
	
	setPlaneValues();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_YX
 */

void SetScreenPlane::OnYxClick( wxCommandEvent& event )
{
	(void)event;
	atom1 = atom2 = atom3 = -1;
	for (int i=0; i<3; i++)
		PlanePts[i].x = PlanePts[i].y = PlanePts[i].z = 0.0;
	
	PlanePts[1].y = 1.0;
	PlanePts[2].x = 1.0;
	
	setPlaneValues();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_XZ
 */

void SetScreenPlane::OnXzClick( wxCommandEvent& event )
{
	(void)event;
	atom1 = atom2 = atom3 = -1;
	for (int i=0; i<3; i++)
		PlanePts[i].x = PlanePts[i].y = PlanePts[i].z = 0.0;
	
	PlanePts[1].x = 1.0;
	PlanePts[2].z = 1.0;
	
	setPlaneValues();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ZX
 */

void SetScreenPlane::OnZxClick( wxCommandEvent& event )
{
	(void)event;
	atom1 = atom2 = atom3 = -1;
	for (int i=0; i<3; i++)
		PlanePts[i].x = PlanePts[i].y = PlanePts[i].z = 0.0;
	
	PlanePts[2].x = 1.0;
	PlanePts[1].z = 1.0;
	
	setPlaneValues();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_YZ
 */

void SetScreenPlane::OnYzClick( wxCommandEvent& event )
{
	(void)event;
	atom1 = atom2 = atom3 = -1;
	for (int i=0; i<3; i++)
		PlanePts[i].x = PlanePts[i].y = PlanePts[i].z = 0.0;
	
	PlanePts[1].y = 1.0;
	PlanePts[2].z = 1.0;
	
	setPlaneValues();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ZY
 */

void SetScreenPlane::OnZyClick( wxCommandEvent& event )
{
	(void)event;
	atom1 = atom2 = atom3 = -1;
	for (int i=0; i<3; i++)
		PlanePts[i].x = PlanePts[i].y = PlanePts[i].z = 0.0;
	
	PlanePts[2].y = 1.0;
	PlanePts[1].z = 1.0;
	
	setPlaneValues();
}
