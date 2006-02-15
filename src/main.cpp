/*
 * (c) 2005 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * main.cpp
 *
 * Created:       10-31-2005  Steven Schulteis
 * Last Modified: 11-14-2005  Steven Schulteis
***************************************/

#include "main.h"

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

    // TODO:  Pass proper arguments to main frame object
    m_Frame = new MpMainFrame(wxT("wxMacMolPlt"));
    // TODO:  Let app continue on frame delete
    // SetExitOnFrameDelete(false);
    SetExitOnFrameDelete(true);

    return true;
}

int MpApp::OnExit() {
    delete m_InstanceChecker;

    return 0;
}

void MpApp::createMainFrame(const wxString &file) {
    // TODO:  Create and store new MpMainFrame
}

void MpApp::destroyMainFrame(MpMainFrame *frame) {
    // TODO:  Close frame and remove it from storage
    // frame->Destroy();
}

// Tell wxWidgets to start the program:
IMPLEMENT_APP(MpApp)

