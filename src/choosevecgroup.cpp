/////////////////////////////////////////////////////////////////////////////
// Name:        choosevecgroup.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri 26 May 08:35:16 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "choosevecgroup.h"
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
#include "BFiles.h"

#include "choosevecgroup.h"

////@begin XPM images
////@end XPM images

/*!
 * ChooseVECgroup type definition
 */

IMPLEMENT_DYNAMIC_CLASS( ChooseVECgroup, wxDialog )

/*!
 * ChooseVECgroup event table definition
 */

BEGIN_EVENT_TABLE( ChooseVECgroup, wxDialog )

////@begin ChooseVECgroup event table entries
    EVT_GRID_EDITOR_SHOWN( ChooseVECgroup::OnEditorShown )

    EVT_UPDATE_UI( wxID_OK, ChooseVECgroup::OnOkUpdate )

////@end ChooseVECgroup event table entries

END_EVENT_TABLE()

/*!
 * ChooseVECgroup constructors
 */

ChooseVECgroup::ChooseVECgroup( )
{
}

ChooseVECgroup::ChooseVECgroup( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * ChooseVECgroup creator
 */

bool ChooseVECgroup::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin ChooseVECgroup member initialisation
    list = NULL;
////@end ChooseVECgroup member initialisation

////@begin ChooseVECgroup creation
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end ChooseVECgroup creation
    return true;
}

/*!
 * Control creation for ChooseVECgroup
 */

void ChooseVECgroup::CreateControls()
{    
////@begin ChooseVECgroup content construction
    ChooseVECgroup* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, _("The following $VEC groups were located in the selected file. Please select the target $VEC group to import."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticText3->Wrap(400);
    itemBoxSizer2->Add(itemStaticText3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    list = new wxGrid( itemDialog1, ID_GRID, wxDefaultPosition, wxSize(516, 300), wxSUNKEN_BORDER|wxVSCROLL );
    if (ShowToolTips())
        list->SetToolTip(_("Choose the $VEC group to import"));
    list->SetDefaultColSize(500);
    list->SetDefaultRowSize(60);
    list->SetColLabelSize(0);
    list->SetRowLabelSize(0);
    list->CreateGrid(1, 1, wxGrid::wxGridSelectRows);
    itemBoxSizer2->Add(list, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer5, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton6 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton7 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end ChooseVECgroup content construction
}

/*!
 * Should we show tooltips?
 */

bool ChooseVECgroup::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap ChooseVECgroup::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin ChooseVECgroup bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end ChooseVECgroup bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon ChooseVECgroup::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin ChooseVECgroup icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end ChooseVECgroup icon retrieval
}

/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void ChooseVECgroup::OnOkUpdate( wxUpdateUIEvent& event )
{
	if (list->IsSelection()) event.Enable(true);
    event.Skip();
}
void ChooseVECgroup::SetBuffer(BufferFile * Buffer) {
	int vecCount=0;
	list->DeleteRows(0, list->GetNumberRows(), true);
	while (Buffer->LocateKeyWord("$VEC", 4)) {
		char Label[3*kMaxLineLength];
		//Read $vec label lines
		long VecPos = Buffer->GetFilePos() - 2;
		long LabelLength;
		LabelLength = 0;
		if (VecPos > 0) {
			Buffer->BackupnLines(3);
			long back3Pos = Buffer->GetFilePos();
			//scan the up to 3 lines
			while (VecPos - Buffer->GetFilePos() > 0) {
				Buffer->GetLine(Label);	//actually looking for $end from a previous group
				if (-1 < FindKeyWord(Label, "$END", 4))
					back3Pos = Buffer->GetFilePos();
			}
			LabelLength = VecPos - back3Pos + 1;
			if (LabelLength > 2) {
				Buffer->SetFilePos(back3Pos);
				Buffer->Read((Ptr) Label, LabelLength-1);	//note: buffer position should be back at $vec
				Label[LabelLength-1] = 0;	//make it a proper C string
			}
		}
		if (LabelLength <= 2) {strcpy(Label, "No label"); LabelLength = 9;}
		list->InsertRows(vecCount, 1, true);
		list->SetCellValue(vecCount, 0, wxString(Label, wxConvUTF8));
		vecCount++;
		if (VecPos < 0) VecPos = 0;
		Buffer->SetFilePos(VecPos);
		Buffer->SkipnLines(2);
	}
	list->SelectRow(0, true);
}
int ChooseVECgroup::GetTarget(void) {
	return list->GetGridCursorRow();
}



/*!
 * wxEVT_GRID_EDITOR_SHOWN event handler for ID_GRID
 */

void ChooseVECgroup::OnEditorShown( wxGridEvent& event )
{
    event.Veto();
}


