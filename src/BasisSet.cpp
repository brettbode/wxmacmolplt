/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	BasisSet.cpp

	Basis Set classes

	Seperated from other files 4/1998 BMB
*/

#include "Globals.h"
#include "GlobalExceptions.h"
#include "BFiles.h"
#include "BasisSet.h"
#include <math.h>
#include <string.h>

//	Basis set related routines

//		Constructor and destructor
BasisSet::BasisSet(long nAtoms, long nShells) {
	Shells = new BasisShell[nShells];
	NumShells = NumFuncs = 0;
	MapLength = nAtoms;
	if (Shells) BasisMap = new long[2*nAtoms];
	if (BasisMap) NuclearCharge = new long[2*nAtoms];
	if ((Shells==NULL)||(BasisMap==NULL)||(NuclearCharge==NULL)) throw MemoryError();	//Check on memory allocation
	for (long i=0; i<nAtoms; i++) {BasisMap[2*i] = 0; BasisMap[2*i+1]=-1;}	//clear the basismap
	NuclearCharge[0] = -1;
}
BasisSet::~BasisSet(void) {
	if (Shells) delete [] Shells;
	if (BasisMap) delete [] BasisMap;
	if (NuclearCharge) delete [] NuclearCharge;
}

//		IO related routines
long BasisSet::GetSize(BufferFile *Buffer) {
	Boolean	cState = Buffer->GetOutput();
	Buffer->SetOutput(false);
	long length = WriteToBuffer(Buffer);
	Buffer->SetOutput(cState);
	return length;
}
long BasisSet::WriteToBuffer(BufferFile *Buffer) {
	long length, code, total;
	
	code = 4;
	total = Buffer->Write((Ptr) &code, sizeof(long));
	length = 3*sizeof(long);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) &MapLength, sizeof(long));
	total += Buffer->Write((Ptr) &NumShells, sizeof(long));
	total += Buffer->Write((Ptr) &NumFuncs, sizeof(long));
	code = 2;
	for (int i=0; i<NumShells; i++) {
		total += Buffer->Write((Ptr) &code, sizeof(long));
		length = Shells[i].GetSize(Buffer);
		total += Buffer->Write((Ptr) &length, sizeof(long));
		total += Shells[i].WriteToBuffer(Buffer);
	}
	code = 3;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = 2*MapLength*sizeof(long);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) BasisMap, length);
	code = 5;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = MapLength*sizeof(long);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) NuclearCharge, length);
	
	return total;
}
BasisSet * BasisSet::Read(BufferFile *Buffer, long NumAtoms, long FullLength) {
	long code, length;
	long nShells, nFuncs, nMap;
	BasisSet * target=NULL;
//	long StartPos = Buffer->GetFilePos();

	Buffer->Read((Ptr) &code, sizeof(long));
	Buffer->Read((Ptr) &length, sizeof(long));
	if (code == 1) {
		if (length != (2*sizeof(Ptr)+3*sizeof(long))) throw DataError();
		Buffer->Read((Ptr) &code, sizeof(long));
		Buffer->Read((Ptr) &code, sizeof(long));
		Buffer->Read((Ptr) &nMap, sizeof(long));
		if (NumAtoms>0) nMap = NumAtoms;
		Buffer->Read((Ptr) &nShells, sizeof(long));
		Buffer->Read((Ptr) &nFuncs, sizeof(long));
		target = new BasisSet(nMap, nShells);
		if (target == NULL) throw MemoryError();
	} else if (code == 4) {
		if (length != 3*sizeof(long)) throw DataError();
		Buffer->Read((Ptr) &nMap, sizeof(long));
		Buffer->Read((Ptr) &nShells, sizeof(long));
		Buffer->Read((Ptr) &nFuncs, sizeof(long));
		target = new BasisSet(nMap, nShells);
		if (target == NULL) throw MemoryError();
	} else throw DataError();
	target->ReadCodes(Buffer, nShells, nFuncs, NumAtoms, (FullLength-Buffer->GetFilePos()));

	return target;
}
void BasisSet::ReadCodes(BufferFile * Buffer, long nShells, long nFuncs, long NumAtoms,
		long FullLength) {
	long code, length;
	long StartPos = Buffer->GetFilePos();

	NumShells = nShells;
	NumFuncs = nFuncs;
	for (int i=0; i<NumShells; i++) {
		Buffer->Read((Ptr) &code, sizeof(long));
		if (code != 2) throw DataError();
		Buffer->Read((Ptr) &length, sizeof(long));
		Shells[i].Read(Buffer, length);
	}
	Buffer->Read((Ptr) &code, sizeof(long));
	if (code != 3) throw DataError();
	Buffer->Read((Ptr) &length, sizeof(long));
	if (NumAtoms>0) MapLength = NumAtoms;
	if (length != 2*MapLength*sizeof(long)) throw DataError();
	Buffer->Read((Ptr) BasisMap, length);
	if (FullLength > (Buffer->GetFilePos() - StartPos)) {
		Buffer->Read((Ptr) &code, sizeof(long));
		if (code == 5) {
			Buffer->Read((Ptr) &length, sizeof(long));
			if (length != MapLength*sizeof(long)) throw DataError();
			Buffer->Read((Ptr) NuclearCharge, length);
		}
	}
}
long BasisSet::GetNumBasisFuncs(Boolean UseSphericalHarmonics) const {
	long result;
	if (!UseSphericalHarmonics)
		result = NumFuncs;
	else {
		result=0;
		for (long atom=0; atom<MapLength; atom++) {
			for (long i=BasisMap[2*atom]; i<=BasisMap[2*atom+1]; i++)
				result += Shells[i].GetNumFuncs(UseSphericalHarmonics);
		}
	}
	return result;
}
void BasisSet::GetShellIndexArray(long * IndexArray) const {
		long i, ShellStart, ShellEnd, j, index=0, shell=0;
	for (i=0; i<NumFuncs; i++) IndexArray[i] = -1;
	for (i=0; i<MapLength; i++) {
		ShellStart = BasisMap[2*i];
		ShellEnd = BasisMap[2*i+1];
		for (j=ShellStart; j<=ShellEnd; j++) {
			IndexArray[shell] = index;
			index += Shells[j].GetNumFuncs(false);
			shell++;
		}
	}
}
void BasisSet::Normalize(Boolean /*InputNormed*/, Boolean /*NormOutput*/) {
//Set up the normalization constants: (2/pi)^3/4 * 2^n/n!!
	float SNorm = sqrt(sqrt(pow(2.0/kPi, 3)));
	float PNorm = SNorm*2;
	float DNorm = PNorm*2/sqrt(3.0);
	float FNorm = DNorm*2/sqrt(5.0);
	float GNorm = FNorm*2/sqrt(7.0);
	for (long ishell=0; ishell<NumShells; ishell++) {
		float *NormCoef, *InputCoef, fExp;
		long	NumPrims;
		short	ShellType = Shells[ishell].ShellType;
		NormCoef = Shells[ishell].NormCoef;
		InputCoef = Shells[ishell].InputCoef;
		NumPrims = Shells[ishell].NumPrims;
		for (long iprim=0; iprim<NumPrims; iprim++) {
			fExp = Shells[ishell].Exponent[iprim];
			double ExpNorm = sqrt(sqrt(fExp));
			double ExpNorm2 = ExpNorm*ExpNorm;
			ExpNorm *= ExpNorm2;
				//Normalization constant is: [fExp^(n+3/2) 2^(2n+3/2)/(pi^3/2 n!!)]^1/2
				//This factor is correct for functions of the form xx, xxx, etc but is off
				//for the xy, xxy, etc functions which must be multiplied by sqrt3, sqrt5 etc later
			switch (ShellType) {
				case -1:	//L shells
					NormCoef[iprim+NumPrims] = PNorm*ExpNorm*ExpNorm2*InputCoef[iprim+NumPrims];
				case 0:		//S shell
					NormCoef[iprim] = SNorm*ExpNorm*InputCoef[iprim];
				break;
				case 1:		//P shell
					NormCoef[iprim] = PNorm*ExpNorm*ExpNorm2*InputCoef[iprim];
				break;
				case 2:		//D shell
					NormCoef[iprim] = DNorm*fExp*ExpNorm*InputCoef[iprim];
				break;
				case 3:		//F shell
					NormCoef[iprim] = FNorm*fExp*ExpNorm*ExpNorm2*InputCoef[iprim];
				break;
				case 4:		//G shell
					NormCoef[iprim] = GNorm*fExp*fExp*ExpNorm*InputCoef[iprim];
				break;
			}
		}
	}
}
void BasisSet::WriteBasis(BufferFile * File, long AtomNum) const {
	float PI = acos(-1.0);
	float PI32 = PI * sqrt(PI);
	if ((AtomNum < MapLength)&&(AtomNum>=0)) {
		long StartShell = BasisMap[2*AtomNum];
		long EndShell = BasisMap[2*AtomNum + 1];
			char	ShellLabel, Line[kMaxLineLength];
			long	ishell, iprim;
		for (ishell=StartShell; ishell<=EndShell; ishell++) {
			switch (Shells[ishell].ShellType) {
				case LShell:
					ShellLabel = 'L';
				break;
				case SShell:
					ShellLabel = 'S';
				break;
				case PShell:
					ShellLabel = 'P';
				break;
				case DShell:
					ShellLabel = 'D';
				break;
				case FShell:
					ShellLabel = 'F';
				break;
				case GShell:
					ShellLabel = 'G';
				break;
			}
			sprintf(Line, "    %c %d", ShellLabel, Shells[ishell].NumPrims);
			File->WriteLine(Line, true);
			for (iprim=0; iprim<Shells[ishell].NumPrims; iprim++) {
				float EE = Shells[ishell].Exponent[iprim] * Shells[ishell].Exponent[iprim];
				float FACS = PI32/(EE*sqrt(EE));
				float FACP = 0.5*FACS/EE;
				float FACD = 0.75*FACS/(EE*EE);
				float FACF = 1.875*FACS/(EE*EE*EE);
				float FACG = 6.5625*FACS/(EE*EE*EE*EE);
				float backconv, backl;
				switch (Shells[ishell].ShellType) {
					case LShell:
						backl = sqrt(FACP);
					case SShell:
						backconv = sqrt(FACS);
					break;
					case PShell:
						backconv = sqrt(FACP);
					break;
					case DShell:
						backconv = sqrt(FACD);
					break;
					case FShell:
						backconv = sqrt(FACF);
					break;
					case GShell:
						backconv = sqrt(FACG);
					break;
				}
				sprintf(Line, "        %d %f %f", iprim+1, Shells[ishell].Exponent[iprim],
					Shells[ishell].NormCoef[iprim]*backconv);
				File->WriteLine(Line, false);
				if (Shells[ishell].ShellType == LShell) {
					sprintf(Line, " %f", Shells[ishell].NormCoef[iprim+Shells[ishell].NumPrims]*backl);
					File->WriteLine(Line, false);
				}
				File->WriteLine("", true);
			}
		}
	} else {
		File->WriteLine("Error! Basis not defined for this atom!", true);
	}
	File->WriteLine("",true);
}

BasisShell::BasisShell(void) {
	Exponent = NormCoef = InputCoef = NULL;
	NumPrims = 0;
}
BasisShell::~BasisShell(void) {
	if (Exponent) delete [] Exponent;
	if (NormCoef) delete [] NormCoef;
	if (InputCoef) delete [] InputCoef;
}
long BasisShell::GetNumFuncs(Boolean UseSphericalHarmonics) const {
		//returns the # of functions for the shell based on the shell type
	long ret;
	long type = ShellType;
	if (UseSphericalHarmonics) type += 10;
	switch (type) {
		case LShell:	//L shell
		case SHLShell:
			ret = 4;
		break;
		case SShell:	//S shell
		case SHSShell:
			ret = 1;
		break;
		case PShell:	//P shell
		case SHPShell:
			ret = 3;
		break;
		case DShell:	//D shell
			ret = 6;
		break;
		case FShell:	//F shell
			ret = 10;
		break;
		case GShell:	//G shell
			ret = 15;
		break;
		case SHDShell:	//Spherical harmonic D
			ret = 5;
		break;
		case SHFShell:	//Spherical harmonic F
			ret = 7;
		break;
		case SHGShell:	//Spherical harmonic G
			ret = 9;
		break;
	}
	return ret;
}
//Return the start of the function in the angular momenta list (MINI)
long BasisShell::GetAngularStart(Boolean UseSphericalHarmonics) const {
	long ret;
	long type = ShellType;
	if (UseSphericalHarmonics) type += 10;
	switch (type) {
		case LShell:	//L shell
		case SHLShell:
			ret = 0;
		break;
		case SShell:	//S shell
		case SHSShell:
			ret = 0;
		break;
		case PShell:	//P shell
		case SHPShell:
			ret = 1;
		break;
		case DShell:	//D shell
		case SHDShell:
			ret = 4;
		break;
		case FShell:	//F shell
			ret = 10;
		break;
		case GShell:	//G shell
			ret = 20;
		break;
		case SHFShell:
			ret = 9;
		break;
		case SHGShell:
			ret = 16;
		break;
	}
	return ret;
}
long BasisShell::GetSize(BufferFile *Buffer) const {
	Boolean	cState = Buffer->GetOutput();
	Buffer->SetOutput(false);
	long length = WriteToBuffer(Buffer);
	Buffer->SetOutput(cState);
	return length;
}
long BasisShell::WriteToBuffer(BufferFile *Buffer) const {
	long	length, total, code;

	code = 4;
	total = Buffer->Write((Ptr) &code, sizeof(long));
	length = 2*sizeof(short);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) &ShellType, sizeof(short));
	total += Buffer->Write((Ptr) &NumPrims, sizeof(short));
	code = 2;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	length = NumPrims*sizeof(float);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) Exponent, length);
	code = 3;
	total += Buffer->Write((Ptr) &code, sizeof(long));
	if (ShellType<0) length = 2*NumPrims*sizeof(float);	//L shell
	else length = NumPrims*sizeof(float);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) NormCoef, length);
	if (InputCoef != NULL) {
		code = 5;
		total += Buffer->Write((Ptr) &code, sizeof(long));
		if (ShellType<0) length = 2*NumPrims*sizeof(float);	//L shell
		else length = NumPrims*sizeof(float);
		total += Buffer->Write((Ptr) &length, sizeof(long));
		total += Buffer->Write((Ptr) InputCoef, length);
	}

	return total;
}
void BasisShell::GetLabel(char * label, short FuncNum, Boolean UseSphericalHarmonics) const {
	long type = ShellType;
	if (UseSphericalHarmonics) type += 10;
	switch (type) {
		case LShell:
		case SShell:
		case SHLShell:
		case SHSShell:
			if (FuncNum==0) {	//s or s part of an L shell
				label[0]='S';
				label[1]=0;
				break;
			} else FuncNum--;
		case PShell:
		case SHPShell:
			label[0] = 'P';
			switch (FuncNum) {
				case 0:	//Px
					label[1]='x';
				break;
				case 1:	//Py
					label[1]='y';
				break;
				case 2:	//Pz
					label[1]='z';
				break;
			}
			label[2]=0;
		break;
		case DShell:		//D shell
			label[0]='D';
			switch (FuncNum) {
				case 0:	//Dxx
					label[1]='x';
					label[2]='2';
				break;
				case 1:	//Dyy
					label[1]='y';
					label[2]='2';
				break;
				case 2:	//Dzz
					label[1]='z';
					label[2]='2';
				break;
				case 3:	//Dxy
					label[1]='x';
					label[2]='y';
				break;
				case 4:	//Dxz
					label[1]='x';
					label[2]='z';
				break;
				case 5:	//Dyz
					label[1]='y';
					label[2]='z';
				break;
			}
			label[3]=0;
		break;
		case FShell:		//F shell
			label[0]='F';
			switch (FuncNum) {
				case 0:	//Fxxx
					label[1]='x';
					label[2]='3';
					label[3]=0;
				break;
				case 1:	//Fyyy
					label[1]='y';
					label[2]='3';
					label[3]=0;
				break;
				case 2:	//Fzzz
					label[1]='z';
					label[2]='3';
					label[3]=0;
				break;
				case 3:	//Fxxy
					label[1]='x';
					label[2]='2';
					label[3]='y';
				break;
				case 4:	//Fxxz
					label[1]='x';
					label[2]='2';
					label[3]='z';
				break;
				case 5:	//Fxyy
					label[1]='x';
					label[2]='y';
					label[3]='2';
				break;
				case 6:	//Fyyz
					label[1]='y';
					label[2]='2';
					label[3]='z';
				break;
				case 7:	//Fxzz
					label[1]='x';
					label[2]='z';
					label[3]='2';
				break;
				case 8:	//Fyzz
					label[1]='y';
					label[2]='z';
					label[3]='2';
				break;
				case 9:	//Fxyz
					label[1]='x';
					label[2]='y';
					label[3]='z';
				break;
			}
			label[4]=0;
		break;
		case GShell:		//G shell
			label[0]='G';
			switch (FuncNum) {
				case 0:	//Gx4
					label[1]='x';
					label[2]='4';
					label[3]=0;
				break;
				case 1:	//Gy4
					label[1]='y';
					label[2]='4';
					label[3]=0;
				break;
				case 2:	//Gz4
					label[1]='z';
					label[2]='4';
					label[3]=0;
				break;
				case 3:	//Gx3y
					label[1]='x';
					label[2]='3';
					label[3]='y';
					label[4]=0;
				break;
				case 4:	//Gx3z
					label[1]='x';
					label[2]='3';
					label[3]='z';
					label[4]=0;
				break;
				case 5:	//Gxy3
					label[1]='x';
					label[2]='y';
					label[3]='3';
					label[4]=0;
				break;
				case 6:	//Gy3z
					label[1]='y';
					label[2]='3';
					label[3]='z';
					label[4]=0;
				break;
				case 7:	//Gxz3
					label[1]='x';
					label[2]='z';
					label[3]='3';
					label[4]=0;
				break;
				case 8:	//Gyz3
					label[1]='y';
					label[2]='z';
					label[3]='3';
					label[4]=0;
				break;
				case 9:	//Gx2y2
					label[1]='x';
					label[2]='2';
					label[3]='y';
					label[4]='2';
				break;
				case 10://Gx2z2
					label[1]='x';
					label[2]='2';
					label[3]='z';
					label[4]='2';
				break;
				case 11://Gy2z2
					label[1]='y';
					label[2]='2';
					label[3]='z';
					label[4]='2';
				break;
				case 12://Gx2yz
					label[1]='x';
					label[2]='2';
					label[3]='y';
					label[4]='z';
				break;
				case 13://Gxy2z
					label[1]='x';
					label[2]='y';
					label[3]='2';
					label[4]='z';
				break;
				case 14://Gxyz2
					label[1]='x';
					label[2]='y';
					label[3]='z';
					label[4]='2';
				break;
			}
			label[5]=0;
		break;
		case SHDShell:
			label[0]='D';
			switch (FuncNum) {
				case 0:
					strcpy((char *) &(label[1]), "xy");
				break;
				case 1:
					strcpy((char *) &(label[1]), "yz");
				break;
				case 2:
					strcpy((char *) &(label[1]), "3z2-r2");
				break;
				case 3:
					strcpy((char *) &(label[1]), "xz");
				break;
				case 4:
					strcpy((char *) &(label[1]), "x2-y2");
				break;
			}
		break;
		case SHFShell:
			label[0]='F';
			switch (FuncNum) {
				case 0:
					strcpy((char *) &(label[1]), "x3-3xy2");
				break;
				case 1:
					strcpy((char *) &(label[1]), "x2z-y2z");
				break;
				case 2:
					strcpy((char *) &(label[1]), "x(5z2-r2)");
				break;
				case 3:
					strcpy((char *) &(label[1]), "z(5z2-3r2)");
				break;
				case 4:
					strcpy((char *) &(label[1]), "y(5z2-r2)");
				break;
				case 5:
					strcpy((char *) &(label[1]), "xyz");
				break;
				case 6:
					strcpy((char *) &(label[1]), "3x2y-y3");
				break;
			}
		break;
		case SHGShell:
			label[0]='G';
			switch (FuncNum) {
				case 0:
					strcpy((char *) &(label[1]), "x4+y4-6x2y2");
				break;
				case 1:
					strcpy((char *) &(label[1]), "xz(x2-3y2)");
				break;
				case 2:
					strcpy((char *) &(label[1]), "(x2-y2)(7z2-r2)");
				break;
				case 3:
					strcpy((char *) &(label[1]), "xz(7z2-3r2)");
				break;
				case 4:
					strcpy((char *) &(label[1]), "35z4-30z2r2+3r4");
				break;
				case 5:
					strcpy((char *) &(label[1]), "yz(7z2-3r2)");
				break;
				case 6:
					strcpy((char *) &(label[1]), "xy(7z2-r2)");
				break;
				case 7:
					strcpy((char *) &(label[1]), "yz(3x2-y2)");
				break;
				case 8:
					strcpy((char *) &(label[1]), "xy(x2-y2)");
				break;
			}
		break;
	}
}
void BasisShell::Read(BufferFile *Buffer, long totalsize) {
	long	code, length, total;

	total = Buffer->Read((Ptr) &code, sizeof(long));
	if ((code!=1)&&(code!=4)) throw DataError();
	total += Buffer->Read((Ptr) &length, sizeof(long));
	if (code == 1) {
//		if (length!=sizeof(BasisShell)) throw DataError();	//incorrect size
				//12 is old size of a BasisShell
		if (length!=12) throw DataError();	//incorrect size
		total += Buffer->BufferSkip(8);
		total += Buffer->Read((Ptr) &ShellType, sizeof(short));
		total += Buffer->Read((Ptr) &NumPrims, sizeof(short));
	} else if (code == 4) {
		if (length != 2*sizeof(short)) throw DataError();
		total += Buffer->Read((Ptr) &ShellType, sizeof(short));
		total += Buffer->Read((Ptr) &NumPrims, sizeof(short));
	}
	Exponent = NormCoef = NULL;
	Exponent = new float[NumPrims];
	if (ShellType>=0) NormCoef = new float[NumPrims];
	else NormCoef = new float[2*NumPrims];
	if ((Exponent==NULL)||(NormCoef==NULL)) throw MemoryError();
		//next should come the code and length for the exponent array
	total += Buffer->Read((Ptr) &code, sizeof(long));
	if (code != 2) throw DataError();
	total += Buffer->Read((Ptr) &length, sizeof(long));
	if (length != NumPrims*sizeof(float)) throw DataError();
	total += Buffer->Read((Ptr) Exponent, length);
	total += Buffer->Read((Ptr) &code, sizeof(long));
	if (code != 3) throw DataError();
	total += Buffer->Read((Ptr) &length, sizeof(long));
	if ((ShellType>=0)&&(length!=NumPrims*sizeof(float))) throw DataError();
	else if ((ShellType<0)&&(length!=2*NumPrims*sizeof(float))) throw DataError();
	total += Buffer->Read((Ptr) NormCoef, length);
	if ((total + 2*sizeof(long))<totalsize) {
		total += Buffer->Read((Ptr) &code, sizeof(long));
		total += Buffer->Read((Ptr) &length, sizeof(long));
		if (code == 5) {
			if (ShellType>=0) InputCoef = new float[NumPrims];
			else InputCoef = new float[2*NumPrims];
			if ((ShellType>=0)&&(length!=NumPrims*sizeof(float))) throw DataError();
			else if ((ShellType<0)&&(length!=2*NumPrims*sizeof(float))) throw DataError();
			total += Buffer->Read((Ptr) InputCoef, length);
		}
	}
}
//Read in a general basis set from a GAMESS log file
BasisSet * BasisSet::ParseGAMESSBasisSet(BufferFile * Buffer, long NumAtoms) {
	long NumShells=0, NextAtom=0, ShellStart=0,
		fShellStart=0, BasisEndPos, nTotFuncs=0, LinePos, EndPos, iatom, items;
	char	LineText[kMaxLineLength+1];
	
	Buffer->LocateKeyWord("CONTRACTION COEFFICIENTS", 22);
	Buffer->SkipnLines(2);
	long StartPos = Buffer->GetFilePos();
		//go to the end of the basis and get the total # of shells to dimension mem
		//output was changed in June, 2000
	Boolean ShellsFound = Buffer->LocateKeyWord("TOTAL NUMBER OF BASIS SET SHELLS", 33);
	if (!ShellsFound) ShellsFound = Buffer->LocateKeyWord("TOTAL NUMBER OF SHELLS", 22);
	if (ShellsFound) {
		BasisEndPos = Buffer->GetFilePos();
		Buffer->GetLine(LineText);
		LinePos = FindKeyWord(LineText, "=", 1) + 1;
		sscanf(&(LineText[LinePos]),"%ld",&NumShells);
	} else return NULL;
	if (NumShells<=0) return NULL;	//invalid number of shells
	BasisSet * Basis = new BasisSet(NumAtoms, NumShells);
	if (Basis == NULL) return NULL;//No memory!

	Buffer->SetFilePos(StartPos);
	Buffer->SkipnLines(2);	//advance to the first primitive line
//Set up the normalization constants: (2/pi)^3/4 * 2^n/n!!
//	float SNorm = sqrt(sqrt(pow(2.0/kPi, 3)));
//	float PNorm = SNorm*2;
//	float DNorm = PNorm*2/sqrt(3.0);
//	float FNorm = DNorm*2/sqrt(5.0);
//	float GNorm = FNorm*2/sqrt(7.0);
//Note the atom label is skipped since it is just a general text string and the
//Basis set is always given in the same order as the coordinates
	//The ishell loop could be a do-while structure, but this ensures
	//that I don't go off the end of the shell array
	for (long ishell=0; ishell<NumShells; ishell++) {
		float *Exponent, *NormCoef, *InputCoef, fExp, fCoef1, fCoef2, fCoef3, scrap;
		char	ShellText[10];
		long	fShell, scrapl, NumPrims, CoefsperPrim;
		short	ShellType;

		StartPos = Buffer->GetFilePos();
		EndPos = Buffer->FindBlankLine();
		NumPrims = Buffer->GetNumLines(EndPos-StartPos);
		Buffer->GetLine(LineText);
		items = sscanf(LineText, "%ld%5s",&fShell, ShellText);
		if (items != 2) throw DataError();
		CoefsperPrim = 1;
		switch (ShellText[0]) {
			case 'S':
				ShellType=0;
			break;
			case 'P':
				ShellType=1;
			break;
			case 'D':
				ShellType=2;
			break;
			case 'F':
				ShellType=3;
			break;
			case 'G':
				ShellType=4;
			break;
			case 'L':
				ShellType=-1;
				CoefsperPrim = 2;
			break;
			default:
				throw DataError();
		}
		Basis->Shells[ishell].NumPrims = NumPrims;
		Basis->Shells[ishell].ShellType = ShellType;
		Exponent = Basis->Shells[ishell].Exponent = new float[NumPrims*sizeof(float)];
		NormCoef = Basis->Shells[ishell].NormCoef = new float[CoefsperPrim*NumPrims*sizeof(float)];
		InputCoef = Basis->Shells[ishell].InputCoef = new float[CoefsperPrim*NumPrims*sizeof(float)];
		if ((Exponent==NULL)||(NormCoef==NULL)) throw MemoryError();
		Boolean OldStyle = false;
		if (FindKeyWord(LineText, "(", 1) != -1) OldStyle=true;
		for (long iprim=0; iprim<NumPrims; iprim++) {
			if (ShellType>=0) {
				if (OldStyle) {
					items = sscanf(LineText, "%ld%5s%ld%f%f (%f)",&fShell, ShellText,
						&scrapl, &fExp, &fCoef1, &scrap);
					if (items != 6) throw DataError();
					InputCoef[iprim] = scrap;
				} else {
					items = sscanf(LineText, "%ld%5s%ld%f%f",&fShell, ShellText,
						&scrapl, &fExp, &fCoef1);
					if (items != 5) throw DataError();
					InputCoef[iprim] = fCoef1;
				}
			} else {
				if (OldStyle) {
					items = sscanf(LineText, "%ld%5s%ld%f%f (%f)%f (%f)",&fShell, ShellText,
						&scrapl, &fExp, &fCoef1,&scrap,&fCoef2,&fCoef3);
					NormCoef[iprim+NumPrims] = fCoef2;
					InputCoef[iprim+NumPrims] = fCoef3;
					InputCoef[iprim] = scrap;
					if (items != 8) throw DataError();
				} else {
					items = sscanf(LineText, "%ld%5s%ld%f%f %f",&fShell, ShellText,
						&scrapl, &fExp, &fCoef1,&fCoef2);
					NormCoef[iprim+NumPrims] = fCoef2;	//This is not normalized, but we take care of that later
					InputCoef[iprim+NumPrims] = fCoef2;
					InputCoef[iprim] = fCoef1;
					if (items != 6) throw DataError();
				}
			}
			Exponent[iprim] = fExp;
			NormCoef[iprim] = fCoef1;
//Coefficients are already normalized when read in from in front of parenthesis
//so this code is unneeded but might be useful in the future
	/*		ExpNorm = sqrt(sqrt(fExp));
			ExpNorm2 = ExpNorm*ExpNorm;
			ExpNorm *= ExpNorm2;
				//Normalization constant is: [fExp^(n+3/2) 2^(2n+3/2)/(pi^3/2 n!!)]^1/2
				//This factor is correct for functions of the form xx, xxx, etc but is off
				//for the xy, xxy, etc functions which must be multiplied by sqrt3, sqrt5 etc later
			switch (ShellType) {
				case -1:	//L shells
					NormCoef[iprim+NumPrims] = fCoef2*PNorm*ExpNorm*ExpNorm2;
				case 0:		//S shell
					NormCoef[iprim] *= SNorm*ExpNorm;
				break;
				case 1:		//P shell
					NormCoef[iprim] *= PNorm*ExpNorm*ExpNorm2;
				break;
				case 2:		//D shell
					NormCoef[iprim] *= DNorm*fExp*ExpNorm;
				break;
				case 3:		//F shell
					NormCoef[iprim] *= FNorm*fExp*ExpNorm*ExpNorm2;
				break;
				case 4:		//G shell
					NormCoef[iprim] *= GNorm*fExp*fExp*ExpNorm;
				break;
			}*/
			Buffer->GetLine(LineText);
		}
		StartPos = Buffer->GetFilePos();
		Buffer->GetLine(LineText);
		Buffer->SetFilePos(StartPos);
		long iscanerr = sscanf(LineText, "%ld %5s %ld %f (%f) ",&scrapl, ShellText,
			&scrapl, &fExp, &scrap);
		nTotFuncs += Basis->Shells[ishell].GetNumFuncs(false);
		Basis->NumShells++;
		if (iscanerr<3) {//Found next atom label so tidy up the current atom
			Buffer->SkipnLines(2);	//Skip Atom label and blank line
			long nGotShells = ishell-ShellStart+1;
			long nMappedAtoms = (fShell-fShellStart-nGotShells)/nGotShells + 1;
			Basis->NumFuncs += nMappedAtoms*nTotFuncs;
			nTotFuncs = 0;	//reset the function counter for the next atom
			for (iatom=0; iatom<nMappedAtoms; iatom++) {
				Basis->BasisMap[2*NextAtom] = ShellStart;
				Basis->BasisMap[2*NextAtom+1] = ishell;
				NextAtom++;
			}
			ShellStart = ishell+1;
			fShellStart = fShell;
		}
		if (fShell >= NumShells) break;
	}
	return Basis;
}	/*ParseGAMESSBasisSet*/
