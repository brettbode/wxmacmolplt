/*
 * (c) 2005 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * mpGLCanvas.h
 *
 * Defines a wxWidgets component that displays a 3-dimensional representation
 * of a molecule.
 ***************************************/

#ifndef MPGLCANVAS_H
#define MPGLCANVAS_H 1

#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "MoleculeData.h"
#include "Frame.h"

typedef class MolDisplayWin MolDisplayWin;
/**
 * Defines a widget for displaying an OpenGL canvas.  At this time it is a
 * child of the wxGLCanvas widget class.
 */
class MpGLCanvas : public wxGLCanvas {
    private:
        MolDisplayWin * MolWin;
        WinPrefs * Prefs;
        bool        initialized;

	MoleculeData *mMainData;

	//GLuint texture[1];  //store texture ID, only need one for now
	int mSelectState;

        void initGL(void);

        DECLARE_EVENT_TABLE()

    public:
        /**
         * Constructor that provides some useful default values.
         * @param parent A pointer to the parent widget.
         * @param id The window identifier for the new widget.
         * @param position The initial position of the new widget.
         * @param size The initial size of the new widget.
         * @param style The style of the new widget.  See wxWindow class docs.
         * @param name The name of the new widget.
         */
        MpGLCanvas(MolDisplayWin  *parent,
                   wxWindowID      id       = wxID_ANY,
                   const wxPoint  &position = wxDefaultPosition,
                   const wxSize   &size     = wxDefaultSize,
                   long            style    = wxNO_BORDER,
                   const wxString &name     = wxT("DemoGLCanvas"));

        /**
         * Sets the internal pointer to the window's preferences.  If the
         * data structure needs to be deleted externally, this function should
         * first be called with the parameter value set to NULL.  This will
         * prevent the MpGLCanvas object from trying to reference a deleted
         * data structure.
         * @param newPrefs A pointer to the window preferences that the canvas should use.
         */
	~MpGLCanvas();

        void setPrefs(WinPrefs *newPrefs);

        /**
         * Updates GL parameters for changes in the window shape or
         * for changes in the lighting and background color.
         */
        void UpdateGLView(void);
        void draw(void);
        
        /**
         * Creates an image of the canvas for purposes such as printing or
         * saving to a file.
         *
         * At present, the width and height parameters are ignored, and the
         * returned image is the resolution of the canvas.  It can be used as
         * follows:
         *
         * void MyFrame::OnSS(wxCommandEvent& WXUNUSED(event))
         * {
         *   wxImage img = GetCanvas()->getImage();
         *   wxInitAllImageHandlers();
         *   img.SaveFile("ss.png");
         * }
         * 
         * @param width The desired width of the image in pixels.
         * @param height The desired height of the image in pixels.
         * @return A wxImage, the validity of which can be tested with
         * wxImage::Ok().
         */
        wxImage getImage(const int width, const int height);
        
        /**
         * @param dc The device context to send the output to.
         * @param scaleFactor The ratio to scale the image up from the screen size.
         * @param progress  A progress dialog to use in case this takes a while.
         * @param center  Center the output in the device context.
         * @param frame  Add a black frame around the image.
         */
        void GenerateHiResImage(wxDC * dc, const float & scaleFactor, Progress * progress,
                                bool Center, bool frame);

        /**
         * Draws the viewport into the given device context at the resolution
         * of the device context.  If the device context height to width ratio
         * is different than that of the viewport, the image will be clipped in
         * the same way that it would be if the viewport were resized to the
         * dimensions of the device context.
         *
         * @param dc An initialized device context to draw the viewport into.
         */
        void GenerateHiResImageForExport(wxDC *dc);

        // Event Handlers

        /**
            * Handles resize events received by the widget.
         * @param event The event to handle.
         */
        void eventSize(wxSizeEvent &event);
        
        /**
         * Handles paint events received by the widget.  This is where OpenGL
         * code is run from.
         * @param event The event to handle.
         */
        void eventPaint(wxPaintEvent &event);

        /**
         * Handles erase events received by the widget.  This is a no-op that
         * is in place to prevent flickering in Microsoft Windows.
         * \param event The event to handle.
         */
        void eventErase(wxEraseEvent &event);

        /**
         * Handles mouse events received by the widget.  This is used to allow
         * rotation of the view and perhaps eventually selection and
         * modification of objects within the view.
         * \param event The event to handle.
         */
        void eventMouse(wxMouseEvent &event);
        
        /**
         * Handles Char events received by the widget. 
         * \param event The event to handle.
         */
        void KeyHandler(wxKeyEvent & event);

	void SelectObj(int x, int y);
};

#endif

