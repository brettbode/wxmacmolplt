#include "wxMolGraph.h"
#include <stdexcept>

using namespace std;

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
    reset();

    /* TEMPORARY: */
    xAxisText  = wxT("X-Axis");
    y1AxisText = wxT("Y1-Axis");
    y2AxisText = wxT("Y2-Axis");
    xMax  = 352.0;
    xMin  = 0.0;
    y1Max = 10.0;
    y1Min = 1.0;
    y2Max = 25.37163;
    y2Min = -1.21398;
    numY1Graphs = 1;
    numY2Graphs = 1;
    precision = 4;
    vector<double> v(5);
    v[0] = 20.5;
    v[1] = 93.7;
    v[2] = 144.0;
    v[3] = 250.0;
    v[4] = 300.0;
    addXSet(v, true);
    YSet ys;
    ys.push_back(make_pair(0, 1.5));
    ys.push_back(make_pair(1, 5.5));
    ys.push_back(make_pair(2, 9.5));
    ys.push_back(make_pair(3, 7.0));
    ys.push_back(make_pair(4, 19.0));
    addYSet(ys, 0, MG_AXIS_Y1, MG_STYLE_POINT_LINE, *wxBLACK);
}

int wxMolGraph::addXSet(vector<double> xData, bool selectable) {
    int index = data.size();
    int selected = 0;

    vector< YSettings > emptyYSettingsGroup;

    if(!selectable) selected = -1;

    vector< YSet > emptyYSetGroup;
    XSet newXSet(make_pair(xData, selected));

    try {
        data.push_back(make_pair(newXSet, emptyYSetGroup));
    }
    catch(const bad_alloc &e) {
        return -1;
    }

    try {
        dataSettings.push_back(emptyYSettingsGroup);
    }
    catch(const bad_alloc &e) {
        data.pop_back();
        return -1;
    }

    // TODO:  Adjust X-Axis Scale
    
    Refresh();

    return index;
}

int wxMolGraph::addYSet(YSet yData,
                        int xSet,
                        int axis,
                        int style,
                        wxColour color) {
    int index = data[xSet].second.size();
    YSettings newYSettings;

    newYSettings.exists = true;
    newYSettings.axis = axis;
    newYSettings.style = style;
    newYSettings.color = color;

    try {
        data[xSet].second.push_back(yData);
    }
    catch(const bad_alloc &e) {
        return -1;
    }
    try {
        dataSettings[xSet].push_back(newYSettings);
    }
    catch(const bad_alloc &e) {
        data[xSet].second.pop_back();
        return -1;
    }

    if(axis == MG_AXIS_Y1) {
        numY1Graphs++;
    }
    else if(axis == MG_AXIS_Y2) {
        numY2Graphs++;
    }
    else {
        data[xSet].second.pop_back();
        dataSettings[xSet].pop_back();
        return -1;
    }

    Refresh();

    return index;
}

void wxMolGraph::delXSet(int xSet) {
    vector< YSettings >::iterator pos;

    for(pos = dataSettings[xSet].begin();
        pos != dataSettings[xSet].end();
        ++pos) {

        if(pos->exists) {
            switch(pos->axis) {
                case MG_AXIS_Y1:
                    numY1Graphs--;
                    break;
                case MG_AXIS_Y2:
                    numY2Graphs--;
                    break;
            }
        }
    }

    data[xSet].first.first.clear();
    data[xSet].first.second = -1;
    data[xSet].second.clear();
    dataSettings[xSet].clear();
}

void wxMolGraph::delYSet(int xSet, int ySet) {
    if(!dataSettings[xSet][ySet].exists) {
        return;
    }

    dataSettings[xSet][ySet].exists = false;
    data[xSet].second.clear();
}

int wxMolGraph::getSelection(int xSet) {
    return data[xSet].first.second;
}

void wxMolGraph::setSelection(int xSet, int index) {
    if(index < data[xSet].first.first.size()) {
        data[xSet].first.second = index;
    }
}

void wxMolGraph::setYAxisMin(int axis, double val) {
    switch(axis) {
        case MG_AXIS_Y1:
            y1Min = val;
            break;
        case MG_AXIS_Y2:
            y2Min = val;
            break;
    }
}

void wxMolGraph::setYAxisMax(int axis, double val) {
    switch(axis) {
        case MG_AXIS_Y1:
            y1Max = val;
            break;
        case MG_AXIS_Y2:
            y2Max = val;
            break;
    }
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
    data.clear();
    dataSettings.clear();
    xAxisText.Empty();
    y1AxisText.Empty();
    y2AxisText.Empty();
    xMax  = 1.0;
    xMin  = 0.0;
    y1Max = 1.0;
    y1Min = 0.0;
    y2Max = 1.0;
    y2Min = 0.0;
    numY1Graphs = 0;
    numY2Graphs = 0;
    precision = 4;
    xAxisRegion.Clear();
    y1AxisRegion.Clear();
    y2AxisRegion.Clear();
    graphRegion.Clear();
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

    xMaxText = wxString::Format("%0.*f", precision, xMax);
    xMinText = wxString::Format("%0.*f", precision, xMin);
    dc.GetTextExtent(xAxisText, &w, &h);
    xAxisTextSize.Set(w, h);
    dc.GetTextExtent(xMaxText, &w, &h);
    xMaxTextSize.Set(w, h);
    dc.GetTextExtent(xMinText, &w, &h);
    xMinTextSize.Set(w, h);

    y1MaxText = wxString::Format("%0.*f", precision, y1Max);
    y1MinText = wxString::Format("%0.*f", precision, y1Min);
    dc.GetTextExtent(y1AxisText, &w, &h);
    y1AxisTextSize.Set(w, h);
    dc.GetTextExtent(y1MaxText, &w, &h);
    y1MaxTextSize.Set(w, h);
    dc.GetTextExtent(y1MinText, &w, &h);
    y1MinTextSize.Set(w, h);

    y2MaxText = wxString::Format("%0.*f", precision, y2Max);
    y2MinText = wxString::Format("%0.*f", precision, y2Min);
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
    wxCoord   xScaleMin   = 0;
    wxCoord   xScaleMax   = 0;
    wxCoord   yScaleMin   = 0;
    wxCoord   yScaleMax   = 0;
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
    double    xConversion = 0.0;
    double    y1Conversion = 0.0;
    double    y2Conversion = 0.0;
    double    spacing    = 0.0;
    double    firstTick  = 0.0;
    double    xCoord     = 0.0;
    double    yCoord     = 0.0;
    int       numTicks   = 0;
    int       i          = 0;
    int       j          = 0;
    int       k          = 0;
    DataSet::iterator pos;

    GetClientSize(&x, &y);
    canvasSize.Set(x, y);

    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.SetFont(*wxSWISS_FONT);

    dc.Clear();


    xMaxText = wxString::Format("%0.*f", precision, xMax);
    xMinText = wxString::Format("%0.*f", precision, xMin);
    dc.GetTextExtent(xAxisText, &x, &y);
    xAxisTextSize.Set(x, y);
    dc.GetTextExtent(xMaxText, &x, &y);
    xMaxTextSize.Set(x, y);
    dc.GetTextExtent(xMinText, &x, &y);
    xMinTextSize.Set(x, y);

    y1MaxText = wxString::Format("%0.*f", precision, y1Max);
    y1MinText = wxString::Format("%0.*f", precision, y1Min);
    dc.GetTextExtent(y1AxisText, &x, &y);
    y1AxisTextSize.Set(x, y);
    dc.GetTextExtent(y1MaxText, &x, &y);
    y1MaxTextSize.Set(x, y);
    dc.GetTextExtent(y1MinText, &x, &y);
    y1MinTextSize.Set(x, y);

    y2MaxText = wxString::Format("%0.*f", precision, y2Max);
    y2MinText = wxString::Format("%0.*f", precision, y2Min);
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
    graphRegion = wxRegion(x2 + dc.GetCharHeight() / 2,
                  BORDER,
                  x3 - x2 - dc.GetCharHeight() + 1,
                  y2 - dc.GetCharHeight() / 2);


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
    xScaleMin = x2 + (dc.GetCharHeight() / 2);
    xScaleMax = x3 - (dc.GetCharHeight() / 2);
    numTicks = (xScaleMax - xScaleMin) / 60;
    spacing = (xMax - xMin) / (double)numTicks;
    // TODO:  Round spacing
    xConversion = (double)(xScaleMax - xScaleMin) / (xMax - xMin);
    y = y2 - 3;
    lineY = y2 - (dc.GetCharHeight() / 2);
    // TODO:  Find first tick
    firstTick = xMin;
    for(i = 0; i * spacing + firstTick < xMax; i++) {
        x = xScaleMin + (int)(((double)i * spacing + firstTick - xMin) * xConversion);
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
        yScaleMin = y2 - (dc.GetCharHeight() / 2);
        yScaleMax = BORDER + (dc.GetCharHeight() / 2);
        numTicks = (yScaleMin - yScaleMax) / 60;
        spacing = (y1Max - y1Min) / (double)numTicks;
        // TODO:  Round spacing
        y1Conversion = (double)(yScaleMin - yScaleMax) / (y1Max - y1Min);
        x = x2 + 3;
        lineX = x2 + (dc.GetCharHeight() / 2);
        // TODO:  Find first tick
        firstTick = y1Min;
        for(i = 0; i * spacing + firstTick < y1Max; i++) {
            y = yScaleMin - (int)(((double)i * spacing + firstTick - y1Min) * y1Conversion);
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
        yScaleMin = y2 - (dc.GetCharHeight() / 2);
        yScaleMax = BORDER + (dc.GetCharHeight() / 2);
        numTicks = (yScaleMin - yScaleMax) / 60;
        spacing = (y2Max - y2Min) / (double)numTicks;
        // TODO:  Round spacing
        y2Conversion = (double)(yScaleMin - yScaleMax) / (y2Max - y2Min);
        x = x3 - (dc.GetCharHeight() / 2);
        lineX = x3 - 2;
        // TODO:  Find first tick
        firstTick = y2Min;
        for(i = 0; i * spacing + firstTick < y2Max; i++) {
            y = yScaleMin - (int)(((double)i * spacing + firstTick - y2Min) * y2Conversion);
            dc.DrawLine(x, y, lineX, y);
        }
    }

    // Data
    dc.SetClippingRegion(graphRegion);
    try {
        for(i = 0; i < data.size(); i++) {
            for(j = 0; j < data[i].second.size(); j++) {
                for(k = 0; k < data[i].second[j].size(); k++) {
                    xCoord = data[i].first.first.at(data[i].second[j][k].first);
                    yCoord = data[i].second[j][k].second;
                    // I don't know if this part works
                    x = xScaleMin + (int)((xCoord - xMin) * xConversion);
                    switch(dataSettings[i][j].axis) {
                        case MG_AXIS_Y1:
                            y = yScaleMin - (int)((yCoord - y1Min) * y1Conversion);
                            break;
                        case MG_AXIS_Y2:
                            y = yScaleMin - (int)((yCoord - y2Min) * y2Conversion);
                            break;
                    }
                    // TEMPORARY:
                    dc.DrawCircle(x, y, 3);
                }
            }
        }
    }
    catch(const out_of_range &e) {
    }
}

void wxMolGraph::onLeftClick(wxMouseEvent &event) {
    wxCommandEvent event_graph(wxEVT_GRAPH_CLICK);

    if(event.LeftDown()) {
        if(graphRegion.Contains(event.GetPosition()) == wxInRegion) {
            cout << "Graph Clicked!" << endl;
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

