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

#ifdef __BORLANDC__
#pragma hdrstop
#endif

static const wxColour defaultColor(128,128,128);

BEGIN_EVENT_TABLE(colorArea, wxPanel)
    EVT_MOUSE_EVENTS(colorArea::OnMouse)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(colorPatternArea, wxPanel)
    EVT_MOUSE_EVENTS(colorPatternArea::OnMouse)
    EVT_PAINT  (colorPatternArea::OnPaint)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(patternSelectDlg, wxDialog)
  EVT_BUTTON( wxID_OK, patternSelectDlg::OnOK )
  EVT_SIZE(patternSelectDlg::OnSize)
END_EVENT_TABLE()

colorArea::colorArea(wxWindow* parent, int id, const RGBColor* color,int w, int h): mWidth(w), mHeight(h), mID(id) {
	Create(parent, id, wxDefaultPosition, wxSize(mWidth, mHeight), wxSUNKEN_BORDER);

	mCurrentColor = RGB2WX(*color);
	SetBackgroundColour(mCurrentColor);

	mPeer = NULL;
}

colorArea::~colorArea() {
	//delete mDC;
}

void colorArea::draw(void) {
	SetBackgroundColour(mCurrentColor);

	Update();
}

void colorArea::setColor(const RGBColor * color) {
	mCurrentColor = RGB2WX(*color);
	SetBackgroundColour(mCurrentColor);
	Refresh();
}

void colorArea::setColor(const wxColour* color) {
	mCurrentColor = *color;
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

void colorArea::OnMouse(wxMouseEvent &event) {
	wxColourData clrData;
	clrData.SetColour(GetBackgroundColour());

	if (event.LeftDClick()) {
		wxColourDialog dialog(this, &clrData);
		dialog.SetTitle(_T("Choose Color"));

		if (dialog.ShowModal() == wxID_OK) {
			mCurrentColor = dialog.GetColourData().GetColour();
			SetBackgroundColour(mCurrentColor);

			Refresh();

			if (mPeer)
				mPeer->setColor(&mCurrentColor);

			wxCommandEvent evt(wxEVT_COMMAND_ENTER, GetId());
			wxPostEvent(this, evt);
		}
	}
}
colorPatternArea::colorPatternArea(wxWindow* parent, int id, const RGBColor* color, int patID, int w, int h)
	: colorArea(parent, id, color, w, h) {

		if ((patID>=0)&&(patID<numPatterns))
			mPattern = new wxBitmap((const char*)atomMaskPatterns[patID], 32, 32);
		else
			mPattern = new wxBitmap((const char*)atomMaskPatterns[0], 32, 32);
		Refresh();

		mParent = parent;
		mPatID = patID;
	}

colorPatternArea::~colorPatternArea() {
	delete mPattern;
}

void colorPatternArea::draw() {
	Refresh();
}

void colorPatternArea::reset() {
	SetBackgroundColour(mCurrentColor);
	Refresh();
}

void colorPatternArea::setPattern(int patID) {
	delete mPattern;

	mPatID = patID;
	mPattern = new wxBitmap((const char*)atomMaskPatterns[mPatID], 32, 32);
	Refresh();
}

void colorPatternArea::OnMouse(wxMouseEvent &event) {
	if (mID < numPatterns && event.LeftDClick()) {
		patternSelectDlg selector(this);

		if (selector.ShowModal() == wxOK) {

		}
	}

	if (mID >= ID_BITMAP_SLT && mID < ID_BITMAP_SLT + numPatterns)  //exclude color areas in the left panel
	{

		if ( mID >= ID_BITMAP_SLT && (event.Entering() || mID == dynamic_cast<patternSelectDlg*>(mParent->GetParent())->getSltId()) ) {
			//"distance" of two colors
			if ( pow((float)(mCurrentColor.Red()-defaultColor.Red()),2.0f) + pow((float)(mCurrentColor.Green()-defaultColor.Green()),2.0f) + pow((float)(mCurrentColor.Blue()-defaultColor.Blue()),2.0f) < 500)
				SetBackgroundColour(wxColour(68,68,68));
			else
				SetBackgroundColour(defaultColor);
		}

		if ( event.Leaving() && mID >= ID_BITMAP_SLT && mPatID != dynamic_cast<patternSelectDlg*>(mParent->GetParent())->getSltPatId() )
			SetBackgroundColour(mCurrentColor);

		if (event.LeftDown() && mID >= ID_BITMAP_SLT) {
			dynamic_cast<patternSelectDlg*>(mParent->GetParent())->setSltId(mID);
		}
	}
}

void colorPatternArea::OnPaint(wxPaintEvent &WXUNUSED(event)) {
	wxPaintDC dc(this);
	PrepareDC(dc);

	dc.SetBrush(wxBrush(*mPattern));
	//dc.SetBrush(wxBrush(*wxRED));
	dc.DrawRectangle(0, 0, mWidth, mHeight);
	//dc.DrawBitmap(*mPattern, 0, 0);
}

patternSelectDlg::patternSelectDlg(colorPatternArea * parent, wxWindowID id, const wxString& caption) {
	mParent = parent;
	mSltPatId = parent->getPattern();
	Create(parent, id, caption);
}

void patternSelectDlg::Create(colorPatternArea * parent, wxWindowID id, const wxString& caption ) {
	wxDialog::Create( parent, id, caption, wxDefaultPosition, wxSize(450,500), wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX);

	mainSizer = new wxBoxSizer(wxVERTICAL);
	innerSizer = new wxGridSizer(5, numPatterns/5+1);
	upperSizer = new wxBoxSizer(wxHORIZONTAL);
	upperLeftSizer = new wxBoxSizer(wxVERTICAL);
	upperRightSizer = new wxBoxSizer(wxHORIZONTAL);
	lowerSizer = new wxBoxSizer(wxHORIZONTAL);

	RGBColor tmpColor;

	sltArea = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(415,400), wxVSCROLL|wxEXPAND);
	tmpColor = WX2RGB(parent->getColor());

	for (int i = 0; i < numPatterns; i++) {
		patSlt[i] = new colorPatternArea(sltArea, ID_BITMAP_SLT+i, &tmpColor, i, 64, 64);
		if ( i == parent->getPattern())
			patSlt[i]->SetBackgroundColour(defaultColor);
		//if the pattern is the one already has been set, use another color

		innerSizer->Add(patSlt[i], 0, wxALIGN_CENTRE | wxALL, 10);
	}

	sltArea->SetSizer(innerSizer);
	sltArea->SetScrollRate(10,10);

	oldPat = new colorPatternArea(this, ID_OLD_PAT, &tmpColor, mSltPatId, 64, 64);
	newPat = new colorPatternArea(this, ID_NEW_PAT, &tmpColor, mSltPatId, 64, 64);
	upperLeftSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("Old Pattern:"))) ,0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	upperLeftSizer->Add(oldPat, 0, wxALIGN_CENTRE | wxALL, 10);
	upperLeftSizer->Add(new wxStaticText(this, wxID_ANY, wxString(wxT("New Pattern:"))) ,0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
	upperLeftSizer->Add(newPat, 0, wxALIGN_CENTRE | wxALL, 10);

	upperRightSizer->Add(sltArea, 0, wxALIGN_CENTRE | wxALL, 10);
	upperSizer->Add(upperLeftSizer, 0, wxALIGN_CENTRE | wxALL, 10);
	upperSizer->Add(upperRightSizer, 0, wxALIGN_CENTRE | wxALL, 10);

	mButtOK = new wxButton(this, wxID_OK, wxT("OK") );
	mButtCancel = new wxButton(this, wxID_CANCEL, wxT("Cancel"));

	lowerSizer->Add(150,10);
	lowerSizer->Add(mButtOK, 0, wxALIGN_CENTRE | wxALL, 10);
	lowerSizer->Add(50,10);
	lowerSizer->Add(mButtCancel, 0, wxALIGN_CENTRE | wxALL, 10);

	mainSizer->Add(upperSizer);
	mainSizer->Add(lowerSizer);

	SetSizer(mainSizer);

	mainSizer->Fit(this);
	mainSizer->SetSizeHints(this);
	mainSizer->Layout();

	Centre(wxBOTH);
}

patternSelectDlg::~patternSelectDlg() {
	for ( int i = 0; i < numPatterns; i++)
		delete patSlt[i];
}

void patternSelectDlg::OnOK( wxCommandEvent& event ) {
	mParent->setPattern(mSltPatId);
	event.Skip();
}

void patternSelectDlg::OnSize(wxSizeEvent & event) {
	wxSize s = event.GetSize();
	sltArea->SetSize(MIN(410, s.x-35), s.y-100);

	mainSizer->Layout();
	Refresh(); 
}

void patternSelectDlg::setSltId(int id) {
	mSltId = id;
	mSltPatId = id - ID_BITMAP_SLT;

	for (int i = 0; i < numPatterns; i++)
		patSlt[i]->reset();

	newPat->setPattern(mSltPatId);
}
