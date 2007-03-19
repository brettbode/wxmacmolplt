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

/*!
 * Includes
 */

////@begin includes
#include "wx/wx.h"
#include "PrefsPanes.h"

////@end includes
// Useful global functions to convert between RGBColor and wxColour
wxColour RGB2WX(const RGBColor &c);
RGBColor WX2RGB(const wxColour &c);

class colorArea : public wxPanel
{
public:
	colorArea(wxWindow* parent, int id, const RGBColor* color);
	virtual ~colorArea();
	virtual void draw(void);
	const wxColour& getColor(void) const;
	void getColor(RGBColor * color) const;
	void setColor(const RGBColor * color);
	void setColor(const wxColour* color);

	void setPeer(colorArea* peer) { mPeer = peer; }

	// function that handles the actual color change
	// A wxEVT_COMMAND_ENTER event is generated if the color is changed.
	void OnMouse(wxMouseEvent &event);

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
  colorPatternArea(wxWindow* parent, int id, const RGBColor* color, int patID = -1);
  virtual ~colorPatternArea();

  virtual void draw(void);
  void OnPaint(wxPaintEvent &event);

 private:
  wxBitmap* mPattern;
  int patID;

  DECLARE_EVENT_TABLE()
};


#endif
