#ifndef PREVIEW_CANVAS_H
#define PREVIEW_CANVAS_H

#include "wx/wx.h"
#include "wx/glcanvas.h"
#include "Geometries.h"

class PreviewCanvas : public wxGLCanvas {
	public:
		PreviewCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
					  int *attributes = 0,
					  const wxPoint& position = wxDefaultPosition,
					  const wxSize& size = wxDefaultSize,
					  long style = 0, const wxString& name = _T("GL Canvas"));
		void Render();
		void InitGL();

	private:
		void OnPaint(wxPaintEvent& event);
		void OnSize(wxSizeEvent& event);
		void OnIdle(wxIdleEvent& event);
		void OnLeftMouseDown(wxMouseEvent& event);
		void OnMouseDrag(wxMouseEvent& event);
		
		wxGLContext *context;
		wxPoint curr_mouse;
		wxPoint prev_mouse;
		Matrix4D global_rotation;

	DECLARE_EVENT_TABLE()
};

#endif
