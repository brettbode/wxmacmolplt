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

#ifdef __WXMAC__
#include <AGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "mpGLCanvas.h"
#include "MoleculeData.h"
#include "Prefs.h"
#include "Progress.h"
#include <deque>

/** This class holds all the window specific OpenGL data. It currently
	functions more like a struct. None of this data is saved to file.
*/
class OpenGLRec {
	friend class MpGLCanvas;
	public:
#ifndef __wxBuild__
		AGLContext aglContext;
		AGLPixelFormat	fmt;
		GLuint fontList;
#endif
		
		myGLTriangle *  transpTriList;		///< The list of transparent triangles
		float *			transpSortVertex;	///< The rotated screen Z coordinate to sort on.
		long *			transpIndex;		///< The sorted triangle list
		long			triangleCount;		///< The number of transparent triangles
		
		GLuint			MainDisplayList;	///< id for a main display list
		GLuint			SurfaceDisplayList;
		bool			MainListActive;
		bool			SurfaceListActive;
		bool			haveTransparentSurfaces;	///< Flag if there are transparent surfaces
		GLuint			length_anno_tex_id;			///< texture id for length annotations
		GLuint			sphere_list;
		
		OpenGLRec(void);
		~OpenGLRec(void);
};
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

/**
	Abstract class to hold snapshot data. All you can do with a snapshot is
	create it and restore it.
*/
class UndoSnapShot {
public:
	virtual ~UndoSnapShot(void) {};
	virtual void Restore(void) {};
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
	virtual ~FrameSnapShot(void);
	/** Restore the saved data. Note the frame point is stored here
		so you can't restore if the original frame memory has been
		destroyed.
	*/
	virtual void Restore(void);
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
	UndoData(void) : operationCount(0), position(0) {};
	~UndoData(void) {Clear();};
	///How many snapshots are saved.
	int GetOperationCount(void) const {return operationCount;};
	///Obtains the position in the snapshot list.
	int GetPosition(void) const {return position;};
	///Move the snapshot position manually (not normally needed).
	void SetPosition(int p) {if ((p>=0)&&(p<operationCount)) position = p;};
	///Is undo possible?
	bool undoPossible(void) const {return ((operationCount > 0)&&(position > 0));};
	///Is redo possible?
	bool redoPossible(void) const {return ((position+1) < operationCount);};
	///Remove all snapshots and free their memory.
	void Clear(void);
	///Add a snapshot to the stack.
	void AddSnapshot(UndoSnapShot *);
	///Undo to the previous snapshot.
	void UndoOperation(void);
	///Redo to the next snapshot.
	void RedoOperation(void);
private:
	std::deque<UndoSnapShot *>	UndoList;
	int	operationCount;
	int position;
};

//Simple class to save window visibility and positioning data
class WindowData {
public:
	WindowData(void);
	static void Validate(wxRect & t);
	
	void WriteXML(XMLElement * parent) const;
	void ReadXML(XMLElement * parent);
	
	bool BondsWindowVisible(void) const {return BondsVis;};
	void BondsWindowVisible(bool v) {BondsVis = v;};
	bool CoordsWindowVisible(void) const {return CoordsVis;};
	void CoordsWindowVisible(bool v) {CoordsVis = v;};
	bool EnergyWindowVisible(void) const {return EnergyVis;};
	void EnergyWindowVisible(bool v) {EnergyVis = v;};
	bool FreqWindowVisible(void) const {return FreqVis;};
	void FreqWindowVisible(bool v) {FreqVis = v;};
	bool SurfacesWindowVisible(void) const {return SurfacesVis;};
	void SurfacesWindowVisible(bool v) {SurfacesVis = v;};
	bool InputBWindowVisible(void) const {return InputBVis;};
	void InputBWindowVisible(bool v) {InputBVis = v;};
	bool PrefsWindowVisible(void) const {return PrefVis;};
	void PrefsWindowVisible(bool v) {PrefVis = v;};
	bool ZMatWindowVisible(void) const {return ZMatVis;};
	void ZMatWindowVisible(bool v) {ZMatVis = v;};
	
	const wxRect & GetMolWinRect(void) const {return MolWinRect;};
	void SetMolWinRect(const wxRect & v) {MolWinRect = v;};
	const wxRect & GetBondsWinRect(void) const {return BondsWinRect;};
	void SetBondsWinRect(const wxRect & v) {BondsWinRect = v;};
	const wxRect & GetCoordsWinRect(void) const {return CoordsWinRect;};
	void SetCoordsWinRect(const wxRect & v) {CoordsWinRect = v;};
	const wxRect & GetEnergyWinRect(void) const {return EnergyWinRect;};
	void SetEnergyWinRect(const wxRect & v) {EnergyWinRect = v;};
	const wxRect & GetFrequencyWinRect(void) const {return FreqWinRect;};
	void SetFrequencyWinRect(const wxRect & v) {FreqWinRect = v;};
	const wxRect & GetSurfacesWinRect(void) const {return SurfacesWinRect;};
	void SetSurfacesWinRect(const wxRect & v) {SurfacesWinRect = v;};
	const wxRect & GetInputBWinRect(void) const {return InputBuilderRect;};
	void SetInputBWinRect(const wxRect & v) {InputBuilderRect = v;};
	const wxRect & GetPrefsWinRect(void) const {return PreferenceWinRect;};
	void SetPrefsWinRect(const wxRect & v) {PreferenceWinRect = v;};
	const wxRect & GetZMatWinRect(void) const {return ZMatRect;};
	void SetZMatWinRect(const wxRect & v) {ZMatRect = v;};
private:
	wxRect	MolWinRect;
	wxRect	BondsWinRect;
	wxRect	CoordsWinRect;
	wxRect	EnergyWinRect;
	wxRect	FreqWinRect;
	wxRect	SurfacesWinRect;
	wxRect	InputBuilderRect;
	wxRect	PreferenceWinRect;
	wxRect	ZMatRect;
	bool	BondsVis, CoordsVis, EnergyVis, FreqVis, SurfacesVis, InputBVis, PrefVis, ZMatVis;
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
		
		Progress		*ProgressInd;    ///< Progress indicator window for long operations
		OpenGLRec		*OpenGLData;     ///< Extra OpenGL data
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
		InputBuilderWindow  *inputBuilderWindow;
		setPreference		*prefsDlg;
		ZMatrixCalculator	*zMatCalcDlg;
		
		WindowData			winData;
		
		wxPageSetupDialogData *pageSetupData;
		wxPrintData           *printData;

		void createMenuBar(void);
		long OpenGAMESSIRCLog(BufferFile * Buffer, long flip, float offset,
							  long NumOccAlpha, long NumOccBeta, long NumFragmentAtoms);

		bool mHighliteState;
		/* bool interactiveMode; */
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
		void menuEditRedo(wxCommandEvent &event);
		void OnUndoUpdate( wxUpdateUIEvent& event );
		void OnRedoUpdate( wxUpdateUIEvent& event );
		void menuEditCut(wxCommandEvent &event);
		void menuEditCopy(wxCommandEvent &event);
		void menuEditCopyCoordinates(wxCommandEvent &event);
		void CopyCoordinates(short coordtype) const;
		void menuEditPaste(wxCommandEvent &event);
		/// wxEVT_UPDATE_UI event handler for wxID_PASTE
		void OnPasteUpdate( wxUpdateUIEvent& event );
		void PasteText(void);
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
		void ClosePrefsWindow(void);

		void menuMoleculeSetBondLength(wxCommandEvent &event);
		void menuMoleculeSetFrameEnergy(wxCommandEvent &event);
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
		//void menuWindowMain_display(wxCommandEvent &event);
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
		 * \param event The event to handle.
		 */
		void KeyUpHandler(wxKeyEvent &event);
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
		/**
		 Open an MKL format filee. 
		 \param Buffer The buffer connected to the MKL file.
		 */
		long OpenMKLFile(BufferFile * Buffer);
		long ParseSIMMOMLogFile(BufferFile *Buffer, long EnergyPos);
		void ExportGAMESS(BufferFile * Buffer, bool AllFrames);
		/**
		 Output the plotted energies for the system to a tab de-limited text file. 
		 \param Buffer A buffer to write the text to.
		 \param AllFrames Should the data for all frames be included or just the current frame?
		 */
		void WriteTabbedEnergies(BufferFile * Buffer, bool AllFrames);
		/**
		 Output the frequencies for the current frame to a text file. 
		 \param Buffer A buffer to write the text to.
		 */
		void WriteFrequencies(BufferFile * Buffer);
		/**
		 Export the current system to an XYZ format file. 
		 \param Buffer A buffer to write the text to.
		 \param AllFrames Should the data for all frames be included or just the current frame?
		 \param AllModes Should the normal modes be output or just the current one?
		 \param AnimateMode Flag to produce a sequence of geometries animating the current normal mode.
		 */
		void WriteXYZFile(BufferFile * Buffer, bool AllFrames, bool AllModes, bool AnimateMode);
		/**
		 Export the current system to an MDL mol file. 
		 \param Buffer A buffer to write the text to.
		 */
		void WriteMDLMolFile(BufferFile * Buffer);
		void WriteVRMLFile(BufferFile * Buffer);
		void WritePOVFile(BufferFile *Buffer);
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
			\param updateCoordsWin	Should the coordinates window be updated (if open)?
			\param updateDisplay Should the main molecule display be updated?
		 */
		void AtomsChanged(bool updateCoordsWin=false, bool updateDisplay=false);
		/**
			Call when the bonds list is changed to update the bonds subwindow and the main display.
		 */
		void BondsChanged(void);
		/**
			Call to update surfaces (as needed) and the main display window. Generally this
			is called after the frame is changed or after a significant change to the frame data.
		 */
		void FrameChanged(void);
		void ChangeFrames(long NewFrame);
		/**
			Call to insert the selected atoms at the provided index.
			\param index The position in the atom list to insert the 1st selected atom.
		 */
		void ChangeAtomOrder(long index);
		void ModeChanged(void);
		void ChangeModes(long NewMode);
		void UpdateModelDisplay(void);
		MoleculeData * GetData(void) const {return MainData;};
		WinPrefs * GetPrefs(void) const {return Prefs;};
		void SetWindowPreferences(WinPrefs * NewPrefs);
		/* void ChangePrefs(WinPrefs * newPrefs); */
		void ResetView(void);
		void ResetModel(bool Center);
		void ResetAllWindows(void);
		void UpdateFrameText(void);
		void BeginOperation(void);
		void FinishOperation(void);
		bool OperInProgress(void) const {return OperationInProgress;};
		/**
		 * Updates the visibility and position information. This should
		 * be called before saving the state of the file.
		 */
		void UpdateWindowData(void);
		
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
		void DrawLabel(void);
		void SelectionChanged(bool mode);
		GLuint GetLengthTexId(void) const {return OpenGLData->length_anno_tex_id;};

		bool IsRotating(void);
		void eventSize(wxSizeEvent &event);
		void SizeChanged(void);
		void OnFrameAnimationTimer(wxTimerEvent & event);
		//Called during normal mode animations
		void OnModeAnimation(wxTimerEvent & event);
		//Function to be called when window loses focus to stop animations
		void OnMenuOpen(wxMenuEvent & event);
		void OnKillFocus(wxFocusEvent & event);
		void StopAnimations(void);
		void LassoStart(const int x, const int y);
		void LassoGrown(const int x, const int y);
		void LassoEnd(void);
		bool LassoContains(const int x, const int y);
		bool LassoHasArea(void);
		bool InSelectionMode(void);
		bool InViewMode(void);
		bool InEditMode(void);
		bool InSymmetryEditMode(void);
		void DrawBondingSites(long iatom, float radius, GLUquadricObj *qobj, int site_id=0, CPoint3D * vector=NULL);
		void SetStatusText(const wxString& label);
		/**
			Call to push a snapshot of the current coordinates to the undo stack.
		 */
		void CreateFrameSnapShot(void);
		void ToggleBuilderPalette();
		WinPrefs *GetPrefs(void) {return Prefs;}
		bool JustFocused(void);
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

