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
#include "main.h"

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
	EVT_SET_FOCUS(PeriodicTableDlg::OnFocusGained)
	EVT_KILL_FOCUS(PeriodicTableDlg::OnFocusLost)
	EVT_CLOSE(PeriodicTableDlg::OnClose)
END_EVENT_TABLE()

/* ------------------------------------------------------------------------- */
/* FUNCTIONS                                                                 */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */

PeriodicTableDlg::PeriodicTableDlg(const wxString& title,
								   int xpos, int ypos) :
#ifdef __WXMAC__
#define BUTTON_SIZE 26
	wxMiniFrame(NULL, wxID_ANY, title, wxPoint(xpos, ypos),
		wxSize(BUTTON_SIZE * 18, BUTTON_SIZE * 10 + 22),
		wxCLOSE_BOX | wxCAPTION)
#else
#define BUTTON_SIZE 34
	wxMiniFrame(NULL, wxID_ANY, title, wxPoint(xpos, ypos),
		wxSize(BUTTON_SIZE * 18 + 5, BUTTON_SIZE * 10 + 22),
		wxCLOSE_BOX | wxCAPTION)
#endif
{

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
	for (int i=0; i<kNumTableElements; i++) {
		coordinationNumber[i] = lcoordination[i];
		LonePairCount[i] = lLPCount[i];
	}
#ifdef __WXMAC__
#define IMAGE_SIZE 26
	font_size = 12;
#else
#define IMAGE_SIZE 20
	font_size = 10;
#endif

	nelements = kNumTableElements;

	/* No element has been selected yet. */
	prev_id = -1;
	keyBuffer[0] = keyBuffer[1] = keyBuffer[2] = '\0';

	// elements = (element_t *) malloc(sizeof(element_t) * nelements); 
	elements = new element_t[nelements];
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
			new wxBitmapButton(this, i, *(elements[i].off_bmp),
			wxPoint(col * BUTTON_SIZE, row * BUTTON_SIZE),
			wxSize(BUTTON_SIZE, BUTTON_SIZE));

		// Display a quick note when user hovers over button.
		wxString tool_tip;
		tool_tip.Printf(wxT("Atomic #: %d, Mass: %f, Oxidation #: %d"), i + 1,
			gPreferences->GetAtomMass(i),
			gPreferences->GetOxidationNumber(i + 1));
		elements[i].button->SetToolTip(tool_tip);

	}

	delete font;
	delete mem_dc;
	
	wxStaticText * label1 = new wxStaticText(this, wxID_ANY, wxT("Selected Element:"), 
											 wxPoint(BUTTON_SIZE*2.5, 3));
	wxSize label1Size = label1->GetSize();
	wxPoint label1Pos = label1->GetPosition();
	mTextArea = new wxStaticText(this, wxID_ANY, wxT("foo"), 
								 wxPoint(label1Pos.x+label1Size.GetWidth() + 2, label1Pos.y));
	
	wxStaticText * label2 = new wxStaticText(this, wxID_ANY, wxT("Coordination number:"), 
											 wxPoint(label1Pos.x, label1Pos.y+label1Size.GetHeight()+8));
	wxSize label2Size = label2->GetSize();
	wxPoint label2Pos = label2->GetPosition();
	wxString coordinationChoices[] = {_T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6")};
	mCoordinationChoice = new wxChoice(this, kPeriodicCoordinationChoice,
									   wxPoint(label2Pos.x+label2Size.GetWidth() + 2, label2Pos.y),
									   wxSize(-1,-1), 7, coordinationChoices);
	wxStaticText * label3 = new wxStaticText(this, wxID_ANY, wxT("Number of lone pairs:"), 
											 wxPoint(label1Pos.x, label2Pos.y+label2Size.GetHeight()+8));
	wxSize label3Size = label3->GetSize();
	wxPoint label3Pos = label3->GetPosition();
	wxString LPChoices[] = {_T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5")};
	mLPChoice = new wxChoice(this, kPeriodicLPChoice,
									   wxPoint(label2Pos.x+label3Size.GetWidth() + 2, label3Pos.y),
									   wxSize(-1,-1), 6, LPChoices);
	
	wxCommandEvent foo(0, 6-1);
	ElementSelected(foo);
}

/* ------------------------------------------------------------------------- */

PeriodicTableDlg::~PeriodicTableDlg() {
	int i;

	for (i = 0; i < nelements; i++) {
		delete elements[i].button;
		delete elements[i].off_bmp;
		delete elements[i].on_bmp;
	}

	// free(elements); 
	delete elements;

	periodic_dlg = NULL;
	show_periodic_dlg = false;
	((MpApp &) wxGetApp()).AdjustAllMenus();
}

/* ------------------------------------------------------------------------- */

void PeriodicTableDlg::ElementSelected(wxCommandEvent& event) {

	/* This function is called when one of the element buttons is pushed.  
	 * The selected button's image is inverted since wxBitmapButton's do not
	 * hold state.  We internally track the button's state and invert and
	 * revert images accordingly. */

	int id;                /* The atomic number and index of element. */

	id = event.GetId();

	if (id < 0 || id >= kNumTableElements) {
		event.Skip();
		return;
	}

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
void PeriodicTableDlg::OnCoordinationChoice(wxCommandEvent& event) {
	coordinationNumber[prev_id] = event.GetSelection();
}
void PeriodicTableDlg::OnLPChoice(wxCommandEvent& event) {
	LonePairCount[prev_id] = event.GetSelection();
}
#include "myFiles.h"
void PeriodicTableDlg::KeyHandler(wxKeyEvent & event) {
	if (!event.HasModifiers()) {
		int key = event.GetKeyCode();
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
		}
	}
}	
/* ------------------------------------------------------------------------- */

int PeriodicTableDlg::GetSelectedID(void) const {

	/* This function returns the atomic number of the currently selected atom,
	 * or 0 if no atom is selected. */

	return prev_id + 1;

}

short PeriodicTableDlg::GetSelectedCoordination(void) const {
	short result = 0;
	if (prev_id >= 0) result = coordinationNumber[prev_id];
	return result;
}

short PeriodicTableDlg::GetSelectedLonePairCount(void) const {
	short result = 0;
	if (prev_id >= 0) result = LonePairCount[prev_id];
	return result;
}

/* ------------------------------------------------------------------------- */

void PeriodicTableDlg::OnClose(wxCloseEvent& event) {
//	parent->ClosePeriodicDlg();

	if (event.CanVeto()) {
		show_periodic_dlg = false;
		((MpApp &) wxGetApp()).AdjustAllMenus();
		Hide();
		event.Veto();
	} else {
		Destroy();
	}
}

/* ------------------------------------------------------------------------- */

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

void PeriodicTableDlg::OnFocusGained(wxFocusEvent& event) {
	// std::cout << "gained focus" << std::endl; 
}

// --------------------------------------------------------------------------- 

void PeriodicTableDlg::OnFocusLost(wxFocusEvent& event) {
	// std::cout << "lost focus" << std::endl; 
}

// --------------------------------------------------------------------------- 

bool PeriodicTableDlg::AcceptsFocus(void) const {
	// std::cout << "called acceptsfocus()" << std::endl; 
	return true;
}

// --------------------------------------------------------------------------- 

