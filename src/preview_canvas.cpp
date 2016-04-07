#include "Globals.h"
#include "preview_canvas.h"
#include "Math3D.h"
#include "VirtualSphere.h"
#include "Prefs.h"
#include "MolDisplayWin.h"

#include <iostream>

extern WinPrefs *gPreferences;

#ifdef __WXMSW__
#define GL_RESCALE_NORMAL 0x803A
#endif

/* ------------------------------------------------------------------------- */

PreviewCanvas::PreviewCanvas(wxWindow *parent,
							 wxWindowID id,
							 int *attributes,
							 const wxPoint& position,
							 const wxSize& size,
							 long style,
							 const wxString& name)
	:
#if wxCHECK_VERSION(2,9,0)
	  wxGLCanvas(parent, id, attributes, position, size, style, name),
#else
	  wxGLCanvas(parent, id, position, size, style, name, attributes),
#endif
	  centroid(0, 0, 0),
	  quadric(NULL) {

#if wxCHECK_VERSION(2,9,0)
	context = new wxGLContext(this);
#endif

	InitRotationMatrix(global_rotation);
	struc = NULL;
	fov = 45.0f;

	gl_initialized = false;
	was_dragging = false;
	selected = -1;

}

/* ------------------------------------------------------------------------- */

PreviewCanvas::~PreviewCanvas() {

	if (quadric) {
		gluDeleteQuadric(quadric);
		quadric = NULL;
	}
#if wxCHECK_VERSION(2,9,0)
	if (context) {
		delete context;
		context = NULL;
	}
#endif
	
}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::InitGL() {

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	SetCurrent();
#endif

	gl_initialized = true;
	
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
	GLfloat ambient_color[] = {0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat diffuse_color[] = {0.7f, 0.7f, 0.7f, 1.0f};
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
	
	/* wxGLCanvas::OnSize(event); */

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

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	if (!GetContext()) {
		return;
	}

	SetCurrent();
#endif

	if (!gl_initialized) {
		InitGL();
	}

	sphere_list = glGenLists(1);
	glNewList(sphere_list, GL_COMPILE);
	gluQuadricTexture(quadric, GL_TRUE);
	gluSphere(quadric, 1.0f, (long) (1.5f * gPreferences->GetQD3DAtomQuality()),
			  (long) (gPreferences->GetQD3DAtomQuality()));
	glEndList();

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

		// Get the global material properties set.  Each atom has its
		// own ambient and diffuse color that we'll set later with glColor.
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
		GLfloat l_specular[] = {0.8f, 0.8f, 0.8f, 1.0f};
		GLfloat l_shininess[] = {80.0};
		glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, l_specular);
		glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, l_shininess);

		DrawAtoms();

		// Should this structure be dropped in at a bonding site, we'll need
		// a bonding site on the structure itself to connect to.  To do this,
		// an atom may have been chosen as the prune atom.  We drop a big, red,
		// screen-aligned X over the prune atom.
		if (struc->atom_to_prune >= 0) {
			mpAtom *atom = &struc->atoms[struc->atom_to_prune];
			float radius;

			glPushMatrix(); // structure centroid

			glTranslatef(atom->Position.x, atom->Position.y, atom->Position.z);
			radius = gPreferences->GetAtomScale() *
					 gPreferences->GetAtomSize(atom->Type - 1);

			// To make the X always face the viewer, we need to figure out what
			// the x-, y-, and z-axes in eye space are in object space (where we
			// are drawing).
			float m[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, m);
			CPoint3D x(m[0], m[4], m[8]);
			CPoint3D y(m[1], m[5], m[9]);
			CPoint3D z(m[2], m[6], m[10]);
			x *= radius * 1.1f;
			y *= radius * 1.1f;
			z *= radius * -1.1f;

			// Display a big red X.  We draw the lines along the eye-space
			// axes, using their object-space coordinates.  The x- and y-axes
			// combine to form the corners of square; we draw along the
			// square's diagonals.  We also move closer to the viewer along the
			// z-axis.
			glDisable(GL_LIGHTING);
			glLineWidth(3.0f);
			glColor3f(1.0f, 0.0f, 0.0f);
			glBegin(GL_LINES);
			glVertex3f(-x.x - y.x - z.x, -x.y - y.y - z.y, -x.z - y.z - z.z);
			glVertex3f( x.x + y.x - z.x,  x.y + y.y - z.y,  x.z + y.z - z.z);
			glVertex3f(-x.x + y.x - z.x, -x.y + y.y - z.y, -x.z + y.z - z.z);
			glVertex3f( x.x - y.x - z.x,  x.y - y.y - z.y,  x.z - y.z - z.z);
			glEnd();
			glLineWidth(1.0f);
			glEnable(GL_LIGHTING);

			glPopMatrix(); // structure centroid
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

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "gluErrorString(err): " << gluErrorString(err) << std::endl;
	}
	
	SwapBuffers();

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::DrawAtoms() {

	mpAtom *atom;          // Shortcut to atom
	float radius;          // Radius of atom

	// Draw all the atoms.
	glEnable(GL_RESCALE_NORMAL);
	for (int i = 0; i < struc->natoms; i++) {
		atom = &struc->atoms[i];
		glPushMatrix();
		glTranslatef(atom->Position.x, atom->Position.y, atom->Position.z);
		radius = gPreferences->GetAtomScale() * gPreferences->GetAtomSize(atom->Type - 1);
		gPreferences->ChangeColorAtomColor(atom->Type);

		glPushMatrix();
		glScalef(radius, radius, radius);
		glLoadName(i + 1);
		glCallList(sphere_list);
		glPopMatrix();
		glPopMatrix();
	}

	glLoadName(0);

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

	selected = -1;

	Render();

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::OnLeftMouseDown(wxMouseEvent& event) {

	int width, height;
	
	GetClientSize(&width, &height);
	
	curr_mouse = event.GetPosition();
	prev_mouse = curr_mouse;
	was_dragging = false;

	CaptureMouse();
	Render();

}

/* ------------------------------------------------------------------------- */

#define SELECT_BUFFER_SIZE 128
int PreviewCanvas::Pick() {

	GLuint buff[SELECT_BUFFER_SIZE];
	GLint hits, view[4];
	int selected = -1;

	glSelectBuffer(SELECT_BUFFER_SIZE, buff);
	glGetIntegerv(GL_VIEWPORT, view);

	sphere_list = glGenLists(1);
	glNewList(sphere_list, GL_COMPILE);
	gluSphere(quadric, 1.0f, (long) (1.5f * gPreferences->GetQD3DAtomQuality()),
			  (long) (gPreferences->GetQD3DAtomQuality()));
	glEndList();

	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);

	int width, height;
	GetClientSize(&width, &height);

	// Set narrow window around mouse point.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPickMatrix(curr_mouse.x, view[3] - curr_mouse.y, 2.0, 2.0, view);
	gluPerspective(fov, ((float) width) / height, 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glMultMatrixf((float *) global_rotation);
	glTranslatef(-centroid.x, -centroid.y, -centroid.z);

	// Now render the atoms only.
	if (struc) {
		DrawAtoms();
	}

	hits = glRenderMode(GL_RENDER);

	// Each hit record has number of names, min depth, max depth, and a list of
	// names on the stack for the hit.
	if (hits) {
		int i;
		unsigned int min_depth;

		selected = buff[3] - 1;
		min_depth = buff[1];
		for (i = 1; i < hits; i++) {
			if (buff[i * 4 + 1] < min_depth) {
				min_depth = buff[i * 4 + 1];
				selected = buff[i * 4 + 3] - 1;
			}
		}
	}

	glDeleteLists(sphere_list, 1);

	return selected;

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::OnMiddleMouseDown(wxMouseEvent& event) {

	curr_mouse = event.GetPosition();
	prev_mouse = curr_mouse;
	was_dragging = false;

	CaptureMouse();
	
}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::OnMouseDrag(wxMouseEvent& event) {

	if (!event.Dragging()) {
		return;
	}

	int width, height;

	GetClientSize(&width, &height);

	prev_mouse = curr_mouse;
	curr_mouse = event.GetPosition();

	was_dragging = true;

	if (event.MiddleIsDown() || (event.LeftIsDown() && event.ShiftDown())) {
		fov += curr_mouse.y - prev_mouse.y;
		if (fov < 1e-6f) {
			fov = 1e-6f;
		}
	}

	else if (event.LeftIsDown()) {
		wxPoint sphere_center;
		sphere_center.x = width / 2;
		sphere_center.y = height / 2;

		long sphere_radius = (long) (MAX(sphere_center.x, sphere_center.y) * 0.9f);
		Matrix4D local_rotation;
		Matrix4D tempcopyMatrix;

		VirtualSphereQD3D(prev_mouse, curr_mouse, sphere_center, sphere_radius,
						  local_rotation, global_rotation);
		MultiplyMatrix(local_rotation, global_rotation, tempcopyMatrix);
		CopyMatrix(tempcopyMatrix, global_rotation);
		OrthogonalizeRotationMatrix(global_rotation);
	}

	Render();
	Refresh(false);

}

// --------------------------------------------------------------------------- 

void PreviewCanvas::OnMiddleMouseUp(wxMouseEvent& event) {

	if (HasCapture()) {
		ReleaseMouse();
	}

}

// --------------------------------------------------------------------------- 

void PreviewCanvas::OnLeftMouseUp(wxMouseEvent& event) {

	if (HasCapture()) {
		ReleaseMouse();
	}

	if (!event.ShiftDown() && !was_dragging) {
		int selected = Pick();
		if (selected >= 0 && struc->atoms[selected].Type == 1 && struc->FragName.size() <=0) {
			struc->SetPruneAtom(selected);
		}
	}

	Render();

}

// --------------------------------------------------------------------------- 

void PreviewCanvas::OnMouseEnterWindow(wxMouseEvent& event) {

	curr_mouse = event.GetPosition();
	prev_mouse = curr_mouse;

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::OnIdle(wxIdleEvent& event) {

	Render();
	event.RequestMore();

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::OnKey(wxKeyEvent& event) {

	/* std::cout << "event.GetKeyCode(): " << event.GetKeyCode() << std::endl; */
	event.Skip();

}	

/* ------------------------------------------------------------------------- */

BEGIN_EVENT_TABLE(PreviewCanvas, wxGLCanvas)
	EVT_PAINT(PreviewCanvas::OnPaint)
	EVT_SIZE(PreviewCanvas::OnSize)
	/* EVT_CHAR(PreviewCanvas::OnKey) */
	EVT_LEFT_DOWN(PreviewCanvas::OnLeftMouseDown)
	EVT_MIDDLE_DOWN(PreviewCanvas::OnMiddleMouseDown)
	EVT_LEFT_UP(PreviewCanvas::OnLeftMouseUp)
	EVT_MIDDLE_UP(PreviewCanvas::OnMiddleMouseUp)
	EVT_MOTION(PreviewCanvas::OnMouseDrag)
	EVT_ERASE_BACKGROUND(PreviewCanvas::OnErase)
	EVT_ENTER_WINDOW(PreviewCanvas::OnMouseEnterWindow)
	/* EVT_IDLE(PreviewCanvas::OnIdle) */
END_EVENT_TABLE()

/* ------------------------------------------------------------------------- */

