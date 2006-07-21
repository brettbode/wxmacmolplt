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

    EVT_MENU( ID_FREQCOPY, FrequenciesDialog::OnFreqcopyClick )

    EVT_MENU( ID_PREVMODE, FrequenciesDialog::OnPrevmodeClick )

    EVT_MENU( ID_NEXTMODE, FrequenciesDialog::OnNextmodeClick )

    EVT_MENU( ID_MENUIRINTEN, FrequenciesDialog::OnMenuirintenClick )

    EVT_MENU( ID_MENURAMANINTEN, FrequenciesDialog::OnMenuramanintenClick )

    EVT_MENU( ID_MENUZOOMIN, FrequenciesDialog::OnMenuzoominClick )

    EVT_MENU( ID_MENUZOOMOUT, FrequenciesDialog::OnMenuzoomoutClick )

    EVT_LISTBOX( ID_FREQLISTBOX, FrequenciesDialog::OnFreqlistboxSelected )

    EVT_GRAPH_CLICK(ID_CUSTOM, FrequenciesDialog::OnCustomGraphClick)

////@end FrequenciesDialog event table entries
	EVT_GRAPH_CLICK(ID_CUSTOM, FrequenciesDialog::OnCustomGraphClick)
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
    mFreqListBox = NULL;
    fGraph = NULL;
////@end FrequenciesDialog member initialisation

////@begin FrequenciesDialog creation
    wxFrame::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
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
    itemMenu3->Append(ID_FREQCOPY, _("&Copy\tCtrl+C"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu3, _("&Edit"));
    wxMenu* itemMenu5 = new wxMenu;
    itemMenu5->Append(ID_PREVMODE, _("&Previous Normal Mode\tCtrl+["), _T(""), wxITEM_NORMAL);
    itemMenu5->Append(ID_NEXTMODE, _("Ne&xt Normal Mode\tCtrl+]"), _T(""), wxITEM_NORMAL);
    itemMenu5->AppendSeparator();
    itemMenu5->Append(ID_MENUIRINTEN, _("Show &IR Intensity"), _T(""), wxITEM_RADIO);
    itemMenu5->Append(ID_MENURAMANINTEN, _("Show &Raman Intensity"), _T(""), wxITEM_RADIO);
    itemMenu5->AppendSeparator();
    itemMenu5->Append(ID_MENUZOOMIN, _("Zoom In\tCtrl++"), _T(""), wxITEM_NORMAL);
    itemMenu5->Append(ID_MENUZOOMOUT, _("Zoom Out\tCtrl+-"), _T(""), wxITEM_NORMAL);
    menuBar->Append(itemMenu5, _("&View"));
    itemFrame1->SetMenuBar(menuBar);

    wxBoxSizer* itemBoxSizer14 = new wxBoxSizer(wxVERTICAL);
    itemFrame1->SetSizer(itemBoxSizer14);

    wxPanel* itemPanel15 = new wxPanel( itemFrame1, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
    itemBoxSizer14->Add(itemPanel15, 1, wxGROW, 0);

    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxVERTICAL);
    itemPanel15->SetSizer(itemBoxSizer16);

    wxBoxSizer* itemBoxSizer17 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer16->Add(itemBoxSizer17, 1, wxGROW|wxALL, 5);

    wxString* mFreqListBoxStrings = NULL;
    mFreqListBox = new wxListBox( itemPanel15, ID_FREQLISTBOX, wxDefaultPosition, wxDefaultSize, 0, mFreqListBoxStrings, wxLB_SINGLE );
    if (ShowToolTips())
        mFreqListBox->SetToolTip(_("Click the frequency to display the desired mode."));
    itemBoxSizer17->Add(mFreqListBox, 0, wxGROW|wxALL, 5);

    fGraph = new wxMolGraph( itemPanel15, ID_CUSTOM, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER );
    itemBoxSizer17->Add(fGraph, 1, wxGROW|wxALL, 5);

////@end FrequenciesDialog content construction
    itemMenu3->Append(wxID_PREFERENCES, wxT("Global Pr&eferences"));
    wxMenu * menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, wxT("&About"));
    menuBar->Append(menuHelp, wxT("&Help"));
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
	mFreqListBox->SetSelection(parent->GetData()->cFrame->Vibs->GetCurrentMode());
    Refresh();
}

void FrequenciesDialog::RegenData(void) {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	WinPrefs * Prefs = parent->GetPrefs();
	FrequencyWindowOptions * fprefs = Prefs->GetFrequencyWindowOptions();
	
	bool UseRaman = fprefs->ShowRamanIntensity();
    
    MoleculeData  *mData = parent->GetData();
    VibRec *Vibs = parent->GetData()->cFrame->Vibs;
    
	std::vector< double > xSetData;
	std::vector< pair< int, double > > freqData;
    float temp;
    int i = 0;
    
	mFreqListBox->Clear();
    fGraph->reset();
    
    if(Vibs == NULL) return;
	wxString freq;
    
    for(i = 0; i < Vibs->GetNumModes(); i++) {
        xSetData.push_back((double)(Vibs->GetFrequency(i)));
		if (UseRaman)
			freqData.push_back(make_pair(i, Vibs->GetRamanIntensity(i)));
		else
			freqData.push_back(make_pair(i, Vibs->GetIntensity(i)));
		
		freq.Printf(wxT("%f"), Vibs->GetFrequency(i));
		mFreqListBox->InsertItems(1, &freq, i);
    }
    fGraph->addXSet(xSetData, true);
    
    fGraph->addYSet(freqData, 0, MG_AXIS_Y1, MG_STYLE_BAR, *wxBLACK);

    //fGraph->setPrecision(eOpts->GetNumDigits());
    fGraph->autoScaleY(MG_AXIS_Y1);
    fGraph->setYAxisMin(MG_AXIS_Y1, 0.0);
	fGraph->setYAxisMax(MG_AXIS_Y1, fGraph->getYAxisMax(MG_AXIS_Y1)*fprefs->GetYScaleFactor());
    fGraph->setXAxisMin(0.0);
	if (UseRaman) 
		fGraph->setAxisLabel(MG_AXIS_Y1, _("Raman Intensity"));
	else
		fGraph->setAxisLabel(MG_AXIS_Y1, _("IR Intensity"));
	fGraph->setAxisLabel(MG_AXIS_X, _("Frequency"));
    fGraph->setSelection(0, Vibs->GetCurrentMode());
	mFreqListBox->SetSelection(Vibs->GetCurrentMode());
}

void FrequenciesDialog::OnCloseWindow( wxCloseEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    parent->CloseFrequenciesWindow();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for wxID_COPY
 */

void FrequenciesDialog::OnFreqcopyClick( wxCommandEvent& event )
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




/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_PREVMODE
 */

void FrequenciesDialog::OnPrevmodeClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    if(parent->GetData()->cFrame->Vibs->CurrentMode > 0) {
        parent->ChangeModes(parent->GetData()->cFrame->Vibs->CurrentMode - 1);
    }
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_NEXTMODE
 */

void FrequenciesDialog::OnNextmodeClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    if(parent->GetData()->cFrame->Vibs->CurrentMode < parent->GetData()->cFrame->Vibs->NumModes - 1) {
        parent->ChangeModes(parent->GetData()->cFrame->Vibs->CurrentMode + 1);
    }
}


/*!
 * wxEVT_COMMAND_LISTBOX_SELECTED event handler for ID_FREQLISTBOX
 */

void FrequenciesDialog::OnFreqlistboxSelected( wxCommandEvent& event )
{
	int newSelection = mFreqListBox->GetSelection();
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    
    MoleculeData  *mData = parent->GetData();
    VibRec *Vibs = parent->GetData()->cFrame->Vibs;
    
    if(Vibs != NULL) {
		if (newSelection != Vibs->GetCurrentMode()) {
			parent->ChangeModes(newSelection);
		}
	}
    event.Skip();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUIRINTEN
 */

void FrequenciesDialog::OnMenuirintenClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	WinPrefs * Prefs = parent->GetPrefs();
	FrequencyWindowOptions * fprefs = Prefs->GetFrequencyWindowOptions();
	
	fprefs->ShowRamanIntensity(false);
	fprefs->ShowIRIntensity(true);

	RegenData();	//regenerate the graph with the new intensity
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENURAMANINTEN
 */

void FrequenciesDialog::OnMenuramanintenClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	WinPrefs * Prefs = parent->GetPrefs();
	FrequencyWindowOptions * fprefs = Prefs->GetFrequencyWindowOptions();
	
	fprefs->ShowRamanIntensity(true);
	fprefs->ShowIRIntensity(false);
	
	RegenData();	//regenerate the graph with the new intensity
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM1
 */
void FrequenciesDialog::OnMenuzoominClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	WinPrefs * Prefs = parent->GetPrefs();
	FrequencyWindowOptions * fprefs = Prefs->GetFrequencyWindowOptions();
	
	fprefs->SetYScaleFactor(0.9*fprefs->GetYScaleFactor());

	fGraph->setYAxisMax(MG_AXIS_Y1, fGraph->getYAxisMax(MG_AXIS_Y1)*0.9);

	Refresh();
}

/*!
 * wxEVT_COMMAND_MENU_SELECTED event handler for ID_MENUITEM2
 */

void FrequenciesDialog::OnMenuzoomoutClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	WinPrefs * Prefs = parent->GetPrefs();
	FrequencyWindowOptions * fprefs = Prefs->GetFrequencyWindowOptions();
	
	fprefs->SetYScaleFactor(1.1*fprefs->GetYScaleFactor());
	
	fGraph->setYAxisMax(MG_AXIS_Y1, fGraph->getYAxisMax(MG_AXIS_Y1)*1.1);
	
	Refresh();
}
