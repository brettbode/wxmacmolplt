/*
 *  (c) 2005 Iowa State University
 *      see the LICENSE file in the top level directory
 */
/*  CML.h
 *
 *  Headers related to MacMolPlt preferences file parsing
 *  Created by Brett Bode June 2005
 */

#ifndef _MMP_PREFS_H_
#define _MMP_PREFS_H_

enum MMPPref_Element {
	invalid_MMPPref_Element=0,
	MMPPref_Wrapper,
	MMPPref_MolDisplayPrefs,
	MMPPref_FileOptions,
	MMPPref_AtomPrefs,
	MMPPref_AtomElement,
	MMPPref_CoordWinPrefs,
	MMPPref_BondsWinPrefs,
	MMPPref_EnergyWinPrefs,
	MMPPref_FreqWinPrefs,
	MMPPref_SurfaceWinPrefs,
	MMPPref_WindowDefaults,
	
	NumberMMPPrefElements
};

enum MMPPref_Color {
	invalid_Color_Element=0,
	MMPPref_ColorXML,
	MMPPref_ColorRed,
	MMPPref_ColorGreen,
	MMPPref_ColorBlue,
	MMPPref_Pattern,
	
	NumberMMPColorElements
};

enum MMPref_WinRect {
	invalid_WinRect_Element=0,
	MMPPref_Rect_top,
	MMPPref_Rect_left,
	MMPPref_Rect_bottom,
	MMPPref_Rect_right,
	MMPPref_Window_Open,
	
	NumberMMPPrefWinRectElements
};

enum MMPFileOpts {
	invalid_MMPFileOpt=0,
	MMPFileOpt_SetCreator,
	MMPFileOpt_CustomIcon,
	MMPFileOpt_Prompt4Save,
	MMPFileOpt_EOLCharMode,
	MMPFileOpt_SetBondOrder,
	MMPFileOpt_CreateHBonds,
	MMPFileOpt_DRCSkipPoint,
	MMPFileOpt_AutoBond,
	MMPFileOpt_AutoBondTolerance,
	MMPFileOpt_MaxBondLength,
	MMPFileOpt_PreventHHBonds,
	
	NumberMMPFileOpts
};

enum MMPAtomAttribs {
	invalid_MMPAtomAttrib=0,
	MMPAtomAtt_AtomicNumber,
	MMPAtomAtt_AtomicSize,
	MMPAtomAtt_AtomicMass,
	MMPAtomAtt_AtomLabel,
	
	NumberMMPAtomAttribs
};

enum MMPMolDisplayElments {
	invalid_MMPMolDisplayElement=0,
	MMPMolDisplay_BackColor,
	MMPMolDisplay_VectorColor,
	MMPMolDisplay_BondColor,
	MMPMolDisplay_BondOrder,
	MMPMolDisplay_AnimateTime,
	MMPMolDisplay_3DAtomQuality,
	MMPMolDisplay_BondWidth,
	MMPMolDisplay_AtomScale,
	MMPMolDisplay_GLFOV,
	MMPMolDisplay_VectorScale,
	MMPMolDisplay_3DFillBrightness,
	MMPMolDisplay_3DPointBrightness,
	MMPMolDisplay_3DLineWidth,
	MMPMolDisplay_LabelFontID,
	MMPMolDisplay_LabelSize,
	MMPMolDisplay_AtomLabelSize,
	MMPMolDisplay_AnnotateSize,
	MMPMolDisplay_AnimationSpeed,
	MMPMolDisplay_StereoOffset,
	MMPMolDisplay_RotateMode,
	MMPMolDisplay_ShaderMode,
	MMPMolDisplay_AnimateMode,
	MMPMolDisplay_ShowAngles,
	MMPMolDisplay_Stereo,
	MMPMolDisplay_ColorHalfBonds,
	MMPMolDisplay_WireframeMode,
	MMPMolDisplay_EFPWireframeMode,
	MMPMolDisplay_OutlineBonds,
	MMPMolDisplay_UseCylinders,
	MMPMolDisplay_Use3DByDefault,
	MMPMolDisplay_Use3DHardwareAccel,
	MMPMolDisplay_ShowSymbolLabels,
	MMPMolDisplay_ShowAtomNumbers,
	MMPMolDisplay_ShowSymmetryOps,
	MMPMolDisplay_ShowAtomPatterns,
	MMPMolDisplay_ShowToolbar,
	MMPMolDisplay_AutoRotateEnabled,
	MMPMolDisplay_AutoRotating,
	MMPMolDisplay_AutoRotationX,
	MMPMolDisplay_AutoRotationY,
	MMPMolDisplay_AutoRotationZ,
	
	NumberMMPMolDisplayElements
};

enum MMPSurfaceWinElements {
	invalid_MMPSurfaceElement=0,
	MMPSurfWin_Rect,
	MMPSurfWin_NumContours,
	MMPSurfWin_NumGridPoints,
	MMPSurfWin_GridSize,
	MMPSurfWin_MaxContourValue,
	MMPSurfWin_PosColor,
	MMPSurfWin_NegColor,
	MMPSurfWin_ShowZeroContour,
	MMPSurfWin_Rotate2DMap,
	MMPSurfWin_Show2DHashMarks,
	
	NumberMMPSurfWinElements
};

enum MMPEnergyOptions {
	invalid_MMPEnergyOption=0,
	MMPEnergyOpt_y1Zero,
	MMPEnergyOpt_y2Zero,
	MMPEnergyOpt_yAdjustFactor,
	MMPEnergyOpt_xAdjustFactor,
	MMPEnergyOpt_indicatorSize,
	MMPEnergyOpt_NumDigits,
	MMPEnergyOpt_TEColor,
	MMPEnergyOpt_MPColor,
	MMPEnergyOpt_KEColor,
	MMPEnergyOpt_PEColor,
	MMPEnergyOpt_PlotTE,
	MMPEnergyOpt_PlotMP,
	MMPEnergyOpt_PlotKE,
	MMPEnergyOpt_PlotPE,
	MMPEnergyOpt_Plotkcal,
	MMPEnergyOpt_PlotRMSGrad,
	MMPEnergyOpt_PlotMaxGrad,
	MMPEnergyOpt_PlotBondLength,
	MMPEnergyOpt_PlotBondAngle,
	MMPEnergyOpt_PlotDihedral,
	MMPEnergyOpt_Atom1,
	MMPEnergyOpt_Atom2,
	MMPEnergyOpt_Atom3,
	MMPEnergyOpt_Atom4,
	
	NumberMMPEnergyOptions
};

//convert enum to string
const char * MMPPref_convert(MMPPref_Element t);
const char * MMPPref_convert(MMPPref_Color t);
const char * MMPPref_convert(MMPFileOpts t);
const char * MMPPref_convert(MMPAtomAttribs t);
const char * MMPPref_convert(MMPMolDisplayElments t);
const char * MMPPref_convert(MMPSurfaceWinElements b);
const char * MMPPref_convert(MMPEnergyOptions t);
const char * MMPPref_convert(MMPref_WinRect t);
//convert string to enum id
bool MMPPref_convert(const char * s, MMPPref_Element & t);
bool MMPPref_convert(const char * s, MMPPref_Color & t);
bool MMPPref_convert(const char * s, MMPFileOpts & t);
bool MMPPref_convert(const char * s, MMPAtomAttribs & t);
bool MMPPref_convert(const char * s, MMPMolDisplayElments & t);
bool MMPPref_convert(const char * s, MMPSurfaceWinElements & t);
bool MMPPref_convert(const char * s, MMPEnergyOptions & b);
bool MMPPref_convert(const char * s, MMPref_WinRect & b);

#endif

