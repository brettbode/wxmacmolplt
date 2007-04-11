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

extern PeriodicTableDlg *periodic_dlg;
extern bool show_periodic_dlg;
extern int glf_initialized;

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
	if (glf_initialized) {
		glfClose();
		glf_initialized = 0;
	}

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

		/*!!! GL font initialization  */
		if (!glf_initialized) {
			glfInit();

			wxStandardPathsBase & gStdPaths = wxStandardPaths::Get();
			wxString pathname = gStdPaths.GetDataDir();
#ifdef __WXMAC__
			//wxWidgets has a funny idea of where the resources are stored. It locates them as "SharedSupport"
			//but xcode is putting them in Resources.
			pathname.Remove(pathname.Length() - 13);
			pathname += wxT("Resources");
#endif
#ifdef __WXMSW__
			pathname += wxT("\\arial1.glf");
#else
			pathname += wxT("/arial1.glf");
#endif
			if (glfLoadFont(pathname.mb_str(wxConvUTF8)) < 0) {
				std::ostringstream buf;
				buf <<"Warning: font file not found! This probably means wxmacmolplt is not"
					"properly installed. Looking for " << pathname.mb_str(wxConvUTF8);
				MessageAlert(buf.str().c_str());
				glfClose();
			} else
				glf_initialized = 1;
		}
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

void MpGLCanvas::constrain_position(const int anno_id, double *x, double *y,
									double *z) {

	Frame *lFrame = mMainData->cFrame;
	Annotation *anno = mMainData->Annotations[anno_id];

	if (!anno->containsAtom(selected)) {
		return;
	}

	int origin_id;
	CPoint3D origin_pos;

	switch (anno->getType()) {
		case MP_ANNOTATION_LENGTH:
			int shifter_id;
			CPoint3D origin_pos;
			CPoint3D shifter_pos;
			float len;

			/* Figure out which atom is moving and
			 * which is planted. */
			shifter_id = anno->getAtom(0);
			if (shifter_id == selected) {
				origin_id = anno->getAtom(1);
			} else {
				origin_id = shifter_id;
				shifter_id = anno->getAtom(1);
			}

			lFrame->GetAtomPosition(origin_id, origin_pos);
			lFrame->GetAtomPosition(shifter_id, shifter_pos);

			CPoint3D bond_vec = origin_pos - shifter_pos;
			CPoint3D bond_vec2 = shifter_pos - origin_pos;
			CPoint3D new_vec;

			new_vec.x = origin_pos.x - *x;
			new_vec.y = origin_pos.y - *y;
			new_vec.z = origin_pos.z - *z;

			float mu;
			len = bond_vec.Magnitude();
			mu = DotProduct3D(&bond_vec, &new_vec) /
				 (len * len);

			if (fabs(mu) > 1e-6) {
				*x = origin_pos.x + bond_vec2.x * mu;
				*y = origin_pos.y + bond_vec2.y * mu;
				*z = origin_pos.z + bond_vec2.z * mu;
			}

			break;
		case MP_ANNOTATION_ANGLE:

			int atom1_id;;
			int atom2_id;;
			CPoint3D vec1;
			CPoint3D vec2;
			CPoint3D atom1_pos;
			CPoint3D atom2_pos;
			CPoint3D new_pos;
			float radius;
			CPoint3D normal;
			CPoint3D vec_new;
			float dist;

			// Grab the vertex atom for the angle.  If it's also the one trying
			// to be moved, the user must not be trying to constrain anything,
			// so we let it move freely.  Or if its not being moved but is
			// selected, we don't constrain anything.
			origin_id = anno->getAtom(1);
			if (selected == origin_id || lFrame->GetAtomSelectState(origin_id)) {
				return;
			}

			// Grab other two atoms and all positions.
			atom1_id = anno->getAtom(0);
			atom2_id = anno->getAtom(2);

			lFrame->GetAtomPosition(origin_id, origin_pos);
			lFrame->GetAtomPosition(atom1_id, atom1_pos);
			lFrame->GetAtomPosition(atom2_id, atom2_pos);

			// Find vectors from vertex atoms to other atoms.
			vec1 = atom1_pos - origin_pos;
			vec2 = atom2_pos - origin_pos;

			// We want to find the distance between the vertex atom and the
			// one clicked on.  That distance will serve as the radius of
			// the circle that the atom will follow.
			if (selected == atom1_id) {
				radius = vec1.Magnitude();
				if (lFrame->GetAtomSelectState(atom2_id)) {
					return;
				}
			} else {
				radius = vec2.Magnitude();
				if (lFrame->GetAtomSelectState(atom1_id)) {
					return;
				}
			}

			// We need to take the mouse point in object space and find the
			// nearest point on the plane defined by the three angle atoms.
			// Once that point is on the plane, we can more easily bring it
			// onto the circle path that the atom can move along.
			
			// Calculate the plane's normal.
			CrossProduct3D(&vec1, &vec2, &normal);
			Normalize3D(&normal);

			new_pos.x = *x;
			new_pos.y = *y;
			new_pos.z = *z;

			// Find vector between point on plane and vertex.  The point's
			// distance from the plain is just the dot product.
			vec_new = new_pos - origin_pos;
			dist = DotProduct3D(&normal, &vec_new);

			// Now, drop that point down to the plane in the direction reverse
			// to the normal, by the correct distance.  After this, the 
			// point is on the plane.
			new_pos = vec_new - normal * dist;

			// Now, we need to reel (or cast) the plane point so it's the
			// same distance away as the atom originally was.
			vec_new = new_pos - origin_pos;
			Normalize3D(&vec_new);

			*x = origin_pos.x + radius * vec_new.x;
			*y = origin_pos.y + radius * vec_new.y;
			*z = origin_pos.z + radius * vec_new.z;

			break;
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

	// if (1) { 
		// printf("event.LeftDown(): %d\n", event.LeftDown()); 
		// printf("event.LeftUp(): %d\n", event.LeftUp()); 
		// printf("event.RightDown(): %d\n", event.RightDown()); 
		// printf("event.RightUp(): %d\n", event.RightUp()); 
		// printf("event.Dragging(): %d\n", event.Dragging()); 
		// printf("event.CmdDown(): %d\n", event.CmdDown()); 
		// printf("event.ShiftDown(): %d\n", event.ShiftDown()); 
	// } 

	// First handle left mouse down.
	if (event.LeftDown() || (event.LeftIsDown() && stale_click)) {

		mSelectState = 0;
		selected = testPicking(tmpPnt.x, tmpPnt.y);

		if (interactiveMode) {
			if (selected >= 0 && selected < NumAtoms) {
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
		selected = testPicking(tmpPnt.x, tmpPnt.y);

		if (selected < 0) {
			// We don't really do anything here, since the user didn't 
			// click on anything.  But by including this null block, we
			// don't need to test for this anymore.
		}

		else if (selected < NumAtoms) {
			if (interactiveMode) {
				interactPopupMenu(tmpPnt.x, tmpPnt.y, 1);
				MolWin->SelectionChanged(deSelectAll);
				MolWin->UpdateGLModel();
			} else {
				measurePopupMenu(tmpPnt.x, tmpPnt.y);
			}
		}

		else if (selected < NumAtoms + lFrame->NumBonds) {
			if (interactiveMode) {
				interactPopupMenu(tmpPnt.x, tmpPnt.y, 0);
				MolWin->SelectionChanged(deSelectAll);
				MolWin->UpdateGLModel();
			} else {
				bondPopupMenu(tmpPnt.x, tmpPnt.y);
			}
		}

		else if (selected < NumAtoms + lFrame->NumBonds +
				 mMainData->GetAnnotationCount()) {
			annoPopupMenu(tmpPnt.x, tmpPnt.y);
		}
	}

	else if (event.MiddleDown()) {

		selected = testPicking(tmpPnt.x, tmpPnt.y);

	}

	// If we made it this far, button states haven't changed.  Are we dragging?
	else if (event.Dragging()) {
		mSelectState++;

		// Are we dragging in edit mode?
		if (interactiveMode) {

			// If an atom is clicked on...
			if (selected >= 0 && selected < NumAtoms) {

				edited_atoms = true;
				GLdouble newX, newY, newZ;

				// If shift is held when an atom is clicked on, we want to
				// change the depths of either the clicked on or selected
				// atoms.  Also do this for the middle mouse button.
				if (event.ShiftDown() || event.MiddleIsDown()) {
					GLdouble tmpX, tmpY, tmpZ;
					float depth_offset;
					float dy = tmpPnt.y - oldTmpPnt.y;

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
					findReal3DCoord(tmpPnt.x - winDiffX, tmpPnt.y - winDiffY,
									atomDepth, newX, newY, newZ);

					int constrain_anno_id = mMainData->GetConstrainAnnotation();
					if (constrain_anno_id != -1) {
						constrain_position(constrain_anno_id, &newX, &newY,
							&newZ);
					}
				}

				// If that atom is a member of the selected atom set, move all
				// atoms that are currently selected.
				if (lFrame->GetAtomSelectState(selected)) {
					GLdouble offset_x, offset_y, offset_z;

					offset_x = lAtoms[selected].Position.x - newX;
					offset_y = lAtoms[selected].Position.y - newY;
					offset_z = lAtoms[selected].Position.z - newZ;

					for (int i = 0; i < lFrame->GetNumAtoms(); i++) {
						if (lFrame->GetAtomSelectState(i)) {
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

					SelectObj(selected, deSelectAll);
					MolWin->SelectionChanged(deSelectAll);
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
			else if (selected >= NumAtoms && selected < NumAtoms + lFrame->NumBonds) {
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
				dy = tmpPnt.y - oldTmpPnt.y;
				angle_offset= dy / GetRect().GetHeight() * 540.0f;

				// Get all trig together for rotating around the bond that
				// was just clicked on.
				radians = angle_offset * kPi / 180.0f;
				sine = sin(radians);
				cosine = cos(radians);
				c_inv = 1.0f - cosine;

				// The axis of rotation is a vector from one atom of the
				// bond to the other.
				lFrame->GetAtomPosition(
					lFrame->GetBondAtom(selected - NumAtoms, 1), pivot_pt);
				lFrame->GetAtomPosition(
					lFrame->GetBondAtom(selected - NumAtoms, 2), other_pt);
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

				rot_mat[3][0] = rot_mat[3][1] = rot_mat[3][3] = 0.0f;
				rot_mat[0][3] = rot_mat[1][3] = rot_mat[2][3] = 0.0f;
				rot_mat[3][3] = 1.0f;

				// For each selected atom, we need to rotate it around the
				// bond.  We want one of the bond atom's to act as the origin,
				// so we translate our coordinate system there,
				// perform the rotation, and then translate back.
				for (int i = 0; i < lFrame->GetNumAtoms(); i++) {
					if (lFrame->GetAtomSelectState(i)) {
						lAtoms[i].Position -= pivot_pt;
						Rotate3DPt(rot_mat, lAtoms[i].Position, &new_pt);
						lAtoms[i].Position = new_pt;
						lAtoms[i].Position += pivot_pt;
					}
				}

				// We need to update our data.
				edited_atoms = true;
			}

			else if (selected >= NumAtoms + lFrame->NumBonds) {

				int anno_id = selected - NumAtoms - lFrame->NumBonds;
				
				if (mMainData->Annotations[anno_id]->getType() == MP_ANNOTATION_LENGTH) {
					float dy;           // No. pixels of mouse change in y-dir
					float offset;       // Corresponding amount of translation
					CPoint3D atom1_pos;
					CPoint3D atom2_pos;
					CPoint3D offset_vec;
					AnnotationLength *length_anno;

					length_anno =
						dynamic_cast<AnnotationLength *>
							(mMainData->Annotations[anno_id]);
				   
					// Calculate the amount of rotation according to the amount
					// of mouse change along the y-axis of the viewport.
					dy = tmpPnt.y - oldTmpPnt.y;
					offset = 1.0f - dy / (GetRect().GetHeight()) * 2.0f;

					lFrame->GetAtomPosition(length_anno->getAtom(0), atom1_pos);
					lFrame->GetAtomPosition(length_anno->getAtom(1), atom2_pos);

					offset_vec = atom2_pos - atom1_pos;

					// get atom2
					atom2_pos.x = atom1_pos.x + offset_vec.x * offset;
					atom2_pos.y = atom1_pos.y + offset_vec.y * offset;
					atom2_pos.z = atom1_pos.z + offset_vec.z * offset;

					lFrame->SetAtomPosition(length_anno->getAtom(1), atom2_pos);

					edited_atoms = true;
				}
			}
			
			else {
				mSelectState = -1;
			}
			
		}
		
	}

	// If the left mouse button is released, the user is either done dragging,
	// in which case we do nothing, or the user has selected an item.
	else if (event.LeftUp()) {
		if (event.CmdDown())
			deSelectAll = false;

		// Allow a little bit of dragging to be interpreted as selection.
		if (mSelectState >= 0 && mSelectState < 3) {
			mSelectState = -1;

			// If editing, we  
			if (interactiveMode) {

				// If the user clicked on nothing, we try to add an atom given
				// the selected element on the periodic table palette.
				if (selected < 0 && periodic_dlg &&
					periodic_dlg->GetSelectedID() != 0) {

					// if (periodic_dlg) { 
						// if (periodic_dlg->GetSelectedID() == 0) { 
							// periodic_dlg->Raise(); 
						// } 
					// } else { 
						// wxRect window_rect = GetRect(); 
						// periodic_dlg = new PeriodicTableDlg( 
												// this, wxT("Periodic Table"), 
												// window_rect.x + window_rect.width, 
												// window_rect.y + 30); 
												
						// periodic_dlg->Show(); 
					// } 
  
					// If an element is selected, add an instance of it.
					// if (periodic_dlg->GetSelectedID() != 0) { 
							GLdouble newX, newY, newZ;
  
							findWinCoord(0.0, 0.0, 0.0, newX, newY, atomDepth);
							//estimate an atomDepth value, X and Y values are of no use 
							CPoint3D newPnt;
							findReal3DCoord((GLdouble)tmpPnt.x, (GLdouble)tmpPnt.y,
												atomDepth, newX, newY, newZ);
							newPnt.x = newX;
							newPnt.y = newY;
							newPnt.z = newZ;
  
							mMainData->NewAtom(periodic_dlg->GetSelectedID(), newPnt);
						// } 
  
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

			SelectObj(selected, deSelectAll);
			MolWin->SelectionChanged(deSelectAll);
			MolWin->UpdateGLModel();

		}

		if (mDragWin) {
			mDragWin->EndDrag();
			delete mDragWin;
			mDragWin = NULL;
		}
	}

	else if (event.ButtonUp()) {
		selected = -1;
	}
	
	oldTmpPnt = tmpPnt;
	stale_click = false;

	if (interactiveMode && edited_atoms) {
		draw();
	}

	// Pass mouse event to MolDisplayWin::Rotate for processing
	else {
		// printf("rotated scene\n"); 
		MolWin->Rotate(event);
	}

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

int MpGLCanvas::testPicking(int x, int y) {
	GLuint buff[128];
	GLint hits, view[4];
	int id;

	glSelectBuffer(128, buff);
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

	int select_id = -1;
	unsigned int min_depth = 0xFFFFFFFF;

	for (int i = 0; i < hits; i++) {
		if (buff[i*4+1] < min_depth) {
			min_depth = buff[i*4+1];
			select_id = buff[i*4+3];
		}
	}

	return (select_id-1);
}

void MpGLCanvas::SelectObj(int select_id, bool unselect_all) 
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
		if (select_id < NumAtoms) {
			bool atom_is_selected = true;

			// If we're to keep other selected items and atom is already
			// selected, then we must be unselecting this atom.
			if (!unselect_all && lAtoms[select_id].GetSelectState())
				atom_is_selected = false;

			lAtoms[select_id].SetSelectState(atom_is_selected);
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

				if (lAtoms[atom1].GetSelectState() &&
					lAtoms[atom2].GetSelectState())
					lBonds[i].SetSelectState(atom_is_selected);
			}
		}
		
		// If select_id indicates a bond
		else if (select_id < (NumAtoms + lFrame->NumBonds)) {
			select_id -= NumAtoms;
			bool newstate = true;
			if (!unselect_all && lBonds[select_id].GetSelectState())
				newstate = false;
			lBonds[select_id].SetSelectState(newstate);

			long atom1 = lBonds[select_id].Atom1;
			long atom2 = lBonds[select_id].Atom2;

			lAtoms[atom1].SetSelectState(newstate);
			lAtoms[atom2].SetSelectState(newstate); //select atoms that this bond connect
		}

		bool result = false;
		for (long i=0; i<NumAtoms; i++) {
			if (lAtoms[i].GetSelectState()) {
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
	}

	// If a bond is clicked on, we show some bond specific items, like
	// the length of the bond and the order.
	if (!isAtom) {
		length_label.Printf(wxT("Bond length: %f"),
			lFrame->GetBondLength(selected - NumAtoms));
		item = menu.Append(wxID_ANY, length_label);
		item->Enable(false);
		bond_order = lFrame->Bonds[selected - NumAtoms].Order;
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
		menu.Append(GL_Popup_Menu_Apply_All, _T("&Apply to All Frames"));
	}

	menu.Append(GL_Popup_Delete_Item_Current_Frame, _T("&Delete"));
	menu.Append(GL_Popup_Delete_Item_All_Frames, _T("&Delete in All Frames"));

	PopupMenu(&menu, x, y);

}
//make a popup menu editing the objets in the scene

void MpGLCanvas::insertAnnotationMenuItems(wxMenu& menu) {

	wxMenuItem *item;
	Frame *lFrame = mMainData->cFrame;
	bool already_exists = false;
	std::vector<Annotation *>::const_iterator anno;
	int anno_id = 0;

	if ((selected >= 0) && (selected < lFrame->GetNumAtoms())) {
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
		if (selected >= 0 && selected < lFrame->GetNumAtoms() &&
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
	int anno_id = selected - lFrame->NumAtoms - lFrame->NumBonds;

	if (interactiveMode &&
		mMainData->Annotations[anno_id]->getType() != MP_ANNOTATION_MARKER) {
		item = menu.AppendCheckItem(GL_Popup_Lock_To_Annotation,
				                    "Constrain atoms");
		if (anno_id == mMainData->GetConstrainAnnotation()) {
			item->Check(true);
		}
	}
	menu.Append(GL_Popup_Delete_Length, "Delete annotation");
	PopupMenu(&menu, x, y);

}

void MpGLCanvas::ConstrainToAnnotation(wxCommandEvent& event) {

	Frame *lFrame = mMainData->cFrame;
	int anno_id = selected - lFrame->NumAtoms - lFrame->NumBonds;

	if (mMainData->GetConstrainAnnotation() != anno_id) {
		mMainData->ConstrainToAnnotation(selected - lFrame->NumAtoms -
			lFrame->NumBonds);
	} else {
		mMainData->RemoveAnnotationConstraint();
	}

}

void MpGLCanvas::DeleteAnnotation(wxCommandEvent& event) {

	Frame *lFrame = mMainData->cFrame;
	int selected_anno = selected - lFrame->NumAtoms - lFrame->NumBonds;

	if (selected_anno < 0 || selected_anno >= mMainData->Annotations.size()) {
		return;
	}

	Annotation * t = mMainData->Annotations[selected_anno];
	mMainData->Annotations.erase(mMainData->Annotations.begin() + selected_anno);
	delete t;

	int constrain_anno_id = mMainData->GetConstrainAnnotation();
	if (constrain_anno_id != -1) {
		/* If we're deleting an annotation that appears earlier in the
		 * annotation list, we need to shift the id of the constrained
		 * annotation. */
		if (constrain_anno_id > selected_anno) {
			mMainData->ConstrainToAnnotation(constrain_anno_id - 1);
		}
		
		/* Or, we may be deleting the constrained annotation itself, in which
		 * case, we have no more constraints. */
		else if (constrain_anno_id == selected_anno) {
			mMainData->RemoveAnnotationConstraint();
		}
	}
}

void MpGLCanvas::bondPopupMenu(int x, int y) {

	// This function shows a popup menu that shows some information and
	// operations for the clicked-on bond.  It's assumed that selected is
	// a valid index in the bonds lists, plus NumAtoms.

	wxMenu menu;
	wxMenuItem *item;
	wxMenu *submenu;
	Frame *lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;
	int bond_order;
	submenu = new wxMenu();
	wxString length_label;

	length_label.Printf(wxT("Bond length: %f"),
					lFrame->GetBondLength(selected - NumAtoms));
	item = menu.Append(wxID_ANY, length_label);
	item->Enable(false);
	bond_order = lFrame->Bonds[selected - NumAtoms].Order;
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
	
	if (selected >= lFrame->GetNumAtoms()) {	//existing bond, change order
		bond = &(lFrame->Bonds[selected - lFrame->GetNumAtoms()]);
		bond->Order = order;
		MolWin->BondsChanged();
		lFrame->resetAllSelectState();
		bond->SetSelectState(true);
		lFrame->Atoms[bond->Atom1].SetSelectState(true);
		lFrame->Atoms[bond->Atom2].SetSelectState(true);
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
		lFrame->Atoms[select_stack[0]].SetSelectState(true);
		lFrame->Atoms[select_stack[1]].SetSelectState(true);
	}
}

void MpGLCanvas::DeleteBond(wxCommandEvent& event) {
	
	// Delete the selected bond
	
	Frame *lFrame = mMainData->cFrame;
	
	if (selected >= lFrame->GetNumAtoms()) {
		lFrame->DeleteBond(selected - lFrame->GetNumAtoms());
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

	if (selected < 0 || selected >= NumAtoms)
		return;

	Frame * cFrame = mMainData->Frames;

	for ( int i = 0; i < mMainData->NumFrames; i++) {
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

	if (selected >= 0 && selected < NumAtoms)
		mMainData->DeleteAtom(selected);

	if (selected >= NumAtoms)
		lFrame->DeleteBond(selected - NumAtoms);

}

void MpGLCanvas::On_Delete_All_Frames(wxCommandEvent& event) {
	Frame * lFrame = mMainData->Frames;
	long NumAtoms = mMainData->cFrame->NumAtoms;
	if (selected >= 0 && selected < NumAtoms)
		mMainData->DeleteAtom(selected, true);

	else if (selected < (NumAtoms+lFrame->GetNumBonds())) {
		while (lFrame) {
			long NumAtoms = lFrame->NumAtoms;


			if (selected >= NumAtoms)
				lFrame->DeleteBond(selected - NumAtoms);

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
END_EVENT_TABLE()


// BEGIN_EVENT_TABLE(MpGLCanvas::AtomTypeDialog, wxDialog) 
	// EVT_CHOICE(ID_NEW_ATOM_TYPE_CHOICE, AtomTypeDialog::OnChoice) 
// END_EVENT_TABLE() 
