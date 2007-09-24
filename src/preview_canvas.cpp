#include "preview_canvas.h"

#include "Math3D.h"
#include "Globals.h"
#include "VirtualSphere.h"
#include "Prefs.h"

#include <iostream>

extern WinPrefs *gPreferences;

/* ------------------------------------------------------------------------- */

PreviewCanvas::PreviewCanvas(
	wxWindow *parent, wxWindowID id,
	int *attributes, const wxPoint& position, const wxSize& size,
	long style, const wxString& name)
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
	
	if (GetContext()) {
		GetClientSize(&width, &height);
		
		glViewport(0, 0, width, height);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, ((float) width) / height, 0.1f, 1000.0f);
		glMatrixMode(GL_MODELVIEW);
	}

}

/* ------------------------------------------------------------------------- */
/**
 * This function draws the preview of the currently selected structure, if
 * any.  It makes and leaves the canvas' own OpenGL context current.  The
 * background color of the canvas and all other parameters are set by the
 * global preferences for now.
 */

void PreviewCanvas::Render() {

	// SetCurrent(*context); 
	SetCurrent();

	RGBColor *BackgroundColor = gPreferences->GetBackgroundColorLoc();
	glClearColor(BackgroundColor->red / 65536.0f,
				 BackgroundColor->green / 65536.0f,
				 BackgroundColor->blue / 65536.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
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

		quadric = gluNewQuadric();

		// Draw all the atoms.
		for (i = 0; i < struc->natoms; i++) {
			atom = &struc->atoms[i];
			glPushMatrix();
			glTranslatef(atom->Position.x, atom->Position.y, atom->Position.z);
			radius = gPreferences->GetAtomScale() * gPreferences->GetAtomSize(atom->Type - 1);
			gPreferences->ChangeColorAtomColor(atom->Type);

			gluSphere(quadric, radius,
					  (long) (1.5 * gPreferences->GetQD3DAtomQuality()),
					  gPreferences->GetQD3DAtomQuality());
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
			DrawBond(struc->bonds[i],
					 struc->atoms[struc->bonds[i].Atom1],
					 struc->atoms[struc->bonds[i].Atom2]);
		}

		gluDeleteQuadric(quadric);

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

	Render();

}

/* ------------------------------------------------------------------------- */

void PreviewCanvas::DrawBond(const Bond& bond, const mpAtom& atom1,
							 const mpAtom& atom2) {

	CPoint3D v1, v2, offset, NormalOffset, NormEnd, NormStart = CPoint3D(0,0,1);
	Matrix4D rotMat;
	BondOrder logical_order = bond.Order;

	/* if (tmpOrder == kHydrogenBond) { */
		/* DrawHydrogenBond(ibond); */
		/* continue; */
	/* } */
	// Draw hydrogen bond

	if (bond.Order == kAromaticBond) logical_order = kDoubleBond;
	if (!gPreferences->ColorBondHalves()) logical_order = kSingleBond;

	GLdouble tmpBondSize = gPreferences->GetQD3DBondWidth() /
						   MAX(logical_order, 1);
	if (logical_order > 1) tmpBondSize *= 1.5;
	GLdouble baseBondOffset = -1.75 * tmpBondSize * (MAX(logical_order, 1) - 1);

	GLdouble scr_coords1[3]; // Screen coordinates of atom1
	GLdouble scr_coords2[3]; // Screen coordinates of atom2
	CPoint3D scr_vec;        // Screen space vector between atoms
	GLdouble perp_obj[3];    // Object coords on vector perp. to scr_vec
	CPoint3D offset_vec;     // Direction to shift bond cylinders

	// Find screen coordinates of one atom.
	gluProject(atom1.Position.x, atom1.Position.y, atom1.Position.z,
			   modelview, proj, viewport,
			   &(scr_coords1[0]), &(scr_coords1[1]), &(scr_coords1[2]));

	// Find screen coordinates of other atom.
	gluProject(atom2.Position.x, atom2.Position.y, atom2.Position.z,
			   modelview, proj, viewport,
			   &(scr_coords2[0]), &(scr_coords2[1]), &(scr_coords2[2]));

	// Find vector perpendicular to vector between two screen points and
	// normalize it so we can scalar multiply it later.  We flip and 
	// negate the slope of the line between the two screen coordinates to
	// get the slop of the perpendicular line.
	scr_vec.x = scr_coords2[1] - scr_coords1[1];
	scr_vec.y = scr_coords1[0] - scr_coords2[0];
	scr_vec.z = 0;
	scr_vec *= 1 / scr_vec.Magnitude();

	// Now find a point on the perpendicular vector with atom1's depth
	// and get its object coordinates.
	gluUnProject(scr_coords1[0] + scr_vec.x * 10,
				 scr_coords1[1] + scr_vec.y * 10,
				 scr_coords1[2],
				 modelview, proj, viewport,
				 &(perp_obj[0]), &(perp_obj[1]), &(perp_obj[2]));

	// Finally, we see what direction all bond cylinders must be offset
	// so that they will always stay in view.
	offset_vec = CPoint3D(perp_obj[0], perp_obj[1], perp_obj[2]) -
				 atom1.Position;
	offset_vec *= 1 / offset_vec.Magnitude();
	
	// For each "sub-bond" between these two atoms...
	for (int ipipe = 0; ipipe < MAX(logical_order, 1); ++ipipe) {

		v1 = atom1.Position + offset_vec * baseBondOffset +
			 offset_vec * tmpBondSize * ipipe * 3.5;
		v2 = atom2.Position + offset_vec * baseBondOffset +
			 offset_vec * tmpBondSize * ipipe * 3.5;

		offset = v2 - v1;
		float length = offset.Magnitude();
		if (length > 0.00001) {
			NormalOffset = offset * (1.0f / length);
		} else {
			NormalOffset = CPoint3D(0, 0, 0);
		}

		SetRotationMatrix(rotMat, &NormStart, &NormalOffset);

		rotMat[3][0] = v1.x;
		rotMat[3][1] = v1.y;
		rotMat[3][2] = v1.z;

		glPushMatrix();
		glMultMatrixf((const GLfloat *) &rotMat);

		//center the color change at the middle of the visible part of the bond
		float radius1 = gPreferences->GetAtomScale() * gPreferences->GetAtomSize(atom1.GetType() - 1);
		float radius2 = gPreferences->GetAtomScale() * gPreferences->GetAtomSize(atom2.GetType() - 1);
		float percent1 = radius1 / length;
		float percent2 = radius2 / length;
		float centerPercent = 0.5 + 0.5 * (percent1 - percent2);
		
		CPoint3D v3; //first half bond from atom 1
		v3 = v1 + (v2 - v1) * centerPercent;

		gPreferences->ChangeColorAtomColor(atom1.GetType());
		glPushMatrix();
		gluCylinder(quadric, tmpBondSize, tmpBondSize, length * centerPercent,
					(long) gPreferences->GetQD3DAtomQuality(), (long) (0.5 * gPreferences->GetQD3DAtomQuality()));
		glPopMatrix();

		gPreferences->ChangeColorAtomColor(atom2.GetType());
		glPopMatrix();
		glPushMatrix();
		rotMat[3][0] = v3.x;
		rotMat[3][1] = v3.y;
		rotMat[3][2] = v3.z;
		glMultMatrixf((const GLfloat *) &rotMat);
		gluCylinder(quadric, tmpBondSize, tmpBondSize,
					length * (1 - centerPercent), (long) gPreferences->GetQD3DAtomQuality(),
					(long) (0.5 * gPreferences->GetQD3DAtomQuality()));

		glPopMatrix();
	}
}

/* ------------------------------------------------------------------------- */

BEGIN_EVENT_TABLE(PreviewCanvas, wxGLCanvas)
	EVT_PAINT(PreviewCanvas::OnPaint)
	EVT_SIZE(PreviewCanvas::OnSize)
	EVT_LEFT_DOWN(PreviewCanvas::OnLeftMouseDown)
	EVT_MOTION(PreviewCanvas::OnMouseDrag)
END_EVENT_TABLE()

