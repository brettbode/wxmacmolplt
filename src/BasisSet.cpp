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
#include "MyTypes.h"
#include "BasisSet.h"
#include <math.h>
#include <string.h>
#include <cctype>

//	Basis set related routines

//		Constructor and destructor
BasisSet::BasisSet(long nAtoms, long nShells) {
	Shells.reserve(nShells);
	NumShells = NumFuncs = 0;
	MapLength = nAtoms;
	goodCharges = false;
	BasisMap.reserve(2*nAtoms);
	NuclearCharge.reserve(nAtoms);
	for (long i=0; i<nAtoms; i++) {
		BasisMap.push_back(0);
		BasisMap.push_back(-1);	//clear the basismap
		NuclearCharge.push_back(-1);
	}
}
BasisSet::~BasisSet(void) {
}

long BasisSet::GetNumBasisFuncs(bool UseSphericalHarmonics) const {
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
void BasisSet::Normalize(bool /*InputNormed*/, bool /*NormOutput*/) {
//Set up the normalization constants: (2/pi)^3/4 * 2^n/n!!
	double SNorm = sqrt(sqrt(pow(2.0/kPi, 3)));
	double PNorm = SNorm*2;
	double DNorm = PNorm*2/sqrt(3.0);
	double FNorm = DNorm*2/sqrt(5.0);
	double GNorm = FNorm*2/sqrt(7.0);
	double HNorm = GNorm*2/sqrt(9.0);
	double INorm = HNorm*2/sqrt(11.0);
	for (long ishell=0; ishell<NumShells; ishell++) {
		float	fExp;
		long	NumPrims;
		short	ShellType = Shells[ishell].ShellType;
		std::vector<float> & NormCoef = Shells[ishell].NormCoef;
		std::vector<float> & InputCoef = Shells[ishell].InputCoef;
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
				case LShell:
					NormCoef[iprim+NumPrims] = (float) (PNorm*ExpNorm*ExpNorm2*InputCoef[iprim+NumPrims]);
				case SShell:
					NormCoef[iprim] = (float) (SNorm*ExpNorm*InputCoef[iprim]);
				break;
				case PShell:
					NormCoef[iprim] = (float) (PNorm*ExpNorm*ExpNorm2*InputCoef[iprim]);
				break;
				case DShell:
					NormCoef[iprim] = (float) (DNorm*fExp*ExpNorm*InputCoef[iprim]);
				break;
				case FShell:
					NormCoef[iprim] = (float) (FNorm*fExp*ExpNorm*ExpNorm2*InputCoef[iprim]);
				break;
				case GShell:
					NormCoef[iprim] = (float) (GNorm*fExp*fExp*ExpNorm*InputCoef[iprim]);
				break;
					//These two need to be checked!!!!
				case HShell:
					NormCoef[iprim] = (float) (HNorm*fExp*fExp*ExpNorm*ExpNorm2*InputCoef[iprim]);
					break;
				case IShell:
					NormCoef[iprim] = (float) (INorm*fExp*fExp*fExp*ExpNorm*InputCoef[iprim]);
					break;
			}
		}
	}
}
void BasisSet::WriteBasis(BufferFile * File, long AtomNum) const {
	double PI = acos(-1.0);
	double PI32 = PI * sqrt(PI);
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
				case HShell:
					ShellLabel = 'H';
					break;
				case IShell:
					ShellLabel = 'I';
					break;
			}
			sprintf(Line, "    %c %d", ShellLabel, Shells[ishell].NumPrims);
			File->WriteLine(Line, true);
			for (iprim=0; iprim<Shells[ishell].NumPrims; iprim++) {
				double EE = Shells[ishell].Exponent[iprim] * Shells[ishell].Exponent[iprim];
				double FACS = PI32/(EE*sqrt(EE));
				double FACP =   0.5*FACS/EE;
				double FACD =   0.75*FACS/(EE*EE);
				double FACF =   1.875*FACS/(EE*EE*EE);
				double FACG =   6.5625*FACS/(EE*EE*EE*EE);
				double FACH =  29.53125*FACS/(EE*EE*EE*EE*EE);
				double FACI = 162.421875*FACS/(EE*EE*EE*EE*EE*EE);
				float backconv, backl;
				switch (Shells[ishell].ShellType) {
					case LShell:
						backl = (float) sqrt(FACP);
					case SShell:
						backconv = (float) sqrt(FACS);
					break;
					case PShell:
						backconv = (float) sqrt(FACP);
					break;
					case DShell:
						backconv = (float) sqrt(FACD);
					break;
					case FShell:
						backconv = (float) sqrt(FACF);
					break;
					case GShell:
						backconv = (float) sqrt(FACG);
					break;
					case HShell:
						backconv = (float) sqrt(FACH);
						break;
					case IShell:
						backconv = (float) sqrt(FACI);
						break;
				}
				sprintf(Line, "        %ld %f %f", iprim+1, Shells[ishell].Exponent[iprim],
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
	NumPrims = 0;
}
BasisShell::~BasisShell(void) {
}
long BasisShell::GetNumFuncs(bool UseSphericalHarmonics) const {
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
		case HShell:
			ret = 21;
			break;
		case IShell:
			ret = 28;
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
		case SHHShell:	//Spherical harmonic H
			ret = 11;
			break;
		case SHIShell:	//Spherical harmonic I
			ret = 13;
			break;
	}
	return ret;
}
//Return the start of the function in the angular momenta list (MINI)
long BasisShell::GetAngularStart(bool UseSphericalHarmonics) const {
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
		case HShell:
			ret = 35;
			break;
		case IShell:
			ret = 56;
			break;
		case SHFShell:
			ret = 9;
		break;
		case SHGShell:
			ret = 16;
		break;
		case SHHShell:
			ret = 25;
			break;
		case SHIShell:
			ret = 36;
			break;
	}
	return ret;
}
void BasisShell::GetLabel(char * label, short FuncNum, bool UseSphericalHarmonics) const {
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
		case HShell:
			switch (FuncNum) {
				case 0:
					strcpy(label, "Hx5");
					break;
				case 1:
					strcpy(label, "Hy5");
					break;
				case 2:
					strcpy(label, "Hz5");
					break;
				case 3:
					strcpy(label, "Hx4y");
					break;
				case 4:
					strcpy(label, "Hx4z");
					break;
				case 5:
					strcpy(label, "Hxy4");
					break;
				case 6:
					strcpy(label, "Hy4z");
					break;
				case 7:
					strcpy(label, "Hxz4");
					break;
				case 8:
					strcpy(label, "Hyz4");
					break;
				case 9:
					strcpy(label, "Hx3y2");
					break;
				case 10:
					strcpy(label, "Hx3z2");
					break;
				case 11:
					strcpy(label, "Hx2y3");
					break;
				case 12:
					strcpy(label, "Hy3z2");
					break;
				case 13:
					strcpy(label, "Hx2z3");
					break;
				case 14:
					strcpy(label, "Hy2z3");
					break;
				case 15:
					strcpy(label, "Hx3yz");
					break;
				case 16:
					strcpy(label, "Hxy3z");
					break;
				case 17:
					strcpy(label, "Hxyz3");
					break;
				case 18:
					strcpy(label, "Hx2y2z");
					break;
				case 19:
					strcpy(label, "Hx2yz2");
					break;
				case 20:
					strcpy(label, "Hxy2z2");
					break;
			}
			break;
		case IShell:
			switch (FuncNum) {
				case 0:
					strcpy(label, "Ix6");
					break;
				case 1:
					strcpy(label, "Iy6");
					break;
				case 2:
					strcpy(label, "Iz6");
					break;
				case 3:
					strcpy(label, "Ix5y");
					break;
				case 4:
					strcpy(label, "Ix5z");
					break;
				case 5:
					strcpy(label, "Ixy5");
					break;
				case 6:
					strcpy(label, "Iy5z");
					break;
				case 7:
					strcpy(label, "Ixz5");
					break;
				case 8:
					strcpy(label, "Iyz5");
					break;
				case 9:
					strcpy(label, "Ix4y2");
					break;
				case 10:
					strcpy(label, "Ix4z2");
					break;
				case 11:
					strcpy(label, "Ix2y4");
					break;
				case 12:
					strcpy(label, "Iy4z2");
					break;
				case 13:
					strcpy(label, "Ix2z4");
					break;
				case 14:
					strcpy(label, "Iy2z4");
					break;
				case 15:
					strcpy(label, "Ix4yz");
					break;
				case 16:
					strcpy(label, "Ixy4z");
					break;
				case 17:
					strcpy(label, "Ixyz4");
					break;
				case 18:
					strcpy(label, "Ix3y3");
					break;
				case 19:
					strcpy(label, "Ix3z3");
					break;
				case 20:
					strcpy(label, "Iy3z3");
					break;
				case 21:
					strcpy(label, "Ix3y2z");
					break;
				case 22:
					strcpy(label, "Ix3yz2");
					break;
				case 23:
					strcpy(label, "Ix2y3z");
					break;
				case 24:
					strcpy(label, "Ixy3z2");
					break;
				case 25:
					strcpy(label, "Ix2yz3");
					break;
				case 26:
					strcpy(label, "Ixy2z3");
					break;
				case 27:
					strcpy(label, "Ix2y2z2");
					break;
			}
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
		case SHHShell:
			label[0]='H';
			switch (FuncNum) {
				case 0:
					strcpy((char *) &(label[1]), "x5-10x3y2+5xy4");
					break;
				case 1:
					strcpy((char *) &(label[1]), "z(x4+y4-6x2y2)");
					break;
				case 2:
					strcpy((char *) &(label[1]), "(x3-3xy2)(9z2-r2)");
					break;
				case 3:
					strcpy((char *) &(label[1]), "(x2-y2)(3z3-zr2)");
					break;
				case 4:
					strcpy((char *) &(label[1]), "21xz4-14xz2r2+xr4");
					break;
				case 5:
					strcpy((char *) &(label[1]), "63z5-70z3r2+15zr4");
					break;
				case 6:
					strcpy((char *) &(label[1]), "21yz4-14yz2r2+yr4");
					break;
				case 7:
					strcpy((char *) &(label[1]), "xyz(3z3-zr2)");
					break;
				case 8:
					strcpy((char *) &(label[1]), "(3x2y-y3)(9z2-r2)");
					break;
				case 9:
					strcpy((char *) &(label[1]), "z(x3y-xy3)");
					break;
				case 10:
					strcpy((char *) &(label[1]), "5x4y-10x2y3+5y5");
					break;
			}
		break;
		case SHIShell:
			label[0]='I';
			switch (FuncNum) {
				case 0:
					strcpy((char *) &(label[1]), "x6-15x4y2+15x2y4-y6");
					break;
				case 1:
					strcpy((char *) &(label[1]), "xz(x4+5y4-10x2y2)");
					break;
				case 2:
					strcpy((char *) &(label[1]), "(x4+y4-6x2y2)(11z2-r2)");
					break;
				case 3:
					strcpy((char *) &(label[1]), "(x3-3xy2)z(11z2-3r2)");
					break;
				case 4:
					strcpy((char *) &(label[1]), "(x2-y2)(33z4-18z2r2+r4)");
					break;
				case 5:
					strcpy((char *) &(label[1]), "xz(33z4-30z2r2+5r4");
					break;
				case 6:
					strcpy((char *) &(label[1]), "231z6-315z4r2+105z2r4-5r6");
					break;
				case 7:
					strcpy((char *) &(label[1]), "yz(33z4-30z2r2+5r4");
					break;
				case 8:
					strcpy((char *) &(label[1]), "xy(33z4-18z2r2+r4)");
					break;
				case 9:
					strcpy((char *) &(label[1]), "z(11z2-3r2)(3x2y-y3)");
					break;
				case 10:
					strcpy((char *) &(label[1]), "xy(x2-y2)(11z2-r2)");
					break;
				case 11:
					strcpy((char *) &(label[1]), "yz(y4+5x4-10x2y2)");
					break;
				case 12:
					strcpy((char *) &(label[1]), "xy(3x4-10x2y2+3y4");
					break;
			}
			break;
		default:
			wxLogMessage(_("Undefined shell type not implemented in BasisShell::GetLabel"));
			break;
	}
}
//Read in a general basis set from a GAMESS log file
BasisSet * BasisSet::ParseGAMESSBasisSet(BufferFile * Buffer, long NumAtoms, const mpAtom * Atoms) {
	long NumShells=0, NextAtom=0, ShellStart=0,
		fShellStart=0, BasisEndPos, nTotFuncs=0, LinePos, EndPos, iatom, items;
	char	LineText[kMaxLineLength+1];
	Buffer->GetLine(LineText);
	
	Buffer->LocateKeyWord("CONTRACTION COEFFICIENT", 21);
	Buffer->SkipnLines(2);
	long StartPos = Buffer->GetFilePos();
		//go to the end of the basis and get the total # of shells to dimension mem
		//output was changed in June, 2000
	bool ShellsFound = Buffer->LocateKeyWord("TOTAL NUMBER OF BASIS SET SHELLS", 32);
	if (!ShellsFound) ShellsFound = Buffer->LocateKeyWord("TOTAL NUMBER OF SHELLS", 22);
	if (ShellsFound) {
		BasisEndPos = Buffer->GetFilePos();
		Buffer->GetLine(LineText);
		LinePos = FindKeyWord(LineText, "=", 1) + 1;
		sscanf(&(LineText[LinePos]),"%ld",&NumShells);
	} else return NULL;
	if (NumShells<=0) return NULL;	//invalid number of shells
	if (NumAtoms <= 0) return NULL;
	BasisSet * Basis = new BasisSet(NumAtoms, NumShells);
	if (Basis == NULL) return NULL;//No memory!

	Buffer->SetFilePos(StartPos);
	Buffer->SkipnLines(2);	//advance to the first primitive line
//Note the atom label is skipped since it is just a general text string and the
//Basis set is always given in the same order as the coordinates
	//The ishell loop could be a do-while structure, but this ensures
	//that I don't go off the end of the shell array
	for (long ishell=0; ishell<NumShells; ishell++) {
		float	fExp, fCoef1, fCoef2, fCoef3, scrap;
		char	ShellText[10];
		long	fShell, scrapl, NumPrims, CoefsperPrim;
		short	ShellType;
		BasisShell	foo;

		StartPos = Buffer->GetFilePos();
		EndPos = Buffer->FindBlankLine();
		NumPrims = Buffer->GetNumLines(EndPos-StartPos);
		Buffer->GetLine(LineText);
		items = sscanf(LineText, "%ld%5s",&fShell, ShellText);
		if (items != 2) throw DataError();
		CoefsperPrim = 1;
		switch (ShellText[0]) {
			case 'S':
				ShellType=SShell;
			break;
			case 'P':
				ShellType=PShell;
			break;
			case 'D':
				ShellType=DShell;
			break;
			case 'F':
				ShellType=FShell;
			break;
			case 'G':
				ShellType=GShell;
			break;
			case 'H':
				ShellType=HShell;
				break;
			case 'I':
				ShellType=IShell;
				break;
			case 'L':
				ShellType=LShell;
				CoefsperPrim = 2;
			break;
			default:
				wxString msg;
				msg.Printf(_("Unknown shell type encountered, shell label = %s"), ShellText);
				wxLogMessage(msg);
				throw DataError();
		}
		Basis->Shells.push_back(foo);
		Basis->Shells[ishell].NumPrims = (short) NumPrims;
		Basis->Shells[ishell].ShellType = ShellType;
		std::vector<float> & Exponent = Basis->Shells[ishell].Exponent;
		std::vector<float> & NormCoef = Basis->Shells[ishell].NormCoef;
		std::vector<float> & InputCoef = Basis->Shells[ishell].InputCoef;
		Exponent.reserve(NumPrims);
		if (ShellType >= 0) {
			NormCoef.reserve(NumPrims);
			InputCoef.reserve(NumPrims);
		} else {
			NormCoef.reserve(2*NumPrims);
			InputCoef.reserve(2*NumPrims);
		}
		bool OldStyle = false;
		if (FindKeyWord(LineText, "(", 1) != -1) OldStyle=true;
		for (long iprim=0; iprim<NumPrims; iprim++) {
			InputCoef.push_back(0.0);
			NormCoef.push_back(0.0);
			if (ShellType<0) {
				InputCoef.push_back(0.0);
				NormCoef.push_back(0.0);
			}
			Exponent.push_back(0.0);
		}
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
			//skip over any MM atoms since they aren't included in the basis
			while (Atoms[NextAtom].IsSIMOMMAtom() && (NextAtom < NumAtoms)) NextAtom++;
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

#include <iostream>
bool BasisSet::ReadMolDenBasisSet(BufferFile * Buffer, long NumAtoms) {
	bool result = true;
//	bool use5D=false, use7F=false, use10G=false;
	char	LineText[kMaxLineLength];
	long	shellCount = 0;
	
	Buffer->SkipnLines(1);
	for (int i=0; i<NumAtoms; i++) {
		long shellStart = shellCount;
		long atomNum=-1;
		Buffer->GetLine(LineText);
		sscanf(LineText, "%ld", &atomNum);
		if (atomNum != (i+1)) return false;	//incorrect order? Need to log a message...
		Buffer->GetLine(LineText);
		while (!IsBlank(LineText)) {
			char	token[kMaxLineLength];
			int		numPrims;
			if (sscanf(LineText, "%s %d", token, &numPrims) == 2) {
				short	ShellType;
				if (strlen(token) == 1) {
					switch (toupper(token[0])) {
						case 'S':
							ShellType = SShell;
							break;
						case 'P':
							ShellType = PShell;
							break;
						case 'D':
							ShellType = DShell;
							break;
						case 'F':
							ShellType = FShell;
							break;
						case 'G':
							ShellType = GShell;
							break;
						case 'H':
							ShellType = HShell;
							break;
						case 'I':
							ShellType = IShell;
							break;
						default:
							wxString msg;
							msg.Printf(_("Unknown shell type encountered, shell label = %s"), token);
							wxLogMessage(msg);
							return false; //invalid type
					}
				} else if (strlen(token) == 2) {
					if ((toupper(token[0])=='S')&&(toupper(token[1])=='P'))
						ShellType = -1;
					else return false;
				} else return false; //invalid shell type
				if (numPrims <= 0) return false;
				
				BasisShell t;
				Shells.push_back(t);
				Shells[shellCount].NumPrims = (short) numPrims;
				Shells[shellCount].ShellType = ShellType;
				std::vector<float> & Exponent = Shells[shellCount].Exponent;
				std::vector<float> & NormCoef = Shells[shellCount].NormCoef;
				std::vector<float> & InputCoef = Shells[shellCount].InputCoef;
				Exponent.reserve(numPrims);
				if (ShellType >= 0) {
					NormCoef.reserve(numPrims);
					InputCoef.reserve(numPrims);
				} else {
					NormCoef.reserve(2*numPrims);
					InputCoef.reserve(2*numPrims);
				}
				for (long iprim=0; iprim<numPrims; iprim++) {
					InputCoef.push_back(0.0);
					NormCoef.push_back(0.0);
					if (ShellType<0) {
						InputCoef.push_back(0.0);
						NormCoef.push_back(0.0);
					}
					Exponent.push_back(0.0);
				}
				for (long iprim=0; iprim<numPrims; iprim++) {
					float	fexp, fcoef, fcoefp;
					Buffer->GetLine(LineText);
					ConvertExponentStyle(LineText);
					if (ShellType >= 0) {
						if (sscanf(LineText, "%f %f", &fexp, &fcoef) != 2) return false;
					} else {
						if (sscanf(LineText, "%f %f %f", &fexp, &fcoef, &fcoefp) == 3) {
							InputCoef[iprim+numPrims] = fcoefp;
						} else return false;
					}
					Exponent[iprim] = fexp;
					InputCoef[iprim] = fcoef;
				}
				NumFuncs += Shells[shellCount].GetNumFuncs(false);
				NumShells ++;
				shellCount ++;
			} else return false;
			Buffer->GetLine(LineText);
		}
		BasisMap[2*i] = shellStart;
		BasisMap[2*i+1] = shellCount - 1;
	}
	return result;
}
