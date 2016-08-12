/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	BasisSet.h

	Basis Set classes

	Seperated from other files 4/1998 BMB
*/
#ifndef __BasisSet__
#define __BasisSet__
#include <vector>

typedef class XMLElement XMLElement;

/**
 * The TypeOfShell enum is pretty self explanatory in that it lists the possible
 * shell types including cartesian S-I shells and the same for spherical harmonics.
 */
enum TypeOfShell {
	LShell=-1,
	SShell=0,
	PShell,
	DShell,
	FShell,
	GShell,
	HShell,
	IShell,
	SHLShell=9,
	SHSShell=10,
	SHPShell,
	SHDShell,
	SHFShell,
	SHGShell,
	SHHShell,
	SHIShell
};

/**
 * BasisShell contains the information on a single shell including the shell type,
 * coefficients and exponents.
 */
class BasisShell {
	public:
		std::vector<float>	Exponent;		///< Exponent for each primitive
		std::vector<float>	NormCoef;		///< Normalized coefficients
		std::vector<float>	InputCoef;		///< Standard input set of coefficients
		short			ShellType;			///< s, p, d, etc
		short			NumPrims;			///< # of primitives in this shell

		BasisShell(void);
		~BasisShell(void);
		/// Convert the shell data into XML format
		void WriteXML(XMLElement * parent) const;
		/// Build this shell from the provided XML
		void ReadXML(XMLElement * parent);
		/// Returns the number of functions in the shell taking into account spherical harmonics
		/// This would be the number of coefficients needed in an MOVector for this shell.
		long GetNumFuncs(bool UseSphericalHarmonics) const;
		/// Return the start of the function in the angular momenta list
		long GetAngularStart(bool UseSphericalHarmonics) const;
		/// Returns a plain text form of the label for each function
		void GetLabel(char * Label, short FuncNum, bool UseSphericalHarmonics) const;
		/// Returns the dimension of the primitive contraction.
		inline long GetNumPrimitives(void) const {return NumPrims;};
		/// Returns the shell type
		inline long GetShellType(void) const {return ((ShellType>=0) ? ShellType : -ShellType);};
};

/**
 * The BasisSet class packages up the basis set information for a typical computation.
 * This include a set of basis shells and a mapping of the shells to the atoms in the molecule.
 */
class BasisSet {
	public:
		std::vector<BasisShell>	Shells;		///< Set of shells in basis
		std::vector<long>	BasisMap;		///< Maps the basis onto molecule
											///< stored as start, end for each atom
		std::vector<long>	NuclearCharge;	///< Normally the same as the atom type, except for ECP's
		long			MapLength;			///< allocation size of the BasisMap
		long			NumShells;			///< number of saved shells (some shells may be reused)
		long			NumFuncs;			///< Total # of funcs, that is the total number of
											///< of shells x the # of functions per shell (ie 1 for
											///< s, 3 for p,...) MOVectors are NumFuncsxNumFuncs
		bool			goodCharges;		///< Are the nuclear charges valid (normally true!)

	//member functions
		BasisSet(long nAtoms, long nShells);
		~BasisSet(void);
		/// Convert the Basis Set to XML for output
		void WriteXML(XMLElement * parent) const;
		/// Parse the provided XML to produce a basis set.
		static BasisSet * ReadXML(XMLElement * parent);
		/// Output the basis for the specifed atom in GAMESS input file format.
		void WriteBasis(BufferFile * File, long AtomNum) const;
		/// Returns the number of basis functions taking spherical harmonics into account.
		/// This is the expected dimension for an MO vector.
		long GetNumBasisFuncs(bool UseSphericalHarmonics) const;
		/// Returns the shell count in the basis set.
		long GetNumShells(void) const {return NumShells;};
		/// Return the nuclear charge array.
		inline std::vector<long> GetNuclearChargeArray(void) const {return NuclearCharge;};
		/// Return in the provided array an indexing of the start of each shell into an MO vector.
		void GetShellIndexArray(long * IndexArray) const;
		/// Normalize the basis set coefficients to save effort when producing grids later.
		void Normalize(bool InputNormed, bool NormOutput);
		/// Parse the basis set from a GAMESS log file
		static BasisSet * ParseGAMESSBasisSet(BufferFile * Buffer, long NumAtoms, const mpAtom * Atoms);
		/// Parses a basis set from a MolDen format file with the given number of atoms.
		bool ReadMolDenBasisSet(BufferFile * Buffer, long NumAtoms);
		/**
		 * Is the nuclear charge array valid?
		 */
		bool AreNuclearChargesValid(void) const {return goodCharges;};
		/**
		 * Set the flag to indicate if the nuclearCharge array is valid.
		 */
		void NuclearChargesAreValid(bool v) {goodCharges = v;};
};

#endif /* __BasisSet__ */
