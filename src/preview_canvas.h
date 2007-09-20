#ifndef PREVIEW_CANVAS_H
#define PREVIEW_CANVAS_H

#ifdef __WXMAC__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "wx/wx.h"
#include "wx/glcanvas.h"
#include "Geometries.h"
#include "build_palette.h"
#include "MyTypes.h"

class Structure;

class PreviewCanvas : public wxGLCanvas {
	public:
		PreviewCanvas(wxWindow *parent,
					  wxWindowID id = wxID_ANY, int *attributes = 0,
					  const wxPoint& position = wxDefaultPosition,
					  const wxSize& size = wxDefaultSize,
					  long style = 0, const wxString& name = _T("GL Canvas"));
		void Render();
		void InitGL();
		void SetStructure(Structure *structure);

	private:
		void OnPaint(wxPaintEvent& event);
		void OnSize(wxSizeEvent& event);
		void OnIdle(wxIdleEvent& event);
		void OnLeftMouseDown(wxMouseEvent& event);
		void OnMouseDrag(wxMouseEvent& event);
		void DrawBond(const Bond& bond, const mpAtom& atom1,
					  const mpAtom& atom2);
		
		wxGLContext *context;
		wxPoint curr_mouse;
		wxPoint prev_mouse;
		Matrix4D global_rotation;
		Structure *struc;
		CPoint3D centroid;
		GLdouble modelview[16];
		GLdouble proj[16];
		GLint viewport[4];
		GLUquadric *quadric;

	DECLARE_EVENT_TABLE()
};

#endif
