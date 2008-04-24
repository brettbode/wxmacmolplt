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
#define ID_TED3D_COLOR_SURF_CHECK 10110
#define ID_USERGB_COLOR_CHECK 10111
#define ID_TED3D_MAX_MAP_EDIT 10112
#define ID_DISPLAY_PLANE_CHECKBOX 10244
#define ID_INVERT_RGB_CHECK 10245

////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * The base class for all the surface types for the Surfaces Window ListBook.
 * This class contains and manages the controls common to all surfaces such
 * as title, visibility, update button, etc.
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

		void Append(const wxString& item) {
			items.push_back(item);
			SetItemCount(items.size());
			RefreshAll();
		}

		wxString OnGetItem(size_t n) const {
			/* std::cout << "items[n]: " << items[n] << std::endl; */
			return items[n];
		}

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
		std::vector<wxString> items;

	DECLARE_DYNAMIC_CLASS(FormattedListBox)
	DECLARE_NO_COPY_CLASS(FormattedListBox)
	DECLARE_EVENT_TABLE()
};

class BaseSurfacePane : public wxPanel
{
	DECLARE_CLASS(BaseSurfacePane)

public:

	BaseSurfacePane() {}
	BaseSurfacePane( wxWindow* parent, Surface * target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );
	~BaseSurfacePane();

	bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );

	virtual void TargetToPane() = 0;	//< Call to update the display with new data in the target surface.
	virtual void refreshControls() = 0;	//< Updates the control state, call when data changes.
	virtual bool UpdateNeeded(void) = 0;	//< Should the update button be activated?

	void SetVisibility(bool state);	//< Toggle the surface visibility.

	void OnCheckAllFrames(wxCommandEvent& event);
	void OnCheckVisible(wxCommandEvent& event);
	void OnChangeTitle(wxCommandEvent& event);

	void setAllFrames(bool state);	//< Should the surface be applied to all frames?

	bool GetVisibility(void) const {return Visible;};	//< Is the surface visible?
	bool GetAllFrames(void) const {return AllFrames;};	//< Is the surface applies to all frames?

	const wxString GetSurfaceName() const;

	void SetUpdateTest(bool test);	//< Set flag to force an update (overrides the automatic tests).
	void setUpdateButton();			//< Updates the state of the update button comparing the current and saved states.
	/**
		Call when the page becomes the active page. Right now this just makes sure the
		update button is the default button.
	 */
	void PageIsNowActive(void);
	Surface * GetTargetSurface(void) const {return mTarget;};	//< Return a pointer to the surface data.

	void OnGridPointSld( wxCommandEvent &event );	//< Call back when the grid points slider is changed.
	/**
	 * Call back for the Set Param button.
	 * Displays the appropriate surface parameters dialog.
	 */
	void OnSetParam( wxCommandEvent &event );
	/**
		Call back for the Export button.
		Prompts the user for a file name, then exports the surface to that file.
	 */
	void OnExport( wxCommandEvent &event );
	void BuildOrbSetPopup(void);
	void OnOrbSetChoice( wxCommandEvent &event );

protected:
	wxBoxSizer* mainSizer;	//< A primary BoxSizer for the page.

	wxSlider* mNumGridPntSld;
	wxButton* mSetParamBut;
	wxButton* mExportBut;
	wxButton* mUpdateBut;

	bool Visible;			//< Should the surface be visible?
	bool AllFrames;			//< flag to apply the surface to all frames.
	bool UpdateTest;		//< flag to force an update.
	long NumGridPoints;
	bool SwitchFixGrid;
	long TargetOrbSet;
	wxChoice* mOrbSetChoice;
	wxTextCtrl* surfTitleEdit;
	wxCheckBox* visibleCheck;
	wxCheckBox* allFrameCheck;

	SurfacesWindow * owner;	//< Our parent window.

 private:
	void CreateControls();	//< Create the controls (currently just the main sizer).

	Surface* mTarget;	//< Pointer to the target surface.

    DECLARE_EVENT_TABLE()
};

class OrbSurfacePane {
	public:
		OrbSurfacePane() {}
		OrbSurfacePane(OrbSurfBase* target, SurfacesWindow* owner);
		~OrbSurfacePane();

	protected:
		bool UpdateEvent(); 

		//put code that is common for orbital panels
		void makeMOList();
		void makeAOList();
		int getOrbSetForOrbPane(std::vector<wxString>& choice);
		int orbSetChangeEvt(int item, SurfacesWindow * owner);
		void setFlagOnOrbFormatChange(int itemtype);

		void OnAtomicOrbitalChoice(wxCommandEvent& event);

		long TargetSet;
		long OrbOptions; //from mac version
		long OrbColumnEnergyOrOccupation;
		long SphericalHarmonics;
		long PlotOrb;
		bool PhaseChange;
		bool coefIsEnabled;
		FormattedListBox *mMOList;
		FormattedListBox *mOrbCoef;

	private:
		OrbSurfBase* mTarget;
		SurfacesWindow * myowner;
};

/*!
 * 2D class declaration
 */

class Surface2DPane : public BaseSurfacePane
{
  DECLARE_CLASS(Surface2DPane)

 public:
  Surface2DPane() {}
  Surface2DPane( wxWindow* parent, Surf2DBase* target, SurfacesWindow* Owner, wxWindowID id, const wxPoint& pos, const wxSize& size, long style );
  ~Surface2DPane();

  void OnPosColorChange(wxCommandEvent & event);
  void OnNegColorChange(wxCommandEvent & event);
  void OnShowZeroChk( wxCommandEvent &event );
  void OnDashChk(wxCommandEvent& event );
  void OnUsePlaneChk( wxCommandEvent &event );
  void OnDisplayPlaneChk( wxCommandEvent &event );
  void OnContourValueText( wxCommandEvent &event );
  void SetContourValueText(void);
  void OnNumContoursText( wxCommandEvent &event );
  void SetNumContoursText(void);
  void OnIdle( wxIdleEvent& WXUNUSED(event) );
  void OnSetPlane( wxCommandEvent &event );
  void OnSetParam( wxCommandEvent &event );

 protected:

  wxStaticText* mNumContourLabel;
  wxStaticText* mContourValLabel;
  wxTextCtrl* mNumContourText;
  wxTextCtrl* mContourValText;
  wxCheckBox* mShowZeroCheck;
  wxCheckBox* mDashCheck;
  wxCheckBox* mUsePlaneChk;
  wxCheckBox* mDisplayPlaneCheck;
  wxButton* mSetPlaneBut;
  colorArea* mOrbColor1;
  colorArea* mOrbColor2;
  RGBColor PosColor;
  RGBColor NegColor;

  long NumContours;
  float MaxContourValue;
  bool ShowZeroContour;
  bool DashLines;
  bool UseScreenPlane;
  bool DisplayPlane;
  wxString mMaxContourCountString;
  wxString mMaxContourValueString;

 private:

  Surf2DBase* mTarget;

  //DECLARE_EVENT_TABLE()
};


class Surface3DPane : public BaseSurfacePane
{
  DECLARE_CLASS(Surface3DPane)

public:
  Surface3DPane() {}

  Surface3DPane( wxWindow* parent, Surf3DBase* target, SurfacesWindow* Owner, wxWindowID id, const wxPoint& pos, const wxSize& size, long style );
  ~Surface3DPane();

  void On3DRadioBox (wxCommandEvent& event );
  void OnSmoothCheck (wxCommandEvent& event );
  /* void OnContourValueEnter(wxCommandEvent& event ); */
  void OnFreeMem(wxCommandEvent& event );
  void OnPosColorChange(wxCommandEvent & event);
  void OnNegColorChange(wxCommandEvent & event);
  void OnTransparencyChange(wxSpinEvent & event);
  void OnIdle( wxIdleEvent& WXUNUSED(event) );
  void OnSetParam( wxCommandEvent &event );
  void OnGridSizeSld(wxCommandEvent &event );

  void setContourValueSld();
  void SetContourValueText(void);
  void OnContourValueSld(wxCommandEvent &event );
  void SetContourMaxValueText(void);

protected:
  wxSlider* mGridSizeSld;
  /* wxSlider* mContourValSld; */
  FloatSlider* mContourValSld;
  /* wxTextCtrl* mContourValueEdit; */
  wxStaticText* mGridMinText;
  wxStaticText* mGridMaxText;
  colorArea* mOrbColor1;
  colorArea* mOrbColor2;
  wxSpinCtrl *mTransparency;
  wxRadioBox* m3DRdoBox;
  wxCheckBox* mSmoothChkBox;
  wxButton* mFreeMemBut;

  float GridSize;
  RGBColor PosColor;
  RGBColor NegColor;
  int Transparency;
  bool UseNormals;
  bool UseSolidSurface;
  float ContourValue;

 private:
  void changeContourValue();

  Surf3DBase* mTarget;

  //DECLARE_EVENT_TABLE()
}; 

class Orbital2DSurfPane : public Surface2DPane, public OrbSurfacePane {    
  DECLARE_CLASS( Orbital2DSurfPane )

 public:
    /// Constructors
    Orbital2DSurfPane() { }
    Orbital2DSurfPane( wxWindow* parent, Orb2DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL2D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL2D_POSITION, const wxSize& size = SYMBOL_ORBITAL2D_SIZE, long style = SYMBOL_ORBITAL2D_STYLE );
    ~Orbital2DSurfPane();

    virtual void TargetToPane();
    virtual void refreshControls();

    void OnOrbSetChoice( wxCommandEvent &event );
    void OnMOList( wxCommandEvent &event );
    void OnOrbFormatChoice( wxCommandEvent &event );
    void OnReversePhase(wxCommandEvent &event );
    void OnSphHarmonicChk(wxCommandEvent &event );
    void OnCoefList( wxCommandEvent &event );

 private:
    virtual bool UpdateNeeded(void);
    void CreateControls();
    void OnUpdate(wxCommandEvent &event );

    wxBoxSizer* upperSizer;
    wxBoxSizer* middleSizer;
    wxBoxSizer* bottomSizer;
    wxBoxSizer* leftMiddleSizer;
    wxBoxSizer* upperLeftMiddleSizer;
    wxBoxSizer* lowerLeftMiddleSizer;
    wxFlexGridSizer* rightMiddleSizer;
    wxBoxSizer* leftBottomSizer;
    wxBoxSizer* rightBottomSizer;

    wxBoxSizer* mSubLeftBot1Sizer;
    wxBoxSizer* mSubLeftBot2Sizer;
    wxBoxSizer* mSubRightBot1Sizer;
    wxBoxSizer* mSubRightBot2Sizer;
    wxGridSizer* mSubRightBot3Sizer;

    wxChoice* mOrbFormatChoice;
    wxCheckBox* mSphHarmonicsChk;
    wxCheckBox* mRevPhaseChk;

    Orb2DSurface* mTarget;

    DECLARE_EVENT_TABLE()
};

class Orbital3DSurfPane : public Surface3DPane, public OrbSurfacePane
{    
  DECLARE_CLASS( Orbital3DSurfPane )

public:
    /// Constructors
    Orbital3DSurfPane() { }
    Orbital3DSurfPane( wxWindow* parent, Orb3DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );
    ~Orbital3DSurfPane();

    virtual void TargetToPane();
    virtual void refreshControls();

    /// Creates the controls and sizers
    void CreateControls();

    void OnOrbFormatChoice( wxCommandEvent &event );
    void OnMOList( wxCommandEvent &event );
    void OnOrbSetChoice( wxCommandEvent &event );
    void OnSphHarmonicChk(wxCommandEvent &event );
    void OnReversePhase(wxCommandEvent &event );
    void OnContourValueSld(wxCommandEvent &event );
    void OnGridSizeSld(wxCommandEvent &event );
    void OnUpdate(wxCommandEvent &event );
    void OnCoefList( wxCommandEvent &event );

////@begin Orbital3D member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end Orbital3D member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

 private:
    virtual bool UpdateNeeded(void);

    wxBoxSizer* upperSizer;
    wxBoxSizer* middleSizer;
    wxBoxSizer* bottomSizer;
    wxBoxSizer* leftMiddleSizer;
    wxBoxSizer* upperLeftMiddleSizer;
    wxBoxSizer* lowerLeftMiddleSizer;
    wxFlexGridSizer* rightMiddleSizer;
    wxBoxSizer* leftBottomSizer;
    wxBoxSizer* rightBottomSizer;

    wxBoxSizer* mSubLeftBot1Sizer;
    wxBoxSizer* mSubLeftBot2Sizer;
    wxBoxSizer* mSubRightBot0Sizer;
    wxBoxSizer* mSubRightBot1Sizer;
    wxBoxSizer* mSubRightBot2Sizer;
    wxBoxSizer* mSubRightBot3Sizer;
    wxBoxSizer* mSubRightBot4Sizer;
    wxGridSizer* mSubRightBot5Sizer;

    wxChoice* mOrbFormatChoice;
    wxCheckBox* mSphHarmonicsChk;
    wxCheckBox* mRevPhaseChk;  

    Orb3DSurface* mTarget;

   DECLARE_EVENT_TABLE()
};

class General3DSurfPane : public Surface3DPane
{    
	DECLARE_CLASS( General3DSurfPane )
	
public:
    /// Constructors
    General3DSurfPane() { }
    General3DSurfPane( wxWindow* parent, General3DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );
    ~General3DSurfPane();
	
    virtual void TargetToPane();
    virtual void refreshControls();
	
    /// Creates the controls and sizers
    void CreateControls();

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );
	
    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
	
    /// Should we show tooltips?
    static bool ShowToolTips();
	
private:
	/// Do the displayed values differ from the saved values.
	virtual bool UpdateNeeded(void);
	
	void OnMultCheck(wxCommandEvent &event);
	void OnSquareCheck(wxCommandEvent &event);
	void OnContourPosNegCheck(wxCommandEvent &event);
	/// Call back function for when the file button is pressed.
	
	/// Prompts the user for the file and then attempts to read it in.
	void OnFileButton(wxCommandEvent &event);
	/// Call back function for when the Update button is pressed.
	void OnUpdate(wxCommandEvent &event );
	/// Call back function for changes in the counter value slider.
    void OnContourValueSld(wxCommandEvent &event );
	/// Call back function for when the text field is changed.
	void OnMultValueEdit(wxCommandEvent& event );
	
    wxCheckBox* mMultCheck;
    wxTextCtrl* mGenMultValue;
    wxCheckBox* mSquareCheck;
    wxCheckBox* mGenContourPosNegCheck;
	wxString MultValueString;

    General3DSurface*	mTarget;
	bool				ContourPosNeg;
	bool				useMultValue;
	bool				squareValues;
	float				MultValue;
	
    DECLARE_EVENT_TABLE()
};

class General2DSurfPane : public Surface2DPane
{    
	DECLARE_CLASS( General3DSurfPane )
	
public:
    /// Constructors
    General2DSurfPane() { }
    General2DSurfPane( wxWindow* parent, General2DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );
    ~General2DSurfPane();
	
    virtual void TargetToPane();
    virtual void refreshControls();
	
    /// Creates the controls and sizers
    void CreateControls();
	
    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );
	
    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
	
    /// Should we show tooltips?
    static bool ShowToolTips() {return true;};
	
private:
	virtual bool UpdateNeeded(void);
	
	void OnMultCheck(wxCommandEvent &event);
	void OnSquareCheck(wxCommandEvent &event);
	void OnFileButton(wxCommandEvent &event);
	void OnUpdate(wxCommandEvent &event );
	void OnMultValueEnter(wxCommandEvent& event );
	void SetMultValue(void);
	
    wxCheckBox* mMultCheck;
    wxTextCtrl* mGenMultValue;
    wxCheckBox* mSquareCheck;
	wxString MultValueString;
	
    General2DSurface*	mTarget;
	bool				useMultValue;
	bool				squareValues;
	float				MultValue;
	
    DECLARE_EVENT_TABLE()
};

class TEDensity2DSurfPane : public Surface2DPane
{    
	DECLARE_CLASS( TEDensity2DSurfPane )
	
public:
    /// Constructors
    TEDensity2DSurfPane() { }
    TEDensity2DSurfPane( wxWindow* parent, TEDensity2DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_TED2D_IDNAME, const wxPoint& pos = SYMBOL_TED2D_POSITION, const wxSize& size = SYMBOL_TED2D_SIZE, long style = SYMBOL_TED2D_STYLE );
    ~TEDensity2DSurfPane();
	
    virtual void TargetToPane();
    virtual void refreshControls();
	
    /// Creates the controls and sizers
    void CreateControls();
	
    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );
	
    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
	
    /// Should we show tooltips?
    static bool ShowToolTips() {return true;};
	
private:
	virtual bool UpdateNeeded(void);

	void OnUpdate(wxCommandEvent &event );

    TEDensity2DSurface*	mTarget;
	
    DECLARE_EVENT_TABLE()
};
class TEDensity3DSurfPane : public Surface3DPane
{    
	DECLARE_CLASS( TEDensity3DSurfPane )
	
public:
    /// Constructors
    TEDensity3DSurfPane() { }
    TEDensity3DSurfPane( wxWindow* parent, TEDensity3DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );
    ~TEDensity3DSurfPane();
	
    virtual void TargetToPane();
    virtual void refreshControls();
	void OnUseMEPCheck(wxCommandEvent &event );
	void OnRGBColorCheck(wxCommandEvent &event );
	void OnInvertRGBCheck(wxCommandEvent &event );
	void OnMaxMEPValueText(wxCommandEvent &event );
	
    /// Creates the controls and sizers
    void CreateControls();
	
    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );
	
    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
	
    /// Should we show tooltips?
    static bool ShowToolTips() {return true;};
	
private:
		virtual bool UpdateNeeded(void);
	
	void OnUpdate(wxCommandEvent &event );
	void SetMaxMEPValueText(void);
	
	wxCheckBox*	mColorSurfCheck;
	wxCheckBox*	mUseRGBColorCheck;
	wxCheckBox*	mInvertRGBCheck;
	wxTextCtrl*	mMaxMapEdit;
	wxString mMaxMapValidator;
	
	bool	UseMEP;
	bool	UseRGBSurfaceColor;
	bool	InvertRGBSurfaceColor;
	float	MaxMEPValue;
	
    TEDensity3DSurface*	mTarget;
	
    DECLARE_EVENT_TABLE()
};
class MEP2DSurfPane : public Surface2DPane
{    
	DECLARE_CLASS( MEP2DSurfPane )
	
public:
    /// Constructors
    MEP2DSurfPane() { }
    MEP2DSurfPane( wxWindow* parent, MEP2DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );
    ~MEP2DSurfPane();
	
    virtual void TargetToPane();
    virtual void refreshControls();
	
    /// Creates the controls and sizers
    void CreateControls();
	
    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );
	
    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
	
    /// Should we show tooltips?
    static bool ShowToolTips() {return true;};
	
private:
		virtual bool UpdateNeeded(void);
	
	void OnUpdate(wxCommandEvent &event );
	
    MEP2DSurface*	mTarget;
	
    DECLARE_EVENT_TABLE()
};
class MEP3DSurfPane : public Surface3DPane
{    
	DECLARE_CLASS( MEP3DSurfPane )
	
public:
    /// Constructors
    MEP3DSurfPane() { }
    MEP3DSurfPane( wxWindow* parent, MEP3DSurface* target, SurfacesWindow* owner, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );
    ~MEP3DSurfPane();
	
    virtual void TargetToPane();
    virtual void refreshControls();
	
    /// Creates the controls and sizers
    void CreateControls();
	
    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );
	
    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
	
    /// Should we show tooltips?
    static bool ShowToolTips() {return true;};
	
private:
		virtual bool UpdateNeeded(void);
	
	void OnUpdate(wxCommandEvent &event );

    MEP3DSurface*	mTarget;
	
    DECLARE_EVENT_TABLE()
};

class Surface2DParamDlg : public wxFrame 
{
  DECLARE_CLASS(Surface2DParamDlg);

 public:
  Surface2DParamDlg(BaseSurfacePane * parent, Surf2DBase * targetSurface, wxWindowID id = ID_2D_PARAM_DIALOG, const wxString& caption = SYMBOL_2D_PARAM_TITLE, const wxPoint& pos = SYMBOL_PARAM_POSITION, const wxSize& size = SYMBOL_PARAM_SIZE, long style = wxDEFAULT_FRAME_STYLE );

  ~Surface2DParamDlg(void) {}

  bool Create(wxWindowID id = ID_2D_PARAM_DIALOG, const wxString& caption = SYMBOL_2D_PARAM_TITLE, const wxPoint& pos = SYMBOL_PARAM_POSITION, const wxSize& size = SYMBOL_PARAM_SIZE, long style = wxDEFAULT_FRAME_STYLE );

 private:
  void createControls();

  void OnClose(wxCommandEvent &event );
  void OnCancel(wxCommandEvent &event );
  void OnCopyAll(wxCommandEvent &event );
  void OnPasteAll(wxCommandEvent &event );

  wxBoxSizer* mainSizer;
  wxBoxSizer *firstTierSizer, *secondTierSizer, *thirdTierSizer;
  wxBoxSizer *fourthTierSizer, *fifthTierSizer;

  wxTextCtrl *numGridPoint;
  wxTextCtrl *originText1, *originText2, *originText3;
  wxTextCtrl *vectorAxis1x, *vectorAxis1y, *vectorAxis1z;
  wxTextCtrl *vectorAxis2x, *vectorAxis2y, *vectorAxis2z;
  wxButton *okButton, *cancelButton, *copyAllButton, *pasteAllButton;

  BaseSurfacePane * mParent;
  Surf2DBase * mTargetSurf;

  DECLARE_EVENT_TABLE()
};


class Surface3DParamDlg : public wxFrame 
{
  DECLARE_CLASS(Surface3DParamDlg);

 public:
  Surface3DParamDlg(BaseSurfacePane * parent, Surf3DBase * targetSurface, wxWindowID id = ID_3D_PARAM_DIALOG, const wxString& caption = SYMBOL_3D_PARAM_TITLE, const wxPoint& pos = SYMBOL_PARAM_POSITION, const wxSize& size = SYMBOL_PARAM_SIZE, long style = wxDEFAULT_FRAME_STYLE );

  ~Surface3DParamDlg(void){}

  bool Create(wxWindowID id = ID_3D_PARAM_DIALOG, const wxString& caption = SYMBOL_3D_PARAM_TITLE, const wxPoint& pos = SYMBOL_PARAM_POSITION, const wxSize& size = SYMBOL_PARAM_SIZE, long style = wxDEFAULT_FRAME_STYLE );
  
 private:
  void createControls();

  void OnClose(wxCommandEvent &event );
  void OnCancel(wxCommandEvent &event );
  void OnCopyAll(wxCommandEvent &event );
  void OnPasteAll(wxCommandEvent &event );

  wxBoxSizer* mainSizer;
  wxBoxSizer *firstTierSizer, *secondTierSizer, *thirdTierSizer;
  wxBoxSizer* fourthTierSizer;

  wxTextCtrl *numGridPoint1, *numGridPoint2, *numGridPoint3;
  wxTextCtrl *originText1, *originText2, *originText3;
  wxTextCtrl *gridIncText1, *gridIncText2, *gridIncText3;
  wxButton *okButton, *cancelButton, *copyAllButton, *pasteAllButton;

  BaseSurfacePane * mParent;
  Surf3DBase *	mTargetSurf;

  DECLARE_EVENT_TABLE()
};

#endif
