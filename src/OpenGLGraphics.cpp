/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*еееееееееееееееееееееееееееееееееееееееееее
  е   routines to implement OpenGL graphics	е
  е	  Brett Bode							е
  еееееееееееееееееееееееееееееееееееееееееее

	Added RotateMoelculeGL routine and code to generate the angle strings and trackball - BMB July 2001
	Corrected Create3DGLPICT for resolutions other than 72 dpi - BMB Feb 2002
*/

#include "Globals.h"

#ifndef __wxBuild__
#include "MyWindowClasses.h"
#endif

#include "MoleculeData.h"
#include "Frame.h"
#include "SurfaceTypes.h"
#include "Math3D.h"
#include "InputData.h"

#ifndef __wxBuild__
#include "MolDisplay.h"
#include "Offscreen.h"
#else
#include "MolDisplayWin.h"
#include "mpGLCanvas.h"
#include "glf.h"
#ifdef __WXMAC__
#include <AGL/glu.h>
#else
#include "GL/glu.h"
#endif
#endif

#include "Prefs.h"
#include "Progress.h"

#ifndef __wxBuild__
#if __GNUC__
#include <AGL/agl.h>
#include <AGL/glu.h>
#else
#include <agl.h>
#include <glu.h>
#endif
#include "aglString.h"
#endif

#include "patterns.h"

#include <new>
#include <string.h>

#include <iostream>

#ifndef __wxBuild__
extern Boolean	gOpenGLAvailable;
#endif
	//0.0577 corresponds to fov=60 with zNear=0.1
#define myGLperspective	0.050	//0.050 seems to match my 2D mode
//#define myGLperspective	0.10	//0.050 seems to match my 2D mode

//#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

void CreateCylinderFromLine(GLUquadricObj * qobj, const CPoint3D & lineStart, const CPoint3D & lineEnd, const float & lineWidth);
void DrawRotationAxis(const CPoint3D & lineStart, const CPoint3D & lineEnd, const int & order);
void DrawInversionPoint(void);
void DrawTranslucentPlane(const CPoint3D & origin, const CPoint3D & p1, const CPoint3D & p2);

const GLubyte stippleMask[128] =

  {0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
    0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
    0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
    0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
    0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
    0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
    0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
    0xaa, 0xaa, 0xaa, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00};

GLfloat d_specular[] = {0.1, 0.1, 0.1, 1.0};
GLfloat d_shininess[] = {1.0};
GLfloat d_diffuse[] = {0.02,0.02,0.02,0.8};
GLfloat d_ambient[] = {0.1,0.1,0.1,0.8};

GLfloat l_specular[] = {0.8, 0.8, 0.8, 1.0};
GLfloat l_shininess[] = {80.0};
GLfloat l_diffuse[] = {0.2,0.2,0.2,0.8};
GLfloat l_ambient[] = {0.1,0.1,0.1,0.8};

class OpenGLRec {
	public:
#ifndef __wxBuild__
		AGLContext aglContext;
		AGLPixelFormat	fmt;
		GLuint fontList;
#endif
		
		myGLTriangle *  transpTriList;
		CPoint3D *		transpSortVertex;
		long *			transpIndex;
		long			triangleCount;
		
		GLuint			MainDisplayList;
		GLuint			SurfaceDisplayList;
		bool			MainListActive;
		bool			SurfaceListActive;
		bool			haveTransparentSurfaces;
		
		OpenGLRec(void);
		~OpenGLRec(void);
};
OpenGLRec::OpenGLRec(void) {
	transpTriList = NULL;
	transpSortVertex = NULL;
	transpIndex = NULL;
	triangleCount = 0;
	
	MainListActive = false;
	SurfaceListActive = false;
	haveTransparentSurfaces = false;
}
OpenGLRec::~OpenGLRec(void) {
	if (transpTriList) {
		delete [] transpTriList;
		transpTriList = NULL;
	}
	if (transpSortVertex) {
		delete [] transpSortVertex;
		transpSortVertex = NULL;
	}
	if (transpIndex) {
		delete [] transpIndex;
		transpIndex = NULL;
	}
}
#ifdef __wxBuild__
void MolDisplayWin::InitGLData(void) {
	OpenGLData = new OpenGLRec;
}
void MolDisplayWin::DeleteGLData(void) {
    if (OpenGLData)
		delete OpenGLData;
	OpenGLData = NULL;
}
#endif

#ifndef __wxBuild__
void MolDisplayWin::SetOpenGLState(bool NewState)
{
	if (gOpenGLAvailable) {	//Make sure OpenGL is really available before activating
		if (winData.is3DModeActive() != NewState) {
			if (winData.is3DModeActive() && !NewState) {
				OpenGLExitWindow();
				ResetModel(false);
			} else if (!winData.is3DModeActive() && NewState) {
				OpenGLInitWindow();
				myInValidWindowRect(thisWindow, &DisplayRect);
			}
		}
	}
}

void MolDisplayWin::OpenGLInitWindow(void)
{
	if (gOpenGLAvailable) {
		SetPortWindowPort(thisWindow);
		EraseRect(&DisplayRect);
		OpenGLData = new OpenGLRec;
		OpenGLData->fontList = 0;
		GLint aglAttributes[] = {AGL_RGBA, AGL_DOUBLEBUFFER, AGL_DEPTH_SIZE, 16, AGL_NONE};
		
		OpenGLData->fmt = aglChoosePixelFormat (NULL, 0, aglAttributes); // get an appropriate pixel format
		GLenum err = aglGetError();

		OpenGLData->aglContext = aglCreateContext (OpenGLData->fmt, NULL);			// Create an AGL context
		err = aglGetError();

		if (!OpenGLData->aglContext || !aglSetDrawable (OpenGLData->aglContext, GetWindowPort (thisWindow)) ||
			(err != noErr))
		{
			OpenGLExitWindow();
			return;			
		}
		else
		{
			aglSetCurrentContext (OpenGLData->aglContext);
				//setup viewport to the appropriate area of the screen
				//bufferRect seems to be x,y of lower left corner, then width and height
			GLint bufferRect[4];
			bufferRect [0] = 0; 
			bufferRect [1] = InfoRect.bottom - InfoRect.top;
			bufferRect [2] = DisplayRect.right - DisplayRect.left;
			bufferRect [3] = DisplayRect.bottom - DisplayRect.top;
			aglSetInteger (OpenGLData->aglContext, AGL_BUFFER_RECT, bufferRect);
			aglEnable (OpenGLData->aglContext, AGL_BUFFER_RECT);
			aglUpdateContext (OpenGLData->aglContext);
			glViewport (0, 0, bufferRect [2], bufferRect [3]);

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
	
			UpdateGLView();
	
			glClear (GL_COLOR_BUFFER_BIT);
			aglSwapBuffers (OpenGLData->aglContext);

				//Setup a agl font so we can use it later
				short fNum;
			GetFNum("\pMonaco", &fNum);									// build font
			OpenGLData->fontList = BuildFontGL (OpenGLData->aglContext, fNum, normal, 9);
		}		
		
		winData.is3DModeActive(true);
	}
}

void MolDisplayWin::OpenGLExitWindow(void)
{
	if (OpenGLData) {
		if (OpenGLData->fontList) {
			DeleteFontGL (OpenGLData->fontList);
			OpenGLData->fontList = 0;
		}
		if (OpenGLData->aglContext) {
			glFinish ();
			if (OpenGLData->MainListActive) {
				glDeleteLists(OpenGLData->MainDisplayList, 1);
				OpenGLData->MainListActive = false;
			}
			if (OpenGLData->SurfaceListActive) {
				glDeleteLists(OpenGLData->SurfaceDisplayList, 1);
				OpenGLData->SurfaceListActive = false;
			}
			aglSetCurrentContext (NULL);
			GLenum err = aglGetError();
			aglSetDrawable (OpenGLData->aglContext, NULL);
			err = aglGetError ();
			aglDestroyContext (OpenGLData->aglContext);
			err = aglGetError ();
			
			OpenGLData->aglContext = NULL;
			
			if (OpenGLData->fmt)
			{
				aglDestroyPixelFormat (OpenGLData->fmt); // pixel format is no longer valid
				err = aglGetError ();
			}
			OpenGLData->fmt = 0;
		}
		delete OpenGLData;
		OpenGLData = NULL;
	}
	winData.is3DModeActive(false);
}

void MolDisplayWin::UpdateGLView(void)
{
	if (OpenGLData) {
		GLint bufferRect[4];
		bufferRect [0] = 0; 
		bufferRect [1] = InfoRect.bottom - InfoRect.top;
		bufferRect [2] = DisplayRect.right - DisplayRect.left;
		bufferRect [3] = DisplayRect.bottom - DisplayRect.top;
		aglSetCurrentContext (OpenGLData->aglContext);
		aglSetInteger (OpenGLData->aglContext, AGL_BUFFER_RECT, bufferRect);
		aglUpdateContext (OpenGLData->aglContext);
		glViewport (0, 0, bufferRect [2], bufferRect [3]);
		float hsize = DisplayRect.right - DisplayRect.left;
		float vsize = DisplayRect.bottom - DisplayRect.top;
		GLdouble aspect = hsize/vsize;
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
#endif
void MolDisplayWin::UpdateGLModel(void) {	//model has changed so force update
	if (OpenGLData != NULL) {
		OpenGLData->triangleCount = 0;
		if (OpenGLData->transpTriList) {
			delete [] OpenGLData->transpTriList;
			OpenGLData->transpTriList = NULL;
		}
		if (OpenGLData->transpSortVertex) {
			delete [] OpenGLData->transpSortVertex;
			OpenGLData->transpSortVertex = NULL;
		}
		if (OpenGLData->transpIndex) {
			delete [] OpenGLData->transpIndex;
			OpenGLData->transpIndex = NULL;
		}
		if (OpenGLData->MainListActive) {
			glDeleteLists(OpenGLData->MainDisplayList, 1);
			OpenGLData->MainListActive = false;
		}
		if (OpenGLData->SurfaceListActive) {
			glDeleteLists(OpenGLData->SurfaceDisplayList, 1);
			OpenGLData->SurfaceListActive = false;
		}
	}
}

#ifndef __wxBuild__
//When using openGL we must update the draw context when a window is moved
void MolDisplayWin::WindowMoved(void)
{
	if (winData.is3DModeActive() && OpenGLData) {
		aglSetCurrentContext (OpenGLData->aglContext);
		aglUpdateContext (OpenGLData->aglContext);
	}
}

void MolDisplayWin::Create3DGLPICT(WindowPtr PrintWindow)
{
	//Our goal is to grab the pixels for the current front buffer and copy them to the printwindow (and thus
	//the PICT).
	int width = DisplayRect.right - DisplayRect.left;
	int height = DisplayRect.bottom - DisplayRect.top;
		Rect windowBounds;
	GetWindowPortBounds(PrintWindow, &windowBounds);
	int ScaledWidth = windowBounds.right - windowBounds.left;
	int ScaledHeight = windowBounds.bottom - windowBounds.top;
	int NumXPasses = ScaledWidth / width;
	if ((NumXPasses * width) < ScaledWidth) NumXPasses++;
	int NumYPasses = ScaledHeight / height;
	if ((NumYPasses * height) < ScaledHeight) NumYPasses++;
	int ViewportScaledX = NumXPasses * width;
	int ViewportScaledY = NumYPasses * height;
	unsigned char * array = new unsigned char[width*height*sizeof(GLbyte)*4];
	unsigned char * arrayorder = new unsigned char[width*height*sizeof(GLbyte)*4];
	GLvoid * pixels= (GLvoid *) array;
		Rect	WorkingPrintRect;
		
	glReadBuffer(GL_BACK);
	PixMapHandle myPixMap = NewPixMap();
	GLdouble hGLsize, vGLsize, GLLeft, GLTop;
	double aspect = ((double)width)/((double)height);
	if (aspect > 1.0) {
		hGLsize = 2.0*(myGLperspective) / NumXPasses;	//This corresponds to fov=60 with zNear=0.1
		vGLsize = hGLsize/aspect;
		GLLeft = -myGLperspective;
		GLTop = -GLLeft/aspect;
	} else {
		vGLsize = 2.0*(myGLperspective) / NumXPasses;
		hGLsize = vGLsize * aspect;
		GLTop = myGLperspective;
		GLLeft = -GLTop *aspect;
	}
	
	if (NumXPasses > 1 || NumYPasses > 1) {
		Prefs->CylindersForLines(true);
		UpdateGLModel();
	}
	
	for (int jpass=0; jpass<NumYPasses; jpass++) {
		int passheight = height;
		if ((jpass+1) == NumYPasses) passheight = height - (ViewportScaledY - ScaledHeight);
		for (int ipass=0; ipass<NumXPasses; ipass++) {
			int passwidth = width;
			if ((ipass+1) == NumXPasses) passwidth = width - (ViewportScaledX - ScaledWidth);

				//Draw into back buffer for each pass, unless there is only one pass total
				//in which case the back buffer should be up to date already
			if (NumXPasses > 1 || NumYPasses > 1) {
				//Setup the projection matrix to view the correct piece of the view for this pass
				glMatrixMode (GL_PROJECTION);	//Setup the model space to screen space mapping
				glLoadIdentity ();
				GLdouble top, bottom, left, right;
				left = GLLeft + ipass*hGLsize;
				right = left + hGLsize;
				top = GLTop - jpass*vGLsize;
				bottom = top - vGLsize;
				glFrustum(left, right, bottom, top, 0.1, 100.0);

				DrawGL();

				glFinish();
			}
			glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

				//reorder the pixels so colors are correct and top to bottom row order
			for (long i=0; i<height; i++ ) {
				long oldrowstart = (height - (i+1))*(width*4*sizeof(GLbyte));
				long newrowstart = i*(width*4*sizeof(GLbyte));
				for (long j=0; j<width; j++) {
#ifdef __LITTLE_ENDIAN__
					arrayorder[newrowstart + j*4] = array[oldrowstart + j*4 +3];
					arrayorder[newrowstart + j*4 + 1] = array[oldrowstart + j*4 +2];
					arrayorder[newrowstart + j*4 + 2] = array[oldrowstart + j*4 +1];
					arrayorder[newrowstart + j*4 + 3] = array[oldrowstart + j*4 +0];
#else
					arrayorder[newrowstart + j*4] = array[oldrowstart + j*4 +3];
					arrayorder[newrowstart + j*4 + 1] = array[oldrowstart + j*4 +0];
					arrayorder[newrowstart + j*4 + 2] = array[oldrowstart + j*4 +1];
					arrayorder[newrowstart + j*4 + 3] = array[oldrowstart + j*4 +2];
#endif
				}
			}


				//Coerce the pixel data to a bitmap structure

			(**myPixMap).baseAddr = (char *)arrayorder;
			(**myPixMap).rowBytes = passwidth*4*sizeof(GLbyte) | 0x8000;
				//setup output rect
			WorkingPrintRect.left = ipass*width;
			WorkingPrintRect.right = WorkingPrintRect.left + passwidth;
			WorkingPrintRect.top = jpass*height;
			WorkingPrintRect.bottom = WorkingPrintRect.top + passheight;
			(**myPixMap).bounds = WorkingPrintRect;
			(**myPixMap).pmVersion = 0;
			(**myPixMap).packType = 0;
			(**myPixMap).packSize = 0;
			(**myPixMap).hRes = (**myPixMap).vRes = 72 << 16;
			(**myPixMap).pixelType = RGBDirect;
			(**myPixMap).pixelSize = 32;
			(**myPixMap).cmpCount = 3;
			(**myPixMap).cmpSize = 8;
#ifdef __LITTLE_ENDIAN__
			(**myPixMap).pixelFormat = k32ABGRPixelFormat;
#else
		//Its not at all clear to me that the following makes any difference on ppc, but the above is 
		//needed on Intel
			(**myPixMap).pixelFormat = k32RGBAPixelFormat;
#endif
			(**myPixMap).pmExt = 0;
			
				//Now use copybits to push the pixels onto the printwindow
			CopyBits ((BitMap *) (*myPixMap), GetPortBitMapForCopyBits(GetWindowPort(PrintWindow)),
				  &WorkingPrintRect, &WorkingPrintRect, srcCopy, NULL);
		  }
	}
		  
	delete [] array;
	delete [] arrayorder;
	DisposePixMap(myPixMap);
	if (NumXPasses > 1 || NumYPasses > 1) {
		Prefs->CylindersForLines(false);
		UpdateGLModel();
		UpdateGLView();
		DrawMoleculeGL();
	}
}

#ifdef PM_OLDAPI
void MolDisplayWin::Print3DGL(PMPrintContext myPrintContext, const PMPageFormat myFormat,
	long PrinterRes, Boolean Center, Boolean FramePrintOut)
#else
void MolDisplayWin::Print3DGL(const PMPrintSession mySession,
	const PMPageFormat myFormat, long PrinterRes, bool Center, bool FramePrintOut)
#endif
{			//This routine is setup for Carbon only
	ProgressInd->ChangeText("Rendering large image...");
		//Calculate a factor to convert the 72dpi QD3D drawing to the printer res
	float ScaleFactor = (float)PrinterRes/72.0;
		PMRect	PageRect;
	PMGetAdjustedPageRect(myFormat, &PageRect);
#ifdef PM_OLDAPI
		GrafPtr printerGPort;
	PMGetGrafPtr(myPrintContext, &printerGPort);
#else
		GrafPtr printerGPort;
	PMSessionGetGraphicsContext(mySession, NULL, (void **) &printerGPort);
#endif
	
		int width = DisplayRect.right - DisplayRect.left;
		int height = DisplayRect.bottom - DisplayRect.top;
		int ScaledWidth = (int) (width * ScaleFactor);
		int ScaledHeight = (int) (height * ScaleFactor);
		int NumXPasses = ScaledWidth / width;
		if ((NumXPasses * width) < ScaledWidth) NumXPasses++;
		int NumYPasses = ScaledHeight / height;
		if ((NumYPasses * height) < ScaledHeight) NumYPasses++;
		int ViewportScaledX = NumXPasses * width;
		int ViewportScaledY = NumYPasses * height;
		unsigned char * array = new unsigned char[width*height*sizeof(GLbyte)*4];
		unsigned char * arrayorder = new unsigned char[width*height*sizeof(GLbyte)*4];
		GLvoid * pixels= (GLvoid *) array;
		Rect	WorkingPrintRect;
	long hOffset=0, vOffset=0;
	if (Center) {	//Compute the offset to move the rect to center on the page
		long PageCenterH = (long) (PageRect.right - PageRect.left);
		long PageCenterV = (long) (PageRect.bottom - PageRect.top);
		PageCenterH /= 2;
		PageCenterV /= 2;
		long DCenterH = ScaledWidth/2;
		long DCenterV = ScaledHeight/2;
		vOffset = PageCenterV - DCenterV;
		hOffset = PageCenterH - DCenterH;
	}

	PixMapHandle myPixMap = NewPixMap();
	aglSetCurrentContext (OpenGLData->aglContext);
	glReadBuffer(GL_BACK);

	GLdouble hGLsize, vGLsize, GLLeft, GLTop;
	double aspect = ((double)width)/((double)height);
	if (aspect > 1.0) {
		hGLsize = 2.0*(myGLperspective) / ScaleFactor;	//This corresponds to fov=60 with zNear=0.1
		vGLsize = hGLsize/aspect;
		GLLeft = -myGLperspective;
		GLTop = -GLLeft/aspect;
	} else {
		vGLsize = 2.0*(myGLperspective) / ScaleFactor;
		hGLsize = vGLsize * aspect;
		GLTop = myGLperspective;
		GLLeft = -GLTop *aspect;
	}

	Prefs->CylindersForLines(true);
	UpdateGLModel();
	
	for (int jpass=0; jpass<NumYPasses; jpass++) {
		int passheight = height;
		if ((jpass+1) == NumYPasses) passheight = height - (ViewportScaledY - ScaledHeight);
		for (int ipass=0; ipass<NumXPasses; ipass++) {
			int passwidth = width;
			if ((ipass+1) == NumXPasses) passwidth = width - (ViewportScaledX - ScaledWidth);

	//Setup the projection matrix to view the correct piece of the view for this pass
			glMatrixMode (GL_PROJECTION);	//Setup the model space to screen space mapping
			glLoadIdentity ();
			GLdouble top, bottom, left, right;
			left = GLLeft + ipass*hGLsize;
			right = left + hGLsize;
			top = GLTop - jpass*vGLsize;
			bottom = top - vGLsize;
			glFrustum(left, right, bottom, top, 0.1, 100.0);

			DrawGL();

			glFinish();
			glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				//reorder the pixels so colors are correct and top to bottom row order
				//Note the last pass over x and y is only over the remaining pixels in the output
				//image not the full image read from GL.
			for (long i=0; i<passheight; i++ ) {
					//Start at the bottom of the scaled image (width is the full GL width)
				long oldrowstart = (height - (i+1))*(width*4*sizeof(GLbyte));
					//New image is just passwidth wide.
				long newrowstart = i*(passwidth*4*sizeof(GLbyte));
				for (long j=0; j<passwidth; j++) {
					arrayorder[newrowstart + j*4] = array[oldrowstart + j*4 +3];
					arrayorder[newrowstart + j*4 + 1] = array[oldrowstart + j*4 +0];
					arrayorder[newrowstart + j*4 + 2] = array[oldrowstart + j*4 +1];
					arrayorder[newrowstart + j*4 + 3] = array[oldrowstart + j*4 +2];
				}
			}

				//Coerce the pixel data to a bitmap structure

			(**myPixMap).baseAddr = (char *)arrayorder;
			(**myPixMap).rowBytes = passwidth*4*sizeof(GLbyte) | 0x8000;
				//setup output rect
			WorkingPrintRect.left = ipass*width + hOffset;
			WorkingPrintRect.right = WorkingPrintRect.left + passwidth;
			WorkingPrintRect.top = jpass*height + vOffset;
			WorkingPrintRect.bottom = WorkingPrintRect.top + passheight;
			(**myPixMap).bounds = WorkingPrintRect;
			(**myPixMap).pmVersion = 0;
			(**myPixMap).packType = 0;
			(**myPixMap).packSize = 0;
			(**myPixMap).hRes = (**myPixMap).vRes = 72 << 16;
			(**myPixMap).pixelType = RGBDirect;
			(**myPixMap).pixelSize = 32;
			(**myPixMap).cmpCount = 3;
			(**myPixMap).cmpSize = 8;
			(**myPixMap).pixelFormat = k32RGBAPixelFormat;
			(**myPixMap).pmExt = 0;
			
				//Now use copybits to push the pixels onto the printwindow
			CopyBits ((BitMap *) (*myPixMap), GetPortBitMapForCopyBits(printerGPort),
				&WorkingPrintRect, &WorkingPrintRect, srcCopy, NULL);
		  
		  }
	}
	delete [] array;
	delete [] arrayorder;
	DisposePixMap(myPixMap);

	if (FramePrintOut) {
			RGBColor	BlackColor={0,0,0};
		PenSize((short)ScaleFactor, (short)ScaleFactor);
		RGBForeColor(&BlackColor);
		Rect TotalRect;
		TotalRect.left = hOffset;
		TotalRect.right = TotalRect.left + ScaledWidth;
		TotalRect.top = vOffset;
		TotalRect.bottom = TotalRect.top + ScaledHeight;
		FrameRect(&TotalRect);
	}
	Prefs->CylindersForLines(false);
	UpdateGLModel();
	SetCursorToArrow();
}

void MolDisplayWin::DrawMoleculeGL(void)
{
	aglSetCurrentContext (OpenGLData->aglContext);

	DrawGL();	//actual drawing is abstracted into drawGL so printing can share the same function

	aglSwapBuffers(OpenGLData->aglContext);	// finally swap buffers to display our work
}
#endif
void MolDisplayWin::RotateMoleculeGL(bool ShowAngles)
{
#ifndef __wxBuild__
	aglSetCurrentContext (OpenGLData->aglContext);
#endif
	
	if (OpenGLData->transpTriList) { //update the transparent surface sorting
		SortTransparentTriangles();
	}
	DrawGL();	//actual drawing

	{	//Now add stuff specific to rotations
//		glDisable(GL_DEPTH_TEST);	//These are not strictly neccessary, but probably increase speed
//		glShadeModel(GL_FLAT);
//		glDisable(GL_LIGHTING);

		GLint matrixMode;
		glGetIntegerv (GL_MATRIX_MODE, &matrixMode);
		glMatrixMode (GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity ();
		glMatrixMode (GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity ();
#ifdef __wxBuild__
		wxRect DisplayRect = glCanvas->GetRect();
		long hsize = DisplayRect.GetWidth();
		long vsize = DisplayRect.GetHeight();
#else
		long hsize = DisplayRect.right - DisplayRect.left;
		long vsize = DisplayRect.bottom - DisplayRect.top;
#endif
		glScalef (2.0 / hsize, -2.0 /  vsize, 1.0);
		glTranslatef (-hsize / 2.0, -vsize / 2.0, 0.0);

		RGBColor * BackgroundColor = Prefs->GetBackgroundColorLoc();
		long backMagnitude = BackgroundColor->red + BackgroundColor->green + BackgroundColor->blue;

	//choose black or white based on the background color
		if (backMagnitude > 70000)  //"light" background choose black
		  glColor3f (0.0, 0.0, 0.0);
		else
		  glColor3f (1.0, 1.0, 1.0);

		if (ShowAngles) {
		  char AngleString[50];
		  float psi, phi, theta;
		  MatrixToEulerAngles(MainData->TotalRotation, &psi, &phi, &theta);
		  sprintf((char *)AngleString, "%.2f, %.2f, %.2f, Scale:%.2f",
			  psi, phi, theta, MainData->WindowSize);

		  int canvasWidth, canvasHeight;
		  glCanvas->GetSize(&canvasWidth, &canvasHeight);
		  float sclX = 20/(float)canvasWidth;
		  float sclY = 20/(float)canvasHeight;

		  glPushMatrix();
		  glLoadIdentity();
		  glTranslatef(-0.85, 0.95, 0);
		  glScalef(sclX, sclY, 1);
		  glfDrawSolidString(AngleString);
		  glPopMatrix();

		  glFlush();
		  //glCanvas->SwapBuffers();
		}
#ifndef __wxBuild__
				if (ShowAngles) {
					glRasterPos3d (10, 12, 0); 
						char AngleString[50];
						float psi, phi, theta;
					MatrixToEulerAngles(MainData->TotalRotation, &psi, &phi, &theta);
					sprintf((char *)AngleString, "%.2f, %.2f, %.2f, Scale:%.2f",
						psi, phi, theta, MainData->WindowSize);
					DrawCStringGL (AngleString, OpenGLData->fontList);
			//		glRasterPos3d (10, (DisplayRect.bottom - DisplayRect.top) - 3, 0); 
			//		DrawCStringGL ((char*) glGetString (GL_RENDER), OpenGLData->fontList);
				}
#endif
				//Draw the trackball outline
				{
					Point			sphereCenter;
					long			sphereRadius; 
					sphereCenter.h = hsize/2; 
					sphereCenter.v = vsize/2;
					if (sphereCenter.h >= sphereCenter.v)
						sphereRadius   = (long)((float) (sphereCenter.h)*0.9);
					else
						sphereRadius   = (long)((float) (sphereCenter.v)*0.9);
					long NumDivisions = (long) (20.0*(1.0+ sphereRadius/200.0));
					float divarc = (2*kPi)/NumDivisions;

					glLineWidth(1);
					glBegin(GL_LINE_LOOP);
					glVertex3d(sphereCenter.h-sphereRadius, sphereCenter.v, 0.0);
					for (int i=0; i<NumDivisions; i++) {
						float x = sphereCenter.h - (sphereRadius*cos(i*divarc));
						float y = sphereCenter.v + (sphereRadius*sin(i*divarc));
						glVertex3d(x, y, 0.0);
					}
					glEnd();
				}
			glPopMatrix(); // GL_MODELVIEW
			glMatrixMode (GL_PROJECTION);
			glPopMatrix();
		glMatrixMode (matrixMode);
	}
	
#ifndef __wxBuild__
	aglSwapBuffers(OpenGLData->aglContext);	// finally swap buffers to display our work
#endif
}
void MolDisplayWin::DrawGL(void)
{
	GLenum error = glGetError();	//clear the error code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Clear out the buffer

		// Setup the rotation matrix
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glTranslatef(0.0, 0.0, -(MainData->WindowSize));

	if (Prefs->ShowAtomicSymbolLabels() || Prefs->ShowAtomNumberLabels() )
	  DrawLabel();

	if (interactiveMode)
	  {
	    const char modeString[] = "editing";

	    DrawStaticLabel(modeString, 0.75, 0.95);
	  }

	glMultMatrixf((const GLfloat *) &(MainData->TotalRotation));

	glEnable(GL_LIGHTING);
		//Draw the main molecular geometry

	if (MainData->cFrame->NumAtoms > 0) {
		if (OpenGLData->MainListActive) {
			glCallList(OpenGLData->MainDisplayList);
		} else {	//build the main display list
         // Suppress this temporarily because double- and triple-bond display
         // requires a transformation depending on the current viewing 
         // transformation.
			// OpenGLData->MainDisplayList = glGenLists(1); 
			// glNewList(OpenGLData->MainDisplayList, GL_COMPILE_AND_EXECUTE); 
			DrawMoleculeCoreGL();
			// glEndList(); 
			// OpenGLData->MainListActive = true; 
		}
	}
		
	if (MainData->ShowAxis()) AddAxisGL();

		//Add any surfaces
	Surface * lSurface = MainData->cFrame->SurfaceList;
	error = glGetError();
		//draw all the normal opaque surfaces
	//Ok the following works fine on all my systems, but for some reason it fails to work
	//on for a few other folks. I sort of think this is really an OpenGL bug, but the difference
	//in performance is probably not enough to worry about...
//	if (OpenGLData->SurfaceListActive) {
//		glCallList(OpenGLData->SurfaceDisplayList);
//		error = glGetError();
//		if (error != GL_NO_ERROR) {
//			Str255 errmsg;
//			sprintf(&(errmsg[1]),"gl error during the glCallList(surfacelist) drawing %d\n", error);
//			errmsg[0] = strlen(&(errmsg[1]));
//			MessageAlert(errmsg);
//		}
//	} else {
		OpenGLData->haveTransparentSurfaces = false;
//		OpenGLData->SurfaceDisplayList = glGenLists(1);
//		if (OpenGLData->SurfaceDisplayList != 0)
//			glNewList(OpenGLData->SurfaceDisplayList, GL_COMPILE_AND_EXECUTE);
		while (lSurface) {
			if (lSurface->GetVisibility()) {
				if (! lSurface->isTransparent()) {
					lSurface->Draw3DGL(MainData, Prefs, NULL);
				} else {
					OpenGLData->haveTransparentSurfaces = true;
				}
			}
			lSurface = lSurface->GetNextSurface();
		}
//		if (OpenGLData->SurfaceDisplayList != 0) {
//			glEndList();
//			OpenGLData->SurfaceListActive = true;
//		}
//		error = glGetError();
//		if (error != GL_NO_ERROR) {
//			if (OpenGLData->SurfaceDisplayList != 0) {
//				glDeleteLists(OpenGLData->SurfaceDisplayList, 1);
//				OpenGLData->SurfaceDisplayList = 0;
//				OpenGLData->SurfaceListActive = false;
//			}
//			Str255 errmsg;
//			sprintf(&(errmsg[1]),"gl returned an error during the surfacelist creation %d\n", error);
//			errmsg[0] = strlen(&(errmsg[1]));
//			MessageAlert(errmsg);
//		}
//	}
	if (Prefs->ShowSymmetryOperators()) AddSymmetryOperators();
	//Transparent surfaces have to be depth sorted and drawn last.
	if (OpenGLData->haveTransparentSurfaces) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Surface * lSurface = MainData->cFrame->SurfaceList;
		if (! OpenGLData->transpTriList) {
			long totalTriCount = 0;
			while (lSurface) {
				if (lSurface->GetVisibility() && lSurface->isTransparent()) {
					totalTriCount += lSurface->getTriangleCount();
				}
				lSurface = lSurface->GetNextSurface();
			}
			OpenGLData->transpTriList = new myGLTriangle[totalTriCount];
			OpenGLData->transpSortVertex = new CPoint3D[totalTriCount];
			OpenGLData->transpIndex = new long[totalTriCount];
			OpenGLData->triangleCount = totalTriCount;
			
			lSurface = MainData->cFrame->SurfaceList;
			long triStartCount = 0;
			while (lSurface) {
				if (lSurface->GetVisibility() && lSurface->isTransparent()) {
					triStartCount += lSurface->Draw3DGL(MainData, Prefs, 
											&(OpenGLData->transpTriList[triStartCount]));
				}
				lSurface = lSurface->GetNextSurface();
			}
			for (int i=0; i<OpenGLData->triangleCount; i++) OpenGLData->transpIndex[i] = i;
			SortTransparentTriangles();
		}
		DrawTransparentTriangles();
		glDisable(GL_BLEND);
	}
	glDisable(GL_LIGHTING);
}

void MolDisplayWin::DrawStaticLabel(const char* label, GLfloat x, GLfloat y)
{
  int canvasWidth, canvasHeight;

  glCanvas->GetSize(&canvasWidth, &canvasHeight);
  float sclX = 20/(float)canvasWidth;
  float sclY = 20/(float)canvasHeight;

  RGBColor * BackgroundColor = Prefs->GetBackgroundColorLoc();
  long backMagnitude = BackgroundColor->red + BackgroundColor->green + BackgroundColor->blue;

  if (backMagnitude > 70000)
    glColor3f (0.0, 0.0, 0.0);
  else
    glColor3f (1.0, 1.0, 1.0);

  GLint matrixMode;
  glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity ();

  glTranslatef(x, y, 0.999999);
  glLoadName(0);
  glScalef(sclX, sclY, 1);
  glfDrawSolidString(label);

  glFlush();

  glPopMatrix(); // GL_MODELVIEW
  glMatrixMode (GL_PROJECTION);
  glPopMatrix();
  glMatrixMode (matrixMode);
}

void MolDisplayWin::DrawLabel()
{
  Frame * lFrame=MainData->cFrame;
  mpAtom * lAtoms = lFrame->Atoms;
  long NumAtoms = lFrame->NumAtoms;
  float AtomScale = Prefs->GetAtomScale();
  GLdouble BondSize = Prefs->GetQD3DBondWidth();

  wxString atomLabel;
  long CurrentAtomType;
  CPoint3D origPt, transPt;
  long CurrentAtom;

  if (!Prefs->DrawWireFrame() || Prefs->ColorBondHalves()) 
    {
      for (long iatom=0; iatom<NumAtoms; iatom++) 
	{
	  if (lAtoms[iatom].GetInvisibility()) continue;	//Atom is invisible so skip

	  atomLabel.Clear();

	  CurrentAtomType = lAtoms[iatom].GetType() - 1;

	  //!!! retrieve atom label
	  if ( Prefs->ShowAtomicSymbolLabels() )
	    Prefs->GetAtomLabel(CurrentAtomType, atomLabel);

	  if (Prefs->ShowAtomNumberLabels() )
	    {
	      wxString tmpStr;

	      tmpStr.Printf(wxT("%d"), iatom+1);
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

	  origPt.x = lAtoms[iatom].Position.x;
	  origPt.y = lAtoms[iatom].Position.y;
	  origPt.z = lAtoms[iatom].Position.z;

	  Rotate3DPt(MainData->TotalRotation, origPt, &transPt );

	  glPushMatrix();
	  glTranslatef(0.0, 0.0, radius+0.01);
	  glTranslatef(transPt.x, transPt.y, transPt.z);

	  glColor3f(1-red, 1-green, 1-blue);
	  glScalef(0.1+0.08*radius, 0.1+0.08*radius, 1);
	  glLoadName(iatom+1);
	  glfDrawSolidString((const char*)atomLabel.mb_str(wxConvUTF8));
	  glPopMatrix();
	}
    }
}

void MolDisplayWin::SortTransparentTriangles(void) {
	for (int i=0; i<OpenGLData->triangleCount; i++) {
		Rotate3DPt(MainData->TotalRotation, OpenGLData->transpTriList[OpenGLData->transpIndex[i]].v1,
			&(OpenGLData->transpSortVertex[i]));	
	}
	bool done = false;
	long maxcount = OpenGLData->triangleCount-1;
	while (!done) {
		done = true;
		for (int i=0; i<maxcount; i++) {
			if (OpenGLData->transpSortVertex[i].z > OpenGLData->transpSortVertex[i+1].z) {
				CPoint3D temp = OpenGLData->transpSortVertex[i];
				long tempIndex = OpenGLData->transpIndex[i];
				OpenGLData->transpSortVertex[i] = OpenGLData->transpSortVertex[i+1];
				OpenGLData->transpIndex[i] = OpenGLData->transpIndex[i+1];
				OpenGLData->transpSortVertex[i+1] = temp;
				OpenGLData->transpIndex[i+1] = tempIndex;
				done = false;
			}
		}
		maxcount--;
	}
}

void MolDisplayWin::DrawTransparentTriangles(void) {
	glBegin(GL_TRIANGLES);
	for (long itri=0; itri<OpenGLData->triangleCount; itri++) {
		long itriIndex = OpenGLData->transpIndex[itri];

		glColor4f(OpenGLData->transpTriList[itriIndex].r1, OpenGLData->transpTriList[itriIndex].g1,
				  OpenGLData->transpTriList[itriIndex].b1, OpenGLData->transpTriList[itriIndex].a1);
		glNormal3f(OpenGLData->transpTriList[itriIndex].n1.x, 
				   OpenGLData->transpTriList[itriIndex].n1.y, OpenGLData->transpTriList[itriIndex].n1.z);
		glVertex3d(OpenGLData->transpTriList[itriIndex].v1.x, OpenGLData->transpTriList[itriIndex].v1.y,
			OpenGLData->transpTriList[itriIndex].v1.z);
		
		glColor4f(OpenGLData->transpTriList[itriIndex].r2, OpenGLData->transpTriList[itriIndex].g2,
				  OpenGLData->transpTriList[itriIndex].b2, OpenGLData->transpTriList[itriIndex].a2);
		glNormal3f(OpenGLData->transpTriList[itriIndex].n2.x, 
				   OpenGLData->transpTriList[itriIndex].n2.y, OpenGLData->transpTriList[itriIndex].n2.z);
		glVertex3d(OpenGLData->transpTriList[itriIndex].v2.x, OpenGLData->transpTriList[itriIndex].v2.y,
				   OpenGLData->transpTriList[itriIndex].v2.z);

		glColor4f(OpenGLData->transpTriList[itriIndex].r3, OpenGLData->transpTriList[itriIndex].g3,
				  OpenGLData->transpTriList[itriIndex].b3, OpenGLData->transpTriList[itriIndex].a3);
		glNormal3f(OpenGLData->transpTriList[itriIndex].n3.x, 
				   OpenGLData->transpTriList[itriIndex].n3.y, OpenGLData->transpTriList[itriIndex].n3.z);
		glVertex3d(OpenGLData->transpTriList[itriIndex].v3.x, OpenGLData->transpTriList[itriIndex].v3.y,
				   OpenGLData->transpTriList[itriIndex].v3.z);
	}
	glEnd();	//End of triangle creation
}

void MolDisplayWin::DrawMoleculeCoreGL(void)
{
	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	if (!qobj) throw std::bad_alloc();

//	gluQuadricDrawStyle(qobj, GLU_FILL); //or GLU_LINE
	gluQuadricOrientation(qobj, GLU_OUTSIDE);
	gluQuadricNormals(qobj, GLU_SMOOTH); //GLU_FLAT GLU_NONE

	Frame *	lFrame=MainData->cFrame;
	mpAtom * lAtoms = lFrame->Atoms;
	Bond * lBonds = lFrame->Bonds;
	long NumAtoms = lFrame->NumAtoms;
	long NumBonds = lFrame->NumBonds;
	float AtomScale = Prefs->GetAtomScale();
	float Quality = Prefs->GetQD3DAtomQuality();
	GLdouble BondSize = Prefs->GetQD3DBondWidth();

	if (!Prefs->DrawWireFrame()) {
//	if (!Prefs->DrawWireFrame() || Prefs->ColorBondHalves()) {
		for (long iatom=0; iatom<NumAtoms; iatom++) {

			if (lAtoms[iatom].GetInvisibility()) continue;	//Atom is invisible so skip
			long curAtomType = lAtoms[iatom].GetType() - 1;

			float radius;
			if (!Prefs->DrawWireFrame()) radius = AtomScale*Prefs->GetAtomSize(curAtomType);
			else radius = BondSize;
			
			if (radius<0.01) continue;	//skip really small spheres

		//	RGBColor * AtomColor = Prefs->GetAtomColorLoc(curAtomType);
		//		float red, green, blue;
		//	red = AtomColor->red/65536.0;
		//	green = AtomColor->green/65536.0;
		//	blue = AtomColor->blue/65536.0;

			glPushMatrix();
			glTranslatef(lAtoms[iatom].Position.x, 
				     lAtoms[iatom].Position.y,
				     lAtoms[iatom].Position.z);
			
		//	glColor3f(red, green, blue);
			Prefs->ChangeColorAtomColor(curAtomType+1);
			
			if ( mHighliteState && !lAtoms[iatom].GetSelectState())
			  {
			    glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, d_specular);
			    glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, d_shininess);
			    glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, d_diffuse);
			    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, d_ambient);
			  }
			else
			  {
			    glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, l_specular);
			    glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, l_shininess);
			    glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, l_diffuse);
			    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, l_ambient);
			  }

			glLoadName(iatom+1);

			gluSphere(qobj, radius, (long)(1.5*Quality), (long)(Quality));	//Create and draw the sphere

			if ( mHighliteState && !lAtoms[iatom].GetSelectState())
			  {
			    glColor3f(0.0f,0.0f,0.0f);
			    glEnable(GL_POLYGON_STIPPLE);
			    glPolygonStipple(stippleMask);
			    gluSphere(qobj, radius*1.01, (long)(1.5*Quality), (long)(Quality));
			    glDisable(GL_POLYGON_STIPPLE);

			    glColor4f(0.5,0.5,0.5,0.7f);
			    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			    glEnable(GL_BLEND);
			    gluSphere(qobj, radius*1.02, (long)(1.5*Quality), (long)(Quality));
			    glDisable(GL_BLEND);
			  }

			if (show2DPatternMode && lAtoms[iatom].GetType() > 6)
			  {
			    glColor3f(0.0f,0.0f,0.0f);
			    glEnable(GL_POLYGON_STIPPLE);
			    glPolygonStipple(atomMaskPatterns[lAtoms[iatom].GetType()%numPatterns]); //for test
			    gluSphere(qobj, radius*1.01, (long)(1.5*Quality), (long)(Quality));
			    glDisable(GL_POLYGON_STIPPLE);
			  }

			glPopMatrix();
		}
	}

   GLdouble modelview[16];
   GLdouble proj[16];
   GLint viewport[4];
   glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
   glGetDoublev(GL_PROJECTION_MATRIX, proj);
   glGetIntegerv(GL_VIEWPORT, viewport);

		//bonds as cylinders
		//In wireframe mode with bonds colored by atom color we simply scink the atom radius to the bond
		//size and get a nice rounded end cap. If bonds are not colored by atom color then the sphere is
		//skipped and a simple disk closes off the cylinder
	for (long ibond=0; ibond<NumBonds; ibond++) {
			CPoint3D	v1, v2,  offset, NormalOffset, NormEnd, NormStart={0,0,1};
			Matrix4D	rotMat;
		long atom1 = lBonds[ibond].Atom1;
		long atom2 = lBonds[ibond].Atom2;
		glLoadName(ibond+NumAtoms+1);   //bond names start after the last atom
		if ( mHighliteState && !lBonds[ibond].GetSelectState()) {
			glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, d_specular);
			glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, d_shininess);
			glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, d_diffuse);
			glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, d_ambient);
		} else {
			glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, l_specular);
			glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, l_shininess);
			glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, l_diffuse);
			glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT, l_ambient);
		}
		
		BondOrder tmpOrder = lBonds[ibond].Order;
		if (tmpOrder == kHydrogenBond) {
			DrawHydrogenBond(ibond);
			continue;
		}
		if (!Prefs->ColorBondHalves()) tmpOrder = kSingleBond;	//only generate multiple pipes when colored by atom color
		//!!! take hydrogen bond as single bond for now
		GLdouble tmpBondSize = BondSize/MAX(tmpOrder,1);
		if (tmpOrder > 1) tmpBondSize *= 1.5;
		GLdouble baseBondOffset = -1.75 * tmpBondSize * (MAX(tmpOrder,1) - 1);

		if (lAtoms[atom1].GetInvisibility() || lAtoms[atom2].GetInvisibility()) continue;

		//if both atoms are selected, the bond is automatically
      
      GLdouble scr_coords1[3];  // Screen coordinates of atom1
      GLdouble scr_coords2[3];  // Screen coordinates of atom2
      CPoint3D scr_vec;         // Screen space vector between atoms
      GLdouble perp_obj[3];     // Object coords on vector perp. to scr_vec
      CPoint3D offset_vec;      // Direction to shift bond cylinders

      // Find screen coordinates of one atom.
      gluProject(lAtoms[atom1].Position.x,
                 lAtoms[atom1].Position.y,
                 lAtoms[atom1].Position.z,
                 modelview, proj, viewport,
                 &(scr_coords1[0]), &(scr_coords1[1]), &(scr_coords1[2]));

      // Find screen coordinates of other atom.
      gluProject(lAtoms[atom2].Position.x,
                 lAtoms[atom2].Position.y,
                 lAtoms[atom2].Position.z,
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
      offset_vec.x = perp_obj[0] - lAtoms[atom1].Position.x;
      offset_vec.y = perp_obj[1] - lAtoms[atom1].Position.y;
      offset_vec.z = perp_obj[2] - lAtoms[atom1].Position.z;
      offset_vec *= 1 / offset_vec.Magnitude();
      
      // For each "sub-bond" between these two atoms...
		for (int ipipe = 0; ipipe < MAX(tmpOrder,1); ++ipipe) {

			v1.x = lAtoms[atom1].Position.x + offset_vec.x * baseBondOffset +
                3.5 * tmpBondSize * offset_vec.x * ipipe;
			v1.y = lAtoms[atom1].Position.y + offset_vec.y * baseBondOffset +
                3.5 * tmpBondSize * offset_vec.y * ipipe;
			v1.z = lAtoms[atom1].Position.z + offset_vec.z * baseBondOffset +
                3.5 * tmpBondSize * offset_vec.z * ipipe;
			v2.x = lAtoms[atom2].Position.x + offset_vec.x * baseBondOffset +
                3.5 * tmpBondSize * offset_vec.x * ipipe;
			v2.y = lAtoms[atom2].Position.y + offset_vec.y * baseBondOffset +
                3.5 * tmpBondSize * offset_vec.y * ipipe;
			v2.z = lAtoms[atom2].Position.z + offset_vec.z * baseBondOffset +
                3.5 * tmpBondSize * offset_vec.z * ipipe;

			offset.x = v2.x - v1.x;
			offset.y = v2.y - v1.y;
			offset.z = v2.z - v1.z;
			float length = offset.Magnitude();
			if (length>0.00001) {
				NormalOffset.x = offset.x/length;
				NormalOffset.y = offset.y/length;
				NormalOffset.z = offset.z/length;
			} else {
				NormalOffset.x=NormalOffset.y=NormalOffset.z=0.0;
         }

			SetRotationMatrix(rotMat, &NormStart, &NormalOffset);
			rotMat[3][0] = v1.x;
			rotMat[3][1] = v1.y;
			rotMat[3][2] = v1.z;

			glPushMatrix();
			glMultMatrixf((const GLfloat *) &rotMat);

         // We may need to draw two cylinders if the user wants the bonds
         // colored according to their element.
			if (Prefs->ColorBondHalves()) {
					//center the color change at the middle of the visible part of the bond
			  float radius1 = AtomScale*Prefs->GetAtomSize(lAtoms[atom1].GetType() - 1);
			  float radius2 = AtomScale*Prefs->GetAtomSize(lAtoms[atom2].GetType() - 1);
			  float percent1 = radius1/length;
			  float percent2 = radius2/length;
			  float centerPercent = 0.5 + 0.5*(percent1-percent2);
			
			  CPoint3D v3;		//first half bond from atom 1
			  v3.x = centerPercent*(v2.x - v1.x)+v1.x;
			  v3.y = centerPercent*(v2.y - v1.y)+v1.y;
			  v3.z = centerPercent*(v2.z - v1.z)+v1.z;

			  Prefs->ChangeColorAtomColor(lAtoms[atom1].GetType());
	//		  RGBColor * BondColor = Prefs->GetAtomColorLoc(lAtoms[atom1].GetType() - 1);
	//		  float red, green, blue;
	//		  red = BondColor->red/65536.0;
	//		  green = BondColor->green/65536.0;
	//		  blue = BondColor->blue/65536.0;
	//		  glColor3f(red, green, blue);
			  glPushMatrix();
			  gluCylinder(qobj, tmpBondSize, tmpBondSize, length*centerPercent, (long)(Quality), (long)(0.5*Quality));
			  if (Prefs->DrawWireFrame()) {	//Add end caps if no spheres
				  gluSphere(qobj, tmpBondSize, (long)(Quality), (long)(0.5*Quality));	//Create and draw the sphere
			  }
			  glPopMatrix();

			  Prefs->ChangeColorAtomColor(lAtoms[atom2].GetType());
	//		  BondColor = Prefs->GetAtomColorLoc(lAtoms[atom2].GetType() - 1);
	//		  red = BondColor->red/65536.0;
	//		  green = BondColor->green/65536.0;
	//		  blue = BondColor->blue/65536.0;
	//		  glColor3f(red, green, blue);
			  glPopMatrix();
			  glPushMatrix();
			  rotMat[3][0] = v3.x;
			  rotMat[3][1] = v3.y;
			  rotMat[3][2] = v3.z;
			  glMultMatrixf((const GLfloat *) &rotMat);
			  gluCylinder(qobj, tmpBondSize, tmpBondSize, length*(1-centerPercent), (long)(Quality), (long)(0.5*Quality));
			  if (Prefs->DrawWireFrame()) {	//Add end caps if no spheres
					glPopMatrix();
					glPushMatrix();
					rotMat[3][0] = v2.x;
					rotMat[3][1] = v2.y;
					rotMat[3][2] = v2.z;
					glMultMatrixf((const GLfloat *) &rotMat);
					// glTranslatef(0.0, baseBondOffset+ipipe*3.5*tmpBondSize, 0.0);
					gluSphere(qobj, tmpBondSize, (long)(Quality), (long)(0.5*Quality));	//Create and draw the sphere
			  }
			  glPopMatrix();
		//		if (Prefs->DrawWireFrame()) {	//Add end caps if no spheres
		//			glPopMatrix();
		//			glPushMatrix();
		//			rotMat[3][0] = v2.x;
		//			rotMat[3][1] = v2.y;
		//			rotMat[3][2] = v2.z;
		//			glMultMatrixf((const GLfloat *) &rotMat);
		//			gluDisk(qobj, 0.0, BondSize, (long)(Quality), 2);
		//		}
			}
         
         // We only need to draw one cylinder the whole length of a bond since
         // the user's not interested in coloring each half differently.
         else {
            Prefs->ChangeColorBondColor(lBonds[ibond].Order);
            for (int i = 0; i < MAX(tmpOrder,1); ++i) {
              glPushMatrix();
              gluCylinder(qobj, tmpBondSize, tmpBondSize, length, 
                          (long) Quality, (long) (0.5f * Quality));
              glPopMatrix();
            }

            if (Prefs->DrawWireFrame()) { //Add end caps if no spheres
              gluDisk(qobj, 0.0, tmpBondSize, (long) Quality, 2);
              glPopMatrix();
              glPushMatrix();
              rotMat[3][0] = v2.x;
              rotMat[3][1] = v2.y;
              rotMat[3][2] = v2.z;
              glMultMatrixf((const GLfloat *) &rotMat);
              gluDisk(qobj, 0.0, tmpBondSize, (long) Quality, 2);
            }
         }

         // Now, if a bond is selected but not this one, we need to draw an
         // encapsulating cylinder to mask it out.
			if (mHighliteState && !lBonds[ibond].GetSelectState()) {
				glPopMatrix();
				glPushMatrix();

				rotMat[3][0] = v1.x;
				rotMat[3][1] = v1.y;
				rotMat[3][2] = v1.z;
				glMultMatrixf((const GLfloat *) &rotMat);

            // Display stippled cylinder and spheres slightly larger than bond
            // cylinder and spheres.
				glColor3f(0.0f, 0.0f, 0.0f);
				glEnable(GL_POLYGON_STIPPLE);
				glPolygonStipple(stippleMask);
				gluCylinder(qobj, tmpBondSize * 1.01f, tmpBondSize * 1.01f,
                        length, (long) Quality, (long) (0.5f * Quality));
				if (Prefs->DrawWireFrame()) {	//Add end caps if no spheres
					gluSphere(qobj, tmpBondSize * 1.01f, (long) Quality,
                         (long) (0.5f * Quality));
               glPushMatrix();
               glTranslatef(0.0f, 0.0f, length);
					gluSphere(qobj, tmpBondSize * 1.01f, (long) Quality,
                         (long) (0.5f * Quality));
               glPopMatrix();
				}
				glDisable(GL_POLYGON_STIPPLE);

            // Display semi-transparent and non-stippled cylinder and spheres
            // slightly larger than the bond and stippled cylinder and spheres.
				glColor4f(0.5f, 0.5f, 0.5f, 0.7f);
				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
            gluCylinder(qobj, tmpBondSize * 1.02f, tmpBondSize * 1.02f,
                        length, (long) Quality, (long) (0.5 * Quality));
				if (Prefs->DrawWireFrame()) {	//Add end caps if no spheres
					gluSphere(qobj, tmpBondSize * 1.02f, (long) Quality,
                         (long) (0.5 * Quality));
               glTranslatef(0.0f, 0.0f, length);
					gluSphere(qobj, tmpBondSize * 1.02f, (long) Quality,
                         (long) (0.5 * Quality));
				}
				glDisable(GL_BLEND);

			}
			glPopMatrix();
		}
	}

	if ( mHighliteState )
	  glEnable(GL_POLYGON_STIPPLE);

	glLoadName(0);  //only atoms and bonds are selectable
	                 //so give a NULL name value to the rest of the geometries

	if (MainData->GetDrawMode() && lFrame->Vibs) {	//Add the current normal mode, if active
			float	VectorScale=Prefs->GetVectorScale();

		long cmode = (lFrame->NumAtoms)*(lFrame->Vibs->CurrentMode);

		Prefs->ChangeColorVectorColor();

		CPoint3D NormStart={0,0,1};
		for (long iatom=0; iatom<NumAtoms; iatom++) {

			if (lAtoms[iatom].GetInvisibility()) continue;	//Atom is invisible so skip
			CPoint3D NMode = (lFrame->Vibs->NormMode[iatom + cmode]);
			NMode *= VectorScale;
			float length = NMode.Magnitude();
			if (length > 0.1) {
					CPoint3D NModeVector;
					Matrix4D	rotMat;
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
	//			float HeadRatio = (length-HeadRadius)/length;
				GLfloat ShaftLength = length - HeadRadius;

				SetRotationMatrix(rotMat, &NormStart, &NModeVector);
				rotMat[3][0] = VStart.x;
				rotMat[3][1] = VStart.y;
				rotMat[3][2] = VStart.z;
				glPushMatrix();
				glMultMatrixf((const GLfloat *) &rotMat);

				gluCylinder(qobj, VectorWidth, VectorWidth, ShaftLength, (long)(Quality), (long)(0.5*Quality));
				glPopMatrix();
				rotMat[3][0] = VStart.x + NModeVector.x * ShaftLength;
				rotMat[3][1] = VStart.y + NModeVector.y * ShaftLength;
				rotMat[3][2] = VStart.z + NModeVector.z * ShaftLength;
				glPushMatrix();
				glMultMatrixf((const GLfloat *) &rotMat);
				gluDisk(qobj, 0.0, 2*VectorWidth, (long)(Quality), 2);
				gluCylinder(qobj, 2*VectorWidth, 0.0, HeadRadius, (long)(Quality), 3);
				glPopMatrix();
			}
		}
	}
	gluDeleteQuadric(qobj);	//finally delete the quadric object

/*	//draw bonds as lines
	glDisable(GL_LIGHTING);
//	glEnable(GL_LINE_SMOOTH);
	GLfloat BondSize = 100*Prefs->GetQD3DBondWidth() *
							MainData->MaxSize / (0.5*MainData->WindowSize);
	glLineWidth(BondSize);
	glBegin(GL_LINES);
		for (long ibond=0; ibond<NumBonds; ibond++) {
				CPoint3D	v1, v2;
			long atom1 = lBonds[ibond].Atom1;
			long atom2 = lBonds[ibond].Atom2;
			if (lAtoms[atom1].GetInvisibility() || lAtoms[atom2].GetInvisibility()) continue;

			v1.x = lAtoms[atom1].Position.x;
			v1.y = lAtoms[atom1].Position.y;
			v1.z = lAtoms[atom1].Position.z;
			v2.x = lAtoms[atom2].Position.x;
			v2.y = lAtoms[atom2].Position.y;
			v2.z = lAtoms[atom2].Position.z;

			if (Prefs->ColorBondHalves()) {	//first half bond from atom 1
		//			CPoint3D v3;
		//		v3.x = 0.5*(v2.x - v1.x)+v1.x;
		//		v3.y = 0.5*(v2.y - v1.y)+v1.y;
		//		v3.z = 0.5*(v2.z - v1.z)+v1.z;

				RGBColor * BondColor = Prefs->GetAtomColorLoc(lAtoms[atom1].GetType() - 1);
					float red, green, blue;
				red = BondColor->red/65536.0;
				green = BondColor->green/65536.0;
				blue = BondColor->blue/65536.0;
				glColor3f(red, green, blue);
				glVertex3d(v1.x, v1.y, v1.z);
		//		glVertex3d(v3.x, v3.y, v3.z);

				BondColor = Prefs->GetAtomColorLoc(lAtoms[atom2].GetType() - 1);
				red = BondColor->red/65536.0;
				green = BondColor->green/65536.0;
				blue = BondColor->blue/65536.0;
				glColor3f(red, green, blue);
		//		glVertex3d(v3.x, v3.y, v3.z);
				glVertex3d(v2.x, v2.y, v2.z);
			} else {
				RGBColor * BondColor = Prefs->GetBondColorLoc(lBonds[ibond].Order);
					float red, green, blue;
				red = BondColor->red/65536.0;
				green = BondColor->green/65536.0;
				blue = BondColor->blue/65536.0;
				glColor3f(red, green, blue);
				glVertex3d(v1.x, v1.y, v1.z);
				glVertex3d(v2.x, v2.y, v2.z);
			}
		}
	glEnd();*/

	glDisable(GL_POLYGON_STIPPLE);  //make sure everything outside 
	                                //this function has no stipple effect
}
void WinPrefs::ChangeColorBondColor(long order) const {
	float red, green, blue;
	red = BondColors[order].red/65536.0;
	green = BondColors[order].green/65536.0;
	blue = BondColors[order].blue/65536.0;
	glColor3f(red, green, blue);
}
void WinPrefs::ChangeColorAtomColor(long atomtype) const {
	float red, green, blue;
	red = AtomColors[atomtype-1].red/65536.0;
	green = AtomColors[atomtype-1].green/65536.0;
	blue = AtomColors[atomtype-1].blue/65536.0;
	glColor3f(red, green, blue);
}
void WinPrefs::ChangeColorVectorColor(void) const {
	float red, green, blue;
	red = VectorColor.red/65536.0;
	green = VectorColor.green/65536.0;
	blue = VectorColor.blue/65536.0;
	glColor3f(red, green, blue);
}

void MolDisplayWin::DrawHydrogenBond(long bondNum) {
	CPoint3D	v1, v2, offset;

	Prefs->ChangeColorBondColor(kHydrogenBond);
	GLdouble BondSize = Prefs->GetQD3DBondWidth() * 0.5;
	float Quality = Prefs->GetQD3DAtomQuality();
	Frame *	lFrame=MainData->cFrame;
	long atom1 = lFrame->Bonds[bondNum].Atom1;
	long atom2 = lFrame->Bonds[bondNum].Atom2;
	v1.x = lFrame->Atoms[atom1].Position.x;
	v1.y = lFrame->Atoms[atom1].Position.y;
	v1.z = lFrame->Atoms[atom1].Position.z;
	v2.x = lFrame->Atoms[atom2].Position.x;
	v2.y = lFrame->Atoms[atom2].Position.y;
	v2.z = lFrame->Atoms[atom2].Position.z;
	offset.x = v2.x - v1.x;
	offset.y = v2.y - v1.y;
	offset.z = v2.z - v1.z;
	
	//GL lines don't scale with distance so they aren't a good solution
//	glLineWidth(10.0);
//	glBegin(GL_LINES);
//	glVertex3d(v1.x, v1.y, v1.z);
//	glVertex3d(v2.x, v2.y, v2.z);
//	glEnd();
	//Plot as a series of spheres
	CPoint3D	NormalOffset, NormEnd, NormStart={0,0,1};
	Matrix4D	rotMat;
	
	float length = offset.Magnitude();
	if (length>0.00001) {
		NormalOffset.x = offset.x/length;
		NormalOffset.y = offset.y/length;
		NormalOffset.z = offset.z/length;
	} else
		NormalOffset.x=NormalOffset.y=NormalOffset.z=0.0;
	NormEnd = v2;
	Normalize3D (&NormEnd);
	SetRotationMatrix(rotMat, &NormStart, &NormalOffset);
	rotMat[3][0] = v1.x;
	rotMat[3][1] = v1.y;
	rotMat[3][2] = v1.z;
	
	glPushMatrix();
	glMultMatrixf((const GLfloat *) &rotMat);
	GLUquadricObj * qobj = NULL;
	qobj = gluNewQuadric();
	if (!qobj) throw std::bad_alloc();
	float pos=0.75*BondSize;
	glTranslatef(0.0, 0.0, pos);
	while (pos < length) {
		gluSphere(qobj, BondSize, (long)(Quality), (long)(0.5*Quality));	//Create and draw the sphere
		if ( mHighliteState && !lFrame->Bonds[bondNum].GetSelectState()) {
			glColor3f(0.0f,0.0f,0.0f);
			glEnable(GL_POLYGON_STIPPLE);
			glPolygonStipple(stippleMask);
			gluSphere(qobj, BondSize*1.01, (long)(Quality), (long)(0.5*Quality));
			glDisable(GL_POLYGON_STIPPLE);
			
			glColor4f(0.5,0.5,0.5,0.7f);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			gluSphere(qobj, BondSize*1.02, (long)(Quality), (long)(0.5*Quality));
			glDisable(GL_BLEND);
		}
		glTranslatef(0.0, 0.0, 2.5*BondSize);
		pos += 2.5*BondSize;
	}
	glPopMatrix();
	if (qobj) gluDeleteQuadric(qobj);	//finally delete the quadric object
}

void MolDisplayWin::AddAxisGL(void)
{
	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	if (!qobj) throw std::bad_alloc();
	glColor3f(0.0, 0.0, 0.0);
	long Quality = (long)(Prefs->GetQD3DAtomQuality());
	float VectorWidth = 0.05;
	float HeadRadius = 2*VectorWidth;

	CPoint3D			vector = {1.0,0.0,0.0}, NormStart={0.0,0.0,1.0};
	glPushMatrix();
	
		// Z-axis
	glTranslatef(0.0, 0.0, -MainData->MaxSize);
	gluDisk(qobj, 0.0, VectorWidth, (long)(Quality), 2);
	gluCylinder(qobj, 0.05, 0.05, (2*MainData->MaxSize - 2*VectorWidth), Quality, (long)(0.5*Quality));
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, (MainData->MaxSize - 2*VectorWidth));
	gluDisk(qobj, 0.0, 2*VectorWidth, Quality, 2);
	gluCylinder(qobj, 2*VectorWidth, 0.0, HeadRadius, Quality, 3);
	glPopMatrix();
		// X-axis
		Matrix4D	rotMat;
	SetRotationMatrix(rotMat, &NormStart, &vector);
	rotMat[3][0] = -MainData->MaxSize;
	glPushMatrix();
	glMultMatrixf((const GLfloat *) &rotMat);
	gluDisk(qobj, 0.0, VectorWidth, Quality, 2);
	gluCylinder(qobj, 0.05, 0.05, (2*MainData->MaxSize - 2*VectorWidth), Quality, (long)(0.5*Quality));
	glPopMatrix();
	glPushMatrix();
	rotMat[3][0] = MainData->MaxSize - 2*VectorWidth;
	glMultMatrixf((const GLfloat *) &rotMat);
	gluDisk(qobj, 0.0, 2*VectorWidth, Quality, 2);
	gluCylinder(qobj, 2*VectorWidth, 0.0, HeadRadius, Quality, 3);
	glPopMatrix();
		// Y-axis
	vector.x = 0.0;
	vector.y = 1.0;
	SetRotationMatrix(rotMat, &NormStart, &vector);
	rotMat[3][1] = -MainData->MaxSize;
	glPushMatrix();
	glMultMatrixf((const GLfloat *) &rotMat);
	gluDisk(qobj, 0.0, VectorWidth, Quality, 2);
	gluCylinder(qobj, 0.05, 0.05, (2*MainData->MaxSize - 2*VectorWidth), Quality, (long)(0.5*Quality));
	glPopMatrix();
	glPushMatrix();
	rotMat[3][1] = MainData->MaxSize - 2*VectorWidth;
	glMultMatrixf((const GLfloat *) &rotMat);
	gluDisk(qobj, 0.0, 2*VectorWidth, Quality, 2);
	gluCylinder(qobj, 2*VectorWidth, 0.0, HeadRadius, Quality, 3);

	glPopMatrix();
	gluDeleteQuadric(qobj);	//finally delete the quadric object
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
	CPoint3D origin={0,0,0}, p1={0,0,0}, p2={0,0,0};
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
		//	break;	//let this rool through to pick up the axis
		case GAMESS_CN:
		{
			int order = MainData->InputOptions->Data->GetPointGroupOrder();
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
				//There are order count of planes bisecting the 2 fold axis
			for (int i=0; i<order; i++) {
				origin.x = - sin(kPi * i/order) * MainData->MaxSize;
				origin.y = - cos(kPi * i/order) * MainData->MaxSize;
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
		case GAMESS_TD:
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
	}
}
long Surf2DBase::Draw3DGL(MoleculeData * MainData, WinPrefs * Prefs, myGLTriangle *)
{
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
#ifdef UseHandles
	HLock(Grid);
	float * lGrid = (float *) *Grid;
#else
	float * lGrid = Grid;
#endif
	
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
	GLenum error = glGetError();
	if (UseLines) {
		glDisable(GL_LIGHTING);
		glLineWidth(1);
		glBegin(GL_LINES);
		error = glGetError();	//maybe clear off odd errors...
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
		error = glGetError();	//This is here to clear off odd errors.
		glEnable(GL_LIGHTING);
	} else {
		if (qobj) gluDeleteQuadric(qobj);	//finally delete the quadric object
	}
#ifdef UseHandles
	HUnlock(Grid);
#endif
}

long General3DSurface::getTriangleCount(void) const {
	long result = NumPosContourTriangles;
	if (Mode & 4) result += NumNegContourTriangles;
	return result;
}

long General3DSurface::Draw3DGL(MoleculeData * MainData, WinPrefs *, myGLTriangle * transpTri) {
	long result=0;
	if (Visible) {
#ifdef UseHandles
		if (ContourHndl && VertexHndl) {
			HLock(ContourHndl);
			HLock(VertexHndl);
			if (SurfaceNormals) HLock(SurfaceNormals);
			if (SolidSurface()) {
				if (UseSurfaceNormals()&&SurfaceNormals) {
					result = CreateSolidSurface((CPoint3D *) *ContourHndl, (CPoint3D *) *SurfaceNormals, (long *) *VertexHndl,
						NumPosContourTriangles, &PosColor, NULL, NULL, 1.0, MainData, transpTri);
					if ((Mode & 4)&&(NumNegContourTriangles > 0))
						result += CreateSolidSurface((CPoint3D *) *ContourHndl,
							(CPoint3D *) *SurfaceNormals, (long *) &(((long *) *VertexHndl)[3*NumPosContourTriangles]),
							NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData, &(transpTri[result]));
				} else {
					result = CreateSolidSurface((CPoint3D *) *ContourHndl, NULL, (long *) *VertexHndl,
						NumPosContourTriangles, &PosColor, NULL, NULL, 1.0, MainData, transpTri);
					if ((Mode & 4)&&(NumNegContourTriangles > 0))
						result += CreateSolidSurface((CPoint3D *) *ContourHndl,
							NULL, (long *) &(((long *) *VertexHndl)[3*NumPosContourTriangles]),
							NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData, &(transpTri[result]));
				}
			} else if (WireFrameSurface()) {
				CreateWireSurface((CPoint3D *) *ContourHndl, NULL, (long *) *VertexHndl,
					NumPosContourTriangles, &PosColor, NULL, NULL, 1.0, MainData);
				if (ContourBothPosNeg()&&(NumNegContourTriangles > 0))
					CreateWireSurface((CPoint3D *) *ContourHndl, NULL, 
						(long *) &(((long *) *VertexHndl)[3*NumPosContourTriangles]),
						NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData);
			}
			HUnlock(ContourHndl);
			HUnlock(VertexHndl);
			if (SurfaceNormals) HUnlock(SurfaceNormals);
		}
#else
		if (ContourHndl && VertexList) {
			if (SolidSurface()) {
				if (UseSurfaceNormals()&&SurfaceNormals) {
					result = CreateSolidSurface(ContourHndl, SurfaceNormals, VertexList,
									NumPosContourTriangles, &PosColor, NULL, NULL, 1.0, MainData, transpTri);
					if ((Mode & 4)&&(NumNegContourTriangles > 0))
						result += CreateSolidSurface(ContourHndl,
													 SurfaceNormals, &(VertexList[3*NumPosContourTriangles]),
													 NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData, &(transpTri[result]));
				} else {
					result = CreateSolidSurface(ContourHndl, NULL, VertexList,
												NumPosContourTriangles, &PosColor, NULL, NULL, 1.0, MainData, transpTri);
					if ((Mode & 4)&&(NumNegContourTriangles > 0))
						result += CreateSolidSurface(ContourHndl,
													 NULL, &(VertexList[3*NumPosContourTriangles]),
													 NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData, &(transpTri[result]));
				}
			} else if (WireFrameSurface()) {
				CreateWireSurface(ContourHndl, NULL, VertexList,
								  NumPosContourTriangles, &PosColor, NULL, NULL, 1.0, MainData);
				if (ContourBothPosNeg()&&(NumNegContourTriangles > 0))
					CreateWireSurface(ContourHndl, NULL, 
									  &(VertexList[3*NumPosContourTriangles]),
									  NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData);
			}
		}
#endif
	}
	return result;
}
long TEDensity3DSurface::Draw3DGL(MoleculeData * MainData, WinPrefs * , myGLTriangle * transpTri) {
	long result = 0;
	if (Visible) {
#ifdef UseHandles
		if (ContourHndl && VertexHndl) {
			HLock(ContourHndl);
			HLock(VertexHndl);
			if (SurfaceNormals) HLock(SurfaceNormals);
			if (List) HLock(List);
			if (SolidSurface()) {
				if ((UseSurfaceNormals())&&SurfaceNormals) {
					result = CreateSolidSurface((CPoint3D *) *ContourHndl, (CPoint3D *) *SurfaceNormals, (long *) *VertexHndl,
						NumPosContourTriangles,
						&PosColor, (List ?(float *) *List:NULL), &NegColor, MaxMEPValue, MainData, transpTri);
				} else {
					result = CreateSolidSurface((CPoint3D *) *ContourHndl, NULL, (long *) *VertexHndl,
						NumPosContourTriangles,
						&PosColor, (List ?(float *) *List:NULL), &NegColor, MaxMEPValue, MainData, transpTri);
				}
			} else if (WireFrameSurface()) {
				CreateWireSurface((CPoint3D *) *ContourHndl, NULL, (long *) *VertexHndl,
					NumPosContourTriangles,
					&PosColor,(List ?(float *) *List:NULL), &NegColor, MaxMEPValue, MainData);
			}
			HUnlock(ContourHndl);
			HUnlock(VertexHndl);
			if (List) HUnlock(List);
			if (SurfaceNormals) HUnlock(SurfaceNormals);
#else
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
									  &PosColor, List, &NegColor, MaxMEPValue, MainData);
				}
#endif
		}
	}
	return result;
}
long Orb3DSurface::Draw3DGL(MoleculeData * MainData, WinPrefs * , myGLTriangle * transpTri) {
	long result=0;
	if (Visible && (PlotOrb>=0)) {
#ifdef UseHandles
		if (ContourHndl && VertexHndl) {
			HLock(ContourHndl);
			HLock(VertexHndl);
			if (SurfaceNormals) HLock(SurfaceNormals);
			if (SolidSurface()) {
				if ((UseSurfaceNormals())&&SurfaceNormals) {
					result = CreateSolidSurface((CPoint3D *) *ContourHndl,
												(CPoint3D *) *SurfaceNormals, (long *) *VertexHndl, NumPosContourTriangles,
												&PosColor, NULL, NULL, 1.0, MainData, transpTri);
					result += CreateSolidSurface((CPoint3D *) *ContourHndl,
												 (CPoint3D *) *SurfaceNormals,
												 (long *) &(((long *) *VertexHndl)[3*NumPosContourTriangles]),
												 NumNegContourTriangles, &NegColor,
												 NULL, NULL, 1.0, MainData, &(transpTri[result]));
				} else {
					result = CreateSolidSurface((CPoint3D *) *ContourHndl, NULL, (long *) *VertexHndl,
												NumPosContourTriangles,
												&PosColor, NULL, NULL, 1.0, MainData, transpTri);
					result += CreateSolidSurface((CPoint3D *) *ContourHndl,
												 NULL, (long *) &(((long *) *VertexHndl)[3*NumPosContourTriangles]),
												 NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData, &(transpTri[result]));
				}
			} else if (WireFrameSurface()) {
				CreateWireSurface((CPoint3D *) *ContourHndl, NULL, (long *) *VertexHndl, NumPosContourTriangles,
								  &PosColor, NULL, NULL, 1.0, MainData);
				CreateWireSurface((CPoint3D *) *ContourHndl, NULL, 
								  (long *) &(((long *) *VertexHndl)[3*NumPosContourTriangles]),
								  NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData);
			}
			HUnlock(ContourHndl);
			HUnlock(VertexHndl);
			if (SurfaceNormals) HUnlock(SurfaceNormals);
		}
#else
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
								  &PosColor, NULL, NULL, 1.0, MainData);
				CreateWireSurface(ContourHndl, NULL, 
								  &(VertexList[3*NumPosContourTriangles]),
								  NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData);
			}
		}
#endif
	}
	return result;
}
long MEP3DSurface::Draw3DGL(MoleculeData * MainData, WinPrefs * , myGLTriangle * transpTri) {
	long result=0;
	if (Visible) {
#ifdef UseHandles
		if (ContourHndl && VertexHndl) {
			HLock(ContourHndl);
			HLock(VertexHndl);
			if (SurfaceNormals) HLock(SurfaceNormals);
			if (SolidSurface()) {
				if ((UseSurfaceNormals())&&SurfaceNormals) {
					result = CreateSolidSurface((CPoint3D *) *ContourHndl, (CPoint3D *) *SurfaceNormals,
						(long *) *VertexHndl, NumPosContourTriangles,
						&PosColor, NULL, NULL, 1.0, MainData, transpTri);
					result += CreateSolidSurface((CPoint3D *) *ContourHndl, (CPoint3D *) *SurfaceNormals,
						(long *) &(((long *) *VertexHndl)[3*NumPosContourTriangles]), 
						NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData, &(transpTri[result]));
				} else {
					result = CreateSolidSurface((CPoint3D *) *ContourHndl, NULL, (long *) *VertexHndl, NumPosContourTriangles,
						&PosColor, NULL, NULL, 1.0, MainData, transpTri);
					result += CreateSolidSurface((CPoint3D *) *ContourHndl,
						NULL, (long *) &(((long *) *VertexHndl)[3*NumPosContourTriangles]),
						NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData, &(transpTri[result]));
				}
			} else if (WireFrameSurface()) {
				CreateWireSurface((CPoint3D *) *ContourHndl, NULL, (long *) *VertexHndl, NumPosContourTriangles,
					&PosColor, NULL, NULL, 1.0, MainData);
				CreateWireSurface((CPoint3D *) *ContourHndl, NULL, 
					&(((long *) *VertexHndl)[3*NumPosContourTriangles]),
					NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData);
			}
			HUnlock(ContourHndl);
			HUnlock(VertexHndl);
			if (SurfaceNormals) HUnlock(SurfaceNormals);
		}
#else
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
								  &PosColor, NULL, NULL, 1.0, MainData);
				CreateWireSurface(ContourHndl, NULL, 
								  &(VertexList[3*NumPosContourTriangles]),
								  NumNegContourTriangles, &NegColor, NULL, NULL, 1.0, MainData);
			}
		}
#endif
	}
	return result;
}

void Surf3DBase::CreateWireSurface(CPoint3D * Vertices, CPoint3D * Normals, long * VertexList,
		long NumTriangles, RGBColor * SurfaceColor, float * SurfaceValue,
		RGBColor * NColor, float MaxSurfaceValue, MoleculeData * MainData)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_LIGHTING);
	glLineWidth(1);
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
		alpha = (((float) TranspColor.red/65536.0) + ((float) TranspColor.green/65536.0) + 
					((float) TranspColor.blue/65536.0))/3.0;
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
void CreateCylinderFromLine(GLUquadricObj * qobj, const CPoint3D & lineStart, const CPoint3D & lineEnd, const float & lineWidth) {
	if (qobj == NULL) return;
	CPoint3D	offset, NormalOffset, NormEnd, NormStart={0,0,1};
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
	Normalize3D (&NormEnd);
	SetRotationMatrix(rotMat, &NormStart, &NormalOffset);
	rotMat[3][0] = lineStart.x;
	rotMat[3][1] = lineStart.y;
	rotMat[3][2] = lineStart.z;
	
	glPushMatrix();
	glMultMatrixf((const GLfloat *) &rotMat);
	gluCylinder(qobj, lineWidth, lineWidth, length, 4, 1);
	glPopMatrix();
}
void DrawRotationAxis(const CPoint3D & lineStart, const CPoint3D & lineEnd, const int & order) {
	float plane_emissive[] = { 0.0, 0.3, 0.7, 0.2 };
	float plane_diffuse[] = { 0.0, 0.3, 0.6, 0.3 };
	float plane_specular[] = { 0.0, 0.3, 0.6, 1.0 };
	float save_emissive[4], save_diffuse[4], save_specular[4];
	
	glGetMaterialfv(GL_FRONT, GL_DIFFUSE, save_diffuse);
	glGetMaterialfv(GL_FRONT, GL_EMISSION, save_emissive);
	glGetMaterialfv(GL_FRONT, GL_SPECULAR, save_specular);

	int imageWidth =16;
	//Our width needs to be a power of two. So orders 1, 2 and 4 are no problem. Other orders such as 3
	//must be padded out to the next higher power of two.
	int repeat = order;
	if (order == 3) repeat = 4;
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
			if (order == 3) {
				for (int t=0; t<5; t++) {
					testimage[p] = 0;
					testimage[p + 1] = 0;
					testimage[p + 2] = 0;
					testimage[p + 3] = 178;
					p+=4;
				}
			}
		}
		if (order == 3) {
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
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plane_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, plane_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plane_specular);
	glColor4f(0, .64, .85, 0.7);

	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	
	GLUquadricObj * qobj = NULL;
	qobj = gluNewQuadric();

	CPoint3D	offset, NormalOffset, NormEnd, NormStart={0,0,1};
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
	Normalize3D (&NormEnd);
	SetRotationMatrix(rotMat, &NormStart, &NormalOffset);
	rotMat[3][0] = lineStart.x;
	rotMat[3][1] = lineStart.y;
	rotMat[3][2] = lineStart.z;
	
	glPushMatrix();
	glMultMatrixf((const GLfloat *) &rotMat);
	gluQuadricTexture(qobj, GL_TRUE);
	gluCylinder(qobj, 0.1, 0.1, 0.2, 12, 1);
	glTranslatef(0, 0, 0.2);
	gluQuadricTexture(qobj, GL_FALSE);
	gluCylinder(qobj, 0.1, 0.1, length-0.4, 12, 1);
	glTranslatef(0, 0, length-0.4);
	gluQuadricTexture(qobj, GL_TRUE);
	gluCylinder(qobj, 0.1, 0.1, 0.2, 12, 1);
	glPopMatrix();
	
	if (qobj) gluDeleteQuadric(qobj);	//finally delete the quadric object
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDeleteTextures(1, &texname);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, save_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, save_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, save_specular);
}

//Draw a transluecent plane
void DrawTranslucentPlane(const CPoint3D & origin, const CPoint3D & p1, const CPoint3D & p2) {
	float plane_emissive[] = { 0.0, 0.3, 0.7, 0.2 };
	float plane_diffuse[] = { 0.0, 0.3, 0.6, 0.3 };
	float plane_specular[] = { 0.0, 0.3, 0.6, 1.0 };
	float save_emissive[4], save_diffuse[4], save_specular[4];
	Matrix4D rotationMatrix;

	glGetMaterialfv(GL_FRONT, GL_DIFFUSE, save_diffuse);
	glGetMaterialfv(GL_FRONT, GL_EMISSION, save_emissive);
	glGetMaterialfv(GL_FRONT, GL_SPECULAR, save_specular);

	CPoint3D vec1 = p1 - origin;
	CPoint3D vec2 = p2 - origin;
	float s1Length = vec1.Magnitude();
	float s2Length = vec2.Magnitude();
	SetPlaneRotation(rotationMatrix, vec1, vec2);

	glPushMatrix();
	glTranslatef(origin.x, origin.y, origin.z);
	glMultMatrixf((const GLfloat *) &rotationMatrix);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plane_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, plane_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plane_specular);
	glColor4f(0, .64, .85, 0.3);
   
	glRectf(0, 0, s1Length, s2Length);

	glDisable(GL_BLEND);

	glDisable(GL_LIGHTING);
	glColor4f(0, .64, .85, 1);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1);
	glRectf(0, 0, s1Length, s2Length);
	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDepthMask(GL_TRUE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, save_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, save_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, save_specular);
	
	glPopMatrix();
}
void DrawInversionPoint(void) {
	GLUquadricObj * qobj = NULL;
	qobj = gluNewQuadric();
	float plane_emissive[] = { 0.0, 0.3, 0.7, 0.2 };
	float plane_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float plane_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float save_emissive[4], save_diffuse[4], save_specular[4], shininess;
	
	glGetMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
	glGetMaterialfv(GL_FRONT, GL_DIFFUSE, save_diffuse);
	glGetMaterialfv(GL_FRONT, GL_EMISSION, save_emissive);
	glGetMaterialfv(GL_FRONT, GL_SPECULAR, save_specular);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, plane_diffuse);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, plane_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, plane_specular);
	glColor4f(.7, .7, .7, 1.0);

	//Assume the inversion point is at the origin
	gluSphere(qobj, 0.2, 30, 20);	//Create and draw the sphere
	
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, save_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, save_emissive);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, save_specular);
	if (qobj) gluDeleteQuadric(qobj);	//finally delete the quadric object
}
