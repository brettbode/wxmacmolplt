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
	: wxGLCanvas(parent, id, position, size, style, name, attributes),
	  centroid(0, 0, 0) {

	// context = NULL; 
	InitRotationMatrix(global_rotation);
	struc = NULL;

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::InitGL() {

	// context = new wxGLContext(this); 
	SetCurrent();
	// context->SetCurrent(*this); 
	
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

	// SetCurrent(*context); 
	SetCurrent();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glMultMatrixf((float *) global_rotation);
	glTranslatef(-centroid.x, -centroid.y, -centroid.z);
	glColor3f(0.0f, 1.0f, 0.0f);

	if (struc) {
		int i;
		GLUquadric *quad;
		mpAtom *atom;

		quad = gluNewQuadric();
		for (i = 0; i < struc->natoms; i++) {
			atom = &struc->atoms[i];
			glPushMatrix();
			glTranslatef(atom->Position.x, atom->Position.y, atom->Position.z);
			gluSphere(quad, 0.3f, 10, 10);
			glPopMatrix();
		}

		CPoint3D *pos1, *pos2;
		glBegin(GL_LINES);
		for (i = 0; i < struc->nbonds; i++) {
			pos1 = &struc->atoms[struc->bonds[i].Atom1].Position;
			pos2 = &struc->atoms[struc->bonds[i].Atom2].Position;
			glVertex3f(pos1->x, pos1->y, pos1->z);
			glVertex3f(pos2->x, pos2->y, pos2->z);
		}
		glEnd();
		gluDeleteQuadric(quad);
	}
	
	SwapBuffers();

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::SetStructure(Structure *structure) {

	struc = structure;

	centroid = CPoint3D(0, 0, 0);
	for (int i = 0; i < struc->natoms; i++) {
		centroid += struc->atoms[i].Position;
	}
	centroid *= 1.0f / struc->natoms;

	Render();

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

