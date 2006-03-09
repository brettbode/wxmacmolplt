/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* MoleculeData.h

	class MoleculeData organizes data on the molecule involved in each window/file.

	Created from other files, BMB, 4/1998
*/

#ifndef __MoleculeData__
#define __MoleculeData__
#ifndef __MyTypes__
#include "MyTypes.h"
#endif

typedef class AtomTypeList AtomTypeList;
typedef class WindowData WindowData;

class MoleculeData {
		friend class MolDisplayWin;
		friend class CoordWin;
		friend class EnergyPlotWin;
		friend class Surf2DBase;
		friend class Surf3DBase;
	private:
		CPoint3D	*RotCoords;				// The currently displayed, rotated coordinates
		long		*zBuffer;				// the sorting order for RotCoords
		Frame *		cFrame;					// pointer to the currently drawn frame
		Frame *		Frames;					// pointer to the first frame
		Internals *	IntCoords;
		BasisSet *	Basis;
		char *		Description;			// Simple one line label
		long		CurrentFrame;			// Number of the current frame
		long		NumFrames;				// Current number of Frames
		long		MaxAtoms;				// The maximum number of atoms in any frame
		float		MaxSize;				// The maximum side length of any frame
		float		WindowSize;				// The Window size in the molecule coordinate space (ie  or Bohrs) user adjustable to change the window scaling
		Matrix4D	TotalRotation;			// Rotation matrix for displaying the molecule
		InputData *	InputOptions;			// Run information
		char		DrawMode;				// Flag for normal mode display & MO display
											/*	bit 1 for showing normal modes
												bit 2 for showing special atoms
												4 & 5 are obsolete, 6 indicates 2D plane display
												bit 7 for showing the axis's
												bit 4 for plotting KE rather than Total E
												bit 5 for plotting PE */
		char		DrawLabels;				// Flag for label drawing (both #'s and atomic labels) bit3 deactivates labeling H atoms
	public:
		MoleculeData();
		~MoleculeData(void);
		long ReadInitialFragmentCoords(BufferFile * Buffer);
		void ReadFragmentCoordinates(BufferFile * Buffer, long NumFragmentAtoms);
#ifndef __wxBuild__
		void ConvertMainWinData1(BufferFile *Buffer, long size);
		void ConvertMainWinData14(BufferFile * Buffer, long length);
		long UnPackOldData(BufferFile *Buffer);
		long UnPackData(BufferFile * Buffer);
		void ReadMORec10(BufferFile * Buffer, long length);
		long ReadMORec48(BufferFile *Buffer, const long & NumBasisFuncs, const long & ByteCount);
#endif
		void ReadRunInfoRec(BufferFile *Buffer, long length);
		void ReadRunTitle(BufferFile *Buffer, long length);
		void ReadBasisOptions(BufferFile * Buffer);
		void ReadControlOptions(BufferFile * Buffer);
		long WriteCMLFile(BufferFile * Buffer, WinPrefs * Prefs, WindowData * wData, bool allFrames, bool AllData);
		long OpenCMLFile(BufferFile * Buffer, WinPrefs * Prefs, WindowData * wData, bool readPrefs);
		inline float GetMoleculeSize(void) {return WindowSize;};
		inline void SetMoleculeSize(float newVal) {if (newVal > 0.0) WindowSize = newVal;};
		void GetModelCenter(CPoint3D * center);
		void SetModelCenter(CPoint3D * center);
		void GetModelRotation(float * Psi, float * Phi, float * Theta);
		void SetModelRotation(float Psi, float Phi, float Theta);
		void AdvanceFrame(void);
		void SetCurrentFrame(long FrameNum);
	//	inline Boolean GetFrameMode(void) {return (DrawMode & (1<<3));};
		AtomTypeList * GetAtomTypes(void);
		bool SurfaceExportPossible(void);
		inline long GetCurrentFrame(void) {return CurrentFrame;};
		inline Frame* GetCurrentFramePtr(void) {return cFrame;};
		inline Frame * GetFirstFrame(void) {return Frames;};
		void ParseGAMESSBasisSet(BufferFile * Buffer);
		long ParseECPotentials(BufferFile * Buffer);
		inline BasisSet * GetBasisSet(void) const {return Basis;};
		long GetNumBasisFunctions(void) const;
		inline long GetNumFrames(void) {return NumFrames;};
		bool OrbSurfacePossible(void);
		bool TotalDensityPossible(void);
		void ResetRotation(void);
		void CenterModelWindow(void);
		void InvertMode(void);
		void UnitConversion(bool AngToBohr);
		void FlipRotation(short theItem);
		bool SetScreenPlane(CPoint3D *Points);
		void LinearLeastSquaresFit(Progress * lProgress);
		void CreateLLM(long NumPts, WinPrefs * Prefs);
		void CreateInternalLLM(long NumPts, WinPrefs * Prefs);
		Frame * AddFrame(long NumAtoms, long NumBonds);
		void DeleteFrame(void);
		bool SetupFrameMemory(long NumAtoms, long NumBonds);
		Frame * LocateNewFrame(float XPosition);
		inline bool DrawAtomLabels(void) const {return (DrawLabels & 1);};
		inline bool DrawHLabels(void) const {return ((DrawLabels & 4) == 0);};
		inline void SetHLabelMode(bool State) {DrawLabels = (DrawLabels & 0xFB) + (State ? 0: 4);};
		inline void SetAtomLabelDrawMode(bool State) {if (DrawLabels&1) DrawLabels--; if (State) DrawLabels++;};
		inline bool DrawAtomNumbers(void) const {return ((DrawLabels & 2)!=0);};
		inline void SetAtomNumbersDrawMode(bool State) {if (DrawLabels&2) DrawLabels-=2; if (State) DrawLabels+=2;};
		bool ModeVisible(void) const;
		void SetDescription(char * NewLabel);
		inline bool SetSpecialAtomDrawMode(bool State) {if (DrawMode&2) DrawMode-=2; if (State) DrawMode+=2; return State;};
		inline bool GetSpecialAtomDrawMode(void) const {return ((DrawMode & 2)!=0);};
		inline bool SetDrawMode(bool State) {if (DrawMode&1) DrawMode--; if (State) DrawMode++; return State;};
		inline bool GetDrawMode(void) const {return (DrawMode & 1);};
		inline bool ShowAxis(void) const {return ((DrawMode & 0x40) != 0);};
		inline void SetShowAxis(bool newState) {DrawMode = (DrawMode & 0xBF) + (newState ? 0x40 : 0);};
		long GetNumBonds(void) const;
		long GetNumElectrons(void) const;
		short GetMultiplicity(void) const;
		void WriteInputFile(WinPrefs * Prefs);
		InputData * GetInputData(void);
		InputData * SetInputData(InputData * NewData);
		void NewAtom(void);
		void StickCoordinates(void);
		inline Internals * GetInternalCoordinates(void) {return IntCoords;};
		void DeleteAtom(long AtomNum);
		bool ValidAtom(long AtomNum);
		void GetRotationMatrix(Matrix4D copy);
};

#endif
