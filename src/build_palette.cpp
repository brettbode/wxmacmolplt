#include <iostream>

#include "wx/sizer.h"
#include <wx/stdpaths.h>
#include <wx/clntdata.h>

#include "build_palette.h"
#include "main.h"
#include "GlobalExceptions.h"
#include "myFiles.h"

extern WinPrefs *gPreferences;
extern BuilderInterface * BuilderTool;

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
	EVT_CHAR_HOOK(BuilderDlg::KeyHandler)
	EVT_KEY_DOWN (BuilderDlg::KeyHandler)
	EVT_CLOSE(BuilderDlg::OnClose)
	EVT_UPDATE_UI(kPeriodicDeleteStructure, BuilderDlg::UpdateDeleteStructures)
	EVT_UPDATE_UI(kPeriodicRenameStructure, BuilderDlg::UpdateRenameStructures)
	EVT_NOTEBOOK_PAGE_CHANGED(kPeriodicNotebookID, BuilderDlg::TabChanged)
	EVT_CHOICE(kPeriodicStrucGroups, BuilderDlg::ChangeStructureGroup)
END_EVENT_TABLE()

/* ------------------------------------------------------------------------- */

StructureGroup::StructureGroup(const wxString & fn, bool custom) {
	filename = fn;
	is_custom = custom;
}

StructureGroup::~StructureGroup(void) {
	std::vector<Structure *>::iterator struc;
	for (struc = structures.begin(); struc != structures.end(); struc++) {
		delete *struc;
	}
}

// --------------------------------------------------------------------------- 
// FUNCTIONS
// --------------------------------------------------------------------------- 

BuilderInterface::BuilderInterface() {
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

	BuilderPalette = NULL;

	short lcoordination[] = {	1,                                0,
								1,2,                    3,4,3,2,1,0,
								1,2,                    3,4,5,6,1,0,
								1,2,3,2,5,3,2,3,2,2,2,2,3,4,3,4,1,0,
								1,2,3,4,5,6,6,3,3,2,1,2,3,4,3,4,1,0,
								1,2,
									3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
									4,5,6,6,4,4,4,3,2,1,2,3,2,1,0,
								1,2,
									3,4,5,6,5,4,3,3,3,3,3,3,3,3,3,
									4,0,6,0,3,4,3,3,3};
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

	nelements = kNumTableElements;
	
	// Default to C selected.
	selectedElement = 5;
	keyBuffer[0] = keyBuffer[1] = keyBuffer[2] = '\0';
	targetList = 0;	//start with the first structure of the first group
	selectedStructure = 0;
	currentTab = 0;
	
	// Read in the structure groups
	wxString user_protos = gStdPaths.GetUserConfigDir();
#if defined(__WXMAC__) || defined(__WXMSW__)
	user_protos += wxT("/macmolplt_prototypes.cml");
#else
	// The standard unix path is the user's home dir. Thus the file should be "hidden".
	user_protos += wxT("/.macmolplt_prototypes.cml");
#endif	
	
	StructureGroup * stg = new StructureGroup(sys_prefs_path + _T("/organics.cml"), false);
	StructureGroups.push_back(stg);
	stg->ReadCMLFile();
	stg = new StructureGroup(sys_prefs_path + _T("/solvents.cml"), false);
	StructureGroups.push_back(stg);
	stg->ReadCMLFile();
	stg = new StructureGroup(sys_prefs_path + _T("/amino_acids.cml"), false);
	StructureGroups.push_back(stg);
	stg->ReadCMLFile();
	stg = new StructureGroup(user_protos, true);
	StructureGroups.push_back(stg);
	stg->ReadCMLFile();
	
}

BuilderInterface::~BuilderInterface() {
	if (BuilderPalette) {
		BuilderPalette->Destroy();
		BuilderPalette = NULL;
	}
	std::vector<StructureGroup *>::const_iterator it = StructureGroups.begin();
	while (it != StructureGroups.end()) {
		delete (*it);
		++it;
	}
	
}

void BuilderInterface::ShowPalette(bool state) {
	if (state) {
		if (!BuilderPalette)
			BuilderPalette = new BuilderDlg(wxT("Builder Tools"));
		if (BuilderPalette) {
			BuilderPalette->Show();
			BuilderPalette->Raise();
		}
	} else {
		if (BuilderPalette) {
			BuilderPalette->Destroy();
			BuilderPalette = NULL;
		}
	}
}
bool BuilderInterface::IsPaletteVisible(void) const {
	bool result = false;

	if (BuilderPalette) {
		result = BuilderPalette->IsShown();
	}
	return result;
}
void BuilderInterface::ClosePalette(void) {
	if (BuilderPalette) {
		BuilderPalette->Destroy();
		BuilderPalette = NULL;
	}
}
/* ------------------------------------------------------------------------- */

int BuilderInterface::GetSelectedElement(void) const {
	
	/* This function returns the atomic number of the currently selected atom,
	 * or 0 if no atom is selected. */
	
	return selectedElement + 1;
	
}
/* ------------------------------------------------------------------------- */

Structure * BuilderInterface::GetSelectedStructure(void) const {
	Structure * result = NULL;
	
	if (targetList < StructureGroups.size()) {
		if (selectedStructure < StructureGroups[targetList]->structures.size())
			result = StructureGroups[targetList]->structures[selectedStructure];
//		result = StructureGroups[targetList]->structures[mStructureChoice->GetSelection()];
	}
	return result;
}
/* ------------------------------------------------------------------------- */

Structure * BuilderInterface::GetStructure(int i) const {
	Structure * result = NULL;
	
	if ((i>=0) && (i<StructureGroups[targetList]->structures.size())) {
		result = StructureGroups[targetList]->structures[i];
	}
	return result;
}
/* ------------------------------------------------------------------------- */

short BuilderInterface::GetSelectedCoordination(void) const {
	short result = 0;
	if (selectedElement >= 0) result = coordinationNumber[selectedElement];
	return result;
}

/* ------------------------------------------------------------------------- */

short BuilderInterface::GetSelectedLonePairCount(void) const {
	short result = 0;
	if (selectedElement >= 0) result = LonePairCount[selectedElement];
	return result;
}
void BuilderInterface::KeyHandler(wxKeyEvent& event) {
	int key = event.GetKeyCode();
	if (!event.HasModifiers()) {
		if (isalpha(key)) {
			int id = -1;
			if (secondKeytimer.Time() < 400) {
				keyBuffer[1] = (unsigned char) key;
				//if less than three seconds try to interpret as the 2nd letter of a two letter element symbol
				id = SetAtomType(keyBuffer);
			}
			if (id < 0) {	//interpret as the first letter of an element symbol
				keyBuffer[0] = (unsigned char) key;
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
				selectedElement = id - 1;
				if (BuilderPalette)
					BuilderPalette->ElementSelected(foo);
			}
		} else if (key == 309) {
		}
	} else {
		if (key == 23) {
//			Close();
		}
	}
}
void BuilderInterface::TabChanged(int index) {
	currentTab = index;
}
void BuilderInterface::AddUserStructure(Structure *structure) {
	
	int i;
	
	if (structure->FragName.size() <= 0) {
		for (i = structure->natoms - 1; i >= 0; i--) {
			if (structure->atoms[i].Type == 1) {
				structure->SetPruneAtom(i);
				break;
			}
		}
	}
	targetList = kNUM_STRUC_GROUPS - 1;
	StructureGroups[targetList]->structures.push_back(structure);
	selectedStructure = StructureGroups[targetList]->structures.size() - 1;
	
	SaveStructures();
	
	if (BuilderPalette)
		BuilderPalette->AddUserStructure(structure);
	//Perhaps should open the dialog if it isn't already open?
}
/* ------------------------------------------------------------------------- */

void BuilderInterface::DeleteStructure(int index) {
	if ((index>=0)&&(index<StructureGroups[targetList]->structures.size())) {
		delete StructureGroups[targetList]->structures[index];
		StructureGroups[targetList]->structures.erase(StructureGroups[targetList]->structures.begin() + index);
		SaveStructures();
	}
}
/* ------------------------------------------------------------------------- */

void BuilderInterface::RenameStructure(int index, wxString & new_name) {
	
	// As long as the new name isn't the empty string, we make the change.
	if (!new_name.IsEmpty()&&(index>=0)&&(index<StructureGroups[targetList]->structures.size())) {
		StructureGroups[targetList]->structures[index]->name = new_name;		
	}
	
	SaveStructures();
	
}

/* ------------------------------------------------------------------------- */
/**
 * This function saves the currently selected structure group out to its
 * file.
 */

void BuilderInterface::SaveStructures() {
	if ((targetList>=0)&&(targetList<StructureGroups.size())) {
		StructureGroups[targetList]->WriteCMLFile();
	}
}

Structure * BuilderInterface::FindFragment(const std::string fragName) const {
	Structure * result = NULL;
	
	std::vector<StructureGroup *>::const_iterator itgroup = StructureGroups.begin();
	while (itgroup != StructureGroups.end()) {
		std::vector<Structure *>::const_iterator it = (*itgroup)->structures.begin();
		while (it != (*itgroup)->structures.end()) {
			if ((*it)->FragName == fragName) {
				result = (*it);
				break;
			}
			++it;
		}
		++itgroup;
	}
	return result;
}
// --------------------------------------------------------------------------- 

void BuilderInterface::ElementSelected(int elementType) {
	if (elementType > 0 || elementType <= kNumTableElements) {
		selectedElement = elementType;
	}
}
bool BuilderInterface::currentGroupIsCustom(void) const {
	bool result = false;
	if ((targetList>=0)&&(targetList<StructureGroups.size()))
		result = StructureGroups[targetList]->is_custom;
	return result;
}
void BuilderInterface::ChangeTargetGroup(int new_group) {
	if ((new_group>=0)&&(new_group<StructureGroups.size())) {
		targetList = new_group;
		selectedStructure = 0;
	}
}
int BuilderInterface::ChangeTargetStructure(int index) {
	if ((index>=0)&&(index<StructureGroups[targetList]->structures.size()))
		selectedStructure = index;
	return selectedStructure;
}
void BuilderInterface::BuildNameList(wxListBox * theList) const {
	//fill the list with the names of each structure
	std::vector<Structure *>::const_iterator it = StructureGroups[targetList]->structures.begin();
	while (it != StructureGroups[targetList]->structures.end()) {
		theList->Append((*it)->name);
		++it;
	}
}
void BuilderInterface::SetSelectedCoordinationNumber(int val) {
	coordinationNumber[selectedElement] = val;
}
void BuilderInterface::SetSelectedLPCount(int val) {
	LonePairCount[selectedElement] = val;
}

BuilderDlg::BuilderDlg(const wxString& title,
					   int xpos, int ypos) :
	wxMiniFrame(NULL, wxID_ANY, title, wxPoint(xpos, ypos),
		wxSize(-1, -1), wxCLOSE_BOX | wxCAPTION) {

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

	prev_id = -1;
	wxCommandEvent foo(0, BuilderTool->GetSelectedElement()-1);
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

	wxPanel *periodic_panel = new wxPanel(tabs, wxID_ANY);
	periodic_panel->Connect(wxEVT_CHAR,
							wxKeyEventHandler(BuilderDlg::KeyHandler),
							NULL, this);
	wxGridBagSizer *sizer = new wxGridBagSizer();
	
#ifdef __WXMAC__
#define IMAGE_SIZE 20
	font_size = 12;
#else
#define IMAGE_SIZE 23
	font_size = 10;
#endif

	nelements = kNumTableElements;

	// No element has been selected yet.
//	prev_id = -1;

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

	struc_sizer = new wxGridBagSizer();

//	int lflags = wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL;
//	int rflags = wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL;
//	int cflags = wxALIGN_CENTER | wxALIGN_CENTER_VERTICAL;

	struc_sizer->SetFlexibleDirection(wxBOTH);
	struc_sizer->SetCols(2);
	struc_sizer->SetRows(3);

	struc_groups = new wxChoice(panel, kPeriodicStrucGroups);
	struc_groups->Append(_T("Organics"));
	struc_groups->Append(_T("Solvents"));
	struc_groups->Append(_T("Amino Acids"));
	struc_groups->Append(_T("User-defined"));

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
		BuilderTool->ElementSelected(id);
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
		
		mCoordinationChoice->SetSelection(BuilderTool->GetSelectedCoordination());
		mLPChoice->SetSelection(BuilderTool->GetSelectedLonePairCount());
	}
	
	Refresh();
}

// --------------------------------------------------------------------------- 

void BuilderDlg::OnCoordinationChoice(wxCommandEvent& event) {
	BuilderTool->SetSelectedCoordinationNumber(event.GetSelection());
//	coordinationNumber[prev_id] = event.GetSelection();
}

// --------------------------------------------------------------------------- 

void BuilderDlg::OnLPChoice(wxCommandEvent& event) {
	BuilderTool->SetSelectedLPCount(event.GetSelection());
//	LonePairCount[prev_id] = event.GetSelection();
}

/* ------------------------------------------------------------------------- */

void BuilderDlg::OnStructureChoice(wxCommandEvent& event) {

	int id = event.GetSelection();
	if (id != wxNOT_FOUND) {
		BuilderTool->ChangeTargetStructure(id);
		canvas->SetStructure(BuilderTool->GetSelectedStructure());
//		canvas->SetStructure(StructureGroups[targetList]->structures[id]);
	}
	canvas->SetFocus();

}

/* ------------------------------------------------------------------------- */

void BuilderDlg::KeyHandler(wxKeyEvent& event) {
	BuilderTool->KeyHandler(event);
}	

/* ------------------------------------------------------------------------- */

void BuilderDlg::OnClose(wxCloseEvent& event) {
	BuilderTool->ClosePalette();
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

void BuilderDlg::AddUserStructure(Structure *structure) {
	
	// Switch to the user structure group if necessary.
	if (struc_groups->GetSelection() < kNUM_STRUC_GROUPS - 1) {
		struc_groups->SetSelection(kNUM_STRUC_GROUPS - 1);
		wxCommandEvent event;
		ChangeStructureGroup(event);
	}
	
	mStructureChoice->Append(structure->name);
	
	// Select the just added structure.
	mStructureChoice->SetSelection(mStructureChoice->GetCount() - 1);
	
	if (canvas) {
		canvas->SetStructure(BuilderTool->GetSelectedStructure());
	}
	
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
 * This function handles clicks on the Delete Structure button.  The current
 * non-system structure is deleted and the first structure in the list is
 * selected in its place.
 * @param event The click event sent in by wxWidgets.
 */

void BuilderDlg::DeleteStructure(wxCommandEvent& event) {

	int id = mStructureChoice->GetSelection();

	BuilderTool->DeleteStructure(id);
	mStructureChoice->Delete(id);
	if (mStructureChoice->GetCount() == 0) {
		mStructureChoice->SetSelection(wxNOT_FOUND);
		canvas->SetStructure(NULL);
	} else {
		id = (id == 0) ? 0 : id - 1;
		mStructureChoice->SetSelection(id);
		canvas->SetStructure(BuilderTool->GetStructure(id));
	}
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
		BuilderTool->RenameStructure(id, new_name);
		mStructureChoice->SetString(id, new_name);
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
				canvas->SetStructure(BuilderTool->GetStructure(mStructureChoice->GetSelection()));
			}
			canvas->Render();
			canvas->Connect(wxEVT_CHAR,
							wxKeyEventHandler(BuilderDlg::KeyHandler),
							NULL, this);
			canvas->SetFocus();
		}

//		Refresh(true, NULL);
		BuilderTool->TabChanged(1);
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
		BuilderTool->TabChanged(0);
		elements[BuilderTool->GetSelectedElement() >= 0 ? BuilderTool->GetSelectedElement() : 0].button->SetFocus();
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

	event.Enable(BuilderTool->currentGroupIsCustom() &&
				 mStructureChoice->GetSelection() != wxNOT_FOUND);

}

/* ------------------------------------------------------------------------- */
/**
 * This function updates the Rename Structure button.  The button is enabled
 * only when a user-defined structure is being viewed.
 * @param event The update event.
 */

void BuilderDlg::UpdateRenameStructures(wxUpdateUIEvent& event) {

	event.Enable(BuilderTool->currentGroupIsCustom()  &&
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

	if (canvas) {
		canvas->SetStructure(NULL);
	}
	
	// And then we delete the rest.
	mStructureChoice->Clear();
	mStructureChoice->SetSelection(wxNOT_FOUND);
	
	BuilderTool->ChangeTargetGroup(struc_groups->GetSelection());
	
	BuilderTool->BuildNameList(mStructureChoice);

	// If there are some structures that were loaded in, select the first one
	// in the list.
	if (mStructureChoice->GetCount()) {
		mStructureChoice->SetSelection(0);
		if (canvas) {
			canvas->SetStructure(BuilderTool->GetStructure(0));
		}
	} else if (canvas) {
		canvas->SetStructure(NULL);
	}

}

/* ------------------------------------------------------------------------- */

