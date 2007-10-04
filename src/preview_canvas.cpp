#include "preview_canvas.h"

#include "Math3D.h"
#include "Globals.h"
#include "VirtualSphere.h"
#include "Prefs.h"
#include "MolDisplayWin.h"

#include <iostream>

extern WinPrefs *gPreferences;

/* ------------------------------------------------------------------------- */

PreviewCanvas::PreviewCanvas(
	wxWindow *parent, wxWindowID id,
	int *attributes, const wxPoint& position, const wxSize& size,
	long style, const wxString& name)
	: wxGLCanvas(parent, id, position, size, style, name, attributes),
	  centroid(0, 0, 0) {

	InitRotationMatrix(global_rotation);
	struc = NULL;
	fov = 45.0f;

	gl_initialized = false;

}

/* ------------------------------------------------------------------------- */

PreviewCanvas::~PreviewCanvas() {

	gluDeleteQuadric(quadric);
	quadric = NULL;

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::InitGL() {

	SetCurrent();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	
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

	quadric = gluNewQuadric();

}


/* ------------------------------------------------------------------------- */

void PreviewCanvas::OnPaint(wxPaintEvent& event) {

	wxPaintDC paintDC(this);
	Render();

}  

/* ------------------------------------------------------------------------- */
   
void PreviewCanvas::OnSize(wxSizeEvent& event) {
	
	wxGLCanvas::OnSize(event);

	Refresh();
	Update();

}

/* ------------------------------------------------------------------------- */
/**
 * This function draws the preview of the currently selected structure, if
 * any.  It makes and leaves the canvas' own OpenGL context current.  The
 * background color of the canvas and all other parameters are set by the
 * global preferences for now.
 */

void PreviewCanvas::Render() {

	if (!GetContext()) {
		return;
	}

	if (!gl_initialized) {
		InitGL();
	}

	sphere_list = glGenLists(1);
	glNewList(sphere_list, GL_COMPILE);
	gluSphere(quadric, 1.0f, (long) (1.5f * gPreferences->GetQD3DAtomQuality()),
			  (long) (gPreferences->GetQD3DAtomQuality()));
	glEndList();

	SetCurrent();

	RGBColor *BackgroundColor = gPreferences->GetBackgroundColorLoc();
	glClearColor(BackgroundColor->red / 65536.0f,
				 BackgroundColor->green / 65536.0f,
				 BackgroundColor->blue / 65536.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	int width, height;
	GetClientSize(&width, &height);
	glViewport(0, 0, width, height);
		
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float) width) / height, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glMultMatrixf((float *) global_rotation);
	glTranslatef(-centroid.x, -centroid.y, -centroid.z);
	glColor3f(0.0f, 1.0f, 0.0f);

	// If a structure has been set, draw it.
	if (struc) {

		int i;                 // Index variable
		mpAtom *atom;          // Shortcut to atom
		float radius;          // Radius of atom

		// Get the global material properties set.  Each atom has its
		// own ambient and diffuse color that we'll set later with glColor.
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
		GLfloat l_specular[] = {0.8, 0.8, 0.8, 1.0};
		GLfloat l_shininess[] = {80.0};
		glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, l_specular);
		glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, l_shininess);

		// Draw all the atoms.
		glEnable(GL_RESCALE_NORMAL);
		for (i = 0; i < struc->natoms; i++) {
			atom = &struc->atoms[i];
			glPushMatrix();
			glTranslatef(atom->Position.x, atom->Position.y, atom->Position.z);
			radius = gPreferences->GetAtomScale() * gPreferences->GetAtomSize(atom->Type - 1);
			gPreferences->ChangeColorAtomColor(atom->Type);

			glPushMatrix();
			glScalef(radius, radius, radius);
			glCallList(sphere_list);
			glPopMatrix();
			glPopMatrix();
		}

		// We need these for calculating the offsets for double- and triple-
		// bonds.  But we only want to retrieve them once, so we do so outside
		// the loop.
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, proj);
		glGetIntegerv(GL_VIEWPORT, viewport);

		// Draw all the bonds.
		for (i = 0; i < struc->nbonds; i++) {
			MolDisplayWin::DrawBond(struc->bonds[i],
								    struc->atoms[struc->bonds[i].Atom1],
								    struc->atoms[struc->bonds[i].Atom2],
									*gPreferences, quadric, modelview,
									proj, viewport, sphere_list);
		}
		glDisable(GL_RESCALE_NORMAL);

	}

	glDeleteLists(sphere_list, 1);
	
	SwapBuffers();

}

/* ------------------------------------------------------------------------- */
/**
 * This function causes the preview canvas to display the specified structure.
 * The canvas does not make its own copy of the structure and will not free
 * the structure on deletion.  The parent control should manage this (as
 * the BuilderDlg does).
 * @param structure A pointer to the structure to display.
 */
void PreviewCanvas::SetStructure(Structure *structure) {

	struc = structure;

	// If we have a new structure to show, let's figure out its centroid so
	// we make sure it's in view.
	// TODO: We also need to get the projection set correctly.  For very large
	// structures, we need to consider the spatial extent in setting the fov.
	if (struc) {
		centroid = CPoint3D(0, 0, 0);
		for (int i = 0; i < struc->natoms; i++) {
			centroid += struc->atoms[i].Position;
		}
		centroid *= 1.0f / struc->natoms;
	}

	Render();

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::OnLeftMouseDown(wxMouseEvent& event) {

	const wxPoint& pt = event.GetPosition();
	int width, height;
	
	GetClientSize(&width, &height);
	
	curr_mouse = event.GetPosition();
	prev_mouse = curr_mouse;

	Render();

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::OnMiddleMouseDown(wxMouseEvent& event) {

	curr_mouse = event.GetPosition();
	prev_mouse = curr_mouse;
	
}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::OnMouseDrag(wxMouseEvent& event) {

	if (!event.Dragging()) {
		return;
	}

	const wxPoint& pt = event.GetPosition();
	int width, height;

	GetClientSize(&width, &height);

	prev_mouse = curr_mouse;
	curr_mouse = event.GetPosition();

	if (event.MiddleIsDown() || (event.LeftIsDown() && event.ShiftDown())) {
		fov += curr_mouse.y - prev_mouse.y;
		if (fov < 1e-6f) {
			fov = 1e-6f;
		}
	}

	else if (event.LeftIsDown()) {
		Point prev_pt;
		prev_pt.h = prev_mouse.x;
		prev_pt.v = prev_mouse.y;

		Point curr_pt;
		curr_pt.h = curr_mouse.x;
		curr_pt.v = curr_mouse.y;

		Point sphere_center;
		sphere_center.h = width / 2;
		sphere_center.v = height / 2;

		long sphere_radius = (long) (MAX(sphere_center.h, sphere_center.v) * 0.9f);
		Matrix4D local_rotation;
		Matrix4D tempcopyMatrix;

		VirtualSphereQD3D(prev_pt, curr_pt, sphere_center, sphere_radius,
						  local_rotation, global_rotation);
		MultiplyMatrix(local_rotation, global_rotation, tempcopyMatrix);
		CopyMatrix(tempcopyMatrix, global_rotation);
		OrthogonalizeRotationMatrix(global_rotation);
	}

	Render();
	Refresh(false);

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::OnIdle(wxIdleEvent& event) {

	Render();
	event.RequestMore();

}

/* ------------------------------------------------------------------------- */

BEGIN_EVENT_TABLE(PreviewCanvas, wxGLCanvas)
	EVT_PAINT(PreviewCanvas::OnPaint)
	EVT_SIZE(PreviewCanvas::OnSize)
	EVT_LEFT_DOWN(PreviewCanvas::OnLeftMouseDown)
	EVT_MIDDLE_DOWN(PreviewCanvas::OnMiddleMouseDown)
	EVT_MOTION(PreviewCanvas::OnMouseDrag)
	EVT_ERASE_BACKGROUND(PreviewCanvas::OnErase)
	/* EVT_IDLE(PreviewCanvas::OnIdle) */
END_EVENT_TABLE()

/* ------------------------------------------------------------------------- */

