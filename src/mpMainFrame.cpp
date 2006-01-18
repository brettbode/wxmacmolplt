/***************************************
 * mpMainFrame.cpp
 *
 * Created:       11-02-2005  Steven Schulteis
 * Last Modified: 11-14-2005  Steven Schulteis
 ***************************************/

#include "mpMainFrame.h"

BEGIN_EVENT_TABLE(MpMainFrame, wxFrame)
    EVT_MENU (wxID_NEW,          MpMainFrame::menuFileNew)
    EVT_MENU (wxID_OPEN,         MpMainFrame::menuFileOpen)
    EVT_MENU (wxID_SAVE,         MpMainFrame::menuFileSave)
    EVT_MENU (wxID_SAVEAS,       MpMainFrame::menuFileSave_as)
    EVT_MENU (wxID_CLOSE,        MpMainFrame::menuFileClose)
    EVT_MENU (wxID_PRINT_SETUP,  MpMainFrame::menuFilePage_setup)
    EVT_MENU (wxID_PREVIEW,      MpMainFrame::menuFilePrint_preview)
    EVT_MENU (wxID_PRINT,        MpMainFrame::menuFilePrint)
    EVT_MENU (wxID_EXIT,         MpMainFrame::menuFileQuit)

    EVT_MENU (wxID_UNDO,         MpMainFrame::menuEditUndo)
    EVT_MENU (wxID_CUT,          MpMainFrame::menuEditCut)
    EVT_MENU (wxID_COPY,         MpMainFrame::menuEditCopy)
    EVT_MENU (wxID_PASTE,        MpMainFrame::menuEditPaste)
    EVT_MENU (wxID_CLEAR,        MpMainFrame::menuEditClear)
    EVT_MENU (wxID_SELECTALL,    MpMainFrame::menuEditSelect_all)

    EVT_MENU (wxID_ABOUT,    MpMainFrame::menuHelpAbout)
END_EVENT_TABLE()

MpMainFrame::MpMainFrame(const wxString &title,
                         const wxPoint  &position,
                         const wxSize   &size,
                         long            style,
                         const wxString &name)
            :wxFrame((wxWindow *)NULL, wxID_ANY, title,
                     position, size, style, name) {

    createMenuBar();
    SetMenuBar(menuBar);

    // TODO:  Make room for other controls (Frame picker, etc.)
    glCanvas = new MpGLCanvas(this);

    Show(true);
}

MpMainFrame::~MpMainFrame() {
    // TODO:  Destroy any dialogs that are still in existence.
}

void MpMainFrame::createMenuBar(void) {
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
    menuHelp->Append(wxID_ABOUT, wxT("&About ...\tShift+F1"));

    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuEdit, wxT("&Edit"));
    menuBar->Append(menuView, wxT("&View"));
    menuBar->Append(menuMolecule, wxT("&Molecule"));
    menuBar->Append(menuWindow, wxT("&Window"));
    menuBar->Append(menuHelp, wxT("&Help"));
}

/* Event handler functions */

/* File menu */
void MpMainFrame::menuFileNew(wxCommandEvent &event) {
}

void MpMainFrame::menuFileOpen(wxCommandEvent &event) {
}

void MpMainFrame::menuFileSave(wxCommandEvent &event) {
}

void MpMainFrame::menuFileSave_as(wxCommandEvent &event) {
}

void MpMainFrame::menuFileClose(wxCommandEvent &event) {
}

void MpMainFrame::menuFilePage_setup(wxCommandEvent &event) {
}

void MpMainFrame::menuFilePrint_preview(wxCommandEvent &event) {
}

void MpMainFrame::menuFilePrint(wxCommandEvent &event) {
}

void MpMainFrame::menuFileQuit(wxCommandEvent &event) {
}

/* Edit menu */

void MpMainFrame::menuEditUndo(wxCommandEvent &event) {
}

void MpMainFrame::menuEditCut(wxCommandEvent &event) {
}

void MpMainFrame::menuEditCopy(wxCommandEvent &event) {
}

void MpMainFrame::menuEditPaste(wxCommandEvent &event) {
}

void MpMainFrame::menuEditClear(wxCommandEvent &event) {
}

void MpMainFrame::menuEditSelect_all(wxCommandEvent &event) {
}

/* Help menu */
void MpMainFrame::menuHelpAbout(wxCommandEvent &event) {
}

