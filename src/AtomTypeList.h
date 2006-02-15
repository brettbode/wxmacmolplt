/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* AtomTypeList.h

	Simple class to collect information about which atom types are used in
	a given system.
	
	BMB 11/1998
*/

class AtomTypeList {
	private:
		long 	Types[kMaxAtomTypes];
		long	NumSortedTypes;
		Boolean	Sorted;
	public:
		AtomTypeList(void);
		void AddAtomType(long newType);
		long GetMaxAtomType(void) const;
		Boolean IsAtomInList(long atomType) const;
		void Sort(void);
		Boolean TransitionMetalInList(void) const;
};
