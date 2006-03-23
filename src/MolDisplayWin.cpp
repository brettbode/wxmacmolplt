/*
 * (c) 2005 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * MolDisplayWin.cpp
 *
 * Created:       11-02-2005  Steven Schulteis
 * Last Modified: 01-19-2006  Steven Schulteis
 ***************************************/

#include "Globals.h"
#include "MolDisplayWin.h"
#include "Prefs.h"
#include "main.h"

extern WinPrefs * gPreferences;

BEGIN_EVENT_TABLE(MolDisplayWin, wxFrame)
    EVT_MENU (wxID_SAVE,         MolDisplayWin::menuFileSave)
    EVT_MENU (wxID_SAVEAS,       MolDisplayWin::menuFileSave_as)
    EVT_MENU (wxID_CLOSE,        MolDisplayWin::menuFileClose)
    EVT_MENU (wxID_PRINT_SETUP,  MolDisplayWin::menuFilePage_setup)
    EVT_MENU (wxID_PREVIEW,      MolDisplayWin::menuFilePrint_preview)
    EVT_MENU (wxID_PRINT,        MolDisplayWin::menuFilePrint)

    EVT_MENU (wxID_UNDO,         MolDisplayWin::menuEditUndo)
    EVT_MENU (wxID_CUT,          MolDisplayWin::menuEditCut)
    EVT_MENU (wxID_COPY,         MolDisplayWin::menuEditCopy)
    EVT_MENU (wxID_PASTE,        MolDisplayWin::menuEditPaste)
    EVT_MENU (wxID_CLEAR,        MolDisplayWin::menuEditClear)
    EVT_MENU (wxID_SELECTALL,    MolDisplayWin::menuEditSelect_all)

END_EVENT_TABLE()

MolDisplayWin::MolDisplayWin(const wxString &title,
                         const wxPoint  &position,
                         const wxSize   &size,
                         long            style,
                         const wxString &name)
            :wxFrame((wxWindow *)NULL, wxID_ANY, title,
                     position, size, style, name) {

    createMenuBar();
    SetMenuBar(menuBar);

    // TODO:  Make room for other controls (Frame picker, etc.)
    MainData = new MoleculeData;
	Prefs = new WinPrefs;
	*Prefs = *gPreferences;
    glCanvas = new MpGLCanvas(this);

    Show(true);
}

MolDisplayWin::~MolDisplayWin() {
    // TODO:  Destroy any dialogs that are still in existence.

	MessageAlert("MolDisplayWin destructor running");
    if(MainData != NULL) {
        glCanvas->setMolData(NULL);
        delete MainData;
    }
	if (Prefs != NULL) {
		delete Prefs;
	}
}

void MolDisplayWin::createMenuBar(void) {
    menuBar = new wxMenuBar;

    menuFile = new wxMenu;
    menuEdit = new wxMenu;
    menuView = new wxMenu;
    menuMolecule = new wxMenu;
    menuWindow = new wxMenu;
    menuHelp = new wxMenu;

    // TODO:  Create IDs for custom menu items
    menuFile->Append(wxID_NEW, wxT("&New\tCtrl+N"));
    menuFile->Append(wxID_OPEN, wxT("&Open ...\tCtrl+O"));
    menuFile->Append(wxID_SAVE, wxT("&Save\tCtrl+S"));
    menuFile->Append(wxID_SAVEAS, wxT("Save &as ...\tCtrl+Shift+S"));
    menuFile->Append(wxID_CLOSE, wxT("&Close\tCtrl+W"));
    menuFile->AppendSeparator();
    //menuFile->Append(, wxT("Append New Frame"));
    //menuFile->Append(, wxT("Add Frames from File ..."));
    //menuFile->Append(, wxT("Delete Frame"));
    //menuFile->AppendSeparator();
    //menuFile->Append(, wxT("Import"));
    //menuFile->Append(, wxT("Export"));
    //menuFile->AppendSeparator();
    menuFile->Append(wxID_PRINT_SETUP, wxT("Page Set&up ..."));
    menuFile->Append(wxID_PREVIEW, wxT("Print Pre&view\tCtrl+Shift+P"));
    menuFile->Append(wxID_PRINT, wxT("&Print ...\tCtrl+P"));
    // TODO:  Make Mac display Quit menu item in the correct place
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, wxT("&Quit\tCtrl+Q"));
    
    menuEdit->Append(wxID_UNDO, wxT("&Undo\tCtrl+Z"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_CUT, wxT("Cu&t\tCtrl+X"));
    menuEdit->Append(wxID_COPY, wxT("&Copy\tCtrl+C"));
    //menuEdit->Append(,wxT("Copy Coordinates"));
    menuEdit->Append(wxID_PASTE, wxT("&Paste\tCtrl+V"));
    menuEdit->Append(wxID_CLEAR, wxT("&Delete\tDel"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_SELECTALL, wxT("&Select all\tCtrl+A"));
    // TODO:  Potentially make Mac display Preferences menu item elsewhere
    //menuEdit->AppendSeparator();
    //menuEdit->Append(, wxT("Default Pr&eferences ..."));

    // TODO:  Create menu items for remaining menus

    // TODO:  Make Mac handle help menu properly
    // TODO:  Make Mac display About menu item in the correct place
    menuHelp->Append(wxID_ABOUT, wxT("&About ..."));

    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuEdit, wxT("&Edit"));
    menuBar->Append(menuView, wxT("&View"));
    menuBar->Append(menuMolecule, wxT("&Molecule"));
    menuBar->Append(menuWindow, wxT("&Window"));
    menuBar->Append(menuHelp, wxT("&Help"));
}

/* Event handler functions */

/* File menu */
void MolDisplayWin::menuFileOpen(wxCommandEvent &event) {
	//Its possible we could handle this here if the current data is empty?
}

void MolDisplayWin::menuFileSave(wxCommandEvent &event) {
}

void MolDisplayWin::menuFileSave_as(wxCommandEvent &event) {
}

void MolDisplayWin::menuFileClose(wxCommandEvent &event) {
	//First we should check to see if a save is needed which could abort the close
	
	//Once we decide to close the window it may be system dependant whether we leave an empty window up
	Destroy();
	MpApp & app = wxGetApp();
	app.destroyMainFrame(this);
    // This should leave the current window open, but close the file
    // associated with it, leaving an "empty" window.
}

void MolDisplayWin::menuFilePage_setup(wxCommandEvent &event) {
}

void MolDisplayWin::menuFilePrint_preview(wxCommandEvent &event) {
}

void MolDisplayWin::menuFilePrint(wxCommandEvent &event) {
}

void MolDisplayWin::menuFileQuit(wxCommandEvent &event) {
    // This should close the entire window.
}

/* Edit menu */

void MolDisplayWin::menuEditUndo(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditCut(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditCopy(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditPaste(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditClear(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditSelect_all(wxCommandEvent &event) {
}


