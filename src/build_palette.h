#ifndef _PERIODIC_TABLE_DLG_H_
#define _PERIODIC_TABLE_DLG_H_

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/bitmap.h"
#include "wx/image.h"
#include "wx/dcmemory.h"
#include "wx/brush.h"
#include "wx/minifram.h"
#include "wx/notebook.h"

#include <stdio.h>

#include "Globals.h"
#ifndef __MyTypes__
#include "MyTypes.h"
#endif
#include "Prefs.h"

#define kNumTableElements	112

/* ------------------------------------------------------------------------- */
/* TYPES                                                                     */
/* ------------------------------------------------------------------------- */

typedef struct {
	wxBitmapButton *button;
	wxBitmap *off_bmp;
	wxBitmap *on_bmp;
} element_t;

/* ------------------------------------------------------------------------- */
/* CLASSES                                                                   */
/* ------------------------------------------------------------------------- */

class PeriodicTableDlg: public wxMiniFrame {    

	public:
		PeriodicTableDlg(
			const wxString& title = _("Periodic Table"), int xpos = 100,
			int ypos = 100);
		~PeriodicTableDlg();

		void New(wxCommandEvent& WXUNUSED(event));
		int GetSelectedID(void) const;
		short GetSelectedCoordination(void) const;
		short GetSelectedLonePairCount(void) const;
		static void NumberToTableCoords(int atomic_number, int *row, int *col);
		void KeyHandler(wxKeyEvent &event);

	private:
		void ElementSelected(wxCommandEvent& event);
		void OnCoordinationChoice(wxCommandEvent& event);
		void OnLPChoice(wxCommandEvent& event);
		void MouseMoved(wxMouseEvent& event);
		void OnClose(wxCloseEvent& event);
		wxPanel *GetPeriodicPanel(void);

		short coordinationNumber[kNumTableElements];
		short LonePairCount[kNumTableElements];
		int nelements;
		int prev_id;
		unsigned char keyBuffer[3];
		wxStopWatch	secondKeytimer;
		wxStaticText *mTextArea;
		wxChoice *mCoordinationChoice;
		wxChoice *mLPChoice;
		element_t *elements;
		wxNotebook *tabs;

	DECLARE_DYNAMIC_CLASS(PeriodicTableDlg)
	DECLARE_EVENT_TABLE()

};

#endif
