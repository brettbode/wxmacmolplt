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

/*******************
 * This header file lacks documentation for the following reason:
 *   It is useless without the source in the associated .cpp file.  It doesn't
 *   define or do anything that would be useful outside of the context of
 *   the program that it is part of.  Look at the .cpp file if you want
 *   to know what's happening here.
*******************/
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
         * @return true for success, false for error/failure.
         */
        bool OnInit();

        /**
         * Overloads OnExit defined in the wxApp class.  This function is
         * called just before the program exits.
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
         * Destroys a document window and removes it from storage.
         *
         * @param frame A pointer to the frame to be destroyed.
         */
        void destroyMainFrame(MpMainFrame *frame);
};

#endif /* #ifndef MAIN_H */

