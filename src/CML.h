/*
 *  (c) 2005 Iowa State University
 *      see the LICENSE file in the top level directory
 */
/*  CML.h
 *
 *  Headers related to CML handling
 *  Created by Brett Bode January 2005
 */

enum CML_Element {
	invalid_CML_Element=0,
	CMLElement,
	MoleculeElement,
	AtomArrayElement,
	AtomElement,
	BondArrayElement,
	BondElement,
	StringArrayElement,
	FloatArrayElement,
	IntegerArrayElement,
	ScalarElement,
	MatrixElement,
	MetaDataElement,
	MetaDataListElement,
	ArrayElement,
	ListElement,
	
	NumberCMLElements
};

enum CML_Attribute {
	invalidCMLAttribute=0,
	IdAttr,
	atomIdAttr,
	titleAttr,
	elementTypeAttr,
	X3Attr,
	Y3Attr,
	Z3Attr,
	xyz3Attr,
	orderAttr,
	atomRefs2Attr,
	bondRefsAttr,
	atomRef1Attr,
	atomRef2Attr,
	atomRefAttr,
	builtinAttr,
	dataTypeAttr,
	rowsAttr,
	columnsAttr,
	sizeAttr,
	nameAttr,
	contentAttr,
	
	NumberCMLAttributes
};

enum MMP_NameSpace { // MacMolPlt specific XML tags
	invalidMMPName=0,
	MMP_RotationMatrix,
	MMP_Energy,
	MMP_KineticEnergy,
	MMP_MP2Energy,
	MMP_Time,
	MMP_ReactionCoord,
	MMP_RMSGradient,
	MMP_MaxGradient,
	MMP_CartesianGradient,
	
	NumberMMPNames
};

enum MMP_VibRecNameSpace {
	invalidMMPVibRecItem=0,
	MMP_CurrentMode,
	MMP_VibIntensity,
	MMP_VibFrequencies,
	MMP_NormalModesCount,
	MMP_VibNormalModes,
	MMP_ReducedMass,
	MMP_RamanIntensity,
	MMP_Depolarization,
	
	NumberMMPVibRecItems
};

enum MMP_MetadataNamespace {
	invalid_MMPMetadata=0,
	MMP_MaxSize,
	MMP_WindowSize,
	MMP_CurrentFrame,
	MMP_DrawNormalMode,
	MMP_DrawSpecialAtoms,
	MMP_ShowAxis,
	MMP_ShowAtomLabels,
	MMP_LabelHAtoms,
	MMP_ShowAtomNumbers,
	MMP_InternalCoordinates,
	MMP_BasisSet,
	MMP_InputOptions,
	MMP_Preferences,
	MMP_WindowData,
	MMP_Use3DMode,
	
	NumberMMPMetadataNames
};

enum MMP_MolListNameSpace {
	invalid_MMPMolList=0,
	MMP_ScalarElement,
	MMP_Gradient,
	MMP_NormalModes,
	MMP_OrbitalSet,
	MMP_SurfaceDescription,
	
	NumberMMPMolListItems
};

enum MMP_SurfaceNameSpace {
	invalid_MMPSurface=0,
	MMP_BaseSurface,
	MMP_OrbSurfBase,
	MMP_1DSurface,
	MMP_2DSurface,
	MMP_3DSurface,
	MMP_VisibleAttr,
	MMP_SurfaceId,
	MMP_SurfaceLabel,
	MMP_OrbSurfPlotOrb,
	MMP_OrbSurfTargetSet,
	MMP_OrbSurfUseAOs,
	MMP_OrbSurfUseBetaOrbs,
	MMP_OrbSurfPhaseChage,
	MMP_OrbSurfSpherical,
	MMP_OrbSurfOccNums,
	MMP_SurfOrigin,
	MMP_SurfXInc,
	MMP_SurfYInc,
	MMP_SurfZInc,
	MMP_SurfGridMax,
	MMP_SurfGridMin,
	MMP_SurfNumGridPoints,
	MMP_SurfPosColor,
	MMP_SurfNegColor,
	MMP_SurfTransColor,
	MMP_SurfNumContours,
	MMP_SurfMaxContourValue,
	MMP_SurfShowZeroContour,
	MMP_SurfScreenPlane,
	MMP_SurfDashedContours,
	MMP_SurfPosNegContours,
	MMP_SurfMaxMEPValue,
	MMP_SurfGrid,
	MMP_SurfNumXGridPoints,
	MMP_SurfNumYGridPoints,
	MMP_SurfNumZGridPoints,
	MMP_SurfContourValue,
	MMP_SurfGridSize,
	MMP_SurfSolidSurface,
	MMP_SurfColorizeSurface,
	MMP_SurfUseSurfaceNormals,
	MMP_SurfUseRGBColor,
	MMP_SurfFixedGrid,
	MMP_SurfContourPosNeg,
	MMP_SurfNumVertices,
	MMP_SurfNumPosTris,
	MMP_SurfNumNegTris,
	MMP_SurfVertices,
	MMP_SurfVerticesList,
	MMP_SurfNormals,
	MMP_SurfSurfaceValues,
	
	NumberMMPSurfaceItems
};

enum MMP_OrbitalSetNS {
	MMP_OrbitalTypeAttr=1,
	MMP_WavefunctionType,
	MMP_OrbitalLabel,
	MMP_NumAlphaOrbs,
	MMP_NumBetaOrbs,
	MMP_NumOccAlphaOrbs,
	MMP_NumOccBetaOrbs,
	MMP_NumBasisFunctions,
	MMP_OrbVectors,
	MMP_BetaOrbVectors,
	MMP_OrbEnergies,
	MMP_BetaOrbEnergies,
	MMP_OrbOccupations,
	MMP_BetaOrbOccupations,
	MMP_OrbSyms,
	MMP_BetaOrbSyms,
	
	NumberMMPOrbitalSetNSItems
};

#define kLeftXML "left"
#define kRightXML "right"
#define kTopXML "top"
#define kBottomXML "bottom"
#define kVisibleXML "visible"
enum MMP_MetaWindowItems {
	MMP_InvalidMetaWindowItem=0,
	MMP_MainWinRect,
	MMP_CoordWinRect,
	MMP_BondsWinRect,
	MMP_EPlotWinRect,
	MMP_FreqWinRect,
	MMP_SurfaceWinRect,
	MMP_PrefsWinRect,
	
	NumberMMPMetaWindowItems
};

enum MMP_InternalCoordNS {
	MMP_InvalidInternal=0,
	MMP_MOPacStyleInternals,
	MMP_MOPacStyleCount,
	MMP_MOPacStyleConnections,
	MMP_MOPacStyleValues,
	MMP_MOPacStyleTypes,
	
	NumberMMPInternalCoordItems
};

enum MMP_BasisSetNS {
	MMP_BasisSetInvalid=0,
	MMP_BasisSetElement,
	MMP_BSMapLength,
	MMP_BSNumShells,
	MMP_BSNumFunctions,
	MMP_BasisShell,
	MMP_BSMap,
	MMP_BSNucCharge,
	MMP_BSShellType,
	MMP_BSNumPrims,
	MMP_BSExponent,
	MMP_BSNormCoef,
	MMP_BSInputCoef,
	
	NumberMMPBasisSetItems
};

enum MMP_InputOptionsNS {
	MMP_InputOptionInvalid=0,
	MMP_InputOptionsElement,
	MMP_IOControlGroupElement,
	MMP_IOSystemGroupElement,
	MMP_IOBasisGroupElement,
	MMP_IODataGroupElement,
	MMP_IOGuessGroupElement,
	MMP_IOSCFGroupElement,
	MMP_IOMP2GroupElement,
	MMP_IOHessianGroupElement,
	MMP_IOStatPtGroupElement,
	MMP_IODFTGroupElement,
	
	NumberMMPInputOptions
};

enum MMP_IOControlGroupNS {
	MMP_IIOControlGroupInvalid=0,
	MMP_IOCGSCFType,
	MMP_IOCGRunType,
	MMP_IOCGExeType,
	MMP_IOCGMPLevel,
	MMP_IOCGCIType,
	MMP_IOCGCCType,
	MMP_IOCGMaxIterations,
	MMP_IOCGCharge,
	MMP_IOCGMultiplicity,
	MMP_IOCGLocalization,
	MMP_IOCGFriend,
	MMP_IOCGPrintOption,
	MMP_IOCGTolerance,
	MMP_IOCGCutoff,
	MMP_IOCGMolPlt,
	MMP_IOCGPlotOrb,
	MMP_IOCGAIMPac,
	MMP_IOCGRPac,
	MMP_IOCGDFTActive,
	MMP_IOCGIntType,
	MMP_IOCGNormF,
	MMP_IOCGNormP,
	
	NumberMMPIOControlGroupItems
};

enum MMP_IOSystemGroupNS {
	MMP_IOSystemGroupInvalid=0,
	MMP_IOSGTimeLimit,
	MMP_IOSGMemory,
	MMP_IOSGTimeUnits,
	MMP_IOSGMemoryUnits,
	MMP_IOSGKDiag,
	MMP_IOSGCoreFlag,
	MMP_IOSGBalanceType,
	MMP_IOSGXDR,
	
	NumberMMPIOSystemGroupItems
};

enum MMP_IOBasisGroupNS {
	MMP_IOBasisGroupInvalid=0,
	MMP_IOBGBasisSet,
	MMP_IOBGNumGauss,
	MMP_IOBGNumDFuncs,
	MMP_IOBGNumFFuncs,
	MMP_IOBGNumPFuncs,
	MMP_IOBGPolar,
	MMP_IOBGECPPotential,
	MMP_IOBGDiffuseSP,
	MMP_IOBGDiffuseS,
	MMP_IOBGDisableBS,
	
	NumberMMPIOBasisGroupItems
};

enum MMP_IODataGroupNS {
	MMP_IODataGroupInvalid=0,
	MMP_IODGTitle,
	MMP_IODGCoordType,
	MMP_IODGNumZVars,
	MMP_IODGPointGroup,
	MMP_IODGPointGroupOrder,
	MMP_IODGUnits,
	MMP_IODGNoSymFlag,
	
	NumberMMPIODataGroupItems
};

enum MMP_IOGuessGroupNS {
	MMP_IOGuessGroupInvalid=0,
	MMP_IOGGGuessType,
	MMP_IOGGNumOrbs,
	MMP_IOGGPrintMO,
	MMP_IOGGVecSource,
	MMP_IOGGOrbReorder,
	MMP_IOGGOrbMix,
	MMP_IOGGMOTolZ,
	MMP_IOGGMOTolEquil,
	
	NumberMMPIOGuessGroupItems
};

enum MMP_IOSCFGroupNS {
	MMP_IOSCFGroupInvalid=0,
	MMP_IOSGDirectSCF,
	MMP_IOSGFockDiff,
	MMP_IOSGUHFNauralOrbitals,
	MMP_IOSGConvCriteria,
	
	NumberMMPIOSCFGroupItems
};

enum MMP_IOMP2GroupNS {
	MMP_IOMP2GroupInvalid=0,
	MMP_IOMGCutOff,
	MMP_IOMGNumCoreElectrons,
	MMP_IOMGMemory,
	MMP_IOMGTransMethod,
	MMP_IOMGAOInts,
	MMP_IOMGLMOMP2,
	
	NumberMMPIOMP2GroupItems
};

enum MMP_IOHessGroupNS {
	MMP_IOHessGroupInvalid=0,
	MMP_IOHGDisplacementSize,
	MMP_IOHGFrequencyScaleFactor,
	MMP_IOHGMethod,
	MMP_IOHGDoubleDiff,
	MMP_IOHGPurify,
	MMP_IOHGInternalFC,
	MMP_IOHGVibAnalysis,
	
	NumberMMPIOHessGroupItems
};

enum MMP_IODFTGroupNS {
	MMP_IODFTGroupInvalid=0,
	MMP_IODFTGGridSwitch,
	MMP_IODFTThreshold,
	MMP_IODFTFunctional,
	MMP_IODFTNumRadialGrids,
	MMP_IODFTNumThetaGrids,
	MMP_IODFTNumPhiGrids,
	MMP_IODFTNumRadialGridsInit,
	MMP_IODFTNumThetaGridsInit,
	MMP_IODFTNumPhiGridsInit,
	MMP_IODFTGridMethod,
	MMP_IODFTGetAuxFunctions,
	MMP_IODFTThree,
	
	NumberMMPIODFTGroupItems
};

enum MMP_IOStatPtGroupNS {
	MMP_IOSPGroupInvalid=0,
	MMP_IOSPGOptConvergance,
	MMP_IOSPGInitTrustRadius,
	MMP_IOSPGMaxTrustRadius,
	MMP_IOSPGMinTrustRadius,
	MMP_IOSPGStatJumpSize,
	MMP_IOSPGModeFollow,
	MMP_IOSPGRadiusUpdate,
	MMP_IOSPGStatPoint,
	MMP_IOSPGHessMethod,
	MMP_IOSPGMethod,
	MMP_IOSPGMaxSteps,
	MMP_IOSPGnRecalcHess,
	MMP_IOSPGAlwaysPrintOrbs,
	
	NumberMMPIOStatPtGroupItems
};

//convert enum to string
const char * CML_convert(CML_Element t);
const char * CML_convert(CML_Attribute t);
const char * CML_convert(BondOrder b);
const char * CML_convert(MMP_NameSpace b);
const char * CML_convert(MMP_VibRecNameSpace b);
const char * CML_convert(MMP_MetadataNamespace b);
const char * CML_convert(MMP_MolListNameSpace b);
const char * CML_convert(MMP_SurfaceNameSpace b);
const char * CML_convert(MMP_OrbitalSetNS b);
const char * CML_convert(MMP_MetaWindowItems b);
const char * CML_convert(MMP_InternalCoordNS b);
const char * CML_convert(MMP_InputOptionsNS b);
const char * CML_convert(MMP_IOControlGroupNS b);
const char * CML_convert(MMP_IOSystemGroupNS b);
const char * CML_convert(MMP_IOBasisGroupNS b);
const char * CML_convert(MMP_IODataGroupNS b);
const char * CML_convert(MMP_IOGuessGroupNS b);
const char * CML_convert(MMP_IOSCFGroupNS b);
const char * CML_convert(MMP_IOMP2GroupNS b);
const char * CML_convert(MMP_IOHessGroupNS b);
const char * CML_convert(MMP_IODFTGroupNS b);
const char * CML_convert(MMP_IOStatPtGroupNS b);
const char * CML_convert(MMP_BasisSetNS b);
const char * CML_TypetoName(short t);
//convert string to enum id
bool CML_convert(const char * s, CML_Element & t);
bool CML_convert(const char * s, CML_Attribute & t);
bool CML_convert(const char * s, BondOrder & b);
bool CML_convert(const char * s, MMP_NameSpace & b);
bool CML_convert(const char * s, MMP_VibRecNameSpace & b);
bool CML_convert(const char * s, MMP_MetadataNamespace & b);
bool CML_convert(const char * s, MMP_MolListNameSpace & b);
bool CML_convert(const char * s, MMP_SurfaceNameSpace & b);
bool CML_convert(const char * s, MMP_OrbitalSetNS & b);
bool CML_convert(const char * s, MMP_MetaWindowItems & b);
bool CML_convert(const char * s, MMP_InternalCoordNS & b);
bool CML_convert(const char * s, MMP_InputOptionsNS & b);
bool CML_convert(const char * s, MMP_IOControlGroupNS & b);
bool CML_convert(const char * s, MMP_IOSystemGroupNS & b);
bool CML_convert(const char * s, MMP_IOBasisGroupNS & b);
bool CML_convert(const char * s, MMP_IODataGroupNS & b);
bool CML_convert(const char * s, MMP_IOGuessGroupNS & b);
bool CML_convert(const char * s, MMP_IOSCFGroupNS & b);
bool CML_convert(const char * s, MMP_IOMP2GroupNS & b);
bool CML_convert(const char * s, MMP_IOHessGroupNS & b);
bool CML_convert(const char * s, MMP_IODFTGroupNS & b);
bool CML_convert(const char * s, MMP_IOStatPtGroupNS & b);
bool CML_convert(const char * s, MMP_BasisSetNS & b);
