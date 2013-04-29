/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*

		Orbitals.cpp
		
		Functions relating to read/writing orbital information
		and for producing orbital and TED grids.
		
		MEP code seperated out - BMB 1/1998
		Removed window dependancies - BMB 4/1998
		Upped the MP thread stack size to 16KB - BMB 9/2000
		Fixed Vec import for more than 500 basis functions - BMB 12/2002
*/

#include "Globals.h"
#include "Progress.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "BasisSet.h"
#include "GlobalExceptions.h"
#include "SurfaceTypes.h"
#include "Math3D.h"
#include <stdio.h>
#include <string.h>
#if defined(powerc) && defined(MacintoshBuild)
#include <Multiprocessing.h>
extern Boolean		gMPAware;
extern long			gNumProcessors;
#endif
#ifdef __wxBuild__
#include <wx/thread.h>
#include <wx/stopwatch.h>
#endif
#include <iostream>

float CalculateMOAmplitude(float X_value, float Y_value, float Z_value, mpAtom *Atoms,
		BasisSet *Basis, float *MOVector, long NumAtoms, bool UseSphericalHarmonics);
void CalculateAOAmplitudeVector(float X_value, float Y_value, float Z_value, mpAtom *Atoms,
		BasisSet *Basis, float *AOVector, long NumAtoms);
/** Setup a pair of filtering arrays indicating whether there are non-zero MO Vector elements for
 each shell and atom.
 */
void SetupMOScreens(const BasisSet * Basis, const float * MOVector, std::vector<bool> & AtomScreen,
					std::vector<bool> & ShellScreen, long NumAtoms, bool UseSphericalHarmonics);
	
const char * ConvertTypeOfWavefunction(const TypeOfWavefunction & t) {
	switch (t) {
		case RHF:
			return "RHF";
		case UHF:
			return "UHF";
		case ROHF:
			return "ROHF";
		case GVB:
			return "GVB";
		case MCSCF:
			return "MCSCF";
		case CI:
			return "CI";
		case RHFMP2:
			return "RMP2";
		case TDDFT:
			return "TDDFT";
		case EOM_CC:
			return "EOM_CC";
		default:
			return "Unknown";
	}
}
bool ConvertTypeOfWavefunction(const char * s, TypeOfWavefunction & t) {
	if (!s || !*s) return false;
	for (int i = 1; i != (int) NumberWavefunctionTypes; ++i) {
		if (strcmp(s, ConvertTypeOfWavefunction((TypeOfWavefunction) i)) == 0) {
			t = (TypeOfWavefunction) i;
			return true;
		}
	}
	return false;
}
const char * ConvertTypeOfOrbital(const TypeOfOrbital & t) {
	switch (t) {
		case OptimizedOrbital:
			return "Optimized";
		case NaturalOrbital:
			return "Natural";
		case LocalizedOrbital:
			return "Localized";
		case RuedenbergLocalizedOrbital:
			return "Ruedenberg Localized";
		case BoysLocalizedOrbital:
			return "Boys Localized";
		case OrientedLocalizedOrbital:
			return "Oriented Localized";
		case GuessOrbital:
			return "Initial Guess";
		case DiabaticMolecularOrbital:
			return "CAS-SCF Diabatic";
		default:
			return "Unknown";
	}
}
bool ConvertTypeOfOrbital(const char * s, TypeOfOrbital & t) {
	if (!s || !*s) return false;
	for (int i = 1; i != (int) NumOrbitalTypes; ++i) {
		if (strcmp(s, ConvertTypeOfOrbital((TypeOfOrbital) i)) == 0) {
			t = (TypeOfOrbital) i;
			return true;
		}
	}
	return false;
}

//	Orbital class functions
//		Constructors and destructor
OrbitalRec::OrbitalRec(long nAlphaOrbs, long nBetaOrbs, long nBasis) {

		Vectors = NULL;
		VectorsB = NULL;
		Energy = NULL;
		EnergyB = NULL;
		SymType = NULL;
		SymTypeB = NULL;
		OrbOccupation = OrbOccupationB = NULL;
		Label = NULL;
		TotalAODensity = NULL;
	if (nAlphaOrbs>0) {
		Vectors = new float[nAlphaOrbs*nBasis];
		Energy = new float[nAlphaOrbs];
		SymType = new char[nAlphaOrbs*5];
		if ((!Vectors)||(!Energy)||(!SymType)) throw MemoryError();
	}
	if (nBetaOrbs > 0) {
		VectorsB = new float[nBetaOrbs*nBasis];
		EnergyB = new float[nBetaOrbs];
		SymTypeB = new char[nBetaOrbs*5];
		if ((!VectorsB)||(!EnergyB)||(!SymTypeB)) throw MemoryError();
	}
	NumAlphaOrbs = nAlphaOrbs;
	NumBetaOrbs = nBetaOrbs;
	NumOccupiedAlphaOrbs = NumOccupiedBetaOrbs = 0;
	NumBasisFunctions = nBasis;
	OrbitalType = unknownOrbitalType;
	BaseWavefunction = unknownWavefunction;
}
OrbitalRec::OrbitalRec(BufferFile *Buffer, long code, long length) {
		//copy the data from the buffer
	Vectors = VectorsB = Energy = EnergyB = NULL;
	OrbOccupation = OrbOccupationB = NULL;
	SymType = SymTypeB = NULL;
	TotalAODensity = NULL;
	NumOccupiedAlphaOrbs = NumOccupiedBetaOrbs = 0;
	NumBasisFunctions = 0;
	Label = NULL;
	OrbitalType = unknownOrbitalType;
	BaseWavefunction = unknownWavefunction;

		long total = 0, OldOrbType;
	if (code == 1) {
		total = Buffer->BufferSkip(8*sizeof(long));
		total += Buffer->Read((Ptr) &NumAlphaOrbs, sizeof(long));
		total += Buffer->Read((Ptr) &NumBetaOrbs, sizeof(long));
		total += Buffer->Read((Ptr) &OldOrbType, sizeof(long));
	} else if (code == 11) {
		total = Buffer->Read((Ptr) &NumAlphaOrbs, sizeof(long));
		total += Buffer->Read((Ptr) &NumBetaOrbs, sizeof(long));
		total += Buffer->Read((Ptr) &OldOrbType, sizeof(long));
	} else if (code == 12) {
		total = Buffer->Read((Ptr) &NumAlphaOrbs, sizeof(long));
		total += Buffer->Read((Ptr) &NumBetaOrbs, sizeof(long));
		total += Buffer->Read((Ptr) &BaseWavefunction, sizeof(long));
		total += Buffer->Read((Ptr) &OrbitalType, sizeof(long));
	} else throw DataError();
	Buffer->BufferSkip(length - total);
	if ((code==1)||(code==11)) {	//convert old OrbType to separate variables
		BaseWavefunction = RHF;
		OrbitalType = OptimizedOrbital;
		switch (BaseWavefunction) {
			case 2:
				BaseWavefunction = UHF;
			break;
			case 3:
				BaseWavefunction = ROHF;
			break;
			case 4:
				BaseWavefunction = GVB;
			break;
			case 5:
				BaseWavefunction = MCSCF;
			break;
			case 6:
				BaseWavefunction = CI;
			break;
			case 7:
				BaseWavefunction = RHFMP2;
				OrbitalType = NaturalOrbital;
			break;
			case 8:
				BaseWavefunction = UHF;
				OrbitalType = NaturalOrbital;
			break;
			case 9:
				BaseWavefunction = MCSCF;
				OrbitalType = NaturalOrbital;
			break;
		}
	}
}
OrbitalRec::~OrbitalRec(void) {
	if (Vectors) delete [] Vectors;
	if (VectorsB) delete [] VectorsB;
	if (Energy) delete [] Energy;
	if (EnergyB) delete [] EnergyB;
	if (OrbOccupation) delete [] OrbOccupation;
	if (OrbOccupationB) delete [] OrbOccupationB;
	if (SymType) delete [] SymType;
	if (SymTypeB) delete [] SymTypeB;
	if (TotalAODensity) delete TotalAODensity;
	if (Label) delete [] Label;
}
void OrbitalRec::ReSize(long nAlphaOrbs, long nBetaOrbs) {
	//You can adjust the # of vectors, but not the # of basis functions
	if (NumBasisFunctions == 0) {//This shouldn't happen
		return;
	}
	if (NumAlphaOrbs != nAlphaOrbs) {	//Need to change the size of the alpha arrays
		if (nAlphaOrbs <= 0) {
			if (Vectors) { delete Vectors;	Vectors = NULL;}
			if (Energy) { delete Energy;	Energy = NULL;}
			if (SymType) { delete SymType;	SymType = NULL;}
		} else {
			float *tempVectors = new float[nAlphaOrbs*NumBasisFunctions];
			if (tempVectors) {
				memcpy(tempVectors, Vectors, nAlphaOrbs*NumBasisFunctions*sizeof(float));
				delete [] Vectors;
				Vectors = tempVectors;
			} else throw MemoryError();
			if (Energy) {
				float *tempEnergy = new float[nAlphaOrbs];
				if (tempEnergy) {
					memcpy(tempEnergy, Energy, nAlphaOrbs*sizeof(float));
					delete [] Energy;
					Energy = tempEnergy;
				} else throw MemoryError();
			}
			if (SymType) {	//MC natural orbitals don't have sym labels
				char *tempSymType = new char[nAlphaOrbs*5];
				if (tempSymType) {
					memcpy(tempSymType, SymType, nAlphaOrbs*5*sizeof(char));
					delete [] SymType;
					SymType = tempSymType;
				} else throw MemoryError();
			}
		}
		NumAlphaOrbs = nAlphaOrbs;
	}
	if (NumBetaOrbs != nBetaOrbs) {	//Need to change the size of the beta arrays
		if (nBetaOrbs <= 0) {
			if (VectorsB) { delete VectorsB;	VectorsB = NULL;}
			if (EnergyB) { delete EnergyB;	EnergyB = NULL;}
			if (SymTypeB) { delete SymTypeB;	SymTypeB = NULL;}
		} else {
			float *tempVectors = new float[nBetaOrbs*NumBasisFunctions];
			if (tempVectors) {
				memcpy(tempVectors, VectorsB, nBetaOrbs*NumBasisFunctions*sizeof(float));
				delete [] VectorsB;
				VectorsB = tempVectors;
			} else throw MemoryError();
			float *tempEnergy = new float[nBetaOrbs];
			if (EnergyB) {
				if (tempEnergy) {
					memcpy(tempEnergy, EnergyB, nBetaOrbs*sizeof(float));
					delete [] EnergyB;
					EnergyB = tempEnergy;
				} else throw MemoryError();
			}
			if (SymTypeB) {
				char *tempSymType = new char[nBetaOrbs*5];
				if (tempSymType) {
					memcpy(tempSymType, SymTypeB, nBetaOrbs*5*sizeof(char));
					delete [] SymTypeB;
					SymTypeB = tempSymType;
				} else throw MemoryError();
			}
		}
		NumBetaOrbs = nBetaOrbs;
	}
}

//		IO routines
//Call with the file positioned to the $Vec line
void OrbitalRec::ReadVecGroup(BufferFile * Buffer, const long & NumBasisFuncs, const TypeOfWavefunction & Type) {
		char	Line[kMaxLineLength], itext[4], jtext[4];
//Read $vec label lines
	NumBasisFunctions = NumBasisFuncs;
	long VecPos = Buffer->GetFilePos() - 2;
	Buffer->BackupnLines(3);
	long back3Pos = Buffer->GetFilePos();
		//scan the up to 3 lines
	while (VecPos - Buffer->GetFilePos() > 0) {
		Buffer->GetLine(Line);	//actually looking for $end from a previous group
		if (-1 < FindKeyWord(Line, "$END", 4))
			back3Pos = Buffer->GetFilePos();
	}
	long LabelLength = VecPos - back3Pos + 2;
	if (LabelLength > 1) {
		Label = new char[LabelLength];
		if (!Label) throw MemoryError();
		Buffer->SetFilePos(back3Pos);
		Buffer->Read((Ptr) Label, LabelLength-1);	//note: buffer position should be back at $vec
		Label[LabelLength-1] = 0;	//make it a proper C string
	}
//read in the vectors in the GAMESS format (I2,I3,5E15.8)
	Buffer->SetFilePos(VecPos+2);
	Buffer->GetLine(Line);	//confirm position of $vec
	if (-1 >= FindKeyWord(Line, "$VEC", 4)) throw DataError();
		long ivec, nVec, nOrbs,n, nAlpha=0, nBeta=0, nScan, nLine, OrbCount;
		int	i,j,linevec;
	float * Vector = Vectors;

	for (int iPass=0; iPass<2; iPass++) {
		if (!Vector) throw MemoryError();
		nOrbs = 0;
		nVec = 0;
		OrbCount = 0;
		for (; nOrbs<NumBasisFuncs; nOrbs++) {	//loop over orbitals, can be up to NumBasisFuncs
			nLine = 1;							//but may be limited to Occupied Orbs
			Buffer->GetLine(Line); n=5, linevec=0;
				//If we have hit a $end then terminate
			if (-1<FindKeyWord(Line, "$END", 4)) break;
			OrbCount++;
			if (OrbCount>99) OrbCount = 0;	//Account for the fact that GAMESS doesn't correctly punch orb # when > 100 (%2d)
			itext[0] = Line[0], itext[1]=Line[1], itext[2]='\0';
			jtext[0] = Line[2], jtext[1]=Line[3], jtext[2]=Line[4], jtext[3]='\0';
			nScan = sscanf(itext, "%d", &i);
			nScan += sscanf(jtext, "%d", &j);
				//The above 4 lines accomplish the following correctly since scanf consumes leading whitespace
	//		nScan = sscanf(Line, "%2d%3d", &i,&j);
			if ((i!=OrbCount)&&(i==1)) break;	//Reached start of beta set of UHF orbs
			if ((i!=OrbCount)||(j!=nLine)||(nScan!=2)) throw DataError();
			for (ivec=0; ivec<NumBasisFuncs; ivec++) {	//Loop over components of orbitals vector, must be NumBasisFuncs long
				if(linevec>=5) {	//5 values per line
					Buffer->GetLine(Line); n=5; linevec=0; nLine++;
					if (nLine>999) nLine = 0;	//That's a big vec!
					itext[0] = Line[0], itext[1]=Line[1], itext[2]='\0';
					jtext[0] = Line[2], jtext[1]=Line[3], jtext[2]=Line[4], jtext[3]='\0';
					nScan = sscanf(itext, "%d", &i);
					nScan += sscanf(jtext, "%d", &j);
					if ((i!=OrbCount)||(j!=nLine)||(nScan!=2)) throw DataError();
				}
				nScan = sscanf(&(Line[n]), "%15e", &(Vector[nVec]));
				if (nScan!=1) throw DataError();
				n+=15;
				nVec++;
				linevec++;
			}
		}
		if ((Type!=1)&&(iPass==0)) {
			nAlpha = nOrbs;
		} else nBeta = nOrbs;
		if (Type == UHF) {
			Vector = VectorsB;
			if (nOrbs < NumBasisFuncs) Buffer->BackupnLines(1);
		} else iPass++;
	}
// resize in case only the occupied orbs were present
	ReSize(nAlpha, nBeta);
	setOrbitalWavefunctionType(Type);
	if ((Type == UHF)&&(-1 < FindKeyWord(Label, "UHF NATURAL ORBITALS", 20))) {
		Buffer->SetFilePos(VecPos);
		ReadUHFVECOccupancies(*Buffer);
	}
	if (Type != 1) {
		if (Energy) { delete [] Energy; Energy = NULL;}
		if (SymType) { delete [] SymType; SymType = NULL;}
	}
	if (Type != 0) {
		if (EnergyB) {delete [] EnergyB; EnergyB = NULL;}
		if (SymTypeB) {delete [] SymTypeB; SymTypeB = NULL;}
	}
}
//This function must be called at the start of the $VEC line
void OrbitalRec::ReadUHFVECOccupancies(BufferFile & Buffer) {
	long VecPos = Buffer.GetFilePos();
	long expected = (NumAlphaOrbs/8) + 6;
	Buffer.BackupnLines(expected);
	
	if (Buffer.LocateKeyWord("UHF OCCUPATION NUMBERS", 22, VecPos)) {
		if (OrbOccupation) delete [] OrbOccupation;
		OrbOccupation = new float[NumAlphaOrbs];
		Buffer.SkipnLines(1);
		
		long lineocc=0, linepos=0;
		int	nChar;
		char	Line[kMaxLineLength];
		Buffer.GetLine(Line);
		for (long i=0; i<NumAlphaOrbs; i++) {
			if (lineocc>=8) {//should be 8 values per line
				Buffer.GetLine(Line);
				lineocc=0;
				linepos=0;
			}
			int nScan = sscanf(&(Line[linepos]), "%f%n", &(OrbOccupation[i]), &nChar);
			if (nScan!=1) throw DataError();
			linepos += nChar;
			lineocc++;
		}
		setOrbitalType(NaturalOrbital);
	}
}
void OrbitalRec::setLabel(const char * str) {
	if (Label) {
		delete [] Label;
		Label = NULL;
	}
	if (str) {
		int len = strlen(str);
		Label = new char[len+1];
		strncpy(Label, str, len);
	}
}
void OrbitalRec::RotateVectors(Matrix4D rotationMatrix, BasisSet * Basis, long NumAtoms) {
	std::vector<BasisShell> & Shells=Basis->Shells;

		//Set up normalization constants for d,f,g functions
	float sqrt3 = sqrt(3.0);
	float onedsqrt3 = 1/sqrt3;
	float sqrt5 = sqrt(5.0);
	float sqrt7 = sqrt(7.0);
	long NumOrbs = NumAlphaOrbs;
	long NumBasisFuncs = Basis->GetNumBasisFuncs(false);
	float * BaseVector = Vectors;
	for (int j=0; j<2; j++) {
		if (!BaseVector) continue;
		for (long i=0; i<NumOrbs; i++) {
			float * MOVector = &(BaseVector[i*NumBasisFuncs]);
			long ivec = 0;
						//loop over the atoms/shells
			for (long iatom=0; iatom<NumAtoms; iatom++) {
				for (long ishell=Basis->BasisMap[2*iatom]; ishell<=Basis->BasisMap[2*iatom+1]; ishell++) {
						//Now multiply by the appropriate x, y, z factors
						float * OrigVec=NULL;
					switch (Shells[ishell].ShellType) {
						case SShell:	//No rotation needed!
							ivec++;
						break;
						case LShell:
							ivec++;	//Skip the S portion
						case PShell:
							{
							CPoint3D PVec, resultVec;
							PVec.x = MOVector[ivec];
							PVec.y = MOVector[ivec+1];
							PVec.z = MOVector[ivec+2];
							Rotate3DOffset(rotationMatrix, PVec, &resultVec);
							MOVector[ivec] = resultVec.x;
							MOVector[ivec+1] = resultVec.y;
							MOVector[ivec+2] = resultVec.z;
							ivec += 3;
							}
						break;
							//D, F, and G shells require higher order tensors. For simplicity the
							//resulting multiplications are expanded out below.
						case DShell:
							OrigVec = new float[6];
							if (!OrigVec) throw MemoryError();
							OrigVec[0] = MOVector[ivec];
							OrigVec[1] = MOVector[ivec+1];
							OrigVec[2] = MOVector[ivec+2];
							OrigVec[3] = MOVector[ivec+3]*sqrt3;
							OrigVec[4] = MOVector[ivec+4]*sqrt3;
							OrigVec[5] = MOVector[ivec+5]*sqrt3;
							MOVector[ivec] = OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][0] +
											OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][0] +
											OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][0] +
											OrigVec[3]*rotationMatrix[0][0]*rotationMatrix[1][0] +
											OrigVec[4]*rotationMatrix[0][0]*rotationMatrix[2][0] +
											OrigVec[5]*rotationMatrix[1][0]*rotationMatrix[2][0];
							MOVector[ivec+1] = OrigVec[0]*rotationMatrix[0][1]*rotationMatrix[0][1] +
											OrigVec[1]*rotationMatrix[1][1]*rotationMatrix[1][1] +
											OrigVec[2]*rotationMatrix[2][1]*rotationMatrix[2][1] +
											OrigVec[3]*rotationMatrix[0][1]*rotationMatrix[1][1] +
											OrigVec[4]*rotationMatrix[0][1]*rotationMatrix[2][1] +
											OrigVec[5]*rotationMatrix[1][1]*rotationMatrix[2][1];
							MOVector[ivec+2] = OrigVec[0]*rotationMatrix[0][2]*rotationMatrix[0][2] +
											OrigVec[1]*rotationMatrix[1][2]*rotationMatrix[1][2] +
											OrigVec[2]*rotationMatrix[2][2]*rotationMatrix[2][2] +
											OrigVec[3]*rotationMatrix[0][2]*rotationMatrix[1][2] +
											OrigVec[4]*rotationMatrix[0][2]*rotationMatrix[2][2] +
											OrigVec[5]*rotationMatrix[1][2]*rotationMatrix[2][2];
							MOVector[ivec+3] = (OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][1] +
											OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][1] +
											OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][1] +
											OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[0][1]) +
											OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[0][1]) +
											OrigVec[5]*(rotationMatrix[1][0]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[1][1])) * onedsqrt3;
							MOVector[ivec+4] = (OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][2] +
											OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][2] +
											OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][2] +
											OrigVec[3]*(rotationMatrix[1][0]*rotationMatrix[0][2] + rotationMatrix[0][0]*rotationMatrix[1][2]) +
											OrigVec[4]*(rotationMatrix[2][0]*rotationMatrix[0][2] + rotationMatrix[0][0]*rotationMatrix[2][2]) +
											OrigVec[5]*(rotationMatrix[2][0]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[2][2])) * onedsqrt3;
							MOVector[ivec+5] = (OrigVec[0]*rotationMatrix[0][1]*rotationMatrix[0][2] +
											OrigVec[1]*rotationMatrix[1][1]*rotationMatrix[1][2] +
											OrigVec[2]*rotationMatrix[2][1]*rotationMatrix[2][2] +
											OrigVec[3]*(rotationMatrix[1][1]*rotationMatrix[0][2] + rotationMatrix[0][1]*rotationMatrix[1][2]) +
											OrigVec[4]*(rotationMatrix[2][1]*rotationMatrix[0][2] + rotationMatrix[0][1]*rotationMatrix[2][2]) +
											OrigVec[5]*(rotationMatrix[2][1]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[2][2])) * onedsqrt3;
							ivec += 6;
						break;
						case FShell:
							OrigVec = new float[10];
							if (!OrigVec) throw MemoryError();
							OrigVec[0] = MOVector[ivec];
							OrigVec[1] = MOVector[ivec+1];
							OrigVec[2] = MOVector[ivec+2];
							OrigVec[3] = MOVector[ivec+3]*sqrt5;
							OrigVec[4] = MOVector[ivec+4]*sqrt5;
							OrigVec[5] = MOVector[ivec+5]*sqrt5;
							OrigVec[6] = MOVector[ivec+6]*sqrt5;
							OrigVec[7] = MOVector[ivec+7]*sqrt5;
							OrigVec[8] = MOVector[ivec+8]*sqrt5;
							OrigVec[9] = MOVector[ivec+9]*sqrt5*sqrt3;
							MOVector[ivec] = OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][0] +
											OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][0] +
											OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][0] +
											OrigVec[3]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][0] +
											OrigVec[4]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][0] +
											OrigVec[5]*rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][0] +
											OrigVec[6]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][0] +
											OrigVec[7]*rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][0] +
											OrigVec[8]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][0] +
											OrigVec[9]*rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][0];
							MOVector[ivec+1] = OrigVec[0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][1] +
											OrigVec[1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][1] +
											OrigVec[2]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][1] +
											OrigVec[3]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][1] +
											OrigVec[4]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][1] +
											OrigVec[5]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][1] +
											OrigVec[6]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][1] +
											OrigVec[7]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[2][1] +
											OrigVec[8]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][1] +
											OrigVec[9]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][1];
							MOVector[ivec+2] = OrigVec[0]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[0][2] +
											OrigVec[1]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[1][2] +
											OrigVec[2]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[2][2] +
											OrigVec[3]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[1][2] +
											OrigVec[4]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[2][2] +
											OrigVec[5]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[1][2] +
											OrigVec[6]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[2][2] +
											OrigVec[7]*rotationMatrix[0][2]*rotationMatrix[2][2]*rotationMatrix[2][2] +
											OrigVec[8]*rotationMatrix[1][2]*rotationMatrix[2][2]*rotationMatrix[2][2] +
											OrigVec[9]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[2][2];
							MOVector[ivec+3] = (OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][1] +
											OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][1] +
											OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][1] +
											OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][1] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[0][1]) +
											OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][1] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[0][1]) +
											OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[0][1]) +
											OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][1] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[1][1]) +
											OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][1]) +
											OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[1][1]) +
											OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][1] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[1][1] + rotationMatrix[2][0]*rotationMatrix[1][0]*rotationMatrix[0][1]))/sqrt5;
							MOVector[ivec+4] = (OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][2] +
											OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][2] +
											OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][2] +
											OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][2] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[0][2]) +
											OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[0][2]) +
											OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[0][2]) +
											OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[1][2]) +
											OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][2]) +
											OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[1][2]) +
											OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[1][2] + rotationMatrix[2][0]*rotationMatrix[1][0]*rotationMatrix[0][2]))/sqrt5;
							MOVector[ivec+5] = (OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][1] +
											OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][1] +
											OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][1] +
											OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][1]) +
											OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][1]) +
											OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[0][1]) +
											OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][1]) +
											OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[2][1]) +
											OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[2][1]) +
											OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[2][1] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[2][1] + rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[0][1]))/sqrt5;
							MOVector[ivec+6] = (OrigVec[0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][2] +
											OrigVec[1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][2] +
											OrigVec[2]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][2] +
											OrigVec[3]*(rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[0][1]*rotationMatrix[0][2]) +
											OrigVec[4]*(rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[0][1]*rotationMatrix[0][2]) +
											OrigVec[5]*(rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[0][2]) +
											OrigVec[6]*(rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[1][1]*rotationMatrix[1][2]) +
											OrigVec[7]*(rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[0][2]) +
											OrigVec[8]*(rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[1][2]) +
											OrigVec[9]*(rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[0][2] + rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[1][2]))/sqrt5;
							MOVector[ivec+7] = (OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[0][2] +
											OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[1][2] +
											OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][2]*rotationMatrix[2][2] +
											OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[0][2]*rotationMatrix[0][2]) +
											OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[0][2]*rotationMatrix[0][2]) +
											OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[0][2]) +
											OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[1][2]*rotationMatrix[1][2]) +
											OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][2]*rotationMatrix[0][2]) +
											OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][2]*rotationMatrix[1][2]) +
											OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[2][2]*rotationMatrix[0][2] + rotationMatrix[2][0]*rotationMatrix[0][2]*rotationMatrix[1][2]))/sqrt5;
							MOVector[ivec+8] = (OrigVec[0]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[0][2] +
											OrigVec[1]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[1][2] +
											OrigVec[2]*rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[2][2] +
											OrigVec[3]*(rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[0][2]) +
											OrigVec[4]*(rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[0][2]) +
											OrigVec[5]*(rotationMatrix[0][1]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[0][2]) +
											OrigVec[6]*(rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[1][2]*rotationMatrix[1][2]) +
											OrigVec[7]*(rotationMatrix[0][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[0][2]) +
											OrigVec[8]*(rotationMatrix[1][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[1][2]) +
											OrigVec[9]*(rotationMatrix[0][1]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[1][1]*rotationMatrix[2][2]*rotationMatrix[0][2] + rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[1][2]))/sqrt5;
							MOVector[ivec+9] = (OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][2] +
											OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][2] +
											OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][2] +
											OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][2] + rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[0][2]) +
											OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[0][2] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][2]) +
											OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[0][2]) +
											OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[1][2] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][2]) +
											OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[0][2]) +
											OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[1][2]) +
											OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[0][2] +
												rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[1][2] + rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[0][2]))/(sqrt5*sqrt3);
							ivec += 10;
						break;
						case GShell:
							OrigVec = new float[15];
							if (!OrigVec) throw MemoryError();
							OrigVec[0] = MOVector[ivec];
							OrigVec[1] = MOVector[ivec+1];
							OrigVec[2] = MOVector[ivec+2];
							OrigVec[3] = MOVector[ivec+3]*sqrt7;
							OrigVec[4] = MOVector[ivec+4]*sqrt7;
							OrigVec[5] = MOVector[ivec+5]*sqrt7;
							OrigVec[6] = MOVector[ivec+6]*sqrt7;
							OrigVec[7] = MOVector[ivec+7]*sqrt7;
							OrigVec[8] = MOVector[ivec+8]*sqrt7;
							OrigVec[9] = MOVector[ivec+9]*sqrt7*sqrt5*onedsqrt3;
							OrigVec[10] = MOVector[ivec+10]*sqrt7*sqrt5*onedsqrt3;
							OrigVec[11] = MOVector[ivec+11]*sqrt7*sqrt5*onedsqrt3;
							OrigVec[12] = MOVector[ivec+12]*sqrt7*sqrt5;
							OrigVec[13] = MOVector[ivec+13]*sqrt7*sqrt5;
							OrigVec[14] = MOVector[ivec+14]*sqrt7*sqrt5;
							MOVector[ivec] = OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][0];
							MOVector[ivec] += OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][0];
							MOVector[ivec] += OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][0];
							MOVector[ivec] += OrigVec[3]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][0];
							MOVector[ivec] += OrigVec[4]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][0];
							MOVector[ivec] += OrigVec[5]*rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][0];
							MOVector[ivec] += OrigVec[6]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][0];
							MOVector[ivec] += OrigVec[7]*rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][0];
							MOVector[ivec] += OrigVec[8]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][0];
							MOVector[ivec] += OrigVec[9]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][0];
							MOVector[ivec] += OrigVec[10]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][0];
							MOVector[ivec] += OrigVec[11]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][0];
							MOVector[ivec] += OrigVec[12]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][0];
							MOVector[ivec] += OrigVec[13]*rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][0];
							MOVector[ivec] += OrigVec[14]*rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][0];
							MOVector[ivec+1] = OrigVec[0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][1];
							MOVector[ivec+1] += OrigVec[1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][1];
							MOVector[ivec+1] += OrigVec[2]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][1];
							MOVector[ivec+1] += OrigVec[3]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][1];
							MOVector[ivec+1] += OrigVec[4]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][1];
							MOVector[ivec+1] += OrigVec[5]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][1];
							MOVector[ivec+1] += OrigVec[6]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][1];
							MOVector[ivec+1] += OrigVec[7]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][1];
							MOVector[ivec+1] += OrigVec[8]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][1];
							MOVector[ivec+1] += OrigVec[9]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][1];
							MOVector[ivec+1] += OrigVec[10]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[2][1];
							MOVector[ivec+1] += OrigVec[11]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][1];
							MOVector[ivec+1] += OrigVec[12]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][1];
							MOVector[ivec+1] += OrigVec[13]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][1];
							MOVector[ivec+1] += OrigVec[14]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][1];
							MOVector[ivec+2] = OrigVec[0]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[0][2];
							MOVector[ivec+2] += OrigVec[1]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[1][2];
							MOVector[ivec+2] += OrigVec[2]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[2][2];
							MOVector[ivec+2] += OrigVec[3]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[1][2];
							MOVector[ivec+2] += OrigVec[4]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[2][2];
							MOVector[ivec+2] += OrigVec[5]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[1][2];
							MOVector[ivec+2] += OrigVec[6]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[2][2];
							MOVector[ivec+2] += OrigVec[7]*rotationMatrix[0][2]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[2][2];
							MOVector[ivec+2] += OrigVec[8]*rotationMatrix[1][2]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[2][2];
							MOVector[ivec+2] += OrigVec[9]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[1][2];
							MOVector[ivec+2] += OrigVec[10]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[2][2]*rotationMatrix[2][2];
							MOVector[ivec+2] += OrigVec[11]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[2][2]*rotationMatrix[2][2];
							MOVector[ivec+2] += OrigVec[12]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[2][2];
							MOVector[ivec+2] += OrigVec[13]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[2][2];
							MOVector[ivec+2] += OrigVec[14]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[2][2]*rotationMatrix[2][2];
							MOVector[ivec+3] = OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][1];
							MOVector[ivec+3] += OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][1];
							MOVector[ivec+3] += OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][1];
							MOVector[ivec+3] += OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][1] + rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[0][1]);
							MOVector[ivec+3] += OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][1] + rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[0][1]);
							MOVector[ivec+3] += OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[0][1]);
							MOVector[ivec+3] += OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][1] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[1][1]);
							MOVector[ivec+3] += OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][1]);
							MOVector[ivec+3] += OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[1][1]);
							MOVector[ivec+3] += OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][1] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[0][1]);
							MOVector[ivec+3] += OrigVec[10]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][1] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][1]);
							MOVector[ivec+3] += OrigVec[11]*(rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][1] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[1][1]);
							MOVector[ivec+3] += OrigVec[12]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][1] + rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[1][1] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[0][1]);
							MOVector[ivec+3] += OrigVec[13]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][1] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[0][1]);
							MOVector[ivec+3] += OrigVec[14]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][1] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][1]);
							MOVector[ivec+3] /= sqrt7;
							MOVector[ivec+4] = OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][2];
							MOVector[ivec+4] += OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][2];
							MOVector[ivec+4] += OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][2];
							MOVector[ivec+4] += OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][2] + rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[0][2]);
							MOVector[ivec+4] += OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[0][2]);
							MOVector[ivec+4] += OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[0][2]);
							MOVector[ivec+4] += OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[1][2]);
							MOVector[ivec+4] += OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][2]);
							MOVector[ivec+4] += OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[1][2]);
							MOVector[ivec+4] += OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][2] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[0][2]);
							MOVector[ivec+4] += OrigVec[10]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][2]);
							MOVector[ivec+4] += OrigVec[11]*(rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[1][2]);
							MOVector[ivec+4] += OrigVec[12]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[1][2] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[0][2]);
							MOVector[ivec+4] += OrigVec[13]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[0][2]);
							MOVector[ivec+4] += OrigVec[14]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][2]);
							MOVector[ivec+4] /= sqrt7;
							MOVector[ivec+5] = OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][1];
							MOVector[ivec+5] += OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][1];
							MOVector[ivec+5] += OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][1];
							MOVector[ivec+5] += OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][1]);
							MOVector[ivec+5] += OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][1]);
							MOVector[ivec+5] += OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[0][1]);
							MOVector[ivec+5] += OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][1]);
							MOVector[ivec+5] += OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[0][1]);
							MOVector[ivec+5] += OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[1][1]);
							MOVector[ivec+5] += OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][1]);
							MOVector[ivec+5] += OrigVec[10]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][1]);
							MOVector[ivec+5] += OrigVec[11]*(rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][1]);
							MOVector[ivec+5] += OrigVec[12]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][1] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][1]);
							MOVector[ivec+5] += OrigVec[13]*(rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][1] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][1]);
							MOVector[ivec+5] += OrigVec[14]*(rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][1] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][1]);
							MOVector[ivec+5] /= sqrt7;
							MOVector[ivec+6] = OrigVec[0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][2];
							MOVector[ivec+6] += OrigVec[1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][2];
							MOVector[ivec+6] += OrigVec[2]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][2];
							MOVector[ivec+6] += OrigVec[3]*(rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][2] + rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[0][2]);
							MOVector[ivec+6] += OrigVec[4]*(rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][2] + rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[0][2]);
							MOVector[ivec+6] += OrigVec[5]*(rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[0][2]);
							MOVector[ivec+6] += OrigVec[6]*(rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][2]);
							MOVector[ivec+6] += OrigVec[7]*(rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[0][2]);
							MOVector[ivec+6] += OrigVec[8]*(rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[1][2]);
							MOVector[ivec+6] += OrigVec[9]*(rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[0][1]*rotationMatrix[0][2]);
							MOVector[ivec+6] += OrigVec[10]*(rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[0][1]*rotationMatrix[0][2]);
							MOVector[ivec+6] += OrigVec[11]*(rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[1][1]*rotationMatrix[1][2]);
							MOVector[ivec+6] += OrigVec[12]*(rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[1][2] + rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[0][2]);
							MOVector[ivec+6] += OrigVec[13]*(rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[0][2]);
							MOVector[ivec+6] += OrigVec[14]*(rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[0][2]);
							MOVector[ivec+6] /= sqrt7;
							MOVector[ivec+7] = OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[0][2];
							MOVector[ivec+7] += OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[1][2];
							MOVector[ivec+7] += OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[2][2];
							MOVector[ivec+7] += OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+7] += OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+7] += OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[0][2]);
							MOVector[ivec+7] += OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[1][2]);
							MOVector[ivec+7] += OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[0][2]);
							MOVector[ivec+7] += OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[1][2]);
							MOVector[ivec+7] += OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+7] += OrigVec[10]*(rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][2]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+7] += OrigVec[11]*(rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][2]*rotationMatrix[1][2]*rotationMatrix[1][2]);
							MOVector[ivec+7] += OrigVec[12]*(rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[1][2]);
							MOVector[ivec+7] += OrigVec[13]*(rotationMatrix[0][0]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[1][2]);
							MOVector[ivec+7] += OrigVec[14]*(rotationMatrix[0][0]*rotationMatrix[1][2]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[0][2]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[0][2]*rotationMatrix[2][2]*rotationMatrix[1][2]);
							MOVector[ivec+7] /= sqrt7;
							MOVector[ivec+8] = OrigVec[0]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[0][2];
							MOVector[ivec+8] += OrigVec[1]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[1][2];
							MOVector[ivec+8] += OrigVec[2]*rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[2][2];
							MOVector[ivec+8] += OrigVec[3]*(rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+8] += OrigVec[4]*(rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+8] += OrigVec[5]*(rotationMatrix[0][1]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[0][2]);
							MOVector[ivec+8] += OrigVec[6]*(rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[1][2]);
							MOVector[ivec+8] += OrigVec[7]*(rotationMatrix[0][1]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[0][2]);
							MOVector[ivec+8] += OrigVec[8]*(rotationMatrix[1][1]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[2][2]*rotationMatrix[1][2]);
							MOVector[ivec+8] += OrigVec[9]*(rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+8] += OrigVec[10]*(rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+8] += OrigVec[11]*(rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[1][2]*rotationMatrix[1][2]);
							MOVector[ivec+8] += OrigVec[12]*(rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[0][2]*rotationMatrix[1][2]);
							MOVector[ivec+8] += OrigVec[13]*(rotationMatrix[0][1]*rotationMatrix[1][2]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[1][2]*rotationMatrix[1][2]);
							MOVector[ivec+8] += OrigVec[14]*(rotationMatrix[0][1]*rotationMatrix[1][2]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[2][2]*rotationMatrix[1][2]);
							MOVector[ivec+8] /= sqrt7;
							MOVector[ivec+9] = OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][1];
							MOVector[ivec+9] += OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][1];
							MOVector[ivec+9] += OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][1];
							MOVector[ivec+9] += OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][1]);
							MOVector[ivec+9] += OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][1]);
							MOVector[ivec+9] += OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[0][1]);
							MOVector[ivec+9] += OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][1]);
							MOVector[ivec+9] += OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[0][1]);
							MOVector[ivec+9] += OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[1][1]);
							MOVector[ivec+9] += OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][1] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[1][1]);
							MOVector[ivec+9] += OrigVec[10]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][1] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[2][1]);
							MOVector[ivec+9] += OrigVec[11]*(rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[2][1]);
							MOVector[ivec+9] += OrigVec[12]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[2][1] + rotationMatrix[1][0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[2][1] +
								rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[1][1] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][1]);
							MOVector[ivec+9] += OrigVec[13]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[2][1] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[2][1] +
								rotationMatrix[2][0]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[0][1] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][1]);
							MOVector[ivec+9] += OrigVec[14]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[2][1] + rotationMatrix[2][0]*rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[2][1] +
								rotationMatrix[2][0]*rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[1][1] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[1][1]);
							MOVector[ivec+9] /= (sqrt7*sqrt5*onedsqrt3);
							MOVector[ivec+10] = OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[0][2];
							MOVector[ivec+10] += OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[1][2];
							MOVector[ivec+10] += OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][2]*rotationMatrix[2][2];
							MOVector[ivec+10] += OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+10] += OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+10] += OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[0][2]);
							MOVector[ivec+10] += OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[1][2]);
							MOVector[ivec+10] += OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][2]*rotationMatrix[0][2]);
							MOVector[ivec+10] += OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][2]*rotationMatrix[1][2]);
							MOVector[ivec+10] += OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[0][2]*rotationMatrix[0][2] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[0][2]*rotationMatrix[1][2]);
							MOVector[ivec+10] += OrigVec[10]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][2]*rotationMatrix[0][2] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[0][2]*rotationMatrix[2][2]);
							MOVector[ivec+10] += OrigVec[11]*(rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[1][2]*rotationMatrix[2][2]);
							MOVector[ivec+10] += OrigVec[12]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[0][0]*rotationMatrix[0][2]*rotationMatrix[2][2] +
								rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[0][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+10] += OrigVec[13]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[0][2]*rotationMatrix[2][2] +
								rotationMatrix[2][0]*rotationMatrix[1][0]*rotationMatrix[1][2]*rotationMatrix[0][2] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[1][2]*rotationMatrix[1][2]);
							MOVector[ivec+10] += OrigVec[14]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[1][0]*rotationMatrix[0][2]*rotationMatrix[2][2] +
								rotationMatrix[2][0]*rotationMatrix[0][0]*rotationMatrix[2][2]*rotationMatrix[1][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][2]*rotationMatrix[1][2]);
							MOVector[ivec+10] /= (sqrt7*sqrt5*onedsqrt3);
							MOVector[ivec+11] = OrigVec[0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[0][2];
							MOVector[ivec+11] += OrigVec[1]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[1][2];
							MOVector[ivec+11] += OrigVec[2]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[2][2];
							MOVector[ivec+11] += OrigVec[3]*(rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+11] += OrigVec[4]*(rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+11] += OrigVec[5]*(rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[0][2]);
							MOVector[ivec+11] += OrigVec[6]*(rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[1][2]);
							MOVector[ivec+11] += OrigVec[7]*(rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[0][2]);
							MOVector[ivec+11] += OrigVec[8]*(rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[1][2]);
							MOVector[ivec+11] += OrigVec[9]*(rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[0][2] + rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[1][2]);
							MOVector[ivec+11] += OrigVec[10]*(rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[0][2] + rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[2][2]);
							MOVector[ivec+11] += OrigVec[11]*(rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[1][2]*rotationMatrix[2][2]);
							MOVector[ivec+11] += OrigVec[12]*(rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[1][1]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[2][2] +
								rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[1][2] + rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+11] += OrigVec[13]*(rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[2][2] +
								rotationMatrix[2][1]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[0][2] + rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[1][2]*rotationMatrix[1][2]);
							MOVector[ivec+11] += OrigVec[14]*(rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][1]*rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[2][2] +
								rotationMatrix[2][1]*rotationMatrix[0][1]*rotationMatrix[2][2]*rotationMatrix[1][2] + rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[1][2]);
							MOVector[ivec+11] /= (sqrt7*sqrt5*onedsqrt3);
							MOVector[ivec+12] = OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][2];
							MOVector[ivec+12] += OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][2];
							MOVector[ivec+12] += OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][2];
							MOVector[ivec+12] += OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[1][2] + rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[0][2] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][2]);
							MOVector[ivec+12] += OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[0][2] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][2]);
							MOVector[ivec+12] += OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[0][2]);
							MOVector[ivec+12] += OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][2]);
							MOVector[ivec+12] += OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[0][2]);
							MOVector[ivec+12] += OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[1][2]);
							MOVector[ivec+12] += OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[1][2] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[1][2] +
								rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[0][2] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][2]);
							MOVector[ivec+12] += OrigVec[10]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[2][2] +
								rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[0][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][2]);
							MOVector[ivec+12] += OrigVec[11]*(rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[2][2] +
								rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[1][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][2]);
							MOVector[ivec+12] += OrigVec[12]*(rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[1][2] +
								rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[1][2] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[0][2] +
								rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][2]);
							MOVector[ivec+12] += OrigVec[13]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[1][2] +
								rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[0][2] +
								rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[0][2]);
							MOVector[ivec+12] += OrigVec[14]*(rotationMatrix[0][0]*rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[2][2] +
								rotationMatrix[2][0]*rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[0][2] +
								rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[1][2] + rotationMatrix[2][0]*rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[0][2]);
							MOVector[ivec+12] /= (sqrt7*sqrt5);
							MOVector[ivec+13] = OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][2];
							MOVector[ivec+13] += OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][2];
							MOVector[ivec+13] += OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][2];
							MOVector[ivec+13] += OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][2] + rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[0][2] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][2]);
							MOVector[ivec+13] += OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[0][2] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[0][2]);
							MOVector[ivec+13] += OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[0][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[0][2]);
							MOVector[ivec+13] += OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[1][2] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[1][2]);
							MOVector[ivec+13] += OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[0][1]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[0][2]);
							MOVector[ivec+13] += OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[1][2]);
							MOVector[ivec+13] += OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][2] +
								rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[0][2] + rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[0][2]);
							MOVector[ivec+13] += OrigVec[10]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[0][2] +
								rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[0][2] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][2]);
							MOVector[ivec+13] += OrigVec[11]*(rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[1][2] +
								rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[1][2] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][2]);
							MOVector[ivec+13] += OrigVec[12]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[1][2] +
								rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[0][2] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][1]*rotationMatrix[1][2] +
								rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[0][2] + rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[0][2]);
							MOVector[ivec+13] += OrigVec[13]*(rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[1][2] +
								rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[0][2] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][1]*rotationMatrix[0][2] +
								rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[1][2]);
							MOVector[ivec+13] += OrigVec[14]*(rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[1][2] +
								rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[2][1]*rotationMatrix[0][2] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[1][1]*rotationMatrix[2][2] +
								rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[2][1]*rotationMatrix[1][2] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[2][1]*rotationMatrix[0][2]);
							MOVector[ivec+13] /= (sqrt7*sqrt5);
							MOVector[ivec+14] = OrigVec[0]*rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[0][2];
							MOVector[ivec+14] += OrigVec[1]*rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[1][2];
							MOVector[ivec+14] += OrigVec[2]*rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[2][2];
							MOVector[ivec+14] += OrigVec[3]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[1][2] + rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[0][2] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+14] += OrigVec[4]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[0][2] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+14] += OrigVec[5]*(rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[0][2]);
							MOVector[ivec+14] += OrigVec[6]*(rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[1][2]);
							MOVector[ivec+14] += OrigVec[7]*(rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[0][2]);
							MOVector[ivec+14] += OrigVec[8]*(rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[2][2]*rotationMatrix[1][2]);
							MOVector[ivec+14] += OrigVec[9]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[1][2] +
								rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+14] += OrigVec[10]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[2][2] +
								rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[0][2]);
							MOVector[ivec+14] += OrigVec[11]*(rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[1][2]*rotationMatrix[2][2] +
								rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[1][2]*rotationMatrix[1][2]);
							MOVector[ivec+14] += OrigVec[12]*(rotationMatrix[0][0]*rotationMatrix[0][1]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[2][2] +
								rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[0][2] + rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[0][2]*rotationMatrix[1][2] +
								rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[0][2] + rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[1][2]);
							MOVector[ivec+14] += OrigVec[13]*(rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[1][2]*rotationMatrix[1][2] +
								rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[1][2]*rotationMatrix[1][2] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[1][2] + rotationMatrix[1][0]*rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[2][2] +
								rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[1][2]);
							MOVector[ivec+14] += OrigVec[14]*(rotationMatrix[0][0]*rotationMatrix[1][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[0][0]*rotationMatrix[2][1]*rotationMatrix[1][2]*rotationMatrix[2][2] +
								rotationMatrix[1][0]*rotationMatrix[0][1]*rotationMatrix[2][2]*rotationMatrix[2][2] + rotationMatrix[1][0]*rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[2][1]*rotationMatrix[0][2]*rotationMatrix[1][2] +
								rotationMatrix[2][0]*rotationMatrix[0][1]*rotationMatrix[1][2]*rotationMatrix[2][2] + rotationMatrix[2][0]*rotationMatrix[1][1]*rotationMatrix[0][2]*rotationMatrix[2][2]);
							MOVector[ivec+14] /= (sqrt7*sqrt5);
							ivec += 15;
						break;
					}
					if (OrigVec) delete [] OrigVec;
					OrigVec = NULL;
				}
			}
		}
		NumOrbs = NumBetaOrbs;
		BaseVector = VectorsB;
	}
}
//Call to exchange the alpha/beta or natural/optimized orbital sets
void OrbitalRec::ExchangeVectors(void) {
	float * temp;
	temp = Vectors;
	Vectors = VectorsB;
	VectorsB = temp;
	temp = Energy;
	Energy = EnergyB;
	EnergyB = temp;
	temp = OrbOccupation;
	OrbOccupation = OrbOccupationB;
	OrbOccupationB = temp;
	char * tempc = SymType;
	SymType = SymTypeB;
	SymTypeB = tempc;
	long templ = NumAlphaOrbs;
	NumAlphaOrbs = NumBetaOrbs;
	NumBetaOrbs = templ;
}
void OrbitalRec::SetOccupancy(float * Occ, long numVec) {
	if (OrbOccupation) {delete OrbOccupation; OrbOccupation = NULL;}
	if (OrbOccupationB) {delete OrbOccupationB; OrbOccupationB = NULL;}
	OrbOccupation = new float[numVec];
	if (!OrbOccupation) throw MemoryError();
	memcpy(OrbOccupation, Occ, numVec*sizeof(float));
	for (long i=0; i<numVec; i++) OrbOccupation[i] = Occ[i];
}
void OrbitalRec::SetOrbitalOccupancy(const long & alpha, const long & beta) {
	if ((alpha >= 0)&&(alpha <= NumAlphaOrbs)) NumOccupiedAlphaOrbs = alpha;
		//The following doesn't guarentee the beta value is within the limits, but there
		//should never be more beta orbitals than alpha, but ROHF wavefunctions have only
		//one set of vectors, yet have separate occupaction counts.
	if ((beta >= 0)&&(beta <= MAX(NumAlphaOrbs, NumBetaOrbs))) NumOccupiedBetaOrbs = beta;
}
bool OrbitalRec::TotalDensityPossible(void) const {
	return ((getNumOccupiedAlphaOrbitals() > 0)||
			(OrbOccupation != NULL));	//Is this a good enough test?
}

/*
MORec::MORec(void) {
	EigenVectors = LocalOrbitals = OrientedLocalOrbitals = NULL;
	TotalAODensity = NULL;
	NumOccupiedAlphaOrbs = NumOccupiedBetaOrbs = 0;
}
MORec::~MORec(void) {
	if (EigenVectors) delete EigenVectors;
	if (LocalOrbitals) delete LocalOrbitals;
	if (OrientedLocalOrbitals) delete OrientedLocalOrbitals;
	if (TotalAODensity) delete TotalAODensity;
}
*/
#define kShellScreenThreshold (1.0e-5)
/** Setup a pair of filtering arrays indicating whether there are non-zero MO Vector elements for
each shell and atom.
 */
void SetupMOScreens(const BasisSet * Basis, const float * MOVector, std::vector<bool> & AtomScreen,
					std::vector<bool> & ShellScreen, long NumAtoms, bool UseSphericalHarmonics) {
	long ivec = 0, nshell=0;
	float VectorSum;
	const std::vector<BasisShell> & Shells=Basis->Shells;
	for (long iatom=0; iatom<NumAtoms; iatom++) {
		bool computeAtom = false;
		for (long ishell=Basis->BasisMap[2*iatom]; ishell<=Basis->BasisMap[2*iatom+1]; ishell++) {
			int	type = Shells[ishell].ShellType;
			if (UseSphericalHarmonics) type += 10;
			switch (type) {
				case LShell:
				case SHLShell:
					//The p part of the L shell must be handled seperately since it has two
					//normalization factors for the same shell
					VectorSum =	MOVector[ivec]+MOVector[ivec+1]+MOVector[ivec+2]+MOVector[ivec+3];
					ivec += 4;
					break;
				case SShell:
				case SHSShell:
					VectorSum = MOVector[ivec];
					ivec += 1;
					break;
				case PShell:
				case SHPShell:
					VectorSum = MOVector[ivec]+MOVector[ivec+1]+MOVector[ivec+2];
					ivec += 3;
					break;
				case DShell:
					VectorSum = MOVector[ivec]+MOVector[ivec+1]+MOVector[ivec+2]+
						MOVector[ivec+3]+MOVector[ivec+4]+MOVector[ivec+5];
					ivec += 6;
					break;
				case FShell:
					VectorSum = MOVector[ivec]+MOVector[ivec+1]+MOVector[ivec+2]+MOVector[ivec+3]+
								MOVector[ivec+4]+MOVector[ivec+5]+MOVector[ivec+6]+MOVector[ivec+7]+
								MOVector[ivec+8]+MOVector[ivec+9];
					ivec += 10;
					break;
				case GShell:
					VectorSum = MOVector[ivec]+MOVector[ivec+1]+MOVector[ivec+2]+MOVector[ivec+3]+
						MOVector[ivec+4]+MOVector[ivec+5]+MOVector[ivec+6]+MOVector[ivec+7]+
						MOVector[ivec+8]+MOVector[ivec+9]+MOVector[ivec+10]+MOVector[ivec+11]+
						MOVector[ivec+12]+MOVector[ivec+13]+MOVector[ivec+14];
					ivec += 15;
					break;
				case SHDShell:	
					VectorSum = MOVector[ivec]+MOVector[ivec+1]+MOVector[ivec+2]+
								MOVector[ivec+3]+MOVector[ivec+4];
					ivec += 5;
					break;
				case SHFShell:
					VectorSum = MOVector[ivec]+MOVector[ivec+1]+MOVector[ivec+2]+
								MOVector[ivec+3]+MOVector[ivec+4]+MOVector[ivec+5]+
								MOVector[ivec+6];
					ivec += 7;
					break;
				case SHGShell:
					VectorSum = MOVector[ivec]+MOVector[ivec+1]+MOVector[ivec+2]+
								MOVector[ivec+3]+MOVector[ivec+4]+MOVector[ivec+5]+
								MOVector[ivec+6]+MOVector[ivec+7]+MOVector[ivec+8];
					ivec += 9;
					break;
			}
			ShellScreen[nshell] = (VectorSum > kShellScreenThreshold);
			computeAtom = computeAtom || ShellScreen[nshell];
			nshell++;
		}
	}
}
//Computes the MO Amplitude at the specified x,y,z
float CalculateMOAmplitude(float X_value, float Y_value, float Z_value, mpAtom *Atoms,
		BasisSet *Basis, float *MOVector, long NumAtoms, bool UseSphericalHarmonics) {
	long ivec = 0, NumPrims;
	float	x, y, z, x2, y2, z2, r=0, r2, VectorSum, expcr2, Amplitude=0.0;
	std::vector<BasisShell> & Shells=Basis->Shells;

		//Set up normalization constants for d,f,g functions
	float sqrt3 = sqrt(3.0);
	float onedsqrt3 = 1/sqrt3;
	float sqrt5 = sqrt(5.0);
	float sqrt7 = sqrt(7.0);
	float sqrt9 = 3.0;
	float sqrt11 = sqrt(11.0);
					//loop over the atoms/shells
	for (long iatom=0; iatom<NumAtoms; iatom++) {
		if (iatom > Basis->MapLength) continue;
		x = X_value - Atoms[iatom].Position.x*kAng2BohrConversion;
		y = Y_value - Atoms[iatom].Position.y*kAng2BohrConversion;
		z = Z_value - Atoms[iatom].Position.z*kAng2BohrConversion;
		x2 = x*x;
		y2 = y*y;
		z2 = z*z;
		r2 = x2+y2+z2;
		if (UseSphericalHarmonics) r = sqrt(r2);
		for (long ishell=Basis->BasisMap[2*iatom]; ishell<=Basis->BasisMap[2*iatom+1]; ishell++) {
				//Now multiply by the appropriate x, y, z factors
			int	type = Shells[ishell].ShellType;
			if (UseSphericalHarmonics) type += 10;
			switch (type) {
				case LShell:
				case SHLShell:
						//The p part of the L shell must be handled seperately since it has two
						//normalization factors for the same shell
					VectorSum =	x*MOVector[ivec+1]+y*MOVector[ivec+2]+z*MOVector[ivec+3];
					if (fabs(VectorSum) > 1.0e-6) {
						NumPrims = Shells[ishell].NumPrims;
						expcr2 = 0.0;
						for (long iprim=0; iprim<NumPrims; iprim++) {
							expcr2 += Shells[ishell].NormCoef[iprim+NumPrims]*
									exp(-r2*Shells[ishell].Exponent[iprim]);
						}
						Amplitude += expcr2*VectorSum;
					}
					VectorSum = MOVector[ivec];
					ivec += 4;
				break;
				case SShell:
				case SHSShell:
					VectorSum = MOVector[ivec];
					ivec += 1;
				break;
				case PShell:
				case SHPShell:
					VectorSum = x*MOVector[ivec]+y*MOVector[ivec+1]+z*MOVector[ivec+2];
					ivec += 3;
				break;
				case DShell:
					VectorSum = (x2*MOVector[ivec]+y2*MOVector[ivec+1]+z2*MOVector[ivec+2])+
								sqrt3*(x*(y*MOVector[ivec+3]+z*MOVector[ivec+4])+
								y*z*MOVector[ivec+5]);
					ivec += 6;
				break;
				case FShell:
					VectorSum = x*x2*MOVector[ivec]+y*y2*MOVector[ivec+1]+
								z*z2*MOVector[ivec+2]+sqrt5*(x2*y*MOVector[ivec+3]+
								x2*z*MOVector[ivec+4]+x*y2*MOVector[ivec+5]+
								y2*z*MOVector[ivec+6]+x*z2*MOVector[ivec+7]+
								y*z2*MOVector[ivec+8]+sqrt3*x*y*z*MOVector[ivec+9]);
					ivec += 10;
				break;
				case GShell:
					VectorSum = x2*x2*MOVector[ivec]+y2*y2*MOVector[ivec+1]+
								z2*z2*MOVector[ivec+2]+sqrt7*
								(x*x2*y*MOVector[ivec+3]+x*x2*z*MOVector[ivec+4]+
								x*y*y2*MOVector[ivec+5]+y*y2*z*MOVector[ivec+6]+
								x*z*z2*MOVector[ivec+7]+y*z*z2*MOVector[ivec+8]+
								sqrt5*(onedsqrt3*(x2*y2*MOVector[ivec+9]+
								x2*z2*MOVector[ivec+10]+y2*z2*MOVector[ivec+11])+
								x2*y*z*MOVector[ivec+12]+x*y2*z*MOVector[ivec+13]+
								x*y*z2*MOVector[ivec+14]));
					ivec += 15;
				break;
				case HShell:
					VectorSum = x*x2*x2*MOVector[ivec]+y*y2*y2*MOVector[ivec+1]+
								z*z2*z2*MOVector[ivec+2]+
								sqrt9*(x2*x2*y*MOVector[ivec+3]+x2*x2*z*MOVector[ivec+4]+
								 x*y2*y2*MOVector[ivec+5]+y2*y2*z*MOVector[ivec+6]+
								 x*z2*z2*MOVector[ivec+7]+y*z2*z2*MOVector[ivec+8]+
								 sqrt7*(onedsqrt3*(x*x2*y2*MOVector[ivec+9]+
										x*x2*z2*MOVector[ivec+10]+x2*y2*y*MOVector[ivec+11]+
										y2*y*z2*MOVector[ivec+12]+x2*z*z2*MOVector[ivec+13]+
										y2*z*z2*MOVector[ivec+14])+
										x*x2*y*z*MOVector[ivec+15]+x*y*y2*z*MOVector[ivec+16]+
										x*y*z*z2*MOVector[ivec+17]+
								sqrt5*onedsqrt3*(x2*y2*z*MOVector[ivec+18]+x2*y*z2*MOVector[ivec+19]+
												 x*y2*z2*MOVector[ivec+20])));
					ivec += 21;
					break;
				case IShell:
					VectorSum = x2*x2*x2*MOVector[ivec]+y2*y2*y2*MOVector[ivec+1]+
								z2*z2*z2*MOVector[ivec+2]+
								sqrt11*(x*x2*x2*y*MOVector[ivec+3]+x*x2*x2*z*MOVector[ivec+4]+
										x*y*y2*y2*MOVector[ivec+5]+y*y2*y2*z*MOVector[ivec+6]+
										x*z*z2*z2*MOVector[ivec+7]+y*z*z2*z2*MOVector[ivec+8]+
									sqrt3*(x2*x2*y2*MOVector[ivec+9]+x2*x2*z2*MOVector[ivec+10]+
										x2*y2*y2*MOVector[ivec+11]+y2*y2*z2*MOVector[ivec+12]+
										x2*z2*z2*MOVector[ivec+13]+y2*z2*z2*MOVector[ivec+14])+
									sqrt9*(x2*x2*y*z*MOVector[ivec+15]+x*y2*y2*z*MOVector[ivec+16]+
										x*y*z2*z2*MOVector[ivec+17])+
									(sqrt3*sqrt7/sqrt5)*(x*x2*y*y2*MOVector[ivec+18]+
										x*x2*z*z2*MOVector[ivec+19]+y*y2*z*z2*MOVector[ivec+20])+
									sqrt3*sqrt7*(x*x2*y2*z*MOVector[ivec+21]+x*x2*y*z2*MOVector[ivec+22]+
										x2*y*y2*z*MOVector[ivec+23]+x*y*y2*z2*MOVector[ivec+24]+
										x2*y*z*z2*MOVector[ivec+25]+x*y2*z*z2*MOVector[ivec+26])+
									sqrt5*sqrt7*x2*y2*z2*MOVector[ivec+27]);
					ivec += 28;
					break;
				case SHDShell:	//normallized except for common 1/sqrt(4pi)
					VectorSum = sqrt5*(sqrt3*(x*y*MOVector[ivec]+y*z*MOVector[ivec+1])+
								0.5*(3*z2-r2)*MOVector[ivec+2]+
								sqrt3*(x*z*MOVector[ivec+3]+0.5*(x2-y2)*MOVector[ivec+4]));
					ivec += 5;
				break;
				case SHFShell:
					VectorSum = sqrt7*((sqrt5/sqrt(8.0))*(x*x2-3*x*y2)*MOVector[ivec]+
								(sqrt5*sqrt3*0.5)*(x2-y2)*z*MOVector[ivec+1]+
								(sqrt3/sqrt(8.0))*x*(5*z2-r2)*MOVector[ivec+2]+
								0.5*z*(5*z2-3*r2)*MOVector[ivec+3]+
								(sqrt3/sqrt(8.0))*y*(5*z2-r2)*MOVector[ivec+4]+
								(sqrt5*sqrt3)*x*y*z*MOVector[ivec+5]+
								(sqrt5/sqrt(8.0))*y*(3*x2-y2)*MOVector[ivec+6]);
					ivec += 7;
				break;
				case SHGShell:
					VectorSum = 3*((sqrt5*sqrt7/8.0)*(x2*x2+y2*y2-6*x2*y2)*MOVector[ivec]+
								(sqrt5*sqrt7/sqrt(8.0))*(x2-3*y2)*x*z*MOVector[ivec+1]+
								(sqrt5/4)*(x2-y2)*(7*z2-r2)*MOVector[ivec+2]+
								(sqrt5/sqrt(8.0))*x*z*(7*z2-3*r2)*MOVector[ivec+3]+
								((35*z2*z2-30*z2*r2+3*r2*r2)/8.0)*MOVector[ivec+4]+
								(sqrt5/sqrt(8.0))*y*z*(7*z2-3*r2)*MOVector[ivec+5]+
								(sqrt5/2)*x*y*(7*z2-r2)*MOVector[ivec+6]+
								(sqrt5*sqrt7/sqrt(8.0))*y*z*(3*x2-y2)*MOVector[ivec+7]+
								(sqrt5*sqrt7/2)*x*y*(x2-y2)*MOVector[ivec+8]);
					ivec += 9;
				break;
			}
				//Only calculate the exponential if the vector is large enough
			if (fabs(VectorSum) > 1.0e-6) {
				expcr2 = 0.0;
					//Form contracted exponential
				for (long iprim=0; iprim<Shells[ishell].NumPrims; iprim++) {
					expcr2 += Shells[ishell].NormCoef[iprim]*
							exp(-r2*Shells[ishell].Exponent[iprim]);
				}
				Amplitude += expcr2*VectorSum;
			}
		}
	}
	return Amplitude;
}
//Computes the AO Amplitude vector at the specified x,y,z
void CalculateAOAmplitudeVector(float X_value, float Y_value, float Z_value, mpAtom *Atoms,
		BasisSet *Basis, float *AOVector, long NumAtoms) {
	long ivec = 0, NumPrims;
	float	x, y, z, x2, y2, z2, r2, expcr2;
	std::vector<BasisShell> & Shells=Basis->Shells;

		//Set up normalization constants for d,f,g functions
	float sqrt3 = sqrt(3.0);
	float onedsqrt3 = 1/sqrt3;
	float sqrt5 = sqrt(5.0);
	float sqrt7 = sqrt(7.0);
	float sqrt9 = 3.0;
	float sqrt11 = sqrt(11.0);
					//loop over the atoms/shells
	for (long iatom=0; iatom<NumAtoms; iatom++) {
		if (iatom > Basis->MapLength) continue;
		x = X_value - Atoms[iatom].Position.x*kAng2BohrConversion;
		y = Y_value - Atoms[iatom].Position.y*kAng2BohrConversion;
		z = Z_value - Atoms[iatom].Position.z*kAng2BohrConversion;
		x2 = x*x;
		y2 = y*y;
		z2 = z*z;
		r2 = x2+y2+z2;
		for (long ishell=Basis->BasisMap[2*iatom]; ishell<=Basis->BasisMap[2*iatom+1]; ishell++) {
			expcr2 = 0.0;
				//Form contracted exponential
			for (long iprim=0; iprim<Shells[ishell].NumPrims; iprim++) {
				expcr2 += Shells[ishell].NormCoef[iprim]*
						exp(-r2*Shells[ishell].Exponent[iprim]);
			}
				//Now multiply by the appropriate x, y, z factors
			switch (Shells[ishell].ShellType) {
				case LShell:
					AOVector[ivec] = expcr2;
						//The p part of the L shell must be handled seperately since it has two
						//normalization factors for the same shell
					NumPrims = Shells[ishell].NumPrims;
					expcr2 = 0.0;
					for (long iprim=0; iprim<NumPrims; iprim++) {
						expcr2 += Shells[ishell].NormCoef[iprim+NumPrims]*
								exp(-r2*Shells[ishell].Exponent[iprim]);
					}
					AOVector[ivec+1] = x*expcr2;
					AOVector[ivec+2] = y*expcr2;
					AOVector[ivec+3] = z*expcr2;
					ivec += 4;
				break;
				case SShell:
					AOVector[ivec] = expcr2;
					ivec += 1;
				break;
				case PShell:
					AOVector[ivec] = x*expcr2;
					AOVector[ivec+1] = y*expcr2;
					AOVector[ivec+2] = z*expcr2;
					ivec += 3;
				break;
				case DShell:
					AOVector[ivec] = x2*expcr2;
					AOVector[ivec+1] = y2*expcr2;
					AOVector[ivec+2] = z2*expcr2;
					AOVector[ivec+3] = sqrt3*x*y*expcr2;
					AOVector[ivec+4] = sqrt3*x*z*expcr2;
					AOVector[ivec+5] = sqrt3*y*z*expcr2;
					ivec += 6;
				break;
				case FShell:
					AOVector[ivec] = x*x2*expcr2;
					AOVector[ivec+1] = y*y2*expcr2;
					AOVector[ivec+2] = z*z2*expcr2;
					AOVector[ivec+3] = sqrt5*x2*y*expcr2;
					AOVector[ivec+4] = sqrt5*x2*z*expcr2;
					AOVector[ivec+5] = sqrt5*x*y2*expcr2;
					AOVector[ivec+6] = sqrt5*y2*z*expcr2;
					AOVector[ivec+7] = sqrt5*x*z2*expcr2;
					AOVector[ivec+8] = sqrt5*y*z2*expcr2;
					AOVector[ivec+9] = sqrt3*sqrt5*x*y*z*expcr2;
					ivec += 10;
				break;
				case GShell:
					AOVector[ivec] = x2*x2*expcr2;
					AOVector[ivec+1] = y2*y2*expcr2;
					AOVector[ivec+2] = z2*z2*expcr2;
					AOVector[ivec+3] = sqrt7*x*x2*y*expcr2;
					AOVector[ivec+4] = sqrt7*x*x2*z*expcr2;
					AOVector[ivec+5] = sqrt7*x*y*y2*expcr2;
					AOVector[ivec+6] = sqrt7*y*y2*z*expcr2;
					AOVector[ivec+7] = sqrt7*x*z*z2*expcr2;
					AOVector[ivec+8] = sqrt7*y*z*z2*expcr2;
					AOVector[ivec+9] = sqrt7*sqrt5*onedsqrt3*x2*y2*expcr2;
					AOVector[ivec+10] = sqrt7*sqrt5*onedsqrt3*x2*z2*expcr2;
					AOVector[ivec+11] = sqrt7*sqrt5*onedsqrt3*y2*z2*expcr2;
					AOVector[ivec+12] = sqrt7*sqrt5*x2*y*z*expcr2;
					AOVector[ivec+13] = sqrt7*sqrt5*x*y2*z*expcr2;
					AOVector[ivec+14] = sqrt7*sqrt5*x*y*z2*expcr2;
					ivec += 15;
				break;
				case HShell:
					AOVector[ivec] = x*x2*x2*expcr2;
					AOVector[ivec+1] = y*y2*y2*expcr2;
					AOVector[ivec+2] = z*z2*z2*expcr2;
					AOVector[ivec+3] = sqrt9*x2*x2*y*expcr2;
					AOVector[ivec+4] = sqrt9*x2*x2*z*expcr2;
					AOVector[ivec+5] = sqrt9*x*y2*y2*expcr2;
					AOVector[ivec+6] = sqrt9*y2*y2*z*expcr2;
					AOVector[ivec+7] = sqrt9*x*z2*z2*expcr2;
					AOVector[ivec+8] = sqrt9*y*z2*z2*expcr2;
					AOVector[ivec+9] = sqrt3*sqrt7*x*x2*y2*expcr2;
					AOVector[ivec+10] = sqrt3*sqrt7*x*x2*z2*expcr2;
					AOVector[ivec+11] = sqrt3*sqrt7*x2*y*y2*expcr2;
					AOVector[ivec+12] = sqrt3*sqrt7*y*y2*z2*expcr2;
					AOVector[ivec+13] = sqrt3*sqrt7*x2*z*z2*expcr2;
					AOVector[ivec+14] = sqrt3*sqrt7*y2*z*z2*expcr2;
					AOVector[ivec+15] = sqrt9*sqrt7*x*x2*y*z*expcr2;
					AOVector[ivec+16] = sqrt9*sqrt7*x*y*y2*z*expcr2;
					AOVector[ivec+17] = sqrt9*sqrt7*x*y*z*z2*expcr2;
					AOVector[ivec+18] = sqrt7*sqrt5*sqrt3*x2*y2*z*expcr2;
					AOVector[ivec+19] = sqrt7*sqrt5*sqrt3*x2*y*z2*expcr2;
					AOVector[ivec+20] = sqrt7*sqrt5*sqrt3*x*y2*z2*expcr2;
					ivec += 21;
					break;
				case IShell:
					AOVector[ivec] = x2*x2*x2*expcr2;
					AOVector[ivec+1] = y2*y2*y2*expcr2;
					AOVector[ivec+2] = z2*z2*z2*expcr2;
					AOVector[ivec+3] = sqrt11*x*x2*x2*y*expcr2;
					AOVector[ivec+4] = sqrt11*x*x2*x2*z*expcr2;
					AOVector[ivec+5] = sqrt11*x*y*y2*y2*expcr2;
					AOVector[ivec+6] = sqrt11*y*y2*y2*z*expcr2;
					AOVector[ivec+7] = sqrt11*x*z*z2*z2*expcr2;
					AOVector[ivec+8] = sqrt11*y*z*z2*z2*expcr2;
					AOVector[ivec+9] = sqrt3*sqrt11*x2*x2*y2*expcr2;
					AOVector[ivec+10] = sqrt3*sqrt11*x2*x2*z2*expcr2;
					AOVector[ivec+11] = sqrt3*sqrt11*x2*y2*y2*expcr2;
					AOVector[ivec+12] = sqrt3*sqrt11*y2*y2*z2*expcr2;
					AOVector[ivec+13] = sqrt3*sqrt11*x2*z2*z2*expcr2;
					AOVector[ivec+14] = sqrt3*sqrt11*y2*z2*z2*expcr2;
					AOVector[ivec+15] = sqrt9*sqrt11*x2*x2*y*z*expcr2;
					AOVector[ivec+16] = sqrt9*sqrt11*x*y2*y2*z*expcr2;
					AOVector[ivec+17] = sqrt9*sqrt11*x*y*z2*z2*expcr2;
					AOVector[ivec+18] = (sqrt3*sqrt7*sqrt11/sqrt5)*x*x2*y*y2*expcr2;
					AOVector[ivec+19] = (sqrt3*sqrt7*sqrt11/sqrt5)*x*x2*z*z2*expcr2;
					AOVector[ivec+20] = (sqrt3*sqrt7*sqrt11/sqrt5)*y*y2*z*z2*expcr2;
					AOVector[ivec+21] = sqrt11*sqrt7*sqrt3*x*x2*y2*z*expcr2;
					AOVector[ivec+22] = sqrt11*sqrt7*sqrt3*x*x2*y*z2*expcr2;
					AOVector[ivec+23] = sqrt11*sqrt7*sqrt3*x2*y*y2*z*expcr2;
					AOVector[ivec+24] = sqrt11*sqrt7*sqrt3*x*y*y2*z2*expcr2;
					AOVector[ivec+25] = sqrt11*sqrt7*sqrt3*x2*y*z*z2*expcr2;
					AOVector[ivec+26] = sqrt11*sqrt7*sqrt3*x*y2*z*z2*expcr2;
					AOVector[ivec+27] = sqrt11*sqrt7*sqrt5*x2*y2*z2*expcr2;
					ivec += 28;
					break;
			}
		}
	}
}
AODensity * Frame::GetAODensity(BasisSet * basis, const long & targetOrbSet) {
	AODensity * result=NULL;

	//If the targetOrbSet < 0 a default will be used, otherwise the target needs to be valid
	if (Orbs.size() > 0) {
		OrbitalRec * t=NULL;
		if (targetOrbSet < 0) {	//pick a default
			std::vector<OrbitalRec *>::const_iterator OrbSet = Orbs.begin();
			while (OrbSet != Orbs.end()) {
				if ((*OrbSet)->getNumOccupiedAlphaOrbitals() > 0) {
					t = (*OrbSet);
				}
				if (t != NULL) break;
				OrbSet++;
			}
		}
		if ((targetOrbSet >= 0)&&(targetOrbSet < Orbs.size())) {
			t = Orbs[targetOrbSet];
			if (t->getNumOccupiedAlphaOrbitals() <= 0) t = NULL;
		}
		if (t != NULL) {
			result = t->GetAODensity(basis, NumAtoms);
		}
	}
	return result;
}


//Generates a 2D grid over the specified MO in the plane specified.
//NOTE: This routine assumes that the atomic coordinates are in Angstroms and are thus
//converted to Bohr when used.
//The cursorID is the ID for the cursor to set when exiting
void Orb2DSurface::CalculateMOGrid(MoleculeData *lData, Progress * lProgress) {
	long		NumPoints = NumGridPoints*NumGridPoints;

	Frame *	lFrame = lData->GetCurrentFramePtr();
	BasisSet * Basis = lData->GetBasisSet();
	long NumBasisFuncs = Basis->GetNumBasisFuncs(UseSphericalHarmonics());
	OrbitalRec	*MOs=NULL;
//	MORec * lOrbs = NULL;
//	if (!(Options&1)) {
//		lOrbs=lFrame->Orbs;
//		if (lFrame->Orbs.size() <= 0) {
//			FreeGrid();
//			return;
//		}
//	}

	if (Grid && (GridAllocation != NumPoints)) FreeGrid();
	if ((PlotOrb < 0)||(PlotOrb > NumBasisFuncs)) {
		FreeGrid();
		return;
	}

	float		*MOVector=NULL;
	if (UseAOs()) {	//Use AOs
		MOs = (OrbitalRec *) 1;
		MOVector = new float[NumBasisFuncs];
		if (MOVector) {
			for (long i=0; i<NumBasisFuncs; i++) MOVector[i] = 0.0;
			MOVector[PlotOrb] = 1.0;	//Set the desired AO to 1
		}
	} else {
		if ((TargetSet >= 0) && (TargetSet < lFrame->Orbs.size())) {
			MOs = lFrame->Orbs[TargetSet];
			if (MOs != NULL) {	//They should never be NULL!
				if (UseBetaSet()) {
					if (PlotOrb>=MOs->NumBetaOrbs) {
						FreeGrid();
						return;
					}	//select the start of the desired vector
					MOVector = &(MOs->VectorsB[NumBasisFuncs*PlotOrb]);
				} else {
					if (PlotOrb>=MOs->NumAlphaOrbs) {
						FreeGrid();
						return;
					}
					MOVector = &(MOs->Vectors[NumBasisFuncs*PlotOrb]);
				}
			}
		}
	}
//	} else if (Options & 4) {	//or use the Localized MO's
//		MOs = lOrbs->LocalOrbitals;
//	} else if (Options & 2) {	//Plot the eigenvectors
//		MOs = lOrbs->EigenVectors;
//	} else if (Options & 8) {	//Plot the oriented LMO's
//		MOs = lOrbs->OrientedLocalOrbitals;
//	}

	if (MOs==NULL) {	//Invalid orbitals set choice, free memory and return
		FreeGrid();
		return;
	}
//	if (!(Options&1)) {
//		if (Options & 16) {
//			if (PlotOrb>=MOs->NumBetaOrbs) {
//				FreeGrid();
//				return;
//			}
//			MOVector = &(MOs->VectorsB[NumBasisFuncs*PlotOrb]);
//		} else {
//			if (PlotOrb>=MOs->NumAlphaOrbs) {
//				FreeGrid();
//				return;
//			}
//			MOVector = &(MOs->Vectors[NumBasisFuncs*PlotOrb]);
//		}
//	} else {	//AOs: fake an MOVector with one non-zero coef.
//		MOVector = new float[NumBasisFuncs];
//		if (MOVector) {
//			for (long i=0; i<NumBasisFuncs; i++) MOVector[i] = 0.0;
//			MOVector[PlotOrb] = 1.0;	//Others are init to zero above
//		}
//	}
	if (MOVector == NULL) throw MemoryError();

	if (!Grid) {
			//Attempt to allocate memory for the 2D grid
		AllocateGrid(NumPoints);
	}
		//If the memory allocation failed the MO calculation can not be done
	if (Grid == NULL) return;
		//If sufficient memory is available then setup pointers for fast local use
	float * lGrid;
	lGrid = Grid;
		//Store the Grid mins and incs at the beginning of the grid
	GridMax = -1.0e20;
	GridMin = 1.0e20;
		//loop over the plotting grid in the x, y, and z directions
	long n=0;
	float XGridValue, YGridValue, ZGridValue;
	CPoint3D	lOrigin, lXInc, lYInc;

	lOrigin = Origin * kAng2BohrConversion;
	lXInc = XInc * kAng2BohrConversion;
	lYInc = YInc * kAng2BohrConversion;
	float PhaseChange = 1.0;
	if (GetPhaseChange()) PhaseChange = -1.0;
	for (long iXPt=0; iXPt<NumGridPoints; iXPt++) {
			//Give up the CPU and check for cancels
		if (!lProgress->UpdateProgress(iXPt*100.0/NumGridPoints)) {	//User canceled so clean things up and abort
			if (Options&1) delete [] MOVector;
			FreeGrid();
			return;
		}

		XGridValue = lOrigin.x + iXPt*lXInc.x;
		YGridValue = lOrigin.y + iXPt*lXInc.y;
		ZGridValue = lOrigin.z + iXPt*lXInc.z;
		for (long iYPt=0; iYPt<NumGridPoints; iYPt++) {
			lGrid[n] = PhaseChange * CalculateMOAmplitude(XGridValue, YGridValue, ZGridValue,
						lFrame->Atoms, Basis, MOVector, lFrame->NumAtoms,UseSphericalHarmonics());
			
			GridMax = MAX(GridMax, lGrid[n]);
			GridMin = MIN(GridMin, lGrid[n]);
			n++;
			XGridValue += lYInc.x;
			YGridValue += lYInc.y;
			ZGridValue += lYInc.z;
		}
	}
		//Unlock the grid handle and return
	if (Options & 1) delete [] MOVector;

}
//NOTE: This routine assumes that the atomic coordinates are in Angstroms and are thus
//converted to Bohr when used.
//The cursorID is the ID for the cursor to set when exiting
void TEDensity2DSurface::CalculateMOGrid(MoleculeData *lData, Progress * lProgress) {
	long		NumPoints = NumGridPoints*NumGridPoints;

	Frame *	lFrame = lData->GetCurrentFramePtr();
	BasisSet * Basis = lData->GetBasisSet();
	long NumBasisFuncs = Basis->GetNumBasisFuncs(false);
	if (lFrame->Orbs.size() <= 0) return;	//safety check
	if (OrbSet > lFrame->Orbs.size()) return;	//Invalid choice of an orbital set
	if (OrbSet < 0) {	//pick a default set
		long i=0;
		std::vector<OrbitalRec *>::const_iterator Set = lFrame->Orbs.begin();
		while (Set != lFrame->Orbs.end()) {
			if ((*Set)->getNumOccupiedAlphaOrbitals() > 0) {
				OrbSet = i;
				break;
			}
		}
		if (OrbSet < 0) return;
	}

	if (Grid && (GridAllocation != NumPoints)) {
		FreeGrid();
	}
	if (!Grid) {
			//Attempt to allocate memory for the 2D grid
		AllocateGrid(NumPoints);
	}
		//If the memory allocation failed the MO calculation can not be done
	if (Grid == NULL) return;
		//If sufficient memory is available then setup pointers for fast local use
	float * lGrid;
	lGrid = Grid;

	OrbitalRec	*MOs=NULL;
	MOs = lFrame->Orbs[OrbSet];
 
	if (MOs==NULL) {	//Invalid orbitals set choice, free memory and return
		FreeGrid();
		return;
	}
	float		*OccupancyA=NULL, *OccupancyB=NULL, *Vectors=NULL, *VectorsB=NULL, *AOVector;
	bool	deleteTempOcc = false;
	if (MOs->getOrbitalWavefunctionType() == RHF) {
		OccupancyA = new float[MOs->NumOccupiedAlphaOrbs];
		if (OccupancyA) for (long i=0; i<MOs->NumOccupiedAlphaOrbs; i++) OccupancyA[i]=2.0;
		Vectors = MOs->Vectors;
		deleteTempOcc = true;
	} else if (MOs->getOrbitalWavefunctionType() == ROHF) {
		OccupancyA = new float[MOs->NumOccupiedAlphaOrbs];
		if (OccupancyA) for (long i=0; i<MOs->NumOccupiedAlphaOrbs; i++) {
			if (i<MOs->NumOccupiedBetaOrbs) OccupancyA[i]=2.0;
			else OccupancyA[i]=1.0;
		}
		Vectors = MOs->Vectors;
		deleteTempOcc = true;
	} else if ((MOs->getOrbitalWavefunctionType() == UHF)&&(MOs->getOrbitalType() != NaturalOrbital)) {
			long i;
		OccupancyA = new float[MOs->NumOccupiedAlphaOrbs];
		if (OccupancyA) for (i=0; i<MOs->NumOccupiedAlphaOrbs; i++) OccupancyA[i]=1.0;
		OccupancyB = new float[MOs->NumOccupiedBetaOrbs];
		if (OccupancyB) for (i=0; i<MOs->NumOccupiedBetaOrbs; i++) OccupancyB[i]=1.0;
		Vectors = MOs->Vectors;
		VectorsB = MOs->VectorsB;
		deleteTempOcc = true;
//	} else if ((MOs->getOrbitalWavefunctionType() == MCSCF)||(MOs->getOrbitalWavefunctionType() == RHFMP2)||
//			   ((MOs->getOrbitalWavefunctionType() == UHF)&&(MOs->getOrbitalType() == NaturalOrbital))) {
	} else {
		OccupancyA = MOs->OrbOccupation;
		Vectors = MOs->Vectors;
	}
	if ((Vectors == NULL)||(OccupancyA == NULL)) {
		if (deleteTempOcc) {
			if (OccupancyA) delete [] OccupancyA;
			if (OccupancyB) delete [] OccupancyB;
		}
		FreeGrid();
		return;
	}
	AOVector = new float[NumBasisFuncs];
	if (AOVector == NULL) throw DataError();
		//Store the Grid mins and incs at the beginning of the grid
	GridMax = -1.0e20;
	GridMin = 1.0e20;
		//loop over the plotting grid in the x, y, and z directions
	long n=0;
	float XGridValue, YGridValue, ZGridValue;
	CPoint3D	lOrigin, lXInc, lYInc;
	long	MONum;

	lOrigin = Origin * kAng2BohrConversion;
	lXInc = XInc * kAng2BohrConversion;
	lYInc = YInc * kAng2BohrConversion;

	for (long iXPt=0; iXPt<NumGridPoints; iXPt++) {
		if (!lProgress->UpdateProgress(iXPt/NumGridPoints)) {	//User canceled so clean things up and abort
			if (deleteTempOcc) {
				if (OccupancyA) delete [] OccupancyA;
				if (OccupancyB) delete [] OccupancyB;
			}
			if (AOVector) delete [] AOVector;
			FreeGrid();
			return;
		}

		XGridValue = lOrigin.x + iXPt*lXInc.x;
		YGridValue = lOrigin.y + iXPt*lXInc.y;
		ZGridValue = lOrigin.z + iXPt*lXInc.z;
		for (long iYPt=0; iYPt<NumGridPoints; iYPt++) {
				//First calculate the ampiltude of each AO at the current grid point
			CalculateAOAmplitudeVector(XGridValue, YGridValue, ZGridValue,
				lFrame->Atoms, Basis, AOVector, lFrame->NumAtoms);
			
				float *MOVector, Density, Sum;
			Sum = 0.0;
			if (MOs->NumOccupiedAlphaOrbs) {
				for (MONum=0; MONum<MOs->NumOccupiedAlphaOrbs; MONum++) {
					MOVector = &(Vectors[NumBasisFuncs*MONum]);
					Density = 0.0;
					for (long ivec=0; ivec<NumBasisFuncs; ivec++)
						Density += MOVector[ivec]*AOVector[ivec];
					Sum += Density*Density*OccupancyA[MONum];
				}
			}
			if (OccupancyB) {
				for (MONum=0; MONum<MOs->NumOccupiedBetaOrbs; MONum++) {
					MOVector = &(VectorsB[NumBasisFuncs*MONum]);
					Density = 0.0;
					for (long ivec=0; ivec<NumBasisFuncs; ivec++)
						Density += MOVector[ivec]*AOVector[ivec];
					Sum += Density*Density*OccupancyB[MONum];
				}
			}
			lGrid[n] = Sum;
			GridMax = MAX(GridMax, lGrid[n]);
			GridMin = MIN(GridMin, lGrid[n]);
			n++;
			XGridValue += lYInc.x;
			YGridValue += lYInc.y;
			ZGridValue += lYInc.z;
		}
	}
		//Unlock the grid handle and return
//	if ((MOs->getOrbitalWavefunctionType() != MCSCF)&&(MOs->getOrbitalWavefunctionType() != RHFMP2)&&
//		!((MOs->getOrbitalWavefunctionType() == UHF)&&(MOs->getOrbitalType() == NaturalOrbital))) {
	if (deleteTempOcc) {
		if (OccupancyA) delete [] OccupancyA;
		if (OccupancyB) delete [] OccupancyB;
	}
	if (AOVector) delete [] AOVector;

}

void TEDensity1DSurface::CalculateMOGrid(MoleculeData *lData,
										 Progress * lProgress) {

	long NumPoints = NumGridPoints;

	Frame *	lFrame = lData->GetCurrentFramePtr();
	BasisSet * Basis = lData->GetBasisSet();
	long NumBasisFuncs = Basis->GetNumBasisFuncs(false);
	if (lFrame->Orbs.size() <= 0) return;	//safety check
	if (OrbSet > lFrame->Orbs.size()) return;	//Invalid choice of an orbital set
	if (OrbSet < 0) {	//pick a default set
		long i=0;
		std::vector<OrbitalRec *>::const_iterator Set = lFrame->Orbs.begin();
		while (Set != lFrame->Orbs.end()) {
			if ((*Set)->getNumOccupiedAlphaOrbitals() > 0) {
				OrbSet = i;
				break;
			}
		}
		if (OrbSet < 0) return;
	}

	if (Grid && (GridAllocation != NumPoints)) {
		FreeGrid();
	}

	if (!Grid) {
		// Attempt to allocate memory for the 1D grid
		AllocateGrid(NumPoints);
	}

	// If the memory allocation failed the MO calculation can not be done
	if (Grid == NULL) return;

	// If sufficient memory is available then setup pointers for fast local use
	float * lGrid;
	lGrid = Grid;

	OrbitalRec	*MOs=NULL;
	MOs = lFrame->Orbs[OrbSet];
 
	if (MOs==NULL) {	//Invalid orbitals set choice, free memory and return
		FreeGrid();
		return;
	}

	float *OccupancyA = NULL,
		  *OccupancyB = NULL,
		  *Vectors = NULL,
		  *VectorsB = NULL,
		  *AOVector;

	bool deleteTempOcc = false;
	if (MOs->getOrbitalWavefunctionType() == RHF) {
		OccupancyA = new float[MOs->NumOccupiedAlphaOrbs];
		if (OccupancyA) for (long i=0; i<MOs->NumOccupiedAlphaOrbs; i++) OccupancyA[i]=2.0;
		Vectors = MOs->Vectors;
		deleteTempOcc = true;
	} else if (MOs->getOrbitalWavefunctionType() == ROHF) {
		OccupancyA = new float[MOs->NumOccupiedAlphaOrbs];
		if (OccupancyA) for (long i=0; i<MOs->NumOccupiedAlphaOrbs; i++) {
			if (i<MOs->NumOccupiedBetaOrbs) OccupancyA[i]=2.0;
			else OccupancyA[i]=1.0;
		}
		Vectors = MOs->Vectors;
		deleteTempOcc = true;
	} else if ((MOs->getOrbitalWavefunctionType() == UHF)&&(MOs->getOrbitalType() != NaturalOrbital)) {
			long i;
		OccupancyA = new float[MOs->NumOccupiedAlphaOrbs];
		if (OccupancyA) for (i=0; i<MOs->NumOccupiedAlphaOrbs; i++) OccupancyA[i]=1.0;
		OccupancyB = new float[MOs->NumOccupiedBetaOrbs];
		if (OccupancyB) for (i=0; i<MOs->NumOccupiedBetaOrbs; i++) OccupancyB[i]=1.0;
		Vectors = MOs->Vectors;
		VectorsB = MOs->VectorsB;
		deleteTempOcc = true;
//	} else if ((MOs->getOrbitalWavefunctionType() == MCSCF)||(MOs->getOrbitalWavefunctionType() == RHFMP2)||
//			   ((MOs->getOrbitalWavefunctionType() == UHF)&&(MOs->getOrbitalType() == NaturalOrbital))) {
	} else {
		OccupancyA = MOs->OrbOccupation;
		Vectors = MOs->Vectors;
	}

	if ((Vectors == NULL)||(OccupancyA == NULL)) {
		if (deleteTempOcc) {
			if (OccupancyA) delete [] OccupancyA;
			if (OccupancyB) delete [] OccupancyB;
		}
		FreeGrid();
		return;
	}

	AOVector = new float[NumBasisFuncs];
	if (AOVector == NULL) throw DataError();

	//Store the Grid mins and incs at the beginning of the grid
	GridMax = -1.0e20;
	GridMin = 1.0e20;

	//loop over the plotting grid in the x, y, and z directions
	long n = 0;
	float XGridValue;
	float YGridValue;
	float ZGridValue;
	CPoint3D lXInc = End - Start;
	lXInc *= 1.0f / NumGridPoints;
	long	MONum;
	CPoint3D Origin = Start * kAng2BohrConversion;

	lXInc *= kAng2BohrConversion;

	for (long iXPt = 0; iXPt < NumGridPoints; iXPt++) {
		if (!lProgress->UpdateProgress(iXPt/NumGridPoints)) {	//User canceled so clean things up and abort
			if (deleteTempOcc) {
				if (OccupancyA) delete [] OccupancyA;
				if (OccupancyB) delete [] OccupancyB;
			}
			if (AOVector) delete [] AOVector;
			FreeGrid();
			return;
		}

		XGridValue = Origin.x + iXPt * lXInc.x;
		YGridValue = Origin.y + iXPt * lXInc.y;
		ZGridValue = Origin.z + iXPt * lXInc.z;

		//First calculate the ampiltude of each AO at the current grid point
		CalculateAOAmplitudeVector(XGridValue, YGridValue, ZGridValue,
			lFrame->Atoms, Basis, AOVector, lFrame->NumAtoms);
		
		float *MOVector, Density, Sum;
		Sum = 0.0;

		if (MOs->NumOccupiedAlphaOrbs) {
			for (MONum=0; MONum<MOs->NumOccupiedAlphaOrbs; MONum++) {
				MOVector = &(Vectors[NumBasisFuncs*MONum]);
				Density = 0.0;
				for (long ivec=0; ivec<NumBasisFuncs; ivec++)
					Density += MOVector[ivec]*AOVector[ivec];
				Sum += Density*Density*OccupancyA[MONum];
			}
		}

		if (OccupancyB) {
			for (MONum=0; MONum<MOs->NumOccupiedBetaOrbs; MONum++) {
				MOVector = &(VectorsB[NumBasisFuncs*MONum]);
				for (long ivec=0; ivec<NumBasisFuncs; ivec++)
					Density += MOVector[ivec]*AOVector[ivec];
				Sum += Density*Density*OccupancyB[MONum];
			}
		}

		lGrid[n] = Sum;
		GridMax = MAX(GridMax, lGrid[n]);
		GridMin = MIN(GridMin, lGrid[n]);
		n++;
		XGridValue += lXInc.x;
		YGridValue += lXInc.y;
		ZGridValue += lXInc.z;
	}

	//Unlock the grid handle and return
	if (deleteTempOcc) {
		if (OccupancyA) delete [] OccupancyA;
		if (OccupancyB) delete [] OccupancyB;
	}
	if (AOVector) delete [] AOVector;
}

#ifdef powerc
typedef struct Orb3DGridData {
	Orb3DSurface *	Surf;
	long	xStart;
	long	xEnd;
	mpAtom *	Atoms;
	BasisSet *	Basis;
	float	*	MOVector;
	long		NumAtoms;
	float		GridMax;
	long		PercentDone;
	MPQueueID		TerminationQueueID;
} Orb3DGridData;
static OSErr CalcGrid(Orb3DGridData * Data) {
	float GridMax = Data->Surf->CalculateGrid(Data->xStart, Data->xEnd, Data->Atoms, Data->Basis,
		Data->MOVector, Data->NumAtoms, NULL, &(Data->PercentDone), true);
	Data->GridMax = GridMax;
	return noErr;
}
#endif
#ifdef __wxBuild__
typedef struct Orb3DGridData {
	Orb3DSurface *	Surf;
	long	xStart;
	long	xEnd;
	mpAtom *	Atoms;
	BasisSet *	Basis;
	float	*	MOVector;
	long		NumAtoms;
	float		GridMax;
	long		PercentDone;
} Orb3DGridData;
//derive a class from wxThread that we will use to create the child threads
class Orb3DThread : public wxThread {
public:
	Orb3DThread(Orb3DGridData * data);
	virtual void * Entry();
	
	long GetPercentDone(void) const {return myData->PercentDone;};
	float GetGridMax(void) const {return myData->GridMax;};
	
	Orb3DGridData * myData;
};
Orb3DThread::Orb3DThread(Orb3DGridData * data) : wxThread(wxTHREAD_JOINABLE) {
	myData = data;
}
void * Orb3DThread::Entry() {
	myData->GridMax = myData->Surf->CalculateGrid(myData->xStart, myData->xEnd, myData->Atoms, myData->Basis,
												  myData->MOVector, myData->NumAtoms, NULL, &(myData->PercentDone),
												  true);
	myData->PercentDone = 100;
	return NULL;
}
typedef Orb3DThread * Orb3DThreadPtr;

#endif

//Generates a 3D grid over the specified MO in the plane specified.
//NOTE: This routine assumes that the atomic coordinates are in Angstroms and are thus
//converted to Bohr when used.
void Orb3DSurface::CalculateMOGrid(MoleculeData *lData, Progress * lProgress) {
	Frame *	lFrame = lData->GetCurrentFramePtr();
//	MORec * lOrbs = NULL;
//	if (!(Options&1)) {
//		lOrbs=lFrame->Orbs;
//		if (!lOrbs) {
//			FreeGrid();
//			return;
//		}
//	}
	if (PlotOrb < 0) {
		FreeGrid();
		return;
	}
	OrbitalRec	*MOs=NULL;
	//Get the appropriate MO vector
	BasisSet * Basis = lData->GetBasisSet();
	long NumBasisFuncs = Basis->GetNumBasisFuncs(UseSphericalHarmonics());
	float		*MOVector=NULL;
	//	if (Options&1) {	//use AOs
//		MOs = (OrbitalRec *) 1;
//	} else if (Options & 4) {	//or use the Localized MO's
//		MOs = lOrbs->LocalOrbitals;
//	} else if (Options & 2) {	//Plot the eigenvectors
//		MOs = lOrbs->EigenVectors;
//	} else if (Options & 8) {	//Plot the oriented LMO's
//		MOs = lOrbs->OrientedLocalOrbitals;
//	}
	if (UseAOs()) {	//Use AOs
		MOs = (OrbitalRec *) 1;
		MOVector = new float[NumBasisFuncs];
		if (MOVector) {
			for (long i=0; i<NumBasisFuncs; i++) MOVector[i] = 0.0;
			MOVector[PlotOrb] = 1.0;	//Set the desired AO to 1
		}
	} else {
		if ((TargetSet >= 0) && (TargetSet < lFrame->Orbs.size())) {
			MOs = lFrame->Orbs[TargetSet];
			if (MOs != NULL) {	//They should never be NULL!
				if (UseBetaSet()) {
					if (PlotOrb>=MOs->NumBetaOrbs) {
						FreeGrid();
						return;
					}	//select the start of the desired vector
					MOVector = &(MOs->VectorsB[NumBasisFuncs*PlotOrb]);
				} else {
					if (PlotOrb>=MOs->NumAlphaOrbs) {
						FreeGrid();
						return;
					}
					MOVector = &(MOs->Vectors[NumBasisFuncs*PlotOrb]);
				}
			}
		}
	}
	
	if (MOs==NULL) {	//Invalid orbitals set choice, free memory and return
		FreeGrid();
		return;
	}
//	if (!(Options&1)) {
//		if (Options & 16) {
//			if (PlotOrb>=MOs->NumBetaOrbs) {
//				FreeGrid();
//				return;
//			}
//			MOVector = &(MOs->VectorsB[NumBasisFuncs*PlotOrb]);
//		} else {
//			if (PlotOrb>=MOs->NumAlphaOrbs) {
//				FreeGrid();
//				return;
//			}
//			MOVector = &(MOs->Vectors[NumBasisFuncs*PlotOrb]);
//		}
//	} else {	//AOs: fake an MOVector with one non-zero coef.
//		MOVector = new float[NumBasisFuncs];
//		if (MOVector) {
//			for (long i=0; i<NumBasisFuncs; i++) MOVector[i] = 0.0;
//			MOVector[PlotOrb] = 1.0;	//Others are init to zero above
//		}
//	}
	if (MOVector == NULL) throw MemoryError();
	SetupGridParameters(lFrame);	//Define the 3D volume

		//allocate the memory needed for the grid
	long		NumPoints = NumXGridPoints*NumYGridPoints*NumZGridPoints;

	if (Grid && (GridAllocation != NumPoints)) {
		FreeGrid();
	}
	if (!Grid) {
			//Attempt to allocate memory for the 3D grid
		AllocateGrid(NumPoints);
	}
		//If the memory allocation failed the MO calculation can not be done
	if (Grid == NULL) return;
		//If sufficient memory is available then setup pointers for fast local use
	float * lGrid;
	lGrid = Grid;
	GridMax = -1.0e20;	//init the Grid max to a value sure to be changed later
#if defined(powerc) && defined(MacintoshBuild)
	if (gMPAware && (gNumProcessors>1)) {
		OSErr	status;
		long	i, start=0;
		MPQueueID	* TerminationQueues = new MPQueueID[gNumProcessors];
		MPTaskID	* TaskIDs = new MPTaskID[gNumProcessors];
		Orb3DGridData * DataPtrs = new Orb3DGridData[gNumProcessors];
		if (!TerminationQueues || !TaskIDs || !DataPtrs) throw MemoryError();
		for (i=0; i<gNumProcessors; i++) {
			status = MPCreateQueue(&(TerminationQueues[i]));	/* Create the queue which will report the completion of the task. */
			if (status != noErr) throw DataError();
			DataPtrs[i].Surf = this;
			DataPtrs[i].TerminationQueueID = TerminationQueues[i];
			DataPtrs[i].xStart = start;
			start += NumXGridPoints/gNumProcessors;
			if (i+1 == gNumProcessors) start = NumXGridPoints;
			DataPtrs[i].xEnd = start;
			DataPtrs[i].Atoms = lFrame->Atoms;
			DataPtrs[i].Basis = Basis;
			DataPtrs[i].MOVector = MOVector;
			DataPtrs[i].NumAtoms = lFrame->NumAtoms;
			DataPtrs[i].PercentDone = 0;
			status = MPCreateTask((TaskProc)CalcGrid,				/* This is the task function. */
                        &(DataPtrs[i]),		/* This is the parameter to the task function. */
                        16*1024,	/* stack size, 0 will use system default of 4KB which is too small for MEPs */
                        TerminationQueues[i],		/* We'll use this to sense task completion. */
                        0,						/* We won't use the first part of the termination message. */
                        0,						/* We won't use the second part of the termination message. */
						0,	/* Use the normal task options. (Currently this *must* be zero!) */
						&(TaskIDs[i]));					/* Here's where the ID of the new task will go. */
			if (status != noErr) throw DataError();
		}
		long NumTasksRunning=gNumProcessors;
		long	TotalPercent;
		lProgress->SetRunTime(0);
		lProgress->SetSleepTime(30);
		while (NumTasksRunning > 0) {
			TotalPercent = 0;
			for (i=0; i<gNumProcessors; i++) {
				TotalPercent += DataPtrs[i].PercentDone;
				if (TerminationQueues[i] != 0) {
						long temp;
					status = MPWaitOnQueue(TerminationQueues[i], 0, 0, (void **) &temp, kDurationImmediate);
					if (status == noErr) {
						GridMax = MAX(GridMax, DataPtrs[i].GridMax);
						status = MPDeleteQueue(TerminationQueues[i]);
						TerminationQueues[i] = 0;
						NumTasksRunning --;
					}
				}
			}
			TotalPercent /= gNumProcessors;
				//Give up the CPU and check for cancels
			if (!lProgress->UpdateProgress(TotalPercent)) {	//User canceled so clean things up and abort
				for (i=0; i<gNumProcessors; i++) {
					if (TerminationQueues[i] != 0) {
  						MPTerminateTask(TaskIDs[i], noErr);
  							long temp;
						status = MPWaitOnQueue(TerminationQueues[i], 0, 0, (void **) &temp, kDurationForever);
						if (status == noErr) {
							status = MPDeleteQueue(TerminationQueues[i]);
							TerminationQueues[i] = 0;
							NumTasksRunning --;
						}
					}
				}
				FreeGrid();
			}
		}
		delete [] TerminationQueues;
		delete [] TaskIDs;
		delete [] DataPtrs;
		lProgress->ResetTimes();
	} else 
#endif
#ifdef __wxBuild__
		if (wxThread::GetCPUCount() > 1) {
			int myCPUCount = wxThread::GetCPUCount();
			//Ok we have multiple CPUs so create a worker thread for each CPU and split up the grid calculation
			//not sure if this is needed. It appears the default is 1?
			//		wxThread::SetConcurrency(myCPUCount);
			
			Orb3DGridData * DataPtrs = new Orb3DGridData[myCPUCount];
			int i;
			Orb3DThreadPtr * myThreads = new Orb3DThreadPtr[myCPUCount];
			long start=0;
			for (i=0; i<myCPUCount; i++) {
				DataPtrs[i].Surf = this;
				DataPtrs[i].xStart = start;
				start += NumXGridPoints/myCPUCount;
				if (i+1 == myCPUCount) start = NumXGridPoints;
				DataPtrs[i].xEnd = start;
				DataPtrs[i].Atoms = lFrame->Atoms;
				DataPtrs[i].Basis = Basis;
				DataPtrs[i].MOVector = MOVector;
				DataPtrs[i].NumAtoms = lFrame->NumAtoms;
				DataPtrs[i].PercentDone = 0;
				
				//Create the actual thread
				myThreads[i] = new Orb3DThread(&(DataPtrs[i]));
				myThreads[i]->Create(16*1024);
				//and fire it off, note my class is always a joinable thread so we will wait on it eventually
				myThreads[i]->Run();
			}
			
			long NumTasksRunning=myCPUCount;
			long	TotalPercent;
			while (NumTasksRunning > 0) {
				TotalPercent = 0;
				for (i=0; i<myCPUCount; i++) {
					TotalPercent += DataPtrs[i].PercentDone;
					if (myThreads[i] != NULL) {
						if (! myThreads[i]->IsAlive()) {
							//The thread is terminating
							myThreads[i]->Wait();
							GridMax = MAX(GridMax, DataPtrs[i].GridMax);
							delete myThreads[i];
							myThreads[i] = NULL;
							NumTasksRunning --;
						}
					}
				}
				TotalPercent /= myCPUCount;
				//Give up the CPU and check for cancels
				if (!lProgress->UpdateProgress(TotalPercent)) {	//User canceled so clean things up and abort
					for (i=0; i<myCPUCount; i++) {
						if (myThreads[i] != NULL) {
							//The docs seem to indicate that Wait causes a thread to terminate,
							//however this does not seem to be true. Wait appears to have the 
							//expected semantics that it blocks until the thread exits. Delete
							//appears to be what is needed.
							myThreads[i]->Delete();
							delete myThreads[i];
							myThreads[i] = NULL;
							NumTasksRunning --;
						}
					}
					GridMax = 1.0;
					FreeGrid();
				} else {
					wxMilliSleep(100);
				}
			}
			
			delete [] DataPtrs;
			delete[] myThreads;
		} else
#endif
		{
			long junk;	//just a placeholder, not used for cooperative tasks
		GridMax = CalculateGrid(0,NumXGridPoints,lFrame->Atoms, Basis, MOVector,
			lFrame->NumAtoms, lProgress, &junk,false);
	}
		//Unlock the grid handle and return
	if (Options & 1) delete [] MOVector;
		//Convert the grid values to Angstroms
	Origin *= kBohr2AngConversion;
	XGridInc *= kBohr2AngConversion;
	YGridInc *= kBohr2AngConversion;
	ZGridInc *= kBohr2AngConversion;
}
float Orb3DSurface::CalculateGrid(long xStart, long xEnd, mpAtom * Atoms, BasisSet * Basis,
		float * MOVector, long NumAtoms, Progress * lProgress, long * PercentDone, bool MPTask) {
	float lGridMax = -1.0e20;
	float lGridMin = 1.0e20;
	float XGridValue, YGridValue, ZGridValue;
	long	iXPt, iYPt, iZPt;
	long n=xStart*(NumYGridPoints*NumZGridPoints);
	float * lGrid;
#ifdef __wxBuild__
	wxStopWatch timer;
	long CheckTime = timer.Time();
#endif
	lGrid = Grid;
	float PhaseChange = 1.0;
	if (GetPhaseChange()) PhaseChange = -1.0;

	XGridValue = Origin.x + xStart * XGridInc;
	for (iXPt=xStart; iXPt<xEnd; iXPt++) {
			//Note the percent done for MP tasks such that the progress dlog can be updated
		if (MPTask) *PercentDone = 100*(iXPt-xStart)/(xEnd-xStart);
		YGridValue = Origin.y;
		for (iYPt=0; iYPt<NumYGridPoints; iYPt++) {
			if (!MPTask) {
					//Give up the CPU and check for cancels if running cooperatively
				if (!lProgress->UpdateProgress(100*iXPt/xEnd)) {	//User canceled so clean things up and abort
					FreeGrid();
					return 0.0;
				}
			}
#ifdef __wxBuild__
			else {
				long tempTime  = timer.Time();
				if ((tempTime - CheckTime) > 50) {
					CheckTime = tempTime;
					if ((wxThread::This())->TestDestroy()) {
						//Getting here means the caller has requested a cancel so exit
						return 0.0;
					}
				}
			}
#endif
			ZGridValue = Origin.z;
			for (iZPt=0; iZPt<NumZGridPoints; iZPt++) {
				lGrid[n] = PhaseChange * CalculateMOAmplitude(XGridValue, YGridValue, ZGridValue,
					Atoms, Basis, MOVector, NumAtoms,UseSphericalHarmonics());
			
				lGridMax = MAX(lGridMax, lGrid[n]);
				lGridMin = MIN(lGridMin, lGrid[n]);
				n++;
				ZGridValue += ZGridInc;
			}
			YGridValue += YGridInc;
		}
		XGridValue += XGridInc;
	}
	lGridMax = MAX(lGridMax, fabs(lGridMin));
	return lGridMax;
}
#ifdef powerc
typedef struct TE3DGridData {
	TEDensity3DSurface *	Surf;
	long	xStart;
	long	xEnd;
	mpAtom *	Atoms;
	BasisSet *	Basis;
	float	*	Vectors;
	float	*	VectorsB;
	float	*	OccupancyA;
	float	*	OccupancyB;
	float	*	AOVector;
	long		NumOccupiedAlphaOrbs;
	long		NumOccupiedBetaOrbs;
	long		NumAtoms;
	long		PercentDone;
	float		GridMax;
	MPQueueID	TerminationQueueID;
} TE3DGridData;
static OSErr CalcTEGrid(TE3DGridData * Data) {
	float GridMax = Data->Surf->CalculateGrid(Data->xStart, Data->xEnd, Data->Atoms, Data->Basis,
		Data->Vectors, Data->VectorsB, Data->OccupancyA, Data->OccupancyB, Data->AOVector, Data->NumOccupiedAlphaOrbs,
		Data->NumOccupiedBetaOrbs, Data->NumAtoms, NULL, &(Data->PercentDone), true);
	Data->GridMax = GridMax;
	return noErr;
}
#endif
#ifdef __wxBuild__
typedef struct TE3DGridData {
	TEDensity3DSurface *	Surf;
	long	xStart;
	long	xEnd;
	mpAtom *	Atoms;
	BasisSet *	Basis;
	float	*	Vectors;
	float	*	VectorsB;
	float	*	OccupancyA;
	float	*	OccupancyB;
	float	*	AOVector;
	long		NumOccupiedAlphaOrbs;
	long		NumOccupiedBetaOrbs;
	long		NumAtoms;
	long		PercentDone;
	float		GridMax;
} TE3DGridData;
//derive a class from wxThread that we will use to create the child threads
class TED3DThread : public wxThread {
public:
	TED3DThread(TE3DGridData * data);
	virtual void * Entry();
	
	long GetPercentDone(void) const {return myData->PercentDone;};
	float GetGridMax(void) const {return myData->GridMax;};
	
	TE3DGridData * myData;
};
TED3DThread::TED3DThread(TE3DGridData * data) : wxThread(wxTHREAD_JOINABLE) {
	myData = data;
}
void * TED3DThread::Entry() {
	myData->GridMax = myData->Surf->CalculateGrid(myData->xStart, myData->xEnd, myData->Atoms, myData->Basis,
								myData->Vectors, myData->VectorsB, myData->OccupancyA, myData->OccupancyB,
								myData->AOVector, myData->NumOccupiedAlphaOrbs,
								myData->NumOccupiedBetaOrbs, myData->NumAtoms, NULL, &(myData->PercentDone),
								true);
	myData->PercentDone = 100;
	return NULL;
}
typedef TED3DThread * TED3DThreadPtr;

#endif
float TEDensity3DSurface::CalculateGrid(long xStart, long xEnd, mpAtom * Atoms, BasisSet * Basis,
			float * Vectors, float * VectorsB, float * OccupancyA, float * OccupancyB, float * AOVector,
			long NumOccupiedAlphaOrbs, long NumOccupiedBetaOrbs, long NumAtoms, Progress * lProgress,
			long * PercentDone, bool MPTask) {
	float lGridMax = -1.0e20;
	float lGridMin = 1.0e20;
	float XGridValue, YGridValue, ZGridValue;
	long	iXPt, iYPt, iZPt;
	long NumBasisFuncs = Basis->GetNumBasisFuncs(false);
	long n=xStart*(NumYGridPoints*NumZGridPoints);
	float * lGrid;
#ifdef __wxBuild__
	wxStopWatch timer;
	long CheckTime = timer.Time();
#endif
	lGrid = Grid;

	XGridValue = Origin.x + xStart * XGridInc;
	for (iXPt=xStart; iXPt<xEnd; iXPt++) {
			//Note the percent done for MP tasks such that the progress dlog can be updated
		YGridValue = Origin.y;
		for (iYPt=0; iYPt<NumYGridPoints; iYPt++) {
			if (!MPTask) {
					//Give up the CPU and check for cancels
				if (!lProgress->UpdateProgress(100*iXPt/xEnd)) {	//User canceled so clean things up and abort
					FreeGrid();
					return 0.0;
				}
			} else {
				*PercentDone = 100*((iXPt-xStart)*NumYGridPoints + iYPt)/((xEnd-xStart)*NumYGridPoints);
			}
			ZGridValue = Origin.z;
			for (iZPt=0; iZPt<NumZGridPoints; iZPt++) {
					//First calculate the ampiltude of each AO at the current grid point
				CalculateAOAmplitudeVector(XGridValue, YGridValue, ZGridValue,
					Atoms, Basis, AOVector, NumAtoms);
				
					float *MOVector, Density, Sum;
					long MONum;
				Sum = 0.0;
				if (NumOccupiedAlphaOrbs) {
					for (MONum=0; MONum<NumOccupiedAlphaOrbs; MONum++) {
						MOVector = &(Vectors[NumBasisFuncs*MONum]);
						Density = 0.0;
						for (long ivec=0; ivec<NumBasisFuncs; ivec++)
							Density += MOVector[ivec]*AOVector[ivec];
						Sum += Density*Density*OccupancyA[MONum];
					}
				}
				if (OccupancyB) {
					for (MONum=0; MONum<NumOccupiedBetaOrbs; MONum++) {
						MOVector = &(VectorsB[NumBasisFuncs*MONum]);
						Density = 0.0;
						for (long ivec=0; ivec<NumBasisFuncs; ivec++)
							Density += MOVector[ivec]*AOVector[ivec];
						Sum += Density*Density*OccupancyB[MONum];
					}
				}
				lGrid[n] = Sum;
				lGridMax = MAX(lGridMax, lGrid[n]);
				lGridMin = MIN(lGridMin, lGrid[n]);
				n++;
				ZGridValue += ZGridInc;
#ifdef __wxBuild__
				if (MPTask) {
					long tempTime  = timer.Time();
					if ((tempTime - CheckTime) > 50) {
						CheckTime = tempTime;
						if ((wxThread::This())->TestDestroy()) {
							//Getting here means the caller has requested a cancel so exit
							return 0.0;
							//		Exit();	//Exit gracefully exits the thread
						}
					}
				}
#endif
			}
			YGridValue += YGridInc;
		}
		XGridValue += XGridInc;
	}
	lGridMax = MAX(lGridMax, fabs(lGridMin));

	return lGridMax;
}
//Generates a 3D grid over the specified MO in the plane specified.
//NOTE: This routine assumes that the atomic coordinates are in Angstroms and are thus
//converted to Bohr when used.
void TEDensity3DSurface::CalculateMOGrid(MoleculeData *lData, Progress * lProgress) {
	Frame *	lFrame = lData->GetCurrentFramePtr();
	if (lFrame->Orbs.size() <= 0) return;
	if (OrbSet > lFrame->Orbs.size()) return;	//Invalid choice of an orbital set
	if (OrbSet < 0) {	//pick a default set
		long i=0;
		std::vector<OrbitalRec *>::const_iterator Set = lFrame->Orbs.begin();
		while (Set != lFrame->Orbs.end()) {
			if ((*Set)->getNumOccupiedAlphaOrbitals() > 0) {
				OrbSet = i;
				break;
			}
		}
		if (OrbSet < 0) return;
	}

	SetupGridParameters(lFrame);	//Define the 3D volume

		//allocate the memory needed for the grid
	long		NumPoints = NumXGridPoints*NumYGridPoints*NumZGridPoints;

	if (Grid && (GridAllocation != NumPoints)) FreeGrid();
	if (!Grid) {
			//Attempt to allocate memory for the 3D grid
		AllocateGrid(NumPoints);
	}
		//If the memory allocation failed the MO calculation can not be done
	if (Grid == NULL) return;
		//If sufficient memory is available then setup pointers for fast local use
	float * lGrid;
	lGrid = Grid;
		//Get the appropriate MO vector
	BasisSet * Basis = lData->GetBasisSet();
	long NumBasisFuncs = Basis->GetNumBasisFuncs(false);
	OrbitalRec	*MOs=NULL;

	MOs = lFrame->Orbs[OrbSet];	//Need to allow selection
 
	if (MOs==NULL) {	//Invalid orbitals set choice, free memory and return
		FreeGrid();
		return;
	}
	float		*OccupancyA=NULL, *OccupancyB=NULL, *Vectors=NULL, *VectorsB=NULL;
	bool	deleteTempOcc = false;
	if (MOs->getOrbitalWavefunctionType() == RHF) {
		OccupancyA = new float[MOs->NumOccupiedAlphaOrbs];
		if (OccupancyA) for (long i=0; i<MOs->NumOccupiedAlphaOrbs; i++) OccupancyA[i]=2.0;
		Vectors = MOs->Vectors;
		deleteTempOcc = true;
	} else if (MOs->getOrbitalWavefunctionType() == ROHF) {
		OccupancyA = new float[MOs->NumOccupiedAlphaOrbs];
		if (OccupancyA) for (long i=0; i<MOs->NumOccupiedAlphaOrbs; i++) {
			if (i<MOs->NumOccupiedBetaOrbs) OccupancyA[i]=2.0;
			else OccupancyA[i]=1.0;
		}
		Vectors = MOs->Vectors;
		deleteTempOcc = true;
	} else if ((MOs->getOrbitalWavefunctionType() == UHF)&&(MOs->getOrbitalType() != NaturalOrbital)) {
			long i;
		OccupancyA = new float[MOs->NumOccupiedAlphaOrbs];
		if (OccupancyA) for (i=0; i<MOs->NumOccupiedAlphaOrbs; i++) OccupancyA[i]=1.0;
		OccupancyB = new float[MOs->NumOccupiedBetaOrbs];
		if (OccupancyB) for (i=0; i<MOs->NumOccupiedBetaOrbs; i++) OccupancyB[i]=1.0;
		Vectors = MOs->Vectors;
		VectorsB = MOs->VectorsB;
		deleteTempOcc = true;
//	} else if ((MOs->getOrbitalWavefunctionType() == MCSCF)||(MOs->getOrbitalWavefunctionType() == RHFMP2)||
//			   ((MOs->getOrbitalWavefunctionType() == UHF)&&(MOs->getOrbitalType() == NaturalOrbital))) {
	} else {
		OccupancyA = MOs->OrbOccupation;
		Vectors = MOs->Vectors;
	}
	if ((Vectors == NULL)||(OccupancyA == NULL)) {
		if (deleteTempOcc) {
			if (OccupancyA) delete [] OccupancyA;
			if (OccupancyB) delete [] OccupancyB;
		}
		FreeGrid();
		return;
	}
		//Store the Grid mins and incs at the beginning of the grid
	GridMax = -1.0e20;
#if defined(powerc) && defined(MacintoshBuild)
	if (gMPAware && (gNumProcessors>1)) {
		OSErr	status;
		long	i, start=0;
		TE3DGridData * DataPtrs = new TE3DGridData[gNumProcessors];
		if (!DataPtrs) throw MemoryError();
			//Clear pointers in case allocation fails and I need to delete part of them
		for (i=0; i<gNumProcessors; i++) DataPtrs[i].AOVector = NULL;
		for (i=0; i<gNumProcessors; i++) {
			DataPtrs[i].AOVector = new float[NumBasisFuncs];
			if (!DataPtrs[i].AOVector) {
				for (int j=0; j<i; j++) if (DataPtrs[j].AOVector) delete [] DataPtrs[j].AOVector;
				delete [] DataPtrs;
				throw MemoryError();
			}
		}
			//Test allocations
		MPQueueID	* TerminationQueues = new MPQueueID[gNumProcessors];
		MPTaskID	* TaskIDs = new MPTaskID[gNumProcessors];
		if (!TerminationQueues || !TaskIDs) {
			for (i=0; i<gNumProcessors; i++)
				if (DataPtrs[i].AOVector) delete [] DataPtrs[i].AOVector;
			delete [] DataPtrs;
			throw MemoryError();
		}
		for (i=0; i<gNumProcessors; i++) {
			status = MPCreateQueue(&(TerminationQueues[i]));	/* Create the queue which will report the completion of the task. */
			if (status != noErr) throw DataError();
			DataPtrs[i].Surf = this;
			DataPtrs[i].TerminationQueueID = TerminationQueues[i];
			DataPtrs[i].xStart = start;
			start += NumXGridPoints/gNumProcessors;
			if (i+1 == gNumProcessors) start = NumXGridPoints;
			DataPtrs[i].xEnd = start;
			DataPtrs[i].Atoms = lFrame->Atoms;
			DataPtrs[i].Basis = Basis;
			DataPtrs[i].Vectors = Vectors;
			DataPtrs[i].VectorsB = VectorsB;
			DataPtrs[i].OccupancyA = OccupancyA;
			DataPtrs[i].OccupancyB = OccupancyB;
			DataPtrs[i].NumOccupiedAlphaOrbs = MOs->NumOccupiedAlphaOrbs;
			DataPtrs[i].NumOccupiedBetaOrbs = MOs->NumOccupiedBetaOrbs;
			DataPtrs[i].NumAtoms = lFrame->NumAtoms;
			DataPtrs[i].PercentDone = 0;
			status = MPCreateTask((TaskProc)CalcTEGrid,				/* This is the task function. */
                        &(DataPtrs[i]),		/* This is the parameter to the task function. */
                        16*1024,	/* stack size, 0 will use system default of 4KB which is too small for MEPs */
                        TerminationQueues[i],		/* We'll use this to sense task completion. */
                        0,						/* We won't use the first part of the termination message. */
                        0,						/* We won't use the second part of the termination message. */
						0,	/* Use the normal task options. (Currently this *must* be zero!) */
						&(TaskIDs[i]));					/* Here's where the ID of the new task will go. */
			if (status != noErr) throw DataError();
		}
		long NumTasksRunning=gNumProcessors;
		long	TotalPercent;
		lProgress->SetRunTime(0);
		lProgress->SetSleepTime(30);
		while (NumTasksRunning > 0) {
			TotalPercent = 0;
			for (i=0; i<gNumProcessors; i++) {
				TotalPercent += DataPtrs[i].PercentDone;
				if (TerminationQueues[i] != 0) {
						long temp;
					status = MPWaitOnQueue(TerminationQueues[i], 0, 0, (void **) &temp, kDurationImmediate);
					if (status == noErr) {
						GridMax = MAX(GridMax, DataPtrs[i].GridMax);
						status = MPDeleteQueue(TerminationQueues[i]);
						TerminationQueues[i] = 0;
						NumTasksRunning --;
					}
				}
			}
			TotalPercent /= gNumProcessors;
				//Give up the CPU and check for cancels
			if (!lProgress->UpdateProgress(TotalPercent)) {	//User canceled so clean things up and abort
				for (i=0; i<gNumProcessors; i++) {
					if (TerminationQueues[i] != 0) {
  						MPTerminateTask(TaskIDs[i], noErr);
  							long temp;
						status = MPWaitOnQueue(TerminationQueues[i], 0, 0, (void **) &temp, kDurationForever);
						if (status == noErr) {
							status = MPDeleteQueue(TerminationQueues[i]);
							TerminationQueues[i] = 0;
							NumTasksRunning --;
						}
					}
				}
				FreeGrid();
			}
		}
		delete [] TerminationQueues;
		delete [] TaskIDs;
		for (i=0; i<gNumProcessors; i++) if (DataPtrs[i].AOVector) delete [] DataPtrs[i].AOVector;
		delete [] DataPtrs;
		lProgress->ResetTimes();
	} else 
#endif
#ifdef __wxBuild__
		if (wxThread::GetCPUCount() > 1) {
		int myCPUCount = wxThread::GetCPUCount();
		//Ok we have multiple CPUs so create a worker thread for each CPU and split up the grid calculation
			//not sure if this is needed. It appears the default is 1?
//		wxThread::SetConcurrency(myCPUCount);

		TE3DGridData * DataPtrs = new TE3DGridData[myCPUCount];
		//Clear pointers in case allocation fails and I need to delete part of them
		int i;
		for (i=0; i<myCPUCount; i++) DataPtrs[i].AOVector = NULL;
		for (i=0; i<myCPUCount; i++) {
			DataPtrs[i].AOVector = new float[NumBasisFuncs];
			if (!DataPtrs[i].AOVector) {
				for (int j=0; j<i; j++) if (DataPtrs[j].AOVector) delete [] DataPtrs[j].AOVector;
				delete [] DataPtrs;
				throw MemoryError();
			}
		}
		TED3DThreadPtr * myThreads = new TED3DThreadPtr[myCPUCount];
		long start=0;
		for (i=0; i<myCPUCount; i++) {
			DataPtrs[i].Surf = this;
			DataPtrs[i].xStart = start;
			start += NumXGridPoints/myCPUCount;
			if (i+1 == myCPUCount) start = NumXGridPoints;
			DataPtrs[i].xEnd = start;
			DataPtrs[i].Atoms = lFrame->Atoms;
			DataPtrs[i].Basis = Basis;
			DataPtrs[i].Vectors = Vectors;
			DataPtrs[i].VectorsB = VectorsB;
			DataPtrs[i].OccupancyA = OccupancyA;
			DataPtrs[i].OccupancyB = OccupancyB;
			DataPtrs[i].NumOccupiedAlphaOrbs = MOs->NumOccupiedAlphaOrbs;
			DataPtrs[i].NumOccupiedBetaOrbs = MOs->NumOccupiedBetaOrbs;
			DataPtrs[i].NumAtoms = lFrame->NumAtoms;
			DataPtrs[i].PercentDone = 0;
			
			//Create the actual thread
			myThreads[i] = new TED3DThread(&(DataPtrs[i]));
			myThreads[i]->Create(16*1024);
			//and fire it off, note my class is always a joinable thread so we will wait on it eventually
			myThreads[i]->Run();
		}

		long NumTasksRunning=myCPUCount;
		long	TotalPercent;
		while (NumTasksRunning > 0) {
			TotalPercent = 0;
			for (i=0; i<myCPUCount; i++) {
				TotalPercent += DataPtrs[i].PercentDone;
				if (myThreads[i] != NULL) {
					if (! myThreads[i]->IsAlive()) {
						//The thread is terminating
						myThreads[i]->Wait();
						GridMax = MAX(GridMax, DataPtrs[i].GridMax);
						delete myThreads[i];
						myThreads[i] = NULL;
						NumTasksRunning --;
					}
				}
			}
			TotalPercent /= myCPUCount;
			//Give up the CPU and check for cancels
			if (!lProgress->UpdateProgress(TotalPercent)) {	//User canceled so clean things up and abort
				for (i=0; i<myCPUCount; i++) {
					if (myThreads[i] != NULL) {
						//The docs seem to indicate that Wait causes a thread to terminate,
						//however this does not seem to be true. Wait appears to have the 
						//expected semantics that it blocks until the thread exits. Delete
						//appears to be what is needed.
						myThreads[i]->Delete();
						delete myThreads[i];
						myThreads[i] = NULL;
						NumTasksRunning --;
					}
				}
				GridMax = 1.0;
				FreeGrid();
			} else {
				wxMilliSleep(100);
			}
		}
		
		for (i=0; i<myCPUCount; i++) if (DataPtrs[i].AOVector) delete [] DataPtrs[i].AOVector;
		delete [] DataPtrs;
		delete[] myThreads;
	} else
#endif
	{
		long junk;	//just a placeholder when cooperative task
		float * AOVector = new float[NumBasisFuncs];
		if (AOVector == NULL) throw MemoryError();
		GridMax = CalculateGrid(0,NumXGridPoints,lFrame->Atoms,  Basis,
			Vectors, VectorsB, OccupancyA, OccupancyB, AOVector,
			MOs->NumOccupiedAlphaOrbs, MOs->NumOccupiedBetaOrbs, lFrame->NumAtoms, lProgress, &junk, false);
		delete [] AOVector;
	}
		//Unlock the grid handle and return
	if (deleteTempOcc) {
		if (OccupancyA) delete [] OccupancyA;
		if (OccupancyB) delete [] OccupancyB;
	}
	Origin *= kBohr2AngConversion;
	XGridInc *= kBohr2AngConversion;
	YGridInc *= kBohr2AngConversion;
	ZGridInc *= kBohr2AngConversion;
}
