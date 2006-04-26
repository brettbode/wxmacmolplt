/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	MyTypes.h	****	A place to put General use Types information.

Created for other header files 2-4-95	BMB
Added OpenGL version of Draw3D to Surface 6/2001 BMB
*/
#ifndef __MyTypes__
#define __MyTypes__
#include "Geometries.h"
#ifndef __BFile__
#include "BFiles.h"
#endif
#ifdef QuickDraw3DHeaders
#ifndef QD3D_h
#include <QD3D.h>
#endif
#endif
#ifdef QuesaHeaders
#ifndef QUESA_HDR
#include <Quesa.h>
#endif
#endif
/* General Use Types independant of my data structures */
#define kMaxAtomTypes		130
#define kMaxBondTypes		4

typedef class MoleculeData MoleculeData;
typedef class WinPrefs WinPrefs;
typedef class Progress Progress;
typedef class GaussHermiteData GaussHermiteData;
typedef class BasisSet BasisSet;
typedef class Frame Frame;
typedef class InputData InputData;
typedef class Internals Internals;
typedef class XMLElement XMLElement;


enum TypeOfPointGroup {
	C1=1,
	Cs,
	Ci,
	S4, S6, S8,
	C2, C3, C4, C5, C6, C7, C8,
	C2v, C3v, C4v, C5v, C6v,
	C2h, C3h, C4h, C5h, C6h,
	D2, D3, D4, D5, D6,
	D2d, D3d, D4d, D5d, D6d,
	D2h, D3h, D4h, D5h, D6h, D8h,
	CInfv, DInfh,
	I, Ih,
	T, Th, Td,
	O, Oh,
	
	NumberPointGroups
};

class mpAtom {
	public:
		CPoint3D	Position;
		short		Type;	//Low byte stores normal atom type, high byte store special bits
		char		Highlite;
		char		Label[16];
			//Type bit 9 - indicates EFP, first 4 bytes of label give fragment number
			//Type bit 10 - pdb style biomolecule, label gives res, res #, atom type (alpha, beta...)
			//Type bit 11 - SIMOMM MM atom
		inline bool GetInvisibility(void) const {return (Highlite & 1);};	//Bit 1 sets invisibility
		bool SetInvisibility(bool State) {if (Highlite&1) Highlite-=1; if (State) Highlite+=1;return GetInvisibility();};
		inline bool GetSelectState(void) const {return ((Highlite&2)!=0);};	//Bit 2 sets selected
		bool SetSelectState(bool State) {if (Highlite&2) Highlite-=2; if (State) Highlite+=2; return GetSelectState();};
		inline long GetType(void) const {return (Type & 0xFF);};
		inline bool SetType(short atmType) {if ((atmType>0)&&(atmType<107)) {Type = atmType; return true;} return false;};
		inline bool IsEffectiveFragment(void) const {return ((Type & (1<<8)) != 0);};
		inline void IsEffectiveFragment(bool state) {Type = (Type & 0xFEFF) + (state ? (1<<8) : 0);};
		inline bool HasBiolabel(void) const {return ((Type & (1<<9)) != 0);};
		inline void IsSIMOMMAtom(bool state) {Type = (Type & 0xFBFF) + (state ? (1<<10) : 0);};
		inline bool IsSIMOMMAtom(void) {return ((Type & (1<<10)) != 0);};
		inline void SetFragmentNumber(long fragNum) {IsEffectiveFragment(true); *((long *) &Label[12]) = fragNum;};
		inline long GetFragmentNumber(void) const {return *((long *) &Label[12]);};
		inline long GetNuclearCharge(void) const {return (GetType());};	//NOT correct for ECP's!!!!!
};

enum BondOrder {
	kHydrogenBond,
	kSingleBond,
	kDoubleBond,
	kTripleBond,
	kMixedBonds,
	kUnknownBond
};
class Bond {
	public:
		long		Atom1;
		long		Atom2;
		BondOrder	Order;
		char		Highlite;

		bool GetSelectState(void) const {return (Highlite & 1);};
		void SetSelectState(bool state) {if (Highlite & 1) Highlite--; if (state) Highlite++;}
};
enum EnergyUnit {
	kDefault,
	kKCalPerMole
};
// Surface Types - Add new items to the end only since this constant is saved in binary files
// to correctly type the surface upon loading from file.
typedef enum SurfaceType {
	kNone=0,
	kOrb3DType,
	kOrb2DType,
	kTotalDensity3D,
	kTotalDensity2D,
	kMEP2D,
	kMEP3D,
	kSolventType3D,
	kGeneral2DSurface,
	kGeneral3DSurface,
	kOrb1DType,
	kTotalDensity1D,
	kMEP1D,
	kGeneral1D,
	
	NumSurfaceTypes
} SurfaceType;
const char * SurfaceTypeToText(const SurfaceType & s);
bool TextToSurfaceType(const char * t, SurfaceType & s);
class VibRec {
	private:
		void init(void);
	public:
		char *			Frequencies;		// packed array of Pascal strings
		CPoint3D *		NormMode;			// Array of normal modes
		float *			Intensities;		// Infrared intensity of each mode
		float *			ReducedMass;		// Reduced Mass for each mode
		float *			RamanIntensity;		// Raman spectrum
		float *			Depolarization;		// depolarization
		long			NumModes;			// number of modes here
		long			CurrentMode;		// Currently Drawn mode
		long			FreqLength;			// size of the Frequency array

		VibRec(const long & NumVibs, const long & NumAtoms);
		VibRec(void);
		~VibRec(void);
		void Setup(const long & NumVibs, const long & NumAtoms);
		bool Resize(long NumAtoms, long FreqLength);
		static VibRec * ReadOldVibRec45(BufferFile * Buffer);
		void ReadCode46(BufferFile * Buffer, long length);
		void ReadCode47(BufferFile * Buffer, long NumAtoms, long length);
		static VibRec * Read(BufferFile * Buffer, long NumAtoms);
		long GetSize(BufferFile *Buffer, long NumAtoms);
		long Write(BufferFile *Buffer, long NumAtoms);
		void WriteXML(XMLElement * parent, long NumAtoms) const;
		void ReadXML(XMLElement * parent, const long & NumAtoms);
		inline long GetNumModes(void) const {return NumModes;};
		float GetIntensity(long Mode) const;
		float GetFrequency(long Mode) const;
		float GetRamanIntensity(long Mode) const;
};

class AODensity {
	public:
		AODensity(void);
		~AODensity(void);
		void SetupMemory(long NumBasisFunctions);
		inline bool CheckDimension(long NumBasisFunctions) {return (NumBasisFunctions == Dimension);};
		inline float * GetDensityArray(void) {return DensityArray;};
		inline long * GetDensityIndex(void) {return IndexArray;};
		inline short * GetDensityCheck(void) {return DensityCheckArray;};
	private:
		float *		DensityArray;
		long *		IndexArray;
		short *		DensityCheckArray;
		long		Dimension;		//Dimension of the matrix (should always be = to # of basis functions)
};

typedef enum TypeOfWavefunction {
	unknownWavefunction=0,
	RHF=1,
	UHF,
	ROHF,
	GVB,
	MCSCF,
	CI,
	RHFMP2,
	
	NumberWavefunctionTypes
} TypeOfWavefunction;
const char * ConvertTypeOfWavefunction(const TypeOfWavefunction & t);
bool ConvertTypeOfWavefunction(const char * s, TypeOfWavefunction & t);
typedef enum TypeOfOrbital {
	unknownOrbitalType=0,
	OptimizedOrbital,
	NaturalOrbital,
	LocalizedOrbital,	//Hmmm do we need the type of localization?
	RuedenbergLocalizedOrbital,
	BoysLocalizedOrbital,
	OrientedLocalizedOrbital,
	GuessOrbital,
	
	NumOrbitalTypes
//	RHFOptimized=1,
//	UHFOptimized,
//	ROHFOptimized,
//	GVBOptimized,
//	MCSCFOptimized,
//	CIOrbitals,
//	RHFMP2Orbitals,
//	UHFNatural,
//	MCSCFNatural
} TypeOfOrbital;
const char * ConvertTypeOfOrbital(const TypeOfOrbital & t);
bool ConvertTypeOfOrbital(const char * s, TypeOfOrbital & t);

class OrbitalRec {
	private:
		void ReadCodes(BufferFile * Buffer, long NumBasisFunctions, long ByteCount);
	public:
		float *			Vectors;			//Eigenvectors (Alpha set in UHF, Natural Orbs in MCSCF)
		float *			VectorsB;			//Eigenvectors (Beta set in UHF, Optimized MOs in MCSCF)
		float *			Energy;				//Orbital Energies (Alpha set in UHF, Natural Orbs in MCSCF)
		float *			EnergyB;			//Orbital Energies (Beta set in UHF, Optimized MOs in MCSCF)
		float *			OrbOccupation;		//Orbital occupation numbers
		float *			OrbOccupationB;		//Orbital occupation numbers for Beta set
		char *			SymType;			//Symmetry of each MO (Alpha set in UHF, Natural Orbs in MCSCF)
		char *			SymTypeB;			//Symmetry of each MO (Beta set in UHF, Optimized MOs in MCSCF)
		AODensity *		TotalAODensity;		//AO Density corresponding to these orbitals
		long			NumAlphaOrbs;		//# alpha orbitals (or total # of orbitals)
		long			NumBetaOrbs;		//# beta orbitals or MCSCF optimized orbs, 0 otherwise
		long			NumOccupiedAlphaOrbs;	//# occupied orbs, normally just the
		long			NumOccupiedBetaOrbs;	//# of e's except for ECP runs
		long			NumBasisFunctions;	//This should be constant reflecting one dimension of the vectors
		char *			Label;				//label lines from a $vec group	
		TypeOfWavefunction	BaseWavefunction;	//RHF, UHF, ...
		TypeOfOrbital	OrbitalType;		//Optimized, Natural, localized...
		
		OrbitalRec(long nAlphaOrbs=0, long nBetaOrbs=0, long nBasis=0);
		OrbitalRec(BufferFile *Buffer, long code, long length);
		~OrbitalRec(void);
		void ReSize(long nAlphaOrbs, long nBetaOrbs);
		long GetSize(BufferFile *Buffer);
		long WriteToBuffer(BufferFile *Buffer);
		static OrbitalRec * Read(BufferFile *Buffer, long NumBasisFuncs, long ByteCount);
		void WriteXML(XMLElement * parent) const;
		bool ReadXML(XMLElement * parent);
		const char * getLabel(void) const {return Label;};
		void setLabel(const char * c);
		void ReadVecGroup(BufferFile *Buffer, const long &NumBasisFuncs, const TypeOfWavefunction &Type);
		void ReadUHFVECOccupancies(BufferFile &);
		void WriteVecGroup(BufferFile * File, const long & NumBasisFuncs, const long & NumOrbs) const;
		void RotateVectors(Matrix4D rotationMatrix, BasisSet * basis, long numAtoms);
		void ExchangeVectors(void);
		void GetAODensityMatrix(float * AODensity, long NumOccAlpha, long NumOccBeta,
			long NumBasisFuncs) const;
		AODensity * GetAODensity(BasisSet * lBasis, long NumAtoms);
		inline TypeOfWavefunction getOrbitalWavefunctionType(void) const {return BaseWavefunction;};
		void setOrbitalWavefunctionType(TypeOfWavefunction t) {BaseWavefunction = t;};
		inline TypeOfOrbital getOrbitalType(void) const {return OrbitalType;};
		void setOrbitalType(TypeOfOrbital t) {OrbitalType = t;};
		inline long getNumOccupiedAlphaOrbitals(void) const {return NumOccupiedAlphaOrbs;};
		inline long getNumOccupiedBetaOrbitals(void) const {return NumOccupiedBetaOrbs;};
		inline long getNumAlphaOrbitals(void) const {return NumAlphaOrbs;};
		inline long getNumBetaOrbitals(void) const {return NumBetaOrbs;};
		void SetOccupancy(float * Occ, long nVec);
		void SetOrbitalOccupancy(const long & alpha, const long & beta);
		bool TotalDensityPossible(void) const;
};

/*class MORec {
	public:
		OrbitalRec *	EigenVectors;	//Canonical Orbitals
		OrbitalRec *	LocalOrbitals;	//Localized orbitals
		OrbitalRec *	OrientedLocalOrbitals;	//Oriented Localized Orbitals
//		AODensity *		TotalAODensity;	//AO Density corresponding to these orbitals
//		long			NumOccupiedAlphaOrbs;	//# occupied orbs, normally just the
//		long			NumOccupiedBetaOrbs;	//# of e's except for ECP runs

		MORec(void);
		~MORec(void);
		long GetSize(BufferFile *Buffer, long NumBasisFuncs);
		long WriteToBuffer(BufferFile *Buffer, long NumBasisFuncs);
		long Read(BufferFile *Buffer, long NumBasisFuncs, long ByteCount);
		inline void SetOccupancy(float * Occ, long nVec) {if (EigenVectors) EigenVectors->SetOccupancy(Occ, nVec);};
		inline void RotateOrbitals(Matrix4D rotationMatrix, BasisSet * basis, long NumAtoms) {
				if (TotalAODensity) delete TotalAODensity;	//The AO density matrix must be recalculated
				if (EigenVectors) EigenVectors->RotateVectors(rotationMatrix, basis, NumAtoms);
				if (LocalOrbitals) LocalOrbitals->RotateVectors(rotationMatrix, basis, NumAtoms);
				if (OrientedLocalOrbitals) OrientedLocalOrbitals->RotateVectors(rotationMatrix, basis, NumAtoms);
			};
		AODensity * GetAODensity(BasisSet * lBasis, long NumAtoms);
};*/
typedef class SurfacePane SurfacePane;
typedef class SurfacesWin SurfacesWin;
typedef class Surface Surface;
typedef class myGLTriangle myGLTriangle;
class Surface {
	friend class Frame;
	protected:
		long		ID;
		Surface *	NextSurface;
		char *		Label;
		bool		Visible;

		void Export3D(const float * Grid3D, long nx, long ny, long nz, const CPoint3D * Origin,
			float XInc, float YInc, float ZInc, const char * Label, BufferFile * Buffer) const;
		void Export2D(const float * Grid2D, long NumPoints, const CPoint3D * Origin,
			const CPoint3D *XInc, const CPoint3D *YInc, const char * Label, BufferFile * Buffer) const;
	public:
		Surface(void);
		virtual ~Surface(void);
		Surface * GetNextSurface(void) const {return NextSurface;};
		void SetNextSurface(Surface * target) {NextSurface = target;}
		virtual char * GetLabel(void) const;
		void SetLabel(const char * NewLabel);
		inline bool DefaultLabel(void) {return (Label == NULL);};
		virtual bool Needs2DPlane(void) const;
#ifndef __wxBuild__
		virtual SurfacePane * CreateSurfacePane(SurfacesWin * window)=0;
#endif
		virtual void Update(MoleculeData * MainData);
		virtual SurfaceType GetSurfaceType(void) const = 0;
		virtual bool isTransparent(void) const {return false;};
		inline bool GetVisibility(void) const {return Visible;};
		inline void SetVisibility(bool state) {Visible = (state!=0);};
		inline long GetSurfaceID(void) const {return ID;};
		inline long SetSurfaceID(long NewID) {ID = NewID; return ID;};
		inline long SetSurfaceID(void) {/*ID = TickCount(); return ID;*/
                                        static long ID = 0; return ++ID;};
		virtual void RotateEvent(MoleculeData * MainData);
		virtual long GetSize(BufferFile * Buffer);
		virtual long Write(BufferFile * Buffer)=0;
		virtual void WriteXML(XMLElement * parent) const;
		virtual bool ReadXML(XMLElement * parent);
		static Surface * ReadSurface(XMLElement * parent);
		long GetSizeofSurface(BufferFile * Buffer);
		long WriteSurface(BufferFile * Buffer);
		long Read(BufferFile * Buffer, long length);
		virtual void Export(BufferFile * Buffer) const;
		void ExportToFile(void);
		virtual void RotateSurface(Matrix4D RotationMatrix);
		virtual bool ExportPossible(void) const {return false;};
		virtual bool NeedsUpdateDuringRotation(void) const {return false;};
		virtual void Draw2D(MoleculeData * lData, long hoffset, long voffset, float scale);
#ifdef QuickDraw3D
		virtual void Draw3D(MoleculeData * lData, TQ3GroupObject myGroup, WinPrefs * Prefs)=0;
#endif
#ifdef UseOpenGL
		virtual long Draw3DGL(MoleculeData * lData,  WinPrefs * Prefs, myGLTriangle *)=0;
		virtual long getTriangleCount(void) const {return 0;};
#endif
};

void ApplyRotation(Matrix4D RotMat, long Axis, float AngleDegrees);
void CalculateCenterOfMass(mpAtom * AtomList, long NumAtoms, float * AtomMasses, CPoint3D * Center);
void MinimizeDifferences(mpAtom * FixedAtoms, mpAtom * targetAtoms, long NumAtoms,
		WinPrefs * Prefs, long NumOptAtoms);
float CalculateSquaresValue(long NumOptAtoms, mpAtom CoordSetA[], CPoint3D CoordSet[]);

#endif
