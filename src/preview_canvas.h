#ifndef PREVIEW_CANVAS_H
#define PREVIEW_CANVAS_H

#include "wx/wx.h"
#include "mmp_gl.h"
#include "Geometries.h"
#include "build_palette.h"
#include "MyTypes.h"
#include "mmp_gl.h"

class Structure;

class PreviewCanvas : public wxGLCanvas {
	public:
		PreviewCanvas(wxWindow *parent,
					  wxWindowID id = wxID_ANY, int *attributes = 0,
					  const wxPoint& position = wxDefaultPosition,
					  const wxSize& size = wxDefaultSize,
					  long style = wxFULL_REPAINT_ON_RESIZE,
					  const wxString& name = _T("GL Canvas"));
		~PreviewCanvas();
		void Render();
		void InitGL();
		void SetStructure(Structure *structure);

	private:
		void OnErase(wxEraseEvent& event) {}
		void OnPaint(wxPaintEvent& event);
		void OnSize(wxSizeEvent& event);
		void OnIdle(wxIdleEvent& event);
		void OnLeftMouseDown(wxMouseEvent& event);
		void OnMiddleMouseDown(wxMouseEvent& event);
		void OnLeftMouseUp(wxMouseEvent& event);
		void OnMiddleMouseUp(wxMouseEvent& event);
		void OnMouseEnterWindow(wxMouseEvent& event);
		void OnMouseDrag(wxMouseEvent& event);
		void OnKey(wxKeyEvent& event);
		void DrawAtoms();
		int Pick();
		
		wxPoint curr_mouse;
		wxPoint prev_mouse;
		Matrix4D global_rotation;
		Structure *struc;
		CPoint3D centroid;
		GLdouble modelview[16];
		GLdouble proj[16];
		GLint viewport[4];
		GLUquadric *quadric;
		GLuint sphere_list;
		float fov;
		bool gl_initialized;
		int selected;
		bool was_dragging;
#if wxCHECK_VERSION(2,9,0)
		wxGLContext *context;
#endif

	DECLARE_EVENT_TABLE()
};

#endif
