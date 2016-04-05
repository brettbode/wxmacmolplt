/*
 *  (c) 2004-2006 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* Prefs.h

	classes related to display and other user definable preferences.

	BMB 4/1998
	Added flag for setting creator type, BMB 1/2001
	Fixed 2 prefs accessors to return true boolean values, BMB 9/2001
	All prefs accessors should now return true boolean values, BMB 11/2001
*/

#ifndef __Prefs__
#define __Prefs__

#ifdef __wxBuild__
#include "wx/wx.h"

#endif

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
		SurfaceOptions() { Options = 0; }
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
#ifndef __wxBuild__
		void SetPosColor(void) const {RGBForeColor(&PosColor);};
#endif
		void GetNegColor(RGBColor *Color) const {*Color = NegColor;};
		void SetNegColor(RGBColor *Color) {NegColor = *Color;};
#ifndef __wxBuild__
		void SetNegColor(void) const {RGBForeColor(&NegColor);};
#endif
		void SetPosColor(const RGBColor & color) {PosColor = color;};
		void SetNegColor(const RGBColor & color) {NegColor = color;};

		bool GetShowZeroContour(void) const {return (Options & 1);};
		bool SetShowZeroContour(bool State);
		bool GetRotate2DMap(void) const {return ((Options & (1<<1))?true:false);};
		bool SetRotate2DMap(bool State);
		bool Get2DHashMarks(void) const {return ((Options & (1<<2))?true:false);};
		bool Set2DHashMarks(bool State);
};

class EnergyOptions {
	public:
	EnergyOptions(void) {nDigits=4; eUnits=kDefault; PlotFlag=0;};
		bool PlotEnergy(void) const {return (PlotFlag & 1);};
		bool PlotMPEnergy(void) const {return ((PlotFlag & 2)?true:false);};
		bool PlotKEnergy(void) const {return ((PlotFlag & 4)?true:false);};
		bool PlotPEnergy(void) const {return ((PlotFlag & 8)?true:false);};
		bool PlotOther(void) const {return ((PlotFlag & 32)?true:false);};
		EnergyUnit GetDisplayUnits(void) const {return eUnits;};
		void SetPlotEnergy(bool State) {if (PlotFlag & 1) PlotFlag--; if (State) PlotFlag++;};
		void SetPlotMPEnergy(bool State) {if (PlotFlag & 2) PlotFlag-=2; if (State) PlotFlag+=2;};
		void SetPlotKEnergy(bool State) {if (PlotFlag & 4) PlotFlag-=4; if (State) PlotFlag+=4;};
		void SetPlotPEnergy(bool State) {if (PlotFlag & 8) PlotFlag-=8; if (State) PlotFlag+=8;};
		void SetDisplayUnits(EnergyUnit NewUnits) {eUnits = NewUnits;};
		void SetPlotOther(bool State) {PlotFlag = (PlotFlag & 0xDF) + (State?32:0);};
		float GetY1Zero(void) const {return y1Zero;};
		float GetY2Zero(void) const {return y2Zero;};
		void SetY1Zero(float NewValue) {y1Zero = NewValue;};
		void SetY2Zero(float NewValue) {y2Zero = NewValue;};
		/// Get the number of digits to print as labels
		short GetNumDigits(void) const {return nDigits;};
		/// Set the number of digits to print as labels ( > 0)
		void SetNumDigits(const short & newValue) {
			if ((newValue>0)&&(newValue<20)) nDigits = newValue;};
		/// Get the screen space indicator point size
		long GetEPlotPointSize(void) const {return EPlotBallSize;};
		///Set the screen space indicator size (1 to 20)
		long SetEPlotPointSize(const long & NewSize) {
			if ((NewSize >= 1)&&(NewSize <= 20)) EPlotBallSize = NewSize;
			return EPlotBallSize;};
#ifndef __wxBuild__
		void SetTEColor(void) {RGBForeColor(&TEColor);};
		void SetMPColor(void) {RGBForeColor(&MPColor);};
		void SetKEColor(void) {RGBForeColor(&KEColor);};
		void SetPEColor(void) {RGBForeColor(&PEColor);};
#endif
		void SetTEColor(const RGBColor & color) {TEColor = color;};
		void SetMPColor(const RGBColor & color) {MPColor = color;};
		void SetKEColor(const RGBColor & color) {KEColor = color;};
		void SetPEColor(const RGBColor & color) {PEColor = color;};

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
		EnergyUnit	eUnits;
		char		PlotFlag;	//indicates what to plot (1=TE,2=MP,3=KE,4=PE,5=kcal/mol,
								//6=other-use graph options)
};

class GraphOptions {
	public:
		GraphOptions(void) {Options=0; atom1 = atom2 = atom3 = atom4 = 0;};
		bool PlotRMSGradient(void) const {return (Options & 1);};
		bool PlotMaxGradient(void) const {return ((Options & 2)?true:false);};
		bool PlotBondLength(void) const {return ((Options & 4)?true:false);};
		bool PlotBondAngle(void) const {return ((Options & 8)?true:false);};
		bool PlotDihedral(void) const {return ((Options & 16)?true:false);};
		void SetPlotRMSGradient(bool State) {Options = (Options & 0xFFFFFFFE) + (State?1:0);};
		void SetPlotMaxGradient(bool State) {Options = (Options & 0xFFFFFFFD) + (State?2:0);};
		void SetPlotBondLength(bool State) {Options = (Options & 0xFFFFFFFB) + (State?4:0);};
		void SetPlotBondAngle(bool State) {Options = (Options & 0xFFFFFFF7) + (State?8:0);};
		void SetPlotDihedral(bool State) {Options = (Options & 0xFFFFFFEF) + (State?16:0);};
		long Get1stAtom(void) const {return atom1;};
		long Get2ndAtom(void) const {return atom2;};
		long Get3rdAtom(void) const {return atom3;};
		long Get4thAtom(void) const {return atom4;};
		void Set1stAtom(long a1) {atom1 = a1;};
		void Set2ndAtom(long a2) {atom2 = a2;};
		void Set3rdAtom(long a3) {atom3 = a3;};
		void Set4thAtom(long a4) {atom4 = a4;};
	private:
		long	Options;	//flags on what to plot: 1=rms, 2=max grad, 3=bond length,
							//4=bond angle, 5=dihedral
		long	atom1;
		long	atom2;
		long	atom3;
		long	atom4;
};

class FrequencyWindowOptions {
private:
    //Only thing I can think of is store the size and position for the window
//    Rect    FreqWindowRect;
	float	YScaleFactor;
	bool	ShowRaman;
	bool	ShowIR;
public:
	FrequencyWindowOptions(void);
	float GetYScaleFactor(void) const {return YScaleFactor;};
	void SetYScaleFactor(const float & val) {YScaleFactor = val;};
	bool ShowRamanIntensity(void) const {return ShowRaman;};
	bool ShowIRIntensity(void) const {return ShowIR;};
	void ShowRamanIntensity(bool val) {ShowRaman = val;};
	void ShowIRIntensity(bool val) {ShowIR = val;};
};

typedef class XMLElement XMLElement;
typedef class wxString wxString;

class WinPrefs {
	private:
		RGBColor		AtomColors[kMaxAtomTypes], BondColors[kMaxBondTypes], VectorColor, BackColor;
		long			AtomSizes[kMaxAtomTypes], AnimateTime, DRCnFileSkip,
						QD3DAtomQuality, BitOptions;
				//bit 1: prompt4save, bit 2: show angles, bit 3: use QD3D hardware
				//bit 4: Stereo, bit 5: color bond halves, bit6: Wireframe mode
				//bit 7: Outline bonds, bit 8: Use cylinders for lines
				//bit 9: Look for H bonds, bit 10: determine bond order
				//bit 11: Default for 3D mode, bit 12: native EOL chars
				//bit 13: Create custom file icon, bit 14: Show 2D atom patterns
				//bit 15: Show EFP atoms in wireframe mode

		float			AtomMasses[kMaxAtomTypes], AutoBondScale, BondWidth, AtomScale,
						GLFOV, VectorScale, QD3DFillBrightness, QD3DPointBrightness,
						MaxBondLength, QD3DLineWidth, AtomLabelSize, AnnotationLabelSize;
		short			AtomPatterns[kMaxAtomTypes], LabelFontID, LabelSize, VectorPattern,
						BondPatterns[kMaxBondTypes], AnimationSpeed, StereoOffset;
		unsigned char	AtomLabels[kMaxAtomTypes][3];
		char *			RendererName;
		Boolean			RotateMode, AnimateMode, AutoBondFlag, HHBondFlag, SetCreator;
		bool			FitToPage, CenterOnPage, FrameOnPage, SymbolLabels, NumberLabels, ShowSymOps,
						AutoRotating,		///< Is the window set to auto-rotate?
						AllowAutoRotation;	///< Flag to enable/disable autorotation
		EnergyOptions	EnergyPlotOptions;
		GraphOptions	PlotOptions;
		SurfaceOptions	SurfaceOpts;
		FrequencyWindowOptions	FreqWinOpts;
		bool show_toolbar;
		int shader_mode;
		int lineWidth, autoSpeedX, autoSpeedY, autoSpeedZ;

	public:
		WinPrefs(void);
		~WinPrefs(void);
		void WriteUserPrefs(void) const;
		long WriteMMPPrefs(XMLElement * root) const;
		long ReadMMPPrefs(XMLElement * root);
		void ReadDefaultPrefs(void);
		void ReadUserPrefs_old(void);
		bool ReadUserPrefs(void);
		void resetAllDefaults(void);
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
		inline FrequencyWindowOptions * GetFrequencyWindowOptions(void) {return &FreqWinOpts;};
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
		inline bool CylindersForLines(void) const {return ((BitOptions & (1<<7))?true:false); };
		inline void CylindersForLines(bool newVal) {BitOptions = (BitOptions & 0xFFFFFF7F) + (newVal?128:0);};
		inline Boolean AllowHydrogenBonds(void) const {return ((BitOptions & (1<<8))?true:false); };
		inline void AllowHydrogenBonds(Boolean newVal) {BitOptions = (BitOptions & 0xFFFFFEFF) + (newVal?256:0);};
		inline Boolean DetermineBondOrder(void) const {return ((BitOptions & (1<<9))?true:false); };
		inline void DetermineBondOrder(Boolean newVal) {BitOptions = (BitOptions & 0xFFFFFDFF) + (newVal?(1<<9):0);};
		inline Boolean Default3DOn(void) const { return ((BitOptions & (1<<10))?true:false); };
		inline void Default3DOn(Boolean newVal) {BitOptions = (BitOptions & 0xFFFFFBFF) + (newVal?(1<<10):0);};
		inline bool GetCenterOnPage(void) const {return CenterOnPage;};
		inline void SetCenterOnPage(bool v) {CenterOnPage = v;};
		inline bool GetFitToPage(void) const {return FitToPage;};
		inline void SetFitToPage(bool v) {FitToPage = v;};
		inline bool GetFramePage(void) const {return FrameOnPage;};
		inline void SetFramePage(bool v) {FrameOnPage = v;};
		inline bool ShowAtomicSymbolLabels(void) const {return SymbolLabels;};
		inline void ShowAtomicSymbolLabels(bool v) {SymbolLabels = v;};
		inline bool ShowAtomNumberLabels(void) const {return NumberLabels;};
		inline void ShowAtomNumberLabels(bool v) {NumberLabels = v;};
		inline bool ShowSymmetryOperators(void) const {return ShowSymOps;};
		inline void ShowSymmetryOperators(bool v) {ShowSymOps = v;};
		/// Flag to determine wether to allow the auto rotate feature
		inline bool AutoRotationEnabled(void) const {return AllowAutoRotation;};
		/// Set function to allow auto rotation via the click and drag method
		inline void EnableAutoRotation(bool v) {AllowAutoRotation = v;};
		/// Is the window set to be auto-rotating
		inline bool isAutoRotating(void) const {return AutoRotating;};
		/// Set function for the auto rotation feature
		inline void SetAutoRotating(bool v) {AutoRotating = v;};
		/// Set the x, y and z components for the auto rotation direction
		inline void SetAutoRotationVector(int x, int y, int z) {autoSpeedX = x; autoSpeedY = y; autoSpeedZ = z;};
		/// Get the components for the auto rotation vector
		inline void GetAutoRotationVector(int & x, int & y, int & z) {x = autoSpeedX; y = autoSpeedY; z = autoSpeedZ;};
		//Note the meaning of the Native EOL bit is reversed
		inline bool NativeEOLChar(void) const {return ((BitOptions & (1<<11))?false:true); };
		inline void NativeEOLChar(bool newVal) {BitOptions = (BitOptions & 0xFFFFF7FF) + (newVal?0:(1<<11));};
		//Set to indicate a custom thumbnail should be created when saving
		inline bool CreateCustomIcon(void) const {return ((BitOptions & (1<<12))?false:true); };
		inline void CreateCustomIcon(bool newVal) {BitOptions = (BitOptions & 0xFFFFEFFF) + (newVal?0:(1<<12));};
		//Set to overlay a 2D pattern on the atoms in ball and stick mode
		inline bool Show2DPattern(void) const {return ((BitOptions & (1<<13))?true:false); };
		inline void Show2DPattern(bool newVal) {BitOptions = (BitOptions & 0xFFFFDFFF) + (newVal?(1<<13):0);};
		/**
		 Display EFP atoms in wireframe mode
		 */
		inline bool ShowEFPWireFrame(void) const {return ((BitOptions & (1<<14))?true:false); };
		/**
		 Set the EFP atoms display mode.
		 @param newVal If true this will overide the general wireframe mode setting for EFP atoms.
		 */
		inline void ShowEFPWireFrame(bool newVal) {BitOptions = (BitOptions & 0xFFFFBFFF) + (newVal?(1<<14):0);};
		inline Boolean ChangeFileType(void) const {return (SetCreator); };
		inline void ChangeFileType(Boolean newVal) {SetCreator = (newVal != 0);};
		inline short GetStereoOffset(void) const {return StereoOffset;};
		inline void SetStereoOffset(short newval) {StereoOffset = newval;};
		/// Obtain the hi-res model space line width
		inline float GetQD3DLineWidth(void) const {return (CylindersForLines() ? QD3DLineWidth : 0.0F);};
		/// Set the hi-red model space line width (0 to 0.2)
		inline void SetQD3DLineWidth(float newVal) {if ((newVal >= 0)&&(newVal<=0.2)) QD3DLineWidth = newVal;};
		inline EnergyOptions * GetEnergyOptions(void) {return &EnergyPlotOptions;};
		inline GraphOptions * GetGraphOptions(void) {return &PlotOptions;};
		inline SurfaceOptions * GetSurfaceOptions(void) {return &SurfaceOpts;};
		/// Set the maximum inter atomic distance to be considered a bond (>0)
		inline float SetMaxBondLength(const float & NewLength) {
			if (NewLength >= 0.0) MaxBondLength = NewLength;
			return MaxBondLength;};
		/// the maximum inter atomic distance to be deemed a bond
		inline float GetMaxBondLength(void) const {return MaxBondLength;}
		/// Obtain the model space bond width
		inline float GetQD3DBondWidth(void) const {return BondWidth;};
		/// Set the model space bond width (values 0.001 to 0.2)
		inline float SetQD3DBondWidth(float NewValue) {
			if ((NewValue>=0.001)&&(NewValue<=0.2)) BondWidth=NewValue;
			return BondWidth;};
		///Obtain the atom (ie sphere) quality for the 3D display
		inline long GetQD3DAtomQuality(void) const {return QD3DAtomQuality;};
		///Set the # of divisions for 3D spheres (ie the quality). values should be between 2 and 40
		inline long SetQD3DAtomQuality(long NewValue) {
			if ((NewValue >= 2)&&(NewValue <= 40)) QD3DAtomQuality = NewValue;
			return QD3DAtomQuality;};
		/// Obtain the fill light brightness
		inline float GetQD3DFillBrightness(void) const {return QD3DFillBrightness;};
		/// Set the fill light brightness (0 to 1)
		inline float SetQD3DFillBrightness(float NewValue) {
			if ((NewValue >= 0.0)&&(NewValue<=1.0)) QD3DFillBrightness = NewValue;
			return QD3DFillBrightness;};
		/// Obtain the point light brightness
		inline float GetQD3DPointBrightness(void) const {return QD3DPointBrightness;};
		/// Set the point light brightness (0 to 1)
		inline float SetQD3DPointBrightness(float NewValue) {
			if ((NewValue >= 0.0)&&(NewValue <= 1.0)) QD3DPointBrightness = NewValue;
			return QD3DPointBrightness;};
		/** Obtain the master atom size scale factor in model space.
		 * This includes the conversion from picometers to angstroms for the individual atom sizes.
		 */
		inline float GetAtomScale(void) const {return AtomScale;};
		/// Set the model space global atom (sphere) scale factor (0 - 0.025)
		inline float SetAtomScale(float NewValue) {
			if ((NewValue >= 0)&&(NewValue <= 0.025)) AtomScale=NewValue;
			return AtomScale;};
		//Note the GL FOV is really stored as FOV/2
		inline float GetGLFOV(void) const {return GLFOV;};
		inline float SetGLFOV(float NewValue) {return GLFOV=((NewValue>0)&&(NewValue<90)?NewValue:30);};
		/// Obtain the inter frame time delay for frame animations
		inline long GetAnimateTime(void) const {return AnimateTime;};
		/// Set the interframe delay for frame animations (values should be between 0 and 120).
		inline long SetAnimateTime(long NewValue) {
			if ((NewValue>=0)&&(NewValue<120)) AnimateTime=NewValue;
			return AnimateTime;};
		/// Obtain the normal mode animation quality value
		inline short GetAnimationSpeed(void) const {return AnimationSpeed;};
		/// set the normal mode animation quality setting ( 1 to 16 )
		inline short SetAnimationSpeed(short NewValue) {
			if ((NewValue>=1)&&(NewValue<=16)) AnimationSpeed=NewValue;
			return AnimationSpeed;};
		inline Boolean GetAutoBond(void) const {return AutoBondFlag;};
		inline Boolean SetAutoBond(Boolean state) {return AutoBondFlag = state;};
		inline Boolean GetHHBondFlag(void) const {return HHBondFlag;};
		inline Boolean SetHHBondFlag(Boolean state) {return HHBondFlag = state;};
		/// obtain the tolerance factor (like a percentage) for determining bonding
		inline float GetAutoBondScale(void) const {return AutoBondScale;};
		/// set the bonding tolerance factor (0.005 to 0.015)
		inline float SetAutoBondScale(const float & NewValue) {
			if ((NewValue>= 0.005)&&(NewValue <= 0.015)) AutoBondScale = NewValue;
			return AutoBondScale;};
		/// Obtain the model space size for atom labels
		inline float GetAtomLabelSize(void) const {return AtomLabelSize;};
		/// Set the model space size for atom labels ( 0.25 to 4.0)
		inline void SetAtomLabelSize(const float & s) {if ((s>0.0)&&(s<= 4.0)) AtomLabelSize = s;};
		/// Obtain the model space size for atom labels
		inline float GetAnnotationLabelSize(void) const {return AnnotationLabelSize;};
		/// Set the model space size for annotation labels ( 0.25 to 4.0)
		inline void SetAnnotationLabelSize(const float & s) {if ((s>0.0)&&(s<= 4.0)) AnnotationLabelSize = s;};
		/// Indicates how many geometries to skip between ones kept (normally 0)
		inline long GetDRCSkip(void) const {return DRCnFileSkip;};
		/// Set the # of geometries to skip between ones kept (normally 0)
		inline long SetDRCSkip(long NewValue) {if (NewValue >=0) DRCnFileSkip=NewValue; return DRCnFileSkip;};
		inline short GetLabelFontID(void) const {return LabelFontID;};
		inline short SetLabelFontID(short NewID) {return LabelFontID=NewID;};
		inline short GetLabelSize(void) const {return LabelSize;};
		inline short SetLabelSize(short NewValue) {return LabelSize=NewValue;};
		inline void SetAtomColor(long AtomNum, const RGBColor & color) {AtomColors[AtomNum] = color; };
		inline bool ToolbarShown() const { return show_toolbar; }
		/* inline bool ToggleToolbar() { show_toolbar = !show_toolbar; } */
		inline void SetToolbarShown(bool enable) { show_toolbar = enable; }
		inline int GetShaderMode() const { return shader_mode; }
		inline void SetShaderMode(int mode) { shader_mode = mode; }

		//void SetAtomPattern(long AtomNum);
		void SetAtomPattern(long AtomType, short pat) {
		  AtomPatterns[AtomType] = pat;
		}

		short GetAtomPattern(long AtomType) const {
			if ((AtomType>=0)&&(AtomType<kMaxAtomTypes))
				return AtomPatterns[AtomType];
			return 0;
		};
		inline RGBColor * GetAtomColorLoc(long AtomNum) {return &(AtomColors[AtomNum]);};
		inline short * GetAtomPatternLoc(long AtomNum) {return &(AtomPatterns[AtomNum]);};
		inline RGBColor * GetBondColorLoc(long BondOrder) {return &(BondColors[BondOrder]);};
		inline short * GetBondPatternLoc(long BondOrder) {return &(BondPatterns[BondOrder]);};
		inline RGBColor * GetVectorColorLoc(void) {return &VectorColor;};
		inline RGBColor * GetBackgroundColorLoc(void) {return &BackColor;};
		inline void SetBackgroundColorLoc(const RGBColor & color) {BackColor = color;};
		inline short * GetVectorPatternLoc(void) {return &VectorPattern;};
		/// Changes the GL color to be the color for the specified bond order.
		void ChangeColorBondColor(long BondOrder) const;
		/// Changes the GL color to be the color for vectors
		void ChangeColorVectorColor(void) const;
		/// Changes the GL color to be the color for the specified element with the optional alpha.
		void ChangeColorAtomColor(long atomtype, float alpha = 1.0) const;
		/// Compute the color space inverse for the specified element.
		/**
		 * @param[in] atomtype The atomic number for the desired element
		 * @param[out] rgb A vector of three floats (red, green, blue) that will contain the result upon exit.
		 */
		void GetAtomColorInverse(long atomtype, float rgb[3]) const;
		inline void SetBondColor(long BondOrder, const RGBColor & color) {BondColors[BondOrder] = color;};
		inline void SetVectorColor(const RGBColor & color) {VectorColor = color; };
		void SetBondPattern(long BondOrder);
		void SetVectorPattern(void);
		/// Obtain the normal mode length scale factor
		inline float GetVectorScale(void) const {return VectorScale;};
		/// Set the normal mode length scale factor (0.1 to 2.5)
		inline float SetVectorScale(float NewValue) {
			if ((NewValue >= 0.1)&&(NewValue<= 2.5)) VectorScale = NewValue;
			return VectorScale;};
		inline unsigned char * GetAtomLabelLoc(void) {return &(AtomLabels[0][0]);};
		void GetAtomLabel(long AtomNum, Str255 text);
#ifdef __wxBuild__
		void GetAtomLabel(long AtomNum, wxString & text) const;
		void SetAtomLabel(long AtomNum, const wxString& text);
#endif
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
		inline long GetBitOpt() { return  BitOptions; } //for debugging
		unsigned char GetOxidationNumber(int atomic_number);
		void SetOxidationNumber(int atomic_number, int ox_num);
		int GetLineWidth(void) const {return lineWidth;};
		void SetLineWidth(int newVal) {lineWidth = newVal;};
};

//Simple class to save window visibility and positioning data
class WindowData {
public:
	WindowData();
	static void Validate(wxRect & t);
	
	void WriteXML(XMLElement * parent) const;
	void ReadXML(XMLElement * parent);
	
	bool BondsWindowVisible() const {return BondsVis;}
	void BondsWindowVisible(bool v) {BondsVis = v;}
	bool CoordsWindowVisible() const {return CoordsVis;}
	void CoordsWindowVisible(bool v) {CoordsVis = v;}
	bool EnergyWindowVisible() const {return EnergyVis;}
	void EnergyWindowVisible(bool v) {EnergyVis = v;}
	bool FreqWindowVisible() const {return FreqVis;}
	void FreqWindowVisible(bool v) {FreqVis = v;}
	bool SurfacesWindowVisible() const {return SurfacesVis;}
	void SurfacesWindowVisible(bool v) {SurfacesVis = v;}
	bool InputBWindowVisible() const {return InputBVis;}
	void InputBWindowVisible(bool v) {InputBVis = v;}
	bool PrefsWindowVisible() const {return PrefVis;}
	void PrefsWindowVisible(bool v) {PrefVis = v;}
	bool ZMatWindowVisible() const {return ZMatVis;}
	void ZMatWindowVisible(bool v) {ZMatVis = v;}
	
	const wxRect & GetMolWinRect() const {return MolWinRect;}
	void SetMolWinRect(const wxRect & v) {MolWinRect = v;}
	const wxRect & GetBondsWinRect() const {return BondsWinRect;}
	void SetBondsWinRect(const wxRect & v) {BondsWinRect = v;}
	const wxRect & GetCoordsWinRect() const {return CoordsWinRect;}
	void SetCoordsWinRect(const wxRect & v) {CoordsWinRect = v;}
	const wxRect & GetEnergyWinRect() const {return EnergyWinRect;}
	void SetEnergyWinRect(const wxRect & v) {EnergyWinRect = v;}
	const wxRect & GetFrequencyWinRect() const {return FreqWinRect;}
	void SetFrequencyWinRect(const wxRect & v) {FreqWinRect = v;}
	const wxRect & GetSurfacesWinRect() const {return SurfacesWinRect;}
	void SetSurfacesWinRect(const wxRect & v) {SurfacesWinRect = v;}
	const wxRect & GetInputBWinRect() const {return InputBuilderRect;}
	void SetInputBWinRect(const wxRect & v) {InputBuilderRect = v;}
	const wxRect & GetPrefsWinRect() const {return PreferenceWinRect;}
	void SetPrefsWinRect(const wxRect & v) {PreferenceWinRect = v;}
	const wxRect & GetZMatWinRect() const {return ZMatRect;}
	void SetZMatWinRect(const wxRect & v) {ZMatRect = v;}
private:
	wxRect	MolWinRect;
	wxRect	BondsWinRect;
	wxRect	CoordsWinRect;
	wxRect	EnergyWinRect;
	wxRect	FreqWinRect;
	wxRect	SurfacesWinRect;
	wxRect	InputBuilderRect;
	wxRect	PreferenceWinRect;
	wxRect	ZMatRect;
	bool	BondsVis, CoordsVis, EnergyVis, FreqVis, SurfacesVis, InputBVis, PrefVis, ZMatVis;
};

// New preferences classes below here..
/*
class FileOptions {
private:
    long    DRCnFileSkip;
    bool    SetCreator;
    bool    CreateCustomFileIcon;
    bool    Prompt4Save;
    bool    EOLCharMode;
    bool    SetBondOrder;
    bool    CreateHBonds;
public:
//  FileOptions(void);
    FileOptions(const FileOptions * source);
//  ~FileOptions(void);
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
    RGBColor    AtomColor;  //Ok should this stay the Carbon specific RGBColor or move to floats?
    long        AtomicNumber;
    long        AtomSize;
    float       AtomMass;
    string      AtomLabel;
    short       AtomPattern;
public:
};

class CoordWindowOptions {
private:
    //Only thing I can think of is store the size and position for the window
    Rect    CoordWindowRect;
public:
};

class BondsWindowOptions {
private:
    //Only thing I can think of is store the size and position for the window
    Rect    BondsWindowRect;
public:
};

class SurfaceWindowOptions {
private:
    Rect            SurfaceWindowRect;
    long            NumContours;        // Max. # of contours (+ or -) in 2D map
    long            NumGridPoints;      // Default # of grid points
    float           GridSize;           // size of grid box
    float           MaxContourValue;    // Maximum 2D contour value;
    RGBColor        PosColor;           // color for + part of contour
    RGBColor        NegColor;           // color for - part of contour
    char            Options;            // bit 1 - show zero value contour
                                        // bit 2 - rotate 2D map
                                        // bit 3 - show 2D hash marks
public:
};
*/

#endif
