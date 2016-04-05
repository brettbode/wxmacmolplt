/////////////////////////////////////////////////////////////////////////////
// Name:        setPreference.cpp
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "setPreference.h"
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
#include "PrefsPanes.h"
#include "MolDisplayWin.h"
#include "main.h"

#include "setPreference.h"

using namespace std;

extern WinPrefs *	gPreferences;
extern WinPrefs *	gPrefDefaults;

IMPLEMENT_DYNAMIC_CLASS( setPreference, wxDialog )

/*
 * SetPreference event table definition
 */

BEGIN_EVENT_TABLE( setPreference, wxDialog )
	EVT_CLOSE( setPreference::OnCloseWindow )

  EVT_CHOICEBOOK_PAGE_CHANGED(ID_NOTEBOOK, setPreference::OnChoicebook)
  /* EVT_CHOICEBOOK_PAGE_CHANGING(ID_NOTEBOOK, setPreference::OnChoicebook) */
  //related to wxNoteBook

  EVT_BUTTON( myID_SETFONT, setPreference::OnSetFont )
  EVT_BUTTON( wxID_OK, setPreference::OnOK )
	EVT_BUTTON( wxID_CANCEL, setPreference::OnCancel )
  EVT_BUTTON( ID_FACTORY_DEFAULT, setPreference::facDefaults )
  EVT_BUTTON( ID_APPLY, setPreference::OnApply )
  EVT_BUTTON( ID_REVERT, setPreference::OnRevert )
  EVT_BUTTON( ID_USER_DEFAULT, setPreference::userDefaults )
END_EVENT_TABLE()


/*!
 * SetPreference constructors
 */

setPreference::setPreference( )
{

}

setPreference::~setPreference( )
{
  //delete mPrefs;
}

setPreference::setPreference( MolDisplayWin* parent, bool isGlobal, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
	mIsGlobal = isGlobal;

	mParent = parent;

	if (isGlobal)
		mPrefs = gPreferences;
	else
		mPrefs = parent->GetPrefs();

	create(parent, id, caption, pos, size, style);
}

/*!
 * setPreference creator
 */

bool setPreference::create( MolDisplayWin* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    wxDialog::Create( parent, id, caption, pos, size, style );
	
	wxBoxSizer * lSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(lSizer);

    m_choiceBook   = (wxChoicebook *)   NULL;
    m_panel      = (wxPanel *)      NULL;

 //   m_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(50,50),
   //     wxTAB_TRAVERSAL | wxCLIP_CHILDREN | wxNO_BORDER | wxNO_FULL_REPAINT_ON_RESIZE);
    m_panel = new wxPanel(this);
	lSizer->Add(m_panel, wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 0);
    m_sizer = new wxBoxSizer(wxVERTICAL);
    m_panel->SetSizer(m_sizer);

 	RecreateBooks();
	
	m_bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    m_midSizer = new wxBoxSizer(wxHORIZONTAL);
    m_midLeftSizer = new wxBoxSizer(wxHORIZONTAL);
    m_midRightSizer = new wxBoxSizer(wxHORIZONTAL);

    m_midSizer->Add(m_midLeftSizer, wxALL, 5);
    m_midSizer->Add(m_midRightSizer, wxALL, 5);
	
    m_sizer->Add(m_midSizer, wxSizerFlags().Center());
    m_sizer->Add(m_bottomSizer, wxSizerFlags().Center());

    m_buttFont = new wxButton(m_panel, myID_SETFONT, wxT("Set Font"));
    m_buttOK = new wxButton(m_panel, wxID_OK, wxT("OK") );
    m_buttCancel = new wxButton(m_panel, wxID_CANCEL, wxT("Cancel"));

	if (mIsGlobal)
		m_buttApply = new wxButton(m_panel, ID_APPLY, wxT("Apply to All"));
	else
		m_buttApply = new wxButton(m_panel, ID_APPLY, wxT("Apply"));

    m_buttFacDef = new wxButton(m_panel, ID_FACTORY_DEFAULT, wxT("Factory Default"));
    m_buttRevert = new wxButton(m_panel, ID_REVERT, wxT("Revert"));

	if (!mIsGlobal)
		m_buttUserDef = new wxButton(m_panel, ID_USER_DEFAULT, wxT("User Defaults"));

    m_bottomSizer->Add(m_buttFont, wxSizerFlags().Border(wxALL, 7));
    m_bottomSizer->Add(80, 30);
    m_bottomSizer->Add(m_buttApply, wxSizerFlags().Border(wxALL, 7));
    m_bottomSizer->Add(m_buttCancel, wxSizerFlags().Border(wxALL, 7));
    m_bottomSizer->Add(m_buttOK, wxSizerFlags().Border(wxALL, 7));

    m_midLeftSizer->Add(m_buttFacDef, wxSizerFlags().Border(wxALL, 7));

    if (!mIsGlobal)
      m_midLeftSizer->Add(m_buttUserDef, wxSizerFlags().Border(wxALL, 7));

    m_midRightSizer->Add(138, 30);
    m_midRightSizer->Add(m_buttRevert, wxSizerFlags().Border(wxALL, 7));

//    m_sizer->Fit(this);
//    m_sizer->SetSizeHints(this);

 //   m_sizer->Layout();
	m_panel->SetSize(size);
	m_panel->Layout();
    lSizer->Fit(this);
	lSizer->SetSizeHints(this);
    lSizer->Layout();

    Centre(wxBOTH);
	//This is a hack to get the window to default to a reasonable size on 2.9.x on OS X
	//Without this the height of the window defaults to ridiculously small.
	//This seems to fix the problem with the global prefs, but not the window specific prefs.
#if wxCHECK_VERSION(2,9,0)
	// I would rather use the incoming size, but the default of -1,-1 simply isn't getting a properly
	// sized window. On the global prefs the width is about 2/3 of correct and the height is passable.
	// Instead set the fixed size below.
	SetSize(MINIMUM_WINDOW_PREF_SIZE);
#endif
	UpdateWindowTitle();
    return true;
}

/*!
 * Update the Window title in the event the file is saved
 */
void setPreference::UpdateWindowTitle(void) {
	if (mIsGlobal) {
		SetTitle(SYMBOL_GLOBALPREFERENCE_TITLE);
	} else {
		wxString foo = mParent->GetTitle();
		foo += wxString(_(" ")) + wxString(SYMBOL_SETPREFERENCE_TITLE);
		SetTitle(foo);
	}
}

/*!
* wxEVT_CLOSE_WINDOW event handler for ID_DIALOG
 */

void setPreference::OnCloseWindow( wxCloseEvent& event )
{
	if (mIsGlobal) {
		MpApp & app = wxGetApp();
		app.CloseGlobalPrefs();
	} else {
		mParent->ClosePrefsWindow();
	}
}
/*!
* wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void setPreference::OnCancel( wxCommandEvent& event ) {
	Close();
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void setPreference::OnOK( wxCommandEvent& event ) {

	saveCurrPrefs(currPanel);

	if (mIsGlobal) {
		*gPreferences = *mPrefs;
		gPreferences->WriteUserPrefs();
	} else {
		mParent->SetWindowPreferences(mPrefs);
		mParent->ResetAllWindows();
	}

	Close();
}

void setPreference::facDefaults( wxCommandEvent& /* event */ ) {

	switch (currPanel) {
		case 0: mPrefs->ReadAtomDefaults(); break;
		case 1: mPrefs->ReadBondDefaults(); break;
		case 2: mPrefs->ReadDisplayDefaults(); break;
		case 3: mPrefs->ReadEnergyDefaults(); break;
		case 4: mPrefs->ReadFileDefaults(); break;
		case 5: mPrefs->ReadScalingDefaults(); break;
		case 6: mPrefs->ReadStereoDefaults(); break;
		case 7: mPrefs->ReadSurfaceDefaults(); break;
		case 8: mPrefs->ReadQD3DDefaults(); break;
	}

	updatePanels(currPanel);

}

void setPreference::userDefaults( wxCommandEvent& /* event */ ) {

	copyCurrPrefs(currPanel, gPreferences);
	updatePanels(currPanel);

}

void setPreference::OnApply( wxCommandEvent& event ) {
	saveCurrPrefs(currPanel);

	if (!mIsGlobal) {
		mParent->SetWindowPreferences(mPrefs);
		mParent->ResetAllWindows();
	} else {
		MpApp & app = wxGetApp();
		app.ApplyPrefsToAll(mPrefs);
	}

}

void setPreference::OnRevert( wxCommandEvent& /* event */ )
{
  WinPrefs* newPrefs;

  if (!mIsGlobal)
    newPrefs = mParent->GetPrefs();
  else
    newPrefs = gPreferences;

  copyCurrPrefs(currPanel, newPrefs);
  updatePanels(currPanel);
}

void setPreference::saveCurrPrefs(int panelID) {

	switch (panelID) {
		case 0: atomPanel->saveToTempPrefs(); break;
		case 1: bondPanel->saveToTempPrefs(); break;
		case 2: displayPanel->saveToTempPrefs(); break;
		case 3: energyPanel->saveToTempPrefs(); break;
		case 4: filePanel->saveToTempPrefs(); break;
		case 5: scalPanel->saveToTempPrefs(); break;
		case 6: stereoPanel->saveToTempPrefs(); break;
		case 7: surfPanel->saveToTempPrefs(); break;
		case 8: qd3dPanel->saveToTempPrefs(); break;
	}

}

void setPreference::copyCurrPrefs(int panelID, WinPrefs* newPrefs) {

	switch (panelID) {
		case 0: mPrefs->CopyAtomPrefs(newPrefs); break;
		case 1: mPrefs->CopyBondPrefs(newPrefs); break;
		case 2: mPrefs->CopyDisplayPrefs(newPrefs); break;
		case 3: mPrefs->CopyEnergyPrefs(newPrefs); break;
		case 4: mPrefs->CopyFilePrefs(newPrefs); break;
		case 5: mPrefs->CopyScalingPrefs(newPrefs); break;
		case 6: mPrefs->CopyStereoPrefs(newPrefs); break;
		case 7: mPrefs->CopySurfacePrefs(newPrefs); break;
		case 8: mPrefs->CopyQD3DPrefs(newPrefs); break;
	}

}

void setPreference::updatePanels(int panelID) {

	m_choiceBook->DeletePage(panelID);

	switch (panelID) {
		case 0:
			atomPanel = new AtomPrefsPane(mParent, m_choiceBook, mPrefs, mIsGlobal);
			atomPanel->SetupPaneItems(mParent);
			m_choiceBook->InsertPage(panelID,  atomPanel, wxT("Elements"), true );
			break;
		case 1: 
			bondPanel = new BondPrefsPane(mParent, m_choiceBook, mPrefs, mIsGlobal);
			bondPanel->SetupPaneItems(mParent);
			m_choiceBook->InsertPage( panelID, bondPanel, wxT("Bonds/Vectors"), true );
			break;
		case 2: 
			displayPanel = new DisplayPrefsPane(mParent, m_choiceBook, mPrefs, mIsGlobal);
			displayPanel->SetupPaneItems(mParent);
			m_choiceBook->InsertPage( panelID, displayPanel, wxT("Display Options"), true ); 
			break;
		case 3: 
			energyPanel = new EnergyPrefsPane(mParent, m_choiceBook, mPrefs, mIsGlobal);
			energyPanel->SetupPaneItems(mParent);
			m_choiceBook->InsertPage( panelID, energyPanel, wxT("Energy Options"), true ); 
			break;
		case 4: 
			filePanel = new FilePrefsPane(mParent, m_choiceBook, mPrefs, mIsGlobal);
			filePanel->SetupPaneItems(mParent);
			m_choiceBook->InsertPage( panelID, filePanel, wxT("File"), true ); 
			break;
		case 5:  
			scalPanel = new ScalingPrefsPane(mParent, m_choiceBook, mPrefs, mIsGlobal);
			scalPanel->SetupPaneItems(mParent);
			m_choiceBook->InsertPage( panelID, scalPanel, wxT("Scaling/Speed"), true );
			break;
		case 6: 
			stereoPanel = new StereoPrefsPane(mParent, m_choiceBook, mPrefs, mIsGlobal);
			stereoPanel->SetupPaneItems(mParent);
			m_choiceBook->InsertPage( panelID, stereoPanel, wxT("Stereo Options"), true ); 
			break;
		case 7: 
			surfPanel = new SurfacePrefsPane(mParent, m_choiceBook, mPrefs, mIsGlobal);
			surfPanel->SetupPaneItems(mParent);
			m_choiceBook->InsertPage( panelID, surfPanel, wxT("Surface Options"), true ); 
			break;
		case 8: 
			qd3dPanel = new QD3DPrefsPane(mParent, m_choiceBook, mPrefs, mIsGlobal);
			qd3dPanel->SetupPaneItems(mParent);
			m_choiceBook->InsertPage( panelID, qd3dPanel, wxT("3D Options"), true ); 
			break;
	}
}

void setPreference::RecreateBooks() { 
	wxChoicebook *oldBook = m_choiceBook;

	m_choiceBook = new wxChoicebook(m_panel, ID_NOTEBOOK, wxDefaultPosition, wxSize(10,10), wxCHB_DEFAULT);

	m_sizer->Add(m_choiceBook, 5, wxALIGN_CENTER_VERTICAL | wxALL | wxEXPAND, 4);                    

	if (oldBook) {
		int sel = oldBook->GetSelection();
		int count = oldBook->GetPageCount();

		for (int n = 0; n < count; n++) {
			wxString str = oldBook->GetPageText(n);
			wxWindow *page = new wxPanel(m_choiceBook);
			m_choiceBook->AddPage(page, str, false );
		}

		m_sizer->Detach(oldBook);
		delete oldBook;

		if (sel != wxNOT_FOUND) {
			m_choiceBook->SetSelection(sel);
		}
	} else {
      CreateInitialPages(m_choiceBook);
    }

//  m_sizer->Insert(0, m_choiceBook, 5, wxALL | wxEXPAND, 4);                    
//  m_sizer->Show(m_choiceBook);
//  m_sizer->Layout();
}
    

wxBookCtrlBase *setPreference::GetCurrentBook() {

  return m_choiceBook;

}

void setPreference::CreateInitialPages(wxBookCtrlBase *parent) {

    // Create and add some panels to the notebook
    atomPanel = new AtomPrefsPane(mParent, parent, mPrefs, mIsGlobal);
    atomPanel->SetupPaneItems(mParent);
    parent->AddPage( atomPanel, wxT("Elements"), false );

	bondPanel = new BondPrefsPane(mParent, parent, mPrefs, mIsGlobal);
	bondPanel->SetupPaneItems(mParent);
	parent->AddPage( bondPanel, wxT("Bonds/Vectors"), false );

	displayPanel = new DisplayPrefsPane(mParent, parent, mPrefs, mIsGlobal);
	displayPanel->SetupPaneItems(mParent);
	parent->AddPage( displayPanel, wxT("Display Options"), false );

    energyPanel = new EnergyPrefsPane(mParent, parent, mPrefs, mIsGlobal);
    energyPanel->SetupPaneItems(mParent);
    parent->AddPage( energyPanel, wxT("Energy Options"), false );

    filePanel = new FilePrefsPane(mParent, parent, mPrefs, mIsGlobal);
    filePanel->SetupPaneItems(mParent);
    parent->AddPage( filePanel, wxT("File"), false );

    scalPanel = new ScalingPrefsPane(mParent, parent, mPrefs, mIsGlobal);
    scalPanel->SetupPaneItems(mParent);
    parent->AddPage( scalPanel, wxT("Scaling/Speed"), false );

	stereoPanel = new StereoPrefsPane(mParent, parent, mPrefs, mIsGlobal);
	stereoPanel->SetupPaneItems(mParent);
	parent->AddPage( stereoPanel, wxT("Stereo Options"), false );

    surfPanel = new SurfacePrefsPane(mParent, parent, mPrefs, mIsGlobal);
    surfPanel->SetupPaneItems(mParent);
    parent->AddPage( surfPanel, wxT("Surface Options"), false );

    qd3dPanel = new QD3DPrefsPane(mParent, parent, mPrefs, mIsGlobal);
    qd3dPanel->SetupPaneItems(mParent);
    parent->AddPage( qd3dPanel, wxT("3D Options"), false );

    parent->SetSelection(0);
    currPanel = 0;
}

void setPreference::OnSetFont(wxCommandEvent& /* event */) {
	wxFontData data;
	wxFontDialog dialog(this, data);

	if ( dialog.ShowModal() == wxID_OK ) {
		wxFontData retData = dialog.GetFontData();
		wxFont myFont = retData.GetChosenFont();
	}
}

void setPreference::OnChoicebook(wxChoicebookEvent& event) {
	int idx = event.GetOldSelection();
  
	saveCurrPrefs(idx);
	currPanel = event.GetSelection();
}
