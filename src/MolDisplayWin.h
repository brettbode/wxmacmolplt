/*
 * (c) 2005 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * MolDisplayWin.h
 *
 * Defines a class that overloads the wxFrame class.  This class is used for
 * the document window(s).
 *
 * Created:       11-02-2005  Steven Schulteis
 * Last Modified: 01-19-2006  Steven Schulteis
 ***************************************/

#ifndef MolDisplayWin_H
#define MolDisplayWin_H 1

#include "wx/wx.h"

#include "mpGLCanvas.h"
#include "MoleculeData.h"
#include "Prefs.h"
#include "Progress.h"

typedef class OpenGLRec OpenGLRec;
typedef class BondsDlg BondsDlg;
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
		wxMenu    *menuViewDisplay_frequency;
		wxMenu    *menuViewRotate;
		wxMenu    *menuMolecule;
		wxMenu    *menuWindow;
		wxMenu    *menuHelp;

        MoleculeData *	MainData;
		WinPrefs 	*	Prefs;
		bool			Dirty;			//Flag to indicate a save is needed
		bool			OperationInProgress;
		
		Progress *		ProgressInd;	// Progress indicator window for long operations
		OpenGLRec *		OpenGLData;		// Extra OpenGL data
        MpGLCanvas *	glCanvas;
		
		BondsDlg *		bondsWindow;

        void createMenuBar(void);
		long OpenGAMESSIRCLog(BufferFile * Buffer, long flip, float offset,
							  long NumOccAlpha, long NumOccBeta, long NumFragmentAtoms);

        DECLARE_EVENT_TABLE()

    public:
        /* Constructor that provides some useful default values.
         * @param title The text that will be displayed in the new window's title bar.
         * @param position The initial position of the new window.
         * @param size The initial size of the new window.
         * @param style The style of the new window.  See wxFrame class docs.
         * @param name The name of the new window.
         */
        MolDisplayWin(const wxString &title,
                    const wxPoint  &position = wxDefaultPosition,
                    const wxSize   &size     = wxDefaultSize,
                    long            style    = wxDEFAULT_FRAME_STYLE,
                    const wxString &name     = wxT("MpMainFrame"));

        /**
         * Destructor, mostly for cleaning up data that we've allocated
         * independently of wxWidgets.
         */
        virtual ~MolDisplayWin();

		//Call to update the menu bar itesm for the current data
		void AdjustMenus(void);
		void ClearMenus(void);
        // Below are the event handler functions for the menu.  The initial
        // list was taken from the original MacMolPlt menus.

        void menuFileOpen(wxCommandEvent &event);
        void menuFileSave(wxCommandEvent &event);
        void menuFileSave_as(wxCommandEvent &event);
        void menuFileClose(wxCommandEvent &event);
        void FileClose(wxCloseEvent &event);
        //void menuFileAppend_new_frame(wxCommandEvent &event);
        //void menuFileAdd_frames_from_file(wxCommandEvent &event);
        //void menuFileDelete_frame(wxCommandEvent &event);
        //void menuFileImport(wxCommandEvent &event);
        //void menuFileExport(wxCommandEvent &event);
        void menuFilePage_setup(wxCommandEvent &event);
        void menuFilePrint_preview(wxCommandEvent &event);
        void menuFilePrint(wxCommandEvent &event);
        
        void menuEditUndo(wxCommandEvent &event);
        void menuEditCut(wxCommandEvent &event);
        void menuEditCopy(wxCommandEvent &event);
        //void menuEditCopy_coordinates(wxCommandEvent &event);
        void menuEditPaste(wxCommandEvent &event);
		void PasteText(void);
        void menuEditClear(wxCommandEvent &event);
        void menuEditSelect_all(wxCommandEvent &event);

        void menuViewShowNormalMode(wxCommandEvent &event);
		void menuViewPrevNormalMode(wxCommandEvent &event);
		void menuViewNextNormalMode(wxCommandEvent &event);
          // void menuViewDisplay_frequencyDOSTUFF(wxCommandEvent &event);
        //void menuViewOffset_along_mode(wxCommandEvent &event);
        //void menuViewAnimate_mode(wxCommandEvent &event);
        //void menuViewAnimate_frames(wxCommandEvent &event);
        //void menuViewShow_special_atoms(wxCommandEvent &event);
        //void menuVeiwShow_hydrogen_labels(wxCommandEvent &event);
        //void menuViewShow_atom_labels(wxCommandEvent &event);
        //void menuViewShow_atom_numbers(wxCommandEvent &event);
		void menuViewShowAxis(wxCommandEvent &event);
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
        //void menuViewSet_window_parameters(wxCommandEvent &event);
        //void menuViewStereo(wxCommandEvent &event);
        
        //void menuMoleculeSet_bond_length(wxCommandEvent &event);
        //void menuMoleculeSet_frame_energy(wxCommandEvent &event);
        //void menuMoleculeCreate_llm_path(wxCommandEvent &event);
        //void menuMoleculeMinimize_frame_movements(wxCommandEvent &event);
        void menuMoleculeConvertToBohr(wxCommandEvent &event);
        void menuMoleculeConvertToAngstroms(wxCommandEvent &event);
        void menuMoleculeInvertNormalMode(wxCommandEvent &event);
        
        //void menuWindow_Zmat_calculator(wxCommandEvent &event);
        /* On Mac, these will be part of dynamically created sub-menus for each
         * main window*/
        //void menuWindowMain_display(wxCommandEvent &event);
        void menuWindowBonds(wxCommandEvent &event);
        //void menuWindowCoordinates(wxCommandEvent &event);
        //void menuWindowEnergy_plot(wxCommandEvent &event);
        //void menuWindowFrequencies(wxCommandEvent &event);
        //void menuWindowInput_builder(wxCommandEvent &event);
        //void menuWindowSurfaces(wxCommandEvent &event);
        //void menuWindowPreferences(wxCommandEvent &event);
		void KeyHandler(wxKeyEvent &event);
		void CloseBondsWindow(void);
		
		//File handling routines
		long OpenFile(wxString fileName);
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
		long ParseSIMMOMLogFile(BufferFile *Buffer, long EnergyPos);
			//Call to dump the message and close the window
		void AbortOpen(const char * msg);
		
		//General Utility routine
		void BondsChanged(void);
		void FrameChanged(void);
		void ChangeFrames(long NewFrame);
		void UpdateModelDisplay(void);
		MoleculeData * GetData(void) const {return MainData;};
		void ResetView(void);
		void ResetModel(bool Center);
		void ResetAllWindows(void);
		void BeginOperation(void);
		void FinishOperation(void);
		bool OperInProgress(void) const {return OperationInProgress;};
		
		//OpenGL drawing routines
		void InitGLData(void);
		void DrawGL(void);
		void SortTransparentTriangles(void);
		void DrawTransparentTriangles(void);
		void DrawMoleculeCoreGL(void);
		void AddAxisGL(void);
		void UpdateGLModel(void);
		void Rotate(wxMouseEvent&);
		void RotateMoleculeGL(bool);
};

#endif /* #ifndef MolDisplayWin_H */

