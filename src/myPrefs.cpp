/*
 Implement preferences...    -Song Li
*/

//Make some useful preferences related definitions

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "PrefsPanes.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/colordlg.h"
#endif

//put ID definitions here later
#define kAtomPrefsPane		1
#define kBondPrefsPane		2
#define kDisplayPrefsPane	3
#define kEPrefsPane		4
#define kFontPrefsPane		5
#define kFilePrefsPane		6
#define kScalingPrefsPane	7
#define kStereoPrefsPane	8
#define kSurfacePrefsPane	9
#define kQD3DPrefsPane		10

#define ID_SHOW_NORMAL_MODE_ANIMATION 5000
#define ID_SHOW_NORMAL_MODE_ROTATION 5001
#define ID_NORMAL_MODE_SCALING_SLIDER 5002
#define ID_BOND_MODE_CHOICE 5003
#define ID_DISPLAY_MODE_RADIOBOX 5004
#define ID_COLOR_BONDS_BY_ATOM_COLOR 5005
#define ID_OUTLINE_BONDS 5006
#define ID_PLOT_BALL_SIZE_SLIDER 5007
#define ID_AUTO_BOND_PASTE_OPEN 5008
#define ID_GUESS_BOND_ORDER 5009
#define ID_PREVENT_HH_BONDS 5010
#define ID_LOOK_HYDROGEN_BONDS 5011
#define ID_SHOW_ANGLES_WHILE_ROTATING 5012
#define ID_USE_MAC_EOL_CHAR 5013
#define ID_PROMPT_SAVE_FILE 5014
#define ID_CREATE_CUSTOM_FILE_ICON 5015
#define ID_CHANGE_FILE_CREATOR 5016
#define ID_AUTO_BOND_TOLERANCE_SLIDER 5017
#define ID_ATOM_SIZE_SLIDER 5018
#define ID_DEPTH_CUEING_SLIDER 5019
#define ID_ANIM_QUALITY_SLIDER 5020
#define ID_FRAME_DELAY_SLIDER 5021
#define ID_STEREO_ACTIVE 5022
#define ID_NUM_GRID_POINT_SLIDER 5023
#define ID_3D_GRID_SIZE_SLIDER 5024
#define ID_SHOW_ZERO_VALUE_CONTOUR 5025
#define ID_SHOW_ATOM_POSITION 5026
#define ID_BOND_SIZE_SLIDER 5027
#define ID_DISPLAY_QUALITY_SLIDER 5028
#define ID_FILL_LIGHT_BRIGHTNESS_SLIDER 5029
#define ID_POINT_LIGHT_BRIGHTNESS_SLIDER 5030
#define ID_LINE_WIDTH_SLIDER 5031
#define ID_ACTIVATE_3D_MODE 5032
#define ID_TOTAL_ENERGY 5033
#define ID_MP2_ENERGY 5034
#define ID_POTENTIAL_ENERGY 5035
#define ID_UNIT_RADIOBOX 5036
#define ID_MISC_RADIOBOX 5037

#include "Globals.h"
#include "GlobalExceptions.h"
#include "MyTypes.h"
#include "Prefs.h"
#include "PrefsPanes.h"
#include <stdio.h>
#include <string.h>

using namespace std;

	//local functions definitions
/*short GetFontValue(ControlHandle FontControl);
void SetFontValue(short FontID, ControlHandle FontControl);
void SetUpFontSizeMenu(short FontID, ControlHandle SizeControl);
long GetFontSize(ControlHandle SizeControl);
void SetFontSizeValue(short Size, ControlHandle SizeControl);
pascal void AtomPrefScroll(ControlHandle control, short part);
	//Pattern picker
void myGetPattern(Str255 Title, short *inPat, short *outPat);
	//Two Pattern picker util routines (used from within myGetPattern)
pascal void myDrawUserItems(DialogPtr dialog, short theItem);
pascal Boolean MyPatFilter(DialogPtr dialog, EventRecord *event, short *itemHit);*/ //Song Li

//extern WinPrefs *	gPreferences;
//extern WinPrefs *	gPrefDefaults;
extern short		gAppResRef;
/*extern PrefsDlg *	gPrefsDlog;
extern PatListHandle	gDitherPatterns; */  //Song Li
extern MolDisplayWin *	gWindow[kMaxOpenDocuments];
/*extern NumVersion	gMacMolPltVersion; */  //Song Li

BEGIN_EVENT_TABLE(colorArea, wxPanel)
    EVT_MOUSE_EVENTS(colorArea::OnMouse)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(BondPrefsPane, wxPanel)
  EVT_CHOICE (ID_BOND_MODE_CHOICE, BondPrefsPane::OnChoice)
  EVT_SLIDER (ID_NORMAL_MODE_SCALING_SLIDER, BondPrefsPane::OnSliderUpdate)
  EVT_CHECKBOX (ID_SHOW_NORMAL_MODE_ANIMATION, BondPrefsPane::OnToggleAnim)
  EVT_CHECKBOX (ID_SHOW_NORMAL_MODE_ROTATION, BondPrefsPane::OnToggleRotation)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(DisplayPrefsPane, wxPanel)
  EVT_RADIOBOX (ID_DISPLAY_MODE_RADIOBOX, DisplayPrefsPane::OnRadio)
  EVT_CHECKBOX (ID_COLOR_BONDS_BY_ATOM_COLOR, DisplayPrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_OUTLINE_BONDS, DisplayPrefsPane::OnCheckBox)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(EnergyPrefsPane, wxPanel)
  EVT_SLIDER (ID_PLOT_BALL_SIZE_SLIDER, EnergyPrefsPane::OnSliderUpdate)
  EVT_RADIOBOX (ID_UNIT_RADIOBOX, EnergyPrefsPane::OnRadio)
  EVT_RADIOBOX (ID_MISC_RADIOBOX, EnergyPrefsPane::OnRadio)
  EVT_CHECKBOX (ID_TOTAL_ENERGY, EnergyPrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_MP2_ENERGY, EnergyPrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_POTENTIAL_ENERGY, EnergyPrefsPane::OnCheckBox)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(FilePrefsPane, wxPanel)
  EVT_CHECKBOX (ID_AUTO_BOND_PASTE_OPEN, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_GUESS_BOND_ORDER, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_PREVENT_HH_BONDS, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_LOOK_HYDROGEN_BONDS, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_SHOW_ANGLES_WHILE_ROTATING, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_USE_MAC_EOL_CHAR, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_PROMPT_SAVE_FILE, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_CREATE_CUSTOM_FILE_ICON, FilePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_CHANGE_FILE_CREATOR, FilePrefsPane::OnCheckBox)
  EVT_SLIDER (ID_AUTO_BOND_TOLERANCE_SLIDER, FilePrefsPane::OnSliderUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(ScalingPrefsPane, wxPanel)
  EVT_SLIDER (ID_ATOM_SIZE_SLIDER, ScalingPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_DEPTH_CUEING_SLIDER, ScalingPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_ANIM_QUALITY_SLIDER, ScalingPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_FRAME_DELAY_SLIDER, ScalingPrefsPane::OnSliderUpdate)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(StereoPrefsPane, wxPanel)
  EVT_CHECKBOX (ID_STEREO_ACTIVE, StereoPrefsPane::OnCheckBox)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(SurfacePrefsPane, wxPanel)
  EVT_SLIDER (ID_NUM_GRID_POINT_SLIDER, SurfacePrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_3D_GRID_SIZE_SLIDER, SurfacePrefsPane::OnSliderUpdate)
  EVT_CHECKBOX (ID_SHOW_ZERO_VALUE_CONTOUR, SurfacePrefsPane::OnCheckBox)
  EVT_CHECKBOX (ID_SHOW_ATOM_POSITION, SurfacePrefsPane::OnCheckBox)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(QD3DPrefsPane, wxPanel)
  EVT_SLIDER (ID_BOND_SIZE_SLIDER, QD3DPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_DISPLAY_QUALITY_SLIDER, QD3DPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_FILL_LIGHT_BRIGHTNESS_SLIDER, QD3DPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_POINT_LIGHT_BRIGHTNESS_SLIDER, QD3DPrefsPane::OnSliderUpdate)
  EVT_SLIDER (ID_LINE_WIDTH_SLIDER, QD3DPrefsPane::OnSliderUpdate)
  EVT_CHECKBOX (ID_ACTIVATE_3D_MODE, QD3DPrefsPane::OnCheckBox)
END_EVENT_TABLE()

void WinPrefs::ReadUserPrefs_old(void) {
  /*
	OSErr	myErr;
	short	fileRefNum=gAppResRef;

	if (PreferencesFileExists ((OSType) 'BMBm', (OSType) 'Pref')) {//Read from the pref file if it exists
		myErr = OpenPreferencesFile ((OSType) 'BMBm', (OSType) 'Pref', &fileRefNum);
		if (myErr != noErr) fileRefNum = gAppResRef;
	} else return;
		//Read in the default atom colors
	myErr  = myReadPreference(fileRefNum, fileRefNum, (ResType) 'AtmC', 4, (Ptr) BondColors, 4*sizeof(RGBColor));
	if (myErr != noErr) {
		myReadPreference(gAppResRef, gAppResRef, (ResType) 'AtmC', 4, (Ptr) BondColors, 4*sizeof(RGBColor));
		myReadPreference(fileRefNum, fileRefNum, (ResType) 'AtmC', 1, (Ptr) &(BondColors[1]), sizeof(RGBColor));
	}
	myErr  = myReadPreference(fileRefNum, fileRefNum, (ResType) 'AtmP', 4, (Ptr) BondPatterns, 4*sizeof(short));
	if (myErr != noErr) {
		myReadPreference(gAppResRef, gAppResRef, (ResType) 'AtmP', 4, (Ptr) BondPatterns, 4*sizeof(short));
		myReadPreference(fileRefNum, fileRefNum, (ResType) 'AtmP', 1, (Ptr) &(BondPatterns[1]), sizeof(short));
	}
	myErr  = myReadPreference(fileRefNum, gAppResRef, (ResType) 'AtmC', 2, (Ptr) &VectorColor, sizeof(RGBColor));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'AtmC', 130, (Ptr) AtomColors, 130*sizeof(RGBColor));
		//Read in the default atom patterns
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'AtmP', 2, (Ptr) &VectorPattern, sizeof(short));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'AtmP', 130, (Ptr) AtomPatterns, 130*sizeof(short));
		//Atom Labels
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'AtmL', 130, (Ptr) AtomLabels, 3*130*sizeof(char));
		//Atom radii
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'AtmS', 130, (Ptr) AtomSizes, 130*sizeof(int));
		//Atom masses
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'AtmM', 115, (Ptr) AtomMasses, 115*sizeof(float));
	AtomMasses[115] = AtomMasses[116] = AtomMasses[117] = AtomMasses[118] = AtomMasses[119] =
		AtomMasses[120] = AtomMasses[121] = AtomMasses[122] = AtomMasses[123] = AtomMasses[124] =
		AtomMasses[125] = AtomMasses[126] = AtomMasses[127] = AtomMasses[128] = AtomMasses[129] = 0.0;
		//Energy Plot Options
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'EnPr', 128, (Ptr) &EnergyPlotOptions, sizeof(EnergyOptions));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'GrPr', 128, (Ptr) &PlotOptions, sizeof(GraphOptions));
	if (EnergyPlotOptions.GetYAdjustment() == 0)
		EnergyPlotOptions.SetYAdjustment(1.0);
		//Font Preferences
	Str255 FontName, TestFontName;
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'AtmF', 1, (Ptr) FontName, -255);
	if (FontName[0] <= 0) {
		LabelFontID = systemFont;
	} else {
#ifdef CarbonBuild
		LabelFontID = FMGetFontFamilyFromName(FontName);
		FMGetFontFamilyName(LabelFontID, TestFontName);
#else
		GetFNum(FontName, &LabelFontID);
		GetFontName(LabelFontID, TestFontName);
#endif
		if (!(EqualString(FontName, TestFontName, false, false)))
			LabelFontID = systemFont;
	}
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'AtmF', 2, (Ptr) &LabelSize, sizeof(short));
		//Display related preferences (Note: different types mixed in)
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'DisP', 1, (Ptr) &AnimationSpeed, sizeof(short));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'DisP', 2, (Ptr) &VectorScale, sizeof(float));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'DisP', 3, (Ptr) &AtomScale, sizeof(float));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'DisP', 4, (Ptr) &ZScale, sizeof(float));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'DisP', 5, (Ptr) &RotateMode, sizeof(Boolean));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'DisP', 6, (Ptr) &AnimateMode, sizeof(Boolean));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'DisP', 7, (Ptr) &AnimateTime, sizeof(long));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'DisP', 9, (Ptr) &BitOptions, sizeof(long));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'DisP', 10, (Ptr) &StereoOffset, sizeof(short));
		//Misc file open and new frame preferences
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'FilP', 1, (Ptr) &AutoBondScale, sizeof(float));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'FilP', 2, (Ptr) &AutoBondFlag, sizeof(Boolean));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'FilP', 3, (Ptr) &HHBondFlag, sizeof(Boolean));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'FilP', 4, (Ptr) &DRCnFileSkip, sizeof(long));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'FilP', 5, (Ptr) &SetCreator, sizeof(Boolean));
	MaxBondLength = 0.0;	//Not really a pref worth saving
		//Surface Options
//	long tempsize = sizeof(SurfaceOptions);
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'Surf', 1, (Ptr) &SurfaceOpts, sizeof(SurfaceOptions));
		//QuickDraw 3D related preferences
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'QD3P', 1, (Ptr) &BondWidth, sizeof(float));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'QD3P', 2, (Ptr) &QD3DAtomQuality, sizeof(long));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'QD3P', 3, (Ptr) &QD3DFillBrightness, sizeof(float));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'QD3P', 4, (Ptr) &QD3DPointBrightness, sizeof(float));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'QD3P', 5, (Ptr) &QD3DLineWidth, sizeof(float));
	myErr += myReadPreference(fileRefNum, gAppResRef, (ResType) 'AtmC', 6, (Ptr) &BackColor, sizeof(RGBColor));
		//If we are using the prefs file close it
	if (fileRefNum != gAppResRef) ClosePreferencesFile(fileRefNum);
	if (myErr != noErr) {	//There was a problem reading the preferences file alert the user
		Str255	ErrMsg;

		GetIndString(ErrMsg, kerrstrings, 7);
		MessageAlert(ErrMsg);
	}
  */ //Song Li
}

void WinPrefs::WriteAllPrefs(short fileRefNum) {
  /*
	short ResID;
		//First the colors: bond, vector, and all atom defaults
	ResID=4;
	WritePreference (fileRefNum, (ResType) 'AtmC', &ResID, (Ptr) BondColors, 4*sizeof(RGBColor));
	ResID=2;
	WritePreference (fileRefNum, (ResType) 'AtmC', &ResID, (Ptr) &VectorColor, sizeof(RGBColor));
	ResID=130;
	WritePreference (fileRefNum, (ResType) 'AtmC', &ResID, (Ptr) AtomColors, 130*sizeof(RGBColor));
		//Now the patterns: bonds, vector, and all atom defaults
	ResID=4;
	WritePreference (fileRefNum, (ResType) 'AtmP', &ResID, (Ptr) BondPatterns, 4*sizeof(short));
	ResID=2;
	WritePreference (fileRefNum, (ResType) 'AtmP', &ResID, (Ptr) &VectorPattern, sizeof(short));
	ResID=130;
	WritePreference (fileRefNum, (ResType) 'AtmP', &ResID, (Ptr) AtomPatterns, 130*sizeof(short));
		//atomlabels
	ResID=130;
	WritePreference (fileRefNum, (ResType) 'AtmL', &ResID, (Ptr) AtomLabels, 130*3*sizeof(char));
		//Atom radii
	ResID=130;
	WritePreference (fileRefNum, (ResType) 'AtmS', &ResID, (Ptr) AtomSizes, 130*sizeof(int));
		//Atom masses (stores as the square root of the actual mass)
	ResID=115;
	WritePreference (fileRefNum, (ResType) 'AtmM', &ResID, (Ptr) AtomMasses, 115*sizeof(float));
	ResID=128;
	WritePreference (fileRefNum, (ResType) 'EnPr', &ResID, (Ptr) &EnergyPlotOptions, sizeof(EnergyOptions));
	ResID=128;
	WritePreference (fileRefNum, (ResType) 'GrPr', &ResID, (Ptr) &PlotOptions, sizeof(GraphOptions));
		//Font preferences
	Str255	FontName;
#ifdef CarbonBuild
	FMGetFontFamilyName(LabelFontID, FontName);
#else
	GetFontName(LabelFontID, FontName);	// Store the font name -- not the ID
#endif
	ResID=1;
	WritePreference (fileRefNum, (ResType) 'AtmF', &ResID, (Ptr) &FontName, (FontName[0]+1)*sizeof(char));
	ResID=2;
	WritePreference (fileRefNum, (ResType) 'AtmF', &ResID, (Ptr) &LabelSize, sizeof(short));
		//Display related preferences (Note: different types mixed in)
	ResID=1;
	WritePreference (fileRefNum, (ResType) 'DisP', &ResID, (Ptr) &AnimationSpeed, sizeof(short));
	ResID=2;
	WritePreference (fileRefNum, (ResType) 'DisP', &ResID, (Ptr) &VectorScale, sizeof(float));
	ResID=3;
	WritePreference (fileRefNum, (ResType) 'DisP', &ResID, (Ptr) &AtomScale, sizeof(float));
	ResID=4;
	WritePreference (fileRefNum, (ResType) 'DisP', &ResID, (Ptr) &ZScale, sizeof(float));
	ResID=5;
	WritePreference (fileRefNum, (ResType) 'DisP', &ResID, (Ptr) &RotateMode, sizeof(Boolean));
	ResID=6;
	WritePreference (fileRefNum, (ResType) 'DisP', &ResID, (Ptr) &AnimateMode, sizeof(Boolean));
	ResID=7;
	WritePreference (fileRefNum, (ResType) 'DisP', &ResID, (Ptr) &AnimateTime, sizeof(long));
	ResID=9;
	WritePreference (fileRefNum, (ResType) 'DisP', &ResID, (Ptr) &BitOptions, sizeof(long));
	ResID=10;
	WritePreference (fileRefNum, (ResType) 'DisP', &ResID, (Ptr) &StereoOffset, sizeof(short));
		//Misc file open and new frame preferences
	ResID=1;
	WritePreference (fileRefNum, (ResType) 'FilP', &ResID, (Ptr) &AutoBondScale, sizeof(float));
	ResID=2;
	WritePreference (fileRefNum, (ResType) 'FilP', &ResID, (Ptr) &AutoBondFlag, sizeof(Boolean));
	ResID=3;
	WritePreference (fileRefNum, (ResType) 'FilP', &ResID, (Ptr) &HHBondFlag, sizeof(Boolean));
	ResID=4;
	WritePreference (fileRefNum, (ResType) 'FilP', &ResID, (Ptr) &DRCnFileSkip, sizeof(long));
	ResID=5;
	WritePreference (fileRefNum, (ResType) 'FilP', &ResID, (Ptr) &SetCreator, sizeof(Boolean));
	ResID=1;
	WritePreference (fileRefNum, (ResType) 'Surf', &ResID, (Ptr) &SurfaceOpts, sizeof(SurfaceOptions));
		//QD3D related preferences
	ResID=1;
	WritePreference (fileRefNum, (ResType) 'QD3P', &ResID, (Ptr) &BondWidth, sizeof(float));
	ResID=2;
	WritePreference (fileRefNum, (ResType) 'QD3P', &ResID, (Ptr) &QD3DAtomQuality, sizeof(long));
	ResID=3;
	WritePreference (fileRefNum, (ResType) 'QD3P', &ResID, (Ptr) &QD3DFillBrightness, sizeof(float));
	ResID=4;
	WritePreference (fileRefNum, (ResType) 'QD3P', &ResID, (Ptr) &QD3DPointBrightness, sizeof(float));
	ResID=5;
	WritePreference (fileRefNum, (ResType) 'QD3P', &ResID, (Ptr) &QD3DLineWidth, sizeof(float));
	ResID=6;
	WritePreference (fileRefNum, (ResType) 'AtmC', &ResID, (Ptr) &BackColor, sizeof(RGBColor));
*/
}

// Read the file preferences out of the already open resource fork of the data file
void WinPrefs::ReadFilePrefs(short fileRef) {
  /*
		//Read in the default atom colors
	myReadPreference(fileRef, fileRef, (ResType) 'AtmC', 4, (Ptr) BondColors, 4*sizeof(RGBColor));
	myReadPreference(fileRef, fileRef, (ResType) 'AtmC', 2, (Ptr) &VectorColor, sizeof(RGBColor));
	myReadPreference(fileRef, fileRef, (ResType) 'AtmC', 130, (Ptr) AtomColors, 130*sizeof(RGBColor));
		//Read in the default atom patterns
	myReadPreference(fileRef, fileRef, (ResType) 'AtmP', 4, (Ptr) BondPatterns, 4*sizeof(short));
	myReadPreference(fileRef, fileRef, (ResType) 'AtmP', 2, (Ptr) &VectorPattern, sizeof(short));
	myReadPreference(fileRef, fileRef, (ResType) 'AtmP', 130, (Ptr) AtomPatterns, 130*sizeof(short));
		//Atom Labels
	myReadPreference(fileRef, fileRef, (ResType) 'AtmL', 130, (Ptr) AtomLabels, 3*130*sizeof(char));
		//Atom radii
	myReadPreference(fileRef, fileRef, (ResType) 'AtmS', 130, (Ptr) AtomSizes, 130*sizeof(int));
		//Atom masses
	myReadPreference(fileRef, fileRef, (ResType) 'AtmM', 115, (Ptr) AtomMasses, 115*sizeof(float));
		// Energy Plot options
	myReadPreference(fileRef, fileRef, (ResType) 'EnPr', 128, (Ptr) &EnergyPlotOptions, sizeof(EnergyOptions));
	myReadPreference(fileRef, fileRef, (ResType) 'GrPr', 128, (Ptr) &PlotOptions, sizeof(GraphOptions));
	if (EnergyPlotOptions.GetYAdjustment() == 0)
		EnergyPlotOptions.SetYAdjustment(1.0);
		//Font Preferences
	Str255 FontName, TestFontName;
	myReadPreference(fileRef, fileRef, (ResType) 'AtmF', 1, (Ptr) FontName, -255);
	if (FontName[0] <= 0) {
		LabelFontID = systemFont;
	} else {
#ifdef CarbonBuild
		LabelFontID = FMGetFontFamilyFromName(FontName);
		FMGetFontFamilyName(LabelFontID, TestFontName);
#else
		GetFNum(FontName, &LabelFontID);
		GetFontName(LabelFontID, TestFontName);
#endif
		if (!(EqualString(FontName, TestFontName, false, false)))
			LabelFontID = systemFont;
	}
	myReadPreference(fileRef, fileRef, (ResType) 'AtmF', 2, (Ptr) &LabelSize, sizeof(short));
		//Display related preferences (Note: different types mixed in)
	myReadPreference(fileRef, fileRef, (ResType) 'DisP', 1, (Ptr) &AnimationSpeed, sizeof(short));
	myReadPreference(fileRef, fileRef, (ResType) 'DisP', 2, (Ptr) &VectorScale, sizeof(float));
	myReadPreference(fileRef, fileRef, (ResType) 'DisP', 3, (Ptr) &AtomScale, sizeof(float));
	myReadPreference(fileRef, fileRef, (ResType) 'DisP', 4, (Ptr) &ZScale, sizeof(float));
	myReadPreference(fileRef, fileRef, (ResType) 'DisP', 5, (Ptr) &RotateMode, sizeof(Boolean));
	myReadPreference(fileRef, fileRef, (ResType) 'DisP', 6, (Ptr) &AnimateMode, sizeof(Boolean));
	myReadPreference(fileRef, fileRef, (ResType) 'DisP', 7, (Ptr) &AnimateTime, sizeof(long));
	myReadPreference(fileRef, fileRef, (ResType) 'DisP', 9, (Ptr) &BitOptions, sizeof(long));
		//Misc file open and new frame preferences
	myReadPreference(fileRef, fileRef, (ResType) 'FilP', 1, (Ptr) &AutoBondScale, sizeof(float));
	myReadPreference(fileRef, fileRef, (ResType) 'FilP', 2, (Ptr) &AutoBondFlag, sizeof(Boolean));
	myReadPreference(fileRef, fileRef, (ResType) 'FilP', 3, (Ptr) &HHBondFlag, sizeof(Boolean));
	myReadPreference(fileRef, fileRef, (ResType) 'FilP', 4, (Ptr) &DRCnFileSkip, sizeof(long));
	myReadPreference(fileRef, fileRef, (ResType) 'FilP', 5, (Ptr) &SetCreator, sizeof(Boolean));
		//QuickDraw 3D related preferences
	myReadPreference(fileRef, fileRef, (ResType) 'QD3P', 1, (Ptr) &BondWidth, sizeof(float));
	myReadPreference(fileRef, fileRef, (ResType) 'QD3P', 2, (Ptr) &QD3DAtomQuality, sizeof(long));
	myReadPreference(fileRef, fileRef, (ResType) 'QD3P', 3, (Ptr) &QD3DFillBrightness, sizeof(float));
	myReadPreference(fileRef, fileRef, (ResType) 'QD3P', 4, (Ptr) &QD3DPointBrightness, sizeof(float));
	myReadPreference(fileRef, fileRef, (ResType) 'QD3P', 5, (Ptr) &QD3DLineWidth, sizeof(float));
	myReadPreference(fileRef, fileRef, (ResType) 'AtmC', 6, (Ptr) &BackColor, sizeof(RGBColor));
}
void WinPrefs::WriteFilePrefs(short fileRef) {
	WriteAllPrefs(fileRef);
}
void WinPrefs::SetAtomPattern(long AtomNum) {
	PenPat((ConstPatternParam) &((**gDitherPatterns).patList[(AtomPatterns[AtomNum])]));
  */ //Song Li
}

//Read the preference specified by the resource type and ID into the
//ptr passed in. Note the memory for preference MUST be allocated by the
//caller
/*OSErr myReadPreference(short fRefNum, short AppRefNum, ResType resourceType,
	short resourceID, Ptr preference, long size) {

	Handle lpreference=NULL;
	OSErr result;
		//First attempt to read the preference from the user pref file
	result = ReadPreference (fRefNum, resourceType, &resourceID, &lpreference);
		//If the preference could not be read in from the pref file get it
		//from the app defaults in the app resource fork if the user passed in a different id
	if ((result!=noErr)&&(fRefNum!=AppRefNum)) {
		if (lpreference) DisposeHandle(lpreference);
		result = ReadPreference (AppRefNum, resourceType, &resourceID, &lpreference);
	}
	if ((result==noErr)&&(size>=0)) {
		BlockMoveData(*lpreference, preference, size);
	} else if ((size < 0)&&(lpreference != NULL)) {	//Copy over whatever is there...
		long size2 = -size;
		size = GetHandleSize(lpreference);
		if ((size<=size2)&&(size>0)) {
			BlockMoveData(*lpreference, preference, size);
		} else result = -1;
	}
	if (lpreference) DisposeHandle(lpreference);
	return result;
	}*/


colorArea::colorArea(wxWindow* parent, int i)
{
  Create(parent, -1, wxDefaultPosition, wxSize(50, 20));
  mID = i;
  //mDC = new wxPaintDC(parent);
}

colorArea::~colorArea()
{
  //delete mDC;
}

void colorArea::draw(RGBColor* color)
{
  SetBackgroundColour(wxColour(color->red/256, color->green/256, color->blue/256));
  mCurrentColor = wxColour(color->red/256, color->green/256, color->blue/256);
}

wxColour& colorArea::getColor()
{
  return mCurrentColor;
}

void colorArea::OnMouse(wxMouseEvent &event)
{
  wxColourData clrData;
  clrData.SetColour(GetBackgroundColour());

  if (event.LeftDClick())
    {
      wxColourDialog dialog(this, &clrData);
      dialog.SetTitle(_T("Choose Color"));

      if (dialog.ShowModal() == wxID_OK)
	{
	  mCurrentColor = dialog.GetColourData().GetColour();
	  SetBackgroundColour(mCurrentColor);
	}
    }
}

AtomPrefsPane::AtomPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, Boolean Global) 
  : PrefsPane(targetWindow, parent, kAtomPrefsPane, Global) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER|wxScrolledWindowStyle );

  mLabels[0] = wxString("Atom");
  mLabels[1] = wxString("#");
  mLabels[2] = wxString("size(pm)");
  mLabels[3] = wxString("mass");
  mLabels[4] = wxString("Color");

  mMainSizer = new wxFlexGridSizer(kMaxAtomTypes, NUM_ATOM_LABELS, 3, 10);

  SetSizer(mMainSizer);

  SetScrollRate( 10, 10 );
  SetVirtualSize( 500, 1000 );
  FitInside();
}

AtomPrefsPane::~AtomPrefsPane()
{
  for ( int i = 0; i < kMaxAtomTypes; i++)
    {
      delete mEleNames[i];
      delete mEleSizes[i];
      delete mEleMasses[i];
      delete mColorArea[i];
    }

}

void AtomPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow, WinPrefs * targetPrefs) 
{
  mTargetPrefs = targetPrefs;

  for ( int i = 0; i < NUM_ATOM_LABELS; i++)
    mMainSizer->Add(new wxStaticText(
                            this,
                            wxID_ANY,
                            mLabels[i],
                            wxDefaultPosition,
                            wxDefaultSize,
                            wxALIGN_CENTER
                           ),
                       0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  //first row

  for ( int i = 0; i < kMaxAtomTypes; i++)
    {
      wxString tmp;

      targetPrefs->GetAtomLabel(i, tmp);
      mEleNames[i] = new wxTextCtrl( this, wxID_ANY, tmp, wxDefaultPosition, wxSize(30, 20));
      mMainSizer->Add(mEleNames[i], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

      tmp.Printf("%d", i+1);
      mMainSizer->Add(new wxStaticText(
                            this,
                            wxID_ANY,
                            tmp,
                            wxDefaultPosition,
                            wxDefaultSize,
                            wxALIGN_CENTER
                           ),
                       0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

      tmp.Printf("%d",targetPrefs->GetAtomSize(i));
      mEleSizes[i] = new wxTextCtrl( this, wxID_ANY, tmp, wxDefaultPosition, wxSize(40, 20));
      mMainSizer->Add(mEleSizes[i], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

      tmp.Printf("%.2f",targetPrefs->GetAtomMass(i));
      mEleMasses[i] = new wxTextCtrl( this, wxID_ANY, tmp, wxDefaultPosition, wxSize(50, 20));
      mMainSizer->Add(mEleMasses[i], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

      mColorArea[i] = new colorArea(this, i);
      mColorArea[i]->draw(targetPrefs->GetAtomColorLoc(i));
      mMainSizer->Add(mColorArea[i], 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
    }

  mMainSizer->Layout();
}

void AtomPrefsPane::saveToTempPrefs()
{
  long size;
  double mass;

  for ( int i = 0; i < kMaxAtomTypes; i++)
    {
      mTargetPrefs->SetAtomLabel(i, mEleNames[i]->GetValue());
      (mEleSizes[i]->GetValue()).ToLong(&size);
      mTargetPrefs->SetAtomSize(i, size);
      (mEleMasses[i]->GetValue()).ToDouble(&mass);
      mTargetPrefs->SetAtomMass(i, mass);
      mTargetPrefs->SetAtomColor(i, WX2RGB(mColorArea[i]->getColor()));
    }
}

BondPrefsPane::BondPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, Boolean Global)
	: PrefsPane(targetWindow, parent, kBondPrefsPane, Global) 
{
	Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

	mMainSizer = new wxBoxSizer(wxVERTICAL);
	mUpperSizer = new wxBoxSizer(wxVERTICAL);
	mSldSizer = new wxBoxSizer(wxHORIZONTAL);
	mBox = new wxStaticBoxSizer(wxHORIZONTAL, this);
	mInnerSizer = new wxFlexGridSizer(3, 3, 0, 0);
	mLowerSizer = new wxBoxSizer(wxVERTICAL);

	SetSizer(mMainSizer);
}

BondPrefsPane::~BondPrefsPane()
{
  delete mColorArea;
  delete mPatternArea;
  delete mNormColorArea;
  delete mNormPatternArea;
}

void BondPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow, WinPrefs * targetPrefs) 
{
  mTargetPrefs = targetPrefs;

  mChkAnim = new wxCheckBox(this, ID_SHOW_NORMAL_MODE_ANIMATION, _T("Show Normal Mode During Animation"), wxDefaultPosition);
  mChkAnim->SetValue(targetPrefs->GetAnimateMode());
  mChkRotation = new wxCheckBox(this, ID_SHOW_NORMAL_MODE_ROTATION, _T("Show Normal Mode During Rotation"), wxDefaultPosition);
  mChkRotation->SetValue(targetPrefs->GetRotateMode());

  mSldScale = new wxSlider( this, ID_NORMAL_MODE_SCALING_SLIDER, 
			    (int)(targetPrefs->GetVectorScale()*10 - 1), 0, 25,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord));

  mUpperSizer->Add(mChkAnim, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mUpperSizer->Add(mChkRotation, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mSldSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Normal Mode Scaling:")) ,0, wxALIGN_CENTER_VERTICAL | wxALL, 3);
  mSldSizer->Add(mSldScale, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mUpperSizer->Add(mSldSizer);

  wxString choices[] = { _T("Hydrogen Bonds"),
		       _T("Single Bonds"),
		       _T("Double Bonds"),
		       _T("Triple Bonds")
  };

  mBondChoice = new wxChoice(this, ID_BOND_MODE_CHOICE, wxPoint(10,10), wxSize(150,wxDefaultCoord), 4, choices);
  mBondChoice->SetSelection(1);
  mChoiceId = 1;

  mInnerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("")), 0, wxALIGN_RIGHT | wxALL, 3);
  mInnerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Color")), 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mInnerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Pattern")), 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mInnerSizer->Add(mBondChoice, 0, wxALIGN_RIGHT | wxALL, 3);

  mColorArea = new colorArea(this, 1);
  mColorArea->draw(targetPrefs->GetBondColorLoc(1));
  mPatternArea = new colorArea(this, 2);
  mPatternArea->draw(targetPrefs->GetBondColorLoc(1));
  //!!! don't know where to get patterns, use color 

  mNormColorArea = new colorArea(this, 3);
  mNormColorArea->draw(targetPrefs->GetVectorColorLoc());
  mNormPatternArea = new colorArea(this, 4);
  mNormPatternArea->draw(targetPrefs->GetVectorColorLoc());

  mInnerSizer->Add(mColorArea, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mInnerSizer->Add(mPatternArea, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

  mInnerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Normal Modes:")), 0, wxALIGN_RIGHT | wxALL, 3);
  mInnerSizer->Add(mNormColorArea, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);
  mInnerSizer->Add(mNormPatternArea, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

  mBox->Add(mInnerSizer);
  mLowerSizer->Add(mBox);

  mMainSizer->Add(mUpperSizer, 0, wxALIGN_CENTER | wxALL, 3);
  mMainSizer->Add(mLowerSizer, 0, wxALIGN_CENTER | wxALL, 3);
}

void BondPrefsPane::saveToTempPrefs()
{
  mTargetPrefs->SetBondColor(mChoiceId, WX2RGB(mColorArea->getColor()));
  mTargetPrefs->SetVectorColor(WX2RGB(mNormColorArea->getColor()));
}

void BondPrefsPane::OnSliderUpdate( wxCommandEvent &WXUNUSED(event) )
{
  mTargetPrefs->SetVectorScale((float)(0.1*mSldScale->GetValue()+1));
}

void BondPrefsPane::OnToggleAnim(wxCommandEvent& WXUNUSED(event))
{
  mTargetPrefs->SetAnimateMode(mChkAnim->GetValue());
}

void BondPrefsPane::OnToggleRotation(wxCommandEvent& WXUNUSED(event))
{
  mTargetPrefs->SetRotateMode(mChkRotation->GetValue());
}

void BondPrefsPane::OnChoice( wxCommandEvent &event )
{
  //int localId = -1;

  mTargetPrefs->SetBondColor(mChoiceId, WX2RGB(mColorArea->getColor())); //save the old setting first

  wxString localStr = event.GetString();

  if ( localStr.Cmp(_T("Hydrogen Bonds")) == 0)
    mChoiceId = 0;
  else if ( localStr.Cmp(_T("Single Bonds")) == 0)
    mChoiceId = 1;
  else if ( localStr.Cmp(_T("Double Bonds")) == 0)
    mChoiceId = 2;
  else if ( localStr.Cmp(_T("Triple Bonds")) == 0)
    mChoiceId = 3;

  mColorArea->draw(mTargetPrefs->GetBondColorLoc(mChoiceId));
  mPatternArea->draw(mTargetPrefs->GetBondColorLoc(mChoiceId));
}


DisplayPrefsPane::DisplayPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, Boolean Global)
	: PrefsPane(targetWindow, parent, kDisplayPrefsPane, Global) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER);

  mMainSizer = new wxBoxSizer(wxVERTICAL);
  mUpperSizer = new wxBoxSizer(wxHORIZONTAL);
  mLowerSizer = new wxBoxSizer(wxVERTICAL);

  wxString choices[] = {_T("Ball and Stick"), _T("WireFrame (Bonds only)")};
  
  mRdoBox = new wxRadioBox( this, ID_DISPLAY_MODE_RADIOBOX, _T("Display Mode"), wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices), choices, 1, wxRA_SPECIFY_COLS );
  mChkColor = new wxCheckBox(this, ID_COLOR_BONDS_BY_ATOM_COLOR, _T("Color bonds by atom color"), wxDefaultPosition);
  mChkOutline = new wxCheckBox(this, ID_OUTLINE_BONDS, _T("Outline Bonds"), wxDefaultPosition);

  SetSizer(mMainSizer);
}

#include <iostream>
void DisplayPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow, WinPrefs * targetPrefs) 
{
  mTargetPrefs = targetPrefs;

  if ( targetPrefs->DrawBallnStick() && !targetPrefs->DrawWireFrame())
    mRdoBox->SetSelection(0);
  else if ( !targetPrefs->DrawBallnStick() && targetPrefs->DrawWireFrame())
    mRdoBox->SetSelection(1);
  else
    cout<<"Something wrong! Check preference setting!"<<endl;

  mUpperSizer->Add(mRdoBox, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 3);

  mChkColor->SetValue(targetPrefs->ColorBondHalves());
  mChkOutline->SetValue(targetPrefs->OutLineBonds());
  mLowerSizer->Add(mChkColor, 1, wxALIGN_LEFT | wxALL, 3);
  mLowerSizer->Add(mChkOutline, 1, wxALIGN_LEFT | wxALL, 3);

  mMainSizer->Add(mUpperSizer, 0, wxALIGN_CENTER | wxALL, 3);
  mMainSizer->Add(mLowerSizer, 0, wxALIGN_CENTER | wxALL, 3);
}

void DisplayPrefsPane::saveToTempPrefs()
{

}

void DisplayPrefsPane::OnRadio( wxCommandEvent &event )
{
  wxString tmpStr = event.GetString();

  if (tmpStr.Cmp(_T("Ball and Stick")) == 0)
    {
      mTargetPrefs->DrawBallnStick(true);
      mTargetPrefs->DrawWireFrame(false);
    }
  else if (tmpStr.Cmp(_T("WireFrame (Bonds only)")) == 0)
    {
      mTargetPrefs->DrawWireFrame(true);
      mTargetPrefs->DrawBallnStick(false);
    }
}

void DisplayPrefsPane::OnCheckBox( wxCommandEvent &event)
{
  if (event.GetId() == ID_COLOR_BONDS_BY_ATOM_COLOR)
    mTargetPrefs->ColorBondHalves(mChkColor->GetValue());
  else if (event.GetId() == ID_OUTLINE_BONDS)
    mTargetPrefs->OutLineBonds(mChkOutline->GetValue());
}


EnergyPrefsPane::EnergyPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, parent, kEPrefsPane, GlobalPrefs) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

  mMainSizer = new wxBoxSizer(wxVERTICAL);
  mUpperSizer = new wxBoxSizer(wxHORIZONTAL);

  if (PrefsAreGlobal())
    mLowerSizer = new wxGridSizer(4,2);
  else
    mLowerSizer = new wxGridSizer(6,2);

  if (!PrefsAreGlobal())
    {
      mBottomSizer = new wxBoxSizer(wxHORIZONTAL);
      mLeftBottomSizer = new wxBoxSizer(wxVERTICAL);
    }

  mRightBottomSizer = NULL;
  mRight1BottomSizer = NULL;
  mRight2BottomSizer = NULL;
  mAtomText[0] = NULL;
  mAtomText[1] = NULL;
  mAtomText[2] = NULL;

  SetSizer(mMainSizer);

}

EnergyPrefsPane::~EnergyPrefsPane()
{
  delete mEColor;
  delete mMPColor;
  delete mPEColor;
  delete mOtherColor;
}

void EnergyPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow, WinPrefs * targetPrefs) 
{
  mTargetPrefs = targetPrefs;

  EnergyOptions * lEOpts = targetPrefs->GetEnergyOptions();
  GraphOptions * lPOpts = targetPrefs->GetGraphOptions();
  //Setup the slider for the E plot point size

  mSldBallSize = new wxSlider( this, ID_PLOT_BALL_SIZE_SLIDER, 
			    (int)(lEOpts->GetEPlotPointSize()), 1, 20,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord),
                             wxSL_AUTOTICKS | wxSL_LABELS);

  wxString tmp;
  tmp.Printf("%d", lEOpts->GetNumDigits());
  mNumDigitsArea = new wxTextCtrl( this, wxID_ANY, tmp, wxDefaultPosition, wxDefaultSize);

  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, wxString("E. Plot Ball Size:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mUpperSizer->Add(mSldBallSize, 0, wxALIGN_LEFT | wxALL, 3);
  mUpperSizer->Add(30, 30);
  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, wxString("# digits")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mUpperSizer->Add(mNumDigitsArea, 0, wxALIGN_CENTER | wxALL, 3);

  mEColor = new colorArea(this, 0);
  mEColor->draw(lEOpts->GetTEColor());
  mMPColor = new colorArea(this, 1);
  mMPColor->draw(lEOpts->GetMPColor());
  mPEColor = new colorArea(this, 2);
  mPEColor->draw(lEOpts->GetPEColor());
  mOtherColor = new colorArea(this, 3);
  mOtherColor->draw(lEOpts->GetKEColor());

  mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("E Color")), 0, wxALIGN_RIGHT | wxALL, 3);
  mLowerSizer->Add(mEColor, 0, wxALIGN_LEFT | wxALL, 3);
  mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("MP Color")), 0, wxALIGN_RIGHT | wxALL, 3);
  mLowerSizer->Add(mMPColor, 0, wxALIGN_LEFT | wxALL, 3);

  if (!PrefsAreGlobal())
    {
      mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Y1 zero pt ")), 0, wxALIGN_RIGHT | wxALL, 3);
      tmp.Printf("%.4f",lEOpts->GetY1Zero());
      mY1pt = new wxTextCtrl(this, wxID_ANY, tmp);
      mLowerSizer->Add(mY1pt, 0, wxALIGN_RIGHT | wxALL, 3);
    }

  mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("PE Color")), 0, wxALIGN_RIGHT | wxALL, 3);
  mLowerSizer->Add(mPEColor, 0, wxALIGN_LEFT | wxALL, 3);
  mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Other Color")), 0, wxALIGN_RIGHT | wxALL, 3);
  mLowerSizer->Add(mOtherColor, 0, wxALIGN_LEFT | wxALL, 3);

  if (!PrefsAreGlobal())
    {
      mLowerSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Y2 zero pt ")), 0, wxALIGN_RIGHT | wxALL, 3);
      tmp.Printf("%.4f",lEOpts->GetY2Zero());
      mY2pt = new wxTextCtrl(this, wxID_ANY, tmp);
      mLowerSizer->Add(mY2pt, 0, wxALIGN_RIGHT | wxALL, 3);
    }

  if (!PrefsAreGlobal())
    {
      mTotalEny = new wxCheckBox(this, ID_TOTAL_ENERGY, _T("Total Energy"));
      mTotalEny->SetValue(lEOpts->PlotEnergy());
      mMP2Eny = new wxCheckBox(this, ID_MP2_ENERGY, _T("MP2 Energy"));
      mMP2Eny->SetValue(lEOpts->PlotMPEnergy());
      mPotEny = new wxCheckBox(this, ID_POTENTIAL_ENERGY, _T("Potential Energy"));
      mPotEny->SetValue(lEOpts->PlotPEnergy());

      mLeftBottomSizer->Add(mTotalEny, 0, wxALIGN_LEFT | wxALL, 3);
      mLeftBottomSizer->Add(mMP2Eny, 0, wxALIGN_LEFT | wxALL, 3);
      mLeftBottomSizer->Add(mPotEny, 0, wxALIGN_LEFT | wxALL, 3);

      EnergyUnit lunit = lEOpts->GetDisplayUnits();

      wxString choices[] = {_T("Default"), _T("kcal/mol")};
  
      mRdoUnit = new wxRadioBox( this, ID_UNIT_RADIOBOX, _T("Unit"), wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices), choices, 1, wxRA_SPECIFY_COLS );

      if (lunit == kDefault)
	mRdoUnit->SetSelection(0);
      else if (lunit == kKCalPerMole)
	mRdoUnit->SetSelection(1);

      wxString choices2[] = {_T("None"), _T("Kinetic Energy"), _T("RMS Grad"), _T("Max. Grad"), _T("Bond Length"), _T("Bond Angle") };
  
      mRdoMisc = new wxRadioBox( this, ID_MISC_RADIOBOX, _T(""), wxDefaultPosition, wxDefaultSize, WXSIZEOF(choices2), choices2, 1, wxRA_SPECIFY_COLS );

      if (lEOpts->PlotOther())
	mRdoMisc->SetSelection(0);
      if (lEOpts->PlotKEnergy())
	mRdoMisc->SetSelection(1);
      if (lPOpts->PlotRMSGradient())
	mRdoMisc->SetSelection(2);
      if (lPOpts->PlotMaxGradient())
	mRdoMisc->SetSelection(3);
      if (lPOpts->PlotBondLength())
	mRdoMisc->SetSelection(4);
      if (lPOpts->PlotBondAngle())
	mRdoMisc->SetSelection(5);

      mLeftBottomSizer->Add(mRdoUnit, 0, wxALIGN_CENTER | wxALL, 3);
      mBottomSizer->Add(mLeftBottomSizer, 0, wxALL, 10);
      mBottomSizer->Add(mRdoMisc, 0, wxALIGN_CENTER | wxALL, 10);

      setHiddenCtrls();

      mRightBottomSizer->Hide(mRight1BottomSizer);
      mRightBottomSizer->Hide(mRight2BottomSizer); 
      //initially hide them

      if (lPOpts->PlotBondAngle())
	{
	  mRightBottomSizer->Show(mRight1BottomSizer, true, true);
	  mRightBottomSizer->Show(mRight2BottomSizer, true, true);
	  mRightBottomSizer->Layout();
	}

      if (lPOpts->PlotBondLength())
	{
	  mRightBottomSizer->Show(mRight1BottomSizer, true, true);
	  mRightBottomSizer->Layout();
	}

    }

  mMainSizer->Add(mUpperSizer);
  mMainSizer->Add(mLowerSizer);

  if (!PrefsAreGlobal())
    mMainSizer->Add(mBottomSizer, 0, wxALL, 10);

}

void EnergyPrefsPane::setHiddenCtrls()
{
  GraphOptions * lPOpts = mTargetPrefs->GetGraphOptions();

  if (!mRightBottomSizer)
    mRightBottomSizer = new wxBoxSizer(wxVERTICAL);

  if (!mRight1BottomSizer)
    mRight1BottomSizer = new wxGridSizer(2,3);

  if (!mRight2BottomSizer)
    mRight2BottomSizer = new wxGridSizer(2,1);

  wxString tmp;
  if (!mAtomText[0])
    {
      tmp.Printf("%d", lPOpts->Get1stAtom()+1);
      mAtomText[0] = new wxTextCtrl(this, wxID_ANY, tmp);
    }
  if (!mAtomText[1])
    {
      tmp.Printf("%d", lPOpts->Get2ndAtom()+1);
      mAtomText[1] = new wxTextCtrl(this, wxID_ANY, tmp);
    }

  mRight1BottomSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Atom 1:")), 0, wxALIGN_CENTER | wxALIGN_RIGHT | wxALL, 3);
  mRight1BottomSizer->Add(mAtomText[0], wxALIGN_CENTER | wxALL, 3);
  mRight1BottomSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Atom 2:")), 0, wxALIGN_CENTER | wxALIGN_RIGHT | wxALL, 3);
  mRight1BottomSizer->Add(mAtomText[1], wxALIGN_CENTER | wxALL, 3);

  if (!mAtomText[2])
    {
      tmp.Printf("%d", lPOpts->Get3rdAtom()+1);
      mAtomText[2] = new wxTextCtrl(this, wxID_ANY, tmp);
    }
  mRight2BottomSizer->Add(new wxStaticText(this, wxID_ANY, wxString("Atom 3:")), 0, wxALIGN_CENTER | wxALIGN_RIGHT | wxALL, 3);
  mRight2BottomSizer->Add(mAtomText[2], wxALIGN_CENTER | wxALL, 3);
	  
  mRightBottomSizer->Add(mRight1BottomSizer);
  mRightBottomSizer->Add(mRight2BottomSizer);
  mBottomSizer->Add(mRightBottomSizer);
}

void EnergyPrefsPane::saveToTempPrefs()
{
  EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();
  GraphOptions * lPOpts = mTargetPrefs->GetGraphOptions();

  long localLong;
  double localDouble;

  (mNumDigitsArea->GetValue()).ToLong(&localLong);
  lEOpts->SetNumDigits(localLong);

  lEOpts->SetTEColor(WX2RGB(mEColor->getColor()));
  lEOpts->SetMPColor(WX2RGB(mMPColor->getColor()));
  lEOpts->SetKEColor(WX2RGB(mOtherColor->getColor()));
  lEOpts->SetPEColor(WX2RGB(mPEColor->getColor()));

  if (!PrefsAreGlobal())
    {
      (mY1pt->GetValue()).ToDouble(&localDouble);
      lEOpts->SetY1Zero(localDouble);
      (mY2pt->GetValue()).ToDouble(&localDouble);
      lEOpts->SetY2Zero(localDouble);

      if (lPOpts->PlotBondLength() || lPOpts->PlotBondAngle())
	{
	  (mAtomText[0]->GetValue()).ToLong(&localLong);
	  lPOpts->Set1stAtom(localLong-1);
	  (mAtomText[1]->GetValue()).ToLong(&localLong);
	  lPOpts->Set2ndAtom(localLong-1);

	  if (lPOpts->PlotBondAngle())
	    {
	      (mAtomText[2]->GetValue()).ToLong(&localLong);
	      lPOpts->Set3rdAtom(localLong-1);
	    }
	}
    }
}

void EnergyPrefsPane::OnSliderUpdate( wxCommandEvent &WXUNUSED(event) )
{
  EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();
  lEOpts->SetEPlotPointSize(mSldBallSize->GetValue());
}

void EnergyPrefsPane::OnCheckBox( wxCommandEvent& event)
{
  int id = event.GetId();

  EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();

  if ( id == ID_TOTAL_ENERGY )
    lEOpts->SetPlotEnergy(mTotalEny->GetValue());
  if ( id == ID_MP2_ENERGY )
    lEOpts->SetPlotMPEnergy(mMP2Eny->GetValue());
  if ( id == ID_POTENTIAL_ENERGY )
    lEOpts->SetPlotPEnergy(mPotEny->GetValue());
}

void EnergyPrefsPane::OnRadio( wxCommandEvent& event )
{
  //int id = event.GetId();

  wxString tmpStr = event.GetString();

  mRightBottomSizer->Hide(mRight1BottomSizer);
  mRightBottomSizer->Hide(mRight2BottomSizer);

  if ( tmpStr.Cmp(_T("Bond Length")) == 0 || tmpStr.Cmp(_T("Bond Angle")) == 0)
    {
      mRightBottomSizer->Show(mRight1BottomSizer, true, true);

      if (tmpStr.Cmp(_T("Bond Angle")) == 0 )
	mRightBottomSizer->Show(mRight2BottomSizer, true, true);

      mRightBottomSizer->Layout();
    }

  EnergyOptions * lEOpts = mTargetPrefs->GetEnergyOptions();
  GraphOptions * lPOpts = mTargetPrefs->GetGraphOptions();

  if (event.GetId() == ID_UNIT_RADIOBOX)
    {
      EnergyUnit lunit;

      if (tmpStr.Cmp(_T("Default")) == 0)
	lunit = kDefault;
      else if (tmpStr.Cmp(_T("kcal/mol")) == 0)
	lunit = kKCalPerMole;

      lEOpts->SetDisplayUnits(lunit);
    }

  if (event.GetId() == ID_MISC_RADIOBOX)
    {
      lEOpts->SetPlotOther(false);
      lEOpts->SetPlotKEnergy(false);
      lPOpts->SetPlotRMSGradient(false);
      lPOpts->SetPlotMaxGradient(false);
      lPOpts->SetPlotBondLength(false);
      lPOpts->SetPlotBondAngle(false);
      //set everything to false first

      if (tmpStr.Cmp(_T("None")) == 0)
	lEOpts->SetPlotOther(true);
      else if (tmpStr.Cmp(_T("Kinetic Energy")) == 0)
	lEOpts->SetPlotKEnergy(true);
      else if (tmpStr.Cmp(_T("RMS Grad")) == 0)
	lPOpts->SetPlotRMSGradient(true);
      else if (tmpStr.Cmp(_T("Max. Grad")) == 0)
	lPOpts->SetPlotMaxGradient(true);
      else if (tmpStr.Cmp(_T("Bond Length")) == 0)
	lPOpts->SetPlotBondLength(true);
      else if (tmpStr.Cmp(_T("Bond Angle")) == 0)
	lPOpts->SetPlotBondAngle(true);
    }
  
}

FilePrefsPane::FilePrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, parent, kFilePrefsPane, GlobalPrefs) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

  mMainSizer = new wxBoxSizer(wxVERTICAL);
  mUpperSizer = new wxBoxSizer(wxVERTICAL);
  mSecondSizer = new wxFlexGridSizer(2,3);
  mMiddleSizer = new wxFlexGridSizer(2,3);
  mLowerSizer = new wxBoxSizer(wxHORIZONTAL);
  mBottomSizer = new wxBoxSizer(wxHORIZONTAL);

  SetSizer(mMainSizer);
}

void FilePrefsPane::SetupPaneItems(MolDisplayWin* targetWindow, WinPrefs * targetPrefs) 
{
  mTargetPrefs = targetPrefs;

  mChkBox[0] = new wxCheckBox(this, ID_AUTO_BOND_PASTE_OPEN, _T("Auto-Bond on Paste and File Open"));
  mChkBox[0]->SetValue(targetPrefs->GetAutoBond());

  mChkBox[1] = new wxCheckBox(this, ID_GUESS_BOND_ORDER, _T("Guess bond order"));
  mChkBox[1]->SetValue(targetPrefs->DetermineBondOrder());

  mChkBox[2] = new wxCheckBox(this, ID_PREVENT_HH_BONDS, _T("Prevent H-H bonds in Auto-Bond"));
  mChkBox[2]->SetValue(targetPrefs->GetHHBondFlag());

  mChkBox[3] = new wxCheckBox(this, ID_LOOK_HYDROGEN_BONDS, _T("Look for hydrogen bonds"));
  mChkBox[3]->SetValue(targetPrefs->AllowHydrogenBonds());
	
  mChkBox[4] = new wxCheckBox(this, ID_SHOW_ANGLES_WHILE_ROTATING, _T("Show angles while rotating"));
  mChkBox[4]->SetValue(targetPrefs->GetShowAngles());

if (PrefsAreGlobal())
    {
      mChkBox[5] = new wxCheckBox(this, ID_USE_MAC_EOL_CHAR, _T("Use Mac EOL chars"));
      mChkBox[5]->SetValue(targetPrefs->NativeEOLChar());
    }

  mChkBox[6] = new wxCheckBox(this, ID_PROMPT_SAVE_FILE, _T("Prompt to save file"));
  mChkBox[6]->SetValue(targetPrefs->GetPrompt4Save());


  mChkBox[7] = new wxCheckBox(this, ID_CREATE_CUSTOM_FILE_ICON, _T("create custom file icon"));
  mChkBox[7]->SetValue(targetPrefs->CreateCustomIcon());

  if (PrefsAreGlobal())
    {
      mChkBox[8] = new wxCheckBox(this, ID_CHANGE_FILE_CREATOR, _T("Change file creator type to wxMacMolPlt"));
      mChkBox[8]->SetValue(targetPrefs->ChangeFileType());
    }

  mUpperSizer->Add(mChkBox[0], 0, wxALIGN_LEFT | wxALL, 3);

  mSecondSizer->Add(30, 30);
  mSecondSizer->Add(mChkBox[1], 0, wxALIGN_LEFT | wxALL, 3);
  mSecondSizer->Add(30, 30);
  mSecondSizer->Add(mChkBox[2], 0, wxALIGN_LEFT | wxALL, 3);
  mSecondSizer->Add(30, 30);
  mSecondSizer->Add(mChkBox[3], 0, wxALIGN_LEFT | wxALL, 3);

  mMiddleSizer->Add(mChkBox[4], 0, wxALIGN_LEFT | wxALL, 3);

  if (PrefsAreGlobal())
    mMiddleSizer->Add(mChkBox[5], 0, wxALIGN_LEFT | wxALL, 3);

  mMiddleSizer->Add(mChkBox[6], 0, wxALIGN_LEFT | wxALL, 3);
  mMiddleSizer->Add(mChkBox[7], 0, wxALIGN_LEFT | wxALL, 3);

  if (PrefsAreGlobal())
    mMiddleSizer->Add(mChkBox[8], 0, wxALIGN_LEFT | wxALL, 3);

  mSldTol = new wxSlider( this, ID_AUTO_BOND_TOLERANCE_SLIDER, 
			  (int)(targetPrefs->GetAutoBondScale()*10000+0.5), 
			  50, 150, wxDefaultPosition, 
			  wxSize(155,wxDefaultCoord));

  mLowerSizer->Add(new wxStaticText(this, wxID_ANY, _T("Auto-Bond Tolerance:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mLowerSizer->Add(mSldTol, 0, wxALIGN_LEFT | wxALL, 3);

  wxString tmp;
  tmp.Printf("%d", targetPrefs->GetDRCSkip());
  mPointSkip = new wxTextCtrl(this, wxID_ANY, tmp);

  mBottomSizer->Add(new wxStaticText(this, wxID_ANY, _T("When reading a DRC file how many\n points should be skipped between\n points read in? (0 reads every point)")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mBottomSizer->Add(mPointSkip, 0, wxALIGN_CENTER | wxALL, 3);

  mMainSizer->Add(mUpperSizer);
  mMainSizer->Add(mSecondSizer);
  mMainSizer->Add(mMiddleSizer);
  mMainSizer->Add(mLowerSizer);
  mMainSizer->Add(mBottomSizer);

}

void FilePrefsPane::saveToTempPrefs()
{
  long v;
  (mPointSkip->GetValue()).ToLong(&v);
  mTargetPrefs->SetDRCSkip(v);
}

void FilePrefsPane::OnCheckBox( wxCommandEvent &event)
{
  if (event.GetId() == ID_AUTO_BOND_PASTE_OPEN)
    {
      Boolean mainStatus = mChkBox[0]->GetValue();
      mTargetPrefs->SetAutoBond(mainStatus);

      if (!mainStatus)
	{
	  mChkBox[1]->Disable();
	  mChkBox[2]->Disable();
	  mChkBox[3]->Disable();
	}
      else
	{
	  mChkBox[1]->Enable();
	  mChkBox[2]->Enable();
	  mChkBox[3]->Enable();
	}
    }
  else if (event.GetId() == ID_GUESS_BOND_ORDER)
    mTargetPrefs->DetermineBondOrder(mChkBox[1]->GetValue());
  else if (event.GetId() == ID_PREVENT_HH_BONDS)
    mTargetPrefs->SetHHBondFlag(mChkBox[2]->GetValue());
  else if (event.GetId() == ID_LOOK_HYDROGEN_BONDS)
    mTargetPrefs->AllowHydrogenBonds(mChkBox[3]->GetValue());
  else if (event.GetId() == ID_SHOW_ANGLES_WHILE_ROTATING)
    mTargetPrefs->SetShowAngles(mChkBox[4]->GetValue());
  else if (event.GetId() == ID_USE_MAC_EOL_CHAR && PrefsAreGlobal())
    mTargetPrefs->NativeEOLChar(mChkBox[5]->GetValue());
  else if (event.GetId() == ID_PROMPT_SAVE_FILE)
    mTargetPrefs->SetPrompt4Save(mChkBox[6]->GetValue());
  else if (event.GetId() == ID_CREATE_CUSTOM_FILE_ICON)
    mTargetPrefs->CreateCustomIcon(mChkBox[7]->GetValue());
  else if (event.GetId() == ID_CHANGE_FILE_CREATOR)
    mTargetPrefs->ChangeFileType(mChkBox[8]->GetValue());

}

void FilePrefsPane::OnSliderUpdate( wxCommandEvent &WXUNUSED(event) )
{
  mTargetPrefs->SetAutoBondScale((float)(mSldTol->GetValue())/10000);
}


ScalingPrefsPane::ScalingPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, parent, kScalingPrefsPane, GlobalPrefs) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

  mMainSizer = new wxFlexGridSizer(2,4);

  SetSizer(mMainSizer);
}

void ScalingPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow, WinPrefs * targetPrefs) 
{
  mTargetPrefs = targetPrefs;

  mSld[0] = new wxSlider( this, ID_ATOM_SIZE_SLIDER, 
			  (int)(targetPrefs->GetAtomScale()*10000+0.5), 
			  0, 250, wxDefaultPosition, 
			  wxSize(155,wxDefaultCoord));

  mSld[1] = new wxSlider( this, ID_DEPTH_CUEING_SLIDER, 
			  (int)(targetPrefs->GetZScale()*100+0.5), 
			  0, 70, wxDefaultPosition, 
			  wxSize(155,wxDefaultCoord));

  mSld[2] = new wxSlider( this, ID_ANIM_QUALITY_SLIDER, 
			  (int)(targetPrefs->GetAnimationSpeed()-1), 
			  0, 15, wxDefaultPosition, 
			  wxSize(155,wxDefaultCoord));

  mSld[3] = new wxSlider( this, ID_FRAME_DELAY_SLIDER, 
			  (int)(targetPrefs->GetAnimateTime()), 
			  0, 120, wxDefaultPosition, 
			  wxSize(155,wxDefaultCoord));

  mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Atom Size:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mMainSizer->Add(mSld[0], 0, wxALIGN_LEFT | wxALL, 3);
  mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Depth Cueing:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mMainSizer->Add(mSld[1], 0, wxALIGN_LEFT | wxALL, 3);
  mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Mode Animation Quality:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mMainSizer->Add(mSld[2], 0, wxALIGN_LEFT | wxALL, 3);
  mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Frame Delay:")), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 3);
  mMainSizer->Add(mSld[3], 0, wxALIGN_LEFT | wxALL, 3);
}

void ScalingPrefsPane::saveToTempPrefs()
{

}

void ScalingPrefsPane::OnSliderUpdate( wxCommandEvent &event )
{
  int id = event.GetId();

  if (id == ID_ATOM_SIZE_SLIDER)
    mTargetPrefs->SetAtomScale((float)(mSld[0]->GetValue())/10000);
  if (id == ID_DEPTH_CUEING_SLIDER)
    mTargetPrefs->SetZScale((float)(mSld[1]->GetValue())/100);
  if (id == ID_ANIM_QUALITY_SLIDER)
    mTargetPrefs->SetAnimationSpeed(mSld[2]->GetValue()+1);
  if (id == ID_FRAME_DELAY_SLIDER)
    mTargetPrefs->SetAnimateTime(mSld[3]->GetValue());
}

StereoPrefsPane::StereoPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, parent, kStereoPrefsPane, GlobalPrefs) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

  mMainSizer = new wxBoxSizer(wxVERTICAL);
  mMiddleSizer = new wxBoxSizer(wxHORIZONTAL);

  SetSizer(mMainSizer);
}

void StereoPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow, WinPrefs * targetPrefs) 
{
  mTargetPrefs = targetPrefs;

  mChkActive = new wxCheckBox(this, ID_STEREO_ACTIVE, _T("Stereo Active"), wxDefaultPosition);
  mChkActive->SetValue(targetPrefs->UseStereo());
  mMainSizer->Add(mChkActive, 0, wxALIGN_LEFT | wxALL, 10);
  mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Enter the number of degrees of offset between the stereo images.")), 0, wxALIGN_LEFT | wxALL, 10);

  mMiddleSizer->Add(new wxStaticText(this, wxID_ANY, _T("Offset (degrees) ")), 0, wxALIGN_LEFT | wxALL, 10);

  wxString tmp;
  tmp.Printf("%d", targetPrefs->GetStereoOffset());
  mOffDegree = new wxTextCtrl( this, wxID_ANY, tmp);

  mMiddleSizer->Add(mOffDegree, 0, wxALIGN_LEFT | wxALL, 3);

  mMainSizer->Add(mMiddleSizer, 0, wxALIGN_LEFT | wxLEFT, 10);
  mMainSizer->Add(new wxStaticText(this, wxID_ANY, _T("Hint: Enter a + number for cross-eyed viewing\n or a - number for straight on viewing.")), 0, wxALIGN_LEFT | wxLEFT, 10);

}

void StereoPrefsPane::saveToTempPrefs()
{
  long v;
  (mOffDegree->GetValue()).ToLong(&v);
  mTargetPrefs->SetStereoOffset(v);
}

void StereoPrefsPane::OnCheckBox( wxCommandEvent& WXUNUSED(event))
{
  mTargetPrefs->UseStereo(mChkActive->GetValue());
}


SurfacePrefsPane::SurfacePrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, parent, kSurfacePrefsPane, GlobalPrefs) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

  mMainSizer = new wxBoxSizer(wxHORIZONTAL);
  mLeftSizer = new wxBoxSizer(wxVERTICAL);
  mRightSizer = new wxBoxSizer(wxVERTICAL);
  mLeftMidSizer = new wxGridSizer(2,2);
  mRightUpperSizer = new wxBoxSizer(wxHORIZONTAL);
  mRightMidSizer = new wxFlexGridSizer(2,2);

  SetSizer(mMainSizer);
}

SurfacePrefsPane::~SurfacePrefsPane()
{
  delete mSurfColor[0];
  delete mSurfColor[1];
}

void SurfacePrefsPane::SetupPaneItems( MolDisplayWin* targetWindow, WinPrefs * targetPrefs) 
{
  mTargetPrefs = targetPrefs;

  SurfaceOptions * lSOpts = targetPrefs->GetSurfaceOptions();

  mLeftSizer->Add(new wxStaticText(this, wxID_ANY, _T("Surface Colors:")), 0, wxALIGN_CENTER | wxALL, 3);

  mLeftMidSizer->Add(new wxStaticText(this, wxID_ANY, _T("+")), 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_BOTTOM | wxALL, 1);
  mLeftMidSizer->Add(new wxStaticText(this, wxID_ANY, _T("-")), 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_BOTTOM | wxALL, 1);

  RGBColor* tmpColor = new RGBColor();

  mSurfColor[0] = new colorArea(this, 0);
  lSOpts->GetPosColor(tmpColor);
  mSurfColor[0]->draw(tmpColor);
  mSurfColor[1] = new colorArea(this, 1);
  lSOpts->GetNegColor(tmpColor);
  mSurfColor[1]->draw(tmpColor);

  mLeftMidSizer->Add(mSurfColor[0], 0, wxALIGN_TOP | wxALL, 1);
  mLeftMidSizer->Add(mSurfColor[1], 0, wxALIGN_TOP | wxALL, 1);

  mLeftSizer->Add(mLeftMidSizer, 0, wxALIGN_CENTER | wxALL, 3);
  mLeftSizer->Add(10,10);

  mLeftSizer->Add(new wxStaticText(this, wxID_ANY, _T("Def. # of Grid Points")), 0, wxALIGN_LEFT | wxALL, 3);

  mSldGridPoint = new wxSlider( this, ID_NUM_GRID_POINT_SLIDER, 
			    lSOpts->GetNumGridPoints(), 10, 150,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord),
                             wxSL_AUTOTICKS | wxSL_LABELS);

  mLeftSizer->Add(mSldGridPoint, 0, wxALIGN_CENTER | wxALL, 3);

  mRightSizer->Add(new wxStaticText(this, wxID_ANY, _T("Specific to 3D surfaces:")), 0, wxALIGN_LEFT | wxALL, 3);

  mRightUpperSizer->Add(new wxStaticText(this, wxID_ANY, _T("Grid Size:")), 0, wxALIGN_CENTER | wxALL, 3);

  mSldGridSize = new wxSlider( this, ID_3D_GRID_SIZE_SLIDER, 
			    (int)(100*(lSOpts->GetGridSize())+0.5), 0, 300,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord),
                             wxSL_AUTOTICKS | wxSL_LABELS);
  mRightUpperSizer->Add(mSldGridSize, 0, wxALIGN_CENTER | wxALL, 3);
  mRightSizer->Add(mRightUpperSizer);

  mRightSizer->Add(new wxStaticText(this, wxID_ANY, _T("Specific to 2D maps:")), 0, wxALIGN_LEFT | wxALL, 3);

  mRightMidSizer->Add(new wxStaticText(this, wxID_ANY, _T("# of Contours:")), 0, wxALIGN_CENTER | wxALL, 3);
  
  wxString tmp;
  tmp.Printf("%d", lSOpts->GetNumContours());
  mNumContour = new wxTextCtrl(this, wxID_ANY, tmp);
  mRightMidSizer->Add(mNumContour, 0, wxALIGN_CENTER | wxALL, 3);

 mRightMidSizer->Add(new wxStaticText(this, wxID_ANY, _T("Max contour value")), 0, wxALIGN_CENTER | wxALL, 3);

  tmp.Printf("%.3f", lSOpts->GetMaxContour());
  mMaxContourValue = new wxTextCtrl(this, wxID_ANY, tmp);
  mRightMidSizer->Add(mMaxContourValue, 0, wxALIGN_CENTER | wxALL, 3);			       
  mRightSizer->Add(mRightMidSizer);

  mZeroContour = new wxCheckBox(this, ID_SHOW_ZERO_VALUE_CONTOUR, _T("Show Zero-value contour"));
  mZeroContour->SetValue(lSOpts->GetShowZeroContour());
  mAtomPosition = new wxCheckBox(this, ID_SHOW_ATOM_POSITION, _T("Show atom positions"));
  mAtomPosition->SetValue(lSOpts->Get2DHashMarks());

  mRightSizer->Add(mZeroContour,  0, wxALIGN_LEFT | wxALL, 3);
  mRightSizer->Add(mAtomPosition,  0, wxALIGN_LEFT | wxALL, 3);

  mMainSizer->Add(mLeftSizer, wxALIGN_CENTER | wxALL, 10);
  mMainSizer->Add(mRightSizer, wxALIGN_CENTER | wxALL, 10);
}

void SurfacePrefsPane::saveToTempPrefs()
{
  SurfaceOptions * lSOpts = mTargetPrefs->GetSurfaceOptions();

  long lv;
  double dv;

  (mNumContour->GetValue()).ToLong(&lv);
  lSOpts->SetNumContours(lv);
  (mMaxContourValue->GetValue()).ToDouble(&dv);
  lSOpts->SetMaxContour(dv);

  lSOpts->SetPosColor(WX2RGB(mSurfColor[0]->getColor()));
  lSOpts->SetNegColor(WX2RGB(mSurfColor[1]->getColor()));
}

void SurfacePrefsPane::OnSliderUpdate( wxCommandEvent& event)
{
  SurfaceOptions * lSOpts = mTargetPrefs->GetSurfaceOptions();
  int id = event.GetId();

  if ( id == ID_NUM_GRID_POINT_SLIDER )
    lSOpts->SetNumGridPoints(mSldGridPoint->GetValue());
  if ( id == ID_3D_GRID_SIZE_SLIDER )
    lSOpts->SetGridSize((float)(mSldGridSize->GetValue()/100));
}

void SurfacePrefsPane::OnCheckBox( wxCommandEvent& event)
{
  SurfaceOptions * lSOpts = mTargetPrefs->GetSurfaceOptions();
  int id = event.GetId();

  if ( id == ID_SHOW_ZERO_VALUE_CONTOUR )
    lSOpts->SetShowZeroContour(mZeroContour->GetValue());
  if ( id == ID_SHOW_ATOM_POSITION )
    lSOpts->Set2DHashMarks(mAtomPosition->GetValue());
}


QD3DPrefsPane::QD3DPrefsPane(MolDisplayWin* targetWindow, wxBookCtrlBase *parent, Boolean GlobalPrefs)
	: PrefsPane(targetWindow, parent, kQD3DPrefsPane, GlobalPrefs) 
{
  Create(parent, -1, wxDefaultPosition, wxDefaultSize,wxSUNKEN_BORDER );

  mMainSizer = new wxBoxSizer(wxVERTICAL);
  mUpperSizer = new wxGridSizer(2,5);
  mLowerSizer = new wxBoxSizer(wxHORIZONTAL);

  SetSizer(mMainSizer);
}

void QD3DPrefsPane::SetupPaneItems(MolDisplayWin* targetWindow, WinPrefs * targetPrefs) 
{
  mTargetPrefs = targetPrefs;

  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, _T("Bond Size:")), 0, wxALIGN_CENTER | wxALL, 3);
  mSld[0] = new wxSlider( this, ID_BOND_SIZE_SLIDER, 
			    (int)(targetPrefs->GetQD3DBondWidth()*500+0.5), 1, 100,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord));
  mUpperSizer->Add(mSld[0], 0, wxALIGN_CENTER | wxALL, 3);

  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, _T("Display Quality:")), 0, wxALIGN_CENTER | wxALL, 3);
  mSld[1] = new wxSlider( this, ID_DISPLAY_QUALITY_SLIDER, 
			    (int)(targetPrefs->GetQD3DAtomQuality()+0.5), 2, 40,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord));
  mUpperSizer->Add(mSld[1], 0, wxALIGN_CENTER | wxALL, 3);

  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, _T("Fill Light Brightness:")), 0, wxALIGN_CENTER | wxALL, 3);
  mSld[2] = new wxSlider( this, ID_FILL_LIGHT_BRIGHTNESS_SLIDER, 
		       (int)(targetPrefs->GetQD3DFillBrightness()*100+0.5), 0, 100,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord));
  mUpperSizer->Add(mSld[2], 0, wxALIGN_CENTER | wxALL, 3);

  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, _T("Point Light Brightness:")), 0, wxALIGN_CENTER | wxALL, 3);
  mSld[3] = new wxSlider( this, ID_POINT_LIGHT_BRIGHTNESS_SLIDER, 
		      (int)(targetPrefs->GetQD3DPointBrightness()*100+0.5), 0, 100,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord));
  mUpperSizer->Add(mSld[3], 0, wxALIGN_CENTER | wxALL, 3);

  mUpperSizer->Add(new wxStaticText(this, wxID_ANY, _T("High-Resolution Line Width:")), 0, wxALIGN_CENTER | wxALL, 3);
  targetPrefs->CylindersForLines(true);
  mSld[4] = new wxSlider( this, ID_LINE_WIDTH_SLIDER, 
		      (int)(targetPrefs->GetQD3DLineWidth()*10000+0.5), 0, 200,
                             wxDefaultPosition, wxSize(155,wxDefaultCoord));
  targetPrefs->CylindersForLines(false);
  mUpperSizer->Add(mSld[4], 0, wxALIGN_CENTER | wxALL, 3);

  mMainSizer->Add(mUpperSizer, 0, wxALIGN_CENTER | wxALL, 3 );

  if (PrefsAreGlobal())
    {
      mChk3D = new wxCheckBox( this, ID_ACTIVATE_3D_MODE, _T("Activate 3D mode by default"));
      mChk3D->SetValue(targetPrefs->Default3DOn());
      mMainSizer->Add(mChk3D, 0, wxALIGN_CENTER | wxALL, 3);
    }
	
  mLowerSizer->Add(new wxStaticText(this, wxID_ANY, _T("Background color:")), 0, wxALIGN_CENTER | wxALL, 3);
  mBackgrdColor = new colorArea(this, 0);
  mBackgrdColor->draw(targetPrefs->GetBackgroundColorLoc());
  mLowerSizer->Add(mBackgrdColor, 0, wxALIGN_CENTER | wxALL, 3);

  mMainSizer->Add(mLowerSizer, 0, wxALIGN_CENTER | wxALL, 3);

}

void QD3DPrefsPane::saveToTempPrefs()
{
  mTargetPrefs->SetBackgroundColorLoc(WX2RGB(mBackgrdColor->getColor()));
}

void QD3DPrefsPane::OnCheckBox( wxCommandEvent& WXUNUSED(event))
{
  mTargetPrefs->Default3DOn(mChk3D->GetValue());
}

void QD3DPrefsPane::OnSliderUpdate( wxCommandEvent &event )
{
  int id = event.GetId();

  if (id == ID_BOND_SIZE_SLIDER)
    mTargetPrefs->SetQD3DBondWidth((float)(mSld[0]->GetValue())/500);
  if (id == ID_DISPLAY_QUALITY_SLIDER)
    mTargetPrefs->SetQD3DAtomQuality(mSld[1]->GetValue());
  if (id == ID_FILL_LIGHT_BRIGHTNESS_SLIDER)
    mTargetPrefs->SetQD3DFillBrightness((float)(mSld[2]->GetValue())/100);
  if (id == ID_POINT_LIGHT_BRIGHTNESS_SLIDER)
    mTargetPrefs->SetQD3DPointBrightness((float)(mSld[3]->GetValue())/100);
  if (id == ID_LINE_WIDTH_SLIDER)
    mTargetPrefs->SetQD3DLineWidth((float)(mSld[4]->GetValue())/10000);
}
