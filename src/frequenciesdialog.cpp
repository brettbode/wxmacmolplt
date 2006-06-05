/////////////////////////////////////////////////////////////////////////////
// Name:        frequenciesdialog.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Wed 31 May 2006 12:01:12 PM CDT
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "frequenciesdialog.h"
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
#include "Frame.h"
#include <wx/clipbrd.h>

#include "frequenciesdialog.h"

////@begin XPM images
////@end XPM images

/*!
 * FrequenciesDialog type definition
 */

IMPLEMENT_CLASS( FrequenciesDialog, wxFrame )

/*!
 * FrequenciesDialog event table definition
 */

BEGIN_EVENT_TABLE( FrequenciesDialog, wxFrame )
////@begin FrequenciesDialog event table entries
    EVT_CLOSE( FrequenciesDialog::OnCloseWindow )

    EVT_MENU( wxID_COPY, FrequenciesDialog::OnCopyClick )

    EVT_GRAPH_CLICK(ID_CUSTOM, FrequenciesDialog::OnCustomGraphClick)

////@end FrequenciesDialog event table entries
END_EVENT_TABLE()

/*!
 * FrequenciesDialog constructors
 */

FrequenciesDialog::FrequenciesDialog( )
{
}

FrequenciesDialog::FrequenciesDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create( parent, id, caption, pos, size, style );
}

/*!
 * FrequenciesDialog creator
 */

bool FrequenciesDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin FrequenciesDialog member initialisation
    fGraph = NULL;
////@end FrequenciesDialog member initialisation

////@begin FrequenciesDialog creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end FrequenciesDialog creation

    RegenData();

    return true;
}

/*!
 * Control creation for FrequenciesDialog
 */

void FrequenciesDialog::CreateControls()
{    
////@begin FrequenciesDialog content construction
    FrequenciesDialog* itemFrame1 = this;

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(wxID_COPY, _("&Copy"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("&Edit"));
    itemFrame1->SetMenuBar(menuBar);

    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer5);

    fGraph = new wxMolGraph( itemFrame1, ID_CUSTOM, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER );
    itemBoxSizer5->Add(fGraph, 1, wxGROW|wxALL, 5);

////@end FrequenciesDialog content construction
}

/*!
 * Should we show tooltips?
 */

bool FrequenciesDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap FrequenciesDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin FrequenciesDialog bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end FrequenciesDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon FrequenciesDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin FrequenciesDialog icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end FrequenciesDialog icon retrieval
}
/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_FRAME
 */

void FrequenciesDialog::FrameChanged(void) {
    RegenData();
    Refresh();
}

void FrequenciesDialog::ModeChanged(void) {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    fGraph->setSelection(0, parent->GetData()->cFrame->Vibs->GetCurrentMode());
    Refresh();
}

void FrequenciesDialog::RegenData(void) {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    
    MoleculeData  *mData = parent->GetData();
    VibRec *Vibs = parent->GetData()->cFrame->Vibs;
    
    vector< double > xSetData;
    vector< pair< int, double > > freqData;
    float temp;
    int i = 0;
    

    fGraph->reset();
    
    if(Vibs == NULL) return;
    
    for(i = 0; i < Vibs->GetNumModes(); i++) {
        xSetData.push_back((double)(Vibs->GetFrequency(i)));
        freqData.push_back(make_pair(i, Vibs->GetIntensity(i)));
    }
    fGraph->addXSet(xSetData, true);
    
    fGraph->addYSet(freqData, 0, MG_AXIS_Y1, MG_STYLE_BAR, *wxBLACK);

    //fGraph->setPrecision(eOpts->GetNumDigits());
    fGraph->autoScaleY(MG_AXIS_Y1);
    fGraph->setXAxisMin(0.0);
    fGraph->setSelection(0, mData->CurrentFrame - 1);
}

void FrequenciesDialog::OnCloseWindow( wxCloseEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    parent->CloseFrequenciesWindow();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_COPY
 */

void FrequenciesDialog::OnCopyClick( wxCommandEvent& event )
{
    int width = 0;
    int height = 0;
    fGraph->GetClientSize(&width, &height);
    wxBitmap tempBmp(width, height);
    wxMemoryDC tempDC;

    tempDC.SelectObject(tempBmp);
    fGraph->draw(tempDC);

    if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxBitmapDataObject(tempBmp));
        wxTheClipboard->Close();
    }
    
    tempDC.SelectObject(wxNullBitmap);
}
/*!
 * wxEVT_GRAPH_CLICK event handler for ID_CUSTOM
 */

void FrequenciesDialog::OnCustomGraphClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    parent->ChangeModes(fGraph->getSelection(0));
}


