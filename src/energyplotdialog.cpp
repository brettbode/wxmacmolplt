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
#include "Gradient.h"

#include "energyplotdialog.h"
#include <iostream>

////@begin XPM images
////@end XPM images

/*!
 * EnergyPlotDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( EnergyPlotDialog, wxDialog )

/*!
 * EnergyPlotDialog event table definition
 */

BEGIN_EVENT_TABLE( EnergyPlotDialog, wxDialog )
////@begin EnergyPlotDialog event table entries
    EVT_CLOSE( EnergyPlotDialog::OnCloseWindow )

    EVT_AXIS_DCLICK(ID_EPGRAPH, EnergyPlotDialog::OnEpgraphAxisDClick)
    EVT_GRAPH_CLICK(ID_EPGRAPH, EnergyPlotDialog::OnEpgraphGraphClick)

////@end EnergyPlotDialog event table entries
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
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end EnergyPlotDialog creation

    // Add data to epGraph
    RegenData();
    
    return true;
}

/*!
 * Control creation for EnergyPlotDialog
 */

void EnergyPlotDialog::CreateControls()
{    
////@begin EnergyPlotDialog content construction
    EnergyPlotDialog* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    epGraph = new wxMolGraph( itemDialog1, ID_EPGRAPH, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER );
    itemBoxSizer2->Add(epGraph, 1, wxGROW|wxALL, 5);

////@end EnergyPlotDialog content construction
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
    //vector< pair< int, double > > rmsGradData;
    //vector< pair< int, double > > maxGradData;
    //vector< pair< int, double > > bondLenData;
    //vector< pair< int, double > > bondAngleData;
    Frame *currFrame = NULL;
    float temp;
    //long atom1 = 0;
    //long atom2 = 0;
    //long atom3 = 0;
    int i = 0;
    

    epGraph->reset();
    
    for(currFrame = mData->Frames, i = 0; currFrame != NULL; currFrame = currFrame->NextFrame, i++) {
        xSetData.push_back((double)(currFrame->time));
        totalEnergyData.push_back(make_pair(i, currFrame->Energy));
        potentialEnergyData.push_back(make_pair(i, currFrame->Energy - currFrame->KE));
        mp2EnergyData.push_back(make_pair(i, currFrame->MP2Energy));
        kineticEnergyData.push_back(make_pair(i, currFrame->KE));
        //rmsGradData.push_back(make_pair(i, (double)(currFrame->Gradient->GetRMS())));
        //maxGradData.push_back(make_pair(i, (double)(currFrame->Gradient->GetMaximum())));
        //bondLenData.push_back(make_pair(i, ));
        //bondAngleData.push_back(make_pair(i, ));
    }
    epGraph->addXSet(xSetData, true);
    
    epGraph->addYSet(totalEnergyData, 0, MG_AXIS_Y1, MG_STYLE_POINT_LINE, RGB2WX(*(eOpts->GetTEColor())));
    epGraph->addYSet(potentialEnergyData, 0, MG_AXIS_Y1, MG_STYLE_POINT_LINE, RGB2WX(*(eOpts->GetPEColor())));
    epGraph->addYSet(mp2EnergyData, 0, MG_AXIS_Y1, MG_STYLE_POINT_LINE, RGB2WX(*(eOpts->GetMPColor())));
    epGraph->addYSet(kineticEnergyData, 0, MG_AXIS_Y1, MG_STYLE_POINT_LINE, RGB2WX(*(eOpts->GetKEColor())));
    //epGraph->addYSet(rmsGradData, 0, MG_AXIS_Y2, MG_STYLE_POINT_LINE, RGB2WX(*(/* TODO */)));
    //epGraph->addYSet(maxGradData, 0, MG_AXIS_Y2, MG_STYLE_POINT_LINE, RGB2WX(*(/* TODO */)));
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
    /*
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
    */
    
    epGraph->setOffsetY(MG_AXIS_Y1, eOpts->GetY1Zero());
    epGraph->setOffsetY(MG_AXIS_Y2, eOpts->GetY2Zero());
    epGraph->setPrecision(eOpts->GetNumDigits());
    epGraph->autoScaleY(MG_AXIS_Y1);
    epGraph->setSelection(0, mData->CurrentFrame - 1);
}

/*!
 * EVT_GRAPH_CLICK event handler for ID_EPGRAPH
 */

void EnergyPlotDialog::OnEpgraphGraphClick( wxCommandEvent& event )
{
////@begin EVT_GRAPH_CLICK event handler for ID_EPGRAPH in EnergyPlotDialog.
////@end EVT_GRAPH_CLICK event handler for ID_EPGRAPH in EnergyPlotDialog. 
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

void EnergyPlotDialog::OnCloseWindow( wxCloseEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    parent->CloseEnergy_plotWindow();
}


