/////////////////////////////////////////////////////////////////////////////
// Name:        surfaceDlg.h
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri  2 Jun 10:08:18 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _SURFACE_DLG_H_
#define _SURFACE_DLG_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "surfaceDlg.cpp"
#endif

/*!
 * Includes
 */

////@begin includes
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/spinctrl.h>
#include <wx/htmllbox.h>
#include <vector>

#include "FloatSlider.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class colorArea;
class MoleculeData;
class WinPrefs;

class Surface;
class Surf2DBase;
class Surf3DBase;
class Orb2DSurface;
class Orb3DSurface;
class General3DSurface;
class General2DSurface;
class TEDensity2DSurface;
class TEDensity3DSurface;
class MEP2DSurface;
class MEP3DSurface;
class OrbSurfBase;
class SurfacesWindow;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_MYDIALOG4 10070
#define SYMBOL_ORBITAL3D_STYLE wxSUNKEN_BORDER
#define SYMBOL_ORBITAL3D_TITLE _("3D Orbital")
#define SYMBOL_ORBITAL3D_IDNAME ID_MYDIALOG4
#define SYMBOL_ORBITAL3D_SIZE wxSize(400, 300)
#define SYMBOL_ORBITAL3D_POSITION wxDefaultPosition

#define SYMBOL_ORBITAL2D_STYLE wxSUNKEN_BORDER
#define SYMBOL_ORBITAL2D_TITLE _("2D Orbital")
#define SYMBOL_ORBITAL2D_IDNAME ID_MYDIALOG4
#define SYMBOL_ORBITAL2D_SIZE wxSize(400, 300)
#define SYMBOL_ORBITAL2D_POSITION wxDefaultPosition

#define SYMBOL_TED2D_STYLE wxSUNKEN_BORDER
#define SYMBOL_TED2D_TITLE _("2D Total Electron Density")
#define SYMBOL_TED2D_IDNAME ID_MYDIALOG4
#define SYMBOL_TED2D_SIZE wxSize(400, 300)
#define SYMBOL_TED2D_POSITION wxDefaultPosition

#define SYMBOL_TED1D_STYLE wxSUNKEN_BORDER
#define SYMBOL_TED1D_TITLE _("1D Total Electron Density")
#define SYMBOL_TED1D_IDNAME ID_MYDIALOG4
#define SYMBOL_TED1D_SIZE wxSize(400, 300)
#define SYMBOL_TED1D_POSITION wxDefaultPosition

#define SYMBOL_PARAM_SIZE wxSize(400, 300)
#define SYMBOL_PARAM_POSITION wxDefaultPosition

#define ID_2D_PARAM_DIALOG 10071
#define ID_3D_PARAM_DIALOG 10072
#define SYMBOL_2D_PARAM_TITLE _("2D Manual Parameter Adjustment")
#define SYMBOL_3D_PARAM_TITLE _("3D Manual Parameter Adjustment")

#define ID_ORB_CHOICE 10074
#define ID_GRID_POINT_SLIDER 10075
#define ID_SET_PARAM_BUT 10076
#define ID_SURFACE_EXPORT_BUT 10077
#define ID_SURFACE_UPDATE_BUT 10079
#define ID_GRID_SIZE_SLIDER 10078
#define ID_CONTOUR_VALUE_SLIDER 10080
#define ID_3D_RADIOBOX 10081
#define ID_SMOOTH_CHECKBOX 10082
#define ID_FREE_MEM_BUT 10083
#define ID_ATOM_LIST 10084
#define ID_TRANSPARENCY 10085
#define ID_REVERSE_PHASE_CHECKBOX 10086
#define ID_ORB_FORMAT_CHOICE 10087
#define ID_2D_COLOR_POSITIVE 10088
#define ID_2D_COLOR_NEGATIVE 10089
#define ID_3D_COLOR_POSITIVE 10090
#define ID_3D_COLOR_NEGATIVE 10091
#define ID_SPH_HARMONICS_CHECKBOX 10092
#define ID_CONTOUR_VALUE_EDIT 10093
#define ID_COPY_ALL 10094
#define ID_PASTE_ALL 10095
#define ID_SHOW_ZERO_CHECKBOX 10096
#define ID_DASH_CHECKBOX 10097
#define ID_SET_PLANE_BUT 10098
#define ID_USE_PLANE_CHECKBOX 10099
#define ID_NUM_CONTOUR_TEXT 10100
#define ID_MAX_CONTOUR_VALUE_TEXT 10101
#define ID_ORB_COEF 10102
#define ID_GENFILEBUTTON 10103
#define ID_GENMULTCHECK 10104
#define ID_GENMULTEDIT 10105
#define ID_GENSQUARECHECK 10106
#define ID_GENCONPOSNEGCHECK 10107
#define ID_GENSURFGRIDMINTEXT 10108
#define ID_GRID_MAX_TEXT 10109
#define ID_3D_COLOR_SURF_CHECK 10110
#define ID_USERGB_COLOR_CHECK 10111
#define ID_3D_MAX_MAP_EDIT 10112
#define ID_SCALE_EDIT 10113
#define ID_DISPLAY_PLANE_CHECKBOX 10244
#define ID_INVERT_RGB_CHECK 10245
#define ID_ENDPT1_XBOX 10246
#define ID_ENDPT1_YBOX 10247
#define ID_ENDPT1_ZBOX 10248
#define ID_ENDPT2_XBOX 10246
#define ID_ENDPT2_YBOX 10247
#define ID_ENDPT2_ZBOX 10248

////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif


/*!
 * The FormattedListBox class is used to display the AO and MO lists and allows things like 
 * super and sub scripts and item colorization. The wx class is abstract so this is a thin wrapper
 * that tracks the items and provides the OnGetItem call that is needed by the base class.
 */
class FormattedListBox : public wxHtmlListBox {
	public:
		FormattedListBox()
			: wxHtmlListBox() {
		}

		FormattedListBox(wxWindow *parent, int id, int style = 0)
			: wxHtmlListBox(parent, id, wxDefaultPosition, wxDefaultSize,
							style) {
			
			SetMargins(0, 0);
#if 0
			SetBackgroundColour(*wxBLUE);
			SetSelectionBackground(*wxRED);
#endif
		}

		/// Add the specified item to the end of the list.
		void Append(const wxString& item) {
			items.push_back(item);
			// This was causing too many refreshes for some reason.
			// Call DoneAppending instead.
			/* SetItemCount(items.size()); */
		}

		/// Call when changes to the list are completed and you want the list refreshed.
		void DoneAppending() {
			SetItemCount(items.size());
		}

		/** OnGetItem is required by the wxHtmlListBox base class to provide the base class with the text for
		 * each row in the table as needed.
		 * @param n The row for which to return the formatted text string
		 * @return The formatted string for the requested row.
		 */
		wxString OnGetItem(size_t n) const {
			return items[n];
		}

		/// Remove all items in the list and clear the display.
		void Clear() {
			wxHtmlListBox::Clear();
			items.clear();
			SetItemCount(0);
			RefreshAll();
		}

		void OnResize(wxSizeEvent& event) {
			Refresh(false);
			event.Skip();
		}

	private:
		/// items contains the formatted text for each row in the table
		std::vector<wxString> items;

	/* DECLARE_DYNAMIC_CLASS(FormattedListBox) */
	/* DECLARE_NO_COPY_CLASS(FormattedListBox) */
	DECLARE_EVENT_TABLE()
};

/*!
 * The base class for all the surface types for the Surfaces Window ListBook.
 * This class contains and manages the controls common to all surfaces such
 * as title, visibility, update button, etc.
 */
class BaseSurfacePane : public wxPanel
{
	/* DECLARE_CLASS(BaseSurfacePane) */

public:

	BaseSurfacePane() {}
	BaseSurfacePane(wxWindow* parent, Surface * target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE);
	~BaseSurfacePane();

	bool Create(wxWindow* parent, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE);

	virtual void TargetToPane() = 0;	///< Call to update the display with new data in the target surface.
	virtual void refreshControls() = 0;	///< Updates the control state, call when data changes.
	virtual bool UpdateNeeded(void) = 0;	///< Should the update button be activated?

	void SetVisibility(bool state);	///< Toggle the surface visibility.

	void OnCheckAllFrames(wxCommandEvent& event);
	void OnCheckVisible(wxCommandEvent& event);
	void OnChangeTitle(wxCommandEvent& event);

	void setAllFrames(bool state);	///< Should the surface be applied to all frames?

	bool GetVisibility(void) const {return Visible;};	///< Is the surface visible?
	bool GetAllFrames(void) const {return AllFrames;};	///< Is the surface applies to all frames?

	const wxString GetSurfaceName() const;

	void SetUpdateTest(bool test);	///< Set flag to force an update (overrides the automatic tests).
	void setUpdateButton();			///< Updates the state of the update button comparing the current and saved states.
	/**
		Call when the page becomes the active page. Right now this just makes sure the
		update button is the default button.
	 */
	void PageIsNowActive(void);
	Surface * GetTargetSurface(void) const {return mTarget;};	///< Return a pointer to the surface data.

	void OnGridPointSld(wxCommandEvent &event);	///< Call back when the grid points slider is changed.
	/**
	 * Call back for the Set Param button.
	 * Displays the appropriate surface parameters dialog.
	 */
	void OnSetParam(wxCommandEvent &event);
	/**
		Call back for the Export button.
		Prompts the user for a file name, then exports the surface to that file.
	 */
	void OnExport(wxCommandEvent &event);
	/// Builds the menu of choices from the orbitals available for the current frame.
	void BuildOrbSetPopup(void);
	/// Called when the user changes the targeted orbital set.
	void OnOrbSetChoice(wxCommandEvent &event);

protected:
	wxBoxSizer* mainSizer;	///< A primary BoxSizer for the page.

	wxSlider* mNumGridPntSld;	///< A slider representing the number of grid points
	wxButton* mSetParamBut;		///< Button to call the manual set parameters dialog
	wxButton* mExportBut;		///< Button to export the surface to a file
	wxButton* mUpdateBut;		///< Button to refresh the surface display, recomputing as needed.

	bool Visible;			///< Should the surface be visible?
	bool AllFrames;			///< flag to apply the surface to all frames.
	bool UpdateTest;		///< flag to force an update.
	long NumGridPoints;		///< The number of grid points to be computed for the surface
	bool SwitchFixGrid;		///< Indicates the grid parameters have changed so the grid must be recomputed.
	long TargetOrbSet;		///< Which set of orbitals should be used?
	wxChoice* mOrbSetChoice;	///< Popup menu allowing the using to select the target orbital set.
	wxTextCtrl* surfTitleEdit;	///< Edit Text field for the title of the surface.
	wxCheckBox* visibleCheck;	///< Check box allowing the user to hide the surface without deleting it.
	wxCheckBox* allFrameCheck;	///< CheckBox causing the surface to be applied to all frames in the file.

	SurfacesWindow * owner;	///< Our parent window.

 private:
	void CreateControls();	///< Create the controls (currently just the main sizer).

	Surface* mTarget;	///< Pointer to the target surface.

    DECLARE_EVENT_TABLE()
};

/*!
 * A utility class handling items common to orbital surfaces.
 */
class OrbSurfacePane {
	public:
		OrbSurfacePane() {}
		OrbSurfacePane(OrbSurfBase* target, SurfacesWindow* owner);
		~OrbSurfacePane();

	protected:
		bool UpdateEvent(); 

		/// Build the molecular orbital list for the current set of orbitals
		void makeMOList();
		/// Build the atomic orbital list including the currently selected MO vector (if any)
		void makeAOList();
		/** Build the item list for the orbital set selector. This is different from the similar
		 * base class function in that for orbitals we allow separately selecting alpha and beta spin orbitals.
		 */
		int getOrbSetForOrbPane(std::vector<wxString>& choice);
		int orbSetChangeEvt(int item, SurfacesWindow * owner);
		/// Toggles the MO list between displaying orbital energies (0) and occupation numbers (non zero)
		void setFlagOnOrbFormatChange(int itemtype);
		/// Called when viewing AOs and the user makes a selection
		void OnAtomicOrbitalChoice(wxCommandEvent& event);

		long TargetSet;					///< The targeted orbital set
		long OrbOptions;				///< Modifies the targeted orbital set to choose alpha/beta orbitals
		long OrbColumnEnergyOrOccupation;	///< Selects orbital energy or occupation number display in the MO list
		long SphericalHarmonics;		///< Should spherical harmonic AOs be used
		long PlotOrb;					///< The selected orbital (can be either MO or AO)
		bool PhaseChange;				///< Flip the phase of the orbital
		bool coefIsEnabled;				///< True if targeting AOs (I think)
		FormattedListBox *mMOList;		///< The MO list
		FormattedListBox *mOrbCoef;		///< The AO list

	private:
		OrbSurfBase* mTarget;
		SurfacesWindow * myowner;
};

/*!
 * A utility class handling items common to one dimensional surfaces.
 */
class Surface1DPane : public BaseSurfacePane {

	public:
		Surface1DPane() {}
		Surface1DPane(wxWindow* parent, Surf1DBase* target, SurfacesWindow* Owner, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);
		~Surface1DPane();

		/// Event handler for a change in the positive color well
		void OnPosColorChange(wxCommandEvent & event);
		/// Event handler for a change in the negative color well
		void OnNegColorChange(wxCommandEvent & event);
		void OnChangeEndpoint(wxCommandEvent &event);
		void OnIdle(wxIdleEvent& WXUNUSED(event));
		void OnChangeClamp(wxCommandEvent &event);
		void OnChangeScale(wxCommandEvent &event);

	protected:

		colorArea* mOrbColor1;
		colorArea* mOrbColor2;
		RGBColor PosColor;
		RGBColor NegColor;
		wxTextCtrl *endpt1_xbox;
		wxTextCtrl *endpt1_ybox;
		wxTextCtrl *endpt1_zbox;
		wxTextCtrl *endpt2_xbox;
		wxTextCtrl *endpt2_ybox;
		wxTextCtrl *endpt2_zbox;
		wxTextCtrl *clamp_box;
		wxTextCtrl *scale_box;
		wxString endpt1_xstr;
		wxString endpt1_ystr;
		wxString endpt1_zstr;
		wxString endpt2_xstr;
		wxString endpt2_ystr;
		wxString endpt2_zstr;

		CPoint3D Start, End;
		float MaxContourValue;
		float Scale;

	private:

		Surf1DBase* mTarget;

	DECLARE_EVENT_TABLE()
};

/*!
 * A utility class handling items common to two dimensional surfaces.
 */
class Surface2DPane : public BaseSurfacePane {
  /* DECLARE_CLASS(Surface2DPane) */

 public:
	Surface2DPane() {}
	Surface2DPane(wxWindow* parent, Surf2DBase* target, SurfacesWindow* Owner, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);
	~Surface2DPane();

	/// Event handler for a change in the positive color well
	void OnPosColorChange(wxCommandEvent & event);
	/// Event handler for a change in the negative color well
	void OnNegColorChange(wxCommandEvent & event);
	/// Event handler for a change in the show zero contour check box
	void OnShowZeroChk(wxCommandEvent &event);
	/// Event handler for a change in the dash line check box
	void OnDashChk(wxCommandEvent& event);
	/// Event handler for a change in the use plane of screen check box
	void OnUsePlaneChk(wxCommandEvent &event);
	/// Event handler for a change in the show plotting plane check box.
	void OnDisplayPlaneChk(wxCommandEvent &event);
	/// Event handler for a change in the maximum contour value edit field
	void OnContourValueText(wxCommandEvent &event);
	/// Outputs the MaxContourValue variable to the Max value to contour edit field
	void SetContourValueText(void);
	/// Event handler for a change in the number of counters edit field
	void OnNumContoursText(wxCommandEvent &event);
	/// Outputs the NumContours variable to the corresponding edit field
	void SetNumContoursText(void);
	/// Grabs the current state of edit fields and sets the update button if needed
	void OnIdle(wxIdleEvent& WXUNUSED(event));
	/// Event handler for the Set plane button. Opens the set plane dialog.
	void OnSetPlane(wxCommandEvent &event);
	/// Event handler for the Set Paramters button. Opens the manual plane setting dialog.
	void OnSetParam(wxCommandEvent &event);

 protected:
	wxTextCtrl* mNumContourText;		///< Edit control for the number of contours
	wxTextCtrl* mContourValText;		///< Edit control for the maximum value to contour
	wxCheckBox* mShowZeroCheck;			///< Show zero contour check box
	wxCheckBox* mDashCheck;				///< Dash negative contours check box
	wxCheckBox* mUsePlaneChk;			///< Use plane of screen as plotting plane check box
	wxCheckBox* mDisplayPlaneCheck;		///< Show plotting plane check box
	colorArea* mOrbColor1;				///< Control for setting the positive color
	colorArea* mOrbColor2;				///< Control for setting the negative color
	RGBColor PosColor;					///< local copy of positive color (pushed to the surface upon update)
	RGBColor NegColor;					///< local copy of negative color (pushed to the surface upon update)

	long NumContours;					///< local copy of number of contours (pushed to the surface upon update)
	float MaxContourValue;				///< local copy of Max value to contour (pushed to the surface upon update)
	bool ShowZeroContour;				///< local copy of show zero contour (pushed to the surface upon update)
	bool DashLines;						///< local copy of dash - lines (pushed to the surface upon update)
	bool UseScreenPlane;				///< local copy of use screen plane (pushed to the surface upon update)
	bool DisplayPlane;					///< local copy of show plotting plane (pushed to the surface upon update)

 private:

  Surf2DBase* mTarget;					///< The corresponding target surface

  DECLARE_EVENT_TABLE()
};


class Surface3DPane : public BaseSurfacePane {
	/* DECLARE_CLASS(Surface3DPane) */

public:
	Surface3DPane() {}

	Surface3DPane(wxWindow* parent, Surf3DBase* target, SurfacesWindow* Owner, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);
	~Surface3DPane();

	/// Event handler for the wire frame/solid surface radio buttons
	void On3DRadioBox(wxCommandEvent& event);
	/// Event handler for the smooth surface check box
	void OnSmoothCheck(wxCommandEvent& event);
	/// Event handler for the contour value edit field
	void OnContourValueEnter(wxCommandEvent& event);
	/// Pure virtual function for the surface update function
	virtual void OnUpdate(wxCommandEvent& event) = 0;
	/// Event handler for when the free memory button is pressed. Causes the grid to be released.
	void OnFreeMem(wxCommandEvent& event);
	/// Event handler for when the positive color is changed.
	void OnPosColorChange(wxCommandEvent & event);
	/// Event handler for when the negative color is changed.
	void OnNegColorChange(wxCommandEvent & event);
	/// Event handler for when the transparency color is changed.
	void OnTransparencyChange(wxSpinEvent & event);
	/// Call back that checks edit field values and sets update button if needed
	void OnIdle(wxIdleEvent& WXUNUSED(event));
	/// Called when the set parameters button is pushed to manually set the grid settings.
	void OnSetParam(wxCommandEvent &event);
	/// Called after the Grid Size slider is changed.
	void OnGridSizeSld(wxCommandEvent &event);
	/// Copy the class contour value to the slider control
	void setContourValueSld();
	/// Event handler for a change in the contour value slider.
	void OnContourValueSld(wxCommandEvent &event);
	/// Copies the class value for the maximum grid value to the contour value slider control
	void SetContourMaxValueText(void);
	/// Activate/setup MEP controls based on whether a MEP is possible for the current target orbital set.
	void refreshMEPControls();
	/// Call back for a change in the check box indicating the surface should be colored based on the MEP value.
	void OnUseMEPCheck(wxCommandEvent &event);
	/// Event handler for the check box indicating the use of an RGB color scheme versus bi-color scheme.
	void OnRGBColorCheck(wxCommandEvent &event);
	/// Event handler for the check box indicating that the color scheme should be inverted.
	void OnInvertRGBCheck(wxCommandEvent &event);
	/// Event handler for a change in the maximum MEP value to colorize edit
	void OnMaxMEPValueText(wxCommandEvent &event);
	/// Push the Max MEP value to the edit field
	void SetMaxMEPValueText();

protected:
	wxSlider* mGridSizeSld;			///< The slider representing the grid volume muliplier
	FloatSlider* mContourValSld;	///< The slider/edit field for setting the isosurface value
	colorArea* mOrbColor1;			///< The color picker control for color 1 (positive)
	colorArea* mOrbColor2;			///< The color picker control for color 2 (negative)
	wxSpinCtrl *mTransparency;		///< Spin control for setting the percent transparency
	wxRadioBox* m3DRdoBox;			///< The radio controls for setting wireframe versus solid mode
	wxCheckBox* mSmoothChkBox;		///< Check box to trigger smoothing the surface with vertex normals
	wxButton* mFreeMemBut;			///< Button to cause the grid to be freed up

	float GridSize;					///< Local temp copy of the grid volume size multiplier
	RGBColor PosColor;				///< Local temp copy of color 1 (positive)
	RGBColor NegColor;				///< Local temp copy of color 2 (negative)
	int Transparency;				///< Local temp copy of the percent transparency
	bool UseNormals;				///< Local temp copy of the flag for vertex normals
	bool UseSolidSurface;			///< Local temp flag for picking wireframe versus a solid surface
	float ContourValue;				///< Local temp copy of the isosurface value

	wxCheckBox*	mColorSurfCheck;	///< Check box control for the option to colorize the surface by the MEP value
	wxCheckBox*	mUseRGBColorCheck;	///< Check box for the MEP colorize by RGB or linear color space
	wxCheckBox*	mInvertRGBCheck;	///< Check box to invert the MEP color mapping
	wxTextCtrl*	mMaxMapEdit;		///< Cutoff for the MEP color mapping
	bool UseMEP;					///< local temp copy of the colorize by MEP flag
	bool UseRGBSurfaceColor;		///< local temp copy of the MEP colorization choice (RGB or linear)
	bool InvertRGBSurfaceColor;		///< local temp copy of the invert MEP color map
	float MaxMEPValue;				///< local temp copy of the MEP colorization value cutoff

private:
	/// Called when there is a change to the contour value via the edit field portion of the slider control
	void changeContourValue();

	Surf3DBase* mTarget;

	DECLARE_EVENT_TABLE()
}; 

class Orbital2DSurfPane : public Surface2DPane, public OrbSurfacePane {    
  /* DECLARE_CLASS(Orbital2DSurfPane) */

public:
	/// Constructors
	Orbital2DSurfPane() { }
	Orbital2DSurfPane(wxWindow* parent, Orb2DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL2D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL2D_POSITION, const wxSize& size = SYMBOL_ORBITAL2D_SIZE, long style = SYMBOL_ORBITAL2D_STYLE);
	~Orbital2DSurfPane();

	/// Copy the data from the target surface to the temporary variables for this window
	virtual void TargetToPane();
	/// Update the controls after changes to the data (such as after a surface update)
	virtual void refreshControls();

	/// Called upon a change to the orbital set selector
	void OnOrbSetChoice(wxCommandEvent &event);
	/// Called when a selection change is made in the molecular orbital list
	void OnMOList(wxCommandEvent &event);
	/// Called when a change is made to the selector for showing orbital energies or occupations
	void OnOrbFormatChoice(wxCommandEvent &event);
	/// Called when the reverse orbital phase check box is changed
	void OnReversePhase(wxCommandEvent &event);
	/// Called when the spherical harmonics check box is changed
	void OnSphHarmonicChk(wxCommandEvent &event);
	/// Called when a selection is changed in the atomic orbital list
	void OnCoefList(wxCommandEvent &event);

private:
	/// Routine tests the current window data against the target surface to determine if an update is needed
	virtual bool UpdateNeeded(void);
	/// Routine creates all the controls
	void CreateControls();
	/// Called when the update button is pressed. Processes all actions needed to update the target surface.
	void OnUpdate(wxCommandEvent &event);

	wxChoice* mOrbFormatChoice;			///< Choice control for orbital occupancy versus energies
	wxCheckBox* mSphHarmonicsChk;		///< Spherical harmonics check box
	wxCheckBox* mRevPhaseChk;			///< Reverse orbital phase checkbox

	Orb2DSurface* mTarget;				///< The target surface

	DECLARE_EVENT_TABLE()
};

class Orbital3DSurfPane : public Surface3DPane, public OrbSurfacePane
{    
  /* DECLARE_CLASS(Orbital3DSurfPane) */

public:
	/// Constructors
	Orbital3DSurfPane() { }
	Orbital3DSurfPane(wxWindow* parent, Orb3DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE);
	~Orbital3DSurfPane();

	/// Copy the data from the target surface to the temporary variables for this window
	virtual void TargetToPane();
	/// Update the controls after changes to the data (such as after a surface update)
	virtual void refreshControls();

	/// Creates the controls and sizers
	void CreateControls();

	/// Called when a change is made to the selector for showing orbital energies or occupations
	void OnOrbFormatChoice(wxCommandEvent &event);
	/// Called when a selection change is made in the molecular orbital list
	void OnMOList(wxCommandEvent &event);
	/// Called upon a change to the orbital set selector
	void OnOrbSetChoice(wxCommandEvent &event);
	/// Called when the spherical harmonics check box is changed
	void OnSphHarmonicChk(wxCommandEvent &event);
	/// Called when the reverse orbital phase check box is changed
	void OnReversePhase(wxCommandEvent &event);
	/// Called when there is a change to the contour value slider
	void OnContourValueSld(wxCommandEvent &event);
	/// Called when there is a change to the Grid slize slider
	void OnGridSizeSld(wxCommandEvent &event);
	/// Called when the update button is pressed. Processes all actions needed to update the target surface.
	void OnUpdate(wxCommandEvent &event);
	/// Called when a selection is changed in the atomic orbital list
	void OnCoefList(wxCommandEvent &event);

	////@begin Orbital3D member function declarations

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource(const wxString& name);

	/// Retrieves icon resources
	wxIcon GetIconResource(const wxString& name);
	////@end Orbital3D member function declarations

private:
	/// Routine tests the current window data against the target surface to determine if an update is needed
	virtual bool UpdateNeeded(void);

	wxChoice* mOrbFormatChoice;			///< Choice control for orbital occupancy versus energies
	wxCheckBox* mSphHarmonicsChk;		///< Spherical harmonics check box
	wxCheckBox* mRevPhaseChk;			///< Reverse orbital phase checkbox

	Orb3DSurface* mTarget;				///< The target surface

	DECLARE_EVENT_TABLE()
};

class General3DSurfPane : public Surface3DPane
{    
	/* DECLARE_CLASS(General3DSurfPane) */
	
public:
	/// Constructors
	General3DSurfPane() { }
	General3DSurfPane(wxWindow* parent, General3DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE);
	~General3DSurfPane();

	/// Copy the data from the target surface to the temporary variables for this window
	virtual void TargetToPane();
	/// Update the controls after changes to the data (such as after a surface update)
	virtual void refreshControls();

	/// Creates the controls and sizers
	void CreateControls();

	/// Retrieves bitmap resources
	wxBitmap GetBitmapResource(const wxString& name);

	/// Retrieves icon resources
	wxIcon GetIconResource(const wxString& name);

private:
	/// Do the displayed values differ from the saved values.
	virtual bool UpdateNeeded(void);
	/// Call back for the check box activating the multiplication function
	void OnMultCheck(wxCommandEvent &event);
	/// Call back for the check box requesting values be squared as read in
	void OnSquareCheck(wxCommandEvent &event);
	/// Call back for check box activating positive and negative contours with the same magnitude
	void OnContourPosNegCheck(wxCommandEvent &event);
	/// Call back function for when the file button is pressed.

	/// Prompts the user for the file and then attempts to read it in.
	void OnFileButton(wxCommandEvent &event);
	/// Call back function for when the Update button is pressed.
	void OnUpdate(wxCommandEvent &event);
	/// Call back function for changes in the counter value slider.
	void OnContourValueSld(wxCommandEvent &event);
	/// Call back function for when the text field is changed.
	void OnMultValueEdit(wxCommandEvent& event);

	wxCheckBox* mMultCheck;					///< Check box control activating multiplier scale factor
	wxTextCtrl* mGenMultValue;				///< The edit control for the multiplier
	wxCheckBox* mSquareCheck;				///< Check box for squaring the values
	wxCheckBox* mGenContourPosNegCheck;		///< Check box for producing positive and negative contours

	General3DSurface*	mTarget;			///< The target surface
	bool				ContourPosNeg;		///< Should both positive and negative contours of the same magnitude be produced
	bool				useMultValue;		///< Should the values be multiplied by MultValue?
	bool				squareValues;		///< Should the values be squared as they are read in?
	float				MultValue;			///< The value scale factor, if active

	DECLARE_EVENT_TABLE()
};

class General2DSurfPane : public Surface2DPane
{    
	/* DECLARE_CLASS(General3DSurfPane) */
	
public:
    /// Constructors
    General2DSurfPane() { }
    General2DSurfPane(wxWindow* parent, General2DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE);
    ~General2DSurfPane();
	
	/// Copy the data from the target surface to the temporary variables for this window
    virtual void TargetToPane();
	/// Update the controls after changes to the data (such as after a surface update)
    virtual void refreshControls();
	
    /// Creates the controls and sizers
    void CreateControls();
	
    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource(const wxString& name);
	
    /// Retrieves icon resources
    wxIcon GetIconResource(const wxString& name);
	
    /// Should we show tooltips?
    static bool ShowToolTips() {return true;};
	
private:
	/// Do the displayed values differ from the saved values.
	virtual bool UpdateNeeded(void);
	/// Call back for the check box activating the multiplication function
	void OnMultCheck(wxCommandEvent &event);
	/// Call back for the check box requesting values be squared as read in
	void OnSquareCheck(wxCommandEvent &event);
	/// Prompts the user for the file and then attempts to read it in.
	void OnFileButton(wxCommandEvent &event);
	/// Call back function for when the Update button is pressed.
	void OnUpdate(wxCommandEvent &event);
	/// Call back function for when the text field is changed.
	void OnMultValueEnter(wxCommandEvent& event);
	/// Pushes the local variable to the control
	void SetMultValue(void);
	
	wxCheckBox* mMultCheck;				///< Check box control activating multiplier scale factor
	wxTextCtrl* mGenMultValue;			///< The edit control for the multiplier
	wxCheckBox* mSquareCheck;			///< Check box for squaring the values

	General2DSurface*	mTarget;		///< The target surface
	bool				useMultValue;	///< Should the values be multiplied by MultValue?
	bool				squareValues;	///< Should the values be squared as they are read in?
	float				MultValue;		///< The value scale factor, if active
	
    DECLARE_EVENT_TABLE()
};

class TEDensity1DSurfPane : public Surface1DPane
{    
	
public:
    /// Constructors
    TEDensity1DSurfPane() { }
    TEDensity1DSurfPane(wxWindow* parent, TEDensity1DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_TED1D_IDNAME, const wxPoint& pos = SYMBOL_TED1D_POSITION, const wxSize& size = SYMBOL_TED1D_SIZE, long style = SYMBOL_TED1D_STYLE);
    ~TEDensity1DSurfPane();
	
	/// Copy the data from the target surface to the temporary variables for this window
    virtual void TargetToPane();
	/// Update the controls after changes to the data (such as after a surface update)
    virtual void refreshControls();
	
    /// Creates the controls and sizers
    void CreateControls();
	
    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource(const wxString& name);
	
    /// Retrieves icon resources
    wxIcon GetIconResource(const wxString& name);
	
    /// Should we show tooltips?
    static bool ShowToolTips() {return true;};
	
private:
	/// Do the displayed values differ from the saved values.
	virtual bool UpdateNeeded(void);
	/// Call back function for when the Update button is pressed. Takes care of all needed computation, etc.
	void OnUpdate(wxCommandEvent &event);

	TEDensity1DSurface*	mTarget;		///< The target surface
	
    DECLARE_EVENT_TABLE()
};

class TEDensity2DSurfPane : public Surface2DPane
{    
	/* DECLARE_CLASS(TEDensity2DSurfPane) */
	
public:
    /// Constructors
    TEDensity2DSurfPane() { }
    TEDensity2DSurfPane(wxWindow* parent, TEDensity2DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_TED2D_IDNAME, const wxPoint& pos = SYMBOL_TED2D_POSITION, const wxSize& size = SYMBOL_TED2D_SIZE, long style = SYMBOL_TED2D_STYLE);
    ~TEDensity2DSurfPane();
	
	/// Copy the data from the target surface to the temporary variables for this window
    virtual void TargetToPane();
	/// Update the controls after changes to the data (such as after a surface update)
    virtual void refreshControls();
	
    /// Creates the controls and sizers
    void CreateControls();
	
    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource(const wxString& name);
	
    /// Retrieves icon resources
    wxIcon GetIconResource(const wxString& name);
	
    /// Should we show tooltips?
    static bool ShowToolTips() {return true;};
	
private:
	/// Do the displayed values differ from the saved values.
	virtual bool UpdateNeeded(void);
	/// Call back function for when the Update button is pressed. Takes care of all needed computation, etc.
	void OnUpdate(wxCommandEvent &event);

	TEDensity2DSurface*	mTarget;		///< The target surface
	
    DECLARE_EVENT_TABLE()
};
class TEDensity3DSurfPane : public Surface3DPane
{    
	/* DECLARE_CLASS(TEDensity3DSurfPane) */

	public:
		/// Constructors
		TEDensity3DSurfPane() { }
		TEDensity3DSurfPane(wxWindow* parent, TEDensity3DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE);
		~TEDensity3DSurfPane();

		/// Copy the data from the target surface to the temporary variables for this window
		virtual void TargetToPane();
		/// Update the controls after changes to the data (such as after a surface update)
		virtual void refreshControls();

		/// Creates the controls and sizers
		void CreateControls();

		/// Retrieves bitmap resources
		wxBitmap GetBitmapResource(const wxString& name);

		/// Retrieves icon resources
		wxIcon GetIconResource(const wxString& name);

		/// Should we show tooltips?
		static bool ShowToolTips() {return true;};

	private:
		/// Do the displayed values differ from the saved values.
		virtual bool UpdateNeeded(void);
		/// Call back function for when the Update button is pressed. Takes care of all needed computation, etc.
		void OnUpdate(wxCommandEvent &event);

		TEDensity3DSurface*	mTarget;		///< The target surface

	DECLARE_EVENT_TABLE()
};
class MEP2DSurfPane : public Surface2DPane
{    
	/* DECLARE_CLASS(MEP2DSurfPane) */
	
public:
    /// Constructors
    MEP2DSurfPane() { }
    MEP2DSurfPane(wxWindow* parent, MEP2DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE);
    ~MEP2DSurfPane();
	
	/// Copy the data from the target surface to the temporary variables for this window
	virtual void TargetToPane();
	/// Update the controls after changes to the data (such as after a surface update)
    virtual void refreshControls();
	
    /// Creates the controls and sizers
    void CreateControls();
	
    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource(const wxString& name);
	
    /// Retrieves icon resources
    wxIcon GetIconResource(const wxString& name);
	
    /// Should we show tooltips?
    static bool ShowToolTips() {return true;};
	
private:
	/// Do the displayed values differ from the saved values.
	virtual bool UpdateNeeded(void);
	/// Call back function for when the Update button is pressed. Takes care of all needed computation, etc.
	void OnUpdate(wxCommandEvent &event);
	
    MEP2DSurface*	mTarget;		///< The target surface
	
    DECLARE_EVENT_TABLE()
};
class MEP3DSurfPane : public Surface3DPane
{    
	/* DECLARE_CLASS(MEP3DSurfPane) */
	
public:
    /// Constructors
    MEP3DSurfPane() { }
    MEP3DSurfPane(wxWindow* parent, MEP3DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE);
    ~MEP3DSurfPane();
	
	/// Copy the data from the target surface to the temporary variables for this window
    virtual void TargetToPane();
	/// Update the controls after changes to the data (such as after a surface update)
    virtual void refreshControls();
	
    /// Creates the controls and sizers
    void CreateControls();
	
    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource(const wxString& name);
	
    /// Retrieves icon resources
    wxIcon GetIconResource(const wxString& name);
	
    /// Should we show tooltips?
    static bool ShowToolTips() {return true;};
	
private:
	/// Do the displayed values differ from the saved values.
	virtual bool UpdateNeeded(void);
	/// Call back function for when the Update button is pressed. Takes care of all needed computation, etc.
	void OnUpdate(wxCommandEvent &event);

    MEP3DSurface*	mTarget;		///< The target surface
	
    DECLARE_EVENT_TABLE()
};

/**
 * A dialog to allow the user to manually setup the grid.
 * Allows the user to directly setup the grid including the number of grid points,
 * the origin and the two vectors defining the plane. The dialog presents the current
 * grid allowing the user to copy all of the parameters to paste into a different surface.
 * While this dialog does maintain a connect to it's parent it is likely best to be run as a modal
 * dialog.
 */
class Surface2DParamDlg : public wxDialog
{
  /* DECLARE_CLASS(Surface2DParamDlg); */

public:
	Surface2DParamDlg(BaseSurfacePane * parent, Surf2DBase * targetSurface, wxWindowID id = ID_2D_PARAM_DIALOG, const wxString& caption = SYMBOL_2D_PARAM_TITLE, const wxPoint& pos = SYMBOL_PARAM_POSITION, const wxSize& size = SYMBOL_PARAM_SIZE, long style = wxDEFAULT_DIALOG_STYLE);

	~Surface2DParamDlg(void) {}

	bool Create(wxWindowID id = ID_2D_PARAM_DIALOG, const wxString& caption = SYMBOL_2D_PARAM_TITLE, const wxPoint& pos = SYMBOL_PARAM_POSITION, const wxSize& size = SYMBOL_PARAM_SIZE, long style = wxDEFAULT_DIALOG_STYLE);

private:
	/// Setup the dialog controls
	void createControls();
	/// Called when the dialog is closed.
	/// The current dialog values are read and applied to the surface.
	void OnClose(wxCommandEvent &event);
	/// Called when the dialog is closed without saving the values.
	void OnCancel(wxCommandEvent &event);
	/// Called to rollup the grid parameters and place them in the copy buffer.
	void OnCopyAll(wxCommandEvent &event);
	/// Pull saved values from the copy buffer and insert them into the dialog.
	void OnPasteAll(wxCommandEvent &event);
	/// Edit controls for each of the grid parameters.
	wxTextCtrl *numGridPoint;
	wxTextCtrl *originText1, *originText2, *originText3;
	wxTextCtrl *vectorAxis1x, *vectorAxis1y, *vectorAxis1z;
	wxTextCtrl *vectorAxis2x, *vectorAxis2y, *vectorAxis2z;

	BaseSurfacePane * mParent;		///< The calling window
	Surf2DBase * mTargetSurf;		///< The target surface

	DECLARE_EVENT_TABLE()
};

/**
 * A dialog to allow the user to manually setup the 3D grid.
 * Allows the user to directly setup the grid including the number of grid points in each direction,
 * the origin and the increment in each direction. The dialog presents the current
 * grid allowing the user to copy all of the parameters to paste into a different surface.
 * While this dialog does maintain a connect to it's parent it is likely best to be run as a modal
 * dialog.
 */
class Surface3DParamDlg : public wxDialog
{
  /* DECLARE_CLASS(Surface3DParamDlg); */

public:
	Surface3DParamDlg(BaseSurfacePane * parent, Surf3DBase * targetSurface, wxWindowID id = ID_3D_PARAM_DIALOG, const wxString& caption = SYMBOL_3D_PARAM_TITLE, const wxPoint& pos = SYMBOL_PARAM_POSITION, const wxSize& size = SYMBOL_PARAM_SIZE, long style = wxDEFAULT_DIALOG_STYLE);

	~Surface3DParamDlg(void){}

	bool Create(wxWindowID id = ID_3D_PARAM_DIALOG, const wxString& caption = SYMBOL_3D_PARAM_TITLE, const wxPoint& pos = SYMBOL_PARAM_POSITION, const wxSize& size = SYMBOL_PARAM_SIZE, long style = wxDEFAULT_DIALOG_STYLE);
	
private:
	/// Setup the dialog controls
	void createControls();
	/// Called when the dialog is closed.
	/// The current dialog values are read and applied to the surface.
	void OnClose(wxCommandEvent &event);
	/// Called to rollup the grid parameters and place them in the copy buffer.
	void OnCopyAll(wxCommandEvent &event);
	/// Pull saved values from the copy buffer and insert them into the dialog.
	void OnPasteAll(wxCommandEvent &event);
	/// Edit controls for each of the grid parameters.

	wxTextCtrl *numGridPoint1, *numGridPoint2, *numGridPoint3;
	wxTextCtrl *originText1, *originText2, *originText3;
	wxTextCtrl *gridIncText1, *gridIncText2, *gridIncText3;

	BaseSurfacePane * mParent;		///< The calling window
	Surf3DBase *	mTargetSurf;	///< The target surface

	DECLARE_EVENT_TABLE()
};

#endif
