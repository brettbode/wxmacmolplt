#include <iostream>

#include "wx/sizer.h"
#include <wx/stdpaths.h>

#include "build_palette.h"
#include "main.h"

extern bool show_build_palette;
extern WinPrefs *gPreferences;
extern BuilderDlg *build_palette;

#define kPeriodicCoordinationChoice 13800
#define kPeriodicLPChoice			13801
#define kPeriodicSaveStructures     13802
#define kPeriodicSaveStructuresAs   13803
#define kPeriodicLoadStructures     13804
#define kPeriodicDeleteStructure    13805
#define kPeriodicNotebookID         13806
#define kPeriodicStructureChoice    13807
#define kPeriodicRenameStructure    13808

IMPLEMENT_DYNAMIC_CLASS(BuilderDlg, wxMiniFrame)

BEGIN_EVENT_TABLE(BuilderDlg, wxMiniFrame)
	EVT_CHOICE(kPeriodicStructureChoice, BuilderDlg::OnStructureChoice)
	EVT_CHOICE(kPeriodicCoordinationChoice, BuilderDlg::OnCoordinationChoice)
	EVT_CHOICE(kPeriodicLPChoice, BuilderDlg::OnLPChoice)
	EVT_COMMAND_RANGE(0, kNumTableElements - 1, wxEVT_COMMAND_BUTTON_CLICKED,
					  BuilderDlg::ElementSelected)
	EVT_BUTTON(kPeriodicSaveStructures, BuilderDlg::SaveStructures)
	EVT_BUTTON(kPeriodicSaveStructuresAs, BuilderDlg::SaveStructuresAs)
	EVT_BUTTON(kPeriodicLoadStructures, BuilderDlg::LoadStructures)
	EVT_BUTTON(kPeriodicDeleteStructure, BuilderDlg::DeleteStructure)
	EVT_BUTTON(kPeriodicRenameStructure, BuilderDlg::RenameStructure)
	EVT_CHAR(BuilderDlg::KeyHandler)
	EVT_CLOSE(BuilderDlg::OnClose)
	EVT_UPDATE_UI(kPeriodicSaveStructuresAs, BuilderDlg::UpdateSaveStructuresAs)
	EVT_UPDATE_UI(kPeriodicSaveStructures, BuilderDlg::UpdateSaveStructures)
	EVT_UPDATE_UI(kPeriodicDeleteStructure, BuilderDlg::UpdateDeleteStructure)
	EVT_UPDATE_UI(kPeriodicRenameStructure, BuilderDlg::UpdateRenameStructure)
	EVT_NOTEBOOK_PAGE_CHANGED(kPeriodicNotebookID, BuilderDlg::TabChanged)
END_EVENT_TABLE()

// --------------------------------------------------------------------------- 
// FUNCTIONS
// --------------------------------------------------------------------------- 

BuilderDlg::BuilderDlg(const wxString& title,
					   int xpos, int ypos) :
	wxMiniFrame(NULL, wxID_ANY, title, wxPoint(xpos, ypos),
		wxSize(-1, -1), wxCLOSE_BOX | wxCAPTION) {

	/* Structure *new_structure = new Structure; */
	/* new_structure->atoms = new mpAtom[1]; */
	/* new_structure->atoms[0].Type = 6; */
	/* new_structure->atoms[0].Position = CPoint3D(0.0f, 0.0f, 0.0f); */
	/* new_structure->name = wxString(_("Carbon Atom")); */
	/* new_structure->natoms = 1; */
	/* structures.push_back(new_structure); */

	canvas = NULL;

	wxBoxSizer *box_sizer = new wxBoxSizer(wxVERTICAL);
	tabs = new wxNotebook(this, kPeriodicNotebookID, wxPoint(-1, -1),
						  wxSize(-1, -1));

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

	wxStandardPathsBase& gStdPaths = wxStandardPaths::Get();
#if wxCHECK_VERSION(2, 8, 0)
	wxString pathname = gStdPaths.GetResourcesDir();
#else
	wxString pathname = gStdPaths.GetDataDir();
#ifdef __WXMAC__
	//wxWidgets has a funny idea of where the resources are stored. It locates
	//them as "SharedSupport" but xcode is putting them in Resources.
	pathname.Remove(pathname.Length() - 13);
	pathname += wxT("Resources");
#endif
#endif
#ifdef __WXMSW__
	pathname += wxT("\\system_structures.cml");
#else
	pathname += wxT("/system_structures.cml");
#endif

	LoadStructuresFromFile(pathname);
	nglobal_structures = structures.size();
	structures_dirty = false;

	if (nglobal_structures) {
		mStructureChoice->SetSelection(0);
	}

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
			new wxBitmapButton(periodic_panel, i, *(elements[i].off_bmp),
					wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW | wxWANTS_CHARS);
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
/**
 * This function encapsulates the creation of a panel that displays a 
 * library of system- and user-defined molecular structures.  A structure in
 * the library can be interactively previewed or deleted, and the user
 * library can be saved to a file.
 */

wxPanel *BuilderDlg::GetStructuresPanel(void) {

	wxPanel *panel = new wxPanel(tabs, wxID_ANY);

	struc_sizer = new wxGridBagSizer(3);

	int lflags = wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL;
	int rflags = wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL;
	int cflags = wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL;

	struc_sizer->SetFlexibleDirection(wxBOTH);
	struc_sizer->SetCols(3);
	struc_sizer->SetRows(7);
	wxStaticText *label =
		new wxStaticText(panel, wxID_ANY, wxT("Structure: "));
	
	mStructureChoice =
		new wxChoice(panel, kPeriodicStructureChoice);//, wxPoint(-1, -1),
					 /* wxSize(-1, -1), 1, &structures[0]->name); */
	/* mStructureChoice->SetSelection(0); */

	wxStaticText *label2 =
		new wxStaticText(panel, wxID_ANY, wxT("User Structures File: "));

	struc_filename_label =
		new wxStaticText(panel, wxID_ANY, wxT("None"));

	wxButton *save_button =
		new wxButton(panel, kPeriodicSaveStructures, _("Save Structures"));

	wxButton *save_as_button =
		new wxButton(panel, kPeriodicSaveStructuresAs, _("Save Structures As"));

	wxButton *load_button =
		new wxButton(panel, kPeriodicLoadStructures, _("Load Structures"));

	wxButton *delete_button =
		new wxButton(panel, kPeriodicDeleteStructure, _("Delete Structure"));

	wxButton *rename_button =
		new wxButton(panel, kPeriodicRenameStructure, _("Rename Structure"));

	// It seems more sensible to just add the canvas directly to the grid
	// bag sizer, but GTK requires us to hide its parent widget before adding
	// it.  But hiding the structure panel causes some page change events
	// in the notebook that we want to avoid.  So, we create an intermediate
	// panel and add the canvas to that.
	canvas_panel = new wxPanel(panel, wxID_ANY);
	canvas_panel_sizer = new wxFlexGridSizer(1, 1, 0, 0);
	canvas_panel_sizer->AddGrowableCol(0);
	canvas_panel_sizer->AddGrowableRow(0);
	canvas_panel_sizer->SetFlexibleDirection(wxBOTH);
	canvas_panel->SetSizerAndFit(canvas_panel_sizer);

	struc_sizer->Add(label, wxGBPosition(0, 0), wxGBSpan(1, 1), rflags);
	struc_sizer->Add(mStructureChoice, wxGBPosition(0, 1), wxGBSpan(1, 1), lflags);

	struc_sizer->Add(label2, wxGBPosition(1, 0), wxGBSpan(1, 1), rflags);
	struc_sizer->Add(struc_filename_label, wxGBPosition(1, 1), wxGBSpan(1, 1), lflags);

	struc_sizer->Add(save_button, wxGBPosition(0, 2), wxGBSpan(1, 1), wxEXPAND);
	struc_sizer->Add(save_as_button, wxGBPosition(1, 2), wxGBSpan(1, 1), wxEXPAND);
	struc_sizer->Add(load_button, wxGBPosition(2, 2), wxGBSpan(1, 1), wxEXPAND);
	struc_sizer->Add(delete_button, wxGBPosition(3, 2), wxGBSpan(1, 1), wxEXPAND);
	struc_sizer->Add(rename_button, wxGBPosition(4, 2), wxGBSpan(1, 1), wxEXPAND);

	struc_sizer->Add(canvas_panel, wxGBPosition(2, 0), wxGBSpan(5, 2), wxEXPAND);

	struc_sizer->AddGrowableCol(0, 1);
	struc_sizer->AddGrowableCol(1, 1);
	struc_sizer->AddGrowableRow(5, 1);

	panel->SetSizerAndFit(struc_sizer);

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

	delete[] elements;

	std::vector<Structure *>::iterator struc;
	for (struc = structures.begin(); struc != structures.end(); struc++) {
		delete *struc;
	}

	// element_label->Disconnect(wxEVT_CHAR, 
		// wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this); 
	// coord_num_label->Disconnect(wxEVT_CHAR, 
		// wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this); 
	// lp_num_label->Disconnect(wxEVT_CHAR, 
		// wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this); 
	/* mTextArea->Disconnect(wxEVT_CHAR, */
		/* wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this); */
	/* mLPChoice->Disconnect(wxEVT_CHAR, */
		/* wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this); */
	/* mCoordinationChoice->Disconnect(wxEVT_CHAR, */
		/* wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this); */

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

/* ------------------------------------------------------------------------- */

void BuilderDlg::OnStructureChoice(wxCommandEvent& event) {

	canvas->SetStructure(structures[event.GetSelection()]);

}

/* ------------------------------------------------------------------------- */

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

/* ------------------------------------------------------------------------- */

int BuilderDlg::GetSelectedElement(void) const {

	/* This function returns the atomic number of the currently selected atom,
	 * or 0 if no atom is selected. */

	return prev_id + 1;

}

/* ------------------------------------------------------------------------- */

Structure *BuilderDlg::GetSelectedStructure(void) const {

	if (mStructureChoice->GetSelection() == wxNOT_FOUND) {
		return NULL;
	} else {
		return structures[mStructureChoice->GetSelection()];
	}

}

/* ------------------------------------------------------------------------- */

short BuilderDlg::GetSelectedCoordination(void) const {
	short result = 0;
	if (prev_id >= 0) result = coordinationNumber[prev_id];
	return result;
}

/* ------------------------------------------------------------------------- */

short BuilderDlg::GetSelectedLonePairCount(void) const {
	short result = 0;
	if (prev_id >= 0) result = LonePairCount[prev_id];
	return result;
}

/* ------------------------------------------------------------------------- */

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

/* ------------------------------------------------------------------------- */

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

/* ------------------------------------------------------------------------- */

bool BuilderDlg::InPeriodicMode(void) const {

	return tabs->GetCurrentPage() == periodic_panel;

}

/* ------------------------------------------------------------------------- */

bool BuilderDlg::InStructuresMode(void) const {

	return tabs->GetCurrentPage() == structures_panel;

}

/* ------------------------------------------------------------------------- */

Structure::Structure() {

	atoms = NULL;
	bonds = NULL;
	natoms = 0;
	nbonds = 0;

}

/* ------------------------------------------------------------------------- */

Structure::~Structure() {

	if (atoms) delete[] atoms;
	if (bonds) delete[] bonds;

}

/* ------------------------------------------------------------------------- */

void BuilderDlg::AddStructure(Structure *structure) {

	int i;
	Bond *bond;

	// In the case where the user drops the structure in to a bonding site,
	// we strip off the last atom of the structure and connect the clicked
	// on bonding site to the one released by stripping off the last atom.  So,
	// first we figure out what atom has the released bonding site -- it's the 
	// one whose bond with the deleted atom was formed last.
	/* for (i = structure->nbonds - 1; i >= 0; i--) { */
		/* bond = &structure->bonds[i]; */
		/* if (bond->Atom1 == structure->natoms - 1) { */
			/* structure->link_atom = bond->Atom2; */
			/* break; */
		/* } else if (bond->Atom2 == structure->natoms - 1) { */
			/* structure->link_atom = bond->Atom1; */
			/* break; */
		/* } */
	/* } */

	// Now we figure out what the bonding site number of unpaired atom is.
	/* structure->link_site = 0; */
	/* for (i = 0; i < structure->nbonds; i++) { */
		/* bond = &structure->bonds[i]; */
		/* if (bond->Atom1 == structure->link_atom || */
			/* bond->Atom2 == structure->link_atom) { */
			/* structure->link_site++; */
		/* } */
	/* } */
	for (i = structure->natoms - 1; i >= 0; i--) {
		if (structure->atoms[i].Type == 1) {
			structure->SetPruneAtom(i);
			break;
		}
	}

	// std::cout << "*structure: " << *structure << std::endl; 
	structures.push_back(structure);

	mStructureChoice->Append(structure->name);

	// Under GTK at least, the menu doesn't expand to accommodate the new,
	// possibly longer label.  We force that expansion now.
	mStructureChoice->SetSize(mStructureChoice->GetBestSize());

	// Select the just added structure.
	mStructureChoice->SetSelection(mStructureChoice->GetCount() - 1);

	if (canvas) {
		canvas->SetStructure(structures[structures.size() - 1]);
	}

	structures_dirty = true;

}

/* ------------------------------------------------------------------------- */

int BuilderDlg::GetNumStructures() const {
	
	return structures.size();

}

/* ------------------------------------------------------------------------- */

Structure *BuilderDlg::GetStructure(int i) const {

	if (i < 0 || i >= structures.size()) {
		return NULL;
	}

	return structures[i];

}

/* ------------------------------------------------------------------------- */

int BuilderDlg::GetNumUserStructures() const {
	
	return structures.size() - nglobal_structures;

}

/* ------------------------------------------------------------------------- */

Structure *BuilderDlg::GetUserStructure(int i) const {

	// Check that is in valid range for user structures.
	if (i < 0 || i >= structures.size() - nglobal_structures) {
		return NULL;
	}

	return structures[i + nglobal_structures];

}

/* ------------------------------------------------------------------------- */

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

void BuilderDlg::SaveStructuresAs(wxCommandEvent& event) {

	wxString filename;
	filename = wxFileSelector(wxT("Save As"), wxT(""), wxT(""), wxT(""),
							  wxT("CML Files (*.cml)|*.cml"),
							  wxSAVE | wxOVERWRITE_PROMPT, this);

	if (!filename.IsEmpty()) {
		struc_filename = filename;
		SaveStructures(event);
	}

}

// --------------------------------------------------------------------------- 

void BuilderDlg::SaveStructures(wxCommandEvent& event) {

	FILE *save_file = NULL;
	BufferFile *buffer = NULL;

	if (!struc_filename.IsEmpty()) {
		save_file = fopen(struc_filename.mb_str(wxConvUTF8), "w");
		if (save_file == NULL) {
			MessageAlert("Unable to access the file.");
			return;
		}

		buffer = new BufferFile(save_file, true);
		WriteCMLFile(buffer);

		if (buffer) {
			delete buffer;
		}
		fclose(save_file);

		structures_dirty = false;
	}

}

// --------------------------------------------------------------------------- 

void BuilderDlg::LoadStructures(wxCommandEvent& event) {

	int i;
	int response = wxYES;
	wxString filename;

	// Before we load the new user structures in, we may want to remove any
	// previously loaded user structures from both the structures dropdown
	// menu and the vector.  We let the user decide if they want to replace
	// or append.
	if (structures.size() > nglobal_structures) {
		response = wxMessageBox(_("Replace the current structures?"),
								_("Load Structures"), wxYES_NO | wxCANCEL,
								this);

		if (response == wxYES) {
			if (nglobal_structures) {
				canvas->SetStructure(structures[0]);
			} else {
				canvas->SetStructure(NULL);
			}
			for (i = nglobal_structures; i < structures.size(); i++) {
				delete structures[i];
			}
			structures.erase(structures.begin() + nglobal_structures,
							 structures.end());
			for (i = mStructureChoice->GetCount() - 1;
				 i >= nglobal_structures; i--) {
				mStructureChoice->Delete(i);
			}

			// Apparently the selection has to be done after the deletion, 
			// otherwise it won't stick.
			if (nglobal_structures) {
				mStructureChoice->SetSelection(0);
			} else {
				mStructureChoice->SetSelection(wxNOT_FOUND);
			}

			struc_filename_label->SetLabel(_("None"));
		} else if (response == wxCANCEL) {
			return;
		}
	}

	filename = wxFileSelector(wxT("Open File"), wxT(""), wxT(""),
							  wxT(""), wxT("CML Files (*.cml)|*.cml"));

	// If a file wasn't selected or can't be opened, let's not change anything.
	if (filename.IsEmpty()) {
		return;
	}

	int prev_size = structures.size();
	bool success = LoadStructuresFromFile(filename);

	structures_dirty = (response != wxYES) && (structures.size() != prev_size);

	if (success && response == wxYES) {
		struc_filename = filename;
		struc_filename_label->SetLabel(struc_filename.AfterLast('/'));
	}


}

/* ------------------------------------------------------------------------- */
/**
 * This function loads structures from the file indicated by the specified
 * pathname.
 * @param filename The pathname to the structures file.
 */

bool BuilderDlg::LoadStructuresFromFile(const wxString& filename) {

	FILE *load_file = NULL;
	BufferFile *buffer = NULL;

	load_file = fopen(filename.mb_str(wxConvUTF8), "r");
	if (load_file == NULL) {
		MessageAlert("Unable to access the file.");
		return false;
	}

	buffer = new BufferFile(load_file, false);
	int result = ReadCMLFile(buffer);

	if (buffer) {
		delete buffer;
	}

	fclose(load_file);

	return result == 0;

}

/* ------------------------------------------------------------------------- */
/**
 * This function updates the Save Structures As button's enabled state.  It is
 * only enabled when at least one user-defined structure has been created.
 * @param event The update event sent in by wxWidgets.
 */

void BuilderDlg::UpdateSaveStructuresAs(wxUpdateUIEvent& event) {

	event.Enable(GetNumUserStructures() > 0 || !struc_filename.IsEmpty());

}

/* ------------------------------------------------------------------------- */
/**
 * This function updates the Save Structures button's enabled state.  It is
 * only enabled when the structures come from an existing file and have been
 * changed in some way.
 * @param event The update event sent in by wxWidgets.
 */

void BuilderDlg::UpdateSaveStructures(wxUpdateUIEvent& event) {

	event.Enable(!struc_filename.IsEmpty() && structures_dirty);

}

// --------------------------------------------------------------------------- 
/**
 * This function updates the Delete Structure button's enabled state.  It is
 * only enabled when a non-system structure is selected.
 * @param event The update event sent in by wxWidgets.
 */

void BuilderDlg::UpdateDeleteStructure(wxUpdateUIEvent& event) {

	event.Enable(mStructureChoice->GetSelection() >= nglobal_structures);

}

/* ------------------------------------------------------------------------- */

void BuilderDlg::UpdateRenameStructure(wxUpdateUIEvent& event) {

	event.Enable(mStructureChoice->GetSelection() >= nglobal_structures);

}

// --------------------------------------------------------------------------- 
/**
 * This function handles clicks on the Delete Structure button.  The current
 * non-system structure is deleted and the first structure in the list is
 * selected in its place.
 * @param event The click event sent in by wxWidgets.
 */

void BuilderDlg::DeleteStructure(wxCommandEvent& event) {

	int id = mStructureChoice->GetSelection();

	if (id < nglobal_structures) return;

	delete structures[id];
	structures.erase(structures.begin() + id);
	mStructureChoice->Delete(id);
	if (mStructureChoice->GetCount() == 0) {
		mStructureChoice->SetSelection(wxNOT_FOUND);
		canvas->SetStructure(NULL);
	} else {
		id = (id == 0) ? 0 : id - 1;
		mStructureChoice->SetSelection(id);
		canvas->SetStructure(structures[id]);
	}

	structures_dirty = true;

}

/* ------------------------------------------------------------------------- */

void BuilderDlg::RenameStructure(wxCommandEvent& event) {

	int id = mStructureChoice->GetSelection();

	if (id < nglobal_structures) return;

	wxString new_name = wxGetTextFromUser(_("Enter structure name:"),
										  _("Rename Structure"),
										  _(""), this);

	if (!new_name.IsEmpty()) {
		structures[id]->name = new_name;
		mStructureChoice->SetString(id, structures[id]->name);
		structures_dirty = true;
	}

}

/* ------------------------------------------------------------------------- */
/**
 * This function handles page change events for the builder palette notebook.
 * It exists so the Structures panel can have a dynamically generated OpenGL
 * canvas.  The alternative is to create the canvas statically and let it sit
 * around and take up memory.  To avoid this, we create and destroy it as
 * needed in this function.
 * @param event The notebook's page changed event.
 */

void BuilderDlg::TabChanged(wxNotebookEvent& event) {

	if (event.GetOldSelection() == 1 && canvas) {
		delete canvas;
		canvas = NULL;
	}

	if (event.GetSelection() == 1) {
		/* GTK has a problem with gtk_widget_set_colormap being called within
		 * wx.  GTK wants it called before the widget is made, but wx calls 
		 * it afterward.  So, we work around that by hiding the parent and
		 * showing it again.  In the hiding, we must also reselect the tab
		 * since the selection is lost. */
		canvas_panel->Hide();
		int attrs[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
		canvas = new PreviewCanvas(canvas_panel, wxID_ANY, attrs);
		canvas_panel_sizer->Add(canvas, wxSizerFlags().Expand());
		canvas_panel_sizer->Layout();
		canvas_panel->Show();

		if (event.GetSelection() == 1) {
			if (structures.size()) {
				canvas->SetStructure(structures[mStructureChoice->GetSelection()]);
			}
			canvas->Render();
		}
	}

}

/* ------------------------------------------------------------------------- */

void BuilderDlg::StructuresSaveCheck() {

	if (structures_dirty) {
		Show();

		if (!struc_filename.IsEmpty()) {
			int r = wxMessageBox(wxT("The structures file ") + struc_filename +
								 wxT(" has unsaved changes.  Do you wish to "
									 "save the changes to this file before "
									 "closing?"), wxT("Structures Modified"),
								 wxICON_QUESTION | wxYES_NO, this);
			if (r == wxYES) {
				wxCommandEvent event;
				SaveStructures(event);
			}
		} else {
			int r = wxMessageBox(wxT("The structures have unsaved changes. Do "
									 "you wish to save these changes before "
									 "closing?"), wxT("Structures Modified"),
								 wxICON_QUESTION | wxYES_NO, this);
			if (r == wxYES) {
				wxCommandEvent event;
				SaveStructuresAs(event);
			}
		}
	}

}

/* ------------------------------------------------------------------------- */
/**
 * This function sets the atom within the structure that should be removed
 * when the structure's dropped in at a bonding site.  The caller of this
 * function should make sure that atom_id refers to a hydrogen atom as this
 * function makes no such check.
 * @param atom_id Index into atoms array of atom to prune.
 */

void Structure::SetPruneAtom(int atom_id) {

	int i;

	// Do nothing if id is out of range.
	if (atom_id < 0 || atom_id >= natoms) {
		return;
	}

	atom_to_prune = atom_id;

	// Find the atom the hydrogen is bonded to.  Since it's a hydrogen atom
	// we're pruning, there should only be one bond for it, and the atom to
	// link to the bonding site should be on the opposite end.
	link_atom = -1;
	for (i = 0; i < nbonds && link_atom == -1; i++) {
		if (bonds[i].Atom1 == atom_to_prune) {
			link_atom = bonds[i].Atom2;
		} else if (bonds[i].Atom2 == atom_to_prune) {
			link_atom = bonds[i].Atom1;
		}
	}

	// Now we figure out what the bonding site number of opposite atom is.
	link_site = 0;
	for (i = 0; i < nbonds; i++) {
		if (bonds[i].Atom1 == link_atom || bonds[i].Atom2 == link_atom) {
			link_site++;
		}
	}

}

/* ------------------------------------------------------------------------- */

