/*
 *  (c) 2004 Iowa State University
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

class MOPacInternals {
	private:
		long *		ConnectionAtoms;
		float *		Values;
		char *		Type;
		long		Allocation;
		long		Count;
	public:
		MOPacInternals(long NumAtoms);
		MOPacInternals(BufferFile * Buffer);
		MOPacInternals(XMLElement * Parent);
		~MOPacInternals(void);
		void WriteZMATToFile(BufferFile * File);
		void WriteCoordinatesToFile(BufferFile * File, MoleculeData * MainData, WinPrefs * Prefs);
		long GetSize(BufferFile * Buffer);
		long Write(BufferFile * Buffer);
		void WriteXML(XMLElement * parent) const;
		void GuessInit(MoleculeData * MainData);
		void CartesiansToInternals(MoleculeData * MainData);
		void InternalsToCartesians(MoleculeData * MainData, WinPrefs * Prefs, long ChangedAtom);
		void AddAtom(MoleculeData * MainData);
		void DeleteAtom(MoleculeData * MainData, long WhichAtom);
		void UpdateAtoms(MoleculeData * MainData);
		inline long GetConnection(long i, short part) {return ConnectionAtoms[3*i+part];};
		inline float GetValue(long i, short part) {return Values[3*i+part];};
		inline void SetConnection(long i, short part, long j) {
			if ((i>=0)&&(i<Count)&&(part>=0)&&(part<3)) ConnectionAtoms[3*i+part]=j;};
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
};

#endif
