#include "preview_canvas.h"

#include "Math3D.h"
#include "Globals.h"
#include "VirtualSphere.h"

#ifdef __WXMAC__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

/* ------------------------------------------------------------------------- */

PreviewCanvas::PreviewCanvas(
	wxWindow *parent, wxWindowID id, int *attributes, const wxPoint& position,
	const wxSize& size, long style, const wxString& name)
	: wxGLCanvas(parent, id, attributes, position, size, style, name) {

	context = NULL;
	InitRotationMatrix(global_rotation);

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::InitGL() {

	context = new wxGLContext(this);
	context->SetCurrent(*this);
	
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	GLfloat light_position[4];
	GLfloat ambient_color[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat diffuse_color[] = {0.7, 0.7, 0.7, 1.0};
	GLfloat specular_color[] = {1.0, 1.0, 1.0, 1.0};
	
	light_position[0] = 0.0;
	light_position[1] = 1.0f;
	light_position[2] = 1.0f;
	light_position[3] = 1.0;
	
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_color);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_color);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

}


/* ------------------------------------------------------------------------- */

void PreviewCanvas::OnPaint(wxPaintEvent& event) {

	Render();

}  

/* ------------------------------------------------------------------------- */
   
void PreviewCanvas::OnSize(wxSizeEvent& event) {

	int width, height;
	
	wxGLCanvas::OnSize(event);
	
	GetClientSize(&width, &height);
	
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, ((float) width) / height, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::Render() {

	SetCurrent(*context);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, 1.0, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	float *rot = (float *) global_rotation;
	printf("rot[<0-15>]:\n"
			"%f %f %f %f\n"
			"%f %f %f %f\n"
			"%f %f %f %f\n"
			"%f %f %f %f\n",
			rot[0], rot[1], rot[2], rot[3],
			rot[4], rot[5], rot[6], rot[7],
			rot[8], rot[9], rot[10], rot[11],
			rot[12], rot[13], rot[14], rot[15]);

	glMultMatrixf((float *) global_rotation);
	GLUquadric *quad;
	glPushMatrix();
	glColor3f(0.0f, 1.0f, 0.0f);
	quad = gluNewQuadric();
	gluSphere(quad, 1.0f, 5, 5);
	glPopMatrix();
	gluDeleteQuadric(quad);
	
	SwapBuffers();

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::OnLeftMouseDown(wxMouseEvent& event) {

	const wxPoint& pt = event.GetPosition();
	int width, height;
	
	GetClientSize(&width, &height);
	
	prev_mouse = curr_mouse;
	curr_mouse = event.GetPosition();

	Render();

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::OnMouseDrag(wxMouseEvent& event) {

	const wxPoint& pt = event.GetPosition();
	int width, height;

	if (!event.Dragging()) {
		return;
	}

	GetClientSize(&width, &height);

	prev_mouse = curr_mouse;
	curr_mouse = event.GetPosition();

	Point prev_pt = {prev_mouse.x, prev_mouse.y};
	Point curr_pt = {curr_mouse.x, curr_mouse.y};
	Point sphere_center = {width / 2, height / 2};
	long sphere_radius = (long) (MAX(sphere_center.h, sphere_center.v) * 0.9f);
	Matrix4D local_rotation;
	Matrix4D tempcopyMatrix;

	VirtualSphereQD3D(prev_pt, curr_pt, sphere_center, sphere_radius,
					  local_rotation, global_rotation);
	MultiplyMatrix(local_rotation, global_rotation, tempcopyMatrix);
	CopyMatrix(tempcopyMatrix, global_rotation);
	OrthogonalizeRotationMatrix(global_rotation);

	Render();

}

/* ------------------------------------------------------------------------- */

BEGIN_EVENT_TABLE(PreviewCanvas, wxGLCanvas)
	EVT_PAINT(PreviewCanvas::OnPaint)
	EVT_SIZE(PreviewCanvas::OnSize)
	EVT_LEFT_DOWN(PreviewCanvas::OnLeftMouseDown)
	EVT_MOTION(PreviewCanvas::OnMouseDrag)
END_EVENT_TABLE()

