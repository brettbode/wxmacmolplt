/////////////////////////////////////////////////////////////////////////////
// Name:        inputbuilder.cpp
// Purpose:     
// Author:      Brett Bode
// Modified by: 
// Created:     Mon 12 Jun 2006 09:37:31 AM CDT
// RCS-ID:      
// Copyright:   (c) 2006 Iowa State University
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "inputbuilder.h"
#endif
#include "Globals.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include <wx/listctrl.h>
#include <wx/listbook.h>
#include <wx/filename.h>

////@begin includes
#include "wx/imaglist.h"
////@end includes

#include "MolDisplayWin.h"
#include "AtomTypeList.h"
#include "inputbuilder.h"
#include "Frame.h"
#include "fmofragmentationdialog.h"
#include <sstream>

////@begin XPM images
////@end XPM images

/*!
 * InputBuilderWindow type definition
 */

IMPLEMENT_CLASS( InputBuilderWindow, wxFrame )

/*!
 * InputBuilderWindow event table definition
 */

BEGIN_EVENT_TABLE( InputBuilderWindow, wxFrame )

////@begin InputBuilderWindow event table entries
	EVT_CLOSE( InputBuilderWindow::OnCloseWindow )
	EVT_LISTBOOK_PAGE_CHANGED( ID_IBNOTEBOOK, InputBuilderWindow::OnIblistbookPageChanged )
	EVT_CHOICE( ID_BASIS_CHOICE, InputBuilderWindow::OnBasisChoiceSelected )
	EVT_CHOICE( ID_ECPT_CHOICE, InputBuilderWindow::OnEcptChoiceSelected )
	EVT_CHOICE( ID_NUMD_CHOICE, InputBuilderWindow::OnNumdChoiceSelected )
	EVT_CHOICE( ID_NUMF_CHOICE, InputBuilderWindow::OnNumfChoiceSelected )
	EVT_CHOICE( ID_NUMP_CHOICE, InputBuilderWindow::OnNumpChoiceSelected )
	EVT_CHOICE( ID_POLAR_CHOICE, InputBuilderWindow::OnPolarChoiceSelected )
	EVT_CHECKBOX( ID_DIFFL_CHECKBOX, InputBuilderWindow::OnDifflCheckboxClick )
	EVT_CHECKBOX( ID_DIFFS_CHECKBOX, InputBuilderWindow::OnDiffsCheckboxClick )
	EVT_CHECKBOX( ID_SPHERE_CHECK, InputBuilderWindow::OnSphereCheckClick )
	EVT_CHOICE( ID_RUN_CHOICE, InputBuilderWindow::OnRunChoiceSelected )
	EVT_CHOICE( ID_SCF_CHOICE, InputBuilderWindow::OnScfChoiceSelected )
	EVT_CHOICE( ID_LOCAL_CHOICE, InputBuilderWindow::OnLocalChoiceSelected )
	EVT_TEXT( ID_MCHARGE_TEXTCTRL, InputBuilderWindow::OnMchargeTextctrlUpdated )
	EVT_TEXT( ID_MULT_TEXTCTRL, InputBuilderWindow::OnMultTextctrlUpdated )
	EVT_CHOICE( ID_EXE_CHOICE, InputBuilderWindow::OnExeChoiceSelected )
	EVT_TEXT( ID_MAXIT_TEXTCTRL, InputBuilderWindow::OnMaxitTextctrlUpdated )
	EVT_CHECKBOX( ID_MP2_CHECKBOX, InputBuilderWindow::OnMp2CheckboxClick )
	EVT_CHECKBOX( ID_DFT_CHECKBOX, InputBuilderWindow::OnDftCheckboxClick )
	EVT_CHOICE( ID_CI_CHOICE, InputBuilderWindow::OnCiChoiceSelected )
	EVT_CHOICE( ID_CC_CHOICE, InputBuilderWindow::OnCcChoiceSelected )
	EVT_TEXT( ID_TITLE_TEXTCTRL, InputBuilderWindow::OnTitleTextctrlUpdated )
	EVT_CHOICE( ID_COORD_CHOICE, InputBuilderWindow::OnCoordChoiceSelected )
	EVT_CHOICE( ID_UNIT_CHOICE, InputBuilderWindow::OnUnitChoiceSelected )
	EVT_TEXT( ID_ZMAT_VARS_TEXT, InputBuilderWindow::OnZmatVarsTextUpdated )
	EVT_CHOICE( ID_POINTGROUP_CHOICE, InputBuilderWindow::OnPointgroupChoiceSelected )
	EVT_CHOICE( ID_ORDER_CHOICE, InputBuilderWindow::OnOrderChoiceSelected )
	EVT_CHECKBOX( ID_SYMMETRY_CHECKBOX, InputBuilderWindow::OnSymmetryCheckboxClick )
	EVT_TEXT( ID_TIMELIMIT_TEXT, InputBuilderWindow::OnTimelimitTextUpdated )
	EVT_CHOICE( ID_TIMELIMITUNITS_CHOICE, InputBuilderWindow::OnTimelimitunitsChoiceSelected )
	EVT_TEXT( ID_MEMORY_TEXT, InputBuilderWindow::OnMemoryTextUpdated )
	EVT_CHOICE( ID_MEMORYUNITS_CHOICE, InputBuilderWindow::OnMemoryunitsChoiceSelected )
	EVT_TEXT( ID_MEMDDI_EDIT, InputBuilderWindow::OnMemddiEditUpdated )
	EVT_CHOICE( ID_MEMDDICHOICE, InputBuilderWindow::OnMemddichoiceSelected )
	EVT_CHOICE( ID_DIAGONALIZATION_CHOICE, InputBuilderWindow::OnDiagonalizationChoiceSelected )
	EVT_CHECKBOX( ID_CORE_CHECKBOX, InputBuilderWindow::OnCoreCheckboxClick )
	EVT_RADIOBOX( ID_PLBALANCE_RADIOBOX, InputBuilderWindow::OnPlbalanceRadioboxSelected )
	EVT_CHECKBOX( ID_EXTERNDATAREP_CHECKBOX, InputBuilderWindow::OnExterndatarepCheckboxClick )
	EVT_CHECKBOX( ID_PARALL_CHECK, InputBuilderWindow::OnParallCheckClick )
	EVT_TEXT( ID_GVB_NCOEDIT, InputBuilderWindow::OnGVBNCOEditTextUpdated )
	EVT_TEXT( ID_GVB_PAIRSEDIT, InputBuilderWindow::OnGVBPairsEditTextUpdated )
	EVT_TEXT( ID_GVB_NSETOEDIT, InputBuilderWindow::OnGVBNSetOeditTextUpdated )
	EVT_TEXT( ID_GVB_NOEDIT, InputBuilderWindow::OnGVBNOeditTextUpdated )
	EVT_CHOICE( ID_DFTMETHOD_CHOICE, InputBuilderWindow::OnDftmethodChoiceSelected )
	EVT_CHOICE( ID_GRID_FUNCTIONAL_CHOICE, InputBuilderWindow::OnGridFunctionalChoiceSelected )
	EVT_CHOICE( ID_GRIDFREE_FUNCTIONAL_CHOICE, InputBuilderWindow::OnGridfreeFunctionalChoiceSelected )
	EVT_CHECKBOX( ID_FMOACTIVECHECK, InputBuilderWindow::OnFMOActiveCheckClick )
	EVT_TEXT( ID_FMOFRAGCOUNT, InputBuilderWindow::OnFMOFragCountTextUpdated )
#if wxCHECK_VERSION(3, 0, 0)
	EVT_GRID_CELL_CHANGED( InputBuilderWindow::OnFMOFragIdCellChange )
#else
	EVT_GRID_CELL_CHANGE( InputBuilderWindow::OnFMOFragIdCellChange )
#endif
	EVT_BUTTON( ID_FMOFRAGBUTTON, InputBuilderWindow::OnFMOFragButtonClick )
	EVT_CHOICE( ID_INITGUESS_CHOICE, InputBuilderWindow::OnInitguessChoiceSelected )
	EVT_CHOICE( ID_MOGUESS_VECSOURCE_CHOICE, InputBuilderWindow::OnMoguessVecsourceChoiceSelected )
	EVT_TEXT( ID_MOGUESS_VECORBCOUNT_TEXT, InputBuilderWindow::OnMoguessVecorbcountTextUpdated )
	EVT_CHECKBOX( ID_PRINTINITGUESS_CHECKBOX, InputBuilderWindow::OnPrintinitguessCheckboxClick )
	EVT_CHECKBOX( ID_ROTORBITALS_CHECKBOX, InputBuilderWindow::OnRotorbitalsCheckboxClick )
	EVT_RADIOBOX( ID_HESSMETHOD_RADIO, InputBuilderWindow::OnHessmethodRadioSelected )
	EVT_TEXT( ID_FORCEDISPLACE_EDIT, InputBuilderWindow::OnForcedisplaceEditUpdated )
	EVT_CHECKBOX( ID_HESSDOUBLEDIFFCHECK, InputBuilderWindow::OnHessdoublediffcheckClick )
	EVT_CHECKBOX( ID_HESSPURIFYCHECK, InputBuilderWindow::OnHesspurifycheckClick )
	EVT_CHECKBOX( ID_HESSPRTINTFC_CHECK, InputBuilderWindow::OnHessprtintfcCheckClick )
	EVT_CHECKBOX( ID_HESSVIBANL_CHECK, InputBuilderWindow::OnHessvibanlCheckClick )
	EVT_TEXT( ID_HESSFREQSCALE_EDIT, InputBuilderWindow::OnHessfreqscaleEditUpdated )
	EVT_CHECKBOX( ID_MOLPLT_CHECK, InputBuilderWindow::OnMolpltCheckClick )
	EVT_CHECKBOX( ID_PLTORB_CHECK, InputBuilderWindow::OnPltorbCheckClick )
	EVT_CHECKBOX( ID_AIMPAC_CHECK, InputBuilderWindow::OnAimpacCheckClick )
	EVT_CHECKBOX( ID_RPAC_CHECK, InputBuilderWindow::OnRpacCheckClick )
	EVT_RADIOBOX( ID_MISC_PROG_RADIO, InputBuilderWindow::OnMiscProgRadioSelected )
	EVT_TEXT( ID_MP2_CORE_EDIT, InputBuilderWindow::OnMp2CoreEditUpdated )
	EVT_TEXT( ID_MP2MEM_EDIT, InputBuilderWindow::OnMp2memEditUpdated )
	EVT_TEXT( ID_MP2INTCUTOFF_EDIT, InputBuilderWindow::OnMp2intcutoffEditUpdated )
	EVT_CHECKBOX( ID_LMOMP2_CHECK, InputBuilderWindow::OnLmomp2CheckClick )
	EVT_CHECKBOX( ID_MP2PROP_CHECK, InputBuilderWindow::OnMp2propCheckClick )
	EVT_RADIOBOX( ID_MP2TRANS_RADIO, InputBuilderWindow::OnMp2transRadioSelected )
	EVT_RADIOBOX( ID_MP2AOSTORAGE_RADIO, InputBuilderWindow::OnMp2aostorageRadioSelected )
	EVT_CHECKBOX( ID_DIRECTSCF_CHECK, InputBuilderWindow::OnDirectscfCheckClick )
	EVT_CHECKBOX( ID_FDIFF_CHECK, InputBuilderWindow::OnFdiffCheckClick )
	EVT_CHECKBOX( ID_UHF_NO_CHECK, InputBuilderWindow::OnUhfNoCheckClick )
	EVT_SPINCTRL( ID_SCF_CONV_SPIN, InputBuilderWindow::OnScfConvSpinUpdated )
	EVT_TEXT( ID_MAXSTEPCOUNT_EDIT, InputBuilderWindow::OnMaxstepcountEditUpdated )
	EVT_TEXT( ID_GRADCONVCRITERIA_EDIT, InputBuilderWindow::OnGradconvcriteriaEditUpdated )
	EVT_CHOICE( ID_OPTMETHOD_CHOICE, InputBuilderWindow::OnOptmethodChoiceSelected )
	EVT_TEXT( ID_INITSTEPSIZE_EDIT, InputBuilderWindow::OnInitstepsizeEditUpdated )
	EVT_TEXT( ID_MINSTEPSIZE_EDIT, InputBuilderWindow::OnMinstepsizeEditUpdated )
	EVT_TEXT( ID_MAXSTEPSIZE_CHECK, InputBuilderWindow::OnMaxstepsizeEditUpdated )
	EVT_CHECKBOX( ID_UPDATESTEPSIZE_CHECK, InputBuilderWindow::OnUpdatestepsizeCheckClick )
	EVT_CHECKBOX( ID_STATPT_CHECK, InputBuilderWindow::OnStatptCheckClick )
	EVT_TEXT( ID_STATPT_JUMP_EDIT, InputBuilderWindow::OnStatptJumpEditUpdated )
	EVT_RADIOBOX( ID_INIT_HESS_RADIO, InputBuilderWindow::OnInitHessRadioSelected )
	EVT_TEXT( ID_HESS_RECALC_EDIT, InputBuilderWindow::OnHessRecalcEditUpdated )
	EVT_TEXT( ID_STATPT_MODE_EDIT, InputBuilderWindow::OnStatptModeEditUpdated )
	EVT_CHECKBOX( ID_PRINTORBS_CHECK, InputBuilderWindow::OnPrintorbsCheckClick )
	EVT_CHECKBOX( ID_HESSEND_CHECK, InputBuilderWindow::OnHessendCheckClick )
	EVT_BUTTON( ID_DEFAULTSBUTTON, InputBuilderWindow::OnDefaultsbuttonClick )
	EVT_BUTTON( ID_REVERTBUTTON, InputBuilderWindow::OnRevertbuttonClick )
	EVT_BUTTON( ID_WRITEFILEBUTTON, InputBuilderWindow::OnWritefilebuttonClick )
	EVT_BUTTON( ID_ADVANCEDBUTTON, InputBuilderWindow::OnAdvancedButtonClicked )
	EVT_BUTTON( wxID_CANCEL, InputBuilderWindow::OnCancelClick )
	EVT_BUTTON( wxID_OK, InputBuilderWindow::OnOkClick )
////@end InputBuilderWindow event table entries
	EVT_BUTTON( ID_ENQUEUEBUTTON, InputBuilderWindow::OnEnqueueButtonClick )
END_EVENT_TABLE()

/*!
 * InputBuilderWindow constructors
 */

InputBuilderWindow::InputBuilderWindow( )
{
}

InputBuilderWindow::InputBuilderWindow( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Create( parent, id, caption, pos, size, style );
}

InputBuilderWindow::~InputBuilderWindow() {
    delete TmpInputRec;
}

/*!
 * InputBuilder creator
 */

bool InputBuilderWindow::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin InputBuilderWindow member initialisation
	listBook = NULL;
	basisChoice = NULL;
	ecpTypeLabel = NULL;
	ecpTypeChoice = NULL;
	numDChoice = NULL;
	numFChoice = NULL;
	numPChoice = NULL;
	polarLabel = NULL;
	polarChoice = NULL;
	diffuseLCheck = NULL;
	diffuseSCheck = NULL;
	spherHarmCheck = NULL;
	runChoice = NULL;
	scfChoice = NULL;
	localChoice = NULL;
	mchargeText = NULL;
	multText = NULL;
	exeChoice = NULL;
	scfIterText = NULL;
	mp2Check = NULL;
	dftCheck = NULL;
	ciLabel = NULL;
	ciChoice = NULL;
	ccLabel = NULL;
	ccChoice = NULL;
	titleText = NULL;
	coordTypeChoice = NULL;
	unitChoice = NULL;
	zmatrixVarsText = NULL;
	pointGroupChoice = NULL;
	paxisOrderText = NULL;
	paxisOrderChoice = NULL;
	symmetryCheck = NULL;
	timeLimitText = NULL;
	timeLimitUnitChoice = NULL;
	memoryText = NULL;
	memoryUnitChoice = NULL;
	memddiText = NULL;
	memddiUnitChoice = NULL;
	diagChoice = NULL;
	coreCheck = NULL;
	plBalanceRadio = NULL;
	externDataRepCheck = NULL;
	mSGParallCheck = NULL;
	mGVB_NCOEdit = NULL;
	mGVB_PairsEdit = NULL;
	mGVB_NSETOEdit = NULL;
	mGVB_NOStatic = NULL;
	mGVB_NOEdit = NULL;
	dftMethodChoice = NULL;
	funcSizer = NULL;
	dftGridFuncChoice = NULL;
	dftGridFreeFuncChoice = NULL;
	mFMOCheck = NULL;
	mNumFragsEdit = NULL;
	mFMOFragList = NULL;
	mFMOGenFragsButton = NULL;
	initGuessChoice = NULL;
	mMOSourceChoice = NULL;
	mVecOrbCountText = NULL;
	printInitGuessCheck = NULL;
	rotOrbitalsCheck = NULL;
	mHessMethodRadio = NULL;
	mHESSDisplaceEdit = NULL;
	mHessDblDiffCheck = NULL;
	mHessPurifyCheck = NULL;
	mHessPrintIntFCCheck = NULL;
	mHessVibAnlCheck = NULL;
	mHessFreqScaleEdit = NULL;
	mMolPltCheck = NULL;
	mPltOrbCheck = NULL;
	aimpacCheck = NULL;
	rpacCheck = NULL;
	mMiscProgRadio = NULL;
	mMP2CoreEleEdit = NULL;
	mMP2MemEdit = NULL;
	mMP2IntCutoffEdit = NULL;
	mLMOMP2Check = NULL;
	mMP2PropCheck = NULL;
	mMP2TransRadio = NULL;
	mMP2AOStorageRadio = NULL;
	mDirectSCFCheck = NULL;
	mFDiffCheck = NULL;
	mUHFNOCheck = NULL;
	mSCFConvSpin = NULL;
	mMaxStepCountEdit = NULL;
	mGradConvEdit = NULL;
	mOptMethodChoice = NULL;
	mInitStepSizeEdit = NULL;
	mMinStepSizeEdit = NULL;
	mMaxStepSizeEdit = NULL;
	mUpdateStepSizeCheck = NULL;
	mIsStatPtCheck = NULL;
	mStatPtJumpSizeEdit = NULL;
	mInitHessRadio = NULL;
	mHessRecalcEdit = NULL;
	mStatPtModeEdit = NULL;
	mPrintOrbsCheck = NULL;
	mTitleText = NULL;
	mBasisSetText = NULL;
	mSCFTypeText = NULL;
	mRunTypeText = NULL;
	mPointGroupText = NULL;
	mElectronCorr = NULL;
	defaultsBtn = NULL;
	revertBtn = NULL;
	writeBtn = NULL;
	advanced_button = NULL;
////@end InputBuilderWindow member initialisation

	TmpInputRec = new InputData(((MolDisplayWin *)parent)->GetData()->GetInputData());
	MoleculeData * MainData = ((MolDisplayWin *)parent)->GetData();
	if (MainData->GetFMOFragmentId(0)>0) {	//copy them over if setup
		Frame * cFrame = MainData->GetCurrentFramePtr();
		FMOFragmentIds.reserve(cFrame->GetNumAtoms());
		for (long i=0; i<cFrame->GetNumAtoms(); i++) {
			FMOFragmentIds.push_back(MainData->GetFMOFragmentId(i));
		}
	}

////@begin InputBuilderWindow creation
	wxFrame::Create( parent, id, caption, pos, size, style );

	CreateControls();
	if (GetSizer())
	{
		GetSizer()->SetSizeHints(this);
	}
	Centre();
////@end InputBuilderWindow creation
	UpdateWindowTitle();

    SetupItems();
    
    return true;
}

/*!
 * Update the Window title in the event the file is saved
 */
void InputBuilderWindow::UpdateWindowTitle(void) {
	MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	wxString foo = parent->GetTitle();
	foo += wxString(_(" ")) + wxString(SYMBOL_INPUTBUILDERWINDOW_TITLE);
	SetTitle(foo);
}

/*!
 * Control creation for InputBuilder
 */

void InputBuilderWindow::CreateControls()
{    

	// visibleTab has to be initialized before any calls AddPage(), since 
	// page change events will be triggered that read visibleTab.
    for(int i = NUM_PANES - 1; i >= 0; i--) {
        visibleTab[i] = true;
    }

////@begin InputBuilderWindow content construction
	InputBuilderWindow* itemFrame1 = this;

	wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
	itemFrame1->SetSizer(itemBoxSizer2);

	wxPanel* itemPanel3 = new wxPanel( itemFrame1, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
	itemBoxSizer2->Add(itemPanel3, 1, wxGROW, 0);

	wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
	itemPanel3->SetSizer(itemBoxSizer4);

	listBook = new wxListbook( itemPanel3, ID_IBNOTEBOOK, wxDefaultPosition, wxDefaultSize, wxLB_DEFAULT|wxLB_LEFT|wxSUNKEN_BORDER );

	wxPanel* itemPanel6 = new wxPanel( listBook, ID_IBBASISPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
	itemPanel6->SetSizer(itemBoxSizer7);

	wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer7->Add(itemBoxSizer8, 0, wxGROW, 5);
	wxStaticText* itemStaticText9 = new wxStaticText( itemPanel6, wxID_STATIC, _("Basis Set:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	itemBoxSizer8->Add(itemStaticText9, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	wxArrayString basisChoiceStrings;
	basisChoiceStrings.Add(_("MINI"));
	basisChoiceStrings.Add(_("MIDI"));
	basisChoiceStrings.Add(_("STO-2G"));
	basisChoiceStrings.Add(_("STO-3G"));
	basisChoiceStrings.Add(_("STO-4G"));
	basisChoiceStrings.Add(_("STO-5G"));
	basisChoiceStrings.Add(_("STO-6G"));
	basisChoiceStrings.Add(_("3-21G"));
	basisChoiceStrings.Add(_("6-21G"));
	basisChoiceStrings.Add(_("4-31G"));
	basisChoiceStrings.Add(_("5-31G"));
	basisChoiceStrings.Add(_("6-31G"));
	basisChoiceStrings.Add(_("6-311G"));
	basisChoiceStrings.Add(_("Double Zeta Valance"));
	basisChoiceStrings.Add(_("Dunning/Hay DZ"));
	basisChoiceStrings.Add(_("Triple Zeta Valence"));
	basisChoiceStrings.Add(_("McLean/Chandler"));
	basisChoiceStrings.Add(wxEmptyString);
	basisChoiceStrings.Add(_("Dunning cc-pVDZ"));
	basisChoiceStrings.Add(_("Dunning cc-pVTZ"));
	basisChoiceStrings.Add(_("Dunning cc-pVQZ"));
	basisChoiceStrings.Add(_("Dunning cc-pV5Z"));
	basisChoiceStrings.Add(_("Dunning cc-pV6Z"));
	basisChoiceStrings.Add(_("Dunning aug-cc-pVDZ"));
	basisChoiceStrings.Add(_("Dunning aug-cc-pVTZ"));
	basisChoiceStrings.Add(_("Dunning aug-cc-pVQZ"));
	basisChoiceStrings.Add(_("Dunning aug-cc-pV5Z"));
	basisChoiceStrings.Add(_("Dunning aug-cc-pV6Z"));
	basisChoiceStrings.Add(_("Dunning cc-pCVDZ"));
	basisChoiceStrings.Add(_("Dunning cc-pCVTZ"));
	basisChoiceStrings.Add(_("Dunning cc-pCVQZ"));
	basisChoiceStrings.Add(_("Dunning cc-pCV5Z"));
	basisChoiceStrings.Add(_("Dunning cc-pCV6Z"));
	basisChoiceStrings.Add(_("Dunning aug-cc-pCVDZ"));
	basisChoiceStrings.Add(_("Dunning aug-cc-pCVTZ"));
	basisChoiceStrings.Add(_("Dunning aug-cc-pCVQZ"));
	basisChoiceStrings.Add(_("Dunning aug-cc-pCV5Z"));
	basisChoiceStrings.Add(_("Dunning aug-cc-pCV6Z"));
	basisChoiceStrings.Add(_("Jensen PC0"));
	basisChoiceStrings.Add(_("Jensen PC1"));
	basisChoiceStrings.Add(_("Jensen PC2"));
	basisChoiceStrings.Add(_("Jensen PC3"));
	basisChoiceStrings.Add(_("Jensen PC4"));
	basisChoiceStrings.Add(_("Jensen aug-PC0"));
	basisChoiceStrings.Add(_("Jensen aug-PC1"));
	basisChoiceStrings.Add(_("Jensen aug-PC2"));
	basisChoiceStrings.Add(_("Jensen aug-PC3"));
	basisChoiceStrings.Add(_("Jensen aug-PC4"));
	basisChoiceStrings.Add(_("Sapporo non-relativistic SPK-DZP"));
	basisChoiceStrings.Add(_("Sapporo non-relativistic SPK-TZP"));
	basisChoiceStrings.Add(_("Sapporo non-relativistic SPK-QZP"));
	basisChoiceStrings.Add(_("Sapporo non-relativistic SPK-ADZP"));
	basisChoiceStrings.Add(_("Sapporo non-relativistic SPK-ATZP"));
	basisChoiceStrings.Add(_("Sapporo non-relativistic SPK-AQZP"));
	basisChoiceStrings.Add(_("Sapporo relativistic SPKrDZP"));
	basisChoiceStrings.Add(_("Sapporo relativistic SPKrTZP"));
	basisChoiceStrings.Add(_("Sapporo relativistic SPKrQZP"));
	basisChoiceStrings.Add(_("Sapporo relativistic SPKrADZP"));
	basisChoiceStrings.Add(_("Sapporo relativistic SPKrATZP"));
	basisChoiceStrings.Add(_("Sapporo relativistic SPKrAQZP"));
	basisChoiceStrings.Add(_("Karlsruhe valence triple zeta KTZV"));
	basisChoiceStrings.Add(_("Karlsruhe valence triple zeta KTZVP"));
	basisChoiceStrings.Add(_("Karlsruhe valence triple zeta KTZVPP"));
	basisChoiceStrings.Add(wxEmptyString);
	basisChoiceStrings.Add(_("SBKJC Valance"));
	basisChoiceStrings.Add(_("Hay/Wadt Valance"));
	basisChoiceStrings.Add(wxEmptyString);
	basisChoiceStrings.Add(_("MCP-DZP"));
	basisChoiceStrings.Add(_("MCP-TZP"));
	basisChoiceStrings.Add(_("MCP-QZP"));
	basisChoiceStrings.Add(_("MCP-ATZP"));
	basisChoiceStrings.Add(_("MCP-AQZP"));
	basisChoiceStrings.Add(_("MCPCDZP"));
	basisChoiceStrings.Add(_("MCPCTZP"));
	basisChoiceStrings.Add(_("MCPCQZP"));
	basisChoiceStrings.Add(_("MCPCADZP"));
	basisChoiceStrings.Add(_("MCPCATZP"));
	basisChoiceStrings.Add(_("MCPCAQZP"));
	basisChoiceStrings.Add(_("IMCP-SR1"));
	basisChoiceStrings.Add(_("IMCP-SR2"));
	basisChoiceStrings.Add(_("IMCP-NR1"));
	basisChoiceStrings.Add(_("IMCP-NR2"));
	basisChoiceStrings.Add(_("ZFK3-DK3"));
	basisChoiceStrings.Add(_("ZFK4-DK3"));
	basisChoiceStrings.Add(_("ZFK5-DK3"));
	basisChoiceStrings.Add(_("ZFK3LDK3"));
	basisChoiceStrings.Add(_("ZFK4LDK3"));
	basisChoiceStrings.Add(_("ZFK5LDK3"));
	basisChoiceStrings.Add(wxEmptyString);
	basisChoiceStrings.Add(_("MNDO"));
	basisChoiceStrings.Add(_("AM1"));
	basisChoiceStrings.Add(_("PM3"));
	basisChoiceStrings.Add(_("RM1"));
	basisChoice = new wxUglyChoice( itemPanel6, ID_BASIS_CHOICE, wxDefaultPosition, wxSize(200, -1), basisChoiceStrings, 0 );
	basisChoice->SetStringSelection(_("MINI"));
	itemBoxSizer8->Add(basisChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	itemBoxSizer8->Add(5, 5, 0, wxGROW|wxALL, 5);

	ecpTypeLabel = new wxStaticText( itemPanel6, wxID_STATIC, _("ECP Type:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	ecpTypeLabel->Enable(false);
	itemBoxSizer8->Add(ecpTypeLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	wxArrayString ecpTypeChoiceStrings;
	ecpTypeChoiceStrings.Add(_("None"));
	ecpTypeChoiceStrings.Add(_("Read"));
	ecpTypeChoiceStrings.Add(_("SBKJC"));
	ecpTypeChoiceStrings.Add(_("Hay-Wadt"));
	ecpTypeChoice = new wxUglyChoice( itemPanel6, ID_ECPT_CHOICE, wxDefaultPosition, wxDefaultSize, ecpTypeChoiceStrings, 0 );
	ecpTypeChoice->SetStringSelection(_("None"));
	ecpTypeChoice->Enable(false);
	itemBoxSizer8->Add(ecpTypeChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxFlexGridSizer* itemFlexGridSizer14 = new wxFlexGridSizer(0, 2, 0, 0);
	itemBoxSizer7->Add(itemFlexGridSizer14, 0, wxGROW, 5);
	wxStaticText* itemStaticText15 = new wxStaticText( itemPanel6, wxID_STATIC, _("# D heavy atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer14->Add(itemStaticText15, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	wxArrayString numDChoiceStrings;
	numDChoiceStrings.Add(_("0"));
	numDChoiceStrings.Add(_("1"));
	numDChoiceStrings.Add(_("2"));
	numDChoiceStrings.Add(_("3"));
	numDChoice = new wxUglyChoice( itemPanel6, ID_NUMD_CHOICE, wxDefaultPosition, wxDefaultSize, numDChoiceStrings, 0 );
	numDChoice->SetStringSelection(_("0"));
	itemFlexGridSizer14->Add(numDChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText17 = new wxStaticText( itemPanel6, wxID_STATIC, _("# F heavy atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer14->Add(itemStaticText17, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	wxArrayString numFChoiceStrings;
	numFChoiceStrings.Add(_("0"));
	numFChoiceStrings.Add(_("1"));
	numFChoiceStrings.Add(_("2"));
	numFChoiceStrings.Add(_("3"));
	numFChoice = new wxUglyChoice( itemPanel6, ID_NUMF_CHOICE, wxDefaultPosition, wxDefaultSize, numFChoiceStrings, 0 );
	numFChoice->SetStringSelection(_("0"));
	itemFlexGridSizer14->Add(numFChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText19 = new wxStaticText( itemPanel6, wxID_STATIC, _("# light atom polarization functions:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer14->Add(itemStaticText19, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	wxArrayString numPChoiceStrings;
	numPChoiceStrings.Add(_("0"));
	numPChoiceStrings.Add(_("1"));
	numPChoiceStrings.Add(_("2"));
	numPChoiceStrings.Add(_("3"));
	numPChoice = new wxUglyChoice( itemPanel6, ID_NUMP_CHOICE, wxDefaultPosition, wxDefaultSize, numPChoiceStrings, 0 );
	numPChoice->SetStringSelection(_("0"));
	itemFlexGridSizer14->Add(numPChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer21 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer7->Add(itemBoxSizer21, 0, wxGROW, 5);
	polarLabel = new wxStaticText( itemPanel6, wxID_STATIC, _("Polar:"), wxDefaultPosition, wxDefaultSize, 0 );
	polarLabel->Enable(false);
	itemBoxSizer21->Add(polarLabel, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	wxArrayString polarChoiceStrings;
	polarChoiceStrings.Add(_("Common"));
	polarChoiceStrings.Add(_("Pople N31"));
	polarChoiceStrings.Add(_("Pople N311"));
	polarChoiceStrings.Add(_("Dunning"));
	polarChoiceStrings.Add(_("Huzinaga"));
	polarChoiceStrings.Add(_("Hondo7"));
	polarChoice = new wxUglyChoice( itemPanel6, ID_POLAR_CHOICE, wxDefaultPosition, wxDefaultSize, polarChoiceStrings, 0 );
	polarChoice->SetStringSelection(_("Huzinaga"));
	polarChoice->Enable(false);
	itemBoxSizer21->Add(polarChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	diffuseLCheck = new wxCheckBox( itemPanel6, ID_DIFFL_CHECKBOX, _("Diffuse L-shell on heavy atoms"), wxDefaultPosition, wxDefaultSize, 0 );
	diffuseLCheck->SetValue(false);
	itemBoxSizer7->Add(diffuseLCheck, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	diffuseSCheck = new wxCheckBox( itemPanel6, ID_DIFFS_CHECKBOX, _("Diffuse s-shell on hydrogens"), wxDefaultPosition, wxDefaultSize, 0 );
	diffuseSCheck->SetValue(false);
	itemBoxSizer7->Add(diffuseSCheck, 0, wxGROW|wxALL, 5);

	spherHarmCheck = new wxCheckBox( itemPanel6, ID_SPHERE_CHECK, _("Use spherical harmonic functions (ie 5D, 7F, etc)"), wxDefaultPosition, wxDefaultSize, 0 );
	spherHarmCheck->SetValue(false);
	spherHarmCheck->SetHelpText(_("$CONTRL:ISPHER - GAMESS defaults to using cartesian gaussians. Some basis sets should only be used with spherical harmonics."));
	if (InputBuilderWindow::ShowToolTips())
		spherHarmCheck->SetToolTip(_("$CONTRL:ISPHER - GAMESS defaults to using cartesian gaussians. Some basis sets should only be used with spherical harmonics."));
	itemBoxSizer7->Add(spherHarmCheck, 0, wxGROW|wxALL, 5);

	listBook->AddPage(itemPanel6, _("Basis"));

	wxPanel* itemPanel27 = new wxPanel( listBook, ID_IBCONTROLPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer28 = new wxBoxSizer(wxVERTICAL);
	itemPanel27->SetSizer(itemBoxSizer28);

	wxStaticBox* itemStaticBoxSizer29Static = new wxStaticBox(itemPanel27, wxID_ANY, wxEmptyString);
	wxStaticBoxSizer* itemStaticBoxSizer29 = new wxStaticBoxSizer(itemStaticBoxSizer29Static, wxVERTICAL);
	itemBoxSizer28->Add(itemStaticBoxSizer29, 0, wxALIGN_LEFT|wxALL, 5);
	wxFlexGridSizer* itemFlexGridSizer30 = new wxFlexGridSizer(0, 2, 0, 0);
	itemStaticBoxSizer29->Add(itemFlexGridSizer30, 0, wxGROW, 5);
	wxStaticText* itemStaticText31 = new wxStaticText( itemPanel27, wxID_STATIC, _("Run Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer30->Add(itemStaticText31, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	wxArrayString runChoiceStrings;
	runChoiceStrings.Add(_("Energy"));
	runChoiceStrings.Add(_("Gradient"));
	runChoiceStrings.Add(_("Hessian"));
	runChoiceStrings.Add(_("Optimization"));
	runChoiceStrings.Add(_("Trudge"));
	runChoiceStrings.Add(_("Saddle Point"));
	runChoiceStrings.Add(_("Min. Energy Crossing"));
	runChoiceStrings.Add(_("IRC"));
	runChoiceStrings.Add(_("Molecular Dynamics"));
	runChoiceStrings.Add(_("Gradient Extremal"));
	runChoiceStrings.Add(_("DRC"));
	runChoiceStrings.Add(_("Energy Surface"));
	runChoiceStrings.Add(_("G3MP2"));
	runChoiceStrings.Add(_("Properties"));
	runChoiceStrings.Add(_("Morokuma"));
	runChoiceStrings.Add(_("Radiative Transition mom."));
	runChoiceStrings.Add(_("Spin Orbit"));
	runChoiceStrings.Add(_("Finite Electric Field"));
	runChoiceStrings.Add(_("TDHF"));
	runChoiceStrings.Add(_("extended TDHF"));
	runChoiceStrings.Add(_("Global Optimization"));
	runChoiceStrings.Add(_("VSCF"));
	runChoiceStrings.Add(_("FMO Optimization"));
	runChoiceStrings.Add(_("Raman Intensities"));
	runChoiceStrings.Add(_("NMR"));
	runChoiceStrings.Add(_("Make EFP"));
	runChoiceStrings.Add(_("Free state FMO"));
	runChoice = new wxUglyChoice( itemPanel27, ID_RUN_CHOICE, wxDefaultPosition, wxDefaultSize, runChoiceStrings, 0 );
	runChoice->SetStringSelection(_("Energy"));
	itemFlexGridSizer30->Add(runChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText33 = new wxStaticText( itemPanel27, wxID_STATIC, _("SCF Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer30->Add(itemStaticText33, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	wxArrayString scfChoiceStrings;
	scfChoiceStrings.Add(_("RHF"));
	scfChoiceStrings.Add(_("UHF"));
	scfChoiceStrings.Add(_("ROHF"));
	scfChoiceStrings.Add(_("GVB"));
	scfChoiceStrings.Add(_("MCSCF"));
	scfChoiceStrings.Add(_("None (CI)"));
	scfChoice = new wxUglyChoice( itemPanel27, ID_SCF_CHOICE, wxDefaultPosition, wxDefaultSize, scfChoiceStrings, 0 );
	scfChoice->SetStringSelection(_("RHF"));
	itemFlexGridSizer30->Add(scfChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText35 = new wxStaticText( itemPanel27, wxID_STATIC, _("Localization Method:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer30->Add(itemStaticText35, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	wxArrayString localChoiceStrings;
	localChoiceStrings.Add(_("None"));
	localChoiceStrings.Add(_("Foster-Boys"));
	localChoiceStrings.Add(_("Edmiston-Ruedenberg"));
	localChoiceStrings.Add(_("Pipek-Mezey"));
	localChoiceStrings.Add(_("SVD"));
	localChoice = new wxUglyChoice( itemPanel27, ID_LOCAL_CHOICE, wxDefaultPosition, wxDefaultSize, localChoiceStrings, 0 );
	localChoice->SetStringSelection(_("None"));
	itemFlexGridSizer30->Add(localChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer37 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer28->Add(itemBoxSizer37, 0, wxGROW, 5);
	wxBoxSizer* itemBoxSizer38 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer37->Add(itemBoxSizer38, 0, wxGROW, 5);
	wxStaticBox* itemStaticBoxSizer39Static = new wxStaticBox(itemPanel27, wxID_ANY, wxEmptyString);
	wxStaticBoxSizer* itemStaticBoxSizer39 = new wxStaticBoxSizer(itemStaticBoxSizer39Static, wxVERTICAL);
	itemBoxSizer38->Add(itemStaticBoxSizer39, 0, wxGROW|wxALL, 5);
	wxFlexGridSizer* itemFlexGridSizer40 = new wxFlexGridSizer(0, 2, 0, 0);
	itemStaticBoxSizer39->Add(itemFlexGridSizer40, 0, wxGROW, 5);
	wxStaticText* itemStaticText41 = new wxStaticText( itemPanel27, wxID_STATIC, _("Molecule Charge:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer40->Add(itemStaticText41, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	mchargeText = new wxTextCtrl( itemPanel27, ID_MCHARGE_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mchargeText->SetToolTip(_("$CONTRL:ICHARG - Enter an integer value for the molecular charge."));
	itemFlexGridSizer40->Add(mchargeText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText43 = new wxStaticText( itemPanel27, wxID_STATIC, _("Multiplicity:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer40->Add(itemStaticText43, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	multText = new wxTextCtrl( itemPanel27, ID_MULT_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		multText->SetToolTip(_("$CONTRL:MULT - Enter an integer value for the spin state."));
	itemFlexGridSizer40->Add(multText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticBox* itemStaticBoxSizer45Static = new wxStaticBox(itemPanel27, wxID_ANY, wxEmptyString);
	wxStaticBoxSizer* itemStaticBoxSizer45 = new wxStaticBoxSizer(itemStaticBoxSizer45Static, wxVERTICAL);
	itemBoxSizer38->Add(itemStaticBoxSizer45, 0, wxGROW|wxALL, 5);
	wxFlexGridSizer* itemFlexGridSizer46 = new wxFlexGridSizer(0, 2, 0, 0);
	itemStaticBoxSizer45->Add(itemFlexGridSizer46, 0, wxGROW, 5);
	wxStaticText* itemStaticText47 = new wxStaticText( itemPanel27, wxID_STATIC, _("Exe. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer46->Add(itemStaticText47, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	wxArrayString exeChoiceStrings;
	exeChoiceStrings.Add(_("Normal Run"));
	exeChoiceStrings.Add(_("Check"));
	exeChoiceStrings.Add(_("Debug"));
	exeChoiceStrings.Add(_("Other..."));
	exeChoice = new wxUglyChoice( itemPanel27, ID_EXE_CHOICE, wxDefaultPosition, wxDefaultSize, exeChoiceStrings, 0 );
	exeChoice->SetStringSelection(_("Normal Run"));
	itemFlexGridSizer46->Add(exeChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText49 = new wxStaticText( itemPanel27, wxID_STATIC, _("Max # SCF Iterations:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer46->Add(itemStaticText49, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	scfIterText = new wxTextCtrl( itemPanel27, ID_MAXIT_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		scfIterText->SetToolTip(_("$CONTRL:MAXIT - Enter the maximum number of SCF iterations. If the wavefunction is not converged at this point the run will be aborted."));
	itemFlexGridSizer46->Add(scfIterText, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticBox* itemStaticBoxSizer51Static = new wxStaticBox(itemPanel27, wxID_ANY, wxEmptyString);
	wxStaticBoxSizer* itemStaticBoxSizer51 = new wxStaticBoxSizer(itemStaticBoxSizer51Static, wxVERTICAL);
	itemBoxSizer37->Add(itemStaticBoxSizer51, 0, wxALIGN_TOP|wxALL, 5);
	mp2Check = new wxCheckBox( itemPanel27, ID_MP2_CHECKBOX, _("Use MP2"), wxDefaultPosition, wxDefaultSize, 0 );
	mp2Check->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mp2Check->SetToolTip(_("$CONTRL:MPLEVL - Click to use 2nd order Moller-Plesset perturbation theory. Implemented for RHF, UHF, ROHF and MCSCF energies and RHF, UHF, and ROHF gradients."));
	itemStaticBoxSizer51->Add(mp2Check, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	dftCheck = new wxCheckBox( itemPanel27, ID_DFT_CHECKBOX, _("Use DFT"), wxDefaultPosition, wxDefaultSize, 0 );
	dftCheck->SetValue(false);
	itemStaticBoxSizer51->Add(dftCheck, 0, wxGROW|wxALL, 5);

	wxFlexGridSizer* itemFlexGridSizer54 = new wxFlexGridSizer(0, 2, 0, 0);
	itemStaticBoxSizer51->Add(itemFlexGridSizer54, 0, wxGROW, 5);
	ciLabel = new wxStaticText( itemPanel27, wxID_STATIC, _("CI:"), wxDefaultPosition, wxDefaultSize, 0 );
	if (InputBuilderWindow::ShowToolTips())
		ciLabel->SetToolTip(_("$CONTRL:CITYP Choose the type of CI to perform on top of the base wavefunction or on the supplied $VEC group for SCFTYP=NONE."));
	itemFlexGridSizer54->Add(ciLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	wxArrayString ciChoiceStrings;
	ciChoiceStrings.Add(_("None"));
	ciChoiceStrings.Add(_("GUGA"));
	ciChoiceStrings.Add(_("Ames Lab. Determinant"));
	ciChoiceStrings.Add(_("Occupation Restricted Multiple Active Space"));
	ciChoiceStrings.Add(_("CI Singles"));
	ciChoiceStrings.Add(_("Full Second Order CI"));
	ciChoiceStrings.Add(_("General CI"));
	ciChoice = new wxUglyChoice( itemPanel27, ID_CI_CHOICE, wxDefaultPosition, wxDefaultSize, ciChoiceStrings, 0 );
	ciChoice->SetStringSelection(_("None"));
	itemFlexGridSizer54->Add(ciChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	ccLabel = new wxStaticText( itemPanel27, wxID_STATIC, _("CC:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer54->Add(ccLabel, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	wxArrayString ccChoiceStrings;
	ccChoiceStrings.Add(_("None"));
	ccChoiceStrings.Add(_("LCCD: linearized CC"));
	ccChoiceStrings.Add(_("CCD: CC with doubles"));
	ccChoiceStrings.Add(_("CCSD: CC with singles and doubles"));
	ccChoiceStrings.Add(_("CCSD(T)"));
	ccChoiceStrings.Add(_("R-CC"));
	ccChoiceStrings.Add(_("CR-CC"));
	ccChoiceStrings.Add(_("CR-CCL"));
	ccChoiceStrings.Add(_("CCSD(TQ)"));
	ccChoiceStrings.Add(_("CR-CC(Q)"));
	ccChoiceStrings.Add(_("EOM-CCSD"));
	ccChoiceStrings.Add(_("CR-EOM"));
	ccChoiceStrings.Add(_("CR-EOML"));
	ccChoiceStrings.Add(_("IP-EOM2"));
	ccChoiceStrings.Add(_("IP-EOM3A"));
	ccChoiceStrings.Add(_("EA-EOM2"));
	ccChoiceStrings.Add(_("EA-EOM3A"));
	ccChoice = new wxUglyChoice( itemPanel27, ID_CC_CHOICE, wxDefaultPosition, wxDefaultSize, ccChoiceStrings, 0 );
	ccChoice->SetStringSelection(_("None"));
	itemFlexGridSizer54->Add(ccChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	listBook->AddPage(itemPanel27, _("Control"));

	wxPanel* itemPanel59 = new wxPanel( listBook, ID_IBDATAPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer60 = new wxBoxSizer(wxVERTICAL);
	itemPanel59->SetSizer(itemBoxSizer60);

	wxBoxSizer* itemBoxSizer61 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer60->Add(itemBoxSizer61, 0, wxALIGN_LEFT, 5);
	titleText = new wxTextCtrl( itemPanel59, ID_TITLE_TEXTCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	if (InputBuilderWindow::ShowToolTips())
		titleText->SetToolTip(_("$DATA - You may enter a one line title which may help you identify this input deck in the future."));
	itemBoxSizer61->Add(titleText, 0, wxGROW|wxALL, 5);

	wxStaticBox* itemStaticBoxSizer63Static = new wxStaticBox(itemPanel59, wxID_ANY, wxEmptyString);
	wxStaticBoxSizer* itemStaticBoxSizer63 = new wxStaticBoxSizer(itemStaticBoxSizer63Static, wxVERTICAL);
	itemBoxSizer61->Add(itemStaticBoxSizer63, 0, wxALIGN_LEFT|wxALL, 5);
	wxFlexGridSizer* itemFlexGridSizer64 = new wxFlexGridSizer(0, 2, 0, 0);
	itemStaticBoxSizer63->Add(itemFlexGridSizer64, 0, wxGROW, 5);
	wxStaticText* itemStaticText65 = new wxStaticText( itemPanel59, wxID_STATIC, _("Coord. Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer64->Add(itemStaticText65, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	wxArrayString coordTypeChoiceStrings;
	coordTypeChoiceStrings.Add(_("Unique cartesian Coords."));
	coordTypeChoiceStrings.Add(_("Hilderbrant internals"));
	coordTypeChoiceStrings.Add(_("Cartesian coordinates"));
	coordTypeChoiceStrings.Add(_("Z-Matrix"));
	coordTypeChoiceStrings.Add(_("MOPAC Z-Matrix"));
	coordTypeChoice = new wxUglyChoice( itemPanel59, ID_COORD_CHOICE, wxDefaultPosition, wxDefaultSize, coordTypeChoiceStrings, 0 );
	coordTypeChoice->SetStringSelection(_("Unique cartesian Coords."));
	itemFlexGridSizer64->Add(coordTypeChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText67 = new wxStaticText( itemPanel59, wxID_STATIC, _("Units:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer64->Add(itemStaticText67, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	wxArrayString unitChoiceStrings;
	unitChoiceStrings.Add(_("Angstroms"));
	unitChoiceStrings.Add(_("Bohr"));
	unitChoice = new wxUglyChoice( itemPanel59, ID_UNIT_CHOICE, wxDefaultPosition, wxDefaultSize, unitChoiceStrings, 0 );
	unitChoice->SetStringSelection(_("Angstroms"));
	itemFlexGridSizer64->Add(unitChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer69 = new wxBoxSizer(wxHORIZONTAL);
	itemStaticBoxSizer63->Add(itemBoxSizer69, 0, wxGROW, 5);
	wxStaticText* itemStaticText70 = new wxStaticText( itemPanel59, wxID_STATIC, _("# of Z-Matrix Variables:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer69->Add(itemStaticText70, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	zmatrixVarsText = new wxTextCtrl( itemPanel59, ID_ZMAT_VARS_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		zmatrixVarsText->SetToolTip(_("$CONTRL:NZVAR - Enter an integer number representing the number of internal coordinates for your molecule. Normally this will be 3N-6 (3N-5 for linear molecules) where N is the number of atoms. A value of 0 selects cartesian coordinates. If set and a set of internal coordinates are defined a $ZMAT group will be punched out."));
	itemBoxSizer69->Add(zmatrixVarsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticBox* itemStaticBoxSizer72Static = new wxStaticBox(itemPanel59, wxID_ANY, wxEmptyString);
	wxStaticBoxSizer* itemStaticBoxSizer72 = new wxStaticBoxSizer(itemStaticBoxSizer72Static, wxVERTICAL);
	itemBoxSizer61->Add(itemStaticBoxSizer72, 0, wxALIGN_LEFT|wxALL, 5);
	wxFlexGridSizer* itemFlexGridSizer73 = new wxFlexGridSizer(0, 2, 0, 0);
	itemStaticBoxSizer72->Add(itemFlexGridSizer73, 0, wxGROW, 5);
	wxStaticText* itemStaticText74 = new wxStaticText( itemPanel59, wxID_STATIC, _("Point Group:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer73->Add(itemStaticText74, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	wxArrayString pointGroupChoiceStrings;
	pointGroupChoiceStrings.Add(_("C1"));
	pointGroupChoiceStrings.Add(_("CS"));
	pointGroupChoiceStrings.Add(_("CI"));
	pointGroupChoiceStrings.Add(_("CnH"));
	pointGroupChoiceStrings.Add(_("CnV"));
	pointGroupChoiceStrings.Add(_("Cn"));
	pointGroupChoiceStrings.Add(_("S2n"));
	pointGroupChoiceStrings.Add(_("DnD"));
	pointGroupChoiceStrings.Add(_("DnH"));
	pointGroupChoiceStrings.Add(_("Dn"));
	pointGroupChoiceStrings.Add(_("TD"));
	pointGroupChoiceStrings.Add(_("TH"));
	pointGroupChoiceStrings.Add(_("T"));
	pointGroupChoiceStrings.Add(_("OH"));
	pointGroupChoiceStrings.Add(_("O"));
	pointGroupChoice = new wxUglyChoice( itemPanel59, ID_POINTGROUP_CHOICE, wxDefaultPosition, wxDefaultSize, pointGroupChoiceStrings, 0 );
	itemFlexGridSizer73->Add(pointGroupChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	paxisOrderText = new wxStaticText( itemPanel59, wxID_STATIC, _("Order of Principle Axis:"), wxDefaultPosition, wxDefaultSize, 0 );
	paxisOrderText->Enable(false);
	itemFlexGridSizer73->Add(paxisOrderText, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	wxArrayString paxisOrderChoiceStrings;
	paxisOrderChoiceStrings.Add(_("2"));
	paxisOrderChoiceStrings.Add(_("3"));
	paxisOrderChoiceStrings.Add(_("4"));
	paxisOrderChoiceStrings.Add(_("5"));
	paxisOrderChoiceStrings.Add(_("6"));
	paxisOrderChoiceStrings.Add(_("7"));
	paxisOrderChoiceStrings.Add(_("8"));
	paxisOrderChoice = new wxUglyChoice( itemPanel59, ID_ORDER_CHOICE, wxDefaultPosition, wxDefaultSize, paxisOrderChoiceStrings, 0 );
	if (InputBuilderWindow::ShowToolTips())
		paxisOrderChoice->SetToolTip(_("Replaces the 'n' above."));
	paxisOrderChoice->Enable(false);
	itemFlexGridSizer73->Add(paxisOrderChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	symmetryCheck = new wxCheckBox( itemPanel59, ID_SYMMETRY_CHECKBOX, _("Use Symmetry During Calculation"), wxDefaultPosition, wxDefaultSize, 0 );
	symmetryCheck->SetValue(true);
	if (InputBuilderWindow::ShowToolTips())
		symmetryCheck->SetToolTip(_("$CONTRL:NOSYM - When checked symmetry will be used as much as possible in the caluclation of integrals, gradients, etc. (This is the normal setting)"));
	itemStaticBoxSizer72->Add(symmetryCheck, 0, wxALIGN_LEFT|wxALL, 5);

	listBook->AddPage(itemPanel59, _("Data"));

	wxPanel* itemPanel79 = new wxPanel( listBook, ID_IBSYSTEMPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer80 = new wxBoxSizer(wxVERTICAL);
	itemPanel79->SetSizer(itemBoxSizer80);

	wxFlexGridSizer* itemFlexGridSizer81 = new wxFlexGridSizer(0, 3, 0, 0);
	itemBoxSizer80->Add(itemFlexGridSizer81, 0, wxGROW, 5);
	wxStaticText* itemStaticText82 = new wxStaticText( itemPanel79, wxID_STATIC, _("Time Limit:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer81->Add(itemStaticText82, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	timeLimitText = new wxTextCtrl( itemPanel79, ID_TIMELIMIT_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	if (InputBuilderWindow::ShowToolTips())
		timeLimitText->SetToolTip(_("$SYSTEM:TIMLIM - Enter a value for the time limit. When the time limit is reached GAMESS will stop the run. The number entered here will have the units given at the right."));
	itemFlexGridSizer81->Add(timeLimitText, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString timeLimitUnitChoiceStrings;
	timeLimitUnitChoiceStrings.Add(_("Seconds"));
	timeLimitUnitChoiceStrings.Add(_("Minutes"));
	timeLimitUnitChoiceStrings.Add(_("Hours"));
	timeLimitUnitChoiceStrings.Add(_("Days"));
	timeLimitUnitChoiceStrings.Add(_("Weeks"));
	timeLimitUnitChoiceStrings.Add(_("Years"));
	timeLimitUnitChoiceStrings.Add(_("Millenia"));
	timeLimitUnitChoice = new wxUglyChoice( itemPanel79, ID_TIMELIMITUNITS_CHOICE, wxDefaultPosition, wxDefaultSize, timeLimitUnitChoiceStrings, 0 );
	timeLimitUnitChoice->SetStringSelection(_("Minutes"));
	itemFlexGridSizer81->Add(timeLimitUnitChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText85 = new wxStaticText( itemPanel79, wxID_STATIC, _("Memory:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer81->Add(itemStaticText85, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	memoryText = new wxTextCtrl( itemPanel79, ID_MEMORY_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	if (InputBuilderWindow::ShowToolTips())
		memoryText->SetToolTip(_("$SYSTEM:MEMORY - Enter the amount of memory (in the units at the right) that GAMESS will request for its dynamic memory pool. You should not normally request more memory than the RAM size."));
	itemFlexGridSizer81->Add(memoryText, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString memoryUnitChoiceStrings;
	memoryUnitChoiceStrings.Add(_("Words"));
	memoryUnitChoiceStrings.Add(_("Bytes"));
	memoryUnitChoiceStrings.Add(_("MegaWords"));
	memoryUnitChoiceStrings.Add(_("MegaBytes"));
	memoryUnitChoice = new wxUglyChoice( itemPanel79, ID_MEMORYUNITS_CHOICE, wxDefaultPosition, wxDefaultSize, memoryUnitChoiceStrings, 0 );
	memoryUnitChoice->SetStringSelection(_("Words"));
	itemFlexGridSizer81->Add(memoryUnitChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* itemStaticText88 = new wxStaticText( itemPanel79, wxID_STATIC, _("MemDDI:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer81->Add(itemStaticText88, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	memddiText = new wxTextCtrl( itemPanel79, ID_MEMDDI_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	if (InputBuilderWindow::ShowToolTips())
		memddiText->SetToolTip(_("$SYSTEM:MEMDDI - The size of the pseudo global shared memory pool. This is most often needed for certain parallel computations, but certain sequential algorithms also use it (such as ROMP2). Default is 0."));
	itemFlexGridSizer81->Add(memddiText, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString memddiUnitChoiceStrings;
	memddiUnitChoiceStrings.Add(_("MegaWords"));
	memddiUnitChoiceStrings.Add(_("MegaBytes"));
	memddiUnitChoiceStrings.Add(_("GigaWords"));
	memddiUnitChoiceStrings.Add(_("GigaBytes"));
	memddiUnitChoice = new wxUglyChoice( itemPanel79, ID_MEMDDICHOICE, wxDefaultPosition, wxDefaultSize, memddiUnitChoiceStrings, 0 );
	memddiUnitChoice->SetStringSelection(_("MegaWords"));
	itemFlexGridSizer81->Add(memddiUnitChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer91 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer80->Add(itemBoxSizer91, 0, wxGROW, 5);
	wxStaticText* itemStaticText92 = new wxStaticText( itemPanel79, wxID_STATIC, _("Diagonalization Method:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer91->Add(itemStaticText92, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	wxArrayString diagChoiceStrings;
	diagChoiceStrings.Add(_("default"));
	diagChoiceStrings.Add(_("EVVRSP"));
	diagChoiceStrings.Add(_("GIVEIS"));
	diagChoiceStrings.Add(_("JACOBI"));
	diagChoice = new wxUglyChoice( itemPanel79, ID_DIAGONALIZATION_CHOICE, wxDefaultPosition, wxDefaultSize, diagChoiceStrings, 0 );
	diagChoice->SetStringSelection(_("default"));
	itemBoxSizer91->Add(diagChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	coreCheck = new wxCheckBox( itemPanel79, ID_CORE_CHECKBOX, _("Produce \"core\" file upon abort"), wxDefaultPosition, wxDefaultSize, 0 );
	coreCheck->SetValue(false);
	itemBoxSizer80->Add(coreCheck, 0, wxALIGN_LEFT|wxALL, 5);

	wxArrayString plBalanceRadioStrings;
	plBalanceRadioStrings.Add(_("Loop"));
	plBalanceRadioStrings.Add(_("Next Value"));
	plBalanceRadio = new wxRadioBox( itemPanel79, ID_PLBALANCE_RADIOBOX, _("Parallel Load Balance Type"), wxDefaultPosition, wxDefaultSize, plBalanceRadioStrings, 1, wxRA_SPECIFY_COLS );
	plBalanceRadio->SetSelection(0);
	if (InputBuilderWindow::ShowToolTips())
		plBalanceRadio->SetToolTip(_("$SYSTEM:BALTYP - Select the type of load balancing to use. Only applicable to runs on parallel computers."));
	itemBoxSizer80->Add(plBalanceRadio, 0, wxALIGN_LEFT|wxALL, 5);

	externDataRepCheck = new wxCheckBox( itemPanel79, ID_EXTERNDATAREP_CHECKBOX, _("Use External Data Representation for messages"), wxDefaultPosition, wxDefaultSize, 0 );
	externDataRepCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		externDataRepCheck->SetToolTip(_("$SYSTEM:XDR= Click to cause messages to be converted into a generic data format to pass between nodes from different vendors."));
	itemBoxSizer80->Add(externDataRepCheck, 0, wxALIGN_LEFT|wxALL, 5);

	mSGParallCheck = new wxCheckBox( itemPanel79, ID_PARALL_CHECK, _("Force parallel methods"), wxDefaultPosition, wxDefaultSize, 0 );
	mSGParallCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mSGParallCheck->SetToolTip(_("$SYSTEM:PARALL - Forces the use of parallel methods even when running on 1 processor. This is necessary to a check run on the parallel MP2 code for example."));
	itemBoxSizer80->Add(mSGParallCheck, 0, wxALIGN_LEFT|wxALL, 5);

	listBook->AddPage(itemPanel79, _("System"));

	wxPanel* itemPanel98 = new wxPanel( listBook, ID_GVBPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer99 = new wxBoxSizer(wxVERTICAL);
	itemPanel98->SetSizer(itemBoxSizer99);

	wxFlexGridSizer* itemFlexGridSizer100 = new wxFlexGridSizer(0, 2, 0, 0);
	itemBoxSizer99->Add(itemFlexGridSizer100, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText101 = new wxStaticText( itemPanel98, wxID_STATIC, _("Number of Closed Shell Orbitals:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer100->Add(itemStaticText101, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mGVB_NCOEdit = new wxTextCtrl( itemPanel98, ID_GVB_NCOEDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mGVB_NCOEdit->SetToolTip(_("$SCF:NCO You almost certainly need to change this from the default!"));
	itemFlexGridSizer100->Add(mGVB_NCOEdit, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* itemStaticText103 = new wxStaticText( itemPanel98, wxID_STATIC, _("Number of Geminal Pairs:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer100->Add(itemStaticText103, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mGVB_PairsEdit = new wxTextCtrl( itemPanel98, ID_GVB_PAIRSEDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mGVB_PairsEdit->SetToolTip(_("$SCF:NPAIR"));
	itemFlexGridSizer100->Add(mGVB_PairsEdit, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStaticText* itemStaticText105 = new wxStaticText( itemPanel98, wxID_STATIC, _("Number of Sets of Open Shells:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer100->Add(itemStaticText105, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mGVB_NSETOEdit = new wxTextCtrl( itemPanel98, ID_GVB_NSETOEDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mGVB_NSETOEdit->SetToolTip(_("$SCF:NSETO"));
	itemFlexGridSizer100->Add(mGVB_NSETOEdit, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mGVB_NOStatic = new wxStaticText( itemPanel98, wxID_STATIC, _("Degeneracy of Each Open Shell:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer100->Add(mGVB_NOStatic, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mGVB_NOEdit = new wxTextCtrl( itemPanel98, ID_GVB_NOEDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mGVB_NOEdit->SetToolTip(_("$SCF:NO - A space-delimited array providing the degeneracy of each open shell. There must be the number of values specified directly above."));
	itemFlexGridSizer100->Add(mGVB_NOEdit, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	listBook->AddPage(itemPanel98, _("GVB Parameters"));

	wxPanel* itemPanel109 = new wxPanel( listBook, ID_IBDFTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer110 = new wxBoxSizer(wxVERTICAL);
	itemPanel109->SetSizer(itemBoxSizer110);

	wxFlexGridSizer* itemFlexGridSizer111 = new wxFlexGridSizer(0, 2, 0, 0);
	itemBoxSizer110->Add(itemFlexGridSizer111, 0, wxALIGN_LEFT, 5);
	wxStaticText* itemStaticText112 = new wxStaticText( itemPanel109, wxID_STATIC, _("Method:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer111->Add(itemStaticText112, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	wxArrayString dftMethodChoiceStrings;
	dftMethodChoiceStrings.Add(_("Grid"));
	dftMethodChoiceStrings.Add(_("Grid-Free"));
	dftMethodChoice = new wxUglyChoice( itemPanel109, ID_DFTMETHOD_CHOICE, wxDefaultPosition, wxDefaultSize, dftMethodChoiceStrings, 0 );
	dftMethodChoice->SetStringSelection(_("Grid"));
	itemFlexGridSizer111->Add(dftMethodChoice, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText114 = new wxStaticText( itemPanel109, wxID_STATIC, _("DFT Functional:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer111->Add(itemStaticText114, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	funcSizer = new wxBoxSizer(wxVERTICAL);
	itemFlexGridSizer111->Add(funcSizer, 1, wxGROW|wxGROW, 5);
	wxArrayString dftGridFuncChoiceStrings;
	dftGridFuncChoiceStrings.Add(_("Slater exchange"));
	dftGridFuncChoiceStrings.Add(_("Becke 1988 exchange"));
	dftGridFuncChoiceStrings.Add(_("Gill 1996 exchange"));
	dftGridFuncChoiceStrings.Add(_("OPTX: Handy-Cohen exchange"));
	dftGridFuncChoiceStrings.Add(_("PW91X: Perdew-Want 1991 exchange"));
	dftGridFuncChoiceStrings.Add(_("PBEX: Perdew-Burke-Ernzerhof exchange"));
	dftGridFuncChoiceStrings.Add(_("VWN: Vosko-Wilk-Nusair (VWN5) correlation"));
	dftGridFuncChoiceStrings.Add(_("VWN1: VWN correlation formula 1"));
	dftGridFuncChoiceStrings.Add(_("PZ81: Perdew-Wang 1981 correlation"));
	dftGridFuncChoiceStrings.Add(_("P86: Perdew 1986 correlation"));
	dftGridFuncChoiceStrings.Add(_("LYP: Lee-Yang-Parr correlation"));
	dftGridFuncChoiceStrings.Add(_("PW91C: Perdew-Wang 1991 correlation"));
	dftGridFuncChoiceStrings.Add(_("PBEC: Perdew-Burke-Ernzerhof correlation"));
	dftGridFuncChoiceStrings.Add(_("OP: one parameter progressive correlation"));
	dftGridFuncChoiceStrings.Add(_("SVWN: Slater exhange + VWN correlation"));
	dftGridFuncChoiceStrings.Add(_("BLYP: BECKE exchange + LYP correlation"));
	dftGridFuncChoiceStrings.Add(_("BOP: BECKE exchange + OP correlation"));
	dftGridFuncChoiceStrings.Add(_("BP86: Becke exchange + P86 correlation"));
	dftGridFuncChoiceStrings.Add(_("GVWN: Gill exchange + VWN5 correlation"));
	dftGridFuncChoiceStrings.Add(_("GPW91: Gill exchange + PW91 correlation"));
	dftGridFuncChoiceStrings.Add(_("PBEVWN: PBE exchange + VWN5 correlation"));
	dftGridFuncChoiceStrings.Add(_("PBEOP: PBE exchange + OP correlation"));
	dftGridFuncChoiceStrings.Add(_("OLYP: OPTX exchange + LYP correlation"));
	dftGridFuncChoiceStrings.Add(_("PW91: PW91 exhange and correlation"));
	dftGridFuncChoiceStrings.Add(_("PBE: exchange and correlation"));
	dftGridFuncChoiceStrings.Add(_("revPBE: PBE revised by Zhang-Yang"));
	dftGridFuncChoiceStrings.Add(_("RPBE: PBE revised by Hammer-Hansen-Norshov"));
	dftGridFuncChoiceStrings.Add(_("PBEsol: PBE revised by Perdew et al for solids"));
	dftGridFuncChoiceStrings.Add(_("EDF1: empirical density functional #1"));
	dftGridFuncChoiceStrings.Add(_("HCTH93: Hamprecht-Cohen-Tozer-Handy 1998 fit to 93 systems"));
	dftGridFuncChoiceStrings.Add(_("HCTH120: later fit to 120 systems"));
	dftGridFuncChoiceStrings.Add(_("HCTH147: later fit to 147 systems"));
	dftGridFuncChoiceStrings.Add(_("HCTH407: later fit to 407 systems"));
	dftGridFuncChoiceStrings.Add(_("SOGGA: PBE revised by Zhang-Truhlar for solids"));
	dftGridFuncChoiceStrings.Add(_("MOHLYP: metal optimized OPTX, half LYP"));
	dftGridFuncChoiceStrings.Add(_("B97-D: Grimme's mod. B97"));
	dftGridFuncChoiceStrings.Add(_("BHHLYP: HF and Becke exchange + LYP correlation"));
	dftGridFuncChoiceStrings.Add(_("B3PW91: Becke's 3 parameter exchange + PW91 correlation"));
	dftGridFuncChoiceStrings.Add(_("B3LYP: Becke+Slater+HF exchange and LYP+VWN5 correlation"));
	dftGridFuncChoiceStrings.Add(_("B3LYP1: use of VWN1 in B3LYP"));
	dftGridFuncChoiceStrings.Add(_("B97: Becke's 1997 hybrid functional"));
	dftGridFuncChoiceStrings.Add(_("B97-1: Hamprect-Cohen-Tozer-Handy reparam. of B97"));
	dftGridFuncChoiceStrings.Add(_("B97-2: Wilson-Bradley-Tozer 2001 mod to B97"));
	dftGridFuncChoiceStrings.Add(_("B97-3: Keal-Tozer 2005 mod to B97"));
	dftGridFuncChoiceStrings.Add(_("B97-K: Boese-Martin 2004 mod for kinetics"));
	dftGridFuncChoiceStrings.Add(_("B98: Schmider-Becke 1998 mod to B97"));
	dftGridFuncChoiceStrings.Add(_("PBE0: hybrid made from PBE"));
	dftGridFuncChoiceStrings.Add(_("X3LYP: HF+Slater+Becke88+PW91 exchange and LYP+VWN1 correlation"));
	dftGridFuncChoiceStrings.Add(_("CAMB3LYP: coulomb attenuated B3LYP"));
	dftGridFuncChoiceStrings.Add(_("wB97: omega separated form of B97"));
	dftGridFuncChoiceStrings.Add(_("wB97X: wB97 with short-range HF exchange"));
	dftGridFuncChoiceStrings.Add(_("wB97X-D: dispersion corrected wB97X"));
	dftGridFuncChoiceStrings.Add(_("B2PLYP: mixes BLYP, HF exchange and MP2"));
	dftGridFuncChoiceStrings.Add(_("xB97X-2"));
	dftGridFuncChoiceStrings.Add(_("wB97X-2L"));
	dftGridFuncChoiceStrings.Add(_("VS98: Voorhis-Scuseria 1998"));
	dftGridFuncChoiceStrings.Add(_("PKZB: Perdew-Kurth-Zupan-Blaha 1999"));
	dftGridFuncChoiceStrings.Add(_("tHCTH: Boese-Handy 2002 metaGGA"));
	dftGridFuncChoiceStrings.Add(_("tHCTHhyb: tHCTH hybrid with 15% HF exchange"));
	dftGridFuncChoiceStrings.Add(_("BMK: Boese-Martin 2004 param. for kinetics"));
	dftGridFuncChoiceStrings.Add(_("TPSS: Tao-Perdew-Staroverov-Scuseria 2003"));
	dftGridFuncChoiceStrings.Add(_("TPSSh: TPSS with 10% HF exchange"));
	dftGridFuncChoiceStrings.Add(_("TPSSm: TPSS with mod. parameter, 2007"));
	dftGridFuncChoiceStrings.Add(_("revTPSS: TPSS rev. 2009"));
	dftGridFuncChoiceStrings.Add(_("M05: Minnesota exchange-corr. 2005"));
	dftGridFuncChoiceStrings.Add(_("M05-2X: M05 with mod. HF exchange"));
	dftGridFuncChoiceStrings.Add(_("M06: Minnesota 2006"));
	dftGridFuncChoiceStrings.Add(_("M06-L: M06 with 0% HF exchange"));
	dftGridFuncChoiceStrings.Add(_("M06-2X: M06 with doubled HF exchange"));
	dftGridFuncChoiceStrings.Add(_("M06-HF: M06 with 100% HF exchange"));
	dftGridFuncChoiceStrings.Add(_("M08-HX: M08 with high HF exchange"));
	dftGridFuncChoiceStrings.Add(_("M08-SO: M08 form different parameters"));
	dftGridFuncChoice = new wxUglyChoice( itemPanel109, ID_GRID_FUNCTIONAL_CHOICE, wxDefaultPosition, wxDefaultSize, dftGridFuncChoiceStrings, 0 );
	dftGridFuncChoice->SetStringSelection(_("Slater exchange"));
	funcSizer->Add(dftGridFuncChoice, 0, wxALIGN_LEFT|wxALL, 5);

	wxArrayString dftGridFreeFuncChoiceStrings;
	dftGridFreeFuncChoiceStrings.Add(_("X-Alpha exchange"));
	dftGridFreeFuncChoiceStrings.Add(_("Slater exchange"));
	dftGridFreeFuncChoiceStrings.Add(_("Becke's 1988 exchange"));
	dftGridFreeFuncChoiceStrings.Add(_("Depristo/Kress exchange"));
	dftGridFreeFuncChoiceStrings.Add(_("CAMA: Handy et al's mods to Becke exchange"));
	dftGridFreeFuncChoiceStrings.Add(_("HALF: 50-50 mix of Becke and HF exchange"));
	dftGridFreeFuncChoiceStrings.Add(_("VWN: Vosko/Wilke/Nusair corr"));
	dftGridFreeFuncChoiceStrings.Add(_("PWLOC: Perdew/Wang local corr"));
	dftGridFreeFuncChoiceStrings.Add(_("LYP: Lee/Yang/Parr corr"));
	dftGridFreeFuncChoiceStrings.Add(_("SVWN: Slater exchange + VWN corr"));
	dftGridFreeFuncChoiceStrings.Add(_("BVWN: Becke exchange + VWN corr"));
	dftGridFreeFuncChoiceStrings.Add(_("BLYP: Becke exchange + LYP corr"));
	dftGridFreeFuncChoiceStrings.Add(_("BPWLOC: Becke exchange +  Perdew/Wang corr"));
	dftGridFreeFuncChoiceStrings.Add(_("B3LYP: hybridized HF/Becke/LYP using VWN"));
	dftGridFreeFuncChoiceStrings.Add(_("CAMB: CAMA exchange + Cambridge corr"));
	dftGridFreeFuncChoiceStrings.Add(_("XVWN: Xalpha exchange + VWN corr"));
	dftGridFreeFuncChoiceStrings.Add(_("XPWLOC: Xalpha exchange + Perdew/Wang corr"));
	dftGridFreeFuncChoiceStrings.Add(_("SVWN: Slater exchange + VWN5 corr"));
	dftGridFreeFuncChoiceStrings.Add(_("SPWLOC: Slater exchange + PWLOC corr"));
	dftGridFreeFuncChoiceStrings.Add(_("WIGNER: Wigner exchange + corr"));
	dftGridFreeFuncChoiceStrings.Add(_("WS: Wigner scaled exchange + corr"));
	dftGridFreeFuncChoiceStrings.Add(_("WIGEXP: Wigner exponential exchange + corr"));
	dftGridFreeFuncChoice = new wxUglyChoice( itemPanel109, ID_GRIDFREE_FUNCTIONAL_CHOICE, wxDefaultPosition, wxDefaultSize, dftGridFreeFuncChoiceStrings, 0 );
	dftGridFreeFuncChoice->SetStringSelection(_("Slater exchange"));
	funcSizer->Add(dftGridFreeFuncChoice, 0, wxALIGN_LEFT|wxALL, 5);

	listBook->AddPage(itemPanel109, _("DFT"));

	wxPanel* itemPanel118 = new wxPanel( listBook, ID_FMOPANE, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer119 = new wxBoxSizer(wxVERTICAL);
	itemPanel118->SetSizer(itemBoxSizer119);

	mFMOCheck = new wxCheckBox( itemPanel118, ID_FMOACTIVECHECK, _("FMO Active"), wxDefaultPosition, wxDefaultSize, 0 );
	mFMOCheck->SetValue(false);
	itemBoxSizer119->Add(mFMOCheck, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer121 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer119->Add(itemBoxSizer121, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText122 = new wxStaticText( itemPanel118, wxID_STATIC, _("Number of FMO fragments:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer121->Add(itemStaticText122, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mNumFragsEdit = new wxTextCtrl( itemPanel118, ID_FMOFRAGCOUNT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	itemBoxSizer121->Add(mNumFragsEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mFMOFragList = new wxGrid( itemPanel118, ID_FMOLIST, wxDefaultPosition, wxSize(220, 150), wxSUNKEN_BORDER|wxVSCROLL|wxALWAYS_SHOW_SB );
	mFMOFragList->SetDefaultColSize(100);
	mFMOFragList->SetDefaultRowSize(25);
	mFMOFragList->SetColLabelSize(25);
	mFMOFragList->SetRowLabelSize(80);
	mFMOFragList->CreateGrid(4, 1, wxGrid::wxGridSelectCells);
	itemBoxSizer119->Add(mFMOFragList, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	mFMOGenFragsButton = new wxButton( itemPanel118, ID_FMOFRAGBUTTON, _("Generate fragmentation"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer119->Add(mFMOGenFragsButton, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

	listBook->AddPage(itemPanel118, _("FMO"));

	wxPanel* itemPanel126 = new wxPanel( listBook, ID_IBMOGUESSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer127 = new wxBoxSizer(wxVERTICAL);
	itemPanel126->SetSizer(itemBoxSizer127);

	wxBoxSizer* itemBoxSizer128 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer127->Add(itemBoxSizer128, 0, wxALIGN_LEFT, 5);
	wxStaticText* itemStaticText129 = new wxStaticText( itemPanel126, wxID_STATIC, _("Initial Guess:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer128->Add(itemStaticText129, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	wxArrayString initGuessChoiceStrings;
	initGuessChoiceStrings.Add(_("Huckel"));
	initGuessChoiceStrings.Add(_("HCore"));
	initGuessChoiceStrings.Add(_("MO Read ($VEC)"));
	initGuessChoiceStrings.Add(_("MO Saved (DICTNRY)"));
	initGuessChoiceStrings.Add(_("Skip"));
	initGuessChoice = new wxUglyChoice( itemPanel126, ID_INITGUESS_CHOICE, wxDefaultPosition, wxSize(300, -1), initGuessChoiceStrings, 0 );
	initGuessChoice->SetStringSelection(_("Huckel"));
	itemBoxSizer128->Add(initGuessChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer131 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer127->Add(itemBoxSizer131, 0, wxALIGN_LEFT|wxALL, 5);
	wxStaticText* itemStaticText132 = new wxStaticText( itemPanel126, wxID_STATIC, _("$VEC source:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer131->Add(itemStaticText132, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxArrayString mMOSourceChoiceStrings;
	mMOSourceChoice = new wxUglyChoice( itemPanel126, ID_MOGUESS_VECSOURCE_CHOICE, wxDefaultPosition, wxSize(300, -1), mMOSourceChoiceStrings, 0 );
	if (InputBuilderWindow::ShowToolTips())
		mMOSourceChoice->SetToolTip(_("test tip"));
	itemBoxSizer131->Add(mMOSourceChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer134 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer127->Add(itemBoxSizer134, 0, wxALIGN_LEFT|wxALL, 5);
	wxStaticText* itemStaticText135 = new wxStaticText( itemPanel126, wxID_STATIC, _("Number of Orbitals to read in:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer134->Add(itemStaticText135, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mVecOrbCountText = new wxTextCtrl( itemPanel126, ID_MOGUESS_VECORBCOUNT_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mVecOrbCountText->SetToolTip(_("$GUESS:NORB - Enter an integer number of orbitals to read in from the $VEC group. The default is to read in the number of occupied orbitals for RHF, UHF, ROHF, and GVB, but you must provide a value for CI and MCSCF runs."));
	itemBoxSizer134->Add(mVecOrbCountText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	printInitGuessCheck = new wxCheckBox( itemPanel126, ID_PRINTINITGUESS_CHECKBOX, _("Print the initial guess"), wxDefaultPosition, wxDefaultSize, 0 );
	printInitGuessCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		printInitGuessCheck->SetToolTip(_("\"$GUESS:PRTMO - When checked GAMESS will write out the initial set of MO vectors to the log file."));
	itemBoxSizer127->Add(printInitGuessCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	rotOrbitalsCheck = new wxCheckBox( itemPanel126, ID_ROTORBITALS_CHECKBOX, _("Rotate alpha and beta orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
	rotOrbitalsCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		rotOrbitalsCheck->SetToolTip(_("$GUESS:MIX - Click to rotate the alpha and beta HOMO and LUMO orbitals such that the alpha and beta orbital spaces are in-equivalent. Only applicable to UHF runs."));
	rotOrbitalsCheck->Enable(false);
	itemBoxSizer127->Add(rotOrbitalsCheck, 0, wxALIGN_LEFT|wxALL, 5);

	listBook->AddPage(itemPanel126, _("MO Guess"));

	wxPanel* itemPanel139 = new wxPanel( listBook, ID_IBHESSOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer140 = new wxBoxSizer(wxVERTICAL);
	itemPanel139->SetSizer(itemBoxSizer140);

	wxBoxSizer* itemBoxSizer141 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer140->Add(itemBoxSizer141, 0, wxALIGN_LEFT, 5);
	wxArrayString mHessMethodRadioStrings;
	mHessMethodRadioStrings.Add(_("Analytic"));
	mHessMethodRadioStrings.Add(_("Numeric"));
	mHessMethodRadio = new wxRadioBox( itemPanel139, ID_HESSMETHOD_RADIO, _("Method"), wxDefaultPosition, wxDefaultSize, mHessMethodRadioStrings, 1, wxRA_SPECIFY_COLS );
	mHessMethodRadio->SetSelection(0);
	if (InputBuilderWindow::ShowToolTips())
		mHessMethodRadio->SetToolTip(_("$FORCE:METHOD - Choose Numerical to compute the 2nd derivative of the energy numerically by repeated energy and gradient evaluations. Generally you should use numeric hessians only when analytic versions are not available or are to expensive."));
	itemBoxSizer141->Add(mHessMethodRadio, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer143 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer141->Add(itemBoxSizer143, 0, wxALIGN_TOP|wxALL, 5);
	wxStaticText* itemStaticText144 = new wxStaticText( itemPanel139, wxID_STATIC, _("Displacement size (bohrs):"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer143->Add(itemStaticText144, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	mHESSDisplaceEdit = new wxTextCtrl( itemPanel139, ID_FORCEDISPLACE_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mHESSDisplaceEdit->SetToolTip(_("$FORCE:VIBSIZ Enter a value (in Bohrs) for the size of the displacement for numerical hessians. (default is 0.01)"));
	itemBoxSizer143->Add(mHESSDisplaceEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mHessDblDiffCheck = new wxCheckBox( itemPanel139, ID_HESSDOUBLEDIFFCHECK, _("Double differenced hessian"), wxDefaultPosition, wxDefaultSize, 0 );
	mHessDblDiffCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mHessDblDiffCheck->SetToolTip(_("$FORCE:NVIB - Click to double difference the numeric hessian. This will nearly double the number of energy and gradients needed, but will give better accuracy. Recomended for transition state searchs."));
	itemBoxSizer140->Add(mHessDblDiffCheck, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	mHessPurifyCheck = new wxCheckBox( itemPanel139, ID_HESSPURIFYCHECK, _("Purify hessian"), wxDefaultPosition, wxDefaultSize, 0 );
	mHessPurifyCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mHessPurifyCheck->SetToolTip(_("$FORCE:PURIFY - Click to transform the hessian to internal coordinates and then back to remove any rotations or translations."));
	itemBoxSizer140->Add(mHessPurifyCheck, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	mHessPrintIntFCCheck = new wxCheckBox( itemPanel139, ID_HESSPRTINTFC_CHECK, _("Print internal force constants"), wxDefaultPosition, wxDefaultSize, 0 );
	mHessPrintIntFCCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mHessPrintIntFCCheck->SetToolTip(_("$FORCE:PRTIFC - Click to print out the internal coordinate force constants."));
	itemBoxSizer140->Add(mHessPrintIntFCCheck, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	mHessVibAnlCheck = new wxCheckBox( itemPanel139, ID_HESSVIBANL_CHECK, _("Vibrational Analysis"), wxDefaultPosition, wxDefaultSize, 0 );
	mHessVibAnlCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mHessVibAnlCheck->SetToolTip(_("$FORCE:VIBANL - Click to perform vibrational analysis. This is very inexpensive once the force constant matrix is known."));
	itemBoxSizer140->Add(mHessVibAnlCheck, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer150 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer140->Add(itemBoxSizer150, 0, wxGROW, 5);
	wxStaticText* itemStaticText151 = new wxStaticText( itemPanel139, wxID_STATIC, _("Frequency scale factor:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer150->Add(itemStaticText151, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	mHessFreqScaleEdit = new wxTextCtrl( itemPanel139, ID_HESSFREQSCALE_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mHessFreqScaleEdit->SetToolTip(_("$FORCE:SCLFAC Enter a floating point value representing the frequency scaling factor to use for the thermochemical analysis. (unscaled frequencies are always printed)."));
	itemBoxSizer150->Add(mHessFreqScaleEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	listBook->AddPage(itemPanel139, _("Hess. Options"));

	wxPanel* itemPanel153 = new wxPanel( listBook, ID_IBMISCPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer154 = new wxBoxSizer(wxVERTICAL);
	itemPanel153->SetSizer(itemBoxSizer154);

	wxStaticText* itemStaticText155 = new wxStaticText( itemPanel153, wxID_STATIC, _("GAMESS interface to other codes."), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer154->Add(itemStaticText155, 0, wxALIGN_LEFT|wxALL, 5);

	wxStaticBox* itemStaticBoxSizer156Static = new wxStaticBox(itemPanel153, wxID_ANY, wxEmptyString);
	wxStaticBoxSizer* itemStaticBoxSizer156 = new wxStaticBoxSizer(itemStaticBoxSizer156Static, wxVERTICAL);
	itemBoxSizer154->Add(itemStaticBoxSizer156, 0, wxALIGN_LEFT|wxALL, 5);
	mMolPltCheck = new wxCheckBox( itemPanel153, ID_MOLPLT_CHECK, _("MolPlt"), wxDefaultPosition, wxDefaultSize, 0 );
	mMolPltCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mMolPltCheck->SetToolTip(_("$CONTRL:MOLPLT=.FALSE. - Click to punch an input deck (to the .dat file) for the MolPlt X-windows program. Note MacMolPlt can also read these files."));
	itemStaticBoxSizer156->Add(mMolPltCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	mPltOrbCheck = new wxCheckBox( itemPanel153, ID_PLTORB_CHECK, _("PltOrb"), wxDefaultPosition, wxDefaultSize, 0 );
	mPltOrbCheck->SetValue(false);
	itemStaticBoxSizer156->Add(mPltOrbCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	aimpacCheck = new wxCheckBox( itemPanel153, ID_AIMPAC_CHECK, _("Atoms In Molecules (AIMPAC)"), wxDefaultPosition, wxDefaultSize, 0 );
	aimpacCheck->SetValue(false);
	itemStaticBoxSizer156->Add(aimpacCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	rpacCheck = new wxCheckBox( itemPanel153, ID_RPAC_CHECK, _("RPAC"), wxDefaultPosition, wxDefaultSize, 0 );
	rpacCheck->SetValue(false);
	itemStaticBoxSizer156->Add(rpacCheck, 0, wxALIGN_LEFT|wxALL, 5);

	wxArrayString mMiscProgRadioStrings;
	mMiscProgRadioStrings.Add(_("None"));
	mMiscProgRadioStrings.Add(_("Hondo 8.2"));
	mMiscProgRadioStrings.Add(_("MELDF"));
	mMiscProgRadioStrings.Add(_("GAMESS (UK version)"));
	mMiscProgRadioStrings.Add(_("Gaussian 9x"));
	mMiscProgRadioStrings.Add(_("All"));
	mMiscProgRadio = new wxRadioBox( itemPanel153, ID_MISC_PROG_RADIO, _("These Force a Check Run Type"), wxDefaultPosition, wxDefaultSize, mMiscProgRadioStrings, 1, wxRA_SPECIFY_COLS );
	mMiscProgRadio->SetSelection(0);
	if (InputBuilderWindow::ShowToolTips())
		mMiscProgRadio->SetToolTip(_("$CONTRL:FRIEND - Click to punch input decks for the programs listed in this box."));
	itemBoxSizer154->Add(mMiscProgRadio, 0, wxALIGN_LEFT|wxALL, 5);

	listBook->AddPage(itemPanel153, _("Misc. Prefs"));

	wxPanel* itemPanel162 = new wxPanel( listBook, ID_IBMP2PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer163 = new wxBoxSizer(wxVERTICAL);
	itemPanel162->SetSizer(itemBoxSizer163);

	wxFlexGridSizer* itemFlexGridSizer164 = new wxFlexGridSizer(0, 2, 0, 0);
	itemBoxSizer163->Add(itemFlexGridSizer164, 0, wxGROW, 5);
	wxStaticText* itemStaticText165 = new wxStaticText( itemPanel162, wxID_STATIC, _("# of core electrons:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer164->Add(itemStaticText165, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	mMP2CoreEleEdit = new wxTextCtrl( itemPanel162, ID_MP2_CORE_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mMP2CoreEleEdit->SetToolTip(_("$MP2:NACORE - Enter an integer value for the number of electrons to leave out of the MP2 calculation."));
	itemFlexGridSizer164->Add(mMP2CoreEleEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText167 = new wxStaticText( itemPanel162, wxID_STATIC, _("Memory (in words):"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer164->Add(itemStaticText167, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	mMP2MemEdit = new wxTextCtrl( itemPanel162, ID_MP2MEM_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mMP2MemEdit->SetToolTip(_("$MP2:NWORD - Enter an integer number for the number of words of memory to use in the MP2 calculation. A value of 0 uses all available memory."));
	itemFlexGridSizer164->Add(mMP2MemEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText169 = new wxStaticText( itemPanel162, wxID_STATIC, _("Integral retention cutoff:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer164->Add(itemStaticText169, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	mMP2IntCutoffEdit = new wxTextCtrl( itemPanel162, ID_MP2INTCUTOFF_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	mMP2IntCutoffEdit->SetHelpText(_("$MP2:CUTOFF - Enter a floating point value for the integral retention cutoff. (default is 1.0e-9)"));
	if (InputBuilderWindow::ShowToolTips())
		mMP2IntCutoffEdit->SetToolTip(_("$MP2:CUTOFF - Enter a floating point value for the integral retention cutoff. (default is 1.0e-9)"));
	itemFlexGridSizer164->Add(mMP2IntCutoffEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mLMOMP2Check = new wxCheckBox( itemPanel162, ID_LMOMP2_CHECK, _("Use Localized Orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
	mLMOMP2Check->SetValue(false);
	itemBoxSizer163->Add(mLMOMP2Check, 0, wxGROW|wxALL, 5);

	mMP2PropCheck = new wxCheckBox( itemPanel162, ID_MP2PROP_CHECK, _("Compute MP2 properties"), wxDefaultPosition, wxDefaultSize, 0 );
	mMP2PropCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mMP2PropCheck->SetToolTip(_("$MP2:MP2PRP - a flag to turn on property computation for MP2 energy jobs. This is much more expensive than a plain energy. Properties are always computed during gradient runs where they are almost free."));
	itemBoxSizer163->Add(mMP2PropCheck, 0, wxALIGN_LEFT|wxALL, 5);

	wxArrayString mMP2TransRadioStrings;
	mMP2TransRadioStrings.Add(_("Segmented transformation"));
	mMP2TransRadioStrings.Add(_("Two phase bin sort"));
	mMP2TransRadio = new wxRadioBox( itemPanel162, ID_MP2TRANS_RADIO, _("Transformation method"), wxDefaultPosition, wxDefaultSize, mMP2TransRadioStrings, 1, wxRA_SPECIFY_COLS );
	mMP2TransRadio->SetSelection(0);
	if (InputBuilderWindow::ShowToolTips())
		mMP2TransRadio->SetToolTip(_("$MP2:Method - The Two phase bin sort is a more conventional method requiring more disk space, but less memory. It is required when using localized orbitals."));
	itemBoxSizer163->Add(mMP2TransRadio, 0, wxALIGN_LEFT|wxALL, 5);

	wxArrayString mMP2AOStorageRadioStrings;
	mMP2AOStorageRadioStrings.Add(_("Duplicated on each node"));
	mMP2AOStorageRadioStrings.Add(_("Distributed across all nodes"));
	mMP2AOStorageRadio = new wxRadioBox( itemPanel162, ID_MP2AOSTORAGE_RADIO, _("AO integral storage"), wxDefaultPosition, wxDefaultSize, mMP2AOStorageRadioStrings, 1, wxRA_SPECIFY_COLS );
	mMP2AOStorageRadio->SetSelection(0);
	if (InputBuilderWindow::ShowToolTips())
		mMP2AOStorageRadio->SetToolTip(_("$MP2:AOINTS -Choose the 2nd to distribute AO integrals over all nodes. This requires less disk space, but should only be used on parallel computers with high speed communications."));
	itemBoxSizer163->Add(mMP2AOStorageRadio, 0, wxALIGN_LEFT|wxALL, 5);

	listBook->AddPage(itemPanel162, _("MP2 Options"));

	wxPanel* itemPanel175 = new wxPanel( listBook, ID_IBSCBOPTSPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer176 = new wxBoxSizer(wxVERTICAL);
	itemPanel175->SetSizer(itemBoxSizer176);

	mDirectSCFCheck = new wxCheckBox( itemPanel175, ID_DIRECTSCF_CHECK, _("Direct SCF"), wxDefaultPosition, wxDefaultSize, 0 );
	mDirectSCFCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mDirectSCFCheck->SetToolTip(_("$SCF:DIRSCF - Direct SCF will be used to calculate the AO integrals. This will also activate direct MP2 if MP2 is requested."));
	itemBoxSizer176->Add(mDirectSCFCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	mFDiffCheck = new wxCheckBox( itemPanel175, ID_FDIFF_CHECK, _("Compute only change in Fock Matrix"), wxDefaultPosition, wxDefaultSize, 0 );
	mFDiffCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mFDiffCheck->SetToolTip(_("$SCF:FDIFF - When checked only the change since the previous iteration in the Fock matrices will be computed. This saves a large amount of CPU time, but may not fully converge cases with a lot of diffuse functions."));
	mFDiffCheck->Enable(false);
	itemBoxSizer176->Add(mFDiffCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	mUHFNOCheck = new wxCheckBox( itemPanel175, ID_UHF_NO_CHECK, _("Generate UHF Natural Orbitals"), wxDefaultPosition, wxDefaultSize, 0 );
	mUHFNOCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mUHFNOCheck->SetToolTip(_("$SCF:UHFNOS - Click to generate the natural orbitals of the UHF function. This option only applies to UHF wavefunctions."));
	mUHFNOCheck->Enable(false);
	itemBoxSizer176->Add(mUHFNOCheck, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

	wxBoxSizer* itemBoxSizer180 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer176->Add(itemBoxSizer180, 0, wxALIGN_LEFT, 5);
	wxStaticText* itemStaticText181 = new wxStaticText( itemPanel175, wxID_STATIC, _("SCF convergence criteria:  10^"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer180->Add(itemStaticText181, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	mSCFConvSpin = new wxSpinCtrl( itemPanel175, ID_SCF_CONV_SPIN, wxT("5"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 2147483647, 5 );
	if (InputBuilderWindow::ShowToolTips())
		mSCFConvSpin->SetToolTip(_("$SCF:NCONV - Enter an integer value to use as the power (in 10**(-n)) for the SCF density convergance criteria. Setting this to less than 5 (the default) will give questionable gradients."));
	itemBoxSizer180->Add(mSCFConvSpin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	listBook->AddPage(itemPanel175, _("SCF Options"));

	wxPanel* itemPanel183 = new wxPanel( listBook, ID_IBSTATPOINTPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer184 = new wxBoxSizer(wxVERTICAL);
	itemPanel183->SetSizer(itemBoxSizer184);

	wxFlexGridSizer* itemFlexGridSizer185 = new wxFlexGridSizer(0, 2, 0, 0);
	itemBoxSizer184->Add(itemFlexGridSizer185, 0, wxGROW, 5);
	wxStaticText* itemStaticText186 = new wxStaticText( itemPanel183, wxID_STATIC, _("Max. number of steps:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer185->Add(itemStaticText186, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	mMaxStepCountEdit = new wxTextCtrl( itemPanel183, ID_MAXSTEPCOUNT_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mMaxStepCountEdit->SetToolTip(_("$STATPT:NSTEP=An integer representing the maximum number of steps to take before cleanly stopping the run (after punching out all needed restart data). Most such runs can be restarted."));
	itemFlexGridSizer185->Add(mMaxStepCountEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText188 = new wxStaticText( itemPanel183, wxID_STATIC, _("Gradient convergance criteria:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer185->Add(itemStaticText188, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	mGradConvEdit = new wxTextCtrl( itemPanel183, ID_GRADCONVCRITERIA_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mGradConvEdit->SetToolTip(_("$STATPT:OPTTOL=a floating point value representing the convergance criteria for the gradient (in Hartree/Bohr)."));
	itemFlexGridSizer185->Add(mGradConvEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer190 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer184->Add(itemBoxSizer190, 0, wxGROW, 5);
	wxStaticText* itemStaticText191 = new wxStaticText( itemPanel183, wxID_STATIC, _("Optimization method:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer190->Add(itemStaticText191, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	wxArrayString mOptMethodChoiceStrings;
	mOptMethodChoiceStrings.Add(_("Newton-Raphson"));
	mOptMethodChoiceStrings.Add(_("Rational Function Optimization"));
	mOptMethodChoiceStrings.Add(_("Quadratic Approximation"));
	mOptMethodChoiceStrings.Add(_("Schlegel (quasi-NR)"));
	mOptMethodChoiceStrings.Add(_("Constrained Optimization"));
	mOptMethodChoice = new wxUglyChoice( itemPanel183, ID_OPTMETHOD_CHOICE, wxDefaultPosition, wxDefaultSize, mOptMethodChoiceStrings, 0 );
	if (InputBuilderWindow::ShowToolTips())
		mOptMethodChoice->SetToolTip(_("$STATPT:METHOD The algorithm used to drive the gradient to zero."));
	itemBoxSizer190->Add(mOptMethodChoice, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer193 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer184->Add(itemBoxSizer193, 0, wxGROW|wxALL, 5);
	wxFlexGridSizer* itemFlexGridSizer194 = new wxFlexGridSizer(0, 2, 0, 0);
	itemBoxSizer193->Add(itemFlexGridSizer194, 0, wxGROW, 5);
	wxStaticText* itemStaticText195 = new wxStaticText( itemPanel183, wxID_STATIC, _("Initial step size:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer194->Add(itemStaticText195, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	mInitStepSizeEdit = new wxTextCtrl( itemPanel183, ID_INITSTEPSIZE_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mInitStepSizeEdit->SetToolTip(_("$STATPT:DXMAX = a floating point value representing the initial size of the step"));
	itemFlexGridSizer194->Add(mInitStepSizeEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText197 = new wxStaticText( itemPanel183, wxID_STATIC, _("Min. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer194->Add(itemStaticText197, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP, 5);

	mMinStepSizeEdit = new wxTextCtrl( itemPanel183, ID_MINSTEPSIZE_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mMinStepSizeEdit->SetToolTip(_("$STATPT:TRMIN= the minimum size of the step."));
	itemFlexGridSizer194->Add(mMinStepSizeEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 5);

	wxStaticText* itemStaticText199 = new wxStaticText( itemPanel183, wxID_STATIC, _("Max. step size:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemFlexGridSizer194->Add(itemStaticText199, 0, wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	mMaxStepSizeEdit = new wxTextCtrl( itemPanel183, ID_MAXSTEPSIZE_CHECK, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mMaxStepSizeEdit->SetToolTip(_("$STATPT:TRMAX= the maximum size of the step."));
	itemFlexGridSizer194->Add(mMaxStepSizeEdit, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer201 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer193->Add(itemBoxSizer201, 0, wxGROW, 5);
	mUpdateStepSizeCheck = new wxCheckBox( itemPanel183, ID_UPDATESTEPSIZE_CHECK, _("Update Step Size"), wxDefaultPosition, wxDefaultSize, 0 );
	mUpdateStepSizeCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mUpdateStepSizeCheck->SetToolTip(_("$STATPT:TRUPD The step size will be allowed to change within the range given at the left."));
	itemBoxSizer201->Add(mUpdateStepSizeCheck, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 5);

	mIsStatPtCheck = new wxCheckBox( itemPanel183, ID_STATPT_CHECK, _("Stationary Pt."), wxDefaultPosition, wxDefaultSize, 0 );
	mIsStatPtCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mIsStatPtCheck->SetToolTip(_("$STATPT:STPT If checked the initial geometry will be considered a stationary point and the jump away from the initial geometry will be given by the step size below."));
	itemBoxSizer201->Add(mIsStatPtCheck, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer204 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer201->Add(itemBoxSizer204, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
	wxStaticText* itemStaticText205 = new wxStaticText( itemPanel183, wxID_STATIC, _("Jump Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer204->Add(itemStaticText205, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mStatPtJumpSizeEdit = new wxTextCtrl( itemPanel183, ID_STATPT_JUMP_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mStatPtJumpSizeEdit->SetToolTip(_("$STATPT:STSTEP= the size of the step taken away from stationary points.\n"));
	itemBoxSizer204->Add(mStatPtJumpSizeEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer207 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer184->Add(itemBoxSizer207, 0, wxGROW, 5);
	wxArrayString mInitHessRadioStrings;
	mInitHessRadioStrings.Add(_("Guess (+ definite)"));
	mInitHessRadioStrings.Add(_("Read (from $HESS)"));
	mInitHessRadioStrings.Add(_("Calculate"));
	mInitHessRadio = new wxRadioBox( itemPanel183, ID_INIT_HESS_RADIO, _("Initial Hessian"), wxDefaultPosition, wxDefaultSize, mInitHessRadioStrings, 1, wxRA_SPECIFY_COLS );
	mInitHessRadio->SetSelection(0);
	if (InputBuilderWindow::ShowToolTips())
		mInitHessRadio->SetToolTip(_("$STATPT:HESS=The source of the initial hessian matrix."));
	itemBoxSizer207->Add(mInitHessRadio, 0, wxGROW|wxALL, 5);

	wxBoxSizer* itemBoxSizer209 = new wxBoxSizer(wxVERTICAL);
	itemBoxSizer207->Add(itemBoxSizer209, 0, wxALIGN_TOP|wxALL, 0);
	wxBoxSizer* itemBoxSizer210 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer209->Add(itemBoxSizer210, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
	wxStaticText* itemStaticText211 = new wxStaticText( itemPanel183, wxID_STATIC, _("Recalc. Hess every"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer210->Add(itemStaticText211, 0, wxALIGN_TOP|wxLEFT|wxTOP|wxBOTTOM, 10);

	mHessRecalcEdit = new wxTextCtrl( itemPanel183, ID_HESS_RECALC_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mHessRecalcEdit->SetToolTip(_("$STATPT:IHREP=an integer representing how often the hessian will be fully recalculated. A value of zero is normal and means never fully recalculate the hessian. The hessian will be updated at every step."));
	itemBoxSizer210->Add(mHessRecalcEdit, 0, wxALIGN_TOP|wxALL, 5);

	wxStaticText* itemStaticText213 = new wxStaticText( itemPanel183, wxID_STATIC, _("steps."), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer210->Add(itemStaticText213, 0, wxALIGN_TOP|wxTOP|wxBOTTOM, 10);

	wxBoxSizer* itemBoxSizer214 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer209->Add(itemBoxSizer214, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 0);
	wxStaticText* itemStaticText215 = new wxStaticText( itemPanel183, wxID_STATIC, _("Follow which mode?"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer214->Add(itemStaticText215, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mStatPtModeEdit = new wxTextCtrl( itemPanel183, ID_STATPT_MODE_EDIT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
	if (InputBuilderWindow::ShowToolTips())
		mStatPtModeEdit->SetToolTip(_("$STATPT:IFOLOW Only applicable to SADPOINT runs and indicates which mode to maximize."));
	itemBoxSizer214->Add(mStatPtModeEdit, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mPrintOrbsCheck = new wxCheckBox( itemPanel183, ID_PRINTORBS_CHECK, _("Print Orbs at each iteration"), wxDefaultPosition, wxDefaultSize, 0 );
	mPrintOrbsCheck->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		mPrintOrbsCheck->SetToolTip(_("If checked the optimized orbitals will be output in the log file for each geometry. Otherwise the default is to output them for the 1st and last geometry."));
	itemBoxSizer184->Add(mPrintOrbsCheck, 0, wxGROW|wxALL, 5);

	wxCheckBox* itemCheckBox218 = new wxCheckBox( itemPanel183, ID_HESSEND_CHECK, _("Compute a hessian on the final optimized structure"), wxDefaultPosition, wxDefaultSize, 0 );
	itemCheckBox218->SetValue(false);
	if (InputBuilderWindow::ShowToolTips())
		itemCheckBox218->SetToolTip(_("Check to automatically compute a hessian for the optimized structure (HSSEND)"));
	itemBoxSizer184->Add(itemCheckBox218, 0, wxGROW|wxALL, 3);

	listBook->AddPage(itemPanel183, _("Stat. Point"));

	wxPanel* itemPanel219 = new wxPanel( listBook, ID_SUMMARYPANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* itemBoxSizer220 = new wxBoxSizer(wxVERTICAL);
	itemPanel219->SetSizer(itemBoxSizer220);

	wxBoxSizer* itemBoxSizer221 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer220->Add(itemBoxSizer221, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText222 = new wxStaticText( itemPanel219, wxID_STATIC, _("Title:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer221->Add(itemStaticText222, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mTitleText = new wxTextCtrl( itemPanel219, ID_SUMMARY_TITLE, wxEmptyString, wxDefaultPosition, wxSize(400, -1), 0 );
	mTitleText->Enable(false);
	itemBoxSizer221->Add(mTitleText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer224 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer220->Add(itemBoxSizer224, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText225 = new wxStaticText( itemPanel219, wxID_STATIC, _("Basis Set:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer224->Add(itemStaticText225, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mBasisSetText = new wxTextCtrl( itemPanel219, ID_SUMMARY_BASISSET, wxEmptyString, wxDefaultPosition, wxSize(400, -1), 0 );
	mBasisSetText->Enable(false);
	itemBoxSizer224->Add(mBasisSetText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer227 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer220->Add(itemBoxSizer227, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText228 = new wxStaticText( itemPanel219, wxID_STATIC, _("SCF Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer227->Add(itemStaticText228, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mSCFTypeText = new wxTextCtrl( itemPanel219, ID_SUMMARY_SCFTYPE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mSCFTypeText->Enable(false);
	itemBoxSizer227->Add(mSCFTypeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer230 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer220->Add(itemBoxSizer230, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText231 = new wxStaticText( itemPanel219, wxID_STATIC, _("Run Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer230->Add(itemStaticText231, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mRunTypeText = new wxTextCtrl( itemPanel219, ID_SUMMARY_RUNTYPE, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mRunTypeText->Enable(false);
	itemBoxSizer230->Add(mRunTypeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer233 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer220->Add(itemBoxSizer233, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText234 = new wxStaticText( itemPanel219, wxID_STATIC, _("Molecular Point Group:"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer233->Add(itemStaticText234, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mPointGroupText = new wxTextCtrl( itemPanel219, ID_SUMMARY_PG, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	mPointGroupText->Enable(false);
	itemBoxSizer233->Add(mPointGroupText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxBoxSizer* itemBoxSizer236 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer220->Add(itemBoxSizer236, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText* itemStaticText237 = new wxStaticText( itemPanel219, wxID_STATIC, _("Electron Correlation"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer236->Add(itemStaticText237, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	mElectronCorr = new wxTextCtrl( itemPanel219, ID_SUMMARY_ELEC, wxEmptyString, wxDefaultPosition, wxSize(200, -1), 0 );
	mElectronCorr->Enable(false);
	itemBoxSizer236->Add(mElectronCorr, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	listBook->AddPage(itemPanel219, _("Summary"));

	itemBoxSizer4->Add(listBook, 1, wxGROW|wxALL, 2);

	wxBoxSizer* itemBoxSizer239 = new wxBoxSizer(wxHORIZONTAL);
	itemBoxSizer4->Add(itemBoxSizer239, 0, wxGROW, 5);

	defaultsBtn = new wxButton( itemPanel3, ID_DEFAULTSBUTTON, _("Use Defaults"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer239->Add(defaultsBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	revertBtn = new wxButton( itemPanel3, ID_REVERTBUTTON, _("Revert"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer239->Add(revertBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	writeBtn = new wxButton( itemPanel3, ID_WRITEFILEBUTTON, _("Write File"), wxDefaultPosition, wxDefaultSize, 0 );
	itemBoxSizer239->Add(writeBtn, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxTOP|wxBOTTOM, 5);

	advanced_button = new wxButton( itemPanel3, ID_ADVANCEDBUTTON, _("Advanced"), wxDefaultPosition, wxDefaultSize, 0 );
	advanced_button->Show(false);
	itemBoxSizer239->Add(advanced_button, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	wxStdDialogButtonSizer* itemStdDialogButtonSizer244 = new wxStdDialogButtonSizer;

	itemBoxSizer239->Add(itemStdDialogButtonSizer244, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxButton* itemButton245 = new wxButton( itemPanel3, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStdDialogButtonSizer244->AddButton(itemButton245);

	wxButton* itemButton246 = new wxButton( itemPanel3, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	itemStdDialogButtonSizer244->AddButton(itemButton246);

	itemStdDialogButtonSizer244->Realize();

////@end InputBuilderWindow content construction

	wxMenuBar* menuBar = new wxMenuBar;
	wxMenu* itemMenu3 = new wxMenu;
	itemMenu3->Append(wxID_NEW, _("&New\tCtrl+N"), _T(""), wxITEM_NORMAL);
	itemMenu3->Append(wxID_OPEN, _("&Open ...\tCtrl+O"), _T(""), wxITEM_NORMAL);
	itemMenu3->Append(wxID_CLOSE, _("&Close\tCtrl+W"), _T(""), wxITEM_NORMAL);
	menuBar->Append(itemMenu3, _("File"));
	wxMenu* lEditMenu = new wxMenu;
	lEditMenu->Append(wxID_UNDO, _("&Undo\tCtrl+Z"), _T(""), wxITEM_NORMAL);
	lEditMenu->AppendSeparator();
	lEditMenu->Append(wxID_CUT, _("Cu&t\tCtrl+X"), _T(""), wxITEM_NORMAL);
	lEditMenu->Append(wxID_COPY, _("&Copy\tCtrl+C"), _T(""), wxITEM_NORMAL);
	lEditMenu->Append(wxID_PASTE, _("&Paste\tCtrl+V"), _T(""), wxITEM_NORMAL);
	lEditMenu->AppendSeparator();
	lEditMenu->Append(wxID_SELECTALL, _("&Select all\tCtrl+A"), _T(""), wxITEM_NORMAL);
	lEditMenu->AppendSeparator();
	lEditMenu->Append(wxID_PREFERENCES, wxT("Global Pr&eferences"));
	menuBar->Append(lEditMenu, _("Edit"));
	
	wxMenu * menuWindow = new wxMenu;
	menuWindow->Append(MMP_MOLECULEDISPLAYWINDOW, wxT("&Molecule Display"), _("The primary molecule display"));
	menuWindow->Append(MMP_BONDSWINDOW, wxT("&Bonds"), _("View/edit the bonding within the molecule"));
	menuWindow->Append(MMP_COORDSWINDOW, wxT("&Coordinates"), _("View/edit cartesian or internal coordinates"));
	menuWindow->Append(MMP_ENERGYPLOTWINDOW, wxT("&Energy Plot"), _("A plot of the energy for each geometry"));
	menuWindow->Append(MMP_FREQUENCIESWINDOW, wxT("&Frequencies"), _("Plot the vibrational frequencies"));
	menuWindow->Append(MMP_SURFACESWINDOW, wxT("&Surfaces"), _T("Add/Edit/Remove various surface types"));
	menuWindow->Append(MMP_ZMATRIXCALC, wxT("&Z-Matrix Calculator"), _("Compute bond lengths/angles or dihedrals between any set of atoms"));
	menuWindow->Append(MMP_LOCAL_PREFERENCES, wxT("Pr&eferences"), _T("Edit the preferences for this window"));
	menuBar->Append(menuWindow, wxT("&Subwindow"));
	
	wxMenu * menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT, wxT("&About MacMolPlt..."), _T("Learn about MacMolPlt"));
	menuHelp->Append(wxID_HELP, wxT("&MacMolPlt Manual..."), _T("Brief documentation"));
	menuBar->Append(menuHelp, wxT("&Help"));
	
	itemFrame1->SetMenuBar(menuBar);

    tabText[0] = wxT("Basis");
    tabText[1] = wxT("Control");
    tabText[2] = wxT("Data");
    tabText[3] = wxT("System");
	tabText[4] = wxT("GVB Parameters");
    tabText[5] = wxT("DFT");
	tabText[6] = wxT("FMO");
    tabText[7] = wxT("MO Guess");
    tabText[8] = wxT("Hess. Options");
    tabText[9] = wxT("Misc. Prefs");
    tabText[10] = wxT("MP2 Options");
    tabText[11] = wxT("SCF Options");
    tabText[12] = wxT("Stat. Point");
    tabText[13] = wxT("Summary");

    for(int i = NUM_PANES - 1; i >= 0; i--) {
        tab[i] = listBook->GetPage(i);
        visibleTab[i] = true;
    }
    
    listBook->SetSelection(1);
}

/*!
 * Should we show tooltips?
 */

bool InputBuilderWindow::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap InputBuilderWindow::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin InputBuilderWindow bitmap retrieval
	wxUnusedVar(name);
	return wxNullBitmap;
////@end InputBuilderWindow bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon InputBuilderWindow::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin InputBuilderWindow icon retrieval
	wxUnusedVar(name);
	return wxNullIcon;
////@end InputBuilderWindow icon retrieval
}
/*!
 * wxEVT_CLOSE_WINDOW event handler for ID_INPUTBUILDERWINDOW
 */

void InputBuilderWindow::OnCloseWindow( wxCloseEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    parent->CloseInputBuilderWindow();
}

bool InputBuilderWindow::ShowBasis() const {

	return TmpInputRec->Control->GetRunType() != G3MP2;

}

bool InputBuilderWindow::ShowStatPoint() const {

	return TmpInputRec->Control->GetRunType() == OptimizeRun ||
		   TmpInputRec->Control->GetRunType() == SadPointRun ||
		   TmpInputRec->Control->GetRunType() == G3MP2;

}

bool InputBuilderWindow::ShowMP2Opts() const {

	return TmpInputRec->Control->GetMPLevel() == 2 ||
		   TmpInputRec->Control->GetRunType() == G3MP2;

}

bool InputBuilderWindow::ShowHessOpts() const {

	return HessianGroup::IsHessianGroupNeeded(TmpInputRec);
}

bool InputBuilderWindow::ShowSCFOpts() const {

	return TmpInputRec->Control->GetSCFType() <= 4;

}

void InputBuilderWindow::SetupItems() {
    SetupBasisItems();
    SetupControlItems();
    SetupDataItems();
    SetupSystemItems();
    SetupDFTItems();
	SetupGVBItems();
	SetupFMOItems();
    SetupMOGuessItems();
    SetupHessOptsItems();
    SetupMiscPrefsItems();
    SetupMP2OptsItems();
    SetupSCFOptsItems();
    SetupStatPointItems();
    SetupSummaryItems();

	setPaneVisible(BASIS_PANE, ShowBasis());
	setPaneVisible(STATPOINT_PANE, ShowStatPoint());
	setPaneVisible(MP2OPTS_PANE, ShowMP2Opts());
	setPaneVisible(HESSOPTS_PANE, ShowHessOpts());

	Fit();
}

void InputBuilderWindow::SetupBasisItems() {
    short BasisValue = TmpInputRec->Basis->GetBasis();
	if (BasisValue == GAMESS_BS_BC) BasisValue = GAMESS_BS_DZV;
    short NumGauss = TmpInputRec->Basis->GetNumGauss();
    int itemValue = 0;
    int testValue = 0;
    
    // basisChoice
    itemValue = BasisValue;
    if(itemValue == 0) itemValue = 1;
    else if(itemValue == GAMESS_BS_STO) itemValue = NumGauss + 1;
    else if(itemValue == GAMESS_BS_N21) {
        itemValue += 4;
        if(NumGauss == 6) itemValue++;
    }
    else if(itemValue == GAMESS_BS_N31) itemValue = NumGauss + 6;
    else if(itemValue >= GAMESS_BS_N311) itemValue += 7;
    basisChoice->SetSelection(itemValue - 1);
    CheckBasisMenu();
    
    // ecpTypeChoice
    if(BasisValue == GAMESS_BS_SBKJC || BasisValue == GAMESS_BS_HW) {
        ecpTypeChoice->Enable(true);
        ecpTypeLabel->Enable(true);
        itemValue = TmpInputRec->Basis->GetECPPotential();
        if(itemValue == 0) {
            if(BasisValue == GAMESS_BS_SBKJC) itemValue = GAMESS_BS_ECP_SBKJC;
            else itemValue = GAMESS_BS_ECP_HW;
        }
        ecpTypeChoice->SetSelection(itemValue);
    }
    else {
        ecpTypeChoice->Enable(false);
        ecpTypeLabel->Enable(false);
    }
    
	spherHarmCheck->Enable(true);
	spherHarmCheck->SetValue(TmpInputRec->Control->UseSphericalHarmonics());
	if ((BasisValue < GAMESS_BS_CC_PVDZ)||(BasisValue ==  GAMESS_BS_SBKJC) ||
		(BasisValue == GAMESS_BS_HW)) {
		// polarChoice
		testValue = TmpInputRec->Basis->GetNumPFuncs() +
					TmpInputRec->Basis->GetNumDFuncs() +
					TmpInputRec->Basis->GetNumFFuncs();
		if(testValue) {
			polarChoice->Enable(true);
			polarLabel->Enable(true);
			itemValue = TmpInputRec->Basis->GetPolar();
			if(itemValue == 0) {
				if(BasisValue == GAMESS_BS_N311 || BasisValue == GAMESS_BS_MC) itemValue = GAMESS_BS_PopN311_Polar;
				else if(BasisValue == GAMESS_BS_DZV || BasisValue == GAMESS_BS_DH) itemValue = GAMESS_BS_Dunning_Polar;
				else if(BasisValue <= GAMESS_BS_MIDI) itemValue = GAMESS_BS_Huzinaga_Polar;
				else if(BasisValue == GAMESS_BS_TZV) itemValue = GAMESS_BS_Hondo7_Polar;
				else if(BasisValue == GAMESS_BS_N31) itemValue = GAMESS_BS_PopN31_Polar;
				else itemValue = GAMESS_BS_Common_Polar;
			}
			polarChoice->SetSelection(itemValue - 1);
		}
		else {
			polarChoice->Enable(false);
			polarLabel->Enable(false);
		}
		
		diffuseLCheck->Enable(true);
		diffuseSCheck->Enable(true);
		numDChoice->Enable(true);
		numFChoice->Enable(true);
		numPChoice->Enable(true);
		// diffuseLCheck
		diffuseLCheck->SetValue(TmpInputRec->Basis->GetDiffuseSP());
		
		// diffuseSCheck
		diffuseSCheck->SetValue(TmpInputRec->Basis->GetDiffuseS());
		
		// numDChoice
		numDChoice->SetSelection(TmpInputRec->Basis->GetNumDFuncs());
		
		// numFChoice
		numFChoice->SetSelection(TmpInputRec->Basis->GetNumFFuncs());
		
		// numPChoice
		numPChoice->SetSelection(TmpInputRec->Basis->GetNumPFuncs());
	} else {
		polarChoice->Enable(false);
		polarLabel->Enable(false);
		diffuseLCheck->Enable(false);
		diffuseSCheck->Enable(false);
		numDChoice->Enable(false);
		numFChoice->Enable(false);
		numPChoice->Enable(false);
		if ((BasisValue < GAMESS_BS_SBKJC)||((BasisValue >= GAMESS_BS_MCP_DZP)&&
											 (BasisValue <= GAMESS_BS_IMCP_NR2))) {
			spherHarmCheck->SetValue(true);
			spherHarmCheck->Enable(false);
		}
	}
}

void InputBuilderWindow::SetupControlItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
        
    short mp2 = TmpInputRec->Control->GetMPLevel();
    bool dft = TmpInputRec->Control->UseDFT();
    short ci = TmpInputRec->Control->GetCIType();
    CCRunType cc = TmpInputRec->Control->GetCCType();
    long scft = TmpInputRec->Control->GetSCFType();
    long NumElectrons = parent->GetData()->GetNumElectrons();
    
    int itemValue = 0;
    
	TypeOfRun runType = TmpInputRec->Control->GetRunType();
    if(runType == InvalidRunType) runType = Energy;
    runChoice->SetSelection(runType - 1);

	if (runType == G3MP2) {
		scfChoice->SetSelection(0);	//G3MP2 only supports RHF
		scfChoice->Enable(false);
	} else {
		scfChoice->Enable(true);
		if(scft == 0) {
			if(NumElectrons & 1) {
				scft = 3;
			} else {
				scft = 1;
			}
		}
		scfChoice->SetSelection(scft - 1);
	}
	
	setPaneVisible(GVB_PANE, (scft == GVB));

    // mp2Check
	if(ci || cc || dft || (mp2 < 0)) {
		mp2Check->SetValue(false);
		mp2Check->Enable(false);
	} else {
		mp2Check->Enable(true);
		if(mp2 < 0) mp2 = 0;
		if(mp2 == 2) mp2 = 1;
		mp2Check->SetValue(mp2);
	}
    
    // dftCheck
	if(ci || cc || (mp2 > 0) || (scft > 3) || (runType == G3MP2)) {
		dftCheck->SetValue(false);
		dftCheck->Enable(false);
		setPaneVisible(DFT_PANE, false);
	} else {
		dftCheck->Enable(true);
		dftCheck->SetValue(dft);
		if(dft) setPaneVisible(DFT_PANE, true);
		else setPaneVisible(DFT_PANE, false);
	}
    
    // ciChoice
	if((mp2 > 0) || dft || cc || scft == 2 || (runType == G3MP2)) {
		ciChoice->SetSelection(0);
		ciChoice->Enable(false);
		ciLabel->Enable(false);
	} else {
		ciChoice->Enable(true);
		ciLabel->Enable(true);
		ciChoice->SetSelection(ci);
	}
    
    // ccChoice - coupled cluster is only availble for RHF and ROHF. There is a much more limited
	//				set of options for ROHF, not sure how to handle that here.
	if((mp2 > 0) || dft || ci || ((scft > GAMESS_RHF)&&(scft != GAMESS_ROHF)) || (runType == G3MP2)) {
		ccChoice->SetSelection(0);
		ccChoice->Enable(false);
		ccLabel->Enable(false);
	} else {
		ccChoice->Enable(true);
		ccLabel->Enable(true);
		ccChoice->SetSelection(cc);
	}
    
    // scfIterText
    itemValue = TmpInputRec->Control->GetMaxIt();
    if(itemValue <= 0) itemValue = 30;
    scfIterText->SetValue(wxString::Format(wxT("%d"), itemValue));
    
	// exeChoice
	exeChoice->SetSelection(TmpInputRec->Control->GetExeType());
	if(TmpInputRec->Control->GetFriend() != Friend_None) {
	//The friend keyword choices force a check run type
		exeChoice->Enable(false);
		exeChoice->SetSelection(1);
	} else {
		exeChoice->Enable(true);
	}
    
    // mchargeText
	mchargeText->SetValue(wxString::Format(wxT("%hd"), TmpInputRec->Control->GetCharge()));
    
    // multText
	itemValue = TmpInputRec->Control->GetMultiplicity();
	if(itemValue <= 0) {
		if(NumElectrons & 1) itemValue = 2;
		else itemValue = 1;
	}
	multText->SetValue(wxString::Format(wxT("%d"), itemValue));
    
    // localChoice
	localChoice->SetSelection(TmpInputRec->Control->GetLocal());
}

void InputBuilderWindow::SetupDataItems() {
    int itemValue;
	
	//Title
	if (TmpInputRec->Data->GetTitle())
		titleText->SetValue(wxString(TmpInputRec->Data->GetTitle(), wxConvUTF8));
	else
		titleText->SetValue(wxString(_T("Title")));
    
    // coordTypeChoice
    itemValue = TmpInputRec->Data->GetCoordType();
    if(itemValue == 0) itemValue = 1;
    coordTypeChoice->SetSelection(itemValue - 1);
    
    // unitChoice
    unitChoice->SetSelection(TmpInputRec->Data->GetUnits());
	
	//# Z-Matrix vars
	wxString zvars;
	zvars.Printf(wxT("%d"), TmpInputRec->Data->GetNumZVar());
	zmatrixVarsText->SetValue(zvars);
    
		//Point Group
	itemValue = TmpInputRec->Data->GetPointGroup();
	if (itemValue == 0) itemValue = 1;
	pointGroupChoice->SetSelection(itemValue-1);

		//Point group order
	SetupPointGroupOrder();
    
    // symmetryCheck
    symmetryCheck->SetValue(TmpInputRec->Data->GetUseSym());
}

void InputBuilderWindow::SetupPointGroupOrder(void) {
	//Point group order - only applicable to certain point groups
	int itemValue = TmpInputRec->Data->GetPointGroup();
	if (itemValue == 0) itemValue = 1;
	if ((itemValue>3)&&(itemValue<11)) {
		paxisOrderChoice->Enable(true);
		paxisOrderText->Enable(true);
		itemValue = TmpInputRec->Data->GetPointGroupOrder()-1;
		if (itemValue <= 0) {
			itemValue = 1;
			TmpInputRec->Data->SetPointGroupOrder(2);
		}
		paxisOrderChoice->SetSelection(itemValue-1);
	} else  {
		paxisOrderChoice->Enable(false);
		paxisOrderText->Enable(false);
	}
}

void InputBuilderWindow::SetupSystemItems() {
    
    // timeLimitText
    wxString time;
	time.Printf(wxT("%.2f"), TmpInputRec->System->GetConvertedTime());
	timeLimitText->SetValue(time);
	
    // timeLimitUnitChoice
	timeLimitUnitChoice->SetSelection(TmpInputRec->System->GetTimeUnits() - 1);
	
    // memoryText
	wxString mem;
	mem.Printf(wxT("%.2f"), TmpInputRec->System->GetConvertedMem());
	memoryText->SetValue(mem);
	
    // memoryUnitChoice
	memoryUnitChoice->SetSelection(TmpInputRec->System->GetMemUnits() - 1);
	
    // memDDI edit
	mem.Printf(wxT("%.2f"), TmpInputRec->System->GetConvertedMemDDI());
	memddiText->SetValue(mem);
	
    // memDDIUnitChoice
	memddiUnitChoice->SetSelection(TmpInputRec->System->GetMemDDIUnits() - megaWordsUnit);

    // diagChoice
	diagChoice->SetSelection(TmpInputRec->System->GetDiag());
	
    // coreCheck
	coreCheck->SetValue(TmpInputRec->System->GetCoreFlag());
	
    // plBalanceRadio
	plBalanceRadio->SetSelection(TmpInputRec->System->GetBalanceType());
	
    // externDataRepCheck
	externDataRepCheck->SetValue(TmpInputRec->System->GetXDR());

    // Parall check
	mSGParallCheck->SetValue(TmpInputRec->System->GetParallel());
}

void InputBuilderWindow::SetupGVBItems() {
	wxString varText;
	// # core Orbs edit
	varText.Printf(wxT("%ld"), TmpInputRec->SCF->GetGVBNumCoreOrbs());
	mGVB_NCOEdit->SetValue(varText);
	// # pairs edit
	varText.Printf(wxT("%ld"), TmpInputRec->SCF->GetGVBNumPairs());
	mGVB_PairsEdit->SetValue(varText);
	// # open shells edit
	varText.Printf(wxT("%ld"), TmpInputRec->SCF->GetGVBNumOpenShells());
	mGVB_NSETOEdit->SetValue(varText);
	if (TmpInputRec->SCF->GetGVBNumOpenShells() > 0) {
		mGVB_NOEdit->Show(true);
		mGVB_NOStatic->Show(true);
		std::vector<long> t = TmpInputRec->SCF->GetGVBOpenShellDeg();
		int num = t.size();
		if (TmpInputRec->SCF->GetGVBNumOpenShells() > num) {	//This shouldn't happen?
			for (int i=num; i<TmpInputRec->SCF->GetGVBNumOpenShells(); i++)
				t.push_back(0);
		} else if (TmpInputRec->SCF->GetGVBNumOpenShells() > num)
			num =TmpInputRec->SCF->GetGVBNumOpenShells();
		std::ostringstream degString;
		for (int i=0; i<num; i++) {
			if (i>0) degString << " ";
			degString << t[i];
		}
		mGVB_NOEdit->SetValue(wxString(degString.str().c_str(), wxConvUTF8));
	} else {
		mGVB_NOEdit->Show(false);
		mGVB_NOStatic->Show(false);
	}
}

void InputBuilderWindow::SetupDFTItems() {
	// dftMethodChoice
    bool grid = TmpInputRec->DFT.MethodGrid();
    dftMethodChoice->SetSelection(grid ? 0 : 1);
    
    // dftGridFuncChoice and dftGridFreeFuncChoice
    if(grid) {
        dftGridFreeFuncChoice->Show(false);
        dftGridFuncChoice->Show(true);
        dftGridFuncChoice->SetSelection(TmpInputRec->DFT.GetFunctional() - 1);
        funcSizer->Layout();
    } else {
        dftGridFuncChoice->Show(false);
        dftGridFreeFuncChoice->Show(true);
        dftGridFreeFuncChoice->SetSelection(TmpInputRec->DFT.GetFunctional() - 1);
        funcSizer->Layout();
    }
}

void InputBuilderWindow::SetupFMOItems() {
	mFMOFragList->HideCellEditControl();
	mFMOFragList->ClearSelection();
	mFMOFragList->SetColLabelValue(0, _T("Fragment Id"));
	//Clear off the old rows
	if (mFMOFragList->GetNumberRows()>0)
		mFMOFragList->DeleteRows(0, mFMOFragList->GetNumberRows(), true);
	
	if (TmpInputRec->FMO.IsFMOActive()) {
		mFMOCheck->SetValue(true);
		mFMOGenFragsButton->Enable(true);
		mFMOFragList->Enable(true);
		mNumFragsEdit->Enable(true);
		wxString nf;
		nf.Printf(wxT("%ld"), TmpInputRec->FMO.GetNumberFragments());
		mNumFragsEdit->SetValue(nf);
		MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
		MoleculeData * MainData = parent->GetData();
		Frame * cFrame = MainData->GetCurrentFramePtr();
		mFMOFragList->InsertRows(0, cFrame->GetNumAtoms(), true);
		
		//Test to see if the fragment list is initialized, if not init it
		if (FMOFragmentIds.size() < cFrame->GetNumAtoms()) {
			FMOFragmentIds.reserve(cFrame->GetNumAtoms());
			while (FMOFragmentIds.size() < cFrame->GetNumAtoms())
				FMOFragmentIds.push_back(1);
		}

		for (long i=0; i<cFrame->GetNumAtoms(); i++) {
			wxString foo;
			foo.Printf(wxT("%ld"), FMOFragmentIds[i]);
			mFMOFragList->SetCellValue(i, 0, foo);
		}
	} else {
		mFMOCheck->SetValue(false);
		mFMOGenFragsButton->Enable(false);
		mFMOFragList->Enable(false);
		mNumFragsEdit->Enable(false);
	}
}

void InputBuilderWindow::SetupMOGuessItems() {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();

	long GuessType = TmpInputRec->Guess->GetGuess();
	MoleculeData * MainData = parent->GetData();
	Frame * lFrame = MainData->GetCurrentFramePtr();
	const std::vector<OrbitalRec *> * Orbs = lFrame->GetOrbitalSetVector();
	if (GuessType == 0) {
		GuessType = 1;
		if (Orbs->size() > 0) {GuessType = 3; TmpInputRec->Guess->SetGuess(GuessType);}
	}
	initGuessChoice->SetSelection(GuessType-1);
	
	if (GuessType == 3) { //only applicable to MO read type of guess
		mMOSourceChoice->Enable(true);
		mMOSourceChoice->Clear();
		mVecOrbCountText->Enable(true);
		mMOSourceChoice->Append(wxString(_("By Hand Later")));
		short tempVec = TmpInputRec->Guess->GetVecSource();
		if (Orbs->size() > 0) {
			if ((tempVec<=0)||(tempVec > (Orbs->size() + 2))) tempVec = 2;
			std::vector<OrbitalRec *>::const_iterator OrbSet = Orbs->begin();
			while (OrbSet != Orbs->end()) {	//Build the popup menu
				mMOSourceChoice->Append(wxString((*OrbSet)->getOrbitalTypeText(), wxConvUTF8));
				OrbSet++;
			}
		} else {	//No orbs so the only choice is by hand later
			tempVec = 1;
		}
		mMOSourceChoice->SetSelection(tempVec-1);

		int numOrbs = TmpInputRec->Guess->GetNumOrbs();
		if (!numOrbs) {	//Obtain the # of orbs if from a local set
			if (Orbs->size() > 0) {
				if (tempVec > 1) {
					OrbitalRec * OrbSet = (*Orbs)[tempVec-2];
					numOrbs = OrbSet->getNumOccupiedAlphaOrbitals();
					if (numOrbs <= 0) numOrbs = OrbSet->getNumAlphaOrbitals();
				}
			}
		}
		wxString noText;
		noText.Printf(wxT("%d"), numOrbs);
		mVecOrbCountText->SetValue(noText);
	} else {
		mMOSourceChoice->Enable(false);
		mVecOrbCountText->Enable(false);
	}
	
	printInitGuessCheck->SetValue(TmpInputRec->Guess->GetPrintMO());
	
	rotOrbitalsCheck->SetValue(TmpInputRec->Guess->GetMix());
	rotOrbitalsCheck->Enable(((TmpInputRec->Control->GetSCFType() == 2)&&
							  ((TmpInputRec->Control->GetMultiplicity() == 1)||
							   (TmpInputRec->Control->GetMultiplicity() == 0))));
}

void InputBuilderWindow::SetupHessOptsItems() {
	if (!TmpInputRec->Hessian) TmpInputRec->Hessian = new HessianGroup;

	bool AnalyticPoss = TmpInputRec->Hessian->IsAnalyticHessianPossible(TmpInputRec);
	
	bool AnalyticSelected = TmpInputRec->Hessian->GetAnalyticMethod() && AnalyticPoss;
	mHessMethodRadio->Enable(AnalyticPoss);
	if (AnalyticSelected) mHessMethodRadio->SetSelection(0);
	else mHessMethodRadio->SetSelection(1);
	//Double difference
	mHessDblDiffCheck->Enable(!AnalyticSelected);
	mHessDblDiffCheck->SetValue(TmpInputRec->Hessian->GetDoubleDiff());
	//Purify
	mHessPurifyCheck->Enable((TmpInputRec->Data->GetNumZVar() > 0));
	mHessPurifyCheck->SetValue(TmpInputRec->Hessian->GetPurify());
	//Print FC
	mHessPrintIntFCCheck->Enable((TmpInputRec->Data->GetNumZVar() > 0));
	mHessPrintIntFCCheck->SetValue(TmpInputRec->Hessian->GetPrintFC());
	//vibartional analysis
	mHessVibAnlCheck->SetValue(TmpInputRec->Hessian->GetVibAnalysis());
	//frequency scale factor
	mHessFreqScaleEdit->Enable(TmpInputRec->Hessian->GetVibAnalysis());
	if (TmpInputRec->Hessian->GetVibAnalysis()) {
		wxString temp;
		temp.Printf(wxT("%f"), TmpInputRec->Hessian->GetFreqScale());
		mHessFreqScaleEdit->SetValue(temp);
	}
	//displacement
	mHESSDisplaceEdit->Enable(!AnalyticSelected);
	if (!AnalyticSelected) {
		wxString temp;
		temp.Printf(wxT("%f"), TmpInputRec->Hessian->GetDisplacementSize());
		mHESSDisplaceEdit->SetValue(temp);
	}
}

void InputBuilderWindow::SetupMiscPrefsItems() {
	mMolPltCheck->SetValue(TmpInputRec->Control->GetMolPlot());
	mPltOrbCheck->SetValue(TmpInputRec->Control->GetPlotOrb());
	
	//aimpac and rpac require normal run type, but the others below require a check run
	if ((TmpInputRec->Control->GetExeType()!=1)&&(TmpInputRec->Control->GetFriend()==Friend_None)) {
		aimpacCheck->Enable(true);
		aimpacCheck->SetValue(TmpInputRec->Control->GetAIMPAC());
		
		rpacCheck->Enable(true);
		rpacCheck->SetValue(TmpInputRec->Control->GetRPAC());
	} else {
		aimpacCheck->Enable(false);
		aimpacCheck->SetValue(false);
		
		rpacCheck->Enable(false);
		rpacCheck->SetValue(false);
	}
	
	mMiscProgRadio->SetSelection(TmpInputRec->Control->GetFriend());
}

void InputBuilderWindow::SetupMP2OptsItems() {
	if (!TmpInputRec->MP2) TmpInputRec->MP2 = new MP2Group;
	
	wxString temp;
	//# core electrons
	if (TmpInputRec->MP2->GetNumCoreElectrons()>=0) {
		temp.Printf(wxT("%ld"), TmpInputRec->MP2->GetNumCoreElectrons());
		mMP2CoreEleEdit->SetValue(temp);
	}
	//memory
	long memVal = TmpInputRec->MP2->GetMemory();
	if (memVal == 0)
		temp.Printf(wxT("All"));
	else
		temp.Printf(wxT("%d"), memVal);
	mMP2MemEdit->SetValue(temp);
	//integral cutoff
	double	tempf = TmpInputRec->MP2->GetIntCutoff();
	if (tempf == 0.0) tempf = 1.0e-9;
	temp.Printf(wxT("%g"), tempf);
	mMP2IntCutoffEdit->SetValue(temp);
	//LMOMP2
	mLMOMP2Check->Enable((1>=TmpInputRec->Control->GetSCFType()));
	mLMOMP2Check->SetValue(TmpInputRec->MP2->GetLMOMP2());
	//MP2 Prop
	mMP2PropCheck->Enable((Energy==TmpInputRec->Control->GetRunType()));
	mMP2PropCheck->SetValue(TmpInputRec->MP2->GetMP2Prop());
	//Trans. method
	if (TmpInputRec->MP2->GetLMOMP2()) {	//LMOMP2 forces method = 3
		mMP2TransRadio->Enable(false);
		mMP2TransRadio->SetSelection(1);
	} else {
		mMP2TransRadio->Enable(true);
		short itemValue = TmpInputRec->MP2->GetMethod();
		if (itemValue <= 2) itemValue = 0;
		else itemValue = 1;
		mMP2TransRadio->SetSelection(itemValue);
	}
	//AO dist.
	short itemValue = TmpInputRec->MP2->GetAOIntMethod();
	if ((itemValue == 0)||(itemValue == 2)) itemValue = 1;
	else itemValue = 0;
	mMP2AOStorageRadio->SetSelection(itemValue);
}

void InputBuilderWindow::SetupSCFOptsItems() {
	if (!TmpInputRec->SCF) TmpInputRec->SCF = new SCFGroup;
	mDirectSCFCheck->SetValue(TmpInputRec->SCF->GetDirectSCF());
	if ((TmpInputRec->SCF->GetDirectSCF() && (3>=TmpInputRec->Control->GetSCFType()))) {
		mFDiffCheck->SetValue(TmpInputRec->SCF->GetFockDiff());
		mFDiffCheck->Enable(true);
	} else {
		mFDiffCheck->SetValue(false);
		mFDiffCheck->Enable(false);
	}
	if (TmpInputRec->Control->GetSCFType() == GAMESS_UHF) {
		mUHFNOCheck->SetValue(TmpInputRec->SCF->GetUHFNO());
		mUHFNOCheck->Enable(true);
	} else {
		mUHFNOCheck->SetValue(false);
		mUHFNOCheck->Enable(false);
	}

	int conv = TmpInputRec->SCF->GetConvergance();
	if (conv == 0) conv = 5;
	mSCFConvSpin->SetValue(conv);
}

void InputBuilderWindow::SetupStatPointItems() {
	short RunType = TmpInputRec->Control->GetRunType();

	wxString temp;
	//max steps
	temp.Printf(wxT("%d"), TmpInputRec->StatPt->GetMaxSteps());
	mMaxStepCountEdit->SetValue(temp);
	
	//conv. criteria
	temp.Printf(wxT("%g"), TmpInputRec->StatPt->GetOptConvergance());
	mGradConvEdit->SetValue(temp);
	
	//method
	short OptMethod = TmpInputRec->StatPt->GetMethod();
	mOptMethodChoice->SetSelection(OptMethod-1);
	
	//init step size
	if (OptMethod == 1) {
		mInitStepSizeEdit->Enable(false);
	} else {
		mInitStepSizeEdit->Enable(true);
		float tempf = TmpInputRec->StatPt->GetInitRadius();
		if (tempf == 0.0) {
			if (OptMethod == 5) tempf = 0.1f;
			else if (RunType == 6) tempf = 0.2f;
			else tempf = 0.3f;
		}
		temp.Printf(wxT("%g"), tempf);
		mInitStepSizeEdit->SetValue(temp);
	}
	//Min step size
	temp.Printf(wxT("%g"), TmpInputRec->StatPt->GetMinRadius());
	mMinStepSizeEdit->SetValue(temp);
	//max step size
	float tempf = TmpInputRec->StatPt->GetMaxRadius();
	if (tempf == 0.0) {
		if (RunType == 6) tempf = 0.3f;
		else tempf = 0.5;
	}
	temp.Printf(wxT("%g"), tempf);
	mMaxStepSizeEdit->SetValue(temp);
	
	//hess recalc interval
	temp.Printf(wxT("%d"), TmpInputRec->StatPt->GetHessRecalcInterval());
	mHessRecalcEdit->SetValue(temp);
	
	//Init Hess
	short HessType = TmpInputRec->StatPt->GetHessMethod();
	if (!HessType) {
		if ((RunType == OptimizeRun)||(RunType == G3MP2)) HessType = 1;
		else HessType = 2;
	}
	mInitHessRadio->SetSelection(HessType-1);
	//step size update radius
	mUpdateStepSizeCheck->SetValue(TmpInputRec->StatPt->GetRadiusUpdate());
	//Statpt check
	mIsStatPtCheck->SetValue(TmpInputRec->StatPt->GetStatPoint());
	//Orbs print check
	mPrintOrbsCheck->SetValue(TmpInputRec->StatPt->AlwaysPrintOrbs());

	//statpt jump off size
	if (TmpInputRec->StatPt->GetStatPoint()) {
		mStatPtJumpSizeEdit->Enable(true);
		temp.Printf(wxT("%g"), TmpInputRec->StatPt->GetStatJump());
		mStatPtJumpSizeEdit->SetValue(temp);
	} else {
		mStatPtJumpSizeEdit->Enable(false);
	}
	//mode to follow
	if (RunType == 6) {
		mStatPtModeEdit->Enable(true);
		temp.Printf(wxT("%ld"), TmpInputRec->StatPt->GetModeFollow());
		mStatPtModeEdit->SetValue(temp);
	} else
		mStatPtModeEdit->Enable(false);
}

void InputBuilderWindow::SetupSummaryItems() {
	
	if (TmpInputRec->Data->GetTitle()) {
		mTitleText->SetValue(wxString(TmpInputRec->Data->GetTitle(), wxConvUTF8));
	} else mTitleText->Clear();
	if (TmpInputRec->Basis->GetBasis()) {
		wxString temp(TmpInputRec->Basis->GetBasisText(), wxConvUTF8);
		wxString t2(TmpInputRec->Basis->GetBasisText(), wxConvUTF8);
		if ((TmpInputRec->Basis->GetBasis()>3)&&(TmpInputRec->Basis->GetBasis()<6)) {
			temp.Printf(wxT("%d-%s"), TmpInputRec->Basis->GetNumGauss(), t2.c_str());
		} else if (TmpInputRec->Basis->GetBasis()==3) {
			temp.Printf(wxT("%s-%dG"), t2.c_str(), TmpInputRec->Basis->GetNumGauss());
		}
		if (TmpInputRec->Basis->GetNumDFuncs()) {
			wxString t;
			t.Printf(wxT(", # D funcs = %d"), TmpInputRec->Basis->GetNumDFuncs());
			temp.Append(t);
		}
		if (TmpInputRec->Basis->GetNumPFuncs()) {
			wxString t;
			t.Printf(wxT(", # P funcs = %d"), TmpInputRec->Basis->GetNumPFuncs());
			temp.Append(t);
		}
		if (TmpInputRec->Basis->GetNumFFuncs()) {
			wxString t;
			t.Printf(wxT(", # F funcs = %d"), TmpInputRec->Basis->GetNumFFuncs());
			temp.Append(t);
		}
		if (TmpInputRec->Basis->GetPolar() > GAMESS_BS_No_Polarization) {
			wxString t;
			wxString t1(TmpInputRec->Basis->GetPolarText(), wxConvUTF8);
			t.Printf(wxT(", Polar = "));
			temp.Append(t);
			temp.Append(t1);
		}
		mBasisSetText->SetValue(temp);
	} else mBasisSetText->Clear();
	
    wxString temp(ControlGroup::GetGAMESSRunText(TmpInputRec->Control->GetRunType()), wxConvUTF8);
	mRunTypeText->SetValue(temp);

	if (TmpInputRec->Control->GetSCFType()) {
		mSCFTypeText->SetValue(wxString(TmpInputRec->Control->GetSCFTypeText(), wxConvUTF8));
	} else mSCFTypeText->Clear();
	{
		wxString pg;
		const char * pgt = TmpInputRec->Data->GetPointGroupText();
		if (TmpInputRec->Data->GetPointGroupOrder() > 0) {
			int i=0;
			while (pgt[i]) {
				if (pgt[i] == 'N') {
					wxString temp;
					temp.Printf(wxT("%d"), TmpInputRec->Data->GetPointGroupOrder());
					pg.Append(temp);
				} else {
					pg.Append((char)(pgt[i]), 1);
				}
				i++;
			}
		} else pg = wxString(pgt, wxConvUTF8);
		mPointGroupText->SetValue(pg);
	}
	wxString eclevel;
	if (TmpInputRec->Control->GetMPLevel() == 2)
		eclevel.Printf(wxT("MP2"));
	else if (TmpInputRec->Control->GetCCType()) {
		wxString t(TmpInputRec->Control->GetGAMESSCCType(TmpInputRec->Control->GetCCType()), wxConvUTF8);
		eclevel = t;
	} else if (TmpInputRec->Control->GetCIType()) {
		wxString t(TmpInputRec->Control->GetCIType(TmpInputRec->Control->GetCIType()), wxConvUTF8);
		eclevel = t;
	}
	mElectronCorr->SetValue(eclevel);
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_BASIS_CHOICE
 */

void InputBuilderWindow::OnBasisChoiceSelected( wxCommandEvent& event )
{
    short BasisValue = 0;
    short NumGauss   = 0;
    int   itemValue  = basisChoice->GetSelection();
    
    if(itemValue < 2) {
        BasisValue = itemValue + 1;
    }
    else if(itemValue < 7) {
        BasisValue = GAMESS_BS_STO;
        NumGauss = itemValue;
    }
    else if(itemValue < 9) {
        BasisValue = GAMESS_BS_N21;
        NumGauss = ((itemValue==7) ? 3 : 6);
    }
    else if(itemValue < 12) {
        BasisValue = GAMESS_BS_N31;
        NumGauss = itemValue - 5;
    }
    else if(itemValue < 18) {
        BasisValue = itemValue - 6;
        if(itemValue == 12) NumGauss = 6;
    } else {
        BasisValue = itemValue - 6;
    }
    
    TmpInputRec->Basis->SetBasis(BasisValue);
    TmpInputRec->Basis->SetNumGauss(NumGauss);
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ECPT_CHOICE
 */

void InputBuilderWindow::OnEcptChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetECPPotential(ecpTypeChoice->GetSelection());
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_POLAR_CHOICE
 */

void InputBuilderWindow::OnPolarChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetPolar((GAMESS_BS_Polarization)(polarChoice->GetSelection() + 1));
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DIFFL_CHECKBOX
 */

void InputBuilderWindow::OnDifflCheckboxClick( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetDiffuseSP(event.IsChecked());
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DIFFS_CHECKBOX
 */

void InputBuilderWindow::OnDiffsCheckboxClick( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetDiffuseS(event.IsChecked());
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_NUMD_CHOICE
 */

void InputBuilderWindow::OnNumdChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetNumDFuncs(numDChoice->GetSelection());
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_NUMF_CHOICE
 */

void InputBuilderWindow::OnNumfChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetNumFFuncs(numFChoice->GetSelection());
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_NUMP_CHOICE
 */

void InputBuilderWindow::OnNumpChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Basis->SetNumPFuncs(numPChoice->GetSelection());
    SetupItems();
}
/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_RUN_CHOICE
 */

void InputBuilderWindow::OnRunChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetRunType((TypeOfRun)(runChoice->GetSelection() + 1));
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_SCF_CHOICE
 */

void InputBuilderWindow::OnScfChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetSCFType((GAMESS_SCFType)(scfChoice->GetSelection() + 1));
	setPaneVisible(SCFOPTS_PANE, ShowSCFOpts());
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_LOCAL_CHOICE
 */

void InputBuilderWindow::OnLocalChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetLocal((GAMESS_Localization)(localChoice->GetSelection()));
    SetupItems();
}
/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_MP2_CHECKBOX
 */

void InputBuilderWindow::OnMp2CheckboxClick( wxCommandEvent& event )
{
    if(event.IsChecked()) {
        TmpInputRec->Control->SetMPLevel(2);
    }
    else {
        TmpInputRec->Control->SetMPLevel(0);
    }
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DFT_CHECKBOX
 */

void InputBuilderWindow::OnDftCheckboxClick( wxCommandEvent& event )
{
    TmpInputRec->Control->UseDFT(event.IsChecked());
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CI_CHOICE
 */

void InputBuilderWindow::OnCiChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetCIType((CIRunType)(ciChoice->GetSelection()));
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CC_CHOICE
 */

void InputBuilderWindow::OnCcChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetCCType((CCRunType)(ccChoice->GetSelection()));
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_EXE_CHOICE
 */

void InputBuilderWindow::OnExeChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Control->SetExeType(exeChoice->GetSelection());
	SetupMiscPrefsItems();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void InputBuilderWindow::OnCancelClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    parent->CloseInputBuilderWindow();
}

void InputBuilderWindow::SaveData(void) const {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    parent->GetData()->SetInputData(TmpInputRec);
	if (TmpInputRec->FMO.IsFMOActive()) {
		if (FMOFragmentIds.size() > 0) {
			MoleculeData * MainData = ((MolDisplayWin *)parent)->GetData();
			Frame * cFrame = MainData->GetCurrentFramePtr();
			for (long i=0; i<cFrame->GetNumAtoms(); i++) {
				MainData->SetFMOFragmentId(i, FMOFragmentIds[i]);
			}
		}
	}
}
void InputBuilderWindow::FrameChanged(void) {
	SetupFMOItems();
}
/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void InputBuilderWindow::OnOkClick( wxCommandEvent& event )
{
	SaveData();
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	parent->ResetModel(false);
    parent->CloseInputBuilderWindow();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_DEFAULTSBUTTON
 */

void InputBuilderWindow::OnDefaultsbuttonClick( wxCommandEvent& event )
{
    switch(getCurrentPane()) {
        case BASIS_PANE:
            TmpInputRec->Basis->InitData();
            break;
        case CONTROL_PANE:
            TmpInputRec->Control->InitControlPaneData();
            break;
        case DATA_PANE:
            TmpInputRec->Data->InitData();
            break;
        case SYSTEM_PANE:
            TmpInputRec->System->InitData();
            break;
        case DFT_PANE:
            TmpInputRec->DFT.InitData();
            break;
		case FMO_Pane:
			TmpInputRec->FMO.InitData();
			FMOFragmentIds.clear();
			break;
        case MOGUESS_PANE:
            TmpInputRec->Guess->InitData();
            break;
        case HESSOPTS_PANE:
            TmpInputRec->Hessian->InitData();
            break;
        case MISCPREFS_PANE:
            TmpInputRec->Control->InitProgPaneData();
            break;
        case MP2OPTS_PANE:
            TmpInputRec->MP2->InitData();
            break;
        case SCFOPTS_PANE:
            TmpInputRec->SCF->InitData();
            break;
        case STATPOINT_PANE:
            TmpInputRec->StatPt->InitData();
            break;
        default:
            // NO-OP
            break;
    }
    
    SetupItems();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_REVERTBUTTON
 */

void InputBuilderWindow::OnRevertbuttonClick( wxCommandEvent& event )
{
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	MoleculeData * MainData = ((MolDisplayWin *)parent)->GetData();
	InputData *OldData = ((MolDisplayWin *)parent)->GetData()->GetInputData();
    
    switch(getCurrentPane()) {
        case BASIS_PANE:
            *(TmpInputRec->Basis) = *(OldData->Basis);
            break;
        case CONTROL_PANE:
            TmpInputRec->Control->RevertControlPane(OldData->Control);
            break;
        case DATA_PANE:
            if(TmpInputRec->Data) delete TmpInputRec->Data;
            TmpInputRec->Data = new DataGroup(OldData->Data);
            break;
        case SYSTEM_PANE:
            *(TmpInputRec->System) = *(OldData->System);
            break;
        case DFT_PANE:
			TmpInputRec->DFT = OldData->DFT;
            break;
		case FMO_Pane:
			TmpInputRec->FMO = OldData->FMO;
			FMOFragmentIds.clear();
			if (MainData->GetFMOFragmentId(0)>0) {	//copy them over if setup
				Frame * cFrame = MainData->GetCurrentFramePtr();
				FMOFragmentIds.reserve(cFrame->GetNumAtoms());
				for (long i=0; i<cFrame->GetNumAtoms(); i++) {
					FMOFragmentIds.push_back(MainData->GetFMOFragmentId(i));
				}
			}
			break;
        case MOGUESS_PANE:
            if(OldData->Guess) {
                *(TmpInputRec->Guess) = *(OldData->Guess);
            }
            else {
                TmpInputRec->Guess->InitData();
            }
            break;
        case HESSOPTS_PANE:
            if(OldData->Hessian) {
                *(TmpInputRec->Hessian) = *(OldData->Hessian);
            }
            else {
                TmpInputRec->Hessian->InitData();
            }
            break;
        case MISCPREFS_PANE:
            TmpInputRec->Control->RevertProgPane(OldData->Control);
            break;
        case MP2OPTS_PANE:
            if(OldData->MP2) {
                *(TmpInputRec->MP2) = *(OldData->MP2);
            }
            else {
                TmpInputRec->MP2->InitData();
            }
            break;
        case SCFOPTS_PANE:
            if(OldData->SCF) {
                *(TmpInputRec->SCF) = *(OldData->SCF);
            }
            else {
                TmpInputRec->SCF->InitData();
            }
            break;
        case STATPOINT_PANE:
            if(OldData->StatPt) {
                *(TmpInputRec->StatPt) = *(OldData->StatPt);
            }
            else {
                TmpInputRec->StatPt->InitData();
            }
            break;
        default:
            // NO-OP
            break;
    }
    
    SetupItems();
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ADVANCEDBUTTON
 */

void InputBuilderWindow::OnAdvancedButtonClicked( wxCommandEvent& event )
{

	// User wants to go to advanced mode.
	if (advanced_button->GetLabel() == wxT("Advanced")) {
		advanced_button->SetLabel(wxT("Basic"));
		setPaneVisible(BASIS_PANE, ShowBasis());
		setPaneVisible(CONTROL_PANE, true);
		setPaneVisible(DATA_PANE, true);
		setPaneVisible(SYSTEM_PANE, true);
		setPaneVisible(DFT_PANE, true);
		setPaneVisible(MOGUESS_PANE, true);
		setPaneVisible(HESSOPTS_PANE, ShowHessOpts());
		setPaneVisible(MISCPREFS_PANE, true);
		setPaneVisible(MP2OPTS_PANE, ShowMP2Opts());
		setPaneVisible(SCFOPTS_PANE, ShowSCFOpts());
		setPaneVisible(STATPOINT_PANE, ShowStatPoint());
		setPaneVisible(SUMMARY_PANE, true);
		SetupControlItems();
	}
	
	// User wants to go to basic mode.
	else {
		listBook->SetSelection(1);
		advanced_button->SetLabel(wxT("Advanced"));
		for (int i = DATA_PANE; i <= SUMMARY_PANE; ++i) {
			setPaneVisible(i, false);
		}
	}

	advanced_button->SetSize(advanced_button->GetBestSize());
	button_sizer->Layout();

}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_WRITEFILEBUTTON
 */

void InputBuilderWindow::OnWritefilebuttonClick( wxCommandEvent& event )
{
	SaveData();
	MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	parent->GetData()->WriteInputFile(parent);
}

/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_ENQUEUEBUTTON
 */

void InputBuilderWindow::OnEnqueueButtonClick( wxCommandEvent& event )
{
	wxFileName file(wxFileName::CreateTempFileName(wxT("wxMacMolPltData_")));
	file.SetExt(wxT("inp"));
	SaveData();
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	TmpInputRec->WriteInputFile(file.GetFullPath(), parent->GetData(), parent);

#ifdef __WXMAC__
	pid_t pid = fork();
	if (pid == 0) {
		//My guess is that ToAscii needs to be replaced with mb_str for this to work.
		execlp("open", "open", "-a", "gamessq",
				(const char *)file.GetFullPath().ToAscii(), NULL);
		exit(1);
	}
#else /* __WXMAC__ */
	wxExecute(wxT("gamessq ") + file.GetFullPath());
#endif /* __WXMAC__ */
}

void InputBuilderWindow::setPaneVisible(int pane, bool visible) {
    int currPos = pane;
    int i = 0;
    
    if (visibleTab[pane] == visible) return;
    
    for(i = 0; i < pane; i++) {
        if(!visibleTab[i]) {
            currPos--;
        }
    }
    
    if (visible) {
        visibleTab[pane] = listBook->InsertPage(currPos, tab[pane], tabText[pane]);
    }
    else {
        visibleTab[pane] = !(listBook->RemovePage(currPos));
    }
}

int InputBuilderWindow::getCurrentPane() {
    return getPaneAtPosition(listBook->GetSelection());
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_COORD_CHOICE
 */

void InputBuilderWindow::OnCoordChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Data->SetCoordType((CoordinateType)(coordTypeChoice->GetSelection() + 1));
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_UNIT_CHOICE
 */

void InputBuilderWindow::OnUnitChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->Data->SetUnits(unitChoice->GetSelection());
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_POINTGROUP_CHOICE
 */

void InputBuilderWindow::OnPointgroupChoiceSelected( wxCommandEvent& event )
{
	TmpInputRec->Data->SetPointGroup((GAMESSPointGroup)(pointGroupChoice->GetSelection()+1));
	SetupPointGroupOrder();
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_ORDER_CHOICE
 */

void InputBuilderWindow::OnOrderChoiceSelected( wxCommandEvent& event )
{
	TmpInputRec->Data->SetPointGroupOrder(paxisOrderChoice->GetSelection()+2);
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SYMMETRY_CHECKBOX
 */

void InputBuilderWindow::OnSymmetryCheckboxClick( wxCommandEvent& event )
{
    TmpInputRec->Data->SetUseSym(event.IsChecked());
}


/*!
 * wxEVT_COMMAND_LISTBOOK_PAGE_CHANGED event handler for ID_IBNOTEBOOK
 */

void InputBuilderWindow::OnIblistbookPageChanged( wxListbookEvent& event )
{
	switch (getPaneAtPosition(event.GetSelection())) {
		case HESSOPTS_PANE:
			SetupHessOptsItems();
			break;
		case SUMMARY_PANE:
			SetupSummaryItems();
			break;
	}
    event.Skip();
}


int InputBuilderWindow::getPaneAtPosition(int pos) {
    int currPane = pos;
    int i = 0;
    
    for(i = 0; i <= currPane && i < NUM_PANES; i++) {
        if (!visibleTab[i]) {
            currPane++;
        }
    }
    
    return currPane;
}

#define kNumBasisMenuItems 89
void InputBuilderWindow::CheckBasisMenu(void) {
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
    AtomTypeList * list = parent->GetData()->GetAtomTypes();
    int i = 0;
    bool states[kNumBasisMenuItems];
    
    if(list) {
        for(i = 0; i < kNumBasisMenuItems; i++) states[i] = false;
        list->Sort();
        long MaxType = list->GetMaxAtomType();
		//Run through the rules for each basis set
		if (MaxType <= 86) {	//H-Rn
			states[0] = true;	//MINI
			states[1] = true;	//MIDI
		}
		if (MaxType <= 54) { //H-Xe
			for (i=2; i<7; i++) states[i] = true;	//STO-NG
			states[7] = true;	//3-21G
		}
		if (MaxType <= 18) //H-Ar
			states[8] = true;	//6-21G
		if ((MaxType <= 17)&&(!list->IsAtomInList(11) && !list->IsAtomInList(12) &&
							  ! list->IsAtomInList(13) && ! list->IsAtomInList(14))) {
				//H-Ne, P-Cl
			states[9] = true;	//4-31G
		}
		if ((MaxType <= 9)&&(!list->IsAtomInList(3) && !list->IsAtomInList(4) &&
							  ! list->IsAtomInList(5))) {
			//H-He, C-F
			states[10] = true;	//5-31G
		}
		if (MaxType <= 36) //H-Kr
			states[11] = true;	//6-31G
		if (MaxType <= 10) //H-Ne
			states[12] = true;	//6-311G
		if ((MaxType <= 36)&&(!list->IsAtomInList(2) && !list->IsAtomInList(11) &&
							  ! list->IsAtomInList(12) && ! list->TransitionMetalInList())) {
			//H, Li, Be-Ne, Al-Cl, K-Ca, Ga-Kr
			states[13] = true;	//DZV
		}
		if ((MaxType <= 17)&&(!list->IsAtomInList(2) && !list->IsAtomInList(11) &&
							 ! list->IsAtomInList(12))) {
			//H, Li, Be-Ne, Al-Cl
			states[14] = true;	//DH
		}
		if ((MaxType <= 30)&&(!list->IsAtomInList(2))) {
			//H, Li-Zn
			states[15] = true;	//TZV
		}
		if (MaxType <= 18) {	//H-Ar
			states[16] = true;	//MC
		}
		if ((MaxType <= 36)&&(!list->IsAtomInList(19))) {
			//H-Ar, Ca, Ga-Kr, Sc-Zn for T,Q only
			if (! list->TransitionMetalInList()) {
				states[17] = true;	//cc-pVDZ
				states[20] = true;	//cc-pV5Z
				states[21] = true;	//cc-pV6Z

				states[22] = true;	//aug-cc-pVDZ
				states[25] = true;	//aug-cc-pV5Z
				states[26] = true;	//aug-cc-pV6Z

				states[27] = true;	//cc-pCVDZ
				states[30] = true;	//cc-pCV5Z
				states[31] = true;	//cc-pCV6Z

				states[32] = true;	//aug-cc-pCVDZ
				states[35] = true;	//aug-cc-pCV5Z
				states[36] = true;	//aug-cc-pCV6Z
			}
			states[18] = true;	//cc-pVTZ
			states[19] = true;	//cc-pVQZ
			states[23] = true;	//aug-cc-pVTZ
			states[24] = true;	//aug-cc-pVQZ
			states[28] = true;	//cc-pCVTZ
			states[29] = true;	//cc-pCVQZ
			states[33] = true;	//aug-cc-pCVTZ
			states[34] = true;	//aug-cc-pCVQZ
		}
		if (MaxType <= 18) {	//H-Ar
			for (i=37; i<(37+10); i++)
				states[i] = true;	//Jensen PCn and APCn
		}
		if (MaxType <= 54) { //H-Xe
			for (i=47; i<(47+12); i++) states[i] = true;	//SPK variations
		}
		//the manual isn't clear on what is support for KTZV basis, for now just make them available
		states[59] = true;
		states[60] = true;
		states[61] = true;
		if (MaxType <= 86) {	//H-Rn
			states[62] = true;	//SBKJC
		}
		if (MaxType <= 54) {	//H-Xe
			states[63] = true;	//HW
		}
		if (MaxType <= 88) {	//H-Ra with exceptions
			states[64] = true;	//MCP family
			states[65] = true;	//MCP family
			states[66] = true;	//MCP family
			states[67] = true;	//MCP family
			states[68] = true;	//MCP family
			states[69] = true;	//MCP family
			states[70] = true;	//MCP family
			states[71] = true;	//MCP family
			states[72] = true;	//MCP family
			states[73] = true;	//MCP family
			states[74] = true;	//MCP family
		}
		if (MaxType <= 86) {	//H-Rn (ok lots of exceptions not included here)
			states[75] = true;	//IMCP-SR series
			states[76] = true;	//IMCP-SR series
			states[77] = true;	//IMCP-SR series
			states[78] = true;	//IMCP-SR series
		}
		if (MaxType <= 86 && ! list->TransitionMetalInList()) {	//H-Rn minus D-block elements
			states[79] = true;	//ZFK series
			states[80] = true;	//ZFK series
			states[81] = true;	//ZFK series
			states[82] = true;	//ZFK series
			states[83] = true;	//ZFK series
			states[84] = true;	//ZFK series
		}
		if (MaxType <= 86) {	//H-Rn, but's lot's of exceptions I'm too lazy to put in here
			states[85] = true;	//MNDO
			states[86] = true;	//AM1
			states[87] = true;	//PM3
			states[88] = true;	//RM1
		}
        
        delete list;
        
        for(i = 0; i < kNumBasisMenuItems; i++) {
            basisChoice->SetEnabled(i, states[i]);
        }
    }
}
/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_ZMAT_VARS_TEXT
 */

void InputBuilderWindow::OnZmatVarsTextUpdated( wxCommandEvent& event )
{
	wxString temp = zmatrixVarsText->GetValue();
	long t;
	if (temp.ToLong(&t)) {
		TmpInputRec->Data->SetNumZVar(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TITLE_TEXTCTRL
 */

void InputBuilderWindow::OnTitleTextctrlUpdated( wxCommandEvent& event )
{
	wxString temp = titleText->GetValue();
	TmpInputRec->Data->SetTitle((const char *)temp.mb_str(wxConvUTF8));
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_CORE_CHECKBOX
 */

void InputBuilderWindow::OnCoreCheckboxClick( wxCommandEvent& event )
{
	TmpInputRec->System->SetCoreFlag(coreCheck->GetValue());
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_TIMELIMIT_TEXT
 */

void InputBuilderWindow::OnTimelimitTextUpdated( wxCommandEvent& event )
{
	wxString temp = timeLimitText->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		TmpInputRec->System->SetConvertedTime(t);
	}

    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TIMELIMITUNITS_CHOICE
 */

void InputBuilderWindow::OnTimelimitunitsChoiceSelected( wxCommandEvent& event )
{
	TmpInputRec->System->SetTimeUnits((TimeUnit) (timeLimitUnitChoice->GetSelection()+1));
    wxString time;
	time.Printf(wxT("%.2f"), TmpInputRec->System->GetConvertedTime());
	timeLimitText->SetValue(time);

    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MEMORY_TEXT
 */

void InputBuilderWindow::OnMemoryTextUpdated( wxCommandEvent& event )
{
	wxString temp = memoryText->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		TmpInputRec->System->SetConvertedMem(t);
	}

    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MEMORYUNITS_CHOICE
 */

void InputBuilderWindow::OnMemoryunitsChoiceSelected( wxCommandEvent& event )
{
	TmpInputRec->System->SetMemUnits((MemoryUnit) (memoryUnitChoice->GetSelection()+1));
	wxString mem;
	mem.Printf(wxT("%.2f"), TmpInputRec->System->GetConvertedMem());
	memoryText->SetValue(mem);

    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_DIAGONALIZATION_CHOICE
 */

void InputBuilderWindow::OnDiagonalizationChoiceSelected( wxCommandEvent& event )
{
	TmpInputRec->System->SetDiag(diagChoice->GetSelection());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_PLBALANCE_RADIOBOX
 */

void InputBuilderWindow::OnPlbalanceRadioboxSelected( wxCommandEvent& event )
{
	TmpInputRec->System->SetBalanceType(plBalanceRadio->GetSelection());
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_EXTERNDATAREP_CHECKBOX
 */

void InputBuilderWindow::OnExterndatarepCheckboxClick( wxCommandEvent& event )
{
	TmpInputRec->System->SetXDR(externDataRepCheck->GetValue());
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_INITGUESS_CHOICE
 */

void InputBuilderWindow::OnInitguessChoiceSelected( wxCommandEvent& event )
{
	int s = initGuessChoice->GetSelection() + 1;
	if (s != TmpInputRec->Guess->GetGuess()) {
		TmpInputRec->Guess->SetGuess(s);
		SetupMOGuessItems();
	}
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PRINTINITGUESS_CHECKBOX
 */

void InputBuilderWindow::OnPrintinitguessCheckboxClick( wxCommandEvent& event )
{
	TmpInputRec->Guess->SetPrintMO(printInitGuessCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_ROTORBITALS_CHECKBOX
 */

void InputBuilderWindow::OnRotorbitalsCheckboxClick( wxCommandEvent& event )
{
	TmpInputRec->Guess->SetMix(rotOrbitalsCheck->GetValue());
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MOGUESS_VECSOURCE_CHOICE
 */

void InputBuilderWindow::OnMoguessVecsourceChoiceSelected( wxCommandEvent& event )
{
	int s = mMOSourceChoice->GetSelection() + 1;
	if (s != TmpInputRec->Guess->GetVecSource()) {
		TmpInputRec->Guess->SetVecSource(s);
		SetupMOGuessItems();
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MOGUESS_VECORBCOUNT_TEXT
 */

void InputBuilderWindow::OnMoguessVecorbcountTextUpdated( wxCommandEvent& event )
{
	long t;
	wxString noTemp = mVecOrbCountText->GetValue();
	if (noTemp.ToLong(&t)) {
		TmpInputRec->Guess->SetNumOrbs(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_MOLPLT_CHECK
 */

void InputBuilderWindow::OnMolpltCheckClick( wxCommandEvent& event )
{
	TmpInputRec->Control->SetMolPlot(mMolPltCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PLTORB_CHECK
 */

void InputBuilderWindow::OnPltorbCheckClick( wxCommandEvent& event )
{
	TmpInputRec->Control->SetPlotOrb(mPltOrbCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_AIMPAC_CHECK
 */

void InputBuilderWindow::OnAimpacCheckClick( wxCommandEvent& event )
{
	TmpInputRec->Control->SetAIMPAC(aimpacCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_RPAC_CHECK
 */

void InputBuilderWindow::OnRpacCheckClick( wxCommandEvent& event )
{
	TmpInputRec->Control->SetRPAC(rpacCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_MISC_PROG_RADIO
 */

void InputBuilderWindow::OnMiscProgRadioSelected( wxCommandEvent& event )
{
	int val = mMiscProgRadio->GetSelection();
	if (val != TmpInputRec->Control->GetFriend()) {
		TmpInputRec->Control->SetFriend((FriendType) val);
		SetupMiscPrefsItems();
		SetupControlItems();
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_DIRECTSCF_CHECK
 */

void InputBuilderWindow::OnDirectscfCheckClick( wxCommandEvent& event )
{
	TmpInputRec->SCF->SetDirectSCF(mDirectSCFCheck->GetValue());
	SetupSCFOptsItems();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_FDIFF_CHECK
 */

void InputBuilderWindow::OnFdiffCheckClick( wxCommandEvent& event )
{
	TmpInputRec->SCF->SetFockDiff(mFDiffCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_UHF_NO_CHECK
 */

void InputBuilderWindow::OnUhfNoCheckClick( wxCommandEvent& event )
{
	TmpInputRec->SCF->SetUHFNO(mUHFNOCheck->GetValue());
 //   event.Skip();
}

/*!
 * wxEVT_COMMAND_SPINCTRL_UPDATED event handler for ID_SCF_CONV_SPIN
 */

void InputBuilderWindow::OnScfConvSpinUpdated( wxSpinEvent& event )
{
	TmpInputRec->SCF->SetConvergance(mSCFConvSpin->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_INIT_HESS_RADIO
 */

void InputBuilderWindow::OnInitHessRadioSelected( wxCommandEvent& event )
{
	int val = mInitHessRadio->GetSelection() + 1;
	if (val != TmpInputRec->StatPt->GetHessMethod()) {
		TmpInputRec->StatPt->SetHessMethod(val);
	}
	SetupStatPointItems();
    SetupItems();
	event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PRINTORBS_CHECK
 */

void InputBuilderWindow::OnPrintorbsCheckClick( wxCommandEvent& event )
{
	TmpInputRec->StatPt->SetAlwaysPrintOrbs(mPrintOrbsCheck->GetValue());
 //   event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_HESS_RECALC_EDIT
 */

void InputBuilderWindow::OnHessRecalcEditUpdated( wxCommandEvent& event )
{
	wxString temp = mHessRecalcEdit->GetValue();
	long t;
	if (temp.ToLong(&t)) {
		TmpInputRec->StatPt->SetHessRecalcInterval(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MAXSTEPCOUNT_EDIT
 */

void InputBuilderWindow::OnMaxstepcountEditUpdated( wxCommandEvent& event )
{
	wxString temp = mMaxStepCountEdit->GetValue();
	long t;
	if (temp.ToLong(&t)) {
		TmpInputRec->StatPt->SetMaxSteps(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_GRADCONVCRITERIA_EDIT
 */

void InputBuilderWindow::OnGradconvcriteriaEditUpdated( wxCommandEvent& event )
{
	wxString temp = mGradConvEdit->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		TmpInputRec->StatPt->SetOptConvergance(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_OPTMETHOD_CHOICE
 */

void InputBuilderWindow::OnOptmethodChoiceSelected( wxCommandEvent& event )
{
	TmpInputRec->StatPt->SetMethod(mOptMethodChoice->GetSelection()+1);
	SetupStatPointItems();
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_INITSTEPSIZE_EDIT
 */

void InputBuilderWindow::OnInitstepsizeEditUpdated( wxCommandEvent& event )
{
	wxString temp = mInitStepSizeEdit->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		//test against the default
		double def = 0.3;
		if (TmpInputRec->StatPt->GetMethod() == 5) def = 0.1;
		else if (TmpInputRec->Control->GetRunType() == 6) def = 0.2;
		if (fabs(t-def) < 1.0e-10) t = 0.0;
		TmpInputRec->StatPt->SetInitRadius(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_UPDATESTEPSIZE_CHECK
 */

void InputBuilderWindow::OnUpdatestepsizeCheckClick( wxCommandEvent& event )
{
	TmpInputRec->StatPt->SetRadiusUpdate(mUpdateStepSizeCheck->GetValue());
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MINSTEPSIZE_EDIT
 */

void InputBuilderWindow::OnMinstepsizeEditUpdated( wxCommandEvent& event )
{
	wxString temp = mMinStepSizeEdit->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		TmpInputRec->StatPt->SetMinRadius(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MAXSTEPSIZE_Edit
 */

void InputBuilderWindow::OnMaxstepsizeEditUpdated( wxCommandEvent& event )
{
	wxString temp = mMaxStepSizeEdit->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		//test against the default
		double def=0.5;
		if (TmpInputRec->Control->GetRunType() == 6) def = 0.3;
		if (fabs(t-def) < 1.0e-10) t = 0.0;
		TmpInputRec->StatPt->SetMaxRadius(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_STATPT_CHECK
 */

void InputBuilderWindow::OnStatptCheckClick( wxCommandEvent& event )
{
	TmpInputRec->StatPt->SetStatPoint(mIsStatPtCheck->GetValue());
	SetupStatPointItems();
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_STATPT_MODE_EDIT
 */

void InputBuilderWindow::OnStatptModeEditUpdated( wxCommandEvent& event )
{
	wxString temp = mStatPtModeEdit->GetValue();
	long t;
	if (temp.ToLong(&t)) {
		TmpInputRec->StatPt->SetModeFollow(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_STATPT_JUMP_EDIT
 */

void InputBuilderWindow::OnStatptJumpEditUpdated( wxCommandEvent& event )
{
	wxString temp = mStatPtJumpSizeEdit->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		TmpInputRec->StatPt->SetStatJump(t);
	}
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MCHARGE_TEXTCTRL
 */

void InputBuilderWindow::OnMchargeTextctrlUpdated( wxCommandEvent& event )
{
    wxString tmpStr = mchargeText->GetValue();
    long val = 0;
    if(tmpStr.ToLong(&val)) {
        TmpInputRec->Control->SetCharge((short)val);
    }
    
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MULT_TEXTCTRL
 */

void InputBuilderWindow::OnMultTextctrlUpdated( wxCommandEvent& event )
{
    wxString tmpStr = multText->GetValue();
    long val = 0;
    if(tmpStr.ToLong(&val)) {
        TmpInputRec->Control->SetMultiplicity((short)val);
    }
    
    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MAXIT_TEXTCTRL
 */

void InputBuilderWindow::OnMaxitTextctrlUpdated( wxCommandEvent& event )
{
    wxString tmpStr = scfIterText->GetValue();
    long val = 0;
    if(tmpStr.ToLong(&val)) {
        TmpInputRec->Control->SetMaxIt((short)val);
    }
    
    event.Skip();
}
/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MP2_CORE_EDIT
 */

void InputBuilderWindow::OnMp2CoreEditUpdated( wxCommandEvent& event )
{
    wxString tmpStr = mMP2CoreEleEdit->GetValue();
    long val;
    if(tmpStr.ToLong(&val)) {
		if (val >= 0)
			TmpInputRec->MP2->SetNumCoreElectrons(val);
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MP2MEM_EDIT
 */

void InputBuilderWindow::OnMp2memEditUpdated( wxCommandEvent& event )
{
    wxString tmpStr = mMP2MemEdit->GetValue();
    long val;
    if(tmpStr.ToLong(&val)) {
        TmpInputRec->MP2->SetMemory(val);
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MP2INTCUTOFF_EDIT
 */

void InputBuilderWindow::OnMp2intcutoffEditUpdated( wxCommandEvent& event )
{
    wxString tmpStr = mMP2IntCutoffEdit->GetValue();
    double val;
    if(tmpStr.ToDouble(&val)) {
		//test against the default
		if (fabs(val - 1.0e-9) < 1.0e-20) val = 0.0;
		TmpInputRec->MP2->SetIntCutoff(val);
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_LMOMP2_CHECK
 */

void InputBuilderWindow::OnLmomp2CheckClick( wxCommandEvent& event )
{
	TmpInputRec->MP2->SetLMOMP2(mLMOMP2Check->GetValue());
	SetupMP2OptsItems();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_MP2PROP_CHECK
 */

void InputBuilderWindow::OnMp2propCheckClick( wxCommandEvent& event )
{
	TmpInputRec->MP2->SetMP2Prop(mMP2PropCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_MP2TRANS_RADIO
 */

void InputBuilderWindow::OnMp2transRadioSelected( wxCommandEvent& event )
{
	TmpInputRec->MP2->SetMethod(mMP2TransRadio->GetSelection()+1);
    event.Skip();
}

/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_MP2AOSTORAGE_RADIO
 */

void InputBuilderWindow::OnMp2aostorageRadioSelected( wxCommandEvent& event )
{
	TmpInputRec->MP2->SetAOIntMethod(mMP2AOStorageRadio->GetSelection()+1);
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_DFTMETHOD_CHOICE
 */

void InputBuilderWindow::OnDftmethodChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->DFT.SetMethodGrid(!(dftMethodChoice->GetSelection()));
    SetupItems();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_FUNCTIONAL_CHOICE
 */

void InputBuilderWindow::OnGridFunctionalChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->DFT.SetFunctional(dftGridFuncChoice->GetSelection() + 1);
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_GRIDFREE_FUNCTIONAL_CHOICE
 */

void InputBuilderWindow::OnGridfreeFunctionalChoiceSelected( wxCommandEvent& event )
{
    TmpInputRec->DFT.SetFunctional(dftGridFreeFuncChoice->GetSelection() + 1);
}

/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_HESSMETHOD_RADIO
 */

void InputBuilderWindow::OnHessmethodRadioSelected( wxCommandEvent& event )
{
	TmpInputRec->Hessian->SetAnalyticMethod(!(mHessMethodRadio->GetSelection()));
	SetupHessOptsItems();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_FORCEDISPLACE_EDIT
 */

void InputBuilderWindow::OnForcedisplaceEditUpdated( wxCommandEvent& event )
{
    wxString tmpStr = mHESSDisplaceEdit->GetValue();
    double val;
    if(tmpStr.ToDouble(&val)) {
        TmpInputRec->Hessian->SetDisplacementSize(val);
    }
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HESSDOUBLEDIFFCHECK
 */

void InputBuilderWindow::OnHessdoublediffcheckClick( wxCommandEvent& event )
{
	TmpInputRec->Hessian->SetDoubleDiff(mHessDblDiffCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HESSPURIFYCHECK
 */

void InputBuilderWindow::OnHesspurifycheckClick( wxCommandEvent& event )
{
	TmpInputRec->Hessian->SetPurify(mHessPurifyCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HESSPRTINTFC_CHECK
 */

void InputBuilderWindow::OnHessprtintfcCheckClick( wxCommandEvent& event )
{
	TmpInputRec->Hessian->SetPrintFC(mHessPrintIntFCCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HESSVIBANL_CHECK
 */

void InputBuilderWindow::OnHessvibanlCheckClick( wxCommandEvent& event )
{
	TmpInputRec->Hessian->SetVibAnalysis(mHessVibAnlCheck->GetValue());
	SetupHessOptsItems();
    event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_HESSFREQSCALE_EDIT
 */

void InputBuilderWindow::OnHessfreqscaleEditUpdated( wxCommandEvent& event )
{
    wxString tmpStr = mHessFreqScaleEdit->GetValue();
    double val;
    if(tmpStr.ToDouble(&val)) {
        TmpInputRec->Hessian->SetFreqScale(val);
    }
    event.Skip();
}


/*!
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MEMDDICHOICE
 */

void InputBuilderWindow::OnMemddichoiceSelected( wxCommandEvent& event )
{
	TmpInputRec->System->SetMemDDIUnits((MemoryUnit) (memddiUnitChoice->GetSelection()+megaWordsUnit));
	wxString mem;
	mem.Printf(wxT("%.2f"), TmpInputRec->System->GetConvertedMemDDI());
	memddiText->SetValue(mem);

    event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_MEMDDI_EDIT
 */

void InputBuilderWindow::OnMemddiEditUpdated( wxCommandEvent& event )
{
	wxString temp = memddiText->GetValue();
	double t;
	if (temp.ToDouble(&t)) {
		TmpInputRec->System->SetConvertedMemDDI(t);
	}

    event.Skip();
}
/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PARALL_CHECK
 */

void InputBuilderWindow::OnParallCheckClick( wxCommandEvent& event )
{
	TmpInputRec->System->SetParallel(mSGParallCheck->GetValue());
    event.Skip();
}

/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SPHERE_CHECK
 */

void InputBuilderWindow::OnSphereCheckClick( wxCommandEvent& event )
{
	TmpInputRec->Control->UseSphericalHarmonics(spherHarmCheck->GetValue());
	event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_HESSEND_CHECK
 */

void InputBuilderWindow::OnHessendCheckClick( wxCommandEvent& event )
{
	TmpInputRec->StatPt->SetHessFlag(event.IsChecked());
    SetupItems();
	event.Skip();
}


/*!
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_FMOACTIVECHECK
 */

void InputBuilderWindow::OnFMOActiveCheckClick( wxCommandEvent& event )
{
	TmpInputRec->FMO.FMOActive(mFMOCheck->GetValue());
	SetupFMOItems();
	event.Skip();
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_FMOFRAGBUTTON
 */

void InputBuilderWindow::OnFMOFragButtonClick( wxCommandEvent& event )
{
	//launch the FMO fragmentation dialog
    MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
	
	FMOFragmentationDialog fragDialog(parent);
	if (fragDialog.ShowModal() == wxID_OK) {
		//retrieve the # nonbonded atoms and call the fragmentation routine
		MoleculeData * MainData = parent->GetData();
		int count = fragDialog.GetMolCount();
		long fragcount = MainData->CreateFMOFragmentation(count, FMOFragmentIds);
		TmpInputRec->FMO.SetNumberFragments(fragcount);
		SetupFMOItems();
		wxString nf;
		nf.Printf(wxT("%ld"), TmpInputRec->FMO.GetNumberFragments());
		mNumFragsEdit->SetValue(nf);
	}
	event.Skip();
}

/*!
 * wxEVT_GRID_CELL_CHANGE event handler for ID_FMOLIST
 */

void InputBuilderWindow::OnFMOFragIdCellChange( wxGridEvent& event )
{
	int row = event.GetRow();

//	MolDisplayWin *parent = (MolDisplayWin *)this->GetParent();
//	MoleculeData * MainData = parent->GetData();

	wxString val = mFMOFragList->GetCellValue(row, 0);
	long newval;
	if (val.ToLong(&newval)) {
		if ((newval>0)&&(newval<=TmpInputRec->FMO.GetNumberFragments())) {
			FMOFragmentIds[row] = newval;
		}
	}
	val.Printf(wxT("%d"), FMOFragmentIds[row]);
	mFMOFragList->SetCellValue(row, 0, val);
	event.Skip();
}

/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_FMOFRAGCOUNT
 */

void InputBuilderWindow::OnFMOFragCountTextUpdated( wxCommandEvent& event )
{
	wxString temp = mNumFragsEdit->GetValue();
	long t;
	if (temp.ToLong(&t)) {
		TmpInputRec->FMO.SetNumberFragments(t);
	}
	event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_GVB_NCOEDIT
 */

void InputBuilderWindow::OnGVBNCOEditTextUpdated( wxCommandEvent& event )
{
	wxString temp = mGVB_NCOEdit->GetValue();
	long t;
	if (temp.ToLong(&t)) {
		TmpInputRec->SCF->SetGVBNumCoreOrbs(t);
	}
	
	event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_GVB_PAIRSEDIT
 */

void InputBuilderWindow::OnGVBPairsEditTextUpdated( wxCommandEvent& event )
{
	wxString temp = mGVB_PairsEdit->GetValue();
	long t;
	if (temp.ToLong(&t)) {
		TmpInputRec->SCF->SetGVBNumPairs(t);
	}
	
	event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_GVB_NSETOEDIT
 */

void InputBuilderWindow::OnGVBNSetOeditTextUpdated( wxCommandEvent& event )
{
	wxString temp = mGVB_NSETOEdit->GetValue();
	long t;
	if (temp.ToLong(&t)) {
		TmpInputRec->SCF->SetGVBNumOpenShells(t);
		//now adjust the shell degeneracy array
		if (TmpInputRec->SCF->GetGVBNumOpenShells() > 0) {
			mGVB_NOEdit->Show(true);
			mGVB_NOStatic->Show(true);
			std::vector<long> t = TmpInputRec->SCF->GetGVBOpenShellDeg();
			int num = t.size();
			if (TmpInputRec->SCF->GetGVBNumOpenShells() > num) {	//This shouldn't happen?
				for (int i=num; i<TmpInputRec->SCF->GetGVBNumOpenShells(); i++)
					t.push_back(0);
			} else if (TmpInputRec->SCF->GetGVBNumOpenShells() < num)
				num =TmpInputRec->SCF->GetGVBNumOpenShells();
			std::ostringstream degString;
			for (int i=0; i<num; i++) {
				if (i>0) degString << " ";
				degString << t[i];
			}
			mGVB_NOEdit->SetValue(wxString(degString.str().c_str(), wxConvUTF8));
		} else {
			mGVB_NOEdit->Show(false);
			mGVB_NOStatic->Show(false);
		}
	}
	
	event.Skip();
}


/*!
 * wxEVT_COMMAND_TEXT_UPDATED event handler for ID_GVB_NOEDIT
 */

void InputBuilderWindow::OnGVBNOeditTextUpdated( wxCommandEvent& event )
{
	wxString temp = mGVB_NOEdit->GetValue();
	std::istringstream buf((const char *)temp.mb_str(wxConvUTF8));
	int count=0;
	long val;
	
	while ((count<TmpInputRec->SCF->GetGVBNumOpenShells())&&(!buf.eof())) {
		buf >> val;
		if (val >= 0) TmpInputRec->SCF->SetGVBNODegValue(count, val);
		count++;
	}
	event.Skip();
}

