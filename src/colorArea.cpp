/*****************************************
*  colorArea.cpp
*****************************************/

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "colorArea.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "Globals.h"
#include "wx/wxprec.h"
#include "wx/colordlg.h"

#include "colorArea.h"
#include "surfaceDlg.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

BEGIN_EVENT_TABLE(colorArea, wxPanel)
    EVT_MOUSE_EVENTS(colorArea::OnMouse)
END_EVENT_TABLE()

colorArea::colorArea(wxWindow* parent, int id, const RGBColor* color)
{
  Create(parent, id, wxDefaultPosition, wxSize(50, 20), wxSUNKEN_BORDER);

  mCurrentColor = RGB2WX(*color);
  SetBackgroundColour(mCurrentColor);
}

colorArea::~colorArea()
{
  //delete mDC;
}

void colorArea::draw(void)
{
  SetBackgroundColour(mCurrentColor);

  Update();
}

void colorArea::setColor(const RGBColor * color) {
	mCurrentColor = RGB2WX(*color);
	SetBackgroundColour(mCurrentColor);
	Refresh();
}

const wxColour& colorArea::getColor() const
{
  return mCurrentColor;
}

void colorArea::getColor(RGBColor * color) const {
	RGBColor tmpRGBColor = WX2RGB(mCurrentColor);
	color->red = tmpRGBColor.red;
	color->green = tmpRGBColor.green;
	color->blue = tmpRGBColor.blue;
}

void colorArea::OnMouse(wxMouseEvent &event)
{
	wxColourData clrData;
	clrData.SetColour(GetBackgroundColour());

	if (event.LeftDClick()) {
		wxColourDialog dialog(this, &clrData);
		dialog.SetTitle(_T("Choose Color"));

		if (dialog.ShowModal() == wxID_OK) {
			mCurrentColor = dialog.GetColourData().GetColour();
			SetBackgroundColour(mCurrentColor);

			Refresh();

			wxCommandEvent evt(wxEVT_COMMAND_ENTER, GetId());
			wxPostEvent(this, evt);
		}
	}
}
