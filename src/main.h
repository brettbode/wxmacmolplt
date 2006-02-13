/*
 * (c) 2005 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * main.h
 *
 * Defines a class that overloads the wxApp class.  This class is responsible
 * for handling the beginning and end of the program's life.
 *
 * Created:       11-02-2005  Steven Schulteis
 * Last Modified: 11-14-2005  Steven Schulteis
***************************************/

#ifndef MAIN_H
#define MAIN_H 1

#include "wx/wx.h"

#include "mpMainFrame.h"

/**
 * Subclasses wxApp to define an application's data and operations.
 */
class MpApp : public wxApp {
    private:
        wxSingleInstanceChecker *m_InstanceChecker;

        // TODO:  Maybe use STL Vectors or something...
        MpMainFrame *m_Frame;

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
        void destroyMainFrame(MpMainFrame *frame);
};

#endif /* #ifndef MAIN_H */

