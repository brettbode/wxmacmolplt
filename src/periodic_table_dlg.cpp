#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "periodic_table_dlg.h"
#endif

#include "periodic_table_dlg.h"
#include "main.h"
#include <wx/gbsizer.h>

extern bool show_periodic_dlg;
extern WinPrefs * gPreferences;
extern PeriodicTableDlg *periodic_dlg;

#define kPeriodicCoordinationChoice 13800
#define kPeriodicLPChoice			13801

IMPLEMENT_DYNAMIC_CLASS(PeriodicTableDlg, wxMiniFrame)

BEGIN_EVENT_TABLE(PeriodicTableDlg, wxMiniFrame)
	EVT_CHOICE(kPeriodicCoordinationChoice, PeriodicTableDlg::OnCoordinationChoice)
	EVT_CHOICE(kPeriodicLPChoice, PeriodicTableDlg::OnLPChoice)
	EVT_BUTTON(wxID_ANY, PeriodicTableDlg::ElementSelected)
	EVT_CHAR(PeriodicTableDlg::KeyHandler)
	EVT_CLOSE(PeriodicTableDlg::OnClose)
END_EVENT_TABLE()

// --------------------------------------------------------------------------- 
// FUNCTIONS
// --------------------------------------------------------------------------- 

PeriodicTableDlg::PeriodicTableDlg(const wxString& title,
								   int xpos, int ypos) :
	wxMiniFrame(NULL, wxID_ANY, title, wxPoint(xpos, ypos),
		wxSize(-1, -1), wxCLOSE_BOX | wxCAPTION) {

	wxPanel *panel = new wxPanel(this);
	wxBoxSizer *box_sizer = new wxBoxSizer(wxVERTICAL);
	tabs = new wxNotebook(panel, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));

	wxPanel *rings_panel = new wxPanel(tabs);
	wxPanel *solvents_panel = new wxPanel(tabs);

	tabs->AddPage(GetPeriodicPanel(), _("Periodic Table"), true);
	tabs->AddPage(rings_panel, _("Rings"), false);
	tabs->AddPage(solvents_panel, _("Solvents"), false);

	box_sizer->Add(tabs);
	// box_sizer->Fit(this); 
	panel->SetSizerAndFit(box_sizer);

	Fit();

	wxCommandEvent foo(0, 6-1);
	ElementSelected(foo);

}

// --------------------------------------------------------------------------- 

wxPanel *PeriodicTableDlg::GetPeriodicPanel(void) {

	wxFont *font;            // Font of atomic symbol
	wxMemoryDC *mem_dc;      // Offscreen renderer for button bitmaps
	wxCoord symbol_width;    // Width of atomic symbol in font
	wxCoord symbol_height;   // Height of atomic symbol in font
	int i;
	int row;
	int col;
	int font_size;
	short lcoordination[] = {1,                                0,
							 1,2,                    3,4,3,2,1,0,
							 1,2,                    3,4,5,6,1,0,
							 1,2,3,2,5,3,2,3,2,2,2,2,3,4,3,4,1,0,
							 1,2,3,4,5,6,6,3,3,2,1,2,3,4,3,4,1,0,
							 1,2,
								3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
								 4,5,6,6,4,4,4,3,2,1,2,3,2,1,0,
							 1,2,
								3,4,5,6,5,4,3,3,3,3,3,3,3,3,3,
								 4,0,6,0,3,0,0,0,0};
	short lLPCount[] = {0,                                0,
						0,0,                    0,0,1,2,3,4,
						0,0,                    0,0,0,0,3,4,
						0,0,0,1,0,2,3,3,3,2,2,2,0,0,1,0,3,4,
						0,0,0,0,0,0,0,3,3,2,2,2,0,0,1,0,3,4,
						0,0,
						   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							  0,0,0,0,0,0,0,0,0,0,0,1,2,3,0,
						0,0,
						   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
							  0,0,0,0,0,0,0,0,0};

	wxPanel *periodic_panel = new wxPanel(tabs, wxID_ANY);
	wxGridBagSizer *sizer = new wxGridBagSizer();
	
	for (int i=0; i<kNumTableElements; i++) {
		coordinationNumber[i] = lcoordination[i];
		LonePairCount[i] = lLPCount[i];
	}

#ifdef __WXMAC__
#define IMAGE_SIZE 20
	font_size = 12;
#else
#define IMAGE_SIZE 23
	font_size = 10;
#endif

	nelements = kNumTableElements;

	// No element has been selected yet.
	prev_id = -1;
	keyBuffer[0] = keyBuffer[1] = keyBuffer[2] = '\0';

	elements = new element_t[nelements];
	if (elements == NULL) {
		fprintf(stderr, "[%s:%d] Couldn't make room for buttons.\n",
			__FILE__, __LINE__);
		perror("");
		exit(1);
	}

	// All button images are generated dynamically.  So, we need a font and an
	// offscreen renderer.
	mem_dc = new wxMemoryDC();
	font = new wxFont(font_size, wxSWISS, wxFONTSTYLE_NORMAL,
		wxFONTWEIGHT_NORMAL, false, wxT(""), wxFONTENCODING_SYSTEM);
	mem_dc->SetFont(*font);

	for (i = 0; i < nelements; i++) {
		wxString symbol;

		gPreferences->GetAtomLabel(i, symbol);

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
			new wxBitmapButton(periodic_panel, i, *(elements[i].off_bmp));

		sizer->Add(elements[i].button, wxGBPosition(row, col));

		// Display a quick note when user hovers over button.
		wxString tool_tip;
		tool_tip.Printf(wxT("Atomic #: %d, Mass: %f"), i + 1,
			gPreferences->GetAtomMass(i));
		elements[i].button->SetToolTip(tool_tip);

	}

	delete font;
	delete mem_dc;

	int lflags = wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL;
	int rflags = wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL;
	
	// Show atomic symbol.
	wxStaticText * label1 = new wxStaticText(periodic_panel, wxID_ANY, wxT("Selected Element: "));
	sizer->Add(label1, wxGBPosition(0, 2), wxGBSpan(1, 6), lflags);

	mTextArea = new wxStaticText(periodic_panel, wxID_ANY, wxT("foo"));
	sizer->Add(mTextArea, wxGBPosition(0, 8), wxGBSpan(1, 4), rflags);

	// Show coordination number drop down menu.
	wxStaticText * label2 = new wxStaticText(periodic_panel, wxID_ANY, wxT("Coordination number: "));
	sizer->Add(label2, wxGBPosition(1, 2), wxGBSpan(1, 6), lflags);

	wxString coordinationChoices[] = {_T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6")};
	mCoordinationChoice = new wxChoice(periodic_panel, kPeriodicCoordinationChoice, wxPoint(-1, -1),
									   wxSize(-1, -1), 7, coordinationChoices);
	sizer->Add(mCoordinationChoice, wxGBPosition(1, 8), wxGBSpan(1, 4), rflags);

	// Show lone pair count drop down menu.
	wxStaticText * label3 = new wxStaticText(periodic_panel, wxID_ANY, wxT("Number of lone pairs: "));
	sizer->Add(label3, wxGBPosition(2, 2), wxGBSpan(1, 6), lflags);

	wxString LPChoices[] = {_T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5")};
	mLPChoice = new wxChoice(periodic_panel, kPeriodicLPChoice, wxPoint(-1, -1),
							 wxSize(-1, -1), 6, LPChoices);
	sizer->Add(mLPChoice, wxGBPosition(2, 8), wxGBSpan(1, 4), rflags);

	sizer->Fit(this);
	periodic_panel->SetSizerAndFit(sizer);
	
	return periodic_panel;

}

// --------------------------------------------------------------------------- 

PeriodicTableDlg::~PeriodicTableDlg() {

	int i;

	for (i = 0; i < nelements; i++) {
		delete elements[i].button;
		delete elements[i].off_bmp;
		delete elements[i].on_bmp;
	}

	delete elements;

	periodic_dlg = NULL;
	show_periodic_dlg = false;
	((MpApp &) wxGetApp()).AdjustAllMenus();
}

// --------------------------------------------------------------------------- 

void PeriodicTableDlg::ElementSelected(wxCommandEvent& event) {

	/* This function is called when one of the element buttons is pushed.  
	 * The selected button's image is inverted since wxBitmapButton's do not
	 * hold state.  We internally track the button's state and invert and
	 * revert images accordingly. */

	int id;                /* The atomic number - 1 and index of element. */

	id = event.GetId();

	if (id < 0 || id >= kNumTableElements) {
		event.Skip();
		return;
	}

	// If the user selects a new button, we want to turn the old one off and
	// the new one on.
	if (id != prev_id) {
		elements[id].button->SetBitmapLabel(*(elements[id].on_bmp));
		if (prev_id >= 0) {
			elements[prev_id].button->SetBitmapLabel(*(elements[prev_id].off_bmp));
		}
		prev_id = id;
		elements[id].button->SetFocus();
	}

	// If the user has selected the same button, we want to turn it off.
//   	else {
//		elements[id].button->SetBitmapLabel(*(elements[id].off_bmp));
//		prev_id = -1;
//	}
	if (prev_id >= 0) {
		wxString symbol;
		gPreferences->GetAtomLabel(prev_id, symbol);
		mTextArea->SetLabel(symbol);
		
		mCoordinationChoice->SetSelection(coordinationNumber[prev_id]);
		mLPChoice->SetSelection(LonePairCount[prev_id]);
	}
	
	Refresh();

}

// --------------------------------------------------------------------------- 

void PeriodicTableDlg::OnCoordinationChoice(wxCommandEvent& event) {
	coordinationNumber[prev_id] = event.GetSelection();
}

// --------------------------------------------------------------------------- 

void PeriodicTableDlg::OnLPChoice(wxCommandEvent& event) {
	LonePairCount[prev_id] = event.GetSelection();
}

// --------------------------------------------------------------------------- 

#include "myFiles.h"
void PeriodicTableDlg::KeyHandler(wxKeyEvent& event) {
	int key = event.GetKeyCode();
	if (!event.HasModifiers()) {
		if (isalpha(key)) {
			int id = -1;
			if (secondKeytimer.Time() < 400) {
				keyBuffer[1] = key;
				//if less than three seconds try to interpert as the 2nd letter of a two letter element symbol
				id = SetAtomType(keyBuffer);
			}
			if (id < 0) {	//interpert as the first letter of an element symbol
				keyBuffer[0] = key;
				keyBuffer[1] = '\0';
				id = SetAtomType(keyBuffer);
				secondKeytimer.Start();	//start the timer for a 2nd keystroke
			}
			if (id > 0) {
				wxCommandEvent foo(0, id-1);
				ElementSelected(foo);
			}
		} else if (key == 309) {
		}
	} else {
		if (key == 23) {
			Close();
		}
	}
}	

// --------------------------------------------------------------------------- 

int PeriodicTableDlg::GetSelectedID(void) const {

	/* This function returns the atomic number of the currently selected atom,
	 * or 0 if no atom is selected. */

	return prev_id + 1;

}

// --------------------------------------------------------------------------- 

short PeriodicTableDlg::GetSelectedCoordination(void) const {
	short result = 0;
	if (prev_id >= 0) result = coordinationNumber[prev_id];
	return result;
}

// --------------------------------------------------------------------------- 

short PeriodicTableDlg::GetSelectedLonePairCount(void) const {
	short result = 0;
	if (prev_id >= 0) result = LonePairCount[prev_id];
	return result;
}

// --------------------------------------------------------------------------- 

void PeriodicTableDlg::OnClose(wxCloseEvent& event) {

	// If possible, we want to try to hide the periodic dialog rather than
	// fully close it.
	if (event.CanVeto()) {
		show_periodic_dlg = false;
		((MpApp &) wxGetApp()).AdjustAllMenus();
		Hide();
		event.Veto();
	} else {
		Destroy();
	}
}

// --------------------------------------------------------------------------- 

void PeriodicTableDlg::NumberToTableCoords(int atomic_number,
										   int *row, int *col) {

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

// --------------------------------------------------------------------------- 

