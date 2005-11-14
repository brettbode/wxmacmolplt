/***************************************
 * mpMainFrame.cpp
 *
 * Created:       11-02-2005  Steven Schulteis
 * Last Modified: 11-14-2005  Steven Schulteis
 ***************************************/

#include "mpMainFrame.h"

BEGIN_EVENT_TABLE(MpMainFrame, wxFrame)
END_EVENT_TABLE()

MpMainFrame::MpMainFrame(const wxString &title,
                         const wxPoint  &position,
                         const wxSize   &size,
                         long            style,
                         const wxString &name)
            :wxFrame((wxWindow *)NULL, wxID_ANY, title,
                     position, size, style, name) {

    menuBar = new wxMenuBar;

    menuFile = new wxMenu;
    menuEdit = new wxMenu;
    menuView = new wxMenu;
    menuWindow = new wxMenu;
    menuHelp = new wxMenu;

    // TODO: Create menu items

    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuEdit, wxT("&Edit"));
    menuBar->Append(menuView, wxT("&View"));
    menuBar->Append(menuMolecule, wxT("&Molecule"));
    menuBar->Append(menuWindow, wxT("&Window"));
    menuBar->Append(menuHelp, wxT("&Help"));

    SetMenuBar(menuBar);

    Show(true);
}

MpMainFrame::~MpMainFrame() {
    // TODO:  Destroy any dialogs that are still in existence.
}

/* Event handler functions */

void MpMainFrame::menuFileOpen(wxCommandEvent &event) {
}

void MpMainFrame::menuFileClose(wxCommandEvent &event) {
}

