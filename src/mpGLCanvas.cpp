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

#include <wx/stdpaths.h>
#ifdef __WXOSX_CARBON__
#include <AGL/glu.h>
#elif __WXOSX_COCOA__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <iostream>
#include <sstream>

#include "main.h"
#include "build_palette.h"
#include "Math3D.h"
#include "VirtualSphere.h"
#include "SurfaceTypes.h"

/* ------------------------------------------------------------------------- */

extern int glf_initialized;
extern BuilderInterface * BuilderTool;

/* ------------------------------------------------------------------------- */

MpGLCanvas::MpGLCanvas(MolDisplayWin *parent, wxWindowID id,
					   const wxPoint& position, const wxSize& size,
					   int *attribs, bool do_stereo, long style,
					   const wxString& name)
	:
#if wxCHECK_VERSION(2,9,0)
	 wxGLCanvas((wxWindow *) parent, id, attribs, position, size,
				style | wxFULL_REPAINT_ON_RESIZE, name),
#else
	 wxGLCanvas((wxWindow *) parent, id, position, size,
				style | wxFULL_REPAINT_ON_RESIZE, name, attribs),
#endif
	  do_stereo(do_stereo) {

#if wxCHECK_VERSION(2,9,0)
	context = new wxGLContext(this);
#endif

	Prefs = NULL;
	MolWin = parent;
	initialized = false;

	was_zooming = false;

	mMainData = parent->GetData();

	mSelectState = -1;
	oldSelect = -1;
	select_stack_top = 0;
	ignore_next_up = false;
	is_lassoing = false;
	did_edit = false;
	
	selected = -1;
	selected_type = MMP_NULL;
	selected_site = -1;
	first_site_clicked = -1;
	first_atom_clicked = -1;
	ndrag_events = 0;
	bitmap_fontd = -1;

}

/**
 * Deconstructs the canvas and all associated resources.
 */	
MpGLCanvas::~MpGLCanvas() {

	// Since each canvas has its own bitmap font textures, we free those
	// resources.  Since GLF tries to free some OpenGL resources, we need to
	// bring the canvas' context back up -- otherwise another canvas' textures
	// will be deleted.
	// SetCurrent(); 
	glfUnloadBMFFontD(bitmap_fontd);

}

/**
 Initializes OpenGL state for the canvas. The OpenGL context isn't quite ready
 in the canvas' constructor, so we have to make this a separate function and
 call it after the canvas has been shown.
 */
void MpGLCanvas::initGL(void) {

	glewInit();

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

	vector_font = pathname + wxT("/arial1.glf");
	bitmap_font = pathname + wxT("/arial1.bmf");

	if (glfLoadBMFFont(bitmap_font.mb_str(wxConvUTF8)) < 0) {
		std::ostringstream buf;
		buf << "Warning: font file not found! This probably means wxmacmolplt is not "
		       "properly installed. Looking for " << bitmap_font.mb_str(wxConvUTF8);
		MessageAlert(buf.str().c_str());
		glfClose();
		glf_initialized = 0;
	} else {
		bitmap_fontd = glfGetCurrentBMFFont();
	}

	// Initialize the OpenGL state here.
	glEnable(GL_DEPTH_TEST);
	
	//  glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glPolygonOffset (1.0, 1.0);
	
	GLfloat mat_specular[] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat mat_shininess[] = {80.0};
	GLfloat mat_diffuse[] = {0.2f,0.2f,0.2f,0.8f};
	GLfloat mat_ambient[] = {0.1f,0.1f,0.1f,0.8f};

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);

	// Setup the static lighting properties.
	GLfloat model_ambient[4] = {0.1f, 0.1f, 0.1f, 0.1f};
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

	// We make a texture of arrowheads for the annotations. The arrowheads
	// will point in the direction the annotations can be moved.
	unsigned char texture[16 * 16] = {
		255,   0,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255, 255,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255, 255,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255, 255,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255, 255,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255, 255,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		255, 255,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
		255,   0,   0,   0,   0,   0,   0,   0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	glGenTextures(1, &(MolWin->length_anno_tex_id));
	glBindTexture(GL_TEXTURE_2D, MolWin->length_anno_tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 16, 16, 0, GL_ALPHA,
				 GL_UNSIGNED_BYTE, texture);

	// Generate projection, quadric objects, shader uniforms, etc.
	MolWin->DoPrefDependent();

	// Don't initialize more than once, so set a flag.
	initialized = true;

}

/**
 * Associate preferences with this canvas.
 * @param newPrefs Pointer to existing preferences instance.
 */
void MpGLCanvas::SetPrefs(WinPrefs *newPrefs) {
	Prefs = newPrefs;
}

wxImage MpGLCanvas::getImage(const int width, const int height) {

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	SetCurrent();
#endif
	glfSetCurrentBMFFont(bitmap_fontd);
	// TODO:  respect width/height
	// TODO:  avoid grabbing the menu
	GLint view[4];
	GLint cwidth, cheight;
	unsigned char *pixels;

	glGetIntegerv(GL_VIEWPORT, view);
	cwidth = view[2];
	cheight = view[3];

	// malloc is required by wxImage,
	// which takes responsibility for the memory
	pixels = (unsigned char *) malloc(3 * cwidth * cheight * sizeof(GLbyte));
	memset(pixels, 0, 3 * cwidth * cheight * sizeof(GLbyte));
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Draw into the back buffer.
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	SetProjection(((float) cwidth) / cheight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	MolWin->DrawGL();
	glFinish();

	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, cwidth, cheight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	// create a wxImage from the data, and mirror it vertically
	return wxImage(cwidth, cheight, pixels).Mirror(false);
}

void MpGLCanvas::GenerateHiResImage(wxDC * dc, const float & ScaleFactor, 
									Progress * progress, bool Center,
									bool frame) {

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	SetCurrent();
#endif
	glfSetCurrentBMFFont(bitmap_fontd);

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
	
	/* unsigned char *pixels = (unsigned char *) malloc(3 * width * height *  */
									/* sizeof(GLbyte)); */
	unsigned char *pixels = new unsigned char[3 * width * height];
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
	
	MolWin->ReleaseLists();

	for (int jpass=0; jpass<NumYPasses; jpass++) {
		int passheight = height;
		if ((jpass+1) == NumYPasses) passheight = height - (ViewportScaledY - ScaledHeight);
		for (int ipass=0; ipass<NumXPasses; ipass++) {
			int passwidth = width;
			if ((ipass+1) == NumXPasses) passwidth = width - (ViewportScaledX - ScaledWidth);
			
			//Setup the projection matrix to view the correct piece of the view for this pass
			glMatrixMode(GL_PROJECTION);   //Setup the model space to screen space mapping
			glLoadIdentity();
			GLdouble top, bottom, left, right;
			left = GLLeft + ipass*hGLsize;
			right = left + hGLsize;
			top = GLTop - jpass*vGLsize;
			bottom = top - vGLsize;
			glFrustum(left, right, bottom, top, zNear, 1000.0);
			
			glDrawBuffer(GL_BACK);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			MolWin->DrawGL();
			glFinish();

			glReadBuffer(GL_BACK);
			memset(pixels, 0, 3*width*height*sizeof(GLbyte));
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

			wxCoord x = ipass*width + hOffset;
			wxCoord y = jpass*height + vOffset;
			//I am not sure this is the best way to do this, but it
			//converts the pixels to a wxImage, flips it vertically to get
			//it into the right orientation, then converts it to a bitmap
			//to draw into the device context. We retain ownership of the pixels
			//and thus have to delete them below.
			dc->DrawBitmap(wxBitmap(wxImage(width, height, pixels, true).Mirror(false)),
							x, y, false);
			 
		}
	}
	delete[] pixels;
	/* free(pixels); */
	 
	if (frame) {
		wxPen lpen = dc->GetPen();
		if (lpen == wxNullPen) {
			lpen = wxPen(*wxBLACK_PEN);
		}
		lpen.SetWidth((int)ScaleFactor);
		dc->SetBrush(*wxTRANSPARENT_BRUSH);
		dc->SetPen(lpen);
		dc->DrawRectangle(hOffset, vOffset, ScaledWidth, ScaledHeight);
	}
	 MolWin->ReleaseLists();
}

void MpGLCanvas::GenerateHiResImageForExport(wxDC *dc) {

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

	GLbyte *pixels;

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	SetCurrent();
#endif
	glfSetCurrentBMFFont(bitmap_fontd);

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
	
	MolWin->ReleaseLists();
	
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
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
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
	
	MolWin->ReleaseLists();
}

/**
 * This function multiplies the current matrix by a projection matrix defined
 * by the preferences.  The calling function should set OpenGL's matrix mode
 * to GL_PROJECTION (and call glLoadIdentity) before calling this function.
 * @param aspect_ratio The width-to-height ratio of the window.
 */
void MpGLCanvas::SetProjection(float aspect_ratio) {

	GLdouble zNear = 0.1;

	// half_width is half the width of the near clipping plane.
	GLdouble half_width = zNear * tan(Prefs->GetGLFOV() * kPi / 180.0);

	// At the moment the prefs limit the GLFOV to > 0 so the glOrtho code will
	// not get run. Before it can be activated need to figure out what to do
	// with it in the hi-res image export routines.
	GLdouble top, right;
	if (aspect_ratio > 1.0) {
		right = half_width;
		top = right / aspect_ratio;
	} else {
		top = half_width;
		right = top * aspect_ratio;
	}

	if (half_width > 0.001) {
		glFrustum(-right, right, -top, top, zNear, 1000.0);
	} else {
		if (aspect_ratio > 1.0) {
			right = mMainData->WindowSize;
			top = right / aspect_ratio;
		} else {
			top = mMainData->WindowSize;
			right = top * aspect_ratio;
		}
		glOrtho(-right, right, -top, top, zNear, 1000.0);
	}

}

void MpGLCanvas::eventSize(wxSizeEvent &event) {

#if !wxCHECK_VERSION(2,9,0)
	wxGLCanvas::OnSize(event);
#endif
	GetClientSize(&width, &height);
	Update();
	Refresh();

}

/**
 * Handles paint events for the OpenGL canvas. Called by event handler after
 * various events that invalidate the window.
 * @param event The wx paint event.
 */
void MpGLCanvas::eventPaint(wxPaintEvent &event) {

	wxPaintDC paintDC(this); // wx requires this. I hope it goes away soon.
	Draw();

}

/**
 * Draws the molecules and all related structures. This is the function that
 * should be called for normal scene drawing. It sets user-specified matrices
 * and draws to fill the canvas dimensions. If you need to do anything that
 * deviates from the standard viewport and matrices, you should call
 * MolDisplayWin::DrawGL() directly after customizing the OpenGL state for
 * your needs. If an operation is in progress, the screen is cleared and 
 * no other drawing is done.
 */
void MpGLCanvas::Draw() {

#if wxCHECK_VERSION(2,9,0)
	if (!IsShownOnScreen()) {
		return;
	}
#else
	if (!GetContext()) {
		return;
	}
#endif

	if (!MolWin->IsShown() || Prefs == NULL) {
		return;
	}

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	SetCurrent();
#endif

	if (!initialized) {
		initGL();
	}

	// Each canvas has its own font instance and hence display lists, so
	// we activate this canvas' lists.
	glfSetCurrentBMFFont(bitmap_fontd);

	// Setup OpenGL matrices.  The projection is defined according to the
	// preferences, and we remain in eye space for now.
	glViewport(0, 0, (GLint) width, (GLint) height);

	/*glDrawBuffer(GL_BACK); */
	/* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */

	// Only do the drawing if there is not an operation in progress otherwise
	// the underlying data may not be complete.
	if (MolWin->OperInProgress()) {
		return;
	}

	if (!do_stereo) {
#if 1
		if (Prefs->GetShaderMode() == 2) {
			CPoint3D world_focus;
			Rotate3DPt(mMainData->TotalRotation,
					   mMainData->MolCentroid - mMainData->Centroid,
					   &world_focus);
			world_focus.z -= mMainData->WindowSize;
			float max_reach = 1.5f * sqrtf(3.0f * mMainData->MaxSize);

			// Render to depth texture.
			glBindTexture(GL_TEXTURE_2D, MolWin->depth_tex_id);
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, MolWin->depth_fbo);

			glClear(GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, FBO_SIZE, FBO_SIZE);

			CPoint3D a(MolWin->light_pos[0], MolWin->light_pos[1], MolWin->light_pos[2]);
			float dist = (world_focus - a).Magnitude();

			float theta = 2.0f * atan(max_reach / dist) * kRadToDegree;
			float my_near = dist - max_reach;
			if (my_near < 0.0f) {
				my_near = 0.1f;
			}

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(theta, 1.0f, my_near, dist + max_reach);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(MolWin->light_pos[0],
					  MolWin->light_pos[1],
					  MolWin->light_pos[2],
					  world_focus.x, world_focus.y, world_focus.z,
					  0.0, 1.0, 0.0);

			glPolygonOffset(4, 4);
			glEnable(GL_POLYGON_OFFSET_FILL);
			bool prev_rotate_mode = MolWin->do_rotate_annotation;
			MolWin->do_rotate_annotation = false;
			MolWin->DrawGL(false);
			MolWin->do_rotate_annotation = prev_rotate_mode;
			glDisable(GL_POLYGON_OFFSET_FILL);

			// Back to normal now.
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
			glViewport(0, 0, (GLint) width, (GLint) height);

			// Set up texture matrix so that it transforms vertex coordinates
			// into indices for the depth texture.
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glTranslatef(0.5f, 0.5f, 0.5f);
			glScalef(0.5f, 0.5f, 0.5f);
			gluPerspective(theta, 1.0f, my_near, dist + max_reach);
			gluLookAt(MolWin->light_pos[0], MolWin->light_pos[1], MolWin->light_pos[2],
					  world_focus.x, world_focus.y, world_focus.z,
					  0.0, 1.0, 0.0);
		}
#endif

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		SetProjection(((float) width) / height);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// MolWin's got the goods, so hand off the rest of the work.
		MolWin->DrawGL();
	} else {
		float near_z = 0.1f;
		float far_z = 1000.0f;
		float focal_length = 1.0f * mMainData->WindowSize;
		float radians = Prefs->GetGLFOV() * kPi / 180.0f;
		float half_width = near_z * tan(radians);
		float ndfl = near_z / focal_length;
		float left, right, bottom, top;
		float eye_separation = focal_length / Prefs->GetStereoOffset();
		
		CPoint3D dir(0.0f, 0.0f, -1.0f);
		CPoint3D up(0.0f, 1.0f, 0.0f);
		CPoint3D r;
		CrossProduct3D(&dir, &up, &r);
		Normalize3D(&r);
		r *= eye_separation * 0.5f;
		
		// Left.
		glDrawBuffer(GL_BACK_LEFT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		float aspect_ratio = ((float) width) / height;
		left = -aspect_ratio * half_width + 0.5f * eye_separation * ndfl;
		right = aspect_ratio * half_width + 0.5f * eye_separation * ndfl;
		top = half_width;
		bottom = -half_width;
		
		glFrustum(left, right, bottom, top, near_z, far_z);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(r.x, r.y, r.z);
		MolWin->DrawGL();

		// Right.
		glDrawBuffer(GL_BACK_RIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		left = -aspect_ratio * half_width - 0.5f * eye_separation * ndfl;
		right = aspect_ratio * half_width - 0.5f * eye_separation * ndfl;
		top = half_width;
		bottom = -half_width;
		
		glFrustum(left, right, bottom, top, near_z, far_z);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(-r.x, -r.y, -r.z);
		MolWin->DrawGL();
	}
	
	SwapBuffers();

}

void MpGLCanvas::eventErase(wxEraseEvent &event) {
	// Don't mess with this.  It's supposed to be empty.
	// This avoids flashing on Windows.
}

void MpGLCanvas::ConstrainPosition(const int anno_id, double x, double y,
								   double z) {

	Frame *lFrame = mMainData->cFrame;
	mpAtom *lAtoms = mMainData->cFrame->Atoms;
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

			new_vec.x = x - anchor_pos.x;
			new_vec.y = y - anchor_pos.y;
			new_vec.z = z - anchor_pos.z;

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
				x = anchor_pos.x + bond_vec.x * mu;
				y = anchor_pos.y + bond_vec.y * mu;
				z = anchor_pos.z + bond_vec.z * mu;
			}

			float offset_x, offset_y, offset_z;
			offset_x = x - shifter_pos.x;
			offset_y = y - shifter_pos.y;
			offset_z = z - shifter_pos.z;

			for (int i = 0; i < lFrame->GetNumAtoms(); i++) {
				if (lFrame->GetAtomSelection(i)) {
					lAtoms[i].Position.x += offset_x;
					lAtoms[i].Position.y += offset_y;
					lAtoms[i].Position.z += offset_z;
				}
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
			if (selected == vertex_id) {
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

			CPoint3D vec_old;

			if (lFrame->GetAtomSelection(vertex_id)) {
				lFrame->SetAtomSelection(vertex_id, false);
			}

			// We want to find the distance between the vertex atom and the
			// one clicked on.  That distance will serve as the radius of
			// the circle whose circumference the atom will follow.  If the
			// other atom is selected, we abandon the constraints because
			// moving both atoms undoes the work of the constrained
			// translation.
			if (selected == atom1_id) {
				radius = vec1.Magnitude();
				if (lFrame->GetAtomSelection(atom2_id)) {
					lFrame->SetAtomSelection(atom2_id, false);
				}
				vec_old = CPoint3D(vec1);
			} else {
				radius = vec2.Magnitude();
				if (lFrame->GetAtomSelection(atom1_id)) {
					lFrame->SetAtomSelection(atom1_id, false);
				}
				vec_old = CPoint3D(vec2);
			}

			// We need to take the mouse point in object space and find the
			// nearest point on the plane defined by the three angle atoms.
			// Once that point is on the plane, we can more easily bring it
			// onto the circle's circumference that the atom can move along.
			
			// Calculate the plane's normal.
			CrossProduct3D(&vec1, &vec2, &normal);
			Normalize3D(&normal);

			new_pos.x = x;
			new_pos.y = y;
			new_pos.z = z;

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
			Normalize3D(&vec_old);
			float dot = DotProduct3D(&vec_new, &vec_old);

			CrossProduct3D(&vec_old, &vec_new, &normal);
			Normalize3D(&normal);

			Matrix4D rotate;
			RotateAroundAxis(rotate, normal, acos(dot) * 180.0f / kPi);

			CPoint3D pt;
			for (int i = 0; i < lFrame->GetNumAtoms(); i++) {
				if (lFrame->GetAtomSelection(i)) {
					lFrame->GetAtomPosition(i, pt);
					vec_old = pt - vertex_pos;
					Rotate3DPt(rotate, vec_old, &vec_new);
					lFrame->SetAtomPosition(i, vertex_pos + vec_new);
				}
			}

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
			int atom1_id;
			int atom2_id;
			int atom3_id;
			int atom4_id;
			CPoint3D atom1_pos;
			CPoint3D atom2_pos;
			CPoint3D atom3_pos;
			CPoint3D atom4_pos;
			CPoint3D vec1;
			CPoint3D vec2;
			CPoint3D new_pos;
			CPoint3D center_pos;
			float radius;
			CPoint3D normal1;
			CPoint3D normal2;
			CPoint3D vec_new;
			float vec1_len;
			float dist;
			CPoint3D origin_pos;
			float mu;
			CPoint3D vec3;
			CPoint3D vec4;

			// First we need to figure out if its the first or fourth atom
			// of the annotation that's being moved.  We allow only one atom
			// of the annotation to be selected.  If this is not true, or
			// its the second or third atom being shifted, then we impose
			// no constraints.
			if (selected == anno->getAtom(0)) {
				atom1_id = anno->getAtom(0);
				atom2_id = anno->getAtom(1);
				atom3_id = anno->getAtom(2);
				atom4_id = anno->getAtom(3);
			} else if (selected == anno->getAtom(3)) {
				atom1_id = anno->getAtom(3);
				atom2_id = anno->getAtom(2);
				atom3_id = anno->getAtom(1);
				atom4_id = anno->getAtom(0);
			} else {
				return;
			}

			/* lFrame->SetAtomSelection(atom1_id, true); */
			lFrame->SetAtomSelection(atom2_id, false);
			lFrame->SetAtomSelection(atom3_id, false);
			lFrame->SetAtomSelection(atom4_id, false);

			// This process is essentially an adaptation of the angle
			// annotation constraint.  The difference is that the circle is
			// centered not necessarily on an atom, but on the shifted atom's
			// projection onto the dihedral set's shared axis.  (A dihedral set
			// of atoms has four atoms, three vectors, and two angles.  The
			// shared axis is the vector between the second and third atoms.)
			// So, we first project the shifted atom onto this vector.
			lFrame->GetAtomPosition(selected, shifted_pos);
			lFrame->GetAtomPosition(atom1_id, atom1_pos);
			lFrame->GetAtomPosition(atom2_id, atom2_pos);
			lFrame->GetAtomPosition(atom3_id, atom3_pos);
			lFrame->GetAtomPosition(atom4_id, atom4_pos);

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

			new_pos.x = x;
			new_pos.y = y;
			new_pos.z = z;

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

			Normalize3D(&vec2);
			float dot = DotProduct3D(&vec2, &vec_new);

			Matrix4D rotate;
			CPoint3D axis;

			CrossProduct3D(&vec2, &vec_new, &axis);
			Normalize3D(&axis);

			RotateAroundAxis(rotate, axis, acos(dot) * 180.0f / kPi);

			CPoint3D pt;
			CPoint3D vec_old;
			CPoint3D rotated_vec;
			for (int i = 0; i < lFrame->GetNumAtoms(); i++) {
				if (lFrame->GetAtomSelection(i)) {
					lFrame->GetAtomPosition(i, pt);
					vec_old = pt - center_pos;
					Rotate3DPt(rotate, vec_old, &rotated_vec);
					lFrame->SetAtomPosition(i, center_pos + rotated_vec);
				}
			}

			break;
		}
	}
}

void MpGLCanvas::eventMouseLeftDoubleClick(wxMouseEvent& event) {

	prev_mouse = curr_mouse;
	curr_mouse = event.GetPosition();

	// If we're in edit mode, we display the periodic table dialog.
	if (MolWin->InEditMode() && selected_type == MMP_NULL &&
	    !BuilderTool->IsPaletteVisible()) {
		MolWin->ToggleBuilderPalette();
	}

}

void MpGLCanvas::eventWindowActivated(wxActivateEvent& event) {
	if (event.GetActive()) {
		mouse_activate_timer.Start();
		ignore_next_up = true;
	}
	event.Skip();
}

void MpGLCanvas::eventMouseEnterWindow(wxMouseEvent& event) {

	curr_mouse = event.GetPosition();
	prev_mouse = curr_mouse;

}

void MpGLCanvas::eventMouseLeftWentDown(wxMouseEvent& event) {

	mpAtom *lAtoms = mMainData->cFrame->Atoms;

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	SetCurrent();
#endif

	// We give the canvas keyboard focus since the builder dialog steals it.
	this->SetFocus();
	
	long time_between = mouse_activate_timer.Time();
	if (time_between < 10) {
		ignore_next_up = true;
		return;
	}
	ignore_next_up = false;

	curr_mouse = event.GetPosition();
	prev_mouse = curr_mouse;

	ndrag_events = 0;

	testPicking(curr_mouse.x, curr_mouse.y);

	// User's lassoing.
	if (MolWin->InSelectionMode()) {
		is_lassoing = true;
		MolWin->LassoStart(curr_mouse.x, height - curr_mouse.y);
		select_stack_top = 5;
	}
	
	// User's editing and clicked on an atom.
	else if (MolWin->InEditMode() && selected_type == MMP_ATOM) {
		GLdouble tmpWinX, tmpWinY;

		findWinCoord(lAtoms[selected].Position.x,
			lAtoms[selected].Position.y,
			lAtoms[selected].Position.z,
			tmpWinX, tmpWinY, atomDepth);

		winDiffX = curr_mouse.x - (int) tmpWinX;
		winDiffY = curr_mouse.y - (int) tmpWinY;

		first_site_clicked = selected_site;
		first_atom_clicked = selected;
	}

	if (MolWin->IsRotating()) {
		was_just_rotating = true;
	} else {
		was_just_rotating = false;
	}

	// Though nothing's rotating on a single mousedown event, we need to
	// register this first mouse click position.
	glfSetCurrentBMFFont(bitmap_fontd);
	MolWin->Rotate(event);

#ifdef __WXMAC__
	CaptureMouse();
#endif

}

void MpGLCanvas::eventMouseRightWentDown(wxMouseEvent& event) {

	bool deSelectAll = true;
	Frame *lFrame = mMainData->cFrame;

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	SetCurrent();
#endif

	ndrag_events = 0;

	curr_mouse = event.GetPosition();
	prev_mouse = curr_mouse;

	testPicking(curr_mouse.x, curr_mouse.y);

	if (selected_type == MMP_ATOM) {

		// If the clicked-on atom wasn't selected, let's select it.  Otherwise
		// another atom may be selected while we're operating on an unselected
		// one, which is odd.
		if (!lFrame->GetAtomSelection(selected)) {
			SelectObj(selected_type, selected, deSelectAll);
			MolWin->SelectionChanged(deSelectAll);
			MolWin->ReleaseLists();
			Draw();
			MolWin->Dirtify();
		}

		if (MolWin->InEditMode()) {
			interactPopupMenu(curr_mouse.x, curr_mouse.y, 1);
		} else {
			measurePopupMenu(curr_mouse.x, curr_mouse.y);
		}
	}

	else if (selected_type == MMP_BOND) {
		if (MolWin->InEditMode()) {
			interactPopupMenu(curr_mouse.x, curr_mouse.y, 0);
			MolWin->SelectionChanged(deSelectAll);
			MolWin->ReleaseLists();
			MolWin->Dirtify();
		} else {
			bondPopupMenu(curr_mouse.x, curr_mouse.y);
		}
	}

	else if (selected_type == MMP_ANNOTATION) {
		annoPopupMenu(curr_mouse.x, curr_mouse.y);
	}

	else {
		// Though nothing's rotating on a single mousedown event, we need to
		// register this first mouse click position.
		glfSetCurrentBMFFont(bitmap_fontd);
		MolWin->Rotate(event);

#ifdef __WXMAC__
		CaptureMouse();
#endif
	}

}

void MpGLCanvas::eventMouseMiddleWentDown(wxMouseEvent& event) {

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	SetCurrent();
#endif

	curr_mouse = event.GetPosition();
	prev_mouse = curr_mouse;

	testPicking(curr_mouse.x, curr_mouse.y);

	// Though nothing's rotating on a single mousedown event, we need to
	// register this first mouse click position.
	glfSetCurrentBMFFont(bitmap_fontd);
	MolWin->Rotate(event);

#ifdef __WXMAC__
	CaptureMouse();
#endif

}

void MpGLCanvas::eventMouseDragging(wxMouseEvent& event) {

	// This function handles all mouse movement, no matter the button being
	// held (or even no button being down).
	
	Frame *lFrame = mMainData->cFrame;

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	SetCurrent();
#endif

	prev_mouse = curr_mouse;
	curr_mouse = event.GetPosition();

	// We only show text if a button isn't held down.
	if (!event.Dragging()) {
		testPicking(curr_mouse.x, curr_mouse.y);
		if (MolWin->InEditMode() && selected_site >= 0) {
			if (BuilderTool->InPeriodicMode()) {
				MolWin->SetStatusText(_("Bond an atom here."));
			}
		} else if (MolWin->InEditMode() && selected < 0 && 
		           BuilderTool->GetSelectedElement()) {
			if (BuilderTool->InPeriodicMode()) {
				MolWin->SetStatusText(_("Add new atom here."));
			} else {
				MolWin->SetStatusText(_("Add new prototype here."));
			}
		} else if (selected_type == MMP_ATOM) {
			wxString info, id;
			Prefs->GetAtomLabel(lFrame->Atoms[selected].GetType() - 1, info);
			id.Printf(wxT(" (%d)"), selected + 1);
			info.Append(id);
			MolWin->SetStatusText(info);
		} else if (selected_type == MMP_BOND) {
			wxString msg;
			msg.Printf(_("Bond (%d)"), selected + 1);
			MolWin->SetStatusText(msg);
		} else {
			MolWin->UpdateFrameText();
		}
		return;
	}

	// We want to allow some mouse drift when the user's trying to simply
	// click on the canvas.  By recording the number of drag events that have
	// occurred, we can classify drags with a limited number of events as
	// clicks.
	ndrag_events++;

	glfSetCurrentBMFFont(bitmap_fontd);

	// Lassoing should only be done with the left mouse button.
	if (is_lassoing && event.LeftIsDown()) {
		HandleLassoing(event, wxPoint(curr_mouse.x, height - curr_mouse.y));
		Draw();
	}

	// User must be wanting to translate or rotate atoms.  This isn't the
	// case if something that's not a bond or atom is clicked on, like a
	// bonding site or an annotation or the background.
	else if (MolWin->InEditMode() &&
	         (selected_type == MMP_BOND || selected_type == MMP_ATOM)) {

		// If a bond site is clicked on, it may be part of drag operation to
		// join two sites.  We aren't editing in such a case, nor do we want
		// to rotate the scene.
		if (selected_site < 0) {

			// We create a snapsot only when the drag has just started and
			// when molecular state will be changed.
			if (ndrag_events == 1) {
				MolWin->CreateFrameSnapShot();
			}

			if (ndrag_events < 3) {
				SelectWholeFragments();
			}

			HandleEditing(event, curr_mouse, prev_mouse);
		}

	}

	else if (selected_type == MMP_1DSURFACE) {
		HandleEditing(event, curr_mouse, prev_mouse);
	}
	
	// Otherwise the user must be transforming the whole scene.
	else {
		MolWin->Rotate(event);
	}

}

/**
 * Selects the remaining unselected atoms belonging to fragments that have at
 * least one atom already selected.  This function should be called before
 * performing any operations on whole fragments, like deletion, movement, and
 * so on.
 */
void MpGLCanvas::SelectWholeFragments() {

	//also check that only whole EFPs are selected
	Frame *lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;
	mpAtom *lAtoms = lFrame->Atoms;
	select_stack_top = 5;

	for (long i=0; i<NumAtoms; i++) {
		if (lAtoms[i].IsEffectiveFragment() && lAtoms[i].GetSelectState()) {
			long fragId = lAtoms[i].GetFragmentNumber();
			for (long j=0; j<NumAtoms; j++) {
				if (lAtoms[j].IsEffectiveFragment() &&
				    lAtoms[j].GetFragmentNumber() == fragId)
					lFrame->SetAtomSelection(j, true);
			}
		}
	}

}

void MpGLCanvas::eventMouseLeftWentUp(wxMouseEvent& event) {

	bool deSelectAll = true;

	Frame *lFrame = mMainData->cFrame;

	curr_mouse = event.GetPosition();
	prev_mouse = curr_mouse;
	was_zooming = false;

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	SetCurrent();
#endif
	glfSetCurrentBMFFont(bitmap_fontd);

	// If either the command or shift keys are held, we want to add to the
	// selection, not replace it.
	if (event.CmdDown() || event.ShiftDown()) {
		deSelectAll = false;
	}

	// If the lasso tool is being used, we want to close off its region.
	if (is_lassoing) {
		is_lassoing = false;
		MolWin->LassoEnd();
		if (ndrag_events <= 0) {
			SelectObj(selected_type, selected, deSelectAll);
		}
		MolWin->Dirtify();
		Draw();
	}

	// Otherwise, if this mouse operation can be considered a stationary click
	// and not a drag, we try to do selection or respond to an action.
	else if (ndrag_events >= 0 && ndrag_events < 3) {

		// If we're in edit mode, the user may be trying to add an atom,
		// possibly to a bonding site.  However, if the mouse click was part
		// of bringing the window to focus, we don't add anything.
		if (MolWin->InEditMode() && !ignore_next_up) {

			testPicking(curr_mouse.x, curr_mouse.y);

			// If no periodic table is shown or an atom is not selected, but
			// the user seems to be trying to add an atom, give them a message.

			if (BuilderTool->GetSelectedElement() == 0) {
				if (selected_site >= 0 || selected < 0) {
					MolWin->SetStatusText(wxT("Select an atom in the periodic table."));
				}
			}

			// If the user is adding a new atom based on the bonding site
			// skeleton, add the atom in the direction of the bonding site.
			else if (selected_site >= 0) {
				MolWin->CreateFrameSnapShot();
				long OrigAtomCount = lFrame->GetNumAtoms();

				if (BuilderTool->InPeriodicMode()) {
					CPoint3D vector, origin;
					MolWin->DrawBondingSites(selected, 0, NULL, selected_site+1, &vector);
					lFrame->GetAtomPosition(selected, origin);
					mMainData->NewAtom(BuilderTool->GetSelectedElement(), origin + vector * 0.01 *
					                   (Prefs->GetAtomSize(lFrame->GetAtomType(selected)-1) +
					                   Prefs->GetAtomSize(BuilderTool->GetSelectedElement() - 1)));
					lFrame->Atoms[lFrame->GetNumAtoms() - 1].SetCoordinationNumber(BuilderTool->GetSelectedCoordination());
					lFrame->Atoms[lFrame->GetNumAtoms() - 1].SetLonePairCount(BuilderTool->GetSelectedLonePairCount());
					lFrame->Atoms[lFrame->GetNumAtoms() - 1].IsSymmetryUnique(MolWin->InSymmetryEditMode());
					lFrame->AddBond(selected,lFrame->GetNumAtoms()-1,kSingleBond);
					MolWin->SetStatusText(wxT("Added new atom."));

					// Let's select the new atom.
					selected = lFrame->NumAtoms - 1;
					deSelectAll = true;
					SelectObj(selected_type, selected, deSelectAll);

					lFrame->SetBonds(Prefs, true, NULL, true);
					MolWin->AtomsChanged(true, false);
					MolWin->ReleaseLists();
					MolWin->AdjustMenus();
					MolWin->Dirtify();
				} else {

					mpAtom new_atom;
					int prev_natoms = lFrame->NumAtoms;
					Structure *structure;

					structure = BuilderTool->GetSelectedStructure();

					if (structure) {
						if (structure->link_atom == -1) {
							MolWin->SetStatusText(wxT("Prototype has no connecting bonding site."));
						} else {
							lFrame->resetAllSelectState();
							MolWin->SetHighliteMode(true);
							
							// Add all atoms and select them so they can be adjusted.
							for (int i = 0; i < structure->natoms; i++) {
								new_atom = mpAtom(structure->atoms[i]);
								mMainData->NewAtom(new_atom);
								lFrame->Atoms[lFrame->GetNumAtoms() - 1].IsSymmetryUnique(MolWin->InSymmetryEditMode());
								lFrame->SetAtomSelection(lFrame->NumAtoms - 1, true);
							}

							Bond *bond;
							bool result;
							for (int i = 0; i < structure->nbonds; i++) {
								bond = &structure->bonds[i];
								result = lFrame->AddBond(bond->Atom1 + prev_natoms,
														 bond->Atom2 + prev_natoms,
														 bond->Order);
							}

							lFrame->DeleteAtom(prev_natoms + structure->atom_to_prune);

							// If the link-to atom was past the just deleted atom to prune, we
							// need to adjust the link-to atom id by -1.
							int offset = 0;
							if (structure->link_atom > structure->atom_to_prune) {
								offset = -1;
							}

							ConnectSelectedToSite(prev_natoms + structure->link_atom + offset,
												  structure->link_site - 1,
												  selected, selected_site);

							lFrame->AddBond(selected,
											prev_natoms + structure->link_atom + offset,
											kSingleBond);

							MolWin->AtomsChanged(true, false);
							MolWin->SelectionChanged(true);
						}
					}
				}
				//Sort fragments to the end of the list
				if (OrigAtomCount > 0 &&
				    !lFrame->Atoms[OrigAtomCount].IsEffectiveFragment() &&
				    lFrame->Atoms[OrigAtomCount-1].IsEffectiveFragment()) {
					long initialFragAtom = 0;
					while (!lFrame->Atoms[initialFragAtom].IsEffectiveFragment() &&
					       initialFragAtom < OrigAtomCount) initialFragAtom++;
					for (long i=OrigAtomCount; i<lFrame->GetNumAtoms(); i++) {
						mMainData->ReorderAtomList(i, initialFragAtom);
						initialFragAtom++;
					}
				}
				
			}

			// If the user clicked on nothing, we try to add an atom given
			// the selected element on the builder palette.
			else if (selected < 0 && !was_just_rotating) {

				CPoint3D newPnt;
				GLdouble newX, newY, newZ;
				int type;

				// Guess atom's depth value by projecting the center of scene
				// to window coordinates.  The mouse coordinates and window
				// depth will be used to place the atom into the scene.
				findWinCoord(0.0, 0.0, 0.0, newX, newY, atomDepth);
				findReal3DCoord(curr_mouse.x, curr_mouse.y, atomDepth,
								newX, newY, newZ);
				newPnt = CPoint3D(newX, newY, newZ);

				MolWin->CreateFrameSnapShot();
				
				long OrigAtomCount = lFrame->GetNumAtoms();

				if (BuilderTool->InPeriodicMode()) {
					type = BuilderTool->GetSelectedElement();
					mMainData->NewAtom(type, newPnt);
					lFrame->Atoms[lFrame->GetNumAtoms() - 1].SetCoordinationNumber(BuilderTool->GetSelectedCoordination());
					lFrame->Atoms[lFrame->GetNumAtoms() - 1].SetLonePairCount(BuilderTool->GetSelectedLonePairCount());
					lFrame->Atoms[lFrame->GetNumAtoms() - 1].IsSymmetryUnique(MolWin->InSymmetryEditMode());

					// Let's select the new atom.
					selected = lFrame->NumAtoms - 1;
					deSelectAll = true;
					selected_type = MMP_ATOM;
					SelectObj(selected_type, selected, deSelectAll);
					MolWin->AtomsChanged(true, false);
					MolWin->SetStatusText(wxT("Added new atom."));
					MolWin->SelectionChanged(true);
				} else if (BuilderTool->InStructuresMode()) {
					Structure *structure; 
					structure = BuilderTool->GetSelectedStructure();

					if (structure) {
						CPoint3D pos = structure->atoms[0].Position;
						CPoint3D offset;
						CPoint3D mouse_pos;
						double mouse_x, mouse_y, mouse_z;
						double atom_depth;
						mpAtom new_atom;

						// The placement of the structure will be at the mouse
						// x,y coordinates in object space, with the z 
						// coordinate set to origin's depth.
						findWinCoord(pos.x, pos.y, pos.z,
									 mouse_x, mouse_y, atom_depth);
						double tmp, center_depth;
						findWinCoord(0.0, 0.0, 0.0, tmp, tmp, center_depth);
						findReal3DCoord(curr_mouse.x, curr_mouse.y, center_depth,
										mouse_x, mouse_y, mouse_z);
						mouse_pos = CPoint3D(mouse_x, mouse_y, mouse_z);
						offset = mouse_pos - pos;

						int prev_natoms = lFrame->NumAtoms;

						lFrame->resetAllSelectState();
						MolWin->SetHighliteMode(true);
						int fragId=-1;

						if (structure->FragName.size() > 0) {
							fragId = mMainData->FragmentNames.size() + 1;
							mMainData->FragmentNames.push_back(structure->FragName);
							if (structure->frag_def.size()) {
								std::map<std::string, EFrag>::iterator frag;
								frag = mMainData->efrags.find(structure->FragName);
								if (frag == mMainData->efrags.end()) {
									mMainData->efrags.insert(std::pair<std::string, EFrag>(structure->FragName, structure->frag_def));
								}
							}
						}

						for (int i = 0; i < structure->natoms; i++) {
							new_atom = mpAtom(structure->atoms[i]);
							if (fragId > 0) new_atom.SetFragmentNumber(fragId);
							mMainData->NewAtom(new_atom);
							lFrame->GetAtomPosition(lFrame->NumAtoms - 1, pos);
							lFrame->SetAtomPosition(lFrame->NumAtoms - 1, pos + offset);
							lFrame->SetAtomSelection(lFrame->NumAtoms - 1, true);
						}

						Bond *bond;
						for (int i = 0; i < structure->nbonds; i++) {
							bond = &structure->bonds[i];
							lFrame->AddBond(bond->Atom1 + prev_natoms,
											bond->Atom2 + prev_natoms,
											bond->Order);
						}

						MolWin->AtomsChanged(true, false);
						MolWin->SelectionChanged(true);
					}

				}
				//Sort fragments to the end of the list
				if (OrigAtomCount > 0 &&
				    !lFrame->Atoms[OrigAtomCount].IsEffectiveFragment() &&
				    lFrame->Atoms[OrigAtomCount-1].IsEffectiveFragment()) {
					long initialFragAtom = 0;
					while (!lFrame->Atoms[initialFragAtom].IsEffectiveFragment() &&
					       initialFragAtom < OrigAtomCount) initialFragAtom++;
					for (long i=OrigAtomCount; i<lFrame->GetNumAtoms(); i++) {
						mMainData->ReorderAtomList(i, initialFragAtom);
						initialFragAtom++;
					}
				}
				lFrame->SetBonds(Prefs, true, NULL, true);
				MolWin->ReleaseLists();
			} else {
				SelectObj(selected_type, selected, deSelectAll);
				MolWin->SelectionChanged(deSelectAll);
			}
		} else {
			// Since we're dealing with a click, it's likely that something was
			// selected.
			SelectObj(selected_type, selected, deSelectAll);
			MolWin->SelectionChanged(deSelectAll);
		}

		Draw();

	}
	
	// If a drag occurred between two bonding sites, we pair them up with
	// a bond.
	else if (MolWin->InEditMode() && first_site_clicked >= 0 &&
	         selected_site >= 0) {

		testPicking(curr_mouse.x, curr_mouse.y);

		// We don't allow two sites of the same atom to bond.
		if (selected != first_atom_clicked) {

			bool okay_to_continue = true;

			// If control/command is down, we translate and rotate all
			// selected atoms so the second site is aligned with the first.
			if (event.ControlDown() || event.CmdDown()) {
				if (lFrame->GetAtomSelection(selected)) {
					MolWin->SetStatusText(_("Destination atom must not be selected."));
					okay_to_continue = false;
				} else {
					ConnectSelectedToSite(first_atom_clicked, first_site_clicked,
										  selected, selected_site);
				}
			}
			
			if (okay_to_continue) {
				int ibond = lFrame->BondExists(first_atom_clicked, selected);
				MolWin->CreateFrameSnapShot();
				if (ibond >= 0) {
					int t = lFrame->GetBondOrder(ibond);
					if (t <= kTripleBond) t++;
					lFrame->SetBondOrder(ibond, (BondOrder) t);
				} else {
					lFrame->AddBond(first_atom_clicked, selected);
				}
				MolWin->BondsChanged();
			}
		}
		first_site_clicked = -1;
		Draw();
	}
	
	else if (did_edit) {
		/* mMainData->RotateToPrincipleOrientation(Prefs); */
		/* mMainData->StickCoordinates(); */
		//update the list of symmetry unique atoms
		/* mMainData->GenerateSymmetryUniqueAtoms(0.1f); */
		/* mMainData->SymmetrizeCoordinates(); */
		/* mMainData->StickCoordinates(); */
		/* MolWin->AtomsChanged(true, true); */
	}
	
	// Otherwise, the drag must have been to transform the whole scene.  We
	// need to call rotate one more time to get rid of the rotation circle
	// that appears.
	else {
		MolWin->Rotate(event);
	}

	// The drag image also captures mouse but doesn't check for that it
	// still has it captured before releasing it.  So, our check and release
	// must occur after ending the drag.
	if (HasCapture()) {
		ReleaseMouse();
	}
	
	ignore_next_up = false;
	leftDown = false;
	did_edit = false;
}

void MpGLCanvas::eventMouseRightWentUp(wxMouseEvent& event) {

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	SetCurrent();
#endif

	prev_mouse = curr_mouse;
	curr_mouse = event.GetPosition();

	if (HasCapture()) {
		ReleaseMouse();
	}

	if (ndrag_events) {
		// We call this to erase the transformation circle that would otherwise
		// stay when the user releases the mouse.
		glfSetCurrentBMFFont(bitmap_fontd);
		MolWin->Rotate(event);
	} else if (MolWin->InEditMode()) {
		ShowBlankPopup(curr_mouse.x, curr_mouse.y);
	}

}

void MpGLCanvas::eventMouseMiddleWentUp(wxMouseEvent& event) {

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	SetCurrent();
#endif
	glfSetCurrentBMFFont(bitmap_fontd);

	if (HasCapture()) {
		ReleaseMouse();
	}

	// We call this to erase the transformation circle that would otherwise
	// stay when the user releases the mouse.
	MolWin->Rotate(event);

}

void MpGLCanvas::eventMouseWheel(wxMouseEvent& event) {

#if wxCHECK_VERSION(2,9,0)
	SetCurrent(*context);
#else
	SetCurrent();
#endif
	glfSetCurrentBMFFont(bitmap_fontd);

	mMainData->WindowSize *= (event.GetWheelRotation() * 0.01f) /
							 event.GetWheelDelta() + 1.0f;		
	// The display window will calculate the appropriate action for mouse 
	// wheel movement.
	MolWin->Rotate(event);

}

void MpGLCanvas::HandleEditing(wxMouseEvent& event, const wxPoint& curr_pt,
		                       const wxPoint& prev_pt) {

	Frame *lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;
	mpAtom *lAtoms = lFrame->Atoms;
	
	/* if (selected_type != MMP_ATOM && selected_type != MMP_BOND) { */
		/* mSelectState = -1; */
		/* return; */
	/* } */

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
			wxPoint prev, curr;
			wxPoint cent;
			prev.x = prev_pt.x;
			prev.y = prev_pt.y;
			curr.x = curr_pt.x;
			curr.y = curr_pt.y;

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

			// The centroid's projection is the center of the virtual sphere.
			cent.x = (unsigned short) proj_pt[0];
			cent.y = height - (unsigned short) proj_pt[1];

			// The distance between the two projected points is the radius.
			proj_pt[0] = proj_pt[0] - proj_max[0];
			proj_pt[1] = proj_pt[1] - proj_max[1];
			radius = (int) sqrt(proj_pt[0] * proj_pt[0] +
			                    proj_pt[1] * proj_pt[1]);

			// Now we assemble the rotation matrix and rotate all the selected
			// atoms.
			Matrix4D rot_matrix;

			VirtualSphereQD3D(prev, curr, cent, radius, rot_matrix,
							  mMainData->TotalRotation);

			CPoint3D new_pt;
			for (atom = atoms.begin(); atom != atoms.end(); atom++) {
				(*atom)->Position -= centroid;
				Rotate3DPt(rot_matrix, (*atom)->Position, &new_pt);
				(*atom)->Position = new_pt;
				(*atom)->Position += centroid;
			}

		}
		
		// If Control/Command is not down, then the user is translating either
		// in the screen plane, or in depth.
		else {
			GLdouble tmpX, tmpY, tmpZ;
			findWinCoord(lAtoms[selected].Position.x,
				lAtoms[selected].Position.y,
				lAtoms[selected].Position.z, tmpX, tmpY, tmpZ);

			// If shift is held when an atom is clicked on, we want to
			// change the depths of either the clicked on or selected
			// atoms.  Also do this for the middle mouse button.
			if (event.ShiftDown() || event.MiddleIsDown()) {
				float depth_offset;
				float dy = curr_pt.y - prev_pt.y;

				// depth_offset = dy / (10.0f * GetClientSize().GetHeight()); 
				depth_offset = dy / (10.0f * height);
				findReal3DCoord(tmpX, tmpY, tmpZ - depth_offset,
								newX, newY, newZ);
				was_zooming = true;
			}

			// If no shift, just translate.
			else {
				if (was_zooming == true) {
					GLdouble tmpWinX, tmpWinY;

					findWinCoord(lAtoms[selected].Position.x,
								 lAtoms[selected].Position.y,
								 lAtoms[selected].Position.z,
								 tmpWinX, tmpWinY, atomDepth);
  
					winDiffX = curr_mouse.x - (int) tmpWinX;
					winDiffY = curr_mouse.y - (int) tmpWinY;
					was_zooming = false;
				}

				findReal3DCoord(curr_pt.x - winDiffX, curr_pt.y - winDiffY,
								tmpZ, newX, newY, newZ);
			}

			if (!lFrame->GetAtomSelection(selected)) {
				SelectObj(selected_type, selected, true);
				MolWin->SelectionChanged(true);
				return;
			}

			// Screen plane translation can be restricted by an annotation
			// constraint.
			int constrain_anno_id = mMainData->GetConstrainAnnotation();
			if (!event.ShiftDown() &&
			    !event.MiddleIsDown() &&
			    constrain_anno_id != -1 &&
			    mMainData->Annotations[constrain_anno_id]->containsAtom(selected)) {
				ConstrainPosition(constrain_anno_id, newX, newY, newZ);
			}

			// Or it can follow the mouse freely.
			else {
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

		}
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
		angle_offset = dy / height * 540.0f;

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
	}

	// If the user clicked on a 1-D surface, he can move the end points.
	else if (selected_type == MMP_1DSURFACE) {

		GLdouble newX, newY, newZ;
		CPoint3D *end_point;
		CPoint3D *other_end_point;

		Surf1DBase *surf1D = dynamic_cast<Surf1DBase *>(mMainData->cFrame->GetSurfaceWithID(selected));

		if (!surf1D || selected_site == -1) {
			return;
		}

		if (selected_site == 0) {
			end_point = &surf1D->Start;
			other_end_point = &surf1D->End;
		} else {
			end_point = &surf1D->End;
			other_end_point = &surf1D->Start;
		}

		GLdouble tmpX, tmpY, tmpZ;
		findWinCoord(end_point->x, end_point->y, end_point->z,
					 tmpX, tmpY, tmpZ);

		// If shift is held when an atom is clicked on, we want to
		// change the depths of either the clicked on or selected
		// atoms.  Also do this for the middle mouse button.
		if (event.ShiftDown() || event.MiddleIsDown() || was_zooming) {
			float depth_offset;
			float dy = curr_pt.y - prev_pt.y;
			depth_offset = dy / (10.0f * height);
			findReal3DCoord(tmpX, tmpY, tmpZ - depth_offset, newX, newY, newZ);
			was_zooming = true;
		}

		// If no shift, just translate.
		else {
			findReal3DCoord(curr_pt.x, curr_pt.y, tmpZ, newX, newY, newZ);
		}

		CPoint3D new_coord((float) newX, (float) newY, (float) newZ);
		CPoint3D offset;

		offset = new_coord - *end_point;
		*end_point += offset;
		if (event.ControlDown() || selected_site == -1) {
			*other_end_point += offset;
		}
		surf1D->FreeGrid();

		MolWin->AtomsChanged(false, false);
		
		Draw();
		return;
	}

	// Other
	else {
		return;
	}

	lFrame->SetTargetAtom(selected);
	lFrame->SetBonds(Prefs, true, NULL, true);
	did_edit = true;
	MolWin->AtomsChanged(true, true);
	selected = lFrame->GetTargetAtom();

	if (MolWin->InSymmetryEditMode()) {
		select_stack_top = 5;
	}
}
			
void MpGLCanvas::HandleLassoing(wxMouseEvent& event, const wxPoint& curr_pt) {

	GLdouble mv[16];
	GLdouble proj[16];
	GLint viewport[4];
	GLdouble win_x, win_y, win_z;
	Frame *lFrame = mMainData->cFrame;
	mpAtom *lAtoms = lFrame->Atoms;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);

	if (!MolWin->LassoHasArea()) {
		MolWin->LassoStart(curr_pt.x, curr_pt.y);
	}

	MolWin->LassoGrown(curr_pt.x, curr_pt.y);

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
			MolWin->AtomsChanged(true, false);
		}
	}

	if (nselected >= 4) {
		select_stack_top = 5;
	}

	// for each atom
	//    project into window coordinates
	//    if contained in lasso area, select it
}

//Keyboard events are normally only passed to the focused "window". Thus we need to grab them and
//explicitely pass them up to our parent
void MpGLCanvas::KeyHandler(wxKeyEvent & event) {
	MolWin->KeyHandler(event);
}
void MpGLCanvas::KeyDownHandler(wxKeyEvent & event) {
	MolWin->KeyHandler(event);
}
void MpGLCanvas::KeyUpHandler(wxKeyEvent & event) {
	MolWin->KeyUpHandler(event);
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

	glSelectBuffer(SELECT_BUFFER_SIZE, buff);
	glGetIntegerv(GL_VIEWPORT, view);

	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix(x, view[3]-y, 1.0, 1.0, view);
	SetProjection(((float) width) / height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	MolWin->DrawGL();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
 
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
		if (buff[j + 1] < min_depth && buff[j + 3] != MMP_NULL && buff[j + 4] != 0) {
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

#if 0
	std::cout << "selected_type: " << selected_type << std::endl;
	std::cout << "selected: " << selected << std::endl;
	std::cout << "selected_site: " << selected_site << std::endl;
#endif

}

void MpGLCanvas::SelectObj(int selected_type, int select_id, bool unselect_all) {

	Frame *lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;
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
	// with the smallest eigenvalue.  The other two vectors are more principal,
	// lying in the plane.
	CPoint3D normal = CPoint3D(out.data[2], out.data[5], out.data[8]);
	for (atom = atoms.begin(); atom != atoms.end(); atom++) {
		ProjectToPlane(normal, centroid, (*atom)->Position);
	}

	MolWin->UpdateModelDisplay();
	MolWin->Dirtify();
	
}

void MpGLCanvas::interactPopupMenu(int x, int y, bool isAtom) {

	wxMenu menu;
	wxMenuItem *item;
	wxMenu *submenu = NULL;
	Frame *lFrame = mMainData->cFrame;
	int bond_order;
	wxString length_label;

	if (isAtom) {
		insertAnnotationMenuItems(menu);

		if (!lFrame->Atoms[selected].IsEffectiveFragment()) {
			// If the periodic table is shown and an atom is selected, offer an
			// option to change the clicked-on atom to the selected type.
			if (BuilderTool->GetSelectedElement() != 0 &&
			    BuilderTool->GetSelectedElement() != lFrame->Atoms[selected].GetType()) {

				wxString label;
				wxString atom_name;
				Prefs->GetAtomLabel(lFrame->Atoms[selected].GetType() - 1,
					atom_name);
				label = wxT("Change ") + atom_name + wxT(" to ");
				Prefs->GetAtomLabel(BuilderTool->GetSelectedElement() - 1, atom_name);
				label.Append(atom_name);
				menu.Append(GL_Popup_Change_Atom, label);
			}

			submenu = new wxMenu;
			short cNum = lFrame->Atoms[selected].GetCoordinationNumber();
			item = submenu->AppendRadioItem(GL_Popup_To_Coordination_Zero, wxT("0"));
			if (cNum == 0) item->Check(true);
			item = submenu->AppendRadioItem(GL_Popup_To_Coordination_One, wxT("1"));
			if (cNum == 1) item->Check(true);
			item = submenu->AppendRadioItem(GL_Popup_To_Coordination_Two, wxT("2"));
			if (cNum == 2) item->Check(true);
			item = submenu->AppendRadioItem(GL_Popup_To_Coordination_Three, wxT("3"));
			if (cNum == 3) item->Check(true);
			item = submenu->AppendRadioItem(GL_Popup_To_Coordination_Four, wxT("4"));
			if (cNum == 4) item->Check(true);
			item = submenu->AppendRadioItem(GL_Popup_To_Coordination_Five, wxT("5"));
			if (cNum == 5) item->Check(true);
			item = submenu->AppendRadioItem(GL_Popup_To_Coordination_Six, wxT("6"));
			if (cNum == 6) item->Check(true);
			menu.Append(GL_Popup_Change_Coord_Num, _("Change coordination #"), submenu);

			short LPNum = lFrame->Atoms[selected].GetLonePairCount();
			submenu = new wxMenu;
			item = submenu->AppendRadioItem(GL_Popup_To_LPCount_Zero, wxT("0"));
			if (LPNum == 0) item->Check(true);
			item = submenu->AppendRadioItem(GL_Popup_To_LPCount_One, wxT("1"));
			if (LPNum == 1) item->Check(true);
			item = submenu->AppendRadioItem(GL_Popup_To_LPCount_Two, wxT("2"));
			if (LPNum == 2) item->Check(true);
			item = submenu->AppendRadioItem(GL_Popup_To_LPCount_Three, wxT("3"));
			if (LPNum == 3) item->Check(true);
			item = submenu->AppendRadioItem(GL_Popup_To_LPCount_Four, wxT("4"));
			if (LPNum == 4) item->Check(true);
			item = submenu->AppendRadioItem(GL_Popup_To_LPCount_Five, wxT("5"));
			if (LPNum == 5) item->Check(true);
			menu.Append(GL_Popup_Change_LPCount, _("Change lone pair count"), submenu);

			// A plane is defined by exactly 3 atoms.  With any more than that, we 
			// can have noncoplanar atoms.  Only then do we allow the user to fit
			// the atoms to a plane.
			if (lFrame->GetNumAtomsSelected() >= 4) {
				menu.Append(GL_Popup_Fit_To_Plane, wxT("Fit atoms to plane"));
			}
		} else {
			//The selected atom is part of an effective fragment and thus should not be 
			//modified directly.
			menu.Append(GL_Popup_Change_EFP_To_AllElec, wxT("Convert EFP to ab initio atoms"));
		}

		if (select_stack_top == 3) {
			item = menu.Append(GL_Popup_Add_Plane_Normal, wxT("Add plane normal"));
		}

		menu.AppendSeparator();

	}

	// If a bond is clicked on, we show some bond specific items, like
	// the length of the bond and the order.
	else  {
		submenu = new wxMenu;
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
		item = submenu->AppendRadioItem(GL_Popup_To_Aromatic_Bond, wxT("Aromatic"));
		if (bond_order == kAromaticBond) {
			item->Check(true);
		}
		menu.Append(wxID_ANY, wxT("Bond Order"), submenu);
		menu.AppendSeparator();
	}
   
	if (isAtom) {
		menu.Append(GL_Popup_Menu_Apply_All, _T("&Apply to All Frames"));
	}

	menu.Append(GL_Popup_Delete_Item_Current_Frame, _T("&Delete"));
	menu.Append(GL_Popup_Delete_Item_All_Frames, _T("&Delete in All Frames"));

	PopupMenu(&menu, x, y);

}

/**
 * This function changes the atom that was clicked on to be an atom of the
 * element type selected in the periodic table dialog.
 * @param event The contextual menu event triggering the change.
 */
void MpGLCanvas::ChangeAtom(wxCommandEvent& event) {

	MolWin->CreateFrameSnapShot();
	mMainData->cFrame->SetAtomType(selected, BuilderTool->GetSelectedElement());
	MolWin->AtomsChanged(true,false);

}

/**
 * This function changes the coordination number for the atom that was clicked
 * on.
 * @param event The contextual menu event triggering the change.
 */
void MpGLCanvas::ChangeCoordinationNumber(wxCommandEvent& event) {
	
	Frame *lFrame = mMainData->cFrame;
	lFrame->Atoms[selected].SetCoordinationNumber(event.GetId() - GL_Popup_To_Coordination_Zero);
	MolWin->AtomsChanged(true, true);
	
}

/**
 * This function changes the lone pair count for the atom that was clicked on.
 * @param event The contextual menu event triggering the change.
 */
void MpGLCanvas::ChangeLPCount(wxCommandEvent& event) {
	
	Frame *lFrame = mMainData->cFrame;
	lFrame->Atoms[selected].SetLonePairCount(event.GetId() - GL_Popup_To_LPCount_Zero);
	MolWin->AtomsChanged(true, true);
	
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
		
		if (lFrame->Atoms[selected].IsEffectiveFragment()) {
			wxString flabel(mMainData->FragmentNames[lFrame->Atoms[selected].GetFragmentNumber()-1].c_str(), wxConvUTF8);
			aLabel.Printf(wxT("EFP (%d) "), lFrame->Atoms[selected].GetFragmentNumber(), (selected+1));
			aLabel.Append(flabel);
			item = menu.Append(wxID_ANY, aLabel);
			item->Enable(false);
		}

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
		// if (selected_type == MMP_ATOM && 
			// select_stack_top >= 2 && select_stack_top <= 4) { 
			// menu.AppendSeparator(); 
		// } 

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
				item->Enable(!already_exists);
				
				menu.AppendSeparator();
				wxMenu * submenu = new wxMenu();
				int bond_id = lFrame->BondExists(select_stack[0],
												select_stack[1]);

				if (bond_id < 0) {
					submenu->Append(GL_Popup_To_Hydrogen_Bond, wxT("Hydrogen"));
					submenu->Append(GL_Popup_To_Single_Bond, wxT("Single"));
					submenu->Append(GL_Popup_To_Double_Bond, wxT("Double"));
					submenu->Append(GL_Popup_To_Triple_Bond, wxT("Triple"));
					submenu->Append(GL_Popup_To_Aromatic_Bond, wxT("Aromatic"));
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
					item = submenu->AppendRadioItem(GL_Popup_To_Aromatic_Bond,
													wxT("Aromatic"));
					if (bond_order == kAromaticBond) {
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
				item->Enable(!already_exists);

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
				item->Enable(!already_exists);

				break;
			default:
				break;
		}
	}

	item = menu.Append(GL_Popup_Save_Prototype, wxT("Prototype selection"));
	/* item->Enable(lFrame->GetNumAtomsSelected() > 0); */

}

void MpGLCanvas::measurePopupMenu(int x, int y) {

	wxMenu menu;

	insertAnnotationMenuItems(menu);
	PopupMenu(&menu, x, y);

}

void MpGLCanvas::annoPopupMenu(int x, int y) {

	wxMenu menu;
	wxMenuItem *item;
	mpAtom *lAtoms = mMainData->cFrame->Atoms;

	if (MolWin->InEditMode() &&
	    mMainData->Annotations[selected]->getType() != MP_ANNOTATION_MARKER &&
	    !mMainData->Annotations[selected]->containsFragment(lAtoms)) {
		item = menu.AppendCheckItem(GL_Popup_Lock_To_Annotation,
				                    wxT("Constrain Annotation"));
		if (selected == mMainData->GetConstrainAnnotation()) {
			item->Check(true);
		}

		switch (mMainData->Annotations[selected]->getType()) {
			case MP_ANNOTATION_LENGTH:
				menu.Append(GL_Popup_Set_Anno_Param, wxT("Set Length"));
				break;
			case MP_ANNOTATION_ANGLE:
				menu.Append(GL_Popup_Set_Anno_Param, wxT("Set Angle"));
				break;
			case MP_ANNOTATION_DIHEDRAL:
				menu.Append(GL_Popup_Set_Anno_Param, wxT("Set Angle"));
				break;
		}
	}
	menu.Append(GL_Popup_Delete_Length, wxT("Delete Annotation"));
	PopupMenu(&menu, x, y);

}

void MpGLCanvas::SetAnnotationParameter(wxCommandEvent& event) {

	wxTextEntryDialog *dlg;
	double new_value;
	Frame *lFrame = mMainData->cFrame;
	wxString default_value;
	wxString prompt;
	Annotation *anno = mMainData->Annotations[selected];

	// Markers have no parameter.
	if (anno->getType() == MP_ANNOTATION_MARKER) {
		return;
	}

	// Set an appropriate prompt.
	switch (anno->getType()) {
		case MP_ANNOTATION_LENGTH:
			prompt.Printf(wxT("Enter length:"));
			break;
		case MP_ANNOTATION_ANGLE:
		case MP_ANNOTATION_DIHEDRAL:
			prompt.Printf(wxT("Enter angle:"));
			break;
	}

	default_value.Printf(wxT("%f"), anno->getParam(*lFrame));
	dlg = new wxTextEntryDialog(this, prompt, wxT("Edit Annotation"),
								default_value);

	// Only if a valid number was entered and OK was clicked do we change the
	// annotation parameter.
	if (dlg->ShowModal() == wxID_OK && dlg->GetValue().ToDouble(&new_value)) {
		MolWin->CreateFrameSnapShot();
		anno->setParam(*lFrame, new_value);
	}

	delete dlg;

	MolWin->UpdateModelDisplay();

}

void MpGLCanvas::ConstrainToAnnotation(wxCommandEvent& event) {

	if (mMainData->GetConstrainAnnotation() != selected) {
		mMainData->ConstrainToAnnotation(selected);
	} else {
		mMainData->RemoveAnnotationConstraint();
	}

}

void MpGLCanvas::DeleteAnnotation(wxCommandEvent& event) {

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

	MolWin->UpdateModelDisplay();
	MolWin->Dirtify();
}

void MpGLCanvas::SavePrototype(wxCommandEvent& event) {

	SelectWholeFragments();
	MolWin->menuBuilderSaveStructure(event);

}

void MpGLCanvas::PasteAtMouse(wxCommandEvent& event) {

	// This function will paste all atoms selected in the clipboard into the
	// scene relative to the mouse location.  This is in contrast to the
	// normal paste operation, which retains atom locations exactly.  Here,
	// the first atom in the selected list is placed at the mouse position and
	// its fellow atoms are placed relatively.

	Frame *lFrame = mMainData->cFrame;
	int new_atom_idx = lFrame->NumAtoms;
	CPoint3D pos;
	CPoint3D offset;
	CPoint3D mouse_pos;
	double mouse_x, mouse_y, mouse_z;
	double atom_depth;

	MolWin->CreateFrameSnapShot();

	// First we call the normal paste which exactly replicates the selected
	// atoms, position and all.
	MolWin->menuEditPaste(event);

	// Make sure some atoms were added.
	if (new_atom_idx >= lFrame->NumAtoms) {
		return;
	}

	// Now, we want to find the difference between the first atom in the
	// replicated list and the mouse coordinates in model space at the same
	// depth as the first atom.  This will give us an offset vector that we
	// displace all atoms by so we can paste the set relative to the mouse.
	// The first atom will, of course, get placed at the mouse position.
	lFrame->GetAtomPosition(new_atom_idx, pos);
	findWinCoord(pos.x, pos.y, pos.z, mouse_x, mouse_y, atom_depth);
	findReal3DCoord(curr_mouse.x, curr_mouse.y, atom_depth,
					mouse_x, mouse_y, mouse_z);
	mouse_pos = CPoint3D(mouse_x, mouse_y, mouse_z);
	offset = mouse_pos - pos;

	// Now we offset all the new atoms.
	for (int i = new_atom_idx; i < lFrame->NumAtoms; i++) {
		lFrame->GetAtomPosition(i, pos);
		lFrame->SetAtomPosition(i, pos + offset);
	}
	
	lFrame->SetBonds(Prefs, true, NULL, true);
	MolWin->BondsChanged();

}

void MpGLCanvas::ShowBlankPopup(int x, int y) {

	// This function shows a popup menu that contains items pertaining to
	// clicking on the blank part of the GL canvas.

	wxMenu menu;
	
	menu.Append(GL_Popup_Paste_At, _("Paste at Mouse Position"));
	PopupMenu(&menu, x, y);

}

void MpGLCanvas::bondPopupMenu(int x, int y) {

	// This function shows a popup menu that shows some information and
	// operations for the clicked-on bond.  It's assumed that selected is
	// a valid index in the bonds lists.

	wxMenu menu;
	wxMenuItem *item;
	wxMenu *submenu;
	Frame *lFrame = mMainData->cFrame;
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
	item = submenu->AppendRadioItem(GL_Popup_To_Aromatic_Bond, wxT("Aromatic"));
	if (bond_order == kAromaticBond) {
		item->Check(true);
	}
	menu.Append(wxID_ANY, wxT("Bond Order"), submenu);
	
	menu.AppendSeparator();
	menu.Append(GL_Popup_Delete_Bond, wxT("Delete Bond"));

	PopupMenu(&menu, x, y);

}

void MpGLCanvas::ChangeBonding(wxCommandEvent& event) {
	
	MolWin->CreateFrameSnapShot();
	// This is a multiple use function. Its action will depend on the menu id
	// that generates the call. It will set the bond type of the selected bond
	// to the desired order, creating a new bond if one didn't exist.
	
	Bond *bond;
	Frame *lFrame = mMainData->cFrame;
	BondOrder order = (BondOrder) ((event.GetId() - GL_Popup_To_Hydrogen_Bond) + kHydrogenBond);
	
	if (selected_type == MMP_BOND) {	//existing bond, change order
		bond = &(lFrame->Bonds[selected]);
		bond->Order = order;
		lFrame->resetAllSelectState();
		bond->SetSelectState(true);
		lFrame->SetAtomSelection(bond->Atom1, true);
		lFrame->SetAtomSelection(bond->Atom2, true);
		//If one of the atoms has no additional bonds adjust its position for the new bond order
		float scale = 0.01;
		if (order == kDoubleBond) scale = 0.0087;
		if (order == kTripleBond) scale = 0.0079;
		if (order == kAromaticBond) scale = 0.0093;
		if (lFrame->GetAtomNumBonds(bond->Atom2) == 1) {
			CPoint3D offset = lFrame->Atoms[bond->Atom2].Position - lFrame->Atoms[bond->Atom1].Position;
			Normalize3D(&offset);
			lFrame->Atoms[bond->Atom2].Position =
			   lFrame->Atoms[bond->Atom1].Position + offset * scale *
               (Prefs->GetAtomSize(lFrame->Atoms[bond->Atom1].Type-1) + Prefs->GetAtomSize(lFrame->Atoms[bond->Atom2].Type-1));
		} else if (lFrame->GetAtomNumBonds(bond->Atom1) == 1) {
			CPoint3D offset = lFrame->Atoms[bond->Atom1].Position - lFrame->Atoms[bond->Atom2].Position;
			Normalize3D(&offset);
			lFrame->Atoms[bond->Atom1].Position =
			   lFrame->Atoms[bond->Atom2].Position + offset * scale *
			   (Prefs->GetAtomSize(lFrame->Atoms[bond->Atom1].Type-1) + Prefs->GetAtomSize(lFrame->Atoms[bond->Atom2].Type-1));
		}
		MolWin->BondsChanged();
	} else if (select_stack_top == 2) { //new bond
		int bond_id = lFrame->BondExists(select_stack[0], select_stack[1]);
		if (bond_id >= 0) {
			bond = &(lFrame->Bonds[bond_id]);
			bond->Order = order;
		} else {
			lFrame->AddBond(select_stack[0], select_stack[1], order);
		}
		lFrame->resetAllSelectState();
		lFrame->Bonds[lFrame->GetNumBonds() - 1].SetSelectState(true);
		lFrame->SetAtomSelection(select_stack[0], true);
		lFrame->SetAtomSelection(select_stack[1], true);
		MolWin->BondsChanged();
	}
}

void MpGLCanvas::DeleteBond(wxCommandEvent& event) {
	
	MolWin->CreateFrameSnapShot();
	// Delete the selected bond
	
	Frame *lFrame = mMainData->cFrame;
	
	if (selected_type == MMP_BOND) {
		lFrame->DeleteBond(selected);
		MolWin->BondsChanged();
	}

}

void MpGLCanvas::AddPlaneNormal(wxCommandEvent& event) {

	Frame *lFrame = mMainData->cFrame;
	CPoint3D pos1;
	CPoint3D pos2;
	CPoint3D pos3;
	CPoint3D vec1;
	CPoint3D vec2;
	CPoint3D normal;

	lFrame->GetAtomPosition(select_stack[0], pos1);
	lFrame->GetAtomPosition(select_stack[1], pos2);
	lFrame->GetAtomPosition(select_stack[2], pos3);

	vec1 = pos2 - pos1;
	Normalize3D(&vec1);

	vec2 = pos3 - pos1;
	Normalize3D(&vec2);

	UnitCrossProduct3D(&vec1, &vec2, &normal);

	mMainData->NewAtom(1, pos3 + normal);
	lFrame->Atoms[lFrame->GetNumAtoms() - 1].IsSymmetryUnique(MolWin->InSymmetryEditMode());
	lFrame->AddBond(select_stack[2], lFrame->NumAtoms - 1);
	MolWin->AtomsChanged();

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

	MolWin->UpdateModelDisplay();
	MolWin->Dirtify();

}

void MpGLCanvas::On_Apply_All(wxCommandEvent& event) {
	Frame *  lFrame = mMainData->cFrame;

	Frame * cFrame = mMainData->Frames;

	for (int i = 0; i < mMainData->NumFrames; i++) {
		if (mMainData->CurrentFrame-1 != i) {
			if (selected >= cFrame->NumAtoms) {
				mMainData->NewAtom(lFrame->Atoms[selected].Type, lFrame->Atoms[selected].Position);
				lFrame->Atoms[lFrame->GetNumAtoms() - 1].IsSymmetryUnique(MolWin->InSymmetryEditMode());
			} else
				cFrame->Atoms[selected] = lFrame->Atoms[selected];
		}

		cFrame = cFrame->NextFrame;
	}

	MolWin->UpdateModelDisplay();
	MolWin->Dirtify();
}

void MpGLCanvas::On_Delete_Single_Frame(wxCommandEvent& event) {
	MolWin->CreateFrameSnapShot();
	Frame * lFrame = mMainData->cFrame;

	if (selected_type == MMP_ATOM) {

		// If the clicked-on atom is selected, delete all selected atoms.
		if (lFrame->GetAtomSelection(selected)) {
			for (int i = 0; i < lFrame->NumAtoms; i++) {
				if (lFrame->GetAtomSelection(i)) {
					 i = mMainData->DeleteAtom(i);
					// Deleting an atom will shift its successor to current
					// position, so we must adjust i.
					i--;
				}
			}
		}
		
		// If it's not selected, delete only it.
		else {
			mMainData->DeleteAtom(selected);
		}

	} else if (selected_type == MMP_BOND) {
		lFrame->DeleteBond(selected);
	}

	MolWin->SetHighliteMode(false);
	select_stack_top = 0;
	MolWin->UpdateModelDisplay();
	MolWin->AtomsChanged(true, false);
	MolWin->AdjustMenus();
}

void MpGLCanvas::On_Delete_All_Frames(wxCommandEvent& event) {

	MolWin->CreateFrameSnapShot();
	Frame * lFrame = mMainData->Frames;

	if (selected_type == MMP_ATOM)
		mMainData->DeleteAtom(selected, true);

	else if (selected_type == MMP_BOND) {
		while (lFrame) {
			if (selected < lFrame->NumBonds)
				lFrame->DeleteBond(selected);
			lFrame = lFrame->NextFrame;
		}
	}

	MolWin->UpdateModelDisplay();
	MolWin->AtomsChanged(true, false);
	MolWin->AdjustMenus();
}

/**
 * This function translates and rotates the selected atoms so that the atom
 * index by dst_atom is bonded to the atom indexed by src_atom.  The exact
 * rotation and direction of translation is determined by the two bonding site
 * vectors, which are made to point toward each other.
 * @param src_atom Index of atom to transform toward.
 * @param src_site Index of bonding site on src_atom to bond selection to.
 * @param dst_atom Index of atom within selected set to bond to src_atom.
 * @param dst_site Index of bonding site on dst_atom to bond selection to.
 */
void MpGLCanvas::ConnectSelectedToSite(int src_atom, int src_site,
									   int dst_atom, int dst_site) {
	CPoint3D atom_pos;
	CPoint3D new_atom_pos;
	CPoint3D offset;
	CPoint3D origin;
	CPoint3D dst_vec;
	CPoint3D src_vec;
	Matrix4D rotmat;
	Frame *lFrame = mMainData->cFrame;

	lFrame->GetAtomPosition(src_atom, atom_pos);
	lFrame->GetAtomPosition(dst_atom, origin);

	// We want to rotate the selected set so that the dragged-to
	// bonding site will meet up with the dragged-from bonding
	// site.  So, we need a rotation matrix that will make these
	// sites point to each other.  To compute that with 
	// SetRotationMatrix, which makes vectors point in the same
	// direction, we have to invert one of them.
	MolWin->DrawBondingSites(src_atom, 0, NULL, src_site + 1, &src_vec);
	MolWin->DrawBondingSites(dst_atom, 0, NULL, dst_site + 1, &dst_vec);
	src_vec *= -1.0f;
	SetRotationMatrix(rotmat, &src_vec, &dst_vec);

	// We translate each selected atom by the distance between
	// the bond offset and the dragged-to atom.
	offset = origin - atom_pos +
	   dst_vec * 0.01 *
	   (Prefs->GetAtomSize(lFrame->GetAtomType(src_atom) - 1) +
	    Prefs->GetAtomSize(lFrame->GetAtomType(dst_atom) - 1));

	// We rotate the selected the atoms using the dragged-to atom
	// as the origin.  The rotation is done first, and then the
	// translation.
	origin = atom_pos;
	MolWin->CreateFrameSnapShot();
	for (int i = 0; i < lFrame->NumAtoms; i++) {
		if (lFrame->GetAtomSelection(i)) {
			lFrame->GetAtomPosition(i, atom_pos);
			atom_pos = atom_pos - origin;
			Rotate3DPt(rotmat, atom_pos, &new_atom_pos);
			new_atom_pos += origin;
			lFrame->SetAtomPosition(i, new_atom_pos + offset);
		}
	}
}

void MpGLCanvas::ConvertEFPToAllElec(wxCommandEvent& event) {
	Frame *lFrame = mMainData->cFrame;
	//Convert all atoms in the selected effective fragment to all electron atoms.
	if (lFrame->Atoms[selected].IsEffectiveFragment()) {
		MolWin->CreateFrameSnapShot();
		int fragId = lFrame->Atoms[selected].GetFragmentNumber();
		int firstatm=-1;
		for (int iatom=0; iatom<lFrame->GetNumAtoms(); iatom++) {
			if (lFrame->Atoms[iatom].IsEffectiveFragment()) {
				if (lFrame->Atoms[iatom].GetFragmentNumber() == fragId) {
					if (firstatm < 0) firstatm = iatom;
					lFrame->Atoms[iatom].IsEffectiveFragment(false);
				} else if (lFrame->Atoms[iatom].GetFragmentNumber() > fragId)
					lFrame->Atoms[iatom].SetFragmentNumber(lFrame->Atoms[iatom].GetFragmentNumber()-1);
			}
		}
		//indexing is now up to date, pop the fragname out of the list
		std::vector<std::string>::iterator iter = mMainData->FragmentNames.begin();
		for (int i=1; i<fragId; i++) ++iter;
		mMainData->FragmentNames.erase(iter);
		//data is update, but need to make sure this does leave mixed all-electron and fragment blocks
		if (firstatm > 0 && lFrame->Atoms[firstatm-1].IsEffectiveFragment()) {
			int firstfrag = -1;
			for (int i=0; i<lFrame->GetNumAtoms(); i++)
				if (lFrame->Atoms[i].IsEffectiveFragment()) {
					firstfrag = i;
					break;
				}
			for (int i=firstfrag; i<lFrame->GetNumAtoms(); i++) {
				if (!lFrame->Atoms[i].IsEffectiveFragment()) {
					mMainData->ReorderAtomList(i, firstfrag);
					firstfrag++;
				}
			}
		}
		MolWin->AtomsChanged(true, true);
	}
}

void MpGLCanvas::OnIdleEvent(wxIdleEvent& event) {
//	draw();
//	event.RequestMore();
}

#if wxCHECK_VERSION(2, 8, 0)
void MpGLCanvas::eventMouseCaptureLost(wxMouseCaptureLostEvent& event) {
	ReleaseMouse();
}
#endif

BEGIN_EVENT_TABLE(MpGLCanvas, wxGLCanvas)
	/* EVT_IDLE(MpGLCanvas::OnIdleEvent) */
	EVT_SIZE(MpGLCanvas::eventSize)
	EVT_PAINT(MpGLCanvas::eventPaint)
	EVT_ERASE_BACKGROUND(MpGLCanvas::eventErase)
	// EVT_MOUSE_EVENTS(MpGLCanvas::eventMouse) 
	EVT_LEFT_DOWN(MpGLCanvas::eventMouseLeftWentDown)
	EVT_RIGHT_DOWN(MpGLCanvas::eventMouseRightWentDown)
	EVT_MIDDLE_DOWN(MpGLCanvas::eventMouseMiddleWentDown)
	EVT_MOTION(MpGLCanvas::eventMouseDragging)
	EVT_ENTER_WINDOW(MpGLCanvas::eventMouseEnterWindow)
	EVT_ACTIVATE(MpGLCanvas::eventWindowActivated)
	EVT_LEFT_UP(MpGLCanvas::eventMouseLeftWentUp)
	EVT_RIGHT_UP(MpGLCanvas::eventMouseRightWentUp)
	EVT_MIDDLE_UP(MpGLCanvas::eventMouseMiddleWentUp)
	EVT_LEFT_DCLICK(MpGLCanvas::eventMouseLeftDoubleClick)
	EVT_MOUSEWHEEL(MpGLCanvas::eventMouseWheel)
#if wxCHECK_VERSION(2, 8, 0)
	EVT_MOUSE_CAPTURE_LOST(MpGLCanvas::eventMouseCaptureLost)
#endif

//	EVT_CHAR(MpGLCanvas::KeyHandler)	//I don't think we need both a keydown and char event handler?
	EVT_KEY_DOWN(MpGLCanvas::KeyDownHandler)
	EVT_KEY_UP(MpGLCanvas::KeyUpHandler)

	EVT_MENU(GL_Popup_Menu_Apply_All, MpGLCanvas::On_Apply_All)
	EVT_MENU(GL_Popup_Delete_Item_Current_Frame, MpGLCanvas::On_Delete_Single_Frame)
	EVT_MENU(GL_Popup_Delete_Item_All_Frames, MpGLCanvas::On_Delete_All_Frames)
	EVT_MENU(GL_Popup_To_Single_Bond, MpGLCanvas::ChangeBonding)
	EVT_MENU(GL_Popup_To_Double_Bond, MpGLCanvas::ChangeBonding)
	EVT_MENU(GL_Popup_To_Triple_Bond, MpGLCanvas::ChangeBonding)
	EVT_MENU(GL_Popup_To_Aromatic_Bond, MpGLCanvas::ChangeBonding)
	EVT_MENU(GL_Popup_To_Hydrogen_Bond, MpGLCanvas::ChangeBonding)
	EVT_MENU(GL_Popup_Delete_Bond, MpGLCanvas::DeleteBond)
	EVT_MENU(GL_Popup_Add_Plane_Normal, MpGLCanvas::AddPlaneNormal)
	EVT_MENU(GL_Popup_Measure_Length, MpGLCanvas::AddAnnotation)
	EVT_MENU(GL_Popup_Measure_Angle, MpGLCanvas::AddAnnotation)
	EVT_MENU(GL_Popup_Measure_Dihedral, MpGLCanvas::AddAnnotation)
	EVT_MENU(GL_Popup_Mark_Atom, MpGLCanvas::AddAnnotation)
	EVT_MENU(GL_Popup_Unmark_Atom, MpGLCanvas::DeleteAnnotation)
	EVT_MENU(GL_Popup_Delete_Length, MpGLCanvas::DeleteAnnotation)
	EVT_MENU(GL_Popup_Delete_Angle, MpGLCanvas::DeleteAnnotation)
	EVT_MENU(GL_Popup_Delete_Dihedral, MpGLCanvas::DeleteAnnotation)
	EVT_MENU(GL_Popup_Set_Anno_Param, MpGLCanvas::SetAnnotationParameter)
	EVT_MENU(GL_Popup_Lock_To_Annotation, MpGLCanvas::ConstrainToAnnotation)
	EVT_MENU(GL_Popup_Fit_To_Plane, MpGLCanvas::FitToPlane)
	EVT_MENU(GL_Popup_Change_Atom, MpGLCanvas::ChangeAtom)
	EVT_MENU(GL_Popup_To_Coordination_Zero, MpGLCanvas::ChangeCoordinationNumber)
	EVT_MENU(GL_Popup_To_Coordination_One, MpGLCanvas::ChangeCoordinationNumber)
	EVT_MENU(GL_Popup_To_Coordination_Two, MpGLCanvas::ChangeCoordinationNumber)
	EVT_MENU(GL_Popup_To_Coordination_Three, MpGLCanvas::ChangeCoordinationNumber)
	EVT_MENU(GL_Popup_To_Coordination_Four, MpGLCanvas::ChangeCoordinationNumber)
	EVT_MENU(GL_Popup_To_Coordination_Five, MpGLCanvas::ChangeCoordinationNumber)
	EVT_MENU(GL_Popup_To_Coordination_Six, MpGLCanvas::ChangeCoordinationNumber)
	EVT_MENU(GL_Popup_To_LPCount_Zero, MpGLCanvas::ChangeLPCount)
	EVT_MENU(GL_Popup_To_LPCount_One, MpGLCanvas::ChangeLPCount)
	EVT_MENU(GL_Popup_To_LPCount_Two, MpGLCanvas::ChangeLPCount)
	EVT_MENU(GL_Popup_To_LPCount_Three, MpGLCanvas::ChangeLPCount)
	EVT_MENU(GL_Popup_To_LPCount_Four, MpGLCanvas::ChangeLPCount)
	EVT_MENU(GL_Popup_To_LPCount_Five, MpGLCanvas::ChangeLPCount)
	EVT_MENU(GL_Popup_Paste_At, MpGLCanvas::PasteAtMouse)
	EVT_MENU(GL_Popup_Save_Prototype, MpGLCanvas::SavePrototype)
	EVT_MENU(GL_Popup_Change_EFP_To_AllElec, MpGLCanvas::ConvertEFPToAllElec)
END_EVENT_TABLE()

