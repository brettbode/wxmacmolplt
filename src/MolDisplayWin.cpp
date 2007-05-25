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
#include "printoptions.h"
#include "windowparameters.h"
#include "zmatrixcalculator.h"
#include "symmetrypointgroupdlg.h"
#include "setPreference.h"
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include <wx/image.h>
#include <wx/printdlg.h>
#include <wx/mstream.h>
#include <wx/display.h>
#include <wx/stdpaths.h>
#include <wx/toolbar.h>

#ifdef __WXMSW__
#include <stdio.h>
#endif

extern WinPrefs * gPreferences;
extern bool show_periodic_dlg;

using namespace std;

//I think these are only needed in this file.
//They are custom ids used to tie the event table to the menus

#define ID_LOCAL_PREFERENCES 501

enum MMP_EventID {
	MMP_SHRINK10=wxID_HIGHEST+1,
	MMP_ENLARGE10,
	MMP_SHOWMODE,
	MMP_PREVMODE,
	MMP_NEXTMODE,
	MMP_SHOWAXIS,
	MMP_SHOWPERIODICDLG,
	MMP_SHOWSYMMETRYOPERATOR,
	MMP_ATOMLABELSSUBMENU,
	MMP_NO_ATOMLABEL,
	MMP_SHOWATOMLABELS,
	MMP_SHOWATOMNUMBER,
	MMP_SHOWPATTERN,
	MMP_BOTHATOMLABELS,
	MMP_DISPLAYMODESUBMENU,
	MMP_WIREFRAMEMODE,
	MMP_BALLANDSTICKMODE,
	MMP_ANNOTATIONSSUBMENU,
	MMP_ADDMARKANNOTATION,
	MMP_ADDLENGTHANNOTATION,
	MMP_ADDANGLEANNOTATION,
	MMP_ADDDIHEDRALANNOTATION,
	MMP_DELETEALLANNOTATIONS,
	MMP_CENTER,
	MMP_ROTATESUBMENU,
	MMP_ROTATETOXAXIS,
	MMP_ROTATETOYAXIS,
	MMP_ROTATETOZAXIS,
	MMP_ROTATE180HOR,
	MMP_ROTATE180VER,
	MMP_ROTATEPRINC,
	MMP_ROTATEOTHER,
	MMP_SETBONDLENGTH,
	MMP_CREATELLMPATH,
	MMP_MINFRAMEMOVEMENTS,
	MMP_DETERMINEPG,
	MMP_SYMADAPTCOORDS,
	MMP_CONVERTTOBOHR,
	MMP_CONVERTTOANGSTROMS,
	MMP_INVERTNORMALMODE,
	MMP_NEWFRAME,
	MMP_ADDFRAMES,
	MMP_DELETEFRAME,
	MMP_IMPORTMENU,
	MMP_COPYCOORDS,
	MMP_BONDSWINDOW,
	MMP_COORDSWINDOW,
	MMP_ENERGYPLOTWINDOW,
	MMP_FREQUENCIESWINDOW,
	MMP_INPUTBUILDERWINDOW,
	MMP_SURFACESWINDOW,
	MMP_FRAMESCROLLBAR,
	MMP_EXPORT,
	MMP_PRINTOPTIONS,
	MMP_ANIMATEFRAMES,
	MMP_ANIMATEFRAMESTIMER,
	MMP_ANIMATEMODE,
	MMP_ANIMATEMODETIMER,
	MMP_OFFSETMODE,
	MMP_ENERGYEDIT,
	MMP_WINDOWPARAMETERS,
	MMP_ZMATRIXCALC,
	MMP_INTERACTIVE,
	MMP_CURPOINTGROUP,
	MMP_POINTGROUPORDER,
	MMP_PGC1,
	MMP_PGCS,
	MMP_PGCI,
	MMP_PGCNH,
	MMP_PGCNV,
	MMP_PGCN,
	MMP_PGS2N,
	MMP_PGDND,
	MMP_PGDNH,
	MMP_PGDN,
	MMP_PGTD,
	MMP_PGTH,
	MMP_PGT,
	MMP_PGOH,
	MMP_PGO,
	MMP_PGORDER2,
	MMP_PGORDER3,
	MMP_PGORDER4,
	MMP_PGORDER5,
	MMP_PGORDER6,
	MMP_PGORDER7,
	MMP_PGORDER8,
	MMP_TOOL_ARROW,
	MMP_TOOL_LASSO,
	MMP_TOOL_HAND,
	
	Number_MMP_Ids
};


BEGIN_EVENT_TABLE(MolDisplayWin, wxFrame)
	EVT_MENU (MMP_NEWFRAME,         MolDisplayWin::menuFileAppendNewFrame)
#ifndef __WXMAC__
	EVT_MENU (wxID_OPEN,            MolDisplayWin::menuFileOpen)
#endif
	EVT_MENU (MMP_ADDFRAMES,        MolDisplayWin::menuFileAddFramesFromFile)
	EVT_MENU (wxID_SAVE,            MolDisplayWin::menuFileSave)
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
	EVT_MENU (wxID_CUT,             MolDisplayWin::menuEditCut)
	EVT_MENU (wxID_COPY,            MolDisplayWin::menuEditCopy)
	EVT_MENU (MMP_COPYCOORDS,       MolDisplayWin::menuEditCopyCoordinates)
	EVT_MENU (wxID_PASTE,           MolDisplayWin::menuEditPaste)
	EVT_UPDATE_UI(wxID_PASTE,       MolDisplayWin::OnPasteUpdate )
	EVT_MENU (wxID_CLEAR,           MolDisplayWin::menuEditClear)
	EVT_MENU (wxID_SELECTALL,       MolDisplayWin::menuEditSelect_all)
	EVT_MENU (MMP_INTERACTIVE,      MolDisplayWin::menuEditInteractive_mode)

	EVT_MENU (MMP_SHOWMODE,         MolDisplayWin::menuViewShowNormalMode)
	EVT_MENU (MMP_ANIMATEMODE,      MolDisplayWin::menuViewAnimateMode)
	EVT_MENU (MMP_OFFSETMODE,       MolDisplayWin::menuViewOffsetAlongMode)
	EVT_MENU (MMP_PREVMODE,         MolDisplayWin::menuViewPrevNormalMode)
	EVT_MENU (MMP_NEXTMODE,         MolDisplayWin::menuViewNextNormalMode)
	EVT_MENU (MMP_SHOWAXIS,         MolDisplayWin::menuViewShowAxis)
	EVT_MENU (MMP_SHOWPERIODICDLG,  MolDisplayWin::menuViewShowPeriodicDlg)
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
	EVT_UPDATE_UI(MMP_ADDMARKANNOTATION, MolDisplayWin::OnAnnotationMarkUpdate )
	EVT_UPDATE_UI(MMP_ADDLENGTHANNOTATION, MolDisplayWin::OnAnnotationLengthUpdate )
	EVT_UPDATE_UI(MMP_ADDANGLEANNOTATION, MolDisplayWin::OnAnnotationAngleUpdate )
	EVT_UPDATE_UI(MMP_ADDDIHEDRALANNOTATION, MolDisplayWin::OnAnnotationDihedralUpdate )
	EVT_MENU (MMP_DELETEALLANNOTATIONS,   MolDisplayWin::menuViewDeleteAllAnnotations)
	EVT_UPDATE_UI(MMP_DELETEALLANNOTATIONS, MolDisplayWin::OnDeleteAnnotationsUpdate )
	EVT_MENU (MMP_WIREFRAMEMODE,	MolDisplayWin::menuViewWireFrameStyle)
	EVT_MENU (MMP_BALLANDSTICKMODE,	MolDisplayWin::menuViewBallAndStickStyle)
	EVT_MENU (MMP_SHOWPATTERN,		MolDisplayWin::menuViewShow2DPattern)
	EVT_UPDATE_UI(MMP_SHOWPATTERN,	MolDisplayWin::OnShowPatternUpdate )
	EVT_MENU (MMP_ANIMATEFRAMES,    MolDisplayWin::menuViewAnimateFrames)
	EVT_MENU (MMP_SHRINK10,         MolDisplayWin::menuViewShrink_10)
	EVT_MENU (MMP_ENLARGE10,        MolDisplayWin::menuViewEnlarge_10)
	EVT_MENU (MMP_CENTER,           MolDisplayWin::menuViewCenter)
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
	EVT_MENU (MMP_CONVERTTOBOHR,      MolDisplayWin::menuMoleculeConvertToBohr)
	EVT_MENU (MMP_CONVERTTOANGSTROMS, MolDisplayWin::menuMoleculeConvertToAngstroms)
	EVT_MENU (MMP_INVERTNORMALMODE,   MolDisplayWin::menuMoleculeInvertNormalMode)

	EVT_MENU (MMP_BONDSWINDOW,			MolDisplayWin::menuWindowBonds)
	EVT_MENU (MMP_COORDSWINDOW,			MolDisplayWin::menuWindowCoordinates)
	EVT_MENU (MMP_ENERGYPLOTWINDOW,		MolDisplayWin::menuWindowEnergy_plot)
	EVT_MENU (MMP_FREQUENCIESWINDOW,	MolDisplayWin::menuWindowFrequencies)
	EVT_MENU (MMP_INPUTBUILDERWINDOW,	MolDisplayWin::menuWindowInput_builder)
	EVT_MENU (MMP_SURFACESWINDOW,		MolDisplayWin::menuWindowSurfaces)
	EVT_MENU (ID_LOCAL_PREFERENCES,		MolDisplayWin::menuPreferences)
	EVT_MENU (MMP_ZMATRIXCALC,			MolDisplayWin::menuWindowZMatrixCalc)

	EVT_TIMER(MMP_ANIMATEFRAMESTIMER, MolDisplayWin::OnFrameAnimationTimer)
	EVT_TIMER(MMP_ANIMATEMODETIMER, MolDisplayWin::OnModeAnimation)
	EVT_SIZE(MolDisplayWin::eventSize)
	EVT_KEY_DOWN (MolDisplayWin::KeyHandler)
	EVT_MENU_OPEN(MolDisplayWin::OnMenuOpen)
	EVT_KILL_FOCUS(MolDisplayWin::OnKillFocus)
	EVT_ACTIVATE(MolDisplayWin::OnActivate)
	EVT_TOOL_RANGE(MMP_TOOL_ARROW, MMP_TOOL_HAND, MolDisplayWin::OnToggleTool)
END_EVENT_TABLE()

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
class WXDLLEXPORT wxCMLDataObject : public wxDataObjectSimple {
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
			:wxFrame((wxWindow *)NULL, wxID_ANY, title,
					 position, size, style, name) {

	MainData = new MoleculeData;
	Prefs = new WinPrefs;
	*Prefs = *gPreferences;
	Dirty = false;
	OperationInProgress = false;
	ProgressInd = NULL;
	OpenGLData = NULL;
	ModeAnimationData = NULL;
	bondsWindow = NULL;
	coordsWindow = NULL;
	energyPlotWindow = NULL;
	frequenciesWindow = NULL;
	surfacesWindow = NULL;
	inputBuilderWindow = NULL;
	prefsDlg = NULL;
	zMatCalcDlg = NULL;
	
	pageSetupData = NULL;
	printData = NULL;
	myStatus = NULL;

	mHighliteState = false;
	interactiveMode = false;
	stale_click = true;

	toolbar = NULL;
	lasso_has_area = false;

#ifdef __WXMSW__
	//Visual studio is a total pile.
	//The %n format specifier is disabled by default and
	//putting this in the app init function didn't work.
	_set_printf_count_output(1);
#endif

	InitGLData();
	
	myStatus = new MolStatusBar(this);
	SetStatusBar(myStatus);
	int width, height;
	GetClientSize(&width, &height);
	
	glCanvas = new MpGLCanvas(this, 11002, wxPoint(0,0), wxSize(height,width));
	glCanvas->setPrefs(Prefs);
	
	SizeChanged();
	glCanvas->SetFocus();
	createMenuBar();
	SetMenuBar(menuBar);
	
	Show(true);
	AdjustMenus();

	unsigned char texture[8] = {255, 0, 0, 255, 255, 0, 0, 255};
	glGenTextures(1, &(OpenGLData->length_anno_tex_id));
	glBindTexture(GL_TEXTURE_1D, OpenGLData->length_anno_tex_id);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_ALPHA, 8, 0, GL_ALPHA,
		GL_UNSIGNED_BYTE, texture);
}

MolDisplayWin::~MolDisplayWin() {
	//As long as all related windows indicate this window as their
	//parent when they are created, they will be automatically destroyed
	//when this window is destroyed.
	if (printData) delete printData;
	if (pageSetupData) delete pageSetupData;
	glCanvas->setPrefs(NULL);
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

	DeleteGLData();

}

void MolDisplayWin::getCanvasSize(long *width, long *height) {
	int w, h;
	glCanvas->GetClientSize(&w, &h);
	*width = w;
	*height = h;
}

void MolDisplayWin::eventSize(wxSizeEvent &event) {
	SizeChanged();
}

void MolDisplayWin::SizeChanged(void) {
	//resize/reposition the controls and the display canvas
	int width, height;
	GetClientSize(&width, &height);

	//The status bar class effects the client size so no need to subtract it off here
	glCanvas->SetSize(wxSize(width, height));
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
	if (!event.GetActive()) {
		StopAnimations();
	} else {
		stale_click = true;
		glCanvas->eventActivate(event);
	}
	event.Skip();
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
	menuEdit->AppendSeparator();
	menuEdit->Append(wxID_CUT, wxT("Cu&t\tCtrl+X"));
	menuEdit->Append(wxID_COPY, wxT("&Copy\tCtrl+C"), wxT("Copy the display as an image"));
	menuEdit->Append(MMP_COPYCOORDS, wxT("Copy Coordinates"), wxT("Copy the current set of coordinates as plain text"));
	menuEdit->Append(wxID_PASTE, wxT("&Paste\tCtrl+V"));
	menuEdit->Append(wxID_CLEAR, wxT("&Delete\tDel"));
	menuEdit->AppendSeparator();
	menuEdit->Append(wxID_SELECTALL, wxT("&Select all\tCtrl+A"));
	menuEdit->AppendSeparator();
#ifdef ENABLE_INTERACTIVE_MODE
	menuEdit->AppendCheckItem(MMP_INTERACTIVE, wxT("Interactive Mode"));
#endif
	menuEdit->Append(wxID_PREFERENCES, wxT("Global Pr&eferences"), wxT("Edit the default preferences for new windows"));

	menuView->AppendCheckItem(MMP_SHOWMODE, wxT("Show &Normal Mode\tCtrl+D"), wxT("Display the vibrational motion as mass-weighted vectors"));
	menuView->Append(MMP_ANIMATEMODE, wxT("&Animate Mode\tShift+Ctrl+M"), wxT("Animate the vibration, click outside the window to stop"));
	menuView->Append(MMP_OFFSETMODE, wxT("&Offset along mode..."), wxT("Generate a new set of coordinates by moving along the current mode"));
	menuView->Append(MMP_PREVMODE, wxT("&Previous Normal Mode\tCtrl+["));
	menuView->Append(MMP_NEXTMODE, wxT("Ne&xt Normal &Mode\tCtrl+]"));
	menuView->AppendSeparator();
	menuView->AppendCheckItem(MMP_SHOWAXIS, wxT("Show Ax&is"), wxT("Display the cartesian axis"));
	menuView->AppendCheckItem(MMP_SHOWPERIODICDLG, _("Show Periodic Table"), _("Display a periodic table"));
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
	menuViewStyle->AppendCheckItem(MMP_SHOWPATTERN, _("Show Atom Patterns"), _("Overlay a 2D pattern on the atom spheres (Ball and Stick mode only)"));
	
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
	
	menuViewRotate = new wxMenu;
	menuView->Append(MMP_ROTATESUBMENU, wxT("&Rotate ..."), menuViewRotate);
	menuView->Append(MMP_WINDOWPARAMETERS, wxT("Set &Window Parameters..."));

	menuViewRotate->Append(MMP_ROTATETOXAXIS, wxT("to &X-axis"));
	menuViewRotate->Append(MMP_ROTATETOYAXIS, wxT("to &Y-axis"));
	menuViewRotate->Append(MMP_ROTATETOZAXIS, wxT("to &Z-axis"));
	menuViewRotate->Append(MMP_ROTATE180HOR, wxT("180 degrees &Horizontal"));
	menuViewRotate->Append(MMP_ROTATE180VER, wxT("180 degrees &Vertical"));
	menuViewRotate->Append(MMP_ROTATEPRINC, wxT("to &Principle Orientation"));
	menuViewRotate->Append(MMP_ROTATEOTHER, wxT("&Other..."));

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
	menuSetPG->Append(MMP_POINTGROUPORDER, _("&Order of Principle Axis"), menuPGOrder, _("Set the order of the principle rotation axis, if any"));
	menuMolecule->Append(wxID_ANY, _("Set &Point Group"), menuSetPG, _("Manually set the point group"));
	menuMolecule->Append(MMP_DETERMINEPG, _("Determine Point Group"), _("Compute the point group for the current coordinates"));
	menuMolecule->Append(MMP_SYMADAPTCOORDS, _("Set &Coordinates to Principle Orientation"), _("Transform coordinates to the symmetry adapted principle orientation"));
	menuMolecule->AppendSeparator();
	menuMolecule->Append(MMP_CONVERTTOBOHR, wxT("Convert to &Bohr"));
	menuMolecule->Append(MMP_CONVERTTOANGSTROMS, wxT("Convert to &Angstroms"));
	menuMolecule->Append(MMP_INVERTNORMALMODE, wxT("&Invert Normal Mode"), _T("Multiply the normal mode by -1 to invert the direction of the vectors"));
// TODO:  Create menu items for remaining menus

	menuWindow->Append(MMP_BONDSWINDOW, wxT("&Bonds"));
	menuWindow->Append(MMP_COORDSWINDOW, wxT("&Coordinates"), _("View/edit cartesian or internal coordinates"));
	menuWindow->Append(MMP_ENERGYPLOTWINDOW, wxT("&Energy Plot"), _("A plot of the energy for each geometry"));
	menuWindow->Append(MMP_FREQUENCIESWINDOW, wxT("&Frequencies"), _("Plot the vibrational frequencies"));
	menuWindow->Append(MMP_INPUTBUILDERWINDOW, wxT("&Input Builder"), _T("Generate a GAMESS input file"));
	menuWindow->Append(MMP_SURFACESWINDOW, wxT("&Surfaces"), _T("Add/Edit/Remove various surface types"));
	menuWindow->Append(MMP_ZMATRIXCALC, wxT("&Z-Matrix Calculator"));
	menuWindow->Append(ID_LOCAL_PREFERENCES, wxT("Pr&eferences"), _T("Edit the preferences for this window"));

	menuBar->Append(menuFile, wxT("&File"));
	menuBar->Append(menuEdit, wxT("&Edit"));
	menuBar->Append(menuView, wxT("&View"));
	menuBar->Append(menuMolecule, wxT("&Molecule"));
	menuBar->Append(menuWindow, wxT("&Subwindow"));

	menuHelp->Append(wxID_ABOUT, wxT("&About MacMolPlt..."), _T("Learn about MacMolPlt"));
	menuBar->Append(menuHelp, wxT("&Help"));
}

void MolDisplayWin::ClearMenus(void) {
	menuFile->Enable(MMP_NEWFRAME, false);
	menuFile->Enable(wxID_SAVE, false);
	menuFile->Enable(MMP_DELETEFRAME, false);
	menuFile->Enable(MMP_EXPORT, false);
	
	menuEdit->Enable(wxID_UNDO, false);
	menuEdit->Enable(wxID_CUT, false);
	menuEdit->Enable(wxID_COPY, false);
	menuEdit->Enable(MMP_COPYCOORDS, false);
	menuEdit->Enable(wxID_CLEAR, false);
	menuEdit->Enable(wxID_SELECTALL, false);
	menuView->Enable(MMP_SHOWMODE, false);
	menuView->Enable(MMP_ANIMATEMODE, false);
	menuView->Enable(MMP_PREVMODE, false);
	menuView->Enable(MMP_NEXTMODE, false);
	menuView->Enable(MMP_OFFSETMODE, false);
	menuView->Enable(MMP_ANNOTATIONSSUBMENU, false);
	menuView->Enable(MMP_ANIMATEFRAMES, false);
	menuMolecule->Enable(MMP_SETBONDLENGTH, false);
	menuMolecule->Enable(MMP_ENERGYEDIT, false);
	menuMolecule->Enable(MMP_CREATELLMPATH, false);
	menuMolecule->Enable(MMP_MINFRAMEMOVEMENTS, false);
	menuMolecule->Enable(MMP_DETERMINEPG, false);
	menuMolecule->Enable(MMP_SYMADAPTCOORDS, false);
	menuMolecule->Enable(MMP_INVERTNORMALMODE, false);
}
void MolDisplayWin::AdjustMenus(void) {
	ClearMenus();
	menuFile->Enable(wxID_SAVE, Dirty);
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

	if (show_periodic_dlg) {
		menuView->Check(MMP_SHOWPERIODICDLG, true);
	} else {
		menuView->Check(MMP_SHOWPERIODICDLG, false);
	}
	
	if (MainData->cFrame->NumAtoms == 0) {
	} else {
		menuFile->Enable(MMP_NEWFRAME, true);
		menuEdit->Enable(wxID_COPY, true);
		menuEdit->Enable(MMP_COPYCOORDS, true);
		menuMolecule->Enable(MMP_ENERGYEDIT, true);
		menuMolecule->Enable(MMP_SETBONDLENGTH, true);
		menuMolecule->Enable(MMP_DETERMINEPG, true);
		menuMolecule->Enable(MMP_SYMADAPTCOORDS, true);
		menuFile->Enable(MMP_EXPORT, true);
		menuView->Enable(MMP_ANNOTATIONSSUBMENU, true);
	}
	if (MainData->NumFrames > 1 ) {
		menuFile->Enable(MMP_DELETEFRAME, true);
		menuView->Enable(MMP_ANIMATEFRAMES, true);
		if (MainData->CurrentFrame < MainData->NumFrames) {
			Frame * lFrame = MainData->GetCurrentFramePtr();
			Frame * lnFrame = lFrame->GetNextFrame();
			if (lFrame->NumAtoms == lnFrame->NumAtoms) {
				for (long i=0; i<lFrame->NumAtoms; i++) {
					bool good = true;
					if (lFrame->Atoms[i].Type != lnFrame->Atoms[i].Type) good = false;
					if (good) menuMolecule->Enable(MMP_CREATELLMPATH, true);
				}
			}
		}
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
/*!
* wxEVT_UPDATE_UI event handler for wxID_PASTE
 */

void MolDisplayWin::OnPasteUpdate( wxUpdateUIEvent& event ) {
	event.Enable(false);
	if (MainData->cFrame->NumAtoms == 0) {
		if (wxTheClipboard->Open()) {
			if (wxTheClipboard->IsSupported(wxDF_TEXT) ||
				wxTheClipboard->IsSupported(_("CML"))) {
				event.Enable(true);
			}
			wxTheClipboard->Close();
		}
	}
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
void MolDisplayWin::menuFileExport(wxCommandEvent &event) {
	wxFileDialog        *fileDlg = NULL;
	ExportOptionsDialog *exportOptionsDlg = NULL;
	wxString   filepath;
	wxMemoryDC memDC;
	wxImage    exportImage;
	wxBitmap  *bmp;
	wxString   wildcards(wxT("Windows Bitmap (*.bmp)|*.bmp|Portable Network Graphics (*.png)|*.png|JPEG (*.jpeg;*.jpg)|*.jpeg;*.jpg"
							 "|GAMESS $DATA group (*.inp)|*.inp"
							 "|MDL MolFile|*.mol"
							 "|XMOL (*.xyz)|*.xyz"
							 "|Tab delimited Energies (*.txt)|*.txt"));
	bool vibs = false;
	int itemCount = 7;
	if (MainData->cFrame->GetNumberNormalModes() > 0) {
		vibs = true;
		wildcards.Append(wxT("|Frequencies (*.txt)|*.txt"));
		itemCount++;
	}
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
							   wxSAVE | wxOVERWRITE_PROMPT);

	if(fileDlg->ShowModal() == wxID_OK) {
		filepath = fileDlg->GetPath();
		index    = fileDlg->GetFilterIndex();
		if (index <= 2) {
			exportOptionsDlg = new ExportOptionsDialog(this);
			if(exportOptionsDlg->ShowModal() == wxID_OK) {
				switch(index) {
					case 0:
						if(!type) {
							type = wxBITMAP_TYPE_BMP;
							if(!filepath.Lower().Matches(wxT("*.bmp"))) {
								filepath.Append(wxT(".bmp"));
							}
						}
					case 1:
						if(!type) {
							type = wxBITMAP_TYPE_PNG;
							if(!filepath.Lower().Matches(wxT("*.png"))) {
								filepath.Append(wxT(".png"));
							}
						}
					case 2:
						if(!type) {
							type = wxBITMAP_TYPE_JPEG;
							if(!filepath.Lower().Matches(wxT("*.jpg")) &&
							   !filepath.Lower().Matches(wxT("*.jpeg"))) {

								filepath.Append(wxT(".jpg"));
							}
						}
						bmp = new wxBitmap(exportOptionsDlg->getWidth(),
										   exportOptionsDlg->getHeight());
						memDC.SelectObject(*bmp);
						glCanvas->GenerateHiResImageForExport(&memDC);
						exportImage = bmp->ConvertToImage();
						exportImage.SaveFile(filepath, type);
						memDC.SelectObject(wxNullBitmap); // bmp has now been
														  // destroyed.
						break;
				}
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

			if((currFile = fopen(filepath.mb_str(wxConvUTF8), "w")) != NULL) {
				BufferFile *buffer = NULL;
				try {
					buffer = new BufferFile(currFile, true);

					bool AllFrames = false;
					switch (index) {
						case 3:
							if (MainData->GetNumFrames()>1) {
								AllFrames = (wxMessageBox(wxT("Should all geometries (frames) be included\?"),
														  wxT(""), wxYES_NO | wxICON_QUESTION) == wxYES);
							}
							ExportGAMESS(buffer, AllFrames);
							break;
						case 4:
							WriteMDLMolFile(buffer);
							break;
						case 5:
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
						case 6:
							if (MainData->GetNumFrames()>1) {
								AllFrames = (wxMessageBox(wxT("Should all geometries (frames) be included\?"),
														  wxT(""), wxYES_NO | wxICON_QUESTION) == wxYES);
							}
							WriteTabbedEnergies(buffer, AllFrames);
							break;
						case 7:
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
	}

	fileDlg->Destroy();
}

void MolDisplayWin::menuFileOpen(wxCommandEvent &event) {
	//Its possible we could handle this here if the current data is empty?
	//On the Mac Open always opens a new window
#ifndef __WXMAC__
	if (!Dirty && (MainData->NumFrames == 1) && (MainData->MaxAtoms == 0)) {
		//First need to use an open file dialog
		wxString filename = wxFileSelector(wxT("Choose a file to open"));
		//If the user chooses a file, create a window and have it process it.
		if (!filename.IsEmpty()) {
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
	//First prompt for the file.
	wxString filename = wxFileSelector(_("Choose a file containing points to be appended to the currently open file."),
									   _T(""), _T(""), _T(""), _T("*"), wxOPEN, this);
	
	//If the user chooses a file, popup the extra options dialog before processing it
	if (filename.length() > 0) {
		AppendFramesOptions * optDlg = new AppendFramesOptions(this);
		optDlg->SetSkip(Prefs->GetDRCSkip());
		optDlg->SetupItems();
		if (optDlg->ShowModal() == wxID_OK) {
			Prefs->SetDRCSkip(optDlg->GetSkip());
			OpenFile(filename, optDlg->GetOffset(), optDlg->GetFlip(), true);
		}
		optDlg->Destroy();
	}
}

void MolDisplayWin::menuFileSave(wxCommandEvent &event) {
	if(!currFilePath.IsEmpty()) {
		FILE *currFile = NULL;
		BufferFile *buffer = NULL;

		if((currFile = fopen(currFilePath.mb_str(wxConvUTF8), "w")) == NULL) {
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
			Dirty = false;
			delete buffer;
		}
#ifdef __WXMAC__
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
							  wxSAVE | wxOVERWRITE_PROMPT, this);

	if(!filePath.IsEmpty()) {
		if((currFile = fopen(filePath.mb_str(wxConvUTF8), "w")) == NULL) {
			MessageAlert("Unable to access the file.");
			return;
		}
		try {
			UpdateWindowData();
			buffer = new BufferFile(currFile, true);
			MainData->WriteCMLFile(buffer, Prefs, &winData, true, true);
			Dirty = false;
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
#ifdef __WXMAC__
		if (Prefs->CreateCustomIcon()) CreateCustomFileIcon(filePath);
#endif
		currFilePath = filePath;
		SetTitle(filePath);
		fclose(currFile);
	}
}

#ifdef __WXMAC__
OSErr	SendFinderAppleEvent( AliasHandle aliasH, AEEventID appleEventID );
OSErr	SaveCustomIcon( const wxString & filename, IconFamilyHandle icnsH );
#include <wx/mac/carbon/private.h>
//	This routine will set the custom icon in the file with
//  an image based on the current molecule display.
//	It does this by creating a PicHandle, then creating an icon from the Picture,
//	and finally saving the custom icon to disk.
//  I am utilizing a private interface to internal data for the wxBitmap class so
//  be warned that this could break in the future!
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
		MainData = new MoleculeData;
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
	Dirty = true;
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
		// TODO:  throw up an error dialog
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
		int h, w;
		wxSize dcp = dc->GetPPI();
		GetPPIPrinter(&w, &h);
		int sh, sw;
		GetPPIScreen(&sw, &sh);
   //     float scale = (float)w/ (float)sw;
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
	glCanvas->GenerateHiResImage(dc, scaleFactor, ProgressInd, Prefs->GetCenterOnPage(),
								 Prefs->GetFramePage());
	Prefs->CylindersForLines(false);
	FinishOperation();
}
/* Edit menu */

void MolDisplayWin::menuEditUndo(wxCommandEvent &event) {
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
//        wxTheClipboard->SetData(new wxBitmapDataObject(glCanvas->getImage(0,0)));
		wxTheClipboard->SetData(comp);
		wxTheClipboard->Close();
	}
}

void MolDisplayWin::menuEditCopyCoordinates(wxCommandEvent &event) {
	CopyCoordinates(0);
}
void MolDisplayWin::CopyCoordinates(short ctype) const {
	//Now copy the coords
	Frame *     lFrame = MainData->cFrame;
	wxString textBuffer;
	
	try {
		if (ctype == 0) {
			wxString    Label;
			for (long iatm=0; iatm<lFrame->NumAtoms; iatm++) {
				Prefs->GetAtomLabel(lFrame->Atoms[iatm].GetType()-1, Label);
				textBuffer.Append(Label);
				Label.Printf(wxT("   %5.1f  %13.8f  %13.8f  %13.8f\r"),
								 (float) (lFrame->Atoms[iatm].GetType()), 
								 lFrame->Atoms[iatm].Position.x, lFrame->Atoms[iatm].Position.y,
								 lFrame->Atoms[iatm].Position.z);
				textBuffer.Append(Label);
			}
		} else if (ctype == 1) {
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
						if (MainData->NumFrames > 1) {
							MoleculeData *	tdatap = new MoleculeData();
							if (!tdatap) return;
							BufferFile *Buffer = new BufferFile(CML, strlen(CML));
							if (tdatap->OpenCMLFile(Buffer, Prefs, NULL, ProgressInd, false)) {
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
									if (MainData->zBuffer) delete [] MainData->zBuffer;
									MainData->RotCoords = tdatap->RotCoords;
									MainData->zBuffer = tdatap->zBuffer;
									tdatap->RotCoords = NULL;
									tdatap->zBuffer = NULL;
								}
								if (tdatap->Basis && !MainData->Basis) {
									MainData->Basis = tdatap->Basis;
									tdatap->Basis = NULL;
								}
								tdatap->cFrame = tdatap->Frames = NULL; //this frame is now in use in MainData
							}
							if (Buffer) delete Buffer;
							delete tdatap;
						} else {
							BufferFile *Buffer = new BufferFile(CML, strlen(CML));
							if (MainData->OpenCMLFile(Buffer, Prefs, NULL, ProgressInd, true)) {
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
	if (MainData->cFrame->NumAtoms != 0) return;    //Do not allow pasting if there are already atoms in this frame
	if (wxTheClipboard->Open()) {
		if (wxTheClipboard->IsSupported(wxDF_TEXT)) {
			long        iline, test, Type;
			CPoint3D    Position, offset;
			
			wxTextDataObject data;
			wxTheClipboard->GetData(data);
			wxString text = data.GetText();
			char * tbuf = new char[text.Length()+1];
			strncpy(tbuf, text.ToAscii(), text.Length()+1);
			BufferFile * TextBuffer=NULL;
			try {
				TextBuffer = new BufferFile(tbuf, text.Length());
				if (MainData->NumFrames == 1) { //If this is the only frame, make sure it is init'ed
					InitRotationMatrix(MainData->TotalRotation);
				}
				long NumLines = TextBuffer->GetNumLines(-1);
				// There may be up to NumLines atoms so dimension memory accordingly
				if (!MainData->SetupFrameMemory(NumLines, 0)) {
					delete TextBuffer;
					delete [] tbuf;
					wxTheClipboard->Close();
					return;
				}
				
				/*Now interpert each of the lines*/
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
						
						MainData->cFrame->AddAtom(Type, Position);
						MainData->MaxSize = MAX(MainData->MaxSize, fabs(Position.x));
						MainData->MaxSize = MAX(MainData->MaxSize, fabs(Position.y));
						MainData->MaxSize = MAX(MainData->MaxSize, fabs(Position.z));
					}
				}
				//Done with the text handle so unlock it
				delete TextBuffer;
			}
			catch (...) {
				//Hmm there was a failure in the paste - what should we do to cleanup?
				if (TextBuffer) delete TextBuffer;
			}
			delete [] tbuf;
		   
			if (iline == 0) {   /*No atoms were found so clear the memory I just allocated*/
		//      MainData->ResetFrameMemory();
				wxTheClipboard->Close();
				return;
			}

	//If there were special atoms found turn on drawing by default
			if (MainData->cFrame->SpecialAtoms) MainData->SetSpecialAtomDrawMode(true);

			if (Prefs->GetAutoBond())
			MainData->cFrame->SetBonds(Prefs, false);
			//Now reset the display to take into account the new atoms
			MainData->CenterModelWindow();
			MainData->WindowSize = 2.0*MainData->MaxSize;
			MainData->ResetRotation();
			FrameChanged();
		}
		wxTheClipboard->Close();
	}
}

void MolDisplayWin::menuEditClear(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditSelect_all(wxCommandEvent &event) {
}

void MolDisplayWin::menuEditInteractive_mode(wxCommandEvent &event)
{
	interactiveMode = 1 - interactiveMode;

	if (interactiveMode) {
		toolbar = CreateToolBar(wxTB_HORIZONTAL | wxTB_FLAT | wxTB_TEXT);

#include "xpms/view.xpm"
#include "xpms/rect_lasso.xpm"
#include "xpms/hand.xpm"

		toolbar->SetToolBitmapSize(wxSize(32, 32));

		// toolbar->AddRadioTool(MMP_TOOL_ARROW, wxT("View"), wxBITMAP(arrow)); 
		// toolbar->AddRadioTool(MMP_TOOL_LASSO, wxT("Select"), wxBITMAP(rect_lasso)); 
		// toolbar->AddRadioTool(MMP_TOOL_HAND, wxT("Edit"), wxBITMAP(hand)); 

		wxBitmap enabled_bmp;
		wxBitmap enabled_bmp2;

		enabled_bmp = wxBitmap(view_xpm);
		toolbar->AddRadioTool(MMP_TOOL_ARROW, wxT("View"), enabled_bmp,
			wxNullBitmap);
		// toolbar->AddTool(MMP_TOOL_ARROW, wxT("View"), enabled_bmp, 
			// wxNullBitmap, wxITEM_RADIO, wxT("Dummy"), 
			// wxT("Don't do anything")); 

		enabled_bmp = wxBitmap(rect_lasso_xpm);
		toolbar->AddRadioTool(MMP_TOOL_LASSO, wxT("Select"), enabled_bmp,
			wxNullBitmap);
		// toolbar->AddTool(MMP_TOOL_LASSO, wxT("Select"), enabled_bmp, 
			// wxNullBitmap, wxITEM_RADIO, wxT("Select"), 
			// wxT("Select atoms by bounding box")); 

		enabled_bmp = wxBitmap(hand_xpm);
		toolbar->AddRadioTool(MMP_TOOL_HAND, wxT("Edit"), enabled_bmp,
			wxNullBitmap);
		// toolbar->AddTool(MMP_TOOL_HAND, wxT("Edit"), enabled_bmp, 
			// wxNullBitmap, wxITEM_RADIO, wxT("Translate atoms"), 
			// wxT("Don't do anything")); 

		toolbar->AddSeparator();
		toolbar->Realize();

	} else {
		delete toolbar;
		toolbar = NULL;

		SetToolBar(NULL);
	}

	if (glCanvas)
		glCanvas->toggleInteractiveMode();

	wxSizeEvent size_event;
	glCanvas->AddPendingEvent(size_event);

}

void MolDisplayWin::menuViewShowPeriodicDlg(wxCommandEvent &event) {
	if (glCanvas)
		glCanvas->togglePeriodicDialog();
}

void MolDisplayWin::menuViewShowNormalMode(wxCommandEvent &event) {
	MainData->SetDrawMode(1-MainData->GetDrawMode());
	ResetModel(false);
	Dirty = true;
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
	Dirty = true;
}
void MolDisplayWin::OnModeAnimation(wxTimerEvent & event) {
	if (ModeAnimationData) {
		if ((ModeAnimationData->iPoint==ModeAnimationData->NumPoints)||
			(ModeAnimationData->iPoint==-ModeAnimationData->NumPoints)) {
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
	Dirty = true;
}
void MolDisplayWin::menuViewShowSymmetryOperators(wxCommandEvent &event) {
	Prefs->ShowSymmetryOperators(1-Prefs->ShowSymmetryOperators());
	UpdateModelDisplay();
	Dirty = true;
}

void MolDisplayWin::menuViewHideAtomLabels(wxCommandEvent &event)
{
	Prefs->ShowAtomicSymbolLabels(false);
	Prefs->ShowAtomNumberLabels(false);
	UpdateModelDisplay();
	Dirty = true;
}

void MolDisplayWin::menuViewShowAtomLabel(wxCommandEvent &event)
{
	Prefs->ShowAtomicSymbolLabels(true);
	Prefs->ShowAtomNumberLabels(false);
	UpdateModelDisplay();
	Dirty = true;
}

void MolDisplayWin::menuViewShowAtomNumber(wxCommandEvent &event)
{
	Prefs->ShowAtomicSymbolLabels(false);
	Prefs->ShowAtomNumberLabels(true);
	UpdateModelDisplay();
	Dirty = true;
}
void MolDisplayWin::menuViewShowBothAtomLabels(wxCommandEvent &event)
{
	Prefs->ShowAtomicSymbolLabels(true);
	Prefs->ShowAtomNumberLabels(true);
	UpdateModelDisplay();
	Dirty = true;
}
void MolDisplayWin::menuViewAddMarkAnnotation(wxCommandEvent &event) {
	for (int i=0; i<MainData->cFrame->GetNumAtoms(); i++) {
		if (MainData->cFrame->GetAtomSelection(i)) {
			AnnotationMarker * t = new AnnotationMarker(i);
			MainData->Annotations.push_back(t);
		}
	}
}
void MolDisplayWin::menuViewAddAnnotation(wxCommandEvent &event) {
	wxCommandEvent foo;
	foo.SetId(event.GetId()-MMP_ADDMARKANNOTATION+GL_Popup_Mark_Atom);
	glCanvas->AddAnnotation(foo);
}
void MolDisplayWin::menuViewDeleteAllAnnotations(wxCommandEvent &event) {
	MainData->DeleteAllAnnotations();
}

void MolDisplayWin::menuViewShow2DPattern(wxCommandEvent &event) {
	Prefs->Show2DPattern(1-Prefs->Show2DPattern());
	UpdateModelDisplay();
	Dirty = true;
}

void MolDisplayWin::menuViewWireFrameStyle(wxCommandEvent &event)
{
	Prefs->DrawBallnStick(false);
	Prefs->DrawWireFrame(true);
	UpdateModelDisplay();
	Dirty = true;
}
void MolDisplayWin::menuViewBallAndStickStyle(wxCommandEvent &event)
{
	Prefs->DrawBallnStick(true);
	Prefs->DrawWireFrame(false);
	UpdateModelDisplay();
	Dirty = true;
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
		m_timer.Start(AnimateTime);
		timerRunning = true;
	}
}
void MolDisplayWin::menuViewShrink_10(wxCommandEvent &event) {
	MainData->WindowSize *= 1.1;
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewEnlarge_10(wxCommandEvent &event) {
	MainData->WindowSize *= 0.9;
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewSetWindowParameters(wxCommandEvent &event) {
	windowparameters * temp = new windowparameters(this);
	if (temp->ShowModal() != wxID_CANCEL) Dirty = true;
	temp->Destroy();
}
void MolDisplayWin::menuViewRotateTo_X_axis(wxCommandEvent &event) {
	MainData->TotalRotation[0][2] = MainData->TotalRotation[1][1] = 1.0;
	MainData->TotalRotation[2][0] = -1.0;
	MainData->TotalRotation[0][0] = MainData->TotalRotation[2][2] =
		MainData->TotalRotation[1][0] = MainData->TotalRotation[0][1] =
		MainData->TotalRotation[1][2] = MainData->TotalRotation[2][1] = 0.0;
	MainData->ResetRotation();
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewRotateTo_Y_axis(wxCommandEvent &event) {
	MainData->TotalRotation[0][0] = MainData->TotalRotation[1][2] = 1.0;
	MainData->TotalRotation[2][1] = -1.0;
	MainData->TotalRotation[1][1] = MainData->TotalRotation[2][2] =
		MainData->TotalRotation[0][1] = MainData->TotalRotation[0][2] =
		MainData->TotalRotation[1][0] = MainData->TotalRotation[2][0] = 0.0;
	MainData->ResetRotation();
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewRotateTo_Z_axis(wxCommandEvent &event) {
	InitRotationMatrix(MainData->TotalRotation);
	MainData->ResetRotation();
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewRotate180_horizontal(wxCommandEvent &event) {
	MainData->FlipRotation(0);
	MainData->ResetRotation();
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewRotate180_vertical(wxCommandEvent &event) {
	MainData->FlipRotation(1);
	MainData->ResetRotation();
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewRotatePrinciple_orientation(wxCommandEvent &event) {
	MainData->RotateToPrincipleOrientation(Prefs);
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuViewRotateOther(wxCommandEvent &event) {
	SetScreenPlane * temp = new SetScreenPlane(this);
	temp->Show();
}

void MolDisplayWin::menuMoleculeSetBondLength(wxCommandEvent &event) {
	//The set bond length dialog does the work
	SetBondLength * dlg = new SetBondLength(this);
	dlg->ShowModal();
	dlg->Destroy();
	Dirty = true;
}

void MolDisplayWin::menuPreferences(wxCommandEvent &event) 
{
		bool isGlobal;
		int id = event.GetId();

	if ( id == wxID_PREFERENCES )
		isGlobal = true;
	else if ( id == ID_LOCAL_PREFERENCES )
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
	Dirty = true;
}

void MolDisplayWin::menuMoleculeCreateLLMPath(wxCommandEvent &event) {
	//The create LLM dialog does the work
	LLMDialog * llm = new LLMDialog(this);
	llm->ShowModal();
	llm->Destroy();
	Dirty = true;
}
void MolDisplayWin::menuMoleculeMinimizeFrameMovements(wxCommandEvent &event) {
	BeginOperation();
	MainData->LinearLeastSquaresFit(ProgressInd);
	FinishOperation();
	Dirty = true;
}
void MolDisplayWin::menuSetPointGroup(wxCommandEvent &event) {
	GAMESSPointGroup pg = (GAMESSPointGroup)(event.GetId() - MMP_PGC1 + GAMESS_C1);
	if (! MainData->InputOptions) MainData->InputOptions = new InputData;
	MainData->InputOptions->Data->SetPointGroup(pg);
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuSetPointGroupOrder(wxCommandEvent &event) {
	int order = event.GetId() - MMP_PGORDER2 + 2;
	if (! MainData->InputOptions) MainData->InputOptions = new InputData;
	MainData->InputOptions->Data->SetPointGroupOrder(order);
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuMoleculeDetermineSym(wxCommandEvent &event) {
	bool pgFlags[kNumSymmetryPointGroups+2];
	MainData->DeterminePointGroup(pgFlags, Prefs);
	SymmetryPointGroupDlg * dlg = new SymmetryPointGroupDlg(this);
	dlg->setup(pgFlags);
	if (dlg->ShowModal() != wxID_CANCEL) {
		//retrieve the selection and set the point group
		GAMESSPointGroup temp;
		int order;
		if (dlg->GetSelectedPointGroup(pgFlags, temp, order)) {
			if (! MainData->InputOptions) MainData->InputOptions = new InputData;
			MainData->InputOptions->Data->SetPointGroup(temp);
			MainData->InputOptions->Data->SetPointGroupOrder(order);
			MainData->RotateToPrincipleOrientation(Prefs);
			MainData->StickCoordinates();
			if (coordsWindow) coordsWindow->FrameChanged();
			ResetView();
			Dirty = true;
		}
	}
	dlg->Destroy();
}
void MolDisplayWin::menuMoleculeSymCoords(wxCommandEvent &event) {
	MainData->RotateToPrincipleOrientation(Prefs);
	MainData->StickCoordinates();
	if (coordsWindow) coordsWindow->FrameChanged();
	ResetView();
	Dirty = true;
}
void MolDisplayWin::menuMoleculeConvertToBohr(wxCommandEvent &event) {
	MainData->UnitConversion(0);
	ResetAllWindows();
	Dirty = true;
}
void MolDisplayWin::menuMoleculeConvertToAngstroms(wxCommandEvent &event) {
	MainData->UnitConversion(1);
	ResetAllWindows();
	Dirty = true;
}
void MolDisplayWin::menuMoleculeInvertNormalMode(wxCommandEvent &event) {
	MainData->InvertMode();
	ResetModel(false);
	Dirty = true;
}
void MolDisplayWin::KeyHandler(wxKeyEvent & event) {
	StopAnimations();
	int key = event.GetKeyCode();
	if (!event.HasModifiers()) {
		switch (key) {
			case '[':
				if (MainData->cFrame->Vibs) {
					if (MainData->cFrame->Vibs->CurrentMode>0) {
						MainData->cFrame->Vibs->CurrentMode--;
						//activate normal mode display if not active
						if (!MainData->GetDrawMode()) {
							MainData->SetDrawMode(true);
							menuView->Check(MMP_SHOWMODE, true);
						}
						ResetModel(false);
						Dirty = true;
					}
				}
				break;
			case ']':
				if (MainData->cFrame->Vibs) {
					if (MainData->cFrame->Vibs->CurrentMode<(MainData->cFrame->Vibs->NumModes-1)) {
						MainData->cFrame->Vibs->CurrentMode++;
						if (!MainData->GetDrawMode()) {
							MainData->SetDrawMode(true);
							menuView->Check(MMP_SHOWMODE, true);
						}
						ResetModel(false);
						Dirty = true;
					}
				}
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
		}
	} else if (event.AltDown()) {
		switch (key) {
			case 140:   //option - a
				MainData->cFrame->toggleAbInitioVisibility();
				ResetModel(false);
				break;
			case 167:   //option - s
				MainData->cFrame->toggleMMAtomVisibility();
				ResetModel(false);
				break;
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
			//Since this window is not resizeable leave the width and height alone
		wxRect temp = winData.GetInputBWinRect();
		temp.width = temp.height = -1;
		inputBuilderWindow->SetSize(temp);
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

void MolDisplayWin::AtomsChanged(void) {
	if (bondsWindow) bondsWindow->ResetList();
	if (surfacesWindow) surfacesWindow->Reset();
	FrameChanged();
}
void MolDisplayWin::BondsChanged(void) {
	if (bondsWindow) bondsWindow->ResetList();
	FrameChanged();
}

//scan the new frame changing any mode bits for consistancy
void MolDisplayWin::FrameChanged(void) {
	//Check for and update any surfaces depending on the screen plane
	Surface * temp = MainData->cFrame->SurfaceList;
	while (temp) {
		temp->RotateEvent(MainData);
		temp = temp->GetNextSurface();
	}
	UpdateModelDisplay();
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
	Dirty = true;
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
		char    eText[kMaxLineLength] = "";
		long    length=0;
		if (lEOpts->GetDisplayUnits() == kKCalPerMole) UnitFactor = kHartreeTokCalPMol;
		if (lEOpts->PlotMPEnergy())
			Energy = (MainData->cFrame->MP2Energy-lEOpts->GetY1Zero())*UnitFactor;
		else if (lEOpts->PlotEnergy())
			Energy = (MainData->cFrame->GetEnergy()-lEOpts->GetY1Zero())*UnitFactor;
		else if (lEOpts->PlotKEnergy())
			Energy = (MainData->cFrame->KE-lEOpts->GetY2Zero())*UnitFactor;
		else if (lEOpts->PlotPEnergy())
			Energy = (MainData->cFrame->Energy - MainData->cFrame->KE-lEOpts->GetY1Zero())*UnitFactor;
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
	ft.Printf(wxT(" Frame %d of %d"), MainData->GetCurrentFrame(), MainData->GetNumFrames());
	output += ft;

	myStatus->SetStatusText(output);
}
void MolDisplayWin::UpdateModelDisplay(void) {
	UpdateFrameText();
	UpdateGLModel();
	glCanvas->draw();
}
void MolDisplayWin::ResetView(void) {
	//Check for and update any surfaces depending on the screen plane
	Surface * lSurface = MainData->cFrame->SurfaceList;
	while (lSurface) {
		lSurface->RotateEvent(MainData);
		lSurface = lSurface->GetNextSurface();
	}
	glCanvas->UpdateGLView();
	glCanvas->draw();
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
	UpdateGLModel();
	// Reset the frame scroll bar
	myStatus->SetScrollBar(MainData->CurrentFrame-1, MainData->NumFrames);
	UpdateFrameText();
	glCanvas->draw();
	Dirty = true;
	AdjustMenus();
}
void MolDisplayWin::ResetAllWindows(void) {
	glCanvas->UpdateGLView();
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
		MainData = new MoleculeData;
		delete Prefs;
		Prefs = new WinPrefs;
		*Prefs = *gPreferences;
		Dirty = false;
		SetTitle(wxT("Untitled"));
		SetName(wxT("Untitled"));
	} else
#endif
	{
		if (this) Close(true);
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
			default:    //Should only get here for unknown file types.
			{
				Buffer->SetFilePos(0);
				char test[4];
				Buffer->Read(test, sizeof(char) * 4);
				if (strcmp(test, "BMBm") == 0 || strcmp(test, "mBMB") == 0) {
					if (append) MessageAlert("Version 6 and later does not support the MacMolPlt binary format. Please convert to CML with version 5.6 and try again.");
					else AbortOpen("Version 6 and later does not support the MacMolPlt binary format. Please convert to CML with version 5.6 and try again.");
				} else {
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
#ifdef __WXMAC__
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
			Raise();
		}
	} else
		test = MainData->OpenCMLFile(Buffer, Prefs, NULL, ProgressInd, readPrefs);
	if (readPrefs && test >= 10) {
		//Update the view since a CMLfile reads in user preferences
		glCanvas->UpdateGLView();
	}
/*  if (test == 0) AbortOpen(0);
	else {
		SetFileType(2);
		Rect tGRect;
		winData.GetMolWinRect(tGRect);
		SetWindowRect(&tGRect);
		CalculateWindowParameters();
		if (winData.is3DModeActive()) {
			winData.is3DModeActive(false);
			Activate3D();
		}
		ResetModel(false);
	}
			*/
	if (localprogress) FinishOperation();
	return test;
}
WindowData::WindowData(void) {
	//These are all used for windows where -1 indicates the default value that
	//causes wx to compute appropriate values
	BondsVis = CoordsVis = EnergyVis = FreqVis = SurfacesVis = InputBVis = PrefVis = ZMatVis = false;
	MolWinRect.x = MolWinRect.y = MolWinRect.width = MolWinRect.height = -1;
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
		if ((testRect.x >= displayRect.x)&&(testRect.y >= displayRect.y)&&
			(testRect.x < (displayRect.x+displayRect.width))&&
			(testRect.y < (displayRect.y+displayRect.height))) {
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

void MolDisplayWin::Rotate(wxMouseEvent &event) {
	static wxPoint      p;
	wxPoint             q, sphereCenter;
	int                 dx, dy;
	Matrix4D            rotationMatrix, tempcopyMatrix;
	wxRect              sphereRect;
//  static wxString     AngleString;
	long                hsize, vsize, width, hoffset, sphereRadius;
	Surface *           lSurface;
	bool                UpdateSurface = false;

	int wheel = event.GetWheelRotation();
	if (wheel != 0) { //zoom
		MainData->WindowSize *= 1.0 + 0.01*wheel/event.GetWheelDelta();
	}
	
	lSurface = MainData->cFrame->SurfaceList;
	while (lSurface) {
		UpdateSurface = UpdateSurface || lSurface->NeedsUpdateDuringRotation();
		lSurface = lSurface->GetNextSurface();
	}
	bool Stereo = Prefs->UseStereo();
	wxRect DisplayRect = glCanvas->GetRect();
	hsize = DisplayRect.GetWidth();
	vsize = DisplayRect.GetHeight();
	if(Stereo) hsize/=2;
	/* Figure out where to place the Virtual Sphere cue.*/
	width = hsize;
	sphereCenter.x = hsize/2; 
	sphereCenter.y = vsize/2;
	if (sphereCenter.x >= sphereCenter.y)
		sphereRadius   = (long)((float) (sphereCenter.x)*0.9);
	else
		sphereRadius   = (long)((float) (sphereCenter.y)*0.9);
	hoffset = sphereCenter.x;
#if wxCHECK_VERSION(2, 8, 0)
	if (Stereo && ! DisplayRect.Contains(p)) sphereCenter.x += hsize;
#else
	if (Stereo && ! DisplayRect.Inside(p)) sphereCenter.x += hsize;
#endif
	hsize = MAX(hsize, vsize);
	sphereRect.SetHeight(sphereRadius);
	sphereRect.SetWidth(sphereRadius);
	sphereRect.SetX(sphereCenter.x - sphereRadius);
	sphereRect.SetY(sphereCenter.y - sphereRadius);
	
	if (event.ButtonDown()) {
		// initial drag setup, just save the initial cursor position
		p = event.GetPosition();
		if (p.x == 0 && p.y == 0) {
			p = ScreenToClient(wxGetMousePosition());
		}
	} else if (stale_click) {
		p = ScreenToClient(wxGetMousePosition());
		stale_click = false;
	} else if (event.Dragging()) {                               
		// main drag
		q = event.GetPosition();
		dx = q.x - p.x;
		dy = q.y - p.y;
		if (dx != 0 || dy != 0) {
			if (event.CmdDown() || event.RightIsDown()) {   //Command key: translate instead of rotate
				CPoint3D offset;
				if (event.ShiftDown()) {    /*The shift key is down so move into/out of the screen*/
					offset.z = dy/(hsize/MainData->WindowSize);
					MainData->TotalRotation[3][2] += offset.z;
				} else {
					offset.x = dx/(hsize/MainData->WindowSize);
					offset.y = dy/(hsize/MainData->WindowSize);
					MainData->TotalRotation[3][0] += offset.x;
					MainData->TotalRotation[3][1] -= offset.y;
				}
			} else {
				if (event.ShiftDown()||event.MiddleIsDown()) {  /*The shift key is down so zoom instead of rotating*/
					if (dy == 0) return;
					if (dy > 99) dy = 99;
					MainData->WindowSize *= 1.0 + 0.01*dy;
					glCanvas->UpdateGLView();
				} else {
					Point pr, cur, sp;
					pr.h = p.x;
					pr.v = p.y;
					cur.h = q.x;
					cur.v = q.y;
					sp.h = sphereCenter.x;
					sp.v = sphereCenter.y;
					VirtualSphereQD3D (pr, cur, sp, sphereRadius, rotationMatrix, MainData->TotalRotation);
					// VirtualSphereQD3D (p, q, sphereCenter, sphereRadius, rotationMatrix, MainData->TotalRotation);

					CPoint3D    InitialTrans, FinalTrans;
					//First back rotate the translation to get the inital translation
					InitialTrans.x = ((MainData->TotalRotation[3][0])*MainData->TotalRotation[0][0] +
						( MainData->TotalRotation[3][1])*MainData->TotalRotation[0][1] +
						( MainData->TotalRotation[3][2])*MainData->TotalRotation[0][2]);
					InitialTrans.y = ((MainData->TotalRotation[3][0])*MainData->TotalRotation[1][0] +
						( MainData->TotalRotation[3][1])*MainData->TotalRotation[1][1] +
						( MainData->TotalRotation[3][2])*MainData->TotalRotation[1][2]);
					InitialTrans.z = ((MainData->TotalRotation[3][0])*MainData->TotalRotation[2][0] +
						( MainData->TotalRotation[3][1])*MainData->TotalRotation[2][1] +
						( MainData->TotalRotation[3][2])*MainData->TotalRotation[2][2]);
					//Now zero out the translation part of the matrix
					MainData->TotalRotation[3][0] = MainData->TotalRotation[3][1] =
					MainData->TotalRotation[3][2] = 0.0;
					/* Concatenate the new rotation with the current rotation */
					MultiplyMatrix (rotationMatrix, MainData->TotalRotation, tempcopyMatrix);
					CopyMatrix (tempcopyMatrix, MainData->TotalRotation);
					//Now rotate the translation to the new orientation
					Rotate3DPt(MainData->TotalRotation, InitialTrans, &FinalTrans);
					MainData->TotalRotation[3][0] = FinalTrans.x;
					MainData->TotalRotation[3][1] = FinalTrans.y;
					MainData->TotalRotation[3][2] = FinalTrans.z;

					//"clean up" the rotation matrix make the rotation part orthogonal and magnitude 1
					OrthogonalizeRotationMatrix (MainData->TotalRotation);
				}
			}   //Update any surfaces which depend on the screen plane
			if (UpdateSurface) {
				lSurface = MainData->cFrame->SurfaceList;
				while (lSurface) {
					lSurface->RotateEvent(MainData);
					lSurface = lSurface->GetNextSurface();
				}
			}
			/* Update the window */
#ifdef UseOpenGL
			if (UpdateSurface) UpdateGLModel();
#endif

			p = q;      /* Remember previous mouse point for next iteration. */
		}
	}

	// We want to draw a circle showing the virtual sphere, but only if
	// the scene is being rotated.
	if (event.LeftIsDown() && (!interactiveMode || ArrowSelected())) {
		RotateMoleculeGL(Prefs->GetShowAngles());
		glCanvas->SwapBuffers();
	} else {
		// drag finished
		//"clean up" the rotation matrix make the rotation part orthogonal and magnitude 1
		OrthogonalizeRotationMatrix (MainData->TotalRotation);
		/* Draw once again to get rid of the rotation sphere */
		glCanvas->draw();
	}
}

void MolDisplayWin::ChangePrefs(WinPrefs * newPrefs) 
{
	SetWindowPreferences(newPrefs);
	ResetAllWindows();
	//if (PrefsDlog) PrefsDlog->PrefsChanged();
}

void MolDisplayWin::SelectionChanged(bool mode)
{
  if (coordsWindow)
	coordsWindow->UpdateSelection(mode);
  if (bondsWindow)
	bondsWindow->UpdateSelection(mode);
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

void MolDisplayWin::OnToggleTool(wxCommandEvent& event) {

	// The tool is toggled automatically by wx, but we don't really want to
	// allow it to be toggled off.  So, we explicitly force any click on the
	// button to be a toggle on.
	toolbar->ToggleTool(event.GetId(), true);

	printf("toolbar->GetToolState(MMP_TOOL_ARROW): %d\n",
		toolbar->GetToolState(MMP_TOOL_ARROW));

	printf("toolbar->GetToolState(MMP_TOOL_LASSO): %d\n",
		toolbar->GetToolState(MMP_TOOL_LASSO));

	printf("toolbar->GetToolState(MMP_TOOL_HAND): %d\n",
		toolbar->GetToolState(MMP_TOOL_HAND));
	
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
}

bool MolDisplayWin::ArrowSelected(void) {
	return toolbar && toolbar->GetToolState(MMP_TOOL_ARROW);
}

bool MolDisplayWin::LassoSelected(void) {
	return toolbar && toolbar->GetToolState(MMP_TOOL_LASSO);
}

bool MolDisplayWin::HandSelected(void) {
	return toolbar && toolbar->GetToolState(MMP_TOOL_HAND);
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

void MolDisplayWin::LassoEnd(const int x, const int y) {
	lasso_has_area = false;
}

bool MolDisplayWin::LassoHasArea(void) {
	return abs(lasso_end.x - lasso_start.x) > 3 &&
		   abs(lasso_end.y - lasso_start.y) > 3;
}

bool MolDisplayWin::LassoContains(const int x, const int y) {
	return ((x >= lasso_start.x && x <= lasso_end.x) ||
			(x <= lasso_start.x && x >= lasso_end.x)) &&
	       ((y >= lasso_start.y && y <= lasso_end.y) ||
			(y <= lasso_start.y && y >= lasso_end.y));
}
