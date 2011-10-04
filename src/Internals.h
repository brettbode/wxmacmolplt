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

/**
	Stores the connectivity information for MOPac style internal coordinates. Methods also
	provide for the conversion of cartesian coordinates to/from MOPac internals coordinates.
	MOPac internals have the form:\n
 \verbatim
	label1 \n
	label2 1 distance1 \n
	label3 2 distance2 1 angle1 \n
	label4 2 distance3 1 angle2 3 dihedral1 \n
	and so on. \n
 \endverbatim
	The connectivity indexes must reference an atom earlier in the list.
 */

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
		/**
		 Copy constructor
		 */
		MOPacInternals(MOPacInternals * copy);
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
		 * @param MainData Master data record.
		 * @param Prefs Master window preferences.
		 */
		void WriteCoordinatesToFile(BufferFile * File, MoleculeData * MainData, WinPrefs * Prefs);
		/**
		 * Write a "MOPac" style zmatrix (in a $DATA group) to file. This outputs a MOPac
		 * style ZMatrix suitable for the $DATA group of a GAMESS input file.
		 * @param File The output file buffer.
		 * @param MainData Master data record.
		 * @param Prefs Master window preferences.
		 */
		void WriteMPCZMatCoordinatesToFile(BufferFile * File, MoleculeData * MainData, WinPrefs * Prefs);
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
		 * @param theAtom If provided, the single atom to initialize.
		 * @param keepOld If parts of the old connection list are valid preserve them.
		 */
		void GuessInit(MoleculeData * MainData, long theAtom=-1, bool keepOld=false);
		/**
		 * Update the current internal coordinate values. Computes the internal coordinate values
		 * based on the cartesian coordinates for the current frame.
		 * @param MainData The coordinate information.
		 */
		void CartesiansToInternals(MoleculeData * MainData);
		/**
		 * Update the current internal coordinate values for a single atom. Computes the internal coordinate values
		 * based on the cartesian coordinates for the current frame.
		 * @param MainData The coordinate information.
		 * @param theAtom The atom to update.
		 */
		void UpdateInternalValuesAtom(MoleculeData * MainData, long theAtom);
		/**
		 * Update the current cartesian coordinates. Computes the cartesian coordinates
		 * based on the current internal coordinate values. The coordinates are aligned with
		 * the previous set to keep the display from jumping around.
		 * @param MainData The coordinate information.
		 * @param Prefs The window preferences.
		 * @param ChangedAtom The atom whose internal coordinate was changed. Pass 0 to update
		 *		all.
		 */
		void InternalsToCartesians(MoleculeData * MainData, WinPrefs * Prefs, long ChangedAtom);
		/**
		 * Add an atom to the list.
		 * @param MainData The coordinate information.
		 */
		void AppendAtom(MoleculeData * MainData);
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
		/**
		 * Change the atom order. Call when the order of the atoms changes. Note the
		 * order of the cartesians should be changed first!
		 * @param MainData The coordinate information.
		 * @param OldPosition The orignal atom index.
		 * @param NewPosition The new atom position.
		 */
		void ChangeAtomIndex(MoleculeData * MainData, long OldPosition, long NewPosition);
};
class GeneralInternals {
	private:
	public:
};
/**
	Wrapper class for internal coordinates. Currently it only supports the zmatrix style
	internals, but it could be expanded to support other methods.
*/
class Internals {
	private:
		MOPacInternals *	MOPacStyle;
		GeneralInternals *	GeneralStyle;
	public:
		Internals(void);
		/**
		 Copy constructor
		 */
		Internals(Internals * copy);
		~Internals(void);
		inline MOPacInternals * GetMOPacStyle(void) {return MOPacStyle;};
		void ReadXML(XMLElement * parent);
		void WriteXML(XMLElement * parent) const;
		void CreateMOPacInternals(long Num);
		/**
		 * Output the connection list to a GAMESS $ZMAT group.
		 * @param File the file to write to.
		 */
		inline void WriteZMATToFile(BufferFile * File) 
			{if (MOPacStyle) MOPacStyle->WriteZMATToFile(File);};
		void WriteCoordinatesToFile(BufferFile * File, MoleculeData * MainData, WinPrefs * Prefs);
		void WriteMPCZMatCoordinatesToFile(BufferFile * File, MoleculeData * MainData, WinPrefs * Prefs);
		/**
		 * Change the atom order. Call when the order of the atoms changes. Note the
		 * order of the cartesians should be changed first!
		 * @param MainData The coordinate information.
		 * @param OldPosition The orignal atom index.
		 * @param NewPosition The new atom position.
		 */
		void ChangeAtomIndex(MoleculeData * MainData, long OldPosition, long NewPosition)
		{ if (MOPacStyle) MOPacStyle->ChangeAtomIndex(MainData, OldPosition, NewPosition); };
};

#endif
