/* ------------------------------------------------------------------------- */
/* FILE:   periodic_table_dlg.cpp                                            */
/* AUTHOR: Chris Johnson                                                     */
/* DATE:   Jan 24 2007                                                       */
/*                                                                           */
/*                                                                           */
/* ------------------------------------------------------------------------- */

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "periodic_table_dlg.h"
#endif

#include "periodic_table_dlg.h"

IMPLEMENT_DYNAMIC_CLASS(PeriodicTableDlg, wxMiniFrame)

BEGIN_EVENT_TABLE(PeriodicTableDlg, wxMiniFrame)
	EVT_CLOSE(PeriodicTableDlg::OnClose)
	EVT_BUTTON(wxID_ANY, PeriodicTableDlg::ElementSelected)
END_EVENT_TABLE()

/* ------------------------------------------------------------------------- */
/* FUNCTIONS                                                                 */
/* ------------------------------------------------------------------------- */

PeriodicTableDlg::PeriodicTableDlg() {
}

/* ------------------------------------------------------------------------- */

PeriodicTableDlg::PeriodicTableDlg(
		MpGLCanvas *parent, const wxChar *title, int xpos, int ypos) {
	Create(parent, title, xpos, ypos);
}

/* ------------------------------------------------------------------------- */

bool PeriodicTableDlg::Create(
		MpGLCanvas *parent, const wxChar *title, int xpos, int ypos) {

	wxFont *font;            // Font of atomic symbol
	wxMemoryDC *mem_dc;      // Offscreen renderer for button bitmaps
	wxCoord symbol_width;    // Width of atomic symbol in font
	wxCoord symbol_height;   // Height of atomic symbol in font
	int i;
	int row;
	int col;
	int font_size;
	int platform_width_offset;
	int platform_height_offset;

#ifdef __WXMAC__
#define BUTTON_SIZE 26
#define IMAGE_SIZE 26
	platform_height_offset = 22;
	platform_width_offset = 0;
	font_size = 12;
#else
#define BUTTON_SIZE 34
#define IMAGE_SIZE 20
	platform_height_offset = 22;
	platform_width_offset = 5;
	font_size = 10;
#endif

	wxMiniFrame::Create(parent, wxID_ANY, title, wxPoint(xpos, ypos),
		wxSize(BUTTON_SIZE * 18 + platform_width_offset,
			BUTTON_SIZE * 10 + platform_height_offset),
		wxCLOSE_BOX);

	nelements = 112;

	this->parent = parent;

	/* No element has been selected yet. */
	prev_id = -1;

	elements = (element_t *) malloc(sizeof(element_t) * nelements);
	if (elements == NULL) {
		fprintf(stderr, "[%s:%d] Couldn't make room for buttons.\n",
			__FILE__, __LINE__);
		perror("");
		exit(1);
	}

	/* All button images are generated dynamically.  So, we need a font and an
	 * offscreen renderer. */
	mem_dc = new wxMemoryDC();
	font = new wxFont(font_size, wxSWISS, wxFONTSTYLE_NORMAL,
		wxFONTWEIGHT_NORMAL, false, wxT(""), wxFONTENCODING_SYSTEM);
	mem_dc->SetFont(*font);

	for (i = 0; i < nelements; i++) {
		wxString symbol;

		parent->GetPrefs()->GetAtomLabel(i, symbol);

		NumberToTableCoords(i + 1, &row, &col);

		elements[i].on_bmp = new wxBitmap(IMAGE_SIZE, IMAGE_SIZE);
		elements[i].off_bmp = new wxBitmap(IMAGE_SIZE, IMAGE_SIZE);

		// Create unselected version with white background, black text.
		mem_dc->SelectObject(*(elements[i].off_bmp));
		mem_dc->GetTextExtent(symbol, &symbol_width, &symbol_height);
		mem_dc->SetBackground(*wxWHITE_BRUSH);
		mem_dc->Clear();
		mem_dc->SetTextForeground(*wxBLACK);
		mem_dc->DrawText(symbol,
			(IMAGE_SIZE - symbol_width) / 2,
			(IMAGE_SIZE - symbol_height) / 2);

		// Create selected version with black background, white text.
		mem_dc->SelectObject(*(elements[i].on_bmp));
		mem_dc->SetBackground(*wxBLACK_BRUSH);
		mem_dc->Clear();
		mem_dc->SetTextForeground(*wxWHITE);
		mem_dc->DrawText(symbol,
			(IMAGE_SIZE - symbol_width) / 2,
			(IMAGE_SIZE - symbol_height) / 2);

		elements[i].button =
			new wxBitmapButton(this, i, *(elements[i].off_bmp),
			wxPoint(col * BUTTON_SIZE, row * BUTTON_SIZE),
			wxSize(BUTTON_SIZE, BUTTON_SIZE));

		// Display a quick note when user hovers over button.
		wxString tool_tip;
		tool_tip.Printf(wxT("Number: %d, Mass: %f"), i + 1,
			parent->GetPrefs()->GetAtomMass(i));
		elements[i].button->SetToolTip(tool_tip);

	}

	delete font;
	delete mem_dc;

	return true;

}

/* ------------------------------------------------------------------------- */

PeriodicTableDlg::~PeriodicTableDlg() {
	int i;

	for (i = 0; i < nelements; i++) {
		delete elements[i].button;
		delete elements[i].off_bmp;
		delete elements[i].on_bmp;
	}

	free(elements);
}

/* ------------------------------------------------------------------------- */

void PeriodicTableDlg::ElementSelected(wxCommandEvent& event) {

	/* This function is called when one of the element buttons is pushed.  
	 * The selected button's image is inverted since wxBitmapButton's do not
	 * hold state.  We internally track the button's state and invert and
	 * revert images accordingly. */

	int id;                /* The atomic number and index of element. */

	id = event.GetId();

	// If the user selects a new button, we want to turn the old one off and
	// the new one one.
	if (id != prev_id) {
		elements[id].button->SetBitmapLabel(*(elements[id].on_bmp));
		if (prev_id >= 0) {
			elements[prev_id].button->SetBitmapLabel(*(elements[prev_id].off_bmp));
		}
		prev_id = id;
	}

	// If the user has selected the same button, we want to turn it off.
   	else {
		elements[id].button->SetBitmapLabel(*(elements[id].off_bmp));
		prev_id = -1;
	}

	Refresh();

}

/* ------------------------------------------------------------------------- */

int PeriodicTableDlg::GetSelectedID(void) {

	/* This function returns the atomic number of the currently selected atom,
	 * or 0 if no atom is selected. */

	return prev_id + 1;

}

/* ------------------------------------------------------------------------- */

void PeriodicTableDlg::OnClose(wxCloseEvent& event) {
	parent->ClosePeriodicDlg();
}

/* ------------------------------------------------------------------------- */

void PeriodicTableDlg::NumberToTableCoords(int atomic_number, int *row,
														 int *col) {

	/* This function calculates the row and column position within the periodic
	 * table of the element indicated by atomic_number.  This information
	 * could be pushed out to preferences file, but this works for now. */

	int i;

	if (atomic_number <= 2) {
		*row = 0;
		*col = (atomic_number - 1) * 17;
	} else if (atomic_number <= 18) {
		*row = 1 + (atomic_number - 3) / 8;
		i = (atomic_number - 3) % 8;
		if (i % 8 <= 1) {
			*col = i;
		} else {
			*col = i + 10;
		}
	} else if (atomic_number <= 57) {
		*row = 3 + (atomic_number - 19) / 18;;
		*col = (atomic_number - 19) % 18;
	} else if (atomic_number <= 71) {
		*row = 8;
		*col = 3 + (atomic_number - 58);
	} else if (atomic_number <= 86) {
		*row = 5 + (atomic_number - 72) / 18;
		*col = 3 + (atomic_number - 72) % 15;
	} else if (atomic_number <= 89) {
		*row = 6;
		*col = atomic_number - 87;
	} else if (atomic_number <= 103) {
		*row = 9;
		*col = 3 + (atomic_number - 90);
	} else if (atomic_number <= 112) {
		*row = 6 + (atomic_number - 104) / 18;
		*col = 3 + (atomic_number - 104) % 15;
	}

}

/* ------------------------------------------------------------------------- */

