/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* Most text file parsing routines

	1/1998 - Modified the GAMESS log file parsing code for the new format BMB
	10/1998 - GAMESS log files also read in normal mode intensities BMB
	11/1998 - Added MDL MolFile input/export code BMB
	12/1998 - modified fragment parser routines BMB
	8/2000 - Fixed log file parser for new MCSCF orbital header and new basis set format
	1/2001 - Moved normal mode parser to Frame::ParseNormalModes BMB
	1/2001 - Added exception class UserCancel to more cleanly handle Progress aborts BMB
	2/2001 - Added check to allow user to disable the change of the creator type for text files BMB
	9/2001 - Fixed initial fragment parser to account for change in GAMESS log file output BMB
	11/2001 - ROHF logs now obtain the correct beta orbital occupation BMB
	11/2001 - OpenGAMESSDRC updated for the modified header in DRC output BMB
	3/2002 - Fixed bug when keywords appear in the Run Title BMB
	3/2002 - Redid ReadControlOptions to remove any order dependance among keywords BMB
	4/2002 - Modified initial fragment parser and fragment energy search BMB
	5/2004 - Modified atomic label parsing to fix conflict between Zr/ZN and fragments BMB
	10/2007 - Added import support for Molkel (.MKL) files ?/BMB/DMR
	6/2008 - Added import support for MOPAC input (.MOP) and archive (.ARC) files DMR
*/

#include "Files.h"
#include "Globals.h"
#include "GlobalExceptions.h"
#include "Progress.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "Gradient.h"
#include "Internals.h"
#include "BasisSet.h"
#include "SurfaceTypes.h"
#include "BFiles.h"
#include "myFiles.h"
#ifndef __wxBuild__
#include "MyWindowClasses.h"
#include "MolDisplay.h"
#include "UtilWin.h"
#include "StdPrefsLib.h"
#else
#include "MolDisplayWin.h"
#endif
#include <iostream>

//#include "mat2quat.h"
#include "InputData.h"
#include "Prefs.h"
#include <string.h>
#include <string>
#include <stdio.h>
#include <cctype>
#include <fstream>
#include <iostream>
//using namespace std;
#include <new>

#if defined(WIN32)
#undef AddAtom
#endif

extern WinPrefs * gPreferences;

	//Local function definitions
bool ReadGVBOccupancy(BufferFile * Buffer, long NumPairOrbs, long MaxOrbs, float * Occupancy);

void FileError::WriteError(void) {
	if (Error == eofErr)
		wxLogMessage(_("Unexpected End Of File. Please check to make sure the file is complete."));
	else {
		wxString err;
		err.Printf(_("File System related error. Please report error number %d."), (int) Error);
		wxLogMessage(err);
	}
}
/**
  * Adds support for opening GAMESS Input (.INP) files. 
  * @param Buffer A BufferFileObject that the .INP or .inp file is buffered
  * into to make parsing the file easier.  See the BufferFile object for 
  * valid BufferFile operations.
*/
long MolDisplayWin::OpenGAMESSInput(BufferFile * Buffer) {
	char	Line[kMaxLineLength], token[kMaxLineLength], DFTTYP[kMaxLineLength]="";
	int	scanerr;
	long	nAtoms;
	wxFileOffset StartPos, EndPos;
	bool	BasisFound=false, BoolTest, EndOfGroup;

	ProgressInd->ChangeText("Reading GAMESS input file...");

	MainData->InputOptions = new InputData;

	EndOfGroup = false;
	if (Buffer->FindGroup("CONTRL")) {
		do {
			Buffer->GetLine(Line);
			if (ReadStringKeyword(Line, "SCFTYP", token))
				MainData->InputOptions->Control->SetSCFType(token);
			if (ReadStringKeyword(Line, "RUNTYP", token))
				MainData->InputOptions->Control->SetRunType(token);
			if (ReadLongKeyword(Line, "MPLEVL", &nAtoms))
				MainData->InputOptions->Control->SetMPLevel(nAtoms);
			if (ReadStringKeyword(Line, "CITYP", token))
				MainData->InputOptions->Control->SetCIType(token);
			if (ReadStringKeyword(Line, "COORD", token))
				MainData->InputOptions->Data->SetCoordType(token);
			if (ReadStringKeyword(Line, "UNITS", token))
				MainData->InputOptions->Data->SetUnits(token);
			if (ReadLongKeyword(Line, "ICHARG", &nAtoms))
				MainData->InputOptions->Control->SetCharge(nAtoms);
			if (ReadStringKeyword(Line, "LOCAL", token))
				MainData->InputOptions->Control->SetLocal(token);
			if (ReadLongKeyword(Line, "MULT", &nAtoms))
				MainData->InputOptions->Control->SetMultiplicity(nAtoms);
			if (ReadLongKeyword(Line, "MAXIT", &nAtoms))
				MainData->InputOptions->Control->SetMaxIt(nAtoms);
			if (ReadStringKeyword(Line, "ECP", token))
				MainData->InputOptions->Basis->SetECPPotential(token);
			if (ReadStringKeyword(Line, "PP", token))
				MainData->InputOptions->Basis->SetECPPotential(token);
			//look for the DFTTYP keyword, but we must allow for reading the DFT group
			//later before setting the functional since it depends on the method
			ReadStringKeyword(Line, "DFTTYP", DFTTYP);
			if (ReadLongKeyword(Line, "ISPHER", &nAtoms)) {
				if (nAtoms == 1)
					MainData->InputOptions->Control->UseSphericalHarmonics(true);
			}
			if (ReadLongKeyword(Line, "NOSYM", &nAtoms)) {
				MainData->InputOptions->Data->SetUseSym((nAtoms==0));
			}
			
			if (-1 < FindKeyWord(Line, "$END", 4)) {	//End of this group
					//scan for multiple occurances of this group
				if (!Buffer->FindGroup("CONTRL")) EndOfGroup = true;
			}
		} while (!EndOfGroup);
	}
	Buffer->SetFilePos(0);	//restart search from beginning of file
	EndOfGroup = false;
	if (Buffer->FindGroup("BASIS")) {
		BasisFound = true;
		do {
			Buffer->GetLine(Line);
			if (ReadStringKeyword(Line, "GBASIS", token))
				MainData->InputOptions->Basis->SetBasis(token);
			if (ReadLongKeyword(Line, "NGAUSS", &nAtoms))
				MainData->InputOptions->Basis->SetNumGauss(nAtoms);
			if (ReadLongKeyword(Line, "NDFUNC", &nAtoms))
				MainData->InputOptions->Basis->SetNumDFuncs(nAtoms);
			if (ReadLongKeyword(Line, "NPFUNC", &nAtoms))
				MainData->InputOptions->Basis->SetNumPFuncs(nAtoms);
			if (ReadLongKeyword(Line, "NFFUNC", &nAtoms))
				MainData->InputOptions->Basis->SetNumFFuncs(nAtoms);
			if (ReadBooleanKeyword(Line, "DIFFS", &BoolTest))
				MainData->InputOptions->Basis->SetDiffuseS(BoolTest);
			if (ReadBooleanKeyword(Line, "DIFFSP", &BoolTest))
				MainData->InputOptions->Basis->SetDiffuseSP(BoolTest);
			if (ReadStringKeyword(Line, "POLAR", token))
				MainData->InputOptions->Basis->SetPolar(token);

			if (-1 < FindKeyWord(Line, "$END", 4)) {	//End of this group
					//scan for multiple occurances of this group
				if (!Buffer->FindGroup("BASIS")) EndOfGroup = true;
			}
		} while (!EndOfGroup);
	}
	Buffer->SetFilePos(0);	//restart search from beginning of file
	EndOfGroup = false;
	if (Buffer->FindGroup("SYSTEM")) {
		do {
			Buffer->GetLine(Line);
			if (ReadLongKeyword(Line, "TIMLIM", &nAtoms))
				MainData->InputOptions->System->SetTimeLimit(nAtoms);
			if (ReadLongKeyword(Line, "MEMORY", &nAtoms))
				MainData->InputOptions->System->SetMemory(nAtoms);
			if (ReadLongKeyword(Line, "MEMDDI", &nAtoms))
				MainData->InputOptions->System->SetMemDDI(nAtoms);
			if (ReadBooleanKeyword(Line, "PARALL", &BoolTest))
				MainData->InputOptions->System->SetParallel(BoolTest);
			if (ReadLongKeyword(Line, "KDIAG", &nAtoms))
				MainData->InputOptions->System->SetDiag(nAtoms);
			if (ReadBooleanKeyword(Line, "COREFL", &BoolTest))
				MainData->InputOptions->System->SetCoreFlag(BoolTest);
			if (ReadStringKeyword(Line, "BALTYP", token)) {
				nAtoms = FindKeyWord(token, "NXTVAL", 6);
				if (nAtoms > -1)
					MainData->InputOptions->System->SetBalanceType(true);
			}
			if (ReadBooleanKeyword(Line, "XDR", &BoolTest))
				MainData->InputOptions->System->SetXDR(BoolTest);

			if (-1 < FindKeyWord(Line, "$END", 4)) {	//End of this group
					//scan for multiple occurances of this group
				if (!Buffer->FindGroup("SYSTEM")) EndOfGroup = true;
			}
		} while (!EndOfGroup);
	}
	Buffer->SetFilePos(0);	//restart search from beginning of file
	EndOfGroup = false;
	if (Buffer->FindGroup("GUESS")) {
		if (!MainData->InputOptions->Guess) MainData->InputOptions->Guess = new GuessGroup;
		do {
			Buffer->GetLine(Line);
			if (ReadStringKeyword(Line, "GUESS", token))
				MainData->InputOptions->Guess->SetGuess(token);
			if (ReadLongKeyword(Line, "NORB", &nAtoms))
				MainData->InputOptions->Guess->SetNumOrbs(nAtoms);
			if (ReadBooleanKeyword(Line, "PRTMO", &BoolTest))
				MainData->InputOptions->Guess->SetPrintMO(BoolTest);
			if (ReadBooleanKeyword(Line, "MIX", &BoolTest))
				MainData->InputOptions->Guess->SetMix(BoolTest);
			
			if (-1 < FindKeyWord(Line, "$END", 4)) {	//End of this group
														//scan for multiple occurances of this group
				if (!Buffer->FindGroup("GUESS")) EndOfGroup = true;
			}
		} while (!EndOfGroup);
	}
	Buffer->SetFilePos(0);	//restart search from beginning of file
	EndOfGroup = false;
	if (Buffer->FindGroup("SCF")) {
		if (!MainData->InputOptions->SCF) MainData->InputOptions->SCF = new SCFGroup;
		do {
			Buffer->GetLine(Line);
			if (ReadBooleanKeyword(Line, "DIRSCF", &BoolTest))
				MainData->InputOptions->SCF->SetDirectSCF(BoolTest);
			if (ReadBooleanKeyword(Line, "FDIFF", &BoolTest))
				MainData->InputOptions->SCF->SetFockDiff(BoolTest);
			if (ReadLongKeyword(Line, "NCONV", &nAtoms))
				MainData->InputOptions->SCF->SetConvergance(nAtoms);
			if (ReadBooleanKeyword(Line, "UHFNOS", &BoolTest))
				MainData->InputOptions->SCF->SetUHFNO(BoolTest);
			if (ReadLongKeyword(Line, "NCO", &nAtoms))
				MainData->InputOptions->SCF->SetGVBNumCoreOrbs(nAtoms);
			if (ReadLongKeyword(Line, "NPAIR", &nAtoms))
				MainData->InputOptions->SCF->SetGVBNumPairs(nAtoms);
			if (ReadLongKeyword(Line, "NSETO", &nAtoms))
				MainData->InputOptions->SCF->SetGVBNumOpenShells(nAtoms);
			if (ReadStringKeyword(Line, "NO(1)", token)) {
				int nchar = 0;
				int tlen = strlen(token);
				MainData->InputOptions->SCF->ClearGVBOpenShellDeg();
				while (nchar < tlen) {
					int nchar2=0;
					long shellDeg;
					if (sscanf(&(token[nchar]), "%ld%n", &shellDeg, &nchar2) == 1) {
						nchar += nchar2+1;
						MainData->InputOptions->SCF->AddGVBOpenShellDeg(shellDeg);
					} else
						break;
				}
			}
			
			if (-1 < FindKeyWord(Line, "$END", 4)) {	//End of this group
														//scan for multiple occurances of this group
				if (!Buffer->FindGroup("SCF")) EndOfGroup = true;
			}
		} while (!EndOfGroup);
	}
	Buffer->SetFilePos(0);	//restart search from beginning of file
	EndOfGroup = false;
	if (Buffer->FindGroup("MP2")) {
		if (!MainData->InputOptions->MP2) MainData->InputOptions->MP2 = new MP2Group;
		do {
			float tempf;
			Buffer->GetLine(Line);
			if (ReadLongKeyword(Line, "NACORE", &nAtoms))
				MainData->InputOptions->MP2->SetNumCoreElectrons(nAtoms);
			if (ReadBooleanKeyword(Line, "MP2PRP", &BoolTest))
				MainData->InputOptions->MP2->SetMP2Prop(BoolTest);
			if (ReadBooleanKeyword(Line, "LMOMP2", &BoolTest))
				MainData->InputOptions->MP2->SetLMOMP2(BoolTest);
			if (ReadLongKeyword(Line, "NWORD", &nAtoms))
				MainData->InputOptions->MP2->SetMemory(nAtoms);
			if (ReadLongKeyword(Line, "METHOD", &nAtoms))
				MainData->InputOptions->MP2->SetMethod(nAtoms);
			if (ReadStringKeyword(Line, "AOINTS", token))
				MainData->InputOptions->MP2->SetAOIntMethod(token);
			if (ReadFloatKeyword(Line, "CUTOFF", &tempf))
				MainData->InputOptions->MP2->SetIntCutoff(tempf);
			
			if (-1 < FindKeyWord(Line, "$END", 4)) {	//End of this group
														//scan for multiple occurances of this group
				if (!Buffer->FindGroup("MP2")) EndOfGroup = true;
			}
		} while (!EndOfGroup);
	}
	Buffer->SetFilePos(0);	//restart search from beginning of file
	EndOfGroup = false;
	if (Buffer->FindGroup("FORCE")) {
		if (!MainData->InputOptions->Hessian) MainData->InputOptions->Hessian = new HessianGroup;
		do {
			float tempf;
			Buffer->GetLine(Line);
			if (ReadStringKeyword(Line, "METHOD", token))
				MainData->InputOptions->Hessian->SetAnalyticMethod(
						strncasecmp(token, "ANALYTIC", 8) == 0);
			if (ReadLongKeyword(Line, "NVIB", &nAtoms))
				MainData->InputOptions->Hessian->SetDoubleDiff(nAtoms==2);
			if (ReadFloatKeyword(Line, "VIBSIZ", &tempf))
				MainData->InputOptions->Hessian->SetDisplacementSize(tempf);
			if (ReadBooleanKeyword(Line, "PURIFY", &BoolTest))
				MainData->InputOptions->Hessian->SetPurify(BoolTest);
			if (ReadBooleanKeyword(Line, "PRTIFC", &BoolTest))
				MainData->InputOptions->Hessian->SetPrintFC(BoolTest);
			if (ReadBooleanKeyword(Line, "VIBANL", &BoolTest))
				MainData->InputOptions->Hessian->SetVibAnalysis(BoolTest);
			if (ReadFloatKeyword(Line, "SCLFAC", &tempf))
				MainData->InputOptions->Hessian->SetFreqScale(tempf);
			
			if (-1 < FindKeyWord(Line, "$END", 4)) {	//End of this group
														//scan for multiple occurances of this group
				if (!Buffer->FindGroup("FORCE")) EndOfGroup = true;
			}
		} while (!EndOfGroup);
	}
	Buffer->SetFilePos(0);	//restart search from beginning of file
	EndOfGroup = false;
	if (Buffer->FindGroup("STATPT")) {
		if (!MainData->InputOptions->StatPt) MainData->InputOptions->StatPt = new StatPtGroup;
		do {
			float tempf;
			Buffer->GetLine(Line);
			if (ReadFloatKeyword(Line, "OPTTOL", &tempf))
				MainData->InputOptions->StatPt->SetOptConvergance(tempf);
			if (ReadLongKeyword(Line, "NSTEP", &nAtoms))
				MainData->InputOptions->StatPt->SetMaxSteps(nAtoms);
			if (ReadStringKeyword(Line, "METHOD", token))
				MainData->InputOptions->StatPt->SetMethod(token);
			if (ReadFloatKeyword(Line, "DXMAX", &tempf))
				MainData->InputOptions->StatPt->SetInitRadius(tempf);
			if (ReadFloatKeyword(Line, "TRMAX", &tempf))
				MainData->InputOptions->StatPt->SetMaxRadius(tempf);
			if (ReadFloatKeyword(Line, "TRMIN", &tempf))
				MainData->InputOptions->StatPt->SetMinRadius(tempf);
			if (ReadLongKeyword(Line, "IFOLOW", &nAtoms))
				MainData->InputOptions->StatPt->SetModeFollow(nAtoms);
			if (ReadBooleanKeyword(Line, "STPT", &BoolTest))
				MainData->InputOptions->StatPt->SetStatPoint(BoolTest);
			if (ReadFloatKeyword(Line, "STSTEP", &tempf))
				MainData->InputOptions->StatPt->SetStatJump(tempf);
			if (ReadLongKeyword(Line, "IHREP", &nAtoms))
				MainData->InputOptions->StatPt->SetHessRecalcInterval(nAtoms);
			if (ReadLongKeyword(Line, "NPRT", &nAtoms))
				MainData->InputOptions->StatPt->SetAlwaysPrintOrbs(nAtoms==1);
			if (ReadStringKeyword(Line, "HESS", token))
				MainData->InputOptions->StatPt->SetHessMethod(token);
			
			if (-1 < FindKeyWord(Line, "$END", 4)) {	//End of this group
														//scan for multiple occurances of this group
				if (!Buffer->FindGroup("STATPT")) EndOfGroup = true;
			}
		} while (!EndOfGroup);
	}
	Buffer->SetFilePos(0);	//restart search from beginning of file
	EndOfGroup = false;
	if (Buffer->FindGroup("DFT")) {
		do {
			Buffer->GetLine(Line);
			if (ReadStringKeyword(Line, "METHOD", token))
				MainData->InputOptions->DFT.SetMethodGrid(
					   strncasecmp(token, "GRIDFREE", 8) != 0);
			
			if (-1 < FindKeyWord(Line, "$END", 4)) {	//End of this group
														//scan for multiple occurances of this group
				if (!Buffer->FindGroup("DFT")) EndOfGroup = true;
			}
		} while (!EndOfGroup);
	}
	//Now can save the functional that is read from the contrl group
	if (strlen(DFTTYP) > 0) {
		MainData->InputOptions->DFT.SetFunctional(DFTTYP);
		if (MainData->InputOptions->DFT.GetFunctional() > 0)
			MainData->InputOptions->Control->UseDFT(true);
		else
			wxLogMessage(_("Unknown DFTTYP detected, ignored."));
	}
	
	Buffer->SetFilePos(0);	//restart search from beginning of file
	EndOfGroup = false;
	if (Buffer->FindGroup("FMO")) {
		MainData->InputOptions->FMO.FMOActive(true);
		do {
			Buffer->GetLine(Line);
			if (ReadLongKeyword(Line, "NFRAG", &nAtoms))
				MainData->InputOptions->FMO.SetNumberFragments(nAtoms);
			//The rest of the items in this group depend on the # of atoms so parse them after the coordinates
			
			if (-1 < FindKeyWord(Line, "$END", 4)) {	//End of this group
				//scan for multiple occurances of this group
				if (!Buffer->FindGroup("FMO")) EndOfGroup = true;
			}
		} while (!EndOfGroup);
	}
	float unitConversion = 1.0f;
	if (MainData->InputOptions->Data->GetUnits()) unitConversion = kBohr2AngConversion;
	Buffer->SetFilePos(0);	//restart search from beginning of file
	Frame * lFrame = MainData->GetCurrentFramePtr();
	if (Buffer->FindGroup("DATA")) {
		Buffer->SkipnLines(1);
		Buffer->GetLine(Line);	//1st line is a title line
		MainData->InputOptions->Data->SetTitle(Line, strlen(Line));
		Buffer->GetLine(Line);	//2nd line contains the point group
		if (FindKeyWord(Line, "$END", 4) < 0) {	// Make sure we're not at $END
			scanerr = sscanf(Line, "%s %ld", token, &nAtoms);
			MainData->InputOptions->Data->SetPointGroup(token);
			if (scanerr == 2)	//if a number was read in set the order
				MainData->InputOptions->Data->SetPointGroupOrder(nAtoms);
				//If !C1 then skip the blank line well I guess rarely it isn't blank...
			if (-1 == FindKeyWord(token, "C1", 2)) Buffer->SkipnLines(1);
			StartPos = Buffer->GetFilePos();
			if (!Buffer->LocateKeyWord("$END", 4)) throw DataError();
			EndPos = Buffer->GetFilePos() - 1;
			Buffer->SetFilePos(StartPos);
			nAtoms = Buffer->GetNumLines(EndPos - StartPos);
			if (MainData->InputOptions->FMO.IsFMOActive()) nAtoms=0;	//FMO coordinates are in $FMOXYZ
			if (nAtoms > 0) {
				if (MainData->InputOptions->Data->GetCoordType() <= CartesianCoordType) {
					if (!MainData->SetupFrameMemory(nAtoms, 0)) throw MemoryError();
					while (Buffer->GetFilePos() < EndPos) {
							CPoint3D	pos;
							float		AtomType=-1;
						Buffer->GetLine(Line);
						if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
						{ throw UserCancel();}
						pos.x = pos.y = pos.z = 0.0f;
						int linecount = sscanf(Line, "%s %f %f %f %f", token, &AtomType, &pos.x, &pos.y, &pos.z);
						pos *= unitConversion;
						if ((linecount!=5)||(AtomType<1)) {
							wxString msg;
							msg.Printf(_("Warning: Parsing issue with the line: %s"), Line);
							wxLogMessage(msg);
						}
						lFrame->AddAtom((long) AtomType, pos);
						StartPos = Buffer->FindBlankLine();
						if ((StartPos <= EndPos)&&(StartPos>-1)) {	//basis set is inlined in $DATA
							Buffer->SetFilePos(StartPos);	//just skip over it
							Buffer->SkipnLines(1);
						}
					}
					if (((MainData->InputOptions->Data->GetCoordType() == UniqueCoordType)||
						 (MainData->InputOptions->Data->GetCoordType() == 0))&&
						(MainData->InputOptions->Data->GetPointGroup()>GAMESS_C1)&&
						(lFrame->GetNumAtoms() > 0)) {
						//Generate symmetry dependant atoms
						for (int i=0; i<lFrame->GetNumAtoms(); ++i)
							lFrame->Atoms[i].IsSymmetryUnique(true);
						MainData->GenerateSymmetryDependentAtoms();
					}
				} else if (MainData->InputOptions->Data->GetCoordType() <= ZMTCoordType) {
					wxFileOffset bPos = Buffer->FindBlankLine();
					if ((bPos > 0)&&(bPos < EndPos)) {
						nAtoms = Buffer->GetNumLines(bPos - StartPos);
					}
					if (nAtoms > 0) {
						if (!MainData->SetupFrameMemory(nAtoms, 0)) throw MemoryError();
					} else {
						wxLogMessage(_("No atoms found in your $DATA group!"));
						throw DataError();
					}
					MainData->ParseZMatrix(Buffer, nAtoms, Prefs);
				} else if (MainData->InputOptions->Data->GetCoordType() <= ZMTMPCCoordType) {
					if (nAtoms > 0) {
						if (!MainData->SetupFrameMemory(nAtoms, 0)) throw MemoryError();
					} else {
						wxLogMessage(_("No atoms found in your $DATA group!"));
						throw DataError();
					}
					MainData->ParseMOPACZMatrix(Buffer, nAtoms, Prefs);
				}
				if (Prefs->GetAutoBond())	//setup bonds, if needed
					lFrame->SetBonds(Prefs, false, ProgressInd);
			}
		}
	}
	Buffer->SetFilePos(0);	//restart search from beginning of file
	if (MainData->InputOptions->FMO.IsFMOActive() && Buffer->FindGroup("FMOXYZ")) {
		Buffer->SkipnLines(1);
		StartPos = Buffer->GetFilePos();
		if (!Buffer->LocateKeyWord("$END", 4)) throw DataError();
		EndPos = Buffer->GetFilePos() - 1;
		Buffer->SetFilePos(StartPos);
		nAtoms = Buffer->GetNumLines(EndPos - StartPos);
		if (nAtoms > 0) {
			ProgressInd->ChangeText("Reading FMO Coordinates...");
			if (!MainData->SetupFrameMemory(nAtoms, 0)) throw MemoryError();
			while (Buffer->GetFilePos() < EndPos) {
				CPoint3D	pos;
				float		AtomType=-1;
				char		token2[kMaxLineLength];
				Buffer->GetLine(Line);
				if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
				{ throw UserCancel();}
				pos.x = pos.y = pos.z = 0.0f;
				//The form is token (skip) atomic charge or symbol, x y z coords
				int linecount = sscanf(Line, "%s %s %f %f %f", token, token2, &pos.x, &pos.y, &pos.z);
				int inum = 1;
				if (isdigit(token2[0])) inum = sscanf(token2, "%f", &AtomType);
				else AtomType = SetAtomType((unsigned char *) token2);
				if ((linecount!=5)||(AtomType<1)) {
					wxString msg;
					msg.Printf(_("Warning: Parsing issue with the line: %s"), Line);
					wxLogMessage(msg);
				}
				pos *= unitConversion;
				lFrame->AddAtom((long) AtomType, pos);
			}
			if (Prefs->GetAutoBond()) {	//setup bonds, if needed
				lFrame->SetBonds(Prefs, false, ProgressInd);
			}
		}
	}
	Buffer->SetFilePos(0);	//restart search from beginning of file
	if (Buffer->FindGroup("EFRAG")) {
		Buffer->SkipnLines(1);
		Buffer->GetLine(Line);	//1st line has all of the options
		if (ReadStringKeyword(Line, "METHOD", token)) {
			MainData->InputOptions->EFP.SetCoordinatesType(token);
		}
		if (ReadStringKeyword(Line, "POLMETHD", token)) {
			MainData->InputOptions->EFP.SetPolMethod(token);
		}
		if (ReadStringKeyword(Line, "POSITION", token)) {
			MainData->InputOptions->EFP.SetPositionType(token);
		}
		long tag;
		if (ReadLongKeyword(Line, "COORD", &tag))
			MainData->InputOptions->EFP.SetMaxMOs(tag);
		if (ReadLongKeyword(Line, "MXBF", &tag))
			MainData->InputOptions->EFP.SetMaxBasisFunctions(tag);
		if (ReadLongKeyword(Line, "NBUFFMO", &tag))
			MainData->InputOptions->EFP.SetNumBufferMOs(tag);

		wxFileOffset former_pos, start_pos, found_it;
		//Now tackle the individual fragment definitions
		//These should consist of 4 line blocks of Fragname=xxx followed by three atom lines
		//For now I am only going to deal with cartesian coordinates
		if (MainData->InputOptions->EFP.UseCartesianCoordinates()) {
			mpAtom *atm;
			CPoint3D dst_locs[3];
			CPoint3D pos;
			int fstart;
			int match[3] = {0, 0, 0};
			CPoint3D src_locs[3];
			std::string labels[3];
			long fragNum;
			int i;
			CPoint3D new_pos;
			CPoint3D rot_pos;
			CPoint3D curr_pos;
			CPoint3D orig;
			Matrix4D vec2vec;
			CPoint3D src_vec;
			CPoint3D dst_vec;
			CPoint3D dst_vec2;
			CPoint3D dst_norm;
			CPoint3D mid_norm;
			CPoint3D mid_vec1, mid_vec2;
			float dot;

			Buffer->GetLine(Line);
			while (ReadStringKeyword(Line, "FRAGNAME", token)) {
				if (!strcasecmp(token, "H2ORHF") || !strcasecmp(token, "H2ODFT")) {	//builtin EFP1 style is limited to H2O with known labels
					MainData->FragmentNames.push_back(std::string(token));
					long fragNum = MainData->FragmentNames.size();
					CPoint3D	pos;
					int		AtomType;
					for (int i=0; i<3; ++i) {
						AtomType = -1;
						Buffer->GetLine(Line);
						//lines have format "label x, y, z"
						sscanf(Line, "%s %f %f %f", token, &pos.x, &pos.y, &pos.z);
						pos *= unitConversion;
						if (!strcasecmp(token, "O1")) AtomType = 8;
						else if (!strcasecmp(token, "H2")||!strcasecmp(token, "H3")) AtomType = 1;
						if (AtomType > 0) {
							mpAtom * atm = lFrame->AddAtom(AtomType, pos);
							atm->SetFragmentNumber(fragNum);
						}
					}
				}

				// Custom fragment type.
				else {

					// Read the three atoms that setup the fragment.  These
					// should be in angstroms.
					MainData->FragmentNames.push_back(std::string(token));
					fragNum = MainData->FragmentNames.size();
					char label[kMaxLineLength];
					for (i = 0; i < 3; ++i) {
						Buffer->GetLine(Line);
						sscanf(Line, "%s %f %f %f", label,
							   &(dst_locs[i].x), &(dst_locs[i].y),
							   &(dst_locs[i].z));
						dst_locs[i] *= unitConversion;
						labels[i] = label;
						/* std::cout << "Line: " << Line << std::endl; */
						/* atm = lFrame->AddAtom(1, dst_locs[i]); */
						/* atm->SetFragmentNumber(fragNum); */
					}

					// See if the fragment template has been loaded in already.
					// If so, go ahead and parse it.
					std::map<std::string, EFrag>::iterator frag;
					frag = MainData->efrags.find(token);
					if (frag == MainData->efrags.end()) {
						// save current position
						former_pos = Buffer->GetFilePos();

						// look for fragment definition
						found_it = Buffer->FindGroup(token);
						if (!found_it) {
							std::cout << "didn't find group " << token << std::endl;
							wxString msg;
							msg.Printf(_("Unable to locate correct EFP2 fragment definition group named %s"), token);
							wxLogMessage(msg);
							throw DataError();
						}
						start_pos = Buffer->GetFilePos();

						// get definition text
						std::string running_text;
						Buffer->GetLine(Line);
						while (FindKeyWord(Line, "$END", 4) < 0) {
							running_text += Line;
							running_text += "\n";
							Buffer->GetLine(Line);
						}
						running_text += Line;

						// restore file position
						Buffer->SetFilePos(former_pos);

						// get text
						MainData->efrags.insert(std::pair<std::string, EFrag>(token, EFrag(running_text)));
						frag = MainData->efrags.find(token);
					}

					fstart = lFrame->NumAtoms;
					const std::vector<EFragAtom>& labeled_atoms = frag->second.GetAtoms();
					std::vector<EFragAtom>::const_iterator efrag_atom;

					for (efrag_atom = labeled_atoms.begin();
						 efrag_atom != labeled_atoms.end();
						 ++efrag_atom) {

						if (efrag_atom->GetLabel().compare(labels[0]) == 0) {
							match[0] = lFrame->NumAtoms;
							src_locs[0] = efrag_atom->GetCoords();
						} else if (efrag_atom->GetLabel().compare(labels[1]) == 0) {
							match[1] = lFrame->NumAtoms;
							src_locs[1] = efrag_atom->GetCoords();
						} else if (efrag_atom->GetLabel().compare(labels[2]) == 0) {
							match[2] = lFrame->NumAtoms;
							src_locs[2] = efrag_atom->GetCoords();
						}

						atm = lFrame->AddAtom(efrag_atom->GetAtomicNumber(),
											  efrag_atom->GetCoords());
						atm->SetFragmentNumber(fragNum);
					}

					// We first find a rotation that one will align a vector
					// in the fragment template to the corresponding vector in
					// the destination space.
					dst_vec = dst_locs[1] - dst_locs[0];
					Normalize3D(&dst_vec);

					src_vec = src_locs[1] - src_locs[0];
					Normalize3D(&src_vec);

					SetRotationMatrix(vec2vec, &src_vec, &dst_vec);

					// The common vector now serves as an axis of rotation.  We
					// need to rotate the fragment template plane so that it
					// coincides with the destination plane.  The angle of rotation
					// can be determined by the angle between the two planes'
					// normals.
					lFrame->GetAtomPosition(match[0], orig);
					Rotate3DOffset(vec2vec, src_locs[1] - orig, &mid_vec1);
					Rotate3DOffset(vec2vec, src_locs[2] - orig, &mid_vec2);
					UnitCrossProduct3D(&mid_vec1, &mid_vec2, &mid_norm);

					dst_vec2 = dst_locs[2] - dst_locs[0];
					UnitCrossProduct3D(&dst_vec, &dst_vec2, &dst_norm);

					Matrix4D tri2tri;
					dot = DotProduct3D(&mid_norm, &dst_norm);

					// Technically, the axis of rotation (the aligned vector)
					// might be facing a different direction than we think it
					// is. To be consistent, we instead use the axis that is
					// normal to both planes' normals. It points in the same
					// or opposite direction as dst_vec.
					CPoint3D axis;
					UnitCrossProduct3D(&mid_norm, &dst_norm, &axis);
					RotateAroundAxis(tri2tri, axis, acos(dot) * 180.0 / kPi);

					// We concatenate the two rotation matrices.
					Matrix4D transform;
					MultiplyMatrix(vec2vec, tri2tri, transform);

					// Okay, for each atom we added for this fragment instance,
					// we move all fragment atoms into destination space. We
					// translate to make the base fragment atom the origin and
					// then rotate to align the fragment with the destination
					// plane, and then translate by the base destination atom.
					for (long i = fstart; i < lFrame->NumAtoms; ++i) {
						lFrame->GetAtomPosition(i, curr_pos);
						new_pos = curr_pos - orig;
						Rotate3DOffset(transform, new_pos, &rot_pos);
						new_pos = rot_pos + dst_locs[0];
						lFrame->SetAtomPosition(i, new_pos);
					}
				}

				Buffer->GetLine(Line);
					//At some point GAMESS started printing out all fragment atoms instead of the first three,
					//since the code above has already properly obtained the full fixed fragment geometry just
					//skip the user provided set which could be incorrect.
				while (((0>FindKeyWord(Line, "FRAGNAME", 8))&&(0>FindKeyWord(Line, "$END", 4)))) {
					Buffer->GetLine(Line);
				}
			}
		}

		lFrame->SetBonds(Prefs, true, NULL, false);

	}
	if (MainData->InputOptions->FMO.IsFMOActive()) {
		Buffer->SetFilePos(0);	//restart search from beginning of file
		if (Buffer->FindGroup("FMO")) {
			wxFileOffset startFMO = Buffer->GetFilePos();
			wxFileOffset EndOfGroup;
			if (Buffer->LocateKeyWord("$END", 4)) {
				EndOfGroup = Buffer->GetFilePos();
			} else {
				EndOfGroup = -1;
				wxLogMessage(_("The FMO group does not have proper termination."));
			}
			Buffer->SetFilePos(startFMO);
			//Parse the items that depend on the # of atoms
			//The rest of the items in this group depend on the # of atoms so parse them after the coordinates
			
			if (Buffer->LocateKeyWord("INDAT", 5, EndOfGroup)) {
				MainData->ParseFMOIds(Buffer, lFrame->GetNumAtoms(), EndOfGroup);
			}
		}
	}
	return 1;
}
/**
 * Parses the MDL Mol Format (just atoms). 
 * @param Buffer A BufferFileObject which the MDL mol file is buffered into
 * @return Returns 1 upon open success or 0 on error or failure to open
 */
long MolDisplayWin::OpenMDLMolFile(BufferFile * Buffer) {
	char Line[kMaxLineLength], partA[16], partB[16], partC[16];
	long	nAtoms, nBonds;
	short	scanerr;
// parts of this are a bit gross since the MDL format is fixed format without
// whitespace in between the fields.

	ProgressInd->ChangeText("Reading MDL MolFile...");
	Frame * lFrame = MainData->cFrame;

	Buffer->GetLine(Line);	//The first Line is a comment/compound name line
	MainData->SetDescription(Line);
	Buffer->GetLine(Line);	//The second line can (optionally) contain program and energy data
	long LineLength = strlen(Line);
	if (LineLength > 34) {
		double	energy;
		if (1==sscanf(&(Line[34]), "%lf", &energy))
			lFrame->Energy = energy;
	}
	Buffer->SkipnLines(1);	//third line for comments (unused here)
	Buffer->GetLine(Line);	//atom/bond counts
	partA[0] = Line[0]; partA[1] = Line[1]; partA[2]=Line[2]; partA[3]='\0';
	partB[0] = Line[3]; partB[1] = Line[4]; partB[2]=Line[5]; partB[3]='\0';
	scanerr = sscanf(partA, "%3ld", &nAtoms);
	scanerr += sscanf(partB, "%3ld", &nBonds);
	if (scanerr!=2 || nAtoms <= 0) {
		wxLogMessage(_("Error parsing MDL MolFile."));
		throw DataError();
	}
	MainData->SetupFrameMemory(nAtoms, nBonds);
		long i;
	for (i=0; i<nAtoms; ++i) {
			CPoint3D tPt;
			char	token[5];
				//Atom line format:  "%10f%10f%10f %3s"
		Buffer->GetLine(Line);
		for (int i=0; i<10; ++i) partA[i] = Line[i];
		partA[10] = '\0';
		scanerr = sscanf(partA, "%10f", &tPt.x);
		for (int i=0; i<10; ++i) partB[i] = Line[i+10];
		partB[10] = '\0';
		scanerr += sscanf(partB, "%10f", &tPt.y);
		for (int i=0; i<10; ++i) partC[i] = Line[i+20];
		partC[10] = '\0';
		scanerr += sscanf(partC, "%10f", &tPt.z);
		scanerr += sscanf(&(Line[31]), "%3s", token);
		if (scanerr == 4) {
				long AtomType;
			AtomType = SetAtomType((unsigned char *) token);
			lFrame->AddAtom(AtomType, tPt);
		} else break;
	}
	if (i==nAtoms && nBonds > 0) {	//Read in bonds if all atom were read successfully
		for (i=0; i<nBonds; ++i) {
			long type, a1, a2;
			Buffer->GetLine(Line);	//Bonds format: "%3ld%3ld%3ld"
			partA[0] = Line[0]; partA[1] = Line[1]; partA[2]=Line[2]; partA[3]='\0';
			partB[0] = Line[3]; partB[1] = Line[4]; partB[2]=Line[5]; partB[3]='\0';
			partC[0] = Line[6]; partC[1] = Line[7]; partC[2]=Line[8]; partC[3]='\0';
			scanerr = sscanf(partA, "%3ld", &a1);
			scanerr += sscanf(partB, "%3ld", &a2);
			scanerr += sscanf(partC, "%3ld", &type);
			if (scanerr == 3 && a1>0 && a2>0 && a1<=nAtoms && a2<=nAtoms && a1!=a2) {
				lFrame->AddBond(a1-1, a2-1);
				if (type>3) type = 1;
				if (type<1) type = 1;
				lFrame->SetBondOrder(lFrame->GetNumBonds()-1, (BondOrder) type);
			} else break;
		}
	}
	
	return 1;
}
/**
 * Parses the Protein Databank Format (just atoms). 
 * @param Buffer A BufferFileObject which the PDB file is buffered into
 * @return Returns 1 upon open success or 0 on error or failure to open
 */
long MolDisplayWin::OpenPDBFile(BufferFile * Buffer) {
	char Line[kMaxLineLength];
	long	nAtoms, nModels;
	short	scanerr;

	ProgressInd->ChangeText("Reading PDB file...");
	Frame * lFrame = MainData->cFrame;
		//First scan the file to determine the atoms
	nAtoms = 0;
	while (Buffer->GetFilePos() < Buffer->GetFileLength()) {
		Buffer->GetLine(Line);
		if (0==FindKeyWord(Line, "MODEL", 5)) ++nModels;
		if (0==FindKeyWord(Line, "ATOM", 4)) ++nAtoms;
		if (0==FindKeyWord(Line, "HETATM", 4)) ++nAtoms;
	}
	Buffer->SetFilePos(0);
	if (nAtoms>0) {
			//allocate memory for the atoms
		if (!MainData->SetupFrameMemory(nAtoms, 0)) throw MemoryError();

			CPoint3D Pos;
			long AtomType, LinePos;
		nAtoms = 0;
			//Scan for Atom or hetAtm lines
			//Format is: a6,i5,2x,a4,a3,2x,i4,4x,3F8.3,2f6.2
			//1 -  6        Record name   "ATOM  "
			//7 - 11        Integer       serial       Atom  serial number.
			//13 - 16        Atom          name         Atom name.
			//17             Character     altLoc       Alternate location indicator.
			//18 - 20        Residue name  resName      Residue name.
			//22             Character     chainID      Chain identifier.
			//23 - 26        Integer       resSeq       Residue sequence number.
			//27             AChar         iCode        Code for insertion of residues.
			//31 - 38        Real(8.3)     x            Orthogonal coordinates for X in Angstroms.
			//39 - 46        Real(8.3)     y            Orthogonal coordinates for Y in Angstroms.
			//47 - 54        Real(8.3)     z            Orthogonal coordinates for Z in Angstroms.
			//55 - 60        Real(6.2)     occupancy    Occupancy.
			//61 - 66        Real(6.2)     tempFactor   Temperature  factor.
			//77 - 78        LString(2)    element      Element symbol, right-justified.
			//79 - 80        LString(2)    charge       Charge  on the atom.
			//This routine only parses simple pdb files. It probably gets many files atom types wrong
			//and it always reads in all atoms including duplicates
		while (Buffer->GetFilePos() < Buffer->GetFileLength()) {
			Buffer->GetLine(Line);
			int LineLength = strlen(Line);
				//Only read in the first model in a file
			if (0==FindKeyWord(Line, "ENDMDL", 6)) {
				bool modelFound=false;
				while (Buffer->GetFilePos() < Buffer->GetFileLength()) {
					Buffer->GetLine(Line);
					if (0==FindKeyWord(Line, "MODEL", 5)) {
						modelFound = true;
						break;
					}
				}
				if (!modelFound) break;
				if (Prefs->GetAutoBond())	//setup bonds, if needed
					lFrame->SetBonds(Prefs, false, ProgressInd);
				lFrame = MainData->AddFrame(lFrame->GetNumAtoms(), lFrame->GetNumBonds());
			}
			long atomTest = FindKeyWord(Line, "ATOM", 4);
			long HetTest = FindKeyWord(Line, "HETATM", 4);
			if ((0==atomTest)||(0==HetTest)) {
				AtomType = -1;
				if (LineLength >= 78) {	//Use the element symbol if present
					unsigned char label[4];
					bool test=false;
					if (isalpha(Line[76])) {
						label[0] = Line[76];
						label[1] = Line[77];
						label[2] = '\0';
						test = true;
					} else if (isalpha(Line[77])) {
						label[0] = Line[77];
						label[1] = '\0';
						test = true;
					}
					if (test)
						AtomType = SetAtomType(label);
				}
				if (AtomType <= 0)	{//If no symbol attempt to use the "name"
					LinePos = 13;
					if (atomTest==0) Line[14] = ' ';
					else if (Line[12] != ' ') LinePos = 12;
					AtomType = SetAtomType((unsigned char *) &(Line[LinePos]));
				}
				if (AtomType > 0) {
					scanerr = sscanf(&(Line[30]), "%8e%8e%8e", &(Pos.x), &(Pos.y), &(Pos.z));
					if (scanerr == 3) {
						lFrame->AddAtom(AtomType, Pos);
						MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.x));
						MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.y));
						MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.z));
						++nAtoms;
					}
				}
			}
		}
		if (Prefs->GetAutoBond())	//setup bonds, if needed
			lFrame->SetBonds(Prefs, false, ProgressInd);
	}
	return 1;
}

/**
  * Adds support for opening the MKL file extension. 
  * @param Buffer A BufferFileObject which the .MKL file is buffered into
  * to make parsing the file easier.  See the BufferFile object for valid
  * BufferFile operations.
  * @return Returns 1 upon open success or 0 on error or failure to open
*/
long MolDisplayWin::OpenMKLFile(BufferFile * Buffer){
	// buffer line for text to be scanned while parsing
	char Line[kMaxLineLength];
	// number of atomes in the structure described by this file
	long nAtoms = 0;
	// Placeholders for traversing file sections in Buffer
	wxFileOffset startOfSection = 0;
	wxFileOffset endOfSection = -1;
	// counters for line-by-line text input:
	// # of tokens sucessfully read in a sscanf call, # bytes total in line, 
	// current position in line, and the # of bytes read through one sscanf call
	int scanCount, lineBytes, bytesRead, bytesConsumed;
	// flags to indicate $KEYWORD section completion and order-of-dependency
	// following.  Also used to enforce data parsing in-order (doesn't guarantee
	// that the file's data was in-order, only that it is parsed in-order)
	bool BasisDone = false, CoefAlphaDone = false, CoefBetaDone = false;
	bool OccAlphaDone = false, OccBetaDone = false;
	
	ProgressInd->ChangeText("Reading MKL file...");
	Frame * lFrame = MainData->cFrame;
	// first scan the file to determine the number of atoms
	//look throughout the file for the $COORD or the last $$
	//and count the number of lines between the two keywords 
	//(each line is another atom in the molecule)
	while (Buffer->GetFilePos() < Buffer->GetFileLength() 
			&& Buffer->LocateKeyWord("$COORD", 6)) {
		Buffer->SkipnLines(1); // skip $COORD line
		startOfSection = Buffer->GetFilePos();
		if (Buffer->LocateKeyWord("$END", 4)) {
			endOfSection = Buffer->GetFilePos();
			Buffer->SetFilePos(startOfSection);
		} else endOfSection = Buffer->GetFileSize();
		while (Buffer->GetFilePos() < endOfSection) {
			wxFileOffset startOfFramePos = Buffer->GetFilePos();
			wxFileOffset endOfFramePos = endOfSection;
			//individual geometries are separated by the "$$" line
			if (Buffer->LocateKeyWord("$$", 2, endOfSection)) {
				endOfFramePos = Buffer->GetFilePos();
				Buffer->SetFilePos(startOfFramePos);
			}
			nAtoms = (long) Buffer->GetNumLines(endOfFramePos);
			if (lFrame->GetNumAtoms()>0)
				lFrame = MainData->AddFrame(nAtoms, 0);
			for (long i=0; i<nAtoms; ++i) {
				CPoint3D Pos;
				long AtomType;
				if (Buffer->GetFilePos() >= endOfFramePos) {
					nAtoms = i;
					break;
				}
				Buffer->GetLine(Line);
				//parse line for atom type and x,y,z positions
				scanCount = sscanf(Line,"%ld %f %f %f",&AtomType,&Pos.x,&Pos.y,&Pos.z);
				if (scanCount == 0) {
					nAtoms = i;
					break;
				}
				if (scanCount != 4) {
					MessageAlert("Error while reading coordinates.");
					return 0;
				}
				//add atom to the molecule
				lFrame->AddAtom(AtomType, Pos);
			}
			if (Buffer->GetFilePos() < endOfSection) Buffer->SkipnLines(1);
			//setup bonds, if needed
			if (Prefs->GetAutoBond() && (nAtoms > 0))
				lFrame->SetBonds(Prefs, false, ProgressInd);
		}
	} // Coords done

	// now look for Basis data
	if ((0<lFrame->GetNumAtoms())&&(Buffer->LocateKeyWord("$BASIS", 6))) {
		bool error = false;
		// totals counters
		unsigned long nShells = 0, linesInBasis = 0;
		// other counters; may be set to -1 as a reset to offset an increment
		// thus placing back at 0 for a subsequent iteration
		long iShell = 0, iAtom = 0, shellsPeriAtom = 0;
		// temp vars for line-at-a-time read 
		long nFunc;	
		char IType[5];
		float Sc;
		Buffer->SkipnLines(1);
		startOfSection = Buffer->GetFilePos();
		
		// count total number of shells in Basis section by counting
		// the number of lines formatted according to the sscanf below
		while ((Buffer->GetFilePos()<Buffer->GetFileSize())) {
			Buffer->GetLine(Line);
			++linesInBasis;
			// continue until we find the end of the group / start of next
			if (FindKeyWord(Line, "$END", 4)<0 && FindKeyWord(Line, "$COEFF", 6)<0 
				&& FindKeyWord(Line, "$OCC", 4)<0) {
				lineBytes = strlen(Line);
				bytesRead = 0;
				while (bytesRead < lineBytes) {
					scanCount = sscanf(&(Line[bytesRead]), 	"%ld %[lLsSpPdDfF] %f%n", 
								&nFunc, &IType, &Sc, &bytesConsumed);
					if (scanCount == 3) { 
						bytesRead+=bytesConsumed;
						++nShells;
					} else
						break;
				}
			}
			else // end of group has been determined
				break;
		}

		// Now that shells are counted, we can create the BasisSet and parse data
		MainData->Basis = new BasisSet(nAtoms, nShells);
		if (MainData->Basis == NULL) error = true;
		
		// go back to the line right after $BASIS so we can parse the data
		Buffer->SetFilePos(startOfSection);
		// reiterate through Basis section to get and save BasisSet data
		while (iShell < nShells && iAtom < nAtoms && !error) {
			Buffer->GetLine(Line);
			
			// we only use IType read; nFunc is redundant; Sc(ale Factor) we don't use
			scanCount = sscanf(Line, "%ld %[lLsSpPdDfF] %f%n", 
								&nFunc, &IType, &Sc, &bytesConsumed);
			if (3==scanCount) {
				// create this BasisShell in the BasisSet
				MainData->Basis->Shells.push_back(BasisShell());	
				// increment the shell count in the BasisSet (how many shells parsed)
				MainData->Basis->NumShells++; 
				// cast "SP" to L for simplicity
				if ('S'==toupper(IType[0]) && 'P'==toupper(IType[1])) {
					IType[0] = 'L';
					IType[1] = '\0';
				}
				// save the type of this BasisShell
				switch (toupper(IType[0])) {
					case 'L':
						MainData->Basis->Shells[iShell].ShellType = LShell;
						break;
					case 'S':
						MainData->Basis->Shells[iShell].ShellType = SShell;
						break;
					case 'P':
						MainData->Basis->Shells[iShell].ShellType = PShell;
						break;
					case 'D':
						MainData->Basis->Shells[iShell].ShellType = DShell;
						break;
					case 'F':
						MainData->Basis->Shells[iShell].ShellType = FShell;
						break;
				}
				// add first shell of this atom to BasisMap and save NuclearCharge
				if (0==shellsPeriAtom) {
					MainData->Basis->BasisMap[2*iAtom]=iShell;
					MainData->Basis->NuclearCharge[iAtom] = (long)(lFrame->GetAtomType(iAtom));
				}
				MainData->Basis->NumFuncs += MainData->Basis->Shells[iShell].GetNumFuncs(false);
				// read in functions for this shell (kinda dumb: grab rows of floats if
				// the line isn't formatted like the lines we grab ITypes from)
				char tmpStr[5];
				float tmpFloat[3];
				bool stopReadingShell = false;
				while ( !stopReadingShell ) {
					Buffer->GetLine(Line);
					if (3==sscanf(Line, "%ld %[lLsSpPdDfF] %f", &nFunc, &IType, &Sc)) {
						Buffer->BackupnLines(1);
						break;
					}
					scanCount = sscanf(Line, "%f %f %f", &tmpFloat[0], &tmpFloat[1], &tmpFloat[2]);
					// non-SP (L) case: one exponent, one coef per line
					if (2==scanCount && LShell!=MainData->Basis->Shells[iShell].ShellType) {
						MainData->Basis->Shells[iShell].NumPrims++;
						MainData->Basis->Shells[iShell].Exponent.push_back(tmpFloat[0]);
						MainData->Basis->Shells[iShell].NormCoef.push_back(tmpFloat[1]);
					}
					// SP (L) case: one exp, two coeff's per line
					else if (3==scanCount && LShell==MainData->Basis->Shells[iShell].ShellType) {
						MainData->Basis->Shells[iShell].NumPrims++;
						MainData->Basis->Shells[iShell].Exponent.push_back(tmpFloat[0]);
						MainData->Basis->Shells[iShell].NormCoef.push_back(tmpFloat[1]);
						MainData->Basis->Shells[iShell].NormCoef.push_back(tmpFloat[2]);
					} 
					// We've found the end of this Atom's Basis Set, end of the $BASIS section
					// or else we've hit an error
					else if (1==sscanf(Line, "%s%n", &tmpStr, &bytesConsumed)) {
						if (2==bytesConsumed && 0==strncmp(tmpStr, "$$", 3)) { 
							// add ending shell of this atom to BasisMap
							MainData->Basis->BasisMap[2*iAtom+1]=iShell;
							// we'll move on to next atom (or be done with Basis)
							++iAtom;
							shellsPeriAtom = -1;	// reset for next atom;
							stopReadingShell = true;
							break;
						}
						else if (4==bytesConsumed && 0==strncmp(tmpStr, "$END", 5)) {
							// same as above, but need to break out of basis parsing
							MainData->Basis->BasisMap[2*iAtom+1]=iShell;
							++iAtom;
							stopReadingShell = true;
							// BasisDone = true;
							// possibly needed when trying to make parser more intelligent when the 
							// file isn't formatted perfectly; eg, there is a blank line after $END
							break;
						}
						else {
							error = true;
						}
					} else { // likely hit a blank line
						MainData->Basis->BasisMap[2*iAtom+1]=iShell;
						++iAtom;
						stopReadingShell = true;
					}
				}
				// move on to next shell in next iteration
				++iShell;
				++shellsPeriAtom;
			}
			else if (FindKeyWord(Line, "$END", 4) >-1) {
				BasisDone = true;
				break;
			}
			else {
				error = true;
				break;
			}

		} // creating BasisSet
		if (error && !BasisDone) {
			// Free what we have and set it NULL so dependent functions won't crash
			if (MainData->Basis != NULL) delete MainData->Basis;
			MainData->Basis = NULL;
			MessageAlert("Error while reading. No basis set created.");
		} else {
			BasisDone = true;
			MainData->Basis->NuclearChargesAreValid(true);
		}
	}
	// now look for Alpha Coefficients (Orbitals; depends on BasisSet)
	if (BasisDone&&(MainData->Basis != NULL)&&(Buffer->LocateKeyWord("$COEFF_ALPHA", 12))) {
		Buffer->GetLine(Line); // skip $COEFF_ALPHA line
		bool error = false;
		char tmpStr[5];
		// counters for within iterations and overall in this section
		unsigned long iSymCount, symCount = 0, eigenvecCount = 0;

		// we assume that the number of Basis Functions is the number
		// coefficients.  Otherwise we should throw an error
		long nOrbs = MainData->Basis->GetNumBasisFuncs(false);
		// create the OrbitalRec and save it if successful
		OrbitalRec *OrbSet = new OrbitalRec(nOrbs, nOrbs, nOrbs);
		if (OrbSet != NULL) lFrame->Orbs.push_back(OrbSet);

		// read the section until we hit the end.  should be modified to halt on EOF
		while ((Buffer->GetFilePos()<Buffer->GetFileSize())&&!error) {
			Buffer->GetLine(Line);
			// continue until we find the end of the group
			if (FindKeyWord(Line, "$", 1)<0 && 0!=strncmp(Line, "\0", kMaxLineLength)) {
				lineBytes = strlen(Line);
				bytesRead = 0;
				iSymCount = 0;
				
				// get the Alpha symmetry symbols a line at a time (should be 5 per line, 
				// except possibly the last block may have fewer per line)
				while (bytesRead < lineBytes) {
					scanCount = sscanf(&Line[bytesRead], "%s%n", tmpStr, &bytesConsumed);
					if (1==scanCount) {
						tmpStr[4] = '\0'; // SymType is 5 chars wide and must be null-terminated
						strncpy(&OrbSet->SymType[5*symCount], tmpStr, 4);
						bytesRead+=bytesConsumed;
						++iSymCount;
						++symCount;
					} else
						break;
				} // got sym line
				
				// next come Alpha eigenvectors (Energy; first row of data after syms)
				Buffer->GetLine(Line);
				lineBytes = strlen(Line);
				bytesRead = 0;
				float tmpEigenvec;
				// get a line of the values at a time (should be 5 per line, except possibly 
				// the last block may have fewer per line)
				while (bytesRead < lineBytes) {
					scanCount = sscanf(&Line[bytesRead], "%f%n", &tmpEigenvec, &bytesConsumed);
					if (scanCount == 1) {
						lFrame->Orbs[0]->Energy[eigenvecCount] = tmpEigenvec;
						bytesRead += bytesConsumed;
						++eigenvecCount;
					} else
						break;
				} //got eigenvector line

				// now get the Alpha coefficients
				for (long iLine=0; iLine < nOrbs; ++iLine) {
					Buffer->GetLine(Line);
					// each line should contain up to 5 orbitals
					lineBytes = strlen(Line);
					bytesRead = 0;
					long iCoef = 0;
					float tmpCoef;
					// get coefficients line at a time, "symCount" times
					while (bytesRead < lineBytes && iCoef < symCount) {
						scanCount = sscanf(&Line[bytesRead], "%f%n", &tmpCoef, &bytesConsumed);
						if (scanCount == 1) {
							OrbSet->Vectors[(symCount-iSymCount)*nOrbs+iCoef*nOrbs+iLine] = tmpCoef;
							bytesRead += bytesConsumed;
							++iCoef;
						} else
							break;
					}
					if (iCoef < iSymCount) error = true;
					if (error) break;
				} // got coefs block
				if (error) break;
			}
			else
				break; // found $END of $COEFF_ALPHA
		}
		if (error) {
			// free what we have and set NULL so dependent functions won't crash
			if (OrbSet != NULL) delete OrbSet;
			lFrame->Orbs[0] = NULL;
			MessageAlert("Error while reading. No orbital set created.");
		}
		else
			CoefAlphaDone = true;
	} // Alpha Coefficients (Orbitals) done

	// now look for Beta Coefficients (Orbitals; optional; depends on Alpha Coefficients)
	if (CoefAlphaDone&&(Buffer->LocateKeyWord("$COEFF_BETA", 11))) {
		Buffer->GetLine(Line); // skip $COEFF_BETA line
		bool error = false;
		char tmpStr[5];
		// counters for within iterations and overall in this section
		unsigned long iSymCount, symCount = 0, eigenvecCount = 0;
		// we assume that the number of Basis Functions is the number
		// coefficients.  Otherwise wei should throw an error
		long nOrbs = MainData->Basis->GetNumBasisFuncs(false);
	
		while ((Buffer->GetFilePos()<Buffer->GetFileSize())&&!error) {
			Buffer->GetLine(Line);
			// continue until we find the end of the group
			if (FindKeyWord(Line, "$", 1)<0 && 0!=strncmp(Line, "\0", kMaxLineLength)) {
				lineBytes = strlen(Line);
				bytesRead = 0;
				iSymCount = 0;
				
				// get the Beta symmetry symbols a line at a time (should be 5 per line, 
				// except possibly the last block may have fewer per line)
				while (bytesRead < lineBytes) {
					scanCount = sscanf(&Line[bytesRead], "%s%n", tmpStr, &bytesConsumed);
					if (1==scanCount) {
						tmpStr[4] = '\0'; // SymType is 5 chars wide and must be null-terminated
						strncpy(&lFrame->Orbs[0]->SymTypeB[5*symCount], tmpStr, 4);
						bytesRead+=bytesConsumed;
						++iSymCount;
						++symCount;
					} else
						break;
				} // get sym line
				
				// next come Beta eigenvectors (Energy; first row of data after syms)
				Buffer->GetLine(Line);
				lineBytes = strlen(Line);
				bytesRead = 0;
				float tmpEigenvec;
				// get a line of the values at a time (should be 5 per line, except possibly 
				// the last block may have fewer per line)
				while (bytesRead < lineBytes) {
					scanCount = sscanf(&Line[bytesRead], "%f%n", &tmpEigenvec, &bytesConsumed);
					if (scanCount == 1) {
						lFrame->Orbs[0]->EnergyB[eigenvecCount] = tmpEigenvec;
						bytesRead += bytesConsumed;
						++eigenvecCount;
					} else
						break;
				} //get eigenvector line

				// now get the Beta coefficients
				for (long iLine=0; iLine < nOrbs; ++iLine) {
					Buffer->GetLine(Line);
					// each line will contain up to 5 orbitals
					lineBytes = strlen(Line);
					bytesRead = 0;
					long iCoef = 0;
					float tmpCoef;
					// get coefficients line at a time, "symCount" times
					while (bytesRead < lineBytes && iCoef < symCount) {
						scanCount = sscanf(&Line[bytesRead], "%f%n", &tmpCoef, &bytesConsumed);
						if (scanCount == 1) {
							lFrame->Orbs[0]->VectorsB[(symCount-iSymCount)*nOrbs+iCoef*nOrbs+iLine] = tmpCoef;
							bytesRead += bytesConsumed;
							++iCoef;
						} else
							break;
					}
					if (iCoef < iSymCount) error = true;
					if (error) break;
				} // get coef block
				if (error) break;
			}
			else
				// if not found $END??
				break; // found $END of $COEFF_BETA
		}
		if (error) {
			// If we got to this point, Alpha was done correctly, so we don't delete the
			// OrbitalRec -just resize it to exclude Beta (check for NULL to be safe)
			if (lFrame->Orbs[0] != NULL) 
				lFrame->Orbs[0]->ReSize(MainData->Basis->GetNumBasisFuncs(false), 0);
			MessageAlert("Error while reading. No beta orbital set added.");
		}
		else
			CoefBetaDone = true;
	} // Beta Coefficients (orbitals) done
	else if (BasisDone && CoefAlphaDone) // no Beta Coeffs
		lFrame->Orbs[0]->ReSize(MainData->Basis->GetNumBasisFuncs(false), 0);

	// now look for Alpha Orbital Occupations (optional; depends on COEFF_ALPHA)
	if (CoefAlphaDone&&(Buffer->LocateKeyWord("$OCC_ALPHA", 10))) { 
		Buffer->SkipnLines(1); // skip $OCC_ALPHA line
		bool error = false;
		// the number of orbital occupations should be the number of basis functions
		long nOrbs = MainData->Basis->GetNumBasisFuncs(false);
		lFrame->Orbs[0]->OrbOccupation = new float[nOrbs];
		// if the array was allocated fine, we can continue
		if (lFrame->Orbs[0]->OrbOccupation != NULL) {
			// counters and a temporary slot for data read one item at a time
			long occCount = 0, lineOccs;
			float tmpOcc;
			// read the occupation numbers end $END; should have a case for EOF read
			while ((Buffer->GetFilePos()<Buffer->GetFileSize())&&!error) {
				Buffer->GetLine(Line);
				// continue until we find the end of the group
				if (FindKeyWord(Line, "$", 1)<0 && 0!=strncmp(Line, "\0", kMaxLineLength)) { 
					lineBytes = strlen(Line);
					bytesRead = 0;
					lineOccs = 0; 
					//read line at a time, up to 5, and within number of orb's defined above
					while (bytesRead < lineBytes && lineOccs < 5 && occCount < nOrbs) {
						scanCount = sscanf(&Line[bytesRead], "%f%n", &tmpOcc, &bytesConsumed);
						if (scanCount == 1) {
							bytesRead+=bytesConsumed;
							lFrame->Orbs[0]->OrbOccupation[occCount] = tmpOcc;
							++lineOccs;
							++occCount;
						} 
						else
							break;
					}
					if (error) break;
				}
				else
					break;
			}
			// insufficient data
			if (occCount < nOrbs) 
				error = true;
			// else set the number occupied to what we read.
			else 
				lFrame->Orbs[0]->NumOccupiedAlphaOrbs = occCount;		
		}
		if (error) {
			// free what we have and set NULL so dependent functions won't crash
			if (lFrame->Orbs[0]->OrbOccupation != NULL) 
				delete lFrame->Orbs[0]->OrbOccupation;
			lFrame->Orbs[0]->OrbOccupation = NULL;
			MessageAlert("Error while reading. No Alpha Occupations added.");
		}
		else
			OccAlphaDone = true;	
	}// Occ_Alpha done

	// now look for Beta Orbital Occupations (optional; depends on COEFF_BETA)
	if (CoefBetaDone&&OccAlphaDone&&(Buffer->LocateKeyWord("$OCC_BETA", 9))) { 
		Buffer->SkipnLines(1); // skip $OCC_BETA line
		bool error = false;

		lFrame->Orbs[0]->setOrbitalWavefunctionType(UHF);
		long nOrbs = MainData->Basis->GetNumBasisFuncs(false);
		lFrame->Orbs[0]->OrbOccupationB = new float[nOrbs];
		if (lFrame->Orbs[0]->OrbOccupationB != NULL) {
			// counters and a temporary slot for data read one item at a time
			long occCount = 0, lineOccs;
			float tmpOcc;
		
			while ((Buffer->GetFilePos()<Buffer->GetFileSize())&&!error) {
				Buffer->GetLine(Line);
				// continue until we find the end of the group
				if (FindKeyWord(Line, "$", 1)<0 && 0!=strncmp(Line, "\0", kMaxLineLength)) { 
					lineBytes = strlen(Line);
					bytesRead = 0;
					lineOccs = 0; 
					//read line at a time, up to 5, and within number of orb's defined above
					while (bytesRead < lineBytes && lineOccs < 5 && occCount < nOrbs) {
						scanCount = sscanf(&Line[bytesRead], "%f%n", &tmpOcc, &bytesConsumed);
						if (scanCount == 1) {
							bytesRead+=bytesConsumed;
							lFrame->Orbs[0]->OrbOccupationB[occCount] = tmpOcc;
							++lineOccs;
							++occCount;
						} 
						else
							break;
					}
					if (error) break;
				}
				else
					break;
			}
			// insufficient data
			if (occCount < nOrbs) 
				error = true;
			// else set the number occupied to what we read.
			else 
				lFrame->Orbs[0]->NumOccupiedBetaOrbs = occCount;		
		}
		if (error) {
			// free what we have and set NULL so dependent functions won't crash
			if (lFrame->Orbs[0]->OrbOccupationB != NULL)
				delete lFrame->Orbs[0]->OrbOccupationB;
			lFrame->Orbs[0]->OrbOccupationB = NULL; 
			MessageAlert("Error while reading. No Beta Occupations added.");
		}
		else
			OccBetaDone = true;		
	}// Occ_Beta Done

	// now look for vibrational frequencies for the last frame (independent)
	if ((0<lFrame->GetNumAtoms())&&(Buffer->LocateKeyWord("$FREQ", 5))) {
		Buffer->SkipnLines(1);	// skip $FREQ line
		// create the VibRec
		lFrame->Vibs = new VibRec(3*lFrame->NumAtoms, lFrame->NumAtoms);
		//continue only if we're successful
		if (lFrame->Vibs != NULL) {
			long nModes=0;
			char freq[kMaxLineLength];
			bool error = false;	//Use the error flag to bail out once we hit an error
			while ((Buffer->GetFilePos())<(Buffer->GetFileSize())&&!error) {
				Buffer->GetLine(Line);
				//continue until we find the end of the group
				if (FindKeyWord(Line, "$", 1)<0 && 0!=strncmp(Line, "\0", kMaxLineLength)) { 
					//the first line is the symmetry symbolis which we ignore
					// (we should've gotten those from the Alpha/Beta Coefficients)
					Buffer->GetLine(Line);	//next line is the frequencies
					long lineModes=0;
					lineBytes = strlen(Line);
					bytesRead = 0;
					// read Frequencies as strings one at a time in one line
					while (bytesRead < lineBytes) {
						scanCount = sscanf(&Line[bytesRead], "%s%n", freq, &bytesConsumed);
						if (scanCount == 1) {
							lFrame->Vibs->Frequencies.push_back(freq);
							bytesRead += bytesConsumed;
							++lineModes;
						} else
							break;
					}
					//Next come the vibrational offsets
					for (long iatm=0; iatm<lFrame->NumAtoms; ++iatm) {
						Buffer->GetLine(Line);
						//each line will normally contain x, y, z for one atom for 3 modes
						bytesRead = 0;
						for (long imode=0; imode<lineModes; ++imode) {
							CPoint3D temp;
							int scanCount = sscanf(&Line[bytesRead], "%f %f %f%n",
												   &temp.x, &temp.y, &temp.z, &bytesConsumed);
							if (scanCount == 3) {
								lFrame->Vibs->NormMode[iatm + lFrame->NumAtoms*(nModes+imode)] = temp;
								bytesRead += bytesConsumed;
							} else {
								lineModes = imode;
								error = true;
								break;
							}
						}
						if (error) break;
					}
					nModes += lineModes;
				} else
					break;
			}// end while in file and not error
			lFrame->Vibs->NumModes = nModes;
			lFrame->Vibs->Resize(nModes);
		}
	} // vibrational frequencies done
	// Don't need an else, case, since lFrame->Vibs would be NULL, anyway
	// We don't handle the $DIPOLE section in MKL files
	// OpenMKLFile() can return "success" because we are done parsing the file
	return 1;
}

/**
  * Adds support for opening the MoPac zMatrix MOP and mop file types, and
  * MoPac archive output ARC and arc files.
  * @param Buffer A BufferFileObject that the .MOP or .mop file is buffereid.
  * @param fileType A char indicating whether the file is a MOP or ARC file.
  * into to make parsing the file easier.  See the BufferFile object for 
  * valid BufferFile operations. Parsing code adapted from 
  * MoleculeData::ParseMOPACZMatrix.
  * @return Returns 1 upon open success or 0 on error or failure to open
*/
long MolDisplayWin::OpenMOPACFile(BufferFile * Buffer, TextFileType fileType) {
	// buffer line for text to be scanned while parsing
	char		Line[kMaxLineLength];
	// placeholder for traversing file sections in Buffer
	wxFileOffset	startOfAtoms = 1;
	// # of tokens sucessfully read in a sscanf call, # line of atoms read 
	int			scanCount = 0, iLine = 0;
	// vars for holding sscanf read, borrowing some from ParseMOPACZMatrix()
	CPoint3D	pos = CPoint3D(0.0f, 0.0f, 0.0f);	// just a placeholder
	char		symbol[4];	// This could probably safely be 2-3 chars long
	float		bondLength = -0.1f, bondAngle = 0.0f, bondDihedral = 0.0f;
	// sane defaults to avoid accessing unassigned variables
	long		AtomType = -1, nAtoms = 0;
	int			j1 = -1, j2 = -1, j3 = -1, con1, con2, con3;
	bool		error = false;

	ProgressInd->ChangeText("Reading MOPAC file...");
	Frame *lFrame = MainData->cFrame;
	//zero out initial symbol
	memset((void *)symbol, 0, (size_t)(4*sizeof(char)));
	
	// file is of type ARC
	if (fileType == MOPACarcFile) {
		// Get to the right part of the file
		Buffer->LocateKeyWord("DATE:", 5);
		Buffer->SkipnLines(1);
		Buffer->LocateKeyWord("DATE:", 5);
		Buffer->SkipnLines(1);
	}
	// file is of type MOP
	else if (fileType == MOPACmopFile) 
		// skip descriptions, MoPac parms and goto start of atoms
		Buffer->SkipnLines(3);	
	else
		return 0;	// quit 'cause we don't know what kind of file we were passed
	
	startOfAtoms = Buffer->GetFilePos();
	// Count the number of Atoms
	while (Buffer->GetFilePos()	< Buffer->GetFileLength()) {
		Buffer->GetLine(Line);
		scanCount = sscanf(Line, "%s %f %d %f %d %f %d %d %d %d", &symbol, &bondLength, 
					&j1, &bondAngle, &j2, &bondDihedral, &j3, &con1, &con2, &con3);
		// The last atom in the list may be a duplicate of the
		// first atom in the list and needs to be ignored
		if (scanCount > 2 && nAtoms > 1 && bondLength < 0.00001) break;
		// otherwise, it is [probably] a new atom
		if ( (scanCount > 9 && nAtoms > 2) || (scanCount > 8 && nAtoms == 2) || 
			(scanCount > 6 && nAtoms == 1) || (scanCount > 4 && nAtoms == 0) ) 
			++nAtoms;
		else 
			break;
	}
	// atoms are now counted; go back to atoms' start for actual parsing
	Buffer->SetFilePos(startOfAtoms);
	
	// data in this file will be treated as a zMatrix 
	// cartesian mop files exist, but usually have a different extension
	if (!(MainData->IntCoords)) 
		MainData->IntCoords = new Internals;
	MOPacInternals * mInts = MainData->IntCoords->GetMOPacStyle();
	if (!mInts) {
		MainData->IntCoords->CreateMOPacInternals(3*nAtoms);
		mInts = MainData->IntCoords->GetMOPacStyle();
	}
	float unitConversion = 1.0f;
	if (MainData->InputOptions && MainData->InputOptions->Data->GetUnits()) 
		unitConversion = kBohr2AngConversion;

	// Get and add atoms (parsing) loop
	while (iLine < nAtoms) {
		con1 = -1;	// Default Values
		con2 = -1;
		con3 = -1;
		Buffer->GetLine(Line);
		scanCount = sscanf(Line, "%s %f %d %f %d %f %d %d %d %d", &symbol, &bondLength, 
					&j1, &bondAngle, &j2, &bondDihedral, &j3, &con1, &con2, &con3);
		if (scanCount < 1) { 
			error = true;
			break;	//failed to get anything
		}
		// Get type of atom by atomic symbol and add it to fram (dummy position 0,0,0)
		AtomType = SetAtomType((unsigned char *)symbol);
		lFrame->AddAtom(AtomType, pos);
		if (iLine > 0) {
			if (scanCount < 2) {
				error = true;
				break;
			}
			if (iLine == 1)	//the second atom will specify only the bond length
				con1 = 1;
			// 3rd Line and beyond (iLine == 2 and up)
			else {
				if (iLine == 2) {	//For the third atom the connectivity is optional
					if ((scanCount >= 5)&&(scanCount <= 7)) {
						con1 = 2;
						con2 = 1;	//The default allows the connections to be assumed
						if (scanCount >= 6) {
							con1 = (int) bondDihedral;
							con2 = j3;
						}
					} 
					else if (scanCount < 5) break;	// in this case the line is invalid
				}
			}
			if (bondLength < 0.00001) 
				break; // bad value or dupe of first atom
			// reduce values by 1 because our indexing starts at 0 not 1
			con1--;
			con2--;
			con3--;
			if (con1 >= iLine) 
				break;
			// store connections and coordinates
			mInts->AddInternalCoordinate(iLine, con1, 0, bondLength*unitConversion);
			if (iLine > 1) {
				mInts->AddInternalCoordinate(iLine, con2, 1, bondAngle);
				if (iLine > 2)
					mInts->AddInternalCoordinate(iLine, con3, 2, bondDihedral);
			}
		}
		++iLine;
	} // end parsing loop
	if (error) 
		return 0; // failure due to an error: quit!
	// if we punted after the AddAtom call delete off the 
	// atom without internal coordinate information
	if (iLine > lFrame->NumAtoms) 
		lFrame->DeleteAtom(iLine-1);
	// now convert the set of internals into cartesians
	// this caused some segfaults in testing, but probably due to bad files
	mInts->InternalsToCartesians(MainData, Prefs, 0);
	// Setup bonds, if needed													
	if (Prefs->GetAutoBond())
		lFrame->SetBonds(Prefs, true, ProgressInd);
	return 1;	// OpenMOPACFile success
}

long MolDisplayWin::OpenXYZFile(BufferFile * Buffer) {
	char	Line[kMaxLineLength];
	long	nAtoms,i;
	short	scanerr;

	ProgressInd->ChangeText("Reading XYZ file...");
	Frame * lFrame = MainData->cFrame;
		//1st line contains the number of atoms
	Buffer->GetLine(Line);
	scanerr = sscanf(Line, "%ld", &nAtoms);
	if ((scanerr!=1)||(nAtoms<=0)) {
		wxLogMessage(_("XYZ files must have an integer representing the number of atoms on the first line of the file."));
		throw DataError();
	}
		//allocate memory for the atoms
	if (lFrame->NumAtoms > 0) {	//If there are already atoms treat as an append
		lFrame = MainData->AddFrame(nAtoms,0);
	} else {
		if (!MainData->SetupFrameMemory(nAtoms, 0)) throw MemoryError();
		Buffer->GetLine(Line);
		MainData->SetDescription(Line);
	}
	long DRCnSkip = Prefs->GetDRCSkip(), nSkip=0;
		bool Done=false;
		bool RdPoint = true;
	try {
		while (!Done) {
			Done = true;
			if (RdPoint) {
				VibRec * lVibs = NULL;
				for (i=0; i<nAtoms; ++i) {
						long AtomType, test;
						CPoint3D Pos, Vector;

					Buffer->GetLine(Line);
					test = ParseCartLine(Line, &AtomType, &Pos, &Vector, -1);
					
					if (test==-1) {	//invalid atom type
						wxLogMessage(_("Error: An invalid Atom Type was encountered in the atom list."));
						throw DataError();
					} else if (test<0) {//other invalid data was encountered
						wxLogMessage(_("An error occured while reading the file. Open File Aborted!"));
						throw DataError();
					}
					if (AtomType > 115) {
						if (AtomType > 255) {
							if (((AtomType - 255) < 1)||((AtomType - 255) > nAtoms)) {
								wxLogMessage(_("Invalid atom number detected in special atom list."));
								throw DataError();
							}
						}
						if (!lFrame->AddSpecialAtom(Vector, i)) {
							throw MemoryError();
						}
					} else if (test == 7) {	//mass weight the normal mode
						if (i==0) {
							lVibs = new VibRec(1, nAtoms);
							if (!lVibs) throw MemoryError();
						}
						if (lVibs) {
							lVibs->NormMode[i].x = Vector.x*Prefs->GetSqrtAtomMass(AtomType-1);
							lVibs->NormMode[i].y = Vector.y*Prefs->GetSqrtAtomMass(AtomType-1);
							lVibs->NormMode[i].z = Vector.z*Prefs->GetSqrtAtomMass(AtomType-1);
						}
					}
					lFrame->AddAtom(AtomType, Pos);
					MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.x));
					MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.y));
					MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.z));
				}
				if (lVibs) lFrame->Vibs = lVibs;
				if (Prefs->GetAutoBond())	//setup bonds, if needed
					lFrame->SetBonds(Prefs, false, ProgressInd);
			}
			wxFileOffset cPos = Buffer->GetFilePos();
			wxFileOffset fileLength = Buffer->GetFileLength();
			if (cPos < fileLength) {	//If we haven't reached the end of the file
				Buffer->GetLine(Line);	//check to see if another frame is available
				scanerr = sscanf(Line, "%ld", &nAtoms);
				if ((scanerr==1)&&(nAtoms>0)) {
					if (nSkip >= DRCnSkip) {
						nSkip = 0;
						RdPoint = true;
						lFrame = MainData->AddFrame(nAtoms,0);
						if (!lFrame) throw MemoryError();
						Buffer->GetLine(Line);
						if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
							{ throw UserCancel();}
					} else {
						++nSkip;
						Buffer->SkipnLines(nAtoms+1);
						RdPoint = false;
					}
					Done = false;
				}
			}
		}
	}
	catch (DataError /*Error*/) {
		//attempt to save part of file
		if (MainData->GetNumFrames() > 1) {
			MainData->DeleteFrame();
		}
		wxLogMessage(_("Error while parsing file. Partial file may be valid."));
	}
	return 1;
}

long MolDisplayWin::OpenMolPltFile(BufferFile *Buffer) {
	long				j,ii, nkinds=0, catm, iscanerr=0, LineLength, LinePos=0, fileAtoms=0,
						fileBonds=0;
	int					nchar;
	long				test=0, Mode=0;
	float				BondLength=0.0f;
	char				LineText[kMaxLineLength+1], KeyWord[kMaxLineLength+1], token[5];

	ProgressInd->ChangeText("Reading MolPlt format file...");
	Frame * lFrame = MainData->cFrame;
/*Now interpert the file data */
		// Grab the first line (containing all keywords)
	Buffer->GetLine(LineText);
	LineLength = strlen(LineText);

	LinePos = FindKeyWord(LineText, "NATOMS", 6);
	if (LinePos >= 0) {
		LinePos += 6;
		iscanerr = sscanf(&LineText[LinePos], "%1s%ld", token, &fileAtoms);
		if (iscanerr != 2) return 0;
	} else return 0;	// NATOMS keyword not found!
	LinePos = FindKeyWord(LineText, "NKINDS", 6);
	if (LinePos >= 0) {
		LinePos += 6;
		sscanf(&LineText[LinePos], "%1s%ld", token, &nkinds);
	}
	LinePos = FindKeyWord(LineText, "NBONDS", 6);
	if (LinePos >= 0) {
		LinePos += 6;
		sscanf(&LineText[LinePos], "%1s%ld", token, &fileBonds);
	}
	LinePos = FindKeyWord(LineText, "MODE", 4);
	if (LinePos >= 0) {
		LinePos += 4;
		sscanf(&LineText[LinePos], "%1s%ld", token, &Mode);
	}
	LinePos = FindKeyWord(LineText, "BNDLENGTH", 9);
	if (LinePos >= 0) {
		LinePos += 9;
		sscanf(&LineText[LinePos], "%1s%f", token, &BondLength);
	}
	LinePos = FindKeyWord(LineText, "BONDLENGTH", 10);
	if (LinePos >= 0) {
		LinePos += 10;
		sscanf(&LineText[LinePos], "%1s%f", token, &BondLength);
	}

	if (fileAtoms <= 0) {			/* There don't appear to be any atoms to read in ???*/
		AbortOpen("NATOMS keyword missing. Are you sure this is a Molplt file\? Open file aborted.");
		return 0;
	}
		//Allocate frame memory
	if (!MainData->SetupFrameMemory(fileAtoms, fileBonds)) throw MemoryError();
			// The second line is just a description so copy it to a string
	Buffer->GetLine(LineText);
	MainData->SetDescription(LineText);

	//Need to keep the single mode storage separated off as the AddAtom function will delete it
	//from the Frame record otherwise.
	VibRec * Mode1Vibs = NULL;
	if (Mode == -1) {
		Mode1Vibs = new VibRec(1, fileAtoms);
		if (!Mode1Vibs) throw MemoryError();
	}
	Buffer->SkipnLines(nkinds);

// read in the coords and atomtypes
	for (j=0; j<fileAtoms; ++j) {			// loop over the number of atoms
			long AtomType;
			CPoint3D Pos, Vector;

		if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
		{ throw UserCancel();}
		Buffer->GetLine(LineText);
		test = ParseCartLine(LineText, &AtomType, &Pos, &Vector, Mode);
		
		if (AtomType==-1) {	//invalid atom type
				//Choices would seem to be to punt and abort or change to some sort of valid type
			wxLogMessage(_("Error: An invalid Atom Type was encountered in the atom list."));
			if (Mode1Vibs) delete Mode1Vibs;
			throw DataError();
		} 
		if (test<0) {
			wxLogMessage(_("An error occured while reading the file. Open File Aborted!"));
			if (Mode1Vibs) delete Mode1Vibs;
			throw DataError();
		}
		if (AtomType > 115) {
			if (Mode < 0) {
				wxLogMessage(_("Error: Special Atom types may not be used with Normal Modes!"));
				if (Mode1Vibs) delete Mode1Vibs;
				throw DataError();
			}
			if (AtomType > 255) {
				if (((AtomType - 255) < 1)||((AtomType - 255) > fileAtoms)) {
					wxLogMessage(_("Invalid atom number detected in special atom list."));
					if (Mode1Vibs) delete Mode1Vibs;
					throw DataError();
				}
			}
			if (!lFrame->AddSpecialAtom(Vector, j)) throw MemoryError();
		}
		if (Mode == -1) {	//mass weight the normal mode
			Mode1Vibs->NormMode[j].x = Vector.x*Prefs->GetSqrtAtomMass(AtomType-1);
			Mode1Vibs->NormMode[j].y = Vector.y*Prefs->GetSqrtAtomMass(AtomType-1);
			Mode1Vibs->NormMode[j].z = Vector.z*Prefs->GetSqrtAtomMass(AtomType-1);
		}
		lFrame->AddAtom(AtomType, Pos);
		MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.x));
		MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.y));
		MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.z));
	}
	if (Mode1Vibs) lFrame->Vibs = Mode1Vibs;
	
	if (fileBonds > 0) {						/* read in the array of bonds */
		long	ibond=-1, temp;
		Buffer->GetLine(LineText);
		LinePos = FindKeyWord(LineText, "BONDATOMS", 9);
		if (LinePos >= 0) {
			LinePos += 9;
			LineLength = strlen(LineText);
			for (j=0; j<(2*fileBonds); ++j) {
				while (LineText[LinePos] && ((LineText[LinePos] < '0')||
						(LineText[LinePos] > '9'))) ++LinePos;
				while (LinePos >= LineLength) {	//EOL reached try getting a new line
					wxFileOffset CurrentPos = Buffer->GetFilePos();
					Buffer->GetLine(LineText);
					if ((-1<FindKeyWord(LineText, "ATOMIC", 6))||
						(-1<FindKeyWord(LineText, "MODE", 4))) {
						Buffer->SetFilePos(CurrentPos);
						break;
					}
					LinePos = 0;
					LineLength = strlen(LineText);
					while (LineText[LinePos] && ((LineText[LinePos] < '0')||
							(LineText[LinePos] > '9'))) ++LinePos;
				}
				if (LinePos < LineLength) {
					iscanerr = sscanf(&LineText[LinePos], "%ld%n", &temp, &nchar);
					if (iscanerr != 1)
						return 1;
					LinePos += nchar;
					if ((temp >= 1)&&(temp <= lFrame->NumAtoms)) {
						temp--;
						if (ibond == -1) ibond = temp;
						else {
							lFrame->AddBond(ibond, temp);
							ibond = -1;		//reset ibond for the next bond pair
						}
					} else	// Bad bonded atom number
						return 1;
				} else break;	//and break since we've run out of bond #'s
			}
		} else {
			return 1;
		}
	}
	if ((BondLength > 0.0)||(Prefs->GetAutoBond())) {
		Prefs->SetMaxBondLength(BondLength);
		lFrame->SetBonds(Prefs, false, ProgressInd);
	}
	if (Mode > 0) {
		nkinds = 3*(lFrame->NumAtoms);		/* In general there will be 3N normal modes */
		VibRec * lVibs = MainData->cFrame->Vibs = new VibRec(nkinds, fileAtoms);
		if (!lVibs) throw MemoryError();
		catm = 0;
		ii=0;
		try {
					/* Now locate the first frequency (skipping the atomic masses) */
			wxFileOffset CurrentPos = Buffer->GetFilePos();
			Buffer->GetLine(LineText);
			while (-1>=FindKeyWord(LineText, "MODE", 4)) {
				CurrentPos = Buffer->GetFilePos();
				Buffer->GetLine(LineText);
			}
			Buffer->SetFilePos(CurrentPos);

			for (; ii < (3*(lFrame->NumAtoms)); ++ii) {
				/* Allow user cancels */
				if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
					{ throw UserCancel();}
				Buffer->GetLine(LineText);
				for (j=0; LineText[j] && (LineText[j] != '='); ++j) ;
				if (LineText[j] != '=') break;	/* no more frequencies so quit */
				sscanf(&LineText[j+1], "%s", KeyWord);
				LineLength = strlen(KeyWord);
	/*Check to see if the first character is a '-' if so change it to an 'i' since it really is an
	imaginary mode and because the menu manager will think it means create a divider in the menu instead
	of a regular text item */
				if (KeyWord[0] == '-') KeyWord[0] = 'i';
				lVibs->Frequencies.push_back(std::string(KeyWord));

				for (j=0; j < (lFrame->NumAtoms); ++j) {
					Buffer->GetLine(LineText);
					iscanerr = sscanf(LineText, "%f%f%f", &((lVibs->NormMode[catm]).x),
						&((lVibs->NormMode[catm]).y), &((lVibs->NormMode[catm]).z));
					if (iscanerr != 3) {			/*Uh Ohh looks like there was a problem reading the file*/
						wxLogMessage(_("Error reading the Normal Mode input. Open file aborted."));
						throw DataError();
					}
					(lVibs->NormMode[catm]).x *= (Prefs->GetSqrtAtomMass((lFrame->Atoms[j].Type)-1));
					(lVibs->NormMode[catm]).y *= (Prefs->GetSqrtAtomMass((lFrame->Atoms[j].Type)-1));
					(lVibs->NormMode[catm]).z *= (Prefs->GetSqrtAtomMass((lFrame->Atoms[j].Type)-1));
					++catm;
				}
			}
		}
		catch (FileError) {	//We've hit the end of file so truncate the Mode list
		}
		lVibs->NumModes = ii;
		if (lVibs->NumModes < 3*lFrame->NumAtoms)	/* Downsize the frequency storage to the size actually used */
			lVibs->Resize(lFrame->NumAtoms);
	}
	if (lFrame->Vibs) {
		lFrame->Vibs->CurrentMode = Mode - 1;
		if (lFrame->Vibs->CurrentMode < 0) lFrame->Vibs->CurrentMode = 0;
		MainData->SetDrawMode(true);
	}
	if (lFrame->SpecialAtoms)	MainData->SetSpecialAtomDrawMode(true);

	return 1;
} /* OpenMolPlt */
long MolDisplayWin::OpenMoldenFile(BufferFile * Buffer) {
	char	LineText[kMaxLineLength];
	Frame * lFrame = MainData->cFrame;
	Buffer->SetFilePos(0);
	if (Buffer->LocateKeyWord("[GEOMETRIES]", 12, -1)) {	//both formats use Angstroms
		Buffer->GetLine(LineText);
		if (FindKeyWord(LineText, "XYZ", 3) >= 0) { //cartesian coordinates
												   // XYZ format looks like
												   //   #_of_atoms
												   //  title often containing "scf done: energy"
												   //  one line for each atom in the form "symbol x y z"
			while (Buffer->GetFilePos() < Buffer->GetFileSize()) {
				Buffer->GetLine(LineText);
				int atmCount=0;
				if (sscanf(LineText, "%d", &atmCount) == 1) {
					if (atmCount <= 0) break;
					Buffer->GetLine(LineText);
					if (lFrame->NumAtoms > 0) {
						float t = lFrame->time;
						lFrame = MainData->AddFrame(atmCount, 0);
						lFrame->time = t+1;
					}
					int epos = FindKeyWord(LineText, "done:", 5);
					if (epos > 0) {
						sscanf(&(LineText[epos+5]), "%lf", &(lFrame->Energy));
					}
					for (int iatm=0; iatm<atmCount; ++iatm) {
						unsigned char token[kMaxLineLength];
						CPoint3D	pos;
						Buffer->GetLine(LineText);
						int rdcount = sscanf(LineText, "%s %f %f %f", token, &(pos.x), &(pos.y), &(pos.z));
						if (rdcount == 4) {
							long atomnum = SetAtomType(token);
							if (atomnum>0)
								lFrame->AddAtom(atomnum, pos);
						}
					}
					if (Prefs->GetAutoBond())
						lFrame->SetBonds(Prefs, false, ProgressInd);
				} else break;
			}
		} else if (FindKeyWord(LineText, "ZMAT", 4) >= 0) {	//GAMESS-UK style z matrix
			while (Buffer->GetFilePos() < Buffer->GetFileSize()) {
				Buffer->GetLine(LineText);
				if (FindKeyWord(LineText, "ZMAT", 4) >= 0) {
					Buffer->BackupnLines(1);
					if (lFrame->NumAtoms > 0) {
						float t = lFrame->time;
						lFrame = MainData->AddFrame(lFrame->NumAtoms, 0);
						lFrame->time = t+1;
					}
					MainData->ParseGAMESSUKZMatrix(Buffer, Prefs);
					if (lFrame->NumAtoms == 0) break;
					if (Prefs->GetAutoBond())
						lFrame->SetBonds(Prefs, false, ProgressInd);
				} else break;
			}
		}	//else there is some undocumented type...
		Buffer->SetFilePos(0);
		if (Buffer->LocateKeyWord("[GEOCONV]", 9, -1)) {
			Buffer->SkipnLines(1);
			bool good = true;
			while (good) {
				good = false;
				Buffer->GetLine(LineText);
				if (FindKeyWord(LineText, "energy", 6) == 0) {
					good = true;
					int iframe = 1;
					MainData->SetCurrentFrame(iframe);
					Buffer->GetLine(LineText);
					while (sscanf(LineText, "%lf", &MainData->cFrame->Energy) == 1) {
						++iframe;
						if (iframe > MainData->GetNumFrames()) break;
						MainData->SetCurrentFrame(iframe);
						Buffer->GetLine(LineText);
					}
					if (iframe <= MainData->GetNumFrames()) Buffer->BackupnLines(1);
				} else if (FindKeyWord(LineText, "max-force", 9) == 0) {
					good = true;
					int iframe = 1;
					MainData->SetCurrentFrame(iframe);
					Buffer->GetLine(LineText);
					float val;
					while (sscanf(LineText, "%f", &val) == 1) {
						MainData->cFrame->SetRMSGradient(val);
						++iframe;
						if (iframe > MainData->GetNumFrames()) break;
						MainData->SetCurrentFrame(iframe);
						Buffer->GetLine(LineText);
					}
					if (iframe <= MainData->GetNumFrames()) Buffer->BackupnLines(1);
				} else if (FindKeyWord(LineText, "rms-force", 9) == 0) {
					good = true;
					int iframe = 1;
					MainData->SetCurrentFrame(iframe);
					Buffer->GetLine(LineText);
					float val;
					while (sscanf(LineText, "%f", &val) == 1) {
						MainData->cFrame->SetMaximumGradient(val);
						++iframe;
						if (iframe > MainData->GetNumFrames()) break;
						MainData->SetCurrentFrame(iframe);
						Buffer->GetLine(LineText);
					}
					if (iframe <= MainData->GetNumFrames()) Buffer->BackupnLines(1);
				}
			}
			MainData->SetCurrentFrame(MainData->GetNumFrames());
		}
	}
	Buffer->SetFilePos(0);
	// Now look for the ATOMS keyword to use as coordinates for any MOs 
	if (Buffer->LocateKeyWord("[ATOMS]", 7, -1)) {
		float unitConv = 1.0f;	//default to angstroms
		Buffer->GetLine(LineText);
		if (FindKeyWord(LineText, "AU", 2) > 0) unitConv = kBohr2AngConversion;
		if (lFrame->NumAtoms > 0) lFrame = MainData->AddFrame(lFrame->NumAtoms, 0);
		//ugh why does everybody have to create their own cartesian format...
		Buffer->GetLine(LineText);
		while ((FindKeyWord(LineText, "[", 1) < 0)&&(Buffer->GetFilePos()<Buffer->GetFileSize())) {
			char	token[kMaxLineLength];
			long atomNum, junk;
			CPoint3D pos;
			//name # atomic_# x y z
			int count = sscanf(LineText, "%s %ld %ld %f %f %f", token, &junk, &atomNum,
							   &(pos.x), &(pos.y), &(pos.z));
			pos *= unitConv;
			if ((count == 6)&&((atomNum>0)&&(atomNum<120))) {
				lFrame->AddAtom(atomNum, pos);
			} else {
				throw DataError();
			}
			Buffer->GetLine(LineText);
		}
		Buffer->SetFilePos(0);
		if (Prefs->GetAutoBond())
			lFrame->SetBonds(Prefs, false, ProgressInd);
		//Now look for a basis set
		//MacMolPlt only supports GTOs so we don't even look for the STO keyword
		if (Buffer->LocateKeyWord("[GTO]", 5, -1)) {
				//We really don't know how many shells there will be so this is just
				//a rough estimate.
			BasisSet * lbasis = new BasisSet(lFrame->NumAtoms, 5*lFrame->NumAtoms);
			if (lbasis->ReadMolDenBasisSet(Buffer, lFrame->NumAtoms)) {
				if (MainData->Basis) delete MainData->Basis;
				MainData->Basis = lbasis;
				lbasis->Normalize(false, false);
			} else
				delete lbasis;
		}
		bool sphericalHarmonics=false;
		//Should look for [5D], [5D10F], [5D7F] to catch unsupported spherical harmonics
		if (Buffer->LocateKeyWord("[5D]", 4, -1)||Buffer->LocateKeyWord("[5D7F]", 6, -1)) {
			wxLogMessage(_("Spherical harmonic basis sets are not supported. Molecular orbitals will be skipped."));
			sphericalHarmonics = true;
		}
		if (MainData->Basis && !sphericalHarmonics) {	//look for orbitals if we have a basis set
			if (Buffer->LocateKeyWord("[MO]", 4, -1)) {
				Buffer->SkipnLines(1);
				lFrame->ReadMolDenOrbitals(Buffer, MainData->Basis->GetNumBasisFuncs(false));
			}
		}
	}
	Buffer->SetFilePos(0);
	if (Buffer->LocateKeyWord("[FR-COORD]", 10, -1)) {	//FR-COORD section is coordinates for frequencies
														//The units are bohr throughout
		if (lFrame->NumAtoms > 0) lFrame = MainData->AddFrame(lFrame->NumAtoms, 0);
		Buffer->SkipnLines(1);
		Buffer->GetLine(LineText);
		while ((LineText[0] != '[')&&(Buffer->GetFilePos()<Buffer->GetFileSize())) {
			unsigned char	token[kMaxLineLength];
			CPoint3D pos;
			//name x y z
			int rdcount = sscanf(LineText, "%s %f %f %f", token, &(pos.x), &(pos.y), &(pos.z));
			pos *= kBohr2AngConversion;
			if (rdcount == 4) {
				long atomnum = SetAtomType(token);
				if (atomnum>0)
					lFrame->AddAtom(atomnum, pos);
			}
			Buffer->GetLine(LineText);
		}
		Buffer->SetFilePos(0);
		if (lFrame->NumAtoms > 0) {
			if (Prefs->GetAutoBond())
				lFrame->SetBonds(Prefs, false, ProgressInd);
			if (Buffer->LocateKeyWord("[FREQ]", 6, -1)) {
				lFrame->ParseMolDenFrequencies(Buffer, Prefs);
			}
		}
	}
	
	return 1;
}

long MoleculeData::ParseECPotentials(BufferFile * Buffer) {
	long	ElectronsRemoved = 0, ProtonsRemoved=0, LinePos, atom;
	char	LineText[kMaxLineLength];
	if (cFrame->NumAtoms <= 0) {
		Buffer->SkipnLines(1);
		return 0;
	}
	long *	zcore = new long[cFrame->NumAtoms];
	if (!zcore) throw MemoryError();
	for (atom=0; atom<cFrame->NumAtoms; ++atom) zcore[atom]=0;
	
	wxFileOffset StartPos = Buffer->GetFilePos();
	//This works for both ECPs and MCPs
	if (!Buffer->LocateKeyWord("CP RUN REMOVES", 14, -1)) {
		delete [] zcore;
		return 0;
	}
	wxFileOffset EndPos = Buffer->GetFilePos();
	Buffer->GetLine(LineText);	//read in the number of electrons removed
	sscanf(&(LineText[15]),"%ld", &ElectronsRemoved);

		//Now read in the number of protons removed from each atom
	Buffer->SetFilePos(StartPos);
	if (Buffer->LocateKeyWord("MODEL-POTENTIALS", 16, EndPos)) {
		while (Buffer->LocateKeyWord("THE MCP PLACED ON ATOM", 22, EndPos)) {
			Buffer->GetLine(LineText);
			LinePos = FindKeyWord(LineText, "ON ATOM", 7)+7;
			sscanf(&(LineText[LinePos]), "%ld", &atom);
			atom--;
			if (atom>=0 && atom<cFrame->NumAtoms) {
				LinePos = FindKeyWord(LineText, "REMOVES ZCORE=", 14)+14;
				if (LinePos>=14) {
					sscanf(&(LineText[LinePos]), "%ld", &(zcore[atom]));
					if (zcore[atom]>0 && zcore[atom] <= cFrame->Atoms[atom].GetNuclearCharge()) {
						Basis->NuclearCharge[atom] -= zcore[atom];
						ProtonsRemoved += zcore[atom];
					}
				}
			}
		}
	} else {
		while (Buffer->LocateKeyWord("PARAMETERS FOR", 14, EndPos)) {
			Buffer->GetLine(LineText);
			LinePos = FindKeyWord(LineText, "ON ATOM", 7)+7;
			sscanf(&(LineText[LinePos]), "%ld", &atom);
			atom--;
			if (atom>=0 && atom<cFrame->NumAtoms) {
				LinePos = FindKeyWord(LineText, "WITH ZCORE", 10)+10;
				if (LinePos>=10) {
					sscanf(&(LineText[LinePos]), "%ld", &(zcore[atom]));
					if (zcore[atom]>0 && zcore[atom] <= cFrame->Atoms[atom].GetNuclearCharge()) {
						Basis->NuclearCharge[atom] -= zcore[atom];
						ProtonsRemoved += zcore[atom];
					}
				} else {
					LinePos = FindKeyWord(LineText, "ARE THE SAME AS ATOM", 20)+20;
					if (LinePos>=20) {
						long OtherAtom;
						sscanf(&(LineText[LinePos]), "%ld", &OtherAtom);
						OtherAtom --;
						if (OtherAtom>=0 && OtherAtom<atom) {
							zcore[atom] = zcore[OtherAtom];
							Basis->NuclearCharge[atom] -= zcore[atom];
							ProtonsRemoved += zcore[atom];
						}
					}
				}
			}
		}
	}
	delete [] zcore;
	if (ProtonsRemoved != ElectronsRemoved) {
		Basis->NuclearChargesAreValid(false);
//		MessageAlert("Parse Error while reading ECP Potential Data. Use with caution!");
	}
	return ElectronsRemoved;
}
long MoleculeData::ParseTinkerCoordinates(BufferFile *Buffer) {
	char		LineText[kMaxLineLength];
	
	Frame * lFrame = cFrame;
	if (!Buffer->LocateKeyWord("Cartesian Coordinates of Atoms in Bulk Model", 44)) {
		return 0;
	}	
	//Grab the tinker coordinate output
	
	Buffer->SkipnLines(3);
	wxFileOffset StartPos = Buffer->GetFilePos();
	if (!(Buffer->LocateKeyWord("-----------", 11, -1))) throw DataError();
	wxFileOffset test = Buffer->GetFilePos() - StartPos;
	Buffer->SetFilePos(StartPos);
	long numlines = Buffer->GetNumLines(test) - 1;
	if (numlines > 0) {
		if (!SetupFrameMemory(numlines, 0)) throw std::bad_alloc();
	} else {
		wxLogMessage(_("Unable to locate Tinker coordinates in file."));
		throw DataError();
	}
	for (long i=0; i<numlines; ++i) {
		long linenum;
		CPoint3D	position;
		unsigned char	Label[kMaxLineLength];
		
		Buffer->GetLine(LineText);
		int scannum = sscanf(LineText, "%ld %s %f %f %f", &linenum, Label, &(position.x),
							 &(position.y), &(position.z));
		if (scannum != 5) {
			wxLogMessage(_("Error encountered while parsing coordinates."));
			throw DataError();
		}
		long atomtype = SetAtomType(Label);
		mpAtom * newAtom = lFrame->AddAtom(atomtype, position);
		if (newAtom) newAtom->IsSIMOMMAtom(true);
	}
	return 1;
}
long MolDisplayWin::ParseSIMMOMLogFile(BufferFile *Buffer, long EnergyPos) {
		char		LineText[kMaxLineLength];

	Frame * lFrame = MainData->cFrame;
	if (!Buffer->LocateKeyWord("Cartesian Coordinates of Atoms in Bulk Model", 44, EnergyPos)) {
		return 0;
	}	
		//Grab the tinker coordinate output from the top of the file
	ProgressInd->ChangeText("Reading Tinker Coordinates");
	if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
		{throw UserCancel();}

	Buffer->SkipnLines(3);
	wxFileOffset StartPos = Buffer->GetFilePos();
	if (!(Buffer->LocateKeyWord("-----------", 11, -1))) throw DataError();
	wxFileOffset test = Buffer->GetFilePos() - StartPos;
	Buffer->SetFilePos(StartPos);
	long numlines = Buffer->GetNumLines(test) - 1;
	if (numlines > 0) {
		if (!MainData->SetupFrameMemory(numlines, 0)) throw std::bad_alloc();
	} else {
		wxLogMessage(_("Unable to locate Tinker coordinates in file."));
		throw DataError();
	}
	for (long i=0; i<numlines; ++i) {
			long linenum;
			CPoint3D	position;
			unsigned char	Label[kMaxLineLength];
			
		Buffer->GetLine(LineText);
		int scannum = sscanf(LineText, "%ld %s %f %f %f", &linenum, Label, &(position.x),
				&(position.y), &(position.z));
		if (scannum != 5) {
			wxLogMessage(_("Error encountered while parsing coordinates."));
			throw DataError();
		}
		long atomtype = SetAtomType(Label);
		mpAtom * newAtom = lFrame->AddAtom(atomtype, position);
		if (newAtom) newAtom->IsSIMOMMAtom(true);
	}
		//Now add the full ab initio atoms
	if (Buffer->LocateKeyWord("COORDINATES (BOHR)", 16, -1)) {	//first normal (ab initio) atoms
		ProgressInd->ChangeText("Reading Coordinates");
		if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
			{ AbortOpen("File open canceled by user"); return 0;}

		Buffer->SkipnLines(2);
		StartPos = Buffer->GetFilePos();
		test = Buffer->FindBlankLine() - StartPos;
		numlines = Buffer->GetNumLines(test);
		if (numlines > 0) {
			if (!MainData->SetupFrameMemory(numlines+lFrame->NumAtoms, 0)) throw MemoryError();
		} else {
			wxLogMessage(_("Unable to locate coordinates in the file."));
			throw DataError();
		}
		if (!ParseGLogLine(Buffer, lFrame, numlines, 0, &(MainData->MaxSize))) {
			wxLogMessage(_("Unable to interpret coordinates."));
			throw DataError();
		}
		lFrame->toggleAbInitioVisibility();
	}
	if (Prefs->GetAutoBond())
		lFrame->SetBonds(Prefs, false, ProgressInd);
	//attempt to read in the atomic basis set
	if (Buffer->LocateKeyWord("ATOMIC BASIS SET", 16)) {
		ProgressInd->ChangeText("Reading atomic basis set");
		if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
		{ throw UserCancel();}
		try {
			MainData->ParseGAMESSBasisSet(Buffer);
			BasisSet * Basis = MainData->Basis;
			if (Basis) {	//generate the nuclear charge array
				if (Basis->NuclearCharge[0] == -1) {
					if (MainData->cFrame->NumAtoms == Basis->MapLength) {
						//for the purpose of the basis set, set the charge of mm atoms to 0
						for (long iatom=0; iatom<MainData->cFrame->NumAtoms; ++iatom) {
							if (! MainData->cFrame->Atoms[iatom].IsSIMOMMAtom() )
								Basis->NuclearCharge[iatom] = MainData->cFrame->Atoms[iatom].GetNuclearCharge();
							else
								Basis->NuclearCharge[iatom] = 0;
						}
					}
				}
			}
		}
		catch (MemoryError) {
			if (MainData->Basis) delete MainData->Basis;
			MainData->Basis = NULL;
			MessageAlert("Insufficient Memory to read the Basis Set.");
		}
		catch (std::bad_alloc) {
			if (MainData->Basis) delete MainData->Basis;
			MainData->Basis = NULL;
			MessageAlert("Insufficient Memory to read the Basis Set.");
		}
		catch (DataError) {
			MessageAlert("An error occured while reading the basis set, basis set skipped.");
			if (MainData->Basis) delete MainData->Basis;
			MainData->Basis = NULL;
		}
	}

	if (Buffer->LocateKeyWord("QM+MM Energy (Hartree):", 23)) {
		Buffer->GetLine(LineText);
			double FrameEnergy;
		sscanf(&(LineText[24]), "%lf", &FrameEnergy);
		lFrame->Energy = FrameEnergy;
	}

		//Read in normal modes, if present
	MainData->cFrame->ParseNormalModes(Buffer, ProgressInd, Prefs);

	return 1;
}
long MolDisplayWin::OpenGAMESSlog(BufferFile *Buffer, bool Append, long flip, float offset) {
	Frame *			lpFrame;
	long			test=0, numlines=0, OccupiedOrbCount=0, NumFragmentAtoms=0,
					NumOccAlpha=0, NumOccBeta=0, NumBetaUHFOrbs=0, LinePos,
					NumCoreOrbs, NumOpenOrbs, NumGVBPairs,
					ReadMP2Orbitals=1;
	wxFileOffset	EnergyPos, NextFinalPos, SavedPos, StartPos;
	float			*Occupancy = NULL;
	bool			KeyWordFound, SIMOMM=false;
	char			LineText[kMaxLineLength], token[kMaxLineLength];
	BufferFile *	DatBuffer=NULL;

	ProgressInd->ChangeText("Reading GAMESS log file...");
	Frame * lFrame = MainData->cFrame;

	EnergyPos = -1;
	NextFinalPos = -1;
		//First Skip over all of the input card lines, the lines are near the beginning of the file and one after another
	while (Buffer->LocateKeyWord("INPUT CARD>", 11, (Buffer->GetFilePos()+50000)))
		Buffer->SkipnLines(1);
	wxFileOffset HeaderEndPos = Buffer->GetFilePos();	//We don't care about anything earlier than this pos.
//	Buffer->LocateKeyWord("RUN TITLE", 9);	//find and skip over run title since
//	Buffer->SkipnLines(3);					//it can contain any arbitrary text
//	if (Buffer->LocateKeyWord("FINAL", 5)) {//locate initial energy since all options are before it
//		EnergyPos = Buffer->GetFilePos();
//		Buffer->SetFilePos(HeaderEndPos);
//	}
	std::vector<std::pair <std::string, int> > OptKeywords; //search tokens for optimizations
	OptKeywords.push_back(make_pair (std::string("BEGINNING GEOMETRY SEARCH POINT"), 0));
	OptKeywords.push_back(make_pair (std::string("1NSERCH"), 1));
	
	if (Append) {
		Buffer->LocateKeyWord("RUN TITLE", 9);	//find and skip over run title since
		Buffer->SkipnLines(3);					//it can contain any arbitrary text
		if (Buffer->LocateFinalEnergy()) {//locate initial energy since all options are before it
			EnergyPos = Buffer->GetFilePos();
			Buffer->SetFilePos(HeaderEndPos);
		}
		if (!MainData->InputOptions) throw MemoryError();
			//Determine the number of occupied alpha and beta orbitals
		if (Buffer->LocateKeyWord("NUMBER OF OCCUPIED ORBITALS (ALPHA)", 35, EnergyPos)) {
			Buffer->GetLine(LineText);
			LinePos = FindKeyWord(LineText, "=", 1) + 1;
			sscanf(&(LineText[LinePos]),"%ld", &NumOccAlpha);
		}
		if (Buffer->LocateKeyWord("NUMBER OF OCCUPIED ORBITALS (BETA )", 35, EnergyPos)) {
			Buffer->GetLine(LineText);
			LinePos = FindKeyWord(LineText, "=", 1) + 1;
			sscanf(&(LineText[LinePos]),"%ld", &NumOccBeta);
		}
		SavedPos = Buffer->GetFilePos();
			//Check for ECP/MCP type run which reduces the number of occupied orbitals
		if (Buffer->LocateKeyWord("CP RUN REMOVES", 14, EnergyPos)) {
			Buffer->GetLine(LineText);	//read in the number of electrons removed
			sscanf(&(LineText[15]),"%ld", &test);
			test /= 2;
			NumOccAlpha -= test;
			if (NumOccAlpha < 0) NumOccAlpha = 0;	//Oops!
			if (NumOccBeta) NumOccBeta -= test;
			if (NumOccBeta<0) NumOccBeta = 0;
			Buffer->SetFilePos(SavedPos);
		}
		if (MainData->InputOptions->Control->GetSCFType()==GAMESS_MCSCF) {	//Calculate the # of occupied MOs by parsing the DRT info
			if (Buffer->LocateKeyWord("GUGA DISTINCT ROW TABLE", 23, EnergyPos)) {
					long nfzc=0, ndoc=0, nmcc=0, naos=0, nbos=0, nalp=0, nval=0;
				if (Buffer->LocateKeyWord("NFZC=",5, EnergyPos)) {
					Buffer->GetLine(LineText);
					sscanf(&(LineText[6]), "%ld NDOC= %ld", &nfzc, &ndoc);
					Buffer->GetLine(LineText);
					sscanf(LineText, " NMCC= %ld NAOS= %ld", &nmcc, &naos);
					Buffer->GetLine(LineText);
					sscanf(LineText, " NBOS= %ld", &nbos);
					Buffer->GetLine(LineText);
					sscanf(LineText, " NALP= %ld", &nalp);
					Buffer->GetLine(LineText);
					sscanf(LineText, " NVAL= %ld", &nval);
					OccupiedOrbCount = nfzc + ndoc + nmcc + naos + nbos + nalp + nval;
				}
			}//GAMESS wasn't nice enough to punch out the # of occupied MO's
			if (!OccupiedOrbCount) OccupiedOrbCount = MainData->GetNumBasisFunctions();
			NumOccAlpha = OccupiedOrbCount;	//setup for localized orbs
		}
	//	test = MainData->InputOptions->Control->SetSCFType(&(LineText[LinePos]));
		if (MainData->InputOptions->Control->GetSCFType()==GAMESS_UHF)
			NumBetaUHFOrbs = NumOccBeta;	//Only seperate Beta spin orbs for UHF wavefunctions
		else NumOccBeta = 0;
		if (IRCRun==MainData->InputOptions->Control->GetRunType())
			return this->OpenGAMESSIRCLog(Buffer, flip, offset, NumOccAlpha, NumOccBeta, NumFragmentAtoms);
		if (DRCRun==MainData->InputOptions->Control->GetRunType())
			return this->OpenGAMESSDRC(Buffer, true, Append, flip,offset);
		else
			while (lFrame->NextFrame) lFrame = lFrame->NextFrame;
		if (MainData->InputOptions->Control->GetRunType() == GLOBOPRun)
			return OpenGAMESSGlobOpLog(Buffer, NumOccAlpha, NumOccBeta, NumFragmentAtoms);
	} else {
		MainData->InputOptions = new InputData;
		if (!MainData->InputOptions) throw MemoryError();
		enum {
			final=0,
			headerEnd,
			basisOptions,
			runTitle,
			pointGroup,
			QMMM,
			coords,
			EFRAG,
			basisSet,
			molCharge,
			multiplicity,
			occAlphaOrbs,
			occBetaOrbs,
			ECPoptions,
			gridDFT,
			gridFreeDFT,
			controlOptions,
			systemOptions,
			GVBOptions,
			abnormalTerm
		};
		std::vector<std::pair <std::string, int> > HeaderKeywords;
//		HeaderKeywords.push_back(make_pair (std::string("FINAL"), (int) final));
		HeaderKeywords.push_back(make_pair (std::string("DONE SETTING UP THE RUN"), (int) headerEnd));
		HeaderKeywords.push_back(make_pair (std::string("BASIS OPTIONS"), (int) basisOptions));
		HeaderKeywords.push_back(make_pair (std::string("RUN TITLE"), (int) runTitle));
		HeaderKeywords.push_back(make_pair (std::string("POINT GROUP"), (int) pointGroup));
		HeaderKeywords.push_back(make_pair (std::string("QMMM PROCEDURE IS ON"), (int) QMMM));
		HeaderKeywords.push_back(make_pair (std::string("COORDINATES (BOHR)"), (int) coords));
		HeaderKeywords.push_back(make_pair (std::string("READING $EFRAG GROUP"), (int) EFRAG));
		HeaderKeywords.push_back(make_pair (std::string("ATOMIC BASIS SET"), (int) basisSet));
		HeaderKeywords.push_back(make_pair (std::string("CHARGE OF MOLECULE"), (int) molCharge));
		HeaderKeywords.push_back(make_pair (std::string("MULTIPLICITY"), (int) multiplicity));
		HeaderKeywords.push_back(make_pair (std::string("NUMBER OF OCCUPIED ORBITALS (ALPHA)"), (int) occAlphaOrbs));
		HeaderKeywords.push_back(make_pair (std::string("NUMBER OF OCCUPIED ORBITALS (BETA )"), (int) occBetaOrbs));
		HeaderKeywords.push_back(make_pair (std::string("ECP POTENTIALS"), (int) ECPoptions));
		HeaderKeywords.push_back(make_pair (std::string("MODEL-POTENTIALS"), (int) ECPoptions));
		HeaderKeywords.push_back(make_pair (std::string("GRID-BASED DFT"), (int) gridDFT));
		HeaderKeywords.push_back(make_pair (std::string("MODEL-POTENTIALS"), (int) gridFreeDFT));
		HeaderKeywords.push_back(make_pair (std::string("CONTRL OPTIONS"), (int) controlOptions));
		HeaderKeywords.push_back(make_pair (std::string("JOB OPTIONS"), (int) controlOptions));
		HeaderKeywords.push_back(make_pair (std::string("SYSTEM OPTIONS"), (int) systemOptions));
		HeaderKeywords.push_back(make_pair (std::string("EXECUTION OF GAMESS TERMINATED -ABNORMALLY- AT"), (int) abnormalTerm));

		bool headerDone=false;
		bool controlOptionsFound = false;
		while (! Buffer->Eof() && ! headerDone) {
			if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
			{ throw UserCancel();}
			int kw;
			if (-1 < (kw = Buffer->LocateKeyWord(HeaderKeywords))) {
				wxFileOffset keywordPosition = Buffer->GetFilePos();
				switch (kw) {
					case final:
						headerDone = true;
						EnergyPos = Buffer->GetFilePos();
						break;
					case headerEnd:
						//Until recently this was printed out at the beginning of the header output - useless!
						//To account for that only consider this done if we've found the coordinates
						if (lFrame->NumAtoms > 0) headerDone = true;
						Buffer->SkipnLines(1);
						break;
					case basisOptions:
	//					KeyWordFound = Buffer->LocateKeyWord("BASIS OPTIONS", 13, EnergyPos);
	//					if (KeyWordFound) {	//Read in the basis set information (for user reference)
							ProgressInd->ChangeText("Reading Basis information");
							if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
							{ throw UserCancel();}
							MainData->ReadBasisOptions(Buffer);
	//					}
						break;
					case runTitle:
		//				if (Buffer->LocateKeyWord("RUN TITLE", 9, EnergyPos)) {	//Read the 1 line run label
							Buffer->SkipnLines(2);
							Buffer->GetLine(LineText);
							MainData->InputOptions->Data->SetTitle(LineText, strlen(LineText));
		//				}
						break;
					case pointGroup:
//						if (Buffer->LocateKeyWord("POINT GROUP", 11, EnergyPos)) {	//Setup molecular point group
							Buffer->GetLine(LineText);
							LinePos = FindKeyWord(LineText, "MOLECULE IS", 11);
							if (LinePos > -1) {
								sscanf(&(LineText[LinePos+12]), "%s", token);
								MainData->InputOptions->Data->SetPointGroup(token);
							}
							
							Buffer->GetLine(LineText);
							LinePos = FindKeyWord(LineText, "AXIS IS", 7);
							if (LinePos > -1) {
								sscanf(&(LineText[LinePos+8]),"%ld", &test);
								MainData->InputOptions->Data->SetPointGroupOrder(test);
							}
//						}
						break;
					case QMMM:
//						Buffer->SetFilePos(HeaderEndPos);
//						if (Buffer->LocateKeyWord("QMMM PROCEDURE IS ON", 20, EnergyPos)) {
							//If this is a simmom type run then switch to that routine since the format is
							//significantly different
							//			return (ParseSIMMOMLogFile(Buffer, EnergyPos));
							SIMOMM=true;
							ProgressInd->ChangeText("Reading Tinker Coordinates");
							if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
							{throw UserCancel();}
							MainData->ParseTinkerCoordinates(Buffer);
							//The ab initio atoms will be added below
//						}
						break;
					case coords:
						//locate the input set of coordinates (present in every file)
	//					if (Buffer->LocateKeyWord("COORDINATES (BOHR)", 16, EnergyPos)) {	//first normal (ab initio) atoms
							ProgressInd->ChangeText("Reading Coordinates");
							if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
							{ AbortOpen("File open canceled by user"); return 0;}
							
							Buffer->SkipnLines(2);
							StartPos = Buffer->GetFilePos();
							test = Buffer->FindBlankLine() - StartPos;
							numlines = Buffer->GetNumLines(test);
							if (numlines > 0) {
								if (!MainData->SetupFrameMemory(numlines+lFrame->NumAtoms, 0)) throw MemoryError();
							} else {
								wxLogMessage(_("Unable to locate coordinates in the file."));
								throw DataError();
							}
							if (!ParseGLogLine(Buffer, lFrame, numlines, 0, &(MainData->MaxSize))) {
								wxLogMessage(_("Unable to interpret coordinates."));
								throw DataError();
							}
	//					}
						break;
					case EFRAG:
//						StartPos = Buffer->GetFilePos();		//next look for fragments
//						if (Buffer->LocateKeyWord("READING $EFRAG GROUP", 20, EnergyPos)) {	//ughh fragments!
							NumFragmentAtoms = MainData->ReadInitialFragmentCoords(Buffer);
//							Buffer->SetFilePos(StartPos);
//						}
						break;
					case basisSet:
						//Read in the atomic basis set (if present) and keep it in case we find MO vectors later
						//if it is not located and read in correctly MO vectors will also be skipped
	//					if (Buffer->LocateKeyWord("ATOMIC BASIS SET", 16, EnergyPos)) {
					{
							ProgressInd->ChangeText("Reading atomic basis set");
							if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
							{ throw UserCancel();}
							try {
								MainData->ParseGAMESSBasisSet(Buffer);
								BasisSet * Basis = MainData->Basis;
								if (Basis && SIMOMM) {	//generate the nuclear charge array
									if (Basis->NuclearCharge[0] == -1) {
										if (MainData->cFrame->NumAtoms == Basis->MapLength) {
											//for the purpose of the basis set, set the charge of mm atoms to 0
											for (long iatom=0; iatom<MainData->cFrame->NumAtoms; ++iatom) {
												if (! MainData->cFrame->Atoms[iatom].IsSIMOMMAtom() )
													Basis->NuclearCharge[iatom] = MainData->cFrame->Atoms[iatom].GetNuclearCharge();
												else
													Basis->NuclearCharge[iatom] = 0;
											}
										}
									}
								}
							}
							catch (MemoryError) {
								if (MainData->Basis) delete MainData->Basis;
								MainData->Basis = NULL;
								MessageAlert("Insufficient Memory to read the Basis Set.");
							}
							catch (std::bad_alloc) {
								if (MainData->Basis) delete MainData->Basis;
								MainData->Basis = NULL;
								MessageAlert("Insufficient Memory to read the Basis Set.");
							}
							catch (DataError) {
								MessageAlert("An error occured while reading the basis set, basis set skipped.");
								if (MainData->Basis) delete MainData->Basis;
								MainData->Basis = NULL;
							}
						}
						break;
					case molCharge:
						//Read in charge and multiplicity
//						if (Buffer->LocateKeyWord("CHARGE OF MOLECULE", 18, EnergyPos)) {
							Buffer->GetLine(LineText);
							LinePos = FindKeyWord(LineText, "=", 1) + 1;
							long charge;
							sscanf(&(LineText[LinePos]),"%ld", &charge);
							MainData->InputOptions->Control->SetCharge(charge);
//						}
						break;
					case multiplicity:
						//There are now two forms of this, "SPIN MULTIPLICITY" and "STATE MULTIPLICITY"
						//however, either should appear soon after the CHARGE.
//						if (Buffer->LocateKeyWord("MULTIPLICITY", 12, Buffer->GetFilePos()+100)) {
							Buffer->GetLine(LineText);
							LinePos = FindKeyWord(LineText, "=", 1) + 1;
							long Multiplicity;
							sscanf(&(LineText[LinePos]),"%ld", &Multiplicity);
							MainData->InputOptions->Control->SetMultiplicity(Multiplicity);
//						}
						break;
					case occAlphaOrbs:
						//Determine the number of occupied alpha and beta orbitals
//						if (Buffer->LocateKeyWord("NUMBER OF OCCUPIED ORBITALS (ALPHA)", 35, EnergyPos)) {
							Buffer->GetLine(LineText);
							LinePos = FindKeyWord(LineText, "=", 1) + 1;
							sscanf(&(LineText[LinePos]),"%ld", &NumOccAlpha);
//						}
						break;
					case occBetaOrbs:
//						if (Buffer->LocateKeyWord("NUMBER OF OCCUPIED ORBITALS (BETA )", 35, EnergyPos)) {
							Buffer->GetLine(LineText);
							LinePos = FindKeyWord(LineText, "=", 1) + 1;
							sscanf(&(LineText[LinePos]),"%ld", &NumOccBeta);
//						}
						break;
					case ECPoptions:
//						SavedPos = Buffer->GetFilePos();
						//Check for ECP type run which reduces the number of occupied orbitals
//						if (Buffer->LocateKeyWord("ECP POTENTIALS", 14, EnergyPos)||
//							Buffer->LocateKeyWord("MODEL-POTENTIALS", 16, EnergyPos)) {
							test = MainData->ParseECPotentials(Buffer);
							test /= 2;
							NumOccAlpha -= test;
							if (NumOccAlpha < 0) NumOccAlpha = 0;	//Oops!
							if (NumOccBeta) NumOccBeta -= test;
							if (NumOccBeta<0) NumOccBeta = 0;
//							Buffer->SetFilePos(SavedPos);
//						}
						break;
					case gridDFT:
						//DFT options
	//					if (Buffer->LocateKeyWord("GRID-BASED DFT", 14, EnergyPos)) 
					{
							Buffer->SkipnLines(2);
							Buffer->GetLine(LineText);
							char DFTTYP[kMaxLineLength];
							if (ReadStringKeyword(LineText, "DFTTYP", DFTTYP)) {
								MainData->InputOptions->DFT.SetFunctional(DFTTYP);
								if (MainData->InputOptions->DFT.GetFunctional() > 0)
									MainData->InputOptions->Control->UseDFT(true);
								else
									wxLogMessage(_("Unknown DFTTYP detected, ignored."));
							}
					}
							break;
					case gridFreeDFT:
					//	 else if (Buffer->LocateKeyWord("GRID-FREE DFT", 13, EnergyPos)) 
					{
							Buffer->SkipnLines(2);
							Buffer->GetLine(LineText);
							char DFTTYP[kMaxLineLength];
							MainData->InputOptions->DFT.SetMethodGrid(false);
							if (ReadStringKeyword(LineText, "DFTTYP", DFTTYP)) {
								MainData->InputOptions->DFT.SetFunctional(DFTTYP);
								if (MainData->InputOptions->DFT.GetFunctional() > 0)
									MainData->InputOptions->Control->UseDFT(true);
								else
									wxLogMessage(_("Unknown DFTTYP detected, ignored."));
							}
						}
						break;
					case controlOptions:
						//Now read other Control options
//						if (!Buffer->LocateKeyWord("CONTRL OPTIONS", 14, EnergyPos)) {
//							// $CONTRL OPTIONS used to be JOB OPTIONS so check for it too
//							if (!Buffer->LocateKeyWord("JOB OPTIONS", 11, EnergyPos)) {
//								//Unable to locate the job options, perhaps the run terminated abnormally?
//								if (Buffer->LocateKeyWord("EXECUTION OF GAMESS TERMINATED -ABNORMALLY- AT", 46))
//									wxLogMessage(_("GAMESS terminated abnormally. Please check the log file for details."));
//								wxLogMessage(_("Unable to locate run options."));
//								return 0;
//							}
//						}
						MainData->ReadControlOptions(Buffer);
						controlOptionsFound = true;
						if (MainData->InputOptions->Control->GetSCFType()==GAMESS_GVB)
							HeaderKeywords.push_back(make_pair (std::string("ROHF-GVB INPUT PARAMETERS"), (int) GVBOptions));
						break;
					case systemOptions:
						//System group is immediately after the control group
			//			if (Buffer->LocateKeyWord("SYSTEM OPTIONS", 14, EnergyPos)) {
							MainData->InputOptions->System->ReadSystemOptions(Buffer);
			//			}
						break;
					case GVBOptions:
						Buffer->SkipnLines(3);
						Buffer->GetLine(LineText);
						sscanf(LineText, " NORB   =%ld NCO    =%ld", &test, &NumCoreOrbs);
						MainData->InputOptions->SCF->SetGVBNumCoreOrbs(NumCoreOrbs);
						NumOccAlpha = test;
						Buffer->GetLine(LineText);
						sscanf(LineText, " NPAIR  =%ld NSETO  =%ld", &NumGVBPairs, &NumOpenOrbs);
						MainData->InputOptions->SCF->SetGVBNumPairs(NumGVBPairs);
						if (NumOpenOrbs>0) {
							//If there are Open Shells we need to parse the degeneracy of each shell
							//This is a space separated list starting with NO =
							//In theory it could be more than a line, but that is highly unlikely
							Buffer->GetLine(LineText);
							int nchar = 0;
							if (sscanf(LineText, " NO   =%n", &nchar) == 0) {
								for (int ishell=0; ishell<NumOpenOrbs; ishell++) {
									int nchar2=0;
									long shellDeg;
									if (sscanf(&(LineText[nchar]), "%ld%n", &shellDeg, &nchar2) == 1) {
										nchar += nchar2;
										MainData->InputOptions->SCF->AddGVBOpenShellDeg(shellDeg);
									} else
										break;
								}
							}
							//Set this after reading in NO so no need to clear the NO array
							MainData->InputOptions->SCF->SetGVBNumOpenShells(NumOpenOrbs);
						}
							Occupancy = new float[MainData->GetNumBasisFunctions()];
							if (!Occupancy) throw MemoryError();
							for (test=0; test<MainData->GetNumBasisFunctions(); ++test) Occupancy[test] = 0.0f;
							for (test=0; test<NumCoreOrbs; ++test) Occupancy[test] = 2.0f;
							if (NumOpenOrbs > 0) {	//Parse the occupancies of the open shells
								if (Buffer->LocateKeyWord("F VECTOR (OCCUPANCIES)", 23)) {
									//Next find the first row indicating the doubly occupied orbitals
									Buffer->SkipnLines(1);
									bool foundPos=false;
									int lineCount=0;
									while (!foundPos) {
										int test;
										Buffer->GetLine(LineText);
										sscanf(LineText, "%d", &test);
										if (test == 1) {	//If there are no cores then the first line is needed below.
											if (NumCoreOrbs<=0) {
												Buffer->BackupnLines(1);
											}
											foundPos = true;
										}
										lineCount++;
										if (lineCount > 10) {
											wxLogMessage(_("Warning: Unable to parse GVB F Vector Occupancies."));
											break;
										}
									}
									if (foundPos) {
										int iorb = NumCoreOrbs;
										for (int nShell=0; nShell<NumOpenOrbs; ++nShell) {
											Buffer->GetLine(LineText);
											float occTemp;
											sscanf(LineText, "%ld %f", &test, &occTemp);
											for (int jj=0; jj<MainData->InputOptions->SCF->GetGVBOpenShellDeg(nShell); jj++) {
												Occupancy[iorb] = occTemp * 2;	//occupancies are listed as % of orbital filled
												iorb++;
											}
										}
									}
								}
							}
	//					}
						break;
					case abnormalTerm:
					//		if (Buffer->LocateKeyWord("EXECUTION OF GAMESS TERMINATED -ABNORMALLY- AT", 46))
						wxLogMessage(_("GAMESS terminated abnormally. Please check the log file for details."));
						return 0;
						break;
				}
					//If still positioned at the keyword, must advance or there will be a loop.
				if (keywordPosition == Buffer->GetFilePos()) Buffer->SkipnLines(1);
			} else break;
		}
		if (lFrame->NumAtoms <= 0) {	//initial coordinates not found! Abort!
			wxLogMessage(_("Unable to locate coordinates in the file."));
			throw DataError();
		}
		if (Prefs->GetAutoBond())
			lFrame->SetBonds(Prefs, false, ProgressInd);
		
		if (!controlOptionsFound) wxLogMessage(_("Unable to locate run options. Attempting to continue"));
		
			//Now that the $control options are read in normalize the basis set (normf and normp are needed)
		if (MainData->Basis) MainData->Basis->Normalize(MainData->InputOptions->Control->GetNormP(),
			MainData->InputOptions->Control->GetNormF());
			//plot the rms gradient by default for run types: gradient, optimize, and sadpoint
		if ((MainData->InputOptions->Control->GetRunType()==4)||
	//		(MainData->InputOptions->Control->GetRunType()==2)|| could add gradient, but output is different
			(MainData->InputOptions->Control->GetRunType()==6)) {
			GraphOptions * lPOpts = Prefs->GetGraphOptions();
			EnergyOptions * lEOpts = Prefs->GetEnergyOptions();
			lEOpts->SetPlotOther(true);
			lPOpts->SetPlotRMSGradient(true);
		}
		if (MainData->InputOptions->Control->GetSCFType()==GAMESS_UHF)
				//Only seperate Beta spin orbs for UHF wavefunctions
			NumBetaUHFOrbs = MainData->GetNumBasisFunctions();
		else if (MainData->InputOptions->Control->GetSCFType()!=GAMESS_ROHF)
			NumOccBeta = 0;

		if (MainData->InputOptions->Control->GetRunType() == GLOBOPRun)
			return OpenGAMESSGlobOpLog(Buffer, NumOccAlpha, NumOccBeta, NumFragmentAtoms);

		HeaderEndPos = Buffer->GetFilePos();
		if (Buffer->LocateFinalEnergy()) {//locate initial energy
			EnergyPos = Buffer->GetFilePos();
			Buffer->SetFilePos(HeaderEndPos);
		}

		if (MainData->InputOptions->Control->GetSCFType()==GAMESS_MCSCF) {	//Calculate the # of occupied MOs by parsing the DRT info
			wxFileOffset filePos = Buffer->GetFilePos();
			if (Buffer->LocateKeyWord("GUGA DISTINCT ROW TABLE", 23, EnergyPos)) {
					long nfzc=0, ndoc=0, nmcc=0, naos=0, nbos=0, nalp=0, nval=0;
				if (Buffer->LocateKeyWord("NFZC=",5)) {
					Buffer->GetLine(LineText);
					sscanf(&(LineText[6]), "%ld NDOC= %ld", &nfzc, &ndoc);
					Buffer->GetLine(LineText);
					sscanf(LineText, " NMCC= %ld NAOS= %ld", &nmcc, &naos);
					Buffer->GetLine(LineText);
					sscanf(LineText, " NBOS= %ld", &nbos);
					Buffer->GetLine(LineText);
					sscanf(LineText, " NALP= %ld", &nalp);
					Buffer->GetLine(LineText);
					sscanf(LineText, " NVAL= %ld", &nval);
					OccupiedOrbCount = nfzc + ndoc + nmcc + naos + nbos + nalp + nval;
				}
			} else if ((Buffer->LocateKeyWord("AMES LABORATORY DETERMINANTAL FULL CI", 37,
					EnergyPos))||(Buffer->LocateKeyWord("DIRECT DETERMINANT ORMAS-CI INPUT SORTER", 40, EnergyPos))) {
				if (Buffer->LocateKeyWord("NUMBER OF OCCUPIED ORBITALS",27,800+Buffer->GetFilePos())) {
					Buffer->GetLine(LineText);
					sscanf(&(LineText[30]), " = %ld", &OccupiedOrbCount);
				}
			}//GAMESS wasn't nice enough to punch out the # of occupied MO's
			if (!OccupiedOrbCount) OccupiedOrbCount = MainData->GetNumBasisFunctions();
			NumOccAlpha = OccupiedOrbCount;	//setup for localized orbs
			Buffer->SetFilePos(filePos);	//reset the file position
		}
		//Could read GUESS options here...
		//Look for the initial guess orbitals in case the user requested them
		if (MainData->GetNumBasisFunctions() > 0) {
			if (Buffer->LocateKeyWord("      INITIAL GUESS ORBITALS", 28, EnergyPos)) {
				lFrame->ParseGAMESSGuessVectors(Buffer, MainData->GetNumBasisFunctions(),
												(TypeOfWavefunction)MainData->InputOptions->Control->GetSCFType(),
												ProgressInd);
			}
		}

	// Switch to DRC routine if neccessary
		if (MainData->InputOptions->Control->GetRunType() == DRCRun) {
			lFrame->NumAtoms = lFrame->NumBonds = 0;
			return this->OpenGAMESSDRC(Buffer, true, Append, flip,offset);
		} else if (MainData->InputOptions->Control->GetRunType() == IRCRun) {
			//Confirm that the first energy is before the start of the IRC
			if (Buffer->LocateKeyWord("JUMPING OFF SADDLE POINT ALONG THE IMAGINARY NORMAL MODE", 55)) {
				wxFileOffset test = Buffer->GetFilePos();
				if (EnergyPos > test) {
					return OpenGAMESSIRCLog(Buffer, flip, offset, NumOccAlpha, NumOccBeta, NumFragmentAtoms);
				}
			}
		} else if (MainData->InputOptions->Control->GetRunType() == VSCFRun) {
			//Read in normal modes, if present
			if (Buffer->LocateKeyWord("NORMAL COORDINATE ANALYSIS", 25, EnergyPos)) 
				MainData->cFrame->ParseNormalModes(Buffer, ProgressInd, Prefs);
		}
		
		if (EnergyPos > 0) {
			Buffer->SetFilePos(EnergyPos);
			Buffer->GetLine(LineText);
			LinePos = FindKeyWord(LineText, "ENERGY", 6);
			if (LinePos > -1) {
				int k = sscanf(&(LineText[LinePos+10]), "%lf", &(lFrame->Energy));
				if (k == 0) {	//Crappy fragment output is different
					k = FindKeyWord(&(LineText[LinePos]), "=", 1);
					if (LinePos > -1)
						sscanf(&(LineText[LinePos+k+1]), "%lf", &(lFrame->Energy));
				}
				if (MainData->InputOptions->Control->GetMPLevel()) {	//MP2 run so look for MP2 energy
					StartPos = Buffer->GetFilePos();
					if (Buffer->LocateKeyWord("E(MP2)=", 7)) {
						double mpE;
						Buffer->GetLine(LineText);
						sscanf(&(LineText[8]), "%lf", &(mpE));
						lFrame->Energies.push_back(EnergyValue(mpE, PT2Energy));
					}
					Buffer->SetFilePos(StartPos);
				}
			}
				//Attempt to read in orbitals for first geometry
			SavedPos = Buffer->GetFilePos();
			NextFinalPos = -1;
			if (Buffer->LocateFinalEnergy()) NextFinalPos = Buffer->GetFilePos();
			Buffer->SetFilePos(SavedPos);
			if (MainData->Basis) {
				try {
					if (MainData->InputOptions->Control->GetSCFType()==GAMESS_MCSCF) {
							bool ReadOrbs = false;
						if (Buffer->LocateKeyWord("MCSCF NATURAL ORBITALS", 22, NextFinalPos)) {
							if ((NextFinalPos<0)||(NextFinalPos>Buffer->GetFilePos()))
								ReadOrbs = true;
							else Buffer->SetFilePos(SavedPos);
						} else if (Buffer->LocateKeyWord("MCSCF OPTIMIZED ORBITALS", 24, NextFinalPos)) {
							if ((NextFinalPos<0)||(NextFinalPos>Buffer->GetFilePos()))
								ReadOrbs = true;
							else Buffer->SetFilePos(SavedPos);
						} else if (Buffer->LocateKeyWord("-MCHF- NATURAL ORBITALS", 23, NextFinalPos)) {
							if ((NextFinalPos<0)||(NextFinalPos>Buffer->GetFilePos()))
								ReadOrbs = true;
							else Buffer->SetFilePos(SavedPos);
						} else if (Buffer->LocateKeyWord("-MCHF- OPTIMIZED ORBITALS", 25, NextFinalPos)) {
							if ((NextFinalPos<0)||(NextFinalPos>Buffer->GetFilePos()))
								ReadOrbs = true;
							else Buffer->SetFilePos(SavedPos);
						} else if (Buffer->LocateKeyWord("MOLECULAR ORBITALS", 18, NextFinalPos)) {
							Buffer->SetFilePos(Buffer->FindBlankLine());
							Buffer->SkipnLines(1);
							lFrame->ParseGAMESSEigenVectors(Buffer, MainData->GetNumBasisFunctions(),
								MainData->GetNumBasisFunctions(), 0, NumOccAlpha, NumOccBeta,
								(TypeOfWavefunction)(MainData->InputOptions->Control->GetSCFType()), ProgressInd);
		//					lFrame->ExchangeEigenVectors();
						}
						if (ReadOrbs)
							lFrame->ParseGAMESSMCSCFVectors(Buffer, MainData->GetNumBasisFunctions(),
								OccupiedOrbCount, ProgressInd);
					} else {
						if (MainData->InputOptions->Control->GetSCFType() == GAMESS_GVB) {
							if (Buffer->LocateKeyWord("PAIR INFORMATION", 16, NextFinalPos)) {
								ReadGVBOccupancy(Buffer, NumGVBPairs, MainData->GetNumBasisFunctions(),
									Occupancy);
							}
						}
//						if (MainData->InputOptions->Control->GetCIType() == CI_CIS) {
//							//CIS output includes the EIGENVECTORS token
//							//I think the regular EIGENVECTORS would be before the CIS line???
//							long bp = Buffer->GetFilePos();
//							KeyWordFound = Buffer->LocateKeyWord("EIGENVECTORS", 12, NextFinalPos);
//							Buffer->BackupnLines(1);
//							if (Buffer->LocateKeyWord("CIS EIGENVALUES", 14, 80+Buffer->GetFilePos())) {
//								Buffer->SkipnLines(1);
//								KeyWordFound = Buffer->LocateKeyWord("EIGENVECTORS", 12, NextFinalPos);
//								if (!KeyWordFound) {
//									Buffer->SetFilePos(bp);
//									KeyWordFound = Buffer->LocateKeyWord("MOLECULAR ORBITALS", 18, NextFinalPos);
//									Buffer->BackupnLines(1);
//									if (Buffer->LocateKeyWord("# MOLECULAR ORBITALS =", 22)) {
//										Buffer->SkipnLines(1);
//										KeyWordFound = Buffer->LocateKeyWord("MOLECULAR ORBITALS", 18, NextFinalPos);
//										if (!KeyWordFound) Buffer->SetFilePos(bp);
//									}
//								}
//							}
//						} else {
							KeyWordFound = Buffer->LocateKeyWord("   EIGENVECTORS", 15, NextFinalPos);
							if (!KeyWordFound)
								KeyWordFound = Buffer->LocateKeyWord("MOLECULAR ORBITALS", 18, NextFinalPos);
//						}
						if (KeyWordFound) {
								//skip over the line to the start of the orbital blocks
								//There is always a blank line before the orbs, but there may a '---' divider line after the key line just found
							if ((NextFinalPos<0)||(NextFinalPos>Buffer->GetFilePos())) {
								Buffer->SetFilePos(Buffer->FindBlankLine());
								Buffer->SkipnLines(1);
								ProgressInd->ChangeText("Reading eigenvectors");
								if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
									{ AbortOpen("File open canceled by user"); return 0;}
								OrbitalRec * OrbSet = lFrame->ParseGAMESSEigenVectors(Buffer, MainData->GetNumBasisFunctions(),
									MainData->GetNumBasisFunctions(), NumBetaUHFOrbs, NumOccAlpha, NumOccBeta,
									(TypeOfWavefunction)(MainData->InputOptions->Control->GetSCFType()), ProgressInd);
								if ((MainData->InputOptions->Control->GetSCFType() == GAMESS_GVB)&&(OrbSet!=NULL)) {
									//Add the Occupancy vector for GVB wavefunctions
									OrbSet->SetOccupancy(Occupancy, MainData->GetNumBasisFunctions());
								}
							} else Buffer->SetFilePos(SavedPos);
						}
					}
					if (MainData->InputOptions->Control->GetSCFType() == GAMESS_GVB) {
						KeyWordFound = Buffer->LocateKeyWord("GI ORBITALS", 11, NextFinalPos);
						if (KeyWordFound && ((NextFinalPos<0)||(NextFinalPos>Buffer->GetFilePos()))) {
							Buffer->SkipnLines(4);
							lFrame->ParseGVBGIOrbitals(Buffer, MainData->GetNumBasisFunctions(), NumGVBPairs, ProgressInd);
						}
					} else if (MainData->InputOptions->Control->GetSCFType() == GAMESS_UHF) {
						if (Buffer->LocateKeyWord("UHF NATURAL ORBITALS AND OCCUPATION NUMBERS",
												  43, NextFinalPos)) {
							lFrame->ParseUHFNOs(Buffer, MainData->GetNumBasisFunctions(), ProgressInd);
						}
					}
					//			if (MainData->InputOptions->Control->GetSCFType() == GAMESS_TDDFT) {
					//I don't have the right control keywords yet. Possibilities are TDDFT=EXCITE in 
					//$CONTRL or TDPRP=.t. in $TDDFT
					if (Buffer->LocateKeyWord("TDDFT NATURAL ORBITALS",
											  23, NextFinalPos)) {
						lFrame->ParseTDDFTNOs(Buffer, MainData->GetNumBasisFunctions(), ProgressInd);
					}
					//			}
					if (MainData->InputOptions->Control->GetMPLevel()) {
						if (Buffer->LocateKeyWord("MP2 NATURAL ORBITAL OCCUPATION NUMBERS",
							38, NextFinalPos) && ReadMP2Orbitals) {
							lFrame->ReadMP2Vectors(Buffer, DatBuffer, MainData->GetNumBasisFunctions(),
								ProgressInd, &ReadMP2Orbitals);
						}
					}
					if (MainData->InputOptions->Control->GetCCType()) {
						if (Buffer->LocateKeyWord("EOM-CC NATURAL ORBITALS",
												  23, NextFinalPos)) {
							lFrame->ParseGAMESSEOM_CC_Vectors(Buffer, MainData->GetNumBasisFunctions(), ProgressInd);
						}
					}
					if (MainData->InputOptions->Control->GetCIType()) {	//Look for CI Natural orbitals
						//loop to add multiple CI NOs, one set for each eigenstate.
						//This would be the place to read in each eigenstate's energy
						if (MainData->InputOptions->Control->GetCIType() == CI_CIS) {
								//The spaces are important since the phrase appears elsewhere....
							if (Buffer->LocateKeyWord("      CIS NATURAL ORBITALS",
													  26, NextFinalPos)) {
								lFrame->ParseGAMESSCIVectors(Buffer, MainData->GetNumBasisFunctions(), ProgressInd);
							}
						} else {
							while (Buffer->LocateKeyWord("NATURAL ORBITALS IN ATOMIC ORBITAL BASIS",
													  40, NextFinalPos)) {
								lFrame->ParseGAMESSCIVectors(Buffer, MainData->GetNumBasisFunctions(), ProgressInd);
							}
						}
					}
				}
				catch (std::bad_alloc) {
					MessageAlert("Insufficient memory to read in eigenvectors.");
				}
				catch (MemoryError) {
					MessageAlert("Insufficient memory to read in eigenvectors.");
				}
				catch (DataError) {
					MessageAlert("Error reading eigenvectors, orbitals skipped.");
				}
			}
			//MRMP2 - Only energy is supported
			if ((MainData->InputOptions->Control->GetSCFType()==GAMESS_MCSCF)&&
				(MainData->InputOptions->Control->GetMPLevel())) {	//MP2 run so look for MP2 energy
				if (Buffer->LocateKeyWord("TOTAL MRPT2, E(MP2) 0TH + 1ST + 2ND ORDER ENERGY", 48)) {
					double mpE;
					Buffer->GetLine(LineText);
					sscanf(&(LineText[51]), "%lf", &(mpE));
					//This should be changed to MRMP2Energy, but that isn't recognized elsewhere currently
					lFrame->Energies.push_back(EnergyValue(mpE, PT2Energy));
				}
			}
			if (SIMOMM) {	//See if we have MM energies and new coords
				//could also grab the MM energy here
				if (Buffer->LocateKeyWord("QM+MM Energy (Hartree):", 23)) {
					Buffer->GetLine(LineText);
					double FrameEnergy;
					sscanf(&(LineText[24]), "%lf", &FrameEnergy);
					lFrame->Energy = FrameEnergy;
				}
				//now update the coordinates with the new MM list
				Frame * preserve = MainData->cFrame;
				lFrame = MainData->AddFrame(MainData->cFrame->NumAtoms,0);
				MainData->ParseTinkerCoordinates(Buffer);
				if (MainData->cFrame->NumAtoms > 0) {
					//update the coordinates in the first frame
					for (int iatom=0; iatom<MainData->cFrame->NumAtoms; ++iatom) {
						if (MainData->cFrame->Atoms[iatom].IsSIMOMMAtom()) {
							preserve->Atoms[iatom].Position.x = MainData->cFrame->Atoms[iatom].Position.x;
							preserve->Atoms[iatom].Position.y = MainData->cFrame->Atoms[iatom].Position.y;
							preserve->Atoms[iatom].Position.z = MainData->cFrame->Atoms[iatom].Position.z;
						}
					}
				}
				// delete off the temporary frame
				MainData->DeleteFrame();
				lFrame = preserve;
			}
				//look for gradient data
			if (Buffer->LocateKeyWord(OptKeywords) >= 0) {
				KeyWordFound = true;
				NextFinalPos = Buffer->GetFilePos();
				Buffer->SetFilePos(EnergyPos);
			}
			lFrame->ReadGradient(Buffer, NextFinalPos);
		} else if (NumFragmentAtoms > 0) {
			if (Buffer->LocateKeyWord("TOT. INTER. ENERGY", 18, NextFinalPos)) {
				Buffer->GetLine(LineText);
				LinePos = FindKeyWord(LineText, "=", 1);
				if (LinePos > -1) {
					sscanf(&(LineText[LinePos+1]), "%lf", &(lFrame->Energy));
				}
			}
		} else if (MainData->InputOptions->Control->GetCIType()) {
			// If this is just a CI grab CI energy and orbitals
			if (Buffer->LocateKeyWord("CI EIGENSTATE", 13)) {
				Buffer->GetLine(LineText);
				sscanf(&(LineText[35]), "%lf", &(lFrame->Energy));
				while (Buffer->LocateKeyWord("NATURAL ORBITALS IN ATOMIC ORBITAL BASIS",
											 40, NextFinalPos)) {
					lFrame->ParseGAMESSCIVectors(Buffer, MainData->GetNumBasisFunctions(), ProgressInd);
				}
			}
		}
	// Switch to IRC routine if neccessary
		if (MainData->InputOptions->Control->GetRunType() == IRCRun) {
			//If this is an MP2 run need to copy the MP2 energy to the total energy as the IRC
			//parsing routine only finds the single total energy
			if (MainData->InputOptions->Control->GetMPLevel()) {
				lFrame->SetEnergy(lFrame->GetEnergy(PT2Energy));
			}
			return this->OpenGAMESSIRCLog(Buffer, flip, offset, NumOccAlpha, NumOccBeta, NumFragmentAtoms);
		}
	}
	KeyWordFound = true;
//	if (SIMOMM) KeyWordFound = false;	//only grab a single geometry for SIMOMM
	double	FrameEnergy, MP2FrameEnergy;
//	bool NewStyleGeometryKeyWord=true;
//	char GeoSearchToken[] = "BEGINNING GEOMETRY SEARCH POINT";
//	if (!(Buffer->LocateKeyWord(GeoSearchToken, 31))) {
//		NewStyleGeometryKeyWord = false;
//		strcpy(GeoSearchToken, "1NSERCH");
//	}
	while (KeyWordFound) {
		if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
			{ throw UserCancel();}
		KeyWordFound = false;
			//Advance to the start of the next geometry step BEGINNING GEOMETRY SEARCH POINT
			//If there isn't one then we are done with the geometries
		if (Buffer->LocateKeyWord(OptKeywords) < 0) break;
		StartPos = Buffer->GetFilePos();
		if (Buffer->LocateFinalEnergy()) {	//Search for final and energy on the same line
			Buffer->GetLine(LineText);				//since final also appears in LMO calcs.
			LinePos = FindKeyWord(LineText, "ENERGY", 6);
			if (LinePos > -1) {
				EnergyPos = Buffer->GetFilePos();
				KeyWordFound = true;
				int k = sscanf(&(LineText[LinePos+10]), "%lf", &FrameEnergy);
				if (k == 0) {	//Crappy fragment output is different
					k = FindKeyWord(&(LineText[LinePos]), "=", 1);
					if (LinePos > -1)
						sscanf(&(LineText[LinePos+k+1]), "%lf", &FrameEnergy);
				}
				if (MainData->InputOptions->Control->GetMPLevel()==2) {	//MP2 run so look for MP2 energy
					wxFileOffset temp = Buffer->GetFilePos();
					if (Buffer->LocateKeyWord("E(MP2)=", 7)) {
						Buffer->GetLine(LineText);
						sscanf(&(LineText[8]), "%lf", &MP2FrameEnergy);
					}
					Buffer->SetFilePos(temp);
				} else MP2FrameEnergy = 0.0;
			} else Buffer->SetFilePos(StartPos);
		}
		if (!KeyWordFound && (NumFragmentAtoms > 0)) { //Look for old style fragment output
			if (Buffer->LocateKeyWord("TOT. INTER. ENERGY", 18)) {
				Buffer->GetLine(LineText);
				LinePos = FindKeyWord(LineText, "=", 1);
				if (LinePos > -1) {
					sscanf(&(LineText[LinePos+1]), "%lf", &FrameEnergy);
					KeyWordFound = true;
					EnergyPos = Buffer->GetFilePos();
				}
			}
		}
		if (!KeyWordFound) {	//This is a last resort in case normal GAMESS energies are missing
			if (MainData->InputOptions->Control->GetRunType() == OptimizeRun) {
				if (Buffer->LocateKeyWord("  NSERCH=", 9)) {
					Buffer->GetLine(LineText);
					LinePos = FindKeyWord(LineText, "ENERGY=", 7);
					sscanf(&(LineText[LinePos+7]), "%lf", &FrameEnergy);
					KeyWordFound = true;
					EnergyPos = Buffer->GetFilePos();
				}
			}
		}

		if (KeyWordFound) {
				//Let the user know we are still making progress
			sprintf(LineText, "Reading Coordinates: frame %ld", MainData->NumFrames);
			ProgressInd->ChangeText(LineText);
			if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
				{ throw UserCancel();}
			EnergyPos = Buffer->GetFilePos();

			Buffer->SkipnLines(1);
			NextFinalPos = -1;
			if (lFrame->NumAtoms - NumFragmentAtoms > 0) {
		//		if (NewStyleGeometryKeyWord&&Buffer->LocateKeyWord("BEGINNING GEOMETRY SEARCH POINT", 31)) {
				if (Buffer->LocateKeyWord(OptKeywords) >= 0) {
		//			KeyWordFound = true;
					NextFinalPos = Buffer->GetFilePos();
					//this doesn't seem to be needed and it messes up the last point in a numerical opt.
	//			} else {
	//				KeyWordFound = Buffer->LocateKeyWord("FINAL", 5);
				}
	//			if (KeyWordFound) NextFinalPos = Buffer->GetFilePos();
				Buffer->SetFilePos(EnergyPos);
			}
		// The output for optimizations was changed in 1/1998 to print all coordinates
		// at the top of an optimization step, but the older style is also still possible
		//Look for ab initio atoms first
			Buffer->SetFilePos(StartPos);	//reset pos to last frame to begin search
			if (Buffer->LocateKeyWord(OptKeywords) >= 0) {
				lpFrame = lFrame;
				lFrame = MainData->AddFrame(lpFrame->NumAtoms,0);

				if (SIMOMM) {	//copy over the MM atoms so that they will be in the same order
					for (int iatom=0; iatom<lpFrame->NumAtoms; ++iatom) {
						if (lpFrame->Atoms[iatom].IsSIMOMMAtom()) {
							lFrame->AddAtom(lpFrame->Atoms[iatom].Type, 
											lpFrame->Atoms[iatom].Position);
							lFrame->Atoms[iatom].IsSIMOMMAtom(true);
						}
					}
				}
				
				try {
					if (lpFrame->NumAtoms-NumFragmentAtoms > 0) {

						//Look for new style with all atoms at top of step first
						if (Buffer->LocateKeyWord("COORDINATES OF ALL ATOMS ARE (ANGS)", 35, EnergyPos)) {
							Buffer->SkipnLines(3);
							test = ParseGLogLine(Buffer, lFrame, lpFrame->NumAtoms-NumFragmentAtoms, 10, &(MainData->MaxSize));
							if (test==-1) {	//Something was wrong with this set of coordinates
								MainData->DeleteFrame();
								break;
							}
						} else if (Buffer->LocateKeyWord(" NSERCH", 7, NextFinalPos)) {
							//Otherwise look for the coordinates at the end of the
							//geometry step (pre 1998 style)
							wxFileOffset npos = Buffer->GetFilePos();
							Buffer->SkipnLines(9);
							test = ParseGLogLine(Buffer, lFrame, lpFrame->NumAtoms-NumFragmentAtoms, 1, &(MainData->MaxSize));
							if (test==-1) {	//Something was wrong with this set of coordinates
								MainData->DeleteFrame();
								break;
							}
							Buffer->SetFilePos(npos);//reset position back to beginning of gradient data
						}
					}
					if (NumFragmentAtoms > 0) {
						wxFileOffset tempPos = Buffer->GetFilePos();
						Buffer->SetFilePos(StartPos);
						if (Buffer->LocateKeyWord(OptKeywords) >= 0) {
							if (Buffer->LocateKeyWord("COORDINATES OF FRAGMENT MULTIPOLE CENTERS", 41, NextFinalPos)) {
								Buffer->SkipnLines(3);
								MainData->ReadFragmentCoordinates(Buffer, NumFragmentAtoms);
							}
							Buffer->SetFilePos(tempPos);
						}
					}
					lFrame->ReadGradient(Buffer, NextFinalPos);
				}
				catch (...) {	//Error occured while parsing coordinates, delete this frame
					MainData->DeleteFrame();
					Buffer->SetFilePos(StartPos);
					break;
				}
			} else {
				Buffer->SetFilePos(StartPos);
				break;
			}	//Make sure we actually found the coordinates
			if (lFrame->GetNumAtoms()==0) {Buffer->SetFilePos(StartPos); break;}
			lFrame->Energy = FrameEnergy;
			if (MainData->InputOptions->Control->GetMPLevel()==2)
				lFrame->Energies.push_back(EnergyValue(MP2FrameEnergy, PT2Energy));
			lFrame->IRCPt = lpFrame->IRCPt + flip;
			lFrame->time = lpFrame->time + flip + offset;
			
			if (SIMOMM) {
				Buffer->SetFilePos(EnergyPos);
				//could also grab the MM energy here
				if (Buffer->LocateKeyWord("QM+MM Energy (Hartree):", 23)) {
					Buffer->GetLine(LineText);
					double FrameEnergy;
					sscanf(&(LineText[24]), "%lf", &FrameEnergy);
					lFrame->Energy = FrameEnergy;
				}
				//now update the coordinates with the new MM list
				Frame * preserve = MainData->cFrame;
				lFrame = MainData->AddFrame(MainData->cFrame->NumAtoms,0);
				MainData->ParseTinkerCoordinates(Buffer);
				if (MainData->cFrame->NumAtoms > 0) {
					//update the coordinates in the first frame
					for (int iatom=0; iatom<MainData->cFrame->NumAtoms; ++iatom) {
						if (MainData->cFrame->Atoms[iatom].IsSIMOMMAtom()) {
							preserve->Atoms[iatom].Position.x = MainData->cFrame->Atoms[iatom].Position.x;
							preserve->Atoms[iatom].Position.y = MainData->cFrame->Atoms[iatom].Position.y;
							preserve->Atoms[iatom].Position.z = MainData->cFrame->Atoms[iatom].Position.z;
						}
					}
				}
				// delete off the temporary frame
				MainData->DeleteFrame();
				lFrame = preserve;
			}

			if (Prefs->GetAutoBond())
				lFrame->SetBonds(Prefs, false, ProgressInd);
//			NextFinalPos = -1;
			Buffer->SetFilePos(EnergyPos);
//			long SavedPos = Buffer->GetFilePos();
//			if (lFrame->NumAtoms - NumFragmentAtoms > 0) {
//				KeyWordFound = Buffer->LocateKeyWord("FINAL", 5);
//				if (KeyWordFound) NextFinalPos = Buffer->GetFilePos();
//				Buffer->SetFilePos(SavedPos);
//			}
				//Attempt to read in orbitals for this geometry
			if (MainData->Basis) {
				Buffer->SetFilePos(EnergyPos);
				try {
					if (MainData->InputOptions->Control->GetSCFType()==GAMESS_MCSCF) {
							bool ReadOrbs = false;
						if (Buffer->LocateKeyWord("MCSCF NATURAL ORBITALS", 22, NextFinalPos)) {
							if ((NextFinalPos<0)||(NextFinalPos>Buffer->GetFilePos()))
								ReadOrbs = true;
							else Buffer->SetFilePos(SavedPos);
						} else if (Buffer->LocateKeyWord("MCSCF OPTIMIZED ORBITALS", 24, NextFinalPos)) {
							if ((NextFinalPos<0)||(NextFinalPos>Buffer->GetFilePos()))
								ReadOrbs = true;
							else Buffer->SetFilePos(SavedPos);
						} else if (Buffer->LocateKeyWord("-MCHF- NATURAL ORBITALS", 23, NextFinalPos)) {
							if ((NextFinalPos<0)||(NextFinalPos>Buffer->GetFilePos()))
								ReadOrbs = true;
							else Buffer->SetFilePos(SavedPos);
						} else if (Buffer->LocateKeyWord("-MCHF- OPTIMIZED ORBITALS", 25, NextFinalPos)) {
							if ((NextFinalPos<0)||(NextFinalPos>Buffer->GetFilePos()))
								ReadOrbs = true;
							else Buffer->SetFilePos(SavedPos);
						} else if (Buffer->LocateKeyWord("MOLECULAR ORBITALS", 18, NextFinalPos)) {
							Buffer->SetFilePos(Buffer->FindBlankLine());
							Buffer->SkipnLines(1);
							lFrame->ParseGAMESSEigenVectors(Buffer, MainData->GetNumBasisFunctions(),
								MainData->GetNumBasisFunctions(), 0, NumOccAlpha, NumOccBeta,
								(TypeOfWavefunction)(MainData->InputOptions->Control->GetSCFType()), ProgressInd);
	//						lFrame->ExchangeEigenVectors();
						}
						if (ReadOrbs)
							lFrame->ParseGAMESSMCSCFVectors(Buffer, MainData->GetNumBasisFunctions(),
								OccupiedOrbCount, ProgressInd);
					} else {
						if (MainData->InputOptions->Control->GetSCFType() == GAMESS_GVB) {
							if (Buffer->LocateKeyWord("PAIR INFORMATION", 16, NextFinalPos)) {
								ReadGVBOccupancy(Buffer, NumGVBPairs, MainData->GetNumBasisFunctions(),
									Occupancy);
							}
						}
						bool test = false;
						if (MainData->InputOptions->Control->GetCIType() == 4) {
							//CIS output includes the EIGENVECTORS token
							//I think the regular EIGENVECTORS would be before the CIS line???
							wxFileOffset bp = Buffer->GetFilePos();
							test = Buffer->LocateKeyWord("EIGENVECTORS", 12, NextFinalPos);
							Buffer->BackupnLines(1);
							if (Buffer->LocateKeyWord("CIS EIGENVALUES", 14, 80+Buffer->GetFilePos())) {
								Buffer->SkipnLines(1);
								test = Buffer->LocateKeyWord("EIGENVECTORS", 12, NextFinalPos);
								if (!test) {
									Buffer->SetFilePos(bp);
									test = Buffer->LocateKeyWord("MOLECULAR ORBITALS", 18, NextFinalPos);
									Buffer->BackupnLines(1);
									if (Buffer->LocateKeyWord("# MOLECULAR ORBITALS =", 22)) {
										Buffer->SkipnLines(1);
										test = Buffer->LocateKeyWord("MOLECULAR ORBITALS", 18, NextFinalPos);
										if (!test) Buffer->SetFilePos(bp);
									}
								}
							}
						} else {
							test = Buffer->LocateKeyWord("EIGENVECTORS", 12, NextFinalPos);
							if (!test) {
								test = Buffer->LocateKeyWord("MOLECULAR ORBITALS", 18, NextFinalPos);
								//for a while GAMESS printed out MOLECULAR ORBITALS with the LZ analysis
								//so add a test to avoid that output
								if (test) {
									Buffer->BackupnLines(0);
									Buffer->GetLine(LineText);
									LinePos = FindKeyWord(LineText, "LZ VALUE ANALYSIS", 16);
									if (LinePos > -1) {
										Buffer->SkipnLines(2);
										test = Buffer->LocateKeyWord("MOLECULAR ORBITALS", 18, NextFinalPos);
									}
								}
							}
						}
						if (test) {
								//skip over the line to the start of the orbital blocks
								//There is always a blank line before the orbs, but there may a '---' divider line after the key line just found
							if ((NextFinalPos<0)||(NextFinalPos>Buffer->GetFilePos())) {
								Buffer->SetFilePos(Buffer->FindBlankLine());
								Buffer->SkipnLines(1);
								ProgressInd->ChangeText("Reading eigenvectors");
								if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
									{throw UserCancel();}
								OrbitalRec * OrbSet = lFrame->ParseGAMESSEigenVectors(Buffer, MainData->GetNumBasisFunctions(),
									MainData->GetNumBasisFunctions(), NumBetaUHFOrbs, NumOccAlpha, NumOccBeta,
									(TypeOfWavefunction)(MainData->InputOptions->Control->GetSCFType()), ProgressInd);
								if ((MainData->InputOptions->Control->GetSCFType() == GAMESS_GVB)&&(OrbSet!=NULL)) {
									//Add the Occupancy vector for GVB wavefunctions
									OrbSet->SetOccupancy(Occupancy, MainData->GetNumBasisFunctions());
								}
							} else Buffer->SetFilePos(SavedPos);
						}
					}
					// It doesn't appear that GVB orbitals are read in after the first set???
					// It appears that GAMESS only prints the GI orbitals on the first iteration
					if (MainData->InputOptions->Control->GetSCFType() == GAMESS_UHF) {
						if (Buffer->LocateKeyWord("UHF NATURAL ORBITALS AND OCCUPATION NUMBERS",
												  43, NextFinalPos)) {
							lFrame->ParseUHFNOs(Buffer, MainData->GetNumBasisFunctions(), ProgressInd);
						}
					}
		//			if (MainData->InputOptions->Control->GetSCFType() == GAMESS_TDDFT) {
					//I don't have the right control keywords yet. Possibilities are TDDFT=EXCITE in 
					//$CONTRL or TDPRP=.t. in $TDDFT
						if (Buffer->LocateKeyWord("TDDFT NATURAL ORBITALS",
												  23, NextFinalPos)) {
							lFrame->ParseTDDFTNOs(Buffer, MainData->GetNumBasisFunctions(), ProgressInd);
						}
		//			}
					if (MainData->InputOptions->Control->GetMPLevel()) {
						if (Buffer->LocateKeyWord("MP2 NATURAL ORBITAL OCCUPATION NUMBERS",
							38, NextFinalPos) && ReadMP2Orbitals) {
							lFrame->ReadMP2Vectors(Buffer, DatBuffer, MainData->GetNumBasisFunctions(),
								ProgressInd, &ReadMP2Orbitals);
						}
					}
					if (MainData->InputOptions->Control->GetCIType()) {	//Look for CI Natural orbitals
						//loop to add multiple CI NOs, one set for each eigenstate.
						//This would be the place to read in each eigenstate's energy
						while (Buffer->LocateKeyWord("NATURAL ORBITALS IN ATOMIC ORBITAL BASIS",
													 40, NextFinalPos)) {
							lFrame->ParseGAMESSCIVectors(Buffer, MainData->GetNumBasisFunctions(), ProgressInd);
						}
					}
				}
				catch (std::bad_alloc) {
					MessageAlert("Insufficient memory to read in eigenvectors.");
				}
				catch (MemoryError) {
					MessageAlert("Insufficient memory to read in eigenvectors.");
				}
				catch (DataError) {
					MessageAlert("Error reading eigenvectors, orbitals skipped.");
				}
			}
		}
	}
		//Read in normal modes, if present
	MainData->cFrame->ParseNormalModes(Buffer, ProgressInd, Prefs);

//Look for localized orbitals which appear only at the end of a log file
	if (MainData->Basis) {
		try {
			enum {
				CASSCFDiabatic=0,
				FockLocalized,
				DensityMatrixLocalized,
				OrientedLocOrbs,
				NonorthogLocOrbs,
				PPASVDOrbs,
				LocalizedOrbitals,
				SVDExternalOrbs,
				SplitQALocOrbs,
				OrderedExternalLocOrbs
			};
			std::vector<std::pair <std::string, int> > LocOrbKeywords;
			if (MainData->InputOptions->Control->GetSCFType()==GAMESS_MCSCF) 
				LocOrbKeywords.push_back(make_pair (std::string("CAS-SCF DIABATIC MOLECULAR ORBITALS"), (int) CASSCFDiabatic));
			LocOrbKeywords.push_back(make_pair (std::string("FOCK OPERATOR FOR THE LOCALIZED ORBITALS"), (int) FockLocalized));
			if (MainData->InputOptions->Control->GetSCFType()==GAMESS_MCSCF)
				LocOrbKeywords.push_back(make_pair (std::string("DENSITY MATRIX FOR THE LOCALIZED ORBITALS"), (int) DensityMatrixLocalized));
			LocOrbKeywords.push_back(make_pair (std::string("ORIENTED LOCALIZED ORBITALS"), (int) OrientedLocOrbs));
			LocOrbKeywords.push_back(make_pair (std::string("NONORTHOGONAL PPA SVD LOCALIZED ORBITALS"), (int) NonorthogLocOrbs));
			LocOrbKeywords.push_back(make_pair (std::string("PPA SVD LOCALIZED ORBITALS"), (int) PPASVDOrbs));
//			LocOrbKeywords.push_back(make_pair (std::string("SVD EXTERNAL LOCALIZED ORBITALS"), (int) SVDExternalOrbs));
			LocOrbKeywords.push_back(make_pair (std::string("SPLITQA LOCALIZED ORBITALS"), (int) SplitQALocOrbs));
			LocOrbKeywords.push_back(make_pair (std::string("ORDERED EXTERNAL LOCALIZED ORBITALS"), (int) OrderedExternalLocOrbs));
//Order is important, look for this last since it is a subset of some of the others
			LocOrbKeywords.push_back(make_pair (std::string("LOCALIZED ORBITALS"), (int) LocalizedOrbitals));
			
			while (! Buffer->Eof()) {
				int kw;
				if (-1 < (kw = Buffer->LocateKeyWord(LocOrbKeywords))) {
					wxFileOffset keywordPosition = Buffer->GetFilePos();
					OrbitalRec * OrbSet;
					switch (kw) {
						case CASSCFDiabatic:
							//MCSCF runs can potentially have diabatic orbitals at the end.
							if (MainData->InputOptions->Control->GetSCFType()==GAMESS_MCSCF) {
								lFrame->ParseGAMESSMCSCFDiabaticVectors(Buffer, MainData->GetNumBasisFunctions(),
																		OccupiedOrbCount, ProgressInd);
							}
							break;
						case FockLocalized:
							Buffer->SkipnLines(1);
							break;
						case OrientedLocOrbs:
							Buffer->SkipnLines(2);
							ProgressInd->ChangeText("Reading oriented localized orbitals");
							if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
							{ throw UserCancel();}
							OrbSet = lFrame->ParseGAMESSLMOs(Buffer, MainData->GetNumBasisFunctions(), MainData->GetNumBasisFunctions(),
															 NumBetaUHFOrbs, ProgressInd, true);
							if (OrbSet != NULL) {
								OrbSet->setOrbitalWavefunctionType((TypeOfWavefunction)MainData->InputOptions->Control->GetSCFType());
								if (MainData->InputOptions->Control->GetSCFType() != GAMESS_MCSCF) {
									OrbSet->SetOrbitalOccupancy(NumOccAlpha, NumOccBeta);
								}
							}
							break;
						case NonorthogLocOrbs:
							Buffer->SetFilePos(Buffer->FindBlankLine());
							Buffer->SkipnLines(1);
							ProgressInd->ChangeText("Reading Nonorthogonal PPA SVD localized orbitals");
							if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
							{ throw UserCancel();}
							OrbSet = lFrame->ParseGAMESSLMOs(Buffer, MainData->GetNumBasisFunctions(), MainData->GetNumBasisFunctions(),
															 NumBetaUHFOrbs, ProgressInd, true);
							OrbSet->setOrbitalType(NonOrthogonalSVDLocalizedOrbital);
							break;
						case PPASVDOrbs:
							Buffer->SetFilePos(Buffer->FindBlankLine());
							Buffer->SkipnLines(1);
							ProgressInd->ChangeText("Reading PPA SVD localized orbitals");
							if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
							{ throw UserCancel();}
							OrbSet = lFrame->ParseGAMESSLMOs(Buffer, MainData->GetNumBasisFunctions(), MainData->GetNumBasisFunctions(),
															 NumBetaUHFOrbs, ProgressInd, true);
							OrbSet->setOrbitalType(PPASVDLocalizedOrbital);
							break;
						case LocalizedOrbitals:
							Buffer->SkipnLines(2);
							ProgressInd->ChangeText("Reading localized orbitals");
							if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
							{ throw UserCancel();}
							OrbSet = lFrame->ParseGAMESSLMOs(Buffer, MainData->GetNumBasisFunctions(), MainData->GetNumBasisFunctions(),
																		  NumBetaUHFOrbs, ProgressInd, false);
							if (OrbSet != NULL) {
								OrbSet->setOrbitalWavefunctionType((TypeOfWavefunction)MainData->InputOptions->Control->GetSCFType());
								if (MainData->InputOptions->Control->GetSCFType() != GAMESS_MCSCF) {
									OrbSet->SetOrbitalOccupancy(NumOccAlpha, NumOccBeta);
								}
							}
							break;
						case SVDExternalOrbs:
							Buffer->SetFilePos(Buffer->FindBlankLine());
							Buffer->SkipnLines(1);
							ProgressInd->ChangeText("Reading SVD External localized orbitals");
							if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
							{ throw UserCancel();}
							OrbSet = lFrame->ParseGAMESSLMOs(Buffer, MainData->GetNumBasisFunctions(), MainData->GetNumBasisFunctions(),
															 NumBetaUHFOrbs, ProgressInd, true);
							OrbSet->setOrbitalType(SVDExternalLocalizedOrbital);
							break;
						case SplitQALocOrbs:
							Buffer->SetFilePos(Buffer->FindBlankLine());
							Buffer->SkipnLines(1);
							ProgressInd->ChangeText("Reading SplitQA localized orbitals");
							if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
							{ throw UserCancel();}
							OrbSet = lFrame->ParseGAMESSLMOs(Buffer, MainData->GetNumBasisFunctions(), MainData->GetNumBasisFunctions(),
															 NumBetaUHFOrbs, ProgressInd, true);
							OrbSet->setOrbitalType(SplitQAExternalLocalizedOrbital);
							break;
						case OrderedExternalLocOrbs:
							Buffer->SetFilePos(Buffer->FindBlankLine());
							Buffer->SkipnLines(1);
							ProgressInd->ChangeText("Reading Ordered External localized orbitals");
							if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
							{ throw UserCancel();}
							OrbSet = lFrame->ParseGAMESSLMOs(Buffer, MainData->GetNumBasisFunctions(), MainData->GetNumBasisFunctions(),
															 NumBetaUHFOrbs, ProgressInd, true);
							OrbSet->setOrbitalType(OrderedExternalLocalizedOrbital);
							break;
					}
					//If still positioned at the keyword, must advance or there will be a loop.
					if (keywordPosition == Buffer->GetFilePos()) Buffer->SkipnLines(1);
				} else break;
			}
				//MCSCF runs can potentially have diabatic orbitals at the end.
//			if (MainData->InputOptions->Control->GetSCFType()==GAMESS_MCSCF) {
//				if (Buffer->LocateKeyWord("CAS-SCF DIABATIC MOLECULAR ORBITALS", 34)) {
//					lFrame->ParseGAMESSMCSCFDiabaticVectors(Buffer, MainData->GetNumBasisFunctions(),
//												OccupiedOrbCount, ProgressInd);
//				}
//			}
				//GAMESS now punchs out the Fock operator for ruedenburg type localization
				//which I need to skip if it is present
//			if (Buffer->LocateKeyWord("FOCK OPERATOR FOR THE LOCALIZED ORBITALS",40))
//				Buffer->SkipnLines(1);
//			if (MainData->InputOptions->Control->GetSCFType()==GAMESS_MCSCF) {
					//skip over the density matrix (present in Ruedenburg MCSCF runs)
//				if (Buffer->LocateKeyWord("DENSITY MATRIX FOR THE LOCALIZED ORBITALS",41))
//					Buffer->SkipnLines(1);
//			}
//			KeyWordFound = Buffer->LocateKeyWord("LOCALIZED ORBITALS",18);
//			if (KeyWordFound) {
//				Buffer->SkipnLines(2);
//				ProgressInd->ChangeText("Reading localized orbitals");
//				if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
//					{ throw UserCancel();}
//				OrbitalRec * OrbSet = lFrame->ParseGAMESSLMOs(Buffer, MainData->GetNumBasisFunctions(), MainData->GetNumBasisFunctions(),
//					NumBetaUHFOrbs, ProgressInd, false);
//				if (OrbSet != NULL) {
//					OrbSet->setOrbitalWavefunctionType((TypeOfWavefunction)MainData->InputOptions->Control->GetSCFType());
//					if (MainData->InputOptions->Control->GetSCFType() != GAMESS_MCSCF) {
//						OrbSet->SetOrbitalOccupancy(NumOccAlpha, NumOccBeta);
//					}
//				}
//				// There may also be oriented localized orbitals
//				KeyWordFound = Buffer->LocateKeyWord("ORIENTED LOCALIZED ORBITALS",27);
//				if (KeyWordFound) {
//					Buffer->SkipnLines(2);
//					ProgressInd->ChangeText("Reading oriented localized orbitals");
//					if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
//						{ throw UserCancel();}
//					OrbSet = lFrame->ParseGAMESSLMOs(Buffer, MainData->GetNumBasisFunctions(), MainData->GetNumBasisFunctions(),
//						NumBetaUHFOrbs, ProgressInd, true);
//					if (OrbSet != NULL) {
//						OrbSet->setOrbitalWavefunctionType((TypeOfWavefunction)MainData->InputOptions->Control->GetSCFType());
//						if (MainData->InputOptions->Control->GetSCFType() != GAMESS_MCSCF) {
//							OrbSet->SetOrbitalOccupancy(NumOccAlpha, NumOccBeta);
//						}
//					}
//				}
//			}
		}
		catch (std::bad_alloc) {
			MessageAlert("Insufficient memory to read in the localized orbitals.");
		}
		catch (MemoryError) {
			MessageAlert("Insufficient memory to read in the localized orbitals.");
		}
		catch (DataError) {
			MessageAlert("Error reading localized orbitals, local orbs. skipped.");
		}
	}
	if (DatBuffer) DatBuffer->CloseFile();
	if (Occupancy) delete [] Occupancy;
	MainData->cFrame = MainData->Frames;
	MainData->CurrentFrame = 1;
	if (MainData->InputOptions->Control->GetMPLevel()==2) {
		EnergyOptions * lOpts = Prefs->GetEnergyOptions();
		lOpts->SetPlotMPEnergy(true);
		lOpts->SetPlotEnergy(false);
	}
	if ((MainData->InputOptions->Control->GetRunType()==4)||
		(MainData->InputOptions->Control->GetRunType()==6)) {
		MainData->SetCurrentFrame(MainData->GetNumFrames());
	}
	return 1;
}	/*OpenGAMESSlog*/
bool ReadGVBOccupancy(BufferFile * Buffer, long NumPairs, long MaxOrbs, float * Occupancy) {
		char Line[kMaxLineLength];
		long	junk, Orb1, Orb2;
		float	junkf, Occ1, Occ2;
	if ((NumPairs <= 0)||(!Occupancy)) return false;	//no GVB pairs or no memory
	Buffer->SkipnLines(4);
	for (long i=0; i<NumPairs; ++i) {
		Buffer->GetLine(Line);
		sscanf(Line, "%ld %ld %ld %f %f %f %f", &junk, &Orb1, &Orb2, &junkf, &junkf,
			&Occ1, &Occ2);
		if ((Orb1<1)||(Orb1>MaxOrbs)||(Orb2<1)||(Orb2>MaxOrbs)) return false;//invalid orbital number
		Occupancy[Orb1 - 1] = Occ1;
		Occupancy[Orb2 - 1] = Occ2;
	}
	return true;
}
//routine to parse initial fragment coordinates which are in a real ugly
//format with all of the multipole expantion points
long MoleculeData::ReadInitialFragmentCoords(BufferFile * Buffer) {
	long NumFragmentAtoms = 0, iatom, NumMultipoles=0, FragmentNumber=0;
	char Label[kMaxLineLength], Line[kMaxLineLength];
	Frame * lFrame = GetCurrentFramePtr();
	iatom = lFrame->GetNumAtoms();
	
	//First read in the fragment names which are separate from the coordinates
	//I am not sure why I originally pulled it as "FOR", but GAMESS hasn't used it for some time.
	//	"COORDINATES FOR FRAGMENT"
	while (Buffer->LocateKeyWord("COORDINATES OF FRAGMENT", 23, Buffer->GetFilePos()+200)) {
		if (Buffer->LocateKeyWord("NAMED", 5, Buffer->GetFilePos()+80)) {
			Buffer->GetLine(Line);
			sscanf(&(Line[5]), "%s", Label);
			FragmentNames.push_back(std::string(Label));
		} else Buffer->SkipnLines(1);
	}
	while (Buffer->LocateKeyWord("READING POTENTIAL PARAMETERS FOR FRAGMENT", 40, Buffer->GetFilePos()+200)) {
		Buffer->SkipnLines(1);
	}

	if (Buffer->LocateKeyWord("TOTAL NUMBER OF MULTIPOLE POINTS", 32, Buffer->GetFilePos()+10000)) {
		Buffer->GetLine(Line);
		sscanf(&(Line[32]), " =%ld", &NumMultipoles);
	}
		//Mike finally fixed the spelling of coordinates
	if ((NumMultipoles>0) && (Buffer->LocateKeyWord("MULTIPOLE COORDINATES", 20, Buffer->GetFilePos()+10000) ||
			Buffer->LocateKeyWord("MULTIPOLE CORDINATES", 19, Buffer->GetFilePos()+10000))) {
		Buffer->SkipnLines(3);
		SetupFrameMemory(lFrame->NumAtoms + 10, 0);
		bool workingFragment=false;
		for (int i=0; i<NumMultipoles; ++i) {
				CPoint3D	Pos;
				float		eCharge, nCharge;
			Buffer->GetLine(Line);
			int iscan = sscanf(Line, "%s%f%f%f%f%f", Label, &Pos.x, &Pos.y, &Pos.z,
				&eCharge, &nCharge);
			if (iscan == 6) {
			
					//The fragment output at the top of the log file is very bad for reading 
					//and parsing. First we test on charge (some types have atom positions with
					//zero nuclear but positive electronic charge). Then strip off the 'Z' which
					//may or may not be present, then try to avoid non-atom positions which most of
					//the time have negative charge and often seem to start with 'B'.
				if (eCharge>0.5 || nCharge > 0.5) {
					long AtomType = -1;
					if (std::toupper(Label[0]) == 'B') {
						//a 'B' almost always not an atom
						if (workingFragment) workingFragment=false;	//flag to indicate end of fragment
						if ((std::isalpha(Label[1]))) {
							if (eCharge<3.5 || nCharge < 3.5) continue;
						}
					}
					if ((Label[0] == 'Z') || (Label[0] == 'z')) {	//z represents an atom pos
						Label[0] = Label[1];
						if (std::isalpha(Label[2])) {
							Label[1] = Label[2];
							Label[2] = 0;
						} else Label[1] = 0;
					}
					AtomType = SetAtomType((unsigned char *) &(Label[0]));
					if ((std::toupper(Label[0]) == 'A')&&(AtomType < 0)) {
						AtomType = nCharge;
					}
					if (lFrame->NumAtoms + 1 > lFrame->AtomAllocation)
						SetupFrameMemory(lFrame->NumAtoms + 10, 0);
					if (AtomType > 0) {
						if (!workingFragment) {
							++FragmentNumber;
							workingFragment = true;
						}
						Pos.x *= kBohr2AngConversion;
						Pos.y *= kBohr2AngConversion;
						Pos.z *= kBohr2AngConversion;
						mpAtom * newAtom = lFrame->AddAtom(AtomType, Pos);
						if (newAtom) newAtom->SetFragmentNumber(FragmentNumber);
						MaxSize = MAX(MaxSize, fabs(Pos.x));
						MaxSize = MAX(MaxSize, fabs(Pos.y));
						MaxSize = MAX(MaxSize, fabs(Pos.z));
						++NumFragmentAtoms;
					}
				} else workingFragment = false;
			}
		}
	}
	return NumFragmentAtoms;
}
void MoleculeData::ReadFragmentCoordinates(BufferFile * Buffer, long NumFragmentAtoms) {
	long		iatom=0, FragmentNumber=0;
	CPoint3D	Pos;
	char		Label[kMaxLineLength], Line[kMaxLineLength];
	Frame *		lFrame = GetCurrentFramePtr();
	Frame *		startFrame = Frames;
	long		referenceFragAtom=0;

	while (iatom<NumFragmentAtoms) {
		Buffer->GetLine(Line);
		if (-1<LocateKeyWord(Line, "FRAGNAME=",9, strlen(Line))) {++FragmentNumber; continue;}
		int iscan = sscanf(Line, "%s%f%f%f", Label, &Pos.x, &Pos.y, &Pos.z);
		if (iscan == 4) {
			long AtomType = -1;
			if ((Label[0] == 'Z') || (Label[0] == 'z')) {	//z represents an atom pos
				Label[0] = Label[1];
				if (std::isalpha(Label[2])) {
					Label[1] = Label[2];
					Label[2] = 0;
				} else Label[1] = 0;
			}
			AtomType = SetAtomType((unsigned char *) &(Label[0]));
			//In the late-2007 style we get only the useless label (starting with an 'A')
			//and the x, y, and z coords. So in order to determine the atomtype we have
			//to reference the initial frame.
			if ((std::toupper(Label[0]) == 'A')&&(AtomType < 0)) {
				while ((referenceFragAtom < startFrame->GetNumAtoms())&&
					   !startFrame->Atoms[referenceFragAtom].IsEffectiveFragment())
					++referenceFragAtom;
				if (referenceFragAtom < startFrame->GetNumAtoms()) {
					AtomType = startFrame->Atoms[referenceFragAtom].Type;
					++referenceFragAtom;
				}
			}
			if (AtomType > 0) {
				mpAtom * newAtom = lFrame->AddAtom(AtomType, Pos);
				if (newAtom) newAtom->SetFragmentNumber(FragmentNumber);
				MaxSize = MAX(MaxSize, fabs(Pos.x));
				MaxSize = MAX(MaxSize, fabs(Pos.y));
				MaxSize = MAX(MaxSize, fabs(Pos.z));
				++iatom;
			}
		}
	}
}
void MoleculeData::ReadBasisOptions(BufferFile * Buffer) {
		char LineText[kMaxLineLength], token[kMaxLineLength];
		long LinePos, test;
	if (Buffer->LocateKeyWord("GBASIS", 6)) {
		Buffer->GetLine(LineText);
		LinePos = FindKeyWord(LineText, "GBASIS", 6) + 7;
		sscanf(&(LineText[LinePos]),"%s", token);
		InputOptions->Basis->SetBasis(token);

		LinePos = FindKeyWord(LineText, "IGAUSS", 6);
		if (LinePos > -1) {
			sscanf(&(LineText[LinePos+7]),"%ld", &test);
			InputOptions->Basis->SetNumGauss(test);
		}

		LinePos = FindKeyWord(LineText, "POLAR", 5);
		if (LinePos > -1) {
			sscanf(&(LineText[LinePos+6]),"%s", token);
			InputOptions->Basis->SetPolar(&(LineText[LinePos+6]));
		}

		Buffer->GetLine(LineText);
		LinePos = FindKeyWord(LineText, "NDFUNC", 6);
		if (LinePos > -1) {
			sscanf(&(LineText[LinePos+7]),"%ld", &test);
			InputOptions->Basis->SetNumDFuncs(test);
		}

		LinePos = FindKeyWord(LineText, "DIFFSP", 6);
		if (LinePos > -1) {
			sscanf(&(LineText[LinePos+7]),"%1s", token);
			InputOptions->Basis->SetDiffuseSP(token[0] == 'T');
		}

		Buffer->GetLine(LineText);
		LinePos = FindKeyWord(LineText, "NPFUNC", 6);
		if (LinePos > -1) {
			sscanf(&(LineText[LinePos+7]),"%ld", &test);
			InputOptions->Basis->SetNumPFuncs(test);
		}

		LinePos = FindKeyWord(LineText, "DIFFS", 5);
		if (LinePos > -1) {
			sscanf(&(LineText[LinePos+6]),"%1s", token);
			InputOptions->Basis->SetDiffuseS(token[0] == 'T');
		}
	}
}
void MoleculeData::ReadControlOptions(BufferFile * Buffer) {
		long	test;
		char	LineText[kMaxLineLength], token[kMaxLineLength];
	wxFileOffset StartPos = Buffer->GetFilePos()+5;	//All keywords should be between these positions
	wxFileOffset EndPos = Buffer->FindBlankLine();
	if (!Buffer->LocateKeyWord("SCFTYP", 6, EndPos)) throw DataError();
	Buffer->GetLine(LineText);
	sscanf(&(LineText[7]), "%s", token);
	test = InputOptions->Control->SetSCFType(token);
	// a -1 return means the SCFTyp was invalid, we'll continue, but there may be problems
	Buffer->SetFilePos(StartPos);

	if (Buffer->LocateKeyWord("RUNTYP", 6, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]), "%s", token);
		InputOptions->Control->SetRunType(token);
		Buffer->SetFilePos(StartPos);
	}

	if (Buffer->LocateKeyWord("EXETYP", 6, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]), "%s", token);
		InputOptions->Control->SetExeType(token);
		Buffer->SetFilePos(StartPos);
	}

	if (Buffer->LocateKeyWord("MPLEVL", 6, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]),"%ld", &test);
		InputOptions->Control->SetMPLevel(test);
		Buffer->SetFilePos(StartPos);
	}

	if (Buffer->LocateKeyWord("CITYP", 5, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]),"%s", token);
		InputOptions->Control->SetCIType(token);
		Buffer->SetFilePos(StartPos);
	}

	if (Buffer->LocateKeyWord("CCTYP", 5, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]),"%s", token);
		InputOptions->Control->SetCCType(token);
		Buffer->SetFilePos(StartPos);
	}
	
	if (Buffer->LocateKeyWord("LOCAL", 5, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]), "%s", token);
		InputOptions->Control->SetLocal(token);
		Buffer->SetFilePos(StartPos);
	}

	if (Buffer->LocateKeyWord("MULT", 4, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]),"%ld", &test);
		InputOptions->Control->SetMultiplicity(test);
		Buffer->SetFilePos(StartPos);
	}

	if (Buffer->LocateKeyWord("ICHARG", 6, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]),"%ld", &test);
		InputOptions->Control->SetCharge(test);
		Buffer->SetFilePos(StartPos);
	}

	if (Buffer->LocateKeyWord("MAXIT", 5, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]),"%ld", &test);
		InputOptions->Control->SetMaxIt(test);
		Buffer->SetFilePos(StartPos);
	}

	bool foo = Buffer->LocateKeyWord("ECP", 3, EndPos);
	if (!foo) foo = Buffer->LocateKeyWord("PP  ", 4, EndPos);
	if (foo) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]),"%s", token);
		InputOptions->Basis->SetECPPotential(token);
		Buffer->SetFilePos(StartPos);
	}

	if (Buffer->LocateKeyWord("ISPHER", 6, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]),"%ld", &test);
		if (test == 1) InputOptions->Control->UseSphericalHarmonics(true);
		Buffer->SetFilePos(StartPos);
	}
	if (Buffer->LocateKeyWord("NOSYM", 5, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]),"%ld", &test);
		InputOptions->Data->SetUseSym((test==0));
		Buffer->SetFilePos(StartPos);
	}
	
	if (Buffer->LocateKeyWord("NORMF", 5, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]),"%ld", &test);
		InputOptions->Control->SetNormF((test!=0));
		Buffer->SetFilePos(StartPos);
	}

	if (Buffer->LocateKeyWord("NORMP", 5, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]),"%ld", &test);
		InputOptions->Control->SetNormP((test!=0));
		Buffer->SetFilePos(StartPos);
	}
}
void SystemGroup::ReadSystemOptions(BufferFile * Buffer) {
	char	LineText[kMaxLineLength], token[kMaxLineLength];
	wxFileOffset StartPos = Buffer->GetFilePos();	//All keywords should be between these positions
	wxFileOffset EndPos = Buffer->FindBlankLine();

	if (Buffer->LocateKeyWord("MEMORY=", 7, EndPos)) {
		Buffer->GetLine(LineText);
		double temp=0.0;
		sscanf(&(LineText[7]), "%lf", &temp);
		SetMemory(temp);
		Buffer->SetFilePos(StartPos);
	}
	if (Buffer->LocateKeyWord("MEMDDI=", 7, EndPos)) {
		Buffer->GetLine(LineText);
		double temp=0.0;
		sscanf(&(LineText[7]), "%lf", &temp);
		SetMemDDI(temp);
		Buffer->SetFilePos(StartPos);
	}
	if (Buffer->LocateKeyWord("TIMLIM=", 7, EndPos)) {
		Buffer->GetLine(LineText);
		double temp=0.0;
		sscanf(&(LineText[7]), "%lf%s", &temp, token);
		if (FindKeyWord(token, "SECONDS", 7)>=0)
			temp /= 60.0;	//For some odd reason TIMLIM is printed in seconds...
		SetTimeLimit((long) temp);
		Buffer->SetFilePos(StartPos);
	}
	if (Buffer->LocateKeyWord("PARALL=", 7, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]), "%s", token);
		if (token[0]=='F') SetParallel(false);
		else if (token[0]=='T') SetParallel(true);
		Buffer->SetFilePos(StartPos);
	}
	if (Buffer->LocateKeyWord("BALTYP=", 7, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]), "%s", token);
		if (FindKeyWord(token, "LOOP", 4)>=0)
			SetBalanceType(true);
		Buffer->SetFilePos(StartPos);
	}
	if (Buffer->LocateKeyWord("COREFL=", 7, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]), "%s", token);
		if (token[0]=='F') SetCoreFlag(false);
		else if (token[0]=='T') SetCoreFlag(true);
		Buffer->SetFilePos(StartPos);
	}
	if (Buffer->LocateKeyWord("KDIAG=", 6, EndPos)) {
		Buffer->GetLine(LineText);
		int temp;
		sscanf(&(LineText[6]), "%d", &temp);
		SetDiag(temp);
	}
}
long MolDisplayWin::OpenGAMESSTRJ(BufferFile * Buffer, bool Append, long flip, float offset)
{
	TypeOfRun		runType=InvalidRunType;
	char			LineText[kMaxLineLength+1];
	bool			FirstFrame = true;
	long IRCnSkip = Prefs->GetDRCSkip();
	long nSkip = 0;
	
	ProgressInd->ChangeText("Reading GAMESS trajectory file...");
	Frame * lFrame = MainData->cFrame;
	
	long NumAIAtoms=0, NumFragments=0, NumMDAtoms=0;
	//Determine the specific run type
	//the 1024 is arbitrary, but if it's not found in the first 1024 bytes it probably isn't there
	if (Buffer->LocateKeyWord("===== IRC DATA PACKET", 21, 1024)) {
		runType	= IRCRun;
	} else if (Buffer->LocateKeyWord("===== DRC DATA PACKET", 21, 1024)) {
		runType = DRCRun;
	} else if (Buffer->LocateKeyWord("===== MD DATA PACKET", 20, 1024)) {
		runType = MolDynamics;
	} else if (Buffer->LocateKeyWord("ACCEPTED AT GLOBAL SEARCH POINT", 20, 1024)) {
		runType = GLOBOPRun;
		wxFileOffset start = Buffer->GetFilePos();
		wxFileOffset fragStart=0;
		if (Buffer->LocateKeyWord("COORDINATES OF FRAGMENT MULTIPOLE CENTERS", 40)) {
			fragStart = Buffer->GetFilePos();
		}
		wxFileOffset nextPoint = Buffer->GetFileLength();
		if (Buffer->LocateKeyWord("ACCEPTED AT GLOBAL SEARCH POINT", 20)) {
			nextPoint = Buffer->GetFilePos();
		}
		if (fragStart > 0) {	
			Buffer->SetFilePos(fragStart);
			while (Buffer->LocateKeyWord("FRAGNAME", 8, nextPoint)) {
				NumFragments++;
				Buffer->SkipnLines(1);
			}
		} else fragStart = nextPoint;
		Buffer->SetFilePos(start);
		NumAIAtoms = Buffer->GetNumLines(fragStart - start) - 5;//subtract off the header lines
	}
	if (!Append) { //set the runtype
		if (!MainData->InputOptions) MainData->InputOptions = new InputData;
		if (runType) MainData->InputOptions->Control->SetRunType(runType);
	}
	Buffer->SetFilePos(0);
	std::vector<std::pair <std::string, int> > keywords;
	keywords.push_back(make_pair (std::string("DATA PACKET"), 1));
	keywords.push_back(make_pair (std::string("ACCEPTED AT GLOBAL SEARCH POINT"), 2));
	
	std::vector<std::pair <std::string, int> > AIkeywords;
	AIkeywords.push_back(make_pair (std::string("----- QM PARTICLE COORDINATES"), 1));
	AIkeywords.push_back(make_pair (std::string("COORDINATES OF ALL ATOMS ARE"), 2));

	std::vector<std::pair <std::string, int> > FragmentAtomsKeywords;
	FragmentAtomsKeywords.push_back(make_pair (std::string("----- EFP PARTICLE COORDINATES"), 1));
	FragmentAtomsKeywords.push_back(make_pair (std::string("COORDINATES OF FRAGMENT MULTIPOLE CENTERS"), 2));

	//search for the beginning of the information packet
	
	while (Buffer->LocateKeyWord(keywords) > -1) {
		if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
		{ throw UserCancel();}
		//if this is an irc we could pull the point out of the header line
		if (runType == IRCRun) {
			Buffer->GetLine(LineText);
			long point=0;
			long pos = FindKeyWord(LineText, "FOR STEP", 8);
			if (pos >= 0) {
				sscanf(&(LineText[pos+8]), "%ld", &point);
			}
			lFrame->IRCPt = point * flip;
		} else
			Buffer->SkipnLines(1);
		//search for the end of the packet?
		//first line give atom counts, nat=       3 nfrg=       1 nqmmm=       0
		Buffer->GetLine(LineText);
		if (nSkip >= IRCnSkip) {
			nSkip = 0;
			float xvalue=0.0;
			double totE=0.0;
			if (runType == GLOBOPRun) {
				Buffer->BackupnLines(2);	//Need the first line for GlobOp files
				Buffer->GetLine(LineText);
				ReadDoubleKeyword(LineText, "ENERGY", totE);
				int Pos = FindKeyWord(LineText, "POINT", 5);
				if (Pos >= 0) {
					sscanf(&(LineText[Pos+5]), "%f", &xvalue);
				}
			} else {
				if (!ReadLongKeyword(LineText, "NAT", &NumAIAtoms))
					wxLogMessage(_("Unable to locate NAT (ab initio atom count)."));
				if (!ReadLongKeyword(LineText, "NFRG", &NumFragments))
					wxLogMessage(_("Unable to locate NFRG (fragment count)."));
				if (!ReadLongKeyword(LineText, "NQMMM", &NumMDAtoms))
					wxLogMessage(_("Unable to locate NQMM (MM atom count)."));

				//second line is the x-axis and total energy, either ttotal or stotal
				//STOTAL=   1.49848 sqrt(amu)*bohr   tot. E=      -91.6219797490 Hartree
				Buffer->GetLine(LineText);
				if (runType == IRCRun) {
					if (!ReadFloatKeyword(LineText, "STOTAL", &xvalue)) {
						wxLogMessage(_("Unable to locate STOTAL in IRC data packet. Unable to position this packet in the sequence."));
					}
				} else {	//MD and DRC spell it TTOTAL
					if (!ReadFloatKeyword(LineText, "TTOTAL", &xvalue)) {
						wxLogMessage(_("Unable to locate TOTAL in trajectory data packet. Unable to position this packet in the sequence."));
					}
				}
				ReadDoubleKeyword(LineText, "TOT. E", totE);
			}

			xvalue *= flip;
			xvalue += offset;
			Frame * lFrame = NULL;
			if (!Append && FirstFrame) {	
				lFrame = MainData->Frames;
				lFrame->time = xvalue;
				FirstFrame = false;
			} else
				lFrame = MainData->LocateNewFrame(xvalue);
			if (lFrame) {
				lFrame->Energy = totE;
				// DRCs have an extra line with the kinetic and potential energies
				//POT. E=        -9.1287351998 kin. E=          .0016326701 Hartree
				if (runType == DRCRun) {
					//DRCs conserve energy so Tot E == Pot. E + kinetic E.
					//Thus only need to store two of them.
					Buffer->GetLine(LineText);
					double kinE;
					if (ReadDoubleKeyword(LineText, "KIN. E", kinE))
						lFrame->SetEnergy(kinE, KineticEnergy);
				} else if (runType == MolDynamics) {
				// MD runs have two extra lines for energies
				//POT. E=        -47726.960743 kcal/mol
				//kin. E=             7.015829  trans KE=   5.422237  rot KE=   1.593592 kcal/mol
				//Todo: For MD runs energy is not necessarily conserved so all energies could be stored
				//Don't have the infrastructure for that at the moment.
					Buffer->GetLine(LineText);
					Buffer->GetLine(LineText);
					double kinE;
					if (ReadDoubleKeyword(LineText, "KIN. E", kinE))
						lFrame->SetEnergy(kinE, KineticEnergy);
					else
						wxLogMessage(_("Unable to parse the kinetic energy while reading a molecular dynamics trajectory file. The file format may not be correct."));
				}

				//setup the frame for the antipated atom count
				if (!MainData->SetupFrameMemory(NumAIAtoms+NumFragments+NumMDAtoms, 0)) throw MemoryError();
				
				if (NumAIAtoms > 0) {
					switch (Buffer->LocateKeyWord(AIkeywords)) {
						case 1:
							Buffer->SkipnLines(1);
							ParseGLogLine(Buffer, lFrame, NumAIAtoms, 10, &(MainData->MaxSize));
							break;
						case 2:
							Buffer->SkipnLines(3); //just skip the header lines
							ParseGLogLine(Buffer, lFrame, NumAIAtoms, 10, &(MainData->MaxSize));
							break;
						default:
							wxLogMessage(_("Expected ab initio atoms, but unable to locate header."));
					}
				}
				int fragkey;
				if ((NumFragments > 0)&&(fragkey = Buffer->LocateKeyWord(FragmentAtomsKeywords))) {
					if (fragkey == 1)
						Buffer->SkipnLines(1);
					else if (fragkey == 2)
						Buffer->SkipnLines(3);
					if (fragkey > 0) {
						mpAtom *atm;
						CPoint3D dst_locs[3];
						CPoint3D pos;
						int fstart;
						int match[3] = {0, 0, 0};
						CPoint3D src_locs[3];
						std::string labels[3];
						long fragNum, fragmentsfound=0;
						int i;
						CPoint3D new_pos;
						CPoint3D rot_pos;
						CPoint3D curr_pos;
						CPoint3D orig;
						Matrix4D vec2vec;
						CPoint3D src_vec;
						CPoint3D dst_vec;
						CPoint3D dst_vec2;
						CPoint3D dst_norm;
						CPoint3D mid_norm;
						CPoint3D mid_vec1, mid_vec2;
						float dot;
						char	token[kMaxLineLength+1];

						Buffer->GetLine(LineText);
						while (ReadStringKeyword(LineText, "FRAGNAME", token)) {
							if (!strcasecmp(token, "H2ORHF") || !strcasecmp(token, "H2ODFT")) {	//builtin EFP1 style is limited to H2O with known labels
								MainData->FragmentNames.push_back(std::string(token));
								long fragNum = MainData->FragmentNames.size();
								CPoint3D	pos;
								int		AtomType;
								for (int i=0; i<3; ++i) {
									AtomType = -1;
									Buffer->GetLine(LineText);
									//lines have format "label x, y, z"
									sscanf(LineText, "%s %f %f %f", token, &pos.x, &pos.y, &pos.z);
									int c = 0;
									if ((token[0] == 'Z')||(token[0] == 'z')) c = 1;
									if (!strcasecmp(&(token[c]), "O1")) AtomType = 8;
									else if (!strcasecmp(&(token[c]), "H2")||!strcasecmp(&(token[c]), "H3")) AtomType = 1;
									if (AtomType > 0) {
										mpAtom * atm = lFrame->AddAtom(AtomType, pos);
										atm->SetFragmentNumber(fragNum);
									}
								}
							}
							
							// Custom fragment type.
							else {
								
								// Read the three atoms that setup the fragment.  These
								// should be in angstroms.
								MainData->FragmentNames.push_back(std::string(token));
								fragNum = MainData->FragmentNames.size();
								char label[kMaxLineLength];
								for (i = 0; i < 3; ++i) {
									Buffer->GetLine(LineText);
									sscanf(LineText, "%s %f %f %f", label,
										   &(dst_locs[i].x), &(dst_locs[i].y),
										   &(dst_locs[i].z));
									labels[i] = label;
									/* std::cout << "Line: " << Line << std::endl; */
									/* atm = lFrame->AddAtom(1, dst_locs[i]); */
									/* atm->SetFragmentNumber(fragNum); */
								}
								
								//The fragment definition must be either already in the local list or in 
								//the builder library. FindFragmentDef will find it either way.
								std::map<std::string, EFrag>::const_iterator frag;
								frag = FindFragmentDef(token);
								if (frag == MainData->efrags.end()) {
									wxString msg;
									msg.Printf(_("Unable to locate correct EFP2 fragment definition group named %s"), token);
									wxLogMessage(msg);
									throw DataError();
								}
								
								fstart = lFrame->NumAtoms;
								const std::vector<EFragAtom>& labeled_atoms = frag->second.GetAtoms();
								std::vector<EFragAtom>::const_iterator efrag_atom;
								
								for (efrag_atom = labeled_atoms.begin();
									 efrag_atom != labeled_atoms.end();
									 ++efrag_atom) {
									
									if (efrag_atom->GetLabel().compare(labels[0]) == 0) {
										match[0] = lFrame->NumAtoms;
										src_locs[0] = efrag_atom->GetCoords();
									} else if (efrag_atom->GetLabel().compare(labels[1]) == 0) {
										match[1] = lFrame->NumAtoms;
										src_locs[1] = efrag_atom->GetCoords();
									} else if (efrag_atom->GetLabel().compare(labels[2]) == 0) {
										match[2] = lFrame->NumAtoms;
										src_locs[2] = efrag_atom->GetCoords();
									}
									
									atm = lFrame->AddAtom(efrag_atom->GetAtomicNumber(),
														  efrag_atom->GetCoords());
									atm->SetFragmentNumber(fragNum);
								}
								
								// We first find a rotation that one will align a vector
								// in the fragment template to the corresponding vector in
								// the destination space.
								dst_vec = dst_locs[1] - dst_locs[0];
								Normalize3D(&dst_vec);
								
								src_vec = src_locs[1] - src_locs[0];
								Normalize3D(&src_vec);
								
								SetRotationMatrix(vec2vec, &src_vec, &dst_vec);
								
								// The common vector now serves as an axis of rotation.  We
								// need to rotate the fragment template plane so that it
								// coincides with the destination plane.  The angle of rotation
								// can be determined by the angle between the two planes'
								// normals.
								lFrame->GetAtomPosition(match[0], orig);
								Rotate3DOffset(vec2vec, src_locs[1] - orig, &mid_vec1);
								Rotate3DOffset(vec2vec, src_locs[2] - orig, &mid_vec2);
								UnitCrossProduct3D(&mid_vec1, &mid_vec2, &mid_norm);
								
								dst_vec2 = dst_locs[2] - dst_locs[0];
								UnitCrossProduct3D(&dst_vec, &dst_vec2, &dst_norm);
								
								Matrix4D tri2tri;
								dot = DotProduct3D(&mid_norm, &dst_norm);
								
								// Technically, the axis of rotation (the aligned vector)
								// might be facing a different direction than we think it
								// is. To be consistent, we instead use the axis that is
								// normal to both planes' normals. It points in the same
								// or opposite direction as dst_vec.
								CPoint3D axis;
								UnitCrossProduct3D(&mid_norm, &dst_norm, &axis);
								RotateAroundAxis(tri2tri, axis, acos(dot) * 180.0f / kPi);
								
								// We concatenate the two rotation matrices.
								Matrix4D transform;
								MultiplyMatrix(vec2vec, tri2tri, transform);
								
								// Okay, for each atom we added for this fragment instance,
								// we move all fragment atoms into destination space. We
								// translate to make the base fragment atom the origin and
								// then rotate to align the fragment with the destination
								// plane, and then translate by the base destination atom.
								for (long i = fstart; i < lFrame->NumAtoms; ++i) {
									lFrame->GetAtomPosition(i, curr_pos);
									new_pos = curr_pos - orig;
									Rotate3DOffset(transform, new_pos, &rot_pos);
									new_pos = rot_pos + dst_locs[0];
									lFrame->SetAtomPosition(i, new_pos);
								}
							}
							fragmentsfound++;
							if (fragmentsfound < NumFragments) {
								//gamess prints out all the coordinates, but we only read the first three
								//need to skip the rest to the start of the next fragment
								if (Buffer->LocateKeyWord("FRAGNAME", 8))
									Buffer->GetLine(LineText);
							}
						}
					}
					
				}
				if ((NumMDAtoms > 0)&&Buffer->LocateKeyWord("----- MM PARTICLE COORDINATES", 29)) {
					Buffer->SkipnLines(1);
				}

				if (Prefs->GetAutoBond())
					lFrame->SetBonds(Prefs, false, ProgressInd);
			}
		} else
			++ nSkip;
	}
	return 1;
}	
long MolDisplayWin::OpenGAMESSIRC(BufferFile * Buffer, bool Append, long flip, float offset)
{	long			LinePos, NumAtoms, point;
	float			Xpos;
	bool			KeyWordFound;
	char			LineText[kMaxLineLength+1];

	ProgressInd->ChangeText("Reading GAMESS IRC file...");
	Frame * lFrame = MainData->cFrame;
	long IRCnSkip = Prefs->GetDRCSkip();
	long nSkip = 0;

	if (!Append) {
		if (!Buffer->LocateKeyWord("IRC INFORMATION PACKET", 22)) return 0;
		if (!Buffer->LocateKeyWord("POINT=", 6)) return 0;
		Buffer->GetLine(LineText);
		LinePos = 6;
		sscanf(&(LineText[LinePos]), "%ld", &point);
		lFrame->IRCPt = (long) (flip*point + offset);
		LinePos = FindKeyWord(LineText, "STOTAL=", 7) + 7;
		sscanf(&(LineText[LinePos]), "%f", &(lFrame->time));
		lFrame->time *= flip;
		lFrame->time += offset;
		LinePos = FindKeyWord(LineText, "E=", 2) + 2;
		sscanf(&(LineText[LinePos]), "%lf", &(lFrame->Energy));
		Buffer->SkipnLines(1);	//skip "Coordinates (bohr)... label line

		wxFileOffset start = Buffer->GetFilePos();
		if (!Buffer->LocateKeyWord("MASS-WEIGHTED GRADIENT", 22)) throw DataError();
		wxFileOffset endpos = Buffer->GetFilePos();
		Buffer->SetFilePos(start);
		NumAtoms = Buffer->GetNumLines(endpos-start);
		
		if (!MainData->SetupFrameMemory(NumAtoms, 0)) throw MemoryError();

		ParseGLogLine(Buffer, lFrame, NumAtoms, 0, &(MainData->MaxSize));

		if (Prefs->GetAutoBond())
			lFrame->SetBonds(Prefs, false, ProgressInd);
	}
	NumAtoms = lFrame->NumAtoms;
	KeyWordFound = Buffer->LocateKeyWord("POINT=", 6);
	while (KeyWordFound) {
		if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
			{ throw UserCancel();}
		Buffer->GetLine(LineText);
		if (nSkip >= IRCnSkip) {
			nSkip = 0;
			LinePos = 6;
			sscanf(&(LineText[LinePos]), "%ld", &point);
			point *= flip;
			LinePos = FindKeyWord(LineText, "STOTAL=", 7) + 7;
			sscanf(&(LineText[LinePos]), "%f", &Xpos);
			Xpos *= flip;
			Xpos += offset;
			lFrame = MainData->LocateNewFrame(Xpos);

			if (lFrame) {
				lFrame->IRCPt = point;
				LinePos = FindKeyWord(LineText, "E=", 2) + 2;
				sscanf(&(LineText[LinePos]), "%lf", &(lFrame->Energy));
				Buffer->SkipnLines(1);	//skip "Coordinates (bohr)... label line

				sprintf(LineText, "Reading in IRC point %ld", point);
				ProgressInd->ChangeText(LineText);

				if (!MainData->SetupFrameMemory(NumAtoms, 0)) throw MemoryError();

				if (ParseGLogLine(Buffer, lFrame, NumAtoms, 0, &(MainData->MaxSize)) <= 0) break;

				if (Prefs->GetAutoBond())
					lFrame->SetBonds(Prefs, false, ProgressInd);
			}
		} else
			++nSkip;
		KeyWordFound = Buffer->LocateKeyWord("POINT=", 6);
	}
	MainData->cFrame = MainData->Frames;
	MainData->CurrentFrame = 1;
	return 1;
}	/*OpenGAMESSIRC*/
long MolDisplayWin::OpenGAMESSIRCLog(BufferFile * Buffer, long flip, float offset,
		long NumOccAlpha, long NumOccBeta, long NumFragmentAtoms) {
	long			point, LinePos, NumBetaUHFOrbs=0;
	char			LineText[kMaxLineLength+1];
	float			Xpos;
	bool			LINEAR=false;

	ProgressInd->ChangeText("Reading GAMESS IRC log file...");
	Frame * lFrame = MainData->cFrame;
	long IRCnSkip = Prefs->GetDRCSkip();
	long nSkip = 0;

	long NumAtoms = lFrame->NumAtoms;
	long NumExpectedAtoms = NumAtoms;
	if (MainData->InputOptions->Control->GetSCFType()==GAMESS_UHF) NumBetaUHFOrbs = NumOccBeta;	//Only seperate Beta spin orbs for UHF wavefunctions
	char NextPointKeyword[30];
	strcpy(NextPointKeyword, "* NEXT POINT ON IRC FOUND *");
	bool KeyWordFound = Buffer->LocateKeyWord(NextPointKeyword, 27);
	if (!KeyWordFound) {	//search for the alternative keyword needed for pace=linear IRC's
		strcpy(NextPointKeyword, "ON THE REACTION PATH");
		KeyWordFound = Buffer->LocateKeyWord(NextPointKeyword, 20);
		if (KeyWordFound) LINEAR=true;
	}
	while (KeyWordFound) {
		if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
			{ throw UserCancel();}
		if (LINEAR) Buffer->BackupnLines(1);
		Buffer->SkipnLines(1);
		if (Buffer->LocateKeyWord("POINT", 5)) {
			Buffer->GetLine(LineText);
			LinePos = 6;
			int result = sscanf(&(LineText[LinePos]), "%ld", &point);
			if (result != 1) break;
		} else break;
		if (Buffer->LocateKeyWord("STOTAL", 6)) {
			Buffer->GetLine(LineText);
			LinePos = 8;
			int result = sscanf(&(LineText[LinePos]), "%f", &Xpos);
			if (result != 1) break;
		} else break;
		lFrame = NULL;
		if (nSkip >= IRCnSkip) {
			nSkip = 0;
			point *= flip;
			Xpos *= flip;
			Xpos += offset;
			lFrame = MainData->LocateNewFrame(Xpos);
			if (lFrame) {
				sprintf(LineText, "Reading in IRC point %ld", point);
				ProgressInd->ChangeText(LineText);

				lFrame->IRCPt = point;
				if (Buffer->LocateKeyWord("ENERGY            =", 19)) {
					Buffer->GetLine(LineText);
					LinePos = 20;
					sscanf(&(LineText[LinePos]), "%lf", &(lFrame->Energy));
				}

				if (!Buffer->LocateKeyWord("COORDINATES OF ALL ATOMS", 24)) break;
				Buffer->SkipnLines(3);

				MainData->SetupFrameMemory(NumAtoms, 0);

				ParseGLogLine(Buffer, lFrame, NumAtoms, 10, &(MainData->MaxSize));

				if ((NumFragmentAtoms <= 0)&&(NumExpectedAtoms > MainData->cFrame->NumAtoms))
					NumFragmentAtoms = NumExpectedAtoms - MainData->cFrame->NumAtoms;
				if (NumFragmentAtoms > 0) {
					Buffer->BackupnLines(2);
					if (Buffer->LocateKeyWord("COORDINATES OF FRAGMENT MULTIPOLE CENTERS", 41)) {
						Buffer->SkipnLines(3);
						MainData->ReadFragmentCoordinates(Buffer, NumFragmentAtoms);
					}
				}
				if (Prefs->GetAutoBond())
					lFrame->SetBonds(Prefs, false, ProgressInd);
			}
		} else
			++ nSkip;
		wxFileOffset SavedPos = Buffer->GetFilePos();
		KeyWordFound = Buffer->LocateKeyWord(NextPointKeyword, 20);
		wxFileOffset NextPointPos = -1;
		if (KeyWordFound) NextPointPos = Buffer->GetFilePos();
		if (lFrame) {
				//Attempt to read in orbitals for this geometry
			if (MainData->Basis) {
				Buffer->SetFilePos(SavedPos);
				try {
					bool test = Buffer->LocateKeyWord("CONVERGED ORBITALS AT IRC POINT", 31, NextPointPos);
					if (test) {
							//skip over the line to the start of the orbital blocks
							//There is always a blank line before the orbs, but there may a '---' divider line after the key line just found
						if ((NextPointPos<0)||(NextPointPos>Buffer->GetFilePos())) {
							Buffer->SetFilePos(Buffer->FindBlankLine());
							Buffer->SkipnLines(1);
							ProgressInd->ChangeText("Reading eigenvectors");
							if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
								{ throw UserCancel();}
							lFrame->ParseGAMESSEigenVectors(Buffer, MainData->GetNumBasisFunctions(),
								MainData->GetNumBasisFunctions(), NumBetaUHFOrbs, NumOccAlpha, NumOccBeta,
									(TypeOfWavefunction)(MainData->InputOptions->Control->GetSCFType()), ProgressInd);
	//						if (MainData->InputOptions->Control->GetSCFType()==MCSCF)
	//							lFrame->ExchangeEigenVectors();
						} else Buffer->SetFilePos(SavedPos);
					}
	//				if (lFrame->Orbs) {
	//					lFrame->Orbs->NumOccupiedAlphaOrbs = NumOccAlpha;
	//					lFrame->Orbs->NumOccupiedBetaOrbs = NumOccBeta;
	//					if (lFrame->Orbs->EigenVectors)
	//						lFrame->Orbs->EigenVectors->OrbType = MainData->InputOptions->Control->GetSCFType();
	//				}
				}
				catch (std::bad_alloc) {
	//				if (lFrame->Orbs) delete lFrame->Orbs;
	//				lFrame->Orbs = NULL;
					MessageAlert("Insufficient memory to read in eigenvectors.");
				}
				catch (MemoryError) {
	//				if (lFrame->Orbs) delete lFrame->Orbs;
	//				lFrame->Orbs = NULL;
					MessageAlert("Insufficient memory to read in eigenvectors.");
				}
				catch (DataError) {
					MessageAlert("Error reading eigenvectors, orbitals skipped.");
	//				if (lFrame->Orbs) delete lFrame->Orbs;
	//				lFrame->Orbs = NULL;
				}
			}
			if (NextPointPos >= 0)
				Buffer->SetFilePos(NextPointPos);
		}
	}
	MainData->cFrame = MainData->Frames;
	MainData->CurrentFrame = 1;
	return 1;
}	/*OpenGAMESSIRCLog*/
long MolDisplayWin::OpenGAMESSDRC(BufferFile * Buffer, bool LogFile, bool Append,
			long flip, float offset) {

	bool			QPpresent, KeyWordFound, newQPformat=false;
	long			NumAtoms, Elength, nskip=0, LinePos, EStartPos;
	char			Etext[40], LineText[kMaxLineLength+1];
	float			tempfloat;
	double			PE, KE;

	ProgressInd->ChangeText("Reading GAMESS DRC file...");
	Frame * lFrame = MainData->cFrame;
	if (!Append) {
	}
	wxFileOffset FilePos = Buffer->GetFilePos();
	QPpresent = Buffer->LocateKeyWord("Q          P", 12);
	if (!QPpresent) newQPformat = Buffer->LocateKeyWord("Q              P", 16);
	Buffer->SetFilePos(FilePos);
		//test for Q P since there are two possible formats (Q P... include some mode information)
	if (QPpresent) {
		strncpy(Etext, "ENERGY         ENERGY       ENERGY", 34);
		Elength = 34;
		EStartPos = 41;
	} else if (newQPformat) {	//After Sep62001R5 the header changed a bit
		strncpy(Etext, "E         ENERGY         ENERGY", 31);
		Elength = 31;
		EStartPos = 41;
		QPpresent = true;
	} else {
		strncpy(Etext, "ENERGY        ENERGY          ENERGY", 36);
		Elength = 36;
		EStartPos = 12;
	}
	KeyWordFound = Buffer->LocateKeyWord(Etext, Elength);
	if (!KeyWordFound) return 0;
	if (!Append) {
		Buffer->SkipnLines(1);
		Buffer->GetLine(LineText);
		sscanf(LineText, "%f", &(lFrame->time));
		lFrame->time *= flip;
		lFrame->time += offset;
		LinePos = EStartPos;
		sscanf(&(LineText[LinePos]), "%lf %lf %lf", &KE, &PE,
			&(lFrame->Energy));//Don't worry about the PE since E-KE=PE
		lFrame->SetEnergy(KE, KineticEnergy);

		if (!Buffer->LocateKeyWord("VELOCITY", 8)) return 0;
		Buffer->SkipnLines(1);
		if (LogFile)
			Buffer->SkipnLines(1);
		FilePos = Buffer->GetFilePos();
		Buffer->LocateKeyWord("--------", 8);
		wxFileOffset endPos = Buffer->GetFilePos();
		Buffer->SetFilePos(FilePos);
		NumAtoms = Buffer->GetNumLines(endPos-FilePos)-1;
		if (NumAtoms <= 0) throw DataError();

		if (!MainData->SetupFrameMemory(NumAtoms, 0)) throw MemoryError();
		
		ParseGLogLine(Buffer, lFrame, NumAtoms, 2, &(MainData->MaxSize));

		if (Prefs->GetAutoBond())
			lFrame->SetBonds(Prefs, false, ProgressInd);

		KeyWordFound = Buffer->LocateKeyWord(Etext, Elength);
	}

	NumAtoms = lFrame->NumAtoms;
	long DRCnSkip = Prefs->GetDRCSkip();
	while (KeyWordFound) {
		Buffer->SkipnLines(1);
		if (nskip >= DRCnSkip) {
			nskip = 0;
			if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
				{ throw UserCancel();}
			Buffer->GetLine(LineText);
			sscanf(LineText, "%f", &tempfloat);
			tempfloat *= flip;
			tempfloat += offset;
			lFrame = MainData->LocateNewFrame(tempfloat);
			if (lFrame == NULL) {
				KeyWordFound = Buffer->LocateKeyWord(Etext, Elength);
				continue;
			}
			LinePos = EStartPos;
			sscanf(&(LineText[LinePos]), "%lf %lf %lf", &KE, &PE,
				&(lFrame->Energy));//Don't worry about the PE since E-KE=PE
			lFrame->SetEnergy(KE, KineticEnergy);

			sprintf(LineText, "Reading DRC time = %f", tempfloat);
			ProgressInd->ChangeText(LineText);

			if (!MainData->SetupFrameMemory(NumAtoms, 0)) throw MemoryError();

			if (!Buffer->LocateKeyWord("VELOCITY", 8)) break;;
			Buffer->SkipnLines(1);
			if (LogFile)
				Buffer->SkipnLines(1);
			if (ParseGLogLine(Buffer, lFrame, NumAtoms, 2, &(MainData->MaxSize)) < 0) break;

			if (Prefs->GetAutoBond())
				lFrame->SetBonds(Prefs, false, ProgressInd);
		} else
			++nskip;
		KeyWordFound = Buffer->LocateKeyWord(Etext, Elength);
	}

	lFrame = MainData->Frames;
	for (long iframe=0; iframe<MainData->NumFrames; ++iframe) {
		lFrame->IRCPt = iframe + 1;
		lFrame = lFrame->NextFrame;
	}
	MainData->cFrame = MainData->Frames;
	MainData->CurrentFrame = 1;
	return 1;
}	/*OpenGAMESSDRC*/

long MolDisplayWin::OpenGAMESSGlobOpLog(BufferFile * Buffer,
									 long NumOccAlpha, long NumOccBeta, long NumFragmentAtoms) {
	long			NumBetaUHFOrbs=0;
	char			LineText[kMaxLineLength+1];
	
	ProgressInd->ChangeText("Reading GAMESS GlobOp log file...");
	Frame * lFrame = MainData->cFrame;
	
	long NumAtoms = lFrame->NumAtoms;
	long NumExpectedAtoms = NumAtoms;
	MainData->SetCurrentFrame(MainData->GetNumFrames());
		//Setup a new frame for the initial geometry.
	lFrame = MainData->AddFrame(NumExpectedAtoms, lFrame->GetNumBonds());
	lFrame->IRCPt = MainData->GetCurrentFrame();
	lFrame->time = MainData->GetCurrentFrame();
	if (MainData->InputOptions->Control->GetSCFType()==GAMESS_UHF) NumBetaUHFOrbs = NumOccBeta;	//Only seperate Beta spin orbs for UHF wavefunctions

// There appear to be two type of files, MCMIN=T|F. If true each accepted geometry is optimized
// and the code should only grab the optimized coordinates and energies. If false then there is just
// a single set of coordinates and energy. 
// If true look for "EQUILIBRIUM GEOMETRY LOCATED", grab the following coordinates and optionally, orbitals.
// Both types end with an energy "ENERGY ACCEPTED AT POINT", probably best to use it.
// It appears the globop frame is bracketed by "GEOMETRY NUMBER" and "ENERGY ACCEPTED", though not all 
// geometries are accepted.
	
// Old GlobOp files lack the keywords, but current ones do have them under the heading of
// ---- MONTE CARLO/SIMULATED ANNEALING OPTIONS -----
	
	enum GlobOpTags {
		GeomStart=0,
		NonOptGeometry,
		GeomEnd,
		GeomEnd2,
		OptGeometry,
		Orbitals,
		FailedOpt
	};

	std::vector<std::pair <std::string, int> > keywords;
	keywords.push_back(make_pair (std::string("GEOMETRY NUMBER"), (int) GeomStart));//old style
	keywords.push_back(make_pair (std::string("EVALUATE ENERGY AT NEW TRANSLATIONAL GEOMETRY"), (int) GeomStart));//new style
	keywords.push_back(make_pair (std::string("BEGINNING GEOMETRY SEARCH POINT NSERCH="), (int) NonOptGeometry));//new style
	keywords.push_back(make_pair (std::string("NUCLEAR COORDINATES FOR GLOBAL OPTIMIZATION POINT"), (int) NonOptGeometry));
	keywords.push_back(make_pair (std::string("ENERGY ACCEPTED AT POINT"), (int) GeomEnd));
	keywords.push_back(make_pair (std::string("ENERGY ACCEPTED AT GLOBAL SEARCH POINT"), (int) GeomEnd2));
	keywords.push_back(make_pair(std::string("EQUILIBRIUM GEOMETRY LOCATED"), (int) OptGeometry));
	keywords.push_back(make_pair(std::string("MOLECULAR ORBITALS"), (int) Orbitals));
	keywords.push_back(make_pair(std::string("FAILURE TO LOCATE STATIONARY POINT"), (int) FailedOpt));
	
	// In the old style there was a clear beginning and end of a search. In the new style it's a bit murkier.
	// The new style doesn't announce the end of an optimization until you are past the coordinates.
	
	int state = 1, kw;
	while (! Buffer->Eof()) {
		if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
		{ throw UserCancel();}
		if (-1 < (kw = Buffer->LocateKeyWord(keywords))) {
			switch (kw) {
				case GeomStart:	//start of a search point
					if (state == 0) {
						state = 1;
					} else {//found another start before finding the end of the previous geometry
						//The most likely reason is that the previous geometry was not accepted.
						//I could search for the rejection text, but this should do the trick.
						MainData->DeleteFrame();
						lFrame = MainData->GetCurrentFramePtr();
					}
					lFrame = MainData->AddFrame(NumExpectedAtoms, lFrame->GetNumBonds());
					lFrame->IRCPt = MainData->GetCurrentFrame();
					lFrame->time = MainData->GetCurrentFrame();
					Buffer->SkipnLines(1);
					break;
				case NonOptGeometry:	// set of coordinates, MCMIN=false, these coords seem to be in an odd format
					if (state == 1) {
						if (lFrame->NumAtoms > 0) {
							MainData->DeleteFrame();
							lFrame = MainData->GetCurrentFramePtr();
							lFrame = MainData->AddFrame(NumExpectedAtoms, lFrame->GetNumBonds());
							lFrame->IRCPt = MainData->GetCurrentFrame();
							lFrame->time = MainData->GetCurrentFrame();
						}
						Buffer->SkipnLines(1);
						
						//The output is in the form of index_# atomic name x y z
						if ((NumAtoms-NumFragmentAtoms)>0) {
							bool done=false;
							int nextExpected=1;
							while (!done) {
								if (Buffer->GetLine(LineText) < 1) break;
								int index;
								CPoint3D position;
								unsigned char Label[kMaxLineLength];
								int items = sscanf(LineText, "%d %s %f%f%f", &index, Label, &(position.x),
												   &(position.y), &(position.z));
								if (items == 5) {
									if (index != nextExpected) {
										wxLogMessage(_("Error parsing coordinates, attempting to continue."));
										break;
									} else {
										int atomtype = SetAtomType(Label);
										if (atomtype > 0) {
											lFrame->AddAtom(atomtype, position);
										} else {
											wxLogMessage(_("Error parsing coordinates, attempting to continue."));
											break;
										}
									}
									
								} else {
									Buffer->BackupnLines(1);	//reset position and end
									break;
								}
								nextExpected ++;
							}
						}
						
						if ((NumFragmentAtoms <= 0)&&(NumExpectedAtoms > MainData->cFrame->NumAtoms))
							NumFragmentAtoms = NumExpectedAtoms - MainData->cFrame->NumAtoms;
						if (NumFragmentAtoms > 0) {
							Buffer->BackupnLines(2);
							if (Buffer->LocateKeyWord("COORDINATES OF FRAGMENT MULTIPOLE CENTERS", 41)) {
								Buffer->SkipnLines(3);
								MainData->ReadFragmentCoordinates(Buffer, NumFragmentAtoms);
							}
						}
						if (Prefs->GetAutoBond())
							lFrame->SetBonds(Prefs, false, ProgressInd);
					} else {
						wxLogMessage(_("Unexpected set of coordinates! Skipping and attempting to continue"));
						Buffer->SkipnLines(1);
					}
					break;
				case GeomEnd: // End of the search, read the energy and store the frame.
				case GeomEnd2:
					if (state == 0) {	// Found an end without a matching begining, skip
						wxLogMessage(_("Unexpected end of search point! Skipping and attempting to continue"));
						Buffer->SkipnLines(1);
					} else {
						Buffer->GetLine(LineText);
						int geom;
						int tokenLen = 25;
						if (kw == GeomEnd2) tokenLen = 38;
						int result = sscanf(&(LineText[tokenLen]), "%d IS %lf", &geom, &(lFrame->Energy));
						if (result != 2)
							wxLogMessage(_("Error parsing final energy for a geometry. Attempting to continue."));
						sprintf(LineText, "Completed parsing geometry %d", geom);
						ProgressInd->ChangeText(LineText);
						state = 0;
					}
					break;
				case OptGeometry:	// Final optimized coordinates, parse and overwrite previous set
					if (state == 1) {
						if (!Buffer->LocateKeyWord("COORDINATES OF ALL ATOMS", 24)) break;
						Buffer->SkipnLines(3);

						ParseGLogLine(Buffer, lFrame, NumAtoms, 10, &(MainData->MaxSize));
						
						if ((NumFragmentAtoms <= 0)&&(NumExpectedAtoms > MainData->cFrame->NumAtoms))
							NumFragmentAtoms = NumExpectedAtoms - MainData->cFrame->NumAtoms;
						if (NumFragmentAtoms > 0) {
							Buffer->BackupnLines(2);
							if (Buffer->LocateKeyWord("COORDINATES OF FRAGMENT MULTIPOLE CENTERS", 41)) {
								Buffer->SkipnLines(3);
								MainData->ReadFragmentCoordinates(Buffer, NumFragmentAtoms);
							}
						}
						if (Prefs->GetAutoBond())
							lFrame->SetBonds(Prefs, false, ProgressInd);
					} else Buffer->SkipnLines(1);
					break;
				case Orbitals: // Set of Orbitals
					//Attempt to read in orbitals for this geometry
					if (state == 1) {
						if (MainData->Basis) {
							try {
								//skip over the line to the start of the orbital blocks
								//There is always a blank line before the orbs, but there may a '---' divider line after the key line just found
								Buffer->SetFilePos(Buffer->FindBlankLine());
								Buffer->SkipnLines(1);
								ProgressInd->ChangeText("Reading eigenvectors");
								if (!ProgressInd->UpdateProgress(Buffer->PercentRead()))
								{ throw UserCancel();}
								lFrame->ParseGAMESSEigenVectors(Buffer, MainData->GetNumBasisFunctions(),
																MainData->GetNumBasisFunctions(), NumBetaUHFOrbs, NumOccAlpha, NumOccBeta,
																(TypeOfWavefunction)(MainData->InputOptions->Control->GetSCFType()), ProgressInd);
							}
							catch (std::bad_alloc) {
								MessageAlert("Insufficient memory to read in eigenvectors.");
							}
							catch (MemoryError) {
								MessageAlert("Insufficient memory to read in eigenvectors.");
							}
							catch (DataError) {
								MessageAlert("Error reading eigenvectors, orbitals skipped.");
							}
						}
					} else Buffer->SkipnLines(1);
					break;
				case FailedOpt:	//optimization failed. 
					if (state == 1) {
						sprintf(LineText, "Geometry optimization incomplete for frame %ld", MainData->GetNumFrames()-1);
						MessageAlert(LineText);
						
						if (Buffer->LocateKeyWord("COORDINATES OF ALL ATOMS ARE (ANGS)", 35)) {
							Buffer->SkipnLines(3);
							ParseGLogLine(Buffer, lFrame, NumAtoms, 10, &(MainData->MaxSize));
							
							if ((NumFragmentAtoms <= 0)&&(NumExpectedAtoms > MainData->cFrame->NumAtoms))
								NumFragmentAtoms = NumExpectedAtoms - MainData->cFrame->NumAtoms;
							if (NumFragmentAtoms > 0) {
								Buffer->BackupnLines(2);
								if (Buffer->LocateKeyWord("COORDINATES OF FRAGMENT MULTIPOLE CENTERS", 41)) {
									Buffer->SkipnLines(3);
									MainData->ReadFragmentCoordinates(Buffer, NumFragmentAtoms);
								}
							}
							if (Prefs->GetAutoBond())
								lFrame->SetBonds(Prefs, false, ProgressInd);
						}
					} else Buffer->SkipnLines(1);
					break;
			}
		} else break;
	}
	if (state == 1) MainData->DeleteFrame();	//started a frame without finding the end.
	MainData->SetCurrentFrame(1);
	MainData->DeleteFrame();	//Delete the initial frame that has only starting geometry
	return 1;
}	/*OpenGAMESSGlobOpLog*/

/**
 Output the coordinates in gamess input format ($DATA and $EFRAG group).
 If AllFrames then a series of $DATA groups will be output for the user to divide as needed.
 @param Buffer A buffer to write the text to.
 @param AllFrames Should the data for all frames be included or just the current frame?
 */
void MolDisplayWin::ExportGAMESS(BufferFile * Buffer, bool AllFrames) {
	//Utilize the data group class to do all the real work
	InputData * lInputOptions = MainData->GetInputData();
	long basisTest = 1;
	if (lInputOptions->Basis) {
		if (lInputOptions->Basis->GetBasis() != 0) basisTest = 0;
	}
	if (!AllFrames) {
		lInputOptions->Data->WriteToFile(Buffer, MainData, Prefs, basisTest);
	} else {
		long savedFrame	= MainData->GetCurrentFrame();
		MainData->CurrentFrame = 1;
		MainData->cFrame = MainData->Frames;
		for (int iframe=0; iframe<MainData->GetNumFrames(); ++iframe) {
			char		text[kMaxLineLength];
			sprintf(text, "! Frame # %d", iframe+1);
			Buffer->WriteLine(text, true);

			MainData->SetCurrentFrame(iframe);

			lInputOptions->Data->WriteToFile(Buffer, MainData, Prefs, basisTest);
		}
		
		MainData->SetCurrentFrame(savedFrame);

		if ((lInputOptions->Data->GetCoordType() == ZMTCoordType)||
			(lInputOptions->Data->GetCoordType() == ZMTMPCCoordType)) {
			//If we are using internals update their values after restoring the frame
			Internals * IntCoords = MainData->GetInternalCoordinates();
			MOPacInternals * mInts = NULL;
			if (IntCoords)
				mInts = IntCoords->GetMOPacStyle();
			if (mInts) {
				mInts->CartesiansToInternals(MainData);
			}
		}
	}
}
void MolDisplayWin::WriteTabbedEnergies(BufferFile * Buffer, bool AllFrames) {
	char	text[kMaxLineLength];
	long	a1, a2, a3;

	EnergyOptions * lEOpts = Prefs->GetEnergyOptions();
	GraphOptions * lPOpts = Prefs->GetGraphOptions();
	bool	PlotTE = lEOpts->PlotEnergy();
	bool	PlotMPE = lEOpts->PlotMPEnergy();
	bool PlotKE = lEOpts->PlotKEnergy();
	bool PlotPE = lEOpts->PlotPEnergy();

	Buffer->PutText("X Pos");
	if (PlotTE) Buffer->PutText("\tEnergy");
	if (PlotMPE) Buffer->PutText("\tMP2 Energy");
	if (PlotKE) Buffer->PutText("\tKinetic Energy");
	if (PlotPE) Buffer->PutText("\tPotential Energy");
	if (lPOpts->PlotRMSGradient()) Buffer->PutText("\tRMS Gradient");
	if (lPOpts->PlotMaxGradient()) Buffer->PutText("\tMax Gradient");
	if (lPOpts->PlotBondLength()) {
		a1 = lPOpts->Get1stAtom();
		a2 = lPOpts->Get2ndAtom();
		sprintf(text, "\tBond %ld-%ld", a1+1, a2+1);
		Buffer->PutText(text);
	}
	if (lPOpts->PlotBondAngle()) {
		a1 = lPOpts->Get1stAtom();
		a2 = lPOpts->Get2ndAtom();
		a3 = lPOpts->Get3rdAtom();
		sprintf(text, "\tAngle %ld-%ld-%ld", a1+1, a2+1, a3+1);
		Buffer->PutText(text);
	}
	Buffer->WriteLine("", true);

	Frame * lFrame = MainData->Frames;
	float	UnitFactor = 1.0f;
	if (lEOpts->GetDisplayUnits() == kKCalPerMole) UnitFactor = kHartreeTokCalPMol;
	long NumFrames = MainData->NumFrames;
	if (!AllFrames) {
		lFrame = MainData->cFrame;
		NumFrames = 1;
	}
	for (long i=0; i<NumFrames; ++i) {
		sprintf(text, "%f", lFrame->time);
		Buffer->PutText(text);
		if (PlotTE) {
			sprintf(text, "\t%f", (lFrame->Energy-lEOpts->GetY1Zero())*UnitFactor);
			Buffer->PutText(text);
		}
		if (PlotMPE) {
			sprintf(text, "\t%f", (lFrame->GetMP2Energy()-lEOpts->GetY1Zero())*UnitFactor);
			Buffer->PutText(text);
		}
		if (PlotKE) {
			sprintf(text, "\t%f", (lFrame->GetKineticEnergy()-lEOpts->GetY2Zero())*UnitFactor);
			Buffer->PutText(text);
		}
		if (PlotPE) {
			sprintf(text, "\t%f", (lFrame->Energy - lFrame->GetKineticEnergy() - lEOpts->GetY1Zero())*UnitFactor);
			Buffer->PutText(text);
		}
		if (lPOpts->PlotRMSGradient()) {
			sprintf(text, "\t%f", (lFrame->GetRMSGradient() - lEOpts->GetY2Zero()));
			Buffer->PutText(text);
		}
		if (lPOpts->PlotMaxGradient()) {
			sprintf(text, "\t%f", (lFrame->GetMaxGradient() - lEOpts->GetY2Zero()));
			Buffer->PutText(text);
		}
		if (lPOpts->PlotBondLength()) {
				float bLength;
			if (lFrame->GetBondLength(a1, a2, &bLength)) {
				sprintf(text, "\t%f", (bLength - lEOpts->GetY2Zero()));
				Buffer->PutText(text);
			} Buffer->PutText("\t");
		}
		if (lPOpts->PlotBondAngle()) {
				float bAngle;
			if (lFrame->GetBondAngle(a1, a2, a3, &bAngle)) {
				sprintf(text, "\t%f", (bAngle - lEOpts->GetY2Zero()));
				Buffer->PutText(text);
			} Buffer->PutText("\t");
		}
		Buffer->WriteLine("", true);

		lFrame = lFrame->NextFrame;
		if (!lFrame) break;
	}
}
void MolDisplayWin::WriteFrequencies(BufferFile * Buffer) {
	char	text[kMaxLineLength];
	Frame * lFrame;
	lFrame = MainData->cFrame;

	if (!lFrame->Vibs) return;

	bool haveInten = (lFrame->Vibs->Intensities.size() > 0);
	bool haveRaman = (lFrame->Vibs->RamanIntensity.size() > 0);

	Buffer->PutText("Frequency\tIntensity");
	if (haveRaman) Buffer->PutText("\tRaman Intensity");
	Buffer->WriteLine("", true);
	for (long i=0; i<lFrame->Vibs->GetNumModes(); ++i) {
		sprintf(text, "%s", lFrame->Vibs->Frequencies[i].c_str());
		Buffer->PutText(text);
		if (haveInten) {
			sprintf(text, "\t%f", lFrame->Vibs->GetIntensity(i));
			Buffer->PutText(text);
		}
		if (haveRaman) {
			sprintf(text, "\t%f", lFrame->Vibs->GetRamanIntensity(i));
			Buffer->PutText(text);
		}
		Buffer->WriteLine("", true);
	}
}
void MolDisplayWin::WriteXYZFile(BufferFile * Buffer, bool AllFrames, bool AllModes,
	bool AnimateMode) {
	char	text[kMaxLineLength];

	Frame * lFrame;
	lFrame = MainData->Frames;
	long NumFrames = MainData->NumFrames;
	if (!AllFrames) {
		lFrame = MainData->cFrame;
		NumFrames = 1;
	}

	if (AnimateMode) {	//One geometry with all normal modes
		if (!lFrame->Vibs) return;
		mpAtom * lAtoms = lFrame->Atoms;
		long	i, iatm;
		long cmode = (lFrame->NumAtoms)*(lFrame->Vibs->CurrentMode);
		CPoint3D * ModeOffset = new CPoint3D[lFrame->NumAtoms];
		CPoint3D * tempAtoms = new CPoint3D[lFrame->NumAtoms];
		if (!ModeOffset || !tempAtoms) return;
		float	VectorScale = Prefs->GetVectorScale();
		float offsetFactor = 1.0/40.0;
		for (iatm=0; iatm<(lFrame->NumAtoms); ++iatm) {
			tempAtoms[iatm] = lAtoms[iatm].Position;
			ModeOffset[iatm] = lFrame->Vibs->NormMode[iatm+cmode];
			ModeOffset[iatm] *= VectorScale;
		}
		for (i=0; i<20; ++i) {
			if ((i==5)||(i==15)) {
				offsetFactor *= -1.0f;
			}
			for (iatm=0; iatm<(lFrame->NumAtoms); ++iatm) {
				tempAtoms[iatm].x += offsetFactor*(ModeOffset[iatm].x);
				tempAtoms[iatm].y += offsetFactor*(ModeOffset[iatm].y);
				tempAtoms[iatm].z += offsetFactor*(ModeOffset[iatm].z);
			}	//Now punch out the XYZ frame
			sprintf(text, "%ld", lFrame->NumAtoms);
			Buffer->WriteLine(text, true);
			sprintf(text, "Frequency %s Frame %ld", 
					lFrame->Vibs->Frequencies[lFrame->Vibs->CurrentMode].c_str(), i+1);
			Buffer->WriteLine(text, true);
			for (long j=0; j<lFrame->NumAtoms; ++j) {
					Str255	Label;
				Prefs->GetAtomLabel(lAtoms[j].Type-1, Label);
				Label[Label[0]+1]=0;
				sprintf(text, "%s %.4f %.4f %.4f", &(Label[1]), tempAtoms[j].x,
					tempAtoms[j].y, tempAtoms[j].z);
				Buffer->PutText(text);
				CPoint3D * lMode = &(lFrame->Vibs->NormMode[(j + cmode)]);
				sprintf(text," %.4f %.4f %.4f", lMode->x, lMode->y, lMode->z);
				Buffer->WriteLine(text, true);
			}
		}
		delete [] tempAtoms;
		delete [] ModeOffset;
	} else if (AllModes) {
		if (!lFrame->Vibs) return;
			mpAtom * lAtoms = lFrame->Atoms;
		for (long i=0; i<lFrame->Vibs->GetNumModes(); ++i) {
			sprintf(text, "%ld", lFrame->NumAtoms);
			Buffer->WriteLine(text, true);
			sprintf(text, "Frequency %s", 
					lFrame->Vibs->Frequencies[i].c_str());
			Buffer->WriteLine(text, true);
			for (long j=0; j<lFrame->NumAtoms; ++j) {
					Str255	Label;
				Prefs->GetAtomLabel(lAtoms[j].Type-1, Label);
				Label[Label[0]+1]=0;
				sprintf(text, "%s %.4f %.4f %.4f", &(Label[1]), lAtoms[j].Position.x,
					lAtoms[j].Position.y, lAtoms[j].Position.z);
				Buffer->PutText(text);
				long cmode = (lFrame->NumAtoms)*i;
				CPoint3D * lMode = &(lFrame->Vibs->NormMode[(j + cmode)]);
				sprintf(text," %.4f %.4f %.4f", lMode->x, lMode->y, lMode->z);
				Buffer->WriteLine(text, true);
			}
		}
	} else {	
		for (long i=0; i<NumFrames; ++i) {
				mpAtom * lAtoms = lFrame->Atoms;
			sprintf(text, "%ld", lFrame->NumAtoms);
			Buffer->WriteLine(text, true);
			sprintf(text, "Frame %ld", i+1);
			Buffer->WriteLine(text, true);
			for (long j=0; j<lFrame->NumAtoms; ++j) {
					Str255	Label;
				Prefs->GetAtomLabel(lAtoms[j].GetType()-1, Label);
				Label[Label[0]+1]=0;
				sprintf(text, "%s %.4f %.4f %.4f", &(Label[1]), lAtoms[j].Position.x,
					lAtoms[j].Position.y, lAtoms[j].Position.z);
				Buffer->PutText(text);
				if (lFrame->Vibs) {	//There are normal modes with this frame so setup drawing parameters
					if (MainData->GetDrawMode()) {	//Are we supposed to show the normal mode?
						long cmode = (lFrame->NumAtoms)*(lFrame->Vibs->CurrentMode);
						CPoint3D * lMode = &(lFrame->Vibs->NormMode[(j + cmode)]);
						sprintf(text," %.4f %.4f %.4f", lMode->x, lMode->y, lMode->z);
						Buffer->PutText(text);
					}
				}
				Buffer->WriteLine("", true);
			}
			lFrame = lFrame->NextFrame;
			if (!lFrame) break;
		}
	}
}
void MolDisplayWin::WriteMDLMolFile(BufferFile * Buffer) {
	char Line[kMaxLineLength];
	Frame * lFrame = MainData->GetCurrentFramePtr();
		//The first line is the molecule name so copy out the current description
	if (MainData->Description)
		Buffer->PutText(MainData->Description);
	else if (MainData->InputOptions) {
		const char * temp = MainData->InputOptions->Data->GetTitle();
		if (temp) {
			Buffer->PutText(temp);
		}
	}
	Buffer->WriteLine("", true);
		//2nd line may contain various program data
	Buffer->PutText("  MacMolPlt                       ");
	if (lFrame->Energy != 0.0) {
		sprintf(Line, "%12.5f", lFrame->Energy);
		Buffer->PutText(Line);
	}
	Buffer->WriteLine("", true);	//finish off 2nd line and leave 3rd line blank
	Buffer->WriteLine("", true);
	sprintf(Line, "%3ld%3ld  0  0  0  0  0  0  0  0  0 v2000",
		lFrame->GetNumAtoms(), lFrame->GetNumBonds());
	Buffer->WriteLine(Line, true);
		mpAtom * lAtoms = lFrame->Atoms;
		long i;
	for (i=0; i<lFrame->GetNumAtoms(); ++i) {
			Str255	Label;
		Prefs->GetAtomLabel(lAtoms[i].GetType()-1, Label);
		Label[Label[0]+1]=0;
		sprintf(Line, "%10.4f%10.4f%10.4f %-3s 0  0  0  0  0  0  0  0  0  0  0  0",
			lAtoms[i].Position.x, lAtoms[i].Position.y,
			lAtoms[i].Position.z, &(Label[1]));
		Buffer->WriteLine(Line, true);
	}
	if (lFrame->GetNumBonds() > 0) {
		Bond * lBonds = lFrame->Bonds;
		for (i=0; i<lFrame->GetNumBonds(); ++i) {
			int	type = lBonds[i].Order;
			if (type>3 || type<=0) type = 1;
			sprintf(Line, "%3ld%3ld%3d  0  0  0  0", lBonds[i].Atom1+1,
				lBonds[i].Atom2+1, type);
			Buffer->WriteLine(Line, true);
		}
	}
		//Final line needs to be end line to terminate properties block
	Buffer->WriteLine("M  END", true);
}

void MolDisplayWin::WriteVRMLFile(BufferFile * Buffer) {
	Buffer->PutText("#VRML V2.0 utf8\n");  //VRML header
	Buffer->PutText("\n");

	Frame * lFrame = MainData->GetCurrentFramePtr();
	mpAtom * lAtoms = lFrame->Atoms;

	//drawing atoms

	long NumAtoms = lFrame->NumAtoms;
	float AtomScale = Prefs->GetAtomScale();
	long curAtomType;
	RGBColor * AtomColor;
	wxString tmpStr;
	float red, green, blue;

	for (long iatom=0; iatom<NumAtoms; ++iatom) {
		if (lAtoms[iatom].GetInvisibility()) continue;
		
		curAtomType = lAtoms[iatom].GetType() - 1;
		AtomColor = Prefs->GetAtomColorLoc(curAtomType);
		red = AtomColor->red/65536.0;
		green = AtomColor->green/65536.0;
		blue = AtomColor->blue/65536.0;
		
		float radius = AtomScale*Prefs->GetAtomSize(curAtomType);
		Buffer->PutText("Transform {\n");
		tmpStr.Printf(wxT("\ttranslation %f %f %f\n"), lAtoms[iatom].Position.x, 
					  lAtoms[iatom].Position.y, lAtoms[iatom].Position.z);
		Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
		Buffer->PutText("\tchildren [\n");
		Buffer->PutText("\t\tShape {\n");
		tmpStr.Printf(wxT("\t\t\tgeometry Sphere { radius %f }\n"), radius);
		Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
		Buffer->PutText("\t\t\tappearance Appearance {\n");
		tmpStr.Printf(wxT("\t\t\t\tmaterial Material { diffuseColor %f %f %f }\n"),
					  red, green, blue);
		Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
		Buffer->PutText("\t\t\t}\n");
		Buffer->PutText("\t\t}\n");
		Buffer->PutText("\t]\n");
		Buffer->PutText("}\n");
	}

	//drawing bonds
	Bond * lBonds = lFrame->Bonds;
	long NumBonds = lFrame->NumBonds;
	double BondSize = Prefs->GetQD3DBondWidth();
	//float dotProd;
	Matrix4D rotMat;
	CPoint3D NormalOffset, NormStart = CPoint3D(0.0f, 1.0f, 0.0f);
	double theta;
	double axisX, axisY, axisZ;
	
	for (long ibond=0; ibond<NumBonds; ++ibond) {
		CPoint3D v1, v2, offset;
		long atom1 = lBonds[ibond].Atom1;
		long atom2 = lBonds[ibond].Atom2;
		//BondOrder tmpOrder = lBonds[ibond].Order;
		
		//for (int ipipe = 0; ipipe < MAX(tmpOrder,1); ++ipipe) {
		
		v1.x = lAtoms[atom1].Position.x; // + offset_vec.x * baseBondOffset +
		//3.5 * tmpBondSize * offset_vec.x * ipipe;
		v1.y = lAtoms[atom1].Position.y; //+ offset_vec.y * baseBondOffset +
		//3.5 * tmpBondSize * offset_vec.y * ipipe;
		v1.z = lAtoms[atom1].Position.z; //+ offset_vec.z * baseBondOffset +
		//3.5 * tmpBondSize * offset_vec.z * ipipe;
		v2.x = lAtoms[atom2].Position.x; //+ offset_vec.x * baseBondOffset +
		//3.5 * tmpBondSize * offset_vec.x * ipipe;
		v2.y = lAtoms[atom2].Position.y; //+ offset_vec.y * baseBondOffset +
		//3.5 * tmpBondSize * offset_vec.y * ipipe;
		v2.z = lAtoms[atom2].Position.z; //+ offset_vec.z * baseBondOffset +
		//3.5 * tmpBondSize * offset_vec.z * ipipe;
		
		offset.x = v2.x - v1.x;
		offset.y = v2.y - v1.y;
		offset.z = v2.z - v1.z;
		
		double length = offset.Magnitude();
		double radius1 = AtomScale*Prefs->GetAtomSize(lAtoms[atom1].GetType() - 1);
		double radius2 = AtomScale*Prefs->GetAtomSize(lAtoms[atom2].GetType() - 1);
		double percent1 = radius1/length;
		double percent2 = radius2/length;
		double centerPercent = 0.5 + 0.5*(percent1-percent2);
		
		if (length>0.00001) {
			NormalOffset.x = offset.x/length;
			NormalOffset.y = offset.y/length;
			NormalOffset.z = offset.z/length;
		} else {
			NormalOffset.x=NormalOffset.y=NormalOffset.z=0.0f;
		}

		SetRotationMatrix(rotMat, &NormStart, &NormalOffset);

		double tmp1 = rotMat[2][1]-rotMat[1][2];
		double tmp2 = rotMat[0][2]-rotMat[2][0];
		double tmp3 = rotMat[1][0]-rotMat[0][1];
		double dev = sqrt(tmp1*tmp1+tmp2*tmp2+tmp3*tmp3);
		theta = -1 * acos((rotMat[0][0] + rotMat[1][1] + rotMat[2][2] - 1)/2);

		if (fabs(dev) > 0.0000001 ) {
			axisX = (rotMat[2][1] - rotMat[1][2]) / dev;
			axisY = (rotMat[0][2] - rotMat[2][0]) / dev;
			axisZ = (rotMat[1][0] - rotMat[0][1]) / dev;
		}

		CPoint3D v3; //first half bond from atom 1
		v3.x = centerPercent*(v2.x - v1.x)+v1.x;
		v3.y = centerPercent*(v2.y - v1.y)+v1.y;
		v3.z = centerPercent*(v2.z - v1.z)+v1.z;
		
		curAtomType = lAtoms[atom1].GetType() - 1;
		AtomColor = Prefs->GetAtomColorLoc(curAtomType);
		red = AtomColor->red/65536.0;
		green = AtomColor->green/65536.0;
		blue = AtomColor->blue/65536.0;
		
		Buffer->PutText("Transform {\n");
		
		if (fabs(dev) > 0.000001) {
			tmpStr.Printf(wxT("\trotation %lf %lf %lf %lf\n"), axisX, axisY, axisZ, theta);
			Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
		}
		tmpStr.Printf(wxT("\ttranslation %lf %lf %lf\n"), (v1.x+v3.x)/2, (v1.y+v3.y)/2, (v1.z+v3.z)/2);
		Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
		Buffer->PutText("\tchildren [\n");
		Buffer->PutText("\t\tShape {\n");
		tmpStr.Printf(wxT("\t\t\tgeometry Cylinder { radius %lf height %lf}\n"), BondSize, (centerPercent)*length);
		Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
		Buffer->PutText("\t\t\tappearance Appearance {\n");
		tmpStr.Printf(wxT("\t\t\t\tmaterial Material { diffuseColor %f %f %f }\n"),
					  red, green, blue);
		Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
		Buffer->PutText("\t\t\t}\n");
		Buffer->PutText("\t\t}\n");
		Buffer->PutText("\t]\n");
		Buffer->PutText("}\n");
		
		curAtomType = lAtoms[atom2].GetType() - 1;
		AtomColor = Prefs->GetAtomColorLoc(curAtomType);
		red = AtomColor->red/65536.0;
		green = AtomColor->green/65536.0;
		blue = AtomColor->blue/65536.0;
		
		Buffer->PutText("Transform {\n");
		
		if (fabs(dev) > 0.000001) {
			tmpStr.Printf(wxT("\trotation %lf %lf %lf %lf\n"), axisX, axisY, axisZ, theta);
			Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
		}
		tmpStr.Printf(wxT("\ttranslation %lf %lf %lf\n"), (v2.x+v3.x)/2, (v2.y+v3.y)/2, (v2.z+v3.z)/2);
		Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
		Buffer->PutText("\tchildren [\n");
		Buffer->PutText("\t\tShape {\n");
		tmpStr.Printf(wxT("\t\t\tgeometry Cylinder { radius %lf height %lf}\n"), BondSize, (1-centerPercent)*length);
		Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
		Buffer->PutText("\t\t\tappearance Appearance {\n");
		tmpStr.Printf(wxT("\t\t\t\tmaterial Material { diffuseColor %f %f %f }\n"),
					  red, green, blue);
		Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
		Buffer->PutText("\t\t\t}\n");
		Buffer->PutText("\t\t}\n");
		Buffer->PutText("\t]\n");
		Buffer->PutText("}\n");
	}
}

void MolDisplayWin::WritePOVFile(BufferFile *Buffer) {

	MainData->ExportPOV(Buffer, Prefs);

}

void General2DSurface::ReadGrid(const bool Square, const bool UseMult, const double & MultValue) {
    wxString filename = wxFileSelector(wxT("Choose a file containing the surface data."));
    //We are looking for $ VEC groups. Scan to see how many are there. If more than 1 the user will
    //have to choose.
	FILE * myfile = NULL;
    if (!filename.empty()) {
        myfile = fopen(filename.mb_str(wxConvUTF8), "rb");
        if (myfile == NULL) {
            MessageAlert("Unable to open the selected file!");
			return;
        }
	} else
		return;
	bool FirstFile = (Grid == NULL);
	
//Ok got a file, create a buffer for it and then attempt to read it in
	if (FirstFile) {
		if (Label) {delete [] Label; Label = NULL;}
		FreeGrid();
	}

	BufferFile * Buffer = NULL;

	bool	success = false;
	long	LineLength, LinePos;
	short	scanerr;
	CPoint3D	tempPt;
	try {
			char Line[kMaxLineLength];
		Buffer = new BufferFile(myfile, false);
		Buffer->GetLine(Line);
		if (FirstFile) SetLabel(Line);
		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%ld", &LinePos);
		if ((scanerr != 1)||(LinePos<=0)) {
			wxLogMessage(_("The second line must contain the # of grid points."));
			throw DataError();
		}
		if (FirstFile) NumGridPoints = LinePos;
		else if (LinePos != NumGridPoints) {
			wxLogMessage(_("The number of grid points does not match the existing grid!"));
			throw DataError();
		}
		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%f%f%f", &(tempPt.x), &(tempPt.y), &(tempPt.z));
		if (scanerr != 3) {
			wxLogMessage(_("Could not parse the x, y, and z values for the origin of the 3D grid."));
			throw DataError();
		}
		if (FirstFile) Origin = tempPt;
		else if ((fabs(tempPt.x) < fabs(100*(tempPt.x-Origin.x)))||
				(fabs(tempPt.y) < fabs(100*(tempPt.y-Origin.y)))||
				 (fabs(tempPt.z) < fabs(100*(tempPt.z-Origin.z)))) {
					wxLogMessage(_("The origin of the file grid does not match the current origin!"));
					throw DataError();
		}
		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%f%f%f", &(tempPt.x), &(tempPt.y), &(tempPt.z));
		if (scanerr != 3) {
			wxLogMessage(_("The fourth line must contain a 3D vector indicating the increment along the first side of the grid."));
			throw DataError();
		}
		if (FirstFile) XInc = tempPt;
		else if ((fabs(tempPt.x) < fabs(100*(tempPt.x-XInc.x)))||
				(fabs(tempPt.y) < fabs(100*(tempPt.y-XInc.y)))||
				 (fabs(tempPt.z) < fabs(100*(tempPt.z-XInc.z)))) {
					wxLogMessage(_("The first increment vector does not match the current grid!"));
					throw DataError();
		}
		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%f%f%f", &(tempPt.x), &(tempPt.y), &(tempPt.z));
		if (scanerr != 3) {
			wxLogMessage(_("The fifth line must contain a 3D vector indicating the increment along the second grid direction."));
			throw DataError();
		}
		if (FirstFile) YInc = tempPt;
		else if ((fabs(tempPt.x) < fabs(100*(tempPt.x-YInc.x)))||
				(fabs(tempPt.y) < fabs(100*(tempPt.y-YInc.y)))||
				 (fabs(tempPt.z) < fabs(100*(tempPt.z-YInc.z)))) {
					wxLogMessage(_("The second increment vector does not match the current grid!"));
					throw DataError();
		}
			//allocate memory for the grid
		long TotalPoints = NumGridPoints*NumGridPoints;
		if (FirstFile) {
			AllocateGrid(TotalPoints);
		}
		if (!Grid) throw MemoryError();
		float * lGrid;
		lGrid = Grid;
		if (FirstFile) for (LinePos=0; LinePos<TotalPoints; ++LinePos)
			lGrid[LinePos] = 0.0;
		Buffer->GetLine(Line);
		LinePos = 0;
		LineLength = strlen(Line);
		long n=0;
		float tempF;
		GridMax = -1.0e20;	//init the Grid max to a value sure to be changed later
		GridMin = 1.0e20;
		while (n<TotalPoints) {
			if (LinePos>=LineLength) {
				Buffer->GetLine(Line);
				LinePos = 0;
				LineLength = strlen(Line);
			}
				int nchar;
			scanerr = sscanf(&(Line[LinePos]), "%f%n", &tempF, &nchar);
			if (scanerr != 1) {
				Buffer->GetLine(Line);
				LinePos = 0;
				LineLength = strlen(Line);
				scanerr = sscanf(&(Line[LinePos]), "%f%n", &tempF, &nchar);
			}
			if (Square) tempF *= tempF;
			if (UseMult) tempF *= MultValue;
			lGrid[n] += tempF;
			GridMax = MAX(GridMax, lGrid[n]);
			GridMin = MIN(GridMin, lGrid[n]);
			LinePos += nchar;
			++n;
		}

		success = true;
	}
	catch (DataError /*Error*/) {}
	catch (FileError Error) { Error.WriteError();}
	catch (...) {
		success = false;
	}
	if (!success && FirstFile) {	//invalid data or out of memory, etc
		if (Label) {delete [] Label; Label = NULL;}
		FreeGrid();
	}
	if (Buffer) delete Buffer;
	fclose(myfile);
}
void General3DSurface::ReadGrid(const bool Square, const bool UseValue, const double & MultValue) {
	//First prompt the user for the file
    wxString filename = wxFileSelector(wxT("Choose a file containing the surface data."));
	FILE * myfile = NULL;
    if (!filename.empty()) {
        myfile = fopen(filename.mb_str(wxConvUTF8), "rb");
        if (myfile == NULL) {
            MessageAlert("Unable to open the selected file!");
			return;
        }
	} else
		return;

	bool FirstFile = (Grid == NULL);

//Ok got a file, create a buffer for it and then attempt to read it in
	if (FirstFile) {	//shouldn't have old data if the first file...
		if (Label) {delete [] Label; Label = NULL;}
		if (Grid) FreeGrid();
		if (ContourHndl) FreeContour();
	}

	BufferFile * Buffer = NULL;

	bool	success = false;
	long	LineLength, LinePos, tempL;
	CPoint3D	tempPt;
	short	scanerr;
	try {
			char Line[kMaxLineLength];
		Buffer = new BufferFile(myfile, false);
		Buffer->GetLine(Line);
		if (FirstFile) SetLabel(Line);	//grab the label only if this is the first data file

		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%ld%ld%ld", &LineLength, &LinePos, &tempL);
		if ((scanerr != 3)||(LineLength<=0)||(LinePos<=0)||(tempL<=0)) {
			wxLogMessage(_("The second line must contain the # of x, y, and z grid points."));
			throw DataError();
		}
		if (FirstFile) {
			NumXGridPoints = LineLength;
			NumYGridPoints = LinePos;
			NumZGridPoints = tempL;
		} else if ((NumXGridPoints!=LineLength)||(NumYGridPoints!=LinePos)||
				   (NumZGridPoints!=tempL)) {
			wxLogMessage(_("The number of grid points does not match the existing grid!"));
			throw DataError();
		}

		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%f%f%f", &(tempPt.x), &(tempPt.y), &(tempPt.z));
		if (scanerr != 3) {
			wxLogMessage(_("Could not parse the x, y, and z values for the origin of the 3D grid."));
			throw DataError();
		}
		if (FirstFile) Origin = tempPt;
		else if ((fabs(tempPt.x) < fabs(100*(tempPt.x-Origin.x)))||
				(fabs(tempPt.y) < fabs(100*(tempPt.y-Origin.y)))||
				 (fabs(tempPt.z) < fabs(100*(tempPt.z-Origin.z)))) {
					wxLogMessage(_("The origin of the file grid does not match the current origin!"));
					throw DataError();
		}

		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%f%f%f", &(tempPt.x), &(tempPt.y), &(tempPt.z));
		if (scanerr != 3) {
			wxLogMessage(_("Could not parse the x, y, and z increment values from the fourth line."));
			throw DataError();
		}
		if (FirstFile) {
			XGridInc = tempPt.x;
			YGridInc = tempPt.y;
			ZGridInc = tempPt.z;
		} else if ((fabs(tempPt.x) < fabs(100*(tempPt.x-XGridInc)))||
				(fabs(tempPt.y) < fabs(100*(tempPt.y-YGridInc)))||
				   (fabs(tempPt.z) < fabs(100*(tempPt.z-ZGridInc)))) {
					wxLogMessage(_("The x, y, and z grid increments must match those of the existing grid."));
					throw DataError();
		}
			//allocate memory for the grid (if needed)
		long TotalPoints = NumXGridPoints*NumYGridPoints*NumZGridPoints;
		if (FirstFile) AllocateGrid(TotalPoints);
		if (!Grid) throw MemoryError();
			float * lGrid;
		lGrid = Grid;
		if (FirstFile) for (LinePos=0; LinePos<TotalPoints; ++LinePos)
			lGrid[LinePos] = 0.0;
		Buffer->GetLine(Line);
		LinePos = 0;
		LineLength = strlen(Line);
		long n=0;
		float tempF;
		GridMax = -1.0e20;	//init the Grid max to a value sure to be changed later
		GridMin = 1.0e20;
		while (n<TotalPoints) {
			if (LinePos>=LineLength) {
				Buffer->GetLine(Line);
				LinePos = 0;
				LineLength = strlen(Line);
			}
				int nchar;
			scanerr = sscanf(&(Line[LinePos]), "%f%n", &tempF, &nchar);
			if (scanerr != 1) {
				Buffer->GetLine(Line);
				LinePos = 0;
				LineLength = strlen(Line);
				scanerr = sscanf(&(Line[LinePos]), "%f%n", &tempF, &nchar);
			}
			if (Square) tempF *= tempF;
			if (UseValue) tempF *= MultValue;
			lGrid[n] += tempF;
			GridMax = MAX(GridMax, lGrid[n]);
			GridMin = MIN(GridMin, lGrid[n]);
			LinePos += nchar;
			++n;
		}

		success = true;
	}
	catch (DataError /*Error*/) {}
	catch (FileError Error) { Error.WriteError();}
	catch (...) {
		success = false;
	}
	if (!success && FirstFile) {	//invalid data or out of memory, etc
		if (Label) {delete [] Label; Label = NULL;}
		if (Grid) FreeGrid();
	}
	if (Buffer) delete Buffer;
	fclose(myfile);
}
long LocateKeyWord(const char *Buffer, const char * KeyWord, long length, long bytecount)
{	long	test=0, pos=-1;

	while (!test) {
		for (pos++; ((toupper(Buffer[pos]) != toupper(KeyWord[0]))||
					 (toupper(Buffer[pos+1]) != toupper(KeyWord[1])))&&(pos<bytecount)&&
			 (Buffer[pos]!='\0'); pos++) ;
		if (pos>=bytecount) return -1;
		if (Buffer[pos]=='\0') return -1;
		test = 2;
		while ((toupper(Buffer[pos+test]) == toupper(KeyWord[test]))&&(test<length)&&
			   ((pos+test)<bytecount)) test++;
		test = (long) test==length;
	}
	return pos;
}	/*LocateKeyWord*/
/* ParseGLogLine converts the various cartesian coordinate formats in
	GAMESS output files to my internal binary coordinate array. Type's
	less than ten mean coordinates are in Bohr and thus need conversion,
	greater than ten means they are already in Angstroms. */
long ParseGLogLine(BufferFile * Buffer, Frame * lFrame, long numExpected, long Type, float *maxsize)
{	long	LinePos=0, iscanerr=0, iatm;
	float	temp;
	char	LineText[kMaxLineLength+1];
	CPoint3D	Position;
	
	for (iatm=0; iatm<numExpected; ++iatm) {
		Buffer->GetLine(LineText);
		switch (Type) {
			case 0:	//Standard Coordinates at the top of the log file
			case 10: // IRC Log file format (same as above but in angstroms)
					//1/1998 changed standard log format to 10
				LinePos = 11;	//Skip the name and space before the nuclear charge
			break;
			case 1: //format used during geometry optimizations
				LinePos = 15;	//skip # and name before the nuclear charge
			break;
			case 2: //DRC irc and log file format, atom labels are missing
				LinePos = 1;
			break;
			default:
				LinePos = 0;
		}
		iscanerr = sscanf(&(LineText[LinePos]), "%f%f%f%f", &temp, &Position.x,
			&Position.y, &Position.z);
		if (iscanerr == 4) {	/*Convert the coordinates to Angstroms*/
			if (Type < 10) {
				Position.x *= kBohr2AngConversion;
				Position.y *= kBohr2AngConversion;
				Position.z *= kBohr2AngConversion;
			}
			*maxsize = MAX(*maxsize, Position.x);
			*maxsize = MAX(*maxsize, Position.y);
			*maxsize = MAX(*maxsize, Position.z);
			if (temp == 0) temp = 115;
			lFrame->AddAtom((long) temp, Position);
		} else break;
	}
	return iatm;
} /*ParseGLogLine*/
long ParseCartLine(char *Line, long *atomtype, CPoint3D *coord, CPoint3D *offset, long Mode)
{	long	iscanerr=0, junk;
	unsigned char Label[kMaxLineLength]="\0";

	if (!sscanf(Line, "%s", Label)) return -1;
	if ((Label[0] > 47)&&(Label[0] < 58)) {	/*A number? must be Gaussian style (bleah)*/
			float arg6;
		iscanerr = sscanf(Line, "%ld%ld%f%f%f%f",&junk, atomtype, &(coord->x),
			&(coord->y), &(coord->z), &arg6);
			//G98 adds an integer column for the 'atomic type' which is normally 0???
		if (iscanerr == 6) {
			coord->x = coord->y;
			coord->y = coord->z;
			coord->z = arg6;
		}
		if ((*atomtype>130)||(*atomtype<1))	//bad atom type: assume whole line is garbage
			return -1;
		if ((iscanerr != 5)&&(iscanerr != 6)) return -iscanerr;
	} else {
		float	tempx, tempy, tempz;

		iscanerr = sscanf(Line, "%s%f%f%f%f%f%f", Label, &(coord->x), 
			&(coord->y), &(coord->z), &tempx, &tempy, &tempz);

		if (iscanerr == 7) {
			*atomtype = SetAtomType(Label);
			if (*atomtype == 116) {
				offset->x = tempx*kDebyeToAU;	/*convert the units from Debye to AU for better viewing*/
				offset->y = tempy*kDebyeToAU;
				offset->z = tempz*kDebyeToAU;
			} else if (*atomtype == 117) {
				coord->x *= kBohr2AngConversion;
				coord->y *= kBohr2AngConversion;
				coord->z *= kBohr2AngConversion;
				offset->x = tempx*kDebyeToAU;	/*convert the units from Debye to AU for better viewing*/
				offset->y = tempy*kDebyeToAU;
				offset->z = tempz*kDebyeToAU;
				*atomtype = 116;
			} else if (Mode==-1) {	// Normal mode is not mass-weighted here!
				offset->x = tempx;
				offset->y = tempy;
				offset->z = tempz;
			}
		} else {			
			if (iscanerr == 5) {	/*looks like GAMESS unit=bohr format --  label  Atomic_num  x, y, z*/
				*atomtype = (long) coord->x;
				coord->x = coord->y;
				coord->y = coord->z;
				coord->z = tempx;
			} else if (iscanerr == 4) {
					//Check for Fragment coords - I "think" a fragment is ZLabel#
				if (Label[0] == 'Z' || Label[0] == 'z') {
					if ((Label[2]!=0)||((Label[1]!='R')&&(Label[1]!='r')&&
						(Label[1]!='N')&&(Label[1]!='n'))) {
						Label[0] = Label[1];
						Label[1] = 0;
					}
				}
				*atomtype = SetAtomType(Label);
			} else return iscanerr;	//return the error code
		}
	}
	return iscanerr;
} /*ParseCartLine*/
long SetAtomType(const unsigned char *TestLabel) {
	char	label[2];
	long result = -1;

	label[0] = TestLabel[0];	label[1]=TestLabel[1];
	if ((label[0]>96)&&(label[0]<123)) label[0] = label[0] - 32;	/* Make the first character upper case */
	if ((label[1]>64)&&(label[1]<91)) label[1] = label[1] + 32;		/* and the second character lower case */
	if ((label[1]<'a')||(label[1]>'z')) label[1] = ' ';	//Not a letter!
	switch (label[0]) {
		case 'H':
			switch (label[1]) {
				case 'e':			/* Helium */
					result = 2;
					break;
				case 'o':			/* Holmium */
					result = 67;
					break;
				case 'f':			/* Hafnium */
					result = 72;
					break;
				case 'g':			/* Mercury */
					result = 80;
					break;
				case 's':
					result = 108;
					break;
				default:			/* Hydrogen */
					result = 1;
					break;
			}
			break;
		case 'L':
			switch (label[1]) {
				case 'i':			/* Lithium */
					result = 3;
					break;
				case 'a':			/* Lathanum */
					result = 57;
					break;
				case 'u':			/* Lutetium */
					result = 71;
					break;
				case 'r':			/* Lawrencium */
					result = 103;
					break;
				case 'v':			/* Livermorium */
					result = 116;
					break;
			}
			break;
		case 'B':
			switch (label[1]) {
				case 'e':			/* Beryllium */
					result = 4;
					break;
				case 'r':			/* Bromine */
					result = 35;
					break;
				case 'a':			/* Barium */
					result = 56;
					break;
				case 'i':			/* Bismuth */
					result = 83;
					break;
				case 'k':			/* Berkelium */
					result = 97;
					break;
				case 'h':			//Bh
					result = 107;
					break;
				default:			/* Boron */
					result = 5;
					break;
			}
			break;
		case 'C':
			switch (label[1]) {
				case 'l':			/* Chlorine */
					result = 17;
					break;
				case 'a':			/* Calcium */
					result = 20;
					break;
				case 'r':			/* Chromium */
					result = 24;
					break;
				case 'o':			/* Cobalt */
					result = 27;
					break;
				case 'u':			/* Copper */
					result = 29;
					break;
				case 'd':			/* Cadmium */
					result = 48;
					break;
				case 's':			/* Cesium */
					result = 55;
					break;
				case 'e':			/* Cerium */
					result = 58;
					break;
				case 'm':			/* Curium */
					result = 96;
					break;
				case 'f':			/* Californium */
					result = 98;
					break;
				case 'n':			/* Copernicium */
					result = 112;
					break;
				default:			/* Carbon */
					result = 6;
					break;
			}
			break;
		case 'N':
			switch (label[1]) {
				case 'e':			/* Neon */
					result = 10;
					break;
				case 'a':			/* Sodium */
					result = 11;
					break;
				case 'i':			/* Nickel */
					result = 28;
					break;
				case 'b':			/* Niobium */
					result = 41;
					break;
				case 'd':			/* Neodymium */
					result = 60;
					break;
				case 'p':			/* Neptunium */
					result = 93;
					break;
				case 'o':			/* Nobelium */
					result = 102;
					break;
				default:			/* Nitrogen */
					result = 7;
					break;
			}
			break;
		case 'O':
			switch (label[1]) {
				case 's':			/* Osmium */
					result = 76;
					break;
				default:			/* Oxygen */
					result = 8;
					break;
			}
			break;
		case 'F':
			switch (label[1]) {
				case 'e':			/* Iron */
					result = 26;
					break;
				case 'r':			/* Francium */
					result = 87;
					break;
				case 'm':			/* Fermium */
					result = 100;
					break;
				case 'l':			/* Flerovium */
					result = 114;
					break;
				default:			/* Fluorine */
					result = 9;
					break;
			}
			break;
		case 'M':
			switch (label[1]) {
				case 'g':			/* Magnesium */
					result = 12;
					break;
				case 'n':			/* Manganese */
					result = 25;
					break;
				case 'o':			/* Molybdenum */
					result = 42;
					break;
				case 'd':			/* Mendelevium */
					result = 101;
					break;
				case 't':
					result = 109;
					break;
			}
			break;
		case 'A':
			switch (label[1]) {
				case 'l':			/* Aluminum */
					result = 13;
					break;
				case 'r':			/* Argon */
					result = 18;
					break;
				case 's':			/* Arsenic */
					result = 33;
					break;
				case 'g':			/* Silver */
					result = 47;
					break;
				case 'u':			/* Gold */
					result = 79;
					break;
				case 't':			/* Astatine */
					result = 85;
					break;
				case 'c':			/* Actinium */
					result = 89;
					break;
				case 'm':			/* Americium */
					result = 95;
					break;
			}
			break;
		case 'S':
			switch (label[1]) {
				case 'i':			/* Silicon */
					result = 14;
					break;
				case 'c':			/* Scandium */
					result = 21;
					break;
				case 'e':			/* Selenium */
					result = 34;
					break;
				case 'r':			/* Strontium */
					result = 38;
					break;
				case 'n':			/* Tin */
					result = 50;
					break;
				case 'b':			/* Antimony */
					result = 51;
					break;
				case 'm':			/* Samarium */
					result = 62;
					break;
				case 'g':			/* Seaborgium */
					result = 106;
					break;
				default:			/* Sulfur */
					result = 16;
					break;
			}
			break;
		case 'P':
			switch (label[1]) {
				case 'd':			/* Palladium */
					result = 46;
					break;
				case 'r':			/* Praseodymium */
					result = 59;
					break;
				case 'm':			/* Promethium */
					result = 61;
					break;
				case 't':			/* Platinum */
					result = 78;
					break;
				case 'b':			/* Lead */
					result = 82;
					break;
				case 'o':			/* Polonium */
					result = 84;
					break;
				case 'a':			/* Protoactinium */
					result = 91;
					break;
				case 'u':			/* Plutonium */
					result = 94;
					break;
				default:			/* Phosphorus */
					result = 15;
					break;
			}
			break;
		case 'K':
			switch (label[1]) {
				case 'r':			/* Krypton */
					result = 36;
					break;
				default:			/* Potassium */
					result = 19;
					break;
			}
			break;
		case 'T':
			switch (label[1]) {
				case 'i':			/* Titanium */
					result = 22;
					break;
				case 'c':			/* Technetium */
					result = 43;
					break;
				case 'e':			/* Tellurium */
					result = 52;
					break;
				case 'b':			/* Terbium */
					result = 65;
					break;
				case 'm':			/* Thullium */
					result = 69;
					break;
				case 'a':			/* Tantalum */
					result = 73;
					break;
				case 'l':			/* Thallium */
					result = 81;
					break;
				case 'h':			/* Thorium */
					result = 90;
					break;
			}
			break;
		case 'V':					/* Vanadium */
					result = 23;
					break;
		case 'Z':
			switch (label[1]) {
				case 'n':			/* Zinc */
					result = 30;
					break;
				case 'r':			/* Zirconium */
					result = 40;
					break;
			}
			break;
		case 'G':
			switch (label[1]) {
				case 'a':			/* Gallium */
					result = 31;
					break;
				case 'e':			/* Germanium */
					result = 32;
					break;
				case 'd':			/* Gadolinium */
					result = 64;
					break;
			}
			break;
		case 'R':
			switch (label[1]) {
				case 'b':			/* Rubidium */
					result = 37;
					break;
				case 'u':			/* Ruthenium */
					result = 44;
					break;
				case 'g':
					result = 111;	//Roentgenium
					break;
				case 'h':			/* Rhodium */
					result = 45;
					break;
				case 'e':			/* Rhenium */
					result = 75;
					break;
				case 'n':			/* Radon */
					result = 86;
					break;
				case 'a':			/* Radium */
					result = 88;
					break;
				case 'f':			/* Rutherfordium */
					result = 104;
					break;
			}
			break;
		case 'Y':
			switch (label[1]) {
				case 'b':			/* Ytterbium */
					result = 70;
					break;
				default:			/* Yttrium */
					result = 39;
					break;
			}
			break;
		case 'I':
			switch (label[1]) {
				case 'n':			/* Indium */
					result = 49;
					break;
				case 'r':			/* Iridium */
					result = 77;
					break;
				default:			/* Iodine */
					result = 53;
					break;
			}
			break;
		case 'X':					/* Xenon */
					result = 54;
					break;
		case 'E':
			switch (label[1]) {
				case 'u':			/* Europium */
					result = 63;
					break;
				case 'r':			/* Erbium */
					result = 68;
					break;
				case 's':			/* Einsteinium */
					result = 99;
					break;
			}
			break;
		case 'D':
			switch (label[1]) {
				case 'y':/* Dysprosium */
					result = 66;
					break;
				case 'b':	/* Dubnium */
					result = 105;
					break;
				case 's':	//Darmstadtium
					result = 110;
					break;
				default:	//Deuterium
					result = 1;
					break;
			}
			break;
		case 'W':					/* Tungsten */
					result = 74;
					break;
		case 'U':					/* Uranium */
					result = 92;
					break;
		case '*':	/*special atom types for special vectors*/
			switch (label[1]) {
				case 'c':	/*Charge centroids with dipole vectors*/
					if (TestLabel[2]=='B' || TestLabel[2]=='b')
						result = 117;
					else
						result = 116;
					break;
		/*		case 'd':	The next two are not yet implemented
					result = 117;
					break;
				case 'p':
					result = 118;
					break;	*/
				case 'v':	/*arbitrary vectors*/
					result = 119;
					break;
				default:	/*This should only be used for *# cases*/
					sscanf((char *)&(label[1]), "%ld", &result);
			}
			break;
	}
	return result;
} /* SetAtomType */

/**
 * This function opens the file specified by file_name and copies the contents
 * to a string.
 * @param file_name Name of file to read
 * @param contents String to write contents to
 * @\return True 
*/
bool FileToString(const std::string& file_name,
				  std::string& contents) {

	std::ifstream f(file_name.c_str(), std::ios::in | std::ios::ate);
	if (!f.is_open()) {
		return false;
	}

	int size;

	// Get file size.
	size = (int) f.tellg();
	f.seekg(0, std::ios::beg);

	// Allocate buffer, read contents in.
	char *cstr = new char[size];
	f.read(cstr, size);
	f.close();
	contents = std::string(cstr, size);
	delete[] cstr;

	return true;

}
