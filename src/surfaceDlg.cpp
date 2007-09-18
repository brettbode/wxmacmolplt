/////////////////////////////////////////////////////////////////////////////
// Name:        orbital3d.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Fri  2 Jun 10:08:18 2006
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "surfaceDlg.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "Globals.h"
#include "SurfaceTypes.h"
#include "surfaceDlg.h"
#include "colorArea.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "BasisSet.h"
#include "Prefs.h"
#include "Progress.h"
#include "MolDisplayWin.h"
#include "surfaceswindow.h"
#include "setscreenplane.h"

#include <wx/config.h>

////@begin XPM images
////@end XPM images

using namespace std;
#include <iostream>

IMPLEMENT_CLASS( BaseSurfacePane, wxPanel )
IMPLEMENT_CLASS( Surface2DPane, wxPanel )
IMPLEMENT_CLASS( Surface3DPane, wxPanel )
IMPLEMENT_CLASS( Orbital2DSurfPane, wxPanel )
IMPLEMENT_CLASS( Orbital3DSurfPane, wxPanel )
  //IMPLEMENT_CLASS( TEDensity2DSurfPane, wxPanel )
IMPLEMENT_CLASS( General3DSurfPane, wxPanel )
IMPLEMENT_CLASS( General2DSurfPane, wxPanel )
IMPLEMENT_CLASS( TEDensity2DSurfPane, wxPanel )
IMPLEMENT_CLASS( TEDensity3DSurfPane, wxPanel )
IMPLEMENT_CLASS( MEP2DSurfPane, wxPanel )
IMPLEMENT_CLASS( MEP3DSurfPane, wxPanel )

IMPLEMENT_CLASS( Surface2DParamDlg, wxFrame )
IMPLEMENT_CLASS( Surface3DParamDlg, wxFrame )

BEGIN_EVENT_TABLE( Orbital2DSurfPane, wxPanel )
  EVT_CHOICE  (ID_ORB_FORMAT_CHOICE,  Orbital2DSurfPane::OnOrbFormatChoice)
  EVT_SLIDER (ID_GRID_POINT_SLIDER, BaseSurfacePane::OnGridPointSld)
  EVT_CHOICE  (ID_ORB_CHOICE, Orbital2DSurfPane::OnOrbSetChoice)
  EVT_BUTTON (ID_SURFACE_EXPORT_BUT, BaseSurfacePane::OnExport)
  EVT_LISTBOX (ID_ATOM_LIST, Orbital2DSurfPane::OnAtomList)
  EVT_LISTBOX (ID_ORB_COEF, Orbital2DSurfPane::OnCoefList)
  EVT_CHECKBOX (ID_USE_PLANE_CHECKBOX, Surface2DPane::OnUsePlaneChk)
  EVT_CHECKBOX (ID_SHOW_ZERO_CHECKBOX, Surface2DPane::OnShowZeroChk)
  EVT_CHECKBOX (ID_DASH_CHECKBOX, Surface2DPane::OnDashChk)
  EVT_CHECKBOX (ID_REVERSE_PHASE_CHECKBOX, Orbital2DSurfPane::OnReversePhase)
  EVT_BUTTON (ID_SURFACE_UPDATE_BUT, Orbital2DSurfPane::OnUpdate)
  EVT_BUTTON (ID_SET_PARAM_BUT, Surface2DPane::OnSetParam)
  EVT_BUTTON (ID_SET_PLANE_BUT, Surface2DPane::OnSetPlane)
  EVT_COMMAND_ENTER(ID_2D_COLOR_POSITIVE, Surface2DPane::OnPosColorChange)
  EVT_COMMAND_ENTER(ID_2D_COLOR_NEGATIVE, Surface2DPane::OnNegColorChange)
	EVT_CHECKBOX (ID_DISPLAY_PLANE_CHECKBOX, Surface2DPane::OnDisplayPlaneChk)
	EVT_TEXT (ID_CONTOUR_VALUE_EDIT, Surface2DPane::OnContourValueText)
	EVT_TEXT (ID_NUM_CONTOUR_TEXT, Surface2DPane::OnNumContoursText)
	EVT_CHECKBOX (ID_SPH_HARMONICS_CHECKBOX, Orbital2DSurfPane::OnSphHarmonicChk)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( Orbital3DSurfPane, wxPanel )
	EVT_CHOICE  (ID_ORB_FORMAT_CHOICE,  Orbital3DSurfPane::OnOrbFormatChoice)
	EVT_CHOICE  (ID_ORB_CHOICE, Orbital3DSurfPane::OnOrbSetChoice)
	EVT_LISTBOX (ID_ATOM_LIST, Orbital3DSurfPane::OnAtomList)
	EVT_LISTBOX (ID_ORB_COEF, Orbital3DSurfPane::OnCoefList)
	EVT_CHECKBOX (ID_SPH_HARMONICS_CHECKBOX, Orbital3DSurfPane::OnSphHarmonicChk)
	EVT_RADIOBOX (ID_3D_RADIOBOX, Surface3DPane::On3DRadioBox)
	EVT_CHECKBOX (ID_SMOOTH_CHECKBOX, Surface3DPane::OnSmoothCheck)
	EVT_CHECKBOX (ID_REVERSE_PHASE_CHECKBOX, Orbital3DSurfPane::OnReversePhase)
	EVT_SLIDER (ID_CONTOUR_VALUE_SLIDER, Orbital3DSurfPane::OnContourValueSld)
	EVT_SLIDER (ID_GRID_SIZE_SLIDER, Orbital3DSurfPane::OnGridSizeSld)
	EVT_SLIDER (ID_GRID_POINT_SLIDER, BaseSurfacePane::OnGridPointSld)
	EVT_BUTTON (ID_SURFACE_UPDATE_BUT, Orbital3DSurfPane::OnUpdate)
	EVT_TEXT (ID_CONTOUR_VALUE_EDIT, Surface3DPane::OnContourValueEnter)
	EVT_BUTTON (ID_SET_PARAM_BUT, Surface3DPane::OnSetParam)
	EVT_BUTTON (ID_FREE_MEM_BUT, Surface3DPane::OnFreeMem)
	EVT_BUTTON (ID_SURFACE_EXPORT_BUT, BaseSurfacePane::OnExport)
	EVT_COMMAND_ENTER(ID_3D_COLOR_POSITIVE, Surface3DPane::OnPosColorChange)
	EVT_COMMAND_ENTER(ID_3D_COLOR_NEGATIVE, Surface3DPane::OnNegColorChange)
	EVT_COMMAND_ENTER(ID_TRANSPARENCY_COLOR, Surface3DPane::OnTranspColorChange)
	EVT_IDLE(Surface3DPane::OnIdle)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( General3DSurfPane, wxPanel )
	EVT_BUTTON (ID_GENFILEBUTTON, General3DSurfPane::OnFileButton)
	EVT_CHECKBOX (ID_GENMULTCHECK, General3DSurfPane::OnMultCheck)
	EVT_CHECKBOX (ID_GENSQUARECHECK, General3DSurfPane::OnSquareCheck)
	EVT_CHECKBOX (ID_GENCONPOSNEGCHECK, General3DSurfPane::OnContourPosNegCheck)
	EVT_RADIOBOX (ID_3D_RADIOBOX, Surface3DPane::On3DRadioBox)
	EVT_CHECKBOX (ID_SMOOTH_CHECKBOX, Surface3DPane::OnSmoothCheck)
	EVT_SLIDER (ID_CONTOUR_VALUE_SLIDER, General3DSurfPane::OnContourValueSld)
	EVT_TEXT (ID_CONTOUR_VALUE_EDIT, Surface3DPane::OnContourValueEnter)
	EVT_TEXT (ID_GENMULTEDIT, General3DSurfPane::OnMultValueEdit)
	EVT_BUTTON (ID_FREE_MEM_BUT, Surface3DPane::OnFreeMem)
	EVT_BUTTON (ID_SURFACE_EXPORT_BUT, BaseSurfacePane::OnExport)
	EVT_COMMAND_ENTER(ID_3D_COLOR_POSITIVE, Surface3DPane::OnPosColorChange)
	EVT_COMMAND_ENTER(ID_3D_COLOR_NEGATIVE, Surface3DPane::OnNegColorChange)
	EVT_COMMAND_ENTER(ID_TRANSPARENCY_COLOR, Surface3DPane::OnTranspColorChange)
	EVT_BUTTON (ID_SURFACE_UPDATE_BUT, General3DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( General2DSurfPane, wxPanel )
	EVT_BUTTON (ID_GENFILEBUTTON, General2DSurfPane::OnFileButton)
	EVT_CHECKBOX (ID_GENMULTCHECK, General2DSurfPane::OnMultCheck)
	EVT_CHECKBOX (ID_GENSQUARECHECK, General2DSurfPane::OnSquareCheck)
	EVT_TEXT (ID_CONTOUR_VALUE_EDIT, Surface2DPane::OnContourValueText)
	EVT_TEXT (ID_NUM_CONTOUR_TEXT, Surface2DPane::OnNumContoursText)
	EVT_TEXT (ID_GENMULTEDIT, General2DSurfPane::OnMultValueEnter)
	EVT_BUTTON (ID_SURFACE_EXPORT_BUT, BaseSurfacePane::OnExport)
	EVT_COMMAND_ENTER(ID_2D_COLOR_POSITIVE, Surface2DPane::OnPosColorChange)
	EVT_COMMAND_ENTER(ID_2D_COLOR_NEGATIVE, Surface2DPane::OnNegColorChange)
	EVT_CHECKBOX (ID_SHOW_ZERO_CHECKBOX, Surface2DPane::OnShowZeroChk)
	EVT_CHECKBOX (ID_DASH_CHECKBOX, Surface2DPane::OnDashChk)
	EVT_CHECKBOX (ID_DISPLAY_PLANE_CHECKBOX, Surface2DPane::OnDisplayPlaneChk)
	EVT_BUTTON (ID_SURFACE_UPDATE_BUT, General2DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( TEDensity2DSurfPane, wxPanel )
	EVT_SLIDER (ID_GRID_POINT_SLIDER, BaseSurfacePane::OnGridPointSld)
	EVT_CHOICE  (ID_ORB_CHOICE, BaseSurfacePane::OnOrbSetChoice)
	EVT_CHECKBOX (ID_USE_PLANE_CHECKBOX, Surface2DPane::OnUsePlaneChk)
	EVT_CHECKBOX (ID_DISPLAY_PLANE_CHECKBOX, Surface2DPane::OnDisplayPlaneChk)
	EVT_TEXT (ID_CONTOUR_VALUE_EDIT, Surface2DPane::OnContourValueText)
	EVT_TEXT (ID_NUM_CONTOUR_TEXT, Surface2DPane::OnNumContoursText)
	EVT_COMMAND_ENTER(ID_2D_COLOR_POSITIVE, Surface2DPane::OnPosColorChange)
	EVT_BUTTON (ID_SURFACE_EXPORT_BUT, BaseSurfacePane::OnExport)
	EVT_BUTTON (ID_SET_PARAM_BUT, Surface2DPane::OnSetParam)
	EVT_BUTTON (ID_SET_PLANE_BUT, Surface2DPane::OnSetPlane)
	EVT_BUTTON (ID_SURFACE_UPDATE_BUT, TEDensity2DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( TEDensity3DSurfPane, wxPanel )
	EVT_SLIDER (ID_GRID_POINT_SLIDER, BaseSurfacePane::OnGridPointSld)
	EVT_CHOICE  (ID_ORB_CHOICE, BaseSurfacePane::OnOrbSetChoice)
	EVT_SLIDER (ID_CONTOUR_VALUE_SLIDER, Surface3DPane::OnContourValueSld)
	EVT_TEXT (ID_CONTOUR_VALUE_EDIT, Surface3DPane::OnContourValueEnter)
	EVT_SLIDER (ID_GRID_SIZE_SLIDER, Surface3DPane::OnGridSizeSld)
	EVT_CHECKBOX (ID_TED3D_COLOR_SURF_CHECK, TEDensity3DSurfPane::OnUseMEPCheck)
	EVT_CHECKBOX (ID_USERGB_COLOR_CHECK, TEDensity3DSurfPane::OnRGBColorCheck)
	EVT_CHECKBOX (ID_INVERT_RGB_CHECK, TEDensity3DSurfPane::OnInvertRGBCheck)
	EVT_TEXT (ID_TED3D_MAX_MAP_EDIT, TEDensity3DSurfPane::OnMaxMEPValueText)
	EVT_COMMAND_ENTER(ID_3D_COLOR_POSITIVE, Surface3DPane::OnPosColorChange)
	EVT_COMMAND_ENTER(ID_3D_COLOR_NEGATIVE, Surface3DPane::OnNegColorChange)
	EVT_COMMAND_ENTER(ID_TRANSPARENCY_COLOR, Surface3DPane::OnTranspColorChange)
	EVT_RADIOBOX (ID_3D_RADIOBOX, Surface3DPane::On3DRadioBox)
	EVT_CHECKBOX (ID_SMOOTH_CHECKBOX, Surface3DPane::OnSmoothCheck)
	EVT_BUTTON (ID_SURFACE_EXPORT_BUT, BaseSurfacePane::OnExport)
	EVT_BUTTON (ID_SET_PARAM_BUT, Surface3DPane::OnSetParam)
	EVT_BUTTON (ID_FREE_MEM_BUT, Surface3DPane::OnFreeMem)
	EVT_BUTTON (ID_SURFACE_UPDATE_BUT, TEDensity3DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( MEP2DSurfPane, wxPanel )
	EVT_SLIDER (ID_GRID_POINT_SLIDER, BaseSurfacePane::OnGridPointSld)
	EVT_CHOICE  (ID_ORB_CHOICE, BaseSurfacePane::OnOrbSetChoice)
	EVT_CHECKBOX (ID_USE_PLANE_CHECKBOX, Surface2DPane::OnUsePlaneChk)
	EVT_CHECKBOX (ID_SHOW_ZERO_CHECKBOX, Surface2DPane::OnShowZeroChk)
	EVT_CHECKBOX (ID_DASH_CHECKBOX, Surface2DPane::OnDashChk)
	EVT_CHECKBOX (ID_DISPLAY_PLANE_CHECKBOX, Surface2DPane::OnDisplayPlaneChk)
	EVT_TEXT (ID_CONTOUR_VALUE_EDIT, Surface2DPane::OnContourValueText)
	EVT_TEXT (ID_NUM_CONTOUR_TEXT, Surface2DPane::OnNumContoursText)
	EVT_COMMAND_ENTER(ID_2D_COLOR_POSITIVE, Surface2DPane::OnPosColorChange)
	EVT_COMMAND_ENTER(ID_2D_COLOR_NEGATIVE, Surface2DPane::OnNegColorChange)
	EVT_BUTTON (ID_SURFACE_EXPORT_BUT, BaseSurfacePane::OnExport)
	EVT_BUTTON (ID_SET_PARAM_BUT, Surface2DPane::OnSetParam)
	EVT_BUTTON (ID_SET_PLANE_BUT, Surface2DPane::OnSetPlane)
	EVT_BUTTON (ID_SURFACE_UPDATE_BUT, MEP2DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( MEP3DSurfPane, wxPanel )
	EVT_SLIDER (ID_GRID_POINT_SLIDER, BaseSurfacePane::OnGridPointSld)
	EVT_CHOICE  (ID_ORB_CHOICE, BaseSurfacePane::OnOrbSetChoice)
	EVT_SLIDER (ID_CONTOUR_VALUE_SLIDER, Surface3DPane::OnContourValueSld)
	EVT_TEXT (ID_CONTOUR_VALUE_EDIT, Surface3DPane::OnContourValueEnter)
	EVT_SLIDER (ID_GRID_SIZE_SLIDER, Surface3DPane::OnGridSizeSld)
	EVT_COMMAND_ENTER(ID_3D_COLOR_POSITIVE, Surface3DPane::OnPosColorChange)
	EVT_COMMAND_ENTER(ID_3D_COLOR_NEGATIVE, Surface3DPane::OnNegColorChange)
	EVT_COMMAND_ENTER(ID_TRANSPARENCY_COLOR, Surface3DPane::OnTranspColorChange)
	EVT_RADIOBOX (ID_3D_RADIOBOX, Surface3DPane::On3DRadioBox)
	EVT_CHECKBOX (ID_SMOOTH_CHECKBOX, Surface3DPane::OnSmoothCheck)
	EVT_BUTTON (ID_SURFACE_EXPORT_BUT, BaseSurfacePane::OnExport)
	EVT_BUTTON (ID_SET_PARAM_BUT, Surface3DPane::OnSetParam)
	EVT_BUTTON (ID_FREE_MEM_BUT, Surface3DPane::OnFreeMem)
	EVT_BUTTON (ID_SURFACE_UPDATE_BUT, MEP3DSurfPane::OnUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( Surface2DParamDlg, wxFrame )
  EVT_BUTTON (wxID_OK, Surface2DParamDlg::OnClose)
  EVT_BUTTON (wxID_CANCEL, Surface2DParamDlg::OnCancel)
  EVT_BUTTON (ID_COPY_ALL, Surface2DParamDlg::OnCopyAll)
  EVT_BUTTON (ID_PASTE_ALL, Surface2DParamDlg::OnPasteAll)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE( Surface3DParamDlg, wxFrame )
  EVT_BUTTON (wxID_OK, Surface3DParamDlg::OnClose)
  EVT_BUTTON (wxID_CANCEL, Surface3DParamDlg::OnCancel)
  EVT_BUTTON (ID_COPY_ALL, Surface3DParamDlg::OnCopyAll)
  EVT_BUTTON (ID_PASTE_ALL, Surface3DParamDlg::OnPasteAll)
END_EVENT_TABLE()
/*!
 * Base class of any Panel
 */

BaseSurfacePane::BaseSurfacePane( wxWindow* parent, Surface* target, SurfacesWindow* p, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
	mTarget = target;
	owner = p;
  //mData = data;

	Visible = target->GetVisibility();
	AllFrames = (mTarget->GetSurfaceID() != 0);
	UpdateTest = false;

	Create(parent, id, pos, size, style);
}

BaseSurfacePane::~BaseSurfacePane()
{

}

bool BaseSurfacePane::Create( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
  wxPanel::Create( parent, id, pos, size, style );

  CreateControls();

  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
  Centre();

  return true;
}

/*
 * In base classes we just create objects
 * leave the adding operation to the final classes
 */

void BaseSurfacePane::CreateControls() {
	mainSizer = new wxBoxSizer(wxVERTICAL);

	SetSizer(mainSizer);
}

void BaseSurfacePane::PageIsNowActive(void) {
	if (mUpdateBut) mUpdateBut->SetDefault();
}

void BaseSurfacePane::OnGridPointSld( wxCommandEvent &event ) {
	NumGridPoints = mNumGridPntSld->GetValue();
	SwitchFixGrid = true;

	setUpdateButton();
}

void BaseSurfacePane::OnExport( wxCommandEvent &event ) {
	FILE *currFile = NULL;
	BufferFile *buffer = NULL;

	wxString filePath = wxFileSelector(wxT("Export As"), wxT(""), wxT(""), wxT(""),
									 wxT(""),
									 wxSAVE | wxOVERWRITE_PROMPT, owner);

	if(!filePath.IsEmpty()) {
		if((currFile = fopen(filePath.mb_str(wxConvUTF8), "wb")) == NULL) {
			MessageAlert("Unable to open the file for output.");
			return;
		}
		try {
			buffer = new BufferFile(currFile, true);
			
			mTarget->Export(buffer);
		}
		catch (...) {
			MessageAlert("Error attempting to write file.");
		}
		if(buffer) {
			delete buffer;
		}
		fclose(currFile);
	}
}

//Call to change the visibilty for the active surface
void BaseSurfacePane::SetVisibility(bool state) {
	Visible = state;

	setUpdateButton();
}

void BaseSurfacePane::setAllFrames(bool state) {
	AllFrames = state;

	setUpdateButton();
}

void BaseSurfacePane::SetUpdateTest(bool test) {
	UpdateTest = test;

	if (test)
		mUpdateBut->Enable();
	else
		mUpdateBut->Disable();

}

void BaseSurfacePane::setUpdateButton() {
	if (UpdateNeeded())
		mUpdateBut->Enable();
	else
		mUpdateBut->Disable();
}

OrbSurfacePane::OrbSurfacePane( OrbSurfBase* target, SurfacesWindow* o)
{
  mTarget = target;
  myowner = o;

  //initialize parameters
  TargetSet = mTarget->GetTargetSet();
  OrbOptions = mTarget->GetOptions();
  OrbColumnEnergyOrOccupation = mTarget->GetOrbOccDisplay();
  PlotOrb = target->GetTargetOrb();
  SphericalHarmonics = target->UseSphericalHarmonics();
  PhaseChange = target->GetPhaseChange();

  if ((TargetSet<0)||(!(OrbOptions&1))) 
    {	//default to something sensible
      MoleculeData * mData = myowner->GetMoleculeData();
      Frame * lFrame = mData->GetCurrentFramePtr();
      const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();

      if (Orbs->size() > 0) 
	{
	  TargetSet = 0;	//just default to the first set...
	} 
      else
	OrbOptions = 1;	//No MO's so default to AO's
    }

  coefIsEnabled = false;
  if (OrbOptions&1) coefIsEnabled = true;
}

OrbSurfacePane::~OrbSurfacePane()
{

}

bool OrbSurfacePane::UpdateEvent() 
{	//user clicked update button, so push the data to the surface

	bool updateGrid = false;
	if (PlotOrb != mTarget->GetTargetOrb()) 
	  {
	    mTarget->SetTargetOrb(PlotOrb);
	    updateGrid = true;
	  }
	if (TargetSet != mTarget->GetTargetSet()) 
	  {
	    mTarget->SetTargetSet(TargetSet);
	    updateGrid = true;
	  }
	if (OrbOptions != mTarget->GetOptions()) 
	  {
	    mTarget->SetOptions(OrbOptions);
	    updateGrid = true;
	  }
	if (OrbColumnEnergyOrOccupation != mTarget->GetOrbOccDisplay()) 
	  {
			//This is just a display flag for the MO column so no
			//grid or contour updates are needed.
	    mTarget->SetOrbOccDisplay(OrbColumnEnergyOrOccupation);
	  }
	if (SphericalHarmonics != mTarget->UseSphericalHarmonics()) 
	  {
	    mTarget->UseSphericalHarmonics(SphericalHarmonics);
	    updateGrid = true;
	  }

	return updateGrid;
}

int OrbSurfacePane::getOrbSetForOrbPane(vector<wxString>& choice)
{
	MoleculeData * mData = myowner->GetMoleculeData();
	Frame * lFrame = mData->GetCurrentFramePtr();
	const vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();

  short item, numitems;

  choice.push_back(_T("Atomic Orbitals"));
  item = 1; numitems = 1;

  if (Orbs->size() > 0) 
    {
      std::vector<OrbitalRec *>::const_iterator OrbSet = Orbs->begin();
      long	OrbSetCount = 0;
      while (OrbSet != Orbs->end()) 
	{
	  if (TargetSet == OrbSetCount) item = numitems + 1;

	  if (((*OrbSet)->getOrbitalWavefunctionType() == UHF)&&
	      (!((*OrbSet)->getOrbitalType() == NaturalOrbital))) 
	    {
	      if ((*OrbSet)->getOrbitalType() == LocalizedOrbital) 
		{
		  choice.push_back(_T("Alpha Spin Localized Orbitals"));
		  choice.push_back(_T("Beta Spin Localized Orbitals"));
		} 
	      else if ((*OrbSet)->getOrbitalType() == LocalizedOrbital) 
		{
		  choice.push_back(_T("Alpha Spin Oriented Localized Orbitals"));
		  choice.push_back(_T("Beta Spin Oriented Localized Orbitals"));
		} 
	      else if ((*OrbSet)->getOrbitalType() == GuessOrbital) 
		{
		  choice.push_back(_T("Alpha Spin Initial Guess Orbitals"));
		  choice.push_back(_T("Beta Spin Initial Guess Orbitals"));
		} 
	      else 
		{
		  choice.push_back(_T("Alpha Spin Orbitals"));
		  choice.push_back(_T("Beta Spin Orbitals"));
		}
	      numitems++;	//increment count for two sets, other increment is done below
				
	      if ((TargetSet == OrbSetCount)&&(OrbOptions & 16)) 
		item = numitems+1;	//target beta set
	    } 
	  else 
	    {
	      switch ((*OrbSet)->getOrbitalType()) 
		{
		case OptimizedOrbital:
		  if ((*OrbSet)->getOrbitalWavefunctionType() == MCSCF)
		    choice.push_back(_T("MCSCF Optimized Orbitals"));
		  else
		    choice.push_back(_T("Molecular EigenVectors"));
		  break;

		case NaturalOrbital:
		  switch ((*OrbSet)->getOrbitalWavefunctionType()) 
		    {
		    case UHF:
		      choice.push_back(_T("UHF Natural Orbitals"));
		      break;
		    case GVB:
		      choice.push_back(_T("GVB GI Orbitals"));
		      break;
		    case MCSCF:
		      choice.push_back(_T("MCSCF Natural Orbitals"));
		      break;
		    case CI:
		      choice.push_back(_T("CI Natural Orbitals"));
		      break;
		    case RHFMP2:
		      choice.push_back(_T("RMP2 Natural Orbitals"));
		      break;
		    default:
		      choice.push_back(_T("Natural Orbitals"));
		    }
		  break;

		case LocalizedOrbital:
		  choice.push_back(_T("Localized Orbitals"));
		  break;
		case OrientedLocalizedOrbital:
		  choice.push_back(_T("Oriented Localized Orbitals"));
		  break;
		case GuessOrbital:
		  choice.push_back(_T("Initial Guess Orbitals"));
		  break;
		default:
		  choice.push_back(_T("Molecular Orbitals"));
		}
	    }
	  numitems++;
	  OrbSetCount++;
	  OrbSet++;
	}
	}

  return item;
}

void OrbSurfacePane::makeMOList(vector<wxString>& choice)
{
	MoleculeData * mData = myowner->GetMoleculeData();
	Frame * lFrame = mData->GetCurrentFramePtr();
	const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();

	wxString tmpStr;

	if (lFrame && (Orbs->size() > 0)) {
		OrbitalRec * lMOs = NULL;

		if ((TargetSet < Orbs->size())&&(TargetSet >= 0)) 
			lMOs = (*Orbs)[TargetSet];

		if (lMOs) {
			bool Alpha = !(OrbOptions & 16);
			long	NumMOs;
			char * SymLabel;
			float * Energy;
			float * OccNum;

			if (Alpha) {
				NumMOs=lMOs->NumAlphaOrbs;
				SymLabel = lMOs->SymType;
				Energy = lMOs->Energy;
				OccNum = lMOs->OrbOccupation;
			} else {
				NumMOs=lMOs->NumBetaOrbs;
				SymLabel = lMOs->SymTypeB;
				Energy = lMOs->EnergyB;
				OccNum = lMOs->OrbOccupationB;
			}

			char* oneSymLabel;

			for (int theCell = 0; theCell < NumMOs; theCell++) {
				tmpStr.Printf(wxT("%d  "), theCell+1); 

				if (SymLabel) {	//Add the symetry of the orb, if known
		
					oneSymLabel = &(SymLabel[theCell*5]);	
					//offset to the label for this orb

					for (int ichar=0; ichar<4; ichar++) {
						if ((oneSymLabel[ichar])=='\0') break;

						if ((oneSymLabel[ichar]>='0')&&(oneSymLabel[ichar]<='9')) {
							tmpStr.Append(oneSymLabel[ichar]);
						} else if ((oneSymLabel[ichar]=='U')||(oneSymLabel[ichar]=='u')) {
							tmpStr.Append('u');
						} else if ((oneSymLabel[ichar]=='G')||(oneSymLabel[ichar]=='g')) {
							tmpStr.Append('g');
						} else
							tmpStr.Append(oneSymLabel[ichar]);
					}
				//TextSize(12);	//reset the point size
				}
				int nchar = tmpStr.Length();

				for ( int ichar = 0; ichar < 12 - nchar; ichar++)
				  tmpStr.Append(' ');

				wxString temp;
				if (OrbColumnEnergyOrOccupation) {	//orb occupation selected
		
					if (OccNum)
						temp.Printf(wxT("%.3f"), OccNum[theCell]);
					else {	
						//attempt to set the occupation based on the wavefunction type
						if (lMOs->getOrbitalWavefunctionType() == RHF) {
							if (theCell<lMOs->getNumOccupiedAlphaOrbitals()) 
								temp.Printf(wxT("2  "));
							else 
								temp.Printf(wxT("0  "));
						} else if (lMOs->getOrbitalWavefunctionType() == ROHF) {
							temp.Printf(wxT("0  "));
							if (theCell<lMOs->getNumOccupiedBetaOrbitals()) 
								temp.Printf(wxT("2  "));
							else if (theCell<lMOs->getNumOccupiedAlphaOrbitals()) 
								temp.Printf(wxT("1  "));
						} else if (lMOs->getOrbitalWavefunctionType() == UHF) {
							temp.Printf(wxT("0  "));
							if (Alpha) {
								if (theCell<lMOs->getNumOccupiedAlphaOrbitals()) 
									temp.Printf(wxT("1  "));
							} else if (theCell<lMOs->getNumOccupiedBetaOrbitals()) 
								temp.Printf(wxT("1  "));
						} else {	//MCSCF or CI occupations can't be guessed
							temp.Printf(wxT("?? "));
						}
					}
				} else if (Energy) {	//punch out the orb energy
					temp.Printf(wxT("%.3f"), Energy[theCell]);
				}

				tmpStr.Append(temp);
				choice.push_back(tmpStr);
			}
		}
	}
}

void OrbSurfacePane::makeAOList(vector<wxString>& choice)
{
  wxString aChoice;

  MoleculeData * mData = myowner->GetMoleculeData();
  BasisSet * BasisPtr = mData->GetBasisSet();

  if (BasisPtr) 
    {
      long NumBasisFuncs = BasisPtr->GetNumBasisFuncs(SphericalHarmonics);

      for (int theCell = 0; theCell < NumBasisFuncs; theCell++) 
	{
	  aChoice.Clear();
	  long ifunc = 0, iatom=0;
	  Frame * lFrame = mData->GetCurrentFramePtr();

	  while (ifunc<=theCell) 
	    {
	      long minshell = BasisPtr->BasisMap[2*iatom];
	      long maxshell = BasisPtr->BasisMap[2*iatom+1];

	      for (long ishell=minshell;ishell<=maxshell;ishell++) 
		{
		  long jfunc = ifunc+(BasisPtr->Shells[ishell]).GetNumFuncs(SphericalHarmonics);
		  if (theCell<jfunc) 
		    {	//Found the right shell, now pick out the right function
		      char label[63];
		      wxString tmpStr;
		      int nchar = 0;
		      //punch out the atom # and symbol if this is the 1st function for this atom

		      if ((ishell==minshell)&&(theCell==ifunc))
			{
			  sprintf(label, "%ld", iatom+1);
				aChoice.Printf(wxT("%ld"), iatom+1);

			  nchar = aChoice.Length();

			  for ( int i = 0; i < 4 - nchar; i++)
			    aChoice.Append(' ');

			  WinPrefs * mPrefs = myowner->GetPrefs();
			  if (mPrefs) 
			    {
			      long AtomType = lFrame->GetAtomType(iatom)-1;
			      mPrefs->GetAtomLabel(AtomType, tmpStr);
			      tmpStr.Trim();
			      aChoice.Append(tmpStr);
			    }
			}

		      nchar = aChoice.Length();

		      for ( int i = 0; i < 11 - nchar; i++)
			aChoice.Append(' ');

		      if (nchar == 0)
			aChoice.Append(' '); //compensate one space

		      jfunc = theCell-ifunc;
		      BasisPtr->Shells[ishell].GetLabel(label, jfunc, SphericalHarmonics);
		      nchar = strlen(label);

		      if (nchar>0)
			{	//Make sure there really is something there
			  //aChoice.Append(' ');
			  //aChoice.Append(' ');
			  wxString ltemp(label, wxConvUTF8);
			  aChoice.Append(ltemp);

			  for (long ichar=1; ichar<=nchar; ichar++)
			    {
			      char isExponent = ((label[ichar]>='0')&&
						 (label[ichar]<='9')&&
						 (label[ichar-1]>='r')&&
						 (label[ichar-1]<='z'));
			      if (isExponent)
				{
				      //adjust vertical position
				}
				
				  //DrawText(label, ichar, 1);
			    }
			}

		      for ( int i = 0; i < 6-nchar; i++)
			aChoice.Append(' ');

		      if (nchar <= 2)
			aChoice.Append(' '); //compensate one space

		      ifunc = theCell+1;
		      ishell=maxshell;
		    }
		  else
		    ifunc = jfunc;
		}
	      iatom++;
	    }
	  
	  if (PlotOrb>=0) 
	    {	//Is an MO selected?
	      if (!(OrbOptions&1)) 
		{	//If not displaying AO's
		  const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();
		  if (Orbs->size() > 0) 
		    {
		      OrbitalRec *MOs = NULL;
		      if ((TargetSet < Orbs->size())&&(TargetSet >= 0)) 
			MOs = (*Orbs)[TargetSet];

		      if (MOs) 
			{
			  float * aVector=NULL;

			  if (OrbOptions & 16) 
			    {
			      if (PlotOrb<MOs->NumBetaOrbs)
				aVector = &(MOs->VectorsB[NumBasisFuncs*PlotOrb]);
			    } 
			  else 
			    {
			      if (PlotOrb<MOs->NumAlphaOrbs)
				aVector = &(MOs->Vectors[NumBasisFuncs*PlotOrb]);
			    }

			  if (aVector) 
			    {
				  wxString label;

			      label.Printf(wxT("%.3f"), aVector[theCell]);//prepare the coef for printing
			      aChoice.Append(label);
			    }
			}
		    }
		}
	    }
	  choice.push_back(aChoice);
	}
    }
}

void OrbSurfacePane::setFlagOnOrbFormatChange(int itemtype)
{
  if (OrbColumnEnergyOrOccupation) OrbColumnEnergyOrOccupation = false;
  if (itemtype) OrbColumnEnergyOrOccupation = true;
}

int OrbSurfacePane::orbSetChangeEvt(int item, SurfacesWindow * owner)
{
  int itemtype=0;

  if (item <= 1) 
    {
      if (!(OrbOptions&1)) 
	{
	  OrbOptions = 1; itemtype=1;
	}
    } 
  else 
    {
      short numitems=1;
      OrbOptions = 0;	//turn off AOs and alpha/beta flags
      MoleculeData * mData = owner->GetMoleculeData();
      Frame * lFrame = mData->GetCurrentFramePtr();

      const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();

      if (Orbs->size() > 0) 
	{
	  vector<OrbitalRec *>::const_iterator OrbSet = Orbs->begin();
	  long	OrbSetCount = 0;

	  while (OrbSet != Orbs->end()) 
	    {
	      numitems++;
	      if (numitems == item) 
		{
		  TargetSet = OrbSetCount;
		  itemtype = 1;
		  break;
		}

	      if (((*OrbSet)->getOrbitalWavefunctionType() == UHF)&&
		  (!((*OrbSet)->getOrbitalType() == NaturalOrbital))) 
		{
		  numitems++;	//Extra increment for the beta set
		  if (numitems == item) 
		    {
		      TargetSet = OrbSetCount;
		      OrbOptions = 16;	//beta set selected
		      itemtype = 1;
		      break;
		    }
		}
	      OrbSetCount++;
	      OrbSet++;
	    }
	}
    }		
			
  return itemtype;
}

/*
 * General 2D surface dialog class
 */

Surface2DPane::Surface2DPane( wxWindow* parent, Surf2DBase* target, 
			      SurfacesWindow* Owner, wxWindowID id, 
			      const wxPoint& pos, const wxSize& size, 
			      long style ) 
  : BaseSurfacePane(parent, target, Owner, id, pos, size, style) {
	mOrbColor1 = NULL;
	mOrbColor2 = NULL;

	mTarget = target;
}

Surface2DPane::~Surface2DPane()
{
  delete mOrbColor1;
  if (mOrbColor2) delete mOrbColor2;
}

void Surface2DPane::OnPosColorChange(wxCommandEvent & event) {
	mOrbColor1->getColor(&PosColor);
	setUpdateButton();
}
void Surface2DPane::OnNegColorChange(wxCommandEvent & event) {
	mOrbColor2->getColor(&NegColor);
	setUpdateButton();
}

/* if the focus in the panel is changed between main panel and 
   two wxTextCtrls, read the content of two wxTextCtrls. */

void Surface2DPane::OnIdle( wxIdleEvent& WXUNUSED(event) ) {
	static wxWindow *s_windowFocus = (wxWindow *)NULL;
	wxWindow *focus = wxWindow::FindFocus();

	if ( focus && (focus != s_windowFocus) ) {
		s_windowFocus = focus;
		wxString className = s_windowFocus->GetClassInfo()->GetClassName();

		if (className.Cmp(_T("wxTextCtrl")) == 0 
			|| className.Cmp(_T("Orbital2DSurfPane")) == 0 ) {
			long t;
			if ((mNumContourText->GetValue()).ToLong(&t))
				NumContours = t;
			double d;
			if ((mContourValText->GetValue()).ToDouble(&d))
				MaxContourValue = d;
			setUpdateButton();
		}
	}
}

void Surface2DPane::OnDashChk(wxCommandEvent& event ) {
	DashLines = mDashCheck->GetValue();
	setUpdateButton();
}

void Surface2DPane::OnShowZeroChk( wxCommandEvent &event )
{
  ShowZeroContour = mShowZeroCheck->GetValue();
  setUpdateButton();
}
void Surface2DPane::OnDisplayPlaneChk( wxCommandEvent &event )
{
	DisplayPlane = mDisplayPlaneCheck->GetValue();
	setUpdateButton();
}

void Surface2DPane::OnSetPlane( wxCommandEvent &event )
{
  SetScreenPlane * temp = new SetScreenPlane(owner->GetParent());
  temp->Show();
}

void Surface2DPane::OnUsePlaneChk( wxCommandEvent &event )
{
	UseScreenPlane = mUsePlaneChk->GetValue();
	setUpdateButton();
}

void Surface2DPane::OnSetParam( wxCommandEvent &event )
{
  Surface2DParamDlg* paramDlg = new Surface2DParamDlg(this, mTarget);
  paramDlg->Show();
}
void Surface2DPane::OnContourValueText(wxCommandEvent& event )
{	//Contour Value edit text has changed, convert the text
	double newVal=0.0;
	
	wxString tmpStr = mContourValText->GetValue();
	if (tmpStr.ToDouble(&newVal)) {
		if (newVal > 0.0) MaxContourValue = newVal;
	}
	setUpdateButton();
}
void Surface2DPane::SetContourValueText(void)
{
	wxString tmpStr;
	tmpStr.Printf(wxT("%.4f"), MaxContourValue);
	mContourValText->SetValue(tmpStr);
}
void Surface2DPane::OnNumContoursText(wxCommandEvent& event )
{	//Num contours edit text has changed, convert the text
	long newVal=-1;
	
	wxString tmpStr = mNumContourText->GetValue();
	if (tmpStr.ToLong(&newVal)) {
		if (newVal > 0) NumContours = newVal;
	}
	
	setUpdateButton();
}
void Surface2DPane::SetNumContoursText(void)
{
	wxString tmpStr;
	tmpStr.Printf(wxT("%ld"), NumContours);
	mNumContourText->SetValue(tmpStr);
}

/*
 * General 3D surface dialog class
 */

Surface3DPane::Surface3DPane( wxWindow* parent, Surf3DBase* target, 
			      SurfacesWindow* Owner, wxWindowID id,
			      const wxPoint& pos, const wxSize& size, 
			      long style ) 
  : BaseSurfacePane(parent, target, Owner, id, pos, size, style)
{
  mTarget = target;
  UseSolidSurface = mTarget->SolidSurface();
  UseNormals = mTarget->UseSurfaceNormals();
}

Surface3DPane::~Surface3DPane()
{
  delete mOrbColor1;
  delete mOrbColor2;
  delete mTransColor;
}

void Surface3DPane::setContourValueSld()
{
  float GridMax = mTarget->GetGridMax();
  mContourValSld->SetValue((short)(100*(ContourValue/((fabs(GridMax)>=0.001)?GridMax:0.25))));

}

void Surface3DPane::On3DRadioBox (wxCommandEvent& event )
{
  UseSolidSurface = 1-m3DRdoBox->GetSelection();

  if (UseSolidSurface)
    mSmoothChkBox->Enable();
  else
    mSmoothChkBox->Disable();

  setUpdateButton();
}

void Surface3DPane::OnSmoothCheck (wxCommandEvent& event )
{
  UseNormals = mSmoothChkBox->GetValue();

  setUpdateButton();
}

void Surface3DPane::changeContourValue() {
	double newVal;

	if (mTarget->GetGridMax() > 0.000001 ){
		wxString tmpStr = mContourValueEdit->GetValue();
		if (tmpStr.ToDouble(&newVal)) {
			if (newVal < 0.0) newVal *= -1.0;
			if (newVal > mTarget->GetGridMax()) 
				newVal = mTarget->GetGridMax();

			ContourValue = newVal;
			setContourValueSld();
			setUpdateButton();
		}
	}
}

void Surface3DPane::OnContourValueEnter(wxCommandEvent& event )
{
	changeContourValue();
}
void Surface3DPane::SetContourValueText(void)
{
	wxString tmpStr;
	tmpStr.Printf(wxT("%.4f"), ContourValue);
	mContourValueEdit->SetValue(tmpStr);
}
void Surface3DPane::SetContourMaxValueText(void)
{
	wxString tmpStr;
	tmpStr.Printf(wxT("%.4f"), mTarget->GetGridMax());
	mGridMaxText->SetLabel(tmpStr);
}
void Surface3DPane::OnContourValueSld(wxCommandEvent &event )
{
	float GridMax = mTarget->GetGridMax();
	ContourValue = 0.01 * mContourValSld->GetValue() * ((fabs(GridMax)>=0.001)?GridMax:0.25);
	
	SetContourValueText();
	
	setUpdateButton();
}
void Surface3DPane::OnGridSizeSld(wxCommandEvent &event )
{
	GridSize = 0.01 * mGridSizeSld->GetValue();
	setUpdateButton();
}

void Surface3DPane::OnIdle( wxIdleEvent& WXUNUSED(event) )
{
  static wxWindow *s_windowFocus = (wxWindow *)NULL;
  wxWindow *focus = wxWindow::FindFocus();

  if ( focus && (focus != s_windowFocus) )
    {
      s_windowFocus = focus;
      wxString className = s_windowFocus->GetClassInfo()->GetClassName();

      if (className.Cmp(_T("wxTextCtrl")) == 0 
	  || className.Cmp(_T("Orbital3DSurfPane")) == 0 )
	{
	  changeContourValue();
	  setUpdateButton();
	}
    }

}

void Surface3DPane::OnFreeMem(wxCommandEvent& event )
{
  mTarget->FreeGrid();
  mFreeMemBut->Disable();
}

void Surface3DPane::OnPosColorChange(wxCommandEvent & event) {
	mOrbColor1->getColor(&PosColor);
	setUpdateButton();
}
void Surface3DPane::OnNegColorChange(wxCommandEvent & event) {
	mOrbColor2->getColor(&NegColor);
	setUpdateButton();
}
void Surface3DPane::OnTranspColorChange(wxCommandEvent & event) {
	mTransColor->getColor(&TranspColor);
	setUpdateButton();
}

void Surface3DPane::OnSetParam( wxCommandEvent &event )
{
  Surface3DParamDlg* paramDlg = new Surface3DParamDlg(this, mTarget);
  paramDlg->Show();
}

/*!
 * Orbital2D class
 */

Orbital2DSurfPane::Orbital2DSurfPane( wxWindow* parent, Orb2DSurface* target, 
				      SurfacesWindow* o, wxWindowID id, 
				      const wxPoint& pos, const wxSize& size, 
				      long style ) 
  : Surface2DPane(parent, target, o, id, pos, size, style), OrbSurfacePane(target, o)
{
  mTarget = target;

  TargetToPane();
  CreateControls();
}

Orbital2DSurfPane::~Orbital2DSurfPane()
{

}

void Orbital2DSurfPane::CreateControls()
{
	//Stuff from the base class creation
	upperSizer = new wxBoxSizer(wxHORIZONTAL);
	middleSizer = new wxBoxSizer(wxHORIZONTAL);
	bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	leftMiddleSizer = new wxBoxSizer(wxVERTICAL);
	upperLeftMiddleSizer = new wxBoxSizer(wxHORIZONTAL);
	lowerLeftMiddleSizer = new wxBoxSizer(wxHORIZONTAL);
	rightMiddleSizer = new wxFlexGridSizer(2,2,0,0);
	leftBottomSizer = new wxBoxSizer(wxHORIZONTAL);
	rightBottomSizer = new wxBoxSizer(wxVERTICAL);
	
	wxStaticText * label0 = new wxStaticText( this, wxID_ANY,
						  _T("Select Orbital Set:"),
						  wxDefaultPosition,
						  wxDefaultSize);
	
	wxStaticText * label1 = new wxStaticText( this, wxID_ANY,
						  _T("Number of\n Grid Points:"),
						  wxDefaultPosition,
						  wxDefaultSize);
	
	mNumGridPntSld = new wxSlider( this, ID_GRID_POINT_SLIDER, 
				       0, 10, 150,
				       wxDefaultPosition, wxSize(155,wxDefaultCoord),
				       wxSL_AUTOTICKS | wxSL_LABELS);
	//set the initial value in the child object
	
	mSetParamBut = new wxButton( this, ID_SET_PARAM_BUT, wxT("Set Parameters"), wxPoint(450, 160) );
	mExportBut = new wxButton( this, ID_SURFACE_EXPORT_BUT, wxT("Export"), wxPoint(450, 160) );

	if (mTarget->ExportPossible())
	  mExportBut->Enable();
	else
	  mExportBut->Disable();

	mUpdateBut = new wxButton( this, ID_SURFACE_UPDATE_BUT, wxT("Update"), wxPoint(450, 160) );
	upperSizer->Add(label0, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

	mNumContourLabel = new wxStaticText( this, wxID_ANY,
					     _T("Max # of contours:"),
					     wxDefaultPosition,
					     wxDefaultSize);
	mContourValLabel = new wxStaticText( this, wxID_ANY,
					     _T("Max contour value:"),
					     wxDefaultPosition,
					     wxDefaultSize);
	
	mNumContourText = new wxTextCtrl( this, ID_NUM_CONTOUR_TEXT, _T(""), wxDefaultPosition, wxDefaultSize);
	
	mContourValText = new wxTextCtrl( this, ID_CONTOUR_VALUE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize);
	
	mShowZeroCheck = new wxCheckBox( this, ID_SHOW_ZERO_CHECKBOX, _T("Show zero contour"), wxPoint(340,130), wxDefaultSize );
	
	mDashCheck = new wxCheckBox( this, ID_DASH_CHECKBOX, _T("Dash - contour"), wxPoint(340,130), wxDefaultSize );
	
	mSetPlaneBut = new wxButton( this, ID_SET_PLANE_BUT, wxT("Set Plane"));
	
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	
	mOrbColor1 = new colorArea(this, ID_2D_COLOR_POSITIVE, &PosColor);
	mOrbColor2 = new colorArea(this, ID_2D_COLOR_NEGATIVE, &NegColor);
	
  vector<wxString> choices;
  int itemSelect = getOrbSetForOrbPane(choices) - 1;

  mOrbSetChoice = new wxChoice( this, ID_ORB_CHOICE, wxPoint(10,10), wxSize(200,wxDefaultCoord), choices.size(), &choices.front() );
  mOrbSetChoice->SetSelection(itemSelect);
  upperSizer->Add(mOrbSetChoice, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSphHarmonicsChk = new wxCheckBox( this, ID_SPH_HARMONICS_CHECKBOX, _T("Spherical Harmonics"), wxDefaultPosition, wxDefaultSize );

  wxString choices1[] = {_T("Energy"), _T("Occupation #")};
  mOrbFormatChoice = new wxChoice( this, ID_ORB_FORMAT_CHOICE, wxDefaultPosition, wxSize(120,wxDefaultCoord), 2, choices1 );
  mOrbFormatChoice->SetSelection(0);
  
  vector<wxString> choices2;
  makeMOList(choices2);

  vector<wxString> choices3;
  makeAOList(choices3);

  mAtomList = new wxListBox( this, ID_ATOM_LIST,
                             wxDefaultPosition, wxSize(150,180),
                             choices2.size(), &choices2.front(), 
			     wxLB_SINGLE |wxLB_ALWAYS_SB );

  mOrbCoef = new wxListBox( this, ID_ORB_COEF, wxPoint(20,160), wxSize(170,200), choices3.size(), &choices3.front(), wxLB_SINGLE |wxLB_ALWAYS_SB);

  mSphHarmonicsChk->SetValue(SphericalHarmonics);
  upperLeftMiddleSizer->Add(mSphHarmonicsChk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  lowerLeftMiddleSizer->Add(label1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  lowerLeftMiddleSizer->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mNumGridPntSld->SetValue(NumGridPoints);

  leftMiddleSizer->Add(upperLeftMiddleSizer, 0, wxALL, 3);
  leftMiddleSizer->Add(lowerLeftMiddleSizer, 0, wxALL, 3);

  wxString tmpStr;

  rightMiddleSizer->Add(mNumContourLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

  tmpStr.Printf(wxT("%ld"), NumContours);
  mNumContourText->SetValue(tmpStr);
  rightMiddleSizer->Add(mNumContourText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

  rightMiddleSizer->Add(mContourValLabel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

  tmpStr.Printf(wxT("%.2f"), MaxContourValue);
  mContourValText->SetValue(tmpStr);
  rightMiddleSizer->Add(mContourValText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

  mSubLeftBot1Sizer = new wxBoxSizer(wxVERTICAL);
  mSubLeftBot2Sizer = new wxBoxSizer(wxVERTICAL);
  
  mSubLeftBot1Sizer->Add(new wxStaticText(this, wxID_ANY,
                            _T("Select Orb:"),
                            wxDefaultPosition,
			    wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSubLeftBot1Sizer->Add(mOrbFormatChoice, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSubLeftBot1Sizer->Add(mAtomList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSubLeftBot2Sizer->Add(new wxStaticText(this, wxID_ANY,
                         _T("Orbital vector: \nAtom Orbital Coef"),
                         wxDefaultPosition,
                         wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  mSubLeftBot2Sizer->Add(mOrbCoef, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  leftBottomSizer->Add(mSubLeftBot1Sizer, 0, wxALL, 5);
  leftBottomSizer->Add(mSubLeftBot2Sizer, 0, wxALL, 5);

  mSubRightBot1Sizer = new wxBoxSizer(wxVERTICAL);
  mUsePlaneChk = new wxCheckBox(this, ID_USE_PLANE_CHECKBOX, _T("Use plane of screen"), wxDefaultPosition);
  mUsePlaneChk->SetValue(UseScreenPlane);
  mDisplayPlaneCheck = new wxCheckBox( this, ID_DISPLAY_PLANE_CHECKBOX, _("Display Plotting Plane"), wxDefaultPosition, wxDefaultSize, 0 );
  mDisplayPlaneCheck->SetValue(DisplayPlane);
  mDisplayPlaneCheck->SetToolTip(_("Display a translucent plane with border to indicate the plotting plane"));
  
  mRevPhaseChk = new wxCheckBox(this, ID_REVERSE_PHASE_CHECKBOX, _T("Reverse Phase"), wxDefaultPosition);
  mRevPhaseChk->SetValue(PhaseChange);
  mDashCheck->SetValue(DashLines);
  mShowZeroCheck->SetValue(ShowZeroContour);

  mSubRightBot1Sizer->Add(mUsePlaneChk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSubRightBot1Sizer->Add(mDisplayPlaneCheck, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSubRightBot1Sizer->Add(mShowZeroCheck, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSubRightBot1Sizer->Add(mDashCheck, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSubRightBot1Sizer->Add(mRevPhaseChk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSubRightBot2Sizer = new wxBoxSizer(wxHORIZONTAL);
  mSubRightBot2Sizer->Add(new wxStaticText(this, wxID_ANY,
                            _T("Orbital\n Colors:"),
                            wxDefaultPosition,
			    wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 8);
  mSubRightBot2Sizer->Add(mOrbColor1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 8);
  mSubRightBot2Sizer->Add(mOrbColor2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 8);

  mSubRightBot3Sizer = new wxGridSizer(2,2,0,0);
  mSubRightBot3Sizer->Add(mSetParamBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSubRightBot3Sizer->Add(mSetPlaneBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSubRightBot3Sizer->Add(mExportBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSubRightBot3Sizer->Add(mUpdateBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mUpdateBut->SetDefault();
  
  rightBottomSizer->Add(mSubRightBot1Sizer);
  rightBottomSizer->Add(mSubRightBot2Sizer);
  rightBottomSizer->Add(mSubRightBot3Sizer);

  middleSizer->Add(leftMiddleSizer, 0, wxALL, 10);
  middleSizer->Add(rightMiddleSizer, 0, wxALL, 10);
  bottomSizer->Add(leftBottomSizer, 0, wxALL, 3);
  bottomSizer->Add(rightBottomSizer, 0, wxALL, 3);
  mainSizer->Add(upperSizer);
  mainSizer->Add(middleSizer);
  mainSizer->Add(bottomSizer);
  
  wxCommandEvent foo;
  OnOrbSetChoice(foo);
}

void Orbital2DSurfPane::TargetToPane(void) 
{
  NumGridPoints = mTarget->GetNumGridPoints();
  NumContours = mTarget->GetNumContours();
  MaxContourValue = mTarget->GetMaxValue();
  mTarget->GetPosColor(&PosColor);
  mTarget->GetNegColor(&NegColor);
  ShowZeroContour = mTarget->GetShowZeroContour();
  UseScreenPlane = mTarget->GetRotate2DMap();
  Visible = mTarget->GetVisibility();
  DashLines = mTarget->GetDashLine();
  DisplayPlane = mTarget->ShowPlottingPlane();
  UpdateTest = false;
}

bool Orbital2DSurfPane::UpdateNeeded(void) {
	bool result = false;

	if (PlotOrb >= 0) {	//Don't update unless a valid orbital is chosen
		if (PlotOrb != mTarget->GetTargetOrb()) result=true;
		if (Visible != mTarget->GetVisibility()) result = true;
		if (AllFrames != (mTarget->GetSurfaceID() != 0)) result = true;
		if (TargetSet != mTarget->GetTargetSet()) result = true;
		if (OrbOptions != mTarget->GetOptions()) result = true;
		if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
		if (NumContours != mTarget->GetNumContours()) result = true;
		if (MaxContourValue != mTarget->GetMaxValue()) result = true;
		if (ShowZeroContour != mTarget->GetShowZeroContour()) result = true;
		if (UseScreenPlane != mTarget->GetRotate2DMap()) result = true;
		if (DashLines != mTarget->GetDashLine()) result = true;
		if (PhaseChange != mTarget->GetPhaseChange()) result = true;
		if (DisplayPlane != mTarget->ShowPlottingPlane()) result = true;
		if (SphericalHarmonics != mTarget->UseSphericalHarmonics()) result = true;

		if (!result) {
			RGBColor	testColor;
			mTarget->GetPosColor(&testColor);
			if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)
				||(PosColor.blue!=testColor.blue)) result=true;
			mTarget->GetNegColor(&testColor);
			if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)||
				(NegColor.blue!=testColor.blue)) result=true;
		}
	}
	return result;
}

void Orbital2DSurfPane::OnUpdate(wxCommandEvent &event ) 
{
	SetNumContoursText();
	SetContourValueText();

	bool updateGrid = UpdateTest;
  MoleculeData * data = owner->GetMoleculeData();

  if (NumGridPoints != mTarget->GetNumGridPoints()) 
    {
      updateGrid = true;
      mTarget->SetNumGridPoints(NumGridPoints);
    }

  mTarget->SetVisibility(Visible);
  mTarget->SetNumContours(NumContours);
  mTarget->SetMaxValue(MaxContourValue);
  mTarget->SetPosColor(&PosColor);
  mTarget->SetNegColor(&NegColor);
  mTarget->SetShowZeroContour(ShowZeroContour);
  mTarget->ShowPlottingPlane(DisplayPlane);
  if (UseScreenPlane && !mTarget->GetRotate2DMap()) updateGrid = true;
  mTarget->SetRotate2DMap(UseScreenPlane);
  mTarget->SetDashLine(DashLines);
  if (SphericalHarmonics != mTarget->UseSphericalHarmonics()) {
	  mTarget->UseSphericalHarmonics(SphericalHarmonics);
	  updateGrid = true;
  }
  if (PhaseChange != mTarget->GetPhaseChange()) {
    updateGrid = true;
    mTarget->SetPhaseChange(PhaseChange);
  }
  if (OrbSurfacePane::UpdateEvent()) updateGrid = true;
  if (AllFrames != (mTarget->GetSurfaceID() != 0)) 
    {	//update all frames
      long	SurfaceID;
      Frame *	lFrame = data->GetFirstFrame();
      if (AllFrames) 
	{	//adding the surface to all frames
	  SurfaceID = mTarget->SetSurfaceID();
	  while (lFrame) 
	    {
	      if (lFrame != data->GetCurrentFramePtr()) 
		{
		  Orb2DSurface * NewSurface = new Orb2DSurface(mTarget);
		  lFrame->AppendSurface(NewSurface);
		}
	      lFrame = lFrame->GetNextFrame();
	    }
	} 
      else 
	{			//deleting the surface from other frames
	  SurfaceID = mTarget->GetSurfaceID();
	  mTarget->SetSurfaceID(0);	//Unmark this frames surface so it doesn't get deleted
	  while (lFrame) 
	    {
	      lFrame->DeleteSurfaceWithID(SurfaceID);
	      lFrame = lFrame->GetNextFrame();
	    }
	}
    } 
  else if (AllFrames) 
    {
      long SurfaceID = mTarget->GetSurfaceID();
      Frame * lFrame = data->GetFirstFrame();
      while (lFrame) 
	{
	  if (lFrame != data->GetCurrentFramePtr()) 
	    {
	      Surface * temp = lFrame->GetSurfaceWithID(SurfaceID);
	      Orb2DSurface * lSurf = NULL;
	      if (temp)
		if (temp->GetSurfaceType() == kOrb2DType)
		  lSurf = (Orb2DSurface *) temp;
	      if (lSurf) {
		lSurf->UpdateData(mTarget);
		if (updateGrid) mTarget->FreeGrid();
	      }
	    }
	  lFrame = lFrame->GetNextFrame();
	}
    }
  if (updateGrid) mTarget->FreeGrid();
  UpdateTest = false;
  mExportBut->Enable();

  mUpdateBut->Disable();

  owner->SurfaceUpdated();
}

void Orbital2DSurfPane::refreshControls()
{
  wxString tmpStr;

  mNumGridPntSld->SetValue(NumGridPoints);
  mSphHarmonicsChk->SetValue(SphericalHarmonics); 

  SetNumContoursText();
  SetContourValueText();

  mUsePlaneChk->SetValue(UseScreenPlane);
  mRevPhaseChk->SetValue(PhaseChange);
  mDashCheck->SetValue(DashLines);
  mShowZeroCheck->SetValue(ShowZeroContour);
  mDisplayPlaneCheck->SetValue(DisplayPlane);

  mOrbColor1->setColor(&PosColor);
  mOrbColor2->setColor(&NegColor);
}

void Orbital2DSurfPane::OnOrbSetChoice( wxCommandEvent &event ) {
	int item = mOrbSetChoice->GetSelection()+1;
	int itemtype = orbSetChangeEvt(item, owner);
	
	if (itemtype) {	//TargetSet has been choosen
		PlotOrb = -1;
		
		if (!(OrbOptions&1)) {	//An MO set is selected
			mSphHarmonicsChk->Enable(false);
			SphericalHarmonics = false;
			mOrbFormatChoice->Enable(true);
		} else {	//Must be looking for AOs
			mSphHarmonicsChk->Enable(true);
			mSphHarmonicsChk->SetValue(SphericalHarmonics);
			mOrbFormatChoice->Enable(false);
		}
	}
	
	if (item <= 1) {
		mAtomList->Clear();
		PlotOrb = -1;
		coefIsEnabled = true;
	} else {
		coefIsEnabled = false;
		
		vector<wxString> newChoice;
		makeMOList(newChoice);
		
		mAtomList->Set(newChoice.size(), &newChoice.front());
	}
	vector<wxString> choice;
	makeAOList(choice);
	mOrbCoef->Set(choice.size(), &choice.front());

	if (!coefIsEnabled) {
		mAtomList->SetFocus();
	} else {
		mOrbCoef->SetFocus();
	}
}

void Orbital2DSurfPane::OnCoefList( wxCommandEvent &event )
{
  if (coefIsEnabled)
    {
      if (OrbOptions&1)
	PlotOrb = mOrbCoef->GetSelection();

      setUpdateButton();
    }

}

void Orbital2DSurfPane::OnAtomList( wxCommandEvent &event )
{
  PlotOrb = event.GetSelection();
  
  vector<wxString> choice;
  makeAOList(choice);
  mOrbCoef->Set(choice.size(), &choice.front());

  setUpdateButton();  
}

void Orbital2DSurfPane::OnOrbFormatChoice( wxCommandEvent &event )
{
  int itemtype = mOrbFormatChoice->GetSelection();
  setFlagOnOrbFormatChange(itemtype);

  vector<wxString> newChoice;
  makeMOList(newChoice);

  mAtomList->Set(newChoice.size(), &newChoice.front());

}

void Orbital2DSurfPane::OnReversePhase(wxCommandEvent &event )
{
  PhaseChange = mRevPhaseChk->GetValue();

  setUpdateButton();
}

void Orbital2DSurfPane::OnSphHarmonicChk(wxCommandEvent &event ) {
	SphericalHarmonics = mSphHarmonicsChk->GetValue();
	mAtomList->Clear();
	PlotOrb = -1;
	coefIsEnabled = true;
	
	vector<wxString> choice;
	makeAOList(choice);
	mOrbCoef->Set(choice.size(), &choice.front());
}

/*!
 * Orbital3D class
 */

Orbital3DSurfPane::Orbital3DSurfPane( wxWindow* parent, Orb3DSurface* target, 
				      SurfacesWindow* o, wxWindowID id,
				      const wxPoint& pos, const wxSize& size, 
				      long style ) 
  : Surface3DPane(parent, target, o, id, pos, size, style), OrbSurfacePane(target, o) {
	mTarget = target;

	TargetToPane();
	CreateControls();
}

Orbital3DSurfPane::~Orbital3DSurfPane()
{

}

void Orbital3DSurfPane::TargetToPane(void) {
	NumGridPoints = mTarget->GetNumGridPoints();
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	mTarget->GetTranspColor(&TranspColor);
	GridSize = mTarget->GetGridSize();
	ContourValue = mTarget->GetContourValue();
	UseSolidSurface = mTarget->SolidSurface();
	UseNormals = mTarget->UseSurfaceNormals();
	SphericalHarmonics = mTarget->UseSphericalHarmonics();
	UpdateTest = false;
	SwitchFixGrid = false;
}

void Orbital3DSurfPane::refreshControls() {
	float GridMax = mTarget->GetGridMax();

	mNumGridPntSld->SetValue(NumGridPoints);
	mGridSizeSld->SetValue((short)(100*GridSize));
	mContourValSld->SetValue((short)(100*(ContourValue/((fabs(GridMax)>=0.001)?GridMax:0.25))));
	m3DRdoBox->SetSelection(1-UseSolidSurface);
	mSmoothChkBox->SetValue(UseNormals);
	mSphHarmonicsChk->SetValue(SphericalHarmonics);

	if (UseSolidSurface)
		mSmoothChkBox->Enable();
	else
		mSmoothChkBox->Disable();

	mOrbColor1->setColor(&PosColor);
	mOrbColor2->setColor(&NegColor);
	mTransColor->setColor(&TranspColor);
}

/*!
 * Control creation for Orbital3D
 */

void Orbital3DSurfPane::CreateControls()
{
	//stuff from the base class creator
	upperSizer = new wxBoxSizer(wxHORIZONTAL);
	middleSizer = new wxBoxSizer(wxHORIZONTAL);
	bottomSizer = new wxBoxSizer(wxHORIZONTAL);
	leftMiddleSizer = new wxBoxSizer(wxVERTICAL);
	upperLeftMiddleSizer = new wxBoxSizer(wxHORIZONTAL);
	lowerLeftMiddleSizer = new wxBoxSizer(wxHORIZONTAL);
	rightMiddleSizer = new wxFlexGridSizer(2,2,0,0);
	leftBottomSizer = new wxBoxSizer(wxHORIZONTAL);
	rightBottomSizer = new wxBoxSizer(wxVERTICAL);
	
	wxStaticText * label0 = new wxStaticText( this, wxID_ANY,
						  _T("Select Orbital Set:"),
						  wxDefaultPosition,
						  wxDefaultSize);
	
	wxStaticText * label1 = new wxStaticText( this, wxID_ANY,
						  _T("Number of\n Grid Points:"),
						  wxDefaultPosition,
						  wxDefaultSize);
	
	mNumGridPntSld = new wxSlider( this, ID_GRID_POINT_SLIDER, 
				       0, 10, 150,
				       wxDefaultPosition, wxSize(155,wxDefaultCoord),
				       wxSL_AUTOTICKS | wxSL_LABELS);
	//set the initial value in the child object
	
	mSetParamBut = new wxButton( this, ID_SET_PARAM_BUT, wxT("Set Parameters"), wxPoint(450, 160) );
	mExportBut = new wxButton( this, ID_SURFACE_EXPORT_BUT, wxT("Export"), wxPoint(450, 160) );

	if (mTarget->ExportPossible())
	  mExportBut->Enable();
	else
	  mExportBut->Disable();

	mUpdateBut = new wxButton( this, ID_SURFACE_UPDATE_BUT, wxT("Update"), wxPoint(450, 160) );

	float GridMax = mTarget->GetGridMax();
	
	wxStaticText * label2 = new wxStaticText( this, wxID_ANY,
						  _T("Grid Size:"),
						  wxDefaultPosition,
						  wxDefaultSize);
	wxStaticText * label3 = new wxStaticText( this, wxID_ANY,
						  _T("Contour Value:"),
						  wxDefaultPosition,
						  wxDefaultSize);
	
	mGridSizeSld = new wxSlider( this, ID_GRID_SIZE_SLIDER, 
				     (short) (100*mTarget->GetGridSize()), 0, 300,
				     wxDefaultPosition, wxSize(155,wxDefaultCoord),
				     wxSL_AUTOTICKS | wxSL_LABELS);
	mContourValSld = new wxSlider( this, ID_CONTOUR_VALUE_SLIDER, 
				       (short)(100*(mTarget->GetContourValue()/((fabs(GridMax)>=0.001)?GridMax:0.25))), 
				       0, 100, wxDefaultPosition, 
				       wxSize(155,wxDefaultCoord));
	
	mContourValueEdit = new wxTextCtrl( this, ID_CONTOUR_VALUE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	
	wxStaticText * label4 = new wxStaticText( this, wxID_ANY,
						  _T("Transparency Color:"),
						  wxDefaultPosition,
						  wxDefaultSize);
	
	mTarget->GetTranspColor(&TranspColor);
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	
	mOrbColor1 = new colorArea(this, ID_3D_COLOR_POSITIVE, &PosColor);
	//  mOrbColor1->draw(&PosColor);
	mOrbColor2 = new colorArea(this, ID_3D_COLOR_NEGATIVE, &NegColor);
	//  mOrbColor2->draw(&NegColor);
	mTransColor = new colorArea(this, ID_TRANSPARENCY_COLOR, &TranspColor);
	//  mTransColor->draw(&TranspColor);
	
	wxString radiochoices[] = {_T("Solid"), _T("Wire Frame")};
	m3DRdoBox = new wxRadioBox( this, ID_3D_RADIOBOX, _T(""), wxDefaultPosition, wxDefaultSize, WXSIZEOF(radiochoices), radiochoices, 1, wxRA_SPECIFY_ROWS );
	m3DRdoBox->SetSelection(1-UseSolidSurface);
	
	mSmoothChkBox = new wxCheckBox( this, ID_SMOOTH_CHECKBOX, _T("Smooth"), wxPoint(340,130), wxDefaultSize );
	mSmoothChkBox->SetValue(UseNormals);
	
	if (UseSolidSurface)
		mSmoothChkBox->Enable();
	else
		mSmoothChkBox->Disable();
	
	mFreeMemBut = new wxButton( this, ID_FREE_MEM_BUT, wxT("Free Mem"), wxPoint(450, 160) );

	if (mTarget->GridAvailable())
	  mFreeMemBut->Enable();
	else
	  mFreeMemBut->Disable();

  upperSizer->Add(label0, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  vector<wxString> choices;
  int itemSelect = getOrbSetForOrbPane(choices) - 1;

  mOrbSetChoice = new wxChoice( this, ID_ORB_CHOICE, wxPoint(10,10), wxSize(200,wxDefaultCoord), choices.size(), &choices.front() );
  mOrbSetChoice->SetSelection(itemSelect);
  upperSizer->Add(mOrbSetChoice, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  upperSizer->Add(10,10);

  mRevPhaseChk = new wxCheckBox(this, ID_REVERSE_PHASE_CHECKBOX, _T("Reverse Phase"), wxDefaultPosition);
  mRevPhaseChk->SetValue(PhaseChange);

  upperSizer->Add(mRevPhaseChk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSphHarmonicsChk = new wxCheckBox( this, ID_SPH_HARMONICS_CHECKBOX, _T("Spherical Harmonics"), wxDefaultPosition, wxDefaultSize );

  wxString choices1[] = {_T("Energy"), _T("Occupation #")};
  mOrbFormatChoice = new wxChoice( this, ID_ORB_FORMAT_CHOICE, wxDefaultPosition, wxSize(120,wxDefaultCoord), 2, choices1 );
  mOrbFormatChoice->SetSelection(0);

  vector<wxString> choices2;
  makeMOList(choices2);

  vector<wxString> choices3;
  makeAOList(choices3);

  mAtomList = new wxListBox( this, ID_ATOM_LIST,
                             wxDefaultPosition, wxSize(160,180),
                             choices2.size(), &choices2.front(), 
			     wxLB_SINGLE |wxLB_ALWAYS_SB );

  mOrbCoef = new wxListBox( this, ID_ORB_COEF, wxPoint(20,160), wxSize(170,200), choices3.size(), &choices3.front(), wxLB_SINGLE |wxLB_ALWAYS_SB );

  mSphHarmonicsChk->SetValue(SphericalHarmonics);
  upperLeftMiddleSizer->Add(mSphHarmonicsChk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  lowerLeftMiddleSizer->Add(label1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  lowerLeftMiddleSizer->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mNumGridPntSld->SetValue(NumGridPoints);

  leftMiddleSizer->Add(upperLeftMiddleSizer, 0, wxALL, 3);
  leftMiddleSizer->Add(lowerLeftMiddleSizer, 0, wxALL, 3);

  rightMiddleSizer->Add(label2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
  rightMiddleSizer->Add(mGridSizeSld, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
  rightMiddleSizer->Add(label3, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
  rightMiddleSizer->Add(mContourValSld, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

  mSubLeftBot1Sizer = new wxBoxSizer(wxVERTICAL);
  mSubLeftBot2Sizer = new wxBoxSizer(wxVERTICAL);
  
  mSubLeftBot1Sizer->Add(new wxStaticText(this, wxID_ANY,
                            _T("Select Orb:"),
                            wxDefaultPosition,
			    wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSubLeftBot1Sizer->Add(mOrbFormatChoice, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSubLeftBot1Sizer->Add(mAtomList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSubLeftBot2Sizer->Add(new wxStaticText(this, wxID_ANY,
                         _T("Orbital vector: \nAtom Orbital Coef"),
                         wxDefaultPosition,
                         wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);
  mSubLeftBot2Sizer->Add(mOrbCoef, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  leftBottomSizer->Add(mSubLeftBot1Sizer, 0, wxALL, 5);
  leftBottomSizer->Add(mSubLeftBot2Sizer, 0, wxALL, 5);

  mSubRightBot0Sizer = new wxBoxSizer(wxHORIZONTAL);
  mSubRightBot0Sizer->Add(mContourValueEdit);

  wxString tmpStr;
  tmpStr.Printf(wxT("%.4f"), ContourValue);
  mContourValueEdit->SetValue(tmpStr);

  mSubRightBot0Sizer->Add(30,30);

  mGridMinText = new wxStaticText(this, wxID_ANY, _T("0"), wxDefaultPosition, wxDefaultSize);
  mSubRightBot0Sizer->Add(mGridMinText, 0, wxALIGN_TOP | wxALL, 3);
  mSubRightBot0Sizer->Add(100,30);

  tmpStr.Printf(wxT("%.4f"), GridMax);
  mGridMaxText = new wxStaticText(this, wxID_ANY, tmpStr, 
				     wxDefaultPosition, wxDefaultSize);
  mSubRightBot0Sizer->Add(mGridMaxText, 
		          0, wxALIGN_TOP | wxALL, 3);

  mSubRightBot1Sizer = new wxBoxSizer(wxHORIZONTAL);
  mSubRightBot1Sizer->Add(new wxStaticText(this, wxID_ANY,
                          _T("Orbital Colors:"),
                          wxDefaultPosition,
                          wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSubRightBot1Sizer->Add(mOrbColor1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSubRightBot1Sizer->Add(mOrbColor2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSubRightBot2Sizer = new wxBoxSizer(wxHORIZONTAL);
  mSubRightBot2Sizer->Add(label4, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSubRightBot2Sizer->Add(mTransColor, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSubRightBot3Sizer = new wxBoxSizer(wxHORIZONTAL);
  mSubRightBot3Sizer->Add(m3DRdoBox, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSubRightBot4Sizer = new wxBoxSizer(wxHORIZONTAL);
  mSubRightBot4Sizer->Add(mSmoothChkBox,0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

  mSubRightBot5Sizer = new wxGridSizer(2,2,0,0);
  mSubRightBot5Sizer->Add(mSetParamBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSubRightBot5Sizer->Add(mExportBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSubRightBot5Sizer->Add(mFreeMemBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSubRightBot5Sizer->Add(mUpdateBut, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mUpdateBut->SetDefault();

  rightBottomSizer->Add(mSubRightBot0Sizer);
  rightBottomSizer->Add(20, 20);
  rightBottomSizer->Add(mSubRightBot1Sizer);
  rightBottomSizer->Add(mSubRightBot2Sizer);
  rightBottomSizer->Add(mSubRightBot3Sizer);
  rightBottomSizer->Add(mSubRightBot4Sizer);
  rightBottomSizer->Add(mSubRightBot5Sizer);

  middleSizer->Add(leftMiddleSizer, 0, wxALL, 10);
  middleSizer->Add(45,10);
  middleSizer->Add(rightMiddleSizer, 0, wxALL, 10);
  bottomSizer->Add(leftBottomSizer, 0, wxALL, 3);
  bottomSizer->Add(rightBottomSizer, 0, wxALL, 3);
  mainSizer->Add(upperSizer);
  mainSizer->Add(middleSizer);
  mainSizer->Add(bottomSizer);
  
  wxCommandEvent foo;
  OnOrbSetChoice(foo);
}

bool Orbital3DSurfPane::UpdateNeeded(void) {
	bool result = false;

	double newVal;
	wxString tmpStr = mContourValueEdit->GetValue();
	if (!tmpStr.ToDouble(&newVal)) {
		tmpStr.Printf(wxT("%.4f"), ContourValue);
		mContourValueEdit->SetValue(tmpStr);
	} else ContourValue = newVal;
		
	if (PlotOrb >= 0) {	//Don't update unless a valid orbital is chosen
		if (PlotOrb != mTarget->GetTargetOrb()) result=true;
		if (Visible != mTarget->GetVisibility()) result = true;
		if (AllFrames != (mTarget->GetSurfaceID() != 0)) result = true;
		if (TargetSet != mTarget->GetTargetSet()) result = true;
		if (OrbOptions != mTarget->GetOptions()) result = true;
		if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
		if (ContourValue != mTarget->GetContourValue()) result = true;
		if (GridSize != mTarget->GetGridSize()) result = true;
		if (UseSolidSurface != mTarget->SolidSurface()) result = true;
		if (UseNormals != mTarget->UseSurfaceNormals()) result = true;
		if (PhaseChange != mTarget->GetPhaseChange()) result = true;
		if (SphericalHarmonics != mTarget->UseSphericalHarmonics()) result = true;

		if (!result) {
		  RGBColor	testColor;
		  mTarget->GetPosColor(&testColor);
		  if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)
			  ||(PosColor.blue!=testColor.blue)) 
			  result=true;

		  mTarget->GetNegColor(&testColor);
		  if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)
			  ||(NegColor.blue!=testColor.blue)) 
			  result=true;

		  mTarget->GetTranspColor(&testColor);
		  if ((TranspColor.red != testColor.red)
			  ||(TranspColor.green!=testColor.green)
			  ||(TranspColor.blue!=testColor.blue)) 
			  result=true;
		}
	}
	return result;
}

void Orbital3DSurfPane::OnOrbFormatChoice( wxCommandEvent &event )
{
  int itemtype = mOrbFormatChoice->GetSelection();
  setFlagOnOrbFormatChange(itemtype);

  vector<wxString> newChoice;
  makeMOList(newChoice);

  mAtomList->Set(newChoice.size(), &newChoice.front());

}

void Orbital3DSurfPane::OnOrbSetChoice( wxCommandEvent &event ) {
	int item = mOrbSetChoice->GetSelection()+1;
	int itemtype = orbSetChangeEvt(item, owner);

	if (itemtype) {	//TargetSet has been choosen
		PlotOrb = -1;

		if (!(OrbOptions&1)) {	//An MO set is selected
			mSphHarmonicsChk->Enable(false);
			SphericalHarmonics = false;
			mOrbFormatChoice->Enable(true);
		} else {	//Must be looking for AOs
			mSphHarmonicsChk->Enable(true);
			mSphHarmonicsChk->SetValue(SphericalHarmonics);
			mOrbFormatChoice->Enable(false);
		}
	}

	if (item <= 1) {
		mAtomList->Clear();
		PlotOrb = -1;
		coefIsEnabled = true;
	} else {
		coefIsEnabled = false;

		vector<wxString> newChoice;
		makeMOList(newChoice);

		mAtomList->Set(newChoice.size(), &newChoice.front());
	}
	vector<wxString> choice;
	makeAOList(choice);
	mOrbCoef->Set(choice.size(), &choice.front());

	if (!coefIsEnabled) {
		mAtomList->SetFocus();
	} else {
		mOrbCoef->SetFocus();
	}
}

void Orbital3DSurfPane::OnCoefList( wxCommandEvent &event ) {
	if (coefIsEnabled) {
		if (OrbOptions&1)
			PlotOrb = mOrbCoef->GetSelection();

		setUpdateButton();
	}
}

void Orbital3DSurfPane::OnAtomList( wxCommandEvent &event )
{
  PlotOrb = event.GetSelection();
  
  vector<wxString> choice;
  makeAOList(choice);
  mOrbCoef->Set(choice.size(), &choice.front());

  setUpdateButton();  
}

void Orbital3DSurfPane::OnSphHarmonicChk(wxCommandEvent &event ) {
	SphericalHarmonics = mSphHarmonicsChk->GetValue();
	mAtomList->Clear();
	PlotOrb = -1;
	coefIsEnabled = true;
	
	vector<wxString> choice;
	makeAOList(choice);
	mOrbCoef->Set(choice.size(), &choice.front());
}

void Orbital3DSurfPane::OnReversePhase(wxCommandEvent &event )
{
  PhaseChange = mRevPhaseChk->GetValue();

  setUpdateButton();
}

void Orbital3DSurfPane::OnContourValueSld(wxCommandEvent &event )
{
  float GridMax = mTarget->GetGridMax();
  ContourValue = 0.01 * mContourValSld->GetValue() * ((fabs(GridMax)>=0.001)?GridMax:0.25);
  //mTarget->SetContourValue(ContourValue);

  wxString tmpStr;
  tmpStr.Printf(wxT("%.4f"), ContourValue);
  mContourValueEdit->SetValue(tmpStr);

  setUpdateButton();
}

void Orbital3DSurfPane::OnGridSizeSld(wxCommandEvent &event )
{
  GridSize = 0.01 * mGridSizeSld->GetValue();
  SwitchFixGrid = true;

  setUpdateButton();
}

void Orbital3DSurfPane::OnUpdate(wxCommandEvent &event ) {
	bool updateGrid=UpdateTest;
	bool updateContour=false;
  
	wxWindow * focusWindow = wxWindow::FindFocus();

	if (PlotOrb >= 0) {	//Don't update unless a valid orbital is chosen
		if (TargetSet != mTarget->GetTargetSet()) updateGrid = true;
		if (PlotOrb != mTarget->GetTargetOrb()) updateGrid=true;
		if (SphericalHarmonics != mTarget->UseSphericalHarmonics()) updateGrid = true;
		if (OrbOptions != mTarget->GetOptions()) updateGrid = true;
		if (NumGridPoints != mTarget->GetNumGridPoints()) updateGrid = true;
		if (ContourValue != mTarget->GetContourValue()) updateContour = true;
		if (GridSize != mTarget->GetGridSize()) updateGrid = true;
		if (PhaseChange != mTarget->GetPhaseChange()) updateGrid = true;
    }

	if (SwitchFixGrid) {
		mTarget->SetFixGrid(false);
		SwitchFixGrid = false;
		updateGrid = true;
    }

	if (Visible && !mTarget->ContourAvail()) updateContour = true;
	//test to see if grid or contour must calculated anyway
	if (updateContour && ! mTarget->GridAvailable()) updateGrid = true;
	if (updateGrid) updateContour = true;

	mTarget->SetVisibility(Visible);
	mTarget->SolidSurface(UseSolidSurface);
	mTarget->SetNumGridPoints(NumGridPoints);
	mTarget->SetContourValue(ContourValue);
	mTarget->SetGridSize(GridSize);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetNegColor(&NegColor);
	mTarget->SetTranspColor(&TranspColor);
	mTarget->SetPhaseChange(PhaseChange);
	mTarget->UseSurfaceNormals(UseNormals);
	mTarget->UseSphericalHarmonics(SphericalHarmonics);

	OrbSurfacePane::UpdateEvent();

	MoleculeData * mData = owner->GetMoleculeData();

	//update this surface's data on all frames if necessary
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) {	//update all frames
		long	SurfaceID;
		Frame *	lFrame = mData->GetFirstFrame();
		updateGrid = updateContour = true;
		if (AllFrames) {	//adding the surface to all frames
			SurfaceID = mTarget->SetSurfaceID();
			while (lFrame) {
				if (lFrame != mData->GetCurrentFramePtr()) {
					Orb3DSurface * NewSurface = new Orb3DSurface(mTarget);
					lFrame->AppendSurface(NewSurface);
				}
				lFrame = lFrame->GetNextFrame();
			}
		} else {			//deleting the surface from other frames
			SurfaceID = mTarget->GetSurfaceID();
			mTarget->SetSurfaceID(0);	//Unmark this frames surface so it doesn't get deleted
			while (lFrame) {
				lFrame->DeleteSurfaceWithID(SurfaceID);
				lFrame = lFrame->GetNextFrame();
			}
		}
	}

	Progress * lProgress = new Progress();
//  if (!lProgress) 
  //  cout<<"Cannot allocate the pregress bar!"<<endl;

	if (AllFrames) {	//compute the contour for each frame, no grid is kept
		long SurfaceID = mTarget->GetSurfaceID();
		long CurrentFrame = mData->GetCurrentFrame();
		long NumFrames = mData->GetNumFrames();

		for (int i=0; i<NumFrames; i++) {
			Orb3DSurface * lSurf;
			lSurf = NULL;
			mData->SetCurrentFrame(i+1);
			Frame * lFrame = mData->GetCurrentFramePtr();

			if (CurrentFrame != mData->GetCurrentFrame()) {
				Surface * temp = lFrame->GetSurfaceWithID(SurfaceID);
				//Confirm that the surface is the correct type

				if (temp)
					if (temp->GetSurfaceType() == kOrb3DType)
						lSurf = (Orb3DSurface *) temp;

				if (lSurf) lSurf->UpdateData(mTarget);
			} else lSurf = mTarget;

			if (lSurf) {
				if (Visible) {
					lProgress->ChangeText("Calculating 3D Grid...");
					lProgress->SetBaseValue(100*i/NumFrames);
					lProgress->SetScaleFactor((float) 0.9/NumFrames);
					if (updateGrid) mTarget->CalculateMOGrid(mData, lProgress);
					lProgress->ChangeText("Contouring grid...");
					lProgress->SetBaseValue((long)(100*i/NumFrames + 90.0/NumFrames));
					lProgress->SetScaleFactor((float) 0.1/NumFrames);
					if (updateContour) mTarget->Contour3DGrid(lProgress);
					lSurf->FreeGrid();
				} else {
					if (updateGrid) mTarget->FreeGrid();
					if (updateContour) mTarget->FreeContour();
				}
			}
		}
		mData->SetCurrentFrame(CurrentFrame);
	} else {	//simply update this surface
		if (Visible) {
			lProgress->ChangeText("Calculating 3D Grid...");
			lProgress->SetScaleFactor(0.9);
			if (updateGrid) mTarget->CalculateMOGrid(mData, lProgress);
			lProgress->ChangeText("Contouring grid...");
			lProgress->SetBaseValue(90);
			lProgress->SetScaleFactor(0.1);
			if (updateContour) mTarget->Contour3DGrid(lProgress);
		} else {
			if (updateGrid) mTarget->FreeGrid();
			if (updateContour) mTarget->FreeContour();
		}
	}
	if (lProgress) delete lProgress;

	if (mTarget->GridAvailable())
		mFreeMemBut->Enable();
	else
		mFreeMemBut->Disable();

	float GridMax = mTarget->GetGridMax();

	wxString tmpStr;
	tmpStr.Printf(wxT("%.4f"), GridMax);
	mGridMaxText->SetLabel(tmpStr);

	setContourValueSld();

//Setup the contour value and grid max text items

	UpdateTest = false;

	if (mTarget->ExportPossible())
		mExportBut->Enable();
	else
		mExportBut->Disable();

	mUpdateBut->Disable();

	owner->SurfaceUpdated();
  
	if (!coefIsEnabled)
		mAtomList->SetFocus();
	else
		mOrbCoef->SetFocus();
}

bool Orbital3DSurfPane::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Orbital3DSurfPane::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin Orbital3D bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end Orbital3D bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Orbital3DSurfPane::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin Orbital3D icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end Orbital3D icon retrieval
}

/*2D TEDensity Panel*/ 
/*
TEDensity2DSurfPane::TEDensity2DSurfPane( wxWindow* parent, 
					  TEDensity2DSurface* target, 
					  SurfacesWindow* o, wxWindowID id,
					  const wxPoint& pos, const wxSize& size, 
					  long style ) 
  : Surface2DPane(parent, target, o, id, pos, size, style)
{
  mTarget = target;

  TargetToPane();
  CreateControls();
}

TEDensity2DSurfPane::~TEDensity2DSurfPane()
{

}

void TEDensity2DSurfPane::TargetToPane()
{
  TargetOrbSet = mTarget->getTargetOrbSet();
  NumGridPoints = mTarget->GetNumGridPoints();
  NumContours = mTarget->GetNumContours();
  MaxContourValue = mTarget->GetMaxValue();
  mTarget->GetPosColor(&Color);
  UseScreenPlane = mTarget->GetRotate2DMap();
  Visible = mTarget->GetVisibility();
  AllFrames = (mTarget->GetSurfaceID() != 0);
  UpdateTest = false;
}

void TEDensity2DSurfPane::CreateControls()
{

}

void TEDensity2DSurfPane::refreshControls()
{

}
*/

/*!
* General3DSurfPane class
 */

General3DSurfPane::General3DSurfPane( wxWindow* parent, General3DSurface* target, 
									  SurfacesWindow* o, wxWindowID id,
									  const wxPoint& pos, const wxSize& size, 
									  long style ) 
			: Surface3DPane(parent, target, o, id, pos, size, style)
{
	mTarget = target;
	MultValue = -1.0;
	useMultValue = false;
	squareValues = false;
	
	TargetToPane();
	CreateControls();
	refreshControls();
}

General3DSurfPane::~General3DSurfPane()
{
	
}

void General3DSurfPane::TargetToPane(void) 
{
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	mTarget->GetTranspColor(&TranspColor);
	ContourValue = mTarget->GetContourValue();
	UseSolidSurface = mTarget->SolidSurface();
	UseNormals = mTarget->UseSurfaceNormals();
	ContourPosNeg = mTarget->ContourBothPosNeg();
	UpdateTest = false;
}

void General3DSurfPane::refreshControls()
{
	float GridMax = mTarget->GetGridMax();
	float GridMin = mTarget->GetGridMin();
	if (ContourPosNeg) GridMin = 0.0;
	float Range = GridMax - GridMin;
	if (fabs(Range)< ContourValue) Range = ContourValue;
	
	mContourValSld->SetValue((short) (100*((ContourValue - GridMin)/Range)));
	wxString tmpStr;
	tmpStr.Printf(wxT("%.4f"), ContourValue);
	mContourValueEdit->SetValue(tmpStr);
	tmpStr.Printf(wxT("%.4f"), GridMax);
	mGridMaxText->SetLabel(tmpStr);
	tmpStr.Printf(wxT("%.4f"), GridMin);
	mGridMinText->SetLabel(tmpStr);

	tmpStr.Printf(wxT("%.4f"), MultValue);
	mGenMultValue->SetValue(tmpStr);
	mMultCheck->SetValue(useMultValue);
	mSquareCheck->SetValue(squareValues);

	m3DRdoBox->SetSelection(1-UseSolidSurface);
	mSmoothChkBox->SetValue(UseNormals);
	
	if (UseSolidSurface)
		mSmoothChkBox->Enable();
	else
		mSmoothChkBox->Disable();
	
	mOrbColor1->setColor(&PosColor);
	mOrbColor2->setColor(&NegColor);
	mTransColor->setColor(&TranspColor);

	mGenContourPosNegCheck->SetValue(ContourPosNeg);
}

/*!
* Control creation General3D
 */

void General3DSurfPane::CreateControls()
{    
    General3DSurfPane * Gen3DPanel = this;	//this is here just to match the code below from DialogBlocks
	
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer5, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText6 = new wxStaticText( Gen3DPanel, wxID_STATIC, _("Choose 3D grid file:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    wxButton* itemButton7 = new wxButton( Gen3DPanel, ID_GENFILEBUTTON, _("Choose File..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer5->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer8, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    mMultCheck = new wxCheckBox( Gen3DPanel, ID_GENMULTCHECK, _("Multiply by:"), wxDefaultPosition, wxDefaultSize, 0 );
    mMultCheck->SetValue(false);
    if (ShowToolTips())
        mMultCheck->SetToolTip(_("check to multiply the grid values by the value to the left as they are read in from file."));
    itemBoxSizer8->Add(mMultCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mGenMultValue = new wxTextCtrl( Gen3DPanel, ID_GENMULTEDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mGenMultValue->SetToolTip(_("Enter a value to multiply the grid by as it is read in from file."));
    itemBoxSizer8->Add(mGenMultValue, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mGenMultValue->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & MultValueString) );

    mSquareCheck = new wxCheckBox( Gen3DPanel, ID_GENSQUARECHECK, _("Square values as read in"), wxDefaultPosition, wxDefaultSize, 0 );
    mSquareCheck->SetValue(false);
    mainSizer->Add(mSquareCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer12, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer12->Add(itemBoxSizer13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText14 = new wxStaticText( Gen3DPanel, wxID_STATIC, _("Contour Value:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemStaticText14, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);
	
	mContourValueEdit = new wxTextCtrl( Gen3DPanel, ID_CONTOUR_VALUE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(mContourValueEdit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer12->Add(itemBoxSizer16, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    mContourValSld = new wxSlider( Gen3DPanel, ID_CONTOUR_VALUE_SLIDER, 0, 0, 100, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_HORIZONTAL );
    itemBoxSizer16->Add(mContourValSld, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	
	wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer16->Add(itemBoxSizer18, 1, wxGROW|wxLEFT|wxTOP|wxBOTTOM, 5);
    mGridMinText = new wxStaticText( Gen3DPanel, ID_GENSURFGRIDMINTEXT, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
    itemBoxSizer18->Add(mGridMinText, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mGridMaxText = new wxStaticText( Gen3DPanel, ID_GRID_MAX_TEXT, _("1"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    itemBoxSizer18->Add(mGridMaxText, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer21, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
    wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer21->Add(itemBoxSizer22, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
    wxStaticText* itemStaticText23 = new wxStaticText( Gen3DPanel, wxID_STATIC, _("Positive Color:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer22->Add(itemStaticText23, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);
	
	mTarget->GetTranspColor(&TranspColor);
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);

    mOrbColor1 = new colorArea( Gen3DPanel, ID_3D_COLOR_POSITIVE, &PosColor);
    itemBoxSizer22->Add(mOrbColor1, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer21->Add(itemBoxSizer25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
    wxStaticText* itemStaticText26 = new wxStaticText( Gen3DPanel, wxID_STATIC, _("Negative Color:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer25->Add(itemStaticText26, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mOrbColor2 = new colorArea( Gen3DPanel, ID_3D_COLOR_NEGATIVE, &NegColor );
    itemBoxSizer25->Add(mOrbColor2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer21->Add(itemBoxSizer28, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
    wxStaticText* itemStaticText29 = new wxStaticText( Gen3DPanel, wxID_STATIC, _("Transparency Color:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer28->Add(itemStaticText29, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mTransColor = new colorArea( Gen3DPanel, ID_TRANSPARENCY_COLOR, &TranspColor );
    itemBoxSizer28->Add(mTransColor, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
	wxBoxSizer* itemBoxSizer31 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer31, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
    wxString m3DRdoBoxStrings[] = {
        _("&Solid"),
        _("&WireFrame")
    };
    m3DRdoBox = new wxRadioBox( Gen3DPanel, ID_3D_RADIOBOX, _("Surface Display"), wxDefaultPosition, wxDefaultSize, 2, m3DRdoBoxStrings, 1, wxRA_SPECIFY_ROWS );
    m3DRdoBox->SetSelection(0);
    itemBoxSizer31->Add(m3DRdoBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mSmoothChkBox = new wxCheckBox( Gen3DPanel, ID_SMOOTH_CHECKBOX, _("Smooth"), wxDefaultPosition, wxDefaultSize, 0 );
    mSmoothChkBox->SetValue(false);
    if (ShowToolTips())
        mSmoothChkBox->SetToolTip(_("Check to use surface normals to smooth out the surface of the contour."));
    itemBoxSizer31->Add(mSmoothChkBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mGenContourPosNegCheck = new wxCheckBox( Gen3DPanel, ID_GENCONPOSNEGCHECK, _("Contour +/- values"), wxDefaultPosition, wxDefaultSize, 0 );
    mGenContourPosNegCheck->SetValue(false);
    if (ShowToolTips())
        mGenContourPosNegCheck->SetToolTip(_("Check to produce both + and - contours with the magnitude given above."));
    mainSizer->Add(mGenContourPosNegCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer35 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer35, 0, wxALIGN_RIGHT|wxALL, 5);
    mFreeMemBut = new wxButton( Gen3DPanel, ID_FREE_MEM_BUT, _("Free Mem"), wxDefaultPosition, wxDefaultSize, 0 );

    if (mTarget->GridAvailable())
      mFreeMemBut->Enable();
    else
      mFreeMemBut->Disable();

    if (ShowToolTips())
        mFreeMemBut->SetToolTip(_("Click to free the memory used by the 3D grid."));
    itemBoxSizer35->Add(mFreeMemBut, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mExportBut = new wxButton( Gen3DPanel, ID_SURFACE_EXPORT_BUT, _("Export..."), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mExportBut->SetToolTip(_("Click to export the selected surface to a text file."));
    itemBoxSizer35->Add(mExportBut, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mUpdateBut = new wxButton( Gen3DPanel, ID_SURFACE_UPDATE_BUT, _("Update"), wxDefaultPosition, wxDefaultSize, 0 );

    if (mTarget->ExportPossible())
      mExportBut->Enable();
    else
      mExportBut->Disable();

    mUpdateBut->SetDefault();
    if (ShowToolTips())
        mUpdateBut->SetToolTip(_("Click to apply your changes to the molecule display."));
    itemBoxSizer35->Add(mUpdateBut, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
}

bool General3DSurfPane::UpdateNeeded(void) 
{
	bool result = UpdateTest;
	
	if (mTarget->GridAvailable()) {	//Don't update unless a valid 3D grid has been read in from file
		if (Visible != mTarget->GetVisibility()) result = true;
		if (ContourValue != mTarget->GetContourValue()) result = true;
		if (UseSolidSurface != mTarget->SolidSurface()) result = true;
//		if (UseColorByValue != mTarget->ColorByValue()) result = true;
		if (ContourPosNeg != mTarget->ContourBothPosNeg()) result = true;
		if (UseNormals != mTarget->UseSurfaceNormals()) result = true;
		if (!result) {
			RGBColor	testColor;
			mTarget->GetPosColor(&testColor);
			if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)||
				(PosColor.blue!=testColor.blue)) result=true;
			mTarget->GetNegColor(&testColor);
			if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)||
				(NegColor.blue!=testColor.blue)) result=true;
			mTarget->GetTranspColor(&testColor);
			if ((TranspColor.red != testColor.red)||(TranspColor.green!=testColor.green)||
				(TranspColor.blue!=testColor.blue)) result=true;
		}
	}
	return result;
}
void General3DSurfPane::OnUpdate(wxCommandEvent &event) {
	bool updateContour = UpdateTest || ! mTarget->ContourAvail();
	if (Visible) {	//update the contour if contour value has changed
		if (ContourValue != mTarget->GetContourValue()) updateContour = true;
	}
	mTarget->SetVisibility(Visible);
	mTarget->SolidSurface(UseSolidSurface);
	if (ContourPosNeg != mTarget->ContourBothPosNeg()) updateContour = true;
	mTarget->SetContourBothPosNeg(ContourPosNeg);
//	mTarget->SetColorByValue(UseColorByValue);
	mTarget->SetContourValue(ContourValue);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetNegColor(&NegColor);
	mTarget->SetTranspColor(&TranspColor);
	mTarget->UseSurfaceNormals(UseNormals);
	if (updateContour && mTarget->GridAvailable()) {
		Progress * lProgress = new Progress();
		lProgress->ChangeText("Contouring grid...");
		mTarget->Contour3DGrid(lProgress);
		delete lProgress;
	}
	//Setup the contour value and grid max text items
	float GridMax = mTarget->GetGridMax();
	float GridMin = mTarget->GetGridMin();
	if (ContourPosNeg) GridMin = 0.0;
	float Range = GridMax - GridMin;
	if (fabs(Range)< ContourValue) Range = ContourValue;
	
	mContourValSld->SetValue((short) (100*((ContourValue-GridMin)/Range)));
	wxString tmpStr;
	tmpStr.Printf(wxT("%.4f"), ContourValue);
	mContourValueEdit->SetValue(tmpStr);
	
	UpdateTest = false;
	setUpdateButton();
	owner->SurfaceUpdated();
}
void General3DSurfPane::OnMultCheck( wxCommandEvent &event )
{
	useMultValue = mMultCheck->GetValue();
	setUpdateButton();
}
void General3DSurfPane::OnSquareCheck( wxCommandEvent &event )
{
	squareValues = mSquareCheck->GetValue();
	setUpdateButton();
}
void General3DSurfPane::OnContourPosNegCheck( wxCommandEvent &event )
{
	ContourPosNeg = mGenContourPosNegCheck->GetValue();
	refreshControls();
	setUpdateButton();
}
void General3DSurfPane::OnContourValueSld(wxCommandEvent &event )
{
	float GridMax = mTarget->GetGridMax();
	float GridMin = mTarget->GetGridMin();
	if (ContourPosNeg) GridMin = 0.0;
	float Range = GridMax - GridMin;
	if (fabs(Range)< ContourValue) Range = ContourValue;

	ContourValue = GridMin + (0.01 * mContourValSld->GetValue() * Range);
	
	wxString tmpStr;
	tmpStr.Printf(wxT("%.4f"), ContourValue);
	mContourValueEdit->SetValue(tmpStr);
	
	setUpdateButton();
}
void General3DSurfPane::OnMultValueEdit(wxCommandEvent& event ) {
	double newVal=0.0;
	wxString temp = mGenMultValue->GetValue();
	
	if (temp.ToDouble(&newVal)) {
		MultValue = newVal;
	}
//	temp.Printf(wxT("%.4f"), MultValue);
//	mGenMultValue->SetValue(temp);
	setUpdateButton();
}
void General3DSurfPane::OnFileButton(wxCommandEvent& event ) {
	mTarget->ReadGrid(squareValues, useMultValue, MultValue);
	UpdateTest = true;
	refreshControls();
	setUpdateButton();
}

bool General3DSurfPane::ShowToolTips()
{
    return true;
}

/*!
* Get bitmap resources
 */

wxBitmap General3DSurfPane::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
	////@begin Orbital3D bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
	////@end Orbital3D bitmap retrieval
}

/*!
* Get icon resources
 */

wxIcon General3DSurfPane::GetIconResource( const wxString& name )
{
    // Icon retrieval
	////@begin Orbital3D icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
	////@end Orbital3D icon retrieval
}
/*!
* General2DSurfPane class
 */

General2DSurfPane::General2DSurfPane( wxWindow* parent, General2DSurface* target, 
									  SurfacesWindow* o, wxWindowID id,
									  const wxPoint& pos, const wxSize& size, 
									  long style ) 
: Surface2DPane(parent, target, o, id, pos, size, style)
{
	mTarget = target;
	MultValue = -1.0;
	useMultValue = false;
	squareValues = false;
	
	TargetToPane();
	CreateControls();
	refreshControls();
}

General2DSurfPane::~General2DSurfPane()
{
	
}

void General2DSurfPane::CreateControls() {
	General2DSurfPane * Gen2DPanel = this;	//this is here just to match the code below from DialogBlocks

	wxBoxSizer* itemBoxSizer41 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer41, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText42 = new wxStaticText( Gen2DPanel, wxID_STATIC, _("Choose 2D grid file:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer41->Add(itemStaticText42, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    wxButton* itemButton43 = new wxButton( Gen2DPanel, ID_GENFILEBUTTON, _("Choose File..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer41->Add(itemButton43, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer44 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer44, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    mMultCheck = new wxCheckBox( Gen2DPanel, ID_GENMULTCHECK, _("Multiply by:"), wxDefaultPosition, wxDefaultSize, 0 );
    mMultCheck->SetValue(false);
    if (ShowToolTips())
        mMultCheck->SetToolTip(_("check to multiply the grid values by the value to the left as they are read in from file."));
    itemBoxSizer44->Add(mMultCheck, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mGenMultValue = new wxTextCtrl( Gen2DPanel, ID_GENMULTEDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (ShowToolTips())
        mGenMultValue->SetToolTip(_("Enter a value to multiply the grid by as it is read in from file."));
    itemBoxSizer44->Add(mGenMultValue, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mSquareCheck = new wxCheckBox( Gen2DPanel, ID_GENSQUARECHECK, _("Square Grid values as read in"), wxDefaultPosition, wxDefaultSize, 0 );
    mSquareCheck->SetValue(false);
    mainSizer->Add(mSquareCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer48 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer48, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText49 = new wxStaticText( Gen2DPanel, wxID_STATIC, _("Max # of contours:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer48->Add(itemStaticText49, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mNumContourText = new wxTextCtrl( Gen2DPanel, ID_NUM_CONTOUR_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer48->Add(mNumContourText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer51 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer51, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText52 = new wxStaticText( Gen2DPanel, wxID_STATIC, _("Max contour value:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer51->Add(itemStaticText52, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mContourValText = new wxTextCtrl( Gen2DPanel, ID_CONTOUR_VALUE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer51->Add(mContourValText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer54 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer54, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
    wxBoxSizer* itemBoxSizer55 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer54->Add(itemBoxSizer55, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
    wxStaticText* itemStaticText56 = new wxStaticText( Gen2DPanel, wxID_STATIC, _("Positive Color:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer55->Add(itemStaticText56, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mOrbColor1 = new colorArea( Gen2DPanel, ID_2D_COLOR_POSITIVE, &PosColor );
    itemBoxSizer55->Add(mOrbColor1, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer58 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer54->Add(itemBoxSizer58, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
    wxStaticText* itemStaticText59 = new wxStaticText( Gen2DPanel, wxID_STATIC, _("Negative Color:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer58->Add(itemStaticText59, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mOrbColor2 = new colorArea( Gen2DPanel, ID_2D_COLOR_NEGATIVE, &NegColor );
    itemBoxSizer58->Add(mOrbColor2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mShowZeroCheck = new wxCheckBox( Gen2DPanel, ID_SHOW_ZERO_CHECKBOX, _("Show zero value contour"), wxDefaultPosition, wxDefaultSize, 0 );
    mShowZeroCheck->SetValue(false);
    if (ShowToolTips())
        mShowZeroCheck->SetToolTip(_("Check to produce a gray contour where the plane changes sign."));
    mainSizer->Add(mShowZeroCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mDisplayPlaneCheck = new wxCheckBox( Gen2DPanel, ID_DISPLAY_PLANE_CHECKBOX, _("Display Plotting Plane"), wxDefaultPosition, wxDefaultSize, 0 );
    mDisplayPlaneCheck->SetValue(false);
    if (ShowToolTips())
        mDisplayPlaneCheck->SetToolTip(_("Display a translucent plane with border to indicate the plotting plane"));
    mainSizer->Add(mDisplayPlaneCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    mDashCheck = new wxCheckBox( Gen2DPanel, ID_DASH_CHECKBOX, _("Dash negative contours"), wxDefaultPosition, wxDefaultSize, 0 );
    mDashCheck->SetValue(false);
    if (ShowToolTips())
        mDashCheck->SetToolTip(_("Produces dashed negative contour lines for easier viewing in B & W."));
    mainSizer->Add(mDashCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer63 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer63, 0, wxALIGN_RIGHT|wxALL, 5);
    mExportBut = new wxButton( Gen2DPanel, ID_SURFACE_EXPORT_BUT, _("Export..."), wxDefaultPosition, wxDefaultSize, 0 );

    if (mTarget->ExportPossible())
      mExportBut->Enable();
    else
      mExportBut->Disable();

    if (ShowToolTips())
        mExportBut->SetToolTip(_("Click to export the selected surface to a text file."));
    itemBoxSizer63->Add(mExportBut, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mUpdateBut = new wxButton( Gen2DPanel, ID_SURFACE_UPDATE_BUT, _("Update"), wxDefaultPosition, wxDefaultSize, 0 );
    mUpdateBut->SetDefault();
    if (ShowToolTips())
        mUpdateBut->SetToolTip(_("Click to apply your changes to the molecule display."));
    itemBoxSizer63->Add(mUpdateBut, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
	mGenMultValue->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & MultValueString) );
	mNumContourText->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & mMaxContourCountString) );
	mContourValText->SetValidator( wxTextValidator(wxFILTER_NUMERIC, & mMaxContourValueString) );
}
void General2DSurfPane::TargetToPane(void) 
{
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	MaxContourValue = mTarget->GetMaxValue();
	NumContours = mTarget->GetNumContours();
	DashLines = mTarget->GetDashLine();
	ShowZeroContour = mTarget->GetShowZeroContour();
	DisplayPlane = mTarget->ShowPlottingPlane();
	UpdateTest = false;
}
bool General2DSurfPane::UpdateNeeded(void) 
{
	bool result = UpdateTest;
	
	if (mTarget->GridAvailable()) {	//Don't update unless a valid 2D grid has been read in from file
		if (Visible != mTarget->GetVisibility()) result = true;
		if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
		if (NumContours != mTarget->GetNumContours()) result = true;
		if (MaxContourValue != mTarget->GetMaxValue()) result = true;
		if (ShowZeroContour != mTarget->GetShowZeroContour()) result = true;
		if (DashLines != mTarget->GetDashLine()) result = true;
		if (DisplayPlane != mTarget->ShowPlottingPlane()) result = true;
		if (!result) {
			RGBColor	testColor;
			mTarget->GetPosColor(&testColor);
			if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)||
				(PosColor.blue!=testColor.blue)) result=true;
			mTarget->GetNegColor(&testColor);
			if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)||
				(NegColor.blue!=testColor.blue)) result=true;
		}
	}
	return result;
}
void General2DSurfPane::OnUpdate(wxCommandEvent &event) {
	SetNumContoursText();
	SetContourValueText();

	mTarget->SetVisibility(Visible);
	mTarget->SetNumContours(NumContours);
	mTarget->SetMaxValue(MaxContourValue);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetNegColor(&NegColor);
	mTarget->SetShowZeroContour(ShowZeroContour);
	mTarget->SetDashLine(DashLines);
	mTarget->ShowPlottingPlane(DisplayPlane);
	
	UpdateTest = false;
	setUpdateButton();
	owner->SurfaceUpdated();
}

void General2DSurfPane::refreshControls()
{
	wxString tmpStr;
	tmpStr.Printf(wxT("%.2f"), MaxContourValue);
	mContourValText->SetValue(tmpStr);
	tmpStr.Printf(wxT("%ld"), NumContours);
	mNumContourText->SetValue(tmpStr);
	
	SetMultValue();
	mMultCheck->SetValue(useMultValue);
	mSquareCheck->SetValue(squareValues);

	mOrbColor1->setColor(&PosColor);
	mOrbColor2->setColor(&NegColor);
	
	mShowZeroCheck->SetValue(ShowZeroContour);
	mDashCheck->SetValue(DashLines);
	mDisplayPlaneCheck->SetValue(DisplayPlane);
}
void General2DSurfPane::SetMultValue(void) {
	wxString temp;
	temp.Printf(wxT("%.4f"), MultValue);
	mGenMultValue->SetValue(temp);
}

void General2DSurfPane::OnFileButton(wxCommandEvent& event ) {
	//Push out the current validated MultValue before we use it
	SetMultValue();

	mTarget->ReadGrid(squareValues, useMultValue, MultValue);
	UpdateTest = true;
	refreshControls();
	setUpdateButton();
}

void General2DSurfPane::OnMultCheck( wxCommandEvent &event )
{
	useMultValue = mMultCheck->GetValue();
	setUpdateButton();
}
void General2DSurfPane::OnSquareCheck( wxCommandEvent &event )
{
	squareValues = mSquareCheck->GetValue();
	setUpdateButton();
}
void General2DSurfPane::OnMultValueEnter(wxCommandEvent& event )
{
	double newVal=0.0;
	wxString temp = mGenMultValue->GetValue();
	
	if (temp.ToDouble(&newVal)) {
		MultValue = newVal;
	}
	setUpdateButton();
}

/*!
* Get bitmap resources
 */

wxBitmap General2DSurfPane::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
	////@begin Orbital3D bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
	////@end Orbital3D bitmap retrieval
}

/*!
* Get icon resources
 */

wxIcon General2DSurfPane::GetIconResource( const wxString& name )
{
    // Icon retrieval
	////@begin Orbital3D icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
	////@end Orbital3D icon retrieval
}

/*!
* TEDensity2DSurfPane class
 */

TEDensity2DSurfPane::TEDensity2DSurfPane( wxWindow* parent, TEDensity2DSurface* target, 
									  SurfacesWindow* o, wxWindowID id,
									  const wxPoint& pos, const wxSize& size, 
									  long style ) 
: Surface2DPane(parent, target, o, id, pos, size, style)
{
	mTarget = target;
	
	TargetToPane();
	CreateControls();
	BuildOrbSetPopup();
	refreshControls();
}

TEDensity2DSurfPane::~TEDensity2DSurfPane()
{
	
}

void BaseSurfacePane::BuildOrbSetPopup(void) {
	MoleculeData * mData = owner->GetMoleculeData();
	Frame * lFrame = mData->GetCurrentFramePtr();
	const vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();
	
	short item, numitems;
	
	item = 0; numitems = 0;
	
	if (Orbs->size() > 0) {
		std::vector<OrbitalRec *>::const_iterator OrbSet = Orbs->begin();
		long	OrbSetCount = 0;
		while (OrbSet != Orbs->end()) {
			if ((*OrbSet)->TotalDensityPossible()) {
				switch ((*OrbSet)->getOrbitalType()) {
					case OptimizedOrbital:
						if ((*OrbSet)->getOrbitalWavefunctionType() == MCSCF)
							mOrbSetChoice->Append(wxString(_T("MCSCF Optimized Orbitals")));
						else
							mOrbSetChoice->Append(wxString(_T("Molecular EigenVectors")));
						break;
					case NaturalOrbital:
						switch ((*OrbSet)->getOrbitalWavefunctionType()) {
							case UHF:
								mOrbSetChoice->Append(wxString(_T("UHF Natural Orbitals")));
								break;
							case GVB:
								mOrbSetChoice->Append(wxString(_T("GVB GI Orbitals")));
								break;
							case MCSCF:
								mOrbSetChoice->Append(wxString(_T("MCSCF Natural Orbitals")));
								break;
							case CI:
								mOrbSetChoice->Append(wxString(_T("CI Natural Orbitals")));
								break;
							case RHFMP2:
								mOrbSetChoice->Append(wxString(_T("RMP2 Natural Orbitals")));
								break;
							default:
								mOrbSetChoice->Append(wxString(_T("Natural Orbitals")));
						}
						break;
					case LocalizedOrbital:
						mOrbSetChoice->Append(wxString(_T("Localized Orbitals")));
						break;
					case OrientedLocalizedOrbital:
						mOrbSetChoice->Append(wxString(_T("Oriented Localized Orbitals")));
						break;
					case GuessOrbital:
						mOrbSetChoice->Append(wxString(_T("Initial Guess Orbitals")));
						break;
					default:
						mOrbSetChoice->Append(wxString(_T("Molecular Orbitals")));
				}
			}
			if (TargetOrbSet < 0) {
				if (((*OrbSet)->getOrbitalType() != GuessOrbital) ||
					((OrbSetCount+1)>=Orbs->size()))
					TargetOrbSet = numitems;	//default to the first valid set
			}
			if (TargetOrbSet == OrbSetCount) item = numitems+1;
			numitems++;
			OrbSetCount++;
			OrbSet++;
		}
	}
}
void BaseSurfacePane::OnOrbSetChoice( wxCommandEvent &event )
{
	TargetOrbSet = mOrbSetChoice->GetSelection();
	setUpdateButton();
}	

void TEDensity2DSurfPane::CreateControls() {
	TEDensity2DSurfPane * TED2DPANEL = this;	//this is here just to match the code below from DialogBlocks
	
	wxBoxSizer* itemBoxSizer68 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer68, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText69 = new wxStaticText( TED2DPANEL, wxID_STATIC, _("Select Orbital Set:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer68->Add(itemStaticText69, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    wxString* mOrbSetChoiceStrings = NULL;
    mOrbSetChoice = new wxChoice( TED2DPANEL, ID_ORB_CHOICE, wxDefaultPosition, wxDefaultSize, 0, mOrbSetChoiceStrings, 0 );
    if (ShowToolTips())
        mOrbSetChoice->SetToolTip(_("Choose the set of vectors to use for producing the TED surface."));
    itemBoxSizer68->Add(mOrbSetChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer71 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer71, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText72 = new wxStaticText( TED2DPANEL, wxID_STATIC, _("Number of grid points:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer71->Add(itemStaticText72, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mNumGridPntSld = new wxSlider( TED2DPANEL, ID_GRID_POINT_SLIDER, 0, 0, 100, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_LABELS );
    itemBoxSizer71->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer74 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer74, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText75 = new wxStaticText( TED2DPANEL, wxID_STATIC, _("Max. # of contours:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer74->Add(itemStaticText75, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mNumContourText = new wxTextCtrl( TED2DPANEL, ID_NUM_CONTOUR_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer74->Add(mNumContourText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer77 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer77, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText78 = new wxStaticText( TED2DPANEL, wxID_STATIC, _("Max. contour value:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer77->Add(itemStaticText78, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mContourValText = new wxTextCtrl( TED2DPANEL, ID_CONTOUR_VALUE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer77->Add(mContourValText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mUsePlaneChk = new wxCheckBox( TED2DPANEL, ID_USE_PLANE_CHECKBOX, _("Use plane of screen"), wxDefaultPosition, wxDefaultSize, 0 );
    mUsePlaneChk->SetValue(false);
    mainSizer->Add(mUsePlaneChk, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mDisplayPlaneCheck = new wxCheckBox( TED2DPANEL, ID_DISPLAY_PLANE_CHECKBOX, _("Display Plotting Plane"), wxDefaultPosition, wxDefaultSize, 0 );
    mDisplayPlaneCheck->SetValue(false);
    if (ShowToolTips())
        mDisplayPlaneCheck->SetToolTip(_("Display a translucent plane with border to indicate the plotting plane."));
    mainSizer->Add(mDisplayPlaneCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer81 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer81, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText82 = new wxStaticText( TED2DPANEL, wxID_STATIC, _("Contour color:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer81->Add(itemStaticText82, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mOrbColor1 = new colorArea( TED2DPANEL, ID_2D_COLOR_POSITIVE, &PosColor );
    itemBoxSizer81->Add(mOrbColor1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer84 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer84, 0, wxALIGN_RIGHT|wxALL, 5);
    wxBoxSizer* itemBoxSizer85 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer84->Add(itemBoxSizer85, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    mSetParamBut = new wxButton( TED2DPANEL, ID_SET_PARAM_BUT, _("Parameters..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer85->Add(mSetParamBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mSetPlaneBut = new wxButton( TED2DPANEL, ID_SET_PLANE_BUT, _("Set Plane..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer85->Add(mSetPlaneBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer88 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer84->Add(itemBoxSizer88, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    mExportBut = new wxButton( TED2DPANEL, ID_SURFACE_EXPORT_BUT, _("Export..."), wxDefaultPosition, wxDefaultSize, 0 );

    if (mTarget->ExportPossible())
      mExportBut->Enable();
    else
      mExportBut->Disable();

    itemBoxSizer88->Add(mExportBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mUpdateBut = new wxButton( TED2DPANEL, ID_SURFACE_UPDATE_BUT, _("Update"), wxDefaultPosition, wxDefaultSize, 0 );
    mUpdateBut->SetDefault();
    itemBoxSizer88->Add(mUpdateBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
}
void TEDensity2DSurfPane::TargetToPane(void) 
{
	TargetOrbSet = mTarget->getTargetOrbSet();
	NumGridPoints = mTarget->GetNumGridPoints();
	mTarget->GetPosColor(&PosColor);
	MaxContourValue = mTarget->GetMaxValue();
	NumContours = mTarget->GetNumContours();
	UseScreenPlane = mTarget->GetRotate2DMap();
	DisplayPlane = mTarget->ShowPlottingPlane();
	Visible = mTarget->GetVisibility();
	AllFrames = (mTarget->GetSurfaceID() != 0);
	UpdateTest = false;
}
bool TEDensity2DSurfPane::UpdateNeeded(void) 
{
	bool result = UpdateTest;
	
	if (Visible != mTarget->GetVisibility()) result = true;
	if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
	if (NumContours != mTarget->GetNumContours()) result = true;
	if (MaxContourValue != mTarget->GetMaxValue()) result = true;
	if (UseScreenPlane != mTarget->GetRotate2DMap()) result = true;
	if (DisplayPlane != mTarget->ShowPlottingPlane()) result = true;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) result = true;
	if (!result) {
		RGBColor	testColor;
		mTarget->GetPosColor(&testColor);
		if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)||
			(PosColor.blue!=testColor.blue)) result=true;
	}
	return result;
}
void TEDensity2DSurfPane::OnUpdate(wxCommandEvent &event) {
	SetNumContoursText();
	SetContourValueText();
	
	bool	updateGrid = UpdateTest;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) {
		mTarget->setTargetOrbSet(TargetOrbSet);
		updateGrid = true;
	}
	if (NumGridPoints != mTarget->GetNumGridPoints()) {
		mTarget->SetNumGridPoints(NumGridPoints);
		updateGrid = true;
	}
	mTarget->SetVisibility(Visible);
	mTarget->SetNumContours(NumContours);
	mTarget->SetMaxValue(MaxContourValue);
	mTarget->SetPosColor(&PosColor);
	if (UseScreenPlane && !mTarget->GetRotate2DMap()) updateGrid = true;
	mTarget->SetRotate2DMap(UseScreenPlane);
	mTarget->ShowPlottingPlane(DisplayPlane);
	
	MoleculeData * mData = owner->GetMoleculeData();
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) {	//update all frames
		long	SurfaceID;
		Frame *	lFrame = mData->GetFirstFrame();
		if (AllFrames) {	//adding the surface to all frames
			SurfaceID = mTarget->SetSurfaceID();
			while (lFrame) {
				if (lFrame != mData->GetCurrentFramePtr()) {
					TEDensity2DSurface * NewSurface = new TEDensity2DSurface(mTarget);
					lFrame->AppendSurface(NewSurface);
				}
				lFrame = lFrame->GetNextFrame();
			}
		} else {			//deleting the surface from other frames
			SurfaceID = mTarget->GetSurfaceID();
			mTarget->SetSurfaceID(0);	//Unmark this frames surface so it doesn't get deleted
			while (lFrame) {
				lFrame->DeleteSurfaceWithID(SurfaceID);
				lFrame = lFrame->GetNextFrame();
			}
		}
	} else if (AllFrames) {
		long SurfaceID = mTarget->GetSurfaceID();
		Frame * lFrame = mData->GetFirstFrame();
		while (lFrame) {
			if (lFrame != mData->GetCurrentFramePtr()) {
				Surface * temp = lFrame->GetSurfaceWithID(SurfaceID);
				TEDensity2DSurface * lSurf = NULL;
				if (temp)
					if (temp->GetSurfaceType() == kTotalDensity2D)
						lSurf = (TEDensity2DSurface *) temp;
				if (lSurf) {
					lSurf->UpdateData(mTarget);
					if (updateGrid) lSurf->FreeGrid();
				}
			}
			lFrame = lFrame->GetNextFrame();
		}
	}	//if the grid needs updating, release the current grid to mark it for recalc.
	if (updateGrid) mTarget->FreeGrid();
	UpdateTest = false;
	setUpdateButton();
	mExportBut->Enable();
	owner->SurfaceUpdated();
}

void TEDensity2DSurfPane::refreshControls()
{
	mOrbSetChoice->SetSelection(TargetOrbSet);
	SetNumContoursText();
	SetContourValueText();
	
	mOrbColor1->setColor(&PosColor);
	
	mNumGridPntSld->SetValue(NumGridPoints);
	
	mUsePlaneChk->SetValue(UseScreenPlane);
	mDisplayPlaneCheck->SetValue(DisplayPlane);
}
/*!
* Get bitmap resources
 */

wxBitmap TEDensity2DSurfPane::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
	////@begin Orbital3D bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
	////@end Orbital3D bitmap retrieval
}

/*!
* Get icon resources
 */

wxIcon TEDensity2DSurfPane::GetIconResource( const wxString& name )
{
    // Icon retrieval
	////@begin Orbital3D icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
	////@end Orbital3D icon retrieval
}

/*!
* TEDensity3DSurfPane class
 */

TEDensity3DSurfPane::TEDensity3DSurfPane( wxWindow* parent, TEDensity3DSurface* target, 
										  SurfacesWindow* o, wxWindowID id,
										  const wxPoint& pos, const wxSize& size, 
										  long style ) 
: Surface3DPane(parent, target, o, id, pos, size, style)
{
	mTarget = target;
	
	TargetToPane();
	CreateControls();
	BuildOrbSetPopup();
	refreshControls();
}

TEDensity3DSurfPane::~TEDensity3DSurfPane()
{
	
}
void TEDensity3DSurfPane::TargetToPane(void) 
{
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	mTarget->GetTranspColor(&TranspColor);
	ContourValue = mTarget->GetContourValue();
	UseSolidSurface = mTarget->SolidSurface();
	UseNormals = mTarget->UseSurfaceNormals();
	NumGridPoints = mTarget->GetNumGridPoints();
	TargetOrbSet = mTarget->getTargetOrbSet();
	GridSize = mTarget->GetGridSize();
	Visible = mTarget->GetVisibility();
	AllFrames = (mTarget->GetSurfaceID() != 0);
	UseMEP = mTarget->ColorByValue();
	UseRGBSurfaceColor = mTarget->UseRGBColoration();
	InvertRGBSurfaceColor = mTarget->InvertRGBColoration();
	MaxMEPValue = mTarget->GetMaxSurfaceValue();
	UpdateTest = false;
}
void TEDensity3DSurfPane::refreshControls()
{
	mOrbSetChoice->SetSelection(TargetOrbSet);
	SetContourValueText();
	setContourValueSld();
	SetContourMaxValueText();
	
	mOrbColor1->setColor(&PosColor);
	mOrbColor2->setColor(&NegColor);
	mTransColor->setColor(&TranspColor);

	mColorSurfCheck->SetValue(UseMEP);
	mUseRGBColorCheck->SetValue(UseRGBSurfaceColor);
	if (UseMEP) {
		mUseRGBColorCheck->Enable();
		if (UseRGBSurfaceColor) mInvertRGBCheck->Enable();
		else mInvertRGBCheck->Disable();
		mMaxMapEdit->Enable();
	} else {
		mUseRGBColorCheck->Disable();
		mInvertRGBCheck->Disable();
		mMaxMapEdit->Disable();
	}
	SetMaxMEPValueText();
	mNumGridPntSld->SetValue(NumGridPoints);
	mGridSizeSld->SetValue((short)(100*GridSize));

	m3DRdoBox->SetSelection(1-UseSolidSurface);
	mSmoothChkBox->SetValue(UseNormals);
	
	if (UseSolidSurface)
		mSmoothChkBox->Enable();
	else
		mSmoothChkBox->Disable();
}
void TEDensity3DSurfPane::OnUseMEPCheck( wxCommandEvent &event )
{
	UseMEP = mColorSurfCheck->GetValue();
	refreshControls();
	setUpdateButton();
}
void TEDensity3DSurfPane::OnRGBColorCheck( wxCommandEvent &event )
{
	UseRGBSurfaceColor = mUseRGBColorCheck->GetValue();
	refreshControls();
	setUpdateButton();
}
void TEDensity3DSurfPane::OnInvertRGBCheck( wxCommandEvent &event )
{
	InvertRGBSurfaceColor = mInvertRGBCheck->GetValue();
	//	refreshControls();
	setUpdateButton();
}
void TEDensity3DSurfPane::OnMaxMEPValueText(wxCommandEvent& event )
{
	double newVal=0.0;
	wxString temp = mMaxMapEdit->GetValue();
	
	if (temp.ToDouble(&newVal)) {
		MaxMEPValue = newVal;
	}
	setUpdateButton();
}
void TEDensity3DSurfPane::SetMaxMEPValueText(void) {
	wxString temp;
	temp.Printf(wxT("%.4f"), MaxMEPValue);
	mMaxMapEdit->SetValue(temp);
}
bool TEDensity3DSurfPane::UpdateNeeded(void) 
{
	bool result = UpdateTest;
	
	if (Visible != mTarget->GetVisibility()) result = true;
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) result = true;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) result = true;
	if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
	if (ContourValue != mTarget->GetContourValue()) result = true;
	if (GridSize != mTarget->GetGridSize()) result = true;
	if (UseSolidSurface != mTarget->SolidSurface()) result = true;
	if (UseMEP != mTarget->ColorByValue()) result = true;
	if (UseNormals != mTarget->UseSurfaceNormals()) result = true;
	if (UseRGBSurfaceColor != mTarget->UseRGBColoration()) result = true;
	if (InvertRGBSurfaceColor != mTarget->InvertRGBColoration()) result = true;
	if (MaxMEPValue != mTarget->GetMaxSurfaceValue()) result = true;
	if (!result) {
		RGBColor	testColor;
		mTarget->GetPosColor(&testColor);
		if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)||
			(PosColor.blue!=testColor.blue)) result=true;
		mTarget->GetNegColor(&testColor);
		if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)||
			(NegColor.blue!=testColor.blue)) result=true;
		mTarget->GetTranspColor(&testColor);
		if ((TranspColor.red != testColor.red)||(TranspColor.green!=testColor.green)||
			(TranspColor.blue!=testColor.blue)) result=true;
	}
	return result;
}
void TEDensity3DSurfPane::OnUpdate(wxCommandEvent &event) {
	SetMaxMEPValueText();
	SetContourValueText();
	
	//only update the grid if needed
	bool updateGrid=UpdateTest, updateContour=false, updateMEP=false;
	if (TargetOrbSet != mTarget->getTargetOrbSet()) {
		updateGrid = true;
		mTarget->SetFixGrid(false);
	}
	if (NumGridPoints != mTarget->GetNumGridPoints()) {
		updateGrid = true;
		mTarget->SetFixGrid(false);
	}
	if (ContourValue != mTarget->GetContourValue()) updateContour = true;
	if (GridSize != mTarget->GetGridSize()) {
		updateGrid = true;
		mTarget->SetFixGrid(false);
	}
	if (Visible && !mTarget->ContourAvail()) updateContour = true;
	if (UseMEP && !mTarget->ColorByValue()) updateMEP = true;
	if (updateMEP && !mTarget->ContourAvail()) updateContour = true;
	//test to see if grid/contour must be updated anyway...
	if (updateContour && ! mTarget->GridAvailable()) updateGrid = true;
	if (updateGrid) updateContour = true;
	if (updateContour && UseMEP) updateMEP = true;
	mTarget->SetVisibility(Visible);
	mTarget->SolidSurface(UseSolidSurface);
	mTarget->setTargetOrbSet(TargetOrbSet);
	mTarget->SetNumGridPoints(NumGridPoints);
	mTarget->SetContourValue(ContourValue);
	mTarget->SetMaxSurfaceValue(MaxMEPValue);
	mTarget->SetGridSize(GridSize);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetNegColor(&NegColor);
	mTarget->SetTranspColor(&TranspColor);
	mTarget->SetColorByValue(UseMEP);
	mTarget->UseSurfaceNormals(UseNormals);
	mTarget->UseRGBColoration(UseRGBSurfaceColor);
	mTarget->InvertRGBColoration(InvertRGBSurfaceColor);
	MoleculeData * data = owner->GetMoleculeData();
	WinPrefs * Prefs = owner->GetPrefs();
	//update this surface's data on all frames if necessary
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) {	//update all frames
		long	SurfaceID;
		Frame *	lFrame = data->GetFirstFrame();
		updateGrid = updateContour = true;
		if (AllFrames) {	//adding the surface to all frames
			SurfaceID = mTarget->SetSurfaceID();
			while (lFrame) {
				if (lFrame != data->GetCurrentFramePtr()) {
					TEDensity3DSurface * NewSurface = new TEDensity3DSurface(Prefs);
					if (NewSurface) {
						NewSurface->SetSurfaceID(SurfaceID);
						lFrame->AppendSurface(NewSurface);
					}
				}
				lFrame = lFrame->GetNextFrame();
			}
		} else {			//deleting the surface from other frames
			SurfaceID = mTarget->GetSurfaceID();
			mTarget->SetSurfaceID(0);	//Unmark this frames surface so it doesn't get deleted
			while (lFrame) {
				lFrame->DeleteSurfaceWithID(SurfaceID);
				lFrame = lFrame->GetNextFrame();
			}
		}
	}
	Progress * lProgress = new Progress();
//	if (!lProgress) throw MemoryError();
	if (AllFrames) {	//compute the contour for each frame, no grid is kept
		long SurfaceID = mTarget->GetSurfaceID();
		long CurrentFrame = data->GetCurrentFrame();
		long NumFrames = data->GetNumFrames();
		for (int i=0; i<NumFrames; i++) {
			TEDensity3DSurface * lSurf;
			lSurf = NULL;
			data->SetCurrentFrame(i+1);
			Frame * lFrame = data->GetCurrentFramePtr();
			if (CurrentFrame != data->GetCurrentFrame()) {
				Surface * temp = lFrame->GetSurfaceWithID(SurfaceID);
				//Confirm that the surface is the correct type
				if (temp)
					if (temp->GetSurfaceType() == kTotalDensity3D)
						lSurf = (TEDensity3DSurface *) temp;
				if (lSurf) lSurf->UpdateData(mTarget);
			} else lSurf = mTarget;
			if (lSurf) {
				if (!UseMEP) mTarget->FreeList();
				if (Visible) {
					float MEPScale = 1.0;
					if (updateMEP) MEPScale = 0.5;
					lProgress->ChangeText("Calculating 3D Grid...");
					lProgress->SetBaseValue((long)(100*i*MEPScale/NumFrames));
					lProgress->SetScaleFactor((float) 0.9*MEPScale/NumFrames);
					if (updateGrid) lSurf->CalculateMOGrid(data, lProgress);
					lProgress->ChangeText("Contouring grid...");
					lProgress->SetBaseValue((long)(100*i/NumFrames + 90.0*MEPScale/NumFrames));
					lProgress->SetScaleFactor((float) 0.1*MEPScale/NumFrames);
					if (updateContour) lSurf->Contour3DGrid(lProgress);
					if (updateMEP) {
						lProgress->ChangeText("Calculating MEP values...");
						lProgress->SetBaseValue(100*i/NumFrames + 50/NumFrames);
						lProgress->SetScaleFactor(0.5/NumFrames);
						mTarget->CalculateSurfaceValues(data, lProgress);
					}
					lSurf->FreeGrid();
				} else {
					if (updateContour) lSurf->FreeContour();
					if (updateMEP) lSurf->FreeList();
				}
			}
		}
		data->SetCurrentFrame(CurrentFrame);
	} else {	//simply update this surface
		if (!UseMEP) mTarget->FreeList();
		if (Visible) {
			float MEPScale = 1.0;
			if (updateMEP) MEPScale = 0.5;
			if (updateGrid) {
				lProgress->ChangeText("Calculating 3D Grid...");
				lProgress->SetScaleFactor(0.9*MEPScale);
				mTarget->CalculateMOGrid(data, lProgress);
			}
			if (updateContour) {
				lProgress->ChangeText("Contouring grid...");
				lProgress->SetBaseValue((long)(90*MEPScale));
				lProgress->SetScaleFactor(0.1*MEPScale);
				mTarget->Contour3DGrid(lProgress);
			}
			if (updateMEP) {
				lProgress->ChangeText("Calculating MEP values...");
				lProgress->SetBaseValue(50);
				lProgress->SetScaleFactor(0.5);
				mTarget->CalculateSurfaceValues(data, lProgress);
			}
		} else {
			//always free the grid for invisable surfaces since they can be big
			mTarget->FreeGrid();
			//free the contour so that it will be updated when it is made visible
			if (updateContour) mTarget->FreeContour();
			if (updateMEP) mTarget->FreeList();
		}
	}
	if (lProgress) delete lProgress;

	if (mTarget->GridAvailable())
	  mFreeMemBut->Enable();
	else
	  mFreeMemBut->Disable();

/*	AbleDItem(SurfaceDlg, kSurfBaseItems+kTE3DFreeMem, target->GridAvailable());
	//Setup the contour value and grid max text items
	short	itemtype;
	Handle	itemhandle;
	Rect	itemrect;
	GetDialogItem(SurfaceDlg, kSurfBaseItems+kTE3DValue, &itemtype, &itemhandle, &itemrect);
	float ContourValue = target->GetContourValue(), GridMax = target->GetGridMax();
	Str255	itemText;
	sprintf((char *) &(itemText[1]), "%.4f", ContourValue);
	itemText[0] = strlen((char *) &(itemText[1]));
	SetDialogItemText(itemhandle, itemText);
	GetDialogItem(SurfaceDlg, kSurfBaseItems+kTE3DMaxValue, &itemtype, &itemhandle, &itemrect);
	sprintf((char *) &(itemText[1]), "%.4f", GridMax);
	itemText[0] = strlen((char *) &(itemText[1]));
	SetDialogItemText(itemhandle, itemText);
	UpdateTest = false;
	SetupContourSlider();
	AbleDItem(SurfaceDlg, kSurfBaseItems+kTE3DExportButton, target->ExportPossible());
	*/
	setContourValueSld();
	SetContourMaxValueText();
	UpdateTest = false;
	setUpdateButton();

	if (mTarget->ExportPossible())
	  mExportBut->Enable();
	else
	  mExportBut->Disable();

	owner->SurfaceUpdated();
}
void TEDensity3DSurfPane::CreateControls() {
	TEDensity3DSurfPane * TED3DPanel = this;	//this is here just to match the code below from DialogBlocks

	wxBoxSizer* itemBoxSizer93 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer93, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText94 = new wxStaticText( TED3DPanel, wxID_STATIC, _("Select Orbital Set:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer93->Add(itemStaticText94, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    wxString* mOrbSetChoiceStrings = NULL;
    mOrbSetChoice = new wxChoice( TED3DPanel, ID_ORB_CHOICE, wxDefaultPosition, wxDefaultSize, 0, mOrbSetChoiceStrings, 0 );
    if (ShowToolTips())
        mOrbSetChoice->SetToolTip(_("Choose the set of vectors to use for producing the TED surface."));
    itemBoxSizer93->Add(mOrbSetChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer96 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer96, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
    wxStaticText* itemStaticText97 = new wxStaticText( TED3DPanel, wxID_STATIC, _("Number of grid points:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer96->Add(itemStaticText97, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mNumGridPntSld = new wxSlider( TED3DPanel, ID_GRID_POINT_SLIDER, 0, 10, 150, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_LABELS );
    itemBoxSizer96->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer99 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer99, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
    wxStaticText* itemStaticText100 = new wxStaticText( TED3DPanel, wxID_STATIC, _("Grid Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer99->Add(itemStaticText100, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mGridSizeSld = new wxSlider( TED3DPanel, ID_GRID_SIZE_SLIDER, 0, 0, 300, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_HORIZONTAL );
    itemBoxSizer99->Add(mGridSizeSld, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer102 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer102, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
    wxBoxSizer* itemBoxSizer103 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer102->Add(itemBoxSizer103, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText104 = new wxStaticText( TED3DPanel, wxID_STATIC, _("Contour Value:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer103->Add(itemStaticText104, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mContourValueEdit = new wxTextCtrl( TED3DPanel, ID_CONTOUR_VALUE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer103->Add(mContourValueEdit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer106 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer102->Add(itemBoxSizer106, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    mContourValSld = new wxSlider( TED3DPanel, ID_CONTOUR_VALUE_SLIDER, 0, 0, 100, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_HORIZONTAL );
    itemBoxSizer106->Add(mContourValSld, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	
    wxBoxSizer* itemBoxSizer108 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer106->Add(itemBoxSizer108, 1, wxGROW|wxLEFT|wxTOP|wxBOTTOM, 5);
    mGridMinText = new wxStaticText( TED3DPanel, ID_GENSURFGRIDMINTEXT, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
    itemBoxSizer108->Add(mGridMinText, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mGridMaxText = new wxStaticText( TED3DPanel, ID_GRID_MAX_TEXT, _("1"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    itemBoxSizer108->Add(mGridMaxText, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    wxBoxSizer* itemBoxSizer111 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer111, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
    wxBoxSizer* itemBoxSizer112 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer111->Add(itemBoxSizer112, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    mColorSurfCheck = new wxCheckBox( TED3DPanel, ID_TED3D_COLOR_SURF_CHECK, _("Colorize using the surface MEP value"), wxDefaultPosition, wxDefaultSize, 0 );
    mColorSurfCheck->SetValue(false);
    itemBoxSizer112->Add(mColorSurfCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mUseRGBColorCheck = new wxCheckBox( TED3DPanel, ID_USERGB_COLOR_CHECK, _("Use RGB surface coloration"), wxDefaultPosition, wxDefaultSize, 0 );
	if (ShowToolTips())
        mUseRGBColorCheck->SetToolTip(_("Uses blue for attractive, green for neutral, and red of repulsive to a + charge"));
	mUseRGBColorCheck->SetValue(false);
    itemBoxSizer112->Add(mUseRGBColorCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mInvertRGBCheck = new wxCheckBox( TED3DPanel, ID_INVERT_RGB_CHECK, _("Invert color map"), wxDefaultPosition, wxDefaultSize, 0 );
	mInvertRGBCheck->SetValue(false);
	if (ShowToolTips())
	   mInvertRGBCheck->SetToolTip(_("Flips the RGB mapping so that red is attractive, blue replusive to a + charge"));
	itemBoxSizer112->Add(mInvertRGBCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	   
    wxBoxSizer* itemBoxSizer115 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer111->Add(itemBoxSizer115, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxStaticText* itemStaticText116 = new wxStaticText( TED3DPanel, wxID_STATIC, _("Max. value to map"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer115->Add(itemStaticText116, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mMaxMapEdit = new wxTextCtrl( TED3DPanel, ID_TED3D_MAX_MAP_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer115->Add(mMaxMapEdit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer118 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer118, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
    wxBoxSizer* itemBoxSizer119 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer118->Add(itemBoxSizer119, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxBoxSizer* itemBoxSizer120 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer119->Add(itemBoxSizer120, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
    wxStaticText* itemStaticText121 = new wxStaticText( TED3DPanel, wxID_STATIC, _("Surface Color:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer120->Add(itemStaticText121, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mOrbColor1 = new colorArea( TED3DPanel, ID_3D_COLOR_POSITIVE, &PosColor );
    itemBoxSizer120->Add(mOrbColor1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mOrbColor2 = new colorArea( TED3DPanel, ID_3D_COLOR_NEGATIVE, &NegColor);
    itemBoxSizer120->Add(mOrbColor2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer124 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer119->Add(itemBoxSizer124, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
    wxStaticText* itemStaticText125 = new wxStaticText( TED3DPanel, wxID_STATIC, _("Transparency color:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer124->Add(itemStaticText125, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mTransColor = new colorArea( TED3DPanel, ID_TRANSPARENCY_COLOR, &TranspColor );
    itemBoxSizer124->Add(mTransColor, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer127 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer119->Add(itemBoxSizer127, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
    wxString m3DRdoBoxStrings[] = {
        _("&Solid"),
        _("&WireFrame")
    };
    m3DRdoBox = new wxRadioBox( TED3DPanel, ID_3D_RADIOBOX, _("Surface Display"), wxDefaultPosition, wxDefaultSize, 2, m3DRdoBoxStrings, 1, wxRA_SPECIFY_ROWS );
    m3DRdoBox->SetSelection(0);
    itemBoxSizer127->Add(m3DRdoBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mSmoothChkBox = new wxCheckBox( TED3DPanel, ID_SMOOTH_CHECKBOX, _("Smooth"), wxDefaultPosition, wxDefaultSize, 0 );
    mSmoothChkBox->SetValue(false);
    if (ShowToolTips())
        mSmoothChkBox->SetToolTip(_("Check to use surface normals to smooth out the surface of the contour."));
    itemBoxSizer127->Add(mSmoothChkBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer130 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer118->Add(itemBoxSizer130, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    mSetParamBut = new wxButton( TED3DPanel, ID_SET_PARAM_BUT, _("Parameters..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer130->Add(mSetParamBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mFreeMemBut = new wxButton( TED3DPanel, ID_FREE_MEM_BUT, _("Free Mem"), wxDefaultPosition, wxDefaultSize, 0 );

    if (mTarget->GridAvailable())
      mFreeMemBut->Enable();
    else
      mFreeMemBut->Disable();

    if (ShowToolTips())
        mFreeMemBut->SetToolTip(_("Click to free the memory used by the 3D grid."));
    itemBoxSizer130->Add(mFreeMemBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mExportBut = new wxButton( TED3DPanel, ID_SURFACE_EXPORT_BUT, _("Export..."), wxDefaultPosition, wxDefaultSize, 0 );

    if (mTarget->ExportPossible())
      mExportBut->Enable();
    else
      mExportBut->Disable();

    itemBoxSizer130->Add(mExportBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mUpdateBut = new wxButton( TED3DPanel, ID_SURFACE_UPDATE_BUT, _("Update"), wxDefaultPosition, wxDefaultSize, 0 );
    mUpdateBut->SetDefault();
    itemBoxSizer130->Add(mUpdateBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
}
/*!
* Get bitmap resources
 */

wxBitmap TEDensity3DSurfPane::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
	////@begin Orbital3D bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
	////@end Orbital3D bitmap retrieval
}

/*!
* Get icon resources
 */

wxIcon TEDensity3DSurfPane::GetIconResource( const wxString& name )
{
    // Icon retrieval
	////@begin Orbital3D icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
	////@end Orbital3D icon retrieval
}
/*!
* TEDensity2DSurfPane class
 */

MEP2DSurfPane::MEP2DSurfPane( wxWindow* parent, MEP2DSurface* target, 
										  SurfacesWindow* o, wxWindowID id,
										  const wxPoint& pos, const wxSize& size, 
										  long style ) 
: Surface2DPane(parent, target, o, id, pos, size, style)
{
	mTarget = target;
	
	TargetToPane();
	CreateControls();
	BuildOrbSetPopup();
	refreshControls();
}

MEP2DSurfPane::~MEP2DSurfPane()
{
	
}
void MEP2DSurfPane::CreateControls() {
	MEP2DSurfPane * MEP2DPanel = this;	//this is here just to match the code below from DialogBlocks

	wxBoxSizer* itemBoxSizer137 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer137, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText138 = new wxStaticText( MEP2DPanel, wxID_STATIC, _("Select Orbital Set:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer137->Add(itemStaticText138, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    wxString* mOrbSetChoiceStrings = NULL;
    mOrbSetChoice = new wxChoice( MEP2DPanel, ID_ORB_CHOICE, wxDefaultPosition, wxDefaultSize, 0, mOrbSetChoiceStrings, 0 );
    if (ShowToolTips())
        mOrbSetChoice->SetToolTip(_("Choose the set of vectors to use for producing the MEP surface."));
    itemBoxSizer137->Add(mOrbSetChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer140 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer140, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText141 = new wxStaticText( MEP2DPanel, wxID_STATIC, _("Number of grid points:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer140->Add(itemStaticText141, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mNumGridPntSld = new wxSlider( MEP2DPanel, ID_GRID_POINT_SLIDER, 0, 0, 100, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_LABELS );
    itemBoxSizer140->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer143 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer143, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText144 = new wxStaticText( MEP2DPanel, wxID_STATIC, _("Max. # of contours:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer143->Add(itemStaticText144, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mNumContourText = new wxTextCtrl( MEP2DPanel, ID_NUM_CONTOUR_TEXT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer143->Add(mNumContourText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer146 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer146, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText147 = new wxStaticText( MEP2DPanel, wxID_STATIC, _("Max. contour value:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer146->Add(itemStaticText147, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mContourValText = new wxTextCtrl( MEP2DPanel, ID_CONTOUR_VALUE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer146->Add(mContourValText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mUsePlaneChk = new wxCheckBox( MEP2DPanel, ID_USE_PLANE_CHECKBOX, _("Use plane of screen"), wxDefaultPosition, wxDefaultSize, 0 );
    mUsePlaneChk->SetValue(false);
    mainSizer->Add(mUsePlaneChk, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mDisplayPlaneCheck = new wxCheckBox( MEP2DPanel, ID_DISPLAY_PLANE_CHECKBOX, _("Display Plotting Plane"), wxDefaultPosition, wxDefaultSize, 0 );
    mDisplayPlaneCheck->SetValue(false);
    if (ShowToolTips())
        mDisplayPlaneCheck->SetToolTip(_("Display a translucent plane with border to indicate the plotting plane"));
    mainSizer->Add(mDisplayPlaneCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mShowZeroCheck = new wxCheckBox( MEP2DPanel, ID_SHOW_ZERO_CHECKBOX, _("Show zero value contour"), wxDefaultPosition, wxDefaultSize, 0 );
    mShowZeroCheck->SetValue(false);
    if (ShowToolTips())
        mShowZeroCheck->SetToolTip(_("Check to produce a gray contour where the plane changes sign."));
    mainSizer->Add(mShowZeroCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mDashCheck = new wxCheckBox( MEP2DPanel, ID_DASH_CHECKBOX, _("Dash negative contours"), wxDefaultPosition, wxDefaultSize, 0 );
    mDashCheck->SetValue(false);
    if (ShowToolTips())
        mDashCheck->SetToolTip(_("Produces dashed negative contour lines for easier viewing in B & W."));
    mainSizer->Add(mDashCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer152 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer152, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
    wxBoxSizer* itemBoxSizer153 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer152->Add(itemBoxSizer153, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
    wxStaticText* itemStaticText154 = new wxStaticText( MEP2DPanel, wxID_STATIC, _("Positive Color:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer153->Add(itemStaticText154, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mOrbColor1 = new colorArea( MEP2DPanel, ID_2D_COLOR_POSITIVE, &PosColor );
    itemBoxSizer153->Add(mOrbColor1, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer156 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer152->Add(itemBoxSizer156, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0);
    wxStaticText* itemStaticText157 = new wxStaticText( MEP2DPanel, wxID_STATIC, _("Negative Color:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer156->Add(itemStaticText157, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mOrbColor2 = new colorArea( MEP2DPanel, ID_2D_COLOR_NEGATIVE, &NegColor);
    itemBoxSizer156->Add(mOrbColor2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer159 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer159, 0, wxALIGN_RIGHT|wxALL, 5);
    wxBoxSizer* itemBoxSizer160 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer159->Add(itemBoxSizer160, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    mSetParamBut = new wxButton( MEP2DPanel, ID_SET_PARAM_BUT, _("Parameters..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer160->Add(mSetParamBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mSetPlaneBut = new wxButton( MEP2DPanel, ID_SET_PLANE_BUT, _("Set Plane..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer160->Add(mSetPlaneBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer163 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer159->Add(itemBoxSizer163, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    mExportBut = new wxButton( MEP2DPanel, ID_SURFACE_EXPORT_BUT, _("Export..."), wxDefaultPosition, wxDefaultSize, 0 );

    if (mTarget->ExportPossible())
      mExportBut->Enable();
    else
      mExportBut->Disable();

    itemBoxSizer163->Add(mExportBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mUpdateBut = new wxButton( MEP2DPanel, ID_SURFACE_UPDATE_BUT, _("Update"), wxDefaultPosition, wxDefaultSize, 0 );
    mUpdateBut->SetDefault();
    itemBoxSizer163->Add(mUpdateBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
}
void MEP2DSurfPane::TargetToPane(void) 
{
	TargetOrbSet = mTarget->getTargetOrbitalSet();
	NumGridPoints = mTarget->GetNumGridPoints();
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	MaxContourValue = mTarget->GetMaxValue();
	NumContours = mTarget->GetNumContours();
	UseScreenPlane = mTarget->GetRotate2DMap();
	DashLines = mTarget->GetDashLine();
	ShowZeroContour = mTarget->GetShowZeroContour();
	DisplayPlane = mTarget->ShowPlottingPlane();
	Visible = mTarget->GetVisibility();
	AllFrames = (mTarget->GetSurfaceID() != 0);
	UpdateTest = false;
}
bool MEP2DSurfPane::UpdateNeeded(void) 
{
	bool result = UpdateTest;
	
	if (Visible != mTarget->GetVisibility()) result = true;
	if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
	if (NumContours != mTarget->GetNumContours()) result = true;
	if (MaxContourValue != mTarget->GetMaxValue()) result = true;
	if (UseScreenPlane != mTarget->GetRotate2DMap()) result = true;
	if (ShowZeroContour != mTarget->GetShowZeroContour()) result = true;
	if (DisplayPlane != mTarget->ShowPlottingPlane()) result = true;
	if (DashLines != mTarget->GetDashLine()) result = true;
	if (TargetOrbSet != mTarget->getTargetOrbitalSet()) result = true;
	if (!result) {
		RGBColor	testColor;
		mTarget->GetPosColor(&testColor);
		if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)||
			(PosColor.blue!=testColor.blue)) result=true;
		mTarget->GetNegColor(&testColor);
		if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)||
			(NegColor.blue!=testColor.blue)) result=true;
	}
	return result;
}
void MEP2DSurfPane::OnUpdate(wxCommandEvent &event) {
	SetNumContoursText();
	SetContourValueText();
	
	bool	updateGrid = UpdateTest;
	if (TargetOrbSet != mTarget->getTargetOrbitalSet()) {
		mTarget->setTargetOrbitalSet(TargetOrbSet);
		updateGrid = true;
	}
	if (NumGridPoints != mTarget->GetNumGridPoints()) {
		mTarget->SetNumGridPoints(NumGridPoints);
		updateGrid = true;
	}
	mTarget->SetVisibility(Visible);
	mTarget->SetNumContours(NumContours);
	mTarget->SetMaxValue(MaxContourValue);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetNegColor(&NegColor);
	mTarget->SetShowZeroContour(ShowZeroContour);
	mTarget->SetDashLine(DashLines);
	mTarget->ShowPlottingPlane(DisplayPlane);
	if (UseScreenPlane && !mTarget->GetRotate2DMap()) updateGrid = true;
	mTarget->SetRotate2DMap(UseScreenPlane);
	
	MoleculeData * mData = owner->GetMoleculeData();
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) {	//update all frames
		long	SurfaceID;
		Frame *	lFrame = mData->GetFirstFrame();
		if (AllFrames) {	//adding the surface to all frames
			SurfaceID = mTarget->SetSurfaceID();
			while (lFrame) {
				if (lFrame != mData->GetCurrentFramePtr()) {
					MEP2DSurface * NewSurface = new MEP2DSurface(mTarget);
					lFrame->AppendSurface(NewSurface);
				}
				lFrame = lFrame->GetNextFrame();
			}
		} else {			//deleting the surface from other frames
			SurfaceID = mTarget->GetSurfaceID();
			mTarget->SetSurfaceID(0);	//Unmark this frames surface so it doesn't get deleted
			while (lFrame) {
				lFrame->DeleteSurfaceWithID(SurfaceID);
				lFrame = lFrame->GetNextFrame();
			}
		}
	} else if (AllFrames) {
		long SurfaceID = mTarget->GetSurfaceID();
		Frame * lFrame = mData->GetFirstFrame();
		while (lFrame) {
			if (lFrame != mData->GetCurrentFramePtr()) {
				Surface * temp = lFrame->GetSurfaceWithID(SurfaceID);
				MEP2DSurface * lSurf = NULL;
				if (temp)
					if (temp->GetSurfaceType() == kOrb2DType)
						lSurf = (MEP2DSurface *) temp;
				if (lSurf) {
					lSurf->UpdateData(mTarget);
					if (updateGrid) lSurf->FreeGrid();
				}
			}
			lFrame = lFrame->GetNextFrame();
		}
	}
	if (updateGrid) mTarget->FreeGrid();
	UpdateTest = false;
	setUpdateButton();
	mExportBut->Enable();
	owner->SurfaceUpdated();
}
void MEP2DSurfPane::refreshControls()
{
	mOrbSetChoice->SetSelection(TargetOrbSet);
	SetNumContoursText();
	SetContourValueText();
	
	mOrbColor1->setColor(&PosColor);
	mOrbColor2->setColor(&NegColor);
	
	mNumGridPntSld->SetValue(NumGridPoints);
	
	mUsePlaneChk->SetValue(UseScreenPlane);
	mShowZeroCheck->SetValue(ShowZeroContour);
	mDashCheck->SetValue(DashLines);
	mDisplayPlaneCheck->SetValue(DisplayPlane);
}
/*!
* Get bitmap resources
 */

wxBitmap MEP2DSurfPane::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
	////@begin bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
	////@end bitmap retrieval
}

/*!
* Get icon resources
 */

wxIcon MEP2DSurfPane::GetIconResource( const wxString& name )
{
    // Icon retrieval
	////@begin icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
	////@end icon retrieval
}

/*!
* MEP3DSurfPane class
 */

MEP3DSurfPane::MEP3DSurfPane( wxWindow* parent, MEP3DSurface* target, 
										  SurfacesWindow* o, wxWindowID id,
										  const wxPoint& pos, const wxSize& size, 
										  long style ) 
: Surface3DPane(parent, target, o, id, pos, size, style)
{
	mTarget = target;
	
	TargetToPane();
	CreateControls();
	BuildOrbSetPopup();
	refreshControls();
}

MEP3DSurfPane::~MEP3DSurfPane()
{
	
}
void MEP3DSurfPane::TargetToPane(void) 
{
	mTarget->GetPosColor(&PosColor);
	mTarget->GetNegColor(&NegColor);
	mTarget->GetTranspColor(&TranspColor);
	ContourValue = mTarget->GetContourValue();
	UseSolidSurface = mTarget->SolidSurface();
	UseNormals = mTarget->UseSurfaceNormals();
	NumGridPoints = mTarget->GetNumGridPoints();
	TargetOrbSet = mTarget->getTargetOrbitalSet();
	GridSize = mTarget->GetGridSize();
	Visible = mTarget->GetVisibility();
	AllFrames = (mTarget->GetSurfaceID() != 0);
	UpdateTest = false;
}
void MEP3DSurfPane::refreshControls()
{
	mOrbSetChoice->SetSelection(TargetOrbSet);
	SetContourValueText();
	setContourValueSld();
	SetContourMaxValueText();
	
	mOrbColor1->setColor(&PosColor);
	mOrbColor2->setColor(&NegColor);
	mTransColor->setColor(&TranspColor);
	
	mNumGridPntSld->SetValue(NumGridPoints);
	mGridSizeSld->SetValue((short)(100*GridSize));
	
	m3DRdoBox->SetSelection(1-UseSolidSurface);
	mSmoothChkBox->SetValue(UseNormals);
	
	if (UseSolidSurface)
		mSmoothChkBox->Enable();
	else
		mSmoothChkBox->Disable();
}
bool MEP3DSurfPane::UpdateNeeded(void) 
{
	bool result = UpdateTest;
	
	if (Visible != mTarget->GetVisibility()) result = true;
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) result = true;
	if (TargetOrbSet != mTarget->getTargetOrbitalSet()) result = true;
	if (NumGridPoints != mTarget->GetNumGridPoints()) result = true;
	if (ContourValue != mTarget->GetContourValue()) result = true;
	if (GridSize != mTarget->GetGridSize()) result = true;
	if (UseSolidSurface != mTarget->SolidSurface()) result = true;
	if (UseNormals != mTarget->UseSurfaceNormals()) result = true;
	if (!result) {
		RGBColor	testColor;
		mTarget->GetPosColor(&testColor);
		if ((PosColor.red != testColor.red)||(PosColor.green!=testColor.green)||
			(PosColor.blue!=testColor.blue)) result=true;
		mTarget->GetNegColor(&testColor);
		if ((NegColor.red != testColor.red)||(NegColor.green!=testColor.green)||
			(NegColor.blue!=testColor.blue)) result=true;
		mTarget->GetTranspColor(&testColor);
		if ((TranspColor.red != testColor.red)||(TranspColor.green!=testColor.green)||
			(TranspColor.blue!=testColor.blue)) result=true;
	}
	return result;
}
void MEP3DSurfPane::OnUpdate(wxCommandEvent &event) {
	SetContourValueText();
	
	//only update the grid if needed
	bool updateGrid=UpdateTest, updateContour=false;
	if (TargetOrbSet != mTarget->getTargetOrbitalSet()) {
		updateGrid = true;
		mTarget->SetFixGrid(false);
	}
	if (NumGridPoints != mTarget->GetNumGridPoints()) {
		updateGrid = true;
		mTarget->SetFixGrid(false);
	}
	if (ContourValue != mTarget->GetContourValue()) updateContour = true;
	if (GridSize != mTarget->GetGridSize()) {
		updateGrid = true;
		mTarget->SetFixGrid(false);
	}
	if (Visible && !mTarget->ContourAvail()) updateContour = true;
	//test to see if grid/contour must be updated anyway...
	if (updateContour && ! mTarget->GridAvailable()) updateGrid = true;
	if (updateGrid) updateContour = true;
	mTarget->SetVisibility(Visible);
	mTarget->SolidSurface(UseSolidSurface);
	mTarget->setTargetOrbitalSet(TargetOrbSet);
	mTarget->SetNumGridPoints(NumGridPoints);
	mTarget->SetContourValue(ContourValue);
	mTarget->SetGridSize(GridSize);
	mTarget->SetPosColor(&PosColor);
	mTarget->SetNegColor(&NegColor);
	mTarget->SetTranspColor(&TranspColor);
	mTarget->UseSurfaceNormals(UseNormals);
	MoleculeData * data = owner->GetMoleculeData();
	WinPrefs * Prefs = owner->GetPrefs();
	//update this surface's data on all frames if necessary
	if (AllFrames != (mTarget->GetSurfaceID() != 0)) {	//update all frames
		long	SurfaceID;
		Frame *	lFrame = data->GetFirstFrame();
		updateGrid = updateContour = true;
		if (AllFrames) {	//adding the surface to all frames
			SurfaceID = mTarget->SetSurfaceID();
			while (lFrame) {
				if (lFrame != data->GetCurrentFramePtr()) {
					MEP3DSurface * NewSurface = new MEP3DSurface(Prefs);
					if (NewSurface) {
						NewSurface->SetSurfaceID(SurfaceID);
						lFrame->AppendSurface(NewSurface);
					}
				}
				lFrame = lFrame->GetNextFrame();
			}
		} else {			//deleting the surface from other frames
			SurfaceID = mTarget->GetSurfaceID();
			mTarget->SetSurfaceID(0);	//Unmark this frames surface so it doesn't get deleted
			while (lFrame) {
				lFrame->DeleteSurfaceWithID(SurfaceID);
				lFrame = lFrame->GetNextFrame();
			}
		}
	}
	Progress * lProgress = new Progress();
	if (AllFrames) {	//compute the contour for each frame, no grid is kept
		long SurfaceID = mTarget->GetSurfaceID();
		long CurrentFrame = data->GetCurrentFrame();
		long NumFrames = data->GetNumFrames();
		for (int i=0; i<NumFrames; i++) {
			MEP3DSurface * lSurf;
			lSurf = NULL;
			data->SetCurrentFrame(i+1);
			Frame * lFrame = data->GetCurrentFramePtr();
			if (CurrentFrame != data->GetCurrentFrame()) {
				Surface * temp = lFrame->GetSurfaceWithID(SurfaceID);
				//Confirm that the surface is the correct type
				if (temp)
					if (temp->GetSurfaceType() == kTotalDensity3D)
						lSurf = (MEP3DSurface *) temp;
				if (lSurf) lSurf->UpdateData(mTarget);
			} else lSurf = mTarget;
			if (lSurf) {
				if (Visible) {
					lProgress->ChangeText("Calculating 3D Grid...");
					lProgress->SetBaseValue(100*i/NumFrames);
					lProgress->SetScaleFactor((float) 0.9/NumFrames);
					if (updateGrid) lSurf->CalculateMEPGrid(data, lProgress);
					lProgress->ChangeText("Contouring grid...");
					lProgress->SetBaseValue((long)(100*i/NumFrames + 90.0/NumFrames));
					lProgress->SetScaleFactor((float) 0.1/NumFrames);
					if (updateContour) lSurf->Contour3DGrid(lProgress);
					lSurf->FreeGrid();
				} else {
					if (updateContour) lSurf->FreeContour();
				}
			}
		}
		data->SetCurrentFrame(CurrentFrame);
	} else {	//simply update this surface
		if (Visible) {
			lProgress->ChangeText("Calculating 3D Grid...");
			lProgress->SetScaleFactor(0.95);
			if (updateGrid) mTarget->CalculateMEPGrid(data, lProgress);
			lProgress->ChangeText("Contouring grid...");
			lProgress->SetBaseValue(95);
			lProgress->SetScaleFactor(0.05);
			if (updateContour) mTarget->Contour3DGrid(lProgress);
		} else {
			mTarget->FreeGrid();
			if (updateContour) mTarget->FreeContour();
		}
	}
	if (lProgress) delete lProgress;

	if (mTarget->GridAvailable())
	  mFreeMemBut->Enable();
	else
	  mFreeMemBut->Disable();

	/*	AbleDItem(SurfaceDlg, kSurfBaseItems+kTE3DFreeMem, target->GridAvailable());
	//Setup the contour value and grid max text items
	short	itemtype;
	Handle	itemhandle;
	Rect	itemrect;
	GetDialogItem(SurfaceDlg, kSurfBaseItems+kTE3DValue, &itemtype, &itemhandle, &itemrect);
	float ContourValue = target->GetContourValue(), GridMax = target->GetGridMax();
	Str255	itemText;
	sprintf((char *) &(itemText[1]), "%.4f", ContourValue);
	itemText[0] = strlen((char *) &(itemText[1]));
	SetDialogItemText(itemhandle, itemText);
	GetDialogItem(SurfaceDlg, kSurfBaseItems+kTE3DMaxValue, &itemtype, &itemhandle, &itemrect);
	sprintf((char *) &(itemText[1]), "%.4f", GridMax);
	itemText[0] = strlen((char *) &(itemText[1]));
	SetDialogItemText(itemhandle, itemText);
	UpdateTest = false;
	SetupContourSlider();
	AbleDItem(SurfaceDlg, kSurfBaseItems+kTE3DExportButton, target->ExportPossible());
	*/
	setContourValueSld();
	SetContourMaxValueText();
	UpdateTest = false;
	setUpdateButton();

	if (mTarget->ExportPossible())
	  mExportBut->Enable();
	else
	  mExportBut->Disable();

	owner->SurfaceUpdated();
}

void MEP3DSurfPane::CreateControls() {
	MEP3DSurfPane * MEP3DPanel = this;	//this is here just to match the code below from DialogBlocks
	
	wxBoxSizer* itemBoxSizer168 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer168, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
    wxStaticText* itemStaticText169 = new wxStaticText( MEP3DPanel, wxID_STATIC, _("Select Orbital Set:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer168->Add(itemStaticText169, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    wxString* mOrbSetChoiceStrings = NULL;
    mOrbSetChoice = new wxChoice( MEP3DPanel, ID_ORB_CHOICE, wxDefaultPosition, wxDefaultSize, 0, mOrbSetChoiceStrings, 0 );
    if (ShowToolTips())
        mOrbSetChoice->SetToolTip(_("Choose the set of vectors to use for producing the MEP surface."));
    itemBoxSizer168->Add(mOrbSetChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer171 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer171, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
    wxStaticText* itemStaticText172 = new wxStaticText( MEP3DPanel, wxID_STATIC, _("Number of grid points:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer171->Add(itemStaticText172, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mNumGridPntSld = new wxSlider( MEP3DPanel, ID_GRID_POINT_SLIDER, 0, 10, 150, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_LABELS );
    itemBoxSizer171->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer174 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer174, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
    wxStaticText* itemStaticText175 = new wxStaticText( MEP3DPanel, wxID_STATIC, _("Grid Size:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer174->Add(itemStaticText175, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mGridSizeSld = new wxSlider( MEP3DPanel, ID_GRID_SIZE_SLIDER, 0, 0, 300, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_HORIZONTAL );
    itemBoxSizer174->Add(mGridSizeSld, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer177 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer177, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
    wxBoxSizer* itemBoxSizer178 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer177->Add(itemBoxSizer178, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    wxStaticText* itemStaticText179 = new wxStaticText( MEP3DPanel, wxID_STATIC, _("Contour Value:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer178->Add(itemStaticText179, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mContourValueEdit = new wxTextCtrl( MEP3DPanel, ID_CONTOUR_VALUE_EDIT, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer178->Add(mContourValueEdit, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer181 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer177->Add(itemBoxSizer181, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    mContourValSld = new wxSlider( MEP3DPanel, ID_CONTOUR_VALUE_SLIDER, 0, 0, 100, wxDefaultPosition, wxSize(155,wxDefaultCoord), wxSL_HORIZONTAL );
    itemBoxSizer181->Add(mContourValSld, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
	
    wxBoxSizer* itemBoxSizer183 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer181->Add(itemBoxSizer183, 1, wxGROW|wxLEFT|wxTOP|wxBOTTOM, 5);
    mGridMinText = new wxStaticText( MEP3DPanel, ID_GENSURFGRIDMINTEXT, _("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT );
    itemBoxSizer183->Add(mGridMinText, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mGridMaxText = new wxStaticText( MEP3DPanel, ID_GRID_MAX_TEXT, _("1"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
    itemBoxSizer183->Add(mGridMaxText, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    wxBoxSizer* itemBoxSizer186 = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(itemBoxSizer186, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
    wxBoxSizer* itemBoxSizer187 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer186->Add(itemBoxSizer187, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxBoxSizer* itemBoxSizer188 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer187->Add(itemBoxSizer188, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
    wxStaticText* itemStaticText189 = new wxStaticText( MEP3DPanel, wxID_STATIC, _("Surface Color:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer188->Add(itemStaticText189, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mOrbColor1 = new colorArea( MEP3DPanel, ID_3D_COLOR_POSITIVE, &PosColor );
    itemBoxSizer188->Add(mOrbColor1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mOrbColor2 = new colorArea( MEP3DPanel, ID_3D_COLOR_NEGATIVE, &NegColor );
    itemBoxSizer188->Add(mOrbColor2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer192 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer187->Add(itemBoxSizer192, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
    wxStaticText* itemStaticText193 = new wxStaticText( MEP3DPanel, wxID_STATIC, _("Transparency color:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer192->Add(itemStaticText193, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);
	
    mTransColor = new colorArea( MEP3DPanel, ID_TRANSPARENCY_COLOR, &TranspColor );
    itemBoxSizer192->Add(mTransColor, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer195 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer187->Add(itemBoxSizer195, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 1);
    wxString m3DRdoBoxStrings[] = {
        _("&Solid"),
        _("&WireFrame")
    };
    m3DRdoBox = new wxRadioBox( MEP3DPanel, ID_3D_RADIOBOX, _("Surface Display"), wxDefaultPosition, wxDefaultSize, 2, m3DRdoBoxStrings, 1, wxRA_SPECIFY_ROWS );
    m3DRdoBox->SetSelection(0);
    itemBoxSizer195->Add(m3DRdoBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    mSmoothChkBox = new wxCheckBox( MEP3DPanel, ID_SMOOTH_CHECKBOX, _("Smooth"), wxDefaultPosition, wxDefaultSize, 0 );
    mSmoothChkBox->SetValue(false);
    if (ShowToolTips())
        mSmoothChkBox->SetToolTip(_("Check to use surface normals to smooth out the surface of the contour."));
    itemBoxSizer195->Add(mSmoothChkBox, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxBoxSizer* itemBoxSizer198 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer186->Add(itemBoxSizer198, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    mSetParamBut = new wxButton( MEP3DPanel, ID_SET_PARAM_BUT, _("Parameters..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer198->Add(mSetParamBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mFreeMemBut = new wxButton( MEP3DPanel, ID_FREE_MEM_BUT, _("Free Mem"), wxDefaultPosition, wxDefaultSize, 0 );

    if (mTarget->GridAvailable())
      mFreeMemBut->Enable();
    else
      mFreeMemBut->Disable();

    if (ShowToolTips())
        mFreeMemBut->SetToolTip(_("Click to free the memory used by the 3D grid."));
    itemBoxSizer198->Add(mFreeMemBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mExportBut = new wxButton( MEP3DPanel, ID_SURFACE_EXPORT_BUT, _("Export..."), wxDefaultPosition, wxDefaultSize, 0 );

    if (mTarget->ExportPossible())
      mExportBut->Enable();
    else
      mExportBut->Disable();

    itemBoxSizer198->Add(mExportBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    mUpdateBut = new wxButton( MEP3DPanel, ID_SURFACE_UPDATE_BUT, _("Update"), wxDefaultPosition, wxDefaultSize, 0 );
    mUpdateBut->SetDefault();
    itemBoxSizer198->Add(mUpdateBut, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
}

Surface2DParamDlg::Surface2DParamDlg(BaseSurfacePane * parent, Surf2DBase * targetSurface, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
  mParent = parent;
  mTargetSurf = targetSurface;

  Create(id, caption, pos, size, style);

}

bool Surface2DParamDlg::Create(wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
  wxFrame::Create( mParent, id, caption, pos, size, style );

  createControls();

  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
  Centre();

  return true;
}

void Surface2DParamDlg::createControls()
{
  wxString tmpStr;
  CPoint3D tempPt;
  mTargetSurf->GetOrigin(&tempPt);

  mainSizer = new wxBoxSizer(wxVERTICAL);

  firstTierSizer = new wxBoxSizer(wxHORIZONTAL);
  secondTierSizer = new wxBoxSizer(wxHORIZONTAL);
  thirdTierSizer = new wxBoxSizer(wxHORIZONTAL);
  fourthTierSizer = new wxBoxSizer(wxHORIZONTAL);
  fifthTierSizer = new wxBoxSizer(wxHORIZONTAL);

  firstTierSizer->Add(new wxStaticText(this, wxID_ANY,
				       _T("Number of grid points:"),
				       wxDefaultPosition, wxDefaultSize), 
		 0, wxALIGN_LEFT | wxALL, 3);

  numGridPoint = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize);
  tmpStr.Printf(wxT("%ld"), mTargetSurf->GetNumGridPoints());
  numGridPoint->SetValue(tmpStr);
  firstTierSizer->Add(numGridPoint, 0, wxALL, 5);

  mainSizer->Add(firstTierSizer);

  mTargetSurf->GetOrigin(&tempPt);

  mainSizer->Add(new wxStaticText(this, wxID_ANY,
				  _T("Origin (x, y, z):"),
				  wxDefaultPosition, wxDefaultSize), 
		 0, wxALIGN_LEFT | wxALL, 3);

  originText1 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), tempPt.x);
  originText1->SetValue(tmpStr);

  originText2 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), tempPt.y);
  originText2->SetValue(tmpStr);

  originText3 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), tempPt.z);
  originText3->SetValue(tmpStr);

  secondTierSizer->Add(originText1, 0, wxALL, 5);
  secondTierSizer->Add(originText2, 0, wxALL, 5);
  secondTierSizer->Add(originText3, 0, wxALL, 5);

  mainSizer->Add(secondTierSizer);

  mTargetSurf->GetXIncrement(&tempPt);

  mainSizer->Add(new wxStaticText(this, wxID_ANY,
				  _T("Vector defining axis 1:"),
				  wxDefaultPosition, wxDefaultSize), 
		 0, wxALIGN_LEFT | wxALL, 3);

  vectorAxis1x = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), tempPt.x);
  vectorAxis1x->SetValue(tmpStr);

  vectorAxis1y = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), tempPt.y);
  vectorAxis1y->SetValue(tmpStr);

  vectorAxis1z = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), tempPt.z);
  vectorAxis1z->SetValue(tmpStr);

  thirdTierSizer->Add(vectorAxis1x, 0, wxALL, 5);
  thirdTierSizer->Add(vectorAxis1y, 0, wxALL, 5);
  thirdTierSizer->Add(vectorAxis1z, 0, wxALL, 5);

  mTargetSurf->GetYIncrement(&tempPt);

  mainSizer->Add(thirdTierSizer);

  mainSizer->Add(new wxStaticText(this, wxID_ANY,
				  _T("Vector defining axis 2:"),
				  wxDefaultPosition, wxDefaultSize), 
		 0, wxALIGN_LEFT | wxALL, 3);

  vectorAxis2x = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), tempPt.x);
  vectorAxis2x->SetValue(tmpStr);

  vectorAxis2y = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), tempPt.y);
  vectorAxis2y->SetValue(tmpStr);

  vectorAxis2z = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), tempPt.z);
  vectorAxis2z->SetValue(tmpStr);

  fourthTierSizer->Add(vectorAxis2x, 0, wxALL, 5);
  fourthTierSizer->Add(vectorAxis2y, 0, wxALL, 5);
  fourthTierSizer->Add(vectorAxis2z, 0, wxALL, 5);

  mainSizer->Add(fourthTierSizer);

  okButton = new wxButton( this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize);
  cancelButton = new wxButton( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize);
  copyAllButton = new wxButton( this, ID_COPY_ALL, _("&Copy All"), wxDefaultPosition, wxDefaultSize);
  pasteAllButton = new wxButton( this, ID_PASTE_ALL, _("&Paste All"), wxDefaultPosition, wxDefaultSize);

  fifthTierSizer->Add(copyAllButton , 0, wxALL, 5);
  fifthTierSizer->Add(pasteAllButton , 0, wxALL, 5);
  fifthTierSizer->Add(cancelButton , 0, wxALL, 5);
  fifthTierSizer->Add(okButton , 0, wxALL, 5);

  mainSizer->Add(fifthTierSizer);

  SetSizer(mainSizer);
}

void Surface2DParamDlg::OnClose(wxCommandEvent &event ) {
	wxString tmpStr;
	long tempLong;
	CPoint3D tempPt;
	double	tempFlt;

	tmpStr = numGridPoint->GetValue();
	if (tmpStr.ToLong(&tempLong))
		mTargetSurf->SetNumGridPoints(tempLong);

	tempPt.x = tempPt.y = tempPt.z = 0.0;
	tmpStr = originText1->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.x = tempFlt;
	tmpStr = originText2->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.y = tempFlt;
	tmpStr = originText3->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.z = tempFlt;

	mTargetSurf->SetOrigin(&tempPt);

	tempPt.x = 0.1;
	tempPt.y = tempPt.z = 0.0;
	tmpStr = vectorAxis1x->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.x = tempFlt;
	tmpStr = vectorAxis1y->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.y = tempFlt;
	tmpStr = vectorAxis1z->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.z = tempFlt;

	mTargetSurf->SetXIncrement(&tempPt);

	tempPt.x = tempPt.z = 0.0;
	tempPt.y = 0.1;
	tmpStr = vectorAxis2x->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.x = tempFlt;
	tmpStr = vectorAxis2y->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.y = tempFlt;
	tmpStr = vectorAxis2z->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.z = tempFlt;

	mTargetSurf->SetYIncrement(&tempPt);

	mTargetSurf->SetRotate2DMap(false);

	mParent->TargetToPane();
	mParent->refreshControls();
	mParent->SetUpdateTest(true);

	Destroy();
}

void Surface2DParamDlg::OnCancel(wxCommandEvent &event ) {
	Destroy();
}

/*!!! Use wxWidgets' config class to implement copyAll and pasteAll
  instead of operating on a file directly
*/

void Surface2DParamDlg::OnCopyAll(wxCommandEvent &event )
{
  wxConfigBase *pConfig = wxConfigBase::Get();

  pConfig->Write(_T("/Parameters/Surface2D/NumGridPoints"), numGridPoint->GetValue());

  pConfig->Write(_T("/Parameters/Surface2D/OriginX"), originText1->GetValue());
  pConfig->Write(_T("/Parameters/Surface2D/OriginY"), originText2->GetValue());
  pConfig->Write(_T("/Parameters/Surface2D/OriginZ"), originText3->GetValue());

  pConfig->Write(_T("/Parameters/Surface2D/vectorAxis1x"), vectorAxis1x->GetValue());
  pConfig->Write(_T("/Parameters/Surface2D/vectorAxis1y"), vectorAxis1y->GetValue());
  pConfig->Write(_T("/Parameters/Surface2D/vectorAxis1z"), vectorAxis1z->GetValue());

  pConfig->Write(_T("/Parameters/Surface2D/vectorAxis2x"), vectorAxis2x->GetValue());
  pConfig->Write(_T("/Parameters/Surface2D/vectorAxis2y"), vectorAxis2y->GetValue());
  pConfig->Write(_T("/Parameters/Surface2D/vectorAxis2z"), vectorAxis2z->GetValue());

}

void Surface2DParamDlg::OnPasteAll(wxCommandEvent &event )
{
  wxConfigBase *pConfig = wxConfigBase::Get();

  pConfig->SetPath(_T("/Parameters/Surface2D"));

  numGridPoint->SetValue(pConfig->Read(_T("NumGridPoints")));

  originText1->SetValue(pConfig->Read(_T("OriginX")));
  originText2->SetValue(pConfig->Read(_T("OriginY")));
  originText3->SetValue(pConfig->Read(_T("OriginZ")));

  vectorAxis1x->SetValue(pConfig->Read(_T("vectorAxis1x")));
  vectorAxis1y->SetValue(pConfig->Read(_T("vectorAxis1y")));
  vectorAxis1z->SetValue(pConfig->Read(_T("vectorAxis1z")));

  vectorAxis2x->SetValue(pConfig->Read(_T("vectorAxis2x")));
  vectorAxis2y->SetValue(pConfig->Read(_T("vectorAxis2y")));
  vectorAxis2z->SetValue(pConfig->Read(_T("vectorAxis2z")));
}

Surface3DParamDlg::Surface3DParamDlg(BaseSurfacePane * parent, Surf3DBase * targetSurface, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
  mParent = parent;
  mTargetSurf = targetSurface;

  Create(id, caption, pos, size, style);
 
}

bool Surface3DParamDlg::Create(wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
  wxFrame::Create( mParent, id, caption, pos, size, style );

  createControls();

  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
  Centre();

  return true;
}

void Surface3DParamDlg::createControls()
{
  wxString tmpStr;
  CPoint3D tempPt;
  float tempFlt;

  mainSizer = new wxBoxSizer(wxVERTICAL);

  mainSizer->Add(new wxStaticText(this, wxID_ANY,
				  _T("Number of grid points (x, y, z):"),
				  wxDefaultPosition, wxDefaultSize), 
		 0, wxALIGN_LEFT | wxALL, 3);

  firstTierSizer = new wxBoxSizer(wxHORIZONTAL);
  numGridPoint1 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize);
  tmpStr.Printf(wxT("%ld"), mTargetSurf->GetNumXGridPoints());
  numGridPoint1->SetValue(tmpStr);

  numGridPoint2 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize);
  tmpStr.Printf(wxT("%ld"), mTargetSurf->GetNumYGridPoints());
  numGridPoint2->SetValue(tmpStr);

  numGridPoint3 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize);
  tmpStr.Printf(wxT("%ld"), mTargetSurf->GetNumZGridPoints());
  numGridPoint3->SetValue(tmpStr);

  firstTierSizer->Add(numGridPoint1, 0, wxALL, 5);
  firstTierSizer->Add(numGridPoint2, 0, wxALL, 5);
  firstTierSizer->Add(numGridPoint3, 0, wxALL, 5);

  mainSizer->Add(firstTierSizer);

  mainSizer->Add(new wxStaticText(this, wxID_ANY,
				  _T("Origin (x, y, z):"),
				  wxDefaultPosition, wxDefaultSize), 
		 0, wxALIGN_LEFT | wxALL, 3);

  secondTierSizer = new wxBoxSizer(wxHORIZONTAL);

  mTargetSurf->GetOrigin(&tempPt);

  originText1 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), tempPt.x);
  originText1->SetValue(tmpStr);

  originText2 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), tempPt.y);
  originText2->SetValue(tmpStr);

  originText3 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), tempPt.z);
  originText3->SetValue(tmpStr);

  secondTierSizer->Add(originText1, 0, wxALL, 5);
  secondTierSizer->Add(originText2, 0, wxALL, 5);
  secondTierSizer->Add(originText3, 0, wxALL, 5);

  mainSizer->Add(secondTierSizer);

  mainSizer->Add(new wxStaticText(this, wxID_ANY,
				  _T("Grid increment (x, y, z):"),
				  wxDefaultPosition, wxDefaultSize), 
		 0, wxALIGN_LEFT | wxALL, 3);

  thirdTierSizer = new wxBoxSizer(wxHORIZONTAL);
  gridIncText1 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), mTargetSurf->GetXGridInc());
  gridIncText1->SetValue(tmpStr);

  gridIncText2 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), mTargetSurf->GetYGridInc());
  gridIncText2->SetValue(tmpStr);

  gridIncText3 = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(100, 25));
  tmpStr.Printf(wxT("%f"), mTargetSurf->GetZGridInc());
  gridIncText3->SetValue(tmpStr);

  thirdTierSizer->Add(gridIncText1, 0, wxALL, 5);
  thirdTierSizer->Add(gridIncText2, 0, wxALL, 5);
  thirdTierSizer->Add(gridIncText3, 0, wxALL, 5);

  mainSizer->Add(thirdTierSizer);

  fourthTierSizer = new wxBoxSizer(wxHORIZONTAL);
  okButton = new wxButton( this, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize);
  cancelButton = new wxButton( this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize);
  copyAllButton = new wxButton( this, ID_COPY_ALL, _("&Copy All"), wxDefaultPosition, wxDefaultSize);
  pasteAllButton = new wxButton( this, ID_PASTE_ALL, _("&Paste All"), wxDefaultPosition, wxDefaultSize);

  fourthTierSizer->Add(copyAllButton , 0, wxALL, 5);
  fourthTierSizer->Add(pasteAllButton , 0, wxALL, 5);
  fourthTierSizer->Add(cancelButton , 0, wxALL, 5);
  fourthTierSizer->Add(okButton , 0, wxALL, 5);

  mainSizer->Add(fourthTierSizer);

  SetSizer(mainSizer);
}

void Surface3DParamDlg::OnClose(wxCommandEvent &event ) {
	wxString tmpStr;
	long tempLong;
	CPoint3D tempPt;
	double	tempFlt;

	tmpStr = numGridPoint1->GetValue();
	if (tmpStr.ToLong(&tempLong))
		mTargetSurf->SetNumXGridPoints(tempLong);

	tmpStr = numGridPoint2->GetValue();
	if (tmpStr.ToLong(&tempLong))
		mTargetSurf->SetNumYGridPoints(tempLong);
				
	tmpStr = numGridPoint3->GetValue();
	if (tmpStr.ToLong(&tempLong))
		mTargetSurf->SetNumZGridPoints(tempLong);

	tempPt.x = tempPt.y = tempPt.z = 0.0;
	tmpStr = originText1->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.x = tempFlt;
	tmpStr = originText2->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.y = tempFlt;
	tmpStr = originText3->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		tempPt.z = tempFlt;

	mTargetSurf->SetOrigin(&tempPt);

	tmpStr = gridIncText1->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		mTargetSurf->SetXGridInc(tempFlt);

	tmpStr = gridIncText2->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		mTargetSurf->SetYGridInc(tempFlt);			
				   
	tmpStr = gridIncText3->GetValue();
	if (tmpStr.ToDouble(&tempFlt))
		mTargetSurf->SetZGridInc(tempFlt);

	mTargetSurf->SetFixGrid(true);

	mParent->TargetToPane();
	mParent->refreshControls();
	mParent->SetUpdateTest(true);

	Destroy();
}

void Surface3DParamDlg::OnCancel(wxCommandEvent &event )
{
  Destroy();
}

/*!!! Use wxWidgets' config class to implement copyAll and pasteAll
  instead of operating on a file directly
*/

void Surface3DParamDlg::OnCopyAll(wxCommandEvent &event )
{
  wxConfigBase *pConfig = wxConfigBase::Get();

  pConfig->Write(_T("/Parameters/Surface3D/NumGridPointsX"), numGridPoint1->GetValue());
  pConfig->Write(_T("/Parameters/Surface3D/NumGridPointsY"), numGridPoint2->GetValue());
  pConfig->Write(_T("/Parameters/Surface3D/NumGridPointsZ"), numGridPoint3->GetValue());

  pConfig->Write(_T("/Parameters/Surface3D/OriginX"), originText1->GetValue());
  pConfig->Write(_T("/Parameters/Surface3D/OriginY"), originText2->GetValue());
  pConfig->Write(_T("/Parameters/Surface3D/OriginZ"), originText3->GetValue());

  pConfig->Write(_T("/Parameters/Surface3D/XInc"), gridIncText1->GetValue());
  pConfig->Write(_T("/Parameters/Surface3D/YInc"), gridIncText2->GetValue());
  pConfig->Write(_T("/Parameters/Surface3D/ZInc"), gridIncText3->GetValue());

}

void Surface3DParamDlg::OnPasteAll(wxCommandEvent &event )
{
  wxConfigBase *pConfig = wxConfigBase::Get();

  pConfig->SetPath(_T("/Parameters/Surface3D"));

  numGridPoint1->SetValue(pConfig->Read(_T("NumGridPointsX")));
  numGridPoint2->SetValue(pConfig->Read(_T("NumGridPointsY")));
  numGridPoint3->SetValue(pConfig->Read(_T("NumGridPointsZ")));

  originText1->SetValue(pConfig->Read(_T("OriginX")));
  originText2->SetValue(pConfig->Read(_T("OriginY")));
  originText3->SetValue(pConfig->Read(_T("OriginZ")));

  gridIncText1->SetValue(pConfig->Read(_T("XInc")));
  gridIncText2->SetValue(pConfig->Read(_T("YInc")));
  gridIncText3->SetValue(pConfig->Read(_T("ZInc")));
}
