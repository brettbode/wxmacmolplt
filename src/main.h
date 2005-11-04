/***************************************
 * main.h
 *
 * Defines a class that overloads the wxApp class.  This class is responsible
 * for handling the beginning and end of the program's life.
 *
 * Created:       11-02-2005  Steven Schulteis
 * Last Modified: 11-02-2005  Steven Schulteis
***************************************/

#ifndef MAIN_H
#define MAIN_H 1

#include "wx/wx.h"

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
        // TODO: define main wxFrame pointer

    public:
        bool OnInit();
        int  OnExit();
};

#endif /* #ifndef MAIN_H */

