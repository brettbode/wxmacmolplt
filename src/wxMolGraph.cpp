#include "wxMolGraph.h"

IMPLEMENT_DYNAMIC_CLASS(wxMolGraph, wxControl)

BEGIN_EVENT_TABLE(wxMolGraph, wxControl)
    EVT_PAINT(wxMolGraph::onPaint)
END_EVENT_TABLE()

wxMolGraph::wxMolGraph(wxWindow* parent, wxWindowID id) : wxControl(parent, id) {
}

int wxMolGraph::addXSet(vector<double> data, bool selectable) {
}

int wxMolGraph::addYSet(vector<pair<int,double>> data,
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

void wxMolGraph::onPaint(wxPaintEvent &event) {
}

