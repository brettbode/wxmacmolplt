/***************************************
 * main.cpp
 *
 * Created:       10-31-2005  Steven Schulteis
 * Last Modified: 11-02-2005  Steven Schulteis
***************************************/

#include "main.h"

/*******************
 * OnInit
 *
 * Overloads OnInit defined in the wxApp class.  This is where program
 * execution begins (as far as we're concerned).
 *
 * Args:
 *     
 * Returns:
 *     bool - true for success, false for error/failure.
*******************/
bool MpApp::OnInit() {
    // TODO:  Handle command line arguments.  See wxApp documentation
    // TODO:  Start logging stuff.
    // TODO:  Figure out what to do about Mac.  wxGetOsVersion may be useful

    const wxString appName = wxString::Format(wxT("wxMacMolPlt-%s"), wxGetUserId().c_str());

    m_InstanceChecker = new wxSingleInstanceChecker();

    if(m_InstanceChecker->Create(appName) == false) {
        // TODO:  Do something intelligent here
    }
    if(m_InstanceChecker->IsAnotherRunning()) {
        // TODO:  Handle this appropriately
        return false;
    }

    // TODO:  Create main wxFrame
    SetExitOnFrameDelete(false);

    return true;
}

/*******************
 * OnExit
 *
 * Overloads OnExit defined in the wxApp class.  This function is called just
 * before the program exits.
 *
 * Args:
 *
 * Returns:
 *     int - Ignored.  Perhaps wxWidgets will eventually define a purpose for
 *           it.
*******************/
int MpApp::OnExit() {
    delete m_InstanceChecker;

    return 0;
}

// Tell wxWidgets to start the program:
IMPLEMENT_APP(MpApp)

