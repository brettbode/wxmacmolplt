/*
 * (c) 2005-2006 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * mpGLCanvas.cpp
 ***************************************/

#include "Globals.h"
#include "mpGLCanvas.h"
#include <wx/image.h>
#include "MolDisplayWin.h"
#include "glf.h"

#ifdef __wxBuild__
#include <wx/stdpaths.h>
#endif
#ifdef __WXMAC__
#include <AGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <iostream>
#include <sstream>

#include "main.h"
#include "periodic_table_dlg.h"
#include "Math3D.h"
#include "VirtualSphere.h"

extern PeriodicTableDlg *periodic_dlg;
extern bool show_periodic_dlg;

int defAttribs[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

MpGLCanvas::MpGLCanvas(MolDisplayWin  *parent,
							  wxWindowID id,
							  const wxPoint  &position,
							  const wxSize &size,
							  long style,
							  const wxString &name)
			  :wxGLCanvas((wxWindow *)parent, id, position, size,
							  style|wxFULL_REPAINT_ON_RESIZE, name, defAttribs) {
	//There is an additional parameter to wxGLCanvas that is an int array
	//of GL options we might need. Might need to use WX_GL_RGBA and
	//WX_GL_DOUBLEBUFFER?
	Prefs = NULL;
	MolWin = parent;
	initialized = false;

	mMainData = parent->GetData();

	mSelectState = -1;
	interactiveMode = false;
	oldSelect = -1;
	mDragWin = NULL;
	select_stack_top = 0;
	stale_click = false;

	//Hmm is this the right spot to initialize our GL settings?
	//initGL();
}

MpGLCanvas::~MpGLCanvas() {
	if (periodic_dlg && periodic_dlg->GetParent() == this) {
		ClosePeriodicDlg();
	}
}

void MpGLCanvas::initGL(void) {
	if(GetContext()) {
		// Initialize the OpenGL context here.
		glEnable(GL_DEPTH_TEST);
		
		//  glShadeModel(GL_FLAT);
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

void MpGLCanvas::setPrefs(WinPrefs *newPrefs) {
	Prefs = newPrefs;
}

wxImage MpGLCanvas::getImage(const int width, const int height) {
	if(!GetContext()) {
		return wxImage();
	}
	SetCurrent();
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
	pixels = (unsigned char *) malloc(3 * cwidth * cheight * sizeof(GLbyte));
	memset(pixels, 0, 3*cwidth*cheight*sizeof(GLbyte));
	glPixelStorei( GL_PACK_ALIGNMENT, 1 );
	//draw into the back buffer
	MolWin->DrawGL();
	glFinish();
	glReadBuffer( GL_BACK );
	glReadPixels( 0, 0, cwidth, cheight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	// create a wxImage from the data, and mirror it vertically
	return wxImage(cwidth,cheight,pixels).Mirror(false);
}
//0.0577 corresponds to fov=60 with zNear=0.1
//#define myGLperspective 0.050	//0.050 seems to match my 2D mode

void MpGLCanvas::GenerateHiResImage(wxDC * dc, const float & ScaleFactor, 
												Progress * progress, bool Center,
												bool frame) {
	
	if (!GetContext()) {
		return;
	}
	SetCurrent();

	GLint view[4];
	GLint width,height;
	
	glGetIntegerv(GL_VIEWPORT, view);
	width = view[2];
	height = view[3];
	int ScaledWidth = (int) (width * ScaleFactor);
	int ScaledHeight = (int) (height * ScaleFactor);
	int NumXPasses = ScaledWidth / width;
	if ((NumXPasses * width) < ScaledWidth) NumXPasses++;
	int NumYPasses = ScaledHeight / height;
	if ((NumYPasses * height) < ScaledHeight) NumYPasses++;
	int ViewportScaledX = NumXPasses * width;
	int ViewportScaledY = NumYPasses * height;

	long hOffset=0, vOffset=0;
	if (Center) {   //Compute the offset to move the rect to center on the page
		int PageCenterH, PageCenterV;
		dc->GetSize(&PageCenterH, &PageCenterV);
		PageCenterH /= 2;
		PageCenterV /= 2;
		long DCenterH = ScaledWidth/2;
		long DCenterV = ScaledHeight/2;
		vOffset = PageCenterV - DCenterV;
		hOffset = PageCenterH - DCenterH;
	}
	
	unsigned char *pixels = (unsigned char *) malloc(3 * width * height * 
									sizeof(GLbyte));
	glReadBuffer(GL_BACK);
	 
	GLdouble zNear = 0.1;
	GLdouble myGLperspective = zNear*tan(Prefs->GetGLFOV()*(kPi)/180.0);
	GLdouble hGLsize, vGLsize, GLLeft, GLTop;
	double aspect = ((double)width)/((double)height);
	if (aspect > 1.0) {
		hGLsize = 2.0*(myGLperspective) / ScaleFactor;  //This corresponds to fov=60 with zNear=0.1
		vGLsize = hGLsize/aspect;
		GLLeft = -myGLperspective;
		GLTop = -GLLeft/aspect;
	} else {
		vGLsize = 2.0*(myGLperspective) / ScaleFactor;
		hGLsize = vGLsize * aspect;
		GLTop = myGLperspective;
		GLLeft = -GLTop *aspect;
	}
	
	MolWin->UpdateGLModel();
	
	for (int jpass=0; jpass<NumYPasses; jpass++) {
		int passheight = height;
		if ((jpass+1) == NumYPasses) passheight = height - (ViewportScaledY - ScaledHeight);
		for (int ipass=0; ipass<NumXPasses; ipass++) {
			int passwidth = width;
			if ((ipass+1) == NumXPasses) passwidth = width - (ViewportScaledX - ScaledWidth);
			
			//Setup the projection matrix to view the correct piece of the view for this pass
			glMatrixMode (GL_PROJECTION);   //Setup the model space to screen space mapping
			glLoadIdentity ();
			GLdouble top, bottom, left, right;
			left = GLLeft + ipass*hGLsize;
			right = left + hGLsize;
			top = GLTop - jpass*vGLsize;
			bottom = top - vGLsize;
			glFrustum(left, right, bottom, top, zNear, 1000.0);
			
			MolWin->DrawGL();
			
			glFinish();
			memset(pixels, 0, 3*width*height*sizeof(GLbyte));
			glPixelStorei( GL_PACK_ALIGNMENT, 1 );
			glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

			wxCoord x = ipass*width + hOffset;
			wxCoord y = jpass*height + vOffset;
			//I am not sure this is the best way to do this, but it
			//converts the pixels to a wxImage, flips it vertically to get
			//it into the right orientation, then converts it to a bitmap
			//to draw into the device context. We retain ownership of the pixels
			//and thus have to delete them below.
			dc->DrawBitmap(wxBitmap(wxImage(width,height,pixels, true).Mirror(false)),
							x, y, false);
			 
		}
	}
	delete [] pixels;
	 
	if (frame) {
		wxPen lpen = dc->GetPen();
		lpen.SetWidth((int)ScaleFactor);
		dc->SetBrush(*wxTRANSPARENT_BRUSH);
		dc->SetPen(lpen);
		dc->DrawRectangle(hOffset, vOffset, ScaledWidth, ScaledHeight);
	}
	 MolWin->UpdateGLModel();
	 UpdateGLView();
}

void MpGLCanvas::GenerateHiResImageForExport(wxDC *dc) {
	//wxPen dcPen(wxT("BLACK"));

	wxCoord dcWidth  = 0;
	wxCoord dcHeight = 0;

	GLint glViewport[4];
	GLint canvasWidth  = 0;
	GLint canvasHeight = 0;

	int numPassesX = 0;
	int numPassesY = 0;

	int passWidth  = 0;
	int passHeight = 0;

	int edgePassWidth = 0;
	int edgePassHeight = 0;

	int passX = 0;
	int passY = 0;

	long hOffset = 0;
	long vOffset = 0;

	GLbyte *pixels;

	if (!GetContext()) {
		return;
	}
	SetCurrent();

	glGetIntegerv(GL_VIEWPORT, glViewport);
	canvasWidth  = glViewport[2];
	canvasHeight = glViewport[3];

	dc->GetSize(&dcWidth, &dcHeight);

	numPassesX = dcWidth / canvasWidth;
	if (dcWidth % canvasWidth) numPassesX++;

	numPassesY = dcHeight / canvasHeight;
	if (dcHeight % canvasHeight) numPassesY++;

	edgePassWidth  = (canvasWidth  * numPassesX) - dcWidth;
	edgePassHeight = (canvasHeight * numPassesY) - dcHeight;

	pixels = (GLbyte *)malloc(canvasWidth * canvasHeight * 3 * sizeof(GLbyte));

	glReadBuffer(GL_BACK);

	GLdouble zNear = 0.1;
	GLdouble myGLperspective = zNear*tan(Prefs->GetGLFOV()*(kPi)/180.0);
	GLdouble hGLsize, vGLsize, GLLeft, GLTop;
	double aspect = ((double)canvasWidth)/((double)canvasHeight);
	if (aspect > 1.0) {
		hGLsize = 2.0*(myGLperspective) / ((double)dcWidth/(double)canvasWidth);  //This corresponds to fov=60 with zNear=0.1
		vGLsize = hGLsize/aspect;
		GLLeft = -myGLperspective;
		GLTop = -GLLeft/aspect;
	} else {
		vGLsize = 2.0*(myGLperspective) / ((double)dcWidth/(double)canvasWidth);
		hGLsize = vGLsize * aspect;
		GLTop = myGLperspective;
		GLLeft = -GLTop *aspect;
	}
	
	MolWin->UpdateGLModel();
	
	for(int passY = 0; passY < numPassesY; passY++) {
		if ((passY + 1) == numPassesY) {
			passHeight = edgePassHeight;
		}
		else {
			passHeight = canvasHeight;
		}
		for (int passX = 0; passX < numPassesX; passX++) {
			if ((passX + 1) == numPassesX) {
				passWidth = edgePassWidth;
			}
			else {
				passWidth = canvasWidth;
			}
			
			//Setup the projection matrix to view the correct piece of the view for this pass
			glMatrixMode (GL_PROJECTION);   //Setup the model space to screen space mapping
			glLoadIdentity ();
			GLdouble top, bottom, left, right;
			left = GLLeft + passX * hGLsize;
			right = left + hGLsize;
			top = GLTop - passY * vGLsize;
			bottom = top - vGLsize;
			glFrustum(left, right, bottom, top, zNear, 1000.0);
			
			MolWin->DrawGL();
			
			glFinish();
			memset(pixels,0, canvasWidth * canvasHeight * 3 * sizeof(GLbyte));
			glPixelStorei( GL_PACK_ALIGNMENT, 1 );
			glReadPixels(0, 0, canvasWidth, canvasHeight, GL_RGB,
						 GL_UNSIGNED_BYTE, pixels);

			wxCoord x = passX * canvasWidth;
			wxCoord y = passY * canvasHeight;
			//I am not sure this is the best way to do this, but it
			//converts the pixels to a wxImage, flips it vertically to get
			//it into the right orientation, then converts it to a bitmap
			//to draw into the device context. We retain ownership of the pixels
			//and thus have to delete them below.
			dc->DrawBitmap(wxBitmap(wxImage(canvasWidth, canvasHeight,
											(unsigned char *)pixels,
											true).Mirror(false)),
						   x, y, false);
			
		}
	}
	delete [] pixels;
	
	MolWin->UpdateGLModel();
	UpdateGLView();
}
void MpGLCanvas::UpdateGLView(void) {
	int width, height;
	if(GetContext()&&(Prefs!=NULL)&&MolWin->IsShown()) {
		SetCurrent();
		GetClientSize(&width, &height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		GLdouble aspect = ((float)width)/height;
		//  GLdouble ysize = 60.0;
		//  if (aspect > 1.0) ysize /= aspect;
		glMatrixMode (GL_PROJECTION);   //Setup the model space to screen space mapping
		glLoadIdentity ();
		GLdouble zNear = 0.1;
		GLdouble myGLperspective = zNear*tan(Prefs->GetGLFOV()*(kPi)/180.0);
		//At the moment the prefs limit the GLFOV to > 0 so the glOrtho code will not
		//get run. Before it can be activated need to figure out what to do with it in the 
		//hi-res image export routines.
		//	  gluPerspective(ysize, aspect, 0.1, 100.0);
		GLdouble top, right;
		if (aspect > 1.0) {
			right = myGLperspective;
			top = right/aspect;
		} else {
			top = myGLperspective;
			right = top * aspect;
		}
		if (myGLperspective > 0.001)
			glFrustum(-right, right, -top, top, zNear, 1000.0);
		else {
			if (aspect > 1.0) {
				right = mMainData->WindowSize;
				top = right/aspect;
			} else {
				top = mMainData->WindowSize;
				right = top * aspect;
			}
			glOrtho(-right, right, -top, top, zNear, 1000.0);
		}
		glMatrixMode (GL_MODELVIEW);	//Prepare for model space by submitting the rotation/translation
		glLoadIdentity ();
		
		RGBColor * BackgroundColor = Prefs->GetBackgroundColorLoc();
		float red, green, blue;
		red = (float) BackgroundColor->red/65536;
		green = (float) BackgroundColor->green/65536;
		blue = (float) BackgroundColor->blue/65536; //Set the color to the Vector color
		glClearColor(red, green, blue, 1.0f);	   // Setup the background "clear" color
		
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
	if(!GetContext()||!MolWin->IsShown()||(Prefs==NULL)) {
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear out the buffer
	
	SwapBuffers();
}
void MpGLCanvas::eventErase(wxEraseEvent &event) {
	// Don't mess with this.  It's supposed to be empty.
	// This avoids flashing on Windows.
}

void MpGLCanvas::eventActivate(wxActivateEvent &event) {
	if (event.GetActive()) {
		stale_click = true;
		wxMouseEvent mouse_event(wxEVT_LEFT_DOWN);
		AddPendingEvent(mouse_event);
	}
}

void MpGLCanvas::ConstrainPosition(const int anno_id, double *x, double *y,
									double *z) {

	Frame *lFrame = mMainData->cFrame;
	Annotation *anno = mMainData->Annotations[anno_id];

	// If the lock annotation doesn't contain the clicked on atom, don't
	// constrain its translation.
	if (!anno->containsAtom(selected)) {
		return;
	}

	// Now, how we constrain the translation is dependent on the annotation
	// type.
	switch (anno->getType()) {

		// If we're dealing with a length annotation, we confine translation
		// to occur only along the vector defined by the two atoms.  That is,
		// only the magnitude of the vector can change.  We do this by
		// project the mouse's position in object space onto the vector between
		// the two atoms.
		case MP_ANNOTATION_LENGTH: {

			int shifter_id;          // The id of the atom being moved 
			CPoint3D shifter_pos;    //   and its position.
			int anchor_id;           // The id of the anchored atom 
			CPoint3D anchor_pos;     //   and its position.
			float len;               // Length of vector between two atoms.
			CPoint3D bond_vec;       // Vector from anchor atom to shift atom.
			CPoint3D new_vec;        // Vector from anchor atom to mouse pt.
			float mu;

			// Figure out which atom is moving and which is planted.
			shifter_id = anno->getAtom(0);
			if (shifter_id == selected) {
				anchor_id = anno->getAtom(1);
			} else {
				anchor_id = shifter_id;
				shifter_id = anno->getAtom(1);
			}

			lFrame->GetAtomPosition(anchor_id, anchor_pos);
			lFrame->GetAtomPosition(shifter_id, shifter_pos);

			// Determine vectors from anchor atom to the shift atom and the
			// mouse point in object coordinates.
			bond_vec = shifter_pos - anchor_pos;

			new_vec.x = *x - anchor_pos.x;
			new_vec.y = *y - anchor_pos.y;
			new_vec.z = *z - anchor_pos.z;

			// Now we project the mouse point onto the vector between the
			// atoms, provided the mouse point is sufficiently far away from
			// the vector already.  If it's not, mu will be zero and the
			// shifted atom will be coincident with the anchor atom, and we'll
			// thereby lose all direction information.
			
			len = bond_vec.Magnitude();
			if (len < 1e-6) {
				return;
			}

			mu = DotProduct3D(&bond_vec, &new_vec) / (len * len);

			if (fabs(mu) > 1e-6) {
				*x = anchor_pos.x + bond_vec.x * mu;
				*y = anchor_pos.y + bond_vec.y * mu;
				*z = anchor_pos.z + bond_vec.z * mu;
			}

			break;

		}

		// If we're dealing with an angle annotation, we confine translation to
		// occur only along the circumference of the circle defined with the
		// angle's vertex atom at its center and its radius the length of the
		// vector between the vertex atom and the shift atom.  That is, only
		// the angle between the shifting atom, vertex atom, and the third atom
		// can change.  Lengths between atoms do not change.  We do this by
		// projecting the mouse's position in object space onto the plane
		// defined by the three atoms, and projecting that plane position onto
		// the circle.
		case MP_ANNOTATION_ANGLE: {

			int atom1_id;         // ID of one non-vertex atom
			CPoint3D atom1_pos;   //   and its position.
			int atom2_id;         // ID of other non-vertex atom
			CPoint3D atom2_pos;   //   and its position.
			int vertex_id;        // ID of vertex atom 
			CPoint3D vertex_pos;  //   and its position.
			CPoint3D vec1;        // Vector from vertex atom to atom1.
			CPoint3D vec2;        // Vector from vertex atom to atom2.
			CPoint3D new_pos;     // Position of mouse point on angle's plane.
			float radius;         // Length between shift atom and vertex.
			CPoint3D normal;      // Normal of angle's plane.
			CPoint3D vec_new;     // Vector between vertex and new_pos.
			float dist;           // Mouse point's distance from plane.

			// Grab the vertex atom for the angle.  If it's also the one trying
			// to be moved, the user must not be trying to constrain anything,
			// so we let it move freely.  Or if its not being moved but is
			// selected, we don't constrain anything.  Doing otherwise would
			// really mess up the goal of constrained movement, since this atom
			// serves as the basis for the constraints.
			vertex_id = anno->getAtom(1);
			if (selected == vertex_id || lFrame->GetAtomSelection(vertex_id)) {
				return;
			}

			// Grab other two atoms and all positions.  It doesn't really
			// matter which position is selected and which isn't, except for
			// determining whose length from the vertex will serve as the
			// radius.  We mostly want to determine the normal's plane, which
			// only considers the two vectors to these positions.
			atom1_id = anno->getAtom(0);
			atom2_id = anno->getAtom(2);

			lFrame->GetAtomPosition(vertex_id, vertex_pos);
			lFrame->GetAtomPosition(atom1_id, atom1_pos);
			lFrame->GetAtomPosition(atom2_id, atom2_pos);

			// Find vectors from vertex atom to other atoms.
			vec1 = atom1_pos - vertex_pos;
			vec2 = atom2_pos - vertex_pos;

			// We want to find the distance between the vertex atom and the
			// one clicked on.  That distance will serve as the radius of
			// the circle whose circumference the atom will follow.  If the
			// other atom is selected, we abandon the constraints because
			// moving both atoms undoes the work of the constrained
			// translation.
			if (selected == atom1_id) {
				radius = vec1.Magnitude();
				if (lFrame->GetAtomSelection(atom2_id)) {
					return;
				}
			} else {
				radius = vec2.Magnitude();
				if (lFrame->GetAtomSelection(atom1_id)) {
					return;
				}
			}

			// We need to take the mouse point in object space and find the
			// nearest point on the plane defined by the three angle atoms.
			// Once that point is on the plane, we can more easily bring it
			// onto the circle's circumference that the atom can move along.
			
			// Calculate the plane's normal.
			CrossProduct3D(&vec1, &vec2, &normal);
			Normalize3D(&normal);

			new_pos.x = *x;
			new_pos.y = *y;
			new_pos.z = *z;

			// Find vector between the mouse point and vertex atom.  The
			// point's distance from the plane is just the dot product of the
			// plane's normal and the vector.
			vec_new = new_pos - vertex_pos;
			dist = DotProduct3D(&normal, &vec_new);

			// Now, drop that point down to the plane in the direction reverse
			// to the normal, by the correct distance.  After this, the 
			// point is on the plane.
			new_pos = new_pos - normal * dist;

			// Now, we need to reel (or cast) the plane point so it's the
			// same distance away as the atom originally was from the vertex.
			vec_new = new_pos - vertex_pos;
			Normalize3D(&vec_new);

			*x = vertex_pos.x + radius * vec_new.x;
			*y = vertex_pos.y + radius * vec_new.y;
			*z = vertex_pos.z + radius * vec_new.z;

			break;

		}

		// If we're dealing with a dihedral annotation, we confine translation
		// to occur only for the first and fourth atoms along the circumference
		// of the circle defined with the atom's projection onto the dihedral's
		// central axis and with the radius determined by the atom's distance
		// to this projection.  Effectively, this allows only the angle of
		// the dihedral to change, with all other lengths preserved.
		case MP_ANNOTATION_DIHEDRAL: {

			CPoint3D shifted_pos;
			int atom2_id;
			CPoint3D atom2_pos;
			int atom3_id;
			CPoint3D atom3_pos;
			CPoint3D vec1;
			CPoint3D vec2;
			CPoint3D new_pos;
			CPoint3D center_pos;
			float radius;
			CPoint3D normal;
			CPoint3D vec_new;
			float vec1_len;
			float dist;
			int origin_id;
			CPoint3D origin_pos;
			float mu;

			// First we need to figure out if its the first or fourth atom
			// of the annotation that's being moved.  We allow only one atom
			// of the annotation to be selected.  If this is not true, or
			// its the second or third atom being shifted, then we impose
			// no constraints.
			if (selected == anno->getAtom(0)) {
				if (lFrame->GetAtomSelection(anno->getAtom(3))) {
					return;
				}
				atom2_id = anno->getAtom(1);
				atom3_id = anno->getAtom(2);
			} else if (selected == anno->getAtom(3)) {
				if (lFrame->GetAtomSelection(anno->getAtom(0))) {
					return;
				}
				atom2_id = anno->getAtom(2);
				atom3_id = anno->getAtom(1);
			} else {
				return;
			}

			if (lFrame->GetAtomSelection(atom2_id) ||
				lFrame->GetAtomSelection(atom3_id)) {
				return;
			}

			// This process is essentially an adaptation of the angle
			// annotation constraint.  The difference is that the circle is
			// centered not necessarily on an atom, but on the shifted atom's
			// projection onto the dihedral set's shared axis.  (A dihedral set
			// of atoms has four atoms, three vectors, and two angles.  The
			// shared axis is the vector between the second and third atoms.)
			// So, we first project the shifted atom onto this vector.
			lFrame->GetAtomPosition(selected, shifted_pos);
			lFrame->GetAtomPosition(atom2_id, atom2_pos);
			lFrame->GetAtomPosition(atom3_id, atom3_pos);

			vec1 = atom2_pos - atom3_pos;
			vec2 = shifted_pos - atom3_pos;

			vec1_len = vec1.Magnitude();
			mu = DotProduct3D(&vec1, &vec2) / (vec1_len * vec1_len);

			if (fabs(mu) > 1e-6f) {
				center_pos = atom3_pos + vec1 * mu;
			} else {
				center_pos = shifted_pos;
			}

			// center_pos now contains the projected point.  We need to figure
			// out the circle's radius, which is just the distance between
			// the center and the atom being shifted.  center_pos is considered
			// the vertex, and the plane passes through this point and is
			// perpendicular to the shared vector.
			vec2 = shifted_pos - center_pos;
			radius = vec2.Magnitude();

			// We need to take the mouse point in object space and find the
			// nearest point on the plane defined by the three angle atoms.
			// Once that point is on the plane, we can more easily bring it
			// onto the circle path that the atom can move along.
			
			// Calculate the plane's normal.  We already calculated the normal
			// above -- it's the shared vector.
			Normalize3D(&vec1);

			new_pos.x = *x;
			new_pos.y = *y;
			new_pos.z = *z;

			// Find vector between the mouse point and vertex atom.  The
			// point's distance from the plane is just the dot product of the
			// plane's normal and the vector.
			vec_new = new_pos - center_pos;
			dist = DotProduct3D(&vec1, &vec_new);

			// Now, drop that point down to the plane in the direction reverse
			// to the normal, by the correct distance.  After this, the 
			// point is on the plane.
			new_pos = new_pos - vec1 * dist;

			// Now, we need to reel (or cast) the plane point so it's the
			// same distance away as the atom originally was.
			vec_new = new_pos - center_pos;
			Normalize3D(&vec_new);

			*x = center_pos.x + radius * vec_new.x;
			*y = center_pos.y + radius * vec_new.y;
			*z = center_pos.z + radius * vec_new.z;

			break;
		}
	}
}

void MpGLCanvas::eventMouse(wxMouseEvent &event) {

	wxPoint tmpPnt;

	if (!GetContext()) {
		return;
	}

	static wxPoint oldTmpPnt;
	bool deSelectAll = true;
	bool edited_atoms = false;
	int width, height;

	// stale_click will be true when the window gains focus after having not
	// had it, i.e., when the application first starts or is backgrounded.  If
	// this is the case, the former mouse position will be invalid.
	// Furthermore, on the Mac at least, if the same click that brings focus to
	// the window is used to change the scene, no mouse event is triggered for
	// the first click.  So, we fake a mouse event on focus.  The position has
	// to be acquired a bit differently, then, since the fake event will hold
	// the wrong position.
	if (stale_click) {
		tmpPnt = MolWin->ScreenToClient(wxGetMousePosition());
		oldTmpPnt = tmpPnt;
	} else {
		tmpPnt = event.GetPosition();
		if (tmpPnt.x == 0 && tmpPnt.y == 0) {
			tmpPnt = MolWin->ScreenToClient(wxGetMousePosition());
			oldTmpPnt = tmpPnt;
			stale_click = true;
		}
	}

	SetCurrent();

	Frame *lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;
	mpAtom *lAtoms = lFrame->Atoms;

	GetClientSize(&width, &height);

	// if (1) { 
		// printf("event.LeftDown(): %d\n", event.LeftDown()); 
		// printf("event.LeftUp(): %d\n", event.LeftUp()); 
		// printf("event.RightDown(): %d\n", event.RightDown()); 
		// printf("event.RightUp(): %d\n", event.RightUp()); 
		// printf("event.Dragging(): %d\n", event.Dragging()); 
		// printf("event.CmdDown(): %d\n", event.CmdDown()); 
		// printf("event.ShiftDown(): %d\n", event.ShiftDown()); 
	// } 
	
	if (event.Entering() && MolWin->LassoSelected()) {
		stale_click = true;
	}

	// First handle left mouse down.
	if (event.LeftDown() || (event.LeftIsDown() && stale_click)) {
		mSelectState = 0;
		testPicking(tmpPnt.x, tmpPnt.y);

		if (interactiveMode) {
			// If the mouse left the window while the user was already
			// lassoing, stale_click will be true and this code will get
			// executed, and the lassoing already in progress will be lost.
			// So, we force that a new lasso is started only when the mouse
			// button has just gone down.
			if (MolWin->LassoSelected() && event.LeftDown()) {
				MolWin->LassoStart(tmpPnt.x, height - tmpPnt.y);
				select_stack_top = 5;
			} else if (selected_type == MMP_ATOM) {
				GLdouble tmpWinX, tmpWinY;

				findWinCoord(lAtoms[selected].Position.x,
					lAtoms[selected].Position.y,
					lAtoms[selected].Position.z,
					tmpWinX, tmpWinY, atomDepth);
	   
				winDiffX = tmpPnt.x - (int)tmpWinX;
				winDiffY = tmpPnt.y - (int)tmpWinY;
			}
		}
	}
	
	// If not left, try right click in edit mode.
	else if (event.RightDown()) {
		testPicking(tmpPnt.x, tmpPnt.y);

		// if (selected < 0) { 
			// We don't really do anything here, since the user didn't 
			// click on anything.  But by including this null block, we
			// don't need to test for this anymore.
		// } 

		if (selected_type == MMP_ATOM) {
			if (interactiveMode) {
				interactPopupMenu(tmpPnt.x, tmpPnt.y, 1);
				MolWin->SelectionChanged(deSelectAll);
				MolWin->UpdateGLModel();
			} else {
				measurePopupMenu(tmpPnt.x, tmpPnt.y);
			}
		}

		else if (selected_type == MMP_BOND) {
			if (interactiveMode) {
				interactPopupMenu(tmpPnt.x, tmpPnt.y, 0);
				MolWin->SelectionChanged(deSelectAll);
				MolWin->UpdateGLModel();
			} else {
				bondPopupMenu(tmpPnt.x, tmpPnt.y);
			}
		}

		else if (selected_type == MMP_ANNOTATION) {
			annoPopupMenu(tmpPnt.x, tmpPnt.y);
		}
	}

	else if (event.MiddleDown()) {
		testPicking(tmpPnt.x, tmpPnt.y);
	}

	else if (event.Leaving() && event.LeftIsDown() && MolWin->LassoSelected()) {
		MolWin->LassoEnd(tmpPnt.x, height - tmpPnt.y);
	}

	// If we made it this far, button states haven't changed.  Are we dragging?
	else if (event.Dragging()) {
		mSelectState++;

		// Lassoing should only be done with the left mouse button.
		if (MolWin->LassoSelected() && event.LeftIsDown()) {
			HandleLassoing(event, wxPoint(tmpPnt.x, height - tmpPnt.y));
			edited_atoms = true;
		}

		// User must be wanting to translate or rotate atoms.
		else if (MolWin->HandSelected() &&
				 (selected_type == MMP_BOND || selected_type == MMP_ATOM)) {
			HandleEditing(event, tmpPnt, oldTmpPnt);
			edited_atoms = true;
		}
		
	}

	// If the left mouse button is released, the user is either done dragging,
	// in which case we do nothing, or the user has selected an item.
	else if (event.LeftUp()) {
		if (event.CmdDown())
			deSelectAll = false;

		if (MolWin->LassoSelected()) {
			MolWin->LassoEnd(tmpPnt.x, height - tmpPnt.y);
			if (!MolWin->LassoHasArea()) {
				SelectObj(selected_type, selected, deSelectAll);
			}
			edited_atoms = true;
		}

		// Allow a little bit of dragging to be interpreted as selection.
		else if (mSelectState >= 0 && mSelectState < 3) {
			mSelectState = -1;

			// If editing, we  
			if (interactiveMode) {

				if (periodic_dlg && MolWin->HandSelected() &&
					periodic_dlg->GetSelectedID() != 0) {


					if (selected_site >= 0) {
						// std::cout << "selected: " << selected << std::endl; 
						// std::cout << "selected_site: " << selected_site << std::endl; 

						CPoint3D newPnt;
						newPnt.x = newPnt.y = newPnt.z = 0.0f;
						mMainData->NewAtom(periodic_dlg->GetSelectedID(), newPnt);
					}

					// If the user clicked on nothing, we try to add an atom given
					// the selected element on the periodic table palette.
					else if (selected < 0) {

						CPoint3D newPnt;
						GLdouble newX, newY, newZ;

						findWinCoord(0.0, 0.0, 0.0, newX, newY, atomDepth);
						//estimate an atomDepth value, X and Y values are of no use 
						findReal3DCoord((GLdouble)tmpPnt.x, (GLdouble)tmpPnt.y,
											atomDepth, newX, newY, newZ);
						newPnt.x = newX;
						newPnt.y = newY;
						newPnt.z = newZ;
						mMainData->NewAtom(periodic_dlg->GetSelectedID(), newPnt);
					}
  
					oldSelect = -1;
				}

				// This functionality prevents selecting one atom, and then
				// selecting a different one -- without creating a bond
				// between them.  Since we have contextual menus now, I'm
				// commenting this code out since I always end up with bonds
				// I don't want.
				
				// If two different items were selected in sequence, try to
				// add a bond between them.  Seems like it'd be wise to see
				// if selected indicates an atom (and not a bond) here...
				// else if (selected != oldSelect) { 
					// int tmpBondStatus = lFrame->BondExists(oldSelect,selected); 

					// if (deSelectAll && tmpBondStatus == -1) 
						// lFrame->AddBond(oldSelect,selected); 

					// oldSelect = selected; 

					// if (deSelectAll && tmpBondStatus == -1) 
						// selected = -1; 
				// } 
			}

			SelectObj(selected_type, selected, deSelectAll);
			MolWin->SelectionChanged(deSelectAll);
			MolWin->UpdateGLModel();
			edited_atoms = true;

		}

		if (mDragWin) {
			mDragWin->EndDrag();
			delete mDragWin;
			mDragWin = NULL;
		}
	}

	else if (event.ButtonUp()) {
		selected_type = MMP_NULL;
		selected = -1;
	}
	
	oldTmpPnt = tmpPnt;
	stale_click = false;

	// Pass mouse event to MolDisplayWin::Rotate for processing
	if (!interactiveMode || !edited_atoms) {
		MolWin->Rotate(event);
	}

	else {
		draw();
	}

}

void MpGLCanvas::HandleEditing(wxMouseEvent& event, const wxPoint& curr_pt,
		                       const wxPoint& prev_pt) {

	Frame *lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;
	mpAtom *lAtoms = lFrame->Atoms;
	int width, height;

	GetClientSize(&width, &height);

	// If an atom is clicked on...
	if (selected_type == MMP_ATOM) {

		GLdouble newX, newY, newZ;

		// If control is held down while a selected atom is clicked on, we
		// rotate the selection only around itself.
		if (event.ControlDown() || event.CmdDown()) {

			// If the clicked on atom isn't selected, or if it's the only one
			// selected, we return immediately since rotating around itself
			// would be silly.
			if (!lFrame->GetAtomSelection(selected) ||
				lFrame->GetNumAtomsSelected() <= 1) {
				return;
			}

			std::vector<mpAtom *> atoms;
			std::vector<mpAtom *>::const_iterator atom;
			CPoint3D centroid = CPoint3D(0.0f, 0.0f, 0.0f);
			CPoint3D sphere_center;

			// First, we find the selection set's center of gravity which
			// we'll rotate the set around.
			for (int i = 0; i < NumAtoms; i++) {
				if (lFrame->GetAtomSelection(i)) {
					atoms.push_back(&lAtoms[i]);
					centroid += lAtoms[i].Position;
				}
			}
			centroid *= 1.0f / atoms.size();

			// Now, we need to find the radius of the trackball we'll use to
			// compute the rotation.  This is a little tricky.  What we do is
			// find the maximum distance between the center of gravity and the
			// atoms in the set.  We throw away the position and keep only the
			// distance.  We then calculate a new point along the x-vector in
			// eye space that's that distance away from the center of gravity.
			// The distance between these two points in screen space is the
			// radius of virtual sphere.
			float max_distance = 0.0f;
			float distance;
			for (atom = atoms.begin(); atom != atoms.end(); atom++) {
				distance = ((*atom)->Position - centroid).Magnitude();
				if (distance > max_distance) {
					max_distance = distance;
				}
			}

			GLdouble proj[16];
			GLdouble mv[16];
			GLint viewport[4];

			glGetDoublev(GL_PROJECTION_MATRIX, proj);
			glGetDoublev(GL_MODELVIEW_MATRIX, mv);
			glGetIntegerv(GL_VIEWPORT, viewport);

			// The virtual sphere current and previous mouse locations.
			Point prev, curr;
			Point cent;
			prev.h = prev_pt.x;
			prev.v = prev_pt.y;
			curr.h = curr_pt.x;
			curr.v = curr_pt.y;

			// Calculate a point at maximal distance in along the x-axis in
			// eye space.
			CPoint3D max_point;
			max_point.x = centroid.x + max_distance * mv[0];
			max_point.y = centroid.y + max_distance * mv[4];
			max_point.z = centroid.z + max_distance * mv[8];

			// Now project the centroid and max point into screen space.
			double proj_pt[3];
			double proj_max[3];
			int radius;
			gluProject(centroid.x, centroid.y, centroid.z,
					   mv, proj, viewport,
					   &(proj_pt[0]), &(proj_pt[1]), &(proj_pt[2]));
			gluProject(max_point.x, max_point.y, max_point.z,
					   mv, proj, viewport,
					   &(proj_max[0]), &(proj_max[1]), &(proj_max[2]));

			// The centroid's project is the center of the virtual sphere.
			cent.h = (unsigned short) proj_pt[0];
			cent.v = height - (unsigned short) proj_pt[1];

			// The distance between the two projected points is the radius.
			proj_pt[0] = proj_pt[0] - proj_max[0];
			proj_pt[1] = proj_pt[1] - proj_max[1];
			radius = (int) sqrt(proj_pt[0] * proj_pt[0] +
								proj_pt[1] * proj_pt[1]);

			// Now we assemble the rotation matrix and rotate all the selected
			// atoms.
			Matrix4D rot_matrix;
			Matrix4D rot;
			glGetFloatv(GL_MODELVIEW_MATRIX, (float *) rot);

			VirtualSphereQD3D(prev, curr, cent, radius, rot_matrix, rot);

			CPoint3D new_pt;
			for (atom = atoms.begin(); atom != atoms.end(); atom++) {
				(*atom)->Position -= centroid;
				Rotate3DPt(rot_matrix, (*atom)->Position, &new_pt);
				(*atom)->Position = new_pt;
				(*atom)->Position += centroid;
			}

		} else {

			// If shift is held when an atom is clicked on, we want to
			// change the depths of either the clicked on or selected
			// atoms.  Also do this for the middle mouse button.
			if (event.ShiftDown() || event.MiddleIsDown()) {
				GLdouble tmpX, tmpY, tmpZ;
				float depth_offset;
				float dy = curr_pt.y - prev_pt.y;

				findWinCoord(lAtoms[selected].Position.x,
					lAtoms[selected].Position.y,
					lAtoms[selected].Position.z, tmpX, tmpY, tmpZ);
				depth_offset = (dy * mMainData->WindowSize) /
								(25.0f * GetRect().GetWidth());
				findReal3DCoord(tmpX, tmpY, tmpZ - depth_offset,
								newX, newY, newZ);
			}

			// If no shift, just translate.
			else {
				findReal3DCoord(curr_pt.x - winDiffX, curr_pt.y - winDiffY,
								atomDepth, newX, newY, newZ);

				int constrain_anno_id = mMainData->GetConstrainAnnotation();
				if (constrain_anno_id != -1) {
					ConstrainPosition(constrain_anno_id, &newX, &newY,
						&newZ);
				}
			}

			// If that atom is a member of the selected atom set, move all
			// atoms that are currently selected.
			if (lFrame->GetAtomSelection(selected)) {
				GLdouble offset_x, offset_y, offset_z;

				offset_x = lAtoms[selected].Position.x - newX;
				offset_y = lAtoms[selected].Position.y - newY;
				offset_z = lAtoms[selected].Position.z - newZ;

				for (int i = 0; i < lFrame->GetNumAtoms(); i++) {
					if (lFrame->GetAtomSelection(i)) {
						lAtoms[i].Position.x -= offset_x;
						lAtoms[i].Position.y -= offset_y;
						lAtoms[i].Position.z -= offset_z;
					}
				}
			}
			
			// If it's not selected, make it the only selected one and
			// move only it.
			else {
				lAtoms[selected].Position.x = newX;
				lAtoms[selected].Position.y = newY;
				lAtoms[selected].Position.z = newZ;

				SelectObj(selected_type, selected, true);
				MolWin->SelectionChanged(true);
			}

		}

		if (mDragWin) {
			mDragWin->EndDrag();
			delete mDragWin;
			mDragWin = (wxDragImage*) NULL;
		}

		wxString tmp3Dcoord;

		tmp3Dcoord.Printf(wxT("%.2f,%.2f,%.2f"), newX, newY, newZ);

		mDragWin = new wxDragImage(tmp3Dcoord, wxCursor(wxCURSOR_HAND));

		if (!mDragWin->BeginDrag(wxPoint(0,30), this)) {
			delete mDragWin;
			mDragWin = (wxDragImage*) NULL;
		} else {
			mDragWin->Move(event.GetPosition());
			mDragWin->Show();
		}

		lFrame->SetBonds(Prefs, true, true);
		MolWin->UpdateGLModel();

	}
	
	// If the user's dragging after selecting a bond, we want to
	// rotated the selected set of atoms around that bond.
	else if (selected_type == MMP_BOND) {
		// if we're a bond
		//   translate to one of bond atoms
		//   rotate selected atoms around bond according to distance
		//     of move by translating to either atom on bond and 
		//     rotating around vector represented by bond
		
		CPoint3D pivot_pt;     // The atom pos to act as origin
		CPoint3D other_pt;     // The pos of pivot atom's bondmate
		CPoint3D new_pt;       // Each atom's pos rotated, untranslated
		CPoint3D axis;         // Vector of rotation bond
		float radians;         // Amount to rotate by
		float cosine;          // Used to rotate around axis
		float sine;
		float c_inv;
		Matrix4D rot_mat;
		float dy;              // No. pixels of mouse change in y-dir
		float angle_offset;    // Corresponding amount of rotation
	   
		// Calculate the amount of rotation according to the amount
		// of mouse change along the y-axis of the viewport.
		dy = curr_pt.y - prev_pt.y;
		angle_offset = dy / GetClientSize().GetHeight() * 540.0f;

		// Get all trig together for rotating around the bond that
		// was just clicked on.
		radians = angle_offset * kPi / 180.0f;
		sine = sin(radians);
		cosine = cos(radians);
		c_inv = 1.0f - cosine;

		// The axis of rotation is a vector from one atom of the
		// bond to the other.

		lFrame->GetAtomPosition(lFrame->GetBondAtom(selected, 1), pivot_pt);
		lFrame->GetAtomPosition(lFrame->GetBondAtom(selected, 2), other_pt);
		axis = other_pt	- pivot_pt;
		Normalize3D(&axis);

		rot_mat[0][0] = c_inv * axis.x * axis.x + cosine;
		rot_mat[1][0] = c_inv * axis.x * axis.y - sine * axis.z;
		rot_mat[2][0] = c_inv * axis.x * axis.z + sine * axis.y;

		rot_mat[0][1] = c_inv * axis.y * axis.x + sine * axis.z;
		rot_mat[1][1] = c_inv * axis.y * axis.y + cosine;
		rot_mat[2][1] = c_inv * axis.y * axis.z - sine * axis.x;

		rot_mat[0][2] = c_inv * axis.z * axis.x - sine * axis.y;
		rot_mat[1][2] = c_inv * axis.z * axis.y + sine * axis.x;
		rot_mat[2][2] = c_inv * axis.z * axis.z + cosine;

		rot_mat[3][0] = rot_mat[3][1] = rot_mat[3][2] = 0.0f;
		rot_mat[0][3] = rot_mat[1][3] = rot_mat[2][3] = 0.0f;
		rot_mat[3][3] = 1.0f;

		// For each selected atom, we need to rotate it around the
		// bond.  We want one of the bond atom's to act as the origin,
		// so we translate our coordinate system there,
		// perform the rotation, and then translate back.
		for (int i = 0; i < lFrame->GetNumAtoms(); i++) {
			if (lFrame->GetAtomSelection(i)) {
				lAtoms[i].Position -= pivot_pt;
				Rotate3DPt(rot_mat, lAtoms[i].Position, &new_pt);
				lAtoms[i].Position = new_pt;
				lAtoms[i].Position += pivot_pt;
			}
		}

		// We need to update our data.
		// edited_atoms = true; 
	}

	// else if (selected >= NumAtoms + lFrame->NumBonds) { 

		// int anno_id = selected - NumAtoms - lFrame->NumBonds; 
		
		// if (mMainData->Annotations[anno_id]->getType() == MP_ANNOTATION_LENGTH) { 
			// float dy;           // No. pixels of mouse change in y-dir 
			// float offset;       // Corresponding amount of translation 
			// CPoint3D atom1_pos; 
			// CPoint3D atom2_pos; 
			// CPoint3D offset_vec; 
			// AnnotationLength *length_anno; 

			// length_anno = 
				// dynamic_cast<AnnotationLength *> 
					// (mMainData->Annotations[anno_id]); 
		   
			// Calculate the amount of rotation according to the amount
			// of mouse change along the y-axis of the viewport.
			// dy = tmpPnt.y - oldTmpPnt.y; 
			// offset = 1.0f - dy / (GetRect().GetHeight()) * 2.0f; 

			// lFrame->GetAtomPosition(length_anno->getAtom(0), atom1_pos); 
			// lFrame->GetAtomPosition(length_anno->getAtom(1), atom2_pos); 

			// offset_vec = atom2_pos - atom1_pos; 

			// get atom2
			// atom2_pos.x = atom1_pos.x + offset_vec.x * offset; 
			// atom2_pos.y = atom1_pos.y + offset_vec.y * offset; 
			// atom2_pos.z = atom1_pos.z + offset_vec.z * offset; 

			// lFrame->SetAtomPosition(length_anno->getAtom(1), atom2_pos); 

			// edited_atoms = true; 
		// } 
	// } 
	
	else {
		mSelectState = -1;
	}

	// return edited_atoms; 

}
			
void MpGLCanvas::HandleLassoing(wxMouseEvent& event, const wxPoint& curr_pt) {

	GLdouble mv[16];
	GLdouble proj[16];
	GLint viewport[4];
	GLdouble win_x, win_y, win_z;
	Frame *lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;
	mpAtom *lAtoms = lFrame->Atoms;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);

	MolWin->LassoGrown(curr_pt.x, curr_pt.y);
	// edited_atoms = true; 

	if (!event.ShiftDown()) {
		lFrame->resetAllSelectState();
	}
	MolWin->SetHighliteMode(true);

	int nselected = 0;
	for (int i = 0; i < lFrame->GetNumAtoms(); i++) {
		gluProject(lAtoms[i].Position.x,
				   lAtoms[i].Position.y,
				   lAtoms[i].Position.z,
				   mv, proj, viewport, &win_x, &win_y, &win_z);
		if (MolWin->LassoContains((int) win_x, (int) win_y)) {
			lFrame->SetAtomSelection(i, true);
			nselected++;
		}
	}

	if (nselected >= 4) {
		select_stack_top = 5;
	}

	// for each atom
	//    project into window coordinates
	//    if contained in lasso area, select it
}

void MpGLCanvas::KeyHandler(wxKeyEvent & event) {
	//char events are passed up the parent chain so se need to explicitely pass them
	MolWin->KeyHandler(event);
}

void MpGLCanvas::findWinCoord(GLfloat x, GLfloat y, GLfloat z, GLdouble& winX, GLdouble& winY, GLdouble& winZ) {
	GLdouble mvMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, mvMatrix);
	glGetDoublev (GL_PROJECTION_MATRIX, projMatrix);

	gluProject(x, y, z, mvMatrix, projMatrix, viewport, &winX, &winY, &winZ);

	winY = viewport[3] - winY;  //"pretend" to have wx's coordinate system
}

void MpGLCanvas::findReal3DCoord(GLdouble x, GLdouble y, GLdouble z, GLdouble& realX, GLdouble& realY, GLdouble& realZ) {
	GLdouble mvMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];
	GLdouble winX, winY;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev (GL_MODELVIEW_MATRIX, mvMatrix);
	glGetDoublev (GL_PROJECTION_MATRIX, projMatrix);

	winX = x;
	winY = (GLdouble)(viewport[3])-y;

	gluUnProject (winX, winY, z, mvMatrix, projMatrix, viewport, &realX, &realY, &realZ);
}

#define SELECT_BUFFER_SIZE 128
void MpGLCanvas::testPicking(int x, int y) {
	GLuint buff[SELECT_BUFFER_SIZE];
	GLint hits, view[4];
	int id;

	glSelectBuffer(SELECT_BUFFER_SIZE, buff);
	glGetIntegerv(GL_VIEWPORT, view);

	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix(x, view[3]-y, 1.0, 1.0, view);
	//gluPerspective(60, 1.0, 0.0001, 1000.0);

	GLdouble top, right;
	int width, height;

	GetClientSize(&width, &height);

	GLdouble aspect = ((float)width)/height;

	GLdouble zNear = 0.1;
	GLdouble myGLperspective = zNear*tan(Prefs->GetGLFOV()*(kPi)/180.0);
	if (aspect > 1.0) {
		right = myGLperspective;
		top = right/aspect;
	} else {
		top = myGLperspective;
		right = top * aspect;
	}

	if (myGLperspective > 0.001)
		glFrustum(-right, right, -top, top, zNear, 1000.0);
	else {
		if (aspect > 1.0) {
			right = mMainData->WindowSize;
			top = right/aspect;
		} else {
			top = mMainData->WindowSize;
			right = top * aspect;
		}
		glOrtho(-right, right, -top, top, zNear, 1000.0);
	}

	glMatrixMode(GL_MODELVIEW);

	MolWin->DrawGL();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
 
	hits = glRenderMode(GL_RENDER);

	selected = -1;
	selected_type = MMP_NULL;
	selected_site = -1;
	unsigned int min_depth = 0xFFFFFFFF;

	// Each hit record has number of names, min depth, max depth, and a list of
	// names on the stack for the hit.  For atoms, bonds, and annotations,
	// there'll be a type identifier and then an item identifier.  For 
	// bonding sites, they'll be the atom type id, the element id, and then
	// the bond id.
	int i, j;
	for (i = 0, j = 0; i < hits; i++) {
		if (buff[j + 1] < min_depth) {
			min_depth = buff[j + 1];
			selected_type = buff[j + 3];
			if (buff[j] > 1) {
				selected = buff[j + 4] - 1;
				if (buff[j] > 2) {
					selected_site = buff[j + 5] - 1;	
				}
			}
		}
		j += buff[j] + 3;
	}

}

void MpGLCanvas::SelectObj(int selected_type, int select_id, bool unselect_all) 
{
	Frame *lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;
	mpAtom *lAtoms = lFrame->Atoms;
	Bond *lBonds = lFrame->Bonds;

	if (unselect_all) {
		lFrame->resetAllSelectState();
		select_stack_top = 0;
	}

	// Make sure select id is either atom or bond id
	if (select_id >= 0) {

		// select_id indicates an atom
		if (selected_type == MMP_ATOM) {
			bool atom_is_selected = true;

			// If we're to keep other selected items and atom is already
			// selected, then we must be unselecting this atom.
			if (!unselect_all && lFrame->GetAtomSelection(select_id))
				atom_is_selected = false;

			lFrame->SetAtomSelection(select_id, atom_is_selected);
			if (atom_is_selected) {
				// In order to know how many atoms are selected, we let 
				// select_stack_top grow indefinitely, though the stack itself
				// only contains 4 indices.  Action should only be taken when
				// the top is in [0, 3].
				select_stack[select_stack_top % 4] = select_id;
				select_stack_top++;
			} else {
				// If the stack hasn't filled up, we let the user's deselection
				// of the atom just added remove it from the stack.  If a
				// different atom was deselected, the stack is in ruins since
				// the order is messed up, and we invalidate it by making it
				// over capacity.
				if (select_stack_top > 0 && select_stack_top <= 4) {
					if (select_id == select_stack[select_stack_top - 1]) {
						select_stack_top--;
					} else {
						select_stack_top = 5;
					}
				}
			}

			// printf("select_stack_top: %d\n", select_stack_top); 
			// for (int i = 0; i < select_stack_top; i++) { 
				// printf("select_stack[%d]: %d\n", i, select_stack[i]); 
			// } 

			// If atom selections change, so might their bonds.
			for (int i = 0; i < lFrame->NumBonds; i++) {
				long atom1 = lBonds[i].Atom1;
				long atom2 = lBonds[i].Atom2;

				if (lFrame->GetAtomSelection(atom1) &&
					lFrame->GetAtomSelection(atom2))
					lBonds[i].SetSelectState(atom_is_selected);
			}
		}
		
		// If select_id indicates a bond
		else if (selected_type == MMP_BOND) {
			bool newstate = true;
			if (!unselect_all && lBonds[select_id].GetSelectState())
				newstate = false;
			lBonds[select_id].SetSelectState(newstate);

			long atom1 = lBonds[select_id].Atom1;
			long atom2 = lBonds[select_id].Atom2;

			lFrame->SetAtomSelection(atom1, newstate);
			lFrame->SetAtomSelection(atom2, newstate); //select atoms that this bond connect
		}

		bool result = false;
		for (long i=0; i<NumAtoms; i++) {
			if (lFrame->GetAtomSelection(i)) {
				result = true;
				break;
			}
		}
		MolWin->SetHighliteMode(result);
	} else {
		lFrame->resetAllSelectState();
		select_stack_top = 0;
		MolWin->SetHighliteMode(false);
	}

	glMatrixMode(GL_MODELVIEW);
//	MolWin->AdjustMenus();
}

void MpGLCanvas::FitToPlane(wxCommandEvent& event) {

	// This function fits the selected atoms to their nearest plane (as
	// calculated by minimizing the atom's orthogonal distances to the plane).
	// It does this by finding the principle components of the atom set and
	// using the third axis as the plane's normal and the centroid as a 
	// point on the plane.  The principle component basis is calculated
	// using singular value decomposition.
	//
	// It's assumed that at least 4 atoms are selected when this function is
	// called.

	Frame *lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;
	mpAtom *lAtoms = lFrame->Atoms;
	CPoint3D centroid = CPoint3D(0.0f, 0.0f, 0.0f);
	int i;
	Matrix2D locs;
	Matrix2D trans_locs;
	Matrix2D cov;
	std::vector<mpAtom *> atoms;
	std::vector<mpAtom *>::const_iterator atom;

	// First we need a list of all the selected atoms.  It doesn't seem right
	// to put all these in a vector because we just trash it right after this
	// when we drop all the atom coordinates in a matrix, but by doing this, 
	// we don't have to iterate through all the atoms twice.
	for (i = 0; i < NumAtoms; i++) {
		if (lFrame->GetAtomSelection(i)) {
			atoms.push_back(&lAtoms[i]);
			centroid += lAtoms[i].Position;
		}
	}

	centroid *= (1.0f / atoms.size());

	// Now store all selected atoms' coordinates in a matrix.
	locs = Matrix2D(atoms.size(), 3);
	for (atom = atoms.begin(), i = 0; atom != atoms.end(); atom++, i++) {
		locs.data[i * 3 + 0] = (*atom)->Position.x - centroid.x;
		locs.data[i * 3 + 1] = (*atom)->Position.y - centroid.y;
		locs.data[i * 3 + 2] = (*atom)->Position.z - centroid.z;
	}

	// Transpose the matrix.
	trans_locs = locs.Transpose();

	// Multiply the two to get the covariance matrix.
	cov = trans_locs * locs;
	cov *= 1.0f / atoms.size();
	
	// Perform SVD on covariance matrix.
	Matrix2D out, gain, in;
	cov.SVD(&out, &gain, &in);

	// Project the atoms onto the plane.  The plane's normal is the eigenvector
	// with the smallest eigenvalue.  The other two vectors are more principle,
	// lying in the plane.
	CPoint3D normal = CPoint3D(out.data[2], out.data[5], out.data[8]);
	for (atom = atoms.begin(); atom != atoms.end(); atom++) {
		ProjectToPlane(normal, centroid, (*atom)->Position);
	}

	// std::cout << "normal: " << normal << std::endl; 
	// std::cout << "normal.Mag(): " << normal.Magnitude() << std::endl; 
	// std::cout << "locs:" << std::endl << locs; 
	// std::cout << "trans_locs:" << std::endl << trans_locs; 
	// std::cout << "cov:" << std::endl << cov; 
	// std::cout << "out:" << std::endl << out; 
	// std::cout << "gain:" << std::endl << gain; 
	// std::cout << "in:" << std::endl << in; 
	
}

void MpGLCanvas::interactPopupMenu(int x, int y, bool isAtom) {

	wxMenu menu;
	wxMenuItem *item;
	wxMenu *submenu;
	Frame *lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;
	int bond_order;
	submenu = new wxMenu();
	wxString length_label;

	if (isAtom) {
		insertAnnotationMenuItems(menu);
		menu.AppendSeparator();

		// A plane is defined by exactly 3 atoms.  With any more than that, we 
		// can have noncoplanar atoms.  Only then do we allow the user to fit
		// the atoms to a plane.
		if (lFrame->GetNumAtomsSelected() >= 4) {
			menu.Append(GL_Popup_Fit_To_Plane, wxT("Fit atoms to plane"));
			menu.AppendSeparator();
		}
	}

	// If a bond is clicked on, we show some bond specific items, like
	// the length of the bond and the order.
	if (!isAtom) {
		length_label.Printf(wxT("Bond length: %f"),
			lFrame->GetBondLength(selected));
		item = menu.Append(wxID_ANY, length_label);
		item->Enable(false);
		bond_order = lFrame->Bonds[selected].Order;
		item = submenu->AppendRadioItem(GL_Popup_To_Hydrogen_Bond,
			wxT("Hydrogen"));
		if (bond_order == kHydrogenBond) {
			item->Check(true);
		}
		item = submenu->AppendRadioItem(GL_Popup_To_Single_Bond, wxT("Single"));
		if (bond_order == kSingleBond) {
			item->Check(true);
		}
		item = submenu->AppendRadioItem(GL_Popup_To_Double_Bond, wxT("Double"));
		if (bond_order == kDoubleBond) {
			item->Check(true);
		}
		item = submenu->AppendRadioItem(GL_Popup_To_Triple_Bond, wxT("Triple"));
		if (bond_order == kTripleBond) {
			item->Check(true);
		}
		menu.Append(wxID_ANY, wxT("Bond Order"), submenu);
		menu.AppendSeparator();
	} else {

		// If the periodic table is shown and an atom is selected, offer an
		// option to change the clicked-on atom to the selected type.
		if (periodic_dlg && periodic_dlg->GetSelectedID() !=
			lFrame->Atoms[selected].GetType()) {
			wxString label;
			wxString atom_name;
			Prefs->GetAtomLabel(lFrame->Atoms[selected].GetType() - 1,
				atom_name);
			label.Printf(wxT("Change %s to "), atom_name.mb_str());
			Prefs->GetAtomLabel(periodic_dlg->GetSelectedID() - 1, atom_name);
			label.Append(atom_name);

			menu.Append(GL_Popup_Change_Atom, label);
			menu.AppendSeparator();
		}

		menu.Append(GL_Popup_Menu_Apply_All, _T("&Apply to All Frames"));
	}

	menu.Append(GL_Popup_Delete_Item_Current_Frame, _T("&Delete"));
	menu.Append(GL_Popup_Delete_Item_All_Frames, _T("&Delete in All Frames"));

	PopupMenu(&menu, x, y);

}
//make a popup menu editing the objets in the scene

void MpGLCanvas::ChangeAtom(wxCommandEvent& event) {
	mMainData->cFrame->SetAtomType(selected, periodic_dlg->GetSelectedID());
}

void MpGLCanvas::insertAnnotationMenuItems(wxMenu& menu) {

	wxMenuItem *item;
	Frame *lFrame = mMainData->cFrame;
	bool already_exists = false;
	std::vector<Annotation *>::const_iterator anno;
	int anno_id = 0;

	if (selected_type == MMP_ATOM) {
		wxString aLabel, nItem;
		Prefs->GetAtomLabel(lFrame->Atoms[selected].GetType()-1, nItem);
		aLabel.Printf(wxT(" (%d)"), (selected+1));
		nItem.Append(aLabel);
		item = menu.Append(wxID_ANY, nItem);
		item->Enable(false);

		for (anno = mMainData->Annotations.begin(), anno_id = 0;
			 anno != mMainData->Annotations.end() && !already_exists;
			 anno++) {
			if ((*anno)->isEquivalent(1, &selected)) {
				already_exists = true;
			}
			anno_id++;
		}

		item = menu.Append(GL_Popup_Mark_Atom, wxT("Mark atom"));
		if (already_exists) {
			item->Enable(false);
		} else {
			item->Enable(true);
		}
	}

	// We need to restore this to false since the atom marker check may have
	// set it to true, which shouldn't affect the availability of annotations.
	already_exists = false;

	if (select_stack_top > 1) {

		// If user clicked on an atom and some things are selected, let's
		// separate the items added above and the annotation items.
		if (selected_type == MMP_ATOM &&
			select_stack_top >= 2 && select_stack_top <= 4) {
			menu.AppendSeparator();
		}

		switch (select_stack_top) {
			case 2:
			{
				float length;
				if (lFrame->GetBondLength(select_stack[0], select_stack[1], &length)) {
					wxString lengthString, name;
					Prefs->GetAtomLabel(lFrame->Atoms[select_stack[0]].GetType()-1, lengthString);
					name.Printf(wxT(" (%d) to "), (select_stack[0]+1));
					lengthString.Append(name);
					Prefs->GetAtomLabel(lFrame->Atoms[select_stack[1]].GetType()-1, name);
					lengthString.Append(name);
					name.Printf(wxT(" (%d)"), (select_stack[1]+1));
					lengthString.Append(name);
					item = menu.Append(wxID_ANY, lengthString);
					item->Enable(false);
					
					lengthString.Printf(wxT("Distance: %f"), length);
					item = menu.Append(wxID_ANY, lengthString);
					item->Enable(false);
				}

				for (anno = mMainData->Annotations.begin(), anno_id = 0;
					 anno != mMainData->Annotations.end() && !already_exists;
					 anno++) {
					if ((*anno)->isEquivalent(2, select_stack)) {
						already_exists = true;
					}
					anno_id++;
				}

				item = menu.Append(GL_Popup_Measure_Length, wxT("Measure length"));
				if (already_exists) {
					item->Enable(false);
				} else {
					item->Enable(true);
				}
				
				menu.AppendSeparator();
				wxMenu * submenu = new wxMenu();
				int bond_id = lFrame->BondExists(select_stack[0],
												select_stack[1]);

				if (bond_id < 0) {
					submenu->Append(GL_Popup_To_Hydrogen_Bond, wxT("Hydrogen"));
					submenu->Append(GL_Popup_To_Single_Bond, wxT("Single"));
					submenu->Append(GL_Popup_To_Double_Bond, wxT("Double"));
					submenu->Append(GL_Popup_To_Triple_Bond, wxT("Triple"));
					menu.Append(wxID_ANY, wxT("Add bond"), submenu);
				} else {
					wxMenuItem *item;
					int bond_order = lFrame->Bonds[bond_id].Order;
					item = submenu->AppendRadioItem(GL_Popup_To_Hydrogen_Bond,
													wxT("Hydrogen"));
					if (bond_order == kHydrogenBond) {
						item->Check(true);
					}
					item = submenu->AppendRadioItem(GL_Popup_To_Single_Bond,
													wxT("Single"));
					if (bond_order == kSingleBond) {
						item->Check(true);
					}
					item = submenu->AppendRadioItem(GL_Popup_To_Double_Bond,
													wxT("Double"));
					if (bond_order == kDoubleBond) {
						item->Check(true);
					}
					item = submenu->AppendRadioItem(GL_Popup_To_Triple_Bond,
													wxT("Triple"));
					if (bond_order == kTripleBond) {
						item->Check(true);
					}

					menu.Append(wxID_ANY, wxT("Change bond"), submenu);
				}
			}
				break;
			case 3:
				for (anno = mMainData->Annotations.begin(), anno_id = 0;
					 anno != mMainData->Annotations.end() && !already_exists;
					 anno++) {
					if ((*anno)->isEquivalent(3, select_stack)) {
						already_exists = true;
					}
					anno_id++;
				}

				item = menu.Append(GL_Popup_Measure_Angle, wxT("Measure angle"));
				if (already_exists) {
					item->Enable(false);
				} else {
					item->Enable(true);
				}

				break;
			case 4:
				for (anno = mMainData->Annotations.begin(), anno_id = 0;
					 anno != mMainData->Annotations.end() && !already_exists;
					 anno++) {
					if ((*anno)->isEquivalent(4, select_stack)) {
						already_exists = true;
					}
					anno_id++;
				}

				item = menu.Append(GL_Popup_Measure_Dihedral, wxT("Measure dihedral"));
				if (already_exists) {
					item->Enable(false);
				} else {
					item->Enable(true);
				}

				break;
			default:
				break;
		}
	}
}

void MpGLCanvas::measurePopupMenu(int x, int y) {

	wxMenu menu;

	insertAnnotationMenuItems(menu);
	PopupMenu(&menu, x, y);

}

void MpGLCanvas::annoPopupMenu(int x, int y) {

	Frame *lFrame = mMainData->cFrame;
	wxMenu menu;
	wxMenuItem *item;

	if (interactiveMode &&
		mMainData->Annotations[selected]->getType() != MP_ANNOTATION_MARKER) {
		item = menu.AppendCheckItem(GL_Popup_Lock_To_Annotation,
				                    _("Constrain atoms"));
		if (selected == mMainData->GetConstrainAnnotation()) {
			item->Check(true);
		}
	}
	menu.Append(GL_Popup_Delete_Length, _("Delete annotation"));
	PopupMenu(&menu, x, y);

}

void MpGLCanvas::ConstrainToAnnotation(wxCommandEvent& event) {

	Frame *lFrame = mMainData->cFrame;

	if (mMainData->GetConstrainAnnotation() != selected) {
		mMainData->ConstrainToAnnotation(selected);
	} else {
		mMainData->RemoveAnnotationConstraint();
	}

}

void MpGLCanvas::DeleteAnnotation(wxCommandEvent& event) {

	Frame *lFrame = mMainData->cFrame;

	Annotation * t = mMainData->Annotations[selected];
	mMainData->Annotations.erase(mMainData->Annotations.begin() + selected);
	delete t;

	int constrain_anno_id = mMainData->GetConstrainAnnotation();
	if (constrain_anno_id != -1) {
		/* If we're deleting an annotation that appears earlier in the
		 * annotation list, we need to shift the id of the constrained
		 * annotation. */
		if (constrain_anno_id > selected) {
			mMainData->ConstrainToAnnotation(constrain_anno_id - 1);
		}
		
		/* Or, we may be deleting the constrained annotation itself, in which
		 * case, we have no more constraints. */
		else if (constrain_anno_id == selected) {
			mMainData->RemoveAnnotationConstraint();
		}
	}
}

void MpGLCanvas::bondPopupMenu(int x, int y) {

	// This function shows a popup menu that shows some information and
	// operations for the clicked-on bond.  It's assumed that selected is
	// a valid index in the bonds lists.

	wxMenu menu;
	wxMenuItem *item;
	wxMenu *submenu;
	Frame *lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;
	int bond_order;
	submenu = new wxMenu();
	wxString length_label;

	length_label.Printf(wxT("Bond length: %f"),
					lFrame->GetBondLength(selected));
	item = menu.Append(wxID_ANY, length_label);
	item->Enable(false);
	bond_order = lFrame->Bonds[selected].Order;
	item = submenu->AppendRadioItem(GL_Popup_To_Hydrogen_Bond,
								wxT("Hydrogen"));
	if (bond_order == kHydrogenBond) {
		item->Check(true);
	}
	item = submenu->AppendRadioItem(GL_Popup_To_Single_Bond, wxT("Single"));
	if (bond_order == kSingleBond) {
		item->Check(true);
	}
	item = submenu->AppendRadioItem(GL_Popup_To_Double_Bond, wxT("Double"));
	if (bond_order == kDoubleBond) {
		item->Check(true);
	}
	item = submenu->AppendRadioItem(GL_Popup_To_Triple_Bond, wxT("Triple"));
	if (bond_order == kTripleBond) {
		item->Check(true);
	}
	menu.Append(wxID_ANY, wxT("Bond Order"), submenu);
	
	menu.AppendSeparator();
	menu.Append(GL_Popup_Delete_Bond, wxT("Delete Bond"));

	PopupMenu(&menu, x, y);

}

void MpGLCanvas::ChangeBonding(wxCommandEvent& event) {
	
	// This is a multiple use function. Its action will depend on the menu id
	// that generates the call. It will set the bond type of the selected bond
	// to the desired order, creating a new bond if one didn't exist.
	
	Bond *bond;
	Frame *lFrame = mMainData->cFrame;
	BondOrder order = (BondOrder) ((event.GetId() - GL_Popup_To_Hydrogen_Bond) + kHydrogenBond);
	
	if (selected_type == MMP_BOND) {	//existing bond, change order
		bond = &(lFrame->Bonds[selected]);
		bond->Order = order;
		MolWin->BondsChanged();
		lFrame->resetAllSelectState();
		bond->SetSelectState(true);
		lFrame->SetAtomSelection(bond->Atom1, true);
		lFrame->SetAtomSelection(bond->Atom2, true);
	} else if (select_stack_top == 2) { //new bond
		int bond_id = lFrame->BondExists(select_stack[0], select_stack[1]);
		if (bond_id >= 0) {
			bond = &(lFrame->Bonds[bond_id]);
			bond->Order = order;
		} else {
			lFrame->AddBond(select_stack[0], select_stack[1], order);
		}
		MolWin->BondsChanged();
		lFrame->resetAllSelectState();
		lFrame->Bonds[lFrame->GetNumBonds() - 1].SetSelectState(true);
		lFrame->SetAtomSelection(select_stack[0], true);
		lFrame->SetAtomSelection(select_stack[1], true);
	}
}

void MpGLCanvas::DeleteBond(wxCommandEvent& event) {
	
	// Delete the selected bond
	
	Frame *lFrame = mMainData->cFrame;
	
	if (selected_type == MMP_BOND) {
		lFrame->DeleteBond(selected);
		MolWin->BondsChanged();
	}
}

void MpGLCanvas::AddAnnotation(wxCommandEvent& event) {

	switch (event.GetId()) {
		case GL_Popup_Mark_Atom:
		{
			AnnotationMarker * t = new AnnotationMarker(selected);
			mMainData->Annotations.push_back(t);
		}
			break;
		case GL_Popup_Measure_Length:
		{
			AnnotationLength * t = new AnnotationLength(select_stack[0], select_stack[1]);
			mMainData->Annotations.push_back(t);
		}
			break;
		case GL_Popup_Measure_Angle:
		{
			AnnotationAngle * t = new AnnotationAngle(select_stack[0], select_stack[1],
													  select_stack[2]);
			mMainData->Annotations.push_back(t);
		}
			break;
		case GL_Popup_Measure_Dihedral:
		{
			AnnotationDihedral * t = new AnnotationDihedral(select_stack[0], select_stack[1],
													  select_stack[2], select_stack[3]);
			mMainData->Annotations.push_back(t);
		}
			break;
		default:
			printf("unknown event\n");
			break;
	}

}

void MpGLCanvas::On_Apply_All(wxCommandEvent& event) {
	Frame *  lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;

	Frame * cFrame = mMainData->Frames;

	for (int i = 0; i < mMainData->NumFrames; i++) {
		if (mMainData->CurrentFrame-1 != i) {
			if (selected >= cFrame->NumAtoms)
				cFrame->AddAtom(lFrame->Atoms[selected].Type, lFrame->Atoms[selected].Position);
			else
				cFrame->Atoms[selected] = lFrame->Atoms[selected];
		}

		cFrame = cFrame->NextFrame;
	}
}

void MpGLCanvas::On_Delete_Single_Frame(wxCommandEvent& event) {
	Frame * lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;

	if (selected_type == MMP_ATOM) {
		mMainData->DeleteAtom(selected);
	} else if (selected_type == MMP_BOND) {
		lFrame->DeleteBond(selected - NumAtoms);
	}

}

void MpGLCanvas::On_Delete_All_Frames(wxCommandEvent& event) {

	Frame * lFrame = mMainData->Frames;
	long NumAtoms = mMainData->cFrame->NumAtoms;

	if (selected_type == MMP_ATOM)
		mMainData->DeleteAtom(selected, true);

	else if (selected_type == MMP_BOND) {
		while (lFrame) {
			if (selected < lFrame->NumBonds)
				lFrame->DeleteBond(selected);
			lFrame = lFrame->NextFrame;
		}
	}
}

// MpGLCanvas::AtomTypeDialog::AtomTypeDialog(MpGLCanvas * parent, wxWindowID id, const wxString& caption) : typeID(1) { 
	// Create(parent, id, caption); 
// } 

// void MpGLCanvas::AtomTypeDialog::Create(MpGLCanvas * parent, wxWindowID id, const wxString& caption) { 
	// wxDialog::Create( parent, id, caption, wxDefaultPosition, wxSize(250, 125), wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX); 

	// std::vector<wxString> atomTypes; 

	// for (int i = 0; i < kMaxAtomTypes; i++) { 
		// wxString tmp; 

		// parent->Prefs->GetAtomLabel(i, tmp); 

		// if (tmp.Length() > 0) 
			// atomTypes.push_back(tmp); 
	// } 

	// mainSizer = new wxBoxSizer(wxVERTICAL); 
	// upperSizer = new wxBoxSizer(wxVERTICAL); 
	// lowerSizer = new wxBoxSizer(wxHORIZONTAL); 

	// mTypeChoice = new wxChoice(this, ID_NEW_ATOM_TYPE_CHOICE, wxPoint(30,30), wxSize(200,wxDefaultCoord), atomTypes.size(), &atomTypes.front()); 
	// mTypeChoice->SetSelection(0); 

	// mButtOK = new wxButton(this, wxID_OK, wxT("OK") ); 
	// mButtCancel = new wxButton(this, wxID_CANCEL, wxT("Cancel")); 

	// upperSizer->Add(mTypeChoice, 0, wxALIGN_CENTRE | wxALL, 20); 
	// upperSizer->Add(8,8); 
	// lowerSizer->Add(10,10); 
	// lowerSizer->Add(mButtOK, 0, wxALIGN_CENTRE | wxALL, 10); 
	// lowerSizer->Add(mButtCancel, 0, wxALIGN_CENTRE | wxALL, 10); 

	// mainSizer->Add(upperSizer); 
	// mainSizer->Add(lowerSizer); 

	// mainSizer->Layout(); 
	// SetSizer(mainSizer); 
	// Centre(wxBOTH); 
// } 

// void MpGLCanvas::AtomTypeDialog::OnChoice( wxCommandEvent &event ) { 
	// typeID = event.GetInt() + 1; 
// } 

void MpGLCanvas::toggleInteractiveMode(void) {

	interactiveMode = 1 - interactiveMode;

}

void MpGLCanvas::togglePeriodicDialog(void) {
	show_periodic_dlg = !show_periodic_dlg;

	if (show_periodic_dlg) {
		if (periodic_dlg) {
			periodic_dlg->Raise();
		} else {
			wxRect window_rect = GetRect();
			periodic_dlg = new PeriodicTableDlg(
				this, wxT("Periodic Table"), window_rect.x + window_rect.width,
				window_rect.y + 30);
			periodic_dlg->Show();
		}
		((MpApp &) wxGetApp()).AdjustAllMenus();
	} else {
		ClosePeriodicDlg();
	}
} 

void MpGLCanvas::ClosePeriodicDlg(void) {
	if (periodic_dlg) {
		periodic_dlg->Destroy();
		periodic_dlg = NULL;
	}
}

BEGIN_EVENT_TABLE(MpGLCanvas, wxGLCanvas)
	EVT_SIZE(MpGLCanvas::eventSize)
	EVT_PAINT(MpGLCanvas::eventPaint)
	EVT_ERASE_BACKGROUND(MpGLCanvas::eventErase)
	EVT_MOUSE_EVENTS(MpGLCanvas::eventMouse)
	EVT_ACTIVATE_APP(MpGLCanvas::eventActivate)

	EVT_CHAR(MpGLCanvas::KeyHandler)

	EVT_MENU(GL_Popup_Menu_Apply_All, MpGLCanvas::On_Apply_All)
	EVT_MENU(GL_Popup_Delete_Item_Current_Frame, MpGLCanvas::On_Delete_Single_Frame)
	EVT_MENU(GL_Popup_Delete_Item_All_Frames, MpGLCanvas::On_Delete_All_Frames)
	EVT_MENU(GL_Popup_To_Single_Bond, MpGLCanvas::ChangeBonding)
	EVT_MENU(GL_Popup_To_Double_Bond, MpGLCanvas::ChangeBonding)
	EVT_MENU(GL_Popup_To_Triple_Bond, MpGLCanvas::ChangeBonding)
	EVT_MENU(GL_Popup_To_Hydrogen_Bond, MpGLCanvas::ChangeBonding)
	EVT_MENU(GL_Popup_Delete_Bond, MpGLCanvas::DeleteBond)
	EVT_MENU(GL_Popup_Measure_Length, MpGLCanvas::AddAnnotation)
	EVT_MENU(GL_Popup_Measure_Angle, MpGLCanvas::AddAnnotation)
	EVT_MENU(GL_Popup_Measure_Dihedral, MpGLCanvas::AddAnnotation)
	EVT_MENU(GL_Popup_Mark_Atom, MpGLCanvas::AddAnnotation)
	EVT_MENU(GL_Popup_Unmark_Atom, MpGLCanvas::DeleteAnnotation)
	EVT_MENU(GL_Popup_Delete_Length, MpGLCanvas::DeleteAnnotation)
	EVT_MENU(GL_Popup_Delete_Angle, MpGLCanvas::DeleteAnnotation)
	EVT_MENU(GL_Popup_Delete_Dihedral, MpGLCanvas::DeleteAnnotation)
	EVT_MENU(GL_Popup_Lock_To_Annotation, MpGLCanvas::ConstrainToAnnotation)
	EVT_MENU(GL_Popup_Fit_To_Plane, MpGLCanvas::FitToPlane)
	EVT_MENU(GL_Popup_Change_Atom, MpGLCanvas::ChangeAtom)
END_EVENT_TABLE()


// BEGIN_EVENT_TABLE(MpGLCanvas::AtomTypeDialog, wxDialog) 
	// EVT_CHOICE(ID_NEW_ATOM_TYPE_CHOICE, AtomTypeDialog::OnChoice) 
// END_EVENT_TABLE() 
