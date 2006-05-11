/*
 * (c) 2005 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * main.cpp
***************************************/

#include "Globals.h"
#include "main.h"
#include "Prefs.h"
#include <wx/cmdline.h>
#include <wx/filename.h>

//The global preferences settings
    WinPrefs *  gPreferences=NULL, * gPrefDefaults=NULL;

static const wxCmdLineEntryDesc g_cmdLineDesc[] = 
{ 
{ wxCMD_LINE_SWITCH, wxT("h"), wxT("help"),    wxT("displays help on "
												   "the command line parameters") }, 
{ wxCMD_LINE_SWITCH, wxT("v"), wxT("version"), wxT("print version") }, 
//{ wxCMD_LINE_OPTION, wxT("d"), wxT("debug"), wxT("specify a debug 
//												 level") }, 
{ wxCMD_LINE_PARAM,  NULL, NULL, wxT("input file"), 
	wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL }, 
{ wxCMD_LINE_NONE } 
};

bool MpApp::OnInit() {
	gPreferences = new WinPrefs;
	gPrefDefaults = new WinPrefs;

	// Now read in the users preferences file from the system:preferences folder if present
	gPrefDefaults->ReadDefaultPrefs();
	*gPreferences = *gPrefDefaults;
		//attempt to read new xml pref file
	gPreferences->ReadUserPrefs();

    const wxString appName = wxString::Format(wxT("wxMacMolPlt-%s"), wxGetUserId().c_str());

    m_InstanceChecker = new wxSingleInstanceChecker();

    if(m_InstanceChecker->Create(appName) == true) {
        if(m_InstanceChecker->IsAnotherRunning()) {
            // TODO:  Open a new window in the existing instance instead
            wxMessageDialog *msg = new wxMessageDialog(NULL, wxT("wxMacMolPlt is already running.  Please close it before attempting to start it again."), wxT("Error"), wxOK | wxSTAY_ON_TOP | wxICON_EXCLAMATION);
            msg->ShowModal();
            delete msg;
            return false;
        }
    }
    else {
        wxMessageDialog *msg = new wxMessageDialog(NULL, wxT("wxMacMolPlt was unable to check for other running instances.  Click OK to continue anyway, or click Cancel to exit."), wxT("Warning"), wxOK | wxCANCEL | wxSTAY_ON_TOP | wxICON_EXCLAMATION);
        int result = msg->ShowModal();
        delete msg;
        if(result == wxID_CANCEL) {
            return false;
        }
    }
    
    // Parse command line 
	wxString cmdFilename; 
	wxCmdLineParser cmdParser(g_cmdLineDesc, argc, argv); 
	int res; 
	{ 
		wxLogNull log; 
		// Pass false to suppress auto Usage() message 
		res = cmdParser.Parse(false); 
	} 
	// Check if the user asked for command-line help 
	if (res == -1 || res > 0 || cmdParser.Found(wxT("h"))) 
	{ 
		cmdParser.Usage(); 
		return false; 
	} 
	// Check if the user asked for the version 
	if (cmdParser.Found(wxT("v"))) 
	{ 
#ifndef __WXMSW__ 
		wxLog::SetActiveTarget(new wxLogStderr); 
#endif 
		wxString msg; 
		wxString date(wxString::FromAscii(__DATE__)); 
		msg.Printf(wxT("wxMacMolPlt, (c) Iowa State University, 2006 " 
					   "Version %.2f, %s"), 7.0, (const wxChar*) date);
		wxLogMessage(msg); 
		return false; 
	} 
	// Check for debug level 
//	long debugLevel = 0; 
//	if (cmdParser.Found(wxT(ÒdÓ), & debugLevel)) 
//	{        
//	} 
	// Check for a project filename 
	if (cmdParser.GetParamCount() > 0) { 
		cmdFilename = cmdParser.GetParam(0); 
		// Under Windows when invoking via a document 
		// in Explorer, we are passed the short form. 
		// So normalize and make the long form. 
		wxFileName fName(cmdFilename); 
		fName.Normalize(wxPATH_NORM_LONG|wxPATH_NORM_DOTS| 
						wxPATH_NORM_TILDE|wxPATH_NORM_ABSOLUTE); 
		cmdFilename = fName.GetFullPath(); 
		if (cmdFilename.length() > 0) {
			MolDisplayWin * temp = new MolDisplayWin(cmdFilename);
			MolWinList.push_back(temp);
			long r = temp->OpenFile(cmdFilename);
			if (r>0) temp->Show(true);
		}
	} 

    // Initialize image handlers so we can save the PNG format and such.
    wxInitAllImageHandlers();
	
    // TODO:  Pass proper arguments to main frame object
//	MolDisplayWin * temp = new MolDisplayWin(wxT("Untitled"));
//	MolWinList.push_back(temp);
//    m_Frame = new MolDisplayWin(wxT("wxMacMolPlt"));
    // I think the following is appropriate.
	// On Macs the app continues to run without any windows open, but Linux and Windows exit
#ifdef __WXMAC__
	wxPoint p(-100,-100);
	wxSize s(10,10);
	menuHolder = new macMenuWinPlaceholder(wxT("offscreen"), p, s);
	SetExitOnFrameDelete(false);
#else
	if (MolWinList.size()<=0){
		MolDisplayWin * temp = new MolDisplayWin(wxT("Untitled"));
		MolWinList.push_back(temp);
	}
	SetExitOnFrameDelete(true);
#endif

    return true;
}

int MpApp::OnExit() {
    delete m_InstanceChecker;

    return 0;
}

void MpApp::createMainFrame(const wxString &filename) {
    MolDisplayWin *temp = NULL;

    if(filename.IsEmpty()) {
	    temp = new MolDisplayWin(wxT("Untitled"));
	    MolWinList.push_back(temp);
    }
    else {
        // TODO:  Error checking
		temp = new MolDisplayWin(filename);
		MolWinList.push_back(temp);
		long r = temp->OpenFile(filename);
		if (r>0) temp->Show(true);
    }
}

void MpApp::destroyMainFrame(MolDisplayWin *frame) {
	MolWinList.remove(frame);
#ifdef __WXMAC__
	if (MolWinList.size() <= 0) {
		menuHolder->Show(true);
//		menuHolder->Show(false);
	}
#endif
}

void MpApp::menuFileQuit(wxCommandEvent &event) {
	//Need to loop over open files, prompting to save if needed
	//then exit
	std::list<MolDisplayWin *>::iterator win = MolWinList.begin();
	while (win != MolWinList.end()) {
			//Need to be careful since the close will remove the window from the list
			//so increment the iterator before closing.
		MolDisplayWin * temp = (*win);
		win++;
		if (!(temp->Close(false))) {
			// User aborted the close
			return;
		}
	}
	
	//This looks like it has the desired effect, but not sure if it is the "correct" way to exit
	ExitMainLoop();
}

void MpApp::menuHelpAbout(wxCommandEvent & WXUNUSED(event)) {
	//Display a simple modal about box
	wxMessageBox(wxT("Welcome to MacMolPlt"), wxT(""));
}
void MpApp::menuPreferences(wxCommandEvent & WXUNUSED(event)) {
	//Default application preferences
#warning Open default preferences dialog here once implemented
}
void MpApp::menuFileNew(wxCommandEvent &event) {
    createMainFrame();
}

void MpApp::menuFileOpen(wxCommandEvent &event) {
	//First need to use an open file dialog
	wxString filename = wxFileSelector(wxT("Choose a file to open"));
	//If the user chooses a file, create a window and have it process it.
	if (filename.length() > 0) {
        createMainFrame(filename);
        // TODO:  Remove the commented code below once Brett is OK with it.
        /*
		MolDisplayWin * temp = new MolDisplayWin(filename);
		MolWinList.push_back(temp);
		long r = temp->OpenFile(filename);
		if (r>0) temp->Show(true);
        */
	}
}
#ifdef __WXMAC__
void MpApp::MacOpenFile(const wxString & filename) {
	if (filename.length() > 0) {
        createMainFrame(filename);
        // TODO:  Remove the commented code below once Brett is OK with it.
        /*
		MolDisplayWin * temp = new MolDisplayWin(filename);
		MolWinList.push_back(temp);
		long r = temp->OpenFile(filename);
		if (r>0) temp->Show(true);
        */
	}
}
#endif

void MessageAlert(const char * message) {
//wxLogMessage throws up a simple dialog alert and gives the user the option
//of viewing and saving the current complete log.
	//We need to convert to a wxString first to allow for unicode environments
	wxString str(message, wxConvUTF8);
	wxLogMessage(str);
}

BEGIN_EVENT_TABLE(MpApp, wxApp)
EVT_MENU (wxID_NEW,          MpApp::menuFileNew)
EVT_MENU (wxID_OPEN,         MpApp::menuFileOpen)
EVT_MENU (wxID_EXIT,         MpApp::menuFileQuit)

EVT_MENU (wxID_ABOUT,    MpApp::menuHelpAbout)
END_EVENT_TABLE()

// Tell wxWidgets to start the program:
IMPLEMENT_APP(MpApp)

#ifdef __WXMAC__
macMenuWinPlaceholder::macMenuWinPlaceholder(const wxString &title,
                         const wxPoint  &position,
                         const wxSize   &size,
                         long            style,
                         const wxString &name)
            :wxFrame((wxWindow *)NULL, wxID_ANY, title,
                     position, size, style, name) {

    createMenuBar();
    SetMenuBar(menuBar);

    Show(true);	//You seem to have to show it to get the menu bar to load
//	Show(false);//then hide it to get it out of the window lists
}

macMenuWinPlaceholder::~macMenuWinPlaceholder() {
}

void macMenuWinPlaceholder::createMenuBar(void) {
    menuBar = new wxMenuBar;

    menuFile = new wxMenu;
    menuEdit = new wxMenu;
    menuHelp = new wxMenu;

    menuFile->Append(wxID_NEW, wxT("&New\tCtrl+N"));
    menuFile->Append(wxID_OPEN, wxT("&Open ...\tCtrl+O"));
  //  menuFile->Append(wxID_SAVE, wxT("&Save\tCtrl+S"));
  //  menuFile->Append(wxID_SAVEAS, wxT("Save &as ...\tCtrl+Shift+S"));
  //  menuFile->Append(wxID_CLOSE, wxT("&Close\tCtrl+W"));
  //  menuFile->AppendSeparator();
    //menuFile->Append(, wxT("Append New Frame"));
    //menuFile->Append(, wxT("Add Frames from File ..."));
    //menuFile->Append(, wxT("Delete Frame"));
    //menuFile->AppendSeparator();
    //menuFile->Append(, wxT("Import"));
    //menuFile->Append(, wxT("Export"));
    menuFile->AppendSeparator();
    menuFile->Append(wxID_PRINT_SETUP, wxT("Page Set&up ..."));
 //   menuFile->Append(wxID_PREVIEW, wxT("Print Pre&view\tCtrl+Shift+P"));
 //   menuFile->Append(wxID_PRINT, wxT("&Print ...\tCtrl+P"));
    // TODO:  Make Mac display Quit menu item in the correct place
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, wxT("&Quit\tCtrl+Q"));
    
    menuEdit->Append(wxID_UNDO, wxT("&Undo\tCtrl+Z"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_CUT, wxT("Cu&t\tCtrl+X"));
    menuEdit->Append(wxID_COPY, wxT("&Copy\tCtrl+C"));
    menuEdit->Append(wxID_PASTE, wxT("&Paste\tCtrl+V"));
    menuEdit->Append(wxID_CLEAR, wxT("&Delete\tDel"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_SELECTALL, wxT("&Select all\tCtrl+A"));

    menuHelp->Append(wxID_ABOUT, wxT("&About ..."));

    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuEdit, wxT("&Edit"));
    menuBar->Append(menuHelp, wxT("&Help"));
}
#endif

