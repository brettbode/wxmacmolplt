/*
 * (c) 2005 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * mpGLCanvas.cpp
 *
 * Created:       01-09-2006  Steven Schulteis
 * Last Modified: 01-19-2006  Steven Schulteis
 ***************************************/

#include "Globals.h"
#include "mpGLCanvas.h"
#include <wx/image.h>
#include "MolDisplayWin.h"

#include <iostream>

#warning This setting might need some verification to ensure it's safe/functional
int defAttribs[] = {WX_GL_DEPTH_SIZE, 16,
                    0};

MpGLCanvas::MpGLCanvas(MolDisplayWin  *parent,
                       wxWindowID      id,
                       const wxPoint  &position,
                       const wxSize   &size,
                       long            style,
                       const wxString &name)
           :wxGLCanvas((wxWindow *)parent, id, position, size,
                       style|wxFULL_REPAINT_ON_RESIZE, name, defAttribs) {
		//There is an additional parameter to wxGLCanvas that is an int
		//array of GL options we might need. Might need to use WX_GL_RGBA and WX_GL_DOUBLEBUFFER?
    molData = NULL;
	Prefs = NULL;
	MolWin = parent;
	initialized = false;
		
	//Hmm is this the right spot to initialize our GL settings?
//	initGL();
}

void MpGLCanvas::initGL(void) {
    if(GetContext()) {
		// Initialize the OpenGL context here.
		glEnable(GL_DEPTH_TEST);
		
		//	glShadeModel(GL_FLAT);
		glShadeModel(GL_SMOOTH);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glPolygonOffset (1.0, 1.0);
		
		GLfloat mat_specular[] = {0.8, 0.8, 0.8, 1.0};
		GLfloat mat_shininess[] = {80.0};
		GLfloat mat_diffuse[] = {0.2,0.2,0.2,0.8};
		GLfloat mat_ambient[] = {0.1,0.1,0.1,0.8};
		glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
		glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
		glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
		glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
					//setup the static lighting properties
		GLfloat ambient[4]  = {0.2,0.2,0.2,1.0};
		GLfloat model_ambient[4]  = {0.1,0.1,0.1,0.1};
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
		glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
		glEnable(GL_LIGHT0);
		initialized = true;
	}
}

void MpGLCanvas::render(void) {
    // TODO:  Test for previous initialization of the GL context
    if( 1/* init */ ) {
        initGL();
        // TODO: Clear init flag
    }

    // Do your OpenGL stuff for rendering the scene here.
}

void MpGLCanvas::setMolData(MoleculeData *newMolData) {
    molData = newMolData;
}
void MpGLCanvas::setPrefs(WinPrefs *newPrefs) {
    Prefs = newPrefs;
}

wxImage MpGLCanvas::getImage(const int width, const int height) {
    // TODO:  respect width/height
    // TODO:  avoid grabbing the menu
    GLint view[4];
    GLint cwidth,cheight;
    unsigned char *pixels;

    glGetIntegerv(GL_VIEWPORT, view);
    cwidth = view[2];
    cheight = view[3];

    // malloc is required by wxImage,
    // which takes responsibility for the memory
    pixels = (unsigned char *) malloc(3 * cwidth * cheight);
    bzero(pixels,3*cwidth*cheight);
    glPixelStorei( GL_PACK_ALIGNMENT, 1 );
    glReadBuffer( GL_BACK );
    glReadPixels( 0, 0, cwidth, cheight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // create a wxImage from the data, and mirror it vertically
    return wxImage(cwidth,cheight,pixels).Mirror(false);
}
//0.0577 corresponds to fov=60 with zNear=0.1
#define myGLperspective	0.050	//0.050 seems to match my 2D mode

void MpGLCanvas::UpdateGLView(void) {
    int width, height;
    if(GetContext()) {
        SetCurrent();
		GetClientSize(&width, &height);
        glViewport(0, 0, (GLint)width, (GLint)height);
		GLdouble aspect = ((float)width)/height;
		//	GLdouble ysize = 60.0;
		//	if (aspect > 1.0) ysize /= aspect;
		glMatrixMode (GL_PROJECTION);	//Setup the model space to screen space mapping
		glLoadIdentity ();
		//		gluPerspective(ysize, aspect, 0.1, 100.0);
		GLdouble top, right;
		if (aspect > 1.0) {
			right = myGLperspective;
			top = right/aspect;
		} else {
			top = myGLperspective;
			right = top * aspect;
		}
		glFrustum(-right, right, -top, top, 0.1, 100.0);
		glMatrixMode (GL_MODELVIEW);	//Prepare for model space by submitting the rotation/translation
		glLoadIdentity ();
		
		RGBColor * BackgroundColor = Prefs->GetBackgroundColorLoc();
		float red, green, blue;
		red = (float) BackgroundColor->red/65536;
		green = (float) BackgroundColor->green/65536;
		blue = (float) BackgroundColor->blue/65536;	//Set the color to the Vector color
		glClearColor(red, green, blue, 1.0f);		// Setup the background "clear" color
		
		float fillBrightness = Prefs->GetQD3DFillBrightness();
		float PointBrightness = Prefs->GetQD3DPointBrightness();
		GLfloat position[4] = {6.0,6.0,12.0,0.0};
		GLfloat diffuse[4]  = {fillBrightness,fillBrightness,fillBrightness,0.0};
		GLfloat specular[4] = {PointBrightness,PointBrightness,PointBrightness,0.0};
		glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
		glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
		glLightfv(GL_LIGHT0,GL_POSITION,position);
		GLfloat ambient[] = {0.0,0.0,0.0,0.0};
		glLightfv(GL_LIGHT1,GL_AMBIENT,ambient);
		glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuse);
		glLightfv(GL_LIGHT1,GL_SPECULAR,specular);
		position[0] = -6.0;
		glLightfv(GL_LIGHT1,GL_POSITION,position);
		glEnable(GL_LIGHT1);
    }
}

void MpGLCanvas::eventSize(wxSizeEvent &event) {
    int width, height;

    wxGLCanvas::OnSize(event);

    GetClientSize(&width, &height);

	UpdateGLView();
}

void MpGLCanvas::eventPaint(wxPaintEvent &event) {
    wxPaintDC paintDC(this);

	draw();
}
void MpGLCanvas::draw(void) {
    if(!GetContext()) {
        return;
    }
    SetCurrent();
	if (!initialized) {
		initGL();
		UpdateGLView();
	}
	//Only do the drawing if there is not an operation in progress
	//otherwise the underlying data may not be complete.
	if (!MolWin->OperInProgress()) MolWin->DrawGL();
	else
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Clear out the buffer
	
    SwapBuffers();
}
void MpGLCanvas::eventErase(wxEraseEvent &event) {
    // Don't mess with this.  It's supposed to be empty.
    // This avoids flashing on Windows.
}

void MpGLCanvas::eventMouse(wxMouseEvent &event) {
    if(!GetContext()) {
        return;
    }
    SetCurrent();
    // Pass mouse event to MolDisplayWin::Rotate for processing
    MolWin->Rotate(event);
}

void MpGLCanvas::KeyHandler(wxKeyEvent & event) {
	//char events are passed up the parent chain so se need to explicitely pass them
	MolWin->KeyHandler(event);
}

BEGIN_EVENT_TABLE(MpGLCanvas, wxGLCanvas)
    EVT_SIZE             (MpGLCanvas::eventSize)
    EVT_PAINT            (MpGLCanvas::eventPaint)
    EVT_ERASE_BACKGROUND (MpGLCanvas::eventErase)
	EVT_MOUSE_EVENTS     (MpGLCanvas::eventMouse)

	EVT_CHAR (MpGLCanvas::KeyHandler)
END_EVENT_TABLE()

