/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*
	Added RotateMoleculeGL routine and code to generate the angle strings and trackball - BMB July 2001
	Corrected Create3DGLPICT for resolutions other than 72 dpi - BMB Feb 2002
*/

#include <new>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <wx/stdpaths.h>

#include "Globals.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "SurfaceTypes.h"
#include "Math3D.h"
#include "InputData.h"
#include "MolDisplayWin.h"
#include "mpGLCanvas.h"
#include "mmp_gl.h"
#include "glf.h"
#include "Prefs.h"
#include "Progress.h"
#include "patterns.h"
#include "Files.h"

// ----------------------------------------------------------------------------
// FUNCTION PROTOTYPES
// ----------------------------------------------------------------------------

void DrawPipeCylinder(float length, GLUquadric *quadric, unsigned int ncaps,
					  GLuint sphere_list, float radius, long quality);
void DrawPipeSpheres(const WinPrefs& Prefs, float length, float scale_factor,
					 bool is_masked, GLuint sphere_list);
void DashedQuadFromLine(const CPoint3D& pt1, const CPoint3D& pt2, float width,
						float m[16], const CPoint3D& x_world, float offset,
						GLuint length_anno_tex_id, const WinPrefs * Prefs,
						bool draw_label = true);
void DrawAngleAnnotation(const CPoint3D *pt1, const CPoint3D *pt2,
						 const CPoint3D *pt3, const WinPrefs *Prefs,
						 GLuint length_anno_tex_id, CPoint3D *ambig_axis = NULL);
void CreateCylinderFromLine(GLUquadricObj * qobj, const CPoint3D & lineStart,
							const CPoint3D & lineEnd, float lineWidth,
							int nslices = 4, int nstacks = 1, bool cap = false);
void DrawRotationAxis(const CPoint3D & lineStart, const CPoint3D & lineEnd,
					  const int & order);
void DrawInversionPoint(void);
void DrawTranslucentPlane(const CPoint3D & origin, const CPoint3D & p1,
						  const CPoint3D & p2);
void DrawArrow(const float & length, const float & width, const int & quality);
void DrawSceneString(const float scale_factor, const float shift_x,
		             const float shift_y, const float shift_z,
					 const wxString& label);
void DrawString(const char *str);

// ----------------------------------------------------------------------------
// GLOBALS
// ----------------------------------------------------------------------------

const GLubyte stippleMask[128] = {
	0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
	0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
	0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
	0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
	0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
	0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
	0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
	0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00,
	0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00
};

GLfloat d_specular[] = {0.1f, 0.1f, 0.1f, 1.0};
GLfloat d_shininess = 1.0;
GLfloat d_diffuse[] = {0.02f,0.02f,0.02f,0.8f};
GLfloat d_ambient[] = {0.1f,0.1f,0.1f,0.8f};

GLfloat l_emissive[] = {0.0, 0.0, 0.0, 1.0};
GLfloat l_specular[] = {0.8f, 0.8f, 0.8f, 1.0};
GLfloat l_shininess = 80.0;
GLfloat l_diffuse[] = {0.2f,0.2f,0.2f,0.8f};
GLfloat l_ambient[] = {0.1f,0.1f,0.1f,0.8f};

// ----------------------------------------------------------------------------
// FUNCTION DEFINITIONS
// ----------------------------------------------------------------------------

/**
 * This function frees any transparent geometries and any active display
 * lists.  It should be called when the view changes (which alters sort order
 * of transparent surfaces), surfaces are updated, or the display list content
 * has been altered.  It does not remake any lists.
 */
void MolDisplayWin::ReleaseLists() {
	triangleCount = 0;

	delete[] transpTriList;
	transpTriList = NULL;
	delete[] transpSortVertex;
	transpSortVertex = NULL;
	delete[] transpIndex;
	transpIndex = NULL;

	if (MainListActive) {
		glDeleteLists(MainDisplayList, 1);
		MainListActive = false;
	}
	if (SurfaceListActive) {
		glDeleteLists(SurfaceDisplayList, 1);
		SurfaceListActive = false;
	}
}

/**
 Display a transformation annotation, optionally consisting of Euler
 angles and an outline of the trackball's footprint.
 */
void MolDisplayWin::ShowRotation(bool ShowAngles, bool ShowTrackball) {
	
	GLint matrixMode;
	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	wxRect DisplayRect = glCanvas->GetRect();
	long hsize = DisplayRect.GetWidth();
	long vsize = DisplayRect.GetHeight();
	glScalef(2.0f / hsize, -2.0f /  vsize, 1.0);
	glTranslatef(-hsize / 2.0f, -vsize / 2.0f, 0.0);

	glColor3fv(fg_color);

	if (ShowAngles) {
		char AngleString[50];
		float psi, phi, theta;
		MatrixToEulerAngles(MainData->TotalRotation, &psi, &phi, &theta);
		sprintf((char *)AngleString, "%.2f, %.2f, %.2f, Scale:%.2f",
				psi, phi, theta, MainData->WindowSize);

		DrawStaticLabel(AngleString, 10, -20);
	}

	//Draw the trackball outline
	if (ShowTrackball) {
		wxPoint sphereCenter;
		long sphereRadius; 
		sphereCenter.x = hsize / 2; 
		sphereCenter.y = vsize / 2;
		if (sphereCenter.x >= sphereCenter.y)
			sphereRadius   = (long) (((float) sphereCenter.x) * 0.9);
		else
			sphereRadius   = (long) (((float) sphereCenter.y) * 0.9);
		long NumDivisions = (long) (20.0 * (1.0 + sphereRadius / 200.0));
		float divarc = (2 * kPi) / NumDivisions;

		glLineWidth(1);
		glBegin(GL_LINE_LOOP);
		glVertex3d(sphereCenter.x - sphereRadius, sphereCenter.y, 0.0);
		for (int i=0; i<NumDivisions; i++) {
			float x = sphereCenter.x - (sphereRadius*cos(i*divarc));
			float y = sphereCenter.y + (sphereRadius*sin(i*divarc));
			glVertex3d(x, y, 0.0);
		}
		glEnd();
	}
	glPopMatrix(); // GL_MODELVIEW
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(matrixMode);
	
}

/**
 * Draws molecular structures, surfaces, and annotations.
 * @pre Context, projection, and viewport have been set. 
 */
void MolDisplayWin::DrawGL(int do_shader) {
	if (GLUpdateInProgress) return;	//a previous call in the stack is already updating this window. Do not repeat.
	
	GLUpdateInProgress = true;	//set the flag to prevent calls from updating at the same time.

	// Make (0, 0, WindowSize) the origin.  Moves geometry away from camera.
	glTranslatef(0, 0, -MainData->WindowSize);

	if ((Prefs->ShowAtomicSymbolLabels() || Prefs->ShowAtomNumberLabels()) &&
		(shader_program == 0 || do_shader)) {
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		DrawLabel();
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	glEnable(GL_LIGHTING);

	// Rotate about the origin (the molecule's centroid).
	glMultMatrixf((const GLfloat *) &(MainData->TotalRotation));

	// Make the molecule's centroid the origin.
	glTranslatef(-MainData->Centroid.x, -MainData->Centroid.y,
	             -MainData->Centroid.z);

	// We're in a good space to draw the axis.  We've rotated, but we haven't
	// moved around for the molecule yet.
	if (MainData->ShowAxis()) AddAxisGL();

	if (GLEW_VERSION_2_0 && do_shader) {
		GLint mode;
		glGetIntegerv(GL_RENDER_MODE, &mode);
		if (Prefs->GetShaderMode() && mode == GL_RENDER) {
			glBindTexture(GL_TEXTURE_2D, depth_tex_id);
			glUseProgram(shader_program);
		}
	}
	
	//Draw the main molecular geometry
	if (MainData->cFrame->NumAtoms > 0) {
		if (MainListActive) {
			glCallList(MainDisplayList);
		} else { // build the main display list
			// Suppress this temporarily because double- and triple-bond display
			// requires a transformation depending on the current viewing 
			// transformation.
			// MainDisplayList = glGenLists(1); 
			// glNewList(MainDisplayList, GL_COMPILE_AND_EXECUTE); 
			DrawMoleculeCoreGL();
			// glEndList(); 
			// MainListActive = true; 
		}
	}

	if (GLEW_VERSION_2_0) {
		glUseProgram(0);
	}

	if (MainData->GetAnnotationCount() > 0) {
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadName(MMP_ANNOTATION);
		glPushName(0);
		std::vector<Annotation *>::const_iterator anno;
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);
		int anno_id = 0;
		for (anno = MainData->Annotations.begin();
			 anno != MainData->Annotations.end(); anno++) {
			glLoadName(anno_id + 1);
			glColor3fv(fg_color);
			(*anno)->draw(this);
			anno_id++;
		}
		glPopName();
		glEnable(GL_LIGHTING);
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	
	if (GLEW_VERSION_2_0 && Prefs->GetShaderMode() && do_shader) {
		glBindTexture(GL_TEXTURE_2D, depth_tex_id);
		glUseProgram(shader_program);
	}

	// Add any surfaces
	Surface * lSurface = MainData->cFrame->SurfaceList;
	haveTransparentSurfaces = false;

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, l_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, l_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30.0f);

	glEnable(GL_RESCALE_NORMAL);
	int surf_id = 0;
	while (lSurface) {
		++surf_id;
		if (lSurface->GetVisibility()) {
			if (!lSurface->isTransparent()) {
				lSurface->Draw3DGL(MainData, Prefs, NULL);
			} else {
				haveTransparentSurfaces = true;
			}
		}
		lSurface = lSurface->GetNextSurface();
	}
	glDisable(GL_RESCALE_NORMAL);

	if (Prefs->ShowSymmetryOperators()) {
		if (GLEW_VERSION_2_0) {
			glUseProgram(0);
		}
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		AddSymmetryOperators();
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		if (GLEW_VERSION_2_0 && Prefs->GetShaderMode() && do_shader) {
			glBindTexture(GL_TEXTURE_2D, depth_tex_id);
			glUseProgram(shader_program);
		}
	}

	//Transparent surfaces have to be depth sorted and drawn last.
	if (haveTransparentSurfaces) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Surface * lSurface = MainData->cFrame->SurfaceList;
		if (! transpTriList) {
			long totalTriCount = 0;
			while (lSurface) {
				if (lSurface->GetVisibility() && lSurface->isTransparent()) {
					totalTriCount += lSurface->getTriangleCount();
				}
				lSurface = lSurface->GetNextSurface();
			}
			transpTriList = new myGLTriangle[totalTriCount];
			transpSortVertex = new float[totalTriCount];
			transpIndex = new long[totalTriCount];
			triangleCount = totalTriCount;
			
			lSurface = MainData->cFrame->SurfaceList;
			long triStartCount = 0;
			while (lSurface) {
				if (lSurface->GetVisibility() && lSurface->isTransparent()) {
					triStartCount += lSurface->Draw3DGL(MainData, Prefs, 
											&(transpTriList[triStartCount]));
				}
				lSurface = lSurface->GetNextSurface();
			}
			for (int i=0; i<triangleCount; i++) transpIndex[i] = i;
			SortTransparentTriangles();
		}
		DrawTransparentTriangles();
		glDisable(GL_BLEND);
	}

	if (GLEW_VERSION_2_0) {
		glUseProgram(0);
	}

	/* glPopMatrix(); // unrotated scene */

	if (lasso_has_area) {
		int canvas_width, canvas_height;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glCanvas->GetClientSize(&canvas_width, &canvas_height);
		gluOrtho2D(0, canvas_width, 0, canvas_height);

		glColor4f(0.5f, 0.5f, 0.5f, 0.4f);
		glRectf(lasso_start.x, lasso_start.y, lasso_end.x, lasso_end.y);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(3, (GLushort) 43690);
		glColor4f(fg_color[0], fg_color[1], fg_color[2], 0.9f);
		glLineWidth(2.0f);
		glBegin(GL_LINE_LOOP);
			glVertex2f(lasso_start.x, lasso_start.y);
			glVertex2f(lasso_end.x, lasso_start.y);
			glVertex2f(lasso_end.x, lasso_end.y);
			glVertex2f(lasso_start.x, lasso_end.y);
		glEnd();
		glLineWidth(1.0f);
		glDisable(GL_LINE_STIPPLE);
		glEnable(GL_DEPTH_TEST);

		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glDisable(GL_BLEND);
	}

	glDisable(GL_LIGHTING);

	if (do_rotate_annotation) {
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		ShowRotation(Prefs->GetShowAngles() && !rotate_timer.IsRunning(),
					 !rotate_timer.IsRunning());
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	GLUpdateInProgress = false;	//reset the flag to allow updates.
}

void AnnotationLength::draw(const MolDisplayWin * win) const {
	MoleculeData * maindata = win->GetData();
	Frame * cFrame = maindata->GetCurrentFramePtr();
	WinPrefs * Prefs = win->GetPrefs();
	//validate the atom references for this frame
	if ((atoms[0] < 0)||(atoms[0] >= cFrame->GetNumAtoms())) return;
	if ((atoms[1] < 0)||(atoms[1] >= cFrame->GetNumAtoms())) return;

	GLdouble modelview[16];
	GLdouble proj[16];
	GLint viewport[4];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	CPoint3D lookat_eye = CPoint3D(0.0f, 0.0f, 1.0f);
	CPoint3D up_eye = CPoint3D(0.0f, 1.0f, 0.0f);
	CPoint3D lookat_world;
	CPoint3D up_world;
	CPoint3D r;
	float m[16];
	Matrix4D mv_inv;

	// What we want to do here is make the annotation always face the
	// viewer.  The computations below compute a rotation matrix to align
	// to the camera.  This only needs to be computed once for all
	// annotations since the camera doesn't change positions.

	// Invert just the rotation portion of the modelview matrix.  This is
	// much faster than inverting an arbitrary matrix.
	mv_inv[0][0] = modelview[0];
	mv_inv[0][1] = modelview[4];
	mv_inv[0][2] = modelview[8];
	mv_inv[0][3] = 0.0f;
	mv_inv[1][0] = modelview[1];
	mv_inv[1][1] = modelview[5];
	mv_inv[1][2] = modelview[9];
	mv_inv[1][3] = 0.0f;
	mv_inv[2][0] = modelview[2];
	mv_inv[2][1] = modelview[6];
	mv_inv[2][2] = modelview[10];
	mv_inv[2][3] = 0.0f;
	mv_inv[3][0] = 0.0f;
	mv_inv[3][1] = 0.0f;
	mv_inv[3][2] = 0.0f;
	mv_inv[3][3] = 1.0f;

	// Transform the eye space vectors to world coordinates, and find 
	// a third vector to form a basis set.
	Rotate3DPt(mv_inv, lookat_eye, &lookat_world);
	Rotate3DPt(mv_inv, up_eye, &up_world);
	CrossProduct3D(&lookat_world, &up_world, &r);

	m[0] = r.x;
	m[1] = r.y;
	m[2] = r.z;
	m[3] = 0.0f;

	m[4] = up_world.x;
	m[5] = up_world.y;
	m[6] = up_world.z;
	m[7] = 0.0f;

	m[8] = lookat_world.x;
	m[9] = lookat_world.y;
	m[10] = lookat_world.z;
	m[11] = 0.0f;

	m[12] = m[13] = m[14] = 0.0f;
	m[15] = 1.0f;

	// x_world will indicate what vector in world coordinates will effect
	// a direction in the eye's x direction.  This is the direction in
	// which the length label will appear.
	CPoint3D x_eye;
	CPoint3D x_world;
	x_eye.x = 1.0f;
	x_eye.y = 0.0f;
	x_eye.z = 0.0f;
	Rotate3DPt(mv_inv, x_eye, &x_world);

	int bond_id;
	float bond_size;

	bond_id = cFrame->BondExists(atoms[0], atoms[1]);

	// If a bond exists between the two atoms, we need to push out the
	// length label accordingly.
	GLdouble BondSize = Prefs->GetQD3DBondWidth();
	if (bond_id > -1) {
		bond_size = BondSize / MAX(cFrame->GetBondOrder(bond_id), 1.0f) *
		3.5f * cFrame->GetBondOrder(bond_id) / 2.0f;
		if (cFrame->GetBondOrder(bond_id) > 1) {
			bond_size *= 1.5;
		}
	} else {
		bond_size = 0.0f;
	}
	
	CPoint3D pt1, pt2;
	cFrame->GetAtomPosition(atoms[0], pt1);
	cFrame->GetAtomPosition(atoms[1], pt2);

	// Draw the dashed line and label.
	DashedQuadFromLine(pt1, pt2, BondSize * 0.25, m, x_world, bond_size,
		win->GetLengthTexId(), Prefs);
}

void AnnotationAngle::draw(const MolDisplayWin * win) const {

	MoleculeData *maindata = win->GetData();
	Frame *cFrame = maindata->GetCurrentFramePtr();

	// Validate the atom references for this frame
	if ((atoms[0] < 0)||(atoms[0] >= cFrame->GetNumAtoms())) return;
	if ((atoms[1] < 0)||(atoms[1] >= cFrame->GetNumAtoms())) return;
	if ((atoms[2] < 0)||(atoms[2] >= cFrame->GetNumAtoms())) return;

	CPoint3D atom1_pos, atom2_pos, atom3_pos;

	cFrame->GetAtomPosition(atoms[0], atom1_pos);
	cFrame->GetAtomPosition(atoms[1], atom2_pos);
	cFrame->GetAtomPosition(atoms[2], atom3_pos);
	
	DrawAngleAnnotation(&atom1_pos, &atom2_pos, &atom3_pos, win->GetPrefs(),
						win->GetLengthTexId());

}

void AnnotationDihedral::draw(const MolDisplayWin * win) const {

	MoleculeData *maindata = win->GetData();
	Frame *cFrame = maindata->GetCurrentFramePtr();

	//validate the atom references for this frame
	if ((atoms[0] < 0)||(atoms[0] >= cFrame->GetNumAtoms())) return;
	if ((atoms[1] < 0)||(atoms[1] >= cFrame->GetNumAtoms())) return;
	if ((atoms[2] < 0)||(atoms[2] >= cFrame->GetNumAtoms())) return;
	if ((atoms[3] < 0)||(atoms[3] >= cFrame->GetNumAtoms())) return;

	CPoint3D atom1_pos;
	CPoint3D atom2_pos;
	CPoint3D atom3_pos;
	CPoint3D atom4_pos;
	CPoint3D vec[2];
	CPoint3D vec2;
	CPoint3D normal1;
	CPoint3D normal2;
	CPoint3D binormal1;
	CPoint3D binormal2;
	Matrix4D plane2xy;
	float angle;
	GLfloat color[4];

	glGetFloatv(GL_CURRENT_COLOR, color);
	
	cFrame->GetAtomPosition(atoms[0], atom1_pos);
	cFrame->GetAtomPosition(atoms[1], atom2_pos);
	cFrame->GetAtomPosition(atoms[2], atom3_pos);
	cFrame->GetAtomPosition(atoms[3], atom4_pos);
	
	// The first three atoms (1, 2, 3) form one plane.  The last three
	// (2, 3, 4) form the second plane.  We want to find the angle
	// between those two planes, for that is the dihedral angle.
	
	// Find the first plane's normal by finding the vectors between its
	// points and calculating the normal from them.
	vec[0] = atom1_pos - atom3_pos;
	Normalize3D(&vec[0]);
	
	vec2 = atom2_pos - atom3_pos;
	Normalize3D(&vec2);
	
	CrossProduct3D(&vec[0], &vec2, &normal1);
	
	// Find the second plane.  The two planes share a vector between
	// atoms 2 and 3, so we just reuse that from the first plane.
	vec[1] = atom4_pos - atom3_pos;
	Normalize3D(&vec[1]);
	
	CrossProduct3D(&vec[1], &vec2, &normal2);
	
	// Okay, we've drawn the planes.  Now we want to draw an angle
	// annotation between them.  The angle annotation should span 
	// from the midpoint on one half-circle's arc to the midpoint on
	// the other half circles arc.  These can be found by considering
	// the vector between atoms 2 and 3 as the tangent vector, and
	// find the the binormal as the crossproduct of tangent and normal.
	CrossProduct3D(&vec2, &normal1, &binormal1);
	CrossProduct3D(&vec2, &normal2, &binormal2);
	
	Normalize3D(&binormal1);
	Normalize3D(&binormal2);
	
	// Figure out the actual points from the vector displacements.
	CPoint3D pt1 = atom3_pos + binormal1;
	CPoint3D pt3 = atom3_pos + binormal2;
	
	Matrix4D m;
	CPoint3D pt1_eye;
	CPoint3D pt3_eye;
	int id;

	glGetFloatv(GL_MODELVIEW_MATRIX, (float *) m);
	Rotate3DPt(m, pt1, &pt1_eye);
	Rotate3DPt(m, pt3, &pt3_eye);

	id = pt3_eye.z < pt1_eye.z;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	for (int j = 0; j <= 1; j++, id = (id + 1) % 2) {
		// Make this plane look like the x-y plane for easier circle
		// drawing.  The vector from atom3 to atom2 should look like
		// the positive x-axis.
		SetPlaneRotation(plane2xy, vec2, vec[id]);
		
		glPushMatrix();
		glTranslatef(atom3_pos.x, atom3_pos.y, atom3_pos.z);
		glMultMatrixf((GLfloat *) plane2xy);

		glColor4f(color[0] - 0.3f, color[1] - 0.3f, color[2] - 0.3f, 0.2f);
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(0.0f, 0.0f, 0.0f);
		for (angle = 0.0f; angle <= 3.1416f; angle += 0.01f) {
			glVertex3f(cos(angle), sin(angle), 0.0f);
		}
		glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();

		glColor4f(color[0] - 0.6f, color[1] - 0.06f, color[2] - 0.06f, 0.2f);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
			glVertex3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);
		glEnd();
		glLineWidth(1.0f);

		glPopMatrix();
	}
	
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	glColor4f(color[0], color[1], color[2], 1.0f);
	DrawAngleAnnotation(&pt1, &atom3_pos, &pt3, win->GetPrefs(),
						win->GetLengthTexId(), &normal1);
	
}

void AnnotationMarker::draw(const MolDisplayWin * win) const {

	float m[16];
	float angle;
	float ca1, sa1;
	float ca2, sa2;
	float invert1[3];
	float invert2[3];
	CPoint3D atom_pos;
	float radius;

	MoleculeData *maindata = win->GetData();
	Frame *cFrame = maindata->GetCurrentFramePtr();
	WinPrefs * Prefs = win->GetPrefs();

	// Validate the atom reference for this frame.
	if (atoms[0] < 0 || atoms[0] >= cFrame->GetNumAtoms()) return;

	float AtomScale = Prefs->GetAtomScale();
	long curAtomType = cFrame->GetAtomType(atoms[0]) - 1;

	radius = AtomScale * Prefs->GetAtomSize(curAtomType);

	cFrame->GetAtomPosition(atoms[0], atom_pos);

	glPushMatrix();
	glTranslatef(atom_pos.x, atom_pos.y, atom_pos.z);

	// RGBColor * BackgroundColor = Prefs->GetBackgroundColorLoc(); 

	// invert1[0] = 1.0f - BackgroundColor->red / 65536.0f; 
	// invert1[1] = 1.0f - BackgroundColor->green / 65536.0f; 
	// invert1[2] = 1.0f - BackgroundColor->blue / 65536.0f; 
	invert1[0] = invert1[1] = invert1[2] = 0.0f;

	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glLineWidth(3.0f);

	invert2[0] = 0.0f;
	invert2[1] = 1.0f;
	invert2[2] = 0.0f;

	glBegin(GL_QUAD_STRIP);
	for (angle = 0.0f; angle <= 6.24f; angle += 0.1f) {
		ca1 = cos(angle) * (radius + 0.03f);
		sa1 = sin(angle) * (radius + 0.03f);
		ca2 = cos(angle) * (radius + 0.08f);
		sa2 = sin(angle) * (radius + 0.08f);
		glColor3fv(invert1);
		glVertex3f(ca1 * m[0] + sa1 * m[1],
					ca1 * m[4] + sa1 * m[5],
					ca1 * m[8] + sa1 * m[9]);
		glColor3fv(invert2);
		glVertex3f(ca2 * m[0] + sa2 * m[1],
					ca2 * m[4] + sa2 * m[5],
					ca2 * m[8] + sa2 * m[9]);
	}
	ca1 = cos(0.0f) * (radius + 0.03f);
	sa1 = sin(0.0f) * (radius + 0.03f);
	ca2 = cos(0.0f) * (radius + 0.08f);
	sa2 = sin(0.0f) * (radius + 0.08f);
	glColor3fv(invert1);
	glVertex3f(ca1 * m[0] + sa1 * m[1],
				ca1 * m[4] + sa1 * m[5],
				ca1 * m[8] + sa1 * m[9]);
	glColor3fv(invert2);
	glVertex3f(ca2 * m[0] + sa2 * m[1],
				ca2 * m[4] + sa2 * m[5],
				ca2 * m[8] + sa2 * m[9]);
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (angle = 0.0f; angle <= 6.24f; angle += 0.1f) {
		ca1 = cos(angle) * (radius + 0.08f);
		sa1 = sin(angle) * (radius + 0.08f);
		ca2 = cos(angle) * (radius + 0.13f);
		sa2 = sin(angle) * (radius + 0.13f);
		glColor3fv(invert2);
		glVertex3f(ca1 * m[0] + sa1 * m[1],
					ca1 * m[4] + sa1 * m[5],
					ca1 * m[8] + sa1 * m[9]);
		glColor3fv(invert1);
		glVertex3f(ca2 * m[0] + sa2 * m[1],
					ca2 * m[4] + sa2 * m[5],
					ca2 * m[8] + sa2 * m[9]);
	}
	ca1 = cos(0.0f) * (radius + 0.08f);
	sa1 = sin(0.0f) * (radius + 0.08f);
	ca2 = cos(0.0f) * (radius + 0.13f);
	sa2 = sin(0.0f) * (radius + 0.13f);
	glColor3fv(invert2);
	glVertex3f(ca1 * m[0] + sa1 * m[1],
				ca1 * m[4] + sa1 * m[5],
				ca1 * m[8] + sa1 * m[9]);
	glColor3fv(invert1);
	glVertex3f(ca2 * m[0] + sa2 * m[1],
				ca2 * m[4] + sa2 * m[5],
				ca2 * m[8] + sa2 * m[9]);
	glEnd();
	glLineWidth(1.0f);
	glPopMatrix();
}

void MolDisplayWin::DrawStaticLabel(const char *label, GLfloat x, GLfloat y) {
	int canvasWidth, canvasHeight;
	
	glCanvas->GetSize(&canvasWidth, &canvasHeight);
	glColor3fv(fg_color);
	
	GLint matrixMode;
	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, canvasWidth, 0, canvasHeight);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if (x < 0) x = canvasWidth + x;
	if (y < 0) y = canvasHeight + y;

	glTranslatef(x, y, 0.0f);
	glScalef(300, 300, 1);
	
	glLoadName(MMP_NULL);

	// glDisable(GL_DEPTH_TEST); 
	glfStartBitmapDrawing();

	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	glEnable(GL_BLEND);
	glfDrawBMaskString(label);
	glDisable(GL_BLEND);

	glfStopBitmapDrawing();
	// glEnable(GL_DEPTH_TEST); 

	glFlush();
	
	glPopMatrix(); // GL_MODELVIEW

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(matrixMode);
}

void MolDisplayWin::DrawLabel() {
	Frame * lFrame=MainData->cFrame;
	mpAtom * lAtoms = lFrame->Atoms;
	long NumAtoms = lFrame->NumAtoms;
	float AtomScale = Prefs->GetAtomScale();
	GLdouble BondSize = Prefs->GetQD3DBondWidth();
	float LabelSize = Prefs->GetAtomLabelSize();
	glfStringCentering(true);

	wxString atomLabel;
	long CurrentAtomType;
	CPoint3D origPt, transPt;
	if (!Prefs->DrawWireFrame() || Prefs->ColorBondHalves()) {
		glLoadName(MMP_ATOM);
		glPushName(0);
		for (long iatom=0; iatom<NumAtoms; iatom++) {
			if (lAtoms[iatom].GetInvisibility()) continue;	//Atom is invisible so skip

			atomLabel.Clear();

			CurrentAtomType = lAtoms[iatom].GetType() - 1;

			//!!! retrieve atom label
			if ( Prefs->ShowAtomicSymbolLabels() )
			Prefs->GetAtomLabel(CurrentAtomType, atomLabel);

			if (Prefs->ShowAtomNumberLabels() ) {
				wxString tmpStr;

				tmpStr.Printf(wxT("%ld"), iatom+1);
				atomLabel.Append(tmpStr);
			}

			float radius;
			if (!Prefs->DrawWireFrame()) radius = AtomScale*Prefs->GetAtomSize(CurrentAtomType);
			else radius = BondSize;

			if (radius<0.01) continue;	//skip really small spheres

			RGBColor * AtomColor = Prefs->GetAtomColorLoc(CurrentAtomType);
			float red, green, blue;
			red = AtomColor->red/65536.0;
			green = AtomColor->green/65536.0;
			blue = AtomColor->blue/65536.0;

			// Find atom's position in world coordinates.  Move there for
			// drawing label screen-aligned.
			origPt = lAtoms[iatom].Position;
			origPt -= MainData->Centroid;
			Rotate3DPt(MainData->TotalRotation, origPt, &transPt);

			glPushMatrix();
			glTranslatef(transPt.x, transPt.y, transPt.z+(radius+0.01));

			glColor3f(1-red, 1-green, 1-blue);
			glScalef((0.1+0.08*radius)*LabelSize, (0.1+0.08*radius)*LabelSize, 1);
			glLoadName(iatom+1);
			glfDrawSolidString(atomLabel.mb_str(wxConvUTF8));
			glPopMatrix();
		}
		glPopName();
	}
	glfStringCentering(false);
}

void MolDisplayWin::SortTransparentTriangles(void) {
	CPoint3D tempV;
	for (int i=0; i<triangleCount; i++) {
		Rotate3DPt(MainData->TotalRotation, transpTriList[i].v1,
			&(tempV));
		transpSortVertex[i] = tempV.z;	//We only need to save the z coordinate to sort
	}
	bool done = false;
	long maxcount = triangleCount-1;
	while (!done) {
		done = true;
		for (int i=0; i<maxcount; i++) {
			long tempi = transpIndex[i];
			long tempj = transpIndex[i+1];
			if (transpSortVertex[tempi] > transpSortVertex[tempj]) {
				transpIndex[i] = tempj;
				transpIndex[i+1] = tempi;
				done = false;
			}
		}
		maxcount--;
	}
}

void MolDisplayWin::DrawTransparentTriangles(void) {
	glBegin(GL_TRIANGLES);
	for (long itri=0; itri<triangleCount; itri++) {
		long itriIndex = transpIndex[itri];

		glColor4f(transpTriList[itriIndex].r1, transpTriList[itriIndex].g1,
				  transpTriList[itriIndex].b1, transpTriList[itriIndex].a1);
		glNormal3f(transpTriList[itriIndex].n1.x, 
				   transpTriList[itriIndex].n1.y, transpTriList[itriIndex].n1.z);
		glVertex3d(transpTriList[itriIndex].v1.x, transpTriList[itriIndex].v1.y,
			transpTriList[itriIndex].v1.z);
		
		glColor4f(transpTriList[itriIndex].r2, transpTriList[itriIndex].g2,
				  transpTriList[itriIndex].b2, transpTriList[itriIndex].a2);
		glNormal3f(transpTriList[itriIndex].n2.x, 
				   transpTriList[itriIndex].n2.y, transpTriList[itriIndex].n2.z);
		glVertex3d(transpTriList[itriIndex].v2.x, transpTriList[itriIndex].v2.y,
				   transpTriList[itriIndex].v2.z);

		glColor4f(transpTriList[itriIndex].r3, transpTriList[itriIndex].g3,
				  transpTriList[itriIndex].b3, transpTriList[itriIndex].a3);
		glNormal3f(transpTriList[itriIndex].n3.x, 
				   transpTriList[itriIndex].n3.y, transpTriList[itriIndex].n3.z);
		glVertex3d(transpTriList[itriIndex].v3.x, transpTriList[itriIndex].v3.y,
				   transpTriList[itriIndex].v3.z);
	}
	glEnd();	//End of triangle creation
}

void MolDisplayWin::DrawMoleculeCoreGL(void) {

	GLUquadric *core_obj;
	core_obj = gluNewQuadric();

	// gluQuadricDrawStyle(qobj, GLU_FILL); //or GLU_LINE
	/* gluQuadricOrientation(core_obj, GLU_OUTSIDE); */
	/* gluQuadricNormals(core_obj, GLU_SMOOTH); //GLU_FLAT GLU_NONE */

	Frame *lFrame = MainData->cFrame;
	mpAtom *lAtoms = lFrame->Atoms;
	Bond *lBonds = lFrame->Bonds;
	long NumAtoms = lFrame->NumAtoms;
	long NumBonds = lFrame->NumBonds;
	float AtomScale = Prefs->GetAtomScale();
	float Quality = Prefs->GetQD3DAtomQuality();
	// For really large systems the size of the individual atoms and bonds gets small so there doesn't
	// seem to be a need for the quality to remain high. This is an attempt to silently degrade the display quality
	// to improve performance.
	// Could disable the reduction when printing if this ends up causing a noticable issue
	int QualReduce = MainData->WindowSize;
	QualReduce /= 15;	//truncate the division
	Quality -= QualReduce;
	Quality = MAX(Quality, 2);	//Don't go below 2

	bool draw_subdued;
	glEnable(GL_RESCALE_NORMAL);
	if (!Prefs->DrawWireFrame()) {
		glLoadName(MMP_ATOM);
		glPushName(0);
		for (long iatom=0; iatom<NumAtoms; iatom++) {

			if (lAtoms[iatom].GetInvisibility()) continue;	//Atom is invisible so skip
			if (Prefs->ShowEFPWireFrame()&&lAtoms[iatom].IsEffectiveFragment()) continue;
			long curAtomType = lAtoms[iatom].GetType() - 1;

			float radius = AtomScale * Prefs->GetAtomSize(curAtomType);
			if (radius < 0.01) continue;	//skip really small spheres

			/* glPushMatrix(); // atom center */
			/* glCallList(sphere_list); */
			/* glPopMatrix(); // atom center */

			glPushMatrix(); // molecule origin
			glTranslatef(lAtoms[iatom].Position.x, 
						 lAtoms[iatom].Position.y,
						 lAtoms[iatom].Position.z);

			if (InSymmetryEditMode() && !lAtoms[iatom].IsSymmetryUnique()) {
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, d_specular);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, d_shininess);
				glLoadName(0);
				glColor3fv(fg_color);
				glEnable(GL_POLYGON_STIPPLE);
				glPolygonStipple(stippleMask);
				gluSphere(core_obj, radius*1.01, (long)(1.5*Quality), (long)(Quality));
				glDisable(GL_POLYGON_STIPPLE);
				glPopMatrix(); // molecule origin
				continue;
			}

			glLoadName(iatom + 1);
			draw_subdued = mHighliteState && !lAtoms[iatom].GetSelectState();
			  
			if (draw_subdued) {
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, d_specular);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, d_shininess);
			} else {
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, l_specular);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, l_shininess);
			}

			if (Prefs->Show2DPattern()) {
				short patternindex = Prefs->GetAtomPattern(lAtoms[iatom].GetType()-1);
				//The 0th pattern is assumed to be solid so no need to draw
				if ((patternindex>0)&&(patternindex<kNumPatterns)) {
					glColor3f(0.0f, 0.0f, 0.0f);
					glEnable(GL_POLYGON_STIPPLE);
					glPolygonStipple(atomMaskPatterns[patternindex]);
					glPushMatrix(); // atom center
					glScalef(radius, radius, radius);
					glCallList(sphere_list);
					glPopMatrix(); // atom center
					glDisable(GL_POLYGON_STIPPLE);
				}
			}

			if (draw_subdued) {
				glColor3f(0.0f,0.0f,0.0f);
				glEnable(GL_POLYGON_STIPPLE);
				glPolygonStipple(stippleMask);
				glPushMatrix(); // atom center
				glScalef(radius * 1.01f, radius * 1.01f, radius * 1.01f);
				glCallList(sphere_list);
				glPopMatrix(); // atom center
				glDisable(GL_POLYGON_STIPPLE);
			}

			Prefs->ChangeColorAtomColor(curAtomType+1);
	  
			if (draw_subdued) {
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, d_specular);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, d_shininess);
			} else {
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, l_specular);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, l_shininess);
			}

			glPushMatrix(); // atom center
			glScalef(radius, radius, radius);
			glCallList(sphere_list);
			glPopMatrix(); // atom center

			if (InEditMode() && show_bond_sites) {
				glPushMatrix();
				DrawBondingSites(iatom, radius, core_obj);
				glPopMatrix();
			}

			glPopMatrix(); // molecular origin
		}
		glPopName();
	}

	GLdouble modelview[16];
	GLdouble proj[16];
	GLint viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetIntegerv(GL_VIEWPORT, viewport);

	// bonds as cylinders
	// In wireframe mode with bonds colored by atom color we simply scink the
	// atom radius to the bond size and get a nice rounded end cap. If bonds
	// are not colored by atom color then the sphere is skipped and a simple
	// disk closes off the cylinder
	glLoadName(MMP_BOND);
	glPushName(0);
	for (long ibond = 0; ibond < NumBonds; ibond++) {

		if (!InSymmetryEditMode() ||
			(lAtoms[lBonds[ibond].Atom1].IsSymmetryUnique() &&
			 lAtoms[lBonds[ibond].Atom2].IsSymmetryUnique())) {
			glLoadName(ibond + 1);
		} else {
			glLoadName(0);
		}

		if (mHighliteState && !lBonds[ibond].GetSelectState()) {
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, d_specular);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, d_shininess);
		} else {
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, l_specular);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, l_shininess);
		}

		DrawBond(lBonds[ibond], lAtoms[lBonds[ibond].Atom1],
				 lAtoms[lBonds[ibond].Atom2], *Prefs, core_obj,
				 modelview, proj, viewport, sphere_list,
				 mHighliteState, InSymmetryEditMode());

	}
	glPopName();
	glDisable(GL_RESCALE_NORMAL);

	glLoadName(0);  //only atoms and bonds are selectable
	                //so give a NULL name value to the rest of the geometries

	if (MainData->GetDrawMode() && lFrame->Vibs) { //Add the current normal mode, if active
		float VectorScale = Prefs->GetVectorScale();

		long cmode = (lFrame->NumAtoms)*(lFrame->Vibs->CurrentMode);

		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, l_specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30.0f);
		Prefs->ChangeColorVectorColor();

		CPoint3D NormStart = CPoint3D(0.0f, 0.0f, 1.0f);
		for (long iatom=0; iatom<NumAtoms; iatom++) {

			if (lAtoms[iatom].GetInvisibility()) continue;	//Atom is invisible so skip
			CPoint3D NMode = (lFrame->Vibs->NormMode[iatom + cmode]);
			NMode *= VectorScale;
			float length = NMode.Magnitude();
			if (length > 0.1) {
				CPoint3D NModeVector;
				Matrix4D rotMat;
				NModeVector.x = NMode.x/length;
				NModeVector.y = NMode.y/length;
				NModeVector.z = NMode.z/length;
				float VectorWidth = (0.03 + 0.005*length);
				//Set up vectors for the shaft of the normal mode
				CPoint3D VStart;
				long curAtomType = lAtoms[iatom].Type - 1;

				float radius;
				if (Prefs->DrawWireFrame()) radius = 0.0;
				else radius = AtomScale*Prefs->GetAtomSize(curAtomType);
				VStart.x = lAtoms[iatom].Position.x + NModeVector.x*0.95*radius;
				VStart.y = lAtoms[iatom].Position.y + NModeVector.y*0.95*radius;
				VStart.z = lAtoms[iatom].Position.z + NModeVector.z*0.95*radius;
				float HeadRadius = 2 * VectorWidth;
				if (2*HeadRadius > length) HeadRadius = length/2.0;
				// float HeadRatio = (length-HeadRadius)/length;
				GLfloat ShaftLength = length - HeadRadius;

				SetRotationMatrix(rotMat, &NormStart, &NModeVector);
				rotMat[3][0] = VStart.x;
				rotMat[3][1] = VStart.y;
				rotMat[3][2] = VStart.z;
				glPushMatrix();
				glMultMatrixf((const GLfloat *) &rotMat);

				gluCylinder(core_obj, VectorWidth, VectorWidth, ShaftLength, (long)(Quality), (long)(0.5*Quality));
				glPopMatrix();
				rotMat[3][0] = VStart.x + NModeVector.x * ShaftLength;
				rotMat[3][1] = VStart.y + NModeVector.y * ShaftLength;
				rotMat[3][2] = VStart.z + NModeVector.z * ShaftLength;
				glPushMatrix();
				glMultMatrixf((const GLfloat *) &rotMat);
				gluDisk(core_obj, 0.0, 2*VectorWidth, (long)(Quality), 2);
				gluCylinder(core_obj, 2*VectorWidth, 0.0, HeadRadius, (long)(Quality), 3);
				glPopMatrix();
			}
		}
	}
	gluDeleteQuadric(core_obj);	//finally delete the quadric object

}

void WinPrefs::ChangeColorBondColor(long order) const {
	float red, green, blue;
	red = BondColors[order].red/65536.0;
	green = BondColors[order].green/65536.0;
	blue = BondColors[order].blue/65536.0;
	glColor3f(red, green, blue);
}

void WinPrefs::ChangeColorAtomColor(long atomtype, float alpha) const {
	float red, green, blue;
	red = AtomColors[atomtype-1].red/65536.0;
	green = AtomColors[atomtype-1].green/65536.0;
	blue = AtomColors[atomtype-1].blue/65536.0;
	glColor4f(red, green, blue, alpha);
}

void WinPrefs::ChangeColorVectorColor(void) const {
	float red, green, blue;
	red = VectorColor.red/65536.0;
	green = VectorColor.green/65536.0;
	blue = VectorColor.blue/65536.0;
	glColor3f(red, green, blue);
}

void WinPrefs::GetAtomColorInverse(long atomtype, float rgb[3]) const {
	rgb[0] = 1.0f - AtomColors[atomtype-1].red / 65536.0f;
	rgb[1] = 1.0f - AtomColors[atomtype-1].green / 65536.0f;
	rgb[2] = 1.0f - AtomColors[atomtype-1].blue / 65536.0f;
}

void MolDisplayWin::DrawHydrogenBond(const Bond& bond, const mpAtom& atom1,
									 const mpAtom& atom2,
									 const WinPrefs& Prefs,
									 GLUquadric *quadric, GLuint sphere_list,
									 bool highlighting_on) {
	CPoint3D	v1, v2, offset;

	Prefs.ChangeColorBondColor(kHydrogenBond);
	GLdouble BondSize = Prefs.GetQD3DBondWidth() * 0.5;
	offset = atom2.Position - atom1.Position;
	
	//Plot as a series of spheres
	CPoint3D	NormalOffset, NormEnd, NormStart = CPoint3D(0.0f, 0.0f, 1.0f);
	Matrix4D	rotMat;
	
	float length = offset.Magnitude();
	if (length>0.00001) {
		NormalOffset.x = offset.x/length;
		NormalOffset.y = offset.y/length;
		NormalOffset.z = offset.z/length;
	} else
		NormalOffset.x=NormalOffset.y=NormalOffset.z=0.0;
	NormEnd = v2;
	Normalize3D(&NormEnd);
	SetRotationMatrix(rotMat, &NormStart, &NormalOffset);
	rotMat[3][0] = atom1.Position.x;
	rotMat[3][1] = atom1.Position.y;
	rotMat[3][2] = atom1.Position.z;
	
	glPushMatrix();
	glMultMatrixf((const GLfloat *) &rotMat);
	float pos=0.75*BondSize;
	glTranslatef(0.0, 0.0, pos);
	while (pos < length) {
		glPushMatrix();
		glScalef(BondSize, BondSize, BondSize);
		Prefs.ChangeColorBondColor(kHydrogenBond);
		glCallList(sphere_list);
		if (highlighting_on && !bond.GetSelectState()) {
			glColor3f(0.0f, 0.0f, 0.0f);
			glEnable(GL_POLYGON_STIPPLE);
			glPolygonStipple(stippleMask);

			glPushMatrix();
			glScalef(1.01f, 1.01f, 1.01f);
			glCallList(sphere_list);
			glPopMatrix();
			glDisable(GL_POLYGON_STIPPLE);
		}
		glPopMatrix();
		glTranslatef(0.0, 0.0, 2.5*BondSize);
		pos += 2.5*BondSize;
	}
	glPopMatrix();
}

void MolDisplayWin::AddAxisGL(void) {
	glEnable(GL_COLOR_MATERIAL);
	glColor3fv(fg_color);

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, l_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, l_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30.0f);

	long Quality = (long)(Prefs->GetQD3DAtomQuality());
	float VectorWidth = 0.02;

	CPoint3D vector = CPoint3D(1.0f, 0.0f, 0.0f);
	CPoint3D NormStart = CPoint3D(0.0f, 0.0f, 1.0f);
	
	// Z-axis
	glPushMatrix();
	glTranslatef(0.0, 0.0, -MainData->MaxSize);
	DrawArrow(2*MainData->MaxSize, VectorWidth, Quality);
	glTranslatef(0.0, 0.0, 2.0f * MainData->MaxSize);
	DrawSceneString(0.3f, 0.0f, 0.0f, 0.0f, wxT("z"));
	glPopMatrix();

	// X-axis
	Matrix4D	rotMat;
	SetRotationMatrix(rotMat, &NormStart, &vector);
	rotMat[3][0] = -MainData->MaxSize;
	glPushMatrix();
	glMultMatrixf((const GLfloat *) &rotMat);
	DrawArrow(2*MainData->MaxSize, VectorWidth, Quality);
	glTranslatef(0.0, 0.0, 2.0f * MainData->MaxSize);
	DrawSceneString(0.3f, 0.0f, 0.0f, 0.0f, wxT("x"));
	glPopMatrix();

	// Y-axis
	vector.x = 0.0;
	vector.y = 1.0;
	SetRotationMatrix(rotMat, &NormStart, &vector);
	rotMat[3][1] = -MainData->MaxSize;
	glPushMatrix();
	glMultMatrixf((const GLfloat *) &rotMat);
	DrawArrow(2*MainData->MaxSize, VectorWidth, Quality);
	glTranslatef(0.0, 0.0, 2.0f * MainData->MaxSize);
	DrawSceneString(0.3f, 0.0f, 0.0f, 0.0f, wxT("y"));
	glPopMatrix();
}

void MolDisplayWin::AddSymmetryOperators(void) {
	//Add planes, rotation axis's, etc as appropriate for the selected symmetry point group
	//This routine attempts to match the GAMESS assumptions on symmetry
	// z is the principle rotation axis (if any)
	// x is a perpendicular two-fold axis (if any),
	// xz is the sigma-v plane (if any), and
	// xy is the sigma-h plane (if any).
	
	if (!MainData->InputOptions) return;
	if (!MainData->InputOptions->Data) return;
	CPoint3D origin = CPoint3D(0.0f, 0.0f, 0.0f);
	CPoint3D p1 = CPoint3D(0.0f, 0.0f, 0.0f), p2 = CPoint3D(0.0f, 0.0f, 0.0f);
	switch (MainData->InputOptions->Data->GetPointGroup()) {
		case GAMESS_CS: //single sigma-h plane - XY plane
			origin.x = origin.y = - MainData->MaxSize;
			p1.x = MainData->MaxSize;
			p1.y = - p1.x;
			p2.x = -MainData->MaxSize;
			p2.y = -p2.x;
			DrawTranslucentPlane(origin, p1, p2);
			break;
		case GAMESS_CI:
			DrawInversionPoint();
			break;
		case GAMESS_CNH:
		{
			int order = MainData->InputOptions->Data->GetPointGroupOrder();
			if (order <= 2) order = 2;
			if ((order == 2)||(order == 4)||(order == 6)) DrawInversionPoint();
			origin.x = origin.y = - MainData->MaxSize;
			p1.x = MainData->MaxSize;
			p1.y = - p1.x;
			p2.x = -MainData->MaxSize;
			p2.y = -p2.x;
			DrawTranslucentPlane(origin, p1, p2);
			origin.x = origin.y = p1.x = p1.y = 0.0;
			origin.z = -MainData->MaxSize;
			p1.z = MainData->MaxSize;
			DrawRotationAxis(origin, p1, order);
		}
			break;
		case GAMESS_CNV:	//CN axis plus N sigma-v planes
		{
			int order = MainData->InputOptions->Data->GetPointGroupOrder();
			if (order <= 2) order = 2;
			for (int i=0; i<order; i++) {
				origin.x = - cos(kPi * i/order) * MainData->MaxSize;
				origin.y = - sin(kPi * i/order) * MainData->MaxSize;
				origin.z = - MainData->MaxSize;
				p1.x = - origin.x;
				p1.y = - origin.y;
				p1.z = origin.z;
				p2.x = origin.x;
				p2.y = origin.y;
				p2.z = - origin.z;
				DrawTranslucentPlane(origin, p1, p2);
			}
			origin.x = origin.y = p1.x = p1.y = 0.0;
		}
		//	break;	//let this roll through to pick up the axis
		case GAMESS_CN:
		{
			int order = MainData->InputOptions->Data->GetPointGroupOrder();
			if (order <= 2) order = 2;
			origin.z = -MainData->MaxSize;
			p1.z = MainData->MaxSize;
			DrawRotationAxis(origin, p1, order);
		}
			break;
		case GAMESS_S2N:
			break;
		case GAMESS_DND:
		{
			int order = MainData->InputOptions->Data->GetPointGroupOrder();
			if (order <= 2) order = 2;
			if ((order == 3)||(order == 5)) DrawInversionPoint();
			origin.x = origin.y = p1.x = p1.y = 0.0;
			origin.z = -MainData->MaxSize;
			p1.z = MainData->MaxSize;
			DrawRotationAxis(origin, p1, order);
			//Set of order C2 axis perpendicular to primary axis
			for (int i=0; i<order; i++) {
				origin.x = - cos(kPi * i/order) * MainData->MaxSize;
				origin.y = - sin(kPi * i/order) * MainData->MaxSize;
				origin.z = 0.0;
				p1.x = - origin.x;
				p1.y = - origin.y;
				p1.z = origin.z;
				DrawRotationAxis(origin, p1, 2);
			}
				//There are order count of planes bisecting the C2 axis
			for (int i=0; i<order; i++) {
				origin.x = - cos(kPi * i/order + kPi/(2*order)) * MainData->MaxSize;
				origin.y = - sin(kPi * i/order + kPi/(2*order)) * MainData->MaxSize;
				origin.z = -MainData->MaxSize;
				p1.x = - origin.x;
				p1.y = - origin.y;
				p1.z = origin.z;
				p2.x = origin.x;
				p2.y = origin.y;
				p2.z = - origin.z;
				DrawTranslucentPlane(origin, p1, p2);
			}
		}
			break;
		case GAMESS_DNH:
		{
			int order = MainData->InputOptions->Data->GetPointGroupOrder();
			if (order <= 2) order = 2;
			if ((order == 2)||(order == 4)||(order == 6)) DrawInversionPoint();
			origin.x = origin.y = - MainData->MaxSize;
			p1.x = MainData->MaxSize;
			p1.y = - p1.x;
			p2.x = -MainData->MaxSize;
			p2.y = -p2.x;
			DrawTranslucentPlane(origin, p1, p2);
		}
//			break;
		case GAMESS_DN:
		{
			int order = MainData->InputOptions->Data->GetPointGroupOrder();
			if (order <= 2) order = 2;
			origin.x = origin.y = p1.x = p1.y = 0.0;
			origin.z = -MainData->MaxSize;
			p1.z = MainData->MaxSize;
			DrawRotationAxis(origin, p1, order);
			//Set of order C2 axis perpendicular to primary axis
			for (int i=0; i<order; i++) {
				origin.x = - cos(kPi * i/order) * MainData->MaxSize;
				origin.y = - sin(kPi * i/order) * MainData->MaxSize;
				origin.z = 0.0;
				p1.x = - origin.x;
				p1.y = - origin.y;
				p1.z = origin.z;
				DrawRotationAxis(origin, p1, 2);
			}
		}
			break;
/*		case GAMESS_TD:
			break;
		case GAMESS_TH:
		{
			DrawInversionPoint();
		}
			break;
		case GAMESS_T:
			break;
		case GAMESS_OH:
		{
			DrawInversionPoint();
		}
			break;
		case GAMESS_O:
			break;
			*/
	}
}

long Surf1DBase::Draw3DGL(MoleculeData *MainData, WinPrefs *Prefs, myGLTriangle *, unsigned int shader_program) {

	if (Visible) {
		// Update the grid if needed, then display
		if (!Grid) Update(MainData);
	}

	glLoadName(MMP_1DSURFACE);
	glPushName(GetSurfaceID()+1);

	glPushName(1);

	glPushMatrix();
	glTranslatef(Start.x, Start.y, Start.z);
	glScalef(0.05f, 0.05f, 0.05f);
	glCallList(MainData->MolWin->GetSphereList());
	glPopMatrix();

	glLoadName(2);
	glPushMatrix();
	glTranslatef(End.x, End.y, End.z);
	glScalef(0.05f, 0.05f, 0.05f);
	glCallList(MainData->MolWin->GetSphereList());
	glPopMatrix();

	CPoint3D vec = End - Start;
	CPoint3D perp;
	OrthoVector(vec, perp);
	vec *= 1.0f / (NumGridPoints - 1);
	CPoint3D curr_pt = Start;
	CPoint3D next_pt;

	// Find a vector perpendicular to both view direction and the vector from
	// Start to End projected onto the screen plane.  The graph will be pushed
	// away along this perpendicular vector so that it can always be seen, no
	// matter the rotation.
	const Matrix4D& rot = MainData->GetRotationMatrix();
	CPoint3D normal(rot[0][2], rot[1][2], rot[2][2]);
	CPoint3D projected_start = Start;
	CPoint3D projected_end = End;
	ProjectToPlane(normal, Start, projected_start);
	ProjectToPlane(normal, Start, projected_end);
	CPoint3D projected_vec = projected_end - projected_start;
	CrossProduct3D(&projected_vec, &normal, &perp);
	Normalize3D(&perp);

	glLoadName(0);
	float range_factor;
	if (GridMax > MaxContourValue) {
		range_factor = 1.0f / (MaxContourValue - GridMin);
	} else {
 		range_factor = 1.0f / (GridMax - GridMin);
	}

	float red;
	float curr_val, next_val;
	curr_val = Grid[0] > MaxContourValue ? MaxContourValue : Grid[0];

#if 1
	GLUquadricObj *quadric = gluNewQuadric();
	CreateCylinderFromLine(quadric, Start, End, 0.02f, 5, 5, false);

	for (int i = 0; i < NumGridPoints - 1; ++i) {
		next_val = Grid[i + 1] > MaxContourValue ? MaxContourValue : Grid[i + 1];
		next_pt = curr_pt + vec;
		red = (curr_val - GridMin) * range_factor; 
		glColor3f(red, 0.0f, 1.0f - red);
		CreateCylinderFromLine(quadric, curr_pt + perp * (curr_val * Scale),
							   next_pt + perp * (next_val * Scale),
							   0.05f, 5, 5, true);
		curr_val = next_val;
		curr_pt = next_pt;
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	if (fabs(Grid[0]) > 1e-6f) {
		glPushMatrix();
		glTranslatef(next_pt.x, next_pt.y, next_pt.z);
		glScalef(0.05f, 0.05f, 0.05f);
		glCallList(MainData->MolWin->GetSphereList());
		glPopMatrix();

		curr_val = Scale * (Grid[0] > MaxContourValue ? MaxContourValue : Grid[0]);
		CreateCylinderFromLine(quadric, Start, Start + perp * curr_val,
							   0.02f, 5, 5, true);
	}

	if (fabs(Grid[NumGridPoints - 1]) > 1e-6f) {
		curr_val = Scale * (Grid[NumGridPoints - 1] > MaxContourValue ? MaxContourValue : Grid[NumGridPoints - 1]);
		CreateCylinderFromLine(quadric, End, End + perp * curr_val,
							   0.02f, 5, 5, true);
	}

	gluDeleteQuadric(quadric);
#else
	if (GLEW_VERSION_2_0 && Prefs->GetShaderMode()) {
		glUseProgram(0);
	}

	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
	for (int i = 0; i < NumGridPoints - 1; ++i) {
		next_val = Grid[i + 1] > MaxContourValue ? MaxContourValue : Grid[i + 1];
		next_pt = curr_pt + vec;
		red = (curr_val - GridMin) * range_factor; 
		glColor3f(red, 0.0f, 1.0f - red);
		glVertex3f(curr_pt.x - perp.x * 0.045f,
				   curr_pt.y - perp.y * 0.045f,
				   curr_pt.z - perp.z * 0.045f);
		glVertex3f(curr_pt.x + perp.x * 0.045f,
				   curr_pt.y + perp.y * 0.045f,
				   curr_pt.z + perp.z * 0.045f);
		red = (Grid[i + 1] - GridMin) * range_factor; 
		glColor3f(red, 0.0f, 1.0f - red);
		glVertex3f(next_pt.x + perp.x * 0.045f,
				   next_pt.y + perp.y * 0.045f,
				   next_pt.z + perp.z * 0.045f);
		glVertex3f(next_pt.x - perp.x * 0.045f,
				   next_pt.y - perp.y * 0.045f,
				   next_pt.z - perp.z * 0.045f);
		curr_pt = next_pt;
		curr_val = next_val;
	}
	glEnd();
	glEnable(GL_LIGHTING);

	if (GLEW_VERSION_2_0 && Prefs->GetShaderMode()) {
		glUseProgram(shader_program);
	}
#endif

	glPopName();
	glPopName();
	return 0;
}

long Surf2DBase::Draw3DGL(MoleculeData * MainData, WinPrefs * Prefs, myGLTriangle *, unsigned int shader_program) {
	if (Visible) {
			//Update the grid if needed, then contour and display
		if (!Grid) Update(MainData);
		if (Grid) Contour2DGrid3DGL(MainData, Prefs);
		if (ShowPlottingPlane()) DrawTranslucentPlane(Origin, Origin+XInc*(NumGridPoints-1), Origin+YInc*(NumGridPoints-1));
	}
	return 0;
}

void Surf2DBase::Contour2DGrid3DGL(MoleculeData * , WinPrefs * Prefs) 
{
		//Scan the Grid producing the contours
	float	TestPoint1, TestPoint2, TestPoint3, TestPoint4, XGridValue, YGridValue, ZGridValue;
	CPoint3D	Contour[4];
	Boolean		HasPoint[4];

	long NumPoints = NumGridPoints;
	float * lGrid = Grid;
	
	CPoint3D	XGridMin, XGridInc, YGridInc;
	XGridMin = Origin;
//	XGridMin *= kBohr2AngConversion;
	XGridInc = XInc;
//	XGridInc *= kBohr2AngConversion;
	YGridInc = YInc;
//	YGridInc *= kBohr2AngConversion;

	float ContourValueInc = MaxContourValue/(NumContours+1);
	long NumPosContours = (long)(fabs(GridMax)/ContourValueInc) + 1;
	long NumNegContours = (long)(fabs(GridMin)/ContourValueInc) + 1;
	
	CPoint3D	lineStart, lineEnd;
	float ContourValue = 0.0;
	float lineWidth = Prefs->GetQD3DLineWidth();
	GLUquadricObj * qobj = NULL;
		Boolean UseLines = true;
	if (lineWidth > 0.0001) {
		UseLines = false; 
		qobj = gluNewQuadric();
		if (!qobj) throw std::bad_alloc();
	}
	long n;
	Boolean Dash = GetDashLine();
	// GLenum error = glGetError(); 
	if (UseLines) {
		glDisable(GL_LIGHTING);
		glLineWidth(1);
		glBegin(GL_LINES);
		// error = glGetError();	//maybe clear off odd errors... 
	}
	glColor3f(0.65, 0.65, 0.65);	//Setup for the gray zero contour color
	
		//Go up to NumContours+1 to allow for the zero contour
	for (long iContour=0; iContour<=NumContours; iContour++) {
		for (int pass=0; pass<2; pass++) {
			if (iContour==0) {	//0 value contour
				pass++;	//only need one 0 contour!
					//Plot zero value contour only if requested
				if (!(SurfOptions&1)) continue;
			} else {
				ContourValue *= -1.0;
				if (pass==0) {
					if (!ContourBothPosNeg()) continue;
					if (iContour > NumNegContours) continue;
					glColor3f((float)NegColor.red/65536, (float)NegColor.green/65536, (float)NegColor.blue/65536);
				} else {
					if (iContour > NumPosContours) continue;
					glColor3f((float)PosColor.red/65536, (float)PosColor.green/65536, (float)PosColor.blue/65536);
				}
			}
			n=NumGridPoints;
			for (long i=1; i<NumPoints; i++) {
				XGridValue = XGridMin.x + i*XGridInc.x;
				YGridValue = XGridMin.y + i*XGridInc.y;
				ZGridValue = XGridMin.z + i*XGridInc.z;
				for (long j=1; j<NumPoints; j++) {
					XGridValue += YGridInc.x;
					YGridValue += YGridInc.y;
					ZGridValue += YGridInc.z;
					n++;
					for (int i=0; i<4; i++) HasPoint[i]=false;
					
					TestPoint1 = lGrid[n]-ContourValue;
					TestPoint2 = lGrid[n-1]-ContourValue;
					TestPoint3 = lGrid[n-NumPoints]-ContourValue;
					TestPoint4 = lGrid[n-1-NumPoints]-ContourValue;

					if ((TestPoint1*TestPoint2)<0.0) {
						HasPoint[0]=true;
						Contour[0].x = XGridValue - YGridInc.x*(TestPoint1/(TestPoint1-TestPoint2));
						Contour[0].y = YGridValue - YGridInc.y*(TestPoint1/(TestPoint1-TestPoint2));
						Contour[0].z = ZGridValue - YGridInc.z*(TestPoint1/(TestPoint1-TestPoint2));
					}
					if ((TestPoint1*TestPoint3)<0.0) {
						HasPoint[1]=true;
						Contour[1].x = XGridValue - XGridInc.x*(TestPoint1/(TestPoint1-TestPoint3));
						Contour[1].y = YGridValue - XGridInc.y*(TestPoint1/(TestPoint1-TestPoint3));
						Contour[1].z = ZGridValue - XGridInc.z*(TestPoint1/(TestPoint1-TestPoint3));
					}
					if ((TestPoint2*TestPoint4)<0.0) {
						HasPoint[2]=true;
						Contour[2].x = XGridValue-YGridInc.x - XGridInc.x*(TestPoint2/(TestPoint2-TestPoint4));
						Contour[2].y = YGridValue-YGridInc.y - XGridInc.y*(TestPoint2/(TestPoint2-TestPoint4));
						Contour[2].z = ZGridValue-YGridInc.z - XGridInc.z*(TestPoint2/(TestPoint2-TestPoint4));
					}
					if ((TestPoint3*TestPoint4)<0.0) {
						HasPoint[3]=true;
						Contour[3].x = XGridValue-XGridInc.x - YGridInc.x*(TestPoint3/(TestPoint3-TestPoint4));
						Contour[3].y = YGridValue-XGridInc.y - YGridInc.y*(TestPoint3/(TestPoint3-TestPoint4));
						Contour[3].z = ZGridValue-XGridInc.z - YGridInc.z*(TestPoint3/(TestPoint3-TestPoint4));
					}
					
					if (HasPoint[0]) {
						lineStart = Contour[0];
						if (HasPoint[1] || HasPoint[2]) {
							if (HasPoint[1]) {
								lineEnd = Contour[1];
								if ((pass==0)&&Dash) {
									lineEnd.x = lineStart.x + (lineEnd.x-lineStart.x)/2.0;
									lineEnd.y = lineStart.y + (lineEnd.y-lineStart.y)/2.0;
									lineEnd.z = lineStart.z + (lineEnd.z-lineStart.z)/2.0;
								}
								if (UseLines) {
									glVertex3d(lineStart.x, lineStart.y, lineStart.z);
									glVertex3d(lineEnd.x, lineEnd.y, lineEnd.z);
								} else {
									CreateCylinderFromLine(qobj, lineStart, lineEnd, lineWidth);
								}
							}
							if (HasPoint[2]) {
								lineEnd = Contour[2];
								if ((pass==0)&&Dash) {
									lineEnd.x = lineStart.x + (lineEnd.x-lineStart.x)/2.0;
									lineEnd.y = lineStart.y + (lineEnd.y-lineStart.y)/2.0;
									lineEnd.z = lineStart.z + (lineEnd.z-lineStart.z)/2.0;
								}
								if (UseLines) {
									glVertex3d(lineStart.x, lineStart.y, lineStart.z);
									glVertex3d(lineEnd.x, lineEnd.y, lineEnd.z);
								} else {
									CreateCylinderFromLine(qobj, lineStart, lineEnd, lineWidth);
								}
							}
						} else if (HasPoint[3]) {
							lineEnd = Contour[3];
							if ((pass==0)&&Dash) {
								lineEnd.x = lineStart.x + (lineEnd.x-lineStart.x)/2.0;
								lineEnd.y = lineStart.y + (lineEnd.y-lineStart.y)/2.0;
								lineEnd.z = lineStart.z + (lineEnd.z-lineStart.z)/2.0;
							}
							if (UseLines) {
								glVertex3d(lineStart.x, lineStart.y, lineStart.z);
								glVertex3d(lineEnd.x, lineEnd.y, lineEnd.z);
							} else {
								CreateCylinderFromLine(qobj, lineStart, lineEnd, lineWidth);
							}
						}
					}
					if (HasPoint[1]) {
						if (HasPoint[3]) {
							lineStart = Contour[1];
							lineEnd = Contour[3];
							if ((pass==0)&&Dash) {
								lineEnd.x = lineStart.x + (lineEnd.x-lineStart.x)/2.0;
								lineEnd.y = lineStart.y + (lineEnd.y-lineStart.y)/2.0;
								lineEnd.z = lineStart.z + (lineEnd.z-lineStart.z)/2.0;
							}
							if (UseLines) {
								glVertex3d(lineStart.x, lineStart.y, lineStart.z);
								glVertex3d(lineEnd.x, lineEnd.y, lineEnd.z);
							} else {
								CreateCylinderFromLine(qobj, lineStart, lineEnd, lineWidth);
							}
						} else if (HasPoint[2] && !HasPoint[0]) {
							lineStart = Contour[1];
							lineEnd = Contour[2];
							if ((pass==0)&&Dash) {
								lineEnd.x = lineStart.x + (lineEnd.x-lineStart.x)/2.0;
								lineEnd.y = lineStart.y + (lineEnd.y-lineStart.y)/2.0;
								lineEnd.z = lineStart.z + (lineEnd.z-lineStart.z)/2.0;
							}
							if (UseLines) {
								glVertex3d(lineStart.x, lineStart.y, lineStart.z);
								glVertex3d(lineEnd.x, lineEnd.y, lineEnd.z);
							} else {
								CreateCylinderFromLine(qobj, lineStart, lineEnd, lineWidth);
							}
						}
					}
					if (HasPoint[2]&&HasPoint[3]) {
						lineStart = Contour[2];
						lineEnd = Contour[3];
						if ((pass==0)&&Dash) {
							lineEnd.x = lineStart.x + (lineEnd.x-lineStart.x)/2.0;
							lineEnd.y = lineStart.y + (lineEnd.y-lineStart.y)/2.0;
							lineEnd.z = lineStart.z + (lineEnd.z-lineStart.z)/2.0;
						}
						if (UseLines) {
							glVertex3d(lineStart.x, lineStart.y, lineStart.z);
							glVertex3d(lineEnd.x, lineEnd.y, lineEnd.z);
						} else {
							CreateCylinderFromLine(qobj, lineStart, lineEnd, lineWidth);
						}
					}
				}
				n++;
			}
		}
		ContourValue += ContourValueInc;
	}
	if (UseLines) {
		glEnd();
		// error = glGetError();	//This is here to clear off odd errors. 
		glEnable(GL_LIGHTING);
	} else {
		if (qobj) gluDeleteQuadric(qobj);	//finally delete the quadric object
	}
}

long General3DSurface::getTriangleCount(void) const {
	long result = NumPosContourTriangles;
	if (Mode & 4) result += NumNegContourTriangles;
	return result;
}

long General3DSurface::Draw3DGL(MoleculeData * MainData, WinPrefs * Prefs, myGLTriangle * transpTri, unsigned int shader_program) {
	long result=0;
	if (Visible) {
		if (ContourHndl && VertexList) {
			if (SolidSurface()) {
				if (UseSurfaceNormals()&&SurfaceNormals) {
					result = CreateSolidSurface(ContourHndl, SurfaceNormals, VertexList,
									NumPosContourTriangles, &PosColor, List, &NegColor, MaxMEPValue, MainData, transpTri);
					if ((Mode & 4)&&(NumNegContourTriangles > 0))
						result += CreateSolidSurface(ContourHndl,
													 SurfaceNormals, &(VertexList[3*NumPosContourTriangles]),
													 NumNegContourTriangles, &NegColor, List, &PosColor, MaxMEPValue, MainData, &(transpTri[result]));
				} else {
					result = CreateSolidSurface(ContourHndl, NULL, VertexList,
												NumPosContourTriangles, &PosColor, List, &NegColor, MaxMEPValue, MainData, transpTri);
					if ((Mode & 4)&&(NumNegContourTriangles > 0))
						result += CreateSolidSurface(ContourHndl,
													 NULL, &(VertexList[3*NumPosContourTriangles]),
													 NumNegContourTriangles, &NegColor, List, &PosColor, MaxMEPValue, MainData, &(transpTri[result]));
				}
			} else if (WireFrameSurface()) {
				CreateWireSurface(ContourHndl, NULL, VertexList,
								  NumPosContourTriangles, &PosColor, List, &NegColor, MaxMEPValue, MainData, Prefs);
				if (ContourBothPosNeg()&&(NumNegContourTriangles > 0))
					CreateWireSurface(ContourHndl, NULL, 
									  &(VertexList[3*NumPosContourTriangles]),
									  NumNegContourTriangles, &NegColor, List, &PosColor, MaxMEPValue, MainData, Prefs);
			}
		}
	}
	return result;
}
long TEDensity3DSurface::Draw3DGL(MoleculeData * MainData, WinPrefs * Prefs, myGLTriangle * transpTri, unsigned int shader_program) {
	long result = 0;
	if (Visible) {
		if (ContourHndl && VertexList) {
			if (SolidSurface()) {
				if ((UseSurfaceNormals())&&SurfaceNormals) {
					result = CreateSolidSurface(ContourHndl, SurfaceNormals, VertexList,
												NumPosContourTriangles,
												&PosColor, List, &NegColor, MaxMEPValue, MainData, transpTri);
				} else {
					result = CreateSolidSurface(ContourHndl, NULL, VertexList,
												NumPosContourTriangles,
												&PosColor, List, &NegColor, MaxMEPValue, MainData, transpTri);
				}
			} else if (WireFrameSurface()) {
				CreateWireSurface(ContourHndl, NULL, VertexList,
								  NumPosContourTriangles,
								  &PosColor, List, &NegColor, MaxMEPValue, MainData, Prefs);
			}
		}
	}
	return result;
}
long Orb3DSurface::Draw3DGL(MoleculeData * MainData, WinPrefs * Prefs, myGLTriangle * transpTri, unsigned int shader_program) {
	long result=0;
	if (Visible && (PlotOrb>=0)) {
		if (ContourHndl && VertexList) {
			if (SolidSurface()) {
				if ((UseSurfaceNormals())&&SurfaceNormals) {
					result = CreateSolidSurface(ContourHndl,
												SurfaceNormals, VertexList, NumPosContourTriangles,
												&PosColor, NULL, NULL, 1.0, MainData, transpTri);
					result += CreateSolidSurface(ContourHndl,
												 SurfaceNormals,
												 &(VertexList[3*NumPosContourTriangles]),
												 NumNegContourTriangles, &NegColor,
												 NULL, NULL, 1.0, MainData, &(transpTri[result]));
				} else {
					result = CreateSolidSurface(ContourHndl, NULL, VertexList,
												NumPosContourTriangles,
												&PosColor, NULL, NULL, 1.0, MainData, transpTri);
					result += CreateSolidSurface(ContourHndl,
												 NULL, &(VertexList[3*NumPosContourTriangles]),
												 NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData, &(transpTri[result]));
				}
			} else if (WireFrameSurface()) {
				CreateWireSurface(ContourHndl, NULL, VertexList, NumPosContourTriangles,
								  &PosColor, NULL, NULL, 1.0, MainData, Prefs);
				CreateWireSurface(ContourHndl, NULL, 
								  &(VertexList[3*NumPosContourTriangles]),
								  NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData, Prefs);
			}
		}
	}
	return result;
}
long MEP3DSurface::Draw3DGL(MoleculeData * MainData, WinPrefs * Prefs, myGLTriangle * transpTri, unsigned int shader_program) {
	long result=0;
	if (Visible) {
		if (ContourHndl && VertexList) {
			if (SolidSurface()) {
				if ((UseSurfaceNormals())&&SurfaceNormals) {
					result = CreateSolidSurface(ContourHndl, SurfaceNormals,
												VertexList, NumPosContourTriangles,
												&PosColor, NULL, NULL, 1.0, MainData, transpTri);
					result += CreateSolidSurface(ContourHndl, SurfaceNormals,
												 &(VertexList[3*NumPosContourTriangles]), 
												 NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData, &(transpTri[result]));
				} else {
					result = CreateSolidSurface(ContourHndl, NULL, VertexList, NumPosContourTriangles,
												&PosColor, NULL, NULL, 1.0, MainData, transpTri);
					result += CreateSolidSurface(ContourHndl,
												 NULL, &(VertexList[3*NumPosContourTriangles]),
												 NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData, &(transpTri[result]));
				}
			} else if (WireFrameSurface()) {
				CreateWireSurface(ContourHndl, NULL, VertexList, NumPosContourTriangles,
								  &PosColor, NULL, NULL, 1.0, MainData, Prefs);
				CreateWireSurface(ContourHndl, NULL, 
								  &(VertexList[3*NumPosContourTriangles]),
								  NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData, Prefs);
			}
		}
	}
	return result;
}

void Surf3DBase::CreateWireSurface(CPoint3D * Vertices, CPoint3D * Normals, long * VertexList,
		long NumTriangles, RGBColor * SurfaceColor, float * SurfaceValue,
		RGBColor * NColor, float MaxSurfaceValue, MoleculeData * MainData, const WinPrefs * Prefs)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_LIGHTING);
	glLineWidth(Prefs->GetLineWidth());
	CreateSolidSurface(Vertices, Normals, VertexList,
		NumTriangles, SurfaceColor, SurfaceValue, NColor, MaxSurfaceValue, MainData, NULL);
	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
/*	glDisable(GL_LIGHTING);
	glLineWidth(1);
	//This code works only slightly better than the above code on the Intel Macs with ATI hardware.
	//It seems to miss some lines?
	CreateWireFrameSurfaceWithLines(Vertices, VertexList,
					   NumTriangles, SurfaceColor, SurfaceValue, NColor, MaxSurfaceValue, MainData);	
//	glEnable(GL_LIGHTING);
	*/
}

void Surf3DBase::SetSurfaceColor(const float & surfaceValue, const RGBColor * pColor, const RGBColor * nColor,
								float & red, float & green, float & blue) const {
	float	localVal = surfaceValue;
	if (UseRGBColoration()) {
		if (InvertRGBColoration()) localVal *= -1.0;
		if (localVal < 0.0) {
			if (localVal < -1.0) localVal = -1.0;
			red = 0.0;
			green = localVal + 1.0;
			blue = -localVal;
		} else {
			if (localVal > 1.0) localVal = 1.0;
			red = localVal;
			green = 1 - localVal;
			blue = 0.0;
		}
	} else {	//color based on +/- color intensity
		if (localVal>=0) {
			if (localVal > 1.0) localVal = 1.0;
			if (pColor) {
				red = ((float) pColor->red/65536)*localVal;
				green = ((float) pColor->green/65536)*localVal;
				blue = ((float) pColor->blue/65536)*localVal;
			} else {
				red = ((float) PosColor.red/65536)*localVal;
				green = ((float) PosColor.green/65536)*localVal;
				blue = ((float) PosColor.blue/65536)*localVal;
			}
		} else {
			localVal *= -1.0;
			if (localVal > 1.0) localVal = 1.0;
			if (nColor) {
				red = ((float) nColor->red/65536)*localVal;
				green = ((float) nColor->green/65536)*localVal;
				blue = ((float) nColor->blue/65536)*localVal;
			} else {	//Hopefully the following is never used
				red = ((float) NegColor.red/65536)*localVal;
				green = ((float) NegColor.green/65536)*localVal;
				blue = ((float) NegColor.blue/65536)*localVal;
			}
		}
	}
}

long Surf3DBase::CreateSolidSurface(CPoint3D * Vertices, CPoint3D * Normals, long * vList,
		long NumTriangles, RGBColor * SurfaceColor, float * SurfaceValue,
		RGBColor * NColor, float MaxSurfaceValue, MoleculeData * , myGLTriangle * transpTri)
{
	long				v1, v2, v3, result=0;
	GLfloat				alpha=1.0, red, green, blue, xnorm, ynorm, znorm;

	red = (float) SurfaceColor->red/65536.0;
	green = (float) SurfaceColor->green/65536.0;
	blue = (float) SurfaceColor->blue/65536.0;
	red = MIN(red, 1.0);
	blue = MIN(blue, 1.0);
	green = MIN(green, 1.0);
	red = MAX(red, 0.0);
	blue = MAX(blue, 0.0);
	green = MAX(green, 0.0);
	long * VertexList = vList;
	if (isTransparent()) {
		alpha = (100 - Transparency) / 100.0f;
		if (!transpTri) return 0;	//transparncy requires a different draw method
		result = NumTriangles;
	}

	if (!SurfaceValue) {	//If we are not using surface coloring setup the color once for all the triangles
		glColor4f(red, green, blue, alpha);
	}


	if (!transpTri)
		glBegin(GL_TRIANGLES);
	for (long itri=0; itri<NumTriangles; itri++) {
		v1 = VertexList[3*itri];
		v2 = VertexList[3*itri+1];
		v3 = VertexList[3*itri+2];
		
		if (Normals) {
			xnorm = Normals[v1].x;
			ynorm = Normals[v1].y;
			znorm = Normals[v1].z;
		} else {	//compute a simple triangle normal for all three vertices
			float qx = Vertices[v2].x - Vertices[v1].x;
			float qy = Vertices[v2].y - Vertices[v1].y;
			float qz = Vertices[v2].z - Vertices[v1].z;
			float px = Vertices[v3].x - Vertices[v1].x;
			float py = Vertices[v3].y - Vertices[v1].y;
			float pz = Vertices[v3].z - Vertices[v1].z;
			xnorm = -(py*qz - pz*qy);
			ynorm = -(pz*qx - px*qz);
			znorm = -(px*qy - py*qx);

			float len = 1.0/sqrt( xnorm*xnorm + ynorm*ynorm + znorm*znorm );
			xnorm *= len;
			ynorm *= len;
			znorm *= len;
		}
		if (SurfaceValue) {
			float temp = SurfaceValue[v1];
			temp /= MaxSurfaceValue;
			SetSurfaceColor(temp, SurfaceColor, NColor, red, green, blue);
			if (!transpTri)
				glColor4f(red, green, blue, alpha);
		}

		if (!transpTri) {
			glNormal3f(xnorm, ynorm, znorm);
			glVertex3d(Vertices[v1].x, Vertices[v1].y, Vertices[v1].z);
		} else {
			transpTri[itri].v1 = Vertices[v1];
			transpTri[itri].n1.x = xnorm;
			transpTri[itri].n1.y = ynorm;
			transpTri[itri].n1.z = znorm;
			transpTri[itri].r1 = red;
			transpTri[itri].g1 = green;
			transpTri[itri].b1 = blue;
			transpTri[itri].a1 = alpha;
		}

		if (Normals) {
			xnorm = Normals[v2].x;
			ynorm = Normals[v2].y;
			znorm = Normals[v2].z;
		}
		if (SurfaceValue) {
			float temp = SurfaceValue[v2];
			temp /= MaxSurfaceValue;
			SetSurfaceColor(temp, SurfaceColor, NColor, red, green, blue);
			if (!transpTri)
				glColor4f(red, green, blue, alpha);
		}
		if (!transpTri) {
			glNormal3f(xnorm, ynorm, znorm);
			glVertex3d(Vertices[v2].x, Vertices[v2].y, Vertices[v2].z);
		} else {
			transpTri[itri].v2 = Vertices[v2];
			transpTri[itri].n2.x = xnorm;
			transpTri[itri].n2.y = ynorm;
			transpTri[itri].n2.z = znorm;
			transpTri[itri].r2 = red;
			transpTri[itri].g2 = green;
			transpTri[itri].b2 = blue;
			transpTri[itri].a2 = alpha;
		}
		
		if (Normals) {
			xnorm = Normals[v3].x;
			ynorm = Normals[v3].y;
			znorm = Normals[v3].z;
		}
		if (SurfaceValue) {
			float temp = SurfaceValue[v3];
			temp /= MaxSurfaceValue;
			SetSurfaceColor(temp, SurfaceColor, NColor, red, green, blue);
			if (!transpTri)
				glColor4f(red, green, blue, alpha);
		}
		if (!transpTri) {
			glNormal3f(xnorm, ynorm, znorm);
			glVertex3d(Vertices[v3].x, Vertices[v3].y, Vertices[v3].z);
		} else {
			transpTri[itri].v3 = Vertices[v3];
			transpTri[itri].n3.x = xnorm;
			transpTri[itri].n3.y = ynorm;
			transpTri[itri].n3.z = znorm;
			transpTri[itri].r3 = red;
			transpTri[itri].g3 = green;
			transpTri[itri].b3 = blue;
			transpTri[itri].a3 = alpha;
		}
	}
	if (!transpTri)
		glEnd();	//End of triangle creation
	return result;
}
long Surf3DBase::CreateWireFrameSurfaceWithLines(CPoint3D * Vertices, long * vList,
									long NumTriangles, RGBColor * SurfaceColor, float * SurfaceValue,
									RGBColor * NColor, float MaxSurfaceValue, MoleculeData * )
{
	long				v1, v2, v3, result=0;
	GLfloat				alpha=1.0, red, green, blue;
	
	red = (float) SurfaceColor->red/65536.0;
	green = (float) SurfaceColor->green/65536.0;
	blue = (float) SurfaceColor->blue/65536.0;
	red = MIN(red, 1.0);
	blue = MIN(blue, 1.0);
	green = MIN(green, 1.0);
	red = MAX(red, 0.0);
	blue = MAX(blue, 0.0);
	green = MAX(green, 0.0);
	long * VertexList = vList;
	
	if (!SurfaceValue) {	//If we are not using surface coloring setup the color once for all the triangles
		glColor4f(red, green, blue, alpha);
	}
	
	glLineWidth(1);
	glBegin(GL_LINES);
	for (long itri=0; itri<NumTriangles; itri++) {
		v1 = VertexList[3*itri];
		v2 = VertexList[3*itri+1];
		v3 = VertexList[3*itri+2];
		
		if (SurfaceValue) {
			float temp = SurfaceValue[v1];
			temp /= MaxSurfaceValue;
			SetSurfaceColor(temp, SurfaceColor, NColor, red, green, blue);
			glColor4f(red, green, blue, alpha);
		}
		
		glVertex3d(Vertices[v1].x, Vertices[v1].y, Vertices[v1].z);

		if (SurfaceValue) {
			float temp = SurfaceValue[v2];
			temp /= MaxSurfaceValue;
			SetSurfaceColor(temp, SurfaceColor, NColor, red, green, blue);
			glColor4f(red, green, blue, alpha);
		}

		glVertex3d(Vertices[v2].x, Vertices[v2].y, Vertices[v2].z);
			//Once for 1-2, once for 2-3
		glVertex3d(Vertices[v2].x, Vertices[v2].y, Vertices[v2].z);
		
		if (SurfaceValue) {
			float temp = SurfaceValue[v3];
			temp /= MaxSurfaceValue;
			SetSurfaceColor(temp, SurfaceColor, NColor, red, green, blue);
			glColor4f(red, green, blue, alpha);
		}
		glVertex3d(Vertices[v3].x, Vertices[v3].y, Vertices[v3].z);

		glVertex3d(Vertices[v3].x, Vertices[v3].y, Vertices[v3].z);

		if (SurfaceValue) {
			float temp = SurfaceValue[v1];
			temp /= MaxSurfaceValue;
			SetSurfaceColor(temp, SurfaceColor, NColor, red, green, blue);
			glColor4f(red, green, blue, alpha);
		}
		
		glVertex3d(Vertices[v1].x, Vertices[v1].y, Vertices[v1].z);
	}
	glEnd();	//End of Line creation
	return result;
}

//Utility function to create a line made up of a variable width cylinder. the GLUquadricObj must be preallocated
void CreateCylinderFromLine(GLUquadricObj *qobj, const CPoint3D& lineStart, const CPoint3D& lineEnd,
							float lineWidth, int nslices, int nstacks, bool cap) {

	if (qobj == NULL) return;

	CPoint3D offset;
	CPoint3D NormalOffset(0.0f, 0.0f, 0.0f);
	CPoint3D NormStart(0.0f, 0.0f, 1.0f);
	Matrix4D rotMat;
	
	offset = lineEnd - lineStart;
	float length = offset.Magnitude();
	if (length>0.00001) {
		NormalOffset.x = offset.x/length;
		NormalOffset.y = offset.y/length;
		NormalOffset.z = offset.z/length;
	}

	SetRotationMatrix(rotMat, &NormStart, &NormalOffset);
	rotMat[3][0] = lineStart.x;
	rotMat[3][1] = lineStart.y;
	rotMat[3][2] = lineStart.z;
	
	glPushMatrix();
	glMultMatrixf((const GLfloat *) &rotMat);
	gluCylinder(qobj, lineWidth, lineWidth, length, nslices, nstacks);
	if (cap) {
		glTranslatef(0.0, 0.0, length);
		gluSphere(qobj, lineWidth, nslices, nstacks);
		/* gluDisk(qobj, 0.0, lineWidth, nslices, 2); */
	}
	glPopMatrix();
}

void DashedQuadFromLine(const CPoint3D& pt1,
						const CPoint3D& pt2, float width, float m[16],
						const CPoint3D& x_world, float offset,
						GLuint length_anno_tex_id,
						const WinPrefs * Prefs, bool draw_label) {

	float len;
	GLdouble scr_coords1[3];  // Screen coordinates of pt1
	GLdouble scr_coords2[3];  // Screen coordinates of pt2
	CPoint3D scr_vec;		  // Screen space vector between atoms
	GLdouble perp_obj[3];	  // Object coords on vector perp. to scr_vec
	CPoint3D offset_vec;	  // Direction to shift bond cylinders
	GLdouble modelview[16];
	GLdouble proj[16];
	GLint viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Find screen coordinates of one point.
	gluProject(pt1.x, pt1.y, pt1.z, modelview, proj, viewport,
				&(scr_coords1[0]), &(scr_coords1[1]), &(scr_coords1[2]));

	// Find screen coordinates of other atom.
	gluProject(pt2.x, pt2.y, pt2.z, modelview, proj, viewport,
				&(scr_coords2[0]), &(scr_coords2[1]), &(scr_coords2[2]));

	// Find vector perpendicular to vector between two screen points and
	// normalize it so we can scalar multiply it later.  We flip and 
	// negate the slope of the line between the two screen coordinates to
	// get the slope of the perpendicular line.
	scr_vec.x = scr_coords2[1] - scr_coords1[1];
	scr_vec.y = scr_coords1[0] - scr_coords2[0];
	scr_vec.z = 0;
	scr_vec *= 1 / scr_vec.Magnitude();

	// Now find a point on the perpendicular vector with pt1's depth
	// and get its object coordinates.
	gluUnProject(scr_coords1[0] + scr_vec.x * 10,
					scr_coords1[1] + scr_vec.y * 10,
					scr_coords1[2],
					modelview, proj, viewport,
					&(perp_obj[0]), &(perp_obj[1]), &(perp_obj[2]));

	// Finally, we see what direction all bond cylinders must be offset
	// so that they will always stay in view.
	offset_vec.x = perp_obj[0] - pt1.x;
	offset_vec.y = perp_obj[1] - pt1.y;
	offset_vec.z = perp_obj[2] - pt1.z;
	offset_vec *= 1 / offset_vec.Magnitude();

	len = (pt2 - pt1).Magnitude();

	CPoint3D new_pt1a;
	CPoint3D new_pt1b;
	CPoint3D new_pt2a;
	CPoint3D new_pt2b;

	new_pt1a.x = pt1.x + offset_vec.x * width;
	new_pt1a.y = pt1.y + offset_vec.y * width;
	new_pt1a.z = pt1.z + offset_vec.z * width;
	new_pt1b.x = pt1.x - offset_vec.x * width;
	new_pt1b.y = pt1.y - offset_vec.y * width;
	new_pt1b.z = pt1.z - offset_vec.z * width;
	new_pt2a.x = pt2.x + offset_vec.x * width;
	new_pt2a.y = pt2.y + offset_vec.y * width;
	new_pt2a.z = pt2.z + offset_vec.z * width;
	new_pt2b.x = pt2.x - offset_vec.x * width;
	new_pt2b.y = pt2.y - offset_vec.y * width;
	new_pt2b.z = pt2.z - offset_vec.z * width;

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);

	glBindTexture(GL_TEXTURE_2D, length_anno_tex_id);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(new_pt1a.x, new_pt1a.y, new_pt1a.z);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(new_pt1b.x, new_pt1b.y, new_pt1b.z);
		glTexCoord2f(len / (5.0f * width), 0.0f);
		glVertex3f(new_pt2b.x, new_pt2b.y, new_pt2b.z);
		glTexCoord2f(len / (5.0f * width), 1.0f);
		glVertex3f(new_pt2a.x, new_pt2a.y, new_pt2a.z);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);

	if (draw_label) {
		char len_label[40];
		float LabelSize = Prefs->GetAnnotationLabelSize();

		// We move the midpoint of the line and align the viewer to draw the 
		// text.
		glPushMatrix();
		glTranslatef((pt1.x + pt2.x) / 2.0f,
					 (pt1.y + pt2.y) / 2.0f,
					 (pt1.z + pt2.z) / 2.0f);
		glMultMatrixf(m);

		// Move out for the kind of bond that exists between the atoms.
		glTranslatef(-offset - 0.01f, 0.0f, 0.0f);

		glScalef(-5.0f * LabelSize, 5.0f * LabelSize, 0.1f);
		
		// This apparently is some magic number.  GLF doesn't start drawing the
		// string at the origin, doesn't consider the anchor point, and doesn't
		// consider the bounding box it returns.  So, we have a magic number.
		// It is indirectly dependent on LabelSize from the scaling above, so
		// it looks constant even though it is not.
		glTranslatef(0.004f, 0.0f, 0.0f);

		sprintf(len_label, "%.2f", len);
		DrawString(len_label);

		glPopMatrix();
	}

}

void DrawRotationAxis(const CPoint3D & lineStart, const CPoint3D & lineEnd, const int & order) {

	float plane_emissive[] = { 0.0, 0.3, 0.7, 0.2 };
//	float plane_diffuse[] = { 0.0, 0.3, 0.6, 0.3 };
	float plane_specular[] = { 0.0, 0.3, 0.6, 1.0 };

	glPushAttrib(GL_LIGHTING_BIT);

	int imageWidth =16;
	//Our width needs to be a power of two. So orders 1, 2 and 4 are no problem. Other orders such as 3
	//must be padded out to the next higher power of two.
	int repeat = order;
	if (order == 3) repeat = 4;
	if (order > 4) repeat = 8;
	GLubyte bw[16][16] ={	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,255,128,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0},
	{0,255,255,255,255,255,255,255,255,255,128,0,0,0,0,0},
	{0,255,255,255,255,255,255,255,255,255,255,0,0,0,0},
	{0,255,255,255,255,255,255,255,255,255,255,128,0,0,0},
	{0,255,255,255,255,255,255,255,255,255,255,128,0,0,0},
	{0,255,255,255,255,255,255,255,255,255,255,0,0,0,0},
	{0,255,255,255,255,255,255,255,255,255,128,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,255,255,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,255,128,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,255,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	GLubyte * testimage = new GLubyte[imageWidth*imageWidth*repeat*4];
	int padding = ((repeat - order)*16)/order;
	int remainder = ((repeat - order)*16)-(padding*order);
	int p = 0;
	for (int i=0; i<imageWidth; i++) {
		for (int o=0; o<order; o++) {
			for (int j=0; j<imageWidth; j++) {
				testimage[p] = bw[i][j];
				testimage[p + 1] = bw[i][j];
				testimage[p + 2] = bw[i][j];
				testimage[p + 3] = bw[i][j];
				if (bw[i][j] == 0) testimage[p + 3]=178;
				p+=4;
			}
			for (int t=0; t<padding; t++) {
				testimage[p] = 0;
				testimage[p + 1] = 0;
				testimage[p + 2] = 0;
				testimage[p + 3] = 178;
				p+=4;
			}
		}
		for (int t=0; t<remainder; t++) {
			testimage[p] = 0;
			testimage[p + 1] = 0;
			testimage[p + 2] = 0;
			testimage[p + 3] = 178;
			p+=4;
		}
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLuint texname;
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth*repeat, imageWidth, 0, GL_RGBA,
				 GL_UNSIGNED_BYTE, testimage);
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, plane_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plane_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30.0f);
	glColor4f(0, .64, .85, 0.7);
	/* glColor4fv(plane_diffuse); */

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	
	GLUquadricObj * qobj = NULL;
	qobj = gluNewQuadric();

	CPoint3D	offset, NormalOffset, NormEnd, NormStart = CPoint3D(0.0f, 0.0f, 1.0f);
	Matrix4D	rotMat;
	
	offset.x =  lineEnd.x - lineStart.x;
	offset.y =  lineEnd.y - lineStart.y;
	offset.z =  lineEnd.z - lineStart.z;
	float length = offset.Magnitude();
	if (length>0.00001) {
		NormalOffset.x = offset.x/length;
		NormalOffset.y = offset.y/length;
		NormalOffset.z = offset.z/length;
	} else
		NormalOffset.x=NormalOffset.y=NormalOffset.z=0.0;
	NormEnd = lineEnd;
	Normalize3D(&NormEnd);
	SetRotationMatrix(rotMat, &NormStart, &NormalOffset);
	rotMat[3][0] = lineStart.x;
	rotMat[3][1] = lineStart.y;
	rotMat[3][2] = lineStart.z;
	
	glPushMatrix();
	glMultMatrixf((const GLfloat *) &rotMat);
	gluQuadricTexture(qobj, GL_TRUE);
	gluCylinder(qobj, 0.1, 0.1, 0.2, 12, 1);
	glTranslatef(0, 0, 0.2);
	glDisable(GL_TEXTURE_2D);
	gluQuadricTexture(qobj, GL_FALSE);
	gluCylinder(qobj, 0.1, 0.1, length-0.4, 12, 1);
	glEnable(GL_TEXTURE_2D);
	glTranslatef(0, 0, length-0.4);
	gluQuadricTexture(qobj, GL_TRUE);
	gluCylinder(qobj, 0.1, 0.1, 0.2, 12, 1);
	glPopMatrix();
	
	if (qobj) gluDeleteQuadric(qobj);	//finally delete the quadric object
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDeleteTextures(1, &texname);

	glPopAttrib();

	delete[] testimage;
}

//Draw a translucent plane
void DrawTranslucentPlane(const CPoint3D & origin, const CPoint3D & p1, const CPoint3D & p2) {

	float plane_emissive[] = { 0.0, 0.3, 0.7, 0.2 };
//	float plane_diffuse[] = { 0.0, 0.3, 0.6, 0.3 };
	float plane_specular[] = { 0.0, 0.3, 0.6, 1.0 };
	glPushAttrib(GL_LIGHTING_BIT);
	Matrix4D rotationMatrix;

	CPoint3D vec1 = p1 - origin;
	CPoint3D vec2 = p2 - origin;
	float s1Length = vec1.Magnitude();
	float s2Length = vec2.Magnitude();
	SetPlaneRotation(rotationMatrix, vec1, vec2);

	glPushMatrix();
	glTranslatef(origin.x, origin.y, origin.z);
	glMultMatrixf((const GLfloat *) &rotationMatrix);

	// Symmetry planes are viewed from either side, so if we should either
	// light both sides or light neither.  Neither is simpler.
	glDisable(GL_LIGHTING);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, plane_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plane_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30.0f);
	glColor4f(0, .64, .85, 0.3);
	glRectf(0, 0, s1Length, s2Length);
	glDisable(GL_BLEND);

	glColor4f(0, .64, .85, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1);
	glRectf(0, 0, s1Length, s2Length);
	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDepthMask(GL_TRUE);
	glPopAttrib();
	
	glPopMatrix();
}

void DrawInversionPoint(void) {
	GLUquadricObj * qobj = NULL;
	qobj = gluNewQuadric();
//	float sphere_emissive[] = { 0.0, 0.3, 0.7, 0.2 };
//	float sphere_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float sphere_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	
	glPushAttrib(GL_LIGHTING_BIT);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sphere_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sphere_specular);
	glColor4f(.7, .7, .7, 1.0);

	//Assume the inversion point is at the origin
	gluSphere(qobj, 0.2, 30, 20);	//Create and draw the sphere
	
	//Add several arrows to indicate the inversion...
	float plane_emissive[] = { 0.0, 0.3, 0.7, 0.2 };
//	float plane_diffuse[] = { 0.0, 0.3, 0.6, 0.3 };
	float plane_specular[] = { 0.0, 0.3, 0.6, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, plane_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plane_specular);
	glColor4f(0, .64, .85, 0.7);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	gluCylinder(qobj, 0.02, 0.02, 0.6, 12, 1);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.6);
	gluDisk(qobj, 0.0, 0.06, 12, 2);
	gluCylinder(qobj, 0.06, 0.0, 0.07, 12, 3);
	glPopMatrix();

	glPushMatrix();
	Matrix4D	rotMat;
	CPoint3D			vector = CPoint3D(1.0f, 0.0f, 0.0), NormStart = CPoint3D(0.0f, 0.0f,1.0f);
	InitRotationMatrix(rotMat);
	rotMat[2][2] = -1;
	glMultMatrixf((const GLfloat *) &rotMat);
	DrawArrow(0.7, 0.02, 12);
	glPopMatrix();
	glPushMatrix();
	SetRotationMatrix(rotMat, &NormStart, &vector);
	glMultMatrixf((const GLfloat *) &rotMat);
	DrawArrow(0.7, 0.02, 12);
	glPopMatrix();
	glPushMatrix();
	rotMat[2][0] *= -1.0;
	glMultMatrixf((const GLfloat *) &rotMat);
	DrawArrow(0.7, 0.02, 12);
	glPopMatrix();
	vector.x = 0.0;
	vector.y = 1.0;
	glPushMatrix();
	SetRotationMatrix(rotMat, &NormStart, &vector);
	glMultMatrixf((const GLfloat *) &rotMat);
	DrawArrow(0.7, 0.02, 12);
	glPopMatrix();
	glPushMatrix();
	rotMat[2][1] *= -1.0;
	glMultMatrixf((const GLfloat *) &rotMat);
	DrawArrow(0.7, 0.02, 12);
	glPopMatrix();

	glDisable(GL_BLEND);
	glPopAttrib();
	if (qobj) gluDeleteQuadric(qobj);	//finally delete the quadric object
}

void DrawArrow(const float & length, const float & width, const int & quality) {
		//Draw an arrow with arrow head at far end.

	GLUquadricObj * qobj = NULL;
	qobj = gluNewQuadric();
	gluDisk(qobj, 0.0, width, quality, 2);
	gluCylinder(qobj, width, width, length-3.5*width, quality, 1);
	glPushMatrix();
	glTranslatef(0.0, 0.0, length-2.5*width);
	gluDisk(qobj, 0.0, 3*width, quality, 2);
	gluCylinder(qobj, 3*width, 0.0, 3.5*width, quality, 2);
	glPopMatrix();
	if (qobj) gluDeleteQuadric(qobj);	//finally delete the quadric object
}

/**
 * This draws a dashed angle between the three specified points and
 * marks the angle's degrees with a text label.  The angle goes from
 * pt1 to pt2 to pt3.  To complicate things a bit, we draw each dash
 * so that it is aligned to the screen so the viewer can always see it.
 */
void DrawAngleAnnotation(const CPoint3D *pt1, const CPoint3D *pt2,
						 const CPoint3D *pt3, const WinPrefs *Prefs,
						 GLuint length_anno_tex_id, CPoint3D *ambig_axis) {

	CPoint3D vec1;
	CPoint3D vec2;
	Matrix4D plane2xy;
	float min_len;
	float len1;
	float len2;
	float angle;
	float m[16];
	float chord_len;
	float x_eye[3];
	float y_eye[3];
	float delta;

	// First we get vectors representing the angle.
	vec1 = *pt1 - *pt2;
	vec2 = *pt3 - *pt2;

	// We need to find the shorter of the two vectors so that we know how far
	// along each vector we can draw the annotation.  And with the lengths
	// saved, we can go ahead and normalize.
	len1 = vec1.Magnitude();
	len2 = vec2.Magnitude();
	min_len = MIN(len1, len2);

	vec1 = vec1 * (1.0f / len1);
	vec2 = vec2 * (1.0f / len2);

	// Grab the angle.
	float dot = DotProduct3D(&vec1, &vec2);
	angle = acos(dot);

	// If two vectors are not (anti-)parallel, drawing the annotation is easy.
	// We move into the space where vec1 is the x-axis and vec2 is in the
	// xy-plane.  In this space we can draw a circular segment with simple
	// geometry.
	if (fabs(dot) < 0.999999f) {
		SetPlaneRotation(plane2xy, vec1, vec2);
	}
	
	// If they are (anti-)parallel, there're many possible planes we can move
	// into.
	else {

		// acos doesn't tolerate values whose absolute values are > 1, which 
		// might happen because of precision errors.  So, we fix the angle.
		angle = dot > 0.0f ? 0.0f : kPi;

		// For dihedral annotations, we push the normal of the dihedral planes
		// into the new space's xy-plane.
		if (ambig_axis) {
			SetPlaneRotation(plane2xy, vec1, *ambig_axis);
		}
		
		// For angle annotations, we try to keep them as visible as possible.
		// We keep vec1 as the x-axis in our new space, and we try to put
		// either the screen's x- or y-axis in the new space's xy-plane.
		else {
			glGetFloatv(GL_MODELVIEW_MATRIX, m);
			CPoint3D vec3;
			vec3.x = m[0];
			vec3.y = m[4];
			vec3.z = m[8];

			if (fabs(DotProduct3D(&vec1, &vec2)) < 0.999999f) {
				SetPlaneRotation(plane2xy, vec1, vec3);
			} else {
				vec3.x = m[1];
				vec3.y = m[5];
				vec3.z = m[9];
				SetPlaneRotation(plane2xy, vec1, vec3);
			}
		}
	}

	glPushMatrix();

	// We move to the center vertex and transform into the space described
	// above.
	glTranslatef(pt2->x, pt2->y, pt2->z);
	glMultMatrixf((const GLfloat *) &plane2xy);

	// We want screen-aligned dashes.  So we want to draw quads that extend in
	// the x- and y-directions in eye space.  Since we draw in object space, we
	// must figure out what the eye's x- and y- axes are there.  To do that,
	// all we have to do is grab the first two axes from the inverse of the
	// modelview matrix.
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	x_eye[0] = m[0];
	x_eye[1] = m[4];
	x_eye[2] = m[8];

	y_eye[0] = m[1];
	y_eye[1] = m[5];
	y_eye[2] = m[9];

	GLdouble BondSize = Prefs->GetQD3DBondWidth();
	CPoint3D origin(0.0f, 0.0f, 0.0f);

	// How long should each dash be, and how often do we want them?
	chord_len = BondSize * 0.25f * 10.0f;
	delta = angle / (min_len * angle / chord_len);

	float ca = cos(angle);
	float sa = sin(angle);

	// First we draw the dashes along each side.
	DashedQuadFromLine(origin, CPoint3D(min_len, 0.0f, 0.0f),
					   BondSize * 0.25f, m,
					   CPoint3D(x_eye[0], x_eye[1], x_eye[2]),
					   0.0f, length_anno_tex_id, Prefs, false);
	DashedQuadFromLine(CPoint3D(min_len * ca, min_len * sa, 0.0f), origin,
					   BondSize * 0.25f, m,
					   CPoint3D(x_eye[0], x_eye[1], x_eye[2]),
					   0.0f, length_anno_tex_id, Prefs, false);

	// Then we draw a series of chords that approximates the angle's arc.
	float next_delta;
	for (float i = 0.0f; i < angle; i += delta) {

		// We want the chord to go from the current slice of the pie to the
		// next, but since the slice angle is constant and the anno angle may
		// not be a multiple of the slice angle, we don't want to overshoot the
		// anno angle.  Also, we subtract off an epsilon so that the texture 
		// doesn't start to repeat.
		next_delta = MIN(i + delta, angle) - 0.01f;

		DashedQuadFromLine(CPoint3D(min_len * cos(i), min_len * sin(i), 0.0f),
						   CPoint3D(min_len * cos(next_delta), min_len * sin(next_delta), 0.0f),
						   BondSize * 0.25f, m,
						   CPoint3D(x_eye[0], x_eye[1], x_eye[2]),
						   0.0f, length_anno_tex_id, Prefs, false);
	}

	CPoint3D lookat_eye = CPoint3D(0.0f, 0.0f, 1.0f);
	CPoint3D up_eye = CPoint3D(0.0f, 1.0f, 0.0f);
	CPoint3D lookat_world;
	CPoint3D up_world;
	CPoint3D r;

	Matrix4D mv_inv;

	// Invert just the rotation portion of the modelview matrix.  We can't
	// use InverseMatrix because it considers the translation factors.
	// We don't want the translation factors because we want to transform
	// a vector, and Rotate3DPt transforms only points (and not vectors).
	mv_inv[0][0] = m[0];
	mv_inv[0][1] = m[4];
	mv_inv[0][2] = m[8];
	mv_inv[0][3] = 0.0f;
	mv_inv[1][0] = m[1];
	mv_inv[1][1] = m[5];
	mv_inv[1][2] = m[9];
	mv_inv[1][3] = 0.0f;
	mv_inv[2][0] = m[2];
	mv_inv[2][1] = m[6];
	mv_inv[2][2] = m[10];
	mv_inv[2][3] = 0.0f;
	mv_inv[3][0] = 0.0f;
	mv_inv[3][1] = 0.0f;
	mv_inv[3][2] = 0.0f;
	mv_inv[3][3] = 1.0f;

	// Transform the eye space vectors to world coordinates, and find 
	// a third vector to form a basis set.
	Rotate3DPt(mv_inv, lookat_eye, &lookat_world);
	Rotate3DPt(mv_inv, up_eye, &up_world);
	CrossProduct3D(&lookat_world, &up_world, &r);

	m[0] = r.x;
	m[1] = r.y;
	m[2] = r.z;
	m[3] = 0.0f;

	m[4] = up_world.x;
	m[5] = up_world.y;
	m[6] = up_world.z;
	m[7] = 0.0f;

	m[8] = lookat_world.x;
	m[9] = lookat_world.y;
	m[10] = lookat_world.z;
	m[11] = 0.0f;

	m[12] = m[13] = m[14] = 0.0f;
	m[15] = 1.0f;

	char angle_label[40];
	sprintf(angle_label, "%.2f", angle * 180.f / kPi);
	glTranslatef(min_len * cos(angle * 0.5f),
				 min_len * sin(angle * 0.5f), 0.0f);
	glMultMatrixf(m);
	float LabelSize = Prefs->GetAnnotationLabelSize();
	glTranslatef(-0.1f + chord_len, 0.0f, 0.0f);
	glScalef(-5.0f * LabelSize, 5.0f * LabelSize, 0.1f);

	// glfDrawSolidString(angle_label); 
	DrawString(angle_label);

	glPopMatrix();

}

void DrawSceneString(const float scale_factor, const float shift_x,
		             const float shift_y, const float shift_z,
					 const wxString& label) {

	// This function draws a string (label) at the origin of the current
	// coordinate system, offset by shift_[xyz] and aligned with the screen.
	// Right before the string is drawn, the coordinate system is uniformly
	// scaled by scale_factor (since glf draws in a larger coordinate system
	// than we want).
	//
	// It's assumed that the color and coordinate system are in the correct
	// state when this function is called.  It's also assumed that lighting is
	// turned on.  The assumed state is restored before the function returns.

	float m[16];                              // OpenGL matrix
	Matrix4D mv_inv;                          // Inverse of modelview matrix
	CPoint3D lookat_eye = CPoint3D(0.0f, 0.0f, 1.0f); // Lookat vector in eye-space
	CPoint3D lookat_world;                    //   transformed to world-space
	CPoint3D up_eye = CPoint3D(0.0f, 1.0f, 0.0f);     // Up vector in eye-space
	CPoint3D up_world;                        //   transformed to world-space
	CPoint3D r;                               // Right vector in world-space

	// Invert just the rotation portion of the modelview matrix.  This is
	// much faster than inverting an arbitrary matrix.
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	mv_inv[0][0] = m[0];
	mv_inv[0][1] = m[4];
	mv_inv[0][2] = m[8];
	mv_inv[0][3] = 0.0f;
	mv_inv[1][0] = m[1];
	mv_inv[1][1] = m[5];
	mv_inv[1][2] = m[9];
	mv_inv[1][3] = 0.0f;
	mv_inv[2][0] = m[2];
	mv_inv[2][1] = m[6];
	mv_inv[2][2] = m[10];
	mv_inv[2][3] = 0.0f;
	mv_inv[3][0] = 0.0f;
	mv_inv[3][1] = 0.0f;
	mv_inv[3][2] = 0.0f;
	mv_inv[3][3] = 1.0f;

	// Transform the eye space vectors to world coordinates, and find 
	// a third vector to form a basis set.
	Rotate3DPt(mv_inv, lookat_eye, &lookat_world);
	Rotate3DPt(mv_inv, up_eye, &up_world);
	CrossProduct3D(&lookat_world, &up_world, &r);

	// Enter the basis into a new modelview matrix.
	m[0] = r.x;
	m[1] = r.y;
	m[2] = r.z;
	m[3] = 0.0f;

	m[4] = up_world.x;
	m[5] = up_world.y;
	m[6] = up_world.z;
	m[7] = 0.0f;

	m[8] = lookat_world.x;
	m[9] = lookat_world.y;
	m[10] = lookat_world.z;
	m[11] = 0.0f;

	m[12] = m[13] = m[14] = 0.0f;
	m[15] = 1.0f;

	glDisable(GL_LIGHTING);

	// Undo the rotation by multiplying by the inverse.  Then shift, scale, and
	// draw the string.
	glPushMatrix();
	glMultMatrixf(m);
	glTranslatef(shift_x, shift_y, shift_z);
	glScalef(-scale_factor, scale_factor, scale_factor);
	glfDrawSolidString(label.mb_str());
	glPopMatrix();

	glEnable(GL_LIGHTING);

}

void DrawString(const char *str) {
	// glfDrawSolidString(str); 

	glfStartBitmapDrawing();

	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	glEnable(GL_BLEND);
	glfDrawBMaskString(str);
	glDisable(GL_BLEND);

	glfStopBitmapDrawing();
}

#define CYL_RADIUS 0.05f
#include <algorithm>
void MolDisplayWin::DrawBondingSites(long iatom, float radius, GLUquadricObj *qobj, int site_id, CPoint3D * vector) {

	// This function either draws remaining bonding sites for the specified
	// atom or returns the vector along the atom's specified bonding site.  If
	// site_vec is 0, drawing is done.  Otherwise, the normalized vector for
	// the site_vec is returned.

	Frame *lFrame=MainData->cFrame;
	mpAtom *lAtoms = lFrame->Atoms;
	Bond *lBonds = lFrame->Bonds;
	long NumBonds = lFrame->NumBonds;
	
	short coordination = lAtoms[iatom].GetCoordinationNumber();
	std::vector<Bond *> bonds;
	int bonded_atoms[6];
	CPoint3D vecs[6];
	unsigned int i;

	bonds.reserve(6);

	if (coordination == 0) return;

	// First we grab all bonds pertaining to the atom.  If the number of bonds
	// exceeds five, then there's no point in showing any bonding sites since
	// the atom is either full or overfull.
	for (i = 0; i < NumBonds; i++) {
		if (((iatom == lBonds[i].Atom1) || (iatom == lBonds[i].Atom2)) &&
			(lBonds[i].Order > kHydrogenBond)) {
			bonds.push_back(&lBonds[i]);
		}
	}

	if (bonds.size() >= coordination) {
		return;
	}

	// Test to see if any bond sites are unused, if not return.
	// if (bonds.size() >= coordination) return; 

	// We sort the bonds by bond order, and figure out what the atom opposite
	// this one is.  We'll need this info to find the vector between the two
	// atoms.
	std::sort(bonds.begin(), bonds.end(), Bond::RefLessThan);
	for (i = 0; i < bonds.size(); i++) {
		bonded_atoms[i] = (iatom == bonds[i]->Atom1) ?
							bonds[i]->Atom2 : bonds[i]->Atom1;
	}

	// Calculate vectors from central atom to each bonded atom.
	for (i = 0; i < bonds.size(); i++) {
		vecs[i] = lAtoms[bonded_atoms[i]].Position - lAtoms[iatom].Position;
		Normalize3D(&vecs[i]);
	}

	int lpCount = lAtoms[iatom].GetLonePairCount();
	
	CPoint3D origin = CPoint3D(0.0f, 0.0f, 0.0f);
	float c, s, b;

	Matrix4D rot;
	InitRotationMatrix(rot);
	CPoint3D b1Offset(0.0f, 1.0f, 0.0f);
	if (bonds.size() > 0) {	//orient our frame to the primary bond
		b1Offset = lAtoms[bonded_atoms[0]].Position - lAtoms[iatom].Position;
		Normalize3D(&b1Offset);
		CPoint3D target(0.0f, 1.0f, 0.0f);
		SetRotationMatrix(rot, &target, &b1Offset);
	}

	if (site_id == 0) {
		glPushName(0);
		glColor3f(0.9f, 0.1f, 0.1f);
	}

#define DO_SITE(vec, vec_site) \
				if (site_id == 0) { \
					glLoadName(vec_site); \
					CreateCylinderFromLine(qobj, origin, \
										   vec * 2.0f * radius, \
										   CYL_RADIUS, 10, 3, true); \
				} else if (site_id == vec_site) { \
					*vector = vec; \
					return; \
				}

	// If we get here, we know that there are some unbonded sites.
	switch (coordination+lpCount) {

		// Configuration will be linear.
		case 1:
			{
				CPoint3D v(0.0f, 1.0f, 0.0f);
				DO_SITE(v, 1);
			}
			break;

		// Configuration will be linear.
		case 2:
			{
				// If we have no bonds yet of the 2 possible, we draw a site
				// along the y-axis and its inverse.
				if (bonds.size() == 0) {
					CPoint3D v(0.0f, 1.0f, 0.0f);
					DO_SITE(v, 1);
					if (lpCount < 1) {DO_SITE(v * -1.0f, 2);}
				}

				else if (bonds.size() == 1) {
					DO_SITE(vecs[0] * -1.0f, 2);
				}
			}
			break;

		// Configuration will be planar triangular.
		case 3:
			if (bonds.size() == 0) {
				CPoint3D v(0.0f, 1.0f, 0.0f);
				DO_SITE(v, 1);
			}

			if (lpCount < 2) {
				CPoint3D v3;
				if (bonds.size() >= 2) {	//Setup the 3rd vector to have an equal angle to both bonds
					//Just add the two bond vectors and invert
					// CPoint3D offset = lAtoms[secondaryBondedAtm].Position - lAtoms[iatom].Position; 
					// Normalize3D(&offset); 
					// v3 = b1Offset + offset; 
					// Normalize3D(&v3); 
					v3 = (vecs[0] + vecs[1]) * -1.0f;
					Normalize3D(&v3);
				} else { //Draw the 2nd position
					b = sqrt(3.0f / 4.0f);
					CPoint3D v2(-b, -0.5f, 0.0f), temp(b, -0.5f, 0.0f);
					Rotate3DOffset(rot,temp,&v3);
					temp = v2;
					Rotate3DOffset(rot,temp,&v2);
					//When dealing with a single bond check the bonded atom for bonds and align them as
					//appropriate
					if (bonds.size() == 1) {
						bool test = false;
						CPoint3D OtherBondVector;
						for (long i=0; i<NumBonds; i++) {
							if ((bonded_atoms[0] == lBonds[i].Atom1)&&(lBonds[i].Order > kHydrogenBond)&&
								(lBonds[i].Atom2 != iatom)) {
								OtherBondVector = lAtoms[lBonds[i].Atom2].Position - lAtoms[bonded_atoms[0]].Position;
								test = true;
								break;
							} else if ((bonded_atoms[0] == lBonds[i].Atom2)&&(lBonds[i].Order > kHydrogenBond)&&
									   (lBonds[i].Atom1 != iatom)) {
								OtherBondVector = lAtoms[lBonds[i].Atom1].Position - lAtoms[bonded_atoms[0]].Position;
								test = true;
								break;
							}
						}
						if (test) {
							Normalize3D(&OtherBondVector);
							float dp = DotProduct3D(&OtherBondVector, &b1Offset);
							if (fabs(dp) < 0.99) {
								CPoint3D cross;
								CrossProduct3D(&b1Offset, &OtherBondVector, &cross);
								Normalize3D(&cross);
								CrossProduct3D(&b1Offset, &cross, &OtherBondVector);
								v2 = b1Offset*-0.5 + OtherBondVector*-0.866025;
								v3 = b1Offset*-0.5 + OtherBondVector*0.866025;
							}
						}
					}
					if (bonds.size() <= 1) {
						DO_SITE(v2, 2);
					}
				}
				if (lpCount < 1) {
					DO_SITE(v3, 3);
				}
			}
			break;

		case 4:
			if (bonds.size() == 0) {
				CPoint3D v(0.0f, 1.0f, 0.0f);
				DO_SITE(v, 1);
			}
			if (lpCount < 3) {

				// If three bonds are already formed, we calculate the fourth
				// as the sum of the three, inverted.
				if (bonds.size() == 3) {
					//obtain the bond vector by summing the bond vectors, normalize and invert
					CPoint3D sum(0.0,0.0,0.0), temp, bonds[3];
					int found=0;
					for (int i=0; i<NumBonds; i++) {
						if (lBonds[i].Atom1 == iatom) {
							temp = lAtoms[lBonds[i].Atom2].Position - lAtoms[iatom].Position;
							Normalize3D(&temp);
							bonds[found] = temp;
							found++;
						} else if (lBonds[i].Atom2 == iatom) {
							temp = lAtoms[lBonds[i].Atom1].Position - lAtoms[iatom].Position;
							Normalize3D(&temp);
							bonds[found] = temp;
							found++;
						}
					}
					CrossProduct3D(&(bonds[0]),&(bonds[1]),&sum);
					Normalize3D(&sum);
					float direction = DotProduct3D(&sum,&(bonds[2]));
					if (direction > 0.0) direction = -1.0;
					else direction = 1.0;
					CrossProduct3D(&(bonds[1]),&(bonds[2]),&temp);
					Normalize3D(&temp);
					sum += temp;
					CrossProduct3D(&(bonds[2]),&(bonds[0]),&temp);
					Normalize3D(&temp);
					sum += temp;
					Normalize3D(&sum);
					sum *= direction;
					DO_SITE(sum, 4);
				}
				
				// If only one bond is already formed, we calculate the second, third
				// and fourth as ....
				else if (bonds.size() < 2) {
					c = -1.0f/3.0f;	//cos(109.5)
					s = sqrt(1.0f-(c*c)); //sin(109.5)
					b = s * sin(kPi/3.0);
					CPoint3D v2, temp(0.0f, c, s), v3(b, c, -0.5*s), v4(-b, c, -0.5*s);
					Rotate3DOffset(rot,temp,&v2);
					DO_SITE(v2, 2);
					if (lpCount < 2) {
						temp = v3;
						Rotate3DOffset(rot,temp,&v3);
						DO_SITE(v3, 3);
						if (lpCount < 1) {
							temp = v4;
							Rotate3DOffset(rot,temp,&v4);
							DO_SITE(v4, 4);
						}
					}
				}
				
				// If two bonds are already formed, we calculate the 
				// remaining two as ...
				else {
					CPoint3D offset = lAtoms[bonded_atoms[1]].Position - lAtoms[iatom].Position;
					Normalize3D(&offset);
					CPoint3D cross;
					CrossProduct3D(&b1Offset,&offset,&cross);
					offset += b1Offset;
					offset *= -1.0f;
					Normalize3D(&offset);
					Normalize3D(&cross);
					CPoint3D v3t;
					float c = cos(0.5*acos(-1.0f/3.0f));	//cos(109.5/2)
					float s = sqrt(1.0f-(c*c)); //sin(109.5/2)
					v3t = offset*c + cross*s;
					Normalize3D(&v3t);
					DO_SITE(v3t, 3);
					if (lpCount == 0) {
						v3t = offset*c - cross*s;
						Normalize3D(&v3t);
						DO_SITE(v3t, 4);
					}
				}
			}
			break;

		case 5:
			// Here axial and equitorial sites are different. LPs should go in
			// axial sites first.
			// Hmm... Unclear why the previous comment was made. LPs prefer to be as far from other
			// items as possible so should be in equitorial positions since axial are 90 degrees from
			// 3 sites while equitorial are 90 degrees from 2 sites and 120 degrees from 2 sites.

			if (bonds.size() == 0) {
				float c = sqrtf(3) / 2.0f;
				CPoint3D axial(0.0f, 1.0f, 0.0f);
				CPoint3D equat1(0.0f, 0.0f, -1.0f);
				CPoint3D equat2(c, 0.0f, 0.5f);
				CPoint3D equat3(-c, 0.0f, 0.5f);

				DO_SITE(axial, 4);
				if (lpCount < 4) {
					DO_SITE(axial * -1.0f, 5);
					if (lpCount < 3) {
						DO_SITE(equat1, 1);
						if (lpCount < 2) {
							DO_SITE(equat2, 2);
							if (lpCount < 1) {DO_SITE(equat3, 3);}
						}
					}
				}
			}
			
			else if (bonds.size() == 1) {

				CPoint3D equat1;
				CPoint3D equat2;
				CPoint3D equat3;
				Matrix4D rotate_mat;

				OrthoVector(vecs[0], equat1);
				RotateAroundAxis(rotate_mat, vecs[0], 120.0f);
				Rotate3DPt(rotate_mat, equat1, &equat2);
				Normalize3D(&equat2);
				equat3 = (equat1 + equat2) * -1.0f;
				Normalize3D(&equat3);

				DO_SITE(vecs[0] * -1.0f, 5);
				if (lpCount < 3) {
					DO_SITE(equat1, 1);
					if (lpCount < 2) {
						DO_SITE(equat2, 2);
						if (lpCount < 1) {DO_SITE(equat3, 3);}
					}
				}
			}

			else if (bonds.size() == 2) {

				float dot = DotProduct3D(&vecs[0], &vecs[1]);

				// If the two vectors are close to orthogonal, we assume the
				// primary bond is axial and the secondary equitorial.
				if (fabs(dot) < 0.25f) {
					Matrix4D rotate_mat;
					CPoint3D equat2;
					CPoint3D equat3;
					
					if (lpCount < 3) {
						DO_SITE(vecs[0] * -1.0f, 5);
						if (lpCount < 2) {
							RotateAroundAxis(rotate_mat, vecs[0], 120.0f);
							Rotate3DPt(rotate_mat, vecs[1], &equat2);
							Normalize3D(&equat2);
							equat3 = (vecs[1] + equat2) * -1.0f;
							Normalize3D(&equat3);

							DO_SITE(equat2, 2);
							if (lpCount < 1) {DO_SITE(equat3, 3);}
						}
					}
				} else {
					if (lpCount < 3) {
						CPoint3D equat1;
						CPoint3D equat2;
						CPoint3D equat3;
						Matrix4D rotate_mat;
						
						OrthoVector(vecs[0], equat1);
						RotateAroundAxis(rotate_mat, vecs[0], 120.0f);
						Rotate3DPt(rotate_mat, equat1, &equat2);
						Normalize3D(&equat2);
						equat3 = (equat1 + equat2) * -1.0f;
						Normalize3D(&equat3);
					
						DO_SITE(equat1, 1);
						if (lpCount < 2) {
							DO_SITE(equat2, 2);
							if (lpCount < 1) {DO_SITE(equat3, 3);}
						}
					}
				}
			}

			else if (bonds.size() == 3) {

				float dot12 = DotProduct3D(&vecs[0], &vecs[1]);
				float dot13 = DotProduct3D(&vecs[0], &vecs[2]);
				float dot23 = DotProduct3D(&vecs[1], &vecs[2]);

				// Two vectors are opposite, meaning two are axial.
				if (dot12 < -0.75 || dot13 < -0.75f || dot23 < -0.75f) {

					CPoint3D cross1;
					CPoint3D cross2;
					CPoint3D equat2;
					CPoint3D equat3;
					int axial_id;
					int equat_id;
					Matrix4D rotate_mat;

					if (dot12 < -0.75) {
						axial_id = 0;
						equat_id = 2;
					} else if (dot13 < -0.75f) {
						axial_id = 0;
						equat_id = 1;
					} else if (dot23 < -0.75f) {
						axial_id = 1;
						equat_id = 0;
					}

					CrossProduct3D(&vecs[axial_id], &vecs[equat_id], &cross1);
					CrossProduct3D(&cross1, &vecs[equat_id], &cross2);
					Normalize3D(&cross2);

					RotateAroundAxis(rotate_mat, cross2, 120.0f);
					Rotate3DPt(rotate_mat, vecs[equat_id], &equat2);
					equat3 = (equat2 + vecs[equat_id]) * -1.0f;
					// Don't need to normalize equat3 since it is by
					// construction.

					DO_SITE(equat2, 4);
					if (lpCount < 1) {DO_SITE(equat3, 5);}
				}

				else if (fabs(dot12) < 0.25f || fabs(dot13) < 0.25f ||
						 fabs(dot23) < 0.25f) {

					int axial_id;
					int equat1_id;
					int equat2_id;

					if (fabs(dot12) < 0.25) {
						equat1_id = 2;
						if (fabs(dot13) < 0.25f) {
							axial_id = 0;
							equat2_id = 1;
						} else {
							axial_id = 1;
							equat2_id = 0;
						}
					} else if (fabs(dot13) < 0.25f) {
						equat1_id = 1;
						if (fabs(dot12) < 0.25f) {
							axial_id = 0;
							equat2_id = 2;
						} else {
							axial_id = 2;
							equat2_id = 0;
						}
					} else if (fabs(dot23) < 0.25f) {
						equat1_id = 0;
						if (fabs(dot13) < 0.25f) {
							axial_id = 2;
							equat2_id = 1;
						} else {
							axial_id = 1;
							equat2_id = 2;
						}
					}

					CPoint3D cross;
					CPoint3D equat3;

					CrossProduct3D(&vecs[equat1_id], &vecs[equat2_id], &cross);
					Normalize3D(&cross);
					if (DotProduct3D(&cross, &vecs[axial_id]) > 0.0f) {
						cross *= -1.0f;
					}

					equat3 = (vecs[equat1_id] + vecs[equat2_id]) * -1.0f;
					Normalize3D(&equat3);

					DO_SITE(cross, 5);
					if (lpCount < 1) {DO_SITE(equat3, 4);}
				}

				else {
					CPoint3D cross;

					CrossProduct3D(&vecs[0], &vecs[1], &cross);
					Normalize3D(&cross);

					DO_SITE(cross, 4);
					if (lpCount < 1) {DO_SITE(cross * -1.0f, 5);}
				}

			}

			else if (bonds.size() == 4) {

				float dot_ij;
				bool is_axial = false;
				bool is_equatorial;
				int i, j;

				for (i = 0; i < 4; i++) {
					is_equatorial = false;
					for (j = 0; j < 4; j++) {
						if (i == j) continue;

						dot_ij = DotProduct3D(&vecs[i], &vecs[j]);

						// Vector has an antiparallel mate, so we can't stop
						// now.
						if (dot_ij < -0.75) {
							is_axial = true;
							break;
						}
						
						// Vector has a non-orthogonal mate (which must also
						// be non-antiparallel if we got here), so it can't
						// be an axial vector.
						else if (fabs(dot_ij) > 0.25) {
							is_equatorial = true;
							break;
						}
					}

					if (is_axial) {
						CPoint3D equat3;
						int ids[4] = {0, 1, 2, 3};
						int tmp;
						tmp = ids[0]; ids[0] = i; ids[i] = tmp;
						tmp = ids[1]; ids[1] = j; ids[j] = tmp;
						equat3 = (vecs[ids[2]] + vecs[ids[3]]) * -1.0f;
						Normalize3D(&equat3);
						DO_SITE(equat3, 5);
						break;
					} else if (!is_equatorial) {
						DO_SITE(vecs[i] * -1.0f, 5);
						break;
					}
				}

				if (!is_axial && is_equatorial) {
					CPoint3D cross;
					CrossProduct3D(&vecs[0], &vecs[1], &cross);
					Normalize3D(&cross);
					DO_SITE(cross, 5);
				}
			}

			break;
		case 6:

			if (bonds.size() == 0) {
				CPoint3D e1(1.0f, 0.0f, 0.0f);
				CPoint3D e2(0.0f, 1.0f, 0.0f);
				CPoint3D e3(0.0f, 0.0f, 1.0f);

				DO_SITE(e1, 1);
				if (lpCount < 5) {
					DO_SITE(e1 * -1.0f, 2);
					if (lpCount < 4) {
						DO_SITE(e2, 3);
						if (lpCount < 3) {
							DO_SITE(e2 * -1.0f, 4);
							if (lpCount < 2) {
								DO_SITE(e3, 5);
								if (lpCount < 1) {DO_SITE(e3 * -1.0f, 6);}
							}
						}
					}
				}
			}

			else if (bonds.size() == 1) {
				CPoint3D ortho;
				CPoint3D cross;

				OrthoVector(vecs[0], ortho);
				CrossProduct3D(&vecs[0], &ortho, &cross);

				DO_SITE(vecs[0] * -1.0f, 2);
				if (lpCount < 4) {
					DO_SITE(ortho, 3);
					if (lpCount < 3) {
						DO_SITE(ortho * -1.0f, 4);
						if (lpCount < 2) {
							DO_SITE(cross, 5);
							if (lpCount < 1) {DO_SITE(cross * -1.0f, 6);}
						}
					}
				}
			}

			else if (bonds.size() == 2) {

				// Use their dot product to determine if they're orthogonal or
				// otherwise.
				float dot = DotProduct3D(&vecs[0], &vecs[1]);

				// If the two existing bonds are closer to being orthogonal
				// than parallel (or antiparallel), then we have four bonds
				// to compute.  One bond will be the inverse of the primary
				// bond, two will be the cross product and its inverse of the
				// two bonds, and the fourth will be another cross product, 
				// this time using the primary bond and the first cross
				// product.  It might make more sense to use the inverse of
				// the secondary bond instead, but doing it this way makes at
				// least 5 of the 6 bonds octahedrally aligned.
				if (fabs(dot) < 0.7f) {
					CPoint3D cross;
					CPoint3D cross2;
					CrossProduct3D(&vecs[0], &vecs[1], &cross);
					Normalize3D(&cross);
					CrossProduct3D(&vecs[0], &cross, &cross2);
					Normalize3D(&cross2);

					if (DotProduct3D(&cross2, &vecs[1]) > 0.0f) {
						cross2 *= -1.0f;
					}

					DO_SITE(vecs[0] * -1.0f, 3);
					if (lpCount < 3) {
						DO_SITE(cross2, 4);
						if (lpCount < 2) {
							DO_SITE(cross, 5);
							if (lpCount < 1) {DO_SITE(cross * -1.0f, 6);}
						}
					}
				}

				// Otherwise, the two existing bonds must be antiparallel.
				// In that case, we calculate the other four bonds like so:
				// one bond is chosen from the many that are orthogonal to
				// the primary bond, the next is its inverse, and the last
				// two are the crossproduct and its inverse of the primary
				// bond and its orthogonal mate.
				else {
					CPoint3D ortho;
					CPoint3D cross;

					OrthoVector(vecs[0], ortho);
					CrossProduct3D(&vecs[0], &ortho, &cross);
					Normalize3D(&cross);

					DO_SITE(ortho, 3);
					if (lpCount < 3) {
						DO_SITE(ortho * -1.0f, 4);
						if (lpCount < 2) {
							DO_SITE(cross, 5);
							if (lpCount < 1) {DO_SITE(cross * -1.0f, 6);}
						}
					}
				}

			}

			else if (bonds.size() == 3) {

				float dot12 = DotProduct3D(&vecs[0], &vecs[1]);
				float dot13 = DotProduct3D(&vecs[0], &vecs[2]);
				float dot23 = DotProduct3D(&vecs[1], &vecs[2]);

				// If all three vectors are close to pairwise orthogonal, then
				// they form a basis.  Unfortunately, they may not be exactly
				// independent.  Since we want to suggest orthogonal sites, we
				// choose the primary vector's inverse as the first site.  The
				// second is calculated as the cross product of the primary
				// and secondary bonds, and the third is the cross product of
				// the primary bond and the first cross product.  This makes
				// all three orthogonal.
				if (fabs(dot12) < 0.7f && fabs(dot13) < 0.7f &&
					fabs(dot23) < 0.7f) {

					CPoint3D cross1, cross2;
					CrossProduct3D(&vecs[0], &vecs[1], &cross1);
					Normalize3D(&cross1);
					CrossProduct3D(&vecs[0], &cross1, &cross2);
					Normalize3D(&cross2);

					// The cross products should point in the opposite
					// direction of the existing bonds, so we may need to
					// flip them since they can end up coincident with them.
					if (DotProduct3D(&cross1, &vecs[2]) > 0.0f) {
						cross1 = cross1 * -1.0f;
					}

					if (DotProduct3D(&cross2, &vecs[1]) > 0.0f) {
						cross2 = cross2 * -1.0f;
					}

					DO_SITE(vecs[0] * -1.0f, 4);
					if (lpCount < 2) {
						DO_SITE(cross1, 5);
						if (lpCount < 1) {DO_SITE(cross2, 6);}
					}
				}

				// Otherwise, two of the three bonds are antiparallel.  In this
				// case, we figure out which bond doesn't have an inverse and
				// one that does.  The first suggested bond is the inverse of
				// the unopposed bond, and the second and third are the cross
				// product and its inverse of the unpaired bond and one of the
				// paired bonds.
				else {
					CPoint3D paired_vec;
					CPoint3D ortho_vec;

					if (dot12 < -0.7f) {
						paired_vec = vecs[0];
						ortho_vec = vecs[2];
					} else if (dot13 < -0.7f) {
						paired_vec = vecs[0];
						ortho_vec = vecs[1];
					} else {
						paired_vec = vecs[2];
						ortho_vec = vecs[0];
					}

					CPoint3D cross;
					CrossProduct3D(&paired_vec, &ortho_vec, &cross);
					Normalize3D(&cross);

					DO_SITE(ortho_vec * -1.0f, 4);
					if (lpCount < 2) {
						DO_SITE(cross, 5);
						if (lpCount < 1) {DO_SITE(cross * -1.0f, 6);}
					}
				}

			}

			else if (bonds.size() == 4) {

				int i, j;
				int nuninverted = 0;
				int uninverted[2];
				int ortho1 = -1, ortho2 = -1;
				bool has_no_inverse;

				for (i = 0; i < 4; i++) {
					has_no_inverse = true;
					for (j = 0; j < 4; j++) {
						if (i != j) {
							if (DotProduct3D(&vecs[i], &vecs[j]) < -0.7f) {
								has_no_inverse = false;
								break;
							} else {
								ortho1 = i;
								ortho2 = j;
							}
						}
					}
					if (has_no_inverse) {
						uninverted[nuninverted] = i;
						nuninverted++;
					}
				}

				if (nuninverted == 2) {
					DO_SITE(vecs[uninverted[0]] * -1.0f, 5);
					if (lpCount == 0) {DO_SITE(vecs[uninverted[1]] * -1.0f, 6);}
				}
				
				else {
					CPoint3D cross;
					CrossProduct3D(&vecs[ortho1], &vecs[ortho2], &cross);
					Normalize3D(&cross);

					DO_SITE(cross, 5);
					if (lpCount == 0) {DO_SITE(cross * -1.0f, 6);}
				}

			}

			else if (bonds.size() == 5) {

				int i, j;
				bool has_no_inverse;

				// If there isn't an uninverted bond, just go opposite the
				// primary bond.
				int uninverted = 0;

				for (i = 0; i < 5; i++) {
					has_no_inverse = true;
					for (j = 0; j < 5; j++) {
						if (i != j) {
							if (DotProduct3D(&vecs[i], &vecs[j]) < -0.7f) {
								has_no_inverse = false;
								break;
							}
						}
					}
					if (has_no_inverse) {
						uninverted = i;
						break;
					}
				}

				DO_SITE(vecs[uninverted] * -1.0f, 6);
			}

			break;
	}

	if (site_id == 0) {
		glPopName();
	}

}

/* ------------------------------------------------------------------------- */

void MolDisplayWin::DrawBond(const Bond& bond, const mpAtom& atom1,
							 const mpAtom& atom2, const WinPrefs& Prefs,
							 GLUquadric *quadric, GLdouble *modelview,
							 GLdouble *proj, GLint *viewport,
							 GLuint sphere_list, bool highlighting_on,
							 bool cap_dependent) {

	CPoint3D v1, v2, offset, NormalOffset, NormEnd;
	CPoint3D NormStart(0, 0, 1);
	Matrix4D rotMat;
	long Quality = (long) Prefs.GetQD3DAtomQuality();
	float AtomScale = Prefs.GetAtomScale();
	GLdouble BondSize = Prefs.GetQD3DBondWidth() * 0.5;
	
	BondOrder logical_order = bond.Order;

	if (logical_order == kAromaticBond) logical_order = kDoubleBond;
	if (!Prefs.ColorBondHalves()) logical_order = kSingleBond;	//only generate multiple pipes when colored by atom color

	GLdouble tmpBondSize = BondSize / MAX(logical_order, 1);
	if (logical_order > 1) tmpBondSize *= 1.5;
	GLdouble baseBondOffset = -1.75 * tmpBondSize * (MAX(logical_order, 1) - 1);

	if (atom1.GetInvisibility() || atom2.GetInvisibility()) return;
	
	// If this is a bond that's going to need multiple pipes, we want each pipe
	// to be shifted in screen space so that all pipes are visible all the
	// time.
	CPoint3D offset_vec;     // Direction to shift bond cylinders
	if (logical_order > 1) {
		GLdouble scr_coords1[3]; // Screen coordinates of atom1
		GLdouble scr_coords2[3]; // Screen coordinates of atom2
		CPoint3D scr_vec;        // Screen space vector between atoms
		GLdouble perp_obj[3];    // Object coords on vector perp. to scr_vec

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
		scr_vec *= 1.0f / scr_vec.Magnitude();

		// Now find a point on the perpendicular vector with atom1's depth
		// and get its object coordinates.
		gluUnProject(scr_coords1[0] + scr_vec.x * 10.0f,
					 scr_coords1[1] + scr_vec.y * 10.0f,
					 scr_coords1[2],
					 modelview, proj, viewport,
					 &(perp_obj[0]), &(perp_obj[1]), &(perp_obj[2]));

		// Finally, we see what direction all bond cylinders must be offset
		// so that they will always stay in view.
		offset_vec = CPoint3D(perp_obj[0], perp_obj[1], perp_obj[2]) -
					 atom1.Position;
		offset_vec *= 1.0f / offset_vec.Magnitude();
	}
	
	// If there's only one pipe, we need no offset.
	else {
		offset_vec = CPoint3D(0.0f, 0.0f, 0.0f);
	}

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

		glPushMatrix(); // bond pipe
		glMultMatrixf((const GLfloat *) &rotMat);

		// Both hydrogen bonds and one part of the aromatic bond and drawn
		// as a line of spheres.
		if (bond.Order == kHydrogenBond ||
			(bond.Order == kAromaticBond && ipipe == 1)) {
			DrawPipeSpheres(Prefs, length, BondSize,
							highlighting_on && !bond.GetSelectState(),
							sphere_list);
			glPopMatrix(); // bond pipe
			continue;
		}
		
		// Now, if a bond is selected but not this one, we need to draw an
		// encapsulating cylinder to mask it out.
		if (highlighting_on && !bond.GetSelectState()) {

			glEnable(GL_POLYGON_STIPPLE);
			glPolygonStipple(stippleMask);
			
			glColor3f(0.0f, 0.0f, 0.0f);
			DrawPipeCylinder(length, quadric, Prefs.DrawWireFrame() ? 3 : 0,
							 sphere_list, tmpBondSize * 1.01f, Quality);

			glDisable(GL_POLYGON_STIPPLE);

		}

		// We may need to draw two cylinders if the user wants the bonds
		// colored according to their element.
		if (Prefs.ColorBondHalves()) {
			// Center the color change at the middle of the visible part of the
			// bond.
			float radius1 = AtomScale*Prefs.GetAtomSize(atom1.GetType() - 1);
			float radius2 = AtomScale*Prefs.GetAtomSize(atom2.GetType() - 1);
			float percent1 = radius1/length;
			float percent2 = radius2/length;
			float centerPercent = 0.5 + 0.5 * (percent1 - percent2);
			
			// Draw first half.
			Prefs.ChangeColorAtomColor(atom1.GetType());
			DrawPipeCylinder(length * centerPercent, quadric,
							 (Prefs.DrawWireFrame() ||
							  (cap_dependent && !atom1.IsSymmetryUnique()) ||
							  (Prefs.ShowEFPWireFrame() &&
							   atom1.IsEffectiveFragment())) ? 1 : 0, 
							 sphere_list,
							 tmpBondSize, Quality);

			// Draw second half starting at the weighted halfway point.
			glPushMatrix(); // halfway point
			glTranslatef(0.0f, 0.0f, length * centerPercent);
			Prefs.ChangeColorAtomColor(atom2.GetType());
			DrawPipeCylinder(length * (1.0f - centerPercent), quadric,
							 (Prefs.DrawWireFrame() ||
							  (cap_dependent && !atom2.IsSymmetryUnique()) ||
							  (Prefs.ShowEFPWireFrame() &&
							   atom2.IsEffectiveFragment())) ? 2 : 0,
							 sphere_list, tmpBondSize, Quality);
			glPopMatrix(); // halfway point

		}
		
		// We only need to draw one cylinder the whole length of a bond since
		// the user's not interested in coloring each half differently.
		else {
			Prefs.ChangeColorBondColor(bond.Order);
			DrawPipeCylinder(length, quadric,
							 (Prefs.DrawWireFrame() ||
							  (cap_dependent &&
							   (!atom1.IsSymmetryUnique() ||
							    !atom2.IsSymmetryUnique())) ||
							  (Prefs.ShowEFPWireFrame() &&
							   (atom1.IsEffectiveFragment() ||
								atom2.IsEffectiveFragment()))) ? 3 : 0,
							 sphere_list, tmpBondSize, Quality);
		}

		glPopMatrix(); // bond pipe
	}

}

/* ------------------------------------------------------------------------- */
/**
 * This function draws a series of spheres from the origin along the negative
 * z-axis for length units.  The spheres are optionally drawn with a hatching
 * mask.
 * @param Prefs Prefences used to determine sphere color.
 * @param length Distance along z-axis along which to draw spheres.
 * @param scale_factor Size of spheres relative to current transform.
 * @param is_masked Flag indicating whether to draw stippled overlay to mask
 *                  spheres.
 * @param sphere_list Preassembled display list that draws a sphere.
 */

void DrawPipeSpheres(const WinPrefs& Prefs, float length, float scale_factor,
					 bool is_masked, GLuint sphere_list) {

	glPushMatrix(); // offset from origin

	// Plot as a series of spheres, starting slightly offset from the origin.
	float pos = 0.75f * scale_factor;
	glTranslatef(0.0, 0.0, pos);
	while (pos < length) {
		glPushMatrix(); // scaled sphere location
		glScalef(scale_factor, scale_factor, scale_factor);

		Prefs.ChangeColorBondColor(kHydrogenBond);
		glCallList(sphere_list);

		if (is_masked) {
			glEnable(GL_POLYGON_STIPPLE);
			glPolygonStipple(stippleMask);
			glPushMatrix(); // stipple-scaled sphere location
			glScalef(1.01f, 1.01f, 1.01f);
			glColor3f(0.0f, 0.0f, 0.0f);
			glCallList(sphere_list);
			glPopMatrix(); // stipple-scaled sphere location
			glDisable(GL_POLYGON_STIPPLE);
		}

		glPopMatrix(); // scaled sphere location

		glTranslatef(0.0f, 0.0f, 2.5f * scale_factor);
		pos += 2.5f * scale_factor;
	}

	glPopMatrix(); // offset from origin

}

/* ------------------------------------------------------------------------- */
/**
 * Draws a cylinder from the current transformation's origin along negative z
 * axis for length units.  Assumes modeling transformation is in place, and
 * OpenGL commands are ready to be issued.  Useful for drawing bond pipes that
 * are optionally capped.
 * @param length Extent of the cylinder along the z-axis.
 * @param quadric Preallocated GLU quadric object.
 * @param ncaps Number of end caps to draw.  Should be 1 to draw endcap only at
 *              origin, 2 only at the opposite end, and 3 to draw both.
 * @param sphere_list Preassembled display list id for drawing endcaps, which
 * 					  should be valid if ncaps != 0.
 * @param radius Radius of cylinder.
 * @param quality Measure of "facetedness" of cylinder's geometric
 * 				  representation.
 */

void DrawPipeCylinder(float length, GLUquadric *quadric, unsigned int ncaps,
					  GLuint sphere_list, float radius, long quality) {

	gluCylinder(quadric, radius, radius, length, quality,
				(long) (0.5f * quality));

	// Draw sphere at end0 if requested.
	if (ncaps & 1) {
		glPushMatrix(); // end0
		glScalef(radius, radius, radius);
		glCallList(sphere_list);
		glPopMatrix(); // end0
	}

	// Draw sphere at end1 if requested.
	if (ncaps & 2) {
		glPushMatrix(); // end1
		glTranslatef(0.0f, 0.0f, length);
		glScalef(radius, radius, radius);
		glCallList(sphere_list);
		glPopMatrix(); // end1
	}

}

/* ------------------------------------------------------------------------- */

GLuint GetShaderProgramFromFiles(const std::string& vert_filename,
                                 const std::string& frag_filename) {

	std::string vert_src;
	std::string frag_src;

	if (!vert_filename.empty()) {
		if (!FileToString(vert_filename, vert_src)) {
			return 0;
		}
	}

	if (!frag_filename.empty()) {
		if (!FileToString(frag_filename, frag_src)) {
			return 0;
		}
	}

	return GetShaderProgram(vert_src, frag_src);

}

/* ------------------------------------------------------------------------- */

GLuint GetShaderProgram(const std::string& vert_src,
						const std::string& frag_src) {

	GLuint shader_prog = 0;

	GLint succeeded;
	char *log;
	GLint log_length=0;
	GLint chars_written;

	shader_prog = glCreateProgram();

	if (vert_src.length()) {
		GLuint vert_shader;
		const char *v2 = &vert_src.c_str()[0];

		// Vertex shader.
		vert_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vert_shader, 1, &v2, NULL);
		glCompileShader(vert_shader);
		glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &succeeded);

		glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 1) {
			log = new char[log_length];
			glGetShaderInfoLog(vert_shader, log_length, &chars_written, log);
			std::cout << "log: " << log << std::endl;
			delete[] log;
		}

		if (succeeded != GL_TRUE) {
			std::cout << "Vertex Program: " << vert_src << std::endl;
			wxLogMessage(wxT("Something went wrong with the shader."));
		}

		glAttachShader(shader_prog, vert_shader);
	}

	// Fragment shader.
	if (frag_src.length()) {
		GLuint frag_shader;
		const char *f2 = &frag_src.c_str()[0];
		frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag_shader, 1, &f2, NULL);

		glCompileShader(frag_shader);
		glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &succeeded);

		glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length > 1) {
			log = new char[log_length];
			glGetShaderInfoLog(frag_shader, log_length, &chars_written, log);
			std::cout << "log: " << log << std::endl;
			delete[] log;
		}

		if (succeeded != GL_TRUE) {
			std::cout << "Fragment Program: " << frag_src << std::endl;
			wxLogMessage(wxT("Something went wrong with the shader."));
		}

		glAttachShader(shader_prog, frag_shader);
	}

	glLinkProgram(shader_prog);
	glGetProgramiv(shader_prog, GL_LINK_STATUS, &succeeded);
	if (succeeded != GL_TRUE) {
		char *log;
		GLint log_length;
		GLint chars_written;
		glGetProgramiv(shader_prog, GL_INFO_LOG_LENGTH, &log_length);
		if (log_length) {
			log = new char[log_length];
			glGetProgramInfoLog(shader_prog, log_length, &chars_written, log);
			std::cout << "log: " << log << std::endl;
			delete[] log;
			exit(1);
		} else {
			wxLogMessage(wxT("Something went wrong with the shader program."));
		}
	}

	return shader_prog;

}

/* ------------------------------------------------------------------------- */

/**
 This function (re)generates any structures that are dependent on preference
 settings. It should be called when preferences change. It assumes that a valid
 OpenGL context exists and is current, so it should not be called by a parent's
 constructor.
 */
void MolDisplayWin::DoPrefDependent() {

	// All atoms are drawn using one display list that draws a sphere. 
	// We delete any current list and create it anew.
	if (sphere_list) {
		glDeleteLists(sphere_list, 1);
	}

	GLUquadric *quad = gluNewQuadric();
	gluQuadricOrientation(quad, GLU_OUTSIDE);
	gluQuadricNormals(quad, GLU_SMOOTH);

	sphere_list = glGenLists(1);
	glNewList(sphere_list, GL_COMPILE);

#if 1
	float *verts;
	int *faces;
	int nverts;
	int nfaces;
	float *normals;
	int nlevels;

	// We do some funny mapping from display quality (2 - 40) to number of
	// subdivisions.  2 maps to 0 levels, while all other values are mapped
	// such that 13 -> 3 levels and 40 -> 7 levels.  Levels >7 recurse too
	// much.
	if (Prefs->GetQD3DAtomQuality() == 2) {
		nlevels = 0;
	} else {
		nlevels = (int) ((Prefs->GetQD3DAtomQuality() - 13.0f) *
						 (4.0f / 27.0f) + 3.0f);
	}

	GenerateOctahedron(nlevels, &verts, nverts, &faces, nfaces, &normals);
	glVertexPointer(3, GL_FLOAT, 0, verts);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, verts);
	glNormalPointer(GL_FLOAT, 0, normals);
	glDrawElements(GL_TRIANGLES, nfaces * 3, GL_UNSIGNED_INT, faces);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	delete[] verts;
	delete[] faces;
	delete[] normals;
#else
	gluSphere(quad, 1.0f, (long) (1.5f * Prefs->GetQD3DAtomQuality()),
			  (long) (Prefs->GetQD3DAtomQuality()));
#endif

	glEndList();

	gluDeleteQuadric(quad);
		
	// Set the background color to the user's preference.
	RGBColor *BackgroundColor = Prefs->GetBackgroundColorLoc();
	float red, green, blue;
	red = (float) BackgroundColor->red / 65536;
	green = (float) BackgroundColor->green / 65536;
	blue = (float) BackgroundColor->blue / 65536;
	glClearColor(red, green, blue, 1.0f);

	// The foreground color is white or black, whichever has better contrast given
	// the background color.
	long backMagnitude = BackgroundColor->red + BackgroundColor->green + BackgroundColor->blue;
	if (backMagnitude > 70000)
		fg_color[0] = fg_color[1] = fg_color[2] = 0.0f;
	else
		fg_color[0] = fg_color[1] = fg_color[2] = 1.0f;
	
	// Setup two lights on either side of the camera.
	float fillBrightness = Prefs->GetQD3DFillBrightness();
	float PointBrightness = Prefs->GetQD3DPointBrightness();
	GLfloat diffuse[4]  = {fillBrightness, fillBrightness, fillBrightness, 0.0f};
	GLfloat specular[4] = {PointBrightness, PointBrightness, PointBrightness, 0.0f};
	GLfloat ambient[4] = {0.2f, 0.2f, 0.2f, 1.0};

	// Make sure we're in eye space so that the lights always
	// illuminate the visible scene.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glEnable(GL_LIGHT0);

	/* position[0] = -6.0; */
	/* ambient[0] = ambient[1] = ambient[2] = 0.0f; */
	/* glLightfv(GL_LIGHT1, GL_AMBIENT, ambient); */
	/* glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse); */
	/* glLightfv(GL_LIGHT1, GL_SPECULAR, specular); */
	/* glLightfv(GL_LIGHT1, GL_POSITION, position); */
	/* glEnable(GL_LIGHT1); */

	if (GLEW_VERSION_2_0) {
		if (depth_fbo) {
			glDeleteFramebuffersEXT(1, &(depth_fbo));
			depth_fbo = 0;
		}

		wxString vector_font, bitmap_font;
		wxStandardPathsBase & gStdPaths = wxStandardPaths::Get();
#if wxCHECK_VERSION(2, 8, 0)
		wxString pathname = gStdPaths.GetResourcesDir();
#else
		wxString pathname = gStdPaths.GetDataDir();
#ifdef __WXMAC__
		//wxWidgets has a funny idea of where the resources are stored. It locates them as "SharedSupport"
		//but xcode is putting them in Resources.
		pathname.Remove(pathname.Length() - 13);
		pathname += wxT("Resources");
#endif
#endif

		std::string vpath, fpath;
		if (Prefs->GetShaderMode() == 1) {
			vpath = std::string(pathname.ToAscii()) + "/perpixel_dirlight_v.glsl";
			fpath = std::string(pathname.ToAscii()) + "/perpixel_dirlight_f.glsl";
			shader_program = GetShaderProgramFromFiles(vpath, fpath);
		} else if (Prefs->GetShaderMode() == 2) {
			vpath = std::string(pathname.ToAscii()) + "/shadows_v.glsl";
			fpath = std::string(pathname.ToAscii()) + "/shadows_f.glsl";
			shader_program = GetShaderProgramFromFiles(vpath, fpath);

			glGenTextures(1, &(depth_tex_id));
			glBindTexture(GL_TEXTURE_2D, depth_tex_id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, FBO_SIZE,
						 FBO_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
			glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

			glGenFramebuffersEXT(1, &depth_fbo);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, depth_fbo);
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
									  GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D,
									  depth_tex_id, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);

			GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
				std::cout << "Bad status: << " << (int) status << std::endl;
			}

			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}

		if (shader_program) {
			glUseProgram(shader_program);

			CPoint3D lightvec(light_pos[0], light_pos[1], light_pos[2]);
			Normalize3D(&lightvec);
			GLint light_pos_loc = glGetUniformLocation(shader_program, "light_dir");
			if (light_pos_loc >= 0) {
				glUniform3f(light_pos_loc, lightvec.x, lightvec.y, lightvec.z);
			} else {
				std::cerr << "Can't set shader uniforms." << std::endl;
			}

			GLint uniform_loc = glGetUniformLocation(shader_program, "depth_map");
			if (uniform_loc >= 0) {
				glUniform1i(uniform_loc, 0);
			}

			glUseProgram(0);
		}
	}
				
}

