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
    const wxString appName = wxString::Format("wxMacMolPlt-%s", wxGetUserId().c_str());
    mpInstanceChecker = new wxSingleInstanceChecker(appName);

    if(mpInstanceChecker->IsAnotherRunning()) {
        // TODO: Handle this appropriately
        return false;
    }

    // TODO: Create main wxFrame

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
 *     int -
*******************/
int MpApp::OnExit() {
    delete mpInstanceChecker;

    return 0;
}

// Tell wxWidgets to start the program:
IMPLEMENT_APP(MpApp)

