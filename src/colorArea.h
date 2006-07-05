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

////@end includes
wxColour RGB2WX(const RGBColor &c);
RGBColor WX2RGB(const wxColour &c);

class colorArea : public wxPanel
{
 public:
//  colorArea(wxWindow* parent, int i);
  colorArea(wxWindow* parent, int i, const RGBColor* color);
  ~colorArea();
  int getId() { return mID; }
//  void draw(RGBColor*);
  void draw(void);
  const wxColour& getColor(void) const;
  void getColor(RGBColor * color) const;
  void setColor(const RGBColor * color);
	// function that handles the actual color change
	// A wxEVT_COMMAND_ENTER event is generated if the color is changed.
  void OnMouse(wxMouseEvent &event);

 private:
  int mID;
  wxColour mCurrentColor;
//  RGBColor mColorPtr;

  DECLARE_EVENT_TABLE()
};

#endif
