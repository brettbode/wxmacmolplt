/*
 * (c) 2005-2006 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * MolDisplayWin.cpp
 ***************************************/

#include "Globals.h"
#include "GlobalExceptions.h"
#include "MolDisplayWin.h"
#include "Prefs.h"
#include "main.h"
#include "Frame.h"
#include "BasisSet.h"
#include "InputData.h"
#include "Math3D.h"
#include "myFiles.h"
#include "setscreenplane.h"
#include "bondsdlg.h"
#include "coordinateswindow.h"
#include "energyplotdialog.h"
#include "VirtualSphere.h"
#include "Internals.h"
#include "llmdialog.h"
#include "setbondlength.h"
#include "appendframesoptions.h"
#include "exportoptionsdialog.h"
#include "frequenciesdialog.h"
#include "inputbuilder.h"
#include "surfaceswindow.h"
#include "choosevecgroup.h"
#include "coordinateoffset.h"
#include "frameenergy.h"
#include "build_palette.h"
#include "printoptions.h"
#include "windowparameters.h"
#include "zmatrixcalculator.h"
#include "symmetrypointgroupdlg.h"
#include "setPreference.h"
#include "MyTypes.h"
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/printdlg.h>
#include <wx/mstream.h>
#include <wx/display.h>
#include <wx/stdpaths.h>
#include <wx/toolbar.h>
#if wxCHECK_VERSION(2,9,0)
#include <wx/quantize.h>
#endif

#ifdef __WXMSW__
#include <stdio.h>
#endif

extern WinPrefs * gPreferences;
extern BuilderInterface * BuilderTool;
extern WindowData	gWindowDefault;

using namespace std;

//Local use class to hold data during the animation of normal modes
class ModeAnimation {
public:
	std::vector<CPoint3D>   SavedCoordinates;
	std::vector<CPoint3D>   ModeOffset;
	float                   offsetFactor;
	int                     iPoint;
	int                     inc;
	int                     NumPoints;
	wxTimer                 m_timer;
	bool                    SavedDrawMode;
};

//derive a status bar class so we can add the scrollbar
class MolStatusBar : public wxStatusBar {
public:
	MolStatusBar(MolDisplayWin * parent);
	
	void OnScrollBarChange(wxScrollEvent & event);
	void SetScrollBar(int pos, int range);
	void SetScrollBarValue(int pos);
	void OnSize(wxSizeEvent & event);

private:
	MolDisplayWin *	myParent;
	wxScrollBar *	myScroll;
		
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MolStatusBar, wxStatusBar)
	EVT_SIZE(MolStatusBar::OnSize)
	EVT_COMMAND_SCROLL(MMP_FRAMESCROLLBAR, MolStatusBar::OnScrollBarChange )
END_EVENT_TABLE()

//Class to hold CML data on the clipboard
//This is really intended for internal use for copying data from one file to another.
class wxCMLDataObject : public wxDataObjectSimple {
public:
	wxCMLDataObject(char * cmlText=NULL) : wxDataObjectSimple(_("CML")) {
		CML = cmlText;};
	virtual ~wxCMLDataObject(void) {if (CML) delete [] CML; CML=NULL;};
	char * GetCML(void) const {return CML;};
	virtual size_t GetDataSize(void) const {return (CML?strlen(CML):0);};
	virtual bool GetDataHere(void * buf) const {if (CML) {strcpy((char *)buf, CML); return true;} return false;};
	virtual bool SetData(size_t len, const void * buf) {
		if (CML) {delete [] CML; CML = NULL;}
		if (len>0) {
			CML = new char[len+1];
			CML[len] = '\0';
			if (buf) strncpy(CML, (const char *)buf, len);
			return true;
		}
		return false;
	};
	
private:
	char * CML;
	DECLARE_NO_COPY_CLASS(wxCMLDataObject)
};

#include <iostream>
MolDisplayWin::MolDisplayWin(const wxString &title,
						 const wxPoint  &position,
						 const wxSize   &size,
						 long            style,
						 const wxString &name)
	: wxFrame(NULL, wxID_ANY, title, position, size, style, name) {

	MainData = new MoleculeData(this);
	Prefs = new WinPrefs;
	*Prefs = *gPreferences;
	Dirty = false;
	OperationInProgress = false;
	ProgressInd = NULL;
	ModeAnimationData = NULL;
	bondsWindow = NULL;
	coordsWindow = NULL;
	energyPlotWindow = NULL;
	frequenciesWindow = NULL;
	surfacesWindow = NULL;
	inputBuilderWindow = NULL;
	prefsDlg = NULL;
	zMatCalcDlg = NULL;
	
	winData = gWindowDefault;
		
	pageSetupData = NULL;
	printData = NULL;
	myStatus = NULL;

	show_fullscreen = false;
	mHighliteState = false;
	window_just_focused = false;
	mAltModifyingToolBar = false;
	edit_symmetrically = false;
	do_rotate_annotation = false;
	GLUpdateInProgress = false;

	toolbar = NULL;
	lasso_has_area = false;
	show_bond_sites = true;

	status_timer.SetOwner(this, MMP_STATUS_TIMER);
	rotate_timer.SetOwner(this, MMP_ROTATE_TIMER);

#ifdef _MSC_VER
	//Visual studio is a total pile.
	//The %n format specifier is disabled by default and
	//putting this in the app init function didn't work.
	_set_printf_count_output(1);
#endif

	transpTriList = NULL;
	transpSortVertex = NULL;
	transpIndex = NULL;
	triangleCount = 0;
	sphere_list = 0;
	shader_program = 0;
	depth_fbo = 0;
	depth_tex_id = 0;
	MainListActive = false;
	SurfaceListActive = false;
	haveTransparentSurfaces = false;

	light_pos[0] = 6.0f;
	light_pos[1] = 6.0f;
	light_pos[2] = 12.0f;
	light_pos[3] = 0.0f;
	
	myStatus = new MolStatusBar(this);
	SetStatusBar(myStatus);
	
	int attribs[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16,
	                 0, 0};
	bool do_stereo = false;
#if wxCHECK_VERSION(2,9,0)
	if (Prefs->UseStereo()) {
		attribs[4] = WX_GL_STEREO;
		if (wxGLCanvas::IsDisplaySupported(attribs)) {
			do_stereo = true;
		} else {
			attribs[4] = 0;
		}
	}
#endif
	glCanvas = new MpGLCanvas(this, wxID_ANY, wxPoint(0,0), wxDefaultSize,
	                          attribs, do_stereo);
	glCanvas->SetPrefs(Prefs);
	
	createMenuBar();
	SetMenuBar(menuBar);
	/* SizeChanged(); */
	glCanvas->SetFocus();
		
		//init the inertia for the autorotation to something reasonable
	inertia.x = 1;
	inertia.y = 1;
	
	/* Show(true); */
	AdjustMenus();

	// If this is a new/empty window default to edit mode (title should be
	// "Untitled")
	if (!title.Cmp(wxT("Untitled"))) {
		Prefs->SetToolbarShown(true);
		ShowToolbar();
		menuBuild->Check(MMP_SHOW_TOOLBAR, Prefs->ToolbarShown());

		wxCommandEvent foo;
		foo.SetId(MMP_TOOL_HAND);
		OnToggleTool(foo);
	}

//	SetClientSize(wxSize(400, 400));
	SetSize(winData.GetMolWinRect());

	Show();
}

MolDisplayWin::~MolDisplayWin() {
	//As long as all related windows indicate this window as their
	//parent when they are created, they will be automatically destroyed
	//when this window is destroyed.
	if (printData) delete printData;
	if (pageSetupData) delete pageSetupData;
	glCanvas->SetPrefs(NULL);
	if (ModeAnimationData) {
		delete ModeAnimationData;
		ModeAnimationData = NULL;
	}
	
	if (ProgressInd != NULL) {
		delete ProgressInd;
		ProgressInd = NULL;
	}
	
	if (MainData != NULL) {
		delete MainData;
		MainData = NULL;
	}

	if (Prefs != NULL) {
		delete Prefs;
		Prefs = NULL;
	}

	delete[] transpTriList;
	transpTriList = NULL;
	delete[] transpSortVertex;
	transpSortVertex = NULL;
	delete[] transpIndex;
	transpIndex = NULL;

	// Since the periodic table dialog no longer has a parent, the application
	// will stay open if the dialog is open but no MolDisplayWins are.  So,
	// if this is the last MolDisplayWin that's open, we close the periodic
	// dialog in order for the whole application to close.
#ifndef __WXMAC__
	MpApp *app = (MpApp *) wxTheApp;
	if (app->WindowCount() == 0) {
		BuilderTool->ClosePalette();
	}
#endif
}

void MolDisplayWin::getCanvasSize(long *width, long *height) {
	int w, h;
	glCanvas->GetClientSize(&w, &h);
	*width = w;
	*height = h;
}

void MolDisplayWin::eventSize(wxSizeEvent &event) {
	event.Skip();
}

void MolDisplayWin::OnMenuOpen(wxMenuEvent & event) {
	StopAnimations();
	event.Skip();
}

void MolDisplayWin::OnKillFocus(wxFocusEvent & event) {
	StopAnimations();
	event.Skip();
}

void MolDisplayWin::OnActivate(wxActivateEvent & event) {
	if (event.GetActive()) {
		window_just_focused = true;
		glCanvas->eventWindowActivated(event);
	} else {
		StopAnimations();
	}
	event.Skip();
}

bool MolDisplayWin::JustFocused(void) {
	return window_just_focused;
}

void MolDisplayWin::StopAnimations(void) {
	bool resetNeeded = false;

	if (m_timer.IsRunning()) {
		m_timer.Stop();
		timerRunning = false;
		resetNeeded = true;
	}
	if (ModeAnimationData) { //if data already exists toggle off the animation
		ModeAnimationData->m_timer.Stop();
		MainData->SetDrawMode(ModeAnimationData->SavedDrawMode);
		for (int iatm=0; iatm<(MainData->cFrame->NumAtoms); iatm++) {
			MainData->cFrame->Atoms[iatm].Position = ModeAnimationData->SavedCoordinates[iatm];
		}
		delete ModeAnimationData;
		ModeAnimationData = NULL;
		resetNeeded = true;
	}
	if (resetNeeded) ResetModel(false);

	// It's kind of nice to leave the auto-rotation running.  Otherwise, if we
	// leave the window, we've got to start it going again.  If you don't like
	// this, uncomment the next lines.
	// if (rotate_timer.IsRunning()) { 
		// rotate_timer.Stop(); 
	// } 
}

void MolDisplayWin::OnFrameAnimationTimer(wxTimerEvent & event) {
	if (MainData->CurrentFrame>=MainData->NumFrames)
		ChangeFrames(1);
	else
		ChangeFrames(MainData->CurrentFrame+1);
}

void MolDisplayWin::createMenuBar(void) {
	menuBar = new wxMenuBar;

	menuFile = new wxMenu;
	menuEdit = new wxMenu;
	menuView = new wxMenu;
	menuBuild = new wxMenu;
	menuMolecule = new wxMenu;
	menuWindow = new wxMenu;
	menuHelp = new wxMenu;

	menuFile->Append(wxID_NEW, wxT("&New\tCtrl+N"), _("Open a new empty window"));
	menuFile->Append(MMP_NEWFRAME, wxT("Append New Frame"), wxT("Add a new, empty frame"));
	menuFile->Append(wxID_OPEN, wxT("&Open ...\tCtrl+O"), wxT("Open a file into a new window"));
	menuFile->Append(MMP_ADDFRAMES, wxT("Add Frames from File..."), wxT("Add additional geometries to this window from a file"));
	menuFile->Append(wxID_SAVE, wxT("&Save\tCtrl+S"));
	menuFile->Append(wxID_SAVEAS, wxT("Save &as ...\tCtrl+Shift+S"));
	menuFile->Append(wxID_CLOSE, wxT("&Close\tCtrl+W"));
	menuFile->AppendSeparator();
	menuFile->Append(MMP_DELETEFRAME, wxT("Delete Frame"), wxT("Delete the visible geometry point"));
	menuFile->AppendSeparator();
	menuFile->Append(MMP_IMPORTMENU, wxT("Import..."), wxT("Import a $VEC group from a GAMESS dat file"));
	menuFile->Append(MMP_EXPORT, wxT("Export..."), wxT("Export this window to one of a variety of formats"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_PRINT_SETUP, wxT("Page Set&up ..."));
	menuFile->Append(MMP_PRINTOPTIONS, wxT("Print Options ..."));
	menuFile->Append(wxID_PREVIEW, wxT("Print Pre&view\tCtrl+Shift+P"));
	menuFile->Append(wxID_PRINT, wxT("&Print ...\tCtrl+P"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, wxT("&Quit\tCtrl+Q"));
	
	menuEdit->Append(wxID_UNDO, wxT("&Undo\tCtrl+Z"));
	menuEdit->Append(wxID_REDO, wxT("&Redo\tCtrl+Shift+Z"));
	menuEdit->AppendSeparator();
	menuEdit->Append(wxID_CUT, wxT("Cu&t\tCtrl+X"));
	menuEdit->Append(wxID_COPY, wxT("&Copy\tCtrl+C"), wxT("Copy the display as an image"));
	menuEdit->Append(MMP_COPYCOORDS, wxT("Copy Coordinates"), wxT("Copy the current set of coordinates as plain text in GAMESS style"));
	menuEdit->Append(MMP_COPYNWCOORDS, wxT("Copy NWChem Coordinates"), wxT("Copy the current set of coordinates as plain text in NWChem style"));
	menuEdit->Append(wxID_PASTE, wxT("&Paste\tCtrl+V"));
	menuEdit->Append(wxID_CLEAR, wxT("&Delete\tDel"));
	menuEdit->AppendSeparator();
	menuEdit->Append(MMP_SELECT_ALL, wxT("&Select All\tCtrl+A"));
	menuEdit->Append(MMP_SELECT_NONE, wxT("Select &None\tShift+Ctrl+A"));
	menuEdit->AppendSeparator();
	menuEdit->Append(wxID_PREFERENCES, wxT("Global Pr&eferences"), wxT("Edit the default preferences for new windows"));

	menuView->AppendCheckItem(MMP_SHOWMODE, wxT("Show &Normal Mode\tCtrl+D"), wxT("Display the vibrational motion as mass-weighted vectors"));
	menuView->Append(MMP_ANIMATEMODE, wxT("&Animate Mode\tShift+Ctrl+M"), wxT("Animate the vibration, click outside the window to stop"));
	menuView->Append(MMP_OFFSETMODE, wxT("&Offset along mode..."), wxT("Generate a new set of coordinates by moving along the current mode"));
	menuView->Append(MMP_PREVMODE, wxT("&Previous Normal Mode\tCtrl+["));
	menuView->Append(MMP_NEXTMODE, wxT("Ne&xt Normal &Mode\tCtrl+]"));
	menuView->AppendSeparator();
	menuView->AppendCheckItem(MMP_SHOWAXIS, wxT("Show Ax&is"), wxT("Display the cartesian axis"));
	menuView->AppendCheckItem(MMP_SHOWSYMMETRYOPERATOR, _("Show S&ymmetry Operators"), _("Overlays the symmetry operators for the current point group. Not all point groups are supported."));
	
	menuViewLabels = new wxMenu;
	menuView->Append(MMP_ATOMLABELSSUBMENU, wxT("Atom &Labels"), menuViewLabels);
	menuViewLabels->AppendRadioItem(MMP_NO_ATOMLABEL, wxT("&None"));
	menuViewLabels->AppendRadioItem(MMP_SHOWATOMLABELS, wxT("Atomic &Symbol"));
	menuViewLabels->AppendRadioItem(MMP_SHOWATOMNUMBER, wxT("Atom &Number"));
	menuViewLabels->AppendRadioItem(MMP_BOTHATOMLABELS, wxT("S&ymbols and Numbers"));
	
	menuViewStyle = new wxMenu;
	menuView->Append(MMP_DISPLAYMODESUBMENU, _("&Display Style"), menuViewStyle);
	menuViewStyle->AppendRadioItem(MMP_WIREFRAMEMODE, _("Wire Frame"), _("Display as bonds only"));
	menuViewStyle->AppendRadioItem(MMP_BALLANDSTICKMODE, _("Ball and Stick"), _("Tradiational display mode, sphere size can be adjusted in the preferences"));
	menuViewStyle->AppendSeparator();
	menuViewStyle->AppendCheckItem(MMP_EFP_WIREFRAME, _("Show Effective Fragments as Wire Frame"), _("Overide the display mode for EFP solvent molecules"));
	menuViewStyle->AppendSeparator();
	menuViewStyle->AppendCheckItem(MMP_SHOWPATTERN, _("Show Atom Patterns"), _("Overlay a 2D pattern on the atom spheres (Ball and Stick mode only)"));

	wxMenu * menuViewToggleVis = new wxMenu;
	menuView->Append(MMP_DISPLAYMODESUBMENU, _("&Toggle visibility of"), menuViewToggleVis);
	menuViewToggleVis->Append(MMP_TOGGLEABINITIO, _("&ab initio atoms\tAlt-a"), _("Show/Hide ab initio atoms"));
	menuViewToggleVis->Append(MMP_TOGGLEMMATOMS, _("&MM atoms\tAlt-s"), _("Show/Hide Molecular Mechanics atoms"));
	menuViewToggleVis->Append(MMP_TOGGLEEFPATOMS, _("&Effective Fragment atoms\tAlt-e"), _("Show/Hide EFP atoms"));
	
	menuViewAnnotations = new wxMenu;
	menuViewAnnotations->Append(MMP_ADDMARKANNOTATION, _("&Mark Selected"), _("Mark the selected atom(s), right-click to remove"));
	menuViewAnnotations->Append(MMP_ADDLENGTHANNOTATION, _("Display &Length"), _("Select 2 atoms then right-click on one of them"));
	menuViewAnnotations->Append(MMP_ADDANGLEANNOTATION, _("Display &Angle"), _("Select 3 atoms then right-click on one of them"));
	menuViewAnnotations->Append(MMP_ADDDIHEDRALANNOTATION, _("Display Di&hedral"), _("Select 4 atoms then right-click on one of them"));
	menuViewAnnotations->Append(MMP_DELETEALLANNOTATIONS, _("&Delete all annotations"), _("Remove all annotations or right-click on the annotation"));
	menuView->Append(MMP_ANNOTATIONSSUBMENU, _("&Annotations"), menuViewAnnotations, _("Annotate the molecule display"));
	
	menuView->Append(MMP_ANIMATEFRAMES, wxT("Animate &Frames\tCtrl+F"), wxT("Run through the geometries producing an animation"));
	menuView->Append(MMP_SHRINK10, wxT("&Shrink 10%\tCtrl-down"), _T("Reduce size by 10%"));
	menuView->Append(MMP_ENLARGE10, wxT("&Enlarge 10%\tCtrl-up"), _T("Enlarge size by 10%"));
	menuView->Append(MMP_CENTER, wxT("&Center View"), _T("Center the molecule in the window"));
	menuView->Append(MMP_AUTOROTATE, wxT("Start AutoRotation"), _T("Start/Stop the molecule autorotation"));
	
	menuViewRotate = new wxMenu;
	menuView->Append(MMP_ROTATESUBMENU, wxT("&Rotate ..."), menuViewRotate);
	menuView->Append(MMP_WINDOWPARAMETERS, wxT("Set &Window Parameters..."));

	menuView->AppendSeparator();
	menuView->AppendCheckItem(MMP_SHOW_FULLSCREEN, _("Show Fullscreen"), _("Display in full screen mode"));

	menuViewRotate->Append(MMP_ROTATETOXAXIS, wxT("to &X-axis"));
	menuViewRotate->Append(MMP_ROTATETOYAXIS, wxT("to &Y-axis"));
	menuViewRotate->Append(MMP_ROTATETOZAXIS, wxT("to &Z-axis"));
	menuViewRotate->Append(MMP_ROTATE180HOR, wxT("180 degrees &Horizontal"));
	menuViewRotate->Append(MMP_ROTATE180VER, wxT("180 degrees &Vertical"));
	menuViewRotate->Append(MMP_ROTATEPRINC, wxT("to &Principal Orientation"));
	menuViewRotate->Append(MMP_ROTATEOTHER, wxT("&Other..."));

#ifdef ENABLE_INTERACTIVE_MODE
	menuBuild->AppendCheckItem(MMP_SHOW_TOOLBAR, _("Show Toolbar\tCtrl+I"), _("Show tools for viewing and editing atoms"));
#endif
	menuBuild->AppendCheckItem(MMP_SHOWBUILDTOOLS, _("Show Build &Tools\tCtrl+T"), _("Display build tools palette"));
#ifdef ENABLE_INTERACTIVE_MODE
	menuBuild->AppendSeparator();
	menuBuild->Append(MMP_ADDHYDROGENS, wxT("Add &Hydrogens"), _T("Complete moleclues by adding hydrogens to incomplete bonds"));
	menuBuild->Append(MMP_DELETEHYDROGENS, wxT("&Delete Hydrogens"), _T("Remove terminal hydrogens (Does not apply to effective fragments or SIMOMM atoms)"));
	menuBuild->AppendCheckItem(MMP_SHOWBONDSITES, wxT("&Show Bonding Sites"), _T("Click on a site to add an atom"));
	menuBuild->AppendCheckItem(MMP_SYMMETRY_EDIT, wxT("Edit with Symmetry"), _T(""));
	/* menuBuild->Append(MMP_SYMMETRY_REGEN, wxT("Regenerate Symmetry"), _T("")); */
	menuBuild->AppendSeparator();
	menuBuild->Append(MMP_SAVESTRUCTURE, wxT("Prototype Selection"), _T("Save structure as a prototype in builder"));
#endif
	
	menuMolecule->Append(MMP_SETBONDLENGTH, wxT("Set Bonds..."), _("Apply the automated bond determination with several options"));
	menuMolecule->Append(MMP_ENERGYEDIT, wxT("Set &Frame Energy..."));
	menuMolecule->Append(MMP_CREATELLMPATH, wxT("Create &LLM Path..."), _("Create a Linear Least Motion path between this geometry and the next one"));
	menuMolecule->Append(MMP_MINFRAMEMOVEMENTS, wxT("&Minimize Frame Movements"), _("Reorient each frame so as to minimize the movement of each atom"));
	menuMolecule->AppendSeparator();
	menuSetPG = new wxMenu;
	menuSetPG->Append(MMP_CURPOINTGROUP, _("cur pg"), _("Currently selected point group"));
	menuPointGroup = new wxMenu;
	menuPointGroup->AppendRadioItem(MMP_PGC1, _("C1"));
	menuPointGroup->AppendRadioItem(MMP_PGCS, _("Cs"));
	menuPointGroup->AppendRadioItem(MMP_PGCI, _("Ci"));
	menuPointGroup->AppendRadioItem(MMP_PGCNH, _("Cnh"));
	menuPointGroup->AppendRadioItem(MMP_PGCNV, _("Cnv"));
	menuPointGroup->AppendRadioItem(MMP_PGCN, _("Cn"));
	menuPointGroup->AppendRadioItem(MMP_PGS2N, _("S2n"));
	menuPointGroup->AppendRadioItem(MMP_PGDND, _("Dnd"));
	menuPointGroup->AppendRadioItem(MMP_PGDNH, _("Dnh"));
	menuPointGroup->AppendRadioItem(MMP_PGDN, _("Dn"));
	menuPointGroup->AppendRadioItem(MMP_PGTD, _("Td"));
	menuPointGroup->AppendRadioItem(MMP_PGTH, _("Th"));
	menuPointGroup->AppendRadioItem(MMP_PGT, _("T"));
	menuPointGroup->AppendRadioItem(MMP_PGOH, _("Oh"));
	menuPointGroup->AppendRadioItem(MMP_PGO, _("O"));
	menuPGOrder = new wxMenu;
	menuPGOrder->AppendRadioItem(MMP_PGORDER2, _("2"));
	menuPGOrder->AppendRadioItem(MMP_PGORDER3, _("3"));
	menuPGOrder->AppendRadioItem(MMP_PGORDER4, _("4"));
	menuPGOrder->AppendRadioItem(MMP_PGORDER5, _("5"));
	menuPGOrder->AppendRadioItem(MMP_PGORDER6, _("6"));
	menuPGOrder->AppendRadioItem(MMP_PGORDER7, _("7"));
	menuPGOrder->AppendRadioItem(MMP_PGORDER8, _("8"));
	
	menuSetPG->Append(wxID_ANY, _("&Point Group"), menuPointGroup, _("Select the Point Group"));
	menuSetPG->Append(MMP_POINTGROUPORDER, _("&Order of Principal Axis"), menuPGOrder, _("Set the order of the principle rotation axis, if any"));
	menuMolecule->Append(wxID_ANY, _("Set &Point Group"), menuSetPG, _("Manually set the point group"));
	menuMolecule->Append(MMP_DETERMINEPG, _("Determine Point Group"), _("Compute the point group for the current coordinates"));
	menuMolecule->Append(MMP_SYMADAPTCOORDS, _("Set &Coordinates to Principal Orientation"), _("Transform coordinates to the symmetry adapted principle orientation"));
	menuMolecule->AppendSeparator();
	menuMolecule->Append(MMP_CONVERTTOBOHR, wxT("Convert to &Bohr"));
	menuMolecule->Append(MMP_CONVERTTOANGSTROMS, wxT("Convert to &Angstroms"));
	menuMolecule->Append(MMP_INVERTNORMALMODE, wxT("&Invert Normal Mode"), _T("Multiply the normal mode by -1 to invert the direction of the vectors"));
// TODO:  Create menu items for remaining menus

	menuWindow->Append(MMP_BONDSWINDOW, wxT("&Bonds"), _("View/edit the bonding within the molecule"));
	menuWindow->Append(MMP_COORDSWINDOW, wxT("&Coordinates"), _("View/edit cartesian or internal coordinates"));
	menuWindow->Append(MMP_ENERGYPLOTWINDOW, wxT("&Energy Plot"), _("A plot of the energy for each geometry"));
	menuWindow->Append(MMP_FREQUENCIESWINDOW, wxT("&Frequencies"), _("Plot the vibrational frequencies"));
	menuWindow->Append(MMP_INPUTBUILDERWINDOW, wxT("&Input Builder"), _T("Generate a GAMESS input file"));
	/* menuWindow->Append(MMP_INPUT_WIZARD, wxT("&Input Builder Wizard"), _T("Easily generate a GAMESS input file")); */
	menuWindow->Append(MMP_SURFACESWINDOW, wxT("&Surfaces"), _T("Add/Edit/Remove various surface types"));
	menuWindow->Append(MMP_ZMATRIXCALC, wxT("&Z-Matrix Calculator"), _("Compute bond lengths/angles or dihedrals between any set of atoms"));
	menuWindow->Append(MMP_LOCAL_PREFERENCES, wxT("Pr&eferences"), _T("Edit the preferences for this window"));

	menuBar->Append(menuFile, wxT("&File"));
	menuBar->Append(menuEdit, wxT("&Edit"));
	menuBar->Append(menuView, wxT("&View"));
	menuBar->Append(menuBuild, wxT("&Builder"));
	menuBar->Append(menuMolecule, wxT("&Molecule"));
	menuBar->Append(menuWindow, wxT("&Subwindow"));

	menuHelp->Append(wxID_ABOUT, wxT("&About MacMolPlt..."), _T("Learn about MacMolPlt"));
	menuHelp->Append(wxID_HELP, wxT("&MacMolPlt Manual..."), _T("Brief documentation"));
	menuBar->Append(menuHelp, wxT("&Help"));
}

void MolDisplayWin::ClearMenus(void) {
	menuFile->Enable(MMP_NEWFRAME, false);
	menuFile->Enable(wxID_SAVE, false);
	menuFile->Enable(MMP_DELETEFRAME, false);
	menuFile->Enable(MMP_EXPORT, false);
	
	menuEdit->Enable(wxID_CUT, false);
	/* menuEdit->Enable(wxID_COPY, false); */
	menuEdit->Enable(MMP_COPYCOORDS, false);
	menuEdit->Enable(MMP_COPYNWCOORDS, false);
	/* menuEdit->Enable(wxID_CLEAR, false); */

	menuView->Enable(MMP_SHOWMODE, false);
	menuView->Enable(MMP_ANIMATEMODE, false);
	menuView->Enable(MMP_PREVMODE, false);
	menuView->Enable(MMP_NEXTMODE, false);
	menuView->Enable(MMP_OFFSETMODE, false);
	menuView->Enable(MMP_ANIMATEFRAMES, false);

	menuMolecule->Enable(MMP_SETBONDLENGTH, false);
	menuMolecule->Enable(MMP_ENERGYEDIT, false);
	menuMolecule->Enable(MMP_MINFRAMEMOVEMENTS, false);
	menuMolecule->Enable(MMP_DETERMINEPG, false);
	menuMolecule->Enable(MMP_SYMADAPTCOORDS, false);
	menuMolecule->Enable(MMP_INVERTNORMALMODE, false);
}

void MolDisplayWin::AdjustMenus(void) {
	ClearMenus();
	/* menuFile->Enable(wxID_SAVE, Dirty); */
	if (Prefs->ShowAtomicSymbolLabels() && Prefs->ShowAtomNumberLabels())
		menuViewLabels->Check(MMP_BOTHATOMLABELS, true);
	else if (Prefs->ShowAtomicSymbolLabels())
		menuViewLabels->Check(MMP_SHOWATOMLABELS, true);
	else if (Prefs->ShowAtomNumberLabels())
		menuViewLabels->Check(MMP_SHOWATOMNUMBER, true);
	else
		menuViewLabels->Check(MMP_NO_ATOMLABEL, true);

	if (Prefs->DrawWireFrame())
		menuViewStyle->Check(MMP_WIREFRAMEMODE, true);
	else
		menuViewStyle->Check(MMP_BALLANDSTICKMODE, true);

	if (MainData->NumFrames > 1 ) {
		menuFile->Enable(MMP_DELETEFRAME, true);
		menuView->Enable(MMP_ANIMATEFRAMES, true);
		menuMolecule->Enable(MMP_MINFRAMEMOVEMENTS, true);
	}
	if (MainData->cFrame->Vibs) {
		menuView->Enable(MMP_SHOWMODE, true);
		menuView->Enable(MMP_ANIMATEMODE, true);
		menuView->Check(MMP_SHOWMODE, MainData->GetDrawMode());
		if (MainData->cFrame->Vibs->CurrentMode>0) menuView->Enable(MMP_PREVMODE, true);
		if (MainData->cFrame->Vibs->CurrentMode<(MainData->cFrame->Vibs->NumModes-1))
			menuView->Enable(MMP_NEXTMODE, true);
		menuView->Enable(MMP_OFFSETMODE, true);
		menuMolecule->Enable(MMP_INVERTNORMALMODE, true);
	}
}

void MolDisplayWin::OnSaveUpdate(wxUpdateUIEvent& event) {
	event.Enable(Dirty);
}

void MolDisplayWin::OnShowToolbarUpdate(wxUpdateUIEvent& event) {
	event.Check(Prefs->ToolbarShown());
}

void MolDisplayWin::OnShowBuildToolsUpdate(wxUpdateUIEvent& event) {
	event.Check(BuilderTool->IsPaletteVisible());
}

void MolDisplayWin::OnUndoUpdate( wxUpdateUIEvent& event ) {
	event.Enable(mUndoBuffer.undoPossible());
}

void MolDisplayWin::OnRedoUpdate( wxUpdateUIEvent& event ) {
	event.Enable(mUndoBuffer.redoPossible());
}

void MolDisplayWin::OnAddHydrogensUpdate( wxUpdateUIEvent& event ) {
	event.Enable((MainData->cFrame->GetNumAtoms() > 0) && InEditMode());
	menuBuild->Enable(MMP_DELETEHYDROGENS, (MainData->cFrame->GetNumAtoms() > 0) &&
	                  InEditMode());
}

void MolDisplayWin::OnShowBondSitesUpdate(wxUpdateUIEvent& event) {
	event.Check(show_bond_sites);
	event.Enable(InEditMode());
}

void MolDisplayWin::OnShowSymmetryEdit(wxUpdateUIEvent& event) {
	event.Check(edit_symmetrically);
	event.Enable(InEditMode());
}

/**
 * This function updates a variety of controls that should be enabled when
 * at least one atom is in the current frame and disabled otherwise.  To
 * register a control with this update function, insert its window ID into
 * the range of IDs this function handles.
 * @param event The update event.
 */
void MolDisplayWin::UpdateAtomsOptions(wxUpdateUIEvent& event) {
	event.Enable(MainData->cFrame->GetNumAtoms() > 0);
}

/*!
* wxEVT_UPDATE_UI event handler for wxID_PASTE
 */

void MolDisplayWin::OnPasteUpdate(wxUpdateUIEvent& event) {

	// This function gets triggered on a paste event (actually, a call to
	// CreateFrameSnapShot, which triggers UI updates).  The clipboard is
	// already open there, and it's a mistake to open it twice.
	if (wxTheClipboard->IsOpened()) return;

	event.Enable(false);
	//paste is allowed for empty frames or while in edit mode
	if ((MainData->cFrame->NumAtoms == 0)||InEditMode()) {
		if (wxTheClipboard->Open()) {
			if (wxTheClipboard->IsSupported(wxDF_TEXT) ||
			    wxTheClipboard->IsSupported(_("CML"))) {
				event.Enable(true);
			}
			wxTheClipboard->Close();
		}
	}
}
/*!
* wxEVT_UPDATE_UI event handler for Edit menu items
 */

void MolDisplayWin::OnSelectNoneUpdate(wxUpdateUIEvent& event) {
	/* menuEdit->Enable(wxID_CLEAR, mHighliteState && InEditMode()); */
	event.Enable(MainData->cFrame->GetNumAtomsSelected() > 0);
}

void MolDisplayWin::OnSelectAllUpdate(wxUpdateUIEvent& event) {
	/* menuEdit->Enable(wxID_CLEAR, mHighliteState && InEditMode()); */
	event.Enable(MainData->cFrame->GetNumAtoms() > 0);
}

/**
 * Handles enable/disabling of the Edit -> Delete menu item.  The menu item is
 * enabled when highlighting is in effect and we're in edit mode.
 * @param event The update event.
 **/
void MolDisplayWin::OnDeleteUpdate(wxUpdateUIEvent& event) {
	event.Enable(mHighliteState && InEditMode());
}

void MolDisplayWin::OnAnnotationMarkUpdate( wxUpdateUIEvent& event ) {
	event.Enable(glCanvas->NumberSelectedAtoms()>0);
}
void MolDisplayWin::OnAnnotationLengthUpdate( wxUpdateUIEvent& event ) {
	event.Enable(glCanvas->NumberSelectedAtoms()==2);
}
void MolDisplayWin::OnAnnotationAngleUpdate( wxUpdateUIEvent& event ) {
	event.Enable(glCanvas->NumberSelectedAtoms()==3);
}
void MolDisplayWin::OnAnnotationDihedralUpdate( wxUpdateUIEvent& event ) {
	event.Enable(glCanvas->NumberSelectedAtoms()==4);
}
void MolDisplayWin::OnShowSymOpsUpdate( wxUpdateUIEvent& event ) {
	bool state = false;
	if (MainData->InputOptions) {
		if (MainData->InputOptions->Data) {
			GAMESSPointGroup pg = MainData->InputOptions->Data->GetPointGroup();
			if ((pg > GAMESS_C1)&&(pg!=GAMESS_S2N)&&(pg <GAMESS_TD)) {
				state = true;
			}
		}
	}
	event.Enable(state);
	event.Check(state && Prefs->ShowSymmetryOperators());
}
void MolDisplayWin::OnDeleteAnnotationsUpdate( wxUpdateUIEvent& event ) {
	event.Enable(MainData->GetAnnotationCount() > 0);
}
void MolDisplayWin::OnShowPatternUpdate( wxUpdateUIEvent& event ) {
	event.Enable(!Prefs->DrawWireFrame());
	event.Check(Prefs->Show2DPattern());
}
void MolDisplayWin::OnEFPWireFrameUpdate( wxUpdateUIEvent& event ) {
	event.Enable(MainData->FragmentNames.size()>0);
	event.Check(Prefs->ShowEFPWireFrame());
}
void MolDisplayWin::OnAutoRotationUpdate( wxUpdateUIEvent& event ) {
	if (rotate_timer.IsRunning()) {
		event.SetText(wxT("Stop AutoRotation"));
	} else {
		event.SetText(wxT("Start AutoRotation"));
	}
}
void MolDisplayWin::OnShowPointGroupUpdate( wxUpdateUIEvent& event ) {
	event.Enable(false);
	GAMESSPointGroup pg = GAMESS_C1;
	int pgOrder = 1;
	if (MainData->InputOptions) {
		pg = MainData->InputOptions->Data->GetPointGroup();
		pgOrder = MainData->InputOptions->Data->GetPointGroupOrder();
	}
	if ((pg<GAMESS_C1)||(pg>NumberGAMESSPointGroups)) pg = GAMESS_C1;
	wxString pgString;
	const char * pgt = DataGroup::GetGAMESSPointGroupText(pg);
	if ((pg >=GAMESS_CNH)&&(pg<=GAMESS_DN)) {
		if (pgOrder<2) pgOrder = 2;
		menuSetPG->Enable(MMP_POINTGROUPORDER, true);
		menuPGOrder->Check((MMP_PGORDER2+pgOrder-2), true);
		int i=0;
		while (pgt[i]) {
			if (pgt[i] == 'N') {
				wxString temp;
				temp.Printf(wxT("%d"), pgOrder);
				pgString.Append(temp);
			} else {
				pgString.Append((char)(pgt[i]), 1);
			}
			i++;
		}
	} else {
		menuSetPG->Enable(MMP_POINTGROUPORDER, false);
		pgString = wxString(pgt, wxConvUTF8);
	}
	menuPointGroup->Check((MMP_PGC1+pg-GAMESS_C1), true);
	event.SetText(pgString);
}
void MolDisplayWin::OnMoleculeCreateLLMPathUpdate( wxUpdateUIEvent& event ) {
	event.Enable(false);
	if (MainData->NumFrames > 1 ) {
		if (MainData->CurrentFrame < MainData->NumFrames) {
			Frame * lFrame = MainData->GetCurrentFramePtr();
			Frame * lnFrame = lFrame->GetNextFrame();
			if (lFrame->NumAtoms == lnFrame->NumAtoms) {
				bool good = true;
				for (long i=0; i<lFrame->NumAtoms; i++) {
					if (lFrame->Atoms[i].Type != lnFrame->Atoms[i].Type) {
						good = false;
						break;
					}
				}
				event.Enable(good);
			}
		}
	}
}
/* Event handler functions */

/* File menu */
void MolDisplayWin::menuFileAppendNewFrame(wxCommandEvent &event) {
	MainData->AddFrame(10,0);
	ResetAllWindows();
}
void MolDisplayWin::menuFileDeleteFrame(wxCommandEvent &event) {
	MainData->DeleteFrame();
	ResetAllWindows();
}
void MolDisplayWin::menuFileImport(wxCommandEvent &event) {
	//First need to use an open file dialog
	wxString filename = wxFileSelector(wxT("Choose a GAMESS .DAT file to import a $VEC group"));
	//We are looking for $ VEC groups. Scan to see how many are there. If more than 1 the user will
	//have to choose.
	if (!filename.empty()) {
		FILE * myfile = fopen(filename.mb_str(wxConvUTF8), "rb");
		if (myfile == NULL) {
			MessageAlert("Unable to open the selected file!");
		} else {
			BufferFile * Buffer = NULL;
			try {
				Buffer = new BufferFile(myfile, false);
				int vecCount=0;
				while (Buffer->LocateKeyWord("$VEC", 4)) {
					vecCount++;
					Buffer->SkipnLines(1);
				}
				Buffer->SetFilePos(0);
				if (vecCount <= 0) {
					MessageAlert("No $VEC group found in the selected file.");
				} else {
					int targetVec = -1;
					if (vecCount == 1) {
						targetVec = 1;
					} else { //The user must choose the desired one
						ChooseVECgroup * chooser = new ChooseVECgroup(this);
						chooser->SetBuffer(Buffer);
						int rcode = chooser->ShowModal();
						if (rcode == wxID_OK) {
							targetVec = chooser->GetTarget() + 1;
						}
						chooser->Destroy();
					}
					if (targetVec >= 1) {
						Buffer->SetFilePos(0);
						for (int i=0; i<targetVec; i++) {
							if (i>0) Buffer->SkipnLines(1);
							Buffer->LocateKeyWord("$VEC", 4);
						}
						if (Buffer->LocateKeyWord("$VEC", 4)) {
							//Buffer should now have the correct position
							BasisSet * lBasis = MainData->GetBasisSet();
							Frame * lFrame = MainData->GetCurrentFramePtr();
							InputData * lInputData = MainData->GetInputData();
							if (lBasis) {
								long NumFuncs = lBasis->GetNumBasisFuncs(false);
								long SCFType = lInputData->Control->GetSCFType();
								long NumBetaOrbs = 0;
								if (SCFType == UHF) NumBetaOrbs = NumFuncs;
								else SCFType = 0;
								OrbitalRec * OrbSet = NULL;
								try {
									OrbSet = new OrbitalRec(NumFuncs, NumBetaOrbs, NumFuncs);
									if (OrbSet) {
										OrbSet->ReadVecGroup(Buffer, NumFuncs, (TypeOfWavefunction) SCFType);
									}
								}
								catch (DataError) {
									MessageAlert("Invalid data encountered while reading $Vec group. Import aborted!");
									if (OrbSet) {
										delete OrbSet;
										OrbSet = NULL;
									}
								}
								catch (MemoryError) {
									MessageAlert("Insufficient memory to import $Vec. Aborted!");
									if (OrbSet) {
										delete OrbSet;
										OrbSet = NULL;
									}
								}
								catch (std::bad_alloc) {
									MessageAlert("Insufficient memory to import $Vec. Aborted!");
									if (OrbSet) {
										delete OrbSet;
										OrbSet = NULL;
									}
								}
								catch (FileError) {
									MessageAlert("Unexpected end of file encountered.");
									if (OrbSet) {
										delete OrbSet;
										OrbSet = NULL;
									}
								}
								if (OrbSet != NULL) lFrame->Orbs.push_back(OrbSet);
							}
						}
					}
				}
			}
			catch (...) {
				MessageAlert("An error occurred importing the $VEC group.");
			}
			if (Buffer) delete Buffer;      //Done reading so free up the buffer
			fclose(myfile);
		}
	}
}
enum {
	mmp_bmp=0,
	mmp_png,
	mmp_jpg,
#if wxCHECK_VERSION(2,9,0)
	mmp_gif,
	mmp_tiff,
#endif
	mmp_data,
	mmp_mdl,
	mmp_xmol,
	mmp_eneg,
	mmp_vrml,
	mmp_pov,
	mmp_freq,
	mmp_qt,
	mmp_ming
};
void MolDisplayWin::menuFileExport(wxCommandEvent &event) {
	wxFileDialog        *fileDlg = NULL;
	ExportOptionsDialog *exportOptionsDlg = NULL;
	wxString   filepath;
	wxMemoryDC memDC;
	wxImage    exportImage;
	wxBitmap  *bmp;
	int animGIFIndex = -1;
	wxString   wildcards(wxT("Windows Bitmap (*.bmp)|*.bmp")
						 wxT("|Portable Network Graphics (*.png)|*.png")
						 wxT("|JPEG (*.jpeg;*.jpg)|*.jpeg;*.jpg")
#if wxCHECK_VERSION(2,9,0)
						 wxT("|GIF (*.gif)|*.gif")
						 wxT("|TIFF (*.tiff)|*.tiff")
#endif
						 wxT("|GAMESS $DATA group (*.inp)|*.inp")
						 wxT("|MDL MolFile|*.mol")
						 wxT("|XMOL (*.xyz)|*.xyz")
						 wxT("|Tab delimited Energies (*.txt)|*.txt")
						 wxT("|VRML (*.wrl)|*.wrl")
						 wxT("|POV-Ray (*.pov)|*.pov"));
	bool vibs = false;
	int itemCount = 9;
#if wxCHECK_VERSION(2,9,0)
	itemCount = 11;
#endif
	if (MainData->cFrame->GetNumberNormalModes() > 0) {
		vibs = true;
		wildcards.Append(wxT("|Frequencies (*.txt)|*.txt"));
		itemCount++;
	}
#if wxCHECK_VERSION(2,9,0)
	if ((MainData->GetNumFrames() > 1)||vibs) {
		wildcards.Append(wxT("|Animated GIF (*.gif)|*.gif"));
		animGIFIndex = itemCount;
		itemCount++;
	}
#endif
#ifdef __MAC_USE_QUICKTIME__
	int QTindex = -1;
	if ((MainData->GetNumFrames() > 1)||vibs) {
		wildcards.Append(wxT("|QuickTime Movie (*.mov)|*.mov"));
		QTindex = itemCount;
		itemCount++;
	}
#endif
#ifdef HAVE_LIBMING
	int FlashIndex = -1;
    if ((MainData->GetNumFrames() > 1)||vibs) {
        wildcards.Append(wxT("|Flash Movie (*.swf)|*.swf"));
		FlashIndex = itemCount;
		itemCount++;
    }
#endif
	int        index = 0;
	int        type  = 0;

	fileDlg = new wxFileDialog(this,
	                           wxT("Export"),
							   wxT(""),
							   wxT(""),
							   wildcards,
#if wxCHECK_VERSION(2,9,0)
							   wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
#else
							   wxSAVE | wxOVERWRITE_PROMPT);
#endif

	if(fileDlg->ShowModal() == wxID_OK) {
		Prefs->CylindersForLines(true);
		filepath = fileDlg->GetPath();
		index    = fileDlg->GetFilterIndex();
		if ((index < mmp_data) || (index == animGIFIndex)) {
			switch(index) {
				case mmp_bmp:
					type = wxBITMAP_TYPE_BMP;
					if(!filepath.Lower().Matches(wxT("*.bmp"))) {
						filepath.Append(wxT(".bmp"));
					}
					break;
				case mmp_png:
					type = wxBITMAP_TYPE_PNG;
					if(!filepath.Lower().Matches(wxT("*.png"))) {
						filepath.Append(wxT(".png"));
					}
					break;
				case mmp_jpg:
					type = wxBITMAP_TYPE_JPEG;
					if(!filepath.Lower().Matches(wxT("*.jpg")) &&
					   !filepath.Lower().Matches(wxT("*.jpeg"))) {

						filepath.Append(wxT(".jpg"));
					}
					break;
#if wxCHECK_VERSION(2,9,0)
				case mmp_gif:
					type = wxBITMAP_TYPE_GIF;
					if(!filepath.Lower().Matches(wxT("*.gif"))) {
						
						filepath.Append(wxT(".gif"));
					}
					break;
				case mmp_tiff:
					type = wxBITMAP_TYPE_TIFF;
					if(!filepath.Lower().Matches(wxT("*.tiff"))) {
						
						filepath.Append(wxT(".tiff"));
					}
					break;
#endif
			}
			exportOptionsDlg = new ExportOptionsDialog(this);
			exportOptionsDlg->setFileType(type);
#if wxCHECK_VERSION(2,9,0)
			if (index == animGIFIndex) {
				exportOptionsDlg->EnableModeMovie((MainData->cFrame->GetNumberNormalModes() > 0));
				if (MainData->GetNumFrames() > 1) {
					exportOptionsDlg->EnableFrameMovie(true);
					exportOptionsDlg->SetMovieChoice(0);
				} else {
					exportOptionsDlg->EnableFrameMovie(false);
					exportOptionsDlg->SetMovieChoice(1);
				}
			}
#endif
			if(exportOptionsDlg->ShowModal() == wxID_OK) {
#if wxCHECK_VERSION(2,9,0)
				if (index == animGIFIndex) {
					WriteGIFMovie(filepath, exportOptionsDlg);
				} else
#endif
				{
					bmp = new wxBitmap(exportOptionsDlg->getWidth(),
																   exportOptionsDlg->getHeight());
					memDC.SelectObject(*bmp);
					Prefs->SetLineWidth(exportOptionsDlg->getImageRatio());
					glCanvas->GenerateHiResImageForExport(&memDC);
					Prefs->SetLineWidth(1);
					exportImage = bmp->ConvertToImage();
					if(exportOptionsDlg->getTransparency()) {
						// This gets really hairy, since there isn't a good way to
						// determine what the actual value of the background color
						// ends up being after it's pumped through both GL and WX.
						//
						// In the end we just compare our "ideal" value with the
						// upper left corner of the image.  If the two are
						// reasonably close, we use the value from the corner.

						RGBColor *bgColor = Prefs->GetBackgroundColorLoc();
						unsigned char red, green, blue;
						short dRed, dGreen, dBlue;

						red = exportImage.GetRed(0, 0);
						green = exportImage.GetGreen(0, 0);
						blue = exportImage.GetBlue(0, 0);

						dRed = (short) abs((short)((bgColor->red) >> 8) - red);
						dGreen = (short) abs((short)((bgColor->green) >> 8) - green);
						dBlue = (short) abs((short)((bgColor->blue) >> 8) - blue);

						if(dRed < 3 && dGreen < 3 && dBlue < 3) {
							exportImage.SetMaskColour(red, green, blue);
						}
					}
#if wxCHECK_VERSION(2,9,0)
					if (type == wxBITMAP_TYPE_GIF) {
						wxQuantize::Quantize(exportImage, exportImage);
						if (exportImage.HasAlpha()) {
							exportImage.ConvertAlphaToMask();
						}
					}
#endif
					exportImage.SaveFile(filepath, (wxBitmapType) type);
					memDC.SelectObject(wxNullBitmap); // bmp has now been
				}									  // destroyed.
			}
			exportOptionsDlg->Destroy();
		}
#ifdef __MAC_USE_QUICKTIME__
		else if (index == QTindex)
			//quicktime movie export
			WriteQTMovie(filepath);
#endif
#ifdef HAVE_LIBMING
		else if (index == FlashIndex)
			//flash movie export
            WriteFlashMovie(filepath);
#endif
		else {
			FILE *currFile = NULL;

			if((currFile = fopen(filepath.mb_str(wxConvUTF8), "wb")) != NULL) {
				BufferFile *buffer = NULL;
				try {
					buffer = new BufferFile(currFile, true);

					bool AllFrames = false;
					switch (index) {
						case mmp_data:
							if (MainData->GetNumFrames()>1) {
								AllFrames = (wxMessageBox(wxT("Should all geometries (frames) be included\?"),
														  wxT(""), wxYES_NO | wxICON_QUESTION) == wxYES);
							}
							ExportGAMESS(buffer, AllFrames);
							break;
						case mmp_mdl:
							WriteMDLMolFile(buffer);
							break;
						case mmp_xmol:
						{
							if (MainData->GetNumFrames()>1) {
								AllFrames = (wxMessageBox(wxT("Should all geometries (frames) be included\?"),
														  wxT(""), wxYES_NO | wxICON_QUESTION) == wxYES);
							}
							bool AllModes = false;
							bool AnimateMode = false;
							if (!AllFrames && MainData->cFrame->Vibs) {
								if (MainData->cFrame->Vibs->GetNumModes() > 1) {
									AllModes = (wxMessageBox(wxT("Should all normal modes be included\?"),
															  wxT(""), wxYES_NO | wxICON_QUESTION) == wxYES);
								}
								if (!AllModes && (MainData->cFrame->Vibs->GetNumModes() > 0)) {
									AnimateMode = (wxMessageBox(wxT("Would you like to include data to animate the current normal mode\?"),
															 wxT(""), wxYES_NO | wxICON_QUESTION) == wxYES);
								}
							}
							WriteXYZFile(buffer, AllFrames, AllModes, AnimateMode);
						}
							break;
						case mmp_eneg:
							if (MainData->GetNumFrames()>1) {
								AllFrames = (wxMessageBox(wxT("Should all geometries (frames) be included\?"),
														  wxT(""), wxYES_NO | wxICON_QUESTION) == wxYES);
							}
							WriteTabbedEnergies(buffer, AllFrames);
							break;
						case mmp_vrml:
							WriteVRMLFile(buffer);
							break;
						case mmp_pov:
							WritePOVFile(buffer);
							break;
						case mmp_freq:
							WriteFrequencies(buffer);
							break;
					}
				}
				catch (MemoryError) {
					MessageAlert("Not enough memory to open the file for writing.");
				}
				if(buffer) {
					delete buffer;
				}
				fclose(currFile);
			} else {
				MessageAlert("Unable to open the file for output.");
			}
		}
		Prefs->CylindersForLines(false);
	}

	fileDlg->Destroy();
}

void MolDisplayWin::menuFileOpen(wxCommandEvent &event) {
	//Its possible we could handle this here if the current data is empty?
	//On the Mac Open always opens a new window
#ifndef __WXMAC__
	if (!Dirty && (MainData->NumFrames == 1) && (MainData->MaxAtoms == 0)) {
		//First need to use an open file dialog
		wxString filename = wxFileSelector(wxT("Choose a file to open"),
										   wxT(""), wxT(""), wxT(""),
										   wxT("*.*"),
#if wxCHECK_VERSION(2,9,0)
										   wxFD_OPEN,
#else
										   wxOPEN,
#endif
										   this);
		//If the user chooses a file, create a window and have it process it.
		if (!filename.IsEmpty()) {
			//An empty window defaults to edit mode with the toolbar active
			//but once we open a file we should default to non-edit mode and hide the toolbar
			if (Prefs->ToolbarShown()) {
				ShowToolbar(false);
			}
			//Ok we have a problem. Abort open can't close the last window!
			long r = OpenFile(filename);
			if (r>0) {
				SetTitle(filename);
			}
	//      if (r>0) temp->Show(true);
		}
	} else
#endif
		//otherwise just skip the event to pass it up the chain to the app handler
		event.Skip();
}
void MolDisplayWin::menuFileAddFramesFromFile(wxCommandEvent &event) {
	/* Add the data from one or more files to the existing file. */
	wxFileDialog fileDlg(this,
						_T("Choose a file(s) containing points to be appended to the currently open file."),
						wxT(""), wxT(""), _T("*.*"),
#if wxCHECK_VERSION(2,9,0)
							   wxFD_OPEN | wxFD_MULTIPLE);
#else
								wxOPEN | wxMULTIPLE);
#endif
	
	if(fileDlg.ShowModal() == wxID_OK) {
		wxArrayString paths;
		fileDlg.GetPaths(paths);
		AppendFramesOptions * optDlg = new AppendFramesOptions(this);
		optDlg->SetSkip(Prefs->GetDRCSkip());
		optDlg->SetupItems();
		if (optDlg->ShowModal() == wxID_OK) {
			Prefs->SetDRCSkip(optDlg->GetSkip());
			//Loop over the filenames in the path list.
			for (size_t i=0; i<paths.GetCount(); i++) {
				OpenFile(paths.Item(i), optDlg->GetOffset(), optDlg->GetFlip(), true);
			}
		}
		optDlg->Destroy();
	}
}

void MolDisplayWin::menuFileSave(wxCommandEvent &event) {
	if(!currFilePath.IsEmpty()) {
		FILE *currFile = NULL;
		BufferFile *buffer = NULL;

		if((currFile = fopen(currFilePath.mb_str(wxConvUTF8), "wb")) == NULL) {
			MessageAlert("Unable to access the file.");
			return;
		}
		try {
			buffer = new BufferFile(currFile, true);
		}
		//catch (std::bad_alloc) {//Out of memory error
			//if (!append)
				//AbortOpen("Not enough memory to open the file. Aborted!");
		//}
		catch (MemoryError) {
			MessageAlert("Not enough memory to open the file for writing.");
		}
		//catch (UserCancel) {
			//if (!append)
				//AbortOpen("File open canceled by user");
		//}
	//  catch (DataError Error) {//Error parsing the file data
	//      if (!Error.ErrorSet())  Window->AbortOpen(21);
	//      else {
	//          Error.WriteError();
	//          delete Window; Window = NULL;
	//      }
	//  }
		//Some kind of File system related error
		//catch (FileError Error) { Error.WriteError();
			//if (!append) AbortOpen(NULL);}
		//catch (...) { 
			//if (!append)
				//AbortOpen("Unknown error reading the selected file. File open aborted.");
		//}

		if(buffer) {
			UpdateWindowData();
			MainData->WriteCMLFile(buffer, Prefs, &winData, true, true);
			Dirtify(false);
			delete buffer;
		}
#ifdef __WXOSX_CARBON__
		if (Prefs->CreateCustomIcon()) CreateCustomFileIcon(currFilePath);
#endif
		fclose(currFile);
	}
	else {
		menuFileSave_as(event);
	}
}

void MolDisplayWin::menuFileSave_as(wxCommandEvent &event) {
	FILE *currFile = NULL;
	BufferFile *buffer = NULL;
	wxString filePath;

	filePath = wxFileSelector(wxT("Save As"), wxT(""), wxT(""), wxT(""),
							  wxT("CML Files (*.cml)|*.cml"),
#if wxCHECK_VERSION(2,9,0)
							  wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
#else
							  wxSAVE | wxOVERWRITE_PROMPT,
#endif
							  this);

	if(!filePath.IsEmpty()) {
		if((currFile = fopen(filePath.mb_str(wxConvUTF8), "wb")) == NULL) {
			MessageAlert("Unable to access the file.");
			return;
		}
		try {
			UpdateWindowData();
			buffer = new BufferFile(currFile, true);
			MainData->WriteCMLFile(buffer, Prefs, &winData, true, true);
			Dirtify(false);
		}
		//catch (std::bad_alloc) {//Out of memory error
			//if (!append)
				//AbortOpen("Not enough memory to open the file. Aborted!");
		//}
		catch (MemoryError) {
			MessageAlert("Not enough memory to open the file for writing.");
		}
		//catch (UserCancel) {
			//if (!append)
				//AbortOpen("File open canceled by user");
		//}
	//  catch (DataError Error) {//Error parsing the file data
	//      if (!Error.ErrorSet())  Window->AbortOpen(21);
	//      else {
	//          Error.WriteError();
	//          delete Window; Window = NULL;
	//      }
	//  }
		//Some kind of File system related error
		//catch (FileError Error) { Error.WriteError();
		//if (!append) AbortOpen(NULL);}
		//catch (...) { 
			//if (!append)
				//AbortOpen("Unknown error reading the selected file. File open aborted.");
		//}
		if(buffer) {
			delete buffer;
		}
#ifdef __WXOSX_CARBON__
		if (Prefs->CreateCustomIcon()) CreateCustomFileIcon(filePath);
#endif
		currFilePath = filePath;
		wxFileName fn(filePath);
		SetTitle(fn.GetFullName());
		fclose(currFile);
		//update the window titles for any open subwindows
		if (bondsWindow) {
			bondsWindow->UpdateWindowTitle();
		}
		if (coordsWindow) {
			coordsWindow->UpdateWindowTitle();
		}
		if (energyPlotWindow) {
			energyPlotWindow->UpdateWindowTitle();
		}
		if (frequenciesWindow) {
			frequenciesWindow->UpdateWindowTitle();
		}
		if (inputBuilderWindow) {
			inputBuilderWindow->UpdateWindowTitle();
		}
		if (surfacesWindow) {
			surfacesWindow->UpdateWindowTitle();
		}
		if (zMatCalcDlg) {
			zMatCalcDlg->UpdateWindowTitle();
		}
		if (prefsDlg) {
			prefsDlg->UpdateWindowTitle();
		}
	}
}

#ifdef __WXOSX_CARBON__
OSErr	SendFinderAppleEvent( AliasHandle aliasH, AEEventID appleEventID );
OSErr	SaveCustomIcon( const wxString & filename, IconFamilyHandle icnsH );
//	This routine will set the custom icon in the file with
//  an image based on the current molecule display.
//	It does this by creating a PicHandle, then creating an icon from the Picture,
//	and finally saving the custom icon to disk.
//  I am utilizing a private interface to internal data for the wxBitmap class so
//  be warned that this could break in the future!

#if wxCHECK_VERSION(2, 9, 0)
#include <wx/osx/private.h>
//This is a gross hack to get access to the picHandle in the wxBitmapRefData class
//that is no longer provided in the wx headers
class wxBitmapRefData {
public:
	PicHandle     GetPictHandle();
};
#else
#include <wx/mac/carbon/private.h>
#endif
bool MolDisplayWin::CreateCustomFileIcon( const wxString & filePath ) {
	bool result = false;
	
	//	create a pict of the current molecule display
	wxImage mImage = glCanvas->getImage(0,0);
	wxBitmap * mBitmap = new wxBitmap(mImage);
	//The following utilizes a Mac specific internal pair of calls
	//to get a PicHandle from a wxBitmap.
	wxBitmapRefData * mBitmapData = mBitmap->GetBitmapData();
	PicHandle pictH = mBitmapData->GetPictHandle();
	if ( pictH != NULL ) {
		//	IconFamilyResource.resourceType + IconFamilyResource.resourceSize
		IconFamilyHandle icnsH	= (IconFamilyHandle) NewHandle( 8 );		
		if ( icnsH == NULL ) return( memFullErr );
		(**icnsH).resourceType			= kIconFamilyType;
		(**icnsH).resourceSize			= 8;
		
		//	Create the icon from the Picture
		OSErr err	= SetIconFamilyData( icnsH, 'PICT', (Handle)pictH );
		if (err == noErr) {
			//	Save the custom icon to disk
			SaveCustomIcon( filePath, icnsH );
		}
		if ( icnsH != NULL ) DisposeHandle( (Handle) icnsH );
	}
	return result;
}
//	Saves the custom icon to disk - Mostly copied from Apple sample code
//	A file with a custom icon must contain a resource ( 'icns', -16455 ) and have its kHasCustomIcon bit set.
//	A folder with a custom icon must contain a file named "\pIcon\r" within it with a resource ( 'icns', -16455 ) and have the folders kHasCustomIcon bit set.
OSErr	SaveCustomIcon( const wxString & filename, IconFamilyHandle icnsH )
{
	OSErr				err=fnfErr;
	
	FSRef mFSRef;
	FSSpec targetSpec;
	const char * t = filename.mb_str(wxConvUTF8);
	OSStatus s = FSPathMakeRef((const UInt8 *) t, &mFSRef, false);
	FSCatalogInfoBitmap fields = kFSCatInfoFinderInfo;
	FSCatalogInfo info;
	if (s == noErr) {
	  err = FSGetCatalogInfo(&mFSRef, fields, &info, NULL, &targetSpec, NULL);
	}
	if (err != noErr) return err;

	HCreateResFile( targetSpec.vRefNum, targetSpec.parID, targetSpec.name );			//	Create the resource forked file "\pIcon\r"
	short refNum = FSpOpenResFile( &targetSpec, fsCurPerm );									//	Open it with write permissions
	
	if ( refNum == -1 )	return err;
	
	UseResFile( refNum );
	Handle h	= Get1Resource( 'icns', kCustomIconResource );
	if ( h != NULL )																		//	If it already has a custom icon
	{
		RemoveResource( h );																//	Remove the existing custom icon
		DisposeHandle( h );
	}
	AddResource( (Handle) icnsH, 'icns', kCustomIconResource, nil );						//	Create a new resource ( 'icns', -16455 )
																							//	err	= ResError(); if ( err != noErr ) DebugStr("\p AddResource Failed" );
	WriteResource( (Handle) icnsH );														//	Save the 'icns' resource
																							//	err	= ResError(); if ( err != noErr ) DebugStr("\p WriteResource Failed" );
	DetachResource( (Handle) icnsH );
	//	err	= ResError(); if ( err != noErr ) DebugStr("\p DetachResource Failed" );
	
	CloseResFile( refNum );																	//	Close the resource fork
		//now modify the finder bit to indicate the file has a custom icon
	FileInfo * mfinfo = (FileInfo *) (&(info.finderInfo));
	if (!(mfinfo->finderFlags & kHasCustomIcon)) {
		mfinfo->finderFlags = mfinfo->finderFlags | kHasCustomIcon;
		FSSetCatalogInfo(&mFSRef, fields, &info);
	}
	
	AliasHandle			aliasH;
	err = FSNewAliasMinimal(&mFSRef, &aliasH);
	if (err == noErr) {
		err	= SendFinderAppleEvent( aliasH, kAESync );											//	Send the Finder a kAESync AppleEvent to force it to update the icon immediately
		DisposeHandle( (Handle) aliasH );
	}
	return( err );
}
//	Utility routine to send the Finder an event within its kAEFinderSuite.  We use it to send a kAESync AppleEvent which forces the Finder
//	to immediately display the new icon.
OSErr	SendFinderAppleEvent( AliasHandle aliasH, AEEventID appleEventID )
{
	ProcessInfoRec			processInfo;
	OSErr					err;
	AppleEvent				appleEvent		= { typeNull, NULL };
	AEDesc					aeDesc			= { typeNull, NULL };
	ProcessSerialNumber		psn				= { kNoProcess, kNoProcess };
	AppleEvent				aeReply			= { typeNull, NULL };
	
	BlockZero( (Ptr)&processInfo, sizeof(processInfo) );
	processInfo.processInfoLength	= sizeof( processInfo );
	
	for ( err = GetNextProcess( &psn ) ; err == noErr ; err = GetNextProcess( &psn ) )
	{
		err	= GetProcessInformation( &psn, &processInfo );
		if ( (processInfo.processSignature == 'MACS') && (processInfo.processType == 'FNDR') )			//	Find the Finders PSN by searching all running processes
			break;
	}
	if (err != noErr) return err;
	
	err	= AECreateDesc( typeProcessSerialNumber, &psn, sizeof( psn ), &aeDesc );
	if (err == noErr) {
		err	= AECreateAppleEvent( kAEFinderSuite, appleEventID, &aeDesc, kAutoGenerateReturnID, kAnyTransactionID, &appleEvent );	//	Create AppleEvent (kAEFinderSuite, appleEventID)
		(void) AEDisposeDesc( &aeDesc );
		if (err == noErr) {
			err	= AECreateDesc( typeAlias, *aliasH, GetHandleSize( (Handle)aliasH ), &aeDesc );
			if (err == noErr) {
				err	= AEPutParamDesc( &appleEvent, keyDirectObject, &aeDesc );
				(void) AEDisposeDesc( &aeDesc );
				
				if (err == noErr) {
					err = AESend( &appleEvent, &aeReply, kAENoReply, kAENormalPriority, kNoTimeOut, NULL, NULL );		//	Send the AppleEvent to the Finder
					(void) AEDisposeDesc( &aeReply );
					
					if (err == noErr)
						(void) AEDisposeDesc( &appleEvent );
				}
			}
		}
	}
	return( err );
}
#endif

void MolDisplayWin::menuFileClose(wxCommandEvent &event) {
	Close();
}

void MolDisplayWin::FileClose(wxCloseEvent &event) {
	bool canVeto = event.CanVeto();
	//First we should check to see if a save is needed which could abort the close
	if (Dirty && Prefs->GetPrompt4Save()) {
		//prompt the user to see if they want the file saved
		//Note the message should be slightly different if we can't abort the close
		int style = wxYES_NO | wxICON_QUESTION;
		if (canVeto) style = style | wxCANCEL;
		wxString fileText = wxT("The file ");
		fileText += GetTitle();
		fileText += wxT(" has unsaved changes.");
		int r = wxMessageBox(wxT("Do you wish to save the current data and customizations before closing?"),
							 fileText, style, this);
		if (r == wxCANCEL) {
			event.Veto(true);
			return;
		} else if (r == wxYES) {
			//process the save
			wxCommandEvent temp;
			menuFileSave(temp);
			if (Dirty) {//Dirty should be false if the save was successful
				event.Veto(true);
				return;
			}
		}
	}
	MpApp & app = wxGetApp();
	//Once we decide to close the window it may be system dependant whether we 
	//leave an empty window up. On the Mac the window is always destroyed.
#ifndef __WXMAC__
/*  if ((app.WindowCount() <= 1) && canVeto) {  
		//This is the last window! Clear it out, but leave it open
		delete MainData;
		MainData = new MoleculeData(this);
		delete Prefs;
		Prefs = new WinPrefs;
		*Prefs = *gPreferences;
		Dirty = false;
		SetTitle(wxT("Untitled"));
		SetName(wxT("Untitled"));
		event.Veto(true);
		return;
	}*/
#endif
	app.destroyMainFrame(this);
	//Freeze();
	Destroy();
}

void MolDisplayWin::menuFilePage_setup(wxCommandEvent &event) {
	wxPrintData     *tempPrintData     = new wxPrintData;
	wxPageSetupData *tempPageSetupData = new wxPageSetupData;

	(*tempPageSetupData) = *tempPrintData;
	
	wxPageSetupDialog pageSetupDialog(this, tempPageSetupData);

	if(pageSetupDialog.ShowModal() == wxID_OK) {
		(*tempPageSetupData) = pageSetupDialog.GetPageSetupData();
		if(pageSetupData != NULL) delete pageSetupData;
		pageSetupData = tempPageSetupData;

		(*tempPrintData) = pageSetupData->GetPrintData();
		if(printData != NULL) delete printData;
		printData = tempPrintData;
	}
	else {
		delete tempPageSetupData;
		delete tempPrintData;
	}
}

void MolDisplayWin::menuFilePrintOptions(wxCommandEvent &event) {
	//throw up a mini dialog to handle some app specific printing options
	PrintOptions * po = new PrintOptions(this);
	po->ShowModal();
	po->Destroy();
	Dirtify();
}

void MolDisplayWin::menuFilePrint_preview(wxCommandEvent &event) {
	if (printData == NULL || pageSetupData == NULL) menuFilePage_setup(event);
	if (printData == NULL || pageSetupData == NULL) return;

	wxPrintDialogData printDialogData(*printData);
	wxString title(_T("MacMolPlt printout"));
	wxPrintPreview * preview = new wxPrintPreview(new MolPrintOut(this, title),
												  new MolPrintOut(this, title),
												  &printDialogData);
	if (!preview->Ok()) {//failure to create the print preview
		delete preview;
		wxLogMessage(_("Unable to create the print preview dialog. Aborted!"));
		return;
	}
	wxPreviewFrame * frame = new wxPreviewFrame(preview, this, _T("wxMacMolPlt print preview"),
												wxPoint(100,100), wxSize(600,650));
	frame->Centre(wxBOTH);
	frame->Initialize();
	frame->Show();
}

void MolDisplayWin::menuFilePrint(wxCommandEvent &event) {
	if (printData == NULL || pageSetupData == NULL) menuFilePage_setup(event);
	if (printData == NULL || pageSetupData == NULL) return;
	
	wxPrintDialogData printDialogData(*printData);
	wxPrinter printer(&printDialogData);
	wxString title(_T("MacMolPlt printout"));
	MolPrintOut printout(this, title);
	if (!printer.Print(this, &printout, true)) {
		if (wxPrinter::GetLastError() == wxPRINTER_ERROR) {
			MessageAlert("The printing attempt failed...");
		}
		//otherwise the user canceled printing...
	} else {
		//save off the print data from a successful job
		(*printData) = printer.GetPrintDialogData().GetPrintData();
	}
	glCanvas->Draw();

}

MolPrintOut::MolPrintOut(MolDisplayWin * parent, wxString & title) :
	wxPrintout(title) {
	Parent = parent;
}

void MolPrintOut::GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo) {
	*minPage = 1;
	*maxPage = 1;
	*selPageFrom = 1;
	*selPageTo = 1;
}

bool MolPrintOut::HasPage(int pageNum) {
	return (pageNum == 1);
}

bool MolPrintOut::OnBeginDocument(int startPage, int endPage) {
	if (!wxPrintout::OnBeginDocument(startPage, endPage))
		return false;
	return true;
}

bool MolPrintOut::OnPrintPage(int page) {
	if (page != 1) return false;
	wxDC * dc = GetDC();
    if (dc) {

#ifdef __WXMAC__
		// Preview has a different scale, which I don't quite get.
		if (!IsPreview()) {
			MapScreenSizeToDevice();
		}
		/* dc->SetBrush(*wxLIGHT_GREY_BRUSH); */
#endif

		wxSize dcp = dc->GetPPI();
		int sh, sw;
		GetPPIScreen(&sw, &sh);
		float scale = (float)dcp.GetWidth()/ (float)sw;
		Parent->PrintGL(dc, scale);
		return true;
	}
	return false;
}

void MolDisplayWin::PrintGL(wxDC * dc, const float & sFactor) {
	float scaleFactor = sFactor;
	BeginOperation();
	ProgressInd->ChangeText("Generating Hi-res image");
	Prefs->CylindersForLines(true);
	if (Prefs->GetFitToPage()) {
		int screenWidth, screenHeight;
		glCanvas->GetSize(&screenWidth, &screenHeight);
		float wRatio = ((float) screenWidth)/((float) screenHeight);
		int pageWidth, pageHeight;
		dc->GetSize(&pageWidth, &pageHeight);
		float pRatio = ((float) pageWidth)/((float) pageHeight);
		if (wRatio >= pRatio) { //base scaleup on page width
			scaleFactor = ((float) pageWidth)/((float) screenWidth);
		} else {    //base scaleup on page height
			scaleFactor = ((float) pageHeight)/((float) screenHeight);
		}
	}
	int scale = scaleFactor;
	if (scale < 1) scale = 1;
	Prefs->SetLineWidth(scale);
	glCanvas->GenerateHiResImage(dc, scaleFactor, ProgressInd, Prefs->GetCenterOnPage(),
								 Prefs->GetFramePage());
	Prefs->SetLineWidth(1);
	Prefs->CylindersForLines(false);
	FinishOperation();
}
/* Edit menu */

void MolDisplayWin::menuEditUndo(wxCommandEvent &event) {
	if (mUndoBuffer.GetPosition() == mUndoBuffer.GetOperationCount()) {
		CreateFrameSnapShot();
		mUndoBuffer.SetPosition(mUndoBuffer.GetOperationCount()-1);
	}
	mUndoBuffer.UndoOperation();
	mHighliteState = false;
	for (long i=0; i<MainData->cFrame->NumAtoms; i++) {
		if (MainData->cFrame->Atoms[i].GetSelectState()) {
			mHighliteState = true;
			break;
		}
	}
	ResetModel(false);
	AtomsChanged(true, false);

	menuEdit->UpdateUI(); // force accelerators to reenable
}

void MolDisplayWin::menuEditRedo(wxCommandEvent &event) {
	mUndoBuffer.RedoOperation();
	mHighliteState = false;
	for (long i=0; i<MainData->cFrame->NumAtoms; i++) {
		if (MainData->cFrame->Atoms[i].GetSelectState()) {
			mHighliteState = true;
			break;
		}
	}
	ResetModel(false);
	AtomsChanged(true, false);

	menuEdit->UpdateUI(); // force accelerators to reenable
}

void MolDisplayWin::menuEditCut(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditCopy(wxCommandEvent &event) {
	//Put the image onto the clipboard
	if (wxTheClipboard->Open()) {
		wxDataObjectComposite * comp = new wxDataObjectComposite();
		comp->Add(new wxBitmapDataObject(glCanvas->getImage(0,0)), true);
		
		//Next add a CML version for internal pasting
		BufferFile *Buffer=NULL;
		char *	CML=NULL;
		try {	//First get the size needed for the buffer
			Buffer = new BufferFile(CML, 0);
			Buffer->SetOutput(false);	//no actual writing, just keep track of the total size
			long datalength = MainData->WriteCMLFile(Buffer, Prefs, NULL, false, true);
			delete Buffer;
			CML = new char[datalength+1];
			CML[datalength]='\0';
			Buffer = new BufferFile(CML, datalength);
			MainData->WriteCMLFile(Buffer, Prefs, NULL, false, true);
			
			comp->Add(new wxCMLDataObject(CML), false);
		}
		catch (MemoryError) {
			if (Buffer) delete Buffer;
			if (CML) delete [] CML;
		}
		catch (FileError) {		//This shouldn't happen
			if (Buffer) delete Buffer;
			if (CML) delete [] CML;
		}
		wxTheClipboard->SetData(comp);
		wxTheClipboard->Close();
	}
}

void MolDisplayWin::menuEditCopyCoordinates(wxCommandEvent &event) {
	CopyCoordinates(0);
}
void MolDisplayWin::menuEditCopyNWChemCoordinates(wxCommandEvent &event) {
	CopyCoordinates(2);
}
void MolDisplayWin::CopyCoordinates(short ctype) const {
	//Now copy the coords
	Frame *     lFrame = MainData->cFrame;
	wxString textBuffer;
	
	try {
		if (ctype == 0) {	// GAMESS style atomic_symbol  atomic_number  x  y  z
			wxString    Label;
			for (long iatm=0; iatm<lFrame->NumAtoms; iatm++) {
				if (mHighliteState && !lFrame->Atoms[iatm].GetSelectState()) continue;
				Prefs->GetAtomLabel(lFrame->Atoms[iatm].GetType()-1, Label);
				textBuffer.Append(Label);
				Label.Printf(wxT("   %5.1f  %13.8f  %13.8f  %13.8f\r"),
								 (float) (lFrame->Atoms[iatm].GetType()), 
								 lFrame->Atoms[iatm].Position.x, lFrame->Atoms[iatm].Position.y,
								 lFrame->Atoms[iatm].Position.z);
				textBuffer.Append(Label);
			}
		} else if (ctype == 1) {	// GAMESS style z-matrix
			//Make a guess for the Handle size based on the # of atoms and the line format
			long datalength = lFrame->NumAtoms*70*sizeof(char);
			char * lText = new char[datalength+1];
			//Create a bufferFile object to protect the text buffer from overrun
			BufferFile *Buffer = new BufferFile(lText, datalength);

			Internals * IntCoords = MainData->GetInternalCoordinates();
			if (IntCoords) IntCoords->WriteCoordinatesToFile(Buffer, MainData, Prefs);

			lText[Buffer->GetFilePos()] = '\0';
			textBuffer = wxString(lText, wxConvUTF8);
			delete Buffer;
			delete [] lText;
		} else if (ctype == 2) {
			wxString    Label;
			for (long iatm=0; iatm<lFrame->NumAtoms; iatm++) {
				if (mHighliteState && !lFrame->Atoms[iatm].GetSelectState()) continue;
				Prefs->GetAtomLabel(lFrame->Atoms[iatm].GetType()-1, Label);
				textBuffer.Append(Label);
				Label.Printf(wxT("   %13.8f  %13.8f  %13.8f\r"),
							 lFrame->Atoms[iatm].Position.x, lFrame->Atoms[iatm].Position.y,
							 lFrame->Atoms[iatm].Position.z);
				textBuffer.Append(Label);
			}
		}
	}
	catch (...) {   //The buffer length was probably exceeded, since this shouldn't happen
		return;
	}
	//Put the text onto the clipboard
	if (wxTheClipboard->Open()) {
		wxTheClipboard->SetData(new wxTextDataObject(textBuffer));
		wxTheClipboard->Close();
	}
}

void MolDisplayWin::menuEditPaste(wxCommandEvent &event) {
	if (wxTheClipboard->Open()) {
		if (wxTheClipboard->IsSupported(_("CML"))) {
			wxCMLDataObject cmlObject;
			if (wxTheClipboard->GetData(cmlObject)) {
				char * CML = cmlObject.GetCML();
				if (CML) {
					BeginOperation();
					try {
						if ((MainData->NumFrames > 1)||InEditMode()) {
							MoleculeData *	tdatap = new MoleculeData(this);
							if (!tdatap) return;
							BufferFile *Buffer = new BufferFile(CML, strlen(CML));
							if (tdatap->OpenCMLFile(Buffer, Prefs, NULL, ProgressInd, false, false)) {
								if (InEditMode()) {
									CreateFrameSnapShot();
									//In builder mode copy the selected atoms over
									bool CopyAllAtoms = true;
									for (long i=0; i<tdatap->cFrame->NumAtoms; i++) {
										if (tdatap->cFrame->Atoms[i].GetSelectState()) {
											CopyAllAtoms = false;
											break;
										}
									}
									long initialAtomCount = MainData->cFrame->NumAtoms;
									for (long i=0; i<tdatap->cFrame->NumAtoms; i++) {
										if (CopyAllAtoms || tdatap->cFrame->Atoms[i].GetSelectState()) {
											MainData->NewAtom(tdatap->cFrame->Atoms[i], false);
										}
									}
									for (long i=0; i<initialAtomCount; i++) {
										MainData->cFrame->SetAtomSelection(i, false);
									}
									for (long i=initialAtomCount; i<MainData->cFrame->NumAtoms; i++) {
										MainData->cFrame->SetAtomSelection(i, true);
									}
									MainData->AtomAdded();//update global structures after all atoms are added
									if (Prefs->GetAutoBond())
										MainData->cFrame->SetBonds(Prefs, false, ProgressInd);
									mHighliteState = true;
								} else {
									tdatap->cFrame->PreviousFrame = MainData->cFrame->PreviousFrame;
									tdatap->cFrame->NextFrame = MainData->cFrame->NextFrame;
									if (MainData->cFrame->PreviousFrame)
										MainData->cFrame->PreviousFrame->NextFrame = tdatap->cFrame;
									if (MainData->cFrame->NextFrame)
										MainData->cFrame->NextFrame->PreviousFrame = tdatap->cFrame;
									MainData->cFrame->PreviousFrame = MainData->cFrame->NextFrame = NULL;
									if (MainData->cFrame == MainData->Frames) MainData->Frames = tdatap->cFrame;
									delete MainData->cFrame;
									MainData->cFrame = tdatap->cFrame;
									if (tdatap->MaxAtoms > MainData->MaxAtoms) {
										MainData->MaxAtoms = tdatap->MaxAtoms;
										if (MainData->RotCoords) delete [] MainData->RotCoords;
										MainData->RotCoords = tdatap->RotCoords;
										tdatap->RotCoords = NULL;
									}
									if (tdatap->Basis && !MainData->Basis) {
										MainData->Basis = tdatap->Basis;
										tdatap->Basis = NULL;
									}
									tdatap->cFrame = tdatap->Frames = NULL; //this frame is now in use in MainData
								}
							}
							if (Buffer) delete Buffer;
							delete tdatap;
						} else {
							BufferFile *Buffer = new BufferFile(CML, strlen(CML));
							if (MainData->OpenCMLFile(Buffer, Prefs, NULL, ProgressInd, true, true)) {
							//if (MainData->OpenCMLFile(Buffer, Prefs, &tempData, true)) {
				//				FileSave = 1;
							}
							if (Buffer) delete Buffer;
						}
					}
					catch (MemoryError) {
						MessageAlert("Insufficient memory to complete the paste!");
						Close();
						return;
					}
					catch (DataError) {
						MessageAlert("Invalid data encountered!");
						Close();
						return;
					}
					FinishOperation();
					MainData->ResetRotation();
					FrameChanged();
				}
			}
			wxTheClipboard->Close();
		} else if (wxTheClipboard->IsSupported(wxDF_TEXT)) {
			wxTheClipboard->Close();
			PasteText();
		} else
			wxTheClipboard->Close();
	}
}
void MolDisplayWin::PasteText(void) {
	//relax this restriction later (while in build mode)
	if ((MainData->cFrame->NumAtoms != 0)&&!InEditMode()) return;    //Do not allow pasting if there are already atoms in this frame
	if (wxTheClipboard->Open()) {
		if (wxTheClipboard->IsSupported(wxDF_TEXT)) {
			if (InEditMode()) CreateFrameSnapShot();
			long        iline, test, Type;
			CPoint3D    Position, offset;
			long initialAtomCount = MainData->cFrame->NumAtoms;
			
			wxTextDataObject data;
			wxTheClipboard->GetData(data);
			wxString text = data.GetText();
			char * tbuf = new char[text.Length()+1];
			strncpy(tbuf, text.ToAscii(), text.Length()+1);
			//Clean up the unicode to 7 bit ASCII conversion. WX replaces anything non
			//7-bit with an '_' so convert those into spaces as the only possible guess.
			for (int i=0; i<=text.Length(); i++) if (tbuf[i]=='_') tbuf[i] = ' ';
			BufferFile * TextBuffer=NULL;
			try {
				TextBuffer = new BufferFile(tbuf, text.Length());
				if ((MainData->NumFrames == 1)&&(MainData->cFrame->NumAtoms==0)) {
					//If this is the only frame, make sure it is init'ed
					InitRotationMatrix(MainData->TotalRotation);
				}
				long NumLines = TextBuffer->GetNumLines(-1);
				// There may be up to NumLines atoms so dimension memory accordingly
				if (!MainData->SetupFrameMemory(NumLines+initialAtomCount, 0)) {
					delete TextBuffer;
					delete [] tbuf;
					wxTheClipboard->Close();
					return;
				}
				
				/*Now interpret each of the lines*/
				for (iline=0; iline < NumLines; iline++) {
					char LineText[kMaxLineLength];
					
					TextBuffer->GetLine(LineText);
					//Parse the current line: All values will be set in place if the
					//parse is successful (test will return (-) if not). Normal modes
					//shouldn't be pasted so pass in 0 for Mode.
					test = ParseCartLine(LineText, &Type,&Position, &offset, 0);
					if (test > 0) { /*something was wrong with this line so skip it*/
						
						//A special atom was entered, store its offset.
						if (Type > 115) {
							if (Type > 255)
								if (((Type - 255) < 1)||((Type - 255) > NumLines)) break;
							if (!MainData->cFrame->AddSpecialAtom(offset, iline)) break;
						}
						
						MainData->NewAtom(Type, Position, false);
					}
				}
				MainData->AtomAdded();
				//Done with the text handle so unlock it
				delete TextBuffer;
			}
			catch (...) {
				//Hmm there was a failure in the paste - what should we do to cleanup?
				if (TextBuffer) delete TextBuffer;
			}
			delete [] tbuf;
  
			if (iline == 0) {   /*No atoms were found so clear the memory I just allocated*/
				wxTheClipboard->Close();
				return;
			}

			//If there were special atoms found turn on drawing by default
			if (MainData->cFrame->SpecialAtoms) MainData->SetSpecialAtomDrawMode(true);

			for (long i=0; i<initialAtomCount; i++) {
				MainData->cFrame->SetAtomSelection(i, false);
			}
			for (long i=initialAtomCount; i<MainData->cFrame->NumAtoms; i++) {
				MainData->cFrame->SetAtomSelection(i, true);
			}
			if (Prefs->GetAutoBond())
				MainData->cFrame->SetBonds(Prefs, false, ProgressInd);
			//Now reset the display to take into account the new atoms
			if (initialAtomCount == 0) {
				MainData->CenterModelWindow();
				if (MainData->MaxSize > 0.1) MainData->WindowSize = 2.0*MainData->MaxSize;
				MainData->ResetRotation();
			} else {
				mHighliteState = true;
			}
			FrameChanged();
		}
		wxTheClipboard->Close();
	}
}

void MolDisplayWin::menuEditClear(wxCommandEvent &event) {
	DeleteSelected();
}

void MolDisplayWin::menuEditSelectAll(wxCommandEvent &event) {

	// Select each atom in the current frame and update.
	SetHighliteMode(true);
	for (int i = 0; i < MainData->cFrame->NumAtoms; i++) {
		MainData->cFrame->SetAtomSelection(i, true);
	}
	SelectionChanged(true);
	UpdateModelDisplay();

	menuEdit->UpdateUI(); // force accelerators to reenable

}

void MolDisplayWin::menuEditSelectNone(wxCommandEvent &event) {

	// Deselect each atom in the current frame and update.
	MainData->cFrame->resetAllSelectState();
	SetHighliteMode(false);
	SelectionChanged(true);
	UpdateModelDisplay();

	menuEdit->UpdateUI(); // force accelerators to reenable

}

void MolDisplayWin::menuBuilderShowToolbar(wxCommandEvent &event) {

	ShowToolbar(event.IsChecked());

}

void MolDisplayWin::ShowToolbar(bool enable) {

	bool needs_change;

	Prefs->SetToolbarShown(enable);

	needs_change = Prefs->ToolbarShown() == (toolbar == NULL);
	if (!needs_change) {
		return;
	}

	if (Prefs->ToolbarShown()) {
		toolbar = CreateToolBar(wxTB_HORIZONTAL | wxTB_FLAT | wxTB_TEXT);

		//The original plain xpms
#include "xpms/view.xpm"
#include "xpms/rect_lasso.xpm"
#include "xpms/hand.xpm"

#if wxCHECK_VERSION(2, 8, 0)
		wxStandardPathsBase& gStdPaths = wxStandardPaths::Get();
		wxString pathname = gStdPaths.GetResourcesDir();
#else
		wxString pathname = gStdPaths.GetDataDir();
#ifdef __WXMAC__
		//wxWidgets has a funny idea of where the resources are stored. It locates them as "SharedSupport"
		//but xcode is putting them in Resources.
		pathname.Remove(pathname.Length() - 13);
		pathname += wxT("Resources");
#endif
#endif
		// toolbar->SetMargins(5,5);
		toolbar->SetToolBitmapSize(wxSize(16, 15));

		wxBitmap enabled_bmp;
		wxBitmap enabled_bmp2;

//		enabled_bmp = wxBitmap(view_xpm);
		if (! enabled_bmp.LoadFile(pathname + wxT("/view.png"), wxBITMAP_TYPE_PNG))
			enabled_bmp = wxBitmap(view_xpm);
//		std::cout << "view w="<< enabled_bmp.GetWidth() << " h="<<enabled_bmp.GetHeight()<<std::endl;
//		enabled_bmp.SetWidth(16);
//		enabled_bmp.SetHeight(15);
		toolbar->AddRadioTool(MMP_TOOL_ARROW, _("View"), enabled_bmp,
			wxNullBitmap);

//		std::cout << "lasso w="<< enabled_bmp.GetWidth() << " h="<<enabled_bmp.GetHeight()<<std::endl;
		enabled_bmp = wxBitmap(rect_lasso_xpm);
		enabled_bmp.SetWidth(16);
		enabled_bmp.SetHeight(15);
		toolbar->AddRadioTool(MMP_TOOL_LASSO, _("Select"), enabled_bmp,
			wxNullBitmap);

		enabled_bmp = wxBitmap(hand_xpm);
//		enabled_bmp.LoadFile(pathname + wxT("/test.jpg"), wxBITMAP_TYPE_JPEG);
//		std::cout << "hand w="<< enabled_bmp.GetWidth() << " h="<<enabled_bmp.GetHeight()<<std::endl;
//		enabled_bmp.SetWidth(16);
//		enabled_bmp.SetHeight(15);
		
		toolbar->AddRadioTool(MMP_TOOL_HAND, _("Edit"), enabled_bmp,
			wxNullBitmap);

		toolbar->AddSeparator();
		toolbar->Realize();

	} else {
		delete toolbar;
		toolbar = NULL;

		SetToolBar(NULL);
		
		//Default to not allowing undo once out of edit mode
		mUndoBuffer.Clear();
	}

	glCanvas->SizeChanged();
}

void MolDisplayWin::menuBuilderShowBuildTools(wxCommandEvent &event) {
	ToggleBuilderPalette();
}

void MolDisplayWin::OnSaveStructureUpdate(wxUpdateUIEvent& event) {

	// Only require some atoms to be selected in order to save a 
	// prototype. A single atom as a prototype doesn't make sense.
	event.Enable(MainData->cFrame->GetNumAtomsSelected() > 1);

}

void MolDisplayWin::menuBuilderSaveStructure(wxCommandEvent &event) {

	Frame *frame = MainData->cFrame;
	mpAtom *atoms = frame->Atoms;
	Bond *bonds = frame->Bonds;
	Structure *struc;
	int si = 0;
	int ai, bi;
	int *new_ids;
	int natoms_selected = frame->GetNumAtomsSelected();
	bool has_abinitio = false, has_efrag = false;
	int fragId = -1;

	// Don't bother adding a prototype if nothing's selected.
	if (!natoms_selected) {
		return;
	}

	// We make space for the new structure here.  The memory will be owned
	// by the build palette.
	struc = new Structure;
	struc->natoms = natoms_selected;
	struc->atoms = new mpAtom[struc->natoms];

	// The IDs of the selected atoms will be collapsed so that they're
	// consecutive within the structure.  But to export bonds to the structure,
	// we need a mapping from the current ID to the new ID.
	new_ids = new int[frame->NumAtoms];

	si = 0;
	for (ai = 0; ai < frame->NumAtoms; ai++) {
		if (frame->GetAtomSelection(ai)) {
			struc->atoms[si] = atoms[ai];
			new_ids[ai] = si;
			si++;

			// A prototype can be any number of all electron atoms or one
			// effective fragment.
			if (atoms[ai].IsEffectiveFragment()) {
				has_efrag = true;
				if (fragId < 0) {
					fragId = atoms[ai].GetFragmentNumber();
					struc->FragName = MainData->FragmentNames[fragId-1];
				} else if (fragId != atoms[ai].GetFragmentNumber()) {
					MessageAlert("A prototype can consist of only a single effective fragment.");
					delete struc;
					delete[] new_ids;
					return;
				}
			} else {
				has_abinitio = true;
			}

		} else {
			new_ids[ai] = -1;
		}
	}

	if (has_abinitio && has_efrag) {
		MessageAlert("Prototypes can not mix all-electron and effective fragment atoms.");
		delete struc;
		delete[] new_ids;
		return;
	}

	// If this prototype is an effective fragment with a special definition,
	// we have to save out its raw text.
	if (struc->FragName.size()) {
		std::map<std::string, EFrag>::iterator frag;
		frag = MainData->efrags.find(struc->FragName);
		if (frag != MainData->efrags.end()) {
			struc->frag_def = std::string(frag->second.GetText());
		}
	}

	Bond new_bond;
	std::vector<Bond> new_bonds;
	for (bi = 0; bi < frame->NumBonds; bi++) {
		if (frame->GetAtomSelection(bonds[bi].Atom1) &&
		    frame->GetAtomSelection(bonds[bi].Atom2)) {
			new_bond = Bond(bonds[bi]);
			new_bond.Atom1 = new_ids[new_bond.Atom1];
			new_bond.Atom2 = new_ids[new_bond.Atom2];
			new_bonds.push_back(new_bond);
		}
	}

	struc->nbonds = new_bonds.size();
	struc->bonds = new Bond[MAX(struc->nbonds,10)];
	if (new_bonds.size() > 0) {
		memcpy(struc->bonds, &(new_bonds[0]), sizeof(Bond) * struc->nbonds);
	}
	delete[] new_ids;

	wxTextEntryDialog *dlg =
	   new wxTextEntryDialog(this,
				_("Please enter a name for this custom prototype:"),
				_("Add Prototype"));
	int result;
	do {
		result = dlg->ShowModal();
	} while (result != wxID_CANCEL && dlg->GetValue().Len() <= 0);

	struc->name = dlg->GetValue();

	if (result == wxID_OK) {
		BuilderTool->AddUserStructure(struc);
	} else {
		delete struc;
	}

}
std::map<std::string, EFrag>::const_iterator MolDisplayWin::FindFragmentDef(std::string fragName) {
	std::map<std::string, EFrag>::const_iterator result;
	//first search in the existing list of fragments
	result = MainData->efrags.find(fragName);
	if (result == MainData->efrags.end()) {
		//It's not in the existing list. Search in the builder library
		Structure *struc = BuilderTool->FindFragment(fragName);
		if (struc) {
			//found it. Add it to the current list of fragment definitions
			MainData->efrags.insert(std::pair<std::string, EFrag>(struc->FragName, struc->frag_def));
			result = MainData->efrags.find(fragName);
		}
	}
	return result;
}

void MolDisplayWin::OnShowNormalScreen(wxUpdateUIEvent& event) {
	event.Check(show_fullscreen);
}

void MolDisplayWin::menuViewShowFullScreen(wxCommandEvent &event) {
	show_fullscreen = !show_fullscreen;
	ShowFullScreen(show_fullscreen,
				   wxFULLSCREEN_NOCAPTION | wxFULLSCREEN_NOBORDER);
}

void MolDisplayWin::menuViewShowNormalMode(wxCommandEvent &event) {
	MainData->SetDrawMode(1-MainData->GetDrawMode());
	ResetModel(false);
	Dirtify();
}

void MolDisplayWin::menuViewPrevNormalMode(wxCommandEvent &event) {
	if (MainData->cFrame->Vibs) {
		if (MainData->cFrame->Vibs->CurrentMode>0) {
			ChangeModes(MainData->cFrame->Vibs->CurrentMode - 1);
		}
	}
}
void MolDisplayWin::menuViewNextNormalMode(wxCommandEvent &event) {
	if (MainData->cFrame->Vibs) {
		if (MainData->cFrame->Vibs->CurrentMode<(MainData->cFrame->Vibs->NumModes-1)) {
			ChangeModes(MainData->cFrame->Vibs->CurrentMode + 1);
		}
	}
}
void MolDisplayWin::menuViewOffsetAlongMode(wxCommandEvent &event) {
	CoordinateOffset * co = new CoordinateOffset(this);
	co->ShowModal();
	co->Destroy();
}
void MolDisplayWin::menuViewCenter(wxCommandEvent &event) {
	MainData->CenterModelWindow();
	ResetModel(false);
	Dirtify();
}
void MolDisplayWin::menuViewToggleAutoRotation(wxCommandEvent &event) {
	if (rotate_timer.IsRunning()) {
		rotate_timer.Stop();
		Prefs->SetAutoRotating(false);
	} else {
		int z;
		Prefs->GetAutoRotationVector(inertia.x, inertia.y, z);
		rotate_timer.Start(33, false);
		Prefs->SetAutoRotating(true);
	}
}
void MolDisplayWin::OnModeAnimation(wxTimerEvent & event) {
	if (ModeAnimationData) {
		if (ModeAnimationData->iPoint == ModeAnimationData->NumPoints ||
		    ModeAnimationData->iPoint == -ModeAnimationData->NumPoints) {
			ModeAnimationData->inc *= -1;
			ModeAnimationData->offsetFactor *= -1.0;
		}
		ModeAnimationData->iPoint += ModeAnimationData->inc;
		Frame * lFrame = MainData->cFrame;
		mpAtom * lAtoms = lFrame->Atoms;
		for (int iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
			lAtoms[iatm].Position.x += ModeAnimationData->offsetFactor*(ModeAnimationData->ModeOffset[iatm].x);
			lAtoms[iatm].Position.y += ModeAnimationData->offsetFactor*(ModeAnimationData->ModeOffset[iatm].y);
			lAtoms[iatm].Position.z += ModeAnimationData->offsetFactor*(ModeAnimationData->ModeOffset[iatm].z);
		}
		ResetModel(false);
	}
}
void MolDisplayWin::menuViewAnimateMode(wxCommandEvent &event) {
	if (ModeAnimationData) { //if data already exists toggle off the animation
		ModeAnimationData->m_timer.Stop();
		MainData->SetDrawMode(ModeAnimationData->SavedDrawMode);
		for (int iatm=0; iatm<(MainData->cFrame->NumAtoms); iatm++) {
			MainData->cFrame->Atoms[iatm].Position = ModeAnimationData->SavedCoordinates[iatm];
		}
		delete ModeAnimationData;
		ModeAnimationData = NULL;
		ResetModel(false);
	} else {
		if (!MainData->cFrame->Vibs) return;
		Frame * lFrame = MainData->cFrame;
		ModeAnimationData = new ModeAnimation();
		ModeAnimationData->SavedDrawMode = false;
		ModeAnimationData->iPoint = 1;
		ModeAnimationData->inc = 1;
		ModeAnimationData->SavedCoordinates.reserve(lFrame->NumAtoms);
		ModeAnimationData->ModeOffset.reserve(lFrame->NumAtoms);
		
		if (MainData->GetDrawMode()) {
			ModeAnimationData->SavedDrawMode = true;
			if (!Prefs->GetAnimateMode()) MainData->SetDrawMode(false);
		}
		long cmode = (lFrame->NumAtoms)*(lFrame->Vibs->CurrentMode);
		//The offset factor may need some work yet...
		ModeAnimationData->NumPoints = Prefs->GetAnimationSpeed();
		ModeAnimationData->offsetFactor = 1.0/(4.5*Prefs->GetAnimationSpeed());
		float   VectorScale = Prefs->GetVectorScale();
		mpAtom * lAtoms = lFrame->Atoms;
		CPoint3D temp;
		for (int iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
			ModeAnimationData->SavedCoordinates.push_back(lAtoms[iatm].Position);
			temp = lFrame->Vibs->NormMode[iatm+cmode];
			temp *= VectorScale;
			ModeAnimationData->ModeOffset.push_back(temp);
		}
		ModeAnimationData->m_timer.SetOwner(this, MMP_ANIMATEMODETIMER);
		ModeAnimationData->m_timer.Start(30);//Go for about 30 frames per second
	}
}
void MolDisplayWin::menuViewShowAxis(wxCommandEvent &event) {
	MainData->SetShowAxis(1-MainData->ShowAxis());
	UpdateModelDisplay();
	Dirtify();
}
void MolDisplayWin::menuViewShowSymmetryOperators(wxCommandEvent &event) {
	Prefs->ShowSymmetryOperators(1-Prefs->ShowSymmetryOperators());
	UpdateModelDisplay();
	Dirtify();
}

void MolDisplayWin::menuViewHideAtomLabels(wxCommandEvent &event)
{
	Prefs->ShowAtomicSymbolLabels(false);
	Prefs->ShowAtomNumberLabels(false);
	UpdateModelDisplay();
	Dirtify();
}

void MolDisplayWin::menuViewShowAtomLabel(wxCommandEvent &event)
{
	Prefs->ShowAtomicSymbolLabels(true);
	Prefs->ShowAtomNumberLabels(false);
	UpdateModelDisplay();
	Dirtify();
}

void MolDisplayWin::menuViewShowAtomNumber(wxCommandEvent &event)
{
	Prefs->ShowAtomicSymbolLabels(false);
	Prefs->ShowAtomNumberLabels(true);
	UpdateModelDisplay();
	Dirtify();
}
void MolDisplayWin::menuViewShowBothAtomLabels(wxCommandEvent &event)
{
	Prefs->ShowAtomicSymbolLabels(true);
	Prefs->ShowAtomNumberLabels(true);
	UpdateModelDisplay();
	Dirtify();
}
void MolDisplayWin::menuViewAddMarkAnnotation(wxCommandEvent &event) {
	for (int i=0; i<MainData->cFrame->GetNumAtoms(); i++) {
		if (MainData->cFrame->GetAtomSelection(i)) {
			AnnotationMarker * t = new AnnotationMarker(i);
			MainData->Annotations.push_back(t);
		}
	}
	UpdateModelDisplay();
	Dirtify();
}
void MolDisplayWin::menuViewAddAnnotation(wxCommandEvent &event) {
	wxCommandEvent foo;
	foo.SetId(event.GetId()-MMP_ADDMARKANNOTATION+GL_Popup_Mark_Atom);
	glCanvas->AddAnnotation(foo);
}
void MolDisplayWin::menuViewDeleteAllAnnotations(wxCommandEvent &event) {
	MainData->DeleteAllAnnotations();
	UpdateModelDisplay();
	Dirtify();
}

void MolDisplayWin::menuViewShow2DPattern(wxCommandEvent &event) {
	Prefs->Show2DPattern(1-Prefs->Show2DPattern());
	UpdateModelDisplay();
	Dirtify();
}

void MolDisplayWin::menuViewWireFrameStyle(wxCommandEvent &event)
{
	Prefs->DrawBallnStick(false);
	Prefs->DrawWireFrame(true);
	UpdateModelDisplay();
	Dirtify();
}
void MolDisplayWin::menuViewBallAndStickStyle(wxCommandEvent &event)
{
	Prefs->DrawBallnStick(true);
	Prefs->DrawWireFrame(false);
	UpdateModelDisplay();
	Dirtify();
}

void MolDisplayWin::menuViewEFP_Wireframe(wxCommandEvent &event)
{
	Prefs->ShowEFPWireFrame(1-Prefs->ShowEFPWireFrame());
	UpdateModelDisplay();
	Dirtify();
}

void MolDisplayWin::menuViewToggleAIAtomsVis(wxCommandEvent &event) {
	MainData->cFrame->toggleAbInitioVisibility();
	ResetModel(false);
	Dirtify();
}

void MolDisplayWin::menuViewToggleMMAtomsVis(wxCommandEvent &event) {
	MainData->cFrame->toggleMMAtomVisibility();
	ResetModel(false);
	Dirtify();
}

void MolDisplayWin::menuViewToggleEFPAtomsVis(wxCommandEvent &event) {
	MainData->cFrame->toggleEFPVisibility();
	ResetModel(false);
	Dirtify();
}

void MolDisplayWin::menuViewAnimateFrames(wxCommandEvent &event) {
	long AnimateTime = 10*Prefs->GetAnimateTime();
	if (AnimateTime < 1) AnimateTime = 1;
	if (m_timer.IsRunning()) {
		m_timer.Stop();
		timerRunning = false;
	} else {
		if (MainData->CurrentFrame >= MainData->NumFrames)
			ChangeFrames(1);
		else
			ChangeFrames(MainData->CurrentFrame+1);
		m_timer.SetOwner(this, MMP_ANIMATEFRAMESTIMER);
		m_timer.Start(AnimateTime, wxTIMER_CONTINUOUS);
		timerRunning = true;
	}
}

void MolDisplayWin::menuViewShrink_10(wxCommandEvent &event) {
	MainData->WindowSize *= 1.1;
	ResetView();
	Dirtify();
}

void MolDisplayWin::menuViewEnlarge_10(wxCommandEvent &event) {
	MainData->WindowSize *= 0.9;
	ResetView();
	Dirtify();
}

void MolDisplayWin::menuViewSetWindowParameters(wxCommandEvent &event) {
	windowparameters * temp = new windowparameters(this);
	if (temp->ShowModal() != wxID_CANCEL) {
		Dirtify();
	}
	temp->Destroy();
}

void MolDisplayWin::menuViewRotateInPlace(const Matrix4D new_m) {

	Matrix4D ri;
	CPoint3D trans_centroid, rot_centroid, rot2_centroid;

	trans_centroid = MainData->MolCentroid - MainData->Centroid;
	Rotate3DPt(MainData->TotalRotation, trans_centroid, &rot_centroid);

	CopyMatrix(new_m, MainData->TotalRotation);

	InverseMatrix(MainData->TotalRotation, ri);
	Rotate3DPt(ri, rot_centroid, &rot2_centroid);
	MainData->Centroid = rot2_centroid - MainData->MolCentroid;
	MainData->Centroid *= -1.0;
	MainData->ResetRotation();
	ResetView();
	Dirtify();
}

void MolDisplayWin::menuViewRotateTo_X_axis(wxCommandEvent &event) {

	Matrix4D new_m;
	InitRotationMatrix(new_m);
	new_m[0][2] = new_m[1][1] = 1.0;
	new_m[2][0] = -1.0;
	new_m[0][0] = new_m[2][2] = new_m[1][0] = new_m[0][1] =
	   new_m[1][2] = new_m[2][1] = 0.0;

	menuViewRotateInPlace(new_m);
}

void MolDisplayWin::menuViewRotateTo_Y_axis(wxCommandEvent &event) {

	Matrix4D new_m;
	InitRotationMatrix(new_m);
	new_m[0][0] = new_m[1][2] = 1.0;
	new_m[2][1] = -1.0;
	new_m[1][1] = new_m[2][2] = new_m[0][1] = new_m[0][2] =
	   new_m[1][0] = new_m[2][0] = 0.0;

	menuViewRotateInPlace(new_m);
}

void MolDisplayWin::menuViewRotateTo_Z_axis(wxCommandEvent &event) {

	Matrix4D new_m;
	InitRotationMatrix(new_m);

	menuViewRotateInPlace(new_m);
}

void MolDisplayWin::menuViewRotate180_horizontal(wxCommandEvent &event) {

	Matrix4D tmp;
	Matrix4D new_m;
	CopyMatrix(MainData->TotalRotation, tmp);
	MainData->FlipRotation(0);
	CopyMatrix(MainData->TotalRotation, new_m);
	CopyMatrix(tmp, MainData->TotalRotation);
	menuViewRotateInPlace(new_m);
}

void MolDisplayWin::menuViewRotate180_vertical(wxCommandEvent &event) {

	Matrix4D tmp;
	Matrix4D new_m;
	CopyMatrix(MainData->TotalRotation, tmp);
	MainData->FlipRotation(1);
	CopyMatrix(MainData->TotalRotation, new_m);
	CopyMatrix(tmp, MainData->TotalRotation);
	menuViewRotateInPlace(new_m);
}

void MolDisplayWin::menuViewRotatePrinciple_orientation(wxCommandEvent &event) {
	MainData->RotateToPrincipleOrientation(Prefs);
	ResetView();
	Dirtify();
}

void MolDisplayWin::menuViewRotateOther(wxCommandEvent &event) {
	MainData->Centroid = MainData->MolCentroid;
	SetScreenPlane * temp = new SetScreenPlane(this);
	temp->Show();
}

void MolDisplayWin::menuMoleculeSetBondLength(wxCommandEvent &event) {
	//The set bond length dialog does the work
	SetBondLength * dlg = new SetBondLength(this);
	dlg->ShowModal();
	dlg->Destroy();
	Dirtify();
}

void MolDisplayWin::menuPreferences(wxCommandEvent &event) 
{
		bool isGlobal;
		int id = event.GetId();

	if ( id == wxID_PREFERENCES )
		isGlobal = true;
	else if ( id == MMP_LOCAL_PREFERENCES )
		isGlobal = false;
	else
		MessageAlert("This shouldn't happen!");

	if (prefsDlg) {
		prefsDlg->Raise();
	} else {
		prefsDlg = new setPreference(this, isGlobal);
		if (!isGlobal) {
			winData.PrefsWindowVisible(true);
			prefsDlg->SetSize(winData.GetPrefsWinRect());
		}
		prefsDlg->Show();
	}
}
void MolDisplayWin::ClosePrefsWindow(void) {
	if (prefsDlg) {
		winData.SetPrefsWinRect(prefsDlg->GetRect());
		prefsDlg->Destroy();
		prefsDlg = NULL;
		winData.PrefsWindowVisible(false);
	}
}

void MolDisplayWin::menuMoleculeSetFrameEnergy(wxCommandEvent &event) {
	FrameEnergy * dlg = new FrameEnergy(this);
	dlg->ShowModal();
	dlg->Destroy();
	UpdateFrameText();
	if(energyPlotWindow != NULL) {
		energyPlotWindow->RegenData();
	}
	Dirtify();
}

void MolDisplayWin::menuMoleculeCreateLLMPath(wxCommandEvent &event) {
	//The create LLM dialog does the work
	LLMDialog * llm = new LLMDialog(this);
	llm->ShowModal();
	llm->Destroy();
	Dirtify();
}
void MolDisplayWin::menuMoleculeMinimizeFrameMovements(wxCommandEvent &event) {
	BeginOperation();
	MainData->LinearLeastSquaresFit(ProgressInd);
	FinishOperation();
	Dirtify();
}
void MolDisplayWin::menuSetPointGroup(wxCommandEvent &event) {
	GAMESSPointGroup pg = (GAMESSPointGroup)(event.GetId() - MMP_PGC1 + GAMESS_C1);
	if (! MainData->InputOptions) MainData->InputOptions = new InputData;
	MainData->InputOptions->Data->SetPointGroup(pg);
	ResetView();
	Dirtify();
}
void MolDisplayWin::menuSetPointGroupOrder(wxCommandEvent &event) {
	int order = event.GetId() - MMP_PGORDER2 + 2;
	if (! MainData->InputOptions) MainData->InputOptions = new InputData;
	MainData->InputOptions->Data->SetPointGroupOrder(order);
	ResetView();
	Dirtify();
}
void MolDisplayWin::menuMoleculeDetermineSym(wxCommandEvent &event) {
	SymmetryPointGroupDlg * dlg = new SymmetryPointGroupDlg(this);
	if (dlg->ShowModal() != wxID_CANCEL) {
		//retrieve the selection and set the point group
		GAMESSPointGroup temp;
		int order;
		if (dlg->GetSelectedPointGroup(temp, order)) {
			CreateFrameSnapShot();
			if (! MainData->InputOptions) MainData->InputOptions = new InputData;
			MainData->InputOptions->Data->SetPointGroup(temp);
			MainData->InputOptions->Data->SetPointGroupOrder(order);
			MainData->RotateToPrincipleOrientation(Prefs, dlg->GetTolerance());
			MainData->StickCoordinates();
			//update the list of symmetry unique atoms
			MainData->GenerateSymmetryUniqueAtoms(dlg->GetTolerance());
			MainData->SymmetrizeCoordinates();
			MainData->StickCoordinates();
			if (coordsWindow) coordsWindow->FrameChanged();
			ResetView();
			Dirtify();
		}
	}
	dlg->Destroy();
}
void MolDisplayWin::menuMoleculeSymCoords(wxCommandEvent &event) {
	MainData->RotateToPrincipleOrientation(Prefs);
	MainData->StickCoordinates();
	if (coordsWindow) coordsWindow->FrameChanged();
	ResetView();
	Dirtify();
}
void MolDisplayWin::menuMoleculeConvertToBohr(wxCommandEvent &event) {
	CreateFrameSnapShot();
	MainData->UnitConversion(0);
	ResetAllWindows();
	Dirtify();
}
void MolDisplayWin::menuMoleculeConvertToAngstroms(wxCommandEvent &event) {
	CreateFrameSnapShot();
	MainData->UnitConversion(1);
	ResetAllWindows();
	Dirtify();
}
void MolDisplayWin::menuMoleculeInvertNormalMode(wxCommandEvent &event) {
	MainData->InvertMode();
	ResetModel(false);
	Dirtify();
}

void MolDisplayWin::menuBuilderAddHydrogens(wxCommandEvent &event) {
	bool snapshotCreated=false;
	Frame *	lFrame=MainData->cFrame;
	long NumAtoms = lFrame->NumAtoms;
	
	for (int iatom=0; iatom<NumAtoms; iatom++) {
		short coordination = lFrame->Atoms[iatom].GetCoordinationNumber();
		int bondCount = 0;
		for (long i=0; i<lFrame->NumBonds; i++) {
			if ((iatom == lFrame->Bonds[i].Atom1 ||
			     iatom == lFrame->Bonds[i].Atom2) &&
			     lFrame->Bonds[i].Order > kHydrogenBond) {
				bondCount++;
			}
		}
		for (int k=bondCount; k<coordination; k++) {
			if (!snapshotCreated) {
				CreateFrameSnapShot();
				snapshotCreated = true;
			}
			CPoint3D vector, origin;
			DrawBondingSites(iatom, 0, NULL, k+1, &vector);
			lFrame->GetAtomPosition(iatom, origin);
			MainData->NewAtom(1, origin + vector * 0.01 *
			                  (Prefs->GetAtomSize(lFrame->GetAtomType(iatom)-1) + Prefs->GetAtomSize(0)));
			lFrame->Atoms[lFrame->GetNumAtoms()-1].SetCoordinationNumber(1);
			lFrame->AddBond(iatom,lFrame->GetNumAtoms()-1,kSingleBond);
		}
	}
	if (snapshotCreated) {
		ResetModel(false);
		AtomsChanged(true, false);
		Dirtify();
	}
}
void MolDisplayWin::menuBuilderDeleteHydrogens(wxCommandEvent &event) {
	//Strip off hydrogen atoms from the system. Leave non-bonded hydrogens alone and leave
	//hydrogens that are part of fragments alone. I am tempted to leave hydrogens that are
	//added as part of a precomputed fragment as well. I don't have the infrastructure for that
	//in place yet though.
	bool snapshotCreated=false;
	Frame *	lFrame=MainData->cFrame;
	
	for (int iatom=0; iatom<lFrame->NumAtoms; iatom++) {
		if (lFrame->Atoms[iatom].Type == 1 ) {
			if (lFrame->Atoms[iatom].IsEffectiveFragment() || lFrame->Atoms[iatom].IsSIMOMMAtom()) continue;
			bool hasBonds=false;
			for (long i=0; i<lFrame->NumBonds; i++) {
				if ((iatom == lFrame->Bonds[i].Atom1 ||
				     iatom == lFrame->Bonds[i].Atom2) &&
				    lFrame->Bonds[i].Order > kHydrogenBond) {
					hasBonds = true;
					break;
				}
			}
			if (hasBonds) {
				if (!snapshotCreated) {
					CreateFrameSnapShot();
					snapshotCreated = true;
				}
				MainData->DeleteAtom(iatom, false);
				iatom --;
			}
		}
	}
	if (snapshotCreated) {
		ResetModel(false);
		AtomsChanged(true, false);
		Dirtify();
	}
}

void MolDisplayWin::menuBuilderShowBondSites(wxCommandEvent &event) {
	show_bond_sites = !show_bond_sites;
	UpdateModelDisplay();
}

void MolDisplayWin::menuBuilderSymmetryEdit(wxCommandEvent &event) {
	edit_symmetrically = !edit_symmetrically;

	// When we start editing symmetrically, we deselect symmetry dependent 
	// atoms so they don't bundled together with operations that blindly
	// edit all selected atoms.
	if (edit_symmetrically) {
		Frame *lFrame = MainData->cFrame;
		for (int i = 0; i < lFrame->NumAtoms; i++) {
			if (!lFrame->Atoms[i].IsSymmetryUnique()) {
				lFrame->SetAtomSelection(i, false);
			}
		}
	}

	UpdateModelDisplay();
}

#if 0
void MolDisplayWin::menuBuilderRegenSymmetry(wxCommandEvent &event) {
	RegenerateSymmetryDependent();
}
#endif

void MolDisplayWin::KeyHandler(wxKeyEvent & event) {
	int key = event.GetKeyCode();
	if (!event.HasModifiers()) {
		switch (key) {
			case 'b': case 'B':
				glEdgeFlag(GL_TRUE);
				break;
			case '[':
				if (MainData->cFrame->Vibs &&
				    MainData->cFrame->Vibs->CurrentMode > 0) {
					ChangeModes(MainData->cFrame->Vibs->CurrentMode - 1);
				}
				break;
			case ']':
				if (MainData->cFrame->Vibs &&
				    MainData->cFrame->Vibs->CurrentMode < MainData->cFrame->Vibs->NumModes - 1) {
					ChangeModes(MainData->cFrame->Vibs->CurrentMode + 1);
				}
				break;
			case WXK_BACK:
			case WXK_DELETE:
				DeleteSelected();
				glCanvas->Draw();
				return;
				break;
			case WXK_LEFT:
				if (MainData->CurrentFrame>1) {
					ChangeFrames(MainData->CurrentFrame - 1);
				}
				break;
			case WXK_RIGHT:
				if (MainData->CurrentFrame<MainData->NumFrames) {
					ChangeFrames(MainData->CurrentFrame + 1);
				}
				break;
			case WXK_HOME:
				if (MainData->CurrentFrame>1) {
					ChangeFrames(1);
				}
				break;
			case WXK_END:
				if (MainData->CurrentFrame<MainData->NumFrames) {
					ChangeFrames(MainData->NumFrames);
				}
				break;
			case WXK_ESCAPE:
				if (show_fullscreen) {
					show_fullscreen = false;
					ShowFullScreen(false);
				}
				break;
			default:
				if (InEditMode() &&
				    ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z'))) {
					//Pass general chars to the periodic table to set the atom type
					// TODO: A temporary hack to register outside event.
					event.m_x = -50;
					event.m_y = -50;
					BuilderTool->KeyHandler(event);
					/* return; //I don't think there is any reason to skip on this case? */
				}
				break;
		}
	} else if (key == WXK_ALT && InEditMode()) {
		toolbar->ToggleTool(MMP_TOOL_LASSO, true);
		glCanvas->SetCursor(wxCursor(*wxCROSS_CURSOR));
		UpdateModelDisplay();
		
		mAltModifyingToolBar = true;
	} else if (event.AltDown()) {
		switch (key) {	//as long as we are called from a keydown handler these will upper case
			case 'A':   //option - a
				MainData->cFrame->toggleAbInitioVisibility();
				ResetModel(false);
				Dirtify();
				break;
			case 'E':   //option - e
				MainData->cFrame->toggleEFPVisibility();
				ResetModel(false);
				Dirtify();
				break;
			case 'S':   //option - s
				MainData->cFrame->toggleMMAtomVisibility();
				ResetModel(false);
				Dirtify();
				break;
		}
	}
	event.Skip();
}

/**
 * Deletes the selected atoms if currently in edit mode.
 */
void MolDisplayWin::DeleteSelected() {

	if (InEditMode()) {
		if (mHighliteState) {
			CreateFrameSnapShot();

			// Delete each atom that's selected.
			for (long i=MainData->cFrame->NumAtoms-1; i>=0; i--) {
				if (MainData->cFrame->Atoms[i].GetSelectState()) {
					long j = MainData->DeleteAtom(i, false);
						//restart the scan if more than 1 atom is removed.
					if ((j==0)&&(i!=0)) i = MainData->cFrame->GetNumAtoms()-1;
				}
			}

			// Nothing's highlighted anymore, so turn some features off.
			mHighliteState = false;
			AtomsChanged(true, true);
			/* AdjustMenus(); */
			menuEdit->UpdateUI();
		}
	}

}

void MolDisplayWin::KeyUpHandler(wxKeyEvent & event) {
	//the only keyup event we care about at the moment is alt up when in edit/select mode
	int key = event.GetKeyCode();
	if (key == WXK_ALT && Prefs->ToolbarShown()) {
		if (mAltModifyingToolBar) {
			toolbar->ToggleTool(MMP_TOOL_HAND, true);
			glCanvas->SetCursor(wxCursor(wxCURSOR_HAND));
			UpdateModelDisplay();
			
			mAltModifyingToolBar = false;
		}
	}
	event.Skip();
}
void MolDisplayWin::menuWindowBonds(wxCommandEvent &event) {
	if (bondsWindow) { //need to bring it to the front...
		bondsWindow->Raise();
	} else {
		bondsWindow = new BondsDlg(this);
		bondsWindow->SetSize(winData.GetBondsWinRect());
		bondsWindow->Show();
	}
	winData.BondsWindowVisible(true);
}
void MolDisplayWin::menuWindowMoleculeDisplay(wxCommandEvent &event) {
	// This function is called by a menu item under a subwindow.
	// Since this window always exists we just need to bring it to the front.
	Raise();
}
void MolDisplayWin::CloseBondsWindow(void) {
	if (bondsWindow) {
		winData.SetBondsWinRect(bondsWindow->GetRect());
		bondsWindow->Destroy();
		bondsWindow = NULL;
	}
	winData.BondsWindowVisible(false);
}
void MolDisplayWin::menuWindowCoordinates(wxCommandEvent &event) {
	if (coordsWindow) { //need to bring it to the front...
		coordsWindow->Raise();
	} else {
		coordsWindow = new CoordinatesWindow(this);
		coordsWindow->SetSize(winData.GetCoordsWinRect());
		coordsWindow->Show();
	}
	winData.CoordsWindowVisible(true);
}
void MolDisplayWin::CloseCoordsWindow(void) {
	if (coordsWindow) {
		winData.SetCoordsWinRect(coordsWindow->GetRect());
		coordsWindow->Destroy();
		coordsWindow = NULL;
	}
	winData.CoordsWindowVisible(false);
}
void MolDisplayWin::menuWindowEnergy_plot(wxCommandEvent &event) {
	if(energyPlotWindow) { // need to bring it to the front...
		energyPlotWindow->Raise();
	}
	else {
		energyPlotWindow = new EnergyPlotDialog(this);
		energyPlotWindow->SetSize(winData.GetEnergyWinRect());
		energyPlotWindow->Show();
	}
	winData.EnergyWindowVisible(true);
}
void MolDisplayWin::CloseEnergy_plotWindow(void) {
	if(energyPlotWindow) {
		winData.SetEnergyWinRect(energyPlotWindow->GetRect());
		energyPlotWindow->Destroy();
		energyPlotWindow = NULL;
	}
	winData.EnergyWindowVisible(false);
}
void MolDisplayWin::menuWindowFrequencies(wxCommandEvent &event) {
	if(frequenciesWindow) { // need to bring it to the front...
		frequenciesWindow->Raise();
	}
	else {
		frequenciesWindow = new FrequenciesDialog(this);
		frequenciesWindow->SetSize(winData.GetFrequencyWinRect());
		frequenciesWindow->Show();
	}
	winData.FreqWindowVisible(true);
}
void MolDisplayWin::CloseFrequenciesWindow(void) {
	if(frequenciesWindow) {
		winData.SetFrequencyWinRect(frequenciesWindow->GetRect());
		frequenciesWindow->Destroy();
		frequenciesWindow = NULL;
	}
	winData.FreqWindowVisible(false);
}

void MolDisplayWin::menuWindowInput_builder(wxCommandEvent &event) {
	if(inputBuilderWindow) {
		inputBuilderWindow->Raise();
	}
	else {
		inputBuilderWindow = new InputBuilderWindow(this);
		inputBuilderWindow->Show();
	}
	winData.InputBWindowVisible(true);
}

void MolDisplayWin::CloseInputBuilderWindow(void) {
	if(inputBuilderWindow) {
		winData.SetInputBWinRect(inputBuilderWindow->GetRect());
		inputBuilderWindow->Destroy();
		inputBuilderWindow = NULL;
	}
	winData.InputBWindowVisible(false);
}
void MolDisplayWin::menuWindowSurfaces(wxCommandEvent &event) {
	if(surfacesWindow) { // need to bring it to the front...
	  surfacesWindow->Raise();
	}
	else {
		surfacesWindow = new SurfacesWindow(this);
		surfacesWindow->SetSize(winData.GetSurfacesWinRect());
		surfacesWindow->Show();
	}
	winData.SurfacesWindowVisible(true);
}
void MolDisplayWin::CloseSurfacesWindow(void) {
	if(surfacesWindow) {
		winData.SetSurfacesWinRect(surfacesWindow->GetRect());
		surfacesWindow->Destroy();
		surfacesWindow = NULL;
	}
	winData.SurfacesWindowVisible(false);
}
void MolDisplayWin::menuWindowZMatrixCalc(wxCommandEvent &event) {
	if(zMatCalcDlg) { // need to bring it to the front...
		zMatCalcDlg->Raise();
	} else {
		zMatCalcDlg = new ZMatrixCalculator(this);
		//Since this window is not resizeable leave the width and height alone
		wxRect temp = winData.GetZMatWinRect();
		temp.width = temp.height = -1;
		zMatCalcDlg->SetSize(temp);
		zMatCalcDlg->Show();
	}
	winData.ZMatWindowVisible(true);
}
void MolDisplayWin::CloseZMatrixCalc(void) {
	if(zMatCalcDlg) {
		winData.SetZMatWinRect(zMatCalcDlg->GetRect());
		zMatCalcDlg->Destroy();
		zMatCalcDlg = NULL;
	}
	winData.ZMatWindowVisible(false);
}

/**
 * This function regenerates symmetry dependent atoms and should be called
 * whenever anything to do with atoms is changed.  It deletes all non-symmetry
 * unique atoms and uses the current point group to recreate them based on
 * the remaining unique atoms.
 */
void MolDisplayWin::RegenerateSymmetryDependent() {

	Frame *lFrame = MainData->GetCurrentFramePtr();
	mpAtom *lAtoms = lFrame->Atoms;

	for (int i = lFrame->GetNumAtoms() - 1; i >= 0; i--) {
		if (!lAtoms[i].IsSymmetryUnique()) {
			MainData->DeleteAtom(i, false);
		}
	}

	MainData->GenerateSymmetryDependentAtoms(false);

	// We changed the atoms, so we better update the bonds.
	lFrame->SetBonds(Prefs, true, ProgressInd, false);

}

/**
 * This function should be called whenever anything to do with atoms is
 * changed.  It triggers symmetry dependent atoms to be regenerated, if
 * necessary, updates related windows, and optionally redraws the frame.
 * @param updateCoordsWin True to refresh the coordinates display.
 * @param updateDisplay True to redraw the OpenGL canvas.
 */
void MolDisplayWin::AtomsChanged(bool updateCoordsWin, bool updateDisplay) {

	if (InSymmetryEditMode()) {
		RegenerateSymmetryDependent();
#if wxCHECK_VERSION(3, 0, 0)
		MainData->SymmetrizeCoordinates(wxGetMouseState().LeftIsDown());
#else
		MainData->SymmetrizeCoordinates(wxGetMouseState().LeftDown());
#endif
	}

	if (updateCoordsWin && coordsWindow) coordsWindow->FrameChanged();
	if (bondsWindow) bondsWindow->ResetList();
	if (surfacesWindow) surfacesWindow->Reset();
	if (updateDisplay) FrameChanged();

	Dirtify();

}

void MolDisplayWin::BondsChanged(void) {
	if (bondsWindow) bondsWindow->ResetList();
	FrameChanged();
}

//scan the new frame changing any mode bits for consistency
void MolDisplayWin::FrameChanged(void) {
	//Check for and update any surfaces depending on the screen plane
	Surface * temp = MainData->cFrame->SurfaceList;
	while (temp) {
		temp->RotateEvent(MainData);
		temp = temp->GetNextSurface();
	}
	UpdateModelDisplay();
	Dirtify();
	AdjustMenus();
}
void MolDisplayWin::ChangeFrames(long NewFrame) {
	if ((NewFrame>0)&&(NewFrame<=MainData->NumFrames)) {
		if (NewFrame < MainData->CurrentFrame) {
			MainData->cFrame = MainData->Frames;
			MainData->CurrentFrame = 1;
		}
		while (MainData->CurrentFrame < NewFrame) {
			MainData->cFrame = MainData->cFrame->NextFrame;
			MainData->CurrentFrame++;
		}
		MainData->ResetRotation();
		FrameChanged();

		if (coordsWindow) coordsWindow->FrameChanged();
		if (bondsWindow) bondsWindow->ResetList();
		if (energyPlotWindow) energyPlotWindow->FrameChanged();
		if (frequenciesWindow) frequenciesWindow->FrameChanged();
		if (surfacesWindow) surfacesWindow->Reset();
		if (zMatCalcDlg) zMatCalcDlg->UpdateValues();
		myStatus->SetScrollBarValue(MainData->CurrentFrame-1);
	}
}

void MolDisplayWin::SetStatusText(const wxString& label) {
	status_timer.Start(5000, true);
	myStatus->SetStatusText(label);
}

void MolDisplayWin::OnStatusTimer(wxTimerEvent& event) {
	UpdateFrameText();
}

void MolDisplayWin::ModeChanged(void) {
	StopAnimations();
	if(frequenciesWindow != NULL) {
		frequenciesWindow->ModeChanged();
	}
	if(!MainData->GetDrawMode()) {
		MainData->SetDrawMode(true);
		menuView->Check(MMP_SHOWMODE, true);
	}
	ResetModel(false);
	Dirtify();
}
void MolDisplayWin::ChangeModes(long NewMode) {
	if(NewMode >= 0 && NewMode < MainData->cFrame->Vibs->GetNumModes()) {
		MainData->cFrame->Vibs->CurrentMode = NewMode;
		ModeChanged();
	}
}
#include <iostream>
void MolDisplayWin::UpdateFrameText(void) {
	// Add a little information
	wxString output;
	Boolean WriteEnergy=true;
	if (MainData->cFrame->Vibs) {
	  if (MainData->GetDrawMode()) {
		  wxString str(MainData->cFrame->Vibs->Frequencies[MainData->cFrame->Vibs->CurrentMode].c_str(), wxConvUTF8);
		  output.Printf(wxT("Freq=%s"), str.c_str());
		  WriteEnergy = false;
	  }
	}
	if (WriteEnergy) {
		double  Energy = 0.0;
		EnergyOptions * lEOpts = Prefs->GetEnergyOptions();
		GraphOptions * lPOpts = Prefs->GetGraphOptions();
		float   UnitFactor = 1.0;
		if (lEOpts->GetDisplayUnits() == kKCalPerMole) UnitFactor = kHartreeTokCalPMol;
		if (lEOpts->PlotMPEnergy())
			Energy = (MainData->cFrame->GetMP2Energy()-lEOpts->GetY1Zero())*UnitFactor;
		else if (lEOpts->PlotEnergy())
			Energy = (MainData->cFrame->GetEnergy()-lEOpts->GetY1Zero())*UnitFactor;
		else if (lEOpts->PlotKEnergy())
			Energy = (MainData->cFrame->GetKineticEnergy()-lEOpts->GetY2Zero())*UnitFactor;
		else if (lEOpts->PlotPEnergy())
			Energy = (MainData->cFrame->Energy - MainData->cFrame->GetKineticEnergy()-lEOpts->GetY1Zero())*UnitFactor;
		if (Energy != 0.0) {
			output.Printf(wxT("E=%.*f"), lEOpts->GetNumDigits(), Energy);
		}
		if (lPOpts->PlotRMSGradient()) {
			wxString temp;
			temp.Printf(wxT(" RMS=%.*f"), lEOpts->GetNumDigits(),
					MainData->cFrame->GetRMSGradient()-lEOpts->GetY2Zero());
			output += temp;
		} else if (lPOpts->PlotMaxGradient()) {
			wxString temp;
			temp.Printf(wxT(" Max Grad=%.*f"), lEOpts->GetNumDigits(),
					MainData->cFrame->GetMaxGradient()-lEOpts->GetY2Zero());
			output += temp;
		} else if (lPOpts->PlotBondLength()) {
			long a1, a2;
			float bLength;
			a1 = lPOpts->Get1stAtom();
			a2 = lPOpts->Get2ndAtom();
			if (MainData->cFrame->GetBondLength(a1, a2, &bLength)) {
				wxString temp;
				temp.Printf(wxT(" Bond %ld-%ld=%.*f"), a1+1, a2+1,
						lEOpts->GetNumDigits(), bLength-lEOpts->GetY2Zero());
				output += temp;
			}
		} else if (lPOpts->PlotBondAngle()) {
			long a1, a2, a3;
			float bAngle;
			a1 = lPOpts->Get1stAtom();
			a2 = lPOpts->Get2ndAtom();
			a3 = lPOpts->Get3rdAtom();
			if (MainData->cFrame->GetBondAngle(a1, a2, a3, &bAngle)) {
				wxString temp;
				temp.Printf(wxT(" Angle %ld-%ld-%ld=%.*f"), a1+1, a2+1, a3+1,
						lEOpts->GetNumDigits(), bAngle-lEOpts->GetY2Zero());
				output += temp;
			}
		}
	}
	wxString ft;
	ft.Printf(wxT(" Frame %ld of %ld"), MainData->GetCurrentFrame(), MainData->GetNumFrames());
	output += ft;

	myStatus->SetStatusText(output);
}

/**
 * This function redraws the scene, essentially.  Use this to do a barebones
 * refresh.  If any data has changed, use a more specific function like
 * AtomsChanged or BondsChanged.
 */
void MolDisplayWin::UpdateModelDisplay(void) {
	UpdateFrameText();
	ReleaseLists();
	glCanvas->Draw();
}

void MolDisplayWin::ResetView(void) {
	//Check for and update any surfaces depending on the screen plane
	Surface * lSurface = MainData->cFrame->SurfaceList;
	while (lSurface) {
		lSurface->RotateEvent(MainData);
		lSurface = lSurface->GetNextSurface();
	}
	//Check the select state
	mHighliteState = false;
	for (long i=0; i<MainData->cFrame->NumAtoms; i++) {
		if (MainData->cFrame->Atoms[i].GetSelectState()) {
			mHighliteState = true;
			break;
		}
	}
	/* glCanvas->UpdateGLView(); */
	glCanvas->Draw();
}

void MolDisplayWin::ResetModel(bool Center) {
	if (Center) {
		MainData->CenterModelWindow();
		MainData->WindowSize = 1.25*(MainData->MaxSize+1.0);
	}
	MainData->ResetRotation();
	//Check for and update any surfaces depending on the screen plane
	Surface * lSurface = MainData->cFrame->SurfaceList;
	while (lSurface) {
		lSurface->RotateEvent(MainData);
		lSurface = lSurface->GetNextSurface();
	}
	ReleaseLists();
	// Reset the frame scroll bar
	myStatus->SetScrollBar(MainData->CurrentFrame-1, MainData->NumFrames);
	UpdateFrameText();

	glCanvas->Draw();

	// This is commented out because this function's called on file open,
	// when the scene is definitely not dirty.
	/* Dirtify(); */

	AdjustMenus();
}

void MolDisplayWin::ResetAllWindows(void) {
	ResetModel(false);
	
	//force updates for all the child windows
	if (coordsWindow) coordsWindow->FrameChanged();
	if (bondsWindow) bondsWindow->ResetList();
	if (energyPlotWindow) energyPlotWindow->RegenData();
	if (frequenciesWindow) frequenciesWindow->FrameChanged();
	if (zMatCalcDlg) zMatCalcDlg->UpdateValues();
	
}
void MolDisplayWin::BeginOperation(void) {
	ProgressInd = new Progress;
	if (!ProgressInd) throw MemoryError();
	OperationInProgress = true;
}
void MolDisplayWin::FinishOperation(void) {
	if (ProgressInd) {
		delete ProgressInd;
		ProgressInd = NULL;
	}
	OperationInProgress = false;
}
void MolDisplayWin::AbortOpen(const char * msg) {
	//On non-Mac systems we don't close the last window, instead
	//reinitialize the window data and leave open
#ifndef __WXMAC__
	MpApp & app = wxGetApp();
	if (app.WindowCount() <= 1) {
		//This is the last window! Clear it out, but leave it open
		delete MainData;
		MainData = new MoleculeData(this);
		delete Prefs;
		Prefs = new WinPrefs;
		*Prefs = *gPreferences;
		Dirtify();
		SetTitle(wxT("Untitled"));
		SetName(wxT("Untitled"));
	} else
#endif
	{
		Close(true);
	}
	
	if (msg != NULL) MessageAlert(msg);
} /* AbortOpen */

long MolDisplayWin::OpenFile(wxString fileName, float offset, bool flip, bool append) {
	//This is modeled on OpenTextFile in the Mac version
	long                test=0;
	TextFileType type;
	
	FILE * myfile = fopen(fileName.mb_str(wxConvUTF8), "rb");
	if (myfile == NULL) {
		if (append)
			AbortOpen("Unable to open the requested file.");
		else
			MessageAlert("Unable to open the requested file.");
		return 0;
	}
	BufferFile * Buffer = NULL;
	try {
		Buffer = new BufferFile(myfile, false);
		//this is temporary, they are supposed to come from the open file dialog
		long flipval = 1;
		if (flip) flipval = -1;
		
		// Attempt to identify the file type by looking for key words
		type = Buffer->GetFileType((const char *) fileName.mb_str(wxConvUTF8));
		BeginOperation();
		switch (type) {
			case kMolType:
				if (!append)
					test = OpenMolPltFile(Buffer);
				break;
			case kGAMESSlogType:
				test = OpenGAMESSlog(Buffer, append, flipval, offset);
				break;
			case kGAMESSIRCType:
				test = OpenGAMESSIRC(Buffer, append,flipval,offset);
				break;
			case kGAMESSDRCType:
				test = OpenGAMESSDRC(Buffer, false, append,flipval,offset);
				break;
			case kGAMESSTRJType:
				test = OpenGAMESSTRJ(Buffer, append,flipval,offset);
				break;
			case kGAMESSInputType:
				if (!append)
					test = OpenGAMESSInput(Buffer);
				break;
			case kXYZType:
				if (!append)
					test = OpenXYZFile(Buffer);
				break;
			case kPDBType:
				if (!append)
					test = OpenPDBFile(Buffer);
				break;
			case kMDLMolFile:
				if (!append)
					test = OpenMDLMolFile(Buffer);
				break;
			case CMLFile:
			{
				if (!append) {
					test = OpenCMLFile(Buffer);
					if (test == 0) AbortOpen(NULL);
				}
			}
				break;
			case MolDenFile:
				test = OpenMoldenFile(Buffer);
				break;
			case MolekelFile:
				test = OpenMKLFile(Buffer);
				break;
			case MOPACarcFile:
				test = OpenMOPACFile(Buffer, type);
				break;
			case MOPACmopFile:
				test = OpenMOPACFile(Buffer, type);
				break;
			default:    //Should only get here for unknown file types.
			{
				Buffer->SetFilePos(0);
				char test[kMaxLineLength];
				Buffer->Read(test, sizeof(char) * kMaxLineLength);
				if (strcmp(test, "BMBm") == 0 || strcmp(test, "mBMB") == 0) {
					if (append) MessageAlert("Version 6 and later does not support the MacMolPlt binary format. Please convert to CML with version 5.6 and try again.");
					else AbortOpen("Version 6 and later does not support the MacMolPlt binary format. Please convert to CML with version 5.6 and try again.");
				} else {
					if (FindKeyWord(test, "\rtf", 4)) wxLogMessage(_("This file appears to be an RTF file. MacMolPlt requires all text files to be in the plain text format."));
					if (!append)
						AbortOpen("Unable to determine the file type.");
					else
						MessageAlert("Unable to determine the file type.");
				}
			}
		}
	}
	catch (std::bad_alloc) {//Out of memory error
		if (!append)
			AbortOpen("Not enough memory to open the file. Aborted!");
	}
	catch (MemoryError) {
		if (!append)
			AbortOpen("Not enough memory to open the file. Aborted!");
	}
	catch (UserCancel) {
		if (!append)
			AbortOpen("File open canceled by user");
	}
//  catch (DataError Error) {//Error parsing the file data
//      if (!Error.ErrorSet())  Window->AbortOpen(21);
//      else {
//          Error.WriteError();
//          delete Window; Window = NULL;
//      }
//  }
	//Some kind of File system related error
	catch (FileError Error) { Error.WriteError();
		if (!append) AbortOpen(NULL);}
	catch (...) {
		if (!append)
			AbortOpen("Unknown error reading the selected file. File open aborted.");
	}
	if (Buffer) delete Buffer;      //Done reading so free up the buffer
	fclose(myfile);					//Text files are not used after opening so close it immediately
	FinishOperation();				//Close the progress dialog, if opened

	if (test && !append) {//Note test is left 0 if any exception occurs(which causes Window to be deleted)
#ifdef __WXOSX_CARBON__
	  if (gPreferences->ChangeFileType()) {
		  // Looks like this is a good file so set the creator type for the neat icon
		  FSRef mFSRef;
		  const char * t = fileName.mb_str(wxConvUTF8);
		  OSStatus s = FSPathMakeRef((const UInt8 *) t, &mFSRef, false);
		  if (s == noErr) {
			  FSCatalogInfoBitmap fields = kFSCatInfoFinderInfo;
			  FSCatalogInfo info;
  
			  FSGetCatalogInfo(&mFSRef, fields, &info, NULL, NULL, NULL);
			  FileInfo * mfinfo = (FileInfo *) (&(info.finderInfo));
			  if (mfinfo->fileCreator != (OSType) 'BMBm') {
				  mfinfo->fileCreator = (OSType) 'BMBm';
				  FSSetCatalogInfo(&mFSRef, fields, &info);
			  }
		  }
	  }
#endif
		if ((type != CMLFile) || (test < 10)) ResetModel(true);
		else ResetModel(false);

		// Set the path for saving changes
		if(type == CMLFile) {
			currFilePath = fileName;
		} else {
			currFilePath = wxT("");
		}
	}
	if (append) {
		if ((type != CMLFile) || (test < 10)) ResetModel(true);
	}

	ShowToolbar(Prefs->ToolbarShown());
	menuBuild->Check(MMP_SHOW_TOOLBAR, Prefs->ToolbarShown());

	return test;
}

long MolDisplayWin::OpenCMLFile(BufferFile * Buffer, bool readPrefs, bool readWindows) {
	bool localprogress=false;
	if (!ProgressInd) {
		localprogress = true;
		BeginOperation();
	}
	ProgressInd->ChangeText("Reading CML file...");
	long test = 0;
	if (readWindows) {
		//This option reads in the saved window rects (if any) and the state of 
		//the various subwindows. This option is normally only run when reading in
		//a file to a new window.
		test = MainData->OpenCMLFile(Buffer, Prefs, &winData, ProgressInd, readPrefs);
		if (test) {
			SetSize(winData.GetMolWinRect());
			if (Prefs->ToolbarShown()) {
				ShowToolbar();
				menuBuild->Check(MMP_SHOW_TOOLBAR, Prefs->ToolbarShown());
				
				wxCommandEvent foo;
				foo.SetId(MMP_TOOL_HAND);
				OnToggleTool(foo);
			}
			wxCommandEvent foo;	//just a placeholder so we can call the menu event functions
			if (winData.BondsWindowVisible()) menuWindowBonds(foo);
			if (winData.CoordsWindowVisible()) menuWindowCoordinates(foo);
			if (winData.EnergyWindowVisible()) menuWindowEnergy_plot(foo);
			if (winData.FreqWindowVisible()) menuWindowFrequencies(foo);
			if (winData.InputBWindowVisible()) menuWindowInput_builder(foo);
			if (winData.SurfacesWindowVisible()) menuWindowSurfaces(foo);
				//I don't think the prefs window should reopen automatically?
			if (winData.PrefsWindowVisible()) winData.PrefsWindowVisible(false);
			if (winData.ZMatWindowVisible()) menuWindowZMatrixCalc(foo);
			if (Prefs->isAutoRotating()) menuViewToggleAutoRotation(foo);
			Raise();
		}
	} else
		test = MainData->OpenCMLFile(Buffer, Prefs, NULL, ProgressInd, readPrefs);
	for (long i=0; i<MainData->cFrame->NumAtoms; i++) {
		if (MainData->cFrame->Atoms[i].GetSelectState()) {
			mHighliteState = true;
			break;
		}
	}
	if (readPrefs && test >= 10) {
		//Update the canvas since a CMLfile reads in user preferences
		glCanvas->SetCurrent();
		DoPrefDependent();
	}
	if (localprogress) FinishOperation();
	return test;
}
WindowData::WindowData(void) {
	//These are all used for windows where -1 indicates the default value that
	//causes wx to compute appropriate values
	BondsVis = CoordsVis = EnergyVis = FreqVis = SurfacesVis = InputBVis = PrefVis = ZMatVis = false;
	MolWinRect.x = MolWinRect.y = -1;
	MolWinRect.width = MolWinRect.height = 450;
	BondsWinRect.x = BondsWinRect.y = BondsWinRect.width = BondsWinRect.height = -1;
	CoordsWinRect.x = CoordsWinRect.y = CoordsWinRect.width = CoordsWinRect.height = -1;
	EnergyWinRect.x = EnergyWinRect.y = EnergyWinRect.width = EnergyWinRect.height = -1;
	FreqWinRect.x = FreqWinRect.y = FreqWinRect.width = FreqWinRect.height = -1;
	SurfacesWinRect.x = SurfacesWinRect.y = SurfacesWinRect.width = SurfacesWinRect.height = -1;
	InputBuilderRect.x = InputBuilderRect.y = InputBuilderRect.width = InputBuilderRect.height = -1;
	ZMatRect.x = ZMatRect.y = ZMatRect.width = ZMatRect.height = -1;
	PreferenceWinRect.x = PreferenceWinRect.y = PreferenceWinRect.width = PreferenceWinRect.height = -1;
}
void WindowData::Validate(wxRect & testRect) {
	bool valid = false;
		//bah... wxDisplay is not implemented on all platforms (doesn't seem to 
		//be active on Windoze with wx 2.6.3)
	int displayCount = 1;
#if wxUSE_DISPLAY
	displayCount = wxDisplay::GetCount();
#endif
	for (int i=0; i<displayCount; i++) { //iterate over the displays
		//retrieve the usable rect for this display
		//In theory wx 2.8 has a GetClientArea function that is equivelant to wxGetClientDisplayRect
		//however in 2.8.0 it doesn't properly account for the Mac menu bar
		wxRect displayRect;
		if (i == 0) {
			displayRect = wxGetClientDisplayRect();
		} else {
#if wxUSE_DISPLAY
	//There is no else here because of the #if above
			wxDisplay iDisplay(i);
			displayRect = iDisplay.GetGeometry();
#endif
		}
		//Test the sample rect to see if it lies (at least partially) on this display
		if (testRect.x >= displayRect.x &&
		    testRect.y >= displayRect.y &&
		    testRect.x < (displayRect.x + displayRect.width) &&
		    testRect.y < (displayRect.y + displayRect.height)) {
			//ok rect tests within this window
			//now test the size to make sure the window can be placed fully on this screen
			if (testRect.width < 1) testRect.width = -1;
			else if (testRect.width > displayRect.width) testRect.width = displayRect.width;
			if (testRect.height < 1) testRect.height = -1;
			else if (testRect.height > displayRect.height) testRect.height = displayRect.height;
			valid = true;
			break;
		}
	}
	if (!valid) {
		testRect.x = testRect.y = testRect.width = testRect.height = -1;
	}
}
void MolDisplayWin::UpdateWindowData(void) {
	winData.SetMolWinRect(GetRect());
	if (bondsWindow != NULL) {
		winData.BondsWindowVisible(true);
		winData.SetBondsWinRect(bondsWindow->GetRect());
	} else winData.BondsWindowVisible(false);
	if (coordsWindow != NULL) {
		winData.CoordsWindowVisible(true);
		winData.SetCoordsWinRect(coordsWindow->GetRect());
	} else winData.CoordsWindowVisible(false);
	if (energyPlotWindow != NULL) {
		winData.EnergyWindowVisible(true);
		winData.SetEnergyWinRect(energyPlotWindow->GetRect());
	} else winData.EnergyWindowVisible(false);
	if (frequenciesWindow != NULL) {
		winData.FreqWindowVisible(true);
		winData.SetFrequencyWinRect(frequenciesWindow->GetRect());
	} else winData.FreqWindowVisible(false);
	if (surfacesWindow != NULL) {
		winData.SurfacesWindowVisible(true);
		winData.SetSurfacesWinRect(surfacesWindow->GetRect());
	} else winData.SurfacesWindowVisible(false);
	if (inputBuilderWindow != NULL) {
		winData.InputBWindowVisible(true);
		winData.SetInputBWinRect(inputBuilderWindow->GetRect());
	} else winData.InputBWindowVisible(false);
	if (prefsDlg != NULL) {
		winData.PrefsWindowVisible(true);
		winData.SetPrefsWinRect(prefsDlg->GetRect());
	} else winData.PrefsWindowVisible(false);
	if (zMatCalcDlg != NULL) {
		winData.ZMatWindowVisible(true);
		winData.SetZMatWinRect(zMatCalcDlg->GetRect());
	} else winData.ZMatWindowVisible(false);
}
void MolDisplayWin::SetDefaultWindowData(void) {
	//first make sure the data is up to date
	UpdateWindowData();
	//copy it onto to the defaults
	gWindowDefault = winData;
	//Now save the user preferences (no way to save only the window data)
	gPreferences->WriteUserPrefs();
}

void MolDisplayWin::OnRotateTimer(wxTimerEvent& event) {

	wxMouseEvent mouse_event = wxMouseEvent(wxEVT_MOTION);
	mouse_event.m_leftDown = true;
	int z;
	Prefs->GetAutoRotationVector(inertia.x, inertia.y, z);
	Rotate(mouse_event);
}

void MolDisplayWin::Rotate(wxMouseEvent &event) {

	wxPoint q, sphereCenter;
	int dx, dy;
	Matrix4D rotationMatrix, tempcopyMatrix;
	wxRect sphereRect;
	long hsize, vsize, width, hoffset, sphereRadius;
	Surface *lSurface;
	bool UpdateSurface = false;

	/* glCanvas->SetCurrent(); */
	
	// Some surfaces need to be adjusted for the new screen plane.
	lSurface = MainData->cFrame->SurfaceList;
	while (lSurface) {
		UpdateSurface = UpdateSurface || lSurface->NeedsUpdateDuringRotation();
		lSurface = lSurface->GetNextSurface();
	}

	wxRect DisplayRect = glCanvas->GetRect();
	hsize = DisplayRect.GetWidth();
	vsize = DisplayRect.GetHeight();

	/* Figure out where to place the Virtual Sphere cue. */
	width = hsize;
	sphereCenter.x = hsize/2;
	sphereCenter.y = vsize/2;
	if (sphereCenter.x >= sphereCenter.y)
		sphereRadius   = (long)((float) (sphereCenter.x)*0.9);
	else
		sphereRadius   = (long)((float) (sphereCenter.y)*0.9);
	hoffset = sphereCenter.x;
	hsize = MAX(hsize, vsize);
	sphereRect.SetHeight(sphereRadius);
	sphereRect.SetWidth(sphereRadius);
	sphereRect.SetX(sphereCenter.x - sphereRadius);
	sphereRect.SetY(sphereCenter.y - sphereRadius);

	// If this mouse event was generated because the user pressed a button
	// down, we reset any autorotation, clear the inertia, and record the
	// position.
	if (event.ButtonDown()) {

		// The mouse position is normally easy to calculate.  However, if the
		// event was generated by the programmer, the position is (0,0), in
		// which case we must determine the real mouse position.
		mouse_start = event.GetPosition();
		if (mouse_start.x == 0 && mouse_start.y == 0) {
			mouse_start = ScreenToClient(wxGetMousePosition());
		}
		//not sure that the following clear is needed.
	//	inertia.x = 0;
	//	inertia.y = 0;
		if (rotate_timer.IsRunning()) {
			rotate_timer.Stop();
			Prefs->SetAutoRotating(false);
		}
	}
	
	// For some reason the mouse position isn't always correct if the click
	// brought focus to the window.
	else if (window_just_focused) {
		mouse_start = ScreenToClient(wxGetMousePosition());
		window_just_focused = false;
	}
	
	// Now, if we're dragging we've got to do a bit of work.
	else if (event.Dragging()) {

		// We're going to need to compare the current mouse position with
		// the previous mouse position to figure out how much to transform.
		// So, we'll need to figure out the current mouse position.


		// If autorotation is going on, the actual mouse position isn't really
		// what we want.  Instead, we pretend that the mouse position is the
		// previous mouse position plus the inertial direction.
		if (rotate_timer.IsRunning()) {
			dx = inertia.x;
			dy = inertia.y;
			mouse_start = sphereCenter;
			q.x = mouse_start.x + inertia.x;
			q.y = mouse_start.y + inertia.y;
		}
		
		// If no autorotation is happening, the mouse position can be retrieved
		// by normal means.
		else {
			q = event.GetPosition();
			dx = q.x - mouse_start.x;
			dy = q.y - mouse_start.y;
			inertia.x = dx;
			inertia.y = dy;
		}

		// Don't do any work if there wasn't any mouse movement.
		if (dx != 0 || dy != 0) {

			// The right mouse button or Command-{Left,Middle} produces a 
			// translation along screen spaces axes.
			if (event.CmdDown() || event.RightIsDown()) {

				// If shift is also held down, we depth-translate along the
				// viewing direction.
				if (event.ShiftDown()) {
					float factor = 1.2f * MainData->WindowSize / hsize;
					CPoint3D eye_z_in_obj(MainData->TotalRotation[0][2],
							              MainData->TotalRotation[1][2],
										  MainData->TotalRotation[2][2]);

					eye_z_in_obj *= -inertia.y * factor;
					MainData->Centroid -= eye_z_in_obj;
				}
				
				// Otherwise we move in proportion to the window size along
				// the screen's x- and y-axes.
				else {
					float factor = 1.2f * MainData->WindowSize / hsize;
					CPoint3D eye_x_in_obj(MainData->TotalRotation[0][0],
							              MainData->TotalRotation[1][0],
										  MainData->TotalRotation[2][0]);
					CPoint3D eye_y_in_obj(MainData->TotalRotation[0][1],
							              MainData->TotalRotation[1][1],
										  MainData->TotalRotation[2][1]);

					eye_x_in_obj *= inertia.x * factor;
					eye_y_in_obj *= -inertia.y * factor;
					MainData->Centroid -= eye_x_in_obj;
					MainData->Centroid -= eye_y_in_obj;
				}
			}
			
			// {Left,Middle} click without Command/Control, at which we rotate or
			// z-translate.
			else {

				// Z-translate.
				if (event.ShiftDown() || event.MiddleIsDown()) {
					if (dy == 0) return;
					if (dy > 99) dy = 99;
					MainData->WindowSize *= 1.0 + 0.01*dy;
				}
				
				// Rotate.
				else {
					VirtualSphereQD3D(mouse_start, q,
					                  sphereCenter, sphereRadius,
					                  rotationMatrix, MainData->TotalRotation);

					// Concatenate the new rotation with the current rotation
					MultiplyMatrix(rotationMatrix, MainData->TotalRotation, tempcopyMatrix);
					CopyMatrix(tempcopyMatrix, MainData->TotalRotation);

					// Clean up the rotation matrix make the rotation part orthogonal and magnitude 1
					OrthogonalizeRotationMatrix (MainData->TotalRotation);
				}
			}
			
			// Remember previous mouse point for next iteration.
			mouse_start = q;
		}

	}
	
	// If the user releases the Left button while still moving, we cause the
	// atoms to autorotate.
	else if (event.LeftUp() && Prefs->AutoRotationEnabled()) {
		if (abs(inertia.x) > 3 || abs(inertia.y) > 3) {
			rotate_timer.Start(33, false);
			Prefs->SetAutoRotating(true);
			Prefs->SetAutoRotationVector(inertia.x, inertia.y, 0);
		}
	}
	// No matter what mouse button/key combination we have, we update
	// any surfaces that depend on the screen plane.
	// This may update a little more than needed, but it needs to be here to handle the mouse wheel event
	if (UpdateSurface) {
		lSurface = MainData->cFrame->SurfaceList;
		while (lSurface) {
			lSurface->RotateEvent(MainData);
			lSurface = lSurface->GetNextSurface();
		}
	}
	
	/* Update the window */
	if (UpdateSurface) ReleaseLists();

	// If the scene is actively being rotated, we call a function that sorts
	// transparent triangles, draws the atoms and some rotation annotations.
	// For autorotation, a left mouse click is faked, but we don't want to
	// annotate then.
	if (event.LeftIsDown() && event.Dragging()) {
		if (transpTriList) { //update the transparent surface sorting
			SortTransparentTriangles();
		}
		do_rotate_annotation = true;
		glCanvas->Draw();
		do_rotate_annotation = false;
		Dirtify();
	}
	
	// Otherwise we do a plain drawing of the atoms.
	else {
		glCanvas->Draw();
	}

}

bool MolDisplayWin::IsRotating(void) {

	return rotate_timer.IsRunning();

}

void MolDisplayWin::SetWindowPreferences(WinPrefs *NewPrefs) {

	if (Prefs && Prefs != NewPrefs) {
		*Prefs = *NewPrefs;
	}

	glCanvas->SetCurrent();
	DoPrefDependent();
	ResetAllWindows();

}

void MolDisplayWin::SelectionChanged(bool mode) {
	if (coordsWindow)
		coordsWindow->UpdateSelection(mode);
	if (bondsWindow)
		bondsWindow->UpdateSelection(mode);
	Dirtify();
}

//The following is the amount of space to leave for the window sizing box used by
//many window maangers that takes some space away from the client area.
#define kMOLSCROLLOFFSET 16
#define kMOLSCROLLWIDTH	120
MolStatusBar::MolStatusBar(MolDisplayWin * p) : wxStatusBar(p, wxID_ANY), myScroll(NULL),
				myParent(p) {
	SetFieldsCount(2);
	const int widths[2] = {-1, kMOLSCROLLWIDTH + kMOLSCROLLOFFSET};
	SetStatusWidths(2, widths);

	myScroll = new wxScrollBar( this, MMP_FRAMESCROLLBAR, wxDefaultPosition, wxSize(kMOLSCROLLWIDTH, -1), wxSB_HORIZONTAL );
	//wxStatusBar sets its window style to small, I don't want a small
	//scroll bar so set its style to normal
	myScroll->SetWindowVariant(wxWINDOW_VARIANT_NORMAL);
	myScroll->SetScrollbar(0, 1, 1, 1);
}

void MolStatusBar::OnSize(wxSizeEvent & event) {
	if (!myScroll) return;
		//The size of the scroll bar is left fixed so we just move it around
	wxRect rect;
	GetFieldRect(1, rect);
	myScroll->Move(rect.x, rect.y);

	event.Skip();
}

void MolStatusBar::SetScrollBar(int pos, int range) {
	myScroll->SetScrollbar(pos, 1, range, 1);
}

void MolStatusBar::SetScrollBarValue(int pos) {
	myScroll->SetThumbPosition(pos);
}

void MolStatusBar::OnScrollBarChange(wxScrollEvent & event) {
	myParent->StopAnimations();
	myParent->ChangeFrames(event.GetPosition() + 1);
}

void MolDisplayWin::ChangeAtomOrder(long index) {
	//first make a list of the selected atoms to move.
	//we have to be careful since the initial index may move around as atoms are moved.
	long sel1atom=-1;
	for (long i=0; i < MainData->cFrame->NumAtoms; i++) {
		if (MainData->cFrame->Atoms[i].GetSelectState()) {
			sel1atom = i;
			break;
		}
	}
	if (sel1atom < 0) return;	//no selected atoms found
	if (sel1atom >= index) {
		//if we are moving atoms closer to the beginning of the list it is
		//simple as we just need to find each selected atom and move it.
		while (sel1atom >= 0) {
			MainData->ReorderAtomList(sel1atom, index);
			if (sel1atom == index) sel1atom ++;
			index++;
			long i = sel1atom;
			sel1atom = -1;
			for ( ; i<MainData->cFrame->NumAtoms; i++) {
				if (MainData->cFrame->Atoms[i].GetSelectState()) {
					sel1atom = i;
					break;
				}
			}
		}
	} else {
		//ok moving the atom later in the list. The catch is we don't want to process the
		//same atom twice and the initial atom may not stay put.
		MainData->ReorderAtomList(sel1atom, index);
		long skipstart = index;
		long skipcount = 1;
		while (sel1atom > 0) {
			long i = sel1atom;
			sel1atom = -1;
			for ( ; i<MainData->cFrame->NumAtoms; i++) {
				if (i == skipstart) {
					i += skipcount;
					index++;
					while (MainData->cFrame->Atoms[i].GetSelectState()) {
						//selected atoms immediately after the already moved atoms are fine
						//where they are.
						i++;
						index++;
					}
					continue;
				}
				if (MainData->cFrame->Atoms[i].GetSelectState()) {
					sel1atom = i;
					break;
				}
			}
			if (sel1atom > 0)
				MainData->ReorderAtomList(sel1atom, index);
			if (sel1atom > index) index++;
			else {
				skipcount++;
				skipstart--;
			}
		}
	}
	AtomsChanged(true, true);	//update all the windows
}

void MolDisplayWin::OnToggleTool(wxCommandEvent& event) {

	// The tool is toggled automatically by wx, but we don't really want to
	// allow it to be toggled off.  So, we explicitly force any click on the
	// button to be a toggle on.
	toolbar->ToggleTool(event.GetId(), true);
	mAltModifyingToolBar = false;

	switch (event.GetId()) {
		case MMP_TOOL_ARROW:
			glCanvas->SetCursor(wxCursor(wxCURSOR_ARROW));
			break;
		case MMP_TOOL_LASSO:
			glCanvas->SetCursor(wxCursor(*wxCROSS_CURSOR));
			break;
		case MMP_TOOL_HAND:
			glCanvas->SetCursor(wxCursor(wxCURSOR_HAND));
			break;
		default:
			glCanvas->SetCursor(wxCursor(*wxSTANDARD_CURSOR));
			break;
	}

	AdjustMenus();
	UpdateModelDisplay();

}

bool MolDisplayWin::InViewMode(void) {
	return !toolbar || toolbar->GetToolState(MMP_TOOL_ARROW);
}

bool MolDisplayWin::InSelectionMode(void) {
	return toolbar && toolbar->GetToolState(MMP_TOOL_LASSO);
}

bool MolDisplayWin::InEditMode(void) {
	return toolbar && toolbar->GetToolState(MMP_TOOL_HAND);
}

bool MolDisplayWin::InSymmetryEditMode(void) {
	return edit_symmetrically;
}

void MolDisplayWin::LassoStart(const int x, const int y) {
	lasso_start.x = x;
	lasso_start.y = y;
	lasso_end.x = x;
	lasso_end.y = y;
	lasso_has_area = false;
}

void MolDisplayWin::LassoGrown(const int x, const int y) {
	lasso_end.x = x;
	lasso_end.y = y;
	lasso_has_area = true;
}

void MolDisplayWin::LassoEnd(void) {
	lasso_has_area = false;
}

bool MolDisplayWin::LassoHasArea(void) {
	return lasso_has_area;
	// return abs(lasso_end.x - lasso_start.x) > 3 && 
		   // abs(lasso_end.y - lasso_start.y) > 3; 
}

bool MolDisplayWin::LassoContains(const int x, const int y) {
	return ((x >= lasso_start.x && x <= lasso_end.x) ||
	        (x <= lasso_start.x && x >= lasso_end.x)) &&
	       ((y >= lasso_start.y && y <= lasso_end.y) ||
	        (y <= lasso_start.y && y >= lasso_end.y));
}

void UndoData::AddSnapshot(UndoSnapShot * s) {
	while (position < operationCount) {
		//If we are not at the end of the list delete the remaining items
		UndoSnapShot * t = UndoList.back();
		delete t;
		UndoList.pop_back();
		operationCount --;
	}
	UndoList.push_back(s);
	operationCount++;
	position++;
}

void UndoData::Clear(void) {
	while (UndoList.size() > 0) {
		UndoSnapShot * t = UndoList.back();
		delete t;
		UndoList.pop_back();
	}
	operationCount = 0;
	position = 0;
}

void UndoData::UndoOperation(void) {
	if ((position > 0)&&(position<=UndoList.size())) {
		UndoList[position-1]->Restore();
		position --;
	}
}

void UndoData::RedoOperation(void) {
	if ((position >= 0)&&((position+1)<UndoList.size())) {
		position++;
		UndoList[position]->Restore();
	}
}

FrameSnapShot::FrameSnapShot(MoleculeData * target) {
	IntCoords = NULL;
	targetData = target;
	mTarget = target->cFrame;
	NumAtoms = mTarget->GetNumAtoms();
	NumBonds = mTarget->GetNumBonds();
	Atoms = new mpAtom[NumAtoms];
	Bonds = new Bond[NumBonds];
	for (int i=0; i<NumAtoms; i++)
		Atoms[i] = mTarget->Atoms[i];
	for (int i=0; i<NumBonds; i++)
		Bonds[i] = mTarget->Bonds[i];
	FragmentNames = target->FragmentNames;
	if (target->IntCoords) {
		IntCoords = new Internals(target->IntCoords);
	}
}

FrameSnapShot::~FrameSnapShot(void) {
	if (Atoms) delete [] Atoms;
	if (Bonds) delete [] Bonds;
	if (IntCoords) delete IntCoords;
}

void FrameSnapShot::Restore(void) {
	//Ok this makes a gross assumption on the validity of the frame pointer
	if (mTarget) {
		if (mTarget->AtomAllocation < NumAtoms) {
			delete [] mTarget->Atoms;
			mTarget->Atoms = new mpAtom[NumAtoms];
			mTarget->AtomAllocation = NumAtoms;
		}
		if (mTarget->BondAllocation < NumBonds) {
			delete [] mTarget->Bonds;
			mTarget->Bonds = new Bond[NumBonds];
			mTarget->BondAllocation = NumBonds;
		}
		for (int i=0; i<NumAtoms; i++)
			mTarget->Atoms[i] = Atoms[i];
		mTarget->NumAtoms = NumAtoms;
		for (int i=0; i<NumBonds; i++)
			mTarget->Bonds[i] = Bonds[i];
		mTarget->NumBonds = NumBonds;
		
		targetData->FragmentNames = FragmentNames;
		
		if (IntCoords) {	//restore the internal coordinate definitions
			if (targetData->IntCoords) delete targetData->IntCoords;
			targetData->IntCoords = new Internals(IntCoords);
		}
	}
}

void MolDisplayWin::CreateFrameSnapShot(void) {
	if (Prefs->ToolbarShown()) {
		FrameSnapShot * f = new FrameSnapShot(MainData);
		mUndoBuffer.AddSnapshot(f);
		menuEdit->UpdateUI();
	}
}

void MolDisplayWin::Dirtify(bool is_dirty) {

	Dirty = is_dirty;
	menuFile->UpdateUI(); // force accelerators to adjust

}

void MolDisplayWin::ToggleBuilderPalette(void) {
	// Have to make a top-level window for the miniframe to show the right
	// toolbar under OS X.
	wxGetApp().SetTopWindow(this);

	if (!BuilderTool->IsPaletteVisible()) {
		BuilderTool->ShowPalette(true);
		wxGetApp().AdjustAllMenus();
	} else {
		BuilderTool->ShowPalette(false);
	Dirtify();;
	}
} 

BEGIN_EVENT_TABLE(MolDisplayWin, wxFrame)
	EVT_MENU (MMP_NEWFRAME,         MolDisplayWin::menuFileAppendNewFrame)
#ifndef __WXMAC__
	EVT_MENU (wxID_OPEN,            MolDisplayWin::menuFileOpen)
#endif
	EVT_MENU (MMP_ADDFRAMES,        MolDisplayWin::menuFileAddFramesFromFile)
	EVT_MENU (wxID_SAVE,            MolDisplayWin::menuFileSave)
	EVT_UPDATE_UI(wxID_SAVE,		MolDisplayWin::OnSaveUpdate)
	EVT_MENU (wxID_SAVEAS,          MolDisplayWin::menuFileSave_as)
	EVT_MENU (wxID_CLOSE,           MolDisplayWin::menuFileClose)
	EVT_CLOSE(                      MolDisplayWin::FileClose)
	EVT_MENU (MMP_DELETEFRAME,      MolDisplayWin::menuFileDeleteFrame)
	EVT_MENU (MMP_IMPORTMENU,       MolDisplayWin::menuFileImport)
	EVT_MENU (MMP_EXPORT,           MolDisplayWin::menuFileExport)
	EVT_MENU (wxID_PRINT_SETUP,     MolDisplayWin::menuFilePage_setup)
	EVT_MENU (MMP_PRINTOPTIONS,     MolDisplayWin::menuFilePrintOptions)
	EVT_MENU (wxID_PREVIEW,         MolDisplayWin::menuFilePrint_preview)
	EVT_MENU (wxID_PRINT,           MolDisplayWin::menuFilePrint)

	EVT_MENU (wxID_UNDO,            MolDisplayWin::menuEditUndo)
	EVT_MENU (wxID_REDO,            MolDisplayWin::menuEditRedo)
	EVT_UPDATE_UI(wxID_UNDO,		MolDisplayWin::OnUndoUpdate)
	EVT_UPDATE_UI(wxID_REDO,		MolDisplayWin::OnRedoUpdate)
	EVT_MENU (wxID_CUT,             MolDisplayWin::menuEditCut)
	EVT_MENU (wxID_COPY,            MolDisplayWin::menuEditCopy)
	EVT_MENU (MMP_COPYCOORDS,       MolDisplayWin::menuEditCopyCoordinates)
	EVT_MENU (MMP_COPYNWCOORDS,		MolDisplayWin::menuEditCopyNWChemCoordinates)
	EVT_MENU (wxID_PASTE,           MolDisplayWin::menuEditPaste)
	EVT_UPDATE_UI(wxID_PASTE,       MolDisplayWin::OnPasteUpdate)
	EVT_MENU (wxID_CLEAR,           MolDisplayWin::menuEditClear)
	EVT_UPDATE_UI(wxID_CLEAR,       MolDisplayWin::OnDeleteUpdate)
	EVT_MENU (MMP_SELECT_ALL,       MolDisplayWin::menuEditSelectAll)
	EVT_MENU (MMP_SELECT_NONE,		MolDisplayWin::menuEditSelectNone)
	EVT_UPDATE_UI(MMP_SELECT_NONE,	MolDisplayWin::OnSelectNoneUpdate)
	EVT_UPDATE_UI(MMP_SELECT_ALL,	MolDisplayWin::OnSelectAllUpdate)

	EVT_MENU (MMP_SHOW_FULLSCREEN,  MolDisplayWin::menuViewShowFullScreen)
	EVT_UPDATE_UI(MMP_SHOW_FULLSCREEN, MolDisplayWin::OnShowNormalScreen)
	EVT_MENU (MMP_SHOWMODE,         MolDisplayWin::menuViewShowNormalMode)
	EVT_MENU (MMP_ANIMATEMODE,      MolDisplayWin::menuViewAnimateMode)
	EVT_MENU (MMP_OFFSETMODE,       MolDisplayWin::menuViewOffsetAlongMode)
	EVT_MENU (MMP_PREVMODE,         MolDisplayWin::menuViewPrevNormalMode)
	EVT_MENU (MMP_NEXTMODE,         MolDisplayWin::menuViewNextNormalMode)
	EVT_MENU (MMP_SHOWAXIS,         MolDisplayWin::menuViewShowAxis)
	EVT_MENU (MMP_SHOWSYMMETRYOPERATOR, MolDisplayWin::menuViewShowSymmetryOperators)
	EVT_UPDATE_UI(MMP_SHOWSYMMETRYOPERATOR, MolDisplayWin::OnShowSymOpsUpdate )
	EVT_MENU (MMP_NO_ATOMLABEL,		MolDisplayWin::menuViewHideAtomLabels)
	EVT_MENU (MMP_SHOWATOMLABELS,   MolDisplayWin::menuViewShowAtomLabel)
	EVT_MENU (MMP_SHOWATOMNUMBER,   MolDisplayWin::menuViewShowAtomNumber)
	EVT_MENU (MMP_BOTHATOMLABELS,   MolDisplayWin::menuViewShowBothAtomLabels)
	EVT_MENU (MMP_ADDMARKANNOTATION,   MolDisplayWin::menuViewAddMarkAnnotation)
	EVT_MENU (MMP_ADDLENGTHANNOTATION,   MolDisplayWin::menuViewAddAnnotation)
	EVT_MENU (MMP_ADDANGLEANNOTATION,   MolDisplayWin::menuViewAddAnnotation)
	EVT_MENU (MMP_ADDDIHEDRALANNOTATION,   MolDisplayWin::menuViewAddAnnotation)
EVT_UPDATE_UI (MMP_ANNOTATIONSSUBMENU,   MolDisplayWin::OnAnnotationLengthUpdate)
	EVT_UPDATE_UI(MMP_ADDMARKANNOTATION, MolDisplayWin::OnAnnotationMarkUpdate )
	EVT_UPDATE_UI(MMP_ADDLENGTHANNOTATION, MolDisplayWin::OnAnnotationLengthUpdate )
	EVT_UPDATE_UI(MMP_ADDANGLEANNOTATION, MolDisplayWin::OnAnnotationAngleUpdate )
	EVT_UPDATE_UI(MMP_ADDDIHEDRALANNOTATION, MolDisplayWin::OnAnnotationDihedralUpdate )
	EVT_MENU (MMP_DELETEALLANNOTATIONS,   MolDisplayWin::menuViewDeleteAllAnnotations)
	EVT_UPDATE_UI(MMP_DELETEALLANNOTATIONS, MolDisplayWin::OnDeleteAnnotationsUpdate )
	EVT_MENU (MMP_WIREFRAMEMODE,	MolDisplayWin::menuViewWireFrameStyle)
	EVT_MENU (MMP_BALLANDSTICKMODE,	MolDisplayWin::menuViewBallAndStickStyle)
	EVT_MENU (MMP_EFP_WIREFRAME,	MolDisplayWin::menuViewEFP_Wireframe)
	EVT_UPDATE_UI(MMP_EFP_WIREFRAME,	MolDisplayWin::OnEFPWireFrameUpdate )
	EVT_MENU (MMP_SHOWPATTERN,		MolDisplayWin::menuViewShow2DPattern)
	EVT_UPDATE_UI(MMP_SHOWPATTERN,	MolDisplayWin::OnShowPatternUpdate )
	EVT_MENU (MMP_ANIMATEFRAMES,    MolDisplayWin::menuViewAnimateFrames)
	EVT_MENU (MMP_TOGGLEABINITIO,	MolDisplayWin::menuViewToggleAIAtomsVis)
	EVT_MENU (MMP_TOGGLEMMATOMS,	MolDisplayWin::menuViewToggleMMAtomsVis)
	EVT_MENU (MMP_TOGGLEEFPATOMS,	MolDisplayWin::menuViewToggleEFPAtomsVis)
	EVT_MENU (MMP_SHRINK10,         MolDisplayWin::menuViewShrink_10)
	EVT_MENU (MMP_ENLARGE10,        MolDisplayWin::menuViewEnlarge_10)
	EVT_MENU (MMP_CENTER,           MolDisplayWin::menuViewCenter)
	EVT_MENU (MMP_AUTOROTATE,       MolDisplayWin::menuViewToggleAutoRotation)
	EVT_UPDATE_UI (MMP_AUTOROTATE,   MolDisplayWin::OnAutoRotationUpdate)
	EVT_MENU (MMP_ROTATETOXAXIS,    MolDisplayWin::menuViewRotateTo_X_axis)
	EVT_MENU (MMP_ROTATETOYAXIS,    MolDisplayWin::menuViewRotateTo_Y_axis)
	EVT_MENU (MMP_ROTATETOZAXIS,    MolDisplayWin::menuViewRotateTo_Z_axis)
	EVT_MENU (MMP_ROTATE180HOR,     MolDisplayWin::menuViewRotate180_horizontal)
	EVT_MENU (MMP_ROTATE180VER,     MolDisplayWin::menuViewRotate180_vertical)
	EVT_MENU (MMP_ROTATEPRINC,      MolDisplayWin::menuViewRotatePrinciple_orientation)
	EVT_MENU (MMP_ROTATEOTHER,      MolDisplayWin::menuViewRotateOther)
	EVT_MENU (MMP_WINDOWPARAMETERS,	MolDisplayWin::menuViewSetWindowParameters)

	EVT_MENU (MMP_SETBONDLENGTH,      MolDisplayWin::menuMoleculeSetBondLength)
	EVT_MENU (MMP_ENERGYEDIT,         MolDisplayWin::menuMoleculeSetFrameEnergy)
	EVT_UPDATE_UI(MMP_CREATELLMPATH,  MolDisplayWin::OnMoleculeCreateLLMPathUpdate )
	EVT_MENU (MMP_CREATELLMPATH,      MolDisplayWin::menuMoleculeCreateLLMPath)
	EVT_MENU (MMP_MINFRAMEMOVEMENTS,  MolDisplayWin::menuMoleculeMinimizeFrameMovements)
	EVT_UPDATE_UI(MMP_CURPOINTGROUP,	MolDisplayWin::OnShowPointGroupUpdate )
	EVT_MENU (MMP_PGC1,					MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGCS,					MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGCI,					MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGCNH,				MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGCNV,				MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGCN,					MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGS2N,				MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGDND,				MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGDNH,				MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGDN,					MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGTD,					MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGTH,					MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGT,					MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGOH,					MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGO,					MolDisplayWin::menuSetPointGroup)
	EVT_MENU (MMP_PGORDER2,				MolDisplayWin::menuSetPointGroupOrder)
	EVT_MENU (MMP_PGORDER3,				MolDisplayWin::menuSetPointGroupOrder)
	EVT_MENU (MMP_PGORDER4,				MolDisplayWin::menuSetPointGroupOrder)
	EVT_MENU (MMP_PGORDER5,				MolDisplayWin::menuSetPointGroupOrder)
	EVT_MENU (MMP_PGORDER6,				MolDisplayWin::menuSetPointGroupOrder)
	EVT_MENU (MMP_PGORDER7,				MolDisplayWin::menuSetPointGroupOrder)
	EVT_MENU (MMP_PGORDER8,				MolDisplayWin::menuSetPointGroupOrder)
	EVT_MENU (MMP_DETERMINEPG,			MolDisplayWin::menuMoleculeDetermineSym)
	EVT_MENU (MMP_SYMADAPTCOORDS,		MolDisplayWin::menuMoleculeSymCoords)
	EVT_MENU (MMP_CONVERTTOBOHR,		MolDisplayWin::menuMoleculeConvertToBohr)
	EVT_MENU (MMP_CONVERTTOANGSTROMS,	MolDisplayWin::menuMoleculeConvertToAngstroms)
	EVT_MENU (MMP_INVERTNORMALMODE,		MolDisplayWin::menuMoleculeInvertNormalMode)
	EVT_MENU (MMP_SHOW_TOOLBAR,			MolDisplayWin::menuBuilderShowToolbar)
	EVT_UPDATE_UI(MMP_SHOW_TOOLBAR,		MolDisplayWin::OnShowToolbarUpdate)
	EVT_UPDATE_UI(MMP_SHOWBUILDTOOLS,	MolDisplayWin::OnShowBuildToolsUpdate)
	EVT_MENU (MMP_SHOWBUILDTOOLS,		MolDisplayWin::menuBuilderShowBuildTools)
	EVT_MENU (MMP_ADDHYDROGENS,			MolDisplayWin::menuBuilderAddHydrogens)
	EVT_MENU (MMP_DELETEHYDROGENS,		MolDisplayWin::menuBuilderDeleteHydrogens)
	EVT_MENU (MMP_SHOWBONDSITES,		MolDisplayWin::menuBuilderShowBondSites)
	EVT_MENU (MMP_SYMMETRY_EDIT,		MolDisplayWin::menuBuilderSymmetryEdit)
	/* EVT_MENU (MMP_SYMMETRY_REGEN,		MolDisplayWin::menuBuilderRegenSymmetry) */
	EVT_MENU (MMP_SAVESTRUCTURE,		MolDisplayWin::menuBuilderSaveStructure)
	EVT_UPDATE_UI(MMP_ADDHYDROGENS,		MolDisplayWin::OnAddHydrogensUpdate)
	EVT_UPDATE_UI(MMP_SAVESTRUCTURE,	MolDisplayWin::OnSaveStructureUpdate)
	EVT_UPDATE_UI(MMP_SHOWBONDSITES,	MolDisplayWin::OnShowBondSitesUpdate)
	EVT_UPDATE_UI(MMP_SYMMETRY_EDIT,	MolDisplayWin::OnShowSymmetryEdit)
	EVT_UPDATE_UI_RANGE(MMP_NEWFRAME, MMP_ANNOTATIONSSUBMENU,
	                    MolDisplayWin::UpdateAtomsOptions)
	EVT_UPDATE_UI(wxID_COPY,			MolDisplayWin::UpdateAtomsOptions)
	EVT_MENU(MMP_MOLECULEDISPLAYWINDOW, MolDisplayWin::menuWindowMoleculeDisplay)
	EVT_MENU (MMP_BONDSWINDOW,			MolDisplayWin::menuWindowBonds)
	EVT_MENU (MMP_COORDSWINDOW,			MolDisplayWin::menuWindowCoordinates)
	EVT_MENU (MMP_ENERGYPLOTWINDOW,		MolDisplayWin::menuWindowEnergy_plot)
	EVT_MENU (MMP_FREQUENCIESWINDOW,	MolDisplayWin::menuWindowFrequencies)
	EVT_MENU (MMP_INPUTBUILDERWINDOW,	MolDisplayWin::menuWindowInput_builder)
	EVT_MENU (MMP_SURFACESWINDOW,		MolDisplayWin::menuWindowSurfaces)
	EVT_MENU (MMP_LOCAL_PREFERENCES,		MolDisplayWin::menuPreferences)
	EVT_MENU (MMP_ZMATRIXCALC,			MolDisplayWin::menuWindowZMatrixCalc)

	EVT_TIMER(MMP_ANIMATEFRAMESTIMER, MolDisplayWin::OnFrameAnimationTimer)
	EVT_TIMER(MMP_ANIMATEMODETIMER, MolDisplayWin::OnModeAnimation)
	EVT_TIMER(MMP_STATUS_TIMER, MolDisplayWin::OnStatusTimer)
	EVT_TIMER(MMP_ROTATE_TIMER, MolDisplayWin::OnRotateTimer)
	EVT_SIZE(MolDisplayWin::eventSize)
	EVT_KEY_DOWN (MolDisplayWin::KeyHandler)
	EVT_KEY_UP (MolDisplayWin::KeyUpHandler)
	EVT_MENU_OPEN(MolDisplayWin::OnMenuOpen)
	EVT_KILL_FOCUS(MolDisplayWin::OnKillFocus)
	EVT_ACTIVATE(MolDisplayWin::OnActivate)
	EVT_TOOL_RANGE(MMP_TOOL_ARROW, MMP_TOOL_HAND, MolDisplayWin::OnToggleTool)
END_EVENT_TABLE()

