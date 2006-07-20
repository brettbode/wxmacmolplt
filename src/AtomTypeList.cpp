/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* AtomTypeList.cpp

	Class to collect information about which atom types are in use in a given system.
	
	BMB 11/1998
*/

#include "Globals.h"
#include "MyTypes.h"
#include "AtomTypeList.h"


AtomTypeList::AtomTypeList(void) {
	for (long i=0; i<kMaxAtomTypes; i++) Types[i] = 0;
	NumSortedTypes = 0;
	Sorted = false;
}
void AtomTypeList::AddAtomType(long newType) {
	Types[newType-1] ++;
}
void AtomTypeList::Sort(void) {
	NumSortedTypes = 0;
	for (long i=0; i<kMaxAtomTypes; i++) {
		if (Types[i] > 0) {
			Types[NumSortedTypes] = i+1;
			NumSortedTypes++;
		}
	}
	Sorted = true;
}
long AtomTypeList::GetMaxAtomType(void) const {
	return (Types[NumSortedTypes-1]);
}
bool AtomTypeList::TransitionMetalInList(void) const {
	bool result = false;
	
	for (long i=0; i<NumSortedTypes; i++) {
		if (Types[i]>=21 && Types[i]<=30) result = true;
		else if (Types[i]>=39 && Types[i]<=48) result = true;
		else if (Types[i]>=72 && Types[i]<=80) result = true;
	}
	return result;
}
bool AtomTypeList::IsAtomInList(long atomType) const {
	bool result = false;
	
	for (long i=0; i<NumSortedTypes; i++) {
		if (Types[i] == atomType) result = true;
	}
	return result;
}

