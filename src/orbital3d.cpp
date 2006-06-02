/////////////////////////////////////////////////////////////////////////////
// Name:        orbital3d.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri  2 Jun 10:08:18 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "orbital3d.h"
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

#include "orbital3d.h"

////@begin XPM images
////@end XPM images

/*!
 * Orbital3D type definition
 */

IMPLEMENT_DYNAMIC_CLASS( Orbital3D, wxPanel )

/*!
 * Orbital3D event table definition
 */

BEGIN_EVENT_TABLE( Orbital3D, wxPanel )

////@begin Orbital3D event table entries
    EVT_TEXT( ID_TEXTCTRL5, Orbital3D::OnTextctrl5Updated )

    EVT_CHECKBOX( ID_CHECKBOX3, Orbital3D::OnCheckbox3Click )

    EVT_CHECKBOX( ID_CHECKBOX4, Orbital3D::OnCheckbox4Click )

    EVT_CHOICE( ID_CHOICE1, Orbital3D::OnChoice1Selected )

    EVT_CHECKBOX( ID_CHECKBOX5, Orbital3D::OnCheckbox5Click )

////@end Orbital3D event table entries

END_EVENT_TABLE()

/*!
 * Orbital3D constructors
 */

Orbital3D::Orbital3D( )
{
}

Orbital3D::Orbital3D( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, pos, size, style);
}

/*!
 * Orbital3D creator
 */

bool Orbital3D::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
////@begin Orbital3D member initialisation
    titleEdit = NULL;
    visCheck = NULL;
    allFrameCheck = NULL;
    orbSetChoice = NULL;
    rPhaseCheck = NULL;
    gridSizeSlider = NULL;
    updateButton = NULL;
////@end Orbital3D member initialisation

////@begin Orbital3D creation
    wxPanel::Create( parent, id, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end Orbital3D creation
    return true;
}

/*!
 * Control creation for Orbital3D
 */

void Orbital3D::CreateControls()
{    
////@begin Orbital3D content construction
    Orbital3D* itemPanel1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemPanel1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    titleEdit = new wxTextCtrl( itemPanel1, ID_TEXTCTRL5, _T(""), wxDefaultPosition, wxSize(300, -1), 0 );
    itemBoxSizer3->Add(titleEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    visCheck = new wxCheckBox( itemPanel1, ID_CHECKBOX3, _("Visible"), wxDefaultPosition, wxDefaultSize, 0 );
    visCheck->SetValue(false);
    itemBoxSizer5->Add(visCheck, 0, wxALIGN_LEFT|wxALL, 5);

    allFrameCheck = new wxCheckBox( itemPanel1, ID_CHECKBOX4, _("All Frames"), wxDefaultPosition, wxDefaultSize, 0 );
    allFrameCheck->SetValue(false);
    itemBoxSizer5->Add(allFrameCheck, 0, wxALIGN_LEFT|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemPanel1, wxID_STATIC, _("Select Orbital Set:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer8->Add(itemStaticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString* orbSetChoiceStrings = NULL;
    orbSetChoice = new wxChoice( itemPanel1, ID_CHOICE1, wxDefaultPosition, wxDefaultSize, 0, orbSetChoiceStrings, 0 );
    itemBoxSizer8->Add(orbSetChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    rPhaseCheck = new wxCheckBox( itemPanel1, ID_CHECKBOX5, _("Reverse Phase"), wxDefaultPosition, wxDefaultSize, 0 );
    rPhaseCheck->SetValue(false);
    itemBoxSizer8->Add(rPhaseCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText13 = new wxStaticText( itemPanel1, wxID_STATIC, _("Grid Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer12->Add(itemStaticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    gridSizeSlider = new wxSlider( itemPanel1, ID_GRIDSIZESLIDER1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemBoxSizer12->Add(gridSizeSlider, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer15, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticText* itemStaticText16 = new wxStaticText( itemPanel1, wxID_STATIC, _("Number of\nGrid Points:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer15->Add(itemStaticText16, 0, wxALIGN_TOP|wxALL|wxADJUST_MINSIZE, 5);

    wxSlider* itemSlider17 = new wxSlider( itemPanel1, ID_SLIDER1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemBoxSizer15->Add(itemSlider17, 0, wxALIGN_TOP|wxALL, 5);

    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer15->Add(itemBoxSizer18, 0, wxALIGN_TOP|wxALL, 5);

    wxStaticText* itemStaticText19 = new wxStaticText( itemPanel1, wxID_STATIC, _("Contour Value:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(itemStaticText19, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    wxTextCtrl* itemTextCtrl20 = new wxTextCtrl( itemPanel1, ID_TEXTCTRL6, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer18->Add(itemTextCtrl20, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer15->Add(itemBoxSizer21, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxSlider* itemSlider22 = new wxSlider( itemPanel1, ID_SLIDER2, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemBoxSizer21->Add(itemSlider22, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer23 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer21->Add(itemBoxSizer23, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText24 = new wxStaticText( itemPanel1, wxID_STATIC, _("0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer23->Add(itemStaticText24, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxStaticText* itemStaticText25 = new wxStaticText( itemPanel1, wxID_STATIC, _("0.0"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer23->Add(itemStaticText25, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxBoxSizer* itemBoxSizer26 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer26, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer27 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer26->Add(itemBoxSizer27, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText28 = new wxStaticText( itemPanel1, wxID_STATIC, _("Select Orbital:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer27->Add(itemStaticText28, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString* itemChoice29Strings = NULL;
    wxChoice* itemChoice29 = new wxChoice( itemPanel1, ID_CHOICE2, wxDefaultPosition, wxDefaultSize, 0, itemChoice29Strings, 0 );
    itemBoxSizer27->Add(itemChoice29, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxString* itemListBox30Strings = NULL;
    wxListBox* itemListBox30 = new wxListBox( itemPanel1, ID_LISTBOX, wxDefaultPosition, wxDefaultSize, 0, itemListBox30Strings, wxLB_SINGLE );
    itemBoxSizer27->Add(itemListBox30, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer31 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer26->Add(itemBoxSizer31, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText32 = new wxStaticText( itemPanel1, wxID_STATIC, _("Orbital Vector:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer31->Add(itemStaticText32, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString* itemListBox33Strings = NULL;
    wxListBox* itemListBox33 = new wxListBox( itemPanel1, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, 0, itemListBox33Strings, wxLB_SINGLE );
    itemBoxSizer31->Add(itemListBox33, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer34 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer26->Add(itemBoxSizer34, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    updateButton = new wxButton( itemPanel1, ID_UPDATEBUTTON, _("Update"), wxDefaultPosition, wxDefaultSize, 0 );
    updateButton->SetDefault();
    itemBoxSizer34->Add(updateButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end Orbital3D content construction
}

/*!
 * Should we show tooltips?
 */

bool Orbital3D::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Orbital3D::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin Orbital3D bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end Orbital3D bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Orbital3D::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin Orbital3D icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end Orbital3D icon retrieval
}
/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL5
 */

void Orbital3D::OnTextctrl5Updated( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL5 in Orbital3D.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TEXTCTRL5 in Orbital3D. 
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX3
 */

void Orbital3D::OnCheckbox3Click( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX3 in Orbital3D.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX3 in Orbital3D. 
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX4
 */

void Orbital3D::OnCheckbox4Click( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX4 in Orbital3D.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX4 in Orbital3D. 
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE1
 */

void Orbital3D::OnChoice1Selected( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE1 in Orbital3D.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE1 in Orbital3D. 
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX5
 */

void Orbital3D::OnCheckbox5Click( wxCommandEvent& event )
{
////@begin wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX5 in Orbital3D.
    // Before editing this code, remove the block markers.
    event.Skip();
////@end wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX5 in Orbital3D. 
}


