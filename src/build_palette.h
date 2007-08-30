#ifndef BUILD_PALETTE_H
#define BUILD_PALETTE_H

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

class Structure {
	friend std::ostream& operator<<(std::ostream& stream, const Structure& s);

	public:
		mpAtom *atoms;
		int natoms;
		Bond *bonds;
		int nbonds;
		wxString name;

		Structure();
		~Structure();

		void WriteXML(XMLElement *parent) const;
		bool ReadXML(XMLElement *struc_el);

	private:
};

/* ------------------------------------------------------------------------- */
/* CLASSES                                                                   */
/* ------------------------------------------------------------------------- */

class BuilderDlg: public wxMiniFrame {    

	public:
		BuilderDlg(
			const wxString& title = _("Periodic Table"), int xpos = 100,
			int ypos = 100);
		~BuilderDlg();

		void New(wxCommandEvent& WXUNUSED(event));
		int GetSelectedElement(void) const;
		Structure *GetSelectedStructure(void);
		int GetNumStructures(void);
		Structure *GetStructure(int i);
		short GetSelectedCoordination(void) const;
		short GetSelectedLonePairCount(void) const;
		static void NumberToTableCoords(int atomic_number, int *row, int *col);
		void KeyHandler(wxKeyEvent &event);
		bool InStructuresMode(void) const;
		bool InPeriodicMode(void) const;
		void AddStructure(Structure *structure);

	private:
		void ElementSelected(wxCommandEvent& event);
		void OnCoordinationChoice(wxCommandEvent& event);
		void OnLPChoice(wxCommandEvent& event);
		void MouseMoved(wxMouseEvent& event);
		void OnClose(wxCloseEvent& event);
		wxPanel *GetPeriodicPanel(void);
		wxPanel *GetStructuresPanel(void);

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
		wxPanel *periodic_panel;
		wxPanel *structures_panel;
		wxChoice *mStructureChoice;
		std::vector<Structure *> structures;

		wxStaticText *element_label;
		wxStaticText *coord_num_label;
		wxStaticText *lp_num_label;


	DECLARE_DYNAMIC_CLASS(BuilderDlg)
	DECLARE_EVENT_TABLE()

};

#endif
