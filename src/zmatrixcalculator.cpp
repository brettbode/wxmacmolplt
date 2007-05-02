/////////////////////////////////////////////////////////////////////////////
// Name:        zmatrixcalculator.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri 30 Jun 10:43:34 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "zmatrixcalculator.h"
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
#include "Math3D.h"
#include "zmatrixcalculator.h"

////@begin XPM images
////@end XPM images

/*!
 * ZMatrixCalculator type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ZMatrixCalculator, wxDialog )

/*!
 * ZMatrixCalculator event table definition
 */

BEGIN_EVENT_TABLE( ZMatrixCalculator, wxDialog )

////@begin ZMatrixCalculator event table entries
    EVT_CLOSE( ZMatrixCalculator::OnCloseWindow )

    EVT_TEXT( ID_ATOM1EDIT, ZMatrixCalculator::OnAtom1editUpdated )

    EVT_TEXT( ID_ATOM2EDIT, ZMatrixCalculator::OnAtom2editUpdated )

    EVT_TEXT( ID_ATOM3EDIT, ZMatrixCalculator::OnAtom3editUpdated )

    EVT_TEXT( ID_ATOM4EDIT, ZMatrixCalculator::OnAtom4editUpdated )

////@end ZMatrixCalculator event table entries

END_EVENT_TABLE()

/*!
 * ZMatrixCalculator constructors
 */

ZMatrixCalculator::ZMatrixCalculator( )
{
}

ZMatrixCalculator::ZMatrixCalculator( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * ZMatrixCalculator creator
 */

bool ZMatrixCalculator::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ZMatrixCalculator member initialisation
    mAtom1Edit = NULL;
    mAtom2Edit = NULL;
    mAtom3Edit = NULL;
    mAtom4Edit = NULL;
    mBondLength = NULL;
    mBondAngle = NULL;
    mDihedralAngle = NULL;
////@end ZMatrixCalculator member initialisation

////@begin ZMatrixCalculator creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end ZMatrixCalculator creation
    return true;
}

/*!
 * Control creation for ZMatrixCalculator
 */

void ZMatrixCalculator::CreateControls()
{    
////@begin ZMatrixCalculator content construction
    ZMatrixCalculator* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Primary Atom:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mAtom1Edit = new wxTextCtrl( itemDialog1, ID_ATOM1EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(mAtom1Edit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer6->Add(itemBoxSizer7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Second Atom:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(itemStaticText8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    mAtom2Edit = new wxTextCtrl( itemDialog1, ID_ATOM2EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer7->Add(mAtom2Edit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer6->Add(itemBoxSizer10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, _("Third Atom:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemStaticText11, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    mAtom3Edit = new wxTextCtrl( itemDialog1, ID_ATOM3EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(mAtom3Edit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer6->Add(itemBoxSizer13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemDialog1, wxID_STATIC, _("Fourth Atom:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemStaticText14, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    mAtom4Edit = new wxTextCtrl( itemDialog1, ID_ATOM4EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(mAtom4Edit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer16, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer16->Add(itemBoxSizer17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer17->Add(itemBoxSizer18, 0, wxALIGN_RIGHT|wxALL, 5);

    wxStaticText* itemStaticText19 = new wxStaticText( itemDialog1, wxID_STATIC, _("Bond Length:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(itemStaticText19, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mBondLength = new wxTextCtrl( itemDialog1, ID_BONDLENGTHEDIT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    itemBoxSizer18->Add(mBondLength, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer17->Add(itemBoxSizer21, 0, wxALIGN_RIGHT|wxALL, 5);

    wxStaticText* itemStaticText22 = new wxStaticText( itemDialog1, wxID_STATIC, _("Bond Angle:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer21->Add(itemStaticText22, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mBondAngle = new wxTextCtrl( itemDialog1, ID_BONDANGLEEDIT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    itemBoxSizer21->Add(mBondAngle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer24 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer17->Add(itemBoxSizer24, 0, wxALIGN_RIGHT|wxALL, 5);

    wxStaticText* itemStaticText25 = new wxStaticText( itemDialog1, wxID_STATIC, _("Dihedral Angle:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer24->Add(itemStaticText25, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    mDihedralAngle = new wxTextCtrl( itemDialog1, ID_DIHEDRALANGLEEDIT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
    itemBoxSizer24->Add(mDihedralAngle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    // Set validators
    mAtom1Edit->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & atom1) );
    mAtom2Edit->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & atom2) );
    mAtom3Edit->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & atom3) );
    mAtom4Edit->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & atom4) );
////@end ZMatrixCalculator content construction
	mAtom1Edit->SetFocus();
}

/*!
 * Should we show tooltips?
 */

bool ZMatrixCalculator::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ZMatrixCalculator::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ZMatrixCalculator bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ZMatrixCalculator bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ZMatrixCalculator::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ZMatrixCalculator icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ZMatrixCalculator icon retrieval
}
/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM1EDIT
 */

void ZMatrixCalculator::OnAtom1editUpdated( wxCommandEvent& event )
{
	UpdateValues();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM2EDIT
 */

void ZMatrixCalculator::OnAtom2editUpdated( wxCommandEvent& event )
{
	UpdateValues();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM3EDIT
 */

void ZMatrixCalculator::OnAtom3editUpdated( wxCommandEvent& event )
{
	UpdateValues();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ATOM4EDIT
 */

void ZMatrixCalculator::OnAtom4editUpdated( wxCommandEvent& event )
{
	UpdateValues();
    event.Skip();
}

void ZMatrixCalculator::UpdateValues(void) {

	MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	MoleculeData * 	MainData = parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	
	long atm1, atm2, atm3, atm4;
	CPoint3D	offset1, offset2, offset3;
	
	// Make sure first two atoms are valid numbers.  We only do the first two
	// for now, since these establish the bond length, which we want to
	// show as soon as possible.
	if ((!mAtom1Edit->GetValue().ToLong(&atm1)) ||
	    (!mAtom2Edit->GetValue().ToLong(&atm2))) {
	   return;
	}

	// Input atom numbers are 1-based.
	atm1--;
	atm2--;

	// Make sure atoms 1 and 2 are valid and not equal.
	if (!MainData->ValidAtom(atm1) || !MainData->ValidAtom(atm2) ||
		(atm1 == atm2)) {
		return;
	}

	// Calculate bond length.
	CPoint3D pos1, pos2;
	lFrame->GetAtomPosition(atm1, pos1);
	lFrame->GetAtomPosition(atm2, pos2);
	offset1.x = pos2.x - pos1.x;
	offset1.y = pos2.y - pos1.y;
	offset1.z = pos2.z - pos1.z;
	float BondLength = offset1.Magnitude();
	wxString temp;
	temp.Printf(wxT("%.5f"), BondLength);
	mBondLength->SetValue(temp);

	// Now move to atom 3 and make sure it's valid.
	if (!mAtom3Edit->GetValue().ToLong(&atm3)) {
		return;
	}

	atm3--;

	// Make sure atom 3 exists and is not equal to 1 or 2.
	if (!MainData->ValidAtom(atm3) || (atm1 == atm3) || (atm2 == atm3)) {
		return;
	}

	// Calculate angle between bond from atom 3 to atom 1 and bond from
	// atom 2 to atom 3.
	CPoint3D pos3;
	lFrame->GetAtomPosition(atm3, pos3);
	offset2.x = pos1.x - pos3.x;
	offset2.y = pos1.y - pos3.y;
	offset2.z = pos1.z - pos3.z;
	float length3 = offset2.Magnitude();

	offset2.x = pos3.x - pos2.x;
	offset2.y = pos3.y - pos2.y;
	offset2.z = pos3.z - pos2.z;
	float length2 = offset2.Magnitude();

	// Using the law of cosines, find cos(theta), with theta being the angle
	// 123.
	length3 = (BondLength * BondLength + length2 * length2 -
				length3 * length3) / (2 * BondLength * length2);

	if (length3 > 1.0) length3 = 1.0;
	else if (length3 < -1.0) length3 = -1.0;

	// BondAngle is the angle between atoms 1, 2, and 3.
	float BondAngle = acos(length3);
	BondAngle *= kRadToDegree;
	
	temp.Printf(wxT("%.5f"), BondAngle);
	mBondAngle->SetValue(temp);
	
	// Make sure atom 4 is an integral value.
	if (!mAtom4Edit->GetValue().ToLong(&atm4)) {
		return;
	}

	atm4--;

	// Make sure atom 4 exists and is not any of the other atoms.
	if (!MainData->ValidAtom(atm4) || (atm1 == atm4) || (atm2 == atm4) ||
		(atm3 == atm4)) {
		return;
	}

	// Dihedral
	CPoint3D pos4;
	lFrame->GetAtomPosition(atm4, pos4);
	offset3.x = pos4.x - pos3.x;
	offset3.y = pos4.y - pos3.y;
	offset3.z = pos4.z - pos3.z;

	CPoint3D UnitIJ = offset1;
	CPoint3D UnitJK = offset2;
	CPoint3D UnitKL = offset3;
	Normalize3D(&UnitIJ);
	Normalize3D(&UnitJK);
	Normalize3D(&UnitKL);
	CPoint3D Normal1, Normal2;
	CrossProduct3D(&UnitIJ, &UnitJK, &Normal1);
	CrossProduct3D(&UnitJK, &UnitKL, &Normal2);
	float DotPJ = -DotProduct3D(&UnitIJ, &UnitJK);
	float DotPK = -DotProduct3D(&UnitJK, &UnitKL);

	if ((fabs(DotPJ) < 1.0)&&(fabs(DotPK) < 1.0)) {	//3 of the atom are linear, Bad!
		float SinPJ = sqrt(1.0-DotPJ*DotPJ);
		float SinPK = sqrt(1.0-DotPK*DotPK);
		float Dot = DotProduct3D(&Normal1, &Normal2)/(SinPJ*SinPK);
		if (fabs(Dot) <= kCosErrorTolerance) {		//Bad value for a cos
			if (Dot > 1.0) Dot += 1.0-Dot;
			else if (Dot < -1.0) Dot -= 1.0+Dot;
			float Dihedral = acos(Dot);
			float Pi = acos(-1.0);
			if (fabs(Dihedral) < kZeroTolerance) Dihedral = 0.0;
			else if (fabs(Dihedral-Pi) < kZeroTolerance) Dihedral = Pi;
			float Sense = DotProduct3D(&Normal2, &offset1);
			if (Sense < 0.0) Dihedral = -Dihedral;
			Dihedral *= 180.0/Pi;
			
			temp.Printf(wxT("%.5f"), Dihedral);
			mDihedralAngle->SetValue(temp);
		}
	}
}
/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_MYDIALOG6
 */

void ZMatrixCalculator::OnCloseWindow( wxCloseEvent& event )
{
	MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	parent->CloseZMatrixCalc();
}


