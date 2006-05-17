#include "wxMolGraph.h"

IMPLEMENT_DYNAMIC_CLASS(wxMolGraph, wxControl)

BEGIN_EVENT_TABLE(wxMolGraph, wxControl)
    EVT_PAINT       (wxMolGraph::onPaint)
    EVT_LEFT_DOWN   (wxMolGraph::onLeftClick)
    EVT_LEFT_UP     (wxMolGraph::onLeftClick)
    EVT_LEFT_DCLICK (wxMolGraph::onLeftDblClick)
END_EVENT_TABLE()

const wxEventType wxEVT_AXIS_DCLICK = wxNewEventType();
const wxEventType wxEVT_GRAPH_CLICK = wxNewEventType();

wxMolGraph::wxMolGraph(wxWindow* parent, wxWindowID id) : wxControl(parent, id) {
}

int wxMolGraph::addXSet(vector<double> data, bool selectable) {
    int index = data.size();

    //data.push_back();

    return index;
}

int wxMolGraph::addYSet(vector<pair<int,double> > data,
                        int xSet,
                        int axis,
                        int style,
                        wxColour color) {
}

void wxMolGraph::delXSet(int xSet) {
}

void wxMolGraph::delYSet(int xSet, int ySet) {
}

int wxMolGraph::getSelection(int xSet) {
}

void wxMolGraph::setSelection(int xSet, int index) {
}

void wxMolGraph::setYAxisMin(int axis, double val) {
}

void wxMolGraph::setYAxisMax(int axis, double val) {
}

void wxMolGraph::autoScaleY(int axis) {
}

void wxMolGraph::setAxisLabel(int axis, wxString &label) {
}

void wxMolGraph::setOffsetY(int axis, double offset) {
}

void wxMolGraph::reset() {
}

void wxMolGraph::onPaint(wxPaintEvent &event) {
    wxPaintDC dc(this);
    wxCoord   width    = 0;
    wxCoord   height   = 0;

    GetClientSize(&width, &height);

    dc.SetPen(*wxRED_PEN);
    dc.SetBrush(*wxBLUE_BRUSH);
    dc.DrawRectangle(0, 0, width, height);
}

void wxMolGraph::onLeftClick(wxMouseEvent &event) {
    wxCommandEvent event_graph(wxEVT_GRAPH_CLICK);

    // TODO:  Figure out what's going on and post a custom event.
    cout << "event.m_x: " << event.m_x << endl;
    cout << "event.m_y: " << event.m_y << endl;
    cout << "event.GetX(): " << event.GetX() << endl;
    cout << "event.GetY(): " << event.GetY() << endl;

    if(event.LeftDown()) {
        if(0/* Coords over graph area */) {
            // Store coords
        }
        event.Skip();
    }
    else if(event.LeftUp()) {
        if(0/* Coords over graph area and in same zone */) {
            wxPostEvent(this, event_graph);
        }
    }

}

void wxMolGraph::onLeftDblClick(wxMouseEvent &event) {
    wxCommandEvent event_axis(wxEVT_AXIS_DCLICK);

    // TODO:  Figure out what's going on and post a custom event.
    cout << "event.m_x: " << event.m_x << endl;
    cout << "event.m_y: " << event.m_y << endl;
    cout << "event.GetX(): " << event.GetX() << endl;
    cout << "event.GetY(): " << event.GetY() << endl;

    if(0/* Is over axis x */) {
        event_axis.SetInt(MG_AXIS_X);
        wxPostEvent(this, event_axis);
    }
    else if(0/* Is over axis y1 */) {
        event_axis.SetInt(MG_AXIS_Y1);
        wxPostEvent(this, event_axis);
    }
    else if(0/* Is over axis y2 */) {
        event_axis.SetInt(MG_AXIS_Y2);
        wxPostEvent(this, event_axis);
    }
}

