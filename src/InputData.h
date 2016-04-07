/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
	InputData.h

	Class definitions for the various input
	group classes.

	Brett Bode - 6-96
	Converted all InputFileData calls to BufferFile calls
	and removed definitions of InputFileData - 8-97
  ¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥*/

#ifndef __INPUTDATA__
#define __INPUTDATA__

#ifndef __BFILE__
#include "BFiles.h"
#endif
#ifndef __MyTypes__
#include "MyTypes.h"
#endif

#define kConRunMenu				1
#define kConSCFMenu				2
#define kConMP2Check			4
#define kConSCFIterEdit			5
#define kConExeTypeMenu			7
#define kConLocalMenu			8
#define kConChargeEdit			10
#define kConMultEdit			12
#define kConCIPopup				13
#define kConGray1				14
#define kConGray2				15
#define kConGray3				16
#define kConDFTRadio			17
#define kConCCPopup				18
#define kConGray4				19

#define kGuessGuessMenu			2
#define kGuessPrintGuess		3
#define kGuessNOrbText			4
#define kGuessNumOrbs			5
#define kGuessRotateOrbs		6
#define kGuessOrbSetPopup		7
//#define kGuessEigenVecsRadio	7
//#define kGuessNatOrbRadio		8
//#define kGuessLMORadio			9
//#define kGuessHandRadio			10
//#define kGuessGray				11
//#define kGuessVecText			12

#define kHessAnalyticRadio		2
#define kHessNumericRadio		3
#define kHessMethodGray			4
#define kHess2DiffCheck			6
#define kHessPurifyCheck		7
#define kHessPrintIntFCCheck	8
#define kHessVibAnalCheck		9
#define kHessDisplacementSize	10
#define kHessScaleEdit			11

#define kStPtnStepEdit			2
#define kStPtOptTolEdit			3
#define kStPtMethodCntrl		4
#define kStPtInitStepEdit		5
#define kStPtMinStepEdit		6
#define kStPtMaxStepEdit		7
#define kStPtJumpEdit			8
#define kStPtHessUpEdit			9
#define kStPtModeEdit			10
#define kStPtStatPtCheck		11
#define kStPtUpStepCheck		12
#define kStPtHessGuess			13
#define kStPtHessRead			14
#define kStPtHessCalc			15
#define kStPtPrintOrbs			16
#define kStPtGray				25

#define kDFTMethodPopup			1
#define kDFTGridFuncPopup		2
#define kDFTGridFreeFuncPopup	4

/** enum for the set of GAMESS run types. */
enum TypeOfRun {
	InvalidRunType=0,
	Energy=1,
	GradientRun=2,
	HessianRun,
	OptimizeRun=4,
	TrudgeRun,
	SadPointRun,
	MinEnergyCrossing,
	IRCRun,
	MolDynamics,
	GradExtrRun,
	DRCRun,
	SurfaceRun,
	G3MP2,
	PropRun,
	MorokumaRun,
	TransitnRun,
	SpinOrbitRun,
	FFieldRun,
	TDHFRun,
	TDHFXRun,
	GLOBOPRun,
	VSCFRun,
	OptFMORun,
	RamanRun,
	NMRRun,
	MakeEFPRun,
	FreeStateFMORun,
	
	NumGAMESSRunTypes
};

enum GAMESS_SCFType {
	GAMESS_Invalid_SCFType=-1,
	GAMESSDefaultSCFType=0,
	GAMESS_RHF,
	GAMESS_UHF,
	GAMESS_ROHF,
	GAMESS_GVB,
	GAMESS_MCSCF,
	GAMESS_NO_SCF,
	
	NumGAMESSSCFTypes
};

enum CCRunType {
	CC_None=0,
	CC_LCCD,
	CC_CCD,
	CC_CCSD,
	CC_CCSDT,
	CC_RCC,
	CC_CRCC,
	CC_CRCCL,
	CC_CCSDTQ,
	CC_CRCCQ,
	CC_EOMCCSD,
	CC_CREOM,
	CC_CREOML,
	CC_IP_EOM2,
	CC_IP_EOM3A,
	CC_EA_EOM2,
	CC_EA_EOM3A,
	
	NumCCTypes
};

enum CIRunType {
	CI_None=0,
	CI_GUGA,
	CI_ALDET,
	CI_ORMAS,
	CI_CIS,
	CI_FSOCI,
	CI_GENCI,
	
	NumCITypes
};

typedef class InputData	InputData;

enum FriendType {
	Friend_invalid=-1,
	Friend_None=0,
	Friend_HONDO,
	Friend_MELDF,
	Friend_GAMESSUK,
	Friend_GAUSSIAN,
	Friend_ALL,
	
	NumFriendTypes
};

enum GAMESS_Localization {
	Invalid_Localization=-1,
	GAMESS_No_Localization=0,
	GAMESS_BOYS_Localization,
	GAMESS_RUEDNBRG_Localization,
	GAMESS_POP_Localization,
	GAMESS_SVD_Localization,
	
	NumGAMESSLocalizations
};

class ControlGroup {
	private:
		char		*ExeType;
		GAMESS_SCFType	SCFType;
		short		MPLevelCIType;	//first 4 bits = MP level, 2nd 4 bits CI Type
		TypeOfRun	RunType;
		short		MaxIt;
		short		Charge;
		short		Multiplicity;
		GAMESS_Localization	Local;
		FriendType	Friend;
		short		NPrint;
		short		ITol;
		short		ICut;
		CCRunType	CCType;
		char		Options;		//1 bit for several options
			//bit 1		MolPlot;
			//bit 2		PlotOrb;
			//bit 3		AIMPAC;
			//bit 4		RPAC;
			//bit 5		DFT active;
			//bit 6		Spherical Harmonic flag;
			//bit 7		NormF;
			//bit 8		NormP;
	public:		//Public data access functions
		GAMESS_SCFType SetSCFType(GAMESS_SCFType NewSCFType);
		GAMESS_SCFType SetSCFType(const char *SCFText);
		const char * GetSCFTypeText(void) const {return GAMESSSCFTypeToText(SCFType);};
		static const char * GAMESSSCFTypeToText(GAMESS_SCFType t);
		inline GAMESS_SCFType GetSCFType(void) const {return SCFType;};
		long SetMPLevel(short NewMPLevel);
		short GetMPLevel(void) const;
		CIRunType GetCIType(void) const;
		CIRunType GetCIType(char * outText) const;
		const char * GetCIType(const CIRunType &) const;
		CIRunType SetCIType(CIRunType NewVal);
		CIRunType SetCIType(const char * CIText);
		TypeOfRun SetRunType(const TypeOfRun & NewRunType);
		TypeOfRun SetRunType(const char *RunText);
		/** Routine to convert the TypeOfRun enum into a char string containing
		 the standard GAMESS Run type.
		 @param r The TypeOfRun identifier
		 */
		static const char * GetGAMESSRunText(const TypeOfRun & r);
		inline TypeOfRun GetRunType(void) const {return RunType;};
		short GetExeType(void);
		short GetExeType(Str255 ExeText);
		short SetExeType(const char *ExeText);
		short SetExeType(short NewType);
		short SetMaxIt(short NumIt);
		inline short GetMaxIt(void) {return MaxIt;};
		short SetCharge(short charge);
		inline short GetCharge(void) {return Charge;};
		short SetMultiplicity(short NewMult);
		inline short GetMultiplicity(void) {return Multiplicity;};
		GAMESS_Localization SetLocal(const char *LocalText);
		GAMESS_Localization SetLocal(GAMESS_Localization NewLocal);
		inline GAMESS_Localization GetLocal(void) const {return Local;};
		static const char * GAMESSLocalizationToText(GAMESS_Localization t);
		const char * GetLocalText(void) const {return GAMESSLocalizationToText(Local);};
		FriendType SetFriend(const char *FriendText);
		FriendType SetFriend(FriendType NewFriend);
		static const char * GetFriendText(FriendType f);
		static FriendType TextToFriend(const char * c);
		inline FriendType GetFriend(void) const {return Friend;};
		inline const char * GetFriendText(void) const {return GetFriendText(Friend);};
		bool SetMolPlot(bool State);
		inline bool GetMolPlot(void) const {return ((Options & 1)?true:false);};
		bool SetPlotOrb(bool State);
		inline bool GetPlotOrb(void) const {return ((Options & (1<<1))?true:false);};
		bool SetAIMPAC(bool State);
		inline bool GetAIMPAC(void) const {return ((Options & (1<<2))?true:false);};
		bool SetRPAC(bool State);
		inline bool GetRPAC(void) const {return ((Options & (1<<3))?true:false);};
		bool UseDFT(bool state);
		bool UseDFT(void) const;
		bool UseSphericalHarmonics(bool State);
		inline bool UseSphericalHarmonics(void) const {return ((Options & (1<<5))?true:false);};
		bool SetNormF(bool State);
		inline bool GetNormF(void) const {return ((Options & (1<<6))?true:false);};
		bool SetNormP(bool State);
		inline bool GetNormP(void) const {return ((Options & (1<<7))?true:false);};
		CCRunType GetCCType(void) const;
		static const char * GetGAMESSCCType(const CCRunType & );
		CCRunType SetCCType(CCRunType n);
		CCRunType SetCCType(const char * n);
				//other member functions
		ControlGroup(void);
		ControlGroup(ControlGroup *Copy);
		~ControlGroup(void);
		void InitControlPaneData(void);
		void InitProgPaneData(void);
		void WriteToFile(BufferFile *File, InputData *IData, long NumElectrons);
		void RevertControlPane(ControlGroup *OrgData);
		void RevertProgPane(ControlGroup *OrgData);
		void WriteXML(XMLElement * parent) const;
		void ReadXML(XMLElement * parent);
};

enum MemoryUnit {
	wordsUnit=1,
	bytesUnit,
	megaWordsUnit,
	megaBytesUnit,
	gigaWordsUnit,
	gigaBytesUnit,
	
	NumberMemoryUnits
};
const char * MemoryUnitToText(const MemoryUnit & mu);
bool TextToMemoryUnit(const char * t, MemoryUnit & mu);

enum TimeUnit {
	secondUnit=1,
	minuteUnit,
	hourUnit,
	dayUnit,
	weekUnit,
	yearUnit,
	milleniaUnit,
	
	NumberTimeUnits
};
const char * TimeUnitToText(const TimeUnit & tu);
bool TextToTimeUnit(const char * t, TimeUnit & tu);

class SystemGroup {
	private:
		long		TimeLimit; //This is always stored as minutes
		double		Memory; //Always stored as words
		double		MemDDI;	//stored as megawords
		char		KDiag;
		TimeUnit	TimeUnits;
		MemoryUnit	MemUnits;
		MemoryUnit	MemDDIUnits;
		char		Flags;		//One bit flags
			//bit 1		CoreFlag;
			//bit 2		BalanceType; set to true for LOOP
			//bit 3		XDR;
			//bit 4		PARALL;
	public:
		inline long GetTimeLimit(void) const {return TimeLimit;};
		long SetTimeLimit(long NewTime);
		inline TimeUnit GetTimeUnits(void) const {return TimeUnits;};
		TimeUnit SetTimeUnits(TimeUnit NewUnits);
		float GetConvertedTime(void) const;
		long SetConvertedTime(float NewTime);
		inline double GetMemory(void) const {return Memory;};
		double SetMemory(double NewMemory);
		inline MemoryUnit GetMemUnits(void) const {return MemUnits;};
		MemoryUnit SetMemUnits(MemoryUnit NewUnits);
		double GetConvertedMem(void) const;
		double SetConvertedMem(double NewMem);
		inline MemoryUnit GetMemDDIUnits(void) const {return MemDDIUnits;};
		inline double GetMemDDI(void) const {return MemDDI;};
		double SetMemDDI(double NewMemDDI);
		double GetConvertedMemDDI(void) const;
		double SetConvertedMemDDI(double NewMem);
		MemoryUnit SetMemDDIUnits(MemoryUnit NewUnits);
		inline char GetDiag(void) const {return KDiag;};
		char SetDiag(char NewMethod);
		bool GetCoreFlag(void) const {return ((Flags & 1)?true:false);};
		bool SetCoreFlag(bool State);
		bool GetBalanceType(void) const {return ((Flags & 2)?true:false);};
		bool SetBalanceType(bool Type);
		bool GetXDR(void) const {return ((Flags & 4)?true:false);};
		bool SetXDR(bool State);
		inline bool GetParallel(void) const {return ((Flags & 8)?true:false);};
		bool SetParallel(bool State);
				//other member functions
		SystemGroup(void);
		SystemGroup(SystemGroup *Copy);
		void InitData(void);
		void WriteToFile(BufferFile *File);
		void WriteXML(XMLElement * parent) const;
		void ReadXML(XMLElement * parent);
		void ReadSystemOptions(BufferFile * Buffer);
};
enum GAMESS_BasisSet {
	GAMESS_BS_None=0,
	GAMESS_BS_MINI,
	GAMESS_BS_MIDI,
	GAMESS_BS_STO,
	GAMESS_BS_N21,
	GAMESS_BS_N31,
	GAMESS_BS_N311,
	GAMESS_BS_DZV,
	GAMESS_BS_DH,
	GAMESS_BS_TZV,
	GAMESS_BS_MC,
	
	GAMESS_BS_CC_PVDZ, GAMESS_BS_CC_PVTZ, GAMESS_BS_CC_PVQZ, GAMESS_BS_CC_PV5Z, GAMESS_BS_CC_PV6Z,
	GAMESS_BS_AUG_CC_PVDZ, GAMESS_BS_AUG_CC_PVTZ, GAMESS_BS_AUG_CC_PVQZ, GAMESS_BS_AUG_CC_PV5Z, GAMESS_BS_AUG_CC_PV6Z,
	GAMESS_BS_CC_PCVDZ, GAMESS_BS_CC_PCVTZ, GAMESS_BS_CC_PCVQZ, GAMESS_BS_CC_PCV5Z, GAMESS_BS_CC_PCV6Z,
	GAMESS_BS_AUG_CC_PCVDZ, GAMESS_BS_AUG_CC_PCVTZ, GAMESS_BS_AUG_CC_PCVQZ, GAMESS_BS_AUG_CC_PCV5Z, GAMESS_BS_AUG_CC_PCV6Z,
	GAMESS_BS_PC0, GAMESS_BS_PC1, GAMESS_BS_PC2, GAMESS_BS_PC3, GAMESS_BS_PC4,
	GAMESS_BS_APC0, GAMESS_BS_APC1, GAMESS_BS_APC2, GAMESS_BS_APC3, GAMESS_BS_APC4,
	GAMESS_BW_SPK_DZP, GAMESS_BW_SPK_TZP, GAMESS_BW_SPK_QZP, GAMESS_BW_SPK_ADZP, GAMESS_BW_SPK_ATZP, GAMESS_BW_SPK_AQZP,
	GAMESS_BW_SPKRDZP, GAMESS_BW_SPKRTZP, GAMESS_BW_SPKRQZP, GAMESS_BW_SPKRADZP, GAMESS_BW_SPKRATZP, GAMESS_BW_SPKRAQZP,
	GAMESS_BW_KTZV,
	GAMESS_BW_KTZVP,
	GAMESS_BW_KTZVPP,
	
	GAMESS_BS_SBKJC,
	GAMESS_BS_HW,
	
	GAMESS_BS_MCP_DZP, GAMESS_BS_MCP_TZP, GAMESS_BS_MCP_QZP,
	GAMESS_BS_MCP_ATZP, GAMESS_BS_MCP_AQZP,
	GAMESS_BS_MCPCDZP, GAMESS_BS_MCPCTZP, GAMESS_BS_MCPCQZP,
	GAMESS_BS_MCPACDZP, GAMESS_BS_MCPACTZP, GAMESS_BS_MCPACQZP,
	GAMESS_BS_IMCP_SR1, GAMESS_BS_IMCP_SR2,
	GAMESS_BS_IMCP_NR1, GAMESS_BS_IMCP_NR2,
	GAMESS_BS_ZFK3_DK3, GAMESS_BS_ZFK4_DK3, GAMESS_BS_ZFK5_DK3,
	GAMESS_BS_ZFK3LDK3, GAMESS_BS_ZFK4LDK3, GAMESS_BS_ZFK5LDK3,
		
	GAMESS_BS_MNDO,
	GAMESS_BS_AM1,
	GAMESS_BS_PM3,
	GAMESS_BS_RM1,
	
	GAMESS_BS_BC,	//Old basis name - appears to be rolled into DZ basis today
	NumGAMESSBasisSetsItem
};
enum GAMESS_BS_Polarization {
	GAMESS_BS_Pople_Polar=-2,	//old one, should not be output
	GAMESS_BS_Invalid_Polar=-1,
	GAMESS_BS_No_Polarization=0,
	GAMESS_BS_Common_Polar,
	GAMESS_BS_PopN31_Polar,
	GAMESS_BS_PopN311_Polar,
	GAMESS_BS_Dunning_Polar,
	GAMESS_BS_Huzinaga_Polar,
	GAMESS_BS_Hondo7_Polar,
	
	NumGAMESSBSPolarItems
};
enum GAMESS_BS_ECPotential {
	GAMESS_BS_Invalid_ECP=-1,
	GAMESS_BS_ECP_None=0,
	GAMESS_BS_ECP_Read,
	GAMESS_BS_ECP_SBKJC,
	GAMESS_BS_ECP_HW,
	GAMESS_BS_ECP_MCP,
	
	NumGAMESSBSECPItems
};
class BasisGroup {
	private:
		float		Split2[2];
		float		Split3[3];
		short		Basis;
		short		NumGauss;
		short		NumHeavyFuncs;	//8 bits for #D funcs, 8 bits for # f funcs
		short		NumPFuncs;
		GAMESS_BS_Polarization		Polar;
		short		ECPPotential;	//Potential type for ECP calculations (in $CONTRL)
		char		Flags;		//boolean type options
			//bit 1		DiffuseSP;
			//bit 2		DiffuseS;
			//bit 3		disables builtin basis types based on current atom set
	public:		//Member functions
		short SetBasis(const char *text);
		short SetBasis(short NewBasis);
		const char * GetBasisText(void) const;
		short GetBasis(void) const;
		static const char * GAMESSBasisSetToText(GAMESS_BasisSet bs);
		short SetNumGauss(short NewNumGauss);
		short GetNumGauss(void) const;
		short SetNumDFuncs(short NewNum);
		short GetNumDFuncs(void) const;
		short SetNumFFuncs(short NewNum);
		short GetNumFFuncs(void) const;
		short SetNumPFuncs(short NewNum);
		short GetNumPFuncs(void) const;
		short SetDiffuseSP(bool state);
		bool GetDiffuseSP(void) const {return ((Flags&1)?true:false);};
		short SetDiffuseS(bool state);
		bool GetDiffuseS(void) const {return ((Flags&2)?true:false);};
		GAMESS_BS_Polarization SetPolar(GAMESS_BS_Polarization NewPolar);
		GAMESS_BS_Polarization SetPolar(const char *text);
		static const char * PolarToText(GAMESS_BS_Polarization p);
		inline const char * GetPolarText(void) const {return PolarToText(Polar);};
		inline GAMESS_BS_Polarization GetPolar(void) const {return Polar;};
		static const char * GAMESSECPToText(GAMESS_BS_ECPotential p);
		short GetECPPotential(void) const;
		const char * GetECPPotentialText(void) const;
		GAMESS_BS_ECPotential SetECPPotential(const char * ECPText);
		short SetECPPotential(short NewType);
		inline bool CheckBasis(void) const {return ((Flags & 4)?true:false);};
		inline void CheckBasis(bool state) {Flags = (Flags& 0xFB) + (state?4:0);};
				//other member functions
		BasisGroup(void);
		BasisGroup(BasisGroup *Copy);
		void InitData(void);
		long WriteToFile(BufferFile *File, MoleculeData * lData);
		void WriteXML(XMLElement * parent) const;
		void ReadXML(XMLElement * parent);
};

enum CoordinateType {
	invalidCoordinateType=0,
	UniqueCoordType,
	HINTCoordType,
	CartesianCoordType,
	ZMTCoordType,
	ZMTMPCCoordType,
	
	NumberCoordinateTypes
};
class DataGroup {
	private:
		char		*Title;
		short		Coord;
		short		NumZVar;
		char		PointGroup;
		char		PGroupOrder;
		char		Options;
			//bit 1 units
			//bit 2 NoSym
	public:		//data access functions
		short SetPointGroup(GAMESSPointGroup NewPGroup);
		short SetPointGroup(char *GroupText);
		static const char * GetGAMESSPointGroupText(GAMESSPointGroup p);
		const char * GetPointGroupText(void) const {return GetGAMESSPointGroupText((GAMESSPointGroup) PointGroup);};
		inline GAMESSPointGroup GetPointGroup(void) const {return (GAMESSPointGroup)PointGroup;};
		short SetPointGroupOrder(short NewOrder);
		inline short GetPointGroupOrder(void) const {return PGroupOrder;};
		short SetTitle(const char *NewTitle, long length=-1);
		const char * GetTitle(void) const;
		CoordinateType GetCoordType(void) const;
		static const char * GetCoordTypeText(CoordinateType t);
		const char * GetCoordText(void) const {return GetCoordTypeText((CoordinateType) Coord);};
		CoordinateType SetCoordType(const char *CoordText);
		CoordinateType SetCoordType(CoordinateType NewType);
		bool GetUnits(void) const {return ((Options&1)?true:false);};
		/** Set the units for the coordinates (true for bohr, false for angstrom).
		 * @param unitFlag Flag to indicate units
		 */
		bool SetUnits(bool unitFlag);
		/** Set the units for the coordinates (bohr or angs (angstroms)).
		 * The return value is 0 for angstroms, 1 for bohr, -1 indicates invalid text.
		 * @param unitText A null-terminated string to parse.
		 */
		int SetUnits(const char * unitText);
		bool SetUseSym(bool State);
		bool GetUseSym(void) const {return ((Options&2)?true:false);};
		short GetNumZVar(void) const {return NumZVar;};
		short SetNumZVar(short NewNum);
				//other member functions
		DataGroup(void);
		DataGroup(DataGroup *Copy);
		~DataGroup(void);
		void InitData(void);
		void WriteToFile(BufferFile *File, MoleculeData * MainData, WinPrefs * Prefs, long BasisTest);
		void WriteXML(XMLElement * parent) const;
		void ReadXML(XMLElement * parent);
};
typedef enum EFRAG_PolMethods {
	invalidPolMethod=0,
	FRGSCF_PolMethod,
	SCF_PolMethod,
	
	NumEFragPolMethods
} EFRAG_PolMethods;
typedef enum EFRAG_PositionTypes {
	invalidEFragPositionType=0,
	Optimize_Position,
	Fixed_Position,
	EFOPT_Position,
	
	NumEFragPositionTypes
} EFRAG_PositionTypes;

/// EffectiveFragmentsGroup stores the options for the EFRAG group, but not the actual fragments
class EffectiveFragmentsGroup {
	private:
		long MaxBasisFuncs;	///< MXBF - max number of basis functions in any of the EFP2 potentials
		long MaxMOs;		///< MXMO - Max number of MOs in any EFP2's PROJECTION section
		long NumBufferMOs;	///< NBUFFMO - First n orbs in the MO matrix belong to the QM/MM buffer - see $MOFRZ
		char flags;			///< Set of bits to handle the 3 text fields
							// bit 1 is 0 for Cartesian coordinates, 1 for internal coordinates
	public:
			/** Constructor. Sets normal default values.
			 */
		EffectiveFragmentsGroup(void) {flags=0; MaxBasisFuncs = MaxMOs = NumBufferMOs = -1;};
			/** = operator to copy one instance to another.
			 */
		const EffectiveFragmentsGroup & operator=(const EffectiveFragmentsGroup & other) {flags=other.flags; MaxBasisFuncs=other.MaxBasisFuncs; MaxMOs=other.MaxMOs; NumBufferMOs=other.NumBufferMOs; return *this;};
		bool UseCartesianCoordinates(void) const {return !(flags & 1);};
		void UseCartesianCoordinates(bool v) {flags = (flags & 0xFE) + (v ? 1 : 0);};
		bool UseInternalCoordinates(void) const {return (flags & 1);};
		void UseInternalCoordinates(bool v) {flags = (flags & 0xFE) + (v ? 0 : 1);};
			/** Set the coordinates type from the text string.
			 * @param v The string containing a valid COORD= value (CART or INT).
			 */
		bool SetCoordinatesType(const char * v);
		const char * GetGAMESSCoordText(void) const {return ConvertCoordToText(UseInternalCoordinates()?1:0);};
		static const char * ConvertCoordToText(int v);
		static const char * ConvertPolMethodToText(EFRAG_PolMethods v);
		static const char * ConvertPositionMethodToText(EFRAG_PositionTypes v);
		bool SetPolMethod(const char * v);
		bool PolMethodIsDefault(void) const {return ((flags & 2)?false:true);};
		void PolMethodIsDefault(bool v) {flags = (flags & 0xFD) + (v ? 2 : 0);};
		EFRAG_PolMethods PolMethod(void) const {return ((flags & 4)?SCF_PolMethod:FRGSCF_PolMethod);};
		void PolMethod(EFRAG_PolMethods v) {flags = (flags & 0xFB) + ((v==SCF_PolMethod) ? 4 : 0); PolMethodIsDefault(false);};
		bool PositionIsDefault(void) const {return ((flags & 8)?false:true);};
		void PositionIsDefault(bool v) {flags = (flags & 0xF7) + (v ? 8 : 0);};
		bool SetPositionType(const char * v);
		EFRAG_PositionTypes PositionMethod(void) const {return (EFRAG_PositionTypes)(((flags & 48)>>4)+1);};
		void PositionMethod(EFRAG_PositionTypes v) {flags = (flags & 0xCF) + ((v-1)<<4);PositionIsDefault(false);};
		long GetMaxMOs(void) const {return MaxMOs;};
		void SetMaxMOs(long v) {MaxMOs = v;};
		long GetNumBufferMOs(void) const {return NumBufferMOs;};
		void SetNumBufferMOs(long v) {NumBufferMOs = v;};
		long GetMaxBasisFunctions(void) const {return MaxBasisFuncs;};
		void SetMaxBasisFunctions(long v) {MaxBasisFuncs = v;};
		void WriteXML(XMLElement * parent) const;
		void ReadXML(XMLElement * parent);
};

typedef enum TypeOfGuess {
	invalidGuessType=0,
	HUCKELGuessType=1,
	HCOREGuessType,
	MOREADGuessType,
	MOSAVEDGuessType,
	SkipGuessType,

	NumberGuessTypes
} TypeOfGuess;
class GuessGroup {
	private:
		float		MOTolZ;
		float		MOTolEquil;
		long		*IOrder;
		long		*JOrder;
		long		NumOrbs;
		short		VecSource;
		short		GuessType;
		char		Options;
			//bit 1 Print initial guess PRTMO
			//bit 2 reordering switch
			//bit 3 orbital mixing switch
	public:		//data access functions
		inline short GetGuess(void) const {return GuessType;};
		const char * GetGuessText(void) const;
		short SetGuess(const char *GuessText);
		inline short SetGuess(short NewGuess) {if ((NewGuess<0)||(NewGuess>=NumberGuessTypes)) return -1; GuessType = NewGuess; return GetGuess();};
		inline short GetVecSource(void) const {return VecSource;};
		inline void SetVecSource(short NewVal) {if (NewVal>0) VecSource = NewVal;};
		inline long GetNumOrbs(void) const {return NumOrbs;};
		inline long SetNumOrbs(long NewNum) {if (NewNum >= 0) NumOrbs = NewNum; return NumOrbs;};
		inline bool GetPrintMO(void) const {return (Options & 1);};
		inline void SetPrintMO(bool State) {Options = (Options & 0xFE) + (State ? 1 : 0);};
		inline bool GetNOrder(void) const {return ((Options & 2)?true:false);};
		inline void SetNOrder(bool State) {Options = (Options & 0xFD) + (State ? 2 : 0);};
		inline bool GetMix(void) const {return ((Options & 4)?true:false);};
		inline void SetMix(bool State) {Options = (Options & 0xFB) + (State ? 4 : 0);};
		static const char * ConvertGuessType(const int & type);
				//other member functions
		GuessGroup(void);
		GuessGroup(GuessGroup *Copy);
	//	~GuessGroup(void);	//not needed until iorder and jorder are used
		void InitData(void);
		void WriteToFile(BufferFile *File, InputData *IData, MoleculeData * orbdata);
		void WriteVecGroup(BufferFile *File, MoleculeData * lData);
		void WriteXML(XMLElement * parent) const;
		void ReadXML(XMLElement * parent);
};
class SCFGroup {
	private:
		float	SOGTolerance;		//SOSCF threshold
		float	EnErrThresh;		//DIIS error threshold
		float	DEMCutoff;			//DEM limit
		float	DampCutoff;			//Damping limit
		long	GVBNumCoreOrbs;		///< GVB NCO - Number Core Orbitals
		long	GVBNumOpenShells;	///< GVB NSETO - Number of open shells
		long	GVBNumPairs;		///< GVP NPAIR - Number of overlapping orbital pairs
		std::vector<long> GVBOpenShellDeg;	///< GVB NO - Array of shell degeneracies (NSETO shells)
		short	ConvCriteria;		//Convergance cutoff 10^(-n)
		short	MaxDIISEq;			//Max size of the DIIS linear equations
		short	MVOCharge;			//Modified Virtual Orbital Charge
		char	Punch;				//Orbital printout flags
		char	Options1;			//Main option flags
					//bit 1 Direct SCF
					//bit 2 Fock matrix diff
					//bit 3 UHF natural orbitals
		char	ConverganceFlags;	//flags for each converger
					// 1 extrapolation
					// 2 damping
					// 3 level shifting
					// 4 restriction
					// 5 DIIS
					// 6 Second order SCF
					// 7 direct energy min.
	public:	//member functions
		bool GetDirectSCF(void) const {return ((Options1 & 1)?true:false);};
		bool SetDirectSCF(bool State);
		bool GetFockDiff(void) const {return ((Options1 & 2)?true:false);};
		bool SetFockDiff(bool State);
		bool GetUHFNO(void) const {return ((Options1 & 4)?true:false);};
		bool SetUHFNO(bool State);
		bool GetExtrapolation(void) const {return ((ConverganceFlags & 1)?true:false);};
		void SetExtrapolation(bool State) {ConverganceFlags = (ConverganceFlags & 0xFE) + (State ? 1 : 0);};
		bool GetDamp(void) const {return ((ConverganceFlags & 2)?true:false);};
		void SetDamp(bool State) {ConverganceFlags = (ConverganceFlags & 0xFD) + (State ? 2 : 0);};
		bool GetShift(void) const {return ((ConverganceFlags & 4)?true:false);};
		void SetShift(bool State) {ConverganceFlags = (ConverganceFlags & 0xFB) + (State ? 4 : 0);};
		bool GetRestriction(void) const {return ((ConverganceFlags & 8)?true:false);};
		void SetRestriction(bool State) {ConverganceFlags = (ConverganceFlags & 0xF7) + (State ? 8 : 0);};
		bool GetDIIS(void) const {return ((ConverganceFlags & 16)?true:false);};
		void SetDIIS(bool State) {ConverganceFlags = (ConverganceFlags & 0xEF) + (State ? 16 : 0);};
		bool GetSOSCF(void) const {return ((ConverganceFlags & 32)?true:false);};
		void SetSOSCF(bool State) {ConverganceFlags = (ConverganceFlags & 0xDF) + (State ? 32 : 0);};
		bool GetDEM(void) const {return ((ConverganceFlags & 64)?true:false);};
		void SetDEM(bool State) {ConverganceFlags = (ConverganceFlags & 0xBF) + (State ? 64 : 0);};
		short GetConvergance(void) const {return ConvCriteria;};
		short SetConvergance(short NewConv);
		long GetGVBNumCoreOrbs(void) const {return GVBNumCoreOrbs;};
		void SetGVBNumCoreOrbs(const long & nco) {GVBNumCoreOrbs = nco;};
		long GetGVBNumOpenShells(void) const {return GVBNumOpenShells;};
		void SetGVBNumOpenShells(const long & no);
		long GetGVBNumPairs(void) const {return GVBNumPairs;};
		void SetGVBNumPairs(const long & npairs) {GVBNumPairs = npairs;};
		std::vector<long> & GetGVBOpenShellDeg(void) {return GVBOpenShellDeg;};
		void ClearGVBOpenShellDeg(void) {GVBOpenShellDeg.clear();};
		void SetGVBNODegValue(int index, long value);
		void AddGVBOpenShellDeg(const long & d) {GVBOpenShellDeg.push_back(d);};
		long GetGVBOpenShellDeg(const long & d) {return ((d<GVBOpenShellDeg.size())?GVBOpenShellDeg[d]:0);};
		SCFGroup(void);
		SCFGroup(SCFGroup *Copy);
		void InitData(void);
			//no destructor for now
		void WriteToFile(BufferFile *File, InputData *IData);
		void WriteXML(XMLElement * parent) const;
		void ReadXML(XMLElement * parent);
};
class MP2Group {
	private:
		double		CutOff;	//double needed to hold very small cutoffs
		long		NumCoreElectrons;
		long		Memory;
		char		Method;
		char		AOInts;
		char		LMOMP2;
		bool		MP2Prop;
	public:
		MP2Group(void);
		MP2Group(MP2Group *Copy);
		void InitData(void);

		inline double GetIntCutoff(void) const {return CutOff;};
		double SetIntCutoff(double NewCutOff);
		inline long GetNumCoreElectrons(void) const {return NumCoreElectrons;};
		long SetNumCoreElectrons(long NewNum);
		inline long GetMemory(void) const {return Memory;};
		long SetMemory(long NewMem);
		inline char GetMethod(void) const {return Method;};
		char SetMethod(char NewMethod);
		inline char GetAOIntMethod(void) const {return AOInts;};
		const char * GetAOIntMethodText(void) const;
		char SetAOIntMethod(char NewMethod);
		void SetAOIntMethod(const char * t);
		bool GetLMOMP2(void) const;
		bool SetLMOMP2(bool State);
		bool GetMP2Prop(void) const {return MP2Prop;};
		void SetMP2Prop(bool state) {MP2Prop = state;};

		void WriteToFile(BufferFile *File, InputData *IData);
		void WriteXML(XMLElement * parent) const;
		void ReadXML(XMLElement * parent);
};
class HessianGroup {
	private:
		float	DisplacementSize;
		float	FrequencyScaleFactor;
		long	BitOptions;	//Bit 1: method
							//Bit 2: double difference
							//Bit 3: purify
							//Bit 4: print internal FC's
							//Bit 5: vibrational analysis
	public:
		HessianGroup(void) {InitData();};
		HessianGroup(HessianGroup *Copy) {*this = *Copy;};
		void InitData(void);

		inline float GetDisplacementSize(void) const {return DisplacementSize;};
		inline float SetDisplacementSize(float NewVal) {if (NewVal > 0.0) DisplacementSize = NewVal; return DisplacementSize;};
		inline float GetFreqScale(void) const {return FrequencyScaleFactor;};
		inline float SetFreqScale(float NewVal) {if (NewVal > 0.0) FrequencyScaleFactor = NewVal; return FrequencyScaleFactor;};
		inline bool GetAnalyticMethod(void) const {return (BitOptions & 1);};
		inline void SetAnalyticMethod(bool NewVal) {if (BitOptions & 1) BitOptions--; if (NewVal) {BitOptions++; if (BitOptions & 32) BitOptions -= 32;}};
		inline bool GetDoubleDiff(void) const {return ((BitOptions & 2)?true:false);};
		inline void SetDoubleDiff(bool NewVal) {if (BitOptions & 2) BitOptions-=2; if (NewVal) BitOptions += 2;};
		inline bool GetPurify(void) const {return ((BitOptions & 4)?true:false);};
		inline void SetPurify(bool NewVal) {if (BitOptions & 4) BitOptions -= 4; if (NewVal) BitOptions += 4;};
		inline bool GetPrintFC(void) const {return ((BitOptions & 8)?true:false);};
		inline void SetPrintFC(bool NewVal) {if (BitOptions & 8) BitOptions -= 8; if (NewVal) BitOptions += 8;};
		inline bool GetVibAnalysis(void) const {return ((BitOptions & 16)?true:false);};
		inline void SetVibAnalysis(bool NewVal) {if (BitOptions & 16) BitOptions -= 16; if (NewVal) BitOptions += 16;};

		/**
		 * Analyze the provided set of input parameters to determine if the run will involve the
		 * computation of a hessian, thus making this group needed.
		 * @param IData Pointer to the set of input data to analyze.
		 */
		static bool IsHessianGroupNeeded(const InputData * IData);
		/**
		 * Analyze the provided set of input parameters to determine if GAMESS can compute
		 * the 2nd derivative of the energy analytically.
		 * @param IData Pointer to the set of input data to analyze.
		 */
		static bool IsAnalyticHessianPossible(const InputData * IData);
	
		void WriteToFile(BufferFile *File, InputData *IData);
		void WriteXML(XMLElement * parent) const;
		void ReadXML(XMLElement * parent);
};
enum DFTFunctionalsGrid {
	invalidDFTGrid=0,
	DFT_Grid_Slater,
	DFT_Grid_Becke,
	DFT_Grid_GILL,
	DFT_Grid_OPTX,
	DFT_Grid_PW91X,
	DFT_Grid_PBEX,
	DFT_Grid_VWN,
	DFT_Grid_VWN1,
	DFT_Grid_PZ81,
	DFT_Grid_P86,
	DFT_Grid_LYP,
	DFT_Grid_PW91C,
	DFT_Grid_PBEC,
	DFT_Grid_OP,
	DFT_Grid_SVWN,
	DFT_Grid_BLYP,
	DFT_Grid_BOP,
	DFT_Grid_BP86,
	DFT_Grid_GVWN,
	DFT_Grid_GPW91,
	DFT_Grid_PBEVWN,
	DFT_Grid_PBEOP,
	DFT_Grid_OLYP,
	DFT_Grid_PW91,
	DFT_Grid_PBE,
	DFT_Grid_revPBE,
	DFT_Grid_RPBE,
	DFT_Grid_PBEsol,
	DFT_Grid_EDF1,
	DFT_Grid_HCTH93,
	DFT_Grid_HCTH120,
	DFT_Grid_HCTH147,
	DFT_Grid_HCTH407,
	DFT_Grid_SOGGA,
	DFT_Grid_MOHLYP,
	DFT_Grid_B97_D,
	DFT_Grid_BHHLYP,
	DFT_Grid_B3PW91,
	DFT_Grid_B3LYP,
	DFT_Grid_B3LYP1,
	DFT_Grid_B97,
	DFT_Grid_B97_1,
	DFT_Grid_B97_2,
	DFT_Grid_B97_3,
	DFT_Grid_B97_K,
	DFT_Grid_B98,
	DFT_Grid_PBE0,
	DFT_Grid_X3LYP,
	DFT_Grid_CAMB3LYP,
	DFT_Grid_wB97,
	DFT_Grid_wB97X,
	DFT_Grid_wB97X_D,
	DFT_Grid_B2PLYP,
	DFT_Grid_xB97X_2,
	DFT_Grid_xB97X_2L,
	DFT_Grid_VS98,
	DFT_Grid_PKZB,
	DFT_Grid_tHCTH,
	DFT_Grid_tHCTHhyb,
	DFT_Grid_BMK,
	DFT_Grid_TPSS,
	DFT_Grid_TPSSh,
	DFT_Grid_TPSSm,
	DFT_Grid_revTPSS,
	DFT_Grid_M05,
	DFT_Grid_M05_2X,
	DFT_Grid_M06,
	DFT_Grid_M06_L,
	DFT_Grid_M06_2X,
	DFT_Grid_M06_HF,
	DFT_Grid_M08_HX,
	DFT_Grid_M08_SO,
	
	NumberGRIDDFTFuncs
};
enum DFTFunctionalsGridFree {
	invalidDFTGridFreeType=0,
	DFT_GridFree_XALPHA,
	DFT_GridFree_Slater,
	DFT_GridFree_Becke,
	DFT_GridFree_Depristo,
	DFT_GridFree_CAMA,
	DFT_GridFree_HALF,
	DFT_GridFree_VWN,
	DFT_GridFree_PWLOC,
	DFT_GridFree_LYP,
	DFT_GridFree_BVWN,
	DFT_GridFree_BLYP,
	DFT_GridFree_BPWLOC,
	DFT_GridFree_B3LYP,
	DFT_GridFree_CAMB,
	DFT_GridFree_XVWN,
	DFT_GridFree_XPWLOC,
	DFT_GridFree_SVWN,
	DFT_GridFree_SPWLOC,
	DFT_GridFree_WIGNER,
	DFT_GridFree_WS,
	DFT_GridFree_WIGEXP,
	
	NumberDFTGridFreeFuncs
};
class DFTGroup {
	private:
		float	GridSwitch;
		float	Threshold;
		short	Functional;
//		short	NumRadialGrids;
//		short	NumThetaGrids;
//		short	NumPhiGrids;
//		short	NumRadialGridsInit;
//		short	NumThetaGridsInit;
//		short	NumPhiGridsInit;
		char	BitFlags;
	public:
		DFTGroup(void) {InitData();};
		DFTGroup(DFTGroup *Copy) {*this = *Copy;};
		const DFTGroup & operator=(const DFTGroup & other) {
			BitFlags=other.BitFlags; 
			GridSwitch=other.GridSwitch; Threshold=other.Threshold; Functional=other.Functional; 
//			NumRadialGrids=other.NumRadialGrids; NumThetaGrids=other.NumThetaGrids;
//			NumPhiGrids=other.NumPhiGrids; NumRadialGridsInit=other.NumRadialGridsInit; 
//			NumThetaGridsInit=other.NumThetaGridsInit; NumPhiGridsInit=other.NumPhiGridsInit; 
			return *this;};
		void InitData(void);
		
		bool MethodGrid(void) const {return ((BitFlags & 1) != 0);};
		void SetMethodGrid(bool state) {BitFlags = (BitFlags & 0xE) + (state ? 1 : 0);};
		bool GetAuxFunctions(void) const {return ((BitFlags & 2) != 0);};
		void SetAuxFunctions(bool state) {BitFlags = (BitFlags & 0xD) + (state ? 2 : 0);};
		bool GetThree(void) const {return ((BitFlags & 4) != 0);};
		void SetThree(bool state) {BitFlags = (BitFlags & 0xB) + (state ? 4 : 0);};
		inline short GetFunctional(void) const {return Functional;};
		const char * GetFunctionalText(void) const;
		short SetFunctional(short newvalue);
		/**
		 * Set the functional based on a text string containing a valid GAMESS DFTTYP.
		 * @param GAMESS_DFT_Type char * with a valid GAMESS DFTTYP string.
		 */
		short SetFunctional(const char * GAMESS_DFT_Type);
		static const char * GetDFTGridFuncText(DFTFunctionalsGrid d);
		static const char * GetDFTGridFreeFuncText(DFTFunctionalsGridFree d);

		void WriteToFile(BufferFile *File, InputData *IData);
		void WriteXML(XMLElement * parent) const;
		void ReadXML(XMLElement * parent);
};
enum OptMethod {
	invalidOptMethodType=0,
	StatPt_OptMethod_NR,
	StatPt_OptMethod_RFO,
	StatPt_OptMethod_QA,
	StatPt_OptMethod_Schlegel,
	StatPt_OptMethod_ConOpt,
	
	NumberStatPtOptMethods
};
enum HessUpdateMethod {
	invalidHessUpdateMethodType=0,
	StatPt_HessUpdateMethod_Guess,
	StatPt_HessUpdateMethod_Read,
	StatPt_HessUpdateMethod_Calc,
	
	NumberStatPtHessUpdateMethods
};
/**
 * Stationary Point related options. Container for options related to searching for a
 * minima or transition state (OPTIMIZE or SADPOINT).
 */
class StatPtGroup {
	private:
		float		OptConvergance;
		float		InitTrustRadius;
		float		MaxTrustRadius;
		float		MinTrustRadius;
		float		StatJumpSize;
		long		ModeFollow;
		/**
		 * bitfield for a group of true/false flags. The bits include:
		 * 1: radius update; 2: Stat. Pt.; 3-5 Hess method; 6: print orbs every iter.
		 * 7: Hessian at stationary point
		 */
		long		BitOptions;
		short		method;
		short		MaxSteps;
		short		nRecalcHess;
	public:
		StatPtGroup(void) {InitData();};
		StatPtGroup(StatPtGroup *Copy) {*this = *Copy;};
		void InitData(void);

		inline float GetOptConvergance(void) const {return OptConvergance;};
		inline void SetOptConvergance(float NewVal) {if (NewVal>0.0) OptConvergance = NewVal;};
		inline float GetInitRadius(void) const {return InitTrustRadius;};
		inline void SetInitRadius(float NewVal) {if (NewVal>0.0) InitTrustRadius = NewVal;};
		inline float GetMaxRadius(void) const {return MaxTrustRadius;};
		inline void SetMaxRadius(float NewVal) {if (NewVal>0.0) MaxTrustRadius = NewVal;};
		inline float GetMinRadius(void) const {return MinTrustRadius;};
		inline void SetMinRadius(float NewVal) {if (NewVal>0.0) MinTrustRadius = NewVal;};
		inline float GetStatJump(void) const {return StatJumpSize;};
		inline void SetStatJump(float NewVal) {if (NewVal>0.0) StatJumpSize = NewVal;};
		inline long GetModeFollow(void) const {return ModeFollow;};
		inline void SetModeFollow(long NewVal) {if (NewVal>0) ModeFollow = 1;};
		static const char * GetMethodText(OptMethod d);
		inline short GetMethod(void) const {return method;};
		inline void SetMethod(short NewVal) {if ((NewVal>0)&&(NewVal<=NumberStatPtOptMethods)) method = NewVal;};
		bool SetMethod(const char * text);
		inline short GetMaxSteps(void) const {return MaxSteps;};
		inline void SetMaxSteps(short NewVal) {if (NewVal>0) MaxSteps = NewVal;};
		inline short GetHessRecalcInterval(void) const {return nRecalcHess;};
		inline void SetHessRecalcInterval(short NewVal) {if (NewVal>=0) nRecalcHess = NewVal;};
		inline bool GetRadiusUpdate(void) const {return (BitOptions & 1);};
		inline void SetRadiusUpdate(bool NewVal) {BitOptions = (BitOptions & 0xFE) + (NewVal ? 1 : 0);};
		inline bool GetStatPoint(void) const {return ((BitOptions & 2) != 0);};
		inline void SetStatPoint(bool NewVal) {BitOptions = (BitOptions & 0xFD) + (NewVal ? 2 : 0);};
		inline short GetHessMethod(void) const {return ((BitOptions & 28) >> 2);};
		inline void SetHessMethod(short NewVal) {if ((NewVal>=1)&&(NewVal<=3)) BitOptions = (BitOptions & 0xE3) + (NewVal << 2);};
		bool SetHessMethod(const char * text);
		static const char * GetHessUpdateMethodText(HessUpdateMethod d);
		inline bool AlwaysPrintOrbs(void) const {return ((BitOptions & 32) != 0);};
		inline void SetAlwaysPrintOrbs(bool NewVal) {BitOptions = (BitOptions & 0xDF) + (NewVal ? 32 : 0);};
		/**
		 * Indicates whether a hessian will be computed on the final optimized structure.
		 */
		inline bool GetHessEndFlag(void) const {return ((BitOptions & 64) != 0);};
		/**
		 * Set the value of the flag to compute a hessian on the optimized structure.
		 * @param newValue bool value indicating whether to compute or not.
		 */
		inline void SetHessFlag(bool newValue) {BitOptions = (BitOptions & 0xBF) + (newValue ? 64 : 0);};
	
		/**
		 * Output the group options in GAMESS input format.
		 * @param File The output buffer.
		 * @param IData Pointer to the main input data class.
		 */
		void WriteToFile(BufferFile *File, InputData *IData);
		/**
		 Output the group to CML suitable for reading by the ReadXML routine.
		 @param parent the parent XML tag to append this group to
		 */
		void WriteXML(XMLElement * parent) const;
		/**
		 Load this groups non-default settings from CML.
		 @param parent the parent XML tag to append this group from
		 */
		void ReadXML(XMLElement * parent);
};

/**
 * Fragment Molecular Orbital primary option group.
 * There are a few basic options, but many of the options are arrays based either on the 
 * number of fragments or the number of atoms.
 */
class FMOGroup {
private:
	long	NumberFragments;	///< the number of fragments in which to divide up the system
	bool	Active;				///< Flag to turn FMO off or on
	bool	OutputStyle;		///< Flag indicating how the INDAT array will be written
public:
	FMOGroup(void) {InitData();};
	const FMOGroup & operator=(const FMOGroup & other) {Active=other.Active; NumberFragments=other.NumberFragments; return *this;};
	void InitData(void) {Active=false; NumberFragments=1; OutputStyle=false;};
	
	inline bool IsFMOActive(void) const {return Active;};
	void FMOActive(bool newState) {Active = newState;};
	inline long GetNumberFragments(void) const {return NumberFragments;};
	void SetNumberFragments(const long & num) {NumberFragments = num;};
		/** If true output INDAT as the atom list for each fragment, else atom ordered list */
	inline bool UseFragmentINDAT(void) const {return OutputStyle;};
	void UseFragmentINDAT(bool choice) {OutputStyle = choice;};	///< Pass true for INDAT ordered by fragment
	
	/**
	 * Output the group options in GAMESS input format.
	 * @param File The output buffer.
	 * @param MainData Pointer to the main data class.
	 */
	void WriteToFile(BufferFile *File, MoleculeData * MainData);
	/**
	 * Output the group to CML suitable for reading by the ReadXML routine.
	 * @param parent the parent XML tag to append this group to
	 * @param MainData Pointer to the main data class.
	 */
	void WriteXML(XMLElement * parent, MoleculeData * MainData) const;
	/**
	 * Load this groups non-default settings from CML.
	 * @param parent the parent XML tag to append this group from
	 * @param MainData Pointer to the main data class.
	 */
	void ReadXML(XMLElement * parent, MoleculeData * MainData);
	
};

class MolDisplayWin;

class InputData {
	public:
		ControlGroup	*Control;
		SystemGroup		*System;
		BasisGroup		*Basis;
		DataGroup		*Data;
		GuessGroup		*Guess;
		SCFGroup		*SCF;
		MP2Group		*MP2;
		HessianGroup	*Hessian;
		StatPtGroup		*StatPt;
		DFTGroup		DFT;
		EffectiveFragmentsGroup	EFP;
		FMOGroup		FMO;
			//member functions
		InputData(void);
		InputData(InputData *Copy);
		~InputData(void);
		//! Write out an input file for another program (GAMESS etc)
		long WriteInputFile(MoleculeData * lData, MolDisplayWin * owner);

		//! Write out an input file for another program (GAMESS etc)
		long WriteInputFile(const wxString &filePath, MoleculeData * lData, MolDisplayWin * owner);
		void WriteXML(XMLElement * parent, MoleculeData * p) const;
		void ReadXML(XMLElement * parent, MoleculeData * p);
};

#endif
