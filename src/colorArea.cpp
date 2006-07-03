/*****************************************
*  colorArea.cpp
*****************************************/

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "colorArea.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "wx/colordlg.h"

#include "colorArea.h"
#include "surfaceDlg.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

wxColour RGB2WX(RGBColor &c);
RGBColor WX2RGB(wxColour &c);

BEGIN_EVENT_TABLE(colorArea, wxPanel)
    EVT_MOUSE_EVENTS(colorArea::OnMouse)
END_EVENT_TABLE()

colorArea::colorArea(wxWindow* parent, int i)
{
  Create(parent, -1, wxDefaultPosition, wxSize(50, 20));

  mID = i;
  mParent = parent;
}

colorArea::colorArea(wxWindow* parent, int i, RGBColor* color)
{
  Create(parent, -1, wxDefaultPosition, wxSize(50, 20));

  mID = i;
  mColorPtr = color;
  mParent = parent;
}

colorArea::~colorArea()
{
  //delete mDC;
}

void colorArea::draw(RGBColor* color)
{
  SetBackgroundColour(wxColour(color->red/256, color->green/256, color->blue/256));
  mCurrentColor = wxColour(color->red/256, color->green/256, color->blue/256);

  Update();
}

wxColour& colorArea::getColor()
{
  return mCurrentColor;
}

void colorArea::OnMouse(wxMouseEvent &event)
{
  wxColourData clrData;
  clrData.SetColour(GetBackgroundColour());

  if (event.LeftDClick())
    {
      wxColourDialog dialog(this, &clrData);
      dialog.SetTitle(_T("Choose Color"));

      if (dialog.ShowModal() == wxID_OK)
	{
	  mCurrentColor = dialog.GetColourData().GetColour();
	  SetBackgroundColour(mCurrentColor);

	  RGBColor tmpRGBColor = WX2RGB(mCurrentColor);
	  mColorPtr->red = tmpRGBColor.red;
	  mColorPtr->green = tmpRGBColor.green;
	  mColorPtr->blue = tmpRGBColor.blue;

	  Update();

	  if (mID == ID_2D_COLOR_POSITIVE || mID == ID_2D_COLOR_NEGATIVE
	      || mID == ID_3D_COLOR_POSITIVE || mID == ID_3D_COLOR_NEGATIVE
	      || mID == ID_TRANSPARENCY_COLOR)
	    (dynamic_cast<BaseSurfacePane*>(mParent))->setUpdateButton();
	}
    }
}
