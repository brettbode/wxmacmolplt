#include <iostream>

#include "wx/sizer.h"
#include <wx/stdpaths.h>
#include <wx/clntdata.h>

#include "build_palette.h"
#include "main.h"
#include "GlobalExceptions.h"

extern bool show_build_palette;
extern WinPrefs *gPreferences;
extern BuilderDlg *build_palette;

#define kPeriodicCoordinationChoice 13800
#define kPeriodicLPChoice			13801
#define kPeriodicDeleteStructure    13802
#define kPeriodicNotebookID         13803
#define kPeriodicStrucChoice        13804
#define kPeriodicRenameStructure    13805
#define kPeriodicStrucGroups        13806
#define kPeriodicCanvasPanel        13807
#define kPeriodicCanvas             13808

IMPLEMENT_DYNAMIC_CLASS(BuilderDlg, wxMiniFrame)

BEGIN_EVENT_TABLE(BuilderDlg, wxMiniFrame)
	EVT_LISTBOX(kPeriodicStrucChoice, BuilderDlg::OnStructureChoice)
	EVT_CHOICE(kPeriodicCoordinationChoice, BuilderDlg::OnCoordinationChoice)
	EVT_CHOICE(kPeriodicLPChoice, BuilderDlg::OnLPChoice)
	EVT_COMMAND_RANGE(0, kNumTableElements - 1, wxEVT_COMMAND_BUTTON_CLICKED,
					  BuilderDlg::ElementSelected)
	EVT_BUTTON(kPeriodicDeleteStructure, BuilderDlg::DeleteStructure)
	EVT_BUTTON(kPeriodicRenameStructure, BuilderDlg::RenameStructure)
	EVT_CHAR(BuilderDlg::KeyHandler)
	EVT_CLOSE(BuilderDlg::OnClose)
	EVT_UPDATE_UI(kPeriodicDeleteStructure, BuilderDlg::UpdateDeleteStructures)
	EVT_UPDATE_UI(kPeriodicRenameStructure, BuilderDlg::UpdateRenameStructures)
	EVT_NOTEBOOK_PAGE_CHANGED(kPeriodicNotebookID, BuilderDlg::TabChanged)
	EVT_CHOICE(kPeriodicStrucGroups, BuilderDlg::ChangeStructureGroup)
END_EVENT_TABLE()

/* ------------------------------------------------------------------------- */

class StrucGroupClientData : public wxClientData {

	public:
		StrucGroupClientData()
			: wxClientData(), filename(), is_custom(false) {}

		StrucGroupClientData(const wxString& filename, bool is_custom = false)
			: wxClientData(), filename(filename), is_custom(is_custom) {}

		wxString filename;
		bool is_custom;

};

// --------------------------------------------------------------------------- 
// FUNCTIONS
// --------------------------------------------------------------------------- 

BuilderDlg::BuilderDlg(const wxString& title,
					   int xpos, int ypos) :
	wxMiniFrame(NULL, wxID_ANY, title, wxPoint(xpos, ypos),
		wxSize(-1, -1), wxCLOSE_BOX | wxCAPTION) {

	wxStandardPathsBase& gStdPaths = wxStandardPaths::Get();
#if wxCHECK_VERSION(2, 8, 0)
	sys_prefs_path = gStdPaths.GetResourcesDir();
#else
	sys_prefs_path = gStdPaths.GetDataDir();
#ifdef __WXMAC__
	//wxWidgets has a funny idea of where the resources are stored. It locates
	//them as "SharedSupport" but xcode is putting them in Resources.
	sys_prefs_path.Remove(pathname.Length() - 13);
	sys_prefs_path += wxT("Resources");
#endif
#endif

	canvas = NULL;

	wxBoxSizer *box_sizer = new wxBoxSizer(wxVERTICAL);
	tabs = new wxNotebook(this, kPeriodicNotebookID, wxPoint(-1, -1),
						  wxSize(-1, -1));

	periodic_panel = GetPeriodicPanel();
	structures_panel = GetStructuresPanel();

	tabs->AddPage(periodic_panel, _("Periodic Table"), true);
	tabs->AddPage(structures_panel, _("Prototypes"), false);

	box_sizer->Add(tabs);
	SetSizerAndFit(box_sizer);

	wxCommandEvent foo(0, 6-1);
	ElementSelected(foo);

	wxCommandEvent event;
	ChangeStructureGroup(event);

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

	wxString coordinationChoices[] = {_T("0"), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6")};
	mCoordinationChoice = new wxChoice(periodic_panel, kPeriodicCoordinationChoice, wxPoint(-1, -1),
									   wxSize(-1, -1), 7, coordinationChoices);
	sizer->Add(mCoordinationChoice, wxGBPosition(1, 8), wxGBSpan(1, 4), rflags);
	mCoordinationChoice->Connect(wxEVT_CHAR,
		wxKeyEventHandler(BuilderDlg::KeyHandler), NULL, this);

	// Show lone pair count drop down menu.
	wxStaticText *lp_num_label = new wxStaticText(periodic_panel, wxID_ANY, wxT("Number of lone pairs: "));
	sizer->Add(lp_num_label, wxGBPosition(2, 2), wxGBSpan(1, 6), lflags);

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

	wxStandardPathsBase & gStdPaths = wxStandardPaths::Get();
	wxString user_protos = gStdPaths.GetUserConfigDir();
#if defined(__WXMAC__) || defined(__WXMSW__)
	user_protos += wxT("/macmolplt_prototypes.cml");
#else
	// The standard unix path is the user's home dir. Thus the file should be "hidden".
	user_protos += wxT("/.macmolplt_prototypes.cml");
#endif	

	struc_sizer = new wxGridBagSizer();

//	int lflags = wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL;
//	int rflags = wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL;
//	int cflags = wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL;

	struc_sizer->SetFlexibleDirection(wxBOTH);
	struc_sizer->SetCols(2);
	struc_sizer->SetRows(3);

#define NUM_STRUC_GROUPS 4
	struc_groups = new wxChoice(panel, kPeriodicStrucGroups);
	struc_groups->Append(_T("Organics"),
						 new StrucGroupClientData(sys_prefs_path +
							 				  _T("/organics.cml")));
	struc_groups->Append(_T("Solvents"),
						 new StrucGroupClientData(sys_prefs_path +
							 				  _T("/solvents.cml")));
	struc_groups->Append(_T("Amino Acids"),
						 new StrucGroupClientData(sys_prefs_path +
							 				  _T("/amino_acids.cml")));
	struc_groups->Append(_T("User-defined"),
						 new StrucGroupClientData(user_protos, true));

	struc_sizer->Add(struc_groups, wxGBPosition(0, 0), wxGBSpan(1, 1),
					 wxEXPAND);
	struc_groups->SetSelection(0);

	mStructureChoice = new wxListBox(panel, kPeriodicStrucChoice);
	struc_sizer->Add(mStructureChoice, wxGBPosition(1, 0), wxGBSpan(1, 1),
					 wxEXPAND);

	// It seems more sensible to just add the canvas directly to the grid
	// bag sizer, but GTK requires us to hide its parent widget before adding
	// it.  But hiding the structure panel causes some page change events
	// in the notebook that we want to avoid.  So, we create an intermediate
	// panel and add the canvas to that.
	canvas_panel = new wxPanel(panel, kPeriodicCanvasPanel);
	canvas_panel_sizer = new wxFlexGridSizer(1, 1, 0, 0);
	canvas_panel_sizer->AddGrowableCol(0);
	canvas_panel_sizer->AddGrowableRow(0);
	canvas_panel_sizer->SetFlexibleDirection(wxBOTH);
	canvas_panel->SetSizerAndFit(canvas_panel_sizer);

	struc_sizer->Add(canvas_panel, wxGBPosition(0, 1), wxGBSpan(3, 1),
					 wxEXPAND);
	
	struc_custom_sizer = new wxBoxSizer(wxVERTICAL);

	delete_button = new wxButton(panel, kPeriodicDeleteStructure,
								 _("Delete Structure"));
	struc_custom_sizer->Add(delete_button, wxSizerFlags().Expand().Border(wxALL, 3));

	rename_button = new wxButton(panel, kPeriodicRenameStructure,
								 _("Rename Structure"));
	struc_custom_sizer->Add(rename_button, wxSizerFlags().Expand().Border(wxALL, 3));

	struc_sizer->Add(struc_custom_sizer, wxGBPosition(2, 0), wxGBSpan(1, 1),
					 wxEXPAND | wxALIGN_CENTER);

	struc_sizer->AddGrowableCol(1, 1);
	struc_sizer->AddGrowableRow(1, 1);

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

	int id = event.GetSelection();
	if (id != wxNOT_FOUND) {
		canvas->SetStructure(structures[id]);
	}
	canvas->SetFocus();

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
			if (id < 0) {	//interpret as the first letter of an element symbol
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
	atom_to_prune = -1;
	link_atom = -1;

}

/* ------------------------------------------------------------------------- */

Structure::~Structure() {

	if (atoms) delete[] atoms;
	if (bonds) delete[] bonds;

}

/* ------------------------------------------------------------------------- */

void BuilderDlg::AddStructure(Structure *structure) {

	int i;

	if (structure->FragName.size() <= 0) {
		for (i = structure->natoms - 1; i >= 0; i--) {
			if (structure->atoms[i].Type == 1) {
				structure->SetPruneAtom(i);
				break;
			}
		}
	}

	structures.push_back(structure);
	mStructureChoice->Append(structure->name);

}

/* ------------------------------------------------------------------------- */
/**
 * This function adds a structure to the user-defined structure group.
 * @param structure The predefined structure which should be dynamically
 *                  allocated. The memory will be owned by the build palette.
 */

void BuilderDlg::AddUserStructure(Structure *structure) {

	// Switch to the user structure group if necessary.
	if (struc_groups->GetSelection() < NUM_STRUC_GROUPS - 1) {
		struc_groups->SetSelection(NUM_STRUC_GROUPS - 1);
		wxCommandEvent event;
		ChangeStructureGroup(event);
	}

	// Add to the fixed user_strucs list and to the currently viewed
	// structures list.  Write the structures out to their file.
	user_strucs.push_back(structure);

	AddStructure(structure);

	SaveStructures();

	// Select the just added structure.
	mStructureChoice->SetSelection(mStructureChoice->GetCount() - 1);

	if (canvas) {
		canvas->SetStructure(structures[structures.size() - 1]);
	}

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

std::ostream& operator<<(std::ostream& stream, const Structure& s) {

	int i;
	
	std::cout << "------------------------" << std::endl;
	std::cout << s.name.ToAscii() << std::endl;
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

/* ------------------------------------------------------------------------- */
/**
 * This function saves the currently selected structure group out to its
 * file.
 */

void BuilderDlg::SaveStructures() {

	FILE *save_file = NULL;
	BufferFile *buffer = NULL;
	StrucGroupClientData *data;

	data = reinterpret_cast<StrucGroupClientData *>
		   (struc_groups->GetClientObject(struc_groups->GetSelection()));

	save_file = fopen(data->filename.mb_str(wxConvUTF8), "wb");
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

}

/* ------------------------------------------------------------------------- */
/**
 * This function removes all the current structures in the structures list.
 */

void BuilderDlg::DeleteAllStructures() {

	int i;

	if (canvas) {
		canvas->SetStructure(NULL);
	}

	// The vector contains pointers to structures, so we delete the
	// pointed-to memory first.
	for (i = structures.size() - 1; i >= 0; i--) {
		delete structures[i];
	}

	// And then we delete the rest.
	structures.clear();
	mStructureChoice->Clear();
	mStructureChoice->SetSelection(wxNOT_FOUND);

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

	// Open file of new structures.  If the file couldn't be opened, we
	// don't do anything.  (There used to be an error message here, but
	// there's a strong chance that the file might not exist, which is
	// a perfectly legal thing.)
	load_file = fopen(filename.mb_str(wxConvUTF8), "rb");
	if (load_file == NULL) {
		return false;
	}

	try {
		buffer = new BufferFile(load_file, false);
	} catch (std::bad_alloc) {
		MessageAlert("Insufficient memory to read in the file.");
	} catch (MemoryError) {
		MessageAlert("Insufficient memory to read in the file.");
	} catch (DataError) {
		MessageAlert("Invalid data encountered while reading the file.");
	} catch (FileError) {
		MessageAlert("File System error, read aborted.");
	}

	int result = 0;
	if (buffer) {
		result = ReadCMLFile(buffer);
		delete buffer;
	}

	fclose(load_file);

	return result == 0;

}


/* ------------------------------------------------------------------------- */
/**
 * This function handles clicks on the Delete Structure button.  The current
 * non-system structure is deleted and the first structure in the list is
 * selected in its place.
 * @param event The click event sent in by wxWidgets.
 */

void BuilderDlg::DeleteStructure(wxCommandEvent& event) {

	int id = mStructureChoice->GetSelection();

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

	StrucGroupClientData *data;
	data = reinterpret_cast<StrucGroupClientData *>
		   (struc_groups->GetClientObject(struc_groups->GetSelection()));

	SaveStructures();

}

/* ------------------------------------------------------------------------- */
/**
 * This function prompts the user to rename the currently selected structure.
 * @param event The click event for the Rename Structure button.
 */

void BuilderDlg::RenameStructure(wxCommandEvent& event) {

	int id = mStructureChoice->GetSelection();

	wxString new_name = wxGetTextFromUser(_("Enter structure name:"),
										  _("Rename Structure"),
										  _(""), this);

	// As long as the new name isn't the empty string, we make the change.
	// We have to change the structure list, the dropdown menu, and set the
	// group's dirty flag.
	if (!new_name.IsEmpty()) {
		structures[id]->name = new_name;
		mStructureChoice->SetString(id, structures[id]->name);

		StrucGroupClientData *data;
		data = reinterpret_cast<StrucGroupClientData *>
			   (struc_groups->GetClientObject(struc_groups->GetSelection()));
	}

	SaveStructures();

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
		/* canvas_panel_sizer->Detach(canvas); */
		/* canvas->Destroy(); */
	}

	if (event.GetSelection() == 1) {
#ifdef __WXMAC__
		mStructureChoice->Enable();
#endif
		/* GTK has a problem with gtk_widget_set_colormap being called within
		 * wx.  GTK wants it called before the widget is made, but wx calls 
		 * it afterward.  So, we work around that by hiding the parent and
		 * showing it again.  In the hiding, we must also reselect the tab
		 * since the selection is lost. */
		canvas_panel->Hide();
		int attrs[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
		canvas = new PreviewCanvas(canvas_panel, kPeriodicCanvas, attrs);
		canvas_panel_sizer->Add(canvas, wxSizerFlags().Expand());
		canvas_panel_sizer->Layout();
		canvas_panel->Show();

		if (event.GetSelection() == 1) {
			if (mStructureChoice->GetSelection() != wxNOT_FOUND) {
				canvas->SetStructure(structures[mStructureChoice->GetSelection()]);
			}
			canvas->Render();
			canvas->Connect(wxEVT_CHAR,
							wxKeyEventHandler(BuilderDlg::KeyHandler),
							NULL, this);
			canvas->SetFocus();
		}

//		Refresh(true, NULL);
#ifdef __WXMAC__
		mStructureChoice->Enable();
		Hide();	//This is a gross hack to get the gl canvas to display properly??
		Show();
#endif
	} else {
#ifdef __WXMAC__
		// This control wants to take over event handling once it gets 
		// focus.  So, we turn him off when he's not visible.
		mStructureChoice->Disable();
#endif
		elements[prev_id >= 0 ? prev_id : 0].button->SetFocus();
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
/**
 * This function updates the Delete Structure button.  The button is enabled
 * only when a user-defined structure is being viewed.
 * @param event The update event.
 */

void BuilderDlg::UpdateDeleteStructures(wxUpdateUIEvent& event) {

	StrucGroupClientData *data =
		reinterpret_cast<StrucGroupClientData *>
		(struc_groups->GetClientObject(struc_groups->GetSelection()));
	event.Enable(data->is_custom &&
				 mStructureChoice->GetSelection() != wxNOT_FOUND);

}

/* ------------------------------------------------------------------------- */
/**
 * This function updates the Rename Structure button.  The button is enabled
 * only when a user-defined structure is being viewed.
 * @param event The update event.
 */

void BuilderDlg::UpdateRenameStructures(wxUpdateUIEvent& event) {

	StrucGroupClientData *data =
		reinterpret_cast<StrucGroupClientData *>
		(struc_groups->GetClientObject(struc_groups->GetSelection()));
	event.Enable(data->is_custom &&
				 mStructureChoice->GetSelection() != wxNOT_FOUND);

}

/* ------------------------------------------------------------------------- */
/**
 * This function handles events on the structure group dropdown menu.  When
 * a group is selected, we read in the structures from the file associated 
 * with the group, add them to the structures list, and display the first
 * one.
 * @param event The change event on the structure choice.
 */

void BuilderDlg::ChangeStructureGroup(wxCommandEvent& event) {

	StrucGroupClientData *data;

	DeleteAllStructures();

	// Load structures from the file using the path associated with the
	// menu item.
	data = reinterpret_cast<StrucGroupClientData *>
		   (struc_groups->GetClientObject(struc_groups->GetSelection()));
	LoadStructuresFromFile(data->filename);

	// If there are some structures that were loaded in, select the first one
	// in the list.
	if (mStructureChoice->GetCount()) {
		mStructureChoice->SetSelection(0);
		if (canvas) {
			canvas->SetStructure(structures[0]);
		}
	} else if (canvas) {
		canvas->SetStructure(NULL);
	}

}

/* ------------------------------------------------------------------------- */

