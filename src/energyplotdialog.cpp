/////////////////////////////////////////////////////////////////////////////
// Name:        energyplotdialog.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Tue 23 May 2006 01:34:03 PM CDT
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "energyplotdialog.h"
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
#include "Gradient.h"
#include <wx/clipbrd.h>
#include "colorArea.h"

#include "energyplotdialog.h"
#include <iostream>

////@begin XPM images
////@end XPM images

/*!
 * EnergyPlotDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( EnergyPlotDialog, wxFrame )

/*!
 * EnergyPlotDialog event table definition
 */

BEGIN_EVENT_TABLE( EnergyPlotDialog, wxFrame )
////@begin EnergyPlotDialog event table entries
	EVT_CLOSE( EnergyPlotDialog::OnCloseWindow )
	EVT_MENU( wxID_CLOSE, EnergyPlotDialog::OnCloseEvent )
	EVT_MENU( wxID_COPY, EnergyPlotDialog::OnCopyClick )
////@end EnergyPlotDialog event table entries
	// Key down should work, but I do not get any key events, so instead tackling from the other
	// approach using the Char_hook
//	EVT_KEY_DOWN (EnergyPlotDialog::KeyHandler)
	EVT_CHAR_HOOK (EnergyPlotDialog::KeyHandler)

	EVT_MENU( wxID_COPY, EnergyPlotDialog::OnCopyClick )

	EVT_AXIS_DCLICK(ID_EPGRAPH, EnergyPlotDialog::OnEpgraphAxisDClick)
	EVT_GRAPH_CLICK(ID_EPGRAPH, EnergyPlotDialog::OnEpgraphGraphClick)
END_EVENT_TABLE()

/*!
 * EnergyPlotDialog constructors
 */

EnergyPlotDialog::EnergyPlotDialog( )
{
}

EnergyPlotDialog::EnergyPlotDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create(parent, id, caption, pos, size, style);
}

/*!
 * EnergyPlotDialog creator
 */

bool EnergyPlotDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin EnergyPlotDialog member initialisation
	epGraph = NULL;
////@end EnergyPlotDialog member initialisation

////@begin EnergyPlotDialog creation
	wxFrame::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end EnergyPlotDialog creation
	UpdateWindowTitle();

    // Add data to epGraph
    RegenData();
    
    return true;
}

/*!
 * Update the Window title in the event the file is saved
 */
void EnergyPlotDialog::UpdateWindowTitle(void) {
	MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	wxString foo = parent->GetTitle();
	foo += wxString(_(" ")) + wxString(SYMBOL_ENERGYPLOTDIALOG_TITLE);
	SetTitle(foo);
}

/*!
 * Control creation for EnergyPlotDialog
 */

void EnergyPlotDialog::CreateControls()
{    
////@begin EnergyPlotDialog content construction
	EnergyPlotDialog* itemFrame1 = this;

	wxMenuBar* menuBar = new wxMenuBar;
	wxMenu* itemMenu3 = new wxMenu;
	itemMenu3->Append(wxID_NEW, _("&New\tCtrl+N"), _T(""), wxITEM_NORMAL);
	itemMenu3->Append(wxID_OPEN, _("&Open ...\tCtrl+O"), _T(""), wxITEM_NORMAL);
	itemMenu3->Append(wxID_CLOSE, _("&Close Window\tCtrl+W"), wxEmptyString, wxITEM_NORMAL);
	itemMenu3->AppendSeparator();
	itemMenu3->Append(wxID_EXIT, _("&Quit\tCtrl+Q"), wxEmptyString, wxITEM_NORMAL);
	menuBar->Append(itemMenu3, _("&File"));
	wxMenu* itemMenu7 = new wxMenu;
	itemMenu7->Append(wxID_COPY, _("&Copy\tCtrl+C"), wxEmptyString, wxITEM_NORMAL);
	itemMenu7->AppendSeparator();
	itemMenu7->Append(wxID_PREFERENCES, wxT("Global Pr&eferences"), wxT("Edit the default preferences for new windows"));
	menuBar->Append(itemMenu7, _("&Edit"));
	itemFrame1->SetMenuBar(menuBar);

	wxBoxSizer* itemBoxSizer9 = new wxBoxSizer(wxVERTICAL);
	itemFrame1->SetSizer(itemBoxSizer9);

	wxPanel* itemPanel10 = new wxPanel( itemFrame1, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
	itemBoxSizer9->Add(itemPanel10, 1, wxGROW, 0);

	wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxVERTICAL);
	itemPanel10->SetSizer(itemBoxSizer11);

	epGraph = new wxMolGraph( itemPanel10, ID_EPGRAPH, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER );
	itemBoxSizer11->Add(epGraph, 1, wxGROW|wxALL, 5);

////@end EnergyPlotDialog content construction
	
	wxMenu * menuWindow = new wxMenu;
	menuWindow->Append(MMP_MOLECULEDISPLAYWINDOW, wxT("&Molecule Display"), _("The primary molecule display"));
	menuWindow->Append(MMP_BONDSWINDOW, wxT("&Bonds"), _("View/edit the bonding within the molecule"));
	menuWindow->Append(MMP_COORDSWINDOW, wxT("&Coordinates"), _("View/edit cartesian or internal coordinates"));
	menuWindow->Append(MMP_FREQUENCIESWINDOW, wxT("&Frequencies"), _("Plot the vibrational frequencies"));
	menuWindow->Append(MMP_INPUTBUILDERWINDOW, wxT("&Input Builder"), _T("Generate a GAMESS input file"));
	menuWindow->Append(MMP_SURFACESWINDOW, wxT("&Surfaces"), _T("Add/Edit/Remove various surface types"));
	menuWindow->Append(MMP_ZMATRIXCALC, wxT("&Z-Matrix Calculator"), _("Compute bond lengths/angles or dihedrals between any set of atoms"));
	menuWindow->Append(MMP_LOCAL_PREFERENCES, wxT("Pr&eferences"), _T("Edit the preferences for this window"));
	menuBar->Append(menuWindow, wxT("&Subwindow"));
 
	wxMenu * menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, wxT("&About MacMolPlt..."), _T("Learn about MacMolPlt"));
	menuHelp->Append(wxID_HELP, wxT("&MacMolPlt Manual..."), _T("Brief documentation"));
	menuBar->Append(menuHelp, wxT("&Help"));
}

/*!
 * Should we show tooltips?
 */

bool EnergyPlotDialog::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap EnergyPlotDialog::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin EnergyPlotDialog bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end EnergyPlotDialog bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon EnergyPlotDialog::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin EnergyPlotDialog icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end EnergyPlotDialog icon retrieval
}

void EnergyPlotDialog::FrameChanged(void) {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();

    epGraph->setSelection(0, (int)(parent->GetData()->GetCurrentFrame()) - 1);
}

void EnergyPlotDialog::RegenData(void) {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    
    MoleculeData  *mData = parent->GetData();
    EnergyOptions *eOpts = parent->GetPrefs()->GetEnergyOptions();
    GraphOptions  *gOpts = parent->GetPrefs()->GetGraphOptions();
    
    vector< double > xSetData;
    vector< pair< int, double > > totalEnergyData;
    vector< pair< int, double > > potentialEnergyData;
    vector< pair< int, double > > mp2EnergyData;
    vector< pair< int, double > > kineticEnergyData;
    vector< pair< int, double > > rmsGradData;
    vector< pair< int, double > > maxGradData;
    vector< pair< int, double > > bondLenData;
    vector< pair< int, double > > bondAngleData;
    Frame *currFrame = NULL;
    long atom1 = gOpts->Get1stAtom();
    long atom2 = gOpts->Get2ndAtom();
    long atom3 = gOpts->Get3rdAtom();
    int i = 0;
    int size = (int)(eOpts->GetEPlotPointSize());


    epGraph->reset();
    
    for(currFrame = mData->Frames, i = 0; currFrame != NULL; currFrame = currFrame->NextFrame, i++) {
        xSetData.push_back((double)(currFrame->time));
        totalEnergyData.push_back(make_pair(i, currFrame->Energy));
        potentialEnergyData.push_back(make_pair(i, currFrame->Energy - currFrame->GetKineticEnergy()));
        mp2EnergyData.push_back(make_pair(i, currFrame->GetMP2Energy()));
        kineticEnergyData.push_back(make_pair(i, currFrame->GetKineticEnergy()));
        if(currFrame->Gradient != NULL) rmsGradData.push_back(make_pair(i, (double)(currFrame->Gradient->GetRMS())));
        if(currFrame->Gradient != NULL) maxGradData.push_back(make_pair(i, (double)(currFrame->Gradient->GetMaximum())));
		float length;
		if (currFrame->GetBondLength(atom1, atom2, &length))
			bondLenData.push_back(make_pair(i, length));
		float angle;
 		if (currFrame->GetBondAngle(atom1, atom2, atom3, &angle))
			bondAngleData.push_back(make_pair(i, angle));
    }
    epGraph->addXSet(xSetData, true);
    
    epGraph->addYSet(totalEnergyData, 0, MG_AXIS_Y1, MG_STYLE_POINT_LINE, RGB2WX(*(eOpts->GetTEColor())), MG_SHAPE_CIRCLE, size);
    epGraph->addYSet(potentialEnergyData, 0, MG_AXIS_Y1, MG_STYLE_POINT_LINE, RGB2WX(*(eOpts->GetPEColor())), MG_SHAPE_CIRCLE, size);
    epGraph->addYSet(mp2EnergyData, 0, MG_AXIS_Y1, MG_STYLE_POINT_LINE, RGB2WX(*(eOpts->GetMPColor())), MG_SHAPE_CIRCLE, size);
    epGraph->addYSet(kineticEnergyData, 0, MG_AXIS_Y1, MG_STYLE_POINT_LINE, RGB2WX(*(eOpts->GetKEColor())), MG_SHAPE_CIRCLE, size);
    epGraph->addYSet(rmsGradData, 0, MG_AXIS_Y2, MG_STYLE_POINT_LINE, RGB2WX(*(eOpts->GetKEColor())), MG_SHAPE_CIRCLE, size);
    epGraph->addYSet(maxGradData, 0, MG_AXIS_Y2, MG_STYLE_POINT_LINE, RGB2WX(*(eOpts->GetKEColor())), MG_SHAPE_CIRCLE, size);
    epGraph->addYSet(bondLenData, 0, MG_AXIS_Y2, MG_STYLE_POINT_LINE, RGB2WX(*(eOpts->GetKEColor())), MG_SHAPE_CIRCLE, size);
    epGraph->addYSet(bondAngleData, 0, MG_AXIS_Y2, MG_STYLE_POINT_LINE, RGB2WX(*(eOpts->GetKEColor())), MG_SHAPE_CIRCLE, size);
    if(!eOpts->PlotEnergy()) {
        epGraph->setVisible(0, 0, false);
    }
    if(!eOpts->PlotPEnergy()) {
        epGraph->setVisible(0, 1, false);
    }
    if(!eOpts->PlotMPEnergy()) {
        epGraph->setVisible(0, 2, false);
    }
    if(!eOpts->PlotKEnergy()) {
        epGraph->setVisible(0, 3, false);
    }
    if(!gOpts->PlotRMSGradient()) {
        epGraph->setVisible(0, 4, false);
    }
    if(!gOpts->PlotMaxGradient()) {
        epGraph->setVisible(0, 5, false);
    }
    
    if(!gOpts->PlotBondLength()) {
        epGraph->setVisible(0, 6, false);
    }
    if(!gOpts->PlotBondAngle()) {
        epGraph->setVisible(0, 7, false);
    }
    
    
    epGraph->setOffsetY(MG_AXIS_Y1, eOpts->GetY1Zero());
    epGraph->setOffsetY(MG_AXIS_Y2, eOpts->GetY2Zero());
    epGraph->setPrecision(eOpts->GetNumDigits());
    epGraph->autoScaleY(MG_AXIS_Y1);
    epGraph->autoScaleY(MG_AXIS_Y2);
    epGraph->setSelection(0, mData->CurrentFrame - 1);
}

/*!
 * EVT_GRAPH_CLICK event handler for ID_EPGRAPH
 */

void EnergyPlotDialog::OnEpgraphGraphClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    
    parent->ChangeFrames(epGraph->getSelection(0) + 1);
}

/*!
 * wxEVT_AXIS_DCLICK event handler for ID_EPGRAPH
 */

void EnergyPlotDialog::OnEpgraphAxisDClick( wxCommandEvent& event )
{
////@begin wxEVT_AXIS_DCLICK event handler for ID_EPGRAPH in EnergyPlotDialog.
////@end wxEVT_AXIS_DCLICK event handler for ID_EPGRAPH in EnergyPlotDialog. 
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_ENERGYPLOTDIALOG
 */

void EnergyPlotDialog::OnCloseEvent( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    parent->CloseEnergy_plotWindow();
}


/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENU
 */

void EnergyPlotDialog::OnCopyClick( wxCommandEvent& event )
{
    int width = 0;
    int height = 0;
    epGraph->GetClientSize(&width, &height);
    wxBitmap tempBmp(width, height);
    wxMemoryDC tempDC;

    tempDC.SelectObject(tempBmp);
    epGraph->draw(tempDC);

    if (wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxBitmapDataObject(tempBmp));
        wxTheClipboard->Close();
    }
    
    tempDC.SelectObject(wxNullBitmap);
}

void EnergyPlotDialog::CopyToBitMap(wxBitmap ** target) {
    int width = 0;
    int height = 0;
    epGraph->GetClientSize(&width, &height);
    *target = new wxBitmap(width, height);
    wxMemoryDC tempDC;
	
    tempDC.SelectObject(**target);
    epGraph->draw(tempDC);

    tempDC.SelectObject(wxNullBitmap);
}

void EnergyPlotDialog::KeyHandler(wxKeyEvent & event) {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	int key = event.GetKeyCode();
	if (!event.HasModifiers()) {
		switch (key) {
			case WXK_LEFT:
			case WXK_RIGHT:
			case WXK_HOME:
			case WXK_END:
				parent->KeyHandler(event);
				break;
		}
	}	event.Skip();
}


/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_EPLOTDIALOG
 */

void EnergyPlotDialog::OnCloseWindow( wxCloseEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    parent->CloseEnergy_plotWindow();
}



