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

#include "Globals.h"
////@end includes

class colorArea : public wxPanel
{
 public:
  colorArea(wxWindow* parent, int i);
  colorArea(wxWindow* parent, int i, RGBColor* color);
  ~colorArea();
  int getId() { return mID; }
  void draw(RGBColor*);
  wxColour& getColor();
  void OnMouse(wxMouseEvent &event);

 private:
  int mID;
  wxColour mCurrentColor;
  RGBColor* mColorPtr;

  wxWindow* mParent;

  DECLARE_EVENT_TABLE()
};

#endif
