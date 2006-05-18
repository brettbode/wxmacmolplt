#include "wx/wx.h"
#include <vector>
#include <utility>

using namespace std;

#ifndef _WX_MOL_GRAPH_H_
#define _WX_MOL_GRAPH_H_

#define MG_AXIS_X  0
#define MG_AXIS_Y1 1
#define MG_AXIS_Y2 2


#define MG_STYLE_NONE  0x00
#define MG_STYLE_POINT 0x01
#define MG_STYLE_LINE  0x02
#define MG_STYLE_BAR   0x04

#define MG_STYLE_POINT_LINE 0x03

extern const wxEventType wxEVT_AXIS_DCLICK;
extern const wxEventType wxEVT_GRAPH_CLICK;

#define EVT_AXIS_DCLICK(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_AXIS_DCLICK, id, -1, (wxObjectEventFunction)(wxEventFunction)(wxNotifyEventFunction)&fn, (wxObject *) NULL )
#define EVT_GRAPH_CLICK(id, fn) DECLARE_EVENT_TABLE_ENTRY( wxEVT_GRAPH_CLICK, id, -1, (wxObjectEventFunction)(wxEventFunction)(wxNotifyEventFunction)&fn, (wxObject *) NULL )

/**
 * A custom widget designed for graphing various types of data associated
 * with molecules in wxMacMolPlt.
 */
class wxMolGraph : public wxControl {
  private:
    /*               x-set  x-val  x-curr  y-sets y-set       xi  y-val */
    vector<pair<pair<vector<double>, int>, vector<vector<pair<int,double> > > > > data;

    wxString xAxisText;
    wxString y1AxisText;
    wxString y2AxisText;

    double xMax;
    double xMin;
    double y1Max;
    double y1Min;
    double y2Max;
    double y2Min;

    int numY1Graphs;
    int numY2Graphs;

    wxRegion xAxisRegion;
    wxRegion y1AxisRegion;
    wxRegion y2AxisRegion;
    wxRegion graphRegion;

    int precision;

    /* TODO */

    DECLARE_DYNAMIC_CLASS(wxMolGraph) /* NOTE:  Everything after this will be
                                       * public!
                                       */

  public:
    /**
     * Default constructor.  This is here to keep wxWidgets RTTI happy.  You
     * shouldn't actually use it.
     */
    wxMolGraph() {}

    
    /**
     * Constructor for general use.
     *
     * @param parent The parent window of the new wxMolGraph.
     * @param id The window ID of the new wxMolGraph.
     * @param pos The initial position of the new wxMolGraph.
     * @param size The initial size of the new wxMolGraph.
     * @param style The initial style of the new wxMolGraph.
     */
    wxMolGraph(wxWindow       *parent,
               wxWindowID      id,
               const wxPoint  &pos   = wxDefaultPosition,
               const wxSize   &size  = wxDefaultSize,
               long            style = wxSUNKEN_BORDER);
    /* TODO:  Write more useful doxygen comments for this function. */


    /**
     * Destructor.  TODO:  Write something interesting for doxygen.
     */
    ~wxMolGraph() {}


    /**
     * Adds a new set of x-values to the wxMolGraph.  If necessary, the scale
     * of the x-axis will be adjusted to contain the new values, but nothing
     * will be drawn for the new set until a y-set is associated with it.
     *
     * @param data A vector of the x-values in the set.  The elements  of the
     *             vector are expected to be in increasing order, and no two
     *             elements may have the same value.
     * @param selectable Sets the selectable flag for the set.  If it is true,
     *                   points may be selected with a mouse (or arrow keys?).
     * @return On success this function returns the index of the x-set within
     *         the wxMolGraph.  This will be one greater than the index of the
     *         last x-set added, with the first x-set added having an index of
     *         zero.  On failure this function returns a value less than zero.
     */
    int addXSet(vector<double> data, bool selectable);


    /**
     * Adds a new set of y-values and associates it with an x-set.  This causes
     * the wxMolGraph to be redrawn.  The scales of all axis are unaffected.
     *
     * @param data A vector of pairs, one for each y-value.  The first element
     *             of each pair is the index of the x-value (within the
     *             associated x-set) with which the y-value is associated.
     *             The indices must occur in increasing order, and no two
     *             y-values in a set may be associated with the same x-index;
     *             however, there need not be a y-value associated with every
     *             possible index in the x-set.  If an x-index is outside the
     *             range of the x-set, the y-value associated with that
     *             x-index, and all y-values occuring after it in the vector,
     *             wils be ignored.  The second element of each pair is the
     *             y-value itself.
     * @param xSet The index of the x-set with which to associate the new y-set.
     * @param axis Specifies which axis to graph the y-set on.  Must be one of
     *             MG_AXIS_Y1 or MG_AXIS_Y2.
     * @param style Specifies the style which should be used to draw the data
     *              points.  This is a bitmask of style flags.  Valid style
     *              flags are:
     *                  MG_STYLE_NONE  - Draw nothing.
     *                  MG_STYLE_POINT - Draw a colored dot at each data point.
     *                  MG_STYLE_LINE  - Draw a line between adjacent
     *                                   data points.
     *                  MG_STYLE_BAR   - Draw a vertical bar between each data
     *                                   point and the x-axis.
     *                  MG_STYLE_POINT_LINE - The same as
     *                                   (MG_STYLE_POINT | MG_STYLE_LINE)
     * @return On success this function returns the index of the y-set within
     *         the associated x-set.  This will be one greater than the index
     *         of the last y-set associated with the same x-set, with the first
     *         y-set associated with a given x-set having and index of zero.
     *         On failure this function returns a value less than zero.
     */
    int addYSet(vector<pair<int,double> > data,
                int xSet,
                int axis,
                int style,
                wxColour color);


    /**
     * Removes an x-set, and all y-sets associated with it, from the
     * wxMolGraph.  This causes the wxMolGraph to be redrawn.  Note that this
     * has no effect on the index assigned to the next x-set added.
     *
     * @param xSet The index of the x-set to remove.
     */
    void delXSet(int xSet);


    /**
     * Disassociates a y-set from its x-set and removes it from the wxMolGraph.
     * This causes the wxMolGraph to be redrawn.  Note that this has no effect
     * on the index assigned to the next y-set associated with the same x-set.
     *
     * @param xSet The index of the x-set from which to remove the y-set.
     * @param ySet The index of the y-set to remove.
     */
    void delYSet(int xSet, int ySet);


    /**
     * Returns the currently selected index for the given x-set.
     *
     * @param xSet The x-set for which to get the selected index.
     * @return The currently selected index for the x-set, or 0 if the x-set's
     *         selectable flag is set to false.
     */
    int getSelection(int xSet);


    /**
     * Sets the currently selected index for the given x-set.  This causes
     * the wxMolGraph to be redrawn.  Note that if the x-set's selectable flag
     * is set to false, this function has no effect.
     *
     * @param xSet The x-set for which to set the selected index.
     * @param index The new selected index.
     */
    void setSelection(int xSet, int index);


    /**
     * Sets the lower bound for the given y-axis, adjusts the scale, and then
     * redraws the wxMolGraph.  If the lower bound is greater than or equal to
     * the current upper bound, no scale will be drawn for the axis, and none
     * of the data sets on the axis will be drawn.
     *
     * @param axis Specifies which axis to modify the lower bound of.  Must be
     *             one of MG_AXIS_Y1 or MG_AXIS_Y2.
     * @param val The new lower bound.
     */
    void setYAxisMin(int axis, double val);


    /**
     * Sets the upper bound for the given y-axis, adjusts the scale, and then
     * redraws the wxMolGraph.  If the upper bound is less than or equal to the
     * current lower bound, no scale will be drawn for the axis, and none of
     * the data sets on the axis will be drawn.
     *
     * @param axis Specifies which axis to modify the upper bound of.  Must be
     *             one of MG_AXIS_Y1 or MG_AXIS_Y2.
     * @param val The new upper bound.
     */
    void setYAxisMax(int axis, double val);


    /**
     * Adjusts the upper and lower bounds of the given y-axis and adjusts the
     * scale to show all data graphed on the axis.  After adjusting, the
     * wxMolGraph is redrawn.
     *
     * @param axis Specifies which y-axis to auto-scale.  Must be one of
     *             MG_AXIS_Y1 or MG_AXIS_Y2.
     */
    void autoScaleY(int axis);


    /**
     * Sets the label for the given axis.
     *
     * @param axis The axis for which to set the label.  Must be one of
     *             MG_AXIS_X, MG_AXIS_Y1, or MG_AXIS_Y2.
     * @param label The new label for the axis.
     */
    void setAxisLabel(int axis, wxString &label);


    /**
     * Changes the scale offset for the given y-axis.  An offset of X will
     * cause the zero point for the offset scale to be placed at X on the
     * actual scale.  This causes the wxMolGraph to be redrawn.
     *
     * @param axis Specifies which y-axis for which to modify the offset.  Must
     *             be one of MG_AXIS_Y1 or MG_AXIS_Y2.
     * @param offset The new offset.
     */
    void setOffsetY(int axis, double offset);


    /**
     * Resets the wxMolGraph to its initial state.  Axis information, such as
     * min/max and offset, is reset, all sets are removed, and
     * the indices of newly added sets will restart at zero.
     */
    void reset();


    /**
     * Overloaded wxControl member function that handles sizing of the control.
     *
     * @return The size the control wants to be.
     */
    wxSize DoGetBestSize() const;


    /* EVENT HANDLERS */

    void onSize(wxSizeEvent &event);
    void onPaint(wxPaintEvent &event);

    void onLeftClick(wxMouseEvent &event);
    void onLeftDblClick(wxMouseEvent &event);
    void onMotion(wxMouseEvent &event);
    void onLeaveWindow(wxMouseEvent &event);


    DECLARE_EVENT_TABLE();
};

#endif /* _WX_MOL_GRAPH_H_ */

