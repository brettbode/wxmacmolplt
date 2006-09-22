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

extern int glf_initialized;

int defAttribs[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16,
                    0};

MpGLCanvas::MpGLCanvas(MolDisplayWin  *parent,
                       wxWindowID      id,
                       const wxPoint  &position,
                       const wxSize   &size,
                       long            style,
                       const wxString &name)
           :wxGLCanvas((wxWindow *)parent, id, position, size,
                       style|wxFULL_REPAINT_ON_RESIZE, name, defAttribs) {
        //There is an additional parameter to wxGLCanvas that is an int
        //array of GL options we might need. Might need to use WX_GL_RGBA and WX_GL_DOUBLEBUFFER?
    Prefs = NULL;
    MolWin = parent;
    initialized = false;

    mMainData = parent->GetData();

    mSelectState = -1;
    interactiveMode = false;
    oldSelect = -1;
    //Hmm is this the right spot to initialize our GL settings?
//  initGL();
}

MpGLCanvas::~MpGLCanvas()
{
  if (glf_initialized)
    {
      glfClose();
      glf_initialized = 0;
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
	if (!glf_initialized)
	  {
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
	    if (glfLoadFont(pathname.mb_str(wxConvUTF8)) < 0)
	      {
		std::cout<<"Warning: font file not found!"<<std::endl;
		glfClose();
	      }
	    else
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
#define myGLperspective 0.050   //0.050 seems to match my 2D mode

void MpGLCanvas::GenerateHiResImage(wxDC * dc, const float & ScaleFactor, Progress * progress,
                                    bool Center, bool frame) {
    
    if(!GetContext()) {
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
    
    unsigned char *pixels = (unsigned char *) malloc(3 * width * height * sizeof(GLbyte));
    glReadBuffer(GL_BACK);
    
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
            glFrustum(left, right, bottom, top, 0.1, 100.0);
            
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

    GLint   glViewport[4];
    GLint   canvasWidth  = 0;
    GLint   canvasHeight = 0;

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


    if(!GetContext()) {
        return;
    }
    SetCurrent();

    glGetIntegerv(GL_VIEWPORT, glViewport);
    canvasWidth  = glViewport[2];
    canvasHeight = glViewport[3];

    dc->GetSize(&dcWidth, &dcHeight);

    numPassesX = dcWidth / canvasWidth;
    if(dcWidth % canvasWidth) numPassesX++;

    numPassesY = dcHeight / canvasHeight;
    if(dcHeight % canvasHeight) numPassesY++;

    edgePassWidth  = (canvasWidth  * numPassesX) - dcWidth;
    edgePassHeight = (canvasHeight * numPassesY) - dcHeight;

    pixels = (GLbyte *)malloc(canvasWidth * canvasHeight * 3 * sizeof(GLbyte));

    glReadBuffer(GL_BACK);

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
            glFrustum(left, right, bottom, top, 0.1, 100.0);
            
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
            dc->DrawBitmap(wxBitmap(wxImage(canvasWidth,
                                            canvasHeight,
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
        //      gluPerspective(ysize, aspect, 0.1, 100.0);
        GLdouble top, right;
        if (aspect > 1.0) {
            right = myGLperspective;
            top = right/aspect;
        } else {
            top = myGLperspective;
            right = top * aspect;
        }
        glFrustum(-right, right, -top, top, 0.1, 100.0);
        glMatrixMode (GL_MODELVIEW);    //Prepare for model space by submitting the rotation/translation
        glLoadIdentity ();
        
        RGBColor * BackgroundColor = Prefs->GetBackgroundColorLoc();
        float red, green, blue;
        red = (float) BackgroundColor->red/65536;
        green = (float) BackgroundColor->green/65536;
        blue = (float) BackgroundColor->blue/65536; //Set the color to the Vector color
        glClearColor(red, green, blue, 1.0f);       // Setup the background "clear" color
        
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

void MpGLCanvas::eventMouse(wxMouseEvent &event) {
	if(!GetContext()) {
		return;
	}

	bool deSelectAll = true;

	SetCurrent();
	wxPoint tmpPnt = event.GetPosition();

	Frame *  lFrame = mMainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;
	mpAtom * lAtoms = lFrame->Atoms;

	if (event.LeftDown())
	  {
	    mSelectState = 0;
	    selected = testPicking(tmpPnt.x, tmpPnt.y);

	    if ( selected >= 0 && selected < NumAtoms && interactiveMode)
	      {
		GLdouble tmpWinX, tmpWinY;

		findWinCoord(lAtoms[selected].Position.x,
			      lAtoms[selected].Position.y,
			      lAtoms[selected].Position.z,
			      tmpWinX, tmpWinY, atomDepth);
 
		winDiffX = tmpPnt.x - (int)tmpWinX;
		winDiffY = tmpPnt.y - (int)tmpWinY;
	      }
	  }

	if (event.Dragging())
	  {
	    mSelectState++;
	    if ( selected >= 0 && selected < NumAtoms && interactiveMode)
	      {
		GLdouble newX, newY, newZ;

		findReal3DCoord(tmpPnt.x-winDiffX, tmpPnt.y-winDiffY, newX, newY, newZ);
		lAtoms[selected].Position.x = newX;
		lAtoms[selected].Position.y = newY;
		lAtoms[selected].Position.z = newZ;

		SelectObj(selected, deSelectAll);
		MolWin->SelectionChanged(deSelectAll);
		MolWin->UpdateGLModel();
	      }
	  }

 //   if (event.ControlDown())
	if (event.CmdDown())
	  deSelectAll = false;

    // allow a little bit dragging to be interpreted as selection
	if ( event.LeftUp())
	  {
	    if (interactiveMode)
	      {
		if (selected < 0)
		  {
		    AtomTypeDialog* newAtomTypeDlg = new AtomTypeDialog(this);
		    long tmpStatus = newAtomTypeDlg->ShowModal();

		    if (tmpStatus == wxID_OK)
		      {
			GLdouble newX, newY, newZ;

			findWinCoord(0.0, 0.0, 0.0, newX, newY, atomDepth);
			//estimate an atomDepth value, X and Y values are of no use 
			CPoint3D newPnt;
			findReal3DCoord(tmpPnt.x, tmpPnt.y, newX, newY, newZ);
			newPnt.x = newX;
			newPnt.y = newY;
			newPnt.z = newZ;

			lFrame->AddAtom(newAtomTypeDlg->getID(), newPnt);
		      }

		    delete newAtomTypeDlg;

		    oldSelect = -1;
		  }
		else if (selected != oldSelect)
		  {
		    int tmpBondStatus = lFrame->BondExists(oldSelect,selected);
 
		    if (tmpBondStatus == -1)
		      lFrame->AddBond(oldSelect,selected);

		    oldSelect = selected;

		    if (tmpBondStatus == -1)
		      selected = -1;
		  }
	      }
	    else if (mSelectState >= 0 && mSelectState < 3)
	      {
		mSelectState = -1;
	      }

	    SelectObj(selected, deSelectAll);
	    MolWin->SelectionChanged(deSelectAll);
	    MolWin->UpdateGLModel();
	}
    // Pass mouse event to MolDisplayWin::Rotate for processing
    if (interactiveMode)
      draw();
    else
      MolWin->Rotate(event);

}

void MpGLCanvas::KeyHandler(wxKeyEvent & event) {
    //char events are passed up the parent chain so se need to explicitely pass them
    MolWin->KeyHandler(event);
}

void MpGLCanvas::findWinCoord(GLfloat x, GLfloat y, GLfloat z, GLdouble& winX, GLdouble& winY, GLdouble& winZ)
{
  GLdouble mvMatrix[16];
  GLdouble projMatrix[16];
  GLint viewport[4];

  glGetIntegerv(GL_VIEWPORT, viewport);	
  glGetDoublev (GL_MODELVIEW_MATRIX, mvMatrix);
  glGetDoublev (GL_PROJECTION_MATRIX, projMatrix);

  gluProject(x, y, z, mvMatrix, projMatrix, viewport, &winX, &winY, &winZ);

  winY = viewport[3] - winY;  //"pretend" to have wx's coordinate system
}

void MpGLCanvas::findReal3DCoord(int x, int y, GLdouble& realX, GLdouble& realY, GLdouble& realZ)
{
  GLdouble mvMatrix[16];
  GLdouble projMatrix[16];
  GLint viewport[4];
  GLfloat winX, winY;

  glGetIntegerv(GL_VIEWPORT, viewport);	
  glGetDoublev (GL_MODELVIEW_MATRIX, mvMatrix);
  glGetDoublev (GL_PROJECTION_MATRIX, projMatrix);

  winX = (float)x;
  winY = (float)(viewport[3]-y);

  gluUnProject (winX, winY, atomDepth, mvMatrix, projMatrix, viewport, &realX, &realY, &realZ);
}

int MpGLCanvas::testPicking(int x, int y)
{
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

   if (aspect > 1.0) {
     right = myGLperspective;
     top = right/aspect;
   } else {
     top = myGLperspective;
     right = top * aspect;
   }

   glFrustum(-right, right, -top, top, 0.1, 100.0);

   glMatrixMode(GL_MODELVIEW);

   MolWin->DrawGL();

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
 
   hits = glRenderMode(GL_RENDER);
   
   int select_id = -1;
   unsigned int min_depth = 0xFFFFFFFF;

   for (int i = 0; i < hits; i++) {
     if (buff[i*4+3] < 0 || buff[i*4+3] > 512)
       continue;   //out of range

     if (buff[i*4+1] < min_depth) {
       min_depth = buff[i*4+1];
       select_id = buff[i*4+3];
     }
   }

   return select_id;
}

void MpGLCanvas::SelectObj(int select_id, bool mode) 
{
  Frame *  lFrame = mMainData->cFrame;
  long NumAtoms = lFrame->NumAtoms;
  mpAtom * lAtoms = lFrame->Atoms;
  Bond * lBonds = lFrame->Bonds;

  if (mode)
      lFrame->resetAllSelectState();

  if (select_id >= 0) {
    if (select_id < NumAtoms) {
      bool newstate = true;
      if (!mode && lAtoms[select_id].GetSelectState()) newstate = false;
      lAtoms[select_id].SetSelectState(newstate);

      for (int i = 0; i < lFrame->NumBonds; i++) {
	long atom1 = lBonds[i].Atom1;
	long atom2 = lBonds[i].Atom2;

	if (lAtoms[atom1].GetSelectState() && lAtoms[atom2].GetSelectState())
	  lBonds[i].SetSelectState(newstate);
      }
    } else if (select_id < (NumAtoms + lFrame->NumBonds)) {
      select_id -= NumAtoms;
      bool newstate = true;
      if (!mode && lBonds[select_id].GetSelectState()) newstate = false;
      lBonds[select_id].SetSelectState(newstate);

      long atom1 = lBonds[select_id].Atom1;
      long atom2 = lBonds[select_id].Atom2;

      lAtoms[atom1].SetSelectState(newstate);
      lAtoms[atom2].SetSelectState(newstate); //select atoms that 							      //this bond connect
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
    MolWin->SetHighliteMode(false);
  }

  glMatrixMode(GL_MODELVIEW);

}

MpGLCanvas::AtomTypeDialog::AtomTypeDialog(MpGLCanvas * parent, wxWindowID id, const wxString& caption) : typeID(1)
{
  Create(parent, id, caption);
}

void MpGLCanvas::AtomTypeDialog::Create(MpGLCanvas * parent, wxWindowID id, const wxString& caption)
{
  wxDialog::Create( parent, id, caption, wxDefaultPosition, wxSize(250, 125), wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX);

  std::vector<wxString> atomTypes;

  for ( int i = 0; i < kMaxAtomTypes; i++)
    {
      wxString tmp;

      parent->Prefs->GetAtomLabel(i, tmp);

      if (tmp.Length() > 0)
	atomTypes.push_back(tmp);
    }

  mainSizer = new wxBoxSizer(wxVERTICAL);
  upperSizer = new wxBoxSizer(wxVERTICAL);
  lowerSizer = new wxBoxSizer(wxHORIZONTAL);

  mTypeChoice = new wxChoice(this, ID_NEW_ATOM_TYPE_CHOICE, wxPoint(30,30), wxSize(200,wxDefaultCoord), atomTypes.size(), &atomTypes.front());
  mTypeChoice->SetSelection(0);

  mButtOK = new wxButton(this, wxID_OK, wxT("OK") );
  mButtCancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

  upperSizer->Add(mTypeChoice, 0, wxALIGN_CENTRE | wxALL, 20);
  upperSizer->Add(8,8);
  lowerSizer->Add(10,10);
  lowerSizer->Add(mButtOK, 0, wxALIGN_CENTRE | wxALL, 10);
  lowerSizer->Add(mButtCancel, 0, wxALIGN_CENTRE | wxALL, 10);

  mainSizer->Add(upperSizer);
  mainSizer->Add(lowerSizer);

  mainSizer->Layout();
  Centre(wxBOTH);
}

void MpGLCanvas::AtomTypeDialog::OnChoice( wxCommandEvent &event )
{
  typeID = event.GetInt() + 1;
}

BEGIN_EVENT_TABLE(MpGLCanvas, wxGLCanvas)
    EVT_SIZE             (MpGLCanvas::eventSize)
    EVT_PAINT            (MpGLCanvas::eventPaint)
    EVT_ERASE_BACKGROUND (MpGLCanvas::eventErase)
    EVT_MOUSE_EVENTS     (MpGLCanvas::eventMouse)

    EVT_CHAR (MpGLCanvas::KeyHandler)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(MpGLCanvas::AtomTypeDialog, wxDialog)
  EVT_CHOICE ( ID_NEW_ATOM_TYPE_CHOICE, AtomTypeDialog::OnChoice )
END_EVENT_TABLE()
