#include "build_palette.h"
#include "main.h"
#include <iostream>

#include <wx/gbsizer.h>

extern bool show_build_palette;
extern WinPrefs * gPreferences;
extern BuilderDlg *build_palette;

#define kPeriodicCoordinationChoice 13800
#define kPeriodicLPChoice			13801

IMPLEMENT_DYNAMIC_CLASS(BuilderDlg, wxMiniFrame)

BEGIN_EVENT_TABLE(BuilderDlg, wxMiniFrame)
	EVT_CHOICE(kPeriodicCoordinationChoice, BuilderDlg::OnCoordinationChoice)
	EVT_CHOICE(kPeriodicLPChoice, BuilderDlg::OnLPChoice)
	EVT_BUTTON(wxID_ANY, BuilderDlg::ElementSelected)
	EVT_CHAR(BuilderDlg::KeyHandler)
	EVT_CLOSE(BuilderDlg::OnClose)
END_EVENT_TABLE()

// --------------------------------------------------------------------------- 
// FUNCTIONS
// --------------------------------------------------------------------------- 

BuilderDlg::BuilderDlg(const wxString& title,
					   int xpos, int ypos) :
	wxMiniFrame(NULL, wxID_ANY, title, wxPoint(xpos, ypos),
		wxSize(-1, -1), wxCLOSE_BOX | wxCAPTION) {

	Structure *new_structure = new Structure;
	mpAtom *atom = new mpAtom;
	atom->Type = 8;
	atom->Position = CPoint3D(0.0f, 0.0f, 0.0f);
	new_structure->name = wxString(_("Carbon Atom"));
	new_structure->atoms = atom;
	new_structure->natoms = 1;
	structures.push_back(new_structure);

	nglobal_structures = structures.size();

	wxBoxSizer *box_sizer = new wxBoxSizer(wxVERTICAL);
	tabs = new wxNotebook(this, wxID_ANY, wxPoint(-1, -1), wxSize(-1, -1));

	wxPanel *solvents_panel = new wxPanel(tabs);

	periodic_panel = GetPeriodicPanel();
	structures_panel = GetStructuresPanel();

	tabs->AddPage(periodic_panel, _("Periodic Table"), true);
	tabs->AddPage(structures_panel, _("Structures"), false);
	tabs->AddPage(solvents_panel, _("Solvents"), false);

	box_sizer->Add(tabs);
	SetSizerAndFit(box_sizer);

	wxCommandEvent foo(0, 6-1);
	ElementSelected(foo);

}

// --------------------------------------------------------------------------- 

wxPanel *BuilderDlg::GetPeriodicPanel(void) {

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
	periodic_panel->Connect(wxEVT_CHAR,
							wxKeyEventHandler(BuilderDlg::KeyHandler),
							NULL, this);
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
		elements[i].button->Connect(wxEVT_CHAR,
									wxKeyEventHandler(BuilderDlg::KeyHandler),
									NULL, this);

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
	
#if 1
	// Show atomic symbol.
	wxStaticText *element_label = new wxStaticText(periodic_panel, wxID_ANY, wxT("Selected Element: "));
	sizer->Add(element_label, wxGBPosition(0, 2), wxGBSpan(1, 6), lflags);
	// element_label->Connect(wxEVT_CHAR, 
		// wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this); 

	mTextArea = new wxStaticText(periodic_panel, wxID_ANY, wxT("foo"));
	sizer->Add(mTextArea, wxGBPosition(0, 8), wxGBSpan(1, 4), rflags);
	mTextArea->Connect(wxEVT_CHAR,
		wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this);

	// Show coordination number drop down menu.
	wxStaticText *coord_num_label = new wxStaticText(periodic_panel, wxID_ANY, wxT("Coordination number: "));
	sizer->Add(coord_num_label, wxGBPosition(1, 2), wxGBSpan(1, 6), lflags);
	// coord_num_label->Connect(wxEVT_CHAR, 
		// wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this); 

	wxString coordinationChoices[] = {_T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6")};
	mCoordinationChoice = new wxChoice(periodic_panel, kPeriodicCoordinationChoice, wxPoint(-1, -1),
									   wxSize(-1, -1), 7, coordinationChoices);
	sizer->Add(mCoordinationChoice, wxGBPosition(1, 8), wxGBSpan(1, 4), rflags);
	mCoordinationChoice->Connect(wxEVT_CHAR,
		wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this);

	// Show lone pair count drop down menu.
	wxStaticText *lp_num_label = new wxStaticText(periodic_panel, wxID_ANY, wxT("Number of lone pairs: "));
	sizer->Add(lp_num_label, wxGBPosition(2, 2), wxGBSpan(1, 6), lflags);
	// lp_num_label->Connect(wxEVT_CHAR, 
		// wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this); 

	wxString LPChoices[] = {_T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5")};
	mLPChoice = new wxChoice(periodic_panel, kPeriodicLPChoice, wxPoint(-1, -1),
							 wxSize(-1, -1), 6, LPChoices);
	sizer->Add(mLPChoice, wxGBPosition(2, 8), wxGBSpan(1, 4), rflags);
	mLPChoice->Connect(wxEVT_CHAR,
		wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this);
#endif

	sizer->Fit(this);
	periodic_panel->SetSizerAndFit(sizer);
	
	return periodic_panel;

}

// --------------------------------------------------------------------------- 

wxPanel *BuilderDlg::GetStructuresPanel(void) {

	// wxPanel *panel2 = new wxPanel(tabs, wxID_ANY); 
	wxPanel *panel = new wxPanel(tabs, wxID_ANY);
	wxGridBagSizer *sizer = new wxGridBagSizer();

	int lflags = wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL;
	int rflags = wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL;

	sizer->SetFlexibleDirection(wxHORIZONTAL);
	sizer->SetCols(2);
	sizer->SetRows(1);
	sizer->AddGrowableCol(0, 1);
	sizer->AddGrowableCol(1, 1);
	
	wxStaticText *label =
		new wxStaticText(panel, wxID_ANY, wxT("Structure: "));
	sizer->Add(label, wxGBPosition(0, 0), wxGBSpan(1, 1), lflags);
	
	mStructureChoice = new wxChoice(panel, wxID_ANY, wxPoint(-1, -1),
									wxSize(-1, -1), 1,
									&structures[0]->name);
	mStructureChoice->SetSelection(0);

	sizer->Add(mStructureChoice, wxGBPosition(0, 1), wxGBSpan(1, 1), rflags);

	panel->SetSizerAndFit(sizer);
	
	return panel;

}

// --------------------------------------------------------------------------- 

BuilderDlg::~BuilderDlg() {

	int i;

	for (i = 0; i < nelements; i++) {
		elements[i].button->Disconnect(wxEVT_CHAR,
			wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this);
		delete elements[i].button;
		delete elements[i].off_bmp;
		delete elements[i].on_bmp;
	}

	delete elements;

	// element_label->Disconnect(wxEVT_CHAR, 
		// wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this); 
	// coord_num_label->Disconnect(wxEVT_CHAR, 
		// wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this); 
	// lp_num_label->Disconnect(wxEVT_CHAR, 
		// wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this); 
	mTextArea->Disconnect(wxEVT_CHAR,
		wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this);
	mLPChoice->Disconnect(wxEVT_CHAR,
		wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this);
	mCoordinationChoice->Disconnect(wxEVT_CHAR,
		wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this);

	build_palette = NULL;
	show_build_palette = false;
	((MpApp &) wxGetApp()).AdjustAllMenus();
}

// --------------------------------------------------------------------------- 

void BuilderDlg::ElementSelected(wxCommandEvent& event) {

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
// #if defined(__WXGTK__) 
		// On GTK, subsequent keypresses won't get handled to change
		// selected elements because nothing in the frame has focus.  We
		// force focus here.  Doing this under Windows gives the whole
		// frame focus, so we do this for GTK only.

		if (!event.GetInt()) {
			elements[id].button->SetFocus();
		}
// #endif 
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

void BuilderDlg::OnCoordinationChoice(wxCommandEvent& event) {
	coordinationNumber[prev_id] = event.GetSelection();
}

// --------------------------------------------------------------------------- 

void BuilderDlg::OnLPChoice(wxCommandEvent& event) {
	LonePairCount[prev_id] = event.GetSelection();
}

// --------------------------------------------------------------------------- 

#include "myFiles.h"
void BuilderDlg::KeyHandler(wxKeyEvent& event) {
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
				/* TODO: fix this! */
				if (event.GetX() == -50 && event.GetY() == -50) {
					foo.SetInt(1);
				}
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

int BuilderDlg::GetSelectedElement(void) const {

	/* This function returns the atomic number of the currently selected atom,
	 * or 0 if no atom is selected. */

	return prev_id + 1;

}

// --------------------------------------------------------------------------- 

Structure *BuilderDlg::GetSelectedStructure(void) {

	return structures[mStructureChoice->GetSelection()];

}

// --------------------------------------------------------------------------- 

short BuilderDlg::GetSelectedCoordination(void) const {
	short result = 0;
	if (prev_id >= 0) result = coordinationNumber[prev_id];
	return result;
}

// --------------------------------------------------------------------------- 

short BuilderDlg::GetSelectedLonePairCount(void) const {
	short result = 0;
	if (prev_id >= 0) result = LonePairCount[prev_id];
	return result;
}

// --------------------------------------------------------------------------- 

void BuilderDlg::OnClose(wxCloseEvent& event) {

	// If possible, we want to try to hide the periodic dialog rather than
	// fully close it.
	if (event.CanVeto()) {
		show_build_palette = false;
		((MpApp &) wxGetApp()).AdjustAllMenus();
		Hide();
		event.Veto();
	} else {
		Destroy();
	}
}

// --------------------------------------------------------------------------- 

void BuilderDlg::NumberToTableCoords(int atomic_number,
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

bool BuilderDlg::InPeriodicMode(void) const {

	return tabs->GetCurrentPage() == periodic_panel;

}

// --------------------------------------------------------------------------- 

bool BuilderDlg::InStructuresMode(void) const {

	return tabs->GetCurrentPage() == structures_panel;

}

// --------------------------------------------------------------------------- 

Structure::Structure() {

	atoms = NULL;
	bonds = NULL;
	natoms = 0;
	nbonds = 0;

}

// --------------------------------------------------------------------------- 

Structure::~Structure() {

	if (atoms) delete atoms;
	if (bonds) delete bonds;

}

// --------------------------------------------------------------------------- 

void BuilderDlg::AddStructure(Structure *structure) {

	// std::cout << "*structure: " << *structure << std::endl; 
	structures.push_back(structure);

	mStructureChoice->Append(structure->name);

	// Under GTK at least, the menu doesn't expand to accommodate the new,
	// possibly longer label.  We force that expansion now.
	mStructureChoice->SetSize(mStructureChoice->GetBestSize());

}

// --------------------------------------------------------------------------- 

int BuilderDlg::GetNumStructures() {
	
	return structures.size();

}

// --------------------------------------------------------------------------- 

Structure *BuilderDlg::GetStructure(int i) {

	return structures[i];

}

// --------------------------------------------------------------------------- 

int BuilderDlg::GetNumUserStructures() {
	
	return structures.size() - nglobal_structures;

}

// --------------------------------------------------------------------------- 

Structure *BuilderDlg::GetUserStructure(int i) {

	return structures[i + nglobal_structures];

}

// --------------------------------------------------------------------------- 

std::ostream& operator<<(std::ostream& stream, const Structure& s) {

	int i;
	
	std::cout << "------------------------" << std::endl;
	// std::cout << "name: " << s.name.c_str() << std::endl; 
	printf("%ls\n", s.name.c_str());
	std::cout << "natoms: " << s.natoms << std::endl;
	for (i = 0; i < s.natoms; i++) {
		std::cout << "s.atoms[" << i << "]: " << s.atoms[i] << std::endl;
	}
	std::cout << "nbonds: " << s.nbonds << std::endl;
	for (i = 0; i < s.nbonds; i++) {
		std::cout << "s.bonds[" << i << "]: " << s.bonds[i] << std::endl;
	}
	std::cout << "------------------------" << std::endl << std::endl;

	return stream;

}

// --------------------------------------------------------------------------- 

