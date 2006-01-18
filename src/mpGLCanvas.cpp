/***************************************
 * mpGLCanvas.cpp
 *
 * Created:       01-09-2006  Steven Schulteis
 * Last Modified: 01-11-2006  Steven Schulteis
 ***************************************/

#include "mpGLCanvas.h"

MpGLCanvas::MpGLCanvas(wxWindow       *parent,
                       wxWindowID      id,
                       const wxPoint  &position,
                       const wxSize   &size,
                       long            style,
                       const wxString &name)
           :wxGLCanvas(parent, id, position, size,
                       style|wxFULL_REPAINT_ON_RESIZE, name) {
    // molData = NULL;
}

void MpGLCanvas::initGL(void) {
    // Initialize the OpenGL context here.
}

void MpGLCanvas::render(void) {
    // TODO:  Test for previous initialization of the GL context
    if( /* init */ ) {
        initGL();
        // Clear init flag
    }

    // Do your OpenGL stuff for rendering the scene here.
}

void MpGLCanvas::eventSize(wxSizeEvent &event) {
    int width, height;

    wxGLCanvas::OnSize(event);

    GetClientSize(&width, &height);

    if(GetContext()) {
        SetCurrent();
        glViewport(0, 0, (GLint)width, (GLint)height);
    }
}

void MpGLCanvas::eventPaint(wxPaintEvent &event) {
    wxPaintDC paintDC(this);

    if(!GetContext()) {
        return;
    }

    SetCurrent();
    render();
    SwapBuffers();
}

void MpGLCanvas::eventErase(wxEraseEvent &event) {
    // Don't mess with this.  It's supposed to be empty.
    // This avoids flashing on Windows.
}

void MpGLCanvas::eventMouse(wxMouseEvent &event) {
    // Do stuff for rotation, etc. here
}

BEGIN_EVENT_TABLE(MpGLCanvas, wxGLCanvas)
    EVT_SIZE             (MpGLCanvas::eventSize)
    EVT_PAINT            (MpGLCanvas::eventPaint)
    EVT_ERASE_BACKGROUND (MpGLCavnas::eventErase)
    EVT_MOUSE_EVENTS     (MpGLCanvas::eventMouse)
END_EVENT_TABLE()
