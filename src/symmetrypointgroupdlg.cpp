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

#include "Globals.h"
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
	EVT_LIST_ITEM_SELECTED( ID_LISTCTRL1, SymmetryPointGroupDlg::OnListctrl1Selected )

	EVT_SLIDER( ID_SLIDER1, SymmetryPointGroupDlg::OnSlider1Updated )

	EVT_BUTTON( ID_SETBUTTON, SymmetryPointGroupDlg::OnSetbuttonClick )
	EVT_UPDATE_UI( ID_SETBUTTON, SymmetryPointGroupDlg::OnSetbuttonUpdate )

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
	Parent = parent;
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
	mSlider = NULL;
////@end SymmetryPointGroupDlg member initialisation
	tolerance = 1.0E-5;
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
	itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mHighestPGText = new wxStaticText( itemDialog1, wxID_STATIC, _("C1"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer3->Add(mHighestPGText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("The highest abelian point group is:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer6->Add(itemStaticText7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mAbelianPGText = new wxStaticText( itemDialog1, wxID_STATIC, _("C1"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer6->Add(mAbelianPGText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer9, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxStaticText* itemStaticText10 = new wxStaticText( itemDialog1, wxID_STATIC, _("This is the list of possible point groups for this geometry:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStaticText10->Wrap(100);
	itemBoxSizer9->Add(itemStaticText10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mPGListCntl = new wxListCtrl( itemDialog1, ID_LISTCTRL1, wxDefaultPosition, wxSize(100, 100), wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL );
	itemBoxSizer9->Add(mPGListCntl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer2->Add(itemBoxSizer12, 0, wxGROW|wxALL, 5);

	wxStaticText* itemStaticText13 = new wxStaticText( itemDialog1, wxID_STATIC, _("Tolerance"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer12->Add(itemStaticText13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mSlider = new wxSlider( itemDialog1, ID_SLIDER1, -5, -7, 2, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	itemBoxSizer12->Add(mSlider, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer12->Add(itemBoxSizer15, 1, wxGROW|wxALL, 5);

	wxStaticText* itemStaticText16 = new wxStaticText( itemDialog1, wxID_STATIC, _("tight"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
	itemBoxSizer15->Add(itemStaticText16, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* itemStaticText17 = new wxStaticText( itemDialog1, wxID_STATIC, _("loose"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	itemBoxSizer15->Add(itemStaticText17, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer2->Add(itemBoxSizer18, 0, wxALIGN_RIGHT|wxALL, 5);

	wxButton* itemButton19 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer18->Add(itemButton19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxButton* itemButton20 = new wxButton( itemDialog1, ID_SETBUTTON, _("&Set Point Group"), wxDefaultPosition, wxDefaultSize, 0 );
	itemButton20->SetDefault();
	itemBoxSizer18->Add(itemButton20, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end SymmetryPointGroupDlg content construction
	setup();
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
void SymmetryPointGroupDlg::setup (void) {
	long itemCount=0;
	wxColour blue(0, 0, 255);
	
	MoleculeData * MainData = Parent->GetData();
	WinPrefs * Prefs = Parent->GetPrefs();
	MainData->DeterminePointGroup(pgFlags, Prefs, tolerance);
	
	//order of the flags
	//c1, cs, ci, c2h-c8h (3-9), c2v-c8v (10-16), c2-c8 (17-23), s2-s8 (24-26),
	//D2d-d8d (27-33), d2h-d8h (34-40), d2-d8 (41-47), Td, Th, T, Oh, O
	GAMESSPointGroup HighestPG=GAMESS_C1, HighestAPG=GAMESS_C1;
	
	mPGListCntl->ClearAll();	//start with a clean slate.
	mPGListCntl->InsertColumn(0, wxT(""));
	mPGListCntl->InsertItem(itemCount, wxT("C1"));
	mPGListCntl->SetItemTextColour(0, blue);
	itemCount++;
	if (pgFlags[1]) {
		mHighestPGText->SetLabel(wxT("Cs"));
		mAbelianPGText->SetLabel(wxT("Cs"));
		mPGListCntl->InsertItem(itemCount, wxT("Cs"));
		mPGListCntl->SetItemTextColour(itemCount, blue);
		itemCount++;
	}
	if (pgFlags[2]) {
		mHighestPGText->SetLabel(wxT("Ci"));
		mAbelianPGText->SetLabel(wxT("Ci"));
		mPGListCntl->InsertItem(itemCount, wxT("Ci"));
		mPGListCntl->SetItemTextColour(itemCount, blue);
		itemCount++;
	}
	if (pgFlags[17]) {
		mHighestPGText->SetLabel(wxT("C2"));
		mAbelianPGText->SetLabel(wxT("C2"));
		mPGListCntl->InsertItem(itemCount, wxT("C2"));
		mPGListCntl->SetItemTextColour(itemCount, blue);
		itemCount++;
	}
	if (pgFlags[18]) {
		mHighestPGText->SetLabel(wxT("C3"));
		mPGListCntl->InsertItem(itemCount, wxT("C3"));
		itemCount++;
	}
	if (pgFlags[19]) {
		mHighestPGText->SetLabel(wxT("C4"));
		mPGListCntl->InsertItem(itemCount, wxT("C4"));
		itemCount++;
	}
	if (pgFlags[20]) {
		mHighestPGText->SetLabel(wxT("C5"));
		mPGListCntl->InsertItem(itemCount, wxT("C5"));
		itemCount++;
	}
	if (pgFlags[21]) {
		mHighestPGText->SetLabel(wxT("C6"));
		mPGListCntl->InsertItem(itemCount, wxT("C6"));
		itemCount++;
	}
	if (pgFlags[22]) {
		mHighestPGText->SetLabel(wxT("C7"));
		mPGListCntl->InsertItem(itemCount, wxT("C7"));
		itemCount++;
	}
	if (pgFlags[23]) {
		mHighestPGText->SetLabel(wxT("C8"));
		mPGListCntl->InsertItem(itemCount, wxT("C8"));
		itemCount++;
	}
	if (pgFlags[3]) {
		mHighestPGText->SetLabel(wxT("C2h"));
		mAbelianPGText->SetLabel(wxT("C2h"));
		HighestPG = HighestAPG = GAMESS_CNH;
		mPGListCntl->InsertItem(itemCount, wxT("C2h"));
		mPGListCntl->SetItemTextColour(itemCount, blue);
		itemCount++;
	}
	if (pgFlags[4]) {
		mHighestPGText->SetLabel(wxT("C3h"));
		HighestPG = GAMESS_CNH;
		mPGListCntl->InsertItem(itemCount, wxT("C3h"));
		itemCount++;
	}
	if (pgFlags[5]) {
		mHighestPGText->SetLabel(wxT("C4h"));
		HighestPG = GAMESS_CNH;
		mPGListCntl->InsertItem(itemCount, wxT("C4h"));
		itemCount++;
	}
	if (pgFlags[6]) {
		mHighestPGText->SetLabel(wxT("C5h"));
		HighestPG = GAMESS_CNH;
		mPGListCntl->InsertItem(itemCount, wxT("C5h"));
		itemCount++;
	}
	if (pgFlags[7]) {
		mHighestPGText->SetLabel(wxT("C6h"));
		HighestPG = GAMESS_CNH;
		mPGListCntl->InsertItem(itemCount, wxT("C6h"));
		itemCount++;
	}
	if (pgFlags[8]) {
		mHighestPGText->SetLabel(wxT("C7h"));
		HighestPG = GAMESS_CNH;
		mPGListCntl->InsertItem(itemCount, wxT("C7h"));
		itemCount++;
	}
	if (pgFlags[9]) {
		mHighestPGText->SetLabel(wxT("C8h"));
		HighestPG = GAMESS_CNH;
		mPGListCntl->InsertItem(itemCount, wxT("C8h"));
		itemCount++;
	}
	if (pgFlags[10]) {
		mHighestPGText->SetLabel(wxT("C2v"));
		mAbelianPGText->SetLabel(wxT("C2v"));
		HighestPG = HighestAPG = GAMESS_CNV;
		mPGListCntl->InsertItem(itemCount, wxT("C2v"));
		mPGListCntl->SetItemTextColour(itemCount, blue);
		itemCount++;
	}
	if (pgFlags[11]) {
		mHighestPGText->SetLabel(wxT("C3v"));
		HighestPG = GAMESS_CNV;
		mPGListCntl->InsertItem(itemCount, wxT("C3v"));
		itemCount++;
	}
	if (pgFlags[12]) {
		mHighestPGText->SetLabel(wxT("C4v"));
		HighestPG = GAMESS_CNV;
		mPGListCntl->InsertItem(itemCount, wxT("C4v"));
		itemCount++;
	}
	if (pgFlags[13]) {
		mHighestPGText->SetLabel(wxT("C5v"));
		HighestPG = GAMESS_CNV;
		mPGListCntl->InsertItem(itemCount, wxT("C5v"));
		itemCount++;
	}
	if (pgFlags[14]) {
		mHighestPGText->SetLabel(wxT("C6v"));
		HighestPG = GAMESS_CNV;
		mPGListCntl->InsertItem(itemCount, wxT("C6v"));
		itemCount++;
	}
	if (pgFlags[15]) {
		mHighestPGText->SetLabel(wxT("C7v"));
		HighestPG = GAMESS_CNV;
		mPGListCntl->InsertItem(itemCount, wxT("C7v"));
		itemCount++;
	}
	if (pgFlags[16]) {
		mHighestPGText->SetLabel(wxT("C8v"));
		HighestPG = GAMESS_CNV;
		mPGListCntl->InsertItem(itemCount, wxT("C8v"));
		itemCount++;
	}
	if (pgFlags[24]) {
		if (HighestPG < GAMESS_CNH) {
			mHighestPGText->SetLabel(wxT("S4"));
		}
		mPGListCntl->InsertItem(itemCount, wxT("S4"));
		itemCount++;
	}
	if (pgFlags[25]) {
		if (HighestPG < GAMESS_CNH)
			mHighestPGText->SetLabel(wxT("S6"));
		mPGListCntl->InsertItem(itemCount, wxT("S6"));
		itemCount++;
	}
	if (pgFlags[26]) {
		if (HighestPG < GAMESS_CNH)
			mHighestPGText->SetLabel(wxT("S8"));
		mPGListCntl->InsertItem(itemCount, wxT("S8"));
		itemCount++;
	}
	if (pgFlags[41]) {
		mHighestPGText->SetLabel(wxT("D2"));
		mAbelianPGText->SetLabel(wxT("D2"));
		mPGListCntl->InsertItem(itemCount, wxT("D2"));
		mPGListCntl->SetItemTextColour(itemCount, blue);
		itemCount++;
	}
	if (pgFlags[42]) {
		mHighestPGText->SetLabel(wxT("D3"));
		mPGListCntl->InsertItem(itemCount, wxT("D3"));
		itemCount++;
	}
	if (pgFlags[43]) {
		mHighestPGText->SetLabel(wxT("D4"));
		mPGListCntl->InsertItem(itemCount, wxT("D4"));
		itemCount++;
	}
	if (pgFlags[44]) {
		mHighestPGText->SetLabel(wxT("D5"));
		mPGListCntl->InsertItem(itemCount, wxT("D5"));
		itemCount++;
	}
	if (pgFlags[45]) {
		mHighestPGText->SetLabel(wxT("D6"));
		mPGListCntl->InsertItem(itemCount, wxT("D6"));
		itemCount++;
	}
	if (pgFlags[46]) {
		mHighestPGText->SetLabel(wxT("D7"));
		mPGListCntl->InsertItem(itemCount, wxT("D7"));
		itemCount++;
	}
	if (pgFlags[47]) {
		mHighestPGText->SetLabel(wxT("D8"));
		mPGListCntl->InsertItem(itemCount, wxT("D8"));
		itemCount++;
	}
	if (pgFlags[27]) {
		mHighestPGText->SetLabel(wxT("D2d"));
		mPGListCntl->InsertItem(itemCount, wxT("D2d"));
		itemCount++;
	}
	if (pgFlags[28]) {
		mHighestPGText->SetLabel(wxT("D3d"));
		mPGListCntl->InsertItem(itemCount, wxT("D3d"));
		itemCount++;
	}
	if (pgFlags[29]) {
		mHighestPGText->SetLabel(wxT("D4d"));
		mPGListCntl->InsertItem(itemCount, wxT("D4d"));
		itemCount++;
	}
	if (pgFlags[30]) {
		mHighestPGText->SetLabel(wxT("D5d"));
		mPGListCntl->InsertItem(itemCount, wxT("D5d"));
		itemCount++;
	}
	if (pgFlags[31]) {
		mHighestPGText->SetLabel(wxT("D6d"));
		mPGListCntl->InsertItem(itemCount, wxT("D6d"));
		itemCount++;
	}
	if (pgFlags[32]) {
		mHighestPGText->SetLabel(wxT("D7d"));
		mPGListCntl->InsertItem(itemCount, wxT("D7d"));
		itemCount++;
	}
	if (pgFlags[33]) {
		mHighestPGText->SetLabel(wxT("D8d"));
		mPGListCntl->InsertItem(itemCount, wxT("D8d"));
		itemCount++;
	}
	if (pgFlags[34]) {
		mHighestPGText->SetLabel(wxT("D2h"));
		mAbelianPGText->SetLabel(wxT("D2h"));
		mPGListCntl->InsertItem(itemCount, wxT("D2h"));
		mPGListCntl->SetItemTextColour(itemCount, blue);
		itemCount++;
	}
	if (pgFlags[35]) {
		mHighestPGText->SetLabel(wxT("D3h"));
		mPGListCntl->InsertItem(itemCount, wxT("D3h"));
		itemCount++;
	}
	if (pgFlags[36]) {
		mHighestPGText->SetLabel(wxT("D4h"));
		mPGListCntl->InsertItem(itemCount, wxT("D4h"));
		itemCount++;
	}
	if (pgFlags[37]) {
		mHighestPGText->SetLabel(wxT("D5h"));
		mPGListCntl->InsertItem(itemCount, wxT("D5h"));
		itemCount++;
	}
	if (pgFlags[38]) {
		mHighestPGText->SetLabel(wxT("D6h"));
		mPGListCntl->InsertItem(itemCount, wxT("D6h"));
		itemCount++;
	}
	if (pgFlags[39]) {
		mHighestPGText->SetLabel(wxT("D7h"));
		mPGListCntl->InsertItem(itemCount, wxT("D7h"));
		itemCount++;
	}
	if (pgFlags[40]) {
		mHighestPGText->SetLabel(wxT("D8h"));
		mPGListCntl->InsertItem(itemCount, wxT("D8h"));
		itemCount++;
	}
	if (pgFlags[50]) {
		mHighestPGText->SetLabel(wxT("T"));
		mPGListCntl->InsertItem(itemCount, wxT("T"));
		itemCount++;
	}
	if (pgFlags[49]) {
		mHighestPGText->SetLabel(wxT("Th"));
		mPGListCntl->InsertItem(itemCount, wxT("Th"));
		itemCount++;
	}
	if (pgFlags[48]) {
		mHighestPGText->SetLabel(wxT("Td"));
		mPGListCntl->InsertItem(itemCount, wxT("Td"));
		itemCount++;
	}
	if (pgFlags[52]) {
		mHighestPGText->SetLabel(wxT("O"));
		mPGListCntl->InsertItem(itemCount, wxT("O"));
		itemCount++;
	}
	if (pgFlags[51]) {
		mHighestPGText->SetLabel(wxT("Oh"));
		mPGListCntl->InsertItem(itemCount, wxT("Oh"));
		itemCount++;
	}
	selection = itemCount-1;
	mPGListCntl->SetItemState(itemCount-1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

bool SymmetryPointGroupDlg::GetSelectedPointGroup(GAMESSPointGroup & pg, int & order) const {
	bool result = false;
	int selectedItem = selection;
	int itemCount = 1;
	order = 1;
	pg = GAMESS_C1;
	if (selectedItem == 0) return true;
	if (pgFlags[1]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CS;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[2]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CI;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[17]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CN;
			order = 2;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[18]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CN;
			order = 3;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[19]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CN;
			order = 4;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[20]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CN;
			order = 5;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[21]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CN;
			order = 6;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[22]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CN;
			order = 7;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[23]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CN;
			order = 8;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[3]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNH;
			order = 2;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[4]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNH;
			order = 3;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[5]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNH;
			order = 4;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[6]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNH;
			order = 5;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[7]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNH;
			order = 6;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[8]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNH;
			order = 7;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[9]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNH;
			order = 8;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[10]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNV;
			order = 2;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[11]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNV;
			order = 3;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[12]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNV;
			order = 4;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[13]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNV;
			order = 5;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[14]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNV;
			order = 6;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[15]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNV;
			order = 7;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[16]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_CNV;
			order = 8;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[24]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_S2N;
			order = 2;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[25]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_S2N;
			order = 3;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[26]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_S2N;
			order = 4;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[41]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DN;
			order = 2;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[42]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DN;
			order = 3;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[43]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DN;
			order = 4;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[44]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DN;
			order = 5;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[45]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DN;
			order = 6;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[46]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DN;
			order = 7;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[47]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DN;
			order = 8;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[27]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DND;
			order = 2;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[28]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DND;
			order = 3;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[29]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DND;
			order = 4;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[30]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DND;
			order = 5;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[31]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DND;
			order = 6;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[32]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DND;
			order = 7;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[33]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DND;
			order = 8;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[34]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DNH;
			order = 2;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[35]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DNH;
			order = 3;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[36]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DNH;
			order = 4;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[37]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DNH;
			order = 5;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[38]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DNH;
			order = 6;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[39]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DNH;
			order = 7;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[40]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_DNH;
			order = 8;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[50]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_T;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[49]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_TH;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[48]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_TD;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[50]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_O;
			return true;
		}
		itemCount++;
	}
	if (pgFlags[51]) {
		if (selectedItem == itemCount) {
			pg = GAMESS_OH;
			return true;
		}
		itemCount++;
	}
	
	return result;
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_SETBUTTON
 */
void SymmetryPointGroupDlg::OnSetbuttonClick( wxCommandEvent& event )
{
	EndModal(ID_SETBUTTON);
	event.Skip();
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_SETBUTTON
 */

void SymmetryPointGroupDlg::OnSetbuttonUpdate( wxUpdateUIEvent& event )
{
	event.Enable(mPGListCntl->GetSelectedItemCount() != 0);
	event.Skip();
}

/*!
 * wxEVT_COMMAND_LIST_ITEM_SELECTED event handler for ID_LISTCTRL1
 */

void SymmetryPointGroupDlg::OnListctrl1Selected( wxListEvent& event )
{
	if (selection != event.GetIndex()) {
		mPGListCntl->SetItemState(selection, 0, wxLIST_STATE_SELECTED);
		selection =	event.GetIndex();
		mPGListCntl->SetItemState(selection, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	}
	event.Skip();
}


/*!
 * wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER1
 */

void SymmetryPointGroupDlg::OnSlider1Updated( wxCommandEvent& event ) {
	int val = mSlider->GetValue();
	if (val < 0) tolerance = pow(10.0, val);
	else tolerance = 0.1 * (val+2);
	setup();
//	event.Skip();
}

