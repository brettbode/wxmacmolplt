/*
 *  (c) 2006 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	colorArea.h
*/

#ifndef __ColorArea__
#define __ColorArea__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "colorArea.h"
#endif

#define PATTERN_SELECT_DIALOG 30001
#define PATTERN_SELECT_TITLE _("Select a pattern")
#define ID_BITMAP_SLT 30010
#define ID_OLD_PAT 30100
#define ID_NEW_PAT 30101
        
/*!
 * Includes
 */

////@begin includes
#include "wx/wx.h"
#include "PrefsPanes.h"
#include "patterns.h"

////@end includes
// Useful global functions to convert between RGBColor and wxColour
wxColour RGB2WX(const RGBColor &c);
RGBColor WX2RGB(const wxColour &c);

class colorArea : public wxPanel
{
public:
	colorArea(wxWindow* parent, int id, const RGBColor* color, int w = 50, int h = 20);
	virtual ~colorArea();
	virtual void draw(void);
	const wxColour& getColor(void) const;
	void getColor(RGBColor * color) const;
	void setColor(const RGBColor * color);
	void setColor(const wxColour* color);

	void setPeer(colorArea* peer) { mPeer = peer; }

	// function that handles the actual color change
	// A wxEVT_COMMAND_ENTER event is generated if the color is changed.
	virtual void OnMouse(wxMouseEvent &event);

protected:
	colorArea* mPeer;
	wxColour mCurrentColor;

	int mWidth;
	int mHeight;
	int mID;

	DECLARE_EVENT_TABLE()
};

class colorPatternArea : public colorArea
{
public:
  colorPatternArea(wxWindow* parent, int id, const RGBColor* color, int patID = -1, int w = 50, int h = 20);
  virtual ~colorPatternArea();

  virtual void draw(void);
  virtual void OnPaint(wxPaintEvent &event);
  virtual void OnMouse(wxMouseEvent &event);
  void reset();
  void setPattern(int patID);
  int getPattern() { return mPatID; }

 private:
  wxBitmap* mPattern;
  int mPatID;
  wxWindow* mParent;

  DECLARE_EVENT_TABLE()
};

class patternSelectDlg : public wxDialog {

  public:
    patternSelectDlg() {}
    patternSelectDlg( colorPatternArea * parent, wxWindowID id = PATTERN_SELECT_DIALOG, const wxString& caption = PATTERN_SELECT_TITLE);
    ~patternSelectDlg();

    void OnOK( wxCommandEvent& WXUNUSED(event) );

    void setSltId(int id);
    int getSltId() { return mSltId; }
    int getSltPatId() {return mSltPatId;}

  private:
    void Create(colorPatternArea * parent, wxWindowID id, const wxString& caption);

    wxBoxSizer *mainSizer, *lowerSizer, *upperSizer, *upperLeftSizer;
    wxGridSizer* innerSizer;
    wxButton *mButtOK, *mButtCancel;
    colorPatternArea* patSlt[kNumPatterns];
    colorPatternArea  *oldPat, *newPat;
    int mSltId;
    int mSltPatId;
    colorPatternArea * mParent;

    DECLARE_EVENT_TABLE()
 };


#endif
