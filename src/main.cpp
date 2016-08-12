/*
 * (c) 2005-2006 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * main.cpp  
***************************************/

#include "Globals.h"
#include "main.h"
#include "Prefs.h"
#include "setPreference.h"
#include "aboutwxmacmolplt.h"
#include <wx/cmdline.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/splash.h>
#include <wx/config.h>
#include <wx/stdpaths.h>
#include <locale.h>
#include <sstream>
#include <iostream>

#include "glf.h"
#include "build_palette.h"

//The global preferences settings
WinPrefs *  gPreferences=NULL, * gPrefDefaults=NULL;
WindowData	gWindowDefault;

BuilderInterface * BuilderTool;

int glf_initialized = 0;

#if wxCHECK_VERSION(2, 9, 0)
static const wxCmdLineEntryDesc g_cmdLineDesc[] = { 
	{ wxCMD_LINE_SWITCH, "h", "help",
		"displays help on the command line parameters" },
	{ wxCMD_LINE_SWITCH, "v", "version", "print version" },
	{ wxCMD_LINE_OPTION, "b", "batch", "export POV-Ray scene", wxCMD_LINE_VAL_STRING},
	{ wxCMD_LINE_PARAM, NULL, NULL, "input file",
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE },
	{ wxCMD_LINE_NONE }
};
#else
static const wxCmdLineEntryDesc g_cmdLineDesc[] = { 
	{ wxCMD_LINE_SWITCH, wxT("h"), wxT("help"),
		wxT("displays help on the command line parameters") },
	{ wxCMD_LINE_SWITCH, wxT("v"), wxT("version"), wxT("print version") },
	{ wxCMD_LINE_OPTION, wxT("b"), wxT("batch"), wxT("export POV-Ray scene") },
	{ wxCMD_LINE_PARAM, NULL, NULL, wxT("input file"),
		wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE },
	{ wxCMD_LINE_NONE }
};
#endif

#ifndef __WXMSW__
MpApp& wxGetApp() {
	return *wx_static_cast(MpApp*, wxApp::GetInstance());
}
#endif

bool MpApp::OnInit() {
	m_InstanceChecker = NULL;
	gPrefDlg = NULL;
	wxSplashScreen * splash = NULL;

	wxStandardPathsBase& gStdPaths = wxStandardPaths::Get();
#ifdef __LINUX__
	//It has become apparent that wx is not determining the install prefix
	//correctly on linux. So set it here as a workaround
	// Ok I don't know how else to get the wxStandardPaths class?
	//Tested on Debian with both wx 2.8 and 3.0, this did not seem to be needed.
	//Need to test with rpms and then relocate an rpm to see if it succeeds.
//	wxStandardPaths * paths = (wxStandardPaths * ) &gStdPaths;
//	if (strcmp(INSTALL_PREFIX, "NONE"))
//		paths->SetInstallPrefix(wxString(INSTALL_PREFIX,wxConvUTF8));
//	else
//		paths->SetInstallPrefix(wxString("/usr/local",wxConvUTF8));
#endif
#ifdef __FORCE_SINGLE_INSTANCE__
#ifndef __WXMAC__
    const wxString appName = wxString::Format(wxT("wxmacmolplt-%s"), wxGetUserId().c_str());
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
#endif
#endif
	
	//Set the numerical locale to "C" throughout
	setlocale(LC_NUMERIC, "C");
	
	gPreferences = new WinPrefs;
	gPrefDefaults = new WinPrefs;

	// Now read in the users preferences file from the system:preferences folder if present
	gPrefDefaults->ReadDefaultPrefs();
	*gPreferences = *gPrefDefaults;
	//attempt to read new xml pref file
	gPreferences->ReadUserPrefs();

	BuilderTool = new BuilderInterface;

    // Parse command line 
	wxString cmdFilename; 
	wxCmdLineParser cmdParser(g_cmdLineDesc, argc, argv); 
	cmdParser.SetLogo(wxT("MacMolPlt usage includes opening one or more files for display or specifying\n")
					  wxT("a single file for batch conversion to a POV file. The remaining options are\n")
					  wxT("given by themselves."));
	int res;
	{ 
		wxLogNull log; 
		// Pass false to suppress auto Usage() message 
		res = cmdParser.Parse(false); 
	}
	// These are left here just in case. Any calls with flags should be redirected to the noGUI version of
	// apps onInit call.

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
		msg.Printf(wxT("wxMacMolPlt, (c) Iowa State University, 2006-2012 ")
					   wxT("Version %s, "), wxT(wxMacMolPlt_VERSION));
		msg += date;
		wxLogMessage(msg); 
		return false; 
	} 

	// GLF needs to be initialized before any fonts are loaded.  The fonts
	// are loaded in the GL canvas since the display lists need to be loaded
	// in the right contexts.
	glfInit();
	glf_initialized = 1;
	wxString vector_font;
#if wxCHECK_VERSION(2, 8, 0)
	wxString pathname = gStdPaths.GetResourcesDir();
#else
	wxString pathname = gStdPaths.GetDataDir();
#ifdef __WXMAC__
	//wxWidgets has a funny idea of where the resources are stored. It locates them as "SharedSupport"
	//but xcode is putting them in Resources.
	pathname.Remove(pathname.Length() - 13);
	pathname += wxT("Resources");
#endif
#endif
	vector_font = pathname + wxT("/arial1.glf");
	if (glfLoadFont(vector_font.mb_str(wxConvUTF8)) < 0) {
		std::ostringstream buf;
		buf << "Warning: font file not found! This probably means wxmacmolplt is not "
			   "properly installed. Looking for " << vector_font.mb_str(wxConvUTF8);
		MessageAlert(buf.str().c_str());
		glfClose();
		glf_initialized = 0;
	}
	
    // Initialize image handlers so we can save the PNG format and such.
    wxInitAllImageHandlers();
	
	// Check for a project filename 
	if (cmdParser.GetParamCount() > 0) {
		//Interesingly it appears OS X is grabbing the command line file arguements and issuing
		//Open file apple events for them. That duplicates this code so deactivate it for OS X.
#ifndef __WXMAC__
		for (int i = 0; i < cmdParser.GetParamCount(); ++i) {
			cmdFilename = cmdParser.GetParam(i); 
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
				if (r>0) temp->Show();
			}
		}
#endif
	} else {
		// Throw up a simple splash screen
		// In theory the wx Splashscreen can be a fire and forget window that will simply destroy
		// itself after a timeout. However, at least on the Mac you have to make sure that you don't
		// try to throw up any other modal dialogs while it's active. It's easy enough to detect the
		// presence of the window, but I can't seem to find a way to block and allow it to close on
		// it's own. The most straight forward workaround appears to be to explicitly track it in this
		// routine and destroy it before exiting this routine.
		
		// Avoid throwing up the splash screen when opening a file since that can generate other
		// modal dialogs.
		
		wxBitmap sp_bitmap;
		if (sp_bitmap.LoadFile(pathname + wxT("/splash.jpg"), wxBITMAP_TYPE_JPEG)) {
			splash = new wxSplashScreen(sp_bitmap, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT, 6000,
										NULL, -1, wxDefaultPosition, wxDefaultSize,
										wxBORDER_SIMPLE|wxSTAY_ON_TOP);
		}
	}
	
    // I think the following is appropriate.
	// On Macs the app continues to run without any windows open, but Linux and Windows exit
#ifdef __WXMAC__
	wxPoint p(-100,-100);
	wxSize s(10,10);
	menuHolder = new macMenuWinPlaceholder(wxT(""), p, s);
	menuHolder->Show(false);
	if (MolWinList.size()<=0){
		MolDisplayWin * temp = new MolDisplayWin(wxT("Untitled"));
		temp->Show();
		MolWinList.push_back(temp);
	} else menuHolder->Show(true);
	SetExitOnFrameDelete(false);
#else
	if (MolWinList.size()<=0){
		MolDisplayWin * temp = new MolDisplayWin(wxT("Untitled"));
		temp->Show();
		MolWinList.push_back(temp);
	}
	SetExitOnFrameDelete(true);
#endif
	
	if (splash) {
		wxSleep(1);
		delete splash;
	}

    return true;
}

int MpApp::OnExit() {
	//If we are using the instance checker we must delete it here or we will leave a stale lock behind
	if (m_InstanceChecker) {
		delete m_InstanceChecker;
		m_InstanceChecker = NULL;
	}
	if (gPreferences) delete gPreferences;
	if (gPrefDefaults) delete gPrefDefaults;

    delete wxConfigBase::Set((wxConfigBase *) NULL);
    //delete config object if there is one created before  -Song Li
	
	//Free up the memory used by glf
	if (glf_initialized) {
		glfClose();
		glf_initialized = 0;
	}
	
    return 0;
}

void MpApp::createMainFrame(const wxString &filename) {
    MolDisplayWin *temp = NULL;

    if(filename.IsEmpty()) {
	    temp = new MolDisplayWin(wxT("Untitled"));
	    MolWinList.push_back(temp);
		temp->Show();
    } else {
		wxFileName fn(filename);
		temp = new MolDisplayWin(fn.GetFullName());
		MolWinList.push_back(temp);
		long r = temp->OpenFile(filename);
		if (r>0) temp->Show(true);
    }
#ifdef __WXMAC__
	menuHolder->Show(false);
#endif
}

void MpApp::ApplyPrefsToAll(WinPrefs * prefs) {
	std::list<MolDisplayWin *>::iterator win = MolWinList.begin();
	while (win != MolWinList.end()) {
		//MolDisplayWin * temp = (*win);
		(*win)->SetWindowPreferences(prefs);
		win++;
	}
}

void MpApp::AdjustAllMenus(void) {
	std::list<MolDisplayWin *>::iterator win = MolWinList.begin();
	while (win != MolWinList.end()) {
		(*win)->AdjustMenus();
		win++;
	}
}

void MpApp::CloseGlobalPrefs(void) {
	gPrefDlg->Destroy();
	gPrefDlg = NULL;
}

void MpApp::destroyMainFrame(MolDisplayWin *frame) {

	MolWinList.remove(frame);
#ifdef __WXMAC__
	if (MolWinList.size() <= 0) {
		menuHolder->Show(true);
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
	AboutwxMacMolPlt * po = new AboutwxMacMolPlt(NULL);
	po->ShowModal();
    po->Destroy();
}

void MpApp::menuHelp(wxCommandEvent & WXUNUSED(event)) {
	//It seems possible to throw up a basic html viewer within the app, or just call out to the users
	//default browser.
	wxStandardPathsBase& gStdPaths = wxStandardPaths::Get();
#if wxCHECK_VERSION(2, 8, 0)
	wxString pathname = gStdPaths.GetResourcesDir();
#ifdef __WXMAC__
	pathname.Remove(pathname.Length() - 34);
#endif
	pathname += wxT("/MacMolPlt_Manual.html");
#else
	wxString pathname = gStdPaths.GetDataDir();
#ifdef __WXMAC__
	//On the Mac this points inside the app bundle, but the docs are at the wxMacMolPlt.app level
	pathname.Remove(pathname.Length() - 34);
	pathname += wxT("/MacMolPlt_Manual.html");
#endif
#endif
	wxString url = wxT("file://") + pathname;
	wxLaunchDefaultBrowser(url, 0);
}
void MpApp::menuPreferences(wxCommandEvent & WXUNUSED(event)) {
	//Default application preferences
    if (gPrefDlg) { //need to bring it to the front...
        gPrefDlg->Raise();
    } else {
        gPrefDlg = new setPreference(NULL, true);
        gPrefDlg->Show();
    }
}
void MpApp::menuFileNew(wxCommandEvent &event) {
    createMainFrame();
}

void MpApp::menuFileOpen(wxCommandEvent &event) {
	//First need to use an open file dialog
	wxString filename = wxFileSelector(wxT("Choose a file to open"));
	//If the user chooses a file, create a window and have it process it.
	if (!filename.empty()) {
		createMainFrame(filename);
	}
}
#ifdef __WXMAC__
void MpApp::MacOpenFile(const wxString & filename) {

	if (filename.length() > 0) {
        createMainFrame(filename);
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
	EVT_MENU (wxID_NEW, MpApp::menuFileNew)
	EVT_MENU (wxID_OPEN, MpApp::menuFileOpen)
	EVT_MENU (wxID_EXIT, MpApp::menuFileQuit)
	EVT_MENU (wxID_PREFERENCES, MpApp::menuPreferences)
	EVT_MENU (wxID_ABOUT, MpApp::menuHelpAbout)
	EVT_MENU (wxID_HELP, MpApp::menuHelp)
END_EVENT_TABLE()

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
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_PREFERENCES, wxT("Global Pr&eferences"));

    menuHelp->Append(wxID_ABOUT, wxT("&About ..."));

    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuEdit, wxT("&Edit"));
    menuBar->Append(menuHelp, wxT("&Help"));
}
#endif

// Tell wxWidgets to start the program:
#ifdef __WXMSW__
IMPLEMENT_APP(MpApp)
#else
int main(int argc, char **argv) {

	//For OS X and Linux the application can be started up in either regular GUI mode or in a
	//command line mode (with very limited functionality). The choice depends on any command
	//line arguments. If no arguments or file names (only) are given then choose the GUI mode
	//which will open the specified files for display or an empty window. If there are any flags
	//(-h/--help, -v/--version or -b/--batch) choose the headless mode. In both cases the command
	//line arguments are parsed in the Init callback for the the wx App.
#ifdef __WXMAC__
	//It appears that older MacOS X versions are tacking on a -psn_0_#### (process serial number) when
	//launched from the finder. This screws up the following logic so need to catch and fix.
	if (argc > 1) {
		if (!strncmp("-psn",argv[1], 4)) argc = 1;
	}
#endif
	
	//Since flags should come first just test the first character of the first argument
	if (argc >= 2 && (argv[1][0] == '-')) {
		/*MpAppNoGUI *guiless_app = */ new MpAppNoGUI;
	} else {
		/*MpApp *gui_app = */ new MpApp;
	}

	return wxEntry(argc, argv);

}

bool MpAppNoGUI::OnInit() {
	bool returnval=false;

    // Parse command line 
	wxString cmdFilename; 
	wxCmdLineParser cmdParser(g_cmdLineDesc, argc, argv);
	cmdParser.SetLogo(wxT("MacMolPlt usage includes opening one or more files for display or specifying\n")
					  wxT("a single file for batch conversion to a POV file. The remaining options are\n")
					  wxT("given by themselves."));
	int res;

	wxLogNull log; 
	// Pass false to suppress auto Usage() message 
	res = cmdParser.Parse(false); 

	// Check if the user asked for command-line help 
	if (res == -1 || res > 0 || cmdParser.Found(wxT("h"))) 
	{ 
		cmdParser.Usage(); 
		return !(res>0);
	} 

	// Check if the user asked for the version 
	if (cmdParser.Found(wxT("v"))) { 
		wxString msg;
		wxString date(wxString::FromAscii(__DATE__)); 
		msg.Printf(wxT("wxMacMolPlt, (c) Iowa State University, 2008-2012,\n")
					   wxT("Version %s, "), wxT(wxMacMolPlt_VERSION));
		msg += wxT("Built on ");
		msg += date;
		msg += wxT("\n");
		// This appears to be the best way to get the output to the console so it isn't lost when we exit.
		wxMessageOutput* msgOut = wxMessageOutput::Get();
		if ( msgOut )
		{
			msgOut->Printf( wxT("%s"), msg.c_str() );
		}
		return true;
	}
	
	wxString pov_file;
	if (cmdParser.Found(_T("b"), &pov_file)) {
	//	std::cout << "pov_file.ToAscii(): " << pov_file.ToAscii() << std::endl;

#ifdef __LINUX__
		wxStandardPathsBase& gStdPaths = wxStandardPaths::Get();

		//It has become apparent that wx is not determining the install prefix
		//correctly on linux. So set it here as a workaround
		// Ok I don't know how else to get the wxStandardPaths class?
		wxStandardPaths * paths = (wxStandardPaths * ) &gStdPaths;
		if (strcmp(INSTALL_PREFIX, "NONE"))
			paths->SetInstallPrefix(wxString(INSTALL_PREFIX, wxConvUTF8));
		else
			paths->SetInstallPrefix(wxString("/usr/local", wxConvUTF8));
#endif

		gPreferences = new WinPrefs;
		gPrefDefaults = new WinPrefs;

		gPrefDefaults->ReadDefaultPrefs();
		*gPreferences = *gPrefDefaults;
		gPreferences->ReadUserPrefs();

		// std::cout << "(int) rc: " << (int) rc << std::endl; 
		// std::cout << "pov_file: " << pov_file << std::endl; 

		MoleculeData *moldata = new MoleculeData(NULL);
		FILE *f = fopen(cmdParser.GetParam(0).ToAscii(), "rb");
		BufferFile *bfile = new BufferFile(f, false);
		moldata->OpenCMLFile(bfile, gPreferences, NULL, NULL, true);
		delete bfile;
		fclose(f);

		f = fopen(pov_file.ToAscii(), "wb");
		bfile = new BufferFile(f, true);
		moldata->ExportPOV(bfile, gPreferences);
		delete bfile;
		fclose(f);

		delete gPreferences;
		delete gPrefDefaults;
		delete moldata;
		returnval = true;
	}
	return returnval;

}

int MpAppNoGUI::OnRun() {
	return 0;
}
#endif
