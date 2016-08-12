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

#ifdef __WXOSX_COCOA__
#include <OpenGL/glu.h>
#elif __WXMAC__
#include <AGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "mpGLCanvas.h"
#include "MoleculeData.h"
#include "Prefs.h"
#include "Progress.h"
#include <deque>
#include "MyTypes.h"

typedef class BondsDlg BondsDlg;
typedef class CoordinatesWindow CoordinatesWindow;
typedef class EnergyPlotDialog EnergyPlotDialog;
typedef class FrequenciesDialog FrequenciesDialog;
typedef class SurfacesWindow SurfacesWindow;
typedef class InputBuilderWindow InputBuilderWindow;
typedef class ModeAnimation ModeAnimation;
typedef class ZMatrixCalculator ZMatrixCalculator;
typedef class setPreference setPreference;
typedef struct qtData qtData;
typedef class MolStatusBar MolStatusBar;
typedef class UndoSnapShot UndoSnapShot;
//typedef class ExportOptionsDialog ExportOptionsDialog;
class ExportOptionsDialog;

//#define ID_LOCAL_PREFERENCES 501
/** MMP_EventID contains a series of ids used mostly for menu items within the
 * MolDisplayWin class. They are defined here so they can be reused for the same menus
 * in subwindows of MolDisplayWin to allow the menu item to transparently be passed up
 * to the parent window.
 */
enum MMP_EventID {
	MMP_SHRINK10=wxID_HIGHEST+1,
	MMP_ENLARGE10,
	MMP_SHOWMODE,
	MMP_PREVMODE,
	MMP_NEXTMODE,
	MMP_SHOWAXIS,
	MMP_SHOWBUILDTOOLS,
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
	MMP_EFP_WIREFRAME,
	MMP_TOGGLEVISSUBMENU,
	MMP_TOGGLEABINITIO,
	MMP_TOGGLEMMATOMS,
	MMP_TOGGLEEFPATOMS,
	MMP_ADDMARKANNOTATION,
	MMP_ADDLENGTHANNOTATION,
	MMP_ADDANGLEANNOTATION,
	MMP_ADDDIHEDRALANNOTATION,
	MMP_DELETEALLANNOTATIONS,
	MMP_CENTER,
	MMP_AUTOROTATE,
	MMP_ROTATESUBMENU,
	MMP_ROTATETOXAXIS,
	MMP_ROTATETOYAXIS,
	MMP_ROTATETOZAXIS,
	MMP_ROTATE180HOR,
	MMP_ROTATE180VER,
	MMP_ROTATEPRINC,
	MMP_ROTATEOTHER,
	MMP_CREATELLMPATH,
	MMP_MINFRAMEMOVEMENTS,
	MMP_CONVERTTOBOHR,
	MMP_CONVERTTOANGSTROMS,
	MMP_INVERTNORMALMODE,
	MMP_ADDHYDROGENS,
	MMP_DELETEHYDROGENS,
	
	MMP_NEWFRAME,
	MMP_COPYCOORDS,
	MMP_COPYNWCOORDS,
	MMP_ENERGYEDIT,
	MMP_SETBONDLENGTH,
	MMP_DETERMINEPG,
	MMP_SYMADAPTCOORDS,
	MMP_EXPORT,
	MMP_ANNOTATIONSSUBMENU,
	
	MMP_ADDFRAMES,
	MMP_DELETEFRAME,
	MMP_IMPORTMENU,
	MMP_MOLECULEDISPLAYWINDOW,
	MMP_BONDSWINDOW,
	MMP_COORDSWINDOW,
	MMP_ENERGYPLOTWINDOW,
	MMP_FREQUENCIESWINDOW,
	MMP_INPUTBUILDERWINDOW,
	MMP_INPUT_WIZARD,
	MMP_SURFACESWINDOW,
	MMP_FRAMESCROLLBAR,
	MMP_PRINTOPTIONS,
	MMP_ANIMATEFRAMES,
	MMP_ANIMATEFRAMESTIMER,
	MMP_ANIMATEMODE,
	MMP_ANIMATEMODETIMER,
	MMP_STATUS_TIMER,
	MMP_ROTATE_TIMER,
	MMP_OFFSETMODE,
	MMP_WINDOWPARAMETERS,
	MMP_ZMATRIXCALC,
	MMP_LOCAL_PREFERENCES,
	MMP_SHOW_TOOLBAR,
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
	MMP_SELECT_ALL,
	MMP_SELECT_NONE,
	MMP_SHOWBONDSITES,
	MMP_SYMMETRY_EDIT,
	/* MMP_SYMMETRY_REGEN, */
	MMP_SAVESTRUCTURE,
	MMP_SHOW_FULLSCREEN,
	
	Number_MMP_Ids
};

/**
	Abstract class to hold snapshot data. All you can do with a snapshot is
	create it and restore it.
*/
class UndoSnapShot {
public:
	virtual ~UndoSnapShot() {}
	virtual void Restore() {}
};

/**
	Class to hold snapshot data based on a single frame. This class will save/restore the basic
	data of a single frame, ie atoms and bonds.
 */
class FrameSnapShot : public UndoSnapShot {
public:
	/** Create a snapshot from the provided frame pointer.
	*/
	FrameSnapShot(MoleculeData * target);
	virtual ~FrameSnapShot();
	/** Restore the saved data. Note the frame point is stored here
		so you can't restore if the original frame memory has been
		destroyed.
	*/
	virtual void Restore();
private:
	MoleculeData * targetData;	///< The target data structure
	Frame *		mTarget;	///< The target Frame.
	mpAtom *	Atoms;		///< Saved atom list
	Bond *		Bonds;		///< Saved bond list
	long		NumAtoms;	///< atom count
	long		NumBonds;	///< bond count
	std::vector<std::string> FragmentNames;	//< Effective Fragment name for each fragment (FRAGNAME)
	Internals * IntCoords;	///< Set of internal coordinate definitions
};

/** Container class for undo/redo data. This class manages the list of
	snapshots such that the program just needs to add or retrieve snapshots.
	Normally each of the MolDisplayWins will contain one of these classes.
*/
class UndoData {
public:
	UndoData() : operationCount(0), position(0) {}
	~UndoData() {Clear();}
	///How many snapshots are saved.
	int GetOperationCount() const {return operationCount;}
	///Obtains the position in the snapshot list.
	int GetPosition() const {return position;}
	///Move the snapshot position manually (not normally needed).
	void SetPosition(int p) {if ((p>=0)&&(p<operationCount)) position = p;}
	///Is undo possible?
	bool undoPossible() const {return ((operationCount > 0)&&(position > 0));}
	///Is redo possible?
	bool redoPossible() const {return ((position+1) < operationCount);}
	///Remove all snapshots and free their memory.
	void Clear();
	///Add a snapshot to the stack.
	void AddSnapshot(UndoSnapShot *);
	///Undo to the previous snapshot.
	void UndoOperation();
	///Redo to the next snapshot.
	void RedoOperation();
private:
	std::deque<UndoSnapShot *>	UndoList;
	int	operationCount;
	int position;
};

/**
 * Subclasses wxFrame to define the main application window.  This is a
 * document window.  There can be multiple instances of the window in the
 * application, and each main window owns its dialogs.
 */
class MolDisplayWin : public wxFrame {
	friend class MpGLCanvas;
	private:
		wxMenuBar	*menuBar;
		wxMenu		*menuFile;
		wxMenu		*menuEdit;
		wxMenu		*menuView;
		wxMenu		*menuViewLabels;
		wxMenu		*menuViewStyle;
		wxMenu		*menuViewAnnotations;
		wxMenu		*menuViewRotate;
		wxMenu		*menuBuild;
		wxMenu		*menuMolecule;
		wxMenu		*menuSetPG;
		wxMenu		*menuPointGroup;
		wxMenu		*menuPGOrder;
		wxMenu		*menuWindow;
		wxMenu		*menuHelp;

		wxString		currFilePath;
		MoleculeData	*MainData;
		WinPrefs		*Prefs;
		UndoData		mUndoBuffer;
		bool			Dirty;          ///< Flag to indicate a save is needed
		bool			OperationInProgress;
		bool			timerRunning;
		bool			show_fullscreen;
		bool			edit_symmetrically;
		bool			do_rotate_annotation;
		bool			GLUpdateInProgress;		///< flag to prevent multiple simultaneous GL updates
		
		Progress		*ProgressInd;    ///< Progress indicator window for long operations
		MpGLCanvas		*glCanvas;
		MolStatusBar	*myStatus;
		ModeAnimation	*ModeAnimationData;
		wxTimer			m_timer;
		wxTimer			status_timer;
		wxTimer			rotate_timer;

		BondsDlg			*bondsWindow;
		CoordinatesWindow	*coordsWindow;
		EnergyPlotDialog	*energyPlotWindow;
		FrequenciesDialog	*frequenciesWindow;
		SurfacesWindow		*surfacesWindow;
		InputBuilderWindow	*inputBuilderWindow;
		setPreference		*prefsDlg;
		ZMatrixCalculator	*zMatCalcDlg;
		
		WindowData			winData;
		
		wxPageSetupDialogData *pageSetupData;
		wxPrintData           *printData;

		bool mHighliteState;
		bool stale_click;
		wxToolBar *toolbar;
		bool mAltModifyingToolBar;

		bool is_lassoing;
		bool lasso_has_area;
		wxPoint lasso_start;
		wxPoint lasso_end;

		wxPoint mouse_start;
		wxPoint inertia;

		bool show_bond_sites;
		bool window_just_focused;

		myGLTriangle *transpTriList;		///< The list of transparent triangles
		float *transpSortVertex;	///< The rotated screen Z coordinate to sort on.
		long *transpIndex;		///< The sorted triangle list
		long triangleCount;		///< The number of transparent triangles
		
		GLuint MainDisplayList;	///< id for a main display list
		GLuint SurfaceDisplayList;
		bool MainListActive;
		bool SurfaceListActive;
		bool haveTransparentSurfaces;	///< Flag if there are transparent surfaces
		GLuint length_anno_tex_id;			///< texture id for length annotations
		GLuint sphere_list;
		GLuint shader_program;
		GLuint depth_fbo;
		GLuint depth_tex_id;
		GLfloat fg_color[3];
		GLfloat light_pos[4];

			///Called by the constructor to setup the initial menubar
		void createMenuBar();
		/**
		 * Called by the GAMESS log file code to specifically handle IRC type log files.
		 * @param Buffer The file buffer to parse.
		 * @param flip Indicator that the IRC distance should be multiplied by -1.
		 * @param offset Amount to offset the transition state for the IRC.
		 * @param NumOccAlpha number of alpha occupied orbitals.
		 * @param NumOccBeta Number of beta occupied orbitals.
		 * @param NumFragmentAtoms Number of EFP style atoms.
		 */
		long OpenGAMESSIRCLog(BufferFile * Buffer, long flip, float offset,
						  long NumOccAlpha, long NumOccBeta, long NumFragmentAtoms);
		/**
		 * Called by the GAMESS log file code to specifically handle GlobOp type log files.
		 * @param Buffer The file buffer to parse.
		 * @param NumOccAlpha number of alpha occupied orbitals.
		 * @param NumOccBeta Number of beta occupied orbitals.
		 * @param NumFragmentAtoms Number of EFP style atoms.
		 */
		long OpenGAMESSGlobOpLog(BufferFile * Buffer, long NumOccAlpha, long NumOccBeta,
								 long NumFragmentAtoms);
		/**
		 * Callback for when the user clicks on a button in the toolbar. The function ensures
		 * at exactly one button is always selected.
		 */
		void OnToggleTool(wxCommandEvent& event);
		void OnStatusTimer(wxTimerEvent& event);
		void OnRotateTimer(wxTimerEvent& event);
		void OnShowNormalScreen(wxUpdateUIEvent& event);
		void OnSaveUpdate(wxUpdateUIEvent& event);
		void ShowToolbar(bool enable = true);
		void OnShowToolbarUpdate(wxUpdateUIEvent& event);
		void OnShowBuildToolsUpdate(wxUpdateUIEvent& event);

		void OnSaveStructureUpdate(wxUpdateUIEvent& event);
		void DeleteSelected();
		void RegenerateSymmetryDependent();
		void DoPrefDependent();
		std::map<std::string, EFrag>::const_iterator FindFragmentDef(std::string fragName);

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
					const wxSize   &size     = wxDefaultSize,
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
		void AdjustMenus();
		void ClearMenus();
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
		void menuEditRedo(wxCommandEvent &event);
		void OnUndoUpdate( wxUpdateUIEvent& event );
		void OnRedoUpdate( wxUpdateUIEvent& event );
		void menuEditCut(wxCommandEvent &event);
		void menuEditCopy(wxCommandEvent &event);
		/// Callback for the GAMESS style copy coords menu item
		void menuEditCopyCoordinates(wxCommandEvent &event);
		/// Callback for the NWChem style copy coords menu item
		void menuEditCopyNWChemCoordinates(wxCommandEvent &event);
		/// Place a text copy of the coordinates into the copy buffer
		/**
		 * Creates a text copy of the coorindates and places on to the copy buffer.
		 * @param[in] coordtype specifies GAMESS style cartesians (0), GAMESS Z-matrix (1) or NWChem style Cartesians(2)
		 */
		void CopyCoordinates(short coordtype) const;
		void menuEditPaste(wxCommandEvent &event);
		/// wxEVT_UPDATE_UI event handler for wxID_PASTE
		void OnPasteUpdate( wxUpdateUIEvent& event );
		void PasteText();
		void menuEditClear(wxCommandEvent &event);
		void menuEditSelectAll(wxCommandEvent &event);
		void menuEditSelectNone(wxCommandEvent &event);
		/// wxEVT_UPDATE_UI event handler for selection changes
		void OnSelectAllUpdate(wxUpdateUIEvent& event);
		void OnSelectNoneUpdate(wxUpdateUIEvent& event);
		void OnDeleteUpdate(wxUpdateUIEvent& event);

		void menuViewShowFullScreen(wxCommandEvent &event);
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
		void menuViewAddAnnotation(wxCommandEvent &event);
		void menuViewAddMarkAnnotation(wxCommandEvent &event);
		///Menu item handler to toggle the visibility of ab inito atoms
		void menuViewToggleAIAtomsVis(wxCommandEvent & event);
		///Menu item handler to toggle the visibility of Molecular mechanics atoms
		void menuViewToggleMMAtomsVis(wxCommandEvent & event);
		///Menu item handler to toggle the visibility of effective fragment potential atoms
		void menuViewToggleEFPAtomsVis(wxCommandEvent & event);
		void OnAnnotationMarkUpdate(wxUpdateUIEvent &event);
		void OnAnnotationLengthUpdate(wxUpdateUIEvent &event);
		void OnAnnotationAngleUpdate(wxUpdateUIEvent &event);
		void OnAnnotationDihedralUpdate(wxUpdateUIEvent &event);
		void OnShowSymOpsUpdate(wxUpdateUIEvent &event);
		void menuViewDeleteAllAnnotations(wxCommandEvent &event);
		void OnDeleteAnnotationsUpdate(wxUpdateUIEvent &event);
		void menuViewShow2DPattern(wxCommandEvent &event);
		void OnShowPatternUpdate(wxUpdateUIEvent &event);
		void menuViewWireFrameStyle(wxCommandEvent &event);
		void menuViewBallAndStickStyle(wxCommandEvent &event);
		void menuViewEFP_Wireframe(wxCommandEvent &event);
		void OnEFPWireFrameUpdate(wxUpdateUIEvent &event);
		void menuViewShrink_10(wxCommandEvent &event);
		void menuViewEnlarge_10(wxCommandEvent &event);
		void menuViewCenter(wxCommandEvent &event);
		void menuViewToggleAutoRotation(wxCommandEvent &event);
		void OnAutoRotationUpdate(wxUpdateUIEvent &event);
		void menuViewRotateInPlace(const Matrix4D new_m);
		void menuViewRotateTo_X_axis(wxCommandEvent &event);
		void menuViewRotateTo_Y_axis(wxCommandEvent &event);
		void menuViewRotateTo_Z_axis(wxCommandEvent &event);
		void menuViewRotate180_horizontal(wxCommandEvent &event);
		void menuViewRotate180_vertical(wxCommandEvent &event);
		void menuViewRotatePrinciple_orientation(wxCommandEvent &event);
		void menuViewRotateOther(wxCommandEvent &event);
		void menuViewSetWindowParameters(wxCommandEvent &event);
		//void menuViewStereo(wxCommandEvent &event);
		
		void menuBuilderSaveStructure(wxCommandEvent &event);

		void menuPreferences(wxCommandEvent &event);
		void ClosePrefsWindow();

		void menuMoleculeSetBondLength(wxCommandEvent &event);
		void menuMoleculeSetFrameEnergy(wxCommandEvent &event);
		/**
		 * Updates the Create LLM menu item.
		 */
		void OnMoleculeCreateLLMPathUpdate(wxUpdateUIEvent &event);
		/**
		 * Put up the Create LLM dialog. This should only be called when there is at
		 * least one frame after the current frame.
		 */
		void menuMoleculeCreateLLMPath(wxCommandEvent &event);
		void menuMoleculeMinimizeFrameMovements(wxCommandEvent &event);
		void OnShowPointGroupUpdate(wxUpdateUIEvent &event);
		void menuSetPointGroup(wxCommandEvent &event);
		void menuSetPointGroupOrder(wxCommandEvent &event);
		void menuMoleculeDetermineSym(wxCommandEvent &event);
		void menuMoleculeSymCoords(wxCommandEvent &event);
		void menuMoleculeConvertToBohr(wxCommandEvent &event);
		void menuMoleculeConvertToAngstroms(wxCommandEvent &event);
		void menuMoleculeInvertNormalMode(wxCommandEvent &event);

		void Dirtify(bool is_dirty = true);

		/**
		 * Fills out the coordination of every atom with hydrogens.
		 * @param event The command event (not used).
		 */
		void menuBuilderAddHydrogens(wxCommandEvent &event);
		/**
		 * Deletes all hydrogen atoms, except non-bonded atoms, and atoms that are
		 * part of fragments or SIMOMM.
		 * @param event The command event (not used).
		 */
		void menuBuilderDeleteHydrogens(wxCommandEvent &event);
		/**
		 * Toggles the display of bonding sites when in edit mode in the molecule builder.
		 * @param event The command event (not used).
		 */
		void menuBuilderShowBondSites(wxCommandEvent &event);
		void menuBuilderSymmetryEdit(wxCommandEvent &event);
		/* void menuBuilderRegenSymmetry(wxCommandEvent &event); */
		/**
		 * Toggles the toolbar. When active a toolbar is added to the main
		 * display window.
		 * @param event The command event (not used).
		 */
		void menuBuilderShowToolbar(wxCommandEvent &event);
		/**
		 * Toggles the build tools palette.
		 * @param event The command event (not used).
		 */
		void menuBuilderShowBuildTools(wxCommandEvent &event);
		/**
		 * Updates the add hydrogens menu item based on the current edit mode and ability to add
		 * hydrogens.
		 * @param event The UpdateUI event.
		 */
		void OnAddHydrogensUpdate( wxUpdateUIEvent& event );
		/**
		 * Updates the show bond sites menu item based on the edit mode.
		 * @param event The UpdateUI event.
		 */
		void OnShowBondSitesUpdate(wxUpdateUIEvent& event);
		void OnShowSymmetryEdit(wxUpdateUIEvent& event);
		void UpdateAtomsOptions(wxUpdateUIEvent& event);
		
		//Commands to open (or raise) the various data subviews.
	
		/// Raise the main molecule display window.
		/// Passed up from subwindows.
		void menuWindowMoleculeDisplay(wxCommandEvent &event);
		void menuWindowBonds(wxCommandEvent &event);
		void menuWindowCoordinates(wxCommandEvent &event);
		void menuWindowEnergy_plot(wxCommandEvent &event);
		void menuWindowFrequencies(wxCommandEvent &event);
		void menuWindowInput_builder(wxCommandEvent &event);
		void menuWindowSurfaces(wxCommandEvent &event);
		void menuWindowZMatrixCalc(wxCommandEvent &event);
		void KeyHandler(wxKeyEvent &event);
		/**
		 * Handles KEY_UP events received by the window. 
		 * @param event The event to handle.
		 */
		void KeyUpHandler(wxKeyEvent &event);
		void CloseBondsWindow();
		void CloseCoordsWindow();
		void CloseEnergy_plotWindow();
		void CloseFrequenciesWindow();
		void CloseSurfacesWindow();
		void CloseInputBuilderWindow();
		void CloseZMatrixCalc();
		void OnActivate(wxActivateEvent & event);
		
		//File handling routines
		long OpenFile(wxString fileName, float offset=0.0, bool flip=false, bool append=false);
		long OpenCMLFile(BufferFile * Buffer, bool readPrefs=true, bool readWindows=true);
		long OpenGAMESSlog(BufferFile *Buffer, bool Append, long flip, float offset);
		/**
		 Open a GAMESS IRC file (pre 2009). 
		 @param Buffer The buffer connected to the DRC file.
		 @param Append true if this file is to be appended to existing points.
		 @param flip is a multiplier for the x-coordinate (such as -1).
		 @param offset is added to the x-coordinate.
		 \return Returns 1 upon open success or 0 on error or failure to open
		 */
		long OpenGAMESSIRC(BufferFile * Buffer, bool Append, long flip, float offset);
		/**
		 Open a GAMESS DRC (aka DRP) file (pre 2009). 
		 @param Buffer The buffer connected to the DRC file.
		 @param LogFile true if this is a DRC GAMESS log file, false if the .irc file.
		 @param Append true if this file is to be appended to existing points.
		 @param flip is a multiplier for the x-coordinate (such as -1).
		 @param offset is added to the x-coordinate.
		 \return Returns 1 upon open success or 0 on error or failure to open
		 */
		long OpenGAMESSDRC(BufferFile * Buffer, bool LogFile, bool Append,
							long flip, float offset);
		/**
		 Open a GAMESS trajectory file. 
		 @param Buffer The buffer connected to the TRJ file.
		 @param Append true if this file is to be appended to existing points.
		 @param flip is a multiplier for the x-coordinate (such as -1).
		 @param offset is added to the x-coordinate.
		 \return Returns 1 upon open success or 0 on error or failure to open
		 */
		long OpenGAMESSTRJ(BufferFile * Buffer, bool Append, long flip, float offset);
		long OpenXYZFile(BufferFile * Buffer);
		/**
		 * Parses the Protein Databank Format (just atoms). 
		 * @param Buffer A BufferFileObject which the PDB file is buffered into
		 * @return Returns 1 upon open success or 0 on error or failure to open
		 */
		long OpenPDBFile(BufferFile * Buffer);
		/**
		 * Parses the MDL Mol Format (just atoms). 
		 * @param Buffer A BufferFileObject which the MDL mol file is buffered into
		 * @return Returns 1 upon open success or 0 on error or failure to open
		 */
		long OpenMDLMolFile(BufferFile * Buffer);
		/**
		 Open a GAMESS input or ".INP" format file. 
		 @param Buffer The buffer connected to the INP file.
		 */
		long OpenGAMESSInput(BufferFile * Buffer);
		long OpenMolPltFile(BufferFile * Buffer);
		long OpenMoldenFile(BufferFile * Buffer);
		/**
		 Open a Molekel or ".MKL" format file. 
		 @param Buffer The buffer connected to the MKL file.
		 \return Returns 1 upon open success or 0 on error or failure to open
		 */
		long OpenMKLFile(BufferFile * Buffer);
		long ParseSIMMOMLogFile(BufferFile * Buffer, long EnergyPos);
		/**
		 Open a MOPAC ".MOP" or archive ".ARC" file.
		 @param Buffer The buffer connected to the MOP file.
		 @param fileType char designating which file extension is used
		 \return Returns 1 upon open success or 0 on error or failure to open
		*/
		long OpenMOPACFile(BufferFile * Buffer, TextFileType fileType);
		/**
		 Output the coordinates in gamess input format ($DATA and $EFRAG group).
		 If AllFrames then a series of $DATA groups will be output for the user to divide as needed.
		 @param Buffer A buffer to write the text to.
		 @param AllFrames Should the data for all frames be included or just the current frame?
		 */
		void ExportGAMESS(BufferFile * Buffer, bool AllFrames);
		/**
		 Output the plotted energies for the system to a tab de-limited text file. 
		 @param Buffer A buffer to write the text to.
		 @param AllFrames Should the data for all frames be included or just the current frame?
		 */
		void WriteTabbedEnergies(BufferFile * Buffer, bool AllFrames);
		/**
		 Output the frequencies for the current frame to a text file. 
		 @param Buffer A buffer to write the text to.
		 */
		void WriteFrequencies(BufferFile * Buffer);
		/**
		 Export the current system to an XYZ format file. 
		 @param Buffer A buffer to write the text to.
		 @param AllFrames Should the data for all frames be included or just the current frame?
		 @param AllModes Should the normal modes be output or just the current one?
		 @param AnimateMode Flag to produce a sequence of geometries animating the current normal mode.
		 */
		void WriteXYZFile(BufferFile * Buffer, bool AllFrames, bool AllModes, bool AnimateMode);
		/**
		 Export the current system to an MDL mol file. 
		 @param Buffer A buffer to write the text to.
		 */
		void WriteMDLMolFile(BufferFile * Buffer);
		/**
		 Export the current model to a Virtual Reality Markup Language file. 
		 @param Buffer A buffer to write the output to.
		 */
		void WriteVRMLFile(BufferFile * Buffer);
		/**
		 Export the current model to a POV-ray input file. 
		 @param Buffer A buffer to write the output to.
		 */
		void WritePOVFile(BufferFile *Buffer);
		/**
		 Export a series of frames or normal mode animation to an animated GIF file.
		 @param filepath The target name for the file
		 @param dlg The export options dialog to be used to retrieve the various movie setup options
		 */
		void WriteGIFMovie(wxString & filepath, ExportOptionsDialog * dlg);
#ifdef __MAC_USE_QUICKTIME__
		//quicktime movie export
		void WriteQTMovie(wxString & filepath);
		void CreateFrameMovie(GWorldPtr lgWorld, Handle CompressedData,
							  const qtData & myqtData, bool IncludeEPlot);
		void CreateModeMovie(GWorldPtr lgWorld, Handle CompressedData,
							 const qtData & myqtData);
#endif
#ifdef HAVE_LIBMING
		void WriteFlashMovie(wxString & filepath);
        void CreateFrameMovie(wxString & filePath, const bool includeEP);
		void CreateModeMovie(wxString & filePath);
#endif
		//Call to dump the message and close the window
		void AbortOpen(const char * msg);
		
		//General Utility routines
		/**
			Call when the atoms list is updated to cause all subwindows to update.
			@param updateCoordsWin	Should the coordinates window be updated (if open)?
			@param updateDisplay Should the main molecule display be updated?
		 */
		void AtomsChanged(bool updateCoordsWin=false, bool updateDisplay=false);
		/**
			Call when the bonds list is changed to update the bonds subwindow and the main display.
		 */
		void BondsChanged();
		/**
			Call to update surfaces (as needed) and the main display window. Generally this
			is called after the frame is changed or after a significant change to the frame data.
		 */
		void FrameChanged();
		void ChangeFrames(long NewFrame);
		/**
			Call to insert the selected atoms at the provided index.
			@param index The position in the atom list to insert the 1st selected atom.
		 */
		void ChangeAtomOrder(long index);
		void ModeChanged();
		void ChangeModes(long NewMode);
		void UpdateModelDisplay();
		MoleculeData * GetData() const {return MainData;}
		WinPrefs * GetPrefs() const {return Prefs;}
		void SetWindowPreferences(WinPrefs * NewPrefs);
		/* void ChangePrefs(WinPrefs * newPrefs); */
		void ResetView();
		void ResetModel(bool Center);
		void ResetAllWindows();
		void UpdateFrameText();
		void BeginOperation();
		void FinishOperation();
		bool OperInProgress() const {return OperationInProgress;}
		/**
		 * Updates the visibility and position information. This should
		 * be called before saving the state of the file.
		 */
		void UpdateWindowData();
		/**
		 * Pulls the window data for this document, copies it to the default and
		 * saves it in the user preferences.
		 */
		void SetDefaultWindowData();
	
		//OpenGL drawing routines
		void DrawGL(int do_shader = true);
		void SortTransparentTriangles();
		void DrawTransparentTriangles();
		void DrawMoleculeCoreGL();
		void AddAxisGL();
		void AddSymmetryOperators();
		void ReleaseLists();
		void Rotate(wxMouseEvent&);
		void ShowRotation(bool ShowAngles, bool ShowTrackball);
		static void DrawHydrogenBond(const Bond& bond, const mpAtom& atom1,
									 const mpAtom& atom2,
									 const WinPrefs& Prefs,
									 GLUquadric *quadric, GLuint sphere_list,
									 bool highlighting_on = false);
		static void DrawBond(const Bond& bond, const mpAtom& atom1,
							 const mpAtom& atom2, const WinPrefs& Prefs,
							 GLUquadric *quadric, GLdouble *modelview,
							 GLdouble *proj, GLint *viewport,
							 GLuint sphere_list, bool highlighting_on = false,
							 bool cap_dependent = false);
		void PrintGL(wxDC * dc, const float & scaleFactor);
		void SetHighliteMode(bool state) { mHighliteState = state; }
		void DrawStaticLabel(const char* label, GLfloat x, GLfloat y);
		void DrawLabel();
		void SelectionChanged(bool mode);
		GLuint GetLengthTexId() const {return length_anno_tex_id;}
		GLuint GetSphereList() const { return sphere_list; }

		bool IsRotating();
		void eventSize(wxSizeEvent &event);
		/* void SizeChanged(); */
		void OnFrameAnimationTimer(wxTimerEvent & event);
		//Called during normal mode animations
		void OnModeAnimation(wxTimerEvent & event);
		//Function to be called when window loses focus to stop animations
		void OnMenuOpen(wxMenuEvent & event);
		void OnKillFocus(wxFocusEvent & event);
		void StopAnimations();
		void LassoStart(const int x, const int y);
		void LassoGrown(const int x, const int y);
		void LassoEnd();
		bool LassoContains(const int x, const int y);
		bool LassoHasArea();
		bool InSelectionMode();
		bool InViewMode();
		bool InEditMode();
		bool InSymmetryEditMode();
		void DrawBondingSites(long iatom, float radius, GLUquadricObj *qobj, int site_id=0, CPoint3D * vector=NULL);
		void SetStatusText(const wxString& label);
		/**
			Call to push a snapshot of the current coordinates to the undo stack.
		 */
		void CreateFrameSnapShot();
		void ToggleBuilderPalette();
		WinPrefs *GetPrefs() {return Prefs;}
		bool JustFocused();
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

