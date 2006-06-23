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

#include <vector>

////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
class colorArea;
class MoleculeData;
class WinPrefs;

class Surface;
class Surf3DBase;
class Orb3DSurface;
class OrbSurfBase;
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
#define ID_TRANSPARENCY_COLOR 10085
#define ID_REVERSE_PHASE_CHECKBOX 10086
#define ID_ORB_FORMAT_CHOICE 10087
#define ID_ORB_COLOR_POSITIVE 10088
#define ID_ORB_COLOR_NEGATIVE 10089
#define ID_SPH_HARMONICS_CHECKBOX 10090

////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * base class declaration
 */

class BaseSurfacePane : public wxPanel
{
  DECLARE_CLASS(BaseSurfacePane)

 public:

  BaseSurfacePane() {}
  BaseSurfacePane( wxWindow* parent, Surface* target, MoleculeData* data, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );
  ~BaseSurfacePane();

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );
 
 protected:
  wxBoxSizer* mainSizer;
  wxBoxSizer* upperSizer;
  wxBoxSizer* middleSizer;
  wxBoxSizer* bottomSizer;
  wxBoxSizer* leftMiddleSizer;
  wxBoxSizer* upperLeftMiddleSizer;
  wxBoxSizer* lowerLeftMiddleSizer;
  wxFlexGridSizer* rightMiddleSizer;
  wxBoxSizer* leftBottomSizer;
  wxBoxSizer* rightBottomSizer;

  wxStaticText* label0;
  wxStaticText* label1;
  wxChoice* mOrbSetChoice;
  wxSlider* mNumGridPntSld;
  wxButton* mSetParamBut;
  wxButton* mExportBut;
  wxButton* mUpdateBut;
  wxCheckBox* mSphHarmonicsChk;

  //MoleculeData* mData;

 private:
  void CreateControls();

  Surface* mTarget;

  //DECLARE_EVENT_TABLE()
};

class OrbSurfacePane
{
 public:
  OrbSurfacePane() {}
  OrbSurfacePane( OrbSurfBase* target, MoleculeData* data, WinPrefs* prefs );
  ~OrbSurfacePane();

 protected:
  void makeMOList(std::vector<wxString>& choice);
  void makeAOList(wxString& choice);
  int getOrbSetForOrbPane(std::vector<wxString>& choice);

  long TargetSet;
  long OrbOptions; //from mac version
  long OrbColumnEnergyOrOccupation;
  long SphericalHarmonics;
  long PlotOrb;
  MoleculeData* mData;

 private:
  OrbSurfBase* mTarget;
  WinPrefs* mPrefs;

};

/*!
 * 2D class declaration
 */
/*
class Surface2DPane : public BaseSurfacePane
{

 public:
  Surface2DPane() {}
  ~Surface2DPane();

 private:
  //DECLARE_EVENT_TABLE()
};
*/

class Surface3DPane : public BaseSurfacePane
{
  DECLARE_CLASS(Surface3DPane)

 public:
  Surface3DPane() {}
  Surface3DPane( wxWindow* parent, Surf3DBase* target, MoleculeData* data, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );
  ~Surface3DPane();

 protected:
  wxStaticText* label2;
  wxStaticText* label3;
  wxStaticText* label4;
  wxSlider* mGridSizeSld;
  wxSlider* mContourValSld;
  colorArea* mTransColor;
  wxRadioBox* m3DRdoBox;
  wxCheckBox* mSmoothChkBox;
  wxButton* mFreeMemBut;

  void On3DRadioBox (wxCommandEvent& event );
  void OnSmoothCheck (wxCommandEvent& event );

 private:
  void CreateControls();

  Surf3DBase* mTarget;
  bool UseNormals;
  bool UseSolidSurface;

  //DECLARE_EVENT_TABLE()
}; 

class Orbital3DSurfPane : public Surface3DPane, public OrbSurfacePane
{    
  DECLARE_CLASS( Orbital3DSurfPane )

public:
    /// Constructors
    Orbital3DSurfPane() { }
    Orbital3DSurfPane( wxWindow* parent, Orb3DSurface* target, MoleculeData* data, WinPrefs* prefs, wxWindowID id = SYMBOL_ORBITAL3D_IDNAME, const wxPoint& pos = SYMBOL_ORBITAL3D_POSITION, const wxSize& size = SYMBOL_ORBITAL3D_SIZE, long style = SYMBOL_ORBITAL3D_STYLE );
    ~Orbital3DSurfPane();

    /// Creates the controls and sizers
    void CreateControls();

////@begin Orbital3D event handler declarations

    /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHOICE1
    //void OnChoice1Selected( wxCommandEvent& event );

    /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CHECKBOX5
    //void OnCheckbox5Click( wxCommandEvent& event );

////@end Orbital3D event handler declarations

////@begin Orbital3D member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end Orbital3D member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

 private:
    void OnOrbFormatChoice( wxCommandEvent &event );
    void OnAtomList( wxCommandEvent &event );
    void OnOrbSetChoice( wxCommandEvent &event );
    void OnSphHarmonicChk(wxCommandEvent &event );

    wxBoxSizer* mSubLeftBot1Sizer;
    wxBoxSizer* mSubLeftBot2Sizer;
    wxBoxSizer* mSubRightBot1Sizer;
    wxBoxSizer* mSubRightBot2Sizer;
    wxBoxSizer* mSubRightBot3Sizer;
    wxBoxSizer* mSubRightBot4Sizer;
    wxGridSizer* mSubRightBot5Sizer;

    wxCheckBox* mRevPhaseChk;
    wxChoice* mSltOrbChoice;
    colorArea* mOrbColor1;
    colorArea* mOrbColor2;
    wxListBox* mAtomList;
    wxTextCtrl* mOrbCoef;
    wxChoice* mOrbFormatChoice;

    Orb3DSurface* mTarget;

    DECLARE_EVENT_TABLE()
};

#endif
