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
typedef class MoleculeData MoleculeData;
typedef class WinPrefs WinPrefs;
typedef class Progress Progress;
typedef class GaussHermiteData GaussHermiteData;
typedef class BasisSet BasisSet;
typedef class GradientData GradientData;
typedef class AtomTypeList AtomTypeList;
typedef class XMLElement XMLElement;

#ifdef WIN32
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
		friend class EnergyPlotWin;
		friend class ZCalcWin;
		friend class Surf2DBase;
		friend class Surf3DBase;
		friend class Orb2DSurface;
		friend class Orb3DSurface;
		friend class TEDensity3DSurface;
		friend class TEDensity2DSurface;
		friend class MOPacInternals;
		friend class DataGroup;
		friend class ImportVecDlg;
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
	public:
		Frame(void);
		~Frame(void);
		void Read(BufferFile * Buffer, long length);
		long Write(BufferFile * Buffer);
		void Read41(BufferFile * Buffer, long length);
		void ConvertFrameCode2(BufferFile * Buffer);
		void ConvertCode3(BufferFile * Buffer, long length);
		void ConvertCode4(BufferFile * Buffer, long length);
		void ConvertCode5(BufferFile * Buffer, long length);
		void ReadSurfaceList(BufferFile * Buffer, long length);
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
		void ChangeBond(long TheBond, short WhichPart, long TheAtom);
		long GetBondAtom(long WhichBond, short ThePart);
		short GetAtomType(long theAtom) const;
		bool SetAtomType(long theAtom, short atmType);
		bool GetAtomPosition(long theAtom, CPoint3D & p) const;
		bool SetAtomPosition(long theAtom, const CPoint3D & p);
		mpAtom * AddAtom(long AtomType, CPoint3D AtomPosition);
		bool IncreaseAtomAllocation(long NumAdditional);
			//BondExists returns the id of the bond, -1 otherwise
		long BondExists(long atom1, long atom2);
		void DeleteAtom(long AtomNum);
			//AddBond validates the two atom references and increases the BondAllocation as
			//necessary, but does not check the uniqueness of the bond
		bool AddBond(long Atom1, long Atom2, const BondOrder & b=kSingleBond);
		bool IncreaseBondAllocation(long NumAdditional);
		void DeleteBond(long BondNum);
		void SetBonds(WinPrefs * Prefs, bool KeepOldBonds);
		bool AddSpecialAtom(CPoint3D Vector, long AtomNum);
		bool SurfaceExportPossible(void);
		void ExportSurface(BufferFile * Buffer);
		AtomTypeList * GetAtomTypes(void);
		inline long GetNumBonds(void) const {return NumBonds;};
		inline long GetNumAtoms(void) const {return NumAtoms;};
		inline Bond * GetBondLoc(long ibond) {return &(Bonds[ibond]);};
		float GetBondLength(long ibond);
		long GetNumMMAtoms(void);
		void toggleMMAtomVisibility(void);
		void toggleAbInitioVisibility(void);
		bool GetBondLength(long atom1, long atom2, float * length);
		bool GetBondAngle(long atom1, long BondAtom, long AngleAtom, float * angle);
		bool GetBondDihedral(long atom1, long BondAtom, long AngleAtom, long DihedralAtom,
			float * angle);
		inline bool GetBondSelectState(long BondNum) {return ((BondNum<NumBonds)?Bonds[BondNum].GetSelectState():false);};
		inline void SetBondSelectState(long BondNum, bool state)
			{if (BondNum<NumBonds) Bonds[BondNum].SetSelectState(state);};
		inline bool GetAtomSelectState(long AtomNum) {return ((AtomNum<NumAtoms)?Atoms[AtomNum].GetSelectState():false);};
		inline void SetAtomSelectState(long AtomNum, bool state)
			{if (AtomNum<NumAtoms) Atoms[AtomNum].SetSelectState(state);};
		inline BondOrder GetBondOrder(long BondNum) {return Bonds[BondNum].Order;};
		long GetNumElectrons(void) const;
		inline void SetBondOrder(long BondNum, BondOrder NewOrder) {Bonds[BondNum].Order = NewOrder;};
		void ParseGAMESSGuessVectors(BufferFile * Buffer, long NumFuncs, TypeOfWavefunction t, Progress * lProgress);
		void ParseGAMESSMCSCFVectors(BufferFile * Buffer, long NumFuncs, long NumOrbs, Progress * lProgress);
		void ParseGAMESSCIVectors(BufferFile * Buffer, long NumFuncs, Progress * lProgress);
		void ParseGVBGIOrbitals(BufferFile * Buffer, const long & NumFuncs, Progress * lProgress);
		OrbitalRec * ParseGAMESSEigenVectors(BufferFile * Buffer, long NumFuncs, long NumOrbs,
			long NumBetaOrbs, const long & NumOccAlpha, const long & NumOccBeta, const TypeOfWavefunction & method, Progress * lProgress);
		OrbitalRec * ParseGAMESSLMOs(BufferFile * Buffer, long NumFuncs, long NumAlphaOrbs,
			long NumOccBeta, Progress * lProgress, bool OrientedSet);
		void ReadMP2Vectors(BufferFile * Buffer, BufferFile * DatBuffer, long NumFuncs,
			Progress * lProgress, long * readflag);
		float CalculateMEP(float x, float y, float z, BasisSet * Basis,
			AODensity * TotalAODensity, GaussHermiteData * GHData, float * ElectronicMEP, float * NuclearMEP);
		AODensity * GetAODensity(BasisSet * basis, const long & targetOrbSet);
		void ReadGradient(BufferFile * Buffer, long SearchLength);
		float GetRMSGradient(void);
		float GetMaxGradient(void);
		inline VibRec * GetFrequencies(void) {return Vibs;};
		void ParseNormalModes(BufferFile * Buffer, Progress *, WinPrefs *);
		void DeleteOrbitals(void);
		bool ReadCMLMolecule(XMLElement * molElement);
		bool ParseAtomXML(XMLElement * atomXML, std::vector<char *> & idList);
		void ParseAtomArray(XMLElement * arrayXML, std::vector<char *> & idList);
		bool ParseBondXML(XMLElement * bondXML, const std::vector<char *> & idList);
		void ParseBondArrayXML(XMLElement * arrayXML, const std::vector<char *> & idList);
		long WriteCMLFrame(XMLElement * parent, bool AllData);
};

#endif
