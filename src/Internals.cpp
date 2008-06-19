/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* Internals.cpp

	Classes to handle internal coordinates

	BMB 4/1998
*/

#include "Globals.h"
#include "GlobalExceptions.h"
#include "Internals.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "Math3D.h"
#include <iostream>

Internals::Internals(void) {
	MOPacStyle = NULL;
	GeneralStyle = NULL;
}
Internals::Internals(Internals * copy) {
	MOPacStyle = NULL;
	GeneralStyle = NULL;
	if (copy->MOPacStyle)
		MOPacStyle = new MOPacInternals(copy->MOPacStyle);
}
Internals::~Internals(void) {
	if (MOPacStyle) delete MOPacStyle;
	if (GeneralStyle) delete GeneralStyle;
}
void Internals::CreateMOPacInternals(long Num) {
	if (!MOPacStyle) {
		MOPacStyle = new MOPacInternals(Num);
	}
}
MOPacInternals::MOPacInternals(long Num) {
	ConnectionAtoms = new long[Num];
	Values = new float[Num];
	Type = new char[Num];
	if (!ConnectionAtoms || !Values || !Type) throw MemoryError();
	Allocation = Num;
	Count = 0;
}
MOPacInternals::MOPacInternals(MOPacInternals * copy) {
	ConnectionAtoms = new long[copy->Count];
	Values = new float[copy->Count];
	Type = new char[copy->Count];
	if (!ConnectionAtoms || !Values || !Type) throw MemoryError();
	Allocation = copy->Count;
	Count = copy->Count;
	for (long i=0; i<Count; i++) {
		ConnectionAtoms[i] = copy->ConnectionAtoms[i];
		Values[i] = copy->Values[i];
		Type[i] = copy->Type[i];
	}
}
MOPacInternals::~MOPacInternals(void) {
	if (ConnectionAtoms) delete [] ConnectionAtoms;
	if (Values) delete [] Values;
	if (Type) delete [] Type;
}
void Internals::WriteCoordinatesToFile(BufferFile * File, MoleculeData * MainData, WinPrefs * Prefs) {
	if (!MOPacStyle) {
		CreateMOPacInternals(3*MainData->GetMaximumAtomCount());
		if (MOPacStyle) MOPacStyle->GuessInit(MainData);
	}
	if (MOPacStyle) MOPacStyle->WriteCoordinatesToFile(File, MainData, Prefs);
}
void Internals::WriteMPCZMatCoordinatesToFile(BufferFile * File, MoleculeData * MainData, WinPrefs * Prefs) {
	if (!MOPacStyle) {
		CreateMOPacInternals(3*MainData->GetMaximumAtomCount());
		if (MOPacStyle) MOPacStyle->GuessInit(MainData);
	}
	if (MOPacStyle) MOPacStyle->WriteMPCZMatCoordinatesToFile(File, MainData, Prefs);
}
//Guess the connection atom set, then setup the values
void MOPacInternals::GuessInit(MoleculeData * MainData, long theAtom, bool keepOld) {
	Frame * lFrame = MainData->GetCurrentFramePtr();
	if (3*lFrame->NumAtoms > Allocation) return;
	if (lFrame->NumAtoms < 2) return;
	long StartAtom=1, EndAtom=lFrame->NumAtoms;
	//Nothing to do for the first atom
	if (theAtom == 0) return;
	if (theAtom>0) {
		StartAtom=theAtom;
		EndAtom=StartAtom+1;
	}
		// First pair of atoms
		CPoint3D BondVector, testVector;
		long BondedAtom, AngleAtom, DihedralAtom, i;
		float BondLength, SecondLength, ThirdLength, testLength;
	for (long iatom=StartAtom; iatom<EndAtom; iatom++) {
			//Find the closest atom to choose as the bonded atom
		BondedAtom = 0;
		BondVector.x = lFrame->Atoms[iatom].Position.x - lFrame->Atoms[0].Position.x;
		BondVector.y = lFrame->Atoms[iatom].Position.y - lFrame->Atoms[0].Position.y;
		BondVector.z = lFrame->Atoms[iatom].Position.z - lFrame->Atoms[0].Position.z;
		BondLength = BondVector.Magnitude();
		SecondLength = ThirdLength = 1.0E10;
		AngleAtom = DihedralAtom = 0;
		for (i=1; i<iatom; i++) {
			testVector.x = lFrame->Atoms[iatom].Position.x - lFrame->Atoms[i].Position.x;
			testVector.y = lFrame->Atoms[iatom].Position.y - lFrame->Atoms[i].Position.y;
			testVector.z = lFrame->Atoms[iatom].Position.z - lFrame->Atoms[i].Position.z;
			testLength = testVector.Magnitude();
			if (testLength<BondLength) {
				ThirdLength = SecondLength;
				DihedralAtom = AngleAtom;
				SecondLength = BondLength;
				AngleAtom = BondedAtom;
				BondedAtom = i;
				BondLength = testLength;
				BondVector = testVector;
			} else if (testLength<SecondLength) {
				ThirdLength = SecondLength;
				DihedralAtom = AngleAtom;
				SecondLength = testLength;
				AngleAtom = i;
			} else if (testLength<ThirdLength) {
				ThirdLength = testLength;
				DihedralAtom = i;
			}
		}	//the three closest atoms are now known
		/* Originally I simply used the closest 3 atoms as the connection atoms.
			However, this apparently does not work so well for large molecules that 
			fold back on themselves, especially those with hydrogen bonds.
			So instead try using the connection list for the atom defining the bond.
		*/
		if (keepOld) {
			if ((ConnectionAtoms[3*iatom]>=0)&&(ConnectionAtoms[3*iatom]<iatom)) {
				if (AngleAtom == ConnectionAtoms[3*iatom]) AngleAtom = BondedAtom;
				if (DihedralAtom == ConnectionAtoms[3*iatom]) DihedralAtom = BondedAtom;
				BondedAtom = ConnectionAtoms[3*iatom];
			}
		}
		//I ran into an issue in an Oh struture where the three defining atoms are linear.
		//Needless to say this doesn't result in a good definition so test for that here.
		//Get the angle between the three defining atoms, if its 0 or 180 they are linear.
		float angle = 0.0f;
		bool valid = lFrame->GetBondAngle(BondedAtom, AngleAtom, DihedralAtom, &angle);
		if (valid && ((fabs(angle)>179.9)||(fabs(angle)<0.1))) {
			float newLength = 1.0E10;
			//find the closest atom that isn't linear with the other two
			for (i=1; i<iatom; i++) {
				if ((i!=BondedAtom)&&(i!=AngleAtom)&&(i!=DihedralAtom)) {
					testVector.x = lFrame->Atoms[iatom].Position.x - lFrame->Atoms[i].Position.x;
					testVector.y = lFrame->Atoms[iatom].Position.y - lFrame->Atoms[i].Position.y;
					testVector.z = lFrame->Atoms[iatom].Position.z - lFrame->Atoms[i].Position.z;
					testLength = testVector.Magnitude();
					if (testLength < newLength) {
						lFrame->GetBondAngle(BondedAtom, AngleAtom, i, &angle);
						if (!((fabs(angle)>179.9)||(fabs(angle)<0.1))) {
							DihedralAtom = i;
							newLength = testLength;
						}
					}
				}
			}
		}
		ConnectionAtoms[3*iatom] = BondedAtom;
		Values[3*iatom] = BondLength;
		Type[3*iatom] = 0;
		if (iatom >= 2) {
			if (keepOld &&(ConnectionAtoms[3*iatom+1]>=0)&&(ConnectionAtoms[3*iatom+1]<iatom)&&
				(ConnectionAtoms[3*iatom+1]!=BondedAtom)) {
				if (DihedralAtom == ConnectionAtoms[3*iatom+1]) DihedralAtom = BondedAtom;
				AngleAtom = ConnectionAtoms[3*iatom+1];
			} else {
					//Bond angle - atom -> BondedAtom -> AngleAtom
				if ((BondedAtom > 0)&&(ConnectionAtoms[3*BondedAtom]!=BondedAtom)&&(lFrame->Atoms[iatom].Type>1)&&
					(lFrame->Atoms[ConnectionAtoms[3*BondedAtom]].Type>1)) {
					ConnectionAtoms[3*iatom+1] = ConnectionAtoms[3*BondedAtom];
					if (DihedralAtom == ConnectionAtoms[3*BondedAtom]) DihedralAtom = AngleAtom;
					AngleAtom = ConnectionAtoms[3*BondedAtom];
				} else
					ConnectionAtoms[3*iatom+1] = AngleAtom;
			}
			Type[3*iatom+1] = 1;
			if (iatom >= 3) {
				if (keepOld &&(ConnectionAtoms[3*iatom+2]>=0)&&(ConnectionAtoms[3*iatom+2]<iatom)&&
					(ConnectionAtoms[3*iatom+2]!=BondedAtom)&&(ConnectionAtoms[3*iatom+2]!=AngleAtom)) {
					if (DihedralAtom == ConnectionAtoms[3*iatom+1]) DihedralAtom = BondedAtom;
					DihedralAtom = ConnectionAtoms[3*iatom+2];
				} else {
						//Dihedral Angle
					if ((BondedAtom > 2)&&(ConnectionAtoms[3*BondedAtom+1]!=AngleAtom)&&(lFrame->Atoms[iatom].Type>1)&&
						(lFrame->Atoms[ConnectionAtoms[3*BondedAtom+1]].Type>1))
						ConnectionAtoms[3*iatom+2] = ConnectionAtoms[3*BondedAtom+1];
					else {
						if (DihedralAtom != ConnectionAtoms[3*iatom+1])
							ConnectionAtoms[3*iatom+2] = DihedralAtom;
						else
							ConnectionAtoms[3*iatom+2] = AngleAtom;
					}
				}
				Type[3*iatom+2] = 2;
			}
		}
	}
	if (theAtom > 0)
		UpdateInternalValuesAtom(MainData, theAtom);
	else {	//update all the atoms
		Count = 3*lFrame->NumAtoms;
		CartesiansToInternals(MainData);
	}
}
void MOPacInternals::AddInternalCoordinate(long whichAtom, long connectedAtom, short type, float value) {
	if (3*whichAtom > Allocation) return;
	if (connectedAtom > whichAtom) return; //forward references are not allowed
	if ((type < 0)||(type>2)) return; //invalid type
	if (3*whichAtom > Count) Count = 3*whichAtom;
	ConnectionAtoms[3*whichAtom+type] = connectedAtom;
	Values[3*whichAtom+type] = value;
}
//Take the given cartesians and connection list and recalculate
//the values of each internal coordinate
void MOPacInternals::CartesiansToInternals(MoleculeData * MainData) {
	Frame * lFrame = MainData->GetCurrentFramePtr();
	if (3*lFrame->NumAtoms > Count) return;
	if (lFrame->NumAtoms < 2) return;
	
	for (long i=1; i<lFrame->NumAtoms; i++) {
		UpdateInternalValuesAtom(MainData, i);
	}
}
void MOPacInternals::UpdateInternalValuesAtom(MoleculeData * MainData, long theAtom) {
	Frame * lFrame = MainData->GetCurrentFramePtr();
	if ((theAtom <= 0)||(theAtom*3 > Count)) return;	//sanity check
	long BondedAtom, AngleAtom, DihedralAtom;
	float BondLength, BondAngle, Dihedral;
	CPoint3D BondVector, offset2, offset3;
	//Bond Length
	BondedAtom = ConnectionAtoms[3*theAtom];
	BondVector.x = lFrame->Atoms[BondedAtom].Position.x - lFrame->Atoms[theAtom].Position.x;
	BondVector.y = lFrame->Atoms[BondedAtom].Position.y - lFrame->Atoms[theAtom].Position.y;
	BondVector.z = lFrame->Atoms[BondedAtom].Position.z - lFrame->Atoms[theAtom].Position.z;
	BondLength = BondVector.Magnitude();
	Values[3*theAtom] = BondLength;
	if (theAtom <= 1) return;
	//Bond angle
	AngleAtom = ConnectionAtoms[3*theAtom+1];
	offset2.x = lFrame->Atoms[theAtom].Position.x - lFrame->Atoms[AngleAtom].Position.x;
	offset2.y = lFrame->Atoms[theAtom].Position.y - lFrame->Atoms[AngleAtom].Position.y;
	offset2.z = lFrame->Atoms[theAtom].Position.z - lFrame->Atoms[AngleAtom].Position.z;
	float length3 = offset2.Magnitude();
	offset2.x = lFrame->Atoms[AngleAtom].Position.x - lFrame->Atoms[BondedAtom].Position.x;
	offset2.y = lFrame->Atoms[AngleAtom].Position.y - lFrame->Atoms[BondedAtom].Position.y;
	offset2.z = lFrame->Atoms[AngleAtom].Position.z - lFrame->Atoms[BondedAtom].Position.z;
	float length2 = offset2.Magnitude();
	if ((fabs(BondLength)>0.0001)&&(fabs(length3)>0.0001)&&(fabs(length2)>0.0001)) {
		float Radians = (BondLength*BondLength+length2*length2-length3*length3)/
		(2*BondLength*length2);
		//Make sure the angle is within the allowable values for acos (-1 to 1)
		if (Radians > 1.0) Radians = 1.0;
		else if (Radians < -1.0) Radians = -1.0;
		BondAngle = acos(Radians);
		BondAngle *= kRadToDegree;
	} else BondAngle = 0.0;
	Values[3*theAtom+1] = BondAngle;
	//Dihedral angle
	if (theAtom > 2) {
		DihedralAtom = ConnectionAtoms[3*theAtom+2];
		offset3.x = lFrame->Atoms[DihedralAtom].Position.x - lFrame->Atoms[AngleAtom].Position.x;
		offset3.y = lFrame->Atoms[DihedralAtom].Position.y - lFrame->Atoms[AngleAtom].Position.y;
		offset3.z = lFrame->Atoms[DihedralAtom].Position.z - lFrame->Atoms[AngleAtom].Position.z;
		//	float length3 = offset3.Magnitude();
		CPoint3D UnitIJ = BondVector;
		CPoint3D UnitJK = offset2;
		CPoint3D UnitKL = offset3;
		Normalize3D(&UnitIJ);
		Normalize3D(&UnitJK);
		Normalize3D(&UnitKL);
		CPoint3D Normal1, Normal2;
		CrossProduct3D(&UnitIJ, &UnitJK, &Normal1);
		CrossProduct3D(&UnitJK, &UnitKL, &Normal2);
		float DotPJ = DotProduct3D(&UnitIJ, &UnitJK);
		float DotPK = DotProduct3D(&UnitJK, &UnitKL);
		DotPJ = 1.0 - DotPJ*DotPJ;
		DotPK = 1.0 - DotPK*DotPK;
		if ((DotPJ > 0.0)||(DotPK > 0.0)) {	//3 of the atom are linear, Bad!
			float SinPJ = sqrt(DotPJ);
			float SinPK = sqrt(DotPK);
			float Dot = DotProduct3D(&Normal1, &Normal2)/(SinPJ*SinPK);
			if (fabs(Dot) <= kCosErrorTolerance) {		//Bad value for a cos
				if (Dot > 0.9999) Dot = 1.0;
				else if (Dot < -0.9999) Dot = -1.0;
				Dihedral = acos(Dot);
				float Pi = acos(-1.0);
				if (fabs(Dihedral) < kZeroTolerance) Dihedral = 0.0;
				else if (fabs(Dihedral-Pi) < kZeroTolerance) Dihedral = Pi;
				float Sense = DotProduct3D(&Normal2, &BondVector);
				if (Sense < 0.0) Dihedral = -Dihedral;
				Dihedral *= 180.0/Pi;
			}
			Values[3*theAtom+2] = Dihedral;
		}
	}
}
//Take the connections list and values to create a set of cartesian coordinates
void MOPacInternals::InternalsToCartesians(MoleculeData * MainData, WinPrefs * Prefs,
		long ChangedAtom) {
	Frame * lFrame = MainData->GetCurrentFramePtr();
	double DegToRad = acos(-1.0)/180.0;

	if (lFrame->NumAtoms>1) {
	 mpAtom * tempAtoms = new mpAtom[lFrame->NumAtoms];
		if (tempAtoms) {
				long i;
			for (i=0; i<lFrame->NumAtoms; i++) tempAtoms[i].Type = lFrame->Atoms[i].Type;
			if (ChangedAtom < 3) {
				tempAtoms[0].Position.x = tempAtoms[0].Position.y = tempAtoms[0].Position.z = 0;
				tempAtoms[1].Position.x = tempAtoms[1].Position.y = 0.0;
				tempAtoms[1].Position.z = Values[3];
			} else {
				tempAtoms[0].Position.x = lFrame->Atoms[0].Position.x;
				tempAtoms[0].Position.y = lFrame->Atoms[0].Position.y;
				tempAtoms[0].Position.z = lFrame->Atoms[0].Position.z;
				tempAtoms[1].Position.x = lFrame->Atoms[1].Position.x;
				tempAtoms[1].Position.y = lFrame->Atoms[1].Position.y;
				tempAtoms[1].Position.z = lFrame->Atoms[1].Position.z;
			}
			if (lFrame->NumAtoms > 2) {
				if (ChangedAtom < 3) {
					tempAtoms[2].Position.x = Values[6]*sin(Values[7]*DegToRad);
					tempAtoms[2].Position.y = 0.0;
					if (ConnectionAtoms[6]==0)
						tempAtoms[2].Position.z = Values[6]*cos(Values[7]*DegToRad);
					else tempAtoms[2].Position.z = tempAtoms[1].Position.z - 
						Values[6]*cos(Values[7]*DegToRad);
					i=3;	//add atoms using simple formula as long as molecule is linear
					while ((tempAtoms[i-1].Position.x <= 1.0e-5)&&(i<lFrame->NumAtoms)) {
						tempAtoms[i].Position.x = Values[3*i]*sin(Values[3*i+1]*DegToRad);
						tempAtoms[i].Position.y = 0.0;
						float test = tempAtoms[ConnectionAtoms[3*i]].Position.z -
							tempAtoms[ConnectionAtoms[3*i+1]].Position.z;
						tempAtoms[i].Position.z = tempAtoms[ConnectionAtoms[3*i]].Position.z -
							Values[3*i]*cos(Values[3*i+1]*DegToRad)*((test>=0.0)?1.0:0.0);
						i++;
					}
				} else {
					for (long j=2; j<ChangedAtom; j++) {
						tempAtoms[j].Position.x = lFrame->Atoms[j].Position.x;
						tempAtoms[j].Position.y = lFrame->Atoms[j].Position.y;
						tempAtoms[j].Position.z = lFrame->Atoms[j].Position.z;
					}
					i = ChangedAtom;
				}	//Now for general 3D molecules
				for (; i<lFrame->NumAtoms; i++) {
						long j, k, l;
					j = ConnectionAtoms[3*i];
					k = ConnectionAtoms[3*i+1];
					l = ConnectionAtoms[3*i+2];
						CPoint3D Vkl, Vjk;
					Vkl.x = tempAtoms[k].Position.x - tempAtoms[l].Position.x;
					Vkl.y = tempAtoms[k].Position.y - tempAtoms[l].Position.y;
					Vkl.z = tempAtoms[k].Position.z - tempAtoms[l].Position.z;
					CPoint3D Unitkl = Vkl;
					Normalize3D(&Unitkl);
					Vjk.x = tempAtoms[j].Position.x - tempAtoms[k].Position.x;
					Vjk.y = tempAtoms[j].Position.y - tempAtoms[k].Position.y;
					Vjk.z = tempAtoms[j].Position.z - tempAtoms[k].Position.z;
					CPoint3D Unitjk = Vjk;
					Normalize3D(&Unitjk);
						CPoint3D KLxJK;
					CrossProduct3D(&Unitkl, &Unitjk, &KLxJK);
						CPoint3D A3;
					double klDotjk = DotProduct3D(&Unitkl, &Unitjk);
					klDotjk = 1.0 - klDotjk*klDotjk;
						double SinA3;
					if (klDotjk > 0.0001)
						SinA3 = sqrt(klDotjk);
					else
						SinA3 = 0.001;
					A3.x = KLxJK.x/SinA3;
					A3.y = KLxJK.y/SinA3;
					A3.z = KLxJK.z/SinA3;
						CPoint3D A4;
					CrossProduct3D(&A3, &Unitjk, &A4);
     					CPoint3D AtomVector;
     				AtomVector.x = Values[3*i]*((-Unitjk.x*cos(Values[3*i+1]*DegToRad)) +
     					A4.x*sin(Values[3*i+1]*DegToRad)*cos(Values[3*i+2]*DegToRad) +
     					A3.x*sin(Values[3*i+1]*DegToRad)*sin(Values[3*i+2]*DegToRad));
     				AtomVector.y = Values[3*i]*((-Unitjk.y*cos(Values[3*i+1]*DegToRad)) +
     					A4.y*sin(Values[3*i+1]*DegToRad)*cos(Values[3*i+2]*DegToRad) +
     					A3.y*sin(Values[3*i+1]*DegToRad)*sin(Values[3*i+2]*DegToRad));
     				AtomVector.z = Values[3*i]*((-Unitjk.z*cos(Values[3*i+1]*DegToRad)) +
     					A4.z*sin(Values[3*i+1]*DegToRad)*cos(Values[3*i+2]*DegToRad) +
     					A3.z*sin(Values[3*i+1]*DegToRad)*sin(Values[3*i+2]*DegToRad));
     				tempAtoms[i].Position.x = tempAtoms[j].Position.x + AtomVector.x;
     				tempAtoms[i].Position.y = tempAtoms[j].Position.y + AtomVector.y;
     				tempAtoms[i].Position.z = tempAtoms[j].Position.z + AtomVector.z;
				}
			}	//rotate/translate the new coordinates to match the existing coords
				//note this only attempts to match the first four atoms, the others
				//will be allowed to wander
//			MinimizeDifferences(lFrame->Atoms, tempAtoms, lFrame->NumAtoms, Prefs,
//				MAX(lFrame->NumAtoms, 4));
			if (ChangedAtom < 4)
				MinimizeDifferences(lFrame->Atoms, tempAtoms, lFrame->NumAtoms, Prefs,
					lFrame->NumAtoms);
			for (i=0; i<lFrame->NumAtoms; i++)
				lFrame->Atoms[i].Position = tempAtoms[i].Position;
			delete [] tempAtoms;
		}
	}
}

void MOPacInternals::AppendAtom(MoleculeData * MainData) {
	Frame * lFrame = MainData->GetCurrentFramePtr();
	if (3*lFrame->NumAtoms > Allocation) {
		long * templ = new long[3*lFrame->NumAtoms+12];
		if (templ) {
			memcpy(templ, ConnectionAtoms, Count*sizeof(long));
			delete [] ConnectionAtoms;
			ConnectionAtoms = templ;
		} else throw MemoryError();
		float * tempVal = new float[3*lFrame->NumAtoms+12];
		if (tempVal) {
			memcpy(tempVal, Values, Count*sizeof(float));
			delete [] Values;
			Values = tempVal;
		} else throw MemoryError();
		char * tempType = new char [3*lFrame->NumAtoms+12];
		if (tempType) {
			memcpy(tempType, Type, Count*sizeof(char));
			delete [] Type;
			Type = tempType;
		} else throw MemoryError();
		Allocation = 3*lFrame->NumAtoms + 12;
	}
		//Now add the new atoms connections to the end of the list - valid since
		//new atoms are always appended to the end of the atom list
	long newAtom = Count/3;
	Count += 3;
	GuessInit(MainData, newAtom, false);
}
void MOPacInternals::DeleteAtom(MoleculeData * MainData, long WhichAtom) {
	Frame * lFrame = MainData->GetCurrentFramePtr();
	for (long i=WhichAtom; i<lFrame->NumAtoms; i++) {	//pull down the atoms higher in the list
		bool update = false;
		for (long j=0; j<3; j++) {
			ConnectionAtoms[3*i+j] = ConnectionAtoms[3*i+j+3];
			Values[3*i+j] = Values[3*i+j+3];
			Type[3*i+j] = Type[3*i+j+3];
			if (ConnectionAtoms[3*i+j] == WhichAtom) {
				//Have to change the reference to a different atom
				update = true;
				ConnectionAtoms[3*i+j] = -1;
			} else if (ConnectionAtoms[3*i+j] > WhichAtom) ConnectionAtoms[3*i+j]--;	//reduce connection atom count
		}
		if (update)
			GuessInit(MainData, i, true);
	}
	if (Count > 0) Count -= 3;	//reduce the number of internal coordinates
}
void MOPacInternals::UpdateAtoms(MoleculeData * MainData) {
	Frame * lFrame = MainData->GetCurrentFramePtr();
	while (lFrame->NumAtoms*3 > Count) {	//More atoms, just append atoms to the end of the list
		AppendAtom(MainData);
	}
	while (lFrame->NumAtoms*3 < Count) {
		DeleteAtom(MainData, lFrame->NumAtoms+1);
	}
}
void MOPacInternals::ChangeAtomIndex(MoleculeData * MainData, long OldPosition, long NewPosition) {
	//rearrange an atom in the list. There are two cases, moving an atom up in the list
	//and pulling it down. Moving up is always safe, just need to update all indexes and possibly the atom
	//that is being moved.
	//Moving down (later) in the list may force some atom references to change since the reference
	//must be to an atom earlier in the list. (noop if the positions are the same)
	if (NewPosition < OldPosition) {
		long i0 = ConnectionAtoms[OldPosition*3];
		long i1 = ConnectionAtoms[OldPosition*3 + 1];
		long i2 = ConnectionAtoms[OldPosition*3 + 2];
		float v0 = Values[OldPosition*3];
		float v1 = Values[OldPosition*3 + 1];
		float v2 = Values[OldPosition*3 + 2];
		char t0 = Type[OldPosition*3];
		char t1 = Type[OldPosition*3 + 1];
		char t2 = Type[OldPosition*3 + 2];
		for (long i=OldPosition; i>NewPosition; i--) {
			ConnectionAtoms[i*3] = ConnectionAtoms[(i-1)*3];
				//update the index to the new index for the same atom.
			if (ConnectionAtoms[i*3] == OldPosition) ConnectionAtoms[3*i] = NewPosition;
			else if ((ConnectionAtoms[i*3] >= NewPosition)&&(ConnectionAtoms[i*3] < OldPosition)) ConnectionAtoms[i*3] ++;
			ConnectionAtoms[i*3+1] = ConnectionAtoms[(i-1)*3+1];
			if (ConnectionAtoms[i*3+1] == OldPosition) ConnectionAtoms[3*i+1] = NewPosition;
			else if ((ConnectionAtoms[i*3+1] >= NewPosition)&&(ConnectionAtoms[i*3+1] < OldPosition)) ConnectionAtoms[i*3+1] ++;
			ConnectionAtoms[i*3+2] = ConnectionAtoms[(i-1)*3+2];
			if (ConnectionAtoms[i*3+2] == OldPosition) ConnectionAtoms[3*i+2] = NewPosition;
			else if ((ConnectionAtoms[i*3+2] >= NewPosition)&&(ConnectionAtoms[i*3+2] < OldPosition)) ConnectionAtoms[i*3+2] ++;
			Values[i*3] = Values[(i-1)*3];
			Values[i*3+1] = Values[(i-1)*3+1];
			Values[i*3+2] = Values[(i-1)*3+2];
			Type[i*3] = Type[(i-1)*3];
			Type[i*3+1] = Type[(i-1)*3+1];
			Type[i*3+2] = Type[(i-1)*3+2];
			if ((i-1)<3)	//The first three atoms do not have fully validated lists yet
				GuessInit(MainData, i, true);
		}
		ConnectionAtoms[NewPosition*3] = i0;
		ConnectionAtoms[NewPosition*3+1] = i1;
		ConnectionAtoms[NewPosition*3+2] = i2;
		Values[NewPosition*3] = v0;
		Values[NewPosition*3+1] = v1;
		Values[NewPosition*3+2] = v2;
		Type[NewPosition*3] = t0;
		Type[NewPosition*3+1] = t1;
		Type[NewPosition*3+2] = t2;
		bool update=false;
		//make sure the moved atoms references point to atoms prior to it in the list
		if (ConnectionAtoms[3*NewPosition] >= NewPosition) {
			ConnectionAtoms[3*NewPosition] = -1;
			update = true;
		}
		if ((ConnectionAtoms[3*NewPosition+1] >= NewPosition)||(ConnectionAtoms[3*NewPosition]==ConnectionAtoms[3*NewPosition+1])) {
			ConnectionAtoms[3*NewPosition+1] = -1;
			update = true;
		}
		if ((ConnectionAtoms[3*NewPosition+2] >= NewPosition)||
			(ConnectionAtoms[3*NewPosition]==ConnectionAtoms[3*NewPosition+2])||
			(ConnectionAtoms[3*NewPosition+1]==ConnectionAtoms[3*NewPosition+2])) {
			ConnectionAtoms[3*NewPosition+2] = -1;
			update = true;
		}
		if (update) GuessInit(MainData, NewPosition, true);
		//Now update references for atoms higher up in the list
		for (long i=(OldPosition+1); (i*3)<Count; i++) {
			for (long j=0; j<3; j++) {
				if (ConnectionAtoms[3*i+j] == OldPosition)
					ConnectionAtoms[3*i+j] = NewPosition;
				else if ((ConnectionAtoms[3*i+j] >= NewPosition) &&
						 (ConnectionAtoms[3*i+j] < OldPosition))
					ConnectionAtoms[3*i+j]++;
			}
		}
	} else if (NewPosition > OldPosition) {
		//When moving down the list the atom being moved does not change, but any atom that references it
		//before the NewPosition will have to be changed.
		long i0 = ConnectionAtoms[OldPosition*3];
		long i1 = ConnectionAtoms[OldPosition*3 + 1];
		long i2 = ConnectionAtoms[OldPosition*3 + 2];
		float v0 = Values[OldPosition*3];
		float v1 = Values[OldPosition*3 + 1];
		float v2 = Values[OldPosition*3 + 2];
		char t0 = Type[OldPosition*3];
		char t1 = Type[OldPosition*3 + 1];
		char t2 = Type[OldPosition*3 + 2];
		for (long i=OldPosition; i<NewPosition; i++) {
			bool update = false;
			ConnectionAtoms[i*3] = ConnectionAtoms[(i+1)*3];
			ConnectionAtoms[i*3+1] = ConnectionAtoms[(i+1)*3+1];
			ConnectionAtoms[i*3+2] = ConnectionAtoms[(i+1)*3+2];
			Values[i*3] = Values[(i+1)*3];
			Values[i*3+1] = Values[(i+1)*3+1];
			Values[i*3+2] = Values[(i+1)*3+2];
			Type[i*3] = Type[(i+1)*3];
			Type[i*3+1] = Type[(i+1)*3+1];
			Type[i*3+2] = Type[(i+1)*3+2];
			//update the index to the new index for the same atom.
			if (ConnectionAtoms[i*3] == OldPosition) {
				//Since the newposition is later in the list this reference must change
				ConnectionAtoms[3*i] = -1;
				update = true;
			} else if ((ConnectionAtoms[i*3] > OldPosition)&&(ConnectionAtoms[i*3] <= NewPosition)) 
				ConnectionAtoms[i*3] --;
			if (ConnectionAtoms[i*3+1] == OldPosition) {
				ConnectionAtoms[3*i+1] = -1;
				update = true;
			} else if ((ConnectionAtoms[i*3+1] > OldPosition)&&(ConnectionAtoms[i*3+1] <= NewPosition))
				ConnectionAtoms[i*3+1] --;
			if (ConnectionAtoms[i*3+2] == OldPosition) {
				ConnectionAtoms[3*i+2] = -1;
				update = true;
			} else if ((ConnectionAtoms[i*3+2] > OldPosition)&&(ConnectionAtoms[i*3+2] <= NewPosition))
				ConnectionAtoms[i*3+2] --;

			if (update) {
				//A reference has changed. Confirm that the three index references are unique
				//then compute the new values for this atom.
				GuessInit(MainData, i, true);
			}
		}
		ConnectionAtoms[NewPosition*3] = i0;
		ConnectionAtoms[NewPosition*3+1] = i1;
		ConnectionAtoms[NewPosition*3+2] = i2;
		Values[NewPosition*3] = v0;
		Values[NewPosition*3+1] = v1;
		Values[NewPosition*3+2] = v2;
		Type[NewPosition*3] = t0;
		Type[NewPosition*3+1] = t1;
		Type[NewPosition*3+2] = t2;
		//Now update references for atoms higher up in the list
		for (long i=(NewPosition+1); (i*3)<Count; i++) {
			for (long j=0; j<3; j++) {
				if (ConnectionAtoms[3*i+j] == OldPosition)
					ConnectionAtoms[3*i+j] = NewPosition;
				else if ((ConnectionAtoms[3*i+j] <= NewPosition) &&
						 (ConnectionAtoms[3*i+j] > OldPosition))
					ConnectionAtoms[3*i+j]--;
			}
		}
	}
}
