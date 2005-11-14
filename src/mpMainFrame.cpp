#include "mpMainFrame.h"

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
    menuBar->Append(menuWindow, wxT("&Window"));
    menuBar->Append(menuHelp, wxT("&Help"));

    SetMenuBar(menuBar);

    Show(true);
}
