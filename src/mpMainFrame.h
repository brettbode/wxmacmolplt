#ifndef MP_MAIN_FRAME_H
#define MP_MAIN_FRAME_H 1

#include "wx/wx.h"

class MpMainFrame : public wxFrame {
    private:
        wxMenuBar *menuBar;
        wxMenu    *menuFile;
        wxMenu    *menuEdit;
        wxMenu    *menuView;
        wxMenu    *menuWindow;
        wxMenu    *menuHelp;

        DECLARE_EVENT_TABLE()

    public:
        MpMainFrame(const wxString &title,
                    const wxPoint  &position = wxDefaultPosition,
                    const wxSize   &size     = wxDefaultSize,
                    long            style    = wxDEFAULT_FRAME_STYLE,
                    const wxString &name     = wxT("MpMainFrame"));
}

#endif /* #ifndef MP_MAIN_FRAME_H */

