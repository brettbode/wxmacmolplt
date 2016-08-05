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
#include <vector>
#include <string>
#include <map>
#include "Math3D.h"
/* General Use Types independant of my data structures */
#define kMaxAtomTypes		130
#define kMaxBondTypes		4

class MoleculeData;
class WinPrefs;
class Progress;
struct GaussHermiteData;
class BasisSet;
class Frame;
class InputData;
class Internals;
class XMLElement;
class MolDisplayWin;
class mpAtom;

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

enum TypeOfEnergy {
	PlainEnergy=0,	///< general untyped energy
	RHFEnergy,
	UHFEnergy,
	ROHFEnergy,
	DFTEnergy,
	PT2Energy,		///< 2nd order perturbation theory energy
	PT4Energy,		///< 4th order perturbation theory energy
	GVBEnergy,
	MCSCFEnergy,
	MRMP2Energy,
	CCSDEnergy,
	CCSDPTPEnergy,	///< CCSD(T)
	CIEnergy,
	G3MP2Energy,
	KineticEnergy,
	TranslationalKE,
	RotationalKE,
	PotentialEnergy,
	
	NumberEnergyTypes
};

enum EnergyUnit {
	kDefault,
	kKCalPerMole
};

class EnergyValue {
public:
	double	value;
	TypeOfEnergy	type;
	
	EnergyValue(void) {value = 0.0, type=PlainEnergy;};
	EnergyValue(double v, TypeOfEnergy t) {value=v; type=t;};
	double GetEnergy(void) const {return value;};
	TypeOfEnergy GetType(void) const {return type;};
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
	Annotation(unsigned int natoms = 0, long atom1_id = -1, long atom2_id = -1,
			   long atom3_id = -1, long atom4_id = -1);
	virtual ~Annotation() {};
	
	virtual bool containsAtom(const int atom_id) const;
	virtual bool containsFragment(const mpAtom *atom_list) const;
	virtual void adjustIds(const int atom_id, int offset = -1);
	virtual bool isEquivalent(const int natoms, const int *new_list) const {
		return false;
	}
	virtual int getAtom(const int index) const;
	virtual int getType() const;

	// Define these in subclasses.
	virtual void WriteXML(XMLElement *parent) const = 0;
	virtual bool ReadXML(XMLElement *p) = 0;
	virtual void draw(const MolDisplayWin * win) const = 0;
	virtual float getParam(const Frame& frame) const = 0;
	virtual void setParam(Frame& frame, float value) = 0;

protected:
	long atoms[4];
	unsigned int natoms;
	int type;
};

class AnnotationLength : public Annotation {
public:
	AnnotationLength(const long atom1_id = -1, const long atom2_id = -1);
	~AnnotationLength() {};
	
	void draw(const MolDisplayWin * win) const;
	bool isEquivalent(const int natoms, const int *new_list) const;
	void WriteXML(XMLElement * parent) const;
	bool ReadXML(XMLElement * p);
	float getParam(const Frame& frame) const;
	void setParam(Frame& frame, float value);
private:
};

class AnnotationMarker : public Annotation {
public:
	AnnotationMarker(const long atom_id = -1);
	~AnnotationMarker() {};
	void draw(const MolDisplayWin * win) const;
	bool isEquivalent(const int natoms, const int *new_list) const;
	void WriteXML(XMLElement * parent) const;
	bool ReadXML(XMLElement * p);
	float getParam(const Frame& frame) const;
	void setParam(Frame& frame, float value);
private:
};

class AnnotationAngle : public Annotation {
public:
	AnnotationAngle(const long atom1_id = -1, const long atom2_id = -1,
					const long atom3_id = -1);
	~AnnotationAngle() {};
	void draw(const MolDisplayWin * win) const;
	bool isEquivalent(const int natoms, const int *new_list) const;
	void WriteXML(XMLElement * parent) const;
	bool ReadXML(XMLElement * p);
	float getParam(const Frame& frame) const;
	void setParam(Frame& frame, float value);
private:
};

class AnnotationDihedral : public Annotation {
public:
	AnnotationDihedral(const long atom1_id = -1, const long atom2_id = -1, 
					   const long atom3_id = -1, const long atom4_id = -1);
	~AnnotationDihedral(void) {};
	void draw(const MolDisplayWin * win) const;
	bool isEquivalent(const int natoms, const int *new_list) const;
	void WriteXML(XMLElement * parent) const;
	bool ReadXML(XMLElement * p);
	float getParam(const Frame& frame) const;
	void setParam(Frame& frame, float value);
private:
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

		inline short GetCoordinationNumber() const {return coordinationNumber;};
		inline short SetCoordinationNumber(short new_num) {return (coordinationNumber = new_num);};
		inline short SetLonePairCount(short new_num) {return (LPCount = new_num);};
		inline short GetLonePairCount(void) const {return LPCount;};
		inline bool GetInvisibility(void) const {return ((flags & 1)? true: false);};	//Bit 1 sets invisibility
		bool SetInvisibility(bool State) {flags = (flags & 0xFE) + (State? 1:0);return GetInvisibility();};
		inline bool GetSelectState(void) const {return ((flags&2)?true:false);};	//Bit 2 sets selected

		// Please don't call SetSelectState directly.  Use the frame method SetAtomSelection, so that the number
		// of selected atoms is properly accounted for.
		bool SetSelectState(bool State) {flags = (flags & 0xFD) + (State?2:0); return GetSelectState();};

		inline long GetType(void) const {return Type;};
		inline bool SetType(short atmType) {if ((atmType>0)&&(atmType<107)) {Type = atmType; return true;} return false;};
		inline bool IsEffectiveFragment(void) const {return ((flags & (1<<2))?true:false);};
		inline void IsEffectiveFragment(bool state) {flags = (flags & 0xFB) + (state ? (1<<2) : 0);};
		inline void IsSIMOMMAtom(bool state) {flags = (flags & 0xF7) + (state ? (1<<3) : 0);};
		inline bool IsSIMOMMAtom(void) const {return ((flags & (1<<3))?true:false);};
		inline void IsSymmetryUnique(bool state) {flags = (flags & 0xEF) + (state ? (1<<4) : 0);};
		inline bool IsSymmetryUnique(void) const {return ((flags & (1<<4))?true:false);};
		// the idea for this one was to have a text label to store pdb style biomolecule, label gives res, res #,
		// atom type (alpha, beta...), but its not currently implemented
		//	inline bool HasBiolabel(void) const {return ((Type & (1<<9)) != 0);};
		inline void SetFragmentNumber(long fragNum) {if (fragNum>=0) fragmentId = fragNum; IsEffectiveFragment(true);};
		inline long GetFragmentNumber(void) const {return fragmentId;};
		inline long GetNuclearCharge(void) const {return (GetType());};	//NOT correct for ECP's!!!!!
		void SetDefaultCoordinationNumber(void);
		void SetDefaultLonePairCount(void);
};

/**
 * The BondOrder defines the accepted bond orders within the program. Most self-explanatory.
 */
enum BondOrder {
	kHydrogenBond,
	kSingleBond,
	kDoubleBond,
	kTripleBond,
	kAromaticBond,
	kMixedBonds,	///< This one should only apply to the selecion of multiple bonds.
	kUnknownBond	///< Fallback, should be unused
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

class EFragAtom {
	public:
		EFragAtom(const std::string& label, const CPoint3D& coords,
				  int atomic_num)
			: label(label),
			  coords(coords),
			  atomic_num(atomic_num) {
		}

		const CPoint3D& GetCoords() const {
			return coords;
		}

		int GetAtomicNumber() const {
			return atomic_num;
		}

		const std::string& GetLabel() const {
			return label;
		}

	private:
		std::string label;
		CPoint3D coords;
		int atomic_num;
};

class EFrag {
	public:
		EFrag(const std::string& text);
		void ParseText();
		const std::vector<EFragAtom>& GetAtoms() const;
		const std::string& GetText() const;

	private:
		std::vector<EFragAtom> labeled_atoms;
		std::string raw_text;
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
/// Container for vibrational normal mode information.
class VibRec {
	private:
		///private function to initalize the class data before first use
		void init(void);
	public:
		std::vector<std::string>	Frequencies;	///< array of strings
		std::vector<CPoint3D>		NormMode;		///< Array of normal modes
		std::vector<std::string>	Symmetry;		///< array of strings for the symmetry of each mode
		std::vector<float>			Intensities;	///< Infrared intensity of each mode
		std::vector<float>			ReducedMass;	///< Reduced Mass for each mode
		std::vector<float>			RamanIntensity;	///< Raman spectrum
		std::vector<float>			Depolarization;	///< depolarization
		long						NumModes;		///< number of modes here
		long						CurrentMode;	///< Currently Drawn mode

		/** Contructor and memory allocation.
		 * This routine sets up the class and allocation and initializes the arrays.
		 * @param NumVibs The number of normal modes expected (normally up to 3*NumAtoms)
		 * @param NumAtoms The number of atoms in the geometry
		 */
		VibRec(const long & NumVibs, const long & NumAtoms);
		/** default constructor.
		 * You probably will need to call Setup to properly allocate the vectors
		 */
		VibRec(void);
		~VibRec(void);
		/** Allocate and initialize the class arrays.
		 * This routine allocates and initializes the arrays. You only need to call this if
		 * you use the default constructor.
		 * @param NumVibs The number of normal modes expected (normally up to 3*NumAtoms)
		 * @param NumAtoms The number of atoms in the geometry
		 */
		void Setup(const long & NumVibs, const long & NumAtoms);
		/** Resize the arrays to the current number of modes.
		 * @param NumAtoms This is needed to correctly dimension the normal mode array as NumModes*NumAtoms
		 */
		bool Resize(long NumAtoms);
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
	TDDFT,
	EOM_CC,
	
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
	DiabaticMolecularOrbital,
	NonOrthogonalSVDLocalizedOrbital,
	PPASVDLocalizedOrbital,
	SVDExternalLocalizedOrbital,
	SplitQAExternalLocalizedOrbital,
	OrderedExternalLocalizedOrbital,
	VB2000VBOrbital,
	VB2000InitialOrbital,
	VB2000LocalizedMolecularOrbital,
	VB2000MolecularOrbital,
	
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
		void WriteXML(XMLElement * parent) const;
		bool ReadXML(XMLElement * parent);
		const char * getLabel(void) const {return Label;};
		const char * getOrbitalTypeText(void) const;
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

typedef class SurfacePane SurfacePane;
typedef class SurfacesWin SurfacesWin;
typedef class Surface Surface;

class myGLTriangle {
	public:
		CPoint3D v1;
		CPoint3D v2;
		CPoint3D v3;
		CPoint3D n1;
		CPoint3D n2;
		CPoint3D n3;
		float r1, g1, b1, a1; //RGBA color for v1
		float r2, g2, b2, a2; //RGBA color for v2
		float r3, g3, b3, a3; //RGBA color for v3
};

class Surface {
	friend class Frame;
	protected:
		long		ID;
		Surface *	NextSurface;
		char *		Label;
		bool		Visible;
		bool		has_default_label;
		long		OrbSet;			//Target Orbital Set

		/**
		 * Exports surface data to our text file type	
		 * This can effectively only be called by Surf3DBase objects
		 * @param Grid3D
		 * @param nx number of grid points on the X axis
		 * @param ny number of grid points on the Y axis
		 * @param nz number of grid points on the Z axis
		 * @param Origin origin of the 3D cartesian grid
		 * @param XInc increment in the X direction 
		 * @param YInc increment in the Y direction 
		 * @param ZInc increment in the Z direction 
		 * @param Label provided text label for this surface
		 * @param Buffer A BufferFileObject which the .txt file is buffered into
		 * to make parsing the file easier.  See the BufferFile object for valid
		 * BufferFile operations.
		 */
		void Export3D(const float * Grid3D, long nx, long ny, long nz, const CPoint3D * Origin,
			float XInc, float YInc, float ZInc, const char * Label, BufferFile * Buffer) const;
		//! Exports to CCP4 file type
		void Export3DCCP4(const float * Grid3D, long nx, long ny, long nz, const CPoint3D * Origin,
			float XInc, float YInc, float ZInc, const char * Label, BufferFile * Buffer) const;
		//! Exports to CNS electron density map file type
		void Export3DCNS(const float * Grid3D, long nx, long ny, long nz, const CPoint3D * Origin,
			float XInc, float YInc, float ZInc, const char * Label, BufferFile * Buffer) const;
		/**
		 * Exports surface data to our text file type	
		 * This can effectively only be called by Surf2DBase objects
		 * @param Grid2D two dimensional grid of data
		 * @param NumPoints number of grid points in both the x and y direction
		 * @param Origin origin of the 3D cartesian grid
		 * @param XInc increment vector in the X direction 
		 * @param YInc increment vector in the Y direction 
		 * @param Label provided text label for this surface
		 * @param Buffer A BufferFileObject which the .txt file is buffered into
		 * to make parsing the file easier.  See the BufferFile object for valid
		 * BufferFile operations.
		 */	
		void Export2D(const float * Grid2D, long NumPoints, const CPoint3D * Origin,
				const CPoint3D *XInc, const CPoint3D *YInc, const char * Label, BufferFile * Buffer) const;
	public:
		enum exportFileType {
			TXTFILE,
			CCP4FILE,
			CNSFILE		
		};

		Surface(void);
		virtual ~Surface(void);
		virtual short GetDimensions(void) const=0;
		Surface * GetNextSurface(void) const {return NextSurface;};
		void SetNextSurface(Surface * target) {NextSurface = target;}
		virtual char *GetLabel(void);
		void SetLabel(const char * NewLabel);
		inline bool DefaultLabel(void) {return has_default_label;};
		virtual bool Needs2DPlane(void) const;
		virtual void Update(MoleculeData * MainData);
		virtual SurfaceType GetSurfaceType(void) const = 0;
		virtual bool isTransparent(void) const {return false;};
		inline bool GetVisibility(void) const {return Visible;};
		inline void SetVisibility(bool state) {Visible = (state!=0);};
		inline long GetSurfaceID(void) const {return ID;};
		inline long SetSurfaceID(long NewID) {ID = NewID; return ID;};
		inline long SetSurfaceID(void) {/*ID = TickCount(); return ID;*/
                                        static long global_ID = 0; ID = ++global_ID; return ID;};
		virtual void RotateEvent(MoleculeData * MainData);
		virtual void WriteXML(XMLElement * parent) const;
		virtual bool ReadXML(XMLElement * parent);
		static Surface * ReadSurface(XMLElement * parent);
		long WriteSurface(BufferFile * Buffer);
		virtual void Export(BufferFile * Buffer, exportFileType eft);
		virtual long ExportPOV(MoleculeData *lData, WinPrefs *Prefs, BufferFile *Buffer) {
			return 0;
		}
	//	void ExportToFile(void);
		virtual void RotateSurface(Matrix4D RotationMatrix);
		virtual bool ExportPossible(void) const {return false;};
		virtual bool NeedsUpdateDuringRotation(void) const {return false;};
		virtual void Draw2D(MoleculeData * lData, long hoffset, long voffset, float scale);
		virtual long Draw3DGL(MoleculeData * lData,  WinPrefs * Prefs, myGLTriangle *, unsigned int shader_program = 0)=0;
		virtual long getTriangleCount(void) const {return 0;};
		inline long getTargetOrbSet(void) const {return OrbSet;};
		inline void setTargetOrbSet(const long & target) {OrbSet = target;};
};

void ApplyRotation(Matrix4D RotMat, long Axis, float AngleDegrees);
void CalculateCenterOfMass(mpAtom * AtomList, long NumAtoms, float * AtomMasses, CPoint3D * Center);
void MinimizeDifferences(mpAtom * FixedAtoms, mpAtom * targetAtoms, long NumAtoms,
	WinPrefs * Prefs, long NumOptAtoms);
float CalculateSquaresValue(long NumOptAtoms, mpAtom CoordSetA[], CPoint3D CoordSet[]);

#endif
