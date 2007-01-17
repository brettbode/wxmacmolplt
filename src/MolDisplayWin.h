/*
 * (c) 2005 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * MolDisplayWin.h
 *
 * Defines a class that overloads the wxFrame class.  This class is used for
 * the document window(s).
 ***************************************/

#ifndef MolDisplayWin_H
#define MolDisplayWin_H 1

#include "wx/wx.h"
#include <wx/print.h>

#include "mpGLCanvas.h"
#include "MoleculeData.h"
#include "Prefs.h"
#include "Progress.h"

typedef class OpenGLRec OpenGLRec;
typedef class BondsDlg BondsDlg;
typedef class CoordinatesWindow CoordinatesWindow;
typedef class EnergyPlotDialog EnergyPlotDialog;
typedef class FrequenciesDialog FrequenciesDialog;
typedef class SurfacesWindow SurfacesWindow;
typedef class InputBuilderWindow InputBuilderWindow;
typedef class ModeAnimation ModeAnimation;
typedef class ZMatrixCalculator ZMatrixCalculator;
typedef class setPreference setPreference;
/**
 * Subclasses wxFrame to define the main application window.  This is a
 * document window.  There can be multiple instances of the window in the
 * application, and each main window owns its dialogs.
 */
class MolDisplayWin : public wxFrame {
    private:
        wxMenuBar *menuBar;
        wxMenu    *menuFile;
        wxMenu    *menuEdit;
        wxMenu    *menuView;
        wxMenu    *menuViewLabels;
        wxMenu    *menuViewStyle;
        wxMenu    *menuViewRotate;
        wxMenu    *menuMolecule;
        wxMenu    *menuWindow;
        wxMenu    *menuHelp;

        wxString		currFilePath;
		wxPanel *		infoPanel;
        MoleculeData	*MainData;
        WinPrefs		*Prefs;
        bool			Dirty;          //Flag to indicate a save is needed
		bool			OperationInProgress;
		bool			timerRunning;
        
        Progress		*ProgressInd;    // Progress indicator window for long operations
        OpenGLRec		*OpenGLData;     // Extra OpenGL data
        MpGLCanvas		*glCanvas;
        wxStaticText	*textBar;
        wxScrollBar		*frameScrollBar;
		ModeAnimation	*ModeAnimationData;
		wxTimer			m_timer;

        BondsDlg			*bondsWindow;
        CoordinatesWindow	*coordsWindow;
        EnergyPlotDialog	*energyPlotWindow;
		FrequenciesDialog	*frequenciesWindow;
		SurfacesWindow		*surfacesWindow;
        InputBuilderWindow  *inputBuilderWindow;
		setPreference		*prefsDlg;
		ZMatrixCalculator	*zMatCalcDlg;
        
        wxPageSetupDialogData *pageSetupData;
        wxPrintData           *printData;

        void createMenuBar(void);
        long OpenGAMESSIRCLog(BufferFile * Buffer, long flip, float offset,
                              long NumOccAlpha, long NumOccBeta, long NumFragmentAtoms);

	bool mHighliteState;
	bool interactiveMode;

        DECLARE_EVENT_TABLE()

    public:
        /**
         * Constructor that provides some useful default values.
         * @param title The text that will be displayed in the new window's title bar.
         * @param position The initial position of the new window.
         * @param size The initial size of the new window.
         * @param style The style of the new window.  See wxFrame class docs.
         * @param name The name of the new window.
         */
        MolDisplayWin(const wxString &title,
                    const wxPoint  &position = wxDefaultPosition,
                    const wxSize   &size     = wxSize(400,420),
                    long            style    = wxDEFAULT_FRAME_STYLE,
                    const wxString &name     = wxT("MpMainFrame"));

        /**
         * Destructor, mostly for cleaning up data that we've allocated
         * independently of wxWidgets.
         */
        virtual ~MolDisplayWin();

        /**
         * Gets the dimensions of the glCanvas.
         * @param width A pointer to an integer that will be set to the width
         * @param height A pointer to an integer that will be set to the height
         */
        void getCanvasSize(long *width, long *height);

        //Call to update the menu bar itesm for the current data
        void AdjustMenus(void);
        void ClearMenus(void);
        // Below are the event handler functions for the menu.  The initial
        // list was taken from the original MacMolPlt menus.

        void menuFileOpen(wxCommandEvent &event);
        void menuFileSave(wxCommandEvent &event);
        void menuFileSave_as(wxCommandEvent &event);
		bool CreateCustomFileIcon(const wxString & filePath);
		void menuFileClose(wxCommandEvent &event);
        void FileClose(wxCloseEvent &event);
        void menuFileAppendNewFrame(wxCommandEvent &event);
        void menuFileAddFramesFromFile(wxCommandEvent &event);
        void menuFileDeleteFrame(wxCommandEvent &event);
        void menuFileImport(wxCommandEvent &event);
        void menuFileExport(wxCommandEvent &event);
        void menuFilePage_setup(wxCommandEvent &event);
        void menuFilePrintOptions(wxCommandEvent &event);
        void menuFilePrint_preview(wxCommandEvent &event);
        void menuFilePrint(wxCommandEvent &event);
        
        void menuEditUndo(wxCommandEvent &event);
        void menuEditCut(wxCommandEvent &event);
        void menuEditCopy(wxCommandEvent &event);
        void menuEditCopyCoordinates(wxCommandEvent &event);
        void CopyCoordinates(short coordtype) const;
        void menuEditPaste(wxCommandEvent &event);
        /// wxEVT_UPDATE_UI event handler for wxID_PASTE
        void OnPasteUpdate( wxUpdateUIEvent& event );
        void PasteText(void);
        void menuEditClear(wxCommandEvent &event);
        void menuEditSelect_all(wxCommandEvent &event);
		void menuEditInteractive_mode(wxCommandEvent &event);

        void menuViewShowNormalMode(wxCommandEvent &event);
        void menuViewPrevNormalMode(wxCommandEvent &event);
        void menuViewNextNormalMode(wxCommandEvent &event);
          // void menuViewDisplay_frequencyDOSTUFF(wxCommandEvent &event);
        void menuViewOffsetAlongMode(wxCommandEvent &event);
        void menuViewAnimateMode(wxCommandEvent &event);
        void menuViewAnimateFrames(wxCommandEvent &event);
        //void menuViewShow_special_atoms(wxCommandEvent &event);
        //void menuVeiwShow_hydrogen_labels(wxCommandEvent &event);
        void menuViewShowAxis(wxCommandEvent &event);
		void menuViewShowSymmetryOperators(wxCommandEvent &event);
		void menuViewHideAtomLabels(wxCommandEvent &event);
		void menuViewShowAtomLabel(wxCommandEvent &event);
		void menuViewShowAtomNumber(wxCommandEvent &event);
		void menuViewShowBothAtomLabels(wxCommandEvent &event);
		void menuViewWireFrameStyle(wxCommandEvent &event);
		void menuViewBallAndStickStyle(wxCommandEvent &event);
        void menuViewShrink_10(wxCommandEvent &event);
        void menuViewEnlarge_10(wxCommandEvent &event);
        void menuViewCenter(wxCommandEvent &event);
        void menuViewRotateTo_X_axis(wxCommandEvent &event);
        void menuViewRotateTo_Y_axis(wxCommandEvent &event);
        void menuViewRotateTo_Z_axis(wxCommandEvent &event);
        void menuViewRotate180_horizontal(wxCommandEvent &event);
        void menuViewRotate180_vertical(wxCommandEvent &event);
        void menuViewRotatePrinciple_orientation(wxCommandEvent &event);
        void menuViewRotateOther(wxCommandEvent &event);
		void menuViewSetWindowParameters(wxCommandEvent &event);
        //void menuViewStereo(wxCommandEvent &event);
        
		void menuPreferences(wxCommandEvent &event);
        void ClosePrefsWindow(void);

        void menuMoleculeSetBondLength(wxCommandEvent &event);
        void menuMoleculeSetFrameEnergy(wxCommandEvent &event);
        void menuMoleculeCreateLLMPath(wxCommandEvent &event);
        void menuMoleculeMinimizeFrameMovements(wxCommandEvent &event);
        void menuMoleculeConvertToBohr(wxCommandEvent &event);
        void menuMoleculeConvertToAngstroms(wxCommandEvent &event);
        void menuMoleculeInvertNormalMode(wxCommandEvent &event);
        
		//Commands to open (or raise) the various data subviews.
        //void menuWindowMain_display(wxCommandEvent &event);
        void menuWindowBonds(wxCommandEvent &event);
        void menuWindowCoordinates(wxCommandEvent &event);
        void menuWindowEnergy_plot(wxCommandEvent &event);
        void menuWindowFrequencies(wxCommandEvent &event);
        void menuWindowInput_builder(wxCommandEvent &event);
        void menuWindowSurfaces(wxCommandEvent &event);
        void menuWindowZMatrixCalc(wxCommandEvent &event);
        void KeyHandler(wxKeyEvent &event);
        void CloseBondsWindow(void);
        void CloseCoordsWindow(void);
        void CloseEnergy_plotWindow(void);
        void CloseFrequenciesWindow(void);
        void CloseSurfacesWindow(void);
        void CloseInputBuilderWindow(void);
        void CloseZMatrixCalc(void);
		void OnActivate(wxActivateEvent & event);
        
        //File handling routines
        long OpenFile(wxString fileName, float offset=0.0, bool flip=false, bool append=false);
        long OpenCMLFile(BufferFile * Buffer, bool readPrefs=true, bool readWindows=true);
        long OpenGAMESSlog(BufferFile *Buffer, bool Append, long flip, float offset);
        long OpenGAMESSIRC(BufferFile * Buffer, bool Append, long flip, float offset);
        long OpenGAMESSDRC(BufferFile * Buffer, bool LogFile, bool Append,
                           long flip, float offset);
        long OpenXYZFile(BufferFile * Buffer);
        long OpenPDBFile(BufferFile * Buffer);
        long OpenMDLMolFile(BufferFile * Buffer);
        long OpenGAMESSInput(BufferFile * Buffer);
        long OpenMolPltFile(BufferFile *Buffer);
		long OpenMoldenFile(BufferFile * Buffer);
        long ParseSIMMOMLogFile(BufferFile *Buffer, long EnergyPos);
		void ExportGAMESS(BufferFile * Buffer, bool AllFrames);
		void WriteTabbedEnergies(BufferFile * Buffer, bool AllFrames);
		void WriteFrequencies(BufferFile * Buffer);
		void WriteXYZFile(BufferFile * Buffer, bool AllFrames, bool AllModes, bool AnimateMode);
		void WriteMDLMolFile(BufferFile * Buffer);
         //Call to dump the message and close the window
        void AbortOpen(const char * msg);
        
        //General Utility routines
        void AtomsChanged(void);
        void BondsChanged(void);
        void FrameChanged(void);
        void ChangeFrames(long NewFrame);
        void ModeChanged(void);
        void ChangeModes(long NewMode);
        void UpdateModelDisplay(void);
        MoleculeData * GetData(void) const {return MainData;};
        WinPrefs * GetPrefs(void) const {return Prefs;};
		void SetWindowPreferences(WinPrefs * NewPrefs) {if (Prefs) *Prefs=*NewPrefs; }
		void ChangePrefs(WinPrefs * newPrefs);
        void ResetView(void);
        void ResetModel(bool Center);
        void ResetAllWindows(void);
        void UpdateFrameText(void);
        void BeginOperation(void);
        void FinishOperation(void);
        bool OperInProgress(void) const {return OperationInProgress;};
        
        //OpenGL drawing routines
        void InitGLData(void);
		void DeleteGLData(void);
        void DrawGL(void);
        void SortTransparentTriangles(void);
        void DrawTransparentTriangles(void);
        void DrawMoleculeCoreGL(void);
        void AddAxisGL(void);
		void AddSymmetryOperators(void);
        void UpdateGLModel(void);
        void Rotate(wxMouseEvent&);
        void RotateMoleculeGL(bool);
		void DrawHydrogenBond(long bondNum);
        void PrintGL(wxDC * dc, const float & scaleFactor);
	void SetHighliteMode(bool state) { mHighliteState = state; }
	void DrawStaticLabel(const char* label, GLfloat x, GLfloat y);
	void DrawLabel(void); //added by Song Li
	void SelectionChanged(bool mode);

        void eventSize(wxSizeEvent &event);
        void SizeChanged(void);
		void OnFrameAnimationTimer(wxTimerEvent & event);
            //Function to receive all changes to the scroll bar
        void OnScrollBarChange(wxScrollEvent& event);
			//Called during normal mode animations
		void OnModeAnimation(wxTimerEvent & event);
			//Function to be called when window loses focus to stop animations
		void OnMenuOpen(wxMenuEvent & event);
		void OnKillFocus(wxFocusEvent & event);
		void StopAnimations(void);
};

class MolPrintOut : public wxPrintout {
public:
    MolPrintOut(MolDisplayWin * parent, wxString & title);
    bool OnPrintPage(int page);
    bool HasPage(int page);
    bool OnBeginDocument(int startPage, int endPage);
    void GetPageInfo(int *minPage, int *maxPage, int *selPageFrom, int *selPageTo);
private:
        MolDisplayWin * Parent; 
};

#endif /* #ifndef MolDisplayWin_H */

