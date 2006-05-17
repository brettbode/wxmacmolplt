#include "wxMolGraph.h"

IMPLEMENT_DYNAMIC_CLASS(wxMolGraph, wxControl)

BEGIN_EVENT_TABLE(wxMolGraph, wxControl)
    EVT_SIZE        (wxMolGraph::onSize)
    EVT_PAINT       (wxMolGraph::onPaint)

    EVT_LEFT_DOWN   (wxMolGraph::onLeftClick)
    EVT_LEFT_UP     (wxMolGraph::onLeftClick)
    EVT_LEFT_DCLICK (wxMolGraph::onLeftDblClick)
    EVT_MOTION      (wxMolGraph::onMotion)
END_EVENT_TABLE()

const wxEventType wxEVT_AXIS_DCLICK = wxNewEventType();
const wxEventType wxEVT_GRAPH_CLICK = wxNewEventType();

const int BORDER = 2;

wxMolGraph::wxMolGraph(wxWindow* parent, wxWindowID id) : wxControl(parent, id) {
    /* TEMPORARY: */
    xAxisText = wxT("X-Axis");
    y1AxisText = wxT("Y1-Axis");
    y2AxisText = wxT("Y2-Axis");
    numY1Graphs = 1;
    numY2Graphs = 1;
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
    switch(axis) {
        case MG_AXIS_X:
            xAxisText = label;
            break;
        case MG_AXIS_Y1:
            y1AxisText = label;
            break;
        case MG_AXIS_Y2:
            y2AxisText = label;
            break;
    }
}

void wxMolGraph::setOffsetY(int axis, double offset) {
}

void wxMolGraph::reset() {
}

wxSize wxMolGraph::DoGetBestSize() const {
    wxASSERT_MSG( m_widget, wxT("wxMolGraph::DoGetBestSize called before creation") );

    wxCoord w = 400;
    wxCoord h = 400;

    return wxSize( w, h );
}

void wxMolGraph::onSize(wxSizeEvent &event) {
    Refresh();
}

void wxMolGraph::onPaint(wxPaintEvent &event) {
    wxPaintDC dc(this);
    wxCoord   canvasWidth      = 0;
    wxCoord   canvasHeight     = 0;
    wxCoord   x                = 0;
    wxCoord   y                = 0;
    wxCoord   xAxisTextWidth   = 0;
    wxCoord   xAxisTextHeight  = 0;
    wxCoord   y1AxisTextWidth  = 0;
    wxCoord   y1AxisTextHeight = 0;
    wxCoord   y2AxisTextWidth  = 0;
    wxCoord   y2AxisTextHeight = 0;

    GetClientSize(&canvasWidth, &canvasHeight);

    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.SetFont(*wxSWISS_FONT);
    dc.SetTextBackground(*wxWHITE);
    dc.SetTextBackground(*wxWHITE);
    dc.SetBackgroundMode(wxSOLID);

    dc.Clear();
    dc.DrawRectangle(x, y, canvasWidth, canvasHeight);

    dc.GetTextExtent(xAxisText, &xAxisTextWidth, &xAxisTextHeight);
    x = (canvasWidth / 2) - (xAxisTextWidth / 2);
    y = (canvasHeight - xAxisTextHeight) - BORDER;
    dc.DrawText(xAxisText, x, y);

    if(numY1Graphs > 0 || numY2Graphs == 0) {
        dc.GetTextExtent(y1AxisText, &y1AxisTextWidth, &y1AxisTextHeight);
        x = BORDER;
        y = (canvasHeight / 2) + (y1AxisTextWidth / 2);
        dc.DrawRotatedText(y1AxisText, x, y, 90.0);
    }

    if(numY2Graphs > 0) {
        dc.GetTextExtent(y2AxisText, &y2AxisTextWidth, &y2AxisTextHeight);
        x = (canvasWidth - y2AxisTextHeight) - BORDER;
        y = (canvasHeight / 2) + (y2AxisTextWidth / 2);
        dc.DrawRotatedText(y2AxisText, x, y, 90.0);
    }
}

void wxMolGraph::onLeftClick(wxMouseEvent &event) {
    wxCommandEvent event_graph(wxEVT_GRAPH_CLICK);

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

void wxMolGraph::onMotion(wxMouseEvent &event) {
}

