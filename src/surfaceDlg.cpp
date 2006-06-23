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

#include "surfaceDlg.h"
#include "colorArea.h"
#include "SurfaceTypes.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "BasisSet.h"
#include "Prefs.h"

////@begin XPM images
////@end XPM images

/*!
 * Orbital3D event table definition
 */

/*BEGIN_EVENT_TABLE( Orbital3DSurf, wxPanel )
    EVT_CHOICE( ID_CHOICE1, Orbital3DSurf::OnChoice1Selected )
    EVT_CHECKBOX( ID_CHECKBOX5, Orbital3DSurf::OnCheckbox5Click )
END_EVENT_TABLE()
*/

using namespace std;

IMPLEMENT_CLASS( BaseSurfacePane, wxPanel )
IMPLEMENT_CLASS( Surface3DPane, wxPanel )
IMPLEMENT_CLASS( Orbital3DSurfPane, wxPanel )


BEGIN_EVENT_TABLE( Orbital3DSurfPane, wxPanel )
  EVT_CHOICE  (ID_ORB_FORMAT_CHOICE,  Orbital3DSurfPane::OnOrbFormatChoice)
  EVT_CHOICE  (ID_ORB_CHOICE, Orbital3DSurfPane::OnOrbSetChoice)
  EVT_LISTBOX (ID_ATOM_LIST, Orbital3DSurfPane::OnAtomList)
  EVT_CHECKBOX (ID_SPH_HARMONICS_CHECKBOX, Orbital3DSurfPane::OnSphHarmonicChk)
  EVT_RADIOBOX (ID_3D_RADIOBOX, Surface3DPane::On3DRadioBox)
  EVT_CHECKBOX (ID_SMOOTH_CHECKBOX, Surface3DPane::OnSmoothCheck)
END_EVENT_TABLE()

/*!
 * Base class of any Panel
 */

BaseSurfacePane::BaseSurfacePane( wxWindow* parent, Surface* target, MoleculeData* data, wxWindowID id, const wxPoint& pos, const wxSize& size, long style )
{
  mTarget = target;
  //mData = data;

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

void BaseSurfacePane::CreateControls()
{
  mainSizer = new wxBoxSizer(wxVERTICAL);
  upperSizer = new wxBoxSizer(wxHORIZONTAL);
  middleSizer = new wxBoxSizer(wxHORIZONTAL);
  bottomSizer = new wxBoxSizer(wxHORIZONTAL);
  leftMiddleSizer = new wxBoxSizer(wxVERTICAL);
  upperLeftMiddleSizer = new wxBoxSizer(wxHORIZONTAL);
  lowerLeftMiddleSizer = new wxBoxSizer(wxHORIZONTAL);
  rightMiddleSizer = new wxFlexGridSizer(2,2,0,0);
  leftBottomSizer = new wxBoxSizer(wxHORIZONTAL);
  rightBottomSizer = new wxBoxSizer(wxVERTICAL);

  label0 = new wxStaticText( this, wxID_ANY,
                            _T("Select Orbital Set:"),
                            wxDefaultPosition,
                            wxDefaultSize);

  label1 = new wxStaticText( this, wxID_ANY,
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
  mUpdateBut = new wxButton( this, ID_SURFACE_UPDATE_BUT, wxT("Update"), wxPoint(450, 160) );

  SetSizer(mainSizer);
}

OrbSurfacePane::OrbSurfacePane( OrbSurfBase* target, MoleculeData* data, WinPrefs* prefs )
{
  mTarget = target;
  mData = data;
  mPrefs = prefs;

  TargetSet = mTarget->GetTargetSet();
  OrbOptions = mTarget->GetOptions();
  OrbColumnEnergyOrOccupation = mTarget->GetOrbOccDisplay();
  PlotOrb = target->GetTargetOrb();
  SphericalHarmonics = target->UseSphericalHarmonics();

  if ((TargetSet<0)||(!(OrbOptions&1))) 
    {	//default to something sensible
      Frame * lFrame = mData->GetCurrentFramePtr();
      const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();

      if (Orbs->size() > 0) 
	{
	  TargetSet = 0;	//just default to the first set...
	} 
      else
	OrbOptions = 1;	//No MO's so default to AO's
    }

}

OrbSurfacePane::~OrbSurfacePane()
{

}

int OrbSurfacePane::getOrbSetForOrbPane(vector<wxString>& choice)
{
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
  Frame * lFrame = mData->GetCurrentFramePtr();
  const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();

  wxString tmpStr;

  if (lFrame && (Orbs->size() > 0)) 
    {
      OrbitalRec * lMOs = NULL;

      if ((TargetSet < Orbs->size())&&(TargetSet >= 0)) 
	lMOs = (*Orbs)[TargetSet];

      if (lMOs) 
	{
	  Boolean Alpha = !(OrbOptions & 16);
	  long	NumMOs;
	  char * SymLabel;
	  float * Energy;
	  float * OccNum;

	  if (Alpha) 
	    {
	      NumMOs=lMOs->NumAlphaOrbs;
	      SymLabel = lMOs->SymType;
	      Energy = lMOs->Energy;
	      OccNum = lMOs->OrbOccupation;
	    } 
	  else 
	    {
	      NumMOs=lMOs->NumBetaOrbs;
	      SymLabel = lMOs->SymTypeB;
	      Energy = lMOs->EnergyB;
	      OccNum = lMOs->OrbOccupationB;
	    }

	  char* oneSymLabel;

	  for (int theCell = 0; theCell < NumMOs; theCell++) 
	    {
	      tmpStr.Printf("%d  ", theCell+1); 
	      char text[30];
	      int nchar;

	      if (SymLabel) 
	      {	//Add the symetry of the orb, if known
		
		oneSymLabel = &(SymLabel[theCell*5]);	
		//offset to the label for this orb

		for (int ichar=0; ichar<4; ichar++) 
		  {
		    if ((oneSymLabel[ichar])=='\0') break;

		    if ((oneSymLabel[ichar]>='0')&&(oneSymLabel[ichar]<='9')) 
		      {
			tmpStr.Append(oneSymLabel[ichar]);
		      } 
		    else if ((oneSymLabel[ichar]=='U')||(oneSymLabel[ichar]=='u')) 
		      {
			tmpStr.Append('u');
		      } 
		    else if ((oneSymLabel[ichar]=='G')||(oneSymLabel[ichar]=='g')) 
		      {
			tmpStr.Append('g');
		      } 
		    else
		      tmpStr.Append(oneSymLabel[ichar]);
		  }
		//TextSize(12);	//reset the point size
	      }

	      tmpStr.Append('\t');

	      nchar=0;
	      if (OrbColumnEnergyOrOccupation) 
		{	//orb occupation selected
		
		  if (OccNum) 
		    sprintf(text, "%.3f%n", OccNum[theCell], &nchar);
		  else 
		    {	
		    //attempt to set the occupation based on the wavefunction type
		    if (lMOs->getOrbitalWavefunctionType() == RHF) 
		      {
			if (theCell<lMOs->getNumOccupiedAlphaOrbitals()) 
			  strcpy(text, "2  ");
			else 
			  strcpy(text, "0  ");
			nchar = 3;
		      } 
		    else if (lMOs->getOrbitalWavefunctionType() == ROHF) 
		      {
			strcpy(text, "0  ");
			if (theCell<lMOs->getNumOccupiedBetaOrbitals()) 
			  strcpy(text, "2  ");
			else if (theCell<lMOs->getNumOccupiedAlphaOrbitals()) 
			  strcpy(text, "1  ");
			nchar = 3;
		      } 
		    else if (lMOs->getOrbitalWavefunctionType() == UHF) 
		      {
			strcpy(text, "0  ");
			if (Alpha) 
			  {
			    if (theCell<lMOs->getNumOccupiedAlphaOrbitals()) 
			      strcpy(text,"1  ");
			  }
			else if (theCell<lMOs->getNumOccupiedBetaOrbitals()) 
			  strcpy(text, "1  ");
			nchar = 3;
		      } 
		    else 
		      {	//MCSCF or CI occupations can't be guessed
			strcpy(text, "??  ");
			nchar = 4;
		      }
		  }
		} 
	      else if (Energy) 
		{	//punch out the orb energy
		  sprintf(text, "%.3f%n", Energy[theCell], &nchar);
		}

	      if (nchar>0) 
		{
		  tmpStr.Append(text);
		  choice.push_back(tmpStr);
		}
	    }
	}
    }
}

void OrbSurfacePane::makeAOList(wxString& choice)
{
  BasisSet * BasisPtr = mData->GetBasisSet();

  if (BasisPtr) 
    {
      long NumBasisFuncs = BasisPtr->GetNumBasisFuncs(SphericalHarmonics);

      for (int theCell = 0; theCell < NumBasisFuncs; theCell++) 
	{
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
		      int nchar;
		      //punch out the atom # and symbol if this is the 1st function for this atom

		      if ((ishell==minshell)&&(theCell==ifunc))
			{
			  sprintf(label, "%ld  ", iatom+1);
			  choice.Append(label);

			  if (mPrefs) 
			    {
			      long AtomType = lFrame->GetAtomType(iatom)-1;
			      mPrefs->GetAtomLabel(AtomType, tmpStr);
			      choice.Append(tmpStr);
			    }
			}
		      choice.Append('\t');

		      jfunc = theCell-ifunc;
		      BasisPtr->Shells[ishell].GetLabel(label, jfunc, SphericalHarmonics);
		      nchar = strlen(label);
		      
		      if (nchar>0)
			{	//Make sure there really is something there
			  choice.Append(' ');
			  choice.Append(' ');
			  choice.Append(label);

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
		      choice.Append('\t');

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
			      char label[63];

			      sprintf(label, "%.3f", aVector[theCell]);//prepare the coef for printing
			      choice.Append(label);
			    }
			}
		    }
		}
	    }
	  choice.Append('\n');
	}
    }
}

/*
 * 3D surface dialog class
 */

Surface3DPane::Surface3DPane( wxWindow* parent, Surf3DBase* target, MoleculeData* data, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : BaseSurfacePane(parent, target, data, id, pos, size, style)
{
  mTarget = target;
  CreateControls();
  UseSolidSurface = mTarget->SolidSurface();
  UseNormals = mTarget->UseSurfaceNormals();
}

Surface3DPane::~Surface3DPane()
{
  delete mTransColor;
}

void Surface3DPane::CreateControls()
{
  float GridMax = mTarget->GetGridMax();

  label2 = new wxStaticText( this, wxID_ANY,
                            _T("Grid Size:"),
                            wxDefaultPosition,
                            wxDefaultSize);
  label3 = new wxStaticText( this, wxID_ANY,
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
				 wxSize(155,wxDefaultCoord),
				 wxSL_AUTOTICKS | wxSL_LABELS);

  label4 = new wxStaticText( this, wxID_ANY,
                            _T("Transparency Color:"),
                            wxDefaultPosition,
                            wxDefaultSize);

  RGBColor transpColor;
  mTarget->GetTranspColor(&transpColor);

  mTransColor = new colorArea(this, ID_TRANSPARENCY_COLOR);
  mTransColor->draw(&transpColor);

  wxString choices[] = {_T("Solid"), _T("Wire Frame")};
  m3DRdoBox = new wxRadioBox( this, ID_3D_RADIOBOX, _T(""), wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices), choices, 1, wxRA_SPECIFY_ROWS );
  m3DRdoBox->SetSelection(1-UseSolidSurface);
  
  mSmoothChkBox = new wxCheckBox( this, ID_SMOOTH_CHECKBOX, _T("Smooth"), wxPoint(340,130), wxDefaultSize );
  mSmoothChkBox->SetValue(UseNormals);

  if (UseSolidSurface)
    mSmoothChkBox->Enable();
  else
    mSmoothChkBox->Disable();

  mFreeMemBut = new wxButton( this, ID_FREE_MEM_BUT, wxT("Free Mem"), wxPoint(450, 160) );

}

void Surface3DPane::On3DRadioBox (wxCommandEvent& event )
{
  int isSolid = m3DRdoBox->GetSelection();

  if (!isSolid)
    mSmoothChkBox->Enable();
  else
    mSmoothChkBox->Disable();

}

void Surface3DPane::OnSmoothCheck (wxCommandEvent& event )
{
  UseNormals = mSmoothChkBox->GetValue();
  mTarget->UseSurfaceNormals(UseNormals);
}

/*!
 * Orbital3D constructors
 */

Orbital3DSurfPane::Orbital3DSurfPane( wxWindow* parent, Orb3DSurface* target, MoleculeData* data, WinPrefs* prefs, wxWindowID id, const wxPoint& pos, const wxSize& size, long style ) : Surface3DPane(parent, target, data, id, pos, size, style), OrbSurfacePane(target, data, prefs)
{
  mTarget = target;
  CreateControls();
}

Orbital3DSurfPane::~Orbital3DSurfPane()
{
  delete mOrbColor1;
  delete mOrbColor2;
}

/*!
 * Control creation for Orbital3D
 */

void Orbital3DSurfPane::CreateControls()
{    
  float GridMax = mTarget->GetGridMax();

    upperSizer->Add(label0, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    vector<wxString> choices;
    int itemSelect = getOrbSetForOrbPane(choices) - 1;

    mOrbSetChoice = new wxChoice( this, ID_ORB_CHOICE, wxPoint(10,10), wxSize(200,wxDefaultCoord), choices.size(), &choices.front() );
    mOrbSetChoice->SetSelection(itemSelect);
    upperSizer->Add(mOrbSetChoice, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    upperSizer->Add(10,10);
    mRevPhaseChk = new wxCheckBox(this, ID_REVERSE_PHASE_CHECKBOX, _T("Reverse Phase"), wxDefaultPosition);
    mRevPhaseChk->SetValue(mTarget->GetPhaseChange());

    upperSizer->Add(mRevPhaseChk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    mSphHarmonicsChk = new wxCheckBox( this, ID_SPH_HARMONICS_CHECKBOX, _T("Spherical Harmonics"), wxDefaultPosition, wxDefaultSize );
    mSphHarmonicsChk->SetValue(SphericalHarmonics);
    upperLeftMiddleSizer->Add(mSphHarmonicsChk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    if (OrbOptions & 1 && PlotOrb >= 0)
      upperLeftMiddleSizer->Show(mSphHarmonicsChk, true);
    else
    upperLeftMiddleSizer->Show(mSphHarmonicsChk, false);

    upperLeftMiddleSizer->Layout();

    lowerLeftMiddleSizer->Add(label1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    lowerLeftMiddleSizer->Add(mNumGridPntSld, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    mNumGridPntSld->SetValue(mTarget->GetNumGridPoints());

    leftMiddleSizer->Add(upperLeftMiddleSizer, 0, wxALL, 3);
    leftMiddleSizer->Add(lowerLeftMiddleSizer, 0, wxALL, 3);

    rightMiddleSizer->Add(label2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    rightMiddleSizer->Add(mGridSizeSld, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    rightMiddleSizer->Add(label3, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
    rightMiddleSizer->Add(mContourValSld, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    mSubLeftBot1Sizer = new wxBoxSizer(wxVERTICAL);
    mSubLeftBot2Sizer = new wxBoxSizer(wxVERTICAL);
  
    mSubLeftBot1Sizer->Add(new wxStaticText(this, wxID_ANY,
                            _T("Select Orb:"),
                            wxDefaultPosition,
			    wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    wxString choices1[] = {_T("Energy"), _T("Occupation #")};
    mOrbFormatChoice = new wxChoice( this, ID_ORB_FORMAT_CHOICE, wxDefaultPosition, wxSize(120,wxDefaultCoord), 2, choices1 );
    mSubLeftBot1Sizer->Add(mOrbFormatChoice, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    vector<wxString> choices2;
    makeMOList(choices2);

    wxString choices3;
    makeAOList(choices3);

    mAtomList = new wxListBox( this, ID_ATOM_LIST,
                               wxDefaultPosition, wxSize(130,180),
                               choices2.size(), &choices2.front(), 
			       wxLB_SINGLE |wxLB_ALWAYS_SB );

    mSubLeftBot1Sizer->Add(mAtomList, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    mSubLeftBot2Sizer->Add(new wxStaticText(this, wxID_ANY,
                            _T("Orbital vector: \nAtom Orbital Coef"),
                            wxDefaultPosition,
                            wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);

    mOrbCoef = new wxTextCtrl( this, wxID_ANY, choices3, wxPoint(20,160), wxSize(120,200), wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);
    mSubLeftBot2Sizer->Add(mOrbCoef, 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);

    leftBottomSizer->Add(mSubLeftBot1Sizer, 0, wxALL, 5);
    leftBottomSizer->Add(mSubLeftBot2Sizer, 0, wxALL, 5);

    mSubRightBot1Sizer = new wxBoxSizer(wxHORIZONTAL);
    mSubRightBot1Sizer->Add(new wxStaticText(this, wxID_ANY,
                            _T("Orbital Colors:"),
                            wxDefaultPosition,
                            wxDefaultSize), 0, wxALIGN_CENTER_VERTICAL | wxALL, 3);


    RGBColor posColor, negColor;
    mTarget->GetPosColor(&posColor);
    mTarget->GetNegColor(&negColor);

    mOrbColor1 = new colorArea(this, ID_ORB_COLOR_POSITIVE);
    mOrbColor1->draw(&posColor);
    mOrbColor2 = new colorArea(this, ID_ORB_COLOR_NEGATIVE);
    mOrbColor2->draw(&negColor);
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

    rightBottomSizer->Add(mSubRightBot1Sizer);
    rightBottomSizer->Add(mSubRightBot2Sizer);
    rightBottomSizer->Add(mSubRightBot3Sizer);
    rightBottomSizer->Add(mSubRightBot4Sizer);
    rightBottomSizer->Add(mSubRightBot5Sizer);

    middleSizer->Add(leftMiddleSizer, 0, wxALL, 10);
    middleSizer->Add(rightMiddleSizer, 0, wxALL, 10);
    bottomSizer->Add(leftBottomSizer, 0, wxALL, 3);
    bottomSizer->Add(rightBottomSizer, 0, wxALL, 3);
    mainSizer->Add(upperSizer);
    mainSizer->Add(middleSizer);
    mainSizer->Add(bottomSizer);
}

void Orbital3DSurfPane::OnOrbFormatChoice( wxCommandEvent &event )
{
  int itemtype = mOrbFormatChoice->GetSelection();

  if (OrbColumnEnergyOrOccupation) OrbColumnEnergyOrOccupation = false;
  if (itemtype) OrbColumnEnergyOrOccupation = true;

  vector<wxString> newChoice;
  makeMOList(newChoice);

  mAtomList->Set(newChoice.size(), &newChoice.front());

}

void Orbital3DSurfPane::OnOrbSetChoice( wxCommandEvent &event )
{
  int item = mOrbSetChoice->GetSelection()+1;
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
			
  if (itemtype) 
    {	//TargetSet has been choosen
      PlotOrb = -1;

      //BasisSet * lBasis = MainData->GetBasisSet();
      //LAddRow(lBasis->GetNumBasisFuncs(SphericalHarmonics), 0, AOList);

      if (!(OrbOptions&1)) 
	{	//add some MO rows!
	  //don't know what's this adding rows for  -Song

	  upperLeftMiddleSizer->Show(mSphHarmonicsChk, false);	  
	  SphericalHarmonics = false;
	} 
      else 
	{	//Must be looking for AOs
	  upperLeftMiddleSizer->Show(mSphHarmonicsChk, true);
	  mSphHarmonicsChk->SetValue(SphericalHarmonics);
	}

      upperLeftMiddleSizer->Layout();
    }
}

void Orbital3DSurfPane::OnAtomList( wxCommandEvent &event )
{
  PlotOrb = event.GetSelection();
  
  wxString choice;
  makeAOList(choice);
  mOrbCoef->SetValue(choice);
}

void Orbital3DSurfPane::OnSphHarmonicChk(wxCommandEvent &event )
{
  SphericalHarmonics = mSphHarmonicsChk->GetValue();
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


