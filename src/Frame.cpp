/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* Frame.cpp

	Class to organize data which is specific to one geometry point

	BMB 4/1998
	Added Bond Order and Hydrogen Bond generation 12/1998
	Fixed problem reading incomplete sets of UHF orbitals in ParseEigenVectors - 10/2000 BMB
	Moved normal mode parsing to ParseNormalModes - 1/2001 BMB
	Updated CI orbital parser for addition of sym - 8/2002 BMB
	Adjusted the MP2 orbital dat file parser for reduced variational space - 9/2002 BMB
*/

#include "Globals.h"
#include "GlobalExceptions.h"
#include "MyTypes.h"
#include "myFiles.h"
#include "Frame.h"
#include "Progress.h"
#include "Prefs.h"
#include "Gradient.h"
#include "AtomTypeList.h"
#include "wx/wx.h"
#include <string.h>
#include <new>
#include <ctype.h>

#ifdef WIN32
#undef AddAtom
#endif

Frame::Frame(void) {
	Energy = 0.0;
	KE = 0.0;
	MP2Energy = 0.0;
	time = 0.0;
	IRCPt = 0;
	Atoms = NULL;
	Bonds = NULL;
	NumAtoms = 0;
	AtomAllocation = 0;
	NumBonds = 0;
	BondAllocation = 0;
	SpecialAtoms = NULL;
	Vibs = NULL;
	SurfaceList = NULL;
	Gradient = NULL;

	NextFrame = NULL;
	PreviousFrame = NULL;
 
}

long Frame::Write(BufferFile * Buffer) {
	long length, total=0;

	length = 3*sizeof(double) + sizeof(float) +3*sizeof(long);
	total += Buffer->Write((Ptr) &length, sizeof(long));

	total += Buffer->Write((Ptr) &Energy, sizeof(double));
	total += Buffer->Write((Ptr) &KE, sizeof(double));
	total += Buffer->Write((Ptr) &MP2Energy, sizeof(double));
	total += Buffer->Write((Ptr) &time, sizeof(float));
	total += Buffer->Write((Ptr) &IRCPt, sizeof(long));
	total += Buffer->Write((Ptr) &NumAtoms, sizeof(long));
	total += Buffer->Write((Ptr) &NumBonds, sizeof(long));
	
	return total;
}

/*
Frame& Frame::operator= (const Frame& f)
{
  delete [] Atoms;
  Atoms = NULL;
  delete [] Bonds;
  Bonds = NULL;

  this->NumAtoms = 0;
  this->NumBonds = 0;
  this->AtomAllocation = 0;
  this->BondAllocation = 0;

  for ( int i = 0; i < f.NumAtoms; i++)
    AddAtom(f.Atoms[i].Type, f.Atoms[i].Position);

  for ( int i = 0; i < f.NumBonds; i++)
    AddBond(f.Bonds[i].Atom1, f.Bonds[i].Atom2, f.Bonds[i].Order);

  return *this;
}
*/
//not needed currently

void Frame::Read(BufferFile * Buffer, long length) {
	if (length != (3*sizeof(double) + sizeof(float) +3*sizeof(long))) throw DataError();

	Buffer->Read((Ptr) &Energy, sizeof(double));
	Buffer->Read((Ptr) &KE, sizeof(double));
	Buffer->Read((Ptr) &MP2Energy, sizeof(double));
	Buffer->Read((Ptr) &time, sizeof(float));
	Buffer->Read((Ptr) &IRCPt, sizeof(long));
	Buffer->Read((Ptr) &NumAtoms, sizeof(long));
	Buffer->Read((Ptr) &NumBonds, sizeof(long));
	if (NumAtoms>0) {
		Atoms = new mpAtom[NumAtoms];
		if (!Atoms) throw MemoryError();
		AtomAllocation = NumAtoms;
	}
	if (NumBonds>0) {
		Bonds = new Bond[NumBonds];
		if (!Bonds) throw MemoryError();
		BondAllocation = NumBonds;
	}
}
void Frame::Read41(BufferFile * Buffer, long length) {
	if (length != 80) throw DataError();
	Buffer->Read((Ptr) &Energy, sizeof(double));
	Buffer->Read((Ptr) &KE, sizeof(double));
	Buffer->Read((Ptr) &MP2Energy, sizeof(double));
	Buffer->Read((Ptr) &time, sizeof(float));
	Buffer->Read((Ptr) &IRCPt, sizeof(long));
	Buffer->BufferSkip(8);
	Buffer->Read((Ptr) &NumAtoms, sizeof(long));
	Buffer->BufferSkip(4);
	Buffer->Read((Ptr) &NumBonds, sizeof(long));
	Buffer->BufferSkip(28);

	if (NumAtoms>0) {
		Atoms = new mpAtom[NumAtoms];
		if (!Atoms) throw MemoryError();
		AtomAllocation = NumAtoms;
	}
	if (NumBonds>0) {
		Bonds = new Bond[NumBonds];
		if (!Bonds) throw MemoryError();
		BondAllocation = NumBonds;
	}
	if (SpecialAtoms) {
		SpecialAtoms = new CPoint3D[NumAtoms];
		if (!SpecialAtoms) throw MemoryError();
	}
}
void Frame::ConvertCode3(BufferFile * Buffer, long length) {
	if (length != NumAtoms*sizeof(long)) throw DataError();
	long * temp = new long[NumAtoms];
	if (temp) {
		Buffer->Read((Ptr) temp, length);
		for (long i=0; i<NumAtoms; i++) {
			Atoms[i].Type = temp[i];
			Atoms[i].Highlite = 0;
		}
		delete [] temp;
	} else throw MemoryError();
}
void Frame::ConvertCode4(BufferFile * Buffer, long length) {
	if (length != NumAtoms*sizeof(CPoint3D)) throw DataError();
	CPoint3D * temp = new CPoint3D[NumAtoms];
	if (temp) {
		Buffer->Read((Ptr) temp, length);
		for (long i=0; i<NumAtoms; i++)
			Atoms[i].Position = temp[i];
		delete [] temp;
	} else throw MemoryError();
}
void Frame::ConvertCode5(BufferFile * Buffer, long length) {
	if (length != NumBonds*2*sizeof(long)) throw DataError();
	long * temp = new long[2*NumBonds];
	if (temp) {
		Buffer->Read((Ptr) temp, length);
		for (long i=0; i<NumBonds; i++) {
			Bonds[i].Atom1 = temp[2*i];
			Bonds[i].Atom2 = temp[2*i+1];
			Bonds[i].Order = kSingleBond;
			Bonds[i].Highlite = 0;
		}
		delete [] temp;
	} else throw MemoryError();
}
Frame::~Frame(void) {
	if (Atoms) delete [] Atoms;
	if (Bonds) delete [] Bonds;
	if (SpecialAtoms) delete [] SpecialAtoms;
	if (Vibs) delete Vibs;
	if (Orbs.size() > 0) {
		std::vector<OrbitalRec *>::const_iterator OrbSet = Orbs.begin();
		while (OrbSet != Orbs.end()) {
			delete (*OrbSet);
			OrbSet++;
		}
	}
	while (SurfaceList) {
		Surface * temp = SurfaceList->NextSurface;
		delete SurfaceList;
		SurfaceList = temp;
	}
	if (Gradient) delete Gradient;
	if (NextFrame) {
		if (PreviousFrame) {
			PreviousFrame->SetNextFrame(NextFrame);
			NextFrame->SetPreviousFrame(PreviousFrame);
		}
	} else if (PreviousFrame) {
		PreviousFrame->SetNextFrame(NULL);
	}
}
void Frame::DeleteOrbitals(void) {
	if (Orbs.size() > 0) {
		std::vector<OrbitalRec *>::const_iterator OrbSet = Orbs.begin();
		while (OrbSet != Orbs.end()) {
			delete (*OrbSet);
			OrbSet++;
		}
		Orbs.clear();
	}
}
void Frame::SetNextFrame(Frame * next) { NextFrame = next; }
void Frame::SetPreviousFrame(Frame * previous) { PreviousFrame = previous; }
Frame * Frame::GetNextFrame(void) { return NextFrame; }
Frame * Frame::GetPreviousFrame(void) { return PreviousFrame; }

mpAtom * Frame::AddAtom(long AtomType, CPoint3D AtomPosition) {
        mpAtom * result = NULL;
	if (NumAtoms>=AtomAllocation) IncreaseAtomAllocation(10);
	if (NumAtoms<AtomAllocation) {
	  Atoms[NumAtoms].Type = AtomType;
	  Atoms[NumAtoms].Position = AtomPosition;
	  Atoms[NumAtoms].Highlite = 0;
	  Atoms[NumAtoms].isMarked = false;
	  result = &Atoms[NumAtoms];
	  NumAtoms++;
	}
	return result;
}
bool Frame::IncreaseAtomAllocation(long NumAdditional) {
	if (AtomAllocation+NumAdditional < NumAtoms) return false;
 mpAtom * temp = new mpAtom[AtomAllocation+NumAdditional];
	if (temp) {
		if (Atoms != NULL) {
            // BlockMoveData is Mac only.
			//BlockMoveData(Atoms, temp, NumAtoms*sizeof(mpAtom));

            memcpy(temp, Atoms, NumAtoms*sizeof(mpAtom));
	    delete [] Atoms;
		}
		Atoms = temp;
		AtomAllocation += NumAdditional;
	} else return false;
	return true;
}
void Frame::ReadGradient(BufferFile * Buffer, long SearchLength) {
	long SavedPos = Buffer->GetFilePos(), npos;
	bool	Style=true;

	if (Buffer->LocateKeyWord(" NSERCH", 7, SearchLength)) {	//Search for gradient data at the end 
		Gradient = new GradientData;			//of a frame of data
		if (Gradient) {
			npos = Buffer->GetFilePos();
			if (Buffer->LocateKeyWord("COORDINATES (BOHR)                         GRADIENT (HARTREE/BOHR)",
					60, SearchLength)) {
				Style=false;
				Buffer->SetFilePos(npos);
			}
			if (!Gradient->ParseGAMESSGradient(Buffer, NumAtoms, SearchLength, Style)) {
				delete Gradient;
				Gradient = NULL;
			}
		}
	}

	Buffer->SetFilePos(SavedPos);	//reset position so other code isn't broken
}
float Frame::GetRMSGradient(void) const {
	float result=-1.0;
	if (Gradient) {
		result = Gradient->GetRMS();
	}
	return result;
}
float Frame::GetMaxGradient(void) const {
	float result=-1.0;
	if (Gradient) {
		result = Gradient->GetMaximum();
	}
	return result;
}
void Frame::SetRMSGradient(float v) {
	if (!Gradient) Gradient = new GradientData;
	if (Gradient) Gradient->SetRMS(v);
}
void Frame::SetMaximumGradient(float v) {
	if (!Gradient) Gradient = new GradientData;
	if (Gradient) Gradient->SetMaximum(v);
}
bool Frame::AddSpecialAtom(CPoint3D Vector, long AtomNum) {
	if (!SpecialAtoms) SpecialAtoms = new CPoint3D[AtomAllocation];
	if (!SpecialAtoms) return false;
	if (AtomNum >= AtomAllocation) return false;
	SpecialAtoms[AtomNum] = Vector;
	return true;
}
bool Frame::AddBond(long Atom1, long Atom2, const BondOrder & b) {
	bool result = false;
		//Validate the pair of atom references
	if ((Atom1>=0)&&(Atom2>=0)&&(Atom1!=Atom2)&&(Atom1<NumAtoms)&&(Atom2<NumAtoms)) {
		if (NumBonds == BondAllocation) IncreaseBondAllocation(10);
		if (NumBonds<BondAllocation) {
			Bonds[NumBonds].Atom1 = Atom1;
			Bonds[NumBonds].Atom2 = Atom2;
			Bonds[NumBonds].Order = b;
			Bonds[NumBonds].Highlite = 0;
			NumBonds++;
			result = true;
		}
	}
	return result;
}
bool Frame::GetBondLength(long atom1, long atom2, float * length) {
	bool Success = false;

	if ((atom1 >= 0)&&(atom2>=0)&&(atom1<NumAtoms)&&(atom2<NumAtoms)&&(atom1!=atom2)) {
		CPoint3D offset;
		offset.x = Atoms[atom1].Position.x - Atoms[atom2].Position.x;
		offset.y = Atoms[atom1].Position.y - Atoms[atom2].Position.y;
		offset.z = Atoms[atom1].Position.z - Atoms[atom2].Position.z;
		*length = offset.Magnitude();
		Success = true;
	}

	return Success;
}
bool Frame::GetBondAngle(long atom1, long atom2, long atom3, float * result) {
	bool	Success = false;

	if ((atom1 >= 0)&&(atom2>=0)&&(atom3>=0)&&(atom1<NumAtoms)&&(atom2<NumAtoms)&&
		(atom3<NumAtoms)&&(atom1!=atom2)&&(atom1!=atom3)&&(atom2!=atom3)) {

		CPoint3D offset;
		float	length1, length2, length3;
		offset.x = Atoms[atom2].Position.x - Atoms[atom1].Position.x;
		offset.y = Atoms[atom2].Position.y - Atoms[atom1].Position.y;
		offset.z = Atoms[atom2].Position.z - Atoms[atom1].Position.z;
		length1 = offset.Magnitude();
		offset.x = Atoms[atom1].Position.x - Atoms[atom3].Position.x;
		offset.y = Atoms[atom1].Position.y - Atoms[atom3].Position.y;
		offset.z = Atoms[atom1].Position.z - Atoms[atom3].Position.z;
		length3 = offset.Magnitude();
		offset.x = Atoms[atom3].Position.x - Atoms[atom2].Position.x;
		offset.y = Atoms[atom3].Position.y - Atoms[atom2].Position.y;
		offset.z = Atoms[atom3].Position.z - Atoms[atom2].Position.z;
		length2 = offset.Magnitude();
		if ((fabs(length1)>0.0001)&&(fabs(length3)>0.0001)&&(fabs(length2)>0.0001)) {
			*result = acos(((length1*length1+length2*length2-length3*length3)/
								(2*length1*length2)));
			*result *= kRadToDegree;
		} else *result = 0.0;
		Success = true;
	}
	return Success;
}
float Frame::GetBondLength(long ibond) {
	CPoint3D offset;
	long atom1 = Bonds[ibond].Atom1;
	long atom2 = Bonds[ibond].Atom2;

	offset.x = Atoms[atom1].Position.x - Atoms[atom2].Position.x;
	offset.y = Atoms[atom1].Position.y - Atoms[atom2].Position.y;
	offset.z = Atoms[atom1].Position.z - Atoms[atom2].Position.z;

	return offset.Magnitude();
}
void Frame::ChangeBond(long theBond, short whichPart, long theAtom) {
	if ((theBond >= 0)&&(theBond < NumBonds)&&(theAtom>=0)&&(theAtom<NumAtoms)) {
		if (whichPart == 1) Bonds[theBond].Atom1 = theAtom;
		else if (whichPart == 2) Bonds[theBond].Atom2 = theAtom;
	}
}
long Frame::GetBondAtom(long theBond, short thePart) {
	if ((theBond >= 0)&&(theBond < NumBonds)) {
		if (thePart == 1) return Bonds[theBond].Atom1;
		else if (thePart == 2) return Bonds[theBond].Atom2;
	}
	return -1;
}
void Frame::DeleteBond(long BondNum) {
	if (BondNum < NumBonds) {
		NumBonds--;
		for (long i=BondNum; i<NumBonds; i++) {
			Bonds[i] = Bonds[i+1];
		}
	}
}
bool Frame::IncreaseBondAllocation(long NumAdditional) {
	if (BondAllocation+NumAdditional < NumBonds) return false;
	Bond * temp = new Bond[BondAllocation+NumAdditional];
	if (temp) {
		if (Bonds != NULL) {
            // BlockMoveData is Mac only.
			//BlockMoveData(Bonds, temp, NumBonds*sizeof(Bond));
            memcpy(temp, Bonds, NumBonds*sizeof(Bond));
			delete [] Bonds;
		}
		Bonds = temp;
		BondAllocation += NumAdditional;
	} else return false;
	return true;
}
void Frame::DeleteAtom(long AtomNum) {	//remove the atom and pull down any higher atoms
	if ((AtomNum>=0)&&(AtomNum<NumAtoms)) {
		if ((AtomNum<(NumAtoms-1))&&(NumAtoms>1))
			// BlockMoveData is Mac only
			//BlockMoveData(&(Atoms[AtomNum+1]), &(Atoms[AtomNum]), (NumAtoms-AtomNum)*sizeof(mpAtom));
			memcpy(&(Atoms[AtomNum]), &(Atoms[AtomNum+1]), (NumAtoms-AtomNum)*sizeof(mpAtom));
		NumAtoms--;
		//remove this atom from the bond list
			
		for (long ii=0; ii<NumBonds; ii++) {
			if ((Bonds[ii].Atom1==AtomNum)||(Bonds[ii].Atom2==AtomNum)) {
				DeleteBond(ii);
				ii--;
			} else {
				if (Bonds[ii].Atom1>AtomNum) Bonds[ii].Atom1 --;
				if (Bonds[ii].Atom2>AtomNum) Bonds[ii].Atom2 --;
			}
		}	//Delete any orbitals and normal modes

		if (Vibs) {
			delete Vibs;
			Vibs = NULL;
		}
		DeleteOrbitals();
		while (SurfaceList) DeleteSurface(0);
	}
}
bool Frame::SetAtomType(long theAtom, short atmType) {
	bool result = false;
	if ((theAtom>=0)&&(theAtom<NumAtoms)) {
		result = Atoms[theAtom].SetType(atmType);
	}
	return result;
}
short Frame::GetAtomType(long iatom) const {
	short	result=0;
	if (Atoms&&(iatom>=0)&&(iatom<NumAtoms)) result = Atoms[iatom].GetType();
	return result;
}
bool Frame::GetAtomPosition(long iatom, CPoint3D & p) const {
	bool	result=false;
	if (Atoms&&(iatom>=0)&&(iatom<NumAtoms)) {
		result = true;
		p = Atoms[iatom].Position;
	}
	return result;
}
bool Frame::SetAtomPosition(long theAtom, const CPoint3D & pos) {
	bool result = false;
	
	if ((theAtom>=0)&&(theAtom<NumAtoms)) {
		Atoms[theAtom].Position = pos;
		result = true;
	}
	return result;
}
long Frame::GetNumElectrons(void) const {
	long	result=0;
	for (long i=0; i<NumAtoms; i++) result += Atoms[i].Type;
	return result;
};

void Frame::SetBonds(WinPrefs * Prefs, bool KeepOldBonds, bool selectedOnly)
{	long		iatm, jatm, maxbonds;
	CPoint3D	offset;
	float		distance, AutoDist=-1.0;
	bool		newBond=true;

	maxbonds = NumAtoms*8;
	Bond * OldBonds = Bonds;
	long NumOldBonds = NumBonds;
	long OldBondAllocation = BondAllocation;
	Bonds = new Bond[maxbonds];
	if (Bonds == NULL) {
		MessageAlert("Insufficient Memory for Set Bond\nLength operation. Old Bonds left untouched.");
		Bonds = OldBonds;
		return;
	}
	NumBonds = 0;
	BondAllocation = maxbonds;
	if (KeepOldBonds) {
		//Here we copy the old bond array over preserving the existing bonds
		NumBonds = 0;
		for (long ibond=0; ibond<NumOldBonds; ibond++) {
			if (selectedOnly) {
				//In selectedOnly mode we do not copy bonds with selected atoms
				if (!Atoms[Bonds[ibond].Atom1].GetSelectState()
					&& !Atoms[Bonds[ibond].Atom2].GetSelectState()) {
					Bonds[NumBonds] = OldBonds[ibond];
					NumBonds++;
				}
			} else {
				Bonds[NumBonds] = OldBonds[ibond];
				NumBonds++;
			}
		}
	}
	float AutoScale = Prefs->GetAutoBondScale();
	bool AutoBond = Prefs->GetAutoBond();
	bool HHBondFlag = Prefs->GetHHBondFlag();
	bool GuessBondOrder = Prefs->DetermineBondOrder();
	float MaxBondLength = Prefs->GetMaxBondLength();
	BondOrder lOrder;
	long iType, jType;
	for (iatm=0; iatm<NumAtoms; iatm++) {
		iType = Atoms[iatm].GetType();
		if (iType > 115) continue;
		for (jatm=iatm+1; jatm<NumAtoms; jatm++) {
			if (selectedOnly && !Atoms[iatm].GetSelectState() && !Atoms[jatm].GetSelectState())
				continue;
			jType = Atoms[jatm].GetType();
			if (HHBondFlag)	/*if both atoms are H's don't allow bonds if HHBondFlag is set*/
				if ((iType == 1)&&(jType == 1)) continue;
			if (jType > 115) continue;
			offset.x = Atoms[iatm].Position.x - Atoms[jatm].Position.x;
			offset.y = Atoms[iatm].Position.y - Atoms[jatm].Position.y;
			offset.z = Atoms[iatm].Position.z - Atoms[jatm].Position.z;
			distance = offset.Magnitude();
			newBond = false;
			if (distance <= MaxBondLength) {
				newBond = true;
				lOrder = kSingleBond;
			}
			if (AutoBond && !newBond) {
				AutoDist = AutoScale*((float) (Prefs->GetAtomSize(iType-1) + 
					Prefs->GetAtomSize(jType-1)));
				if (distance <= AutoDist) {
					newBond = true;
					lOrder = kSingleBond;
				}
				if (newBond && GuessBondOrder) {//See if this might qualify as a multiple bond
					if ((iType != 1) && (jType != 1)) {
						if (distance <= (.725 * AutoDist))
							lOrder = kTripleBond;
						else if (distance <= (0.80 * AutoDist))
							lOrder = kDoubleBond;
					}
				}
			}
			if (newBond) {
				if (KeepOldBonds) {
					if (BondExists(iatm, jatm) >= 0) newBond = false;
				}
				if (newBond) {
					if (Atoms[iatm].IsSIMOMMAtom() != Atoms[jatm].IsSIMOMMAtom())
						continue;	//bonds are not allowed between SIMOMM and ab intio atoms
					if (! AddBond(iatm, jatm, lOrder)) {
						MessageAlert("Insufficient Memory for Set Bond\nLength operation. Old Bonds left untouched.");
						delete [] Bonds;
						Bonds = OldBonds;
						NumBonds = NumOldBonds;
						BondAllocation = OldBondAllocation;
						return;
					}
				}
			}
		}
	}
	if (AutoBond && Prefs->AllowHydrogenBonds()) {
		for (iatm=0; iatm<NumAtoms; iatm++) {
			iType = Atoms[iatm].GetType();
			//only consider H bonds with N, O, F, P, S, Cl, Se, and Br
			if (!((iType==1)||((iType>=7)&&(iType<=9))||((iType>=15)&&(iType<=17))||
				  (iType==34)||(iType==35))) continue;
			for (jatm=iatm+1; jatm<NumAtoms; jatm++) {
				if (selectedOnly && !Atoms[iatm].GetSelectState() && !Atoms[jatm].GetSelectState())
					continue;
				jType = Atoms[jatm].GetType();
				if (!((jType==1)||((jType>=7)&&(jType<=9))||((jType>=15)&&(jType<=17))||
					  (jType==34)||(jType==35))) continue;
				if (((iType == 1)&&(jType != 1)) ||
					((iType != 1)&&(jType == 1))) {//is one atom a hydrogen?
					if (BondExists(iatm, jatm) < 0) {	//can't be an existing bond
						newBond = false;
						offset.x = Atoms[iatm].Position.x - Atoms[jatm].Position.x;
						offset.y = Atoms[iatm].Position.y - Atoms[jatm].Position.y;
						offset.z = Atoms[iatm].Position.z - Atoms[jatm].Position.z;
						distance = offset.Magnitude();
						AutoDist = AutoScale*((float) (Prefs->GetAtomSize(iType-1) + 
							Prefs->GetAtomSize(jType-1)));
						long HBonder;
						if (iType == 1) HBonder = jType;
						else HBonder = iType;
						float testDistance = 1.6*AutoDist;
						if (distance <= testDistance) newBond = true;
						
		/*				if (HBonder==7 || HBonder == 15) {
							if (distance <= 1.0+AutoDist) {
								newBond = true;
								lOrder = kHydrogenBond;
							}
						} else if (distance <= (1.3 + 1.5*AutoDist)) {
							//distance criteria is met, see if the other atom is a
							//reasonable hydrogen bonder
							if (((HBonder>=8)&&(HBonder<=9))||((HBonder>=16)&&
								(HBonder<=17))||((HBonder>=34)&&(HBonder<=35))) {
								newBond = true;
								lOrder = kHydrogenBond;
							}
						}
		*/
						if (newBond) {
							//scan the bond list, prevent h-bonds between any pair of
							//atoms which are bonded to the same atom
							for (long i=0; i<NumAtoms; i++) {
								long ib = BondExists(i, iatm);
								//Don't filter on preexisting hydrogen bonds
								if (ib >= 0)
									if (Bonds[ib].Order == kHydrogenBond) ib = -1;
								long jb = BondExists(i,jatm);
								if (jb >= 0)
									if (Bonds[jb].Order == kHydrogenBond) jb = -1;
								if ((ib>=0) && (jb>=0)) {
									//Both atoms have an existing single (or higher) bond
									//and it thus doesn't make sense for them to have an H bond
									newBond = false;
									break;
		/*						} else if ((ib>=0) || (jb>=0)) {
									long nBondedAtom = iatm;
									if (ib>=0) nBondedAtom = jatm;
									for (long j=0; j<NumAtoms; j++) {
										ib = BondExists(i, j);
										if (ib >= 0)
											if (Bonds[ib].Order == kHydrogenBond) ib = -1;
										jb = BondExists(j,nBondedAtom);
										if (jb >= 0)
											if (Bonds[jb].Order == kHydrogenBond) jb = -1;
										if ((ib>=0)&&(jb>=0)) {
											newBond = false;
											break;
										}
									}
			*/
								}
								if (!newBond) break;
							}
							if (newBond) {
								if (! AddBond(iatm, jatm, kHydrogenBond)) {
									MessageAlert("Insufficient Memory for Set Bond\nLength operation. Old Bonds left untouched.");
									delete [] Bonds;
									Bonds = OldBonds;
									NumBonds = NumOldBonds;
									BondAllocation = OldBondAllocation;
									return;
								}
							}
						}
					}
				}
			}
		}
	}
	if (OldBonds) delete [] OldBonds;
	if (BondAllocation > NumBonds+50) {
		Bond * temp = new Bond[NumBonds+5];
		maxbonds = NumBonds+5;
		if (temp) {
            memcpy(temp, Bonds, NumBonds*sizeof(Bond));
			delete [] Bonds;
			Bonds = temp;
			BondAllocation = maxbonds;
		}
	}
} /* SetBonds */
long Frame::BondExists(long a1, long a2) const {
	long result = -1;

	for (long i=0; i<NumBonds; i++) {
		if ((Bonds[i].Atom1 == a1 && Bonds[i].Atom2 == a2) ||
			(Bonds[i].Atom1 == a2 && Bonds[i].Atom2 == a1)) {
				result = i;
				break;
		}
	}
	return result;
}

void ReadGAMESSlogVectors(BufferFile * Buffer, float *Vectors, long MaxFuncs, long NumFuncs);

void Frame::ParseGAMESSGuessVectors(BufferFile * Buffer, long NumFuncs, TypeOfWavefunction t,
									Progress * lProgress) {
	//first we need to parse off the separate list of occupancies, then try
	//using the normal eigenvector routine to parse the rest.
	//What is the UHF behavior? There are a couple of problems. First UHF needs to be handled
	//also ROHF printout seems broke since it prints alpha and beta orbitals separately
	//finally the MCSCF guess appears to have all zero occupation #'s.
	float * Occupancies=NULL;
	char	Line[kMaxLineLength+1];
	if (Buffer->LocateKeyWord("ASSIGNED OCCUPANCIES", 20)) {
		Buffer->SkipnLines(2);
		Occupancies = new float[NumFuncs];
		for (int i=0; i<NumFuncs; i++) Occupancies[i] = 0.0;
		int NumOrbs = NumFuncs;
		int iorb = 0;
		while (iorb < NumOrbs) {
			int imaxorb = MIN(10, NumOrbs-iorb);	//Max of 10 orbitals per line
			Buffer->GetLine(Line);
			int LinePos = 0;
			for (int jorb=0; jorb<imaxorb; jorb++) {
				int nChar;
				int ScanErr = sscanf(&(Line[LinePos]), "%f%n", &(Occupancies[iorb]), &nChar);
				if (ScanErr==1) LinePos += nChar;
				else {
					imaxorb = jorb;
					if (jorb==0) {	//No more orbitals found
						imaxorb = 0;
						NumOrbs = iorb;
					}
					break;
				}
				iorb++;
			}
			if (imaxorb <= 0) break;
		}
		NumOrbs = iorb;
		Buffer->BackupnLines(1);
		Buffer->SetFilePos(Buffer->FindBlankLine());
		Buffer->SkipnLines(1);
		if (NumOrbs > 0) {
			OrbitalRec * orbs = ParseGAMESSEigenVectors(Buffer, NumFuncs, NumOrbs,
									/*long NumBetaOrbs*/ 0, NumOrbs, 0,
									t, lProgress);
			if (orbs) {
				orbs->SetOccupancy(Occupancies, NumFuncs);
				orbs->setOrbitalType(GuessOrbital);
			}
		}
	}
}
//Handle MCSCF vectors (Natural and Optimized)
void Frame::ParseGAMESSMCSCFVectors(BufferFile * Buffer, long NumFuncs,
		long NumOrbs, Progress * lProgress) {
	long	iorb, imaxorb=0, NumNOrbs=0, NumOptOrbs=0, TestOrb,
			ScanErr, LinePos, jorb;
	int		nChar;
	float	*Vectors, *Energy, *OccNums;
	char	*SymType, Line[kMaxLineLength+1];

	long StartPos = Buffer->GetFilePos(), NOrbPos, OptOrbPos;
		//Look for the MCSCF Natural Orbitals, really should only read them in
		//for FORS type MCSCF functions
	if (Buffer->LocateKeyWord("MCSCF NATURAL ORBITALS", 22)) {
		NOrbPos = Buffer->GetFilePos();
		imaxorb =1;
		NumNOrbs = NumOrbs;
	} else if (Buffer->LocateKeyWord("-MCHF- NATURAL ORBITALS", 23)) {
		NOrbPos = Buffer->GetFilePos();
		imaxorb =1;
		NumNOrbs = NumOrbs;
	}
	Buffer->SetFilePos(StartPos);
	if (Buffer->LocateKeyWord("MCSCF OPTIMIZED ORBITALS", 24)) {
		OptOrbPos = Buffer->GetFilePos();
		imaxorb++;
		NumOptOrbs = NumFuncs;	//GAMESS now seems to print all optimized orbs
	} else if (Buffer->LocateKeyWord("-MCHF- OPTIMIZED ORBITALS", 25)) {
		OptOrbPos = Buffer->GetFilePos();
		imaxorb++;
		NumOptOrbs = NumFuncs;	//GAMESS now seems to print all optimized orbs
	}
	Buffer->SetFilePos(StartPos);
	if (imaxorb==0) return;	//No MC orbital vectors found

	long maxfuncs = NumFuncs;	//Always true
	OrbitalRec * OrbSet = NULL;
	
//	Orbs = new MORec;
//	if (Orbs==NULL) throw MemoryError();
//	Orbs->EigenVectors = new OrbitalRec(NumNOrbs, NumOptOrbs, maxfuncs);
//	if (Orbs->EigenVectors==NULL) throw MemoryError();
//	Orbs->EigenVectors->OrbType=MCSCF;
	try {
		if (NumNOrbs) {	//read in the Natural Orbitals
			OrbSet = new OrbitalRec(NumNOrbs, 0, maxfuncs);
			OrbSet->setOrbitalWavefunctionType(MCSCF);
			OrbSet->setOrbitalType(NaturalOrbital);
			Vectors = OrbSet->Vectors;
			Energy = OrbSet->Energy;
			if (OrbSet->SymType) {	//symmetry labels are not printed for NO's
				delete [] OrbSet->SymType;
				OrbSet->SymType = NULL;
			}
			OccNums = OrbSet->OrbOccupation = new float [NumNOrbs];
			if (!OccNums) throw MemoryError();
				//Clear the occupation numbers just in case a full set is not read in
			for (iorb=0; iorb<NumNOrbs; iorb++) OccNums[iorb] = 0.0;

			Buffer->SetFilePos(NOrbPos);
			Buffer->SkipnLines(3);
			iorb=0;
			while (iorb<NumNOrbs) {
					// Allow a little backgrounding and user cancels
				if (!lProgress->UpdateProgress(Buffer->GetPercentRead())) {
					delete OrbSet;
					return;
				}
	//			imaxorb = MIN(10, NumNOrbs-iorb);	//Max of 10 orbitals per line
				imaxorb = ((10) > (NumNOrbs-iorb)) ? (NumNOrbs-iorb) : (10);	//Max of 10 orbitals per line
				Buffer->GetLine(Line);
				LinePos = 0;
				for (jorb=0; jorb<imaxorb; jorb++) {
					ScanErr = sscanf(&(Line[LinePos]), "%ld%n", &TestOrb, &nChar);
					if (ScanErr && (TestOrb==jorb+iorb+1)) LinePos += nChar;
					else {
						imaxorb = jorb;
						if (jorb==0) {	//No more orbitals found
							imaxorb = 0;
							NumNOrbs = iorb;
						}
						break;
					}
				}
				if (imaxorb <= 0) break;
				Buffer->SkipnLines(1);	//Skip blank line
					//first read in the orbital energy/occupation number of each orbital
				Buffer->GetLine(Line);
				LinePos = 0;
				for (jorb=0; jorb<imaxorb; jorb++) {//Grab the orbital energies
					ScanErr = sscanf(&(Line[LinePos]), "%f%n", &(Energy[iorb+jorb]),&nChar);
					if (ScanErr<=0) throw DataError();	//Looks like the MO's are not complete
					if (Energy[iorb+jorb] >= 0.0) {	//Only core orbital energies are printed
						if (OccNums) OccNums[iorb+jorb] = Energy[iorb+jorb];
						Energy[iorb+jorb] = 0.0;
					} else if (OccNums) OccNums[iorb+jorb] = 2.0;
					LinePos+=nChar;		//nChar contains the number of char's read by sscanf including spaces
				}
				Buffer->SkipnLines(1);	//skip blank line
					//orbital symetries are not printed for Natural orbitals
					//read in the vector block
				ReadGAMESSlogVectors(Buffer, &(Vectors[iorb*maxfuncs]), maxfuncs, imaxorb);
				iorb += imaxorb;
				Buffer->SkipnLines(1);	//Skip blank line between blocks
			}
			OrbSet->ReSize(NumNOrbs, 0);
		}
	}
	catch (...) {
		if (OrbSet != NULL) {
			delete OrbSet;
			OrbSet = NULL;
		}
	}
	if (OrbSet) {
		OrbSet->SetOrbitalOccupancy(NumNOrbs, 0);	//We should know the occupation # for all natural orbs.
		Orbs.push_back(OrbSet);
		OrbSet = NULL;
	}
	try {
		if (NumOptOrbs) {
			OrbSet = new OrbitalRec(NumOptOrbs, 0, maxfuncs);
			Vectors = OrbSet->Vectors;
			Energy = OrbSet->Energy;
			SymType = OrbSet->SymType;
			OrbSet->setOrbitalWavefunctionType(MCSCF);
			OrbSet->setOrbitalType(OptimizedOrbital);

			Buffer->SetFilePos(OptOrbPos);	//move to the pre-determined start of the optimized orbitals
			Buffer->SkipnLines(3);
			iorb=0;
			while (iorb<NumOptOrbs) {
					// Allow a little backgrounding and user cancels
				if (!lProgress->UpdateProgress(Buffer->GetPercentRead())) {
					delete OrbSet;
					return;
				}
	//			imaxorb = MIN(10, NumOptOrbs-iorb);	//Max of 10 orbitals per line
				imaxorb = ((10) > (NumOptOrbs-iorb)) ? (NumOptOrbs-iorb) : (10);	//Max of 10 orbitals per line
				Buffer->GetLine(Line);
				LinePos = 0;
				for (jorb=0; jorb<imaxorb; jorb++) {
					ScanErr = sscanf(&(Line[LinePos]), "%ld%n", &TestOrb, &nChar);
					if (ScanErr && (TestOrb==jorb+iorb+1)) LinePos += nChar;
					else {
						imaxorb = jorb;
						if (jorb==0) {	//No more orbitals found
							imaxorb = 0;
							NumOptOrbs = iorb;
						}
						break;
					}
				}
				if (imaxorb <= 0) break;
					//first read in the orbital energy/occupation number of each orbital
				Buffer->GetLine(Line);
				LinePos = 0;
				for (jorb=0; jorb<imaxorb; jorb++) {//Grab the orbital energies
					ScanErr = sscanf(&(Line[LinePos]), "%f%n", &(Energy[iorb+jorb]),&nChar);
					if (ScanErr<=0) throw DataError();	//Looks like the MO's are not complete
					LinePos+=nChar;		//nChar contains the number of char's read by sscanf including spaces
				}
				Buffer->GetLine(Line);
				LinePos = 0;
				for (jorb=0; jorb<imaxorb; jorb++) {	//Get the orbital symmetries
					ScanErr = sscanf(&(Line[LinePos]), "%4s%n", &(SymType[(iorb+jorb)*5]),&nChar);
					if (ScanErr<=0) throw DataError();	//Looks like the MO's are not complete
					LinePos+=nChar;		//nChar contains the number of char's read by sscanf including spaces
				}
					//read in the vector block
				ReadGAMESSlogVectors(Buffer, &(Vectors[iorb*maxfuncs]), maxfuncs, imaxorb);
				iorb += imaxorb;
				Buffer->SkipnLines(1);	//Skip blank line between blocks
			}
			OrbSet->ReSize(NumOptOrbs, 0);
		}
	}
	catch (...) {
		if (OrbSet != NULL) {
			delete OrbSet;
			OrbSet = NULL;
		}
	}
	if (OrbSet) {
		Orbs.push_back(OrbSet);
	}
}

//Reads in a general set of eigenvectors from a GAMESS log file. The number of beta
//orbitals is used only to indicate the need to read in both alpha and beta sets.
//It is possible that there will be fewer orbitals than NumOrbs. If so then this
//routine will (hopefully) recognize that and truncate the read.
OrbitalRec * Frame::ParseGAMESSEigenVectors(BufferFile * Buffer, long NumFuncs, long NumOrbs,
		long NumBetaOrbs, const long & NumOccAlpha, const long & NumOccBeta,
		const TypeOfWavefunction & method, Progress * lProgress) {
	long	iorb=0, imaxorb, maxfuncs, TestOrb, LinePos, ScanErr, jorb;
	int		nChar;
	float	*Vectors, *Energy;
	char	*SymType, Line[kMaxLineLength+1];

	maxfuncs = NumFuncs;	//Always true
	OrbitalRec * OrbSet = NULL;
	try {
		OrbSet = new OrbitalRec(NumOrbs, (NumBetaOrbs?NumOrbs:0), NumFuncs);
		Vectors = OrbSet->Vectors;
		Energy = OrbSet->Energy;
		SymType = OrbSet->SymType;
		OrbSet->setOrbitalWavefunctionType(RHF);
		OrbSet->setOrbitalType(OptimizedOrbital);

			//UHF orbs are not always organized in the same format
			//To simplify just search for the 1 denoting the first vector
		if (NumBetaOrbs) Buffer->LocateKeyWord("1", 1, -1);
		while (iorb<NumOrbs) {
				// Allow a little backgrounding and user cancels
			if (!lProgress->UpdateProgress(Buffer->GetPercentRead())) {
				delete OrbSet;
				return NULL;
			}
			imaxorb = MIN(10, NumOrbs-iorb);	//Max of 10 orbitals per line
			Buffer->GetLine(Line);
			LinePos = 0;
			for (jorb=0; jorb<imaxorb; jorb++) {
				ScanErr = sscanf(&(Line[LinePos]), "%ld%n", &TestOrb, &nChar);
				if (ScanErr && (TestOrb==jorb+iorb+1)) LinePos += nChar;
				else {
					imaxorb = jorb;
					if (jorb==0) {	//No more orbitals found
						imaxorb = 0;
						NumOrbs = iorb;
					}
					break;
				}
			}
			if (imaxorb > 0) {
					//read in energy and the symmetry of each orbital (ie. A1 A2 B1É)
				Buffer->GetLine(Line);
				if (strlen(Line) <= 0) Buffer->GetLine(Line);
				LinePos = 0;
				for (jorb=0; jorb<imaxorb; jorb++) {//Grab the orbital energies
					ScanErr = sscanf(&(Line[LinePos]), "%f%n", &(Energy[iorb+jorb]),&nChar);
					if (ScanErr <= 0) throw DataError();
					LinePos+=nChar;		//nChar contains the number of char's read by sscanf including spaces
				}
				Buffer->GetLine(Line);
				if ((strlen(Line) > 0)&&SymType) {
					LinePos = 0;
					for (jorb=0; jorb<imaxorb; jorb++) {	//Get the orbital symmetries
						ScanErr = sscanf(&(Line[LinePos]), "%4s%n", &(SymType[(iorb+jorb)*5]),&nChar);
						if (ScanErr <= 0) throw DataError();	//Looks like the MO's are not complete
						LinePos+=nChar;		//nChar contains the number of char's read by sscanf including spaces
					}
				} else if (SymType) {
					delete [] OrbSet->SymType;
					SymType = OrbSet->SymType = NULL;
				}
					//read in the vector block
				ReadGAMESSlogVectors(Buffer, &(Vectors[iorb*maxfuncs]), maxfuncs, imaxorb);
				iorb += imaxorb;
				Buffer->SkipnLines(1);	//Skip blank line between blocks
			}
		}
		OrbSet->ReSize(NumOrbs, (NumBetaOrbs?NumOrbs:0));
		if (NumBetaOrbs) {
			OrbSet->setOrbitalWavefunctionType(UHF);
			Vectors = OrbSet->VectorsB;
			Energy = OrbSet->EnergyB;
			SymType = OrbSet->SymTypeB;
			Buffer->BackupnLines(2);
			if (!Buffer->LocateKeyWord("BETA SET",8)) throw DataError();
				//UHF orbs are not always organized in the same format
				//To simplify just search for the 1 denoting the first vector
			Buffer->LocateKeyWord("1", 1, -1);
	//		Buffer->SkipnLines(6);
			iorb=0;

			while (iorb<NumOrbs) {
					// Allow a little backgrounding and user cancels
				if (!lProgress->UpdateProgress(Buffer->GetPercentRead())) {
					delete OrbSet;
					return NULL;
				}
				imaxorb = MIN(10, NumOrbs-iorb);	//Max of 10 orbitals per line
				Buffer->GetLine(Line);
				LinePos = 0;
				for (jorb=0; jorb<imaxorb; jorb++) {
					ScanErr = sscanf(&(Line[LinePos]), "%ld%n", &TestOrb, &nChar);
					if (ScanErr && (TestOrb==jorb+iorb+1)) LinePos += nChar;
					else {
						imaxorb = jorb;
						if (jorb==0) {	//No more orbitals found
							imaxorb = 0;
							NumOrbs = iorb;
						}
						break;
					}
				}
				if (imaxorb > 0) {
						//first read in and the symmetry of each orbital (ie. A1 A2 B1É)
					Buffer->GetLine(Line);
					if (strlen(Line) <= 0) Buffer->GetLine(Line);
					LinePos = 0;
					for (jorb=0; jorb<imaxorb; jorb++) {//Grab the orbital energies
						ScanErr = sscanf(&(Line[LinePos]), "%f%n", &(Energy[iorb+jorb]),&nChar);
						if (ScanErr<=0) throw DataError();	//Looks like the MO's are not complete
						LinePos+=nChar;		//nChar contains the number of char's read by sscanf including spaces
					}
					Buffer->GetLine(Line);
					if ((strlen(Line) > 0)&&SymType) {
						LinePos = 0;
						for (jorb=0; jorb<imaxorb; jorb++) {	//Get the orbital symmetries
							ScanErr = sscanf(&(Line[LinePos]), "%4s%n", &(SymType[(iorb+jorb)*5]),&nChar);
							if (ScanErr<=0) throw DataError();	//Looks like the MO's are not complete
							LinePos+=nChar;		//nChar contains the number of char's read by sscanf including spaces
						}
					} else if (SymType) {
						delete [] OrbSet->SymTypeB;
						SymType = OrbSet->SymTypeB = NULL;
					}
						//read in the vector block
					ReadGAMESSlogVectors(Buffer, &(Vectors[iorb*maxfuncs]), maxfuncs, imaxorb);
					iorb += imaxorb;
					Buffer->SkipnLines(1);	//Skip blank line between blocks
				}
			}
		}
	}
	catch (...) {
		if (OrbSet) {
			delete OrbSet;
			OrbSet = NULL;
		}
	}
	if (OrbSet != NULL) {
		OrbSet->SetOrbitalOccupancy(NumOccAlpha, NumOccBeta);
		OrbSet->setOrbitalWavefunctionType(method);
		Orbs.push_back(OrbSet);
	}
	return OrbSet;
}
void Frame::ReadMolDenOrbitals(BufferFile * Buffer, long NumFuncs) {
	//We don't have a lot of information about the orbitals so assume the largest case
	
	OrbitalRec * OrbSet = NULL;
	try {
		//There doesn't seem to be a good way to predetermine the number of orbitals so dimension a 
		//worse case
		OrbSet = new OrbitalRec(NumFuncs, NumFuncs, NumFuncs);
		char	Line[kMaxLineLength+1];
		bool	done=false;
		long alphaCount=0, betaCount=0;
		
		while (!done && (Buffer->GetFilePos()<Buffer->GetFileSize())) {
			float * vector = NULL, energy=0.0, occ=-1;
			bool header=true, good=true, alphaSpin=true;
			char symLabel[5]="";
			while (header) {
				Buffer->GetLine(Line);
				if (FindKeyWord(Line,"[",1)>=0) {
					good = false;
					done = true;
					break;
				}
				int p;
				if ((p=FindKeyWord(Line, "ENE=", 4))>=0) {
					sscanf(&(Line[p+4]),"%f", &energy);
				} else if (FindKeyWord(Line, "SPIN=", 5)>=0) {
					if (FindKeyWord(Line, "BETA", 4)>=0) alphaSpin = false;
				} else if ((p=FindKeyWord(Line, "OCCUP=", 6))>=0) {
					sscanf(&(Line[p+6]),"%f", &occ);
				} else if ((p=FindKeyWord(Line, "SYM=", 4))>=0) {
					sscanf(&(Line[p+4]),"%4s", symLabel);
				} else {
					int test;
					float junk;
					if (sscanf(Line, "%d %f", &test, &junk) == 2 ) {//start of the vector
						Buffer->BackupnLines(1);
						header = false;
						break;
					}
				}
			}
			if (!good) break;
			if (alphaSpin) {
				vector = &(OrbSet->Vectors[alphaCount*NumFuncs]);
				OrbSet->Energy[alphaCount] = energy;
				if (occ > -1.0) {
					if (!(OrbSet->OrbOccupation)) {
						OrbSet->OrbOccupation = new float[NumFuncs];
					}
					OrbSet->OrbOccupation[alphaCount] = occ;
				}
				strncpy(&(OrbSet->SymType[alphaCount*5]), symLabel, 4);
			} else {
				vector = &(OrbSet->VectorsB[betaCount*NumFuncs]);
				OrbSet->EnergyB[betaCount] = energy;
				if (occ > -1.0) {
					if (!(OrbSet->OrbOccupationB)) {
						OrbSet->OrbOccupationB = new float[NumFuncs];
					}
					OrbSet->OrbOccupationB[betaCount] = occ;
				}
				strncpy(&(OrbSet->SymTypeB[betaCount*5]), symLabel, 4);
			}
			
			for (int i=0; i<NumFuncs; i++) {	//loop over the vector itself
				Buffer->GetLine(Line);
				int iline;
				float vec;
				if (sscanf(Line, "%d %f", &iline, &vec) == 2) {
					if (iline != (i+1)) throw DataError();
					vector[i] = vec;
				} else throw DataError();
			}
			if (alphaSpin) alphaCount++;
			else betaCount++;
		}
		
		OrbSet->NumAlphaOrbs = alphaCount;
		if (OrbSet->OrbOccupation) OrbSet->NumOccupiedAlphaOrbs = alphaCount;
		OrbSet->NumBetaOrbs = betaCount;
		if (OrbSet->OrbOccupationB) OrbSet->NumOccupiedBetaOrbs = betaCount;
	}
	catch (...) {
		if (OrbSet) {
			delete OrbSet;
			OrbSet=NULL;
		}
	}
	if (OrbSet) Orbs.push_back(OrbSet);
}
//Handle CI vectors
void Frame::ParseGAMESSCIVectors(BufferFile * Buffer, long NumFuncs, Progress * lProgress) {
	long	iorb, jorb, imaxorb=0, NumNOrbs=0, TestOrb,
			ScanErr, LinePos;
	int		nChar;
	float	*Vectors, *OccNums;
	char	Line[kMaxLineLength+1], *SymType=NULL;
	OrbitalRec * OrbSet = NULL;

	if (Buffer->LocateKeyWord("NATURAL ORBITALS", 16)) {
		try {
			Buffer->SkipnLines(3); //setup to beginning of the first block
			bool	Style = false;
			long begPos = Buffer->GetFilePos();
				//Determine the style of the orbital output
			Buffer->SkipnLines(1);
			Buffer->GetLine(Line);
			int t = strlen(Line);
			if (t > 5) Style=true;
			Buffer->SetFilePos(begPos);

			OrbSet = new OrbitalRec(NumFuncs, 0, NumFuncs);
			OrbSet->setOrbitalWavefunctionType(CI);
			OrbSet->setOrbitalType(NaturalOrbital);

			Vectors = OrbSet->Vectors;
			if (OrbSet->Energy) {
				delete [] OrbSet->Energy;
				OrbSet->Energy = NULL;
			}
			if (Style) {
				if (OrbSet->SymType == NULL) {
					OrbSet->SymType = new char [NumFuncs*5];
					if (!OrbSet->SymType) throw MemoryError();
				}
				SymType = OrbSet->SymType;
			} else {
				if (OrbSet->SymType) {	//symmetry labels are not printed for NO's
					delete [] OrbSet->SymType;
					OrbSet->SymType = NULL;
				}
			}
			OccNums = OrbSet->OrbOccupation = new float [NumFuncs];
			if (!OccNums) throw MemoryError();

			iorb=0;
			NumNOrbs = NumFuncs;
			while (iorb<NumNOrbs) {
					// Allow a little backgrounding and user cancels
				if (!lProgress->UpdateProgress(Buffer->GetPercentRead())) {
					delete OrbSet;
					return;
				}
				imaxorb = MIN(10, NumNOrbs-iorb);	//Max of 10 orbitals per line
				Buffer->GetLine(Line);
				LinePos = 0;
				for (jorb=0; jorb<imaxorb; jorb++) {
					ScanErr = sscanf(&(Line[LinePos]), "%ld%n", &TestOrb, &nChar);
					if (ScanErr && (TestOrb==jorb+iorb+1)) LinePos += nChar;
					else {
						imaxorb = jorb;
						if (jorb==0) {	//No more orbitals found
							imaxorb = 0;
							NumNOrbs = iorb;
						}
						break;
					}
				}
				if (imaxorb <= 0) break;
				if (!Style) Buffer->SkipnLines(1);	//Skip blank line
					//first read in the orbital occupation number of each orbital
				Buffer->GetLine(Line);
				LinePos = 0;
				for (jorb=0; jorb<imaxorb; jorb++) {//Grab the orbital occupations
					ScanErr = sscanf(&(Line[LinePos]), "%f%n", &(OccNums[iorb+jorb]),&nChar);
					if (ScanErr<=0) throw DataError();	//Looks like the MO's are not complete
					LinePos+=nChar;		//nChar contains the number of char's read by sscanf including spaces
				}
				if (Style) {	//New style has orbital symmetries
					Buffer->GetLine(Line);
					if (strlen(Line) > 0) {
						LinePos = 0;
						for (jorb=0; jorb<imaxorb; jorb++) {	//Get the orbital symmetries
							ScanErr = sscanf(&(Line[LinePos]), "%4s%n", &(SymType[(iorb+jorb)*5]),&nChar);
							if (ScanErr<=0) throw DataError();	//Looks like the MO's are not complete
							LinePos+=nChar;		//nChar contains the number of char's read by sscanf including spaces
						}
					}
				} else	//old style has blank line
					Buffer->SkipnLines(1);	//skip blank line
					//orbital symetries are not printed for Natural orbitals
					//read in the vector block
				ReadGAMESSlogVectors(Buffer, &(Vectors[iorb*NumFuncs]), NumFuncs, imaxorb);
				iorb += imaxorb;
				Buffer->SkipnLines(1);	//Skip blank line between blocks
			}
		}
		catch (...) {
			if (OrbSet) {
				delete OrbSet;
				OrbSet=NULL;
			}
		}
		OrbSet->NumAlphaOrbs = NumNOrbs;
		OrbSet->NumOccupiedAlphaOrbs = NumNOrbs;
		if (OrbSet) Orbs.push_back(OrbSet);
	}
}
//Parse a set of Localized MOs from a GAMESS log file. Note that
//there is no symmetry or orbital energy information available.
OrbitalRec * Frame::ParseGAMESSLMOs(BufferFile * Buffer, long NumFuncs, long NumAlphaOrbs,
		long NumOccBeta, Progress * lProgress, bool OrientedSet) {
	long	iorb=0, imaxorb, maxfuncs, TestOrb, LinePos, ScanErr;
	int		nChar;
	float	*Vectors;
	char	Line[150];

	maxfuncs = NumFuncs;	//Always true
	if (NumAlphaOrbs <= 0) throw DataError();
	OrbitalRec * TargetSet = NULL;
	try {
		TargetSet = new OrbitalRec(NumAlphaOrbs, NumOccBeta, NumFuncs);
		if (!OrientedSet) {
			TargetSet->setOrbitalType(LocalizedOrbital);
		} else {
			TargetSet->setOrbitalType(OrientedLocalizedOrbital);
		}
	//	TargetSet->setOrbitalWavefunctionType(RHF);
		Vectors = TargetSet->Vectors;
		if (TargetSet->Energy) delete [] TargetSet->Energy;
		TargetSet->Energy = NULL;
		if (TargetSet->SymType) delete [] TargetSet->SymType;
		TargetSet->SymType = NULL;
		if (NumOccBeta) {
			if (TargetSet->EnergyB) delete [] TargetSet->EnergyB;
			TargetSet->EnergyB = NULL;
			if (TargetSet->SymTypeB) delete [] TargetSet->SymTypeB;
			TargetSet->SymTypeB = NULL;
		}

		while (iorb<NumAlphaOrbs) {
				// Allow a little backgrounding and user cancels
			if (!lProgress->UpdateProgress(Buffer->GetPercentRead())) {
				delete TargetSet;
				return NULL;
			}
			imaxorb = MIN(10, NumAlphaOrbs-iorb);	//Max of 10 orbitals per line
			Buffer->GetLine(Line);
			LinePos = 0;
			for (long jorb=0; jorb<imaxorb; jorb++) {
				ScanErr = sscanf(&(Line[LinePos]), "%ld%n", &TestOrb, &nChar);
				if (ScanErr && (TestOrb==jorb+iorb+1)) LinePos += nChar;
				else {
					imaxorb = jorb;
					if (jorb==0) {	//No more orbitals found
						imaxorb = 0;
						NumAlphaOrbs = iorb;
					}
					break;
				}
			}
			if (imaxorb > 0) {
				Buffer->SkipnLines(1);	//Skip blank line
					//read in the vector block
				ReadGAMESSlogVectors(Buffer, &(Vectors[iorb*maxfuncs]), maxfuncs, imaxorb);
				iorb += imaxorb;
				Buffer->SkipnLines(1);	//Skip blank line between blocks
			}
		}
		if (NumOccBeta) {
			TargetSet->setOrbitalWavefunctionType(UHF);
			Vectors = TargetSet->VectorsB;
				//GAMESS now punchs out the Fock operator for ruedenburg type localization
				//which I need to skip if it is present
			if (Buffer->LocateKeyWord("FOCK OPERATOR FOR THE LOCALIZED ORBITALS",40))
				Buffer->SkipnLines(1);
			if (!Buffer->LocateKeyWord("LOCALIZED ORBITALS",18)) throw DataError();
			Buffer->SkipnLines(2);
			iorb=0;

			while (iorb<NumOccBeta) {
					// Allow a little backgrounding and user cancels
				if (!lProgress->UpdateProgress(Buffer->GetPercentRead())) {
					delete TargetSet;
					return NULL;
				}
				imaxorb = MIN(10, NumOccBeta-iorb);	//Max of 10 orbitals per line
				Buffer->GetLine(Line);
				LinePos = 0;
				for (long jorb=0; jorb<imaxorb; jorb++) {
					ScanErr = sscanf(&(Line[LinePos]), "%ld%n", &TestOrb, &nChar);
					if (ScanErr && (TestOrb==jorb+iorb+1)) LinePos += nChar;
					else {
						imaxorb = jorb;
						if (jorb==0) {	//No more orbitals found
							imaxorb = 0;
							NumOccBeta = iorb;
						}
						break;
					}
				}
				if (imaxorb > 0) {
					Buffer->SkipnLines(1);	//Skip blank line
						//read in the vector block
					ReadGAMESSlogVectors(Buffer, &(Vectors[iorb*maxfuncs]), maxfuncs, imaxorb);
					iorb += imaxorb;
					Buffer->SkipnLines(1);	//Skip blank line between blocks
				}
			}
		}
		TargetSet->ReSize(NumAlphaOrbs, NumOccBeta);
	}
	catch (...) {
		if (TargetSet) {
			delete TargetSet;
			TargetSet = NULL;
		}
	}
	if (TargetSet) Orbs.push_back(TargetSet);
		//Kludge for Jan
//	if (!Orbs->EigenVectors && NumAlphaOrbs == 1) {
//		Orbs->EigenVectors = Orbs->LocalOrbitals;
//		Orbs->LocalOrbitals = NULL;
//		Orbs->NumOccupiedAlphaOrbs = 1;
//		if (NumOccBeta == 1) Orbs->NumOccupiedBetaOrbs = 1;
//	}
	return TargetSet;
}
void Frame::ParseGVBGIOrbitals(BufferFile * Buffer, const long & NumFuncs, Progress * lProgress) {
	long	iorb=0, imaxorb, TestOrb, LinePos, ScanErr, jorb, NumOrbs;
	int		nChar;
	char	Line[kMaxLineLength+1];
	Buffer->SkipnLines(2);
	NumOrbs = NumFuncs;

	OrbitalRec * OrbSet = NULL;
	
	try {
		OrbSet = new OrbitalRec(NumFuncs, 0, NumFuncs);
		OrbSet->setOrbitalWavefunctionType(GVB);
		OrbSet->setOrbitalType(NaturalOrbital);
		
		float * Vectors = OrbSet->Vectors;
		if (OrbSet->Energy) {
			delete [] OrbSet->Energy;
			OrbSet->Energy = NULL;
		}
		if (OrbSet->SymType) {	//symmetry labels are not printed for NO's
			delete [] OrbSet->SymType;
			OrbSet->SymType = NULL;
		}

		while (iorb<NumOrbs) {
				// Allow a little backgrounding and user cancels
			if (!lProgress->UpdateProgress(Buffer->GetPercentRead())) {
				delete OrbSet;
				return;
			}
			imaxorb = MIN(10, NumOrbs-iorb);	//Max of 10 orbitals per line
			Buffer->GetLine(Line);
			LinePos = 0;
			for (jorb=0; jorb<imaxorb; jorb++) {
				ScanErr = sscanf(&(Line[LinePos]), "%ld%n", &TestOrb, &nChar);
				if (ScanErr && (TestOrb==jorb+iorb+1)) LinePos += nChar;
				else {
					imaxorb = jorb;
					if (jorb==0) {	//No more orbitals found
						imaxorb = 0;
						NumOrbs = iorb;
					}
					break;
				}
			}
			Buffer->SkipnLines(1);
			if (imaxorb > 0) {
					//read in the vector block
				ReadGAMESSlogVectors(Buffer, &(Vectors[iorb*NumFuncs]), NumFuncs, imaxorb);
				iorb += imaxorb;
				Buffer->SkipnLines(1);	//Skip blank line between blocks
			}
		}
	}
	catch (...) {
		if (OrbSet) {
			delete OrbSet;
			OrbSet = NULL;
		}
	}
	if (OrbSet != NULL) {
		OrbSet->ReSize(NumOrbs, 0);
		Orbs.push_back(OrbSet);
	}
}

void Frame::ReadMP2Vectors(BufferFile * Buffer, BufferFile * DatBuffer, long NumFuncs, Progress * /*lProgress*/,
	long * readflag) {

	if (*readflag == 1) {	//If first pass query the user about reading vectors
        // 1 = Yes
        // 0 = No
#ifdef __wxBuild__
        int result = wxMessageBox(wxT("Do you wish to read the MP2 natural orbitals from the .dat file\?"),
								  wxT(""), wxYES_NO | wxICON_QUESTION);
        if(result == wxYES)
            *readflag = 1;
        else
            *readflag = 0;
#else
		*readflag = YesOrNoDialog(131, 2);
#endif
	}
	if (!*readflag) return;
	
	if (!DatBuffer)
		DatBuffer = OpenDatFile();
	if (DatBuffer) {
		DatBuffer->SetFilePos(0);
		*readflag = 2;
			bool found = false;
			char	Line[kMaxLineLength];
			double	testEnergy;
		while (!found && DatBuffer->LocateKeyWord("MP2 NATURAL ORBITALS, E(MP2)=", 29,-1)) {
			DatBuffer->GetLine(Line);
			sscanf(&(Line[30]), "%lf", &testEnergy);
			if (fabs(testEnergy - MP2Energy) < 1.0e-8) found = true;
		}
		if (found) {	//Found the correct MP2 vectors
			OrbitalRec * OrbSet=NULL;
			try {
				OrbSet = new OrbitalRec(NumFuncs, 0, NumFuncs);

//				OrbSet->ReadVecGroup(DatBuffer, NumFuncs, 0);
				OrbSet->ReadVecGroup(DatBuffer, NumFuncs, RHFMP2);

				long lNumOrbs = OrbSet->NumAlphaOrbs;
				float * OccNums = OrbSet->OrbOccupation = new float [lNumOrbs];
				if (!OccNums) throw MemoryError();
				Buffer->GetLine(Line);
					long scanerr, LinePos;
					int nchar;
				LinePos = 0;
				for (long ifunc=0; ifunc<lNumOrbs; ifunc++) {
					scanerr = sscanf(&(Line[LinePos]), "%f%n", &(OccNums[ifunc]), &nchar);
					if (scanerr != 1) {
						Buffer->GetLine(Line);
						LinePos = 0;
						scanerr = sscanf(Line, "%f%n", &(OccNums[ifunc]), &nchar);
						if (scanerr != 1) throw DataError();
					}
					LinePos += nchar;
				}
				OrbSet->NumOccupiedAlphaOrbs = lNumOrbs;
			}
			catch (...) {
				if (OrbSet) {
					delete OrbSet;
					OrbSet = NULL;
				}
			}
			if (OrbSet) {
				OrbSet->setOrbitalWavefunctionType(RHFMP2);
				OrbSet->setOrbitalType(NaturalOrbital);
				Orbs.push_back(OrbSet);
			}
		}
	}
}

long Frame::GetNumMMAtoms(void) {
	long result = 0;
	for (int i=0; i<NumAtoms; i++)
		if (Atoms[i].IsSIMOMMAtom()) result++;
	return result;
}

//call to read in the coefficient vectors for orbitals. This routine reads one block
//of vectors which is in the same format for all wavefunction types. You should call
//with the Buffer positioned at the beginning of the line containing the first
//coef. and the Vectors positioned to accept it.
void ReadGAMESSlogVectors(BufferFile * Buffer, float *Vectors, long MaxFuncs, long NumFuncs) {
	long	LinePos, iorb;
	int		nChar;
	char	LineText[kMaxLineLength+1];

	for (long ifunc=0; ifunc<MaxFuncs; ifunc++) {
		Buffer->GetLine(LineText);
		LinePos = 15;	//skip past the junk at the beginning of each line
		if (isalpha(LineText[LinePos])) LinePos++;
		for (iorb=0; iorb<NumFuncs; iorb++) {	//get the coef for each orbital/function
			sscanf(&(LineText[LinePos]), "%f%n", &(Vectors[ifunc+iorb*MaxFuncs]),&nChar);
			LinePos+=nChar;		//nChar contains the number of char's read by sscanf including spaces
		}
	}
}
AtomTypeList * Frame::GetAtomTypes(void) {
	AtomTypeList * list = new AtomTypeList;
	if (list) {
		for (long i=0; i<NumAtoms; i++) {
			list->AddAtomType(Atoms[i].Type);
		}
	}
	return list;
}
//Read in the full set of normal modes, if present from a GAMESS log file
void Frame::ParseNormalModes(BufferFile * Buffer, Progress * ProgressInd, WinPrefs * Prefs) {

	try {//catch errors locally
		if (Buffer->LocateKeyWord("NORMAL COORDINATE ANALYSIS", 25)) {
			//If there are no normal modes we just fall out the bottom and return
			ProgressInd->ChangeText("Reading Normal Modes");
			if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
				{throw UserCancel();}
				
				//If this is a SIMOMM run then only the MM atoms have modes and are assumed
				//to be first in the atom list.
				//Update! Apparently the current code does print out any vibration information
				//for any MM atoms.
			long NumSIMOMMAtoms = GetNumMMAtoms();
			long NumModes = 3*NumAtoms;
			long NumActiveAtoms = NumAtoms;
			if (NumSIMOMMAtoms > 0) {
				NumActiveAtoms = NumAtoms - NumSIMOMMAtoms;
				NumModes = (NumActiveAtoms)*3;
			}
			VibRec * lVibs = Vibs = new VibRec(NumModes, NumAtoms);

	//insufficient memory available to store the normal modes, so abort reading them in...
	// should alert the user to the reason he didn't get normal modes...
	//		if (!lVibs) {
	//			break;
	//		}
			long	imode=0, ifreq, LastPass=0, test, cmode;
			int		nchar;
			char	LineText[kMaxLineLength], token[kMaxLineLength];
			while (imode<NumModes) {
				if (!Buffer->LocateKeyWord("FREQUENCY:", 10)) break;
				long NumVibs = MIN(9, NumModes-imode);
		//		long NumVibs = ((9) > (NumModes-imode)) ? (NumModes-imode) : (9);
				Buffer->GetLine(LineText);
				long LinePos = 11;
				long LineLength = strlen(LineText);
				for (unsigned long icol=0; icol<NumVibs; icol++) {
					if (LinePos<LineLength) {
						sscanf(&(LineText[LinePos]), "%s%n", token, &nchar);
						LinePos += nchar;
						long test = strlen(token);
						if ((nchar>0)&&(test>0)) {
							if (LineText[LinePos+1] == 'I') {
								token[test] = 'i';
								test++;
								token[test]=0;
							}
							lVibs->Frequencies.push_back(std::string(token));
							LinePos+=2;
						} else NumVibs = icol;
					} else NumVibs = icol;
				}
				imode += NumVibs;
				if (Buffer->LocateKeyWord("REDUCED MASS:", 13, Buffer->GetFilePos()+132)) {
					Buffer->GetLine(LineText);
					LinePos = 14;
					if ((imode == NumVibs)&&(lVibs->ReducedMass.empty())) {
						lVibs->ReducedMass.reserve(NumModes);
					}
					LineLength = strlen(LineText);
					long	tVib = NumVibs;
					float	rmass;
					for (long icol=0; icol<tVib; icol++) {
						if (LinePos<LineLength) {
							test = sscanf(&(LineText[LinePos]), "%s%n", &token, &nchar);
							LinePos += nchar;
							if (test) {
								if (token[0] != '*') {
									test = sscanf(token, "%f", &rmass);
									if (test)
										lVibs->ReducedMass.push_back(rmass);
								} else lVibs->ReducedMass.push_back(10000.0);
							} else tVib = icol;
						} else tVib = icol;
					}
				}
				if (Buffer->LocateKeyWord("INTENSITY:", 10, Buffer->GetFilePos()+132)) {
					Buffer->GetLine(LineText);
					LinePos = 11;
					LineLength = strlen(LineText);
					long	tVib = NumVibs;
					float	Inten;
					for (long icol=0; icol<tVib; icol++) {
						if (LinePos<LineLength) {
							test = sscanf(&(LineText[LinePos]), "%s%n", &token, &nchar);
							LinePos += nchar;
							if (test) {
								if (token[0] != '*') {
									test = sscanf(token, "%f", &Inten);
									if (test)
										lVibs->Intensities[icol+LastPass] = Inten;
								} else lVibs->Intensities[icol+LastPass] = 10000.0;
							} else tVib = icol;
						} else tVib = icol;
					}
				}
				if (Buffer->LocateKeyWord("RAMAN INTENSITY:", 16, Buffer->GetFilePos()+132)) {
					Buffer->GetLine(LineText);
					LinePos = 17;
					if ((imode == NumVibs)&&(lVibs->RamanIntensity.empty())) {
						lVibs->RamanIntensity.reserve(NumModes);
					}
					LineLength = strlen(LineText);
					long	tVib = NumVibs;
					float	raman;
					for (long icol=0; icol<tVib; icol++) {
						if (LinePos<LineLength) {
							test = sscanf(&(LineText[LinePos]), "%s%n", &token, &nchar);
							LinePos += nchar;
							if (test) {
								if (token[0] != '*') {
									test = sscanf(token, "%f", &raman);
									if (test)
										lVibs->RamanIntensity.push_back(raman);
								} else lVibs->RamanIntensity.push_back(10000.0);
							} else tVib = icol;
						} else tVib = icol;
					}
				}
				if (Buffer->LocateKeyWord("DEPOLARIZATION:", 15, Buffer->GetFilePos()+132)) {
					Buffer->GetLine(LineText);
					LinePos = 16;
					if ((imode == NumVibs)&&(lVibs->Depolarization.empty())) {
						lVibs->Depolarization.reserve(NumModes);
					}
					LineLength = strlen(LineText);
					long	tVib = NumVibs;
					float	depol;
					for (long icol=0; icol<tVib; icol++) {
						if (LinePos<LineLength) {
							test = sscanf(&(LineText[LinePos]), "%s%n", &token, &nchar);
							LinePos += nchar;
							if (test) {
								if (token[0] != '*') {
									test = sscanf(token, "%f", &depol);
									if (test)
										lVibs->Depolarization.push_back(depol);
								} else lVibs->Depolarization.push_back(10000.0);
							} else tVib = icol;
						} else tVib = icol;
					}
				}
				test = Buffer->FindBlankLine();
				Buffer->SetFilePos(test);
				Buffer->SkipnLines(1);

				for (test=NumSIMOMMAtoms; test<NumAtoms; test++) {
					if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
						{throw UserCancel();}

					Buffer->GetLine(LineText);
					LinePos = 20;
					float lmass = Prefs->GetSqrtAtomMass(Atoms[test].GetType()-1);
					for (ifreq=LastPass; ifreq<imode; ifreq++) {
						cmode = test + ifreq*NumAtoms;
						sscanf(&(LineText[LinePos]), "%f%n", &((lVibs->NormMode[cmode]).x), &nchar);
						LinePos += nchar;
						(lVibs->NormMode[cmode]).x *= lmass;
					}
					Buffer->GetLine(LineText);
					LinePos = 20;
					for (ifreq=LastPass; ifreq<imode; ifreq++) {
						cmode = test + ifreq*NumAtoms;
						sscanf(&(LineText[LinePos]), "%f%n", &((lVibs->NormMode[cmode]).y), &nchar);
						LinePos += nchar;
						(lVibs->NormMode[cmode]).y *= lmass;
					}
					Buffer->GetLine(LineText);
					LinePos = 20;
					for (ifreq=LastPass; ifreq<imode; ifreq++) {
						cmode = test + ifreq*NumAtoms;
						sscanf(&(LineText[LinePos]), "%f%n", &((lVibs->NormMode[cmode]).z), &nchar);
						LinePos += nchar;
						(lVibs->NormMode[cmode]).z *= lmass;
					}
				}
			//ok apparently its the simomm atoms that are missing...
		//		if (NumSIMOMMAtoms > 0) { //zero out remaining atoms for SIMOMM runs
		//			for (int i=NumSIMOMMAtoms; i<NumAtoms; i++) {
		//				cmode = i+ifreq*NumAtoms;
		//				(lVibs->NormMode[cmode]).x = 0.0;
		//				(lVibs->NormMode[cmode]).y = 0.0;
		//				(lVibs->NormMode[cmode]).z = 0.0;
		//			}
		//		}
				LastPass = imode;
				sprintf(LineText, "Read in %d normal modes", LastPass);
				ProgressInd->ChangeText(LineText);
				if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
					{throw UserCancel();}
			}
			lVibs->NumModes = LastPass;
		}
	}	//trap errors here and delete the VibRec
	catch (std::bad_alloc) {//Memory error, cleanup and return.
		if (Vibs) { delete Vibs; Vibs = NULL; }
		MessageAlert("Insufficient memory to read normal modes. Normal Modes will be skipped!");
	}
	catch (UserCancel) {//We need to rethrow this one since the whole operation should be aborted
		if (Vibs) { delete Vibs; Vibs = NULL; }
		throw;
	}
	catch (...) {//File and data errors. Either way delete the vectors and return.
		if (Vibs) { delete Vibs; Vibs = NULL; }
		MessageAlert("Error parsing normal modes. Normal modes will be skipped.");
	}
}
void Frame::ParseMolDenFrequencies(BufferFile * Buffer, WinPrefs * Prefs) {
	try {//catch errors locally
		if (Buffer->LocateKeyWord("[FREQ]", 6)) {
			Buffer->SkipnLines(1);
			long NumModes = 3*NumAtoms;
			VibRec * lVibs = Vibs = new VibRec(NumModes, NumAtoms);
			
			char	LineText[kMaxLineLength], token[kMaxLineLength];
			for (int i=0; i<NumModes; i++) {
				Buffer->GetLine(LineText);
				if (LineText[0] == '[') {
					NumModes = i;
					break;
				}
				int c =	sscanf(LineText, "%s", token);
				if (c == 1) {
					lVibs->Frequencies.push_back(std::string(token));
				} else {
					NumModes = i;
					break;
				}
				if (Buffer->GetFilePos() >= Buffer->GetFileSize()) {
					NumModes = i;
					break;
				}
			}
			Buffer->SetFilePos(0);
			if (Buffer->LocateKeyWord("[INT]", 5)) {
				Buffer->SkipnLines(1);
				char token2[kMaxLineLength];
				for (int i=0; i<NumModes; i++) {
					Buffer->GetLine(LineText);
					if (LineText[0] == '[') break;
					int c =	sscanf(LineText, "%s %s", token, token2);
					float inten;
					if (c >= 1) {
						int check = sscanf(token, "%f", &inten);
						if (check != 1) inten = 10000.0;
						lVibs->Intensities[i] = inten;
						if (c == 2) {
							check = sscanf(token, "%f", &inten);
							if (check != 1) inten = 10000.0;
							lVibs->RamanIntensity.push_back(inten);
						}
					} else break;
					if (Buffer->GetFilePos() >= Buffer->GetFileSize()) break;
				}
			}
			Buffer->SetFilePos(0);
			if (Buffer->LocateKeyWord("[FR-NORM-COORD]", 14)) {
				Buffer->SkipnLines(1);
				for (int i=0; i<NumModes; i++) {
					Buffer->GetLine(LineText);
					int check;
					sscanf(LineText, "%s %d", token, &check);
					if (check != (i+1)) {
						NumModes = i;
						break;
					}
					for (int j=0; j<NumAtoms; j++) {
						Buffer->GetLine(LineText);
						int imode = j + i*NumAtoms;
						check = sscanf(LineText, "%f %f %f", &(lVibs->NormMode[imode].x), &(lVibs->NormMode[imode].y),
									   &(lVibs->NormMode[imode].z));
						float lmass = Prefs->GetSqrtAtomMass(Atoms[j].GetType()-1);
						lVibs->NormMode[imode] *= lmass;
					}
				}
			}
			lVibs->NumModes = NumModes;
		}
	}	//trap errors here and delete the VibRec
	catch (std::bad_alloc) {//Memory error, cleanup and return.
		if (Vibs) { delete Vibs; Vibs = NULL; }
		MessageAlert("Insufficient memory to read normal modes. Normal Modes will be skipped!");
	}
	catch (...) {//File and data errors. Either way delete the vectors and return.
		if (Vibs) { delete Vibs; Vibs = NULL; }
		MessageAlert("Error parsing normal modes. Normal modes will be skipped.");
	}
}

void Frame::toggleMMAtomVisibility(void) {
	for (int i=0; i<NumAtoms; i++) {
		if (Atoms[i].IsSIMOMMAtom()) 
			Atoms[i].SetInvisibility(1-Atoms[i].GetInvisibility());
	}
}

void Frame::toggleAbInitioVisibility(void) {
	for (int i=0; i<NumAtoms; i++) {
		if (! (Atoms[i].IsSIMOMMAtom())) 
			Atoms[i].SetInvisibility(1-Atoms[i].GetInvisibility());
	}
}

void Frame::resetAllSelectState()
{
  for ( int i = 0; i < NumAtoms; i++)
    Atoms[i].SetSelectState(false);

  for ( int i = 0; i < NumBonds; i++)
    Bonds[i].SetSelectState(false);
}
