/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* AtomTypeList.h

	Simple class to collect information about which atom types are used in
	a given system.
	
	BMB 11/1998
*/

#ifndef ATOM_TYPE_LIST_H
#define ATOM_TYPE_LIST_H

class AtomTypeList {
	private:
		long 	Types[kMaxAtomTypes];
		long	NumSortedTypes;
		bool	Sorted;
	public:
		AtomTypeList(void);
		void AddAtomType(long newType);
		long GetMaxAtomType(void) const;
		bool IsAtomInList(long atomType) const;
		void Sort(void);
		bool TransitionMetalInList(void) const;
};

#endif

