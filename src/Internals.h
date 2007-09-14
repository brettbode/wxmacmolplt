/*
 *  (c) 2007 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* Internals.cpp

	Classes to handle internal coordinates

	BMB 4/1998
*/

#ifndef __Internals__
#define __Internals__
#ifndef __BFile__
#include "BFiles.h"
#endif
#ifndef __MyTypes__
#include "MyTypes.h"
#endif

/// MOPacInterals handles the conversion of coordinates to/from MOPac style internal coordinates

/// Stores the connectivity information for MOPac style internal coordinates. Methods also
/// provide for the conversion of cartesian coordinates to/from MOPac internals coordinates.
/// MOPac internals have the form:
/// label1
/// label2 1 distance1
/// label3 2 distance2 1 angle1
/// label4 2 distance3 1 angle2 3 dihedral1
/// and so on.
/// The connectivity indexes must reference an atom earlier in the list.

class MOPacInternals {
	private:
		long *		ConnectionAtoms;	///< Array of the connection indexes
		float *		Values;				///< Array of the internal coordinate values
		char *		Type;				///< The type of each internal coordinate
		long		Allocation;			///< Allocation size of the arrays
		long		Count;				///< Internal coordinate count
	public:
		/**
		 * Constructor.
		 * @param count The initial number of coordinates to expect.
		 */
		MOPacInternals(long count);
		MOPacInternals(BufferFile * Buffer);
		/**
		 * Constructor with initial data from CML.
		 * @param Parent The XML parent element containing the internal coordinate data.
		 */
		MOPacInternals(XMLElement * Parent);
		/**
		 * Destructor.
		 */
		~MOPacInternals(void);
		/**
		 * Write a $ZMAT group to a GAMESS input file. A $ZMAT group is just connection list.
		 * @param File The output file buffer.
		 */
		void WriteZMATToFile(BufferFile * File);
		/**
		 * Write a "Gaussian" style zmatrix (in a $DATA group) to file. This outputs a normal
		 * GAMESS ZMatrix suitable for the $DATA group of a GAMESS input file.
		 * @param File The output file buffer.
		 */
		void WriteCoordinatesToFile(BufferFile * File, MoleculeData * MainData, WinPrefs * Prefs);
		/**
		 * Write a "MOPac" style zmatrix (in a $DATA group) to file. This outputs a MOPac
		 * style ZMatrix suitable for the $DATA group of a GAMESS input file.
		 * @param File The output file buffer.
		 */
		void WriteMPCZMatCoordinatesToFile(BufferFile * File, MoleculeData * MainData, WinPrefs * Prefs);
		long GetSize(BufferFile * Buffer);
		long Write(BufferFile * Buffer);
		/**
		 * Output the class in XML for saving to file. Save the class data in XML suitable for
		 * the XML based constructor to read back in.
		 * @param parent The parent XML element.
		 */
		void WriteXML(XMLElement * parent) const;
		/**
		 * Initialize the connection list based on the provided molecule. The connection list will
		 * be setup based on distances and other information, then the current internal coordinate
		 * values computed.
		 * @param MainData The coordinate information.
		 */
		void GuessInit(MoleculeData * MainData);
		/**
		 * Update the current internal coordinate values. Computes the internal coordinate values
		 * based on the cartesian coordinates for the current frame.
		 * @param MainData The coordinate information.
		 */
		void CartesiansToInternals(MoleculeData * MainData);
		/**
		 * Update the current cartesian coordinates. Computes the cartesian coordinates
		 * based on the current internal coordinate values. The coordinates are aligned with
		 * the previous set to keep the display from jumping around.
		 * @param MainData The coordinate information.
		 * @param Prefs The window preferences.
		 * @param ChangeAtom The atom whose internal coordinate was changed. Pass 0 to update
		 *		all.
		 */
		void InternalsToCartesians(MoleculeData * MainData, WinPrefs * Prefs, long ChangedAtom);
		/**
		 * Add an atom to the list.
		 * @param MainData The coordinate information.
		 */
		void AddAtom(MoleculeData * MainData);
		/**
		 * Delete an atom from the list.
		 * @param MainData The coordinate information.
		 * @param WhichAtom The atom to delete.
		 */
		void DeleteAtom(MoleculeData * MainData, long WhichAtom);
		/**
		 * Update the connection list given the current coordinates. This routine can be called
		 * after adding atoms to make sure the connection list correctly covers all atoms.
		 * @param MainData The coordinate information.
		 */
		void UpdateAtoms(MoleculeData * MainData);
		/**
		 * Obtain a connection index. 
		 * @param i The desired atom.
		 * @param part 0 for bond atom, 1 for bond angle atom, 2 for dihedral atom.
		 */
		inline long GetConnection(long i, short part) {return ConnectionAtoms[3*i+part];};
		/**
		 * Obtain an internal coordinate value. 
		 * @param i The desired atom.
		 * @param part 0 for bond atom, 1 for bond angle atom, 2 for dihedral atom.
		 */
		inline float GetValue(long i, short part) {return Values[3*i+part];};
		/**
		 * Add an internal coordinate with a specific value. 
		 * @param whichAtom The target atom.
		 * @param connectedAtom The index of the reference atom
		 * @param type 0 for bond atom, 1 for bond angle atom, 2 for dihedral atom.
		 * @param value The coordinate value.
		 */
		void AddInternalCoordinate(long whichAtom, long connectedAtom, short type, float value);
		/**
		 * Set an internal coordinate connection index. 
		 * @param i The desired atom.
		 * @param part 0 for bond atom, 1 for bond angle atom, 2 for dihedral atom.
		 * @param j The new target atom index.
		 */
		inline void SetConnection(long i, short part, long j) {
			if ((i>=0)&&(i<Count)&&(part>=0)&&(part<3)) ConnectionAtoms[3*i+part]=j;};
		/**
		 * Set an internal coordinate value. 
		 * @param i The desired atom.
		 * @param part 0 for bond atom, 1 for bond angle atom, 2 for dihedral atom.
		 * @param val The desired value.
		 */
		inline void SetValue(long i, short part, float val) {
			if ((i>=0)&&(i<Count)&&(part>=0)&&(part<3)) Values[3*i+part]=val;};
};
class GeneralInternals {
	private:
	public:
};
class Internals {
	private:
		MOPacInternals *	MOPacStyle;
		GeneralInternals *	GeneralStyle;
	public:
		Internals(void);
		~Internals(void);
		inline MOPacInternals * GetMOPacStyle(void) {return MOPacStyle;};
		long GetSize(BufferFile * Buffer);
		long Write(BufferFile * Buffer);
		void ReadXML(XMLElement * parent);
		void WriteXML(XMLElement * parent) const;
		Internals * Read(BufferFile * Buffer);
		void CreateMOPacInternals(long Num);
		inline void WriteZMATToFile(BufferFile * File) {if (MOPacStyle) MOPacStyle->WriteZMATToFile(File);};
		inline void WriteCoordinatesToFile(BufferFile * File, MoleculeData * MainData, WinPrefs * Prefs) 
			{if (MOPacStyle) MOPacStyle->WriteCoordinatesToFile(File, MainData, Prefs);};
		inline void WriteMPCZMatCoordinatesToFile(BufferFile * File, MoleculeData * MainData, WinPrefs * Prefs) 
		{if (MOPacStyle) MOPacStyle->WriteMPCZMatCoordinatesToFile(File, MainData, Prefs);};
};

#endif
