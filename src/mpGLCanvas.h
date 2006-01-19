/***************************************
 * mpGLCanvas.h
 *
 * Description goes here.
 *
 * Created:       01-09-2006  Steven Schulteis
 * Last Modified: 01-19-2006  Steven Schulteis
 ***************************************/

#ifndef MPGLCANVAS_H
#define MPGLCANVAS_H 1

#include "wx/wx.h"
#include "wx/glcanvas.h"

struct GLWorldData {
};

class MpGLCanvas : public wxGLCanvas {
    private:
        // TODO:  Define data pointer
        // Type   *molData;
        GLWorldData  worldData;

        void initGL(void);
        void render(void);

        DECLARE_EVENT_TABLE()

    public:
        MpGLCanvas(wxWindow       *parent,
                   wxWindowID      id       = wxID_ANY,
                   const wxPoint  &position = wxDefaultPosition,
                   const wxSize   &size     = wxDefaultSize,
                   long            style    = wxSUNKEN_BORDER,
                   const wxString &name     = wxT("DemoGLCanvas"));

        // TODO:  Define function to reset data pointer
        // Call setMolData(NULL) before deleting the MpMolData object
        // void setMolData(const Type *newMolData);
        
        // TODO:  Make this function:
        //wxImage *getImage(const int width, const int height);
        
        // Event Handlers
        void eventSize(wxSizeEvent &event);
        void eventPaint(wxPaintEvent &event);
        void eventErase(wxEraseEvent &event);
        void eventMouse(wxMouseEvent &event);
};

#endif

