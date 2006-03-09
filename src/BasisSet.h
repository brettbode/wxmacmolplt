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
typedef class XMLElement XMLElement;

enum TypeOfShell {
	LShell=-1,
	SShell=0,
	PShell,
	DShell,
	FShell,
	GShell,
	SHLShell=9,
	SHSShell=10,
	SHPShell,
	SHDShell,
	SHFShell,
	SHGShell
};

class BasisShell {
	public:
		float			*Exponent;			//Exponent for each primitive
		float			*NormCoef;			//Normalized coefficients
		float			*InputCoef;			//Standard input set of coefficients
		short			ShellType;			//s, p, d, etc
		short			NumPrims;			//# of primitives in this shell

		BasisShell(void);
		~BasisShell(void);
		long GetSize(BufferFile *Buffer) const;
		long WriteToBuffer(BufferFile *Buffer) const;
		void WriteXML(XMLElement * parent) const;
		void ReadXML(XMLElement * parent);
		long GetNumFuncs(bool UseSphericalHarmonics) const;
		long GetAngularStart(bool UseSphericalHarmonics) const;
		void Read(BufferFile *Buffer, long length);
		void GetLabel(char * Label, short FuncNum, bool UseSphericalHarmonics) const;
		inline long GetNumPrimitives(void) const {return NumPrims;};
		inline long GetShellType(void) const {return ((ShellType>=0) ? ShellType : -ShellType);};
};

class BasisSet {
	public:
		BasisShell *	Shells;				//Set of shells in basis
		long			*BasisMap;			//Maps the basis onto molecule
											//stored as start, end for each atom
		long			MapLength;			//allocation size of the BasisMap
		long			NumShells;			//# of saved shells (some shells may be reused)
		long			NumFuncs;			//Total # of funcs, that is the total number of
											//of shells x the # of functions per shell (ie 1 for
											//s, 3 for p,...) MOVectors are NumFuncsxNumFuncs
		long *			NuclearCharge;		//Normally the same as the atom type, except for ECP's

	//member functions
		BasisSet(long nAtoms, long nShells);
		~BasisSet(void);
		long GetSize(BufferFile *Buffer);
		long WriteToBuffer(BufferFile *Buffer);
		void WriteXML(XMLElement * parent) const;
		static BasisSet * ReadXML(XMLElement * parent);
		static BasisSet * Read(BufferFile *Buffer, long test, long length);
		void ReadCodes(BufferFile *Buffer, long NumShells, long NumFuncs, long NumAtoms,
			long length);
		void WriteBasis(BufferFile * File, long AtomNum) const;
		long GetNumBasisFuncs(bool UseSphericalHarmonics) const;
		inline long * GetNuclearChargeArray(void) const {return NuclearCharge;};
		void GetShellIndexArray(long * IndexArray) const;
		void Normalize(bool InputNormed, bool NormOutput);
		static BasisSet * ParseGAMESSBasisSet(BufferFile * Buffer, long NumAtoms);
};

#endif /* __BasisSet__ */
