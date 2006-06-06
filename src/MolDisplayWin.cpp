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
#include "surfaceswindow.h"
#include "choosevecgroup.h"
#include "coordinateoffset.h"
#include "frameenergy.h"
#include "printoptions.h"
#include <wx/clipbrd.h>
#include <wx/dataobj.h>
#include <wx/image.h>
#include <wx/printdlg.h>

extern WinPrefs * gPreferences;
extern long  gNumDocuments;
extern MolDisplayWin *	gWindow[kMaxOpenDocuments];

using namespace std;

//I think these are only needed in this file.
//They are custom ids used to tie the event table to the menus

#define ID_LOCAL_PREFERENCES 501
//temporarily put ID definitions here   -Song Li

enum MMP_EventID {
    MMP_SHRINK10=wxID_HIGHEST+1,
    MMP_ENLARGE10,
    MMP_SHOWMODE,
    MMP_PREVMODE,
    MMP_NEXTMODE,
    MMP_SHOWAXIS,
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
	MMP_SURFACESWINDOW,
    MMP_STATUSBAR,
    MMP_FRAMESCROLLBAR,
    MMP_EXPORT,
    MMP_PRINTOPTIONS,
    MMP_ANIMATEFRAMES,
    MMP_ANIMATEFRAMESTIMER,
    MMP_ANIMATEMODE,
    MMP_ANIMATEMODETIMER,
    MMP_OFFSETMODE,
    MMP_ENERGYEDIT,
    
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
	EVT_MENU (wxID_PREFERENCES,  MolDisplayWin::menuPreferences)
	EVT_MENU (ID_LOCAL_PREFERENCES,   MolDisplayWin::menuPreferences)
    EVT_MENU (MMP_SHOWMODE,         MolDisplayWin::menuViewShowNormalMode)
    EVT_MENU (MMP_ANIMATEMODE,      MolDisplayWin::menuViewAnimateMode)
    EVT_MENU (MMP_OFFSETMODE,       MolDisplayWin::menuViewOffsetAlongMode)
    EVT_MENU (MMP_PREVMODE,         MolDisplayWin::menuViewPrevNormalMode)
    EVT_MENU (MMP_NEXTMODE,         MolDisplayWin::menuViewNextNormalMode)
    EVT_MENU (MMP_SHOWAXIS,         MolDisplayWin::menuViewShowAxis)
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

    EVT_MENU (MMP_SETBONDLENGTH,      MolDisplayWin::menuMoleculeSetBondLength)
    EVT_MENU (MMP_ENERGYEDIT,         MolDisplayWin::menuMoleculeSetFrameEnergy)
    EVT_MENU (MMP_CREATELLMPATH,      MolDisplayWin::menuMoleculeCreateLLMPath)
    EVT_MENU (MMP_MINFRAMEMOVEMENTS,  MolDisplayWin::menuMoleculeMinimizeFrameMovements)
    EVT_MENU (MMP_CONVERTTOBOHR,      MolDisplayWin::menuMoleculeConvertToBohr)
    EVT_MENU (MMP_CONVERTTOANGSTROMS, MolDisplayWin::menuMoleculeConvertToAngstroms)
    EVT_MENU (MMP_INVERTNORMALMODE,   MolDisplayWin::menuMoleculeInvertNormalMode)

    EVT_MENU (MMP_BONDSWINDOW,       MolDisplayWin::menuWindowBonds)
    EVT_MENU (MMP_COORDSWINDOW,      MolDisplayWin::menuWindowCoordinates)
    EVT_MENU (MMP_ENERGYPLOTWINDOW,  MolDisplayWin::menuWindowEnergy_plot)
    EVT_MENU (MMP_FREQUENCIESWINDOW, MolDisplayWin::menuWindowFrequencies)
	EVT_MENU (MMP_SURFACESWINDOW,	MolDisplayWin::menuWindowSurfaces)

    EVT_TIMER(MMP_ANIMATEFRAMESTIMER, MolDisplayWin::OnFrameAnimationTimer)
    EVT_TIMER(MMP_ANIMATEMODETIMER, MolDisplayWin::OnModeAnimation)
    EVT_SIZE( MolDisplayWin::eventSize )
    EVT_KEY_DOWN (MolDisplayWin::KeyHandler)
    EVT_COMMAND_SCROLL(MMP_FRAMESCROLLBAR, MolDisplayWin::OnScrollBarChange )
    EVT_MENU_OPEN(MolDisplayWin::OnMenuOpen )
    EVT_KILL_FOCUS(MolDisplayWin::OnKillFocus)
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
    
    pageSetupData = NULL;
    printData = NULL;

    int i;
    for (i = 0; (gWindow[i]!=NULL)&&(i<kMaxOpenDocuments); i++) ;
    if (i>=kMaxOpenDocuments) return;
    gWindow[i] = this;
    gNumDocuments++;
    mGlobalID = i;
/* Increment the open document count */

    InitGLData();
    
    int width, height;
    GetClientSize(&width, &height);
    frameScrollBar = new wxScrollBar( this, MMP_FRAMESCROLLBAR, wxPoint(width-120, height-20), wxSize(100,-1), wxSB_HORIZONTAL );
    int swidth, sheight;
    frameScrollBar->GetClientSize(&swidth, &sheight);

    glCanvas = new MpGLCanvas(this, 11002, wxPoint(0,0), wxSize(height-sheight,width));
    glCanvas->setPrefs(Prefs);
    
    textBar = new wxStaticText(this, 11003, wxString(""), wxDefaultPosition, wxDefaultSize,
                           wxALIGN_LEFT | wxST_NO_AUTORESIZE);

    SizeChanged();
    frameScrollBar->SetScrollbar(1, 1, 1, 1);
    createMenuBar();
    SetMenuBar(menuBar);

    Show(true);
    AdjustMenus();
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
    
    if(MainData != NULL) {
        delete MainData;
    }
    if (Prefs != NULL) {
        delete Prefs;
    }

    gWindow[mGlobalID] = NULL;
    gNumDocuments--;
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

    int swidth, sheight;
    frameScrollBar->GetClientSize(&swidth, &sheight);
    frameScrollBar->Move(width-114, height-sheight);
    glCanvas->SetSize(wxSize(width, (height-sheight)));
    
    textBar->SetSize(0, height-sheight, width-114, sheight, wxSIZE_USE_EXISTING);
}

void MolDisplayWin::OnMenuOpen(wxMenuEvent & event) {
    StopAnimations();
    event.Skip();
}
void MolDisplayWin::OnKillFocus(wxFocusEvent & event) {
    StopAnimations();
    event.Skip();
}
void MolDisplayWin::StopAnimations(void) {
    if (m_timer.IsRunning()) {
        m_timer.Stop();
        timerRunning = false;
    }
    if (ModeAnimationData) { //if data already exists toggle off the animation
        ModeAnimationData->m_timer.Stop();
        MainData->SetDrawMode(ModeAnimationData->SavedDrawMode);
        for (int iatm=0; iatm<(MainData->cFrame->NumAtoms); iatm++) {
            MainData->cFrame->Atoms[iatm].Position = ModeAnimationData->SavedCoordinates[iatm]; 
        }
        delete ModeAnimationData;
        ModeAnimationData = NULL;
    }
    ResetModel(false);
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

    menuFile->Append(wxID_NEW, wxT("&New\tCtrl+N"));
    menuFile->Append(MMP_NEWFRAME, wxT("Append New Frame"));
    menuFile->Append(wxID_OPEN, wxT("&Open ...\tCtrl+O"));
    menuFile->Append(MMP_ADDFRAMES, wxT("Add Frames from File..."));
    menuFile->Append(wxID_SAVE, wxT("&Save\tCtrl+S"));
    menuFile->Append(wxID_SAVEAS, wxT("Save &as ...\tCtrl+Shift+S"));
    menuFile->Append(wxID_CLOSE, wxT("&Close\tCtrl+W"));
    menuFile->AppendSeparator();
    menuFile->Append(MMP_DELETEFRAME, wxT("Delete Frame"));
    menuFile->AppendSeparator();
    menuFile->Append(MMP_IMPORTMENU, wxT("Import..."));
    menuFile->Append(MMP_EXPORT, wxT("Export..."));
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
    menuEdit->Append(wxID_COPY, wxT("&Copy\tCtrl+C"));
    menuEdit->Append(MMP_COPYCOORDS, wxT("Copy Coordinates"));
    menuEdit->Append(wxID_PASTE, wxT("&Paste\tCtrl+V"));
    menuEdit->Append(wxID_CLEAR, wxT("&Delete\tDel"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_SELECTALL, wxT("&Select all\tCtrl+A"));
    menuEdit->AppendSeparator();
    menuEdit->Append(wxID_PREFERENCES, wxT("Global Pr&eferences"));

    menuView->AppendCheckItem(MMP_SHOWMODE, wxT("Show &Normal Mode\tCtrl+D"));
    menuView->Append(MMP_ANIMATEMODE, wxT("Animate Mode\tCtrl+M"));
    menuView->Append(MMP_OFFSETMODE, wxT("Offset along mode..."));
    menuView->Append(MMP_PREVMODE, wxT("&Previous Normal Mode\tCtrl+["));
    menuView->Append(MMP_NEXTMODE, wxT("Ne&xt Normal &Mode\tCtrl+]"));
    menuView->AppendCheckItem(MMP_SHOWAXIS, wxT("Show &Axis"));
    menuView->Append(MMP_ANIMATEFRAMES, wxT("Animate &Frames\tCtrl+F"));
    menuView->Append(MMP_SHRINK10, wxT("&Shrink 10%\tCtrl+-"));
    menuView->Append(MMP_ENLARGE10, wxT("&Enlarge 10%\tCtrl+="));
    menuView->Append(MMP_CENTER, wxT("&Center View"));
    
    menuViewRotate = new wxMenu;
    menuView->Append(MMP_ROTATESUBMENU, wxT("&Rotate ..."), menuViewRotate);

    menuViewRotate->Append(MMP_ROTATETOXAXIS, wxT("to &X-axis"));
    menuViewRotate->Append(MMP_ROTATETOYAXIS, wxT("to &Y-axis"));
    menuViewRotate->Append(MMP_ROTATETOZAXIS, wxT("to &Z-axis"));
    menuViewRotate->Append(MMP_ROTATE180HOR, wxT("180 degrees &Horizontal"));
    menuViewRotate->Append(MMP_ROTATE180VER, wxT("180 degrees &Vertical"));
    menuViewRotate->Append(MMP_ROTATEPRINC, wxT("to &Principle Orientation"));
    menuViewRotate->Append(MMP_ROTATEOTHER, wxT("&Other..."));

    menuMolecule->Append(MMP_SETBONDLENGTH, wxT("Set Bonds..."));
    menuMolecule->Append(MMP_ENERGYEDIT, wxT("Set &Frame Energy..."));
    menuMolecule->Append(MMP_CREATELLMPATH, wxT("Create &LLM Path..."));
    menuMolecule->Append(MMP_MINFRAMEMOVEMENTS, wxT("&Minimize Frame Movements"));
    menuMolecule->AppendSeparator();
    menuMolecule->Append(MMP_CONVERTTOBOHR, wxT("Convert to &Bohr"));
    menuMolecule->Append(MMP_CONVERTTOANGSTROMS, wxT("Convert to &Angstroms"));
    menuMolecule->Append(MMP_INVERTNORMALMODE, wxT("&Invert Normal Mode"));
// TODO:  Create menu items for remaining menus

    menuWindow->Append(MMP_BONDSWINDOW, wxT("&Bonds"));
    menuWindow->Append(MMP_COORDSWINDOW, wxT("&Coordinates"));
    menuWindow->Append(MMP_ENERGYPLOTWINDOW, wxT("&Energy Plot"));
    menuWindow->Append(MMP_FREQUENCIESWINDOW, wxT("&Frequencies"));
    menuWindow->Append(MMP_SURFACESWINDOW, wxT("&Surfaces"));
    menuWindow->Append(ID_LOCAL_PREFERENCES, wxT("Pr&eferences ..."));
    menuHelp->Append(wxID_ABOUT, wxT("&About ..."));

    menuBar->Append(menuFile, wxT("&File"));
    menuBar->Append(menuEdit, wxT("&Edit"));
    menuBar->Append(menuView, wxT("&View"));
    menuBar->Append(menuMolecule, wxT("&Molecule"));
    menuBar->Append(menuWindow, wxT("&Subwindow"));
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
    menuView->Enable(MMP_ANIMATEFRAMES, false);
    menuMolecule->Enable(MMP_SETBONDLENGTH, false);
    menuMolecule->Enable(MMP_ENERGYEDIT, false);
    menuMolecule->Enable(MMP_CREATELLMPATH, false);
    menuMolecule->Enable(MMP_MINFRAMEMOVEMENTS, false);
    menuMolecule->Enable(MMP_INVERTNORMALMODE, false);
}
void MolDisplayWin::AdjustMenus(void) {
    ClearMenus();
    menuFile->Enable(wxID_SAVE, Dirty);
    if (MainData->cFrame->NumAtoms == 0) {
    } else {
        menuFile->Enable(MMP_NEWFRAME, true);
        menuEdit->Enable(wxID_COPY, true);
        menuEdit->Enable(MMP_COPYCOORDS, true);
        menuMolecule->Enable(MMP_ENERGYEDIT, true);
        menuMolecule->Enable(MMP_SETBONDLENGTH, true);
        menuFile->Enable(MMP_EXPORT, true);
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
            if (wxTheClipboard->IsSupported(wxDF_TEXT)) {
                event.Enable(true);
            }
            wxTheClipboard->Close();
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
        FILE * myfile = fopen(filename.mb_str(wxConvUTF8), "r");
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
                        targetVec = 0;
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
    wxString   wildcards(wxT("Windows Bitmap (*.bmp)|*.bmp|Portable Network Graphics (*.png)|*.png|JPEG (*.jpeg;*.jpg)|*.jpeg;*.jpg"));
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
            }
        }
    }

    delete exportOptionsDlg;
    delete fileDlg;
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
    //      MolDisplayWin * temp = new MolDisplayWin(filename);
    //      MolWinList.push_back(temp);
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
    //First prompt for the file. I don't know if there is anyway to add the
    //extra options to the open file dialog.
    wxFileDialog * fdlg = new wxFileDialog(this, 
        wxT("Choose a file containing points to be appended to the currently open file."),
                        wxString(""), wxString(""), wxString("*"), wxOPEN|wxOVERWRITE_PROMPT);
    fdlg->ShowModal();
    wxString filename = fdlg->GetPath();
    
    //If the user chooses a file, create a window and have it process it.
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
            MainData->WriteCMLFile(buffer, Prefs, NULL, true, true);
            Dirty = false;
            delete buffer;
        }
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
                              wxT("CML Files (*.cml)"),
                              wxSAVE | wxOVERWRITE_PROMPT, this);

    if(!filePath.IsEmpty()) {
        if((currFile = fopen(filePath.mb_str(wxConvUTF8), "w")) == NULL) {
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
            MainData->WriteCMLFile(buffer, Prefs, NULL, true, true);
            Dirty = false;
            delete buffer;
        }
        fclose(currFile);
    }
}

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
    Destroy();
    app.destroyMainFrame(this);
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
        wxTheClipboard->SetData(new wxBitmapDataObject(glCanvas->getImage(0,0)));
        wxTheClipboard->Close();
    }
}

void MolDisplayWin::menuEditCopyCoordinates(wxCommandEvent &event) {
    CopyCoordinates(0);
}
#include <sstream>
void MolDisplayWin::CopyCoordinates(short ctype) const {
    //Now copy the coords
    Frame *     lFrame = MainData->cFrame;
    wxString textBuffer;
    
    try {
        if (ctype == 0) {
            char    LineText[100];
            wxString    Label;
            std::ostringstream  sbuf;
            for (long iatm=0; iatm<lFrame->NumAtoms; iatm++) {
                Prefs->GetAtomLabel(lFrame->Atoms[iatm].GetType()-1, Label);
                sbuf << Label;
                textBuffer += Label;
                Label.Printf("   %5.1f  %13.8f  %13.8f  %13.8f\r",
                                 (float) (lFrame->Atoms[iatm].GetType()), 
                                 lFrame->Atoms[iatm].Position.x, lFrame->Atoms[iatm].Position.y,
                                 lFrame->Atoms[iatm].Position.z);
                sbuf << Label;
            }
            textBuffer.Printf("%s", sbuf.str().c_str());
        } else if (ctype == 1) {
            //Make a guess for the Handle size based on the # of atoms and the line format
            long datalength = lFrame->NumAtoms*70*sizeof(char);
            char * lText = new char[datalength];
            //Create a bufferFile object to protect the text buffer from overrun
            BufferFile *Buffer = new BufferFile(lText, datalength);

            Internals * IntCoords = MainData->GetInternalCoordinates();
            if (IntCoords) IntCoords->WriteCoordinatesToFile(Buffer, MainData, Prefs);

            textBuffer.Printf("%s", lText);
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
    PasteText();
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
            BufferFile * TextBuffer = new BufferFile(tbuf, text.Length());
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
void MolDisplayWin::menuViewAnimateFrames(wxCommandEvent &event) {
    long AnimateTime = Prefs->GetAnimateTime();
    if (m_timer.IsRunning()) {
        m_timer.Stop();
        timerRunning = false;
    } else {
        if (MainData->CurrentFrame>=MainData->NumFrames)
            ChangeFrames(1);
        else
            ChangeFrames(MainData->CurrentFrame+1);
        m_timer.SetOwner(this, MMP_ANIMATEFRAMESTIMER);
        m_timer.Start(10*AnimateTime);
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
    MessageAlert("Sure would be nice if this was implemented...");
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

    setPreference * pre_dlg = new setPreference(this, isGlobal);
    pre_dlg->ShowModal();
    pre_dlg->Destroy();
    Dirty = true;
}

void MolDisplayWin::menuMoleculeSetFrameEnergy(wxCommandEvent &event) {
    FrameEnergy * dlg = new FrameEnergy(this);
    dlg->ShowModal();
    dlg->Destroy();
    UpdateFrameText();
#ifndef WIN32
#warning Need to reset the energy plot here if open as the energy data may have changed
#endif
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
void MolDisplayWin::OnScrollBarChange( wxScrollEvent& event ) {
    StopAnimations();
        //this function receives all events from the scroll bar and
        //and thus provides live dragging.
    int newpos = event.GetPosition() + 1;
    if (newpos != MainData->CurrentFrame) {
        ChangeFrames(newpos);
    }
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
        bondsWindow->Show();
    }
}
void MolDisplayWin::CloseBondsWindow(void) {
    if (bondsWindow) {
        bondsWindow->Destroy();
        bondsWindow = NULL;
    }
}
void MolDisplayWin::menuWindowCoordinates(wxCommandEvent &event) {
    if (coordsWindow) { //need to bring it to the front...
        coordsWindow->Raise();
    } else {
        coordsWindow = new CoordinatesWindow(this);
        coordsWindow->Show();
    }
}
void MolDisplayWin::CloseCoordsWindow(void) {
    if (coordsWindow) {
        coordsWindow->Destroy();
        coordsWindow = NULL;
    }
}
void MolDisplayWin::menuWindowEnergy_plot(wxCommandEvent &event) {
    if(energyPlotWindow) { // need to bring it to the front...
        energyPlotWindow->Raise();
    }
    else {
        energyPlotWindow = new EnergyPlotDialog(this);
        energyPlotWindow->Show();
    }
}
void MolDisplayWin::CloseEnergy_plotWindow(void) {
    if(energyPlotWindow) {
        energyPlotWindow->Destroy();
        energyPlotWindow = NULL;
    }
}
void MolDisplayWin::menuWindowFrequencies(wxCommandEvent &event) {
    if(frequenciesWindow) { // need to bring it to the front...
        frequenciesWindow->Raise();
    }
    else {
        frequenciesWindow = new FrequenciesDialog(this);
        frequenciesWindow->Show();
    }
}
void MolDisplayWin::CloseFrequenciesWindow(void) {
    if(frequenciesWindow) {
        frequenciesWindow->Destroy();
        frequenciesWindow = NULL;
    }
}
void MolDisplayWin::menuWindowSurfaces(wxCommandEvent &event) {
    if(surfacesWindow) { // need to bring it to the front...
        surfacesWindow->Raise();
    }
    else {
        surfacesWindow = new SurfacesWindow(this);
        surfacesWindow->Show();
    }
}
void MolDisplayWin::CloseSurfacesWindow(void) {
    if(surfacesWindow) {
        surfacesWindow->Destroy();
        surfacesWindow = NULL;
    }
}

void MolDisplayWin::AtomsChanged(void) {
    if (bondsWindow) bondsWindow->ResetList();
#ifndef WIN32
#warning Update surface window here
#endif
//  if (SurfaceDlog) SurfaceDlog->Reset();
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

#ifndef WIN32
#warning Update windows here when they are added
#endif
        if (coordsWindow) coordsWindow->FrameChanged();
        if (bondsWindow) bondsWindow->ResetList();
        if (energyPlotWindow) energyPlotWindow->FrameChanged();
        if (frequenciesWindow) frequenciesWindow->FrameChanged();
    //  if (SurfaceDlog) SurfaceDlog->Reset();
        frameScrollBar->SetThumbPosition(MainData->CurrentFrame-1);
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
void MolDisplayWin::UpdateFrameText(void) {
    // Add a little information
    wxString output;
    Boolean WriteEnergy=true;
    if (MainData->cFrame->Vibs) {
      if (MainData->GetDrawMode()) {
          wxString temp;
          output.Printf("Freq=%s", MainData->cFrame->Vibs->Frequencies[MainData->cFrame->Vibs->CurrentMode].c_str());
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
            output.Printf("E=%.*f", lEOpts->GetNumDigits(), Energy);
        }
        if (lPOpts->PlotRMSGradient()) {
            wxString temp;
            temp.Printf(" RMS=%.*f", lEOpts->GetNumDigits(),
                    MainData->cFrame->GetRMSGradient()-lEOpts->GetY2Zero());
            output += temp;
        } else if (lPOpts->PlotMaxGradient()) {
            wxString temp;
            temp.Printf(" Max Grad=%.*f", lEOpts->GetNumDigits(),
                    MainData->cFrame->GetMaxGradient()-lEOpts->GetY2Zero());
            output += temp;
        } else if (lPOpts->PlotBondLength()) {
            long a1, a2;
            float bLength;
            a1 = lPOpts->Get1stAtom();
            a2 = lPOpts->Get2ndAtom();
            if (MainData->cFrame->GetBondLength(a1, a2, &bLength)) {
                wxString temp;
                temp.Printf(" Bond %ld-%ld=%.*f", a1+1, a2+1,
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
                temp.Printf(" Angle %ld-%ld-%ld=%.*f", a1+1, a2+1, a3+1,
                        lEOpts->GetNumDigits(), bAngle-lEOpts->GetY2Zero());
                output += temp;
            }
        }
    }
    wxString ft;
    ft.Printf(" Frame %d of %d", MainData->GetCurrentFrame(), MainData->GetNumFrames());
    output += ft;
    
    textBar->SetLabel(output);
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
        MainData->WindowSize = 2.0*(MainData->MaxSize+1.0);
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
    frameScrollBar->SetScrollbar(MainData->CurrentFrame-1, 1, MainData->NumFrames, 1);
    UpdateFrameText();
    glCanvas->draw();
    AdjustMenus();
}
void MolDisplayWin::ResetAllWindows(void) {
    glCanvas->UpdateGLView();
    ResetModel(false);
    //update the frame info.
    
    //force updates for all the child windows
    if (coordsWindow) coordsWindow->FrameChanged();
    if (bondsWindow) bondsWindow->ResetList();
    // TODO:  update energyPlotWindow
    // TODO:  update frequenciesWindow
    
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
    
    FILE * myfile = fopen(fileName.mb_str(wxConvUTF8), "r");
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
            default:    //Should only get here for unknown file types.
                if (!append)
                    AbortOpen("Unable to determine the file type.");
                else
                    MessageAlert("Unable to determine the file type.");
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
    if (test) {//Note test is left 0 if any exception occurs(which causes Window to be deleted)
//      if (gPreferences->ChangeFileType()) {
//          // Looks like this is a good file so set the creator type for the neat icon
//          FInfo   myFInfo;
//          HGetFInfo(myFile->vRefNum, myFile->parID, myFile->name, &myFInfo);
//          if (myFInfo.fdCreator != (OSType) 'BMBm') {
//              myFInfo.fdCreator = (OSType) 'BMBm';
//              HSetFInfo(myFile->vRefNum, myFile->parID, myFile->name, &myFInfo);
//          }
//      }
        //Text files are not used after opening so close it immediately
        fclose(myfile);
//      Window->CloseFile();    //Hmmm should this happen for CML files?
//      if (!Window->IsSavedFile()) Window->SetFileType(5);
        FinishOperation();  //Close the progress dialog, if opened
//      if (!Window->IsSavedFile() && gPreferences->Default3DOn()) Window->Activate3D();
        //Tell the window its data has changed so that it will be redrawn correctly
//      if (!Window->IsSavedFile()) 
        ResetModel(true);

        // Set the path for saving changes
        if(type == CMLFile) {
            currFilePath = fileName;
        } else {
            currFilePath = wxT("");
        }
    }
    return test;
}
long MolDisplayWin::OpenCMLFile(BufferFile * Buffer, bool readPrefs, bool readWindows) {
    bool localprogress=false;
    if (!ProgressInd) {
        localprogress = true;
        BeginOperation();
    }
    ProgressInd->ChangeText("Reading CML fileÉ");
    long test = 0;
    if (readWindows)
        test = MainData->OpenCMLFile(Buffer, Prefs, NULL, ProgressInd, readPrefs);
//      test = MainData->OpenCMLFile(Buffer, Prefs, &winData, readPrefs);
    else
        test = MainData->OpenCMLFile(Buffer, Prefs, NULL, ProgressInd, readPrefs);
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
    if (Stereo && ! DisplayRect.Inside(p)) sphereCenter.x += hsize;
    hsize = MAX(hsize, vsize);
    sphereRect.SetHeight(sphereRadius);
    sphereRect.SetWidth(sphereRadius);
    sphereRect.SetX(sphereCenter.x - sphereRadius);
    sphereRect.SetY(sphereCenter.y - sphereRadius);
    
    if(event.ButtonDown()) {
        // initial drag setup, just save the initial cursor position
        p = event.GetPosition();
    } else if(event.Dragging()) {                               
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
                } else {
                    Point pr, cur, sp;
                    pr.h = p.x;
                    pr.v = p.y;
                    cur.h = q.x;
                    cur.v = q.y;
                    sp.h = sphereCenter.x;
                    sp.v = sphereCenter.y;
                    VirtualSphereQD3D (pr, cur, sp, sphereRadius, rotationMatrix, MainData->TotalRotation);
            //      VirtualSphereQD3D (p, q, sphereCenter, sphereRadius, rotationMatrix, MainData->TotalRotation);

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
    if (event.LeftIsDown()) {
            //This will draw with the angles and circle displayed
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
