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
#include <vector>
#include <string>
#include "Math3D.h"
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
typedef class MolDisplayWin;

/* class Frame; */

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

enum TypeofEnergy {
	PlainEnergy=0,	//general untyped energy
	HFEnergy,		//RHF, ROHF, UHF
	DFTEnergy,
	PT2Energy,		//2nd order perturbation theory energy
	PT4Energy,		//4th order perturbation theory energy
	MCSCFEnergy,
	MRMP2Energy,
	CCSDEnergy,
	CCSDPTPEnergy,	//CCSD(T)
	CIEnergy,
	
	NumberEnergyTypes
};

enum AnnotationType {
	MP_ANNOTATION,
	MP_ANNOTATION_MARKER,
	MP_ANNOTATION_LENGTH,
	MP_ANNOTATION_ANGLE,
	MP_ANNOTATION_DIHEDRAL
};

class Annotation {
public:
	Annotation(void) {};
	virtual ~Annotation(void) {};
	
	virtual void draw(const MolDisplayWin * win) const = 0;
	virtual bool containsAtom(const int atom_id) const {return false;};
	virtual void adjustIds(const int atom_id) {};
	virtual void WriteXML(XMLElement *parent) const = 0;
	virtual bool ReadXML(XMLElement *p) = 0;
	virtual bool isEquivalent(const int natoms, const int *new_list) const {
		return false;
	}
	virtual int getAtom(const int index) const = 0;
	virtual int getType(void) const {return MP_ANNOTATION;}
	virtual float getParam(const Frame& frame) const = 0;
	virtual void setParam(Frame& frame, float value) = 0;
private:
};

class AnnotationLength : public Annotation {
public:
	AnnotationLength(void);
	AnnotationLength(const long atom1_id, const long atom2_id);
	~AnnotationLength(void) {};
	
	void draw(const MolDisplayWin * win) const;
	bool containsAtom(const int atom_id) const;
	void adjustIds(const int atom_id);
	bool isEquivalent(const int natoms, const int *new_list) const;
	void WriteXML(XMLElement * parent) const;
	bool ReadXML(XMLElement * p);
	int getAtom(const int index) const;
	int getType(void) const;
	float getParam(const Frame& frame) const;
	void setParam(Frame& frame, float value);
private:
	long atoms[2];
};

class AnnotationMarker : public Annotation {
public:
	AnnotationMarker(void);
	AnnotationMarker(const long atom_id);
	~AnnotationMarker(void) {};
	void draw(const MolDisplayWin * win) const;
	bool containsAtom(const int atom_id) const;
	void adjustIds(const int atom_id);
	bool isEquivalent(const int natoms, const int *new_list) const;
	void WriteXML(XMLElement * parent) const;
	bool ReadXML(XMLElement * p);
	int getAtom(const int index) const;
	int getType(void) const;
	float getParam(const Frame& frame) const;
	void setParam(Frame& frame, float value);
private:
	long atom;
};

class AnnotationAngle : public Annotation {
public:
	AnnotationAngle(void);
	AnnotationAngle(const long atom1_id, const long atom2_id,
		const long atom3_id);
	~AnnotationAngle(void) {};
	void draw(const MolDisplayWin * win) const;
	bool containsAtom(const int atom_id) const;
	void adjustIds(const int atom_id);
	bool isEquivalent(const int natoms, const int *new_list) const;
	void WriteXML(XMLElement * parent) const;
	bool ReadXML(XMLElement * p);
	int getAtom(const int index) const;
	int getType(void) const;
	float getParam(const Frame& frame) const;
	void setParam(Frame& frame, float value);
private:
	long atoms[3];
};

class AnnotationDihedral : public Annotation {
public:
	AnnotationDihedral(void);
	AnnotationDihedral(const long atom1_id, const long atom2_id, 
		const long atom3_id, const long atom4_id);
	~AnnotationDihedral(void) {};
	void draw(const MolDisplayWin * win) const;
	bool containsAtom(const int atom_id) const;
	void adjustIds(const int atom_id);
	bool isEquivalent(const int natoms, const int *new_list) const;
	void WriteXML(XMLElement * parent) const;
	bool ReadXML(XMLElement * p);
	int getAtom(const int index) const;
	int getType(void) const;
	float getParam(const Frame& frame) const;
	void setParam(Frame& frame, float value);
private:
	long atoms[4];
};

enum OrbitalGeometry {
	OG_SINGLE,
	OG_LINEAR,
	OG_TRIGONALPLANAR,
	OG_BENT,
	OG_TETRAHEDRAL
};
enum OrbitalHybridization {
	OH_NONE=0,
	OH_S=1,
	OH_SP,
	OH_SP2,
	OH_SP3,
	OH_SP3D,
	OH_SP3D2
};

class mpAtom {
	friend std::ostream& operator<<(std::ostream& stream, const mpAtom& m);

	public:
		CPoint3D	Position;
		long		fragmentId;
		short		Type;	//Low byte stores normal atom type, high byte store special bits
		char		flags;	//bit 0: invisibility, bit 1: select state, bit 2: Is effective fragment
							//bit 3: Is SIMOMM atom, bit 4: symmetry unique
		short	coordinationNumber;
		short	LPCount;

		mpAtom(void) : Type(0), flags(0) {
			coordinationNumber = 0;
			LPCount = 0;
		};

		mpAtom(const mpAtom& src) {
			Position = src.Position;
			coordinationNumber = src.coordinationNumber;
			LPCount = src.LPCount;
			fragmentId = src.fragmentId;
			Type = src.Type;
			flags = src.flags;
		}

		const mpAtom& operator=(const mpAtom& src) {
			if (this != &src) {
				Position = src.Position;
				coordinationNumber = src.coordinationNumber;
				LPCount = src.LPCount;
				fragmentId = src.fragmentId;
				Type = src.Type;
				flags = src.flags;
			}

			return *this;
		}

		inline short GetCoordinationNumber(void) const {return coordinationNumber;};
		inline short SetCoordinationNumber(short new_num) {return (coordinationNumber = new_num);};
		inline short SetLonePairCount(short new_num) {return (LPCount = new_num);};
		inline short GetLonePairCount(void) const {return LPCount;};
		inline bool GetInvisibility(void) const {return ((flags & 1)? true: false);};	//Bit 1 sets invisibility
		bool SetInvisibility(bool State) {flags = (flags & 0xFE) + (State? 1:0);return GetInvisibility();};
		inline bool GetSelectState(void) const {return ((flags&2)?true:false);};	//Bit 2 sets selected
		bool SetSelectState(bool State) {flags = (flags & 0xFD) + (State?2:0); return GetSelectState();};
		inline long GetType(void) const {return Type;};
		inline bool SetType(short atmType) {if ((atmType>0)&&(atmType<107)) {Type = atmType; return true;} return false;};
		inline bool IsEffectiveFragment(void) const {return ((flags & (1<<2))?true:false);};
		inline void IsEffectiveFragment(bool state) {flags = (Type & 0xFB) + (flags ? (1<<2) : 0);};
		inline void IsSIMOMMAtom(bool state) {flags = (flags & 0xF7) + (state ? (1<<3) : 0);};
		inline bool IsSIMOMMAtom(void) const {return ((flags & (1<<3))?true:false);};
		inline void IsSymmetryUnique(bool state) {flags = (flags & 0xEF) + (state ? (1<<4) : 0);};
		inline bool IsSymmetryUnique(void) const {return ((flags & (1<<4))?true:false);};
		// the idea for this one was to have a text label to store pdb style biomolecule, label gives res, res #,
		// atom type (alpha, beta...), but its not currently implemented
		//	inline bool HasBiolabel(void) const {return ((Type & (1<<9)) != 0);};
		inline void SetFragmentNumber(long fragNum) {if (fragNum>=0) fragmentId = fragNum;};
		inline long GetFragmentNumber(void) const {return fragmentId;};
		inline long GetNuclearCharge(void) const {return (GetType());};	//NOT correct for ECP's!!!!!
		void SetDefaultCoordinationNumber(void);
		void SetDefaultLonePairCount(void);
};

enum BondOrder {
	kHydrogenBond,
	kSingleBond,
	kDoubleBond,
	kTripleBond,
	kAromaticBond,
	kMixedBonds,
	kUnknownBond
};

class Bond {
	friend std::ostream& operator<<(std::ostream& stream, const Bond& b);

	public:
		long		Atom1;
		long		Atom2;
		BondOrder	Order;
		char		Highlite;

		Bond(void) {
			Atom1 = -1;
			Atom2 = -1;
			Order = kSingleBond;
			Highlite = 0;
		}

		Bond(const Bond& src) {
			Atom1 = src.Atom1;
			Atom2 = src.Atom2;
			Order = src.Order;
			Highlite = src.Highlite;
		}

		const Bond& operator=(const Bond& src) {
			if (this != &src) {
				Atom1 = src.Atom1;
				Atom2 = src.Atom2;
				Order = src.Order;
				Highlite = src.Highlite;
			}

			return *this;
		}

		static const bool RefLessThan(const Bond *a, const Bond *b) {
			if (a == NULL) {
				return b != NULL;
			} else if (b == NULL) {
				return false;
			} else {
				return a->Order < b->Order;
			}
		}

		bool GetSelectState(void) const {return (Highlite & 1);};
		void SetSelectState(bool state) {if (Highlite & 1) Highlite--; if (state) Highlite++;}
};
enum EnergyUnit {
	kDefault,
	kKCalPerMole
};

#define kNumSymmetryPointGroups 53
enum GAMESSPointGroup {
	invalidPGroup=0,
	GAMESS_C1,
	GAMESS_CS,
	GAMESS_CI,
	GAMESS_CNH,
	GAMESS_CNV,
	GAMESS_CN,
	GAMESS_S2N,
	GAMESS_DND,
	GAMESS_DNH,
	GAMESS_DN,
	GAMESS_TD,
	GAMESS_TH,
	GAMESS_T,
	GAMESS_OH,
	GAMESS_O,
	
	NumberGAMESSPointGroups
};
// Class to abstract out symmetry operations
class SymmetryOps {
public:
	SymmetryOps(GAMESSPointGroup pg, short pgOrder);
	long getOperationCount(void) const {return (operations.size()/9);};
	void ApplyOperator(const CPoint3D & source, CPoint3D & dest, long theOp) const;
private:
		std::vector<float> operations;
	
	void AddMatrix(const Matrix4D source);
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
		std::vector<std::string>	Frequencies;	// array of strings
		std::vector<CPoint3D>		NormMode;		// Array of normal modes
		std::vector<float>			Intensities;	// Infrared intensity of each mode
		std::vector<float>			ReducedMass;	// Reduced Mass for each mode
		std::vector<float>			RamanIntensity;	// Raman spectrum
		std::vector<float>			Depolarization;	// depolarization
		long						NumModes;		// number of modes here
		long						CurrentMode;	// Currently Drawn mode

		VibRec(const long & NumVibs, const long & NumAtoms);
		VibRec(void);
		~VibRec(void);
		void Setup(const long & NumVibs, const long & NumAtoms);
		bool Resize(long NumAtoms);
#ifndef __wxBuild__
		static VibRec * ReadOldVibRec45(BufferFile * Buffer);
		void ReadCode46(BufferFile * Buffer, long length);
		void ReadCode47(BufferFile * Buffer, long NumAtoms, long length);
		static VibRec * Read(BufferFile * Buffer, long NumAtoms);
		long GetSize(BufferFile *Buffer, long NumAtoms);
		long Write(BufferFile *Buffer, long NumAtoms);
#endif
		void WriteXML(XMLElement * parent, long NumAtoms) const;
		void ReadXML(XMLElement * parent, const long & NumAtoms);
		inline long GetNumModes(void) const {return NumModes;};
		inline long GetCurrentMode(void) const {return CurrentMode;};
		void GetModeOffset(const long & loc, CPoint3D & offset) const {offset = NormMode[loc];};
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
		virtual long ExportPOV(MoleculeData *lData, WinPrefs *Prefs, BufferFile *Buffer) {
		}
	//	void ExportToFile(void);
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
