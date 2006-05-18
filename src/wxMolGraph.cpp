#include "wxMolGraph.h"

IMPLEMENT_DYNAMIC_CLASS(wxMolGraph, wxControl)

BEGIN_EVENT_TABLE(wxMolGraph, wxControl)
    EVT_SIZE         (wxMolGraph::onSize)
    EVT_PAINT        (wxMolGraph::onPaint)

    EVT_LEFT_DOWN    (wxMolGraph::onLeftClick)
    EVT_LEFT_UP      (wxMolGraph::onLeftClick)
    EVT_LEFT_DCLICK  (wxMolGraph::onLeftDblClick)
    EVT_MOTION       (wxMolGraph::onMotion)
    EVT_LEAVE_WINDOW (wxMolGraph::onLeaveWindow)
END_EVENT_TABLE()

const wxEventType wxEVT_AXIS_DCLICK = wxNewEventType();
const wxEventType wxEVT_GRAPH_CLICK = wxNewEventType();

const int BORDER = 2;

wxMolGraph::wxMolGraph(wxWindow       *parent,
                       wxWindowID      id,
                       const wxPoint  &pos,
                       const wxSize   &size,
                       long            style) :
            wxControl(parent, id, pos, size, style) {
    /* TEMPORARY: */
    xAxisText = wxT("X-Axis");
    y1AxisText = wxT("Y1-Axis");
    y2AxisText = wxT("Y2-Axis");
    xMax = 352.0;
    xMin = 0.0;
    y1Max = 1.0;
    y1Min = 0.0;
    y2Max = 25.37163;
    y2Min = -1.21398;
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

    wxClientDC dc(const_cast<wxMolGraph*> (this));
    wxString  xMaxText;
    wxString  xMinText;
    wxString  y1MaxText;
    wxString  y1MinText;
    wxString  y2MaxText;
    wxString  y2MinText;
    wxCoord w = 0;
    wxCoord h = 0;
    wxSize x(0, 0);
    wxSize xAxisTextSize(0, 0);
    wxSize xMaxTextSize(0, 0);
    wxSize xMinTextSize(0, 0);
    wxSize y1(0, 0);
    wxSize y1AxisTextSize(0, 0);
    wxSize y1MaxTextSize(0, 0);
    wxSize y1MinTextSize(0, 0);
    wxSize y2(0, 0);
    wxSize y2AxisTextSize(0, 0);
    wxSize y2MaxTextSize(0, 0);
    wxSize y2MinTextSize(0, 0);


    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.SetFont(*wxSWISS_FONT);

    xMaxText = wxString::Format("%0.4f", xMax);
    xMinText = wxString::Format("%0.4f", xMin);
    dc.GetTextExtent(xAxisText, &w, &h);
    xAxisTextSize.Set(w, h);
    dc.GetTextExtent(xMaxText, &w, &h);
    xMaxTextSize.Set(w, h);
    dc.GetTextExtent(xMinText, &w, &h);
    xMinTextSize.Set(w, h);

    y1MaxText = wxString::Format("%0.4f", y1Max);
    y1MinText = wxString::Format("%0.4f", y1Min);
    dc.GetTextExtent(y1AxisText, &w, &h);
    y1AxisTextSize.Set(w, h);
    dc.GetTextExtent(y1MaxText, &w, &h);
    y1MaxTextSize.Set(w, h);
    dc.GetTextExtent(y1MinText, &w, &h);
    y1MinTextSize.Set(w, h);

    y2MaxText = wxString::Format("%0.4f", y2Max);
    y2MinText = wxString::Format("%0.4f", y2Min);
    dc.GetTextExtent(y2AxisText, &w, &h);
    y2AxisTextSize.Set(w, h);
    dc.GetTextExtent(y2MaxText, &w, &h);
    y2MaxTextSize.Set(w, h);
    dc.GetTextExtent(y2MinText, &w, &h);
    y2MinTextSize.Set(w, h);


    x.SetWidth(xMinTextSize.GetHeight() +
               xAxisTextSize.GetWidth() +
               xMaxTextSize.GetHeight());
    x.SetHeight(wxMax(xMinTextSize.GetWidth(), xMaxTextSize.GetWidth()) +
                xAxisTextSize.GetHeight());

    if(numY1Graphs > 0 || numY2Graphs == 0) {
        y1.SetWidth(wxMax(y1MinTextSize.GetWidth(), y1MaxTextSize.GetWidth()) +
                    y1AxisTextSize.GetHeight());
        y1.SetHeight(y1MinTextSize.GetHeight() +
                     y1AxisTextSize.GetWidth() +
                     y1MaxTextSize.GetHeight());
    }
    else {
        y1.Set(0, 0);
    }

    if(numY2Graphs > 0) {
        y2.SetWidth(wxMax(y2MinTextSize.GetWidth(), y2MaxTextSize.GetWidth()) +
                    y2AxisTextSize.GetHeight());
        y2.SetHeight(y2MinTextSize.GetHeight() +
                     y2AxisTextSize.GetWidth() +
                     y2MaxTextSize.GetHeight());
    }
    else {
        y2.Set(0, 0);
    }

    w = (2 * BORDER) +
        y1.GetWidth() +
        wxMax(x.GetWidth(), 300) +
        y2.GetWidth();
    h = (2 * BORDER) +
        wxMax(wxMax(y1.GetHeight(), y2.GetHeight()), 200) +
        x.GetHeight();

    return wxSize( w, h );
}

void wxMolGraph::onSize(wxSizeEvent &event) {
    Refresh();
}

void wxMolGraph::onPaint(wxPaintEvent &event) {
    wxPaintDC dc(this);
    wxString  xMaxText;
    wxString  xMinText;
    wxString  y1MaxText;
    wxString  y1MinText;
    wxString  y2MaxText;
    wxString  y2MinText;
    wxCoord   x          = 0;
    wxCoord   x2         = 0;
    wxCoord   x3         = 0;
    wxCoord   x4         = 0;
    wxCoord   y          = 0;
    wxCoord   y2         = 0;
    wxCoord   y3         = 0;
    wxCoord   lineX      = 0;
    wxCoord   lineY      = 0;
    wxCoord   scaleMin   = 0;
    wxCoord   scaleMax   = 0;
    wxSize    canvasSize(0, 0);
    wxSize    xAxisTextSize(0, 0);
    wxSize    xMaxTextSize(0, 0);
    wxSize    xMinTextSize(0, 0);
    wxSize    y1AxisTextSize(0, 0);
    wxSize    y1MaxTextSize(0, 0);
    wxSize    y1MinTextSize(0, 0);
    wxSize    y2AxisTextSize(0, 0);
    wxSize    y2MaxTextSize(0, 0);
    wxSize    y2MinTextSize(0, 0);
    double    conversion = 0;
    double    spacing    = 0;
    double    firstTick  = 0;
    int       numTicks   = 0;
    int       i          = 0;

    GetClientSize(&x, &y);
    canvasSize.Set(x, y);

    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.SetFont(*wxSWISS_FONT);

    dc.Clear();


    xMaxText = wxString::Format("%0.4f", xMax);
    xMinText = wxString::Format("%0.4f", xMin);
    dc.GetTextExtent(xAxisText, &x, &y);
    xAxisTextSize.Set(x, y);
    dc.GetTextExtent(xMaxText, &x, &y);
    xMaxTextSize.Set(x, y);
    dc.GetTextExtent(xMinText, &x, &y);
    xMinTextSize.Set(x, y);

    y1MaxText = wxString::Format("%0.4f", y1Max);
    y1MinText = wxString::Format("%0.4f", y1Min);
    dc.GetTextExtent(y1AxisText, &x, &y);
    y1AxisTextSize.Set(x, y);
    dc.GetTextExtent(y1MaxText, &x, &y);
    y1MaxTextSize.Set(x, y);
    dc.GetTextExtent(y1MinText, &x, &y);
    y1MinTextSize.Set(x, y);

    y2MaxText = wxString::Format("%0.4f", y2Max);
    y2MinText = wxString::Format("%0.4f", y2Min);
    dc.GetTextExtent(y2AxisText, &x, &y);
    y2AxisTextSize.Set(x, y);
    dc.GetTextExtent(y2MaxText, &x, &y);
    y2MaxTextSize.Set(x, y);
    dc.GetTextExtent(y2MinText, &x, &y);
    y2MinTextSize.Set(x, y);

    
    // Calculate axis and graph regions.
    y2 = canvasSize.GetHeight() -
         (xAxisTextSize.GetHeight() + BORDER +
          wxMax(xMaxTextSize.GetWidth(), xMinTextSize.GetWidth()));
    y3 = canvasSize.GetHeight() - BORDER;
    if(numY1Graphs > 0 || numY2Graphs == 0) {
        x2 = y1AxisTextSize.GetHeight() + BORDER +
             wxMax(y1MaxTextSize.GetWidth(), y1MinTextSize.GetWidth());
        y1AxisRegion = wxRegion(BORDER, BORDER, x2, y2);
    }
    else {
        x2 = BORDER;
        y1AxisRegion.Clear();
    }
    x4 = canvasSize.GetWidth() - BORDER;
    if(numY2Graphs > 0) {
        x3 = canvasSize.GetWidth() -
             (y2AxisTextSize.GetHeight() + BORDER +
              wxMax(y2MaxTextSize.GetWidth(), y2MinTextSize.GetWidth()));
        y2AxisRegion = wxRegion(x3, BORDER, (x4 - x3), y2);
    }
    else {
        x3 = x4;
        y2AxisRegion.Clear();
    }

    xAxisRegion = wxRegion(x2, y2, (x3 - x2), (y3 - y2));
    graphRegion = wxRegion(x2, BORDER, (x3 - x2), y2);


    // X Axis
    x = x2 + ((x3 - x2) / 2) - (xAxisTextSize.GetWidth() / 2);
    y = y3 - xAxisTextSize.GetHeight();
    dc.DrawText(xAxisText, x, y);
    x = x2;
    y = y2 + xMinTextSize.GetWidth();
    dc.DrawRotatedText(xMinText, x, y, 90.0);
    x = x3 - xMaxTextSize.GetHeight();
    y = y2 + xMaxTextSize.GetWidth();
    dc.DrawRotatedText(xMaxText, x, y, 90.0);
    x = x2 + 3;
    y = y2 - (dc.GetCharHeight() / 2);
    lineX = x3 - 2;
    dc.DrawLine(x, y, lineX, y);
    // Tick marks:
    scaleMin = x2 + (dc.GetCharHeight() / 2);
    scaleMax = x3 - (dc.GetCharHeight() / 2);
    numTicks = (scaleMax - scaleMin) / 60;
    spacing = (xMax - xMin) / (double)numTicks;
    // TODO:  Round spacing
    conversion = (double)(scaleMax - scaleMin) / (xMax - xMin);
    y = y2 - 3;
    lineY = y2 - (dc.GetCharHeight() / 2);
    // TODO:  Find first tick
    firstTick = xMin;
    for(i = 0; i * spacing + firstTick < xMax; i++) {
        x = (int)(((double)i * spacing + firstTick) * conversion) + scaleMin;
        dc.DrawLine(x, y, x, lineY);
    }


    // Y1 Axis
    if(numY1Graphs > 0 || numY2Graphs == 0) {
        x = BORDER;
        y = BORDER + ((y2 - BORDER) / 2) + (y1AxisTextSize.GetWidth() / 2);
        dc.DrawRotatedText(y1AxisText, x, y, 90.0);
        x = x2 - y1MaxTextSize.GetWidth();
        y = BORDER;
        dc.DrawText(y1MaxText, x, y);
        x = x2 - y1MinTextSize.GetWidth();
        y = y2 - y1MinTextSize.GetHeight();
        dc.DrawText(y1MinText, x, y);
        x = x2 + (dc.GetCharHeight() / 2);
        y = y2 - 3;
        lineY = BORDER + (dc.GetCharHeight() / 2);
        dc.DrawLine(x, y, x, lineY);
        lineX = x2 + 3;
        dc.DrawLine(lineX, lineY, x + 1, lineY);
        // Tick marks:
        scaleMin = y2 - (dc.GetCharHeight() / 2);
        scaleMax = BORDER + (dc.GetCharHeight() / 2);
        numTicks = (scaleMin - scaleMax) / 60;
        spacing = (y1Max - y1Min) / (double)numTicks;
        // TODO:  Round spacing
        conversion = (double)(scaleMin - scaleMax) / (y1Max - y1Min);
        x = x2 + 3;
        lineX = x2 + (dc.GetCharHeight() / 2);
        // TODO:  Find first tick
        firstTick = y1Min;
        for(i = 0; i * spacing + firstTick < y1Max; i++) {
            y = scaleMin - (int)(((double)i * spacing + firstTick) * conversion);
            dc.DrawLine(x, y, lineX, y);
        }
    }

    // Y2 Axis
    if(numY2Graphs > 0) {
        x = x4 - y2AxisTextSize.GetHeight();
        y = BORDER + ((y2 - BORDER) / 2) + (y2AxisTextSize.GetWidth() / 2);
        dc.DrawRotatedText(y2AxisText, x, y, 90.0);
        x = x3;
        y = BORDER;
        dc.DrawText(y2MaxText, x, y);
        x = x3;
        y = y2 - y2MinTextSize.GetHeight();
        dc.DrawText(y2MinText, x, y);
        x = x3 - (dc.GetCharHeight() / 2);
        y = y2 - 3;
        lineY = BORDER + (dc.GetCharHeight() / 2);
        dc.DrawLine(x, y, x, lineY);
        lineX = x3 - 2;
        dc.DrawLine(x, lineY, lineX, lineY);
        // Tick marks:
        scaleMin = y2 - (dc.GetCharHeight() / 2);
        scaleMax = BORDER + (dc.GetCharHeight() / 2);
        numTicks = (scaleMin - scaleMax) / 60;
        spacing = (y2Max - y2Min) / (double)numTicks;
        // TODO:  Round spacing
        conversion = (double)(scaleMin - scaleMax) / (y2Max - y2Min);
        x = x3 - (dc.GetCharHeight() / 2);
        lineX = x3 - 2;
        // TODO:  Find first tick
        firstTick = y2Min;
        for(i = 0; i * spacing + firstTick < y2Max; i++) {
            y = scaleMin - (int)(((double)i * spacing + firstTick) * conversion);
            dc.DrawLine(x, y, lineX, y);
        }
    }
}

void wxMolGraph::onLeftClick(wxMouseEvent &event) {
    wxCommandEvent event_graph(wxEVT_GRAPH_CLICK);

    if(event.LeftDown()) {
        if(graphRegion.Contains(event.GetPosition()) == wxInRegion) {
            // Store coords
        }
        event.Skip();
    }
    else if(event.LeftUp()) {
        if(graphRegion.Contains(event.GetPosition()) == wxInRegion &&
           0 /* Still hovering over same x-val */) {
            wxPostEvent(this, event_graph);
        }
    }

}

void wxMolGraph::onLeftDblClick(wxMouseEvent &event) {
    wxCommandEvent event_axis(wxEVT_AXIS_DCLICK);

    if(xAxisRegion.Contains(event.GetPosition()) == wxInRegion) {
        event_axis.SetInt(MG_AXIS_X);
        wxPostEvent(this, event_axis);
    }
    else if(y1AxisRegion.Contains(event.GetPosition()) == wxInRegion) {
        event_axis.SetInt(MG_AXIS_Y1);
        wxPostEvent(this, event_axis);
    }
    else if(y2AxisRegion.Contains(event.GetPosition()) == wxInRegion) {
        event_axis.SetInt(MG_AXIS_Y2);
        wxPostEvent(this, event_axis);
    }
}

void wxMolGraph::onMotion(wxMouseEvent &event) {
    if(graphRegion.Contains(event.GetPosition()) == wxInRegion) {
    }
    else {
    }
}

void wxMolGraph::onLeaveWindow(wxMouseEvent &event) {
}

