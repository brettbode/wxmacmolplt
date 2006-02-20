/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* Prefs.h

	classes related to display and other user definable preferences.

	BMB 4/1998
	Added flag for setting creator type, BMB 1/2001
	Fixed 2 prefs accessors to return true boolean values, BMB 9/2001
	All prefs accessors should now return true boolean values, BMB 11/2001
*/

struct RGBColor {
    unsigned short red;
    unsigned short green;
    unsigned short blue;
};
typedef struct RGBColor RGBColor;
//typedef RGBColor * RGBColorPtr;

#ifndef __Prefs__
#define __Prefs__
class SurfaceOptions {
	private:
		long			NumContours;		// Max. # of contours (+ or -) in 2D map
		long			NumGridPoints;		// Default # of grid points
		float			GridSize;			// size of grid box
		float			MaxContourValue;	// Maximum 2D contour value;
		RGBColor		PosColor;			// color for + part of contour
		RGBColor		NegColor;			// color for - part of contour
		char			Options;			// bit 1 - show zero value contour
											// bit 2 - rotate 2D map
											// bit 3 - show 2D hash marks
	public:
		long GetNumContours(void) const {return NumContours;};
		long SetNumContours(long NewNum);
		long GetNumGridPoints(void) const {return NumGridPoints;};
		long SetNumGridPoints(long NewNum);
		float GetGridSize(void) const {return GridSize;};
		float SetGridSize(float NewSize);
		float GetMaxContour(void) const {return MaxContourValue;};
		float SetMaxContour(float NewMax);
		void GetPosColor(RGBColor *Color) const {*Color = PosColor;};
		void SetPosColor(RGBColor *Color) {PosColor = *Color;};
		void SetPosColor(void) const {RGBForeColor(&PosColor);};
		void GetNegColor(RGBColor *Color) const {*Color = NegColor;};
		void SetNegColor(RGBColor *Color) {NegColor = *Color;};
		void SetNegColor(void) const {RGBForeColor(&NegColor);};
		Boolean GetShowZeroContour(void) const {return (Options & 1);};
		Boolean SetShowZeroContour(Boolean State);
		Boolean GetRotate2DMap(void) const {return ((Options & (1<<1))?true:false);};
		Boolean SetRotate2DMap(Boolean State);
		Boolean Get2DHashMarks(void) const {return ((Options & (1<<2))?true:false);};
		Boolean Set2DHashMarks(Boolean State);
};
class EnergyOptions {
	public:
		Boolean PlotEnergy(void) const {return (PlotFlag & 1);};
		Boolean PlotMPEnergy(void) const {return ((PlotFlag & 2)?true:false);};
		Boolean PlotKEnergy(void) const {return ((PlotFlag & 4)?true:false);};
		Boolean PlotPEnergy(void) const {return ((PlotFlag & 8)?true:false);};
		Boolean PlotOther(void) const {return ((PlotFlag & 32)?true:false);};
		EnergyUnit GetDisplayUnits(void) const {return (EnergyUnit)((PlotFlag & 16) >> 4);};
		void SetPlotEnergy(Boolean State) {if (PlotFlag & 1) PlotFlag--; if (State) PlotFlag++;};
		void SetPlotMPEnergy(Boolean State) {if (PlotFlag & 2) PlotFlag-=2; if (State) PlotFlag+=2;};
		void SetPlotKEnergy(Boolean State) {if (PlotFlag & 4) PlotFlag-=4; if (State) PlotFlag+=4;};
		void SetPlotPEnergy(Boolean State) {if (PlotFlag & 8) PlotFlag-=8; if (State) PlotFlag+=8;};
		void SetDisplayUnits(EnergyUnit NewUnits) {PlotFlag = ((PlotFlag & 0xEF)+(NewUnits << 4));};
		void SetPlotOther(Boolean State) {PlotFlag = (PlotFlag & 0xDF) + (State?32:0);};
		float GetY1Zero(void) const {return y1Zero;};
		float GetY2Zero(void) const {return y2Zero;};
		void SetY1Zero(float NewValue) {y1Zero = NewValue;};
		void SetY2Zero(float NewValue) {y2Zero = NewValue;};
		short GetNumDigits(void) const {return nDigits;};
		void SetNumDigits(short newValue) {nDigits = newValue;};
		long GetEPlotPointSize(void) const {return EPlotBallSize;};
		long SetEPlotPointSize(long NewSize) {return EPlotBallSize = NewSize;};
		void SetTEColor(void) {RGBForeColor(&TEColor);};
		void SetMPColor(void) {RGBForeColor(&MPColor);};
		void SetKEColor(void) {RGBForeColor(&KEColor);};
		void SetPEColor(void) {RGBForeColor(&PEColor);};
		const RGBColor * GetTEColorPtr(void) const {return &TEColor;};
		const RGBColor * GetMPColorPtr(void) const {return &MPColor;};
		const RGBColor * GetKEColorPtr(void) const {return &KEColor;};
		const RGBColor * GetPEColorPtr(void) const {return &PEColor;};
		RGBColor * GetTEColor(void) {return &TEColor;};
		RGBColor * GetMPColor(void) {return &MPColor;};
		RGBColor * GetKEColor(void) {return &KEColor;};
		RGBColor * GetPEColor(void) {return &PEColor;};
		float GetYAdjustment(void) const {return yAdjustFactor;};
		void SetYAdjustment(float newval) {yAdjustFactor = newval;};
	private:
		float		y1Zero;
		float		y2Zero;
		float		yAdjustFactor;
		float		xAdjustFactor;
		long		EPlotBallSize;
		RGBColor	TEColor;
		RGBColor	MPColor;
		RGBColor	KEColor;
		RGBColor	PEColor;
		short		nDigits;
		char		PlotFlag;		//indicates what to plot (1=TE,2=MP,3=KE,4=PE,5=kcal/mol,
									//6=other-use graph options)
};
class GraphOptions {
	public:
		GraphOptions(void) {Options=0; atom1 = atom2 = atom3 = atom4 = 0;};
		Boolean PlotRMSGradient(void) const {return (Options & 1);};
		Boolean PlotMaxGradient(void) const {return ((Options & 2)?true:false);};
		Boolean PlotBondLength(void) const {return ((Options & 4)?true:false);};
		Boolean PlotBondAngle(void) const {return ((Options & 8)?true:false);};
		Boolean PlotDihedral(void) const {return ((Options & 16)?true:false);};
		void SetPlotRMSGradient(Boolean State) {Options = (Options & 0xFFFFFFFE) + (State?1:0);};
		void SetPlotMaxGradient(Boolean State) {Options = (Options & 0xFFFFFFFD) + (State?2:0);};
		void SetPlotBondLength(Boolean State) {Options = (Options & 0xFFFFFFFB) + (State?4:0);};
		void SetPlotBondAngle(Boolean State) {Options = (Options & 0xFFFFFFF7) + (State?8:0);};
		void SetPlotDihedral(Boolean State) {Options = (Options & 0xFFFFFFEF) + (State?16:0);};
		long Get1stAtom(void) const {return atom1;};
		long Get2ndAtom(void) const {return atom2;};
		long Get3rdAtom(void) const {return atom3;};
		long Get4thAtom(void) const {return atom4;};
		void Set1stAtom(long a1) {atom1 = a1;};
		void Set2ndAtom(long a2) {atom2 = a2;};
		void Set3rdAtom(long a3) {atom3 = a3;};
		void Set4thAtom(long a4) {atom4 = a4;};
	private:
		long		Options;		//flags on what to plot: 1=rms, 2=max grad, 3=bond length,
									//4=bond angle, 5=dihedral
		long		atom1;
		long		atom2;
		long		atom3;
		long		atom4;
};

typedef class XMLElement XMLElement;
class WinPrefs {
	private:
		RGBColor		AtomColors[kMaxAtomTypes], BondColors[kMaxBondTypes], VectorColor, BackColor;
		long			AtomSizes[kMaxAtomTypes], AnimateTime, DRCnFileSkip,
						QD3DAtomQuality, BitOptions;	//bit 1: prompt4save, bit 2: show angles, bit 3: use QD3D hardware
														//bit 4: Stereo, bit 5: color bond halves, bit6: Wireframe mode
														//bit 7: Outline bonds, bit 8: Use cylinders for lines
														//bit 9: Look for H bonds, bit 10: determine bond order
														//bit 11: Default for 3D mode, bit 12: native EOL chars
														//bit 13: Create custom file icon
		float			AtomMasses[kMaxAtomTypes], AutoBondScale, BondWidth, AtomScale,
						ZScale, VectorScale, QD3DFillBrightness, QD3DPointBrightness,
						MaxBondLength, QD3DLineWidth;
		short			AtomPatterns[kMaxAtomTypes], LabelFontID, LabelSize, VectorPattern,
						BondPatterns[kMaxBondTypes], AnimationSpeed, StereoOffset;
		unsigned char	AtomLabels[kMaxAtomTypes][3];
		char *			RendererName;
		Boolean			RotateMode, AnimateMode, AutoBondFlag, HHBondFlag, SetCreator;
		EnergyOptions	EnergyPlotOptions;
		GraphOptions	PlotOptions;
		SurfaceOptions	SurfaceOpts;
		
	public:
		WinPrefs(void);
		~WinPrefs(void);
		void WriteUserPrefs(void) const;
		long WriteMMPPrefs(XMLElement * root) const;
		long ReadMMPPrefs(XMLElement * root);
		void ReadDefaultPrefs(void);
		void ReadUserPrefs_old(void);
		bool ReadUserPrefs(void);
		void ReadAtomDefaults(void);
		void ReadBondDefaults(void);
		void ReadDisplayDefaults(void);
		void ReadEnergyDefaults(void);
		void ReadFontDefaults(void);
		void ReadFileDefaults(void);
		void ReadScalingDefaults(void);
		void ReadStereoDefaults(void);
		void ReadSurfaceDefaults(void);
		void ReadQD3DDefaults(void);
		void CopyAtomPrefs(WinPrefs * Orig);
		void CopyBondPrefs(WinPrefs * Orig);
		void CopyDisplayPrefs(WinPrefs * Orig);
		void CopyEnergyPrefs(WinPrefs * Orig);
		void CopyFontPrefs(WinPrefs * Orig);
		void CopyFilePrefs(WinPrefs * Orig);
		void CopyScalingPrefs(WinPrefs * Orig);
		void CopySurfacePrefs(WinPrefs * Orig);
		inline void CopyStereoPrefs(WinPrefs * Orig) {UseStereo(Orig->UseStereo()); SetStereoOffset(Orig->GetStereoOffset());};
		void CopyQD3DPrefs(WinPrefs * Orig);
		void ReadFilePrefs(short fileRef);
		void WriteFilePrefs(short fileRef);
		void WriteAllPrefs(short FileRef);
		inline char * GetRendererName(void) const {return RendererName;};
		void SetRendererName(char * NewName);
		inline Boolean Get2DHashMarks(void) {return SurfaceOpts.Get2DHashMarks();};
		inline Boolean GetPrompt4Save(void) const {return (BitOptions & 1);};
		inline Boolean SetPrompt4Save(Boolean State) {if (BitOptions&1) BitOptions--; if (State) BitOptions++; return State;};
		inline Boolean GetShowAngles(void) const {return ((BitOptions&(1<<1)?true:false));};
		inline Boolean SetShowAngles(Boolean state) {if (BitOptions&(1<<1)) BitOptions-=(1<<1); if (state) BitOptions+=(1<<1); return state;};
		inline Boolean UseQD3DHardware(void) const {return ((BitOptions & (1<<2)?true:false));};
		inline void UseQD3DHardware(Boolean State) {BitOptions = (BitOptions & 0xFFFFFFFB) + (State?4:0);};
		inline Boolean UseStereo(void) const {return ((BitOptions & (1<<3))?true:false);};
		inline void UseStereo(Boolean State) {BitOptions = (BitOptions & 0xFFFFFFF7) + (State?8:0);};
		inline Boolean ColorBondHalves(void) const {return ((BitOptions & (1<<4))?true:false);};
		inline void ColorBondHalves(Boolean State) {BitOptions = (BitOptions & 0xFFFFFFEF) + (State?16:0);};
		inline Boolean DrawBallnStick(void) const {return ((BitOptions & (1<<5))==0);};
		inline void DrawBallnStick(Boolean State) {BitOptions = (BitOptions & 0xFFFFFFDF) + (State?0:32);};
		inline Boolean DrawWireFrame(void) const {return ((BitOptions & (1<<5))?true:false);};
		inline void DrawWireFrame(Boolean State) {BitOptions = (BitOptions & 0xFFFFFFDF) + (State?32:0);};
		inline Boolean OutLineBonds(void) const {return ((BitOptions & (1<<6))?true:false);};
		inline void OutLineBonds(Boolean State) {BitOptions = (BitOptions & 0xFFFFFFBF) + (State?64:0);};
		inline Boolean CylindersForLines(void) const {return ((BitOptions & (1<<7))?true:false); };
		inline void CylindersForLines(Boolean newVal) {BitOptions = (BitOptions & 0xFFFFFF7F) + (newVal?128:0);};
		inline Boolean AllowHydrogenBonds(void) const {return ((BitOptions & (1<<8))?true:false); };
		inline void AllowHydrogenBonds(Boolean newVal) {BitOptions = (BitOptions & 0xFFFFFEFF) + (newVal?256:0);};
		inline Boolean DetermineBondOrder(void) const {return ((BitOptions & (1<<9))?true:false); };
		inline void DetermineBondOrder(Boolean newVal) {BitOptions = (BitOptions & 0xFFFFFDFF) + (newVal?(1<<9):0);};
		inline Boolean Default3DOn(void) const {return ((BitOptions & (1<<10))?true:false); };
		inline void Default3DOn(Boolean newVal) {BitOptions = (BitOptions & 0xFFFFFBFF) + (newVal?(1<<10):0);};
			//Note the meaning of the Native EOL bit is reversed
		inline Boolean NativeEOLChar(void) const {return ((BitOptions & (1<<11))?false:true); };
		inline void NativeEOLChar(Boolean newVal) {BitOptions = (BitOptions & 0xFFFFF7FF) + (newVal?0:(1<<11));};
			//Set to indicate a custom thumbnail should be created when saving
		inline bool CreateCustomIcon(void) const {return ((BitOptions & (1<<12))?false:true); };
		inline void CreateCustomIcon(bool newVal) {BitOptions = (BitOptions & 0xFFFFEFFF) + (newVal?0:(1<<12));};
		inline Boolean ChangeFileType(void) const {return (SetCreator); };
		inline void ChangeFileType(Boolean newVal) {SetCreator = (newVal != 0);};
		inline short GetStereoOffset(void) const {return StereoOffset;};
		inline void SetStereoOffset(short newval) {StereoOffset = newval;};
		inline float GetQD3DLineWidth(void) const {return (CylindersForLines() ? QD3DLineWidth : 0.0);};
		inline void SetQD3DLineWidth(float newVal) {if (newVal >= 0) QD3DLineWidth = newVal;};
		inline EnergyOptions * GetEnergyOptions(void) {return &EnergyPlotOptions;};
		inline GraphOptions * GetGraphOptions(void) {return &PlotOptions;};
		inline SurfaceOptions * GetSurfaceOptions(void) {return &SurfaceOpts;};
		inline float SetMaxBondLength(float NewLength) {return (MaxBondLength = NewLength);};
		inline float GetMaxBondLength(void) const {return MaxBondLength;}
		inline float GetQD3DBondWidth(void) const {return BondWidth;};
		inline float SetQD3DBondWidth(float NewValue) {return BondWidth=NewValue;};
		inline long GetQD3DAtomQuality(void) const {return QD3DAtomQuality;};
		inline long SetQD3DAtomQuality(long NewValue) {return QD3DAtomQuality = NewValue;};
		inline float GetQD3DFillBrightness(void) const {return QD3DFillBrightness;};
		inline float SetQD3DFillBrightness(float NewValue) {return QD3DFillBrightness = NewValue;};
		inline float GetQD3DPointBrightness(void) const {return QD3DPointBrightness;};
		inline float SetQD3DPointBrightness(float NewValue) {return QD3DPointBrightness = NewValue;};
		inline float GetAtomScale(void) const {return AtomScale;};
		inline float SetAtomScale(float NewValue) {return AtomScale=NewValue;};
		inline float GetZScale(void) const {return ZScale;};
		inline float SetZScale(float NewValue) {return ZScale=NewValue;};
		inline long GetAnimateTime(void) const {return AnimateTime;};
		inline long SetAnimateTime(long NewValue) {return AnimateTime=NewValue;};
		inline short GetAnimationSpeed(void) const {return AnimationSpeed;};
		inline short SetAnimationSpeed(short NewValue) {return AnimationSpeed=NewValue;};
		inline Boolean GetAutoBond(void) const {return AutoBondFlag;};
		inline Boolean SetAutoBond(Boolean state) {return AutoBondFlag = state;};
		inline Boolean GetHHBondFlag(void) const {return HHBondFlag;};
		inline Boolean SetHHBondFlag(Boolean state) {return HHBondFlag = state;};
		inline float GetAutoBondScale(void) const {return AutoBondScale;};
		inline float SetAutoBondScale(float NewValue) {return AutoBondScale = NewValue;};
		inline long GetDRCSkip(void) const {return DRCnFileSkip;};
		inline long SetDRCSkip(long NewValue) {return DRCnFileSkip=NewValue;};
		inline short GetLabelFontID(void) const {return LabelFontID;};
		inline short SetLabelFontID(short NewID) {return LabelFontID=NewID;};
		inline short GetLabelSize(void) const {return LabelSize;};
		inline short SetLabelSize(short NewValue) {return LabelSize=NewValue;};
		inline void SetAtomColor(long AtomNum) {RGBForeColor(&(AtomColors[AtomNum]));};
		void SetAtomPattern(long AtomNum);
		inline RGBColor * GetAtomColorLoc(long AtomNum) {return &(AtomColors[AtomNum]);};
		inline short * GetAtomPatternLoc(long AtomNum) {return &(AtomPatterns[AtomNum]);};
		inline RGBColor * GetBondColorLoc(long BondOrder) {return &(BondColors[BondOrder]);};
		inline short * GetBondPatternLoc(long BondOrder) {return &(BondPatterns[BondOrder]);};
		inline RGBColor * GetVectorColorLoc(void) {return &VectorColor;};
		inline RGBColor * GetBackgroundColorLoc(void) {return &BackColor;};
		inline short * GetVectorPatternLoc(void) {return &VectorPattern;};
		inline void SetBondColor(long BondOrder) {RGBForeColor(&(BondColors[BondOrder]));};
		void SetBondPattern(long BondOrder);
		inline void SetVectorColor(void) const {RGBForeColor(&VectorColor);};
		void SetVectorPattern(void);
		inline float GetVectorScale(void) const {return VectorScale;};
		inline float SetVectorScale(float NewValue) {return VectorScale = NewValue;};
		inline unsigned char * GetAtomLabelLoc(void) {return &(AtomLabels[0][0]);};
		void GetAtomLabel(long AtomNum, Str255 text);
		void SetAtomLabel(long AtomNum, Str255 text);
		inline long * GetAtomSizeLoc(void) {return AtomSizes;};
		inline long GetAtomSize(long AtomNum) const {return AtomSizes[AtomNum];};
		long SetAtomSize(long AtomNum, long NewSize);
		inline float * GetAtomMassLoc(void) {return AtomMasses;};
		inline float GetAtomMass(long AtomNum) const {return (AtomMasses[AtomNum]*AtomMasses[AtomNum]);};
		inline float GetSqrtAtomMass(long AtomNum) const {return AtomMasses[AtomNum];};
		float SetAtomMass(long AtomNum, float NewMass);
		inline Boolean GetRotateMode(void) const {return RotateMode;};
		inline Boolean SetRotateMode(Boolean state) {return RotateMode = state;};
		inline Boolean GetAnimateMode(void) const {return AnimateMode;};
		inline Boolean SetAnimateMode(Boolean state) {return AnimateMode=state;};
};

// New preferences classes below here..
/*
class FileOptions {
private:
	long	DRCnFileSkip;
	bool	SetCreator;
	bool	CreateCustomFileIcon;
	bool	Prompt4Save;
	bool	EOLCharMode;
	bool	SetBondOrder;
	bool	CreateHBonds;
public:
//	FileOptions(void);
	FileOptions(const FileOptions * source);
//	~FileOptions(void);
	bool SetFileCreator(void) const {return SetCreator;};
	void SetFileCreator(bool v) {SetCreator = v;};
	bool CreateCustomFileIconOnSave(void) const {return CreateCustomFileIcon;};
	void CreateCustomFileIconOnSave(bool v) {CreateCustomFileIcon = v;};
	bool GetPrompt4Save(void) const {return Prompt4Save;};
	void SetPrompt4Save(bool v) {Prompt4Save = v;};
	bool GetEOLCharMode(void) const {return EOLCharMode;};
	void SetEOLCharMode(bool v) {EOLCharMode = v;};
	bool DetermineBondOrder(void) const {return SetBondOrder;};
	void DetermineBondOrder(bool v) {SetBondOrder = v;};
	bool AllowHydrogenBonds(void) const {return CreateHBonds;};
	void AllowHydrogenBonds(bool v) {CreateHBonds = v;};
};

//class to hold various attributes related to elements
class AtomOptions {
private:
	RGBColor	AtomColor;	//Ok should this stay the Carbon specific RGBColor or move to floats?
	long		AtomicNumber;
	long		AtomSize;
	float		AtomMass;
	string		AtomLabel;
	short		AtomPattern;
public:
};

class CoordWindowOptions {
private:
	//Only thing I can think of is store the size and position for the window
	Rect	CoordWindowRect;
public:
};

class BondsWindowOptions {
private:
	//Only thing I can think of is store the size and position for the window
	Rect	BondsWindowRect;
public:
};

class FrequencyWindowOptions {
private:
	//Only thing I can think of is store the size and position for the window
	Rect	FreqWindowRect;
public:
};

class SurfaceWindowOptions {
private:
	Rect			SurfaceWindowRect;
	long			NumContours;		// Max. # of contours (+ or -) in 2D map
	long			NumGridPoints;		// Default # of grid points
	float			GridSize;			// size of grid box
	float			MaxContourValue;	// Maximum 2D contour value;
	RGBColor		PosColor;			// color for + part of contour
	RGBColor		NegColor;			// color for - part of contour
	char			Options;			// bit 1 - show zero value contour
										// bit 2 - rotate 2D map
										// bit 3 - show 2D hash marks
public:
};

class MMPPrefs {
private:
	FileOptions				FileOpts;
	std::vector<AtomOptions>	AtomPrefs;
	CoordWindowOptions		CoordWinOpts;
	BondsWindowOptions		BondsOpts;
	MolDisplayOptions		MolDisOpts;
	FrequencyWindowOptions	FreqOpts;
	SurfaceWindowOptions	SurfaceOpts;
public:
	MMPPrefs(void);
	MMPPrefs(const MMPPrefs * source);
	~MMPPrefs(void);
};
*/

#endif
