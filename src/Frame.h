/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* Frame.h

	class to organize all data specific to one particular geometry point

	BMB 4/1998
*/

#ifndef __Frame__
#define __Frame__

#include "MyTypes.h"
/* #include "MolDisplayWin.h" */

typedef class MoleculeData MoleculeData;
typedef class WinPrefs WinPrefs;
typedef class Progress Progress;
typedef struct GaussHermiteData GaussHermiteData;
typedef class BasisSet BasisSet;
typedef class GradientData GradientData;
typedef class AtomTypeList AtomTypeList;
typedef class XMLElement XMLElement;
#ifdef USE_OPENBABEL
typedef class OBMol OBMol;
#endif

#if defined(WIN32)
#undef AddAtom
#endif

#include <vector>

typedef class Frame Frame;
class Frame {
		friend class MolDisplayWin;
		friend class SetPlaneDlg;
		friend class ModeOffsetDlg;
		friend class LLMDlg;
		friend class FrameEnergyDlg;
		friend class CoordWin;
		friend class MoleculeData;
		friend class EnergyPlotDialog;
		friend class FrequenciesDialog;
		friend class ZCalcWin;
		friend class Surf2DBase;
		friend class Surf3DBase;
		friend class Orb2DSurface;
		friend class Orb3DSurface;
		friend class TEDensity3DSurface;
		friend class TEDensity2DSurface;
		friend class TEDensity1DSurface;
		friend class MOPacInternals;
		friend class DataGroup;
		friend class ImportVecDlg;
		friend class MpGLCanvas;
		friend class FrameSnapShot;
	private:
		double		Energy;
		double		KE;
		double		MP2Energy;
		float		time;
		long		IRCPt;
		mpAtom *	Atoms;
		Bond *		Bonds;
		long		NumAtoms;
		long		AtomAllocation;
		long		NumBonds;
		long		BondAllocation;
		CPoint3D *	SpecialAtoms;
		VibRec *	Vibs;
		std::vector<OrbitalRec *> Orbs;
		Surface *	SurfaceList;
		GradientData *	Gradient;

		Frame *		NextFrame;
		Frame *		PreviousFrame;

		int natoms_selected;
		MolDisplayWin *MolWin;
		int targeted_atom;

	public:
		Frame(MolDisplayWin *MolWin);
		~Frame(void);

		//Frame& operator= (const Frame& f);
		//assignment operator overload -- not needed currently

		void SetNextFrame(Frame *);
		void SetPreviousFrame(Frame *);
		Frame * GetNextFrame(void);
		Frame * GetPreviousFrame(void);
		Surface * GetSurface(long SurfaceNum);
		Surface * GetSurfaceWithID(long SurfaceID);
		const std::vector<OrbitalRec *> * GetOrbitalSetVector(void) const {return &Orbs;};
		void DeleteSurface(long SurfaceNum);
		void DeleteSurfaceWithID(long SurfaceID);
		void AppendSurface(Surface * NewSurface);
		long GetNumSurfaces(void);
		inline double GetEnergy(void) const {return Energy;};
		inline double GetKineticEnergy(void) const {return KE;};
		inline double GetMP2Energy(void) const {return MP2Energy;};
		inline float GetTime(void) const {return time;};
		inline void SetEnergy(const double & val) {Energy = val;};
		inline void SetKineticEnergy(const double & val) {KE = val;};
		inline void SetMP2Energy(const double & val) {MP2Energy = val;};
		inline void SetTime(const float & val) {time = val;};
		void ChangeBond(long TheBond, short WhichPart, long TheAtom);
		long GetBondAtom(long WhichBond, short ThePart);
		short GetAtomType(long theAtom) const;
		bool SetAtomType(long theAtom, short atmType);
		bool GetAtomPosition(long theAtom, CPoint3D & p) const;
		bool SetAtomPosition(long theAtom, const CPoint3D & p);
		mpAtom * AddAtom(long AtomType, const CPoint3D & AtomPosition, long index=-1);
		mpAtom *AddAtom(const mpAtom& atm, long index=-1, const CPoint3D *pos = NULL);
		bool IncreaseAtomAllocation(long NumAdditional);
		//BondExists returns the id of the bond, -1 otherwise
		long BondExists(long atom1, long atom2) const;
		void DeleteAtom(long AtomNum);
		//AddBond validates the two atom references and increases the BondAllocation as
		//necessary, but does not check the uniqueness of the bond
		bool AddBond(long Atom1, long Atom2, const BondOrder& b=kSingleBond);
		bool IncreaseBondAllocation(long NumAdditional);
		void DeleteBond(long BondNum);
			/**
			 * Autmatically determines bonds based in atomic radii and interatomic distances
			 * along with user selected tolerances.
			 * @param Prefs The user preferences object.
			 * @param KeepOldBonds If true bonds will only be added to the existing set.
			 * @param selectedOnly If true bonds for the currently selected set of atoms will be
			 *                     erased and redetermined.
			 */
		void SetBonds(WinPrefs * Prefs, bool KeepOldBonds, bool selectedOnly=false);
		bool AddSpecialAtom(CPoint3D Vector, long AtomNum);
		bool SurfaceExportPossible(void);
		void ExportSurface(BufferFile * Buffer);
		AtomTypeList * GetAtomTypes(void);
		inline long GetNumBonds(void) const {return NumBonds;};
		inline long GetNumAtoms(void) const {return NumAtoms;};
		inline Bond * GetBondLoc(long ibond) {return &(Bonds[ibond]);};
		float GetBondLength(long ibond);
		long GetNumMMAtoms(void);
		/** Toggle the visibility of MM atoms (ie in SiMOMM runs).
		 */
		void toggleMMAtomVisibility(void);
		/** Toggle the visibility of ab initio atoms.
		 */
		void toggleAbInitioVisibility(void);
		/** Toggle the visibility of effective fragments.
		 */
		void toggleEFPVisibility(void);
		bool GetBondLength(long atom1, long atom2, float * length);
		bool GetBondAngle(long atom1, long BondAtom, long AngleAtom, float * angle);
		bool GetBondDihedral(long atom1, long BondAtom, long AngleAtom, long DihedralAtom,
			float * angle);
		inline bool GetBondSelectState(long BondNum) const {return ((BondNum<NumBonds)?Bonds[BondNum].GetSelectState():false);};
		inline void SetBondSelectState(long BondNum, bool state)
			{if (BondNum<NumBonds) Bonds[BondNum].SetSelectState(state);};
		inline BondOrder GetBondOrder(long BondNum) const {return Bonds[BondNum].Order;};
		long GetNumElectrons(void) const;
		inline void SetBondOrder(long BondNum, BondOrder NewOrder) {Bonds[BondNum].Order = NewOrder;};
		void ParseGAMESSGuessVectors(BufferFile * Buffer, long NumFuncs, TypeOfWavefunction t, Progress * lProgress);
		void ParseGAMESSMCSCFVectors(BufferFile * Buffer, long NumFuncs, long NumOrbs, Progress * lProgress);
		void ParseGAMESSCIVectors(BufferFile * Buffer, long NumFuncs, Progress * lProgress);
		/** Parse the GAMESS UHF natural orbitals and occupation numbers.
		 * @param Buffer the input buffer.
		 * @param NumFuncs the number of basis functions.
		 * @param lProgress the progress indicator.
		 */
		void ParseUHFNOs(BufferFile * Buffer, long NumFuncs, Progress * lProgress);
		/** Parse the GAMESS TD-DFT natural orbitals and occupation numbers.
		 * @param Buffer the input buffer.
		 * @param NumFuncs the number of basis functions.
		 * @param lProgress the progress indicator.
		 */
		void ParseTDDFTNOs(BufferFile * Buffer, long NumFuncs, Progress * lProgress);
		void ParseGVBGIOrbitals(BufferFile * Buffer, const long & NumFuncs, Progress * lProgress);
		OrbitalRec * ParseGAMESSEigenVectors(BufferFile * Buffer, long NumFuncs, long NumOrbs,
			long NumBetaOrbs, const long & NumOccAlpha, const long & NumOccBeta, const TypeOfWavefunction & method, Progress * lProgress);
		OrbitalRec * ParseGAMESSLMOs(BufferFile * Buffer, long NumFuncs, long NumAlphaOrbs,
			long NumOccBeta, Progress * lProgress, bool OrientedSet);
		void ReadMolDenOrbitals(BufferFile * Buffer, long NumFuncs);
		void ReadMP2Vectors(BufferFile * Buffer, BufferFile * DatBuffer, long NumFuncs,
			Progress * lProgress, long * readflag);
		float CalculateMEP(float x, float y, float z, BasisSet * Basis,
			AODensity * TotalAODensity, GaussHermiteData * GHData, float * ElectronicMEP, float * NuclearMEP);
		AODensity * GetAODensity(BasisSet * basis, const long & targetOrbSet);
		void ReadGradient(BufferFile * Buffer, long SearchLength);
		float GetRMSGradient(void) const;
		float GetMaxGradient(void) const;
		void SetRMSGradient(float val);
		void SetMaximumGradient(float val);
		inline VibRec * GetFrequencies(void) const {return Vibs;};
		inline long GetNumberNormalModes(void) const {return ((Vibs!=NULL)?Vibs->GetNumModes():0);}
		void ParseNormalModes(BufferFile * Buffer, Progress *, WinPrefs *);
		void ParseMolDenFrequencies(BufferFile * Buffer, WinPrefs *);
		void DeleteOrbitals(void);
		bool ReadCMLMolecule(XMLElement * molElement);
		bool ParseAtomXML(XMLElement * atomXML, std::vector<char *> & idList);
		void ParseAtomArray(XMLElement * arrayXML, std::vector<char *> & idList);
		bool ParseBondXML(XMLElement * bondXML, const std::vector<char *> & idList);
		void ParseBondArrayXML(XMLElement * arrayXML, const std::vector<char *> & idList);
		void ParseAtomAttributeArrayXML(XMLElement * atomXML, const std::vector<char *> & idList);
		long WriteCMLFrame(XMLElement * parent, bool AllData);

		void resetAllSelectState();
		bool SetAtomSelection(long atom_id, bool select_it);
		bool GetAtomSelection(long atom_id) const;
		int GetNumAtomsSelected(void) const;
		int GetAtomNumBonds(int atom_id) const;
#ifdef USE_OPENBABEL
		OBMol * ConvertToOBMol(void) const;
		bool ConvertFromOBMol(const OBMol & mol);
#endif
		bool SetAtomOxidationNumber(int atom_id, int ox_num);
		int GetAtomOxidationNumber(int atom_id);

		//
		inline int GetTargetAtom() const { return targeted_atom; }
		inline void SetTargetAtom(int id) { targeted_atom = id; }
};

#endif
