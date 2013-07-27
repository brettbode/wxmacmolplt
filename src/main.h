/*
 * (c) 2005 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * main.h
 *
 * Defines a class that overloads the wxApp class.  This class is responsible
 * for handling the beginning and end of the program's life.
***************************************/

#ifndef MAIN_H
#define MAIN_H 1

#include "wx/wx.h"
#include "wx/snglinst.h"

#include "MolDisplayWin.h"
#include <list>

#ifdef __WXMAC__
/**
 * Subclasses wxFrame to define a simple frame solely to manage a menubar
 * when there is apparently no windows open. Only applicable to the Mac.
 */
class macMenuWinPlaceholder : public wxFrame {
    private:
        wxMenuBar *menuBar;
        wxMenu    *menuFile;
        wxMenu    *menuEdit;
        wxMenu    *menuHelp;

        void createMenuBar(void);

	public:
        /* Constructor that provides some useful default values.
         * @param title The text that will be displayed in the new window's title bar.
         * @param position The initial position of the new window.
         * @param size The initial size of the new window.
         * @param style The style of the new window.  See wxFrame class docs.
         * @param name The name of the new window.
         */
        macMenuWinPlaceholder(const wxString &title,
                    const wxPoint  &position,
                    const wxSize   &size,
                    long            style    = wxSYSTEM_MENU | wxFRAME_NO_TASKBAR,
                    const wxString &name     = wxT("offscreen"));

        /**
         * Destructor, mostly for cleaning up data that we've allocated
         * independently of wxWidgets.
         */
        ~macMenuWinPlaceholder();
};

#endif

typedef class setPreference setPreference;
/**
 * Subclasses wxApp to define an application's data and operations.
 */
class MpApp : public wxApp {
    private:
        wxSingleInstanceChecker *m_InstanceChecker;

		std::list<MolDisplayWin *> MolWinList;
		setPreference *	gPrefDlg;
#ifdef __WXMAC__
		macMenuWinPlaceholder	* menuHolder;
#endif

        DECLARE_EVENT_TABLE()
			
    public:
        /**
         * Overloads OnInit defined in the wxApp class.  This is where program
         * execution begins (as far as we're concerned).
         *
         * @return true for success/continue, false for failure/exit.
         */
        bool OnInit();

        /**
         * Overloads OnExit defined in the wxApp class.  This function is
         * called just before the program exits.  From the wxWidgets
         * documentation:  OnExit is called after destroying all application
         * windows and controls, but before wxWidgets cleanup. Note that it is
         * not called at all if OnInit failed.
         *
         * @return Ignored.  Perhaps wxWidgets will eventually define a purpose
         *         for it.
         */
        int  OnExit();

        /**
         * Creates a new document window and stores it.
         *
         * @param file The path to the file to open in the new window, if any.
         */
        void createMainFrame(const wxString &file = wxT(""));

        /**
         * Removes a document window from storage and destroys it. 
         *
         * @param frame A pointer to the frame to be destroyed.
         */
        void destroyMainFrame(MolDisplayWin *frame);
		
		/**
		 * Returns the open window count
		 */
		int WindowCount(void) const {return MolWinList.size();};


        void menuFileQuit(wxCommandEvent &event);
		void menuHelpAbout(wxCommandEvent &event);
		/**
		 * Display an html browswer pointed at the provided documentation
		 */
		void menuHelp(wxCommandEvent &event);
        void menuFileNew(wxCommandEvent &event);
        void menuFileOpen(wxCommandEvent &event);
		void menuPreferences(wxCommandEvent &event);
		void ApplyPrefsToAll(WinPrefs * prefs);
		void AdjustAllMenus(void);
		void CloseGlobalPrefs(void);
#ifdef __WXMAC__
		/**
		 * Event sent on Macs when double click a document
		 *
		 * @param the filename to open
		 */
		void MacOpenFile(const wxString & fileName);
#endif
};

#ifndef __WXMSW__
class MpAppNoGUI : public wxAppConsole {
	public:
		bool OnInit();
		int OnRun();
};
#endif

DECLARE_APP(MpApp)

#endif /* #ifndef MAIN_H */

