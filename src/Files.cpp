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
*/

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
#include "InputData.h"
#include "Prefs.h"
#include <string.h>
#include <stdio.h>
#include <cctype>
#include <new>

extern WinPrefs *	gPreferences;
#ifndef __wxBuild__
extern Boolean		gQ3DAvail, gOpenGLAvailable;
#endif

	//Local function definitions
bool ReadGVBOccupancy(BufferFile * Buffer, long NumPairOrbs, long MaxOrbs, float * Occupancy);

void FileError::WriteError(void) {
#ifdef __wxBuild__
#ifndef WIN32
#warning Need warning string replacement
#endif
#else
	Str255	errmsg;

	SetCursorToArrow();
	if (Error == eofErr) {
		GetIndString(errmsg, kerrstrings, 36);
	} else {
		GetIndString(errmsg, kerrstrings, 35);
		sprintf((char *)&(errmsg[errmsg[0]+1]), " %d", Error);
	}
	MessageAlert(errmsg);
#endif
}
void DataError::WriteError(void) {
#ifdef __wxBuild__
#ifndef WIN32
#warning Need warning string replacement
#endif
#else
	SetCursorToArrow();
	if (ErrNum>0) {
		Str255	errmsg;
		GetIndString(errmsg, kDataErrMsgs, ErrNum);
		MessageAlert(errmsg);
	}
#endif
};

#ifndef __wxBuild__
void OpenMolPFile(FSSpec *myFile) {
	OSErr				myerr;
	Rect				tRect;
	bool				NeedRect=true;

	SpinCursor(0);
		//First attempt to get the window Rect from the resource fork
		//otherwise get it from the end of the data fork
		//the resource fork is used in files written later than v3.0.1
	short ResFileRef = FSpOpenResFile(myFile, fsRdPerm);
	myerr = ResError();	//check for errors
	if (myerr==noErr) {
		myerr = myReadPreference(ResFileRef, ResFileRef, (ResType) 'RECT', 1, (Ptr) &tRect, sizeof(Rect));
		if (myerr == noErr) NeedRect = false;	//Got the resource!
	} else ResFileRef = 0;

	MolDisplayWin * Window = NULL;
	if (!NeedRect) Window = new MolDisplayWin(myFile->name, &tRect);
	else Window = new MolDisplayWin(myFile->name, NULL);
	if (Window == NULL) return;
	Window->ShowWindow();
	Window->SetFileSpec(myFile, 0);
	SpinCursor(0);

	Window->ReadMolPFile(ResFileRef);

	if (ResFileRef) CloseResFile(ResFileRef);

	SetCursorToArrow();					/* reset the cursor before returning */
} /*OpenMolPFile*/
void MolDisplayWin::ReadMolPFile(short ResFileRef) {
	if (ResFileRef) {	//attempt to read in the files preferences
		Prefs->ReadFilePrefs(ResFileRef);
		if (gQ3DAvail || gOpenGLAvailable) {
			//Attempt to read the QD3D state
				Boolean t;
			OSErr myerr = myReadPreference(ResFileRef, ResFileRef, (ResType) 'QD3P', 128, (Ptr) &t, sizeof(Boolean));
			if ((myerr == noErr)&&(t)) winData.is3DModeActive(true);
		}
	}
		short vreftemp;
	OSErr myerr = FSpOpenDF(&fileFSSpec, fsRdPerm, &vreftemp);	/* Open the data fork */
	if (myerr != noErr) {
		AbortOpen(0);
		return;		// abort if there is an error opening the file
	}
	fileRefNum = vreftemp;

	BufferFile * lBuffer = NULL;
	bool Success = false;
	try {
		lBuffer = new BufferFile(vreftemp, false);
		SpinCursor(0);
		if (!MainData->UnPackData(lBuffer)) { /*the unpack failed so close the window*/
				Str255	tempStr;
			Close();
			GetIndString(tempStr, kerrstrings, 21);
			MessageAlert(tempStr);
			return;
		}
		SpinCursor(0);
		Success = true;
	}
	catch (std::bad_alloc) {
		Close();
		MessageAlert("\pInsufficient memory to read in the file.");
	}
	catch (MemoryError) {
		Close();
		MessageAlert("\pInsufficient memory to read in the file.");
	}
	catch (DataError) {
		Close();
		MessageAlert("\pInvalid data encountered while reading the file.");
	}
	catch (FileError) {
		Close();
		MessageAlert("\pFile System error, read aborted.");
	}
	if (lBuffer) delete lBuffer;
	if (Success) {
		if (FileSave & 1) FileSave -= 1;
		if (!(FileSave & 2)) FileSave += 2;
			//Initialize QuickDraw3D if neccessary
#ifdef QuickDraw3D
		if (gQ3DAvail) {
			if (QD3DActive) Q3DInitWindow();
		}
#endif
#ifdef UseOpenGL
		if (gOpenGLAvailable) {
			if (winData.is3DModeActive()) OpenGLInitWindow();
		}
#endif
		ResetModel(false);
	}
}

long MoleculeData::UnPackData(BufferFile * Buffer) {
	long		length=0, code;
	char		Version;
	
	if (Frames) delete Frames;
	cFrame = Frames = NULL;
	Buffer->Read((Ptr) &length, sizeof(long));
	if (length != 'BMBm') throw DataError();
	Buffer->Read(&Version, 1);
	if (Version < 2) throw DataError();
	if (Version == 2) {
		return UnPackOldData(Buffer);
	}
	if (Version > 5) throw DataError();
	Buffer->Read((Ptr) &code, sizeof(long));
	Buffer->Read((Ptr) &length, sizeof(long));
	if (code == 1)	// The first item must be the Main Data struct
		ConvertMainWinData1(Buffer, length);
	else if (code == 14)
		ConvertMainWinData14(Buffer, length);
	else if (code == 40) {
		if (length != sizeof(MoleculeData)) throw DataError(); //The size of the struct had better be right!
		Buffer->Read((Ptr) this, length);
	} else throw DataError();
	cFrame = Frames = NULL;		//zero out all pointers that were just read in and thus are invalid
	RotCoords = NULL;
	zBuffer = NULL;
	Description = NULL;
	Basis = NULL;
	IntCoords = NULL;
	InputOptions = NULL;
	NumFrames = 0;

	Frame * lFrame = cFrame = Frames = new Frame;
	if (!lFrame) throw MemoryError();
	Buffer->Read((Ptr) &code, sizeof(long));
	if (code == 2) lFrame->ConvertFrameCode2(Buffer);
	else if (code == 41) {
		Buffer->Read((Ptr) &length, sizeof(long));
		lFrame->Read41(Buffer, length);
	} else if (code == 54) {
		Buffer->Read((Ptr) &length, sizeof(long));
		lFrame->Read(Buffer, length);
	} else throw DataError();	// Next must come a Frame struct
	NumFrames = 1;
	MaxAtoms = MAX(MaxAtoms, lFrame->NumAtoms);
	Buffer->Read((Ptr) &code, sizeof(long));
	while (lFrame) {
		while ((code>0)&&(code!=2)&&(code!=41)&&(code!=54)) { //process until the next frame or end of file is reached
			Buffer->Read((Ptr) &length, sizeof(long));
			Buffer->SetBlockLength(length);
			switch (code) {
				case 3:
					lFrame->ConvertCode3(Buffer, length);
				break;
				case 4:
					lFrame->ConvertCode4(Buffer, length);
				break;
				case 5:
					lFrame->ConvertCode5(Buffer, length);
				break;
				case 42:
					if (length != lFrame->NumAtoms*sizeof(Atom)) throw DataError();
					Buffer->Read((Ptr) (lFrame->Atoms), length);
				break;
				case 43:
					if (length != lFrame->NumBonds*sizeof(Bond)) throw DataError();
					Buffer->Read((Ptr) (lFrame->Bonds), length);
				break;
				case 6:
				case 44:
					if (length == lFrame->NumAtoms*sizeof(CPoint3D)) {
						if (! lFrame->SpecialAtoms) {
							lFrame->SpecialAtoms = new CPoint3D[lFrame->NumAtoms];
							if (!lFrame->SpecialAtoms) throw MemoryError();
						}
						Buffer->Read((Ptr) (lFrame->SpecialAtoms), length);
					}
				break;
				case 7:
				case 45:
					lFrame->Vibs = VibRec::ReadOldVibRec45(Buffer);
				break;
				case 8:
				case 46:
					if (lFrame->Vibs) lFrame->Vibs->ReadCode46(Buffer, length);
				break;
				case 9:
				case 47:
					if (lFrame->Vibs) lFrame->Vibs->ReadCode47(Buffer, lFrame->NumAtoms, length);
				break;
				case 56:	//new style vibrec
					lFrame->Vibs = VibRec::Read(Buffer, lFrame->NumAtoms);
				break;
				case 57:
					if (Basis) {
						OrbitalRec * OrbSet = OrbitalRec::Read(Buffer, Basis->GetNumBasisFuncs(false), length);
						if (OrbSet != NULL) lFrame->Orbs.push_back(OrbSet);
					}
				break;
				case 10:	//Read in old orbitals
					ReadMORec10(Buffer, length);
				break;
				case 48:
					if (Basis) {
						ReadMORec48(Buffer, Basis->GetNumBasisFuncs(false), length);
					}
				break;
				case 49:
					lFrame->ReadSurfaceList(Buffer, length);
				break;
				case 12:
					ReadRunInfoRec(Buffer, length);
				break;
				case 13:
					ReadRunTitle(Buffer, length);
				break;
				case 50:
					IntCoords = IntCoords->Read(Buffer);
				break;
				case 51:
					if (!Basis)
						Basis = Basis->Read(Buffer, 0, length);
					else throw DataError();
				break;
				case 52:
					Description = new char[length];
					if (!Description) throw MemoryError();
					Buffer->Read((Ptr) Description, length);
				break;
				case 15:	//Input Options
				case 53:
					if (InputOptions) delete InputOptions;
					InputOptions = new InputData;
					if (InputOptions) 
						InputOptions->ReadFromBuffer(Buffer, length);
				break;
				case 55:
					lFrame->Gradient = GradientData::Read(Buffer);
				break;
			}
			Buffer->FinishBlock();
			Buffer->Read((Ptr) &code, sizeof(long));
		}
		Frame * lpFrame = lFrame;	lFrame = NULL;
		if ((code == 2)||(code == 41)||(code == 54)) {
			lFrame = new Frame;
			if (!lFrame) throw MemoryError();
			lpFrame->NextFrame = lFrame;
			lFrame->PreviousFrame = lpFrame;
			NumFrames ++;
		}
		if (code == 2) lFrame->ConvertFrameCode2(Buffer);
		else if (code == 41) {
			Buffer->Read((Ptr) &length, sizeof(long));
			lFrame->Read41(Buffer, length);
		} else if (code == 54) {
			Buffer->Read((Ptr) &length, sizeof(long));
			lFrame->Read(Buffer, length);
		}
		if ((code == 2)||(code == 41)||(code == 54)) {
			MaxAtoms = MAX(MaxAtoms, lFrame->NumAtoms);
			Buffer->Read((Ptr) &code, sizeof(long));
				//copy over the next/previous ptr after the read operation
			lFrame->PreviousFrame = lpFrame;
			lFrame->NextFrame = NULL;			
		}
	}	// Done reading the buffer
	length = CurrentFrame;
	CurrentFrame = 1;
	while (CurrentFrame < length) {
		if (cFrame->NextFrame) cFrame = cFrame->NextFrame;
		else break;
		CurrentFrame ++;
	}

	RotCoords = new CPoint3D[MaxAtoms];
	zBuffer = new long[MaxAtoms];
	if (!(RotCoords)||!(zBuffer)) return 0;
	if (Basis) {	//Check to see if the nuclear charge array needs to be generated
		if (Basis->NuclearCharge[0] == -1) {
			if (cFrame->NumAtoms == Basis->MapLength) {
				for (long iatom=0; iatom<cFrame->NumAtoms; iatom++)
					Basis->NuclearCharge[iatom] = cFrame->Atoms[iatom].GetNuclearCharge();
			}
		}
	}

	ResetRotation();
	return 1;
}

void OpenTEXTFile(FSSpec *myFile, long flip, float offset, long nSkip) {
	short				vreftemp;
	OSErr				myerr;
	long				test=0;

	SpinCursor(0);
	TextFont(systemFont);
	myerr = FSpOpenDF(myFile, fsRdPerm, &vreftemp);	/* Open the data fork */
	if (myerr != noErr) {
		MessageAlertByID(kerrstrings, 1);
		return;		// abort if there is an error opening the file
	}
	MolDisplayWin * Window = NULL;
	BufferFile * Buffer=NULL;
	try {
		Window = new MolDisplayWin(myFile->name, NULL);

		Window->ShowWindow();
		Window->SetFileSpec(myFile, vreftemp);
		Window->SetSkipPoints(nSkip);
		SpinCursor(0);

		Buffer = new BufferFile(vreftemp, false);
		if (!Buffer) throw MemoryError();

		SpinCursor(0);
				// Attempt to identify the file type by looking for key words
		TextFileType type = Buffer->GetFileType((char *) myFile->name);
		Window->BeginOperation();
		switch (type) {
			case kMolType:
				test = Window->OpenMolPltFile(Buffer);
			break;
			case kGAMESSlogType:
				test = Window->OpenGAMESSlog(Buffer, false, flip, offset);
			break;
			case kGAMESSIRCType:
				test = Window->OpenGAMESSIRC(Buffer, false,flip,offset);
			break;
			case kGAMESSDRCType:
				test = Window->OpenGAMESSDRC(Buffer, false, false,flip,offset);
			break;
			case kGAMESSInputType:
				test = Window->OpenGAMESSInput(Buffer);
			break;
			case kXYZType:
				test = Window->OpenXYZFile(Buffer);
			break;
			case kPDBType:
				test = Window->OpenPDBFile(Buffer);
			break;
			case kMDLMolFile:
				test = Window->OpenMDLMolFile(Buffer);
			break;
			case CMLFile:
			{
				test = Window->OpenCMLFile(Buffer);
				if (test == 0) Window->AbortOpen(0);
			}
			break;
			default:	//Should only get here for unknown file types.
				Window->AbortOpen(34);
		}
	}
	catch (std::bad_alloc) {//Out of memory error
		Window->AbortOpen(3);
	}
	catch (MemoryError) {
		Window->AbortOpen(3);
	}
	catch (UserCancel) {
		Window->AbortOpen(6);
	}
	catch (DataError Error) {//Error parsing the file data
		if (!Error.ErrorSet())  Window->AbortOpen(21);
		else {
			Error.WriteError();
			delete Window; Window = NULL;
		}
	}
		//Some kind of File system related error
	catch (FileError Error) { Error.WriteError(); Window->AbortOpen(-1);}
	catch (...) { Window->AbortOpen(40);}
	if (Buffer) delete Buffer;		//Done reading so free up the buffer
	if (test) {//Note test is left 0 if any exception occurs(which causes Window to be deleted)
		if (gPreferences->ChangeFileType()) {
				// Looks like this is a good file so set the creator type for the neat icon
				FInfo	myFInfo;
			HGetFInfo(myFile->vRefNum, myFile->parID, myFile->name, &myFInfo);
			if (myFInfo.fdCreator != (OSType) 'BMBm') {
				myFInfo.fdCreator = (OSType) 'BMBm';
				HSetFInfo(myFile->vRefNum, myFile->parID, myFile->name, &myFInfo);
			}
		}
			//Text files are not used after opening so close it immediately
		Window->CloseFile();	//Hmmm should this happen for CML files?
		if (!Window->IsSavedFile()) Window->SetFileType(5);
		Window->FinishOperation();	//Close the progress dialog, if opened
		if (!Window->IsSavedFile() && gPreferences->Default3DOn()) Window->Activate3D();
			//Tell the window its data has changed so that it will be redrawn correctly
		if (!Window->IsSavedFile()) Window->ResetModel(true);
	}
	SetCursorToArrow();
}	/*OpenTEXTFile*/
#endif

long MolDisplayWin::OpenGAMESSInput(BufferFile * Buffer) {
	char	Line[kMaxLineLength], token[kMaxLineLength];
	short	scanerr;
	long	nAtoms, EndOfGroup, StartPos, EndPos;
	bool	BasisFound=false, BoolTest;

	ProgressInd->ChangeText("Reading GAMESS input fileÉ");

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
	Frame * lFrame = MainData->GetCurrentFramePtr();
	if (Buffer->FindGroup("DATA")) {
		Buffer->SkipnLines(1);
		Buffer->GetLine(Line);	//1st line is a title line
		MainData->InputOptions->Data->SetTitle(Line, strlen(Line));
		Buffer->GetLine(Line);	//2nd line contains the point group
		scanerr = sscanf(Line, "%s %ld", token, &nAtoms);
		MainData->InputOptions->Data->SetPointGroup(token);
		if (scanerr == 2)	//if a number was read in set the order
			MainData->InputOptions->Data->SetPointGroupOrder(nAtoms);
			//If !C1 then skip the blank line well I guess rarely it isn't blank...
		if (-1 == FindKeyWord(token, "C1", 2)) Buffer->SkipnLines(1);
		StartPos = Buffer->GetFilePos();
		if (!Buffer->LocateKeyWord("$END", 4)) throw DataError();
		EndPos = Buffer->GetFilePos();
		if (MainData->InputOptions->Data->GetCoordType() <= 3) {
			Buffer->SetFilePos(StartPos);
			nAtoms = Buffer->GetNumLines(EndPos - StartPos) - 1;
			if (nAtoms > 0) {
				if (!MainData->SetupFrameMemory(nAtoms, 0)) throw MemoryError();
			} else {
				throw DataError(14);
			}
			while (Buffer->GetFilePos() < EndPos) {
					CPoint3D	pos;
					float		AtomType;
				Buffer->GetLine(Line);
				if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
				{ throw UserCancel();}
				sscanf(Line, "%s %f %f %f %f", token, &AtomType, &pos.x, &pos.y, &pos.z);
				lFrame->AddAtom((long) AtomType, pos);
				StartPos = Buffer->FindBlankLine();
				if ((StartPos < EndPos)&&(StartPos>-1)) {	//basis set is inlined in $DATA
					Buffer->SetFilePos(StartPos);	//just skip over it
					Buffer->SkipnLines(1);
				}
			}
			if (Prefs->GetAutoBond())	//setup bonds, if needed
				lFrame->SetBonds(Prefs, false);
		}
	}
	
	return 1;
}
long MolDisplayWin::OpenMDLMolFile(BufferFile * Buffer) {
	char Line[kMaxLineLength], partA[16], partB[16], partC[16];
	long	nAtoms, nBonds;
	short	scanerr;
// parts of this are a bit gross since the MDL format is fixed format without
// whitespace in between the fields.

	ProgressInd->ChangeText("Reading MDL MolFileÉ");
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
	if (scanerr!=2 || nAtoms <= 0) throw DataError(16);
	MainData->SetupFrameMemory(nAtoms, nBonds);
		long i;
	for (i=0; i<nAtoms; i++) {
			CPoint3D tPt;
			char	token[5];
				//Atom line format:  "%10f%10f%10f %3s"
		Buffer->GetLine(Line);
		for (int i=0; i<10; i++) partA[i] = Line[i];
		partA[10] = '\0';
		scanerr = sscanf(partA, "%10f", &tPt.x);
		for (int i=0; i<10; i++) partB[i] = Line[i+10];
		partB[10] = '\0';
		scanerr += sscanf(partB, "%10f", &tPt.y);
		for (int i=0; i<10; i++) partC[i] = Line[i+20];
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
		for (i=0; i<nBonds; i++) {
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
long MolDisplayWin::OpenPDBFile(BufferFile * Buffer) {
	char Line[kMaxLineLength];
	long	nAtoms;
	short	scanerr;

	ProgressInd->ChangeText("Reading PDB fileÉ");
	Frame * lFrame = MainData->cFrame;
		//First scan the file to determine the atoms
	nAtoms = 0;
	while (Buffer->GetFilePos() < Buffer->GetFileLength()) {
		Buffer->GetLine(Line);
		if (0==FindKeyWord(Line, "ATOM", 4)) nAtoms++;
		if (0==FindKeyWord(Line, "HETATM", 4)) nAtoms++;
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
			//This routine only parses simple pdb files. It probably gets many files atom types wrong
			//and it always reads in all atoms including duplicates
		while (Buffer->GetFilePos() < Buffer->GetFileLength()) {
			Buffer->GetLine(Line);
			long atomTest = FindKeyWord(Line, "ATOM", 4);
			long HetTest = FindKeyWord(Line, "HETATM", 4);
			if ((0==atomTest)||(0==HetTest)) {
				LinePos = 13;
				if (atomTest==0) Line[14] = ' ';
				else if (Line[12] != ' ') LinePos = 12;
				AtomType = SetAtomType((unsigned char *) &(Line[LinePos]));
				if (AtomType > 0) {
					scanerr = sscanf(&(Line[30]), "%8e%8e%8e", &(Pos.x), &(Pos.y), &(Pos.z));
					if (scanerr == 3) {
						lFrame->AddAtom(AtomType, Pos);
						MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.x));
						MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.y));
						MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.z));
						nAtoms++;
					}
				}
			}
		}
		if (Prefs->GetAutoBond())	//setup bonds, if needed
			lFrame->SetBonds(Prefs, false);
	}
	return 1;
}

long MolDisplayWin::OpenXYZFile(BufferFile * Buffer) {
	char Line[kMaxLineLength];
	long	nAtoms,i;
	short	scanerr;

	ProgressInd->ChangeText("Reading XYZ fileÉ");
	Frame * lFrame = MainData->cFrame;
		//1st line contains the number of atoms
	Buffer->GetLine(Line);
	scanerr = sscanf(Line, "%ld", &nAtoms);
	if ((scanerr!=1)||(nAtoms<=0)) throw DataError(15);
		//allocate memory for the atoms
	if (!MainData->SetupFrameMemory(nAtoms, 0)) throw MemoryError();
	Buffer->GetLine(Line);
	MainData->SetDescription(Line);
	long DRCnSkip = Prefs->GetDRCSkip(), nSkip=0;
		bool Done=false;
		bool RdPoint = true;
	while (!Done) {
		Done = true;
		if (RdPoint) {
			for (i=0; i<nAtoms; i++) {
					long AtomType, test;
					CPoint3D Pos, Vector;

				Buffer->GetLine(Line);
				test = ParseCartLine(Line, &AtomType, &Pos, &Vector, -1);
				
				if (test==-1) {	//invalid atom type
					throw DataError(17);
				} else if (test<0) {//other invalid data was encountered
					throw DataError(18);
				}
				if (AtomType > 115) {
					if (AtomType > 255) {
						if (((AtomType - 255) < 1)||((AtomType - 255) > nAtoms)) throw DataError(19);
					}
					if (!lFrame->AddSpecialAtom(Vector, i)) {
						throw MemoryError();
					}
				} else if (test == 7) {	//mass weight the normal mode
					if (i==0) {
						lFrame->Vibs = new VibRec(1, nAtoms);
						if (!lFrame->Vibs) throw MemoryError();
					}
					if (lFrame->Vibs) {
						lFrame->Vibs->NormMode[i].x = Vector.x*Prefs->GetSqrtAtomMass(AtomType-1);
						lFrame->Vibs->NormMode[i].y = Vector.y*Prefs->GetSqrtAtomMass(AtomType-1);
						lFrame->Vibs->NormMode[i].z = Vector.z*Prefs->GetSqrtAtomMass(AtomType-1);
					}
				}
				lFrame->AddAtom(AtomType, Pos);
				MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.x));
				MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.y));
				MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.z));
			}
			if (Prefs->GetAutoBond())	//setup bonds, if needed
				lFrame->SetBonds(Prefs, false);
		}
		long cPos = Buffer->GetFilePos();
		long fileLength = Buffer->GetFileLength();
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
					if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
						{ throw UserCancel();}
				} else {
					nSkip++;
					Buffer->SkipnLines(nAtoms+1);
					RdPoint = false;
				}
				Done = false;
			}
		}
	}
	return 1;
}

long MolDisplayWin::OpenMolPltFile(BufferFile *Buffer) {
	long				j,ii, nkinds=0, catm, iscanerr=0, LineLength, LinePos=0, fileAtoms=0,
						fileBonds=0;
	int					nchar;
	long				test=0, Mode=0;
	float				BondLength=0.0;
	char				LineText[kMaxLineLength+1], KeyWord[kMaxLineLength+1], token[5];

	ProgressInd->ChangeText("Reading MolPlt format fileÉ");
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
#ifndef __wxBuild__
	FileSave = 5;	//bit 2 marks .mol files
#endif
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

	if (Mode == -1) {
		MainData->cFrame->Vibs = new VibRec(1, fileAtoms);
		if (!MainData->cFrame->Vibs) throw MemoryError();
	}
	Buffer->SkipnLines(nkinds);

// read in the coords and atomtypes
	for (j=0; j<fileAtoms; j++) {			// loop over the number of atoms
			long AtomType;
			CPoint3D Pos, Vector;

		if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
		{ throw UserCancel();}
		Buffer->GetLine(LineText);
		test = ParseCartLine(LineText, &AtomType, &Pos, &Vector, Mode);
		
		if (test==-1) {	//invalid atom type
			throw DataError(17);
		} else if (test<0) {
			throw DataError(18);
		}
		if (AtomType > 115) {
			if (Mode < 0) throw DataError(20);
			if (AtomType > 255) {
				if (((AtomType - 255) < 1)||((AtomType - 255) > fileAtoms)) throw DataError(19);
			}
			if (!lFrame->AddSpecialAtom(Vector, j)) throw MemoryError();
		}
		if (Mode == -1) {	//mass weight the normal mode
			lFrame->Vibs->NormMode[j].x = Vector.x*Prefs->GetSqrtAtomMass(AtomType-1);
			lFrame->Vibs->NormMode[j].y = Vector.y*Prefs->GetSqrtAtomMass(AtomType-1);
			lFrame->Vibs->NormMode[j].z = Vector.z*Prefs->GetSqrtAtomMass(AtomType-1);
		}
		lFrame->AddAtom(AtomType, Pos);
		MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.x));
		MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.y));
		MainData->MaxSize = MAX(MainData->MaxSize, fabs(Pos.z));
	}
	if (fileBonds > 0) {						/* read in the array of bonds */
		long	ibond=-1, temp;
		Buffer->GetLine(LineText);
		LinePos = FindKeyWord(LineText, "BONDATOMS", 9);
		if (LinePos >= 0) {
			LinePos += 9;
			LineLength = strlen(LineText);
			for (j=0; j<(2*fileBonds); j++) {
				while (LineText[LinePos] && ((LineText[LinePos] < '0')||
						(LineText[LinePos] > '9'))) LinePos++;
				while (LinePos >= LineLength) {	//EOL reached try getting a new line
					long CurrentPos = Buffer->GetFilePos();
					Buffer->GetLine(LineText);
					if ((-1<FindKeyWord(LineText, "ATOMIC", 6))||
						(-1<FindKeyWord(LineText, "MODE", 4))) {
						Buffer->SetFilePos(CurrentPos);
						break;
					}
					LinePos = 0;
					LineLength = strlen(LineText);
					while (LineText[LinePos] && ((LineText[LinePos] < '0')||
							(LineText[LinePos] > '9'))) LinePos++;
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
	if (BondLength > 0.0) {
		Prefs->SetMaxBondLength(BondLength);
		lFrame->SetBonds(Prefs, false);
	}
	if (Mode > 0) {
		nkinds = 3*(lFrame->NumAtoms);		/* In general there will be 3N normal modes */
		VibRec * lVibs = MainData->cFrame->Vibs = new VibRec(nkinds, fileAtoms);
		if (!lVibs) throw MemoryError();
		catm = 0;
		nkinds = 0;
		ii=0;
		try {
					/* Now locate the first frequency (skipping the atomic masses) */
			long CurrentPos = Buffer->GetFilePos();
			Buffer->GetLine(LineText);
			while (-1>=FindKeyWord(LineText, "MODE", 4)) {
				CurrentPos = Buffer->GetFilePos();
				Buffer->GetLine(LineText);
			}
			Buffer->SetFilePos(CurrentPos);

			for (; ii < (3*(lFrame->NumAtoms)); ii++) {
				/* Allow a small amount of background processing and allow user cancels */
				if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
					{ throw UserCancel();}
				Buffer->GetLine(LineText);
				for (j=0; LineText[j] && (LineText[j] != '='); j++) ;
				if (LineText[j] != '=') break;	/* no more frequencies so quit */
				sscanf(&LineText[j+1], "%s", KeyWord);
				LineLength = strlen(KeyWord);
				lVibs->Frequencies[nkinds] = LineLength;
				nkinds++;
	/*Check to see if the first character is a '-' if so change it to an 'i' since it really is an
	imaginary mode and because the menu manager will think it means create a divider in the menu instead
	of a regular text item */
				if (KeyWord[0] == '-') KeyWord[0] = 'i';
				if (nkinds+LineLength > 20*3*(lFrame->NumAtoms)) break;
				strncpy((char *) &(lVibs->Frequencies[nkinds]), KeyWord, LineLength);
				nkinds += LineLength;

				for (j=0; j < (lFrame->NumAtoms); j++) {
					Buffer->GetLine(LineText);
					iscanerr = sscanf(LineText, "%f%f%f", &((lVibs->NormMode[catm]).x),
						&((lVibs->NormMode[catm]).y), &((lVibs->NormMode[catm]).z));
					if (iscanerr != 3) {			/*Uh Ohh looks like there was a problem reading the file*/
						throw DataError(21);
					}
					(lVibs->NormMode[catm]).x *= (Prefs->GetSqrtAtomMass((lFrame->Atoms[j].Type)-1));
					(lVibs->NormMode[catm]).y *= (Prefs->GetSqrtAtomMass((lFrame->Atoms[j].Type)-1));
					(lVibs->NormMode[catm]).z *= (Prefs->GetSqrtAtomMass((lFrame->Atoms[j].Type)-1));
					catm++;
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
long MoleculeData::ParseECPotentials(BufferFile * Buffer) {
	long 	ElectronsRemoved = 0, ProtonsRemoved=0, LinePos, atom;
	char	LineText[kMaxLineLength];
	long *	zcore = new long[cFrame->NumAtoms];
	if (!zcore) throw MemoryError();
	for (atom=0; atom<cFrame->NumAtoms; atom ++) zcore[atom]=0;
	
	long StartPos = Buffer->GetFilePos();
	if (!Buffer->LocateKeyWord("THE ECP RUN REMOVES", 19, -1)) {
		delete [] zcore;
		return 0;
	}
	long EndPos = Buffer->GetFilePos();
	Buffer->GetLine(LineText);	//read in the number of electrons removed
	sscanf(&(LineText[20]),"%ld", &ElectronsRemoved);

		//Now read in the number of protons removed from each atom
	Buffer->SetFilePos(StartPos);
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
	delete [] zcore;
	if (ProtonsRemoved != ElectronsRemoved) {
		MessageAlert("Parse Error while reading ECP Potential Data. Use with caution!");
	}
	return ElectronsRemoved;
}
long MolDisplayWin::ParseSIMMOMLogFile(BufferFile *Buffer, long EnergyPos) {
		char		LineText[kMaxLineLength];

	Frame * lFrame = MainData->cFrame;
	if (!Buffer->LocateKeyWord("Cartesian Coordinates of Atoms in Bulk Model", 44, EnergyPos)) {
		return 0;
	}	
		//Grab the tinker coordinate output from the top of the file
	ProgressInd->ChangeText("Reading Tinker Coordinates");
	if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
		{throw UserCancel();}

	Buffer->SkipnLines(3);
	long StartPos = Buffer->GetFilePos();
	if (!(Buffer->LocateKeyWord("-----------", 11, EnergyPos))) throw DataError();
	long test = Buffer->GetFilePos() - StartPos;
	Buffer->SetFilePos(StartPos);
	long numlines = Buffer->GetNumLines(test) - 1;
	if (numlines > 0) {
		if (!MainData->SetupFrameMemory(numlines, 0)) throw std::bad_alloc();
	} else {
		throw DataError(12);
	}
	for (long i=0; i<numlines; i++) {
			long linenum;
			CPoint3D	position;
			unsigned char	Label[kMaxLineLength];
			
		Buffer->GetLine(LineText);
		int scannum = sscanf(LineText, "%ld %s %f %f %f", &linenum, Label, &(position.x),
				&(position.y), &(position.z));
		if (scannum != 5) throw DataError(13);
		long atomtype = SetAtomType(Label);
		mpAtom * newAtom = lFrame->AddAtom(atomtype, position);
		if (newAtom) newAtom->IsSIMOMMAtom(true);
	}
		//Now add the full ab initio atoms
	if (Buffer->LocateKeyWord("COORDINATES (BOHR)", 16, EnergyPos)) {	//first normal (ab initio) atoms
		ProgressInd->ChangeText("Reading Coordinates");
		if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
			{ AbortOpen("File open canceled by user"); return 0;}

		Buffer->SkipnLines(2);
		StartPos = Buffer->GetFilePos();
		test = Buffer->FindBlankLine() - StartPos;
		numlines = Buffer->GetNumLines(test);
		if (numlines > 0) {
			if (!MainData->SetupFrameMemory(numlines+lFrame->NumAtoms, 0)) throw MemoryError();
		} else {
			throw DataError(22);
		}
		if (!ParseGLogLine(Buffer, lFrame, numlines, 0, &(MainData->MaxSize)))
			throw DataError(23);
		lFrame->toggleAbInitioVisibility();
	}
	if (Prefs->GetAutoBond())
		lFrame->SetBonds(Prefs, false);
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
					NumOccAlpha=0, NumOccBeta=0, NumBetaUHFOrbs=0, LinePos, StartPos,
					NumCoreOrbs, NumOpenOrbs, NumGVBPairOrbs, EnergyPos,
					ReadMP2Orbitals=1, NextFinalPos, SavedPos;
	float			*Occupancy = NULL;
	bool			KeyWordFound;
	char			LineText[kMaxLineLength], token[kMaxLineLength];
	BufferFile *	DatBuffer=NULL;

	ProgressInd->ChangeText("Reading GAMESS log fileÉ");
	Frame * lFrame = MainData->cFrame;

	EnergyPos = -1;
	NextFinalPos = -1;
		//First Skip over all of the input card lines
	while (Buffer->LocateKeyWord("INPUT CARD>", 11))
		Buffer->SkipnLines(1);
	long HeaderEndPos = Buffer->GetFilePos();
	Buffer->LocateKeyWord("RUN TITLE", 9);	//find and skip over run title since
	Buffer->SkipnLines(3);					//it can contain any arbitrary text
	if (Buffer->LocateKeyWord("FINAL", 5)) {//locate initial energy since all options are before it
		EnergyPos = Buffer->GetFilePos();
		Buffer->SetFilePos(HeaderEndPos);
	}

	if (Append) {
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
		LinePos = Buffer->GetFilePos();
			//Check for ECP type run which reduces the number of occupied orbitals
		if (Buffer->LocateKeyWord("THE ECP RUN REMOVES", 19, EnergyPos)) {
			Buffer->GetLine(LineText);	//read in the number of electrons removed
			sscanf(&(LineText[20]),"%ld", &test);
			test /= 2;
			NumOccAlpha -= test;
			if (NumOccAlpha < 0) NumOccAlpha = 0;	//Oops!
			if (NumOccBeta) NumOccBeta -= test;
			if (NumOccBeta<0) NumOccBeta = 0;
			Buffer->SetFilePos(LinePos);
		}
		if (MainData->InputOptions->Control->GetSCFType()==GAMESS_MCSCF) {	//Calculate the # of occupied MOs by parsing the DRT info
		//	long filePos = Buffer->GetFilePos();
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
	} else {
		MainData->InputOptions = new InputData;
		if (!MainData->InputOptions) throw MemoryError();
		KeyWordFound = Buffer->LocateKeyWord("BASIS OPTIONS", 13, EnergyPos);
		if (KeyWordFound) {	//Read in the basis set information (for user reference)
			ProgressInd->ChangeText("Reading Basis information");
			if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
				{ throw UserCancel();}
			MainData->ReadBasisOptions(Buffer);
		}
		if (Buffer->LocateKeyWord("RUN TITLE", 9, EnergyPos)) {	//Read the 1 line run label
			Buffer->SkipnLines(2);
			Buffer->GetLine(LineText);
			MainData->InputOptions->Data->SetTitle(LineText, strlen(LineText));
		}
		if (Buffer->LocateKeyWord("POINT GROUP", 11, EnergyPos)) {	//Setup molecular point group
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
		}
		Buffer->SetFilePos(0);
		if (Buffer->LocateKeyWord("QMMM PROCEDURE IS ON", 20, EnergyPos)) {
			//If this is a simmom type run then switch to that routine since the format is
			//significantly different
			return (ParseSIMMOMLogFile(Buffer, EnergyPos));
		}
	//locate the input set of coordinates (present in every file)
		if (Buffer->LocateKeyWord("COORDINATES (BOHR)", 16, EnergyPos)) {	//first normal (ab initio) atoms
			ProgressInd->ChangeText("Reading Coordinates");
			if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
				{ AbortOpen("File open canceled by user"); return 0;}

			Buffer->SkipnLines(2);
			StartPos = Buffer->GetFilePos();
			test = Buffer->FindBlankLine() - StartPos;
			numlines = Buffer->GetNumLines(test);
			if (numlines > 0) {
				if (!MainData->SetupFrameMemory(numlines, 0)) throw MemoryError();
			} else {
				throw DataError(22);
			}
			if (!ParseGLogLine(Buffer, lFrame, numlines, 0, &(MainData->MaxSize)))
				throw DataError(23);
		}
		LinePos = Buffer->GetFilePos();		//next look for fragments
		if (Buffer->LocateKeyWord("READING $EFRAG GROUP", 20, EnergyPos)) {	//ughh fragments!
			NumFragmentAtoms = MainData->ReadInitialFragmentCoords(Buffer);
			Buffer->SetFilePos(LinePos);
		}
		if (lFrame->NumAtoms <= 0) {	//initial coordinates not found! Abort!
			throw DataError(22);
		}
		if (Prefs->GetAutoBond())
			lFrame->SetBonds(Prefs, false);

	//Read in the atomic basis set (if present) and keep it in case we find MO vectors later
	//if it is not located and read in correctly MO vectors will also be skipped
		if (Buffer->LocateKeyWord("ATOMIC BASIS SET", 16, EnergyPos)) {
			ProgressInd->ChangeText("Reading atomic basis set");
			if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
				{ throw UserCancel();}
			try {
				MainData->ParseGAMESSBasisSet(Buffer);
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

			//Read in charge and multiplicity
		if (Buffer->LocateKeyWord("CHARGE OF MOLECULE", 18, EnergyPos)) {
			Buffer->GetLine(LineText);
			LinePos = FindKeyWord(LineText, "=", 1) + 1;
			long charge;
			sscanf(&(LineText[LinePos]),"%ld", &charge);
			MainData->InputOptions->Control->SetCharge(charge);
		}
			//There are now two forms of this, "SPIN MULTIPLICITY" and "STATE MULTIPLICITY"
			//however, either should appear soon after the CHARGE.
		if (Buffer->LocateKeyWord("MULTIPLICITY", 12, Buffer->GetFilePos()+100)) {
			Buffer->GetLine(LineText);
			LinePos = FindKeyWord(LineText, "=", 1) + 1;
			long Multiplicity;
			sscanf(&(LineText[LinePos]),"%ld", &Multiplicity);
			MainData->InputOptions->Control->SetMultiplicity(Multiplicity);
		}
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
		LinePos = Buffer->GetFilePos();
			//Check for ECP type run which reduces the number of occupied orbitals
		if (Buffer->LocateKeyWord("ECP POTENTIALS", 14, EnergyPos)) {
			test = MainData->ParseECPotentials(Buffer);
			test /= 2;
			NumOccAlpha -= test;
			if (NumOccAlpha < 0) NumOccAlpha = 0;	//Oops!
			if (NumOccBeta) NumOccBeta -= test;
			if (NumOccBeta<0) NumOccBeta = 0;
			Buffer->SetFilePos(LinePos);
		}
			//Now read other Control options
		if (!Buffer->LocateKeyWord("CONTRL OPTIONS", 14, EnergyPos)) {
				// $CONTRL OPTIONS used to be JOB OPTIONS so check for it too
			if (!Buffer->LocateKeyWord("JOB OPTIONS", 11, EnergyPos)) throw DataError();
		}
		MainData->ReadControlOptions(Buffer);
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

		if (MainData->InputOptions->Control->GetSCFType()==GAMESS_GVB) { // Parse the GVB input parameters
			if (Buffer->LocateKeyWord("ROHF-GVB INPUT PARAMETERS", 26, EnergyPos)) {
				Buffer->SkipnLines(3);
				Buffer->GetLine(LineText);
				sscanf(LineText, " NORB   =%ld NCO    =%ld", &test, &NumCoreOrbs);
				NumOccAlpha = test;
				Buffer->GetLine(LineText);
				sscanf(LineText, " NPAIR  =%ld NSETO  =%ld", &NumGVBPairOrbs, &NumOpenOrbs);
			}
			Occupancy = new float[MainData->GetNumBasisFunctions()];
			if (!Occupancy) throw MemoryError();
			for (test=0; test<MainData->GetNumBasisFunctions(); test++) Occupancy[test] = 0.0;
			for (test=0; test<NumCoreOrbs; test++) Occupancy[test] = 2.0;
			if (NumOpenOrbs > 0) {	//Parse the occupancies of the open shells
				if (Buffer->LocateKeyWord("F VECTOR (OCCUPANCIES)", 23)) {
						long nSkip = 3;
					if (NumCoreOrbs > 0) nSkip = 4;	//skip the one line for all doubly occupied core orbs
					Buffer->SkipnLines(nSkip);
					for (nSkip=0; nSkip<NumOpenOrbs; nSkip++) {
						Buffer->GetLine(LineText);
						sscanf(LineText, "%ld %f", &test, &(Occupancy[NumCoreOrbs + nSkip]));
						Occupancy[NumCoreOrbs + nSkip] *= 2;	//occupancies are listed as % of orbital filled
					}
				}
			}
		} else if (MainData->InputOptions->Control->GetSCFType()==GAMESS_MCSCF) {	//Calculate the # of occupied MOs by parsing the DRT info
			long filePos = Buffer->GetFilePos();
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
			if (Buffer->LocateKeyWord("INITIAL GUESS ORBITALS", 22, EnergyPos)) {
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
				long test = Buffer->GetFilePos();
				if (EnergyPos > test)
					return OpenGAMESSIRCLog(Buffer, flip, offset, NumOccAlpha, NumOccBeta, NumFragmentAtoms);
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
						Buffer->GetLine(LineText);
						sscanf(&(LineText[8]), "%lf", &(lFrame->MP2Energy));
					}
					Buffer->SetFilePos(StartPos);
				}
			}
				//Attempt to read in orbitals for first geometry
			SavedPos = Buffer->GetFilePos();
			NextFinalPos = -1;
			if (Buffer->LocateKeyWord("FINAL", 5)) NextFinalPos = Buffer->GetFilePos();
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
								ReadGVBOccupancy(Buffer, NumGVBPairOrbs, MainData->GetNumBasisFunctions(),
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
								if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
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
							lFrame->ParseGVBGIOrbitals(Buffer, MainData->GetNumBasisFunctions(), ProgressInd);
						}
					}
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
				//look for gradient data
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
			return this->OpenGAMESSIRCLog(Buffer, flip, offset, NumOccAlpha, NumOccBeta, NumFragmentAtoms);
		}
	}
	long memlength = sizeof(Frame) + lFrame->NumAtoms*sizeof(mpAtom) +
					lFrame->NumBonds*sizeof(Bond) + 5000;
	KeyWordFound = true;
	double	FrameEnergy, MP2FrameEnergy;
	while (KeyWordFound) {
		if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
			{ throw UserCancel();}
//Test for available memory
#ifndef __wxBuild__
		if (MaxBlock() < memlength) {
			MessageAlertByID(kerrstrings, 29);
			break;
		}
#endif
		KeyWordFound = false;
			//Advance to the start of the next geometry step BEGINNING GEOMETRY SEARCH POINT
			//If there isn't one then we are done with the geometries
		if (!(Buffer->LocateKeyWord("BEGINNING GEOMETRY SEARCH POINT", 31) || 
			  Buffer->LocateKeyWord("1NSERCH", 7))) break;
		StartPos = Buffer->GetFilePos();
		if (Buffer->LocateKeyWord("FINAL", 5)) {	//Search for final and energy on the same line
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
					long temp = Buffer->GetFilePos();
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
	//		sprintf(LineText, "Reading Coordinates: frame %ld", MainData->NumFrames);
	//		ProgressInd->ChangeText(LineText);
	//		if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
	//			{ throw UserCancel();}
			EnergyPos = Buffer->GetFilePos();

			Buffer->SkipnLines(1);
			NextFinalPos = -1;
			if (lFrame->NumAtoms - NumFragmentAtoms > 0) {
				if (Buffer->LocateKeyWord("BEGINNING GEOMETRY SEARCH POINT", 31)) {
					KeyWordFound = true;
				} else {
					KeyWordFound = Buffer->LocateKeyWord("FINAL", 5);
				}
				if (KeyWordFound) NextFinalPos = Buffer->GetFilePos();
				Buffer->SetFilePos(EnergyPos);
			}
		// The output for optimizations was changed in 1/1998 to print all coordinates
		// at the top of am optimization step, but the older style is also still possible
		//Look for ab initio atoms first
			Buffer->SetFilePos(StartPos);	//reset pos to last frame to begin search
			if (Buffer->LocateKeyWord("BEGINNING GEOMETRY SEARCH POINT", 31) || 
				 Buffer->LocateKeyWord("1NSERCH", 7)) {
				lpFrame = lFrame;
				lFrame = MainData->AddFrame(lpFrame->NumAtoms,0);

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
							lFrame->ReadGradient(Buffer, NextFinalPos);
						} else if (Buffer->LocateKeyWord(" NSERCH", 7, NextFinalPos)) {
							//Otherwise look for the coordinates at the end of the
							//geometry step (pre 1998 style)
							long npos = Buffer->GetFilePos();
							Buffer->SkipnLines(9);
							test = ParseGLogLine(Buffer, lFrame, lpFrame->NumAtoms-NumFragmentAtoms, 1, &(MainData->MaxSize));
							if (test==-1) {	//Something was wrong with this set of coordinates
								MainData->DeleteFrame();
								break;
							}
							Buffer->SetFilePos(npos);//reset position back to beginning of gradient data
							lFrame->ReadGradient(Buffer, NextFinalPos);
						}
					}
					if (NumFragmentAtoms > 0) {
						long tempPos = Buffer->GetFilePos();
						Buffer->SetFilePos(StartPos);
						if (Buffer->LocateKeyWord("BEGINNING GEOMETRY SEARCH POINT", 31) || 
							 Buffer->LocateKeyWord("1NSERCH", 7)) {
							if (Buffer->LocateKeyWord("COORDINATES OF FRAGMENT MULTIPOLE CENTERS", 41, NextFinalPos)) {
								Buffer->SkipnLines(3);
								MainData->ReadFragmentCoordinates(Buffer, NumFragmentAtoms);
							}
							Buffer->SetFilePos(tempPos);
							lFrame->ReadGradient(Buffer, NextFinalPos);
						}
					}
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
			lFrame->MP2Energy = MP2FrameEnergy;
			if (Prefs->GetAutoBond())
				lFrame->SetBonds(Prefs, false);
			lFrame->IRCPt = lpFrame->IRCPt + flip;
			lFrame->time = lpFrame->time + flip + offset;
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
								ReadGVBOccupancy(Buffer, NumGVBPairOrbs, MainData->GetNumBasisFunctions(),
									Occupancy);
							}
						}
						bool test = false;
						if (MainData->InputOptions->Control->GetCIType() == 4) {
							//CIS output includes the EIGENVECTORS token
							//I think the regular EIGENVECTORS would be before the CIS line???
							long bp = Buffer->GetFilePos();
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
							if (!test)
								test = Buffer->LocateKeyWord("MOLECULAR ORBITALS", 18, NextFinalPos);
						}
						if (test) {
								//skip over the line to the start of the orbital blocks
								//There is always a blank line before the orbs, but there may a '---' divider line after the key line just found
							if ((NextFinalPos<0)||(NextFinalPos>Buffer->GetFilePos())) {
								Buffer->SetFilePos(Buffer->FindBlankLine());
								Buffer->SkipnLines(1);
								ProgressInd->ChangeText("Reading eigenvectors");
								if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
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
				//GAMESS now punchs out the Fock operator for ruedenburg type localization
				//which I need to skip if it is present
			if (Buffer->LocateKeyWord("FOCK OPERATOR FOR THE LOCALIZED ORBITALS",40))
				Buffer->SkipnLines(1);
			if (MainData->InputOptions->Control->GetSCFType()==GAMESS_MCSCF) {
					//skip over the density matrix (present in Ruedenburg MCSCF runs)
				if (Buffer->LocateKeyWord("DENSITY MATRIX FOR THE LOCALIZED ORBITALS",41))
					Buffer->SkipnLines(1);
			}
			KeyWordFound = Buffer->LocateKeyWord("LOCALIZED ORBITALS",18);
			if (KeyWordFound) {
				Buffer->SkipnLines(2);
				ProgressInd->ChangeText("Reading localized orbitals");
				if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
					{ throw UserCancel();}
				OrbitalRec * OrbSet = lFrame->ParseGAMESSLMOs(Buffer, MainData->GetNumBasisFunctions(), NumOccAlpha,
					NumBetaUHFOrbs, ProgressInd, false);
				if (OrbSet != NULL) {
					OrbSet->setOrbitalWavefunctionType((TypeOfWavefunction)MainData->InputOptions->Control->GetSCFType());
					if (MainData->InputOptions->Control->GetSCFType() != GAMESS_MCSCF) {
						OrbSet->SetOrbitalOccupancy(NumOccAlpha, NumOccBeta);
					}
				}
				// There may also be oriented localized orbitals
				KeyWordFound = Buffer->LocateKeyWord("ORIENTED LOCALIZED ORBITALS",27);
				if (KeyWordFound) {
					Buffer->SkipnLines(2);
					ProgressInd->ChangeText("Reading oriented localized orbitals");
					if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
						{ throw UserCancel();}
					OrbSet = lFrame->ParseGAMESSLMOs(Buffer, MainData->GetNumBasisFunctions(), NumOccAlpha,
						NumBetaUHFOrbs, ProgressInd, true);
					if (OrbSet != NULL) {
						OrbSet->setOrbitalWavefunctionType((TypeOfWavefunction)MainData->InputOptions->Control->GetSCFType());
						if (MainData->InputOptions->Control->GetSCFType() != GAMESS_MCSCF) {
							OrbSet->SetOrbitalOccupancy(NumOccAlpha, NumOccBeta);
						}
					}
				}
			}
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
bool ReadGVBOccupancy(BufferFile * Buffer, long NumPairOrbs, long MaxOrbs, float * Occupancy) {
		char Line[kMaxLineLength];
		long	junk, Orb1, Orb2;
		float	junkf, Occ1, Occ2;
	if ((NumPairOrbs <= 0)||(!Occupancy)) return false;	//no GVB pairs or no memory
	Buffer->SkipnLines(4);
	for (long i=0; i<NumPairOrbs; i++) {
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
	long NumFragmentAtoms = 0, iatom, NumMultipoles=0, lastAtom=0, FragmentNumber=1;
	char Label[kMaxLineLength], Line[kMaxLineLength];
	Frame * lFrame = GetCurrentFramePtr();
	iatom = lFrame->GetNumAtoms();

	if (Buffer->LocateKeyWord("TOTAL NUMBER OF MULTIPOLE POINTS=", 33)) {
		Buffer->GetLine(Line);
		sscanf(&(Line[33]), "%ld", &NumMultipoles);
	}
		//Mike finally fixed the spelling of coordinates
	if ((NumMultipoles>0) && (Buffer->LocateKeyWord("MULTIPOLE COORDINATES", 20) ||
			Buffer->LocateKeyWord("MULTIPOLE CORDINATES", 19))) {
		Buffer->SkipnLines(3);
		SetupFrameMemory(lFrame->NumAtoms + 10, 0);
		for (int i=0; i<NumMultipoles; i++) {
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
					if (std::toupper(Label[0]) == 'B') {
						//a 'B' almost always not an atom
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
					if (lFrame->NumAtoms + 1 > lFrame->AtomAllocation)
						SetupFrameMemory(lFrame->NumAtoms + 10, 0);
					long AtomType = SetAtomType((unsigned char *) &(Label[0]));
					if (AtomType > 0) {
						if (lastAtom < i-1) FragmentNumber ++;
						lastAtom = i;
						Pos.x *= kBohr2AngConversion;
						Pos.y *= kBohr2AngConversion;
						Pos.z *= kBohr2AngConversion;
						mpAtom * newAtom = lFrame->AddAtom(AtomType, Pos);
						if (newAtom) newAtom->SetFragmentNumber(FragmentNumber);
						MaxSize = MAX(MaxSize, fabs(Pos.x));
						MaxSize = MAX(MaxSize, fabs(Pos.y));
						MaxSize = MAX(MaxSize, fabs(Pos.z));
						NumFragmentAtoms ++;
					}
				}
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

	while (iatom<NumFragmentAtoms) {
		Buffer->GetLine(Line);
		if (-1<LocateKeyWord(Line, "FRAGNAME=",9, strlen(Line))) {FragmentNumber++; continue;}
		int iscan = sscanf(Line, "%s%f%f%f", Label, &Pos.x, &Pos.y, &Pos.z);
		if (iscan == 4) {
			if ((Label[0] == 'Z') || (Label[0] == 'z')) {	//z represents an atom pos
				Label[0] = Label[1];
				if (std::isalpha(Label[2])) {
					Label[1] = Label[2];
					Label[2] = 0;
				} else Label[1] = 0;
			}
			long AtomType = SetAtomType((unsigned char *) &(Label[0]));
			if (AtomType > 0) {
				mpAtom * newAtom = lFrame->AddAtom(AtomType, Pos);
				if (newAtom) newAtom->SetFragmentNumber(FragmentNumber);
				MaxSize = MAX(MaxSize, fabs(Pos.x));
				MaxSize = MAX(MaxSize, fabs(Pos.y));
				MaxSize = MAX(MaxSize, fabs(Pos.z));
				iatom ++;
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
	long StartPos = Buffer->GetFilePos();	//All keywords should be between these positions
	long EndPos = Buffer->FindBlankLine();
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

	if (Buffer->LocateKeyWord("ECP", 3, EndPos)) {
		Buffer->GetLine(LineText);
		sscanf(&(LineText[7]),"%s", token);
		InputOptions->Basis->SetECPPotential(token);
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
long MolDisplayWin::OpenGAMESSIRC(BufferFile * Buffer, bool Append, long flip, float offset)
{	long			LinePos, NumAtoms, point;
	float			Xpos;
	bool			KeyWordFound;
	char			LineText[kMaxLineLength+1];

	ProgressInd->ChangeText("Reading GAMESS IRC fileÉ");
	Frame * lFrame = MainData->cFrame;

	if (!Append) {
#ifdef __wxBuild__
#ifndef WIN32
#warning Need to set file type
#endif
#else
		FileSave = 17; //IRC bit 5 plus bit 1 for saving
#endif
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

		LinePos = Buffer->GetFilePos();
		if (!Buffer->LocateKeyWord("MASS-WEIGHTED GRADIENT", 22)) throw DataError();
		NumAtoms = Buffer->GetFilePos();
		Buffer->SetFilePos(LinePos);
		NumAtoms = Buffer->GetNumLines(NumAtoms-LinePos);
		
		if (!MainData->SetupFrameMemory(NumAtoms, 0)) throw MemoryError();

		ParseGLogLine(Buffer, lFrame, NumAtoms, 0, &(MainData->MaxSize));

		if (Prefs->GetAutoBond())
			lFrame->SetBonds(Prefs, false);
	}
	NumAtoms = lFrame->NumAtoms;
	KeyWordFound = Buffer->LocateKeyWord("POINT=", 6);
	long memlength = sizeof(Frame)+NumAtoms*sizeof(mpAtom)+lFrame->NumBonds*sizeof(Bond)+5000;
	while (KeyWordFound) {
		if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
			{ throw UserCancel();}
#ifndef __wxBuild__
		if (MaxBlock() < memlength) {
			MessageAlertByID(kerrstrings, 29);
			break;
		}
#endif
		Buffer->GetLine(LineText);
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
				lFrame->SetBonds(Prefs, false);
		}
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

	ProgressInd->ChangeText("Reading GAMESS IRC log fileÉ");
	Frame * lFrame = MainData->cFrame;

	long NumAtoms = lFrame->NumAtoms;
	long NumExpectedAtoms = NumAtoms;
	if (MainData->InputOptions->Control->GetSCFType()==GAMESS_UHF) NumBetaUHFOrbs = NumOccBeta;	//Only seperate Beta spin orbs for UHF wavefunctions
	char * NextPointKeyword = new char[30];
	strcpy(NextPointKeyword, "* NEXT POINT ON IRC FOUND *");
	bool KeyWordFound = Buffer->LocateKeyWord(NextPointKeyword, 27);
	if (!KeyWordFound) {	//search for the alternative keyword needed for pace=linear IRC's
		strcpy(NextPointKeyword, "ON THE REACTION PATH");
		KeyWordFound = Buffer->LocateKeyWord(NextPointKeyword, 20);
		if (KeyWordFound) LINEAR=true;
	}
	long memlength = sizeof(Frame)+NumAtoms*sizeof(mpAtom)+lFrame->NumBonds*sizeof(Bond)+5000;
	while (KeyWordFound) {
		if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
			{ throw UserCancel();}
#ifndef __wxBuild__
		if (MaxBlock() < memlength) {
			MessageAlertByID(kerrstrings, 29);
			break;
		}
#endif
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
				lFrame->SetBonds(Prefs, false);
		}
		long SavedPos = Buffer->GetFilePos();
		KeyWordFound = Buffer->LocateKeyWord(NextPointKeyword, 20);
		long NextPointPos = -1;
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
							if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
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

//	Frame *			lpFrame=NULL;
	bool			QPpresent, KeyWordFound, newQPformat=false;
	long			NumAtoms, Elength, nskip=0, LinePos, EStartPos;
	char			Etext[40], LineText[kMaxLineLength+1];
	float			tempfloat;
	double			PE;

	ProgressInd->ChangeText("Reading GAMESS DRC fileÉ");
	Frame * lFrame = MainData->cFrame;
	if (!Append) {
#ifdef __wxBuild__
#ifndef WIN32
#warning Need to setup file type
#endif
#else
		FileSave = 33; //DRC bit 6 plus bit 1 for saving
#endif
		MainData->DrawMode += 8;
	}
	long FilePos = Buffer->GetFilePos();
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
		sscanf(&(LineText[LinePos]), "%lf %lf %lf", &(lFrame->KE), &PE,
			&(lFrame->Energy));//Don't worry about the PE since E-KE=PE

		if (!Buffer->LocateKeyWord("VELOCITY", 8)) return 0;
		Buffer->SkipnLines(1);
		if (LogFile)
			Buffer->SkipnLines(1);
		FilePos = Buffer->GetFilePos();
		Buffer->LocateKeyWord("--------", 8);
		NumAtoms = Buffer->GetFilePos();
		Buffer->SetFilePos(FilePos);
		NumAtoms = Buffer->GetNumLines(NumAtoms-FilePos)-1;
		if (NumAtoms <= 0) throw DataError();

		if (!MainData->SetupFrameMemory(NumAtoms, 0)) throw MemoryError();
		
		ParseGLogLine(Buffer, lFrame, NumAtoms, 2, &(MainData->MaxSize));

		if (Prefs->GetAutoBond())
			lFrame->SetBonds(Prefs, false);

		KeyWordFound = Buffer->LocateKeyWord(Etext, Elength);
	}

	NumAtoms = lFrame->NumAtoms;
	long memlength = sizeof(Frame)+NumAtoms*sizeof(mpAtom)+lFrame->NumBonds*sizeof(Bond)+5000;
	long DRCnSkip = Prefs->GetDRCSkip();
	while (KeyWordFound) {
		Buffer->SkipnLines(1);
		if (nskip >= DRCnSkip) {
			nskip = 0;
			if (!ProgressInd->UpdateProgress((100*Buffer->GetFilePos())/Buffer->GetFileLength()))
				{ throw UserCancel();}
#ifndef __wxBuild__
			if (MaxBlock() < memlength) {
				MessageAlertByID(kerrstrings, 29);
				break;
			}
#endif
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
			sscanf(&(LineText[LinePos]), "%lf %lf %lf", &(lFrame->KE), &PE,
				&(lFrame->Energy));//Don't worry about the PE since E-KE=PE

			sprintf(LineText, "Reading DRC time = %f", tempfloat);
			ProgressInd->ChangeText(LineText);

			if (!MainData->SetupFrameMemory(NumAtoms, 0)) throw MemoryError();

			if (!Buffer->LocateKeyWord("VELOCITY", 8)) break;;
			Buffer->SkipnLines(1);
			if (LogFile)
				Buffer->SkipnLines(1);
			if (ParseGLogLine(Buffer, lFrame, NumAtoms, 2, &(MainData->MaxSize)) < 0) break;

			if (Prefs->GetAutoBond())
				lFrame->SetBonds(Prefs, false);
		} else
			nskip ++;
		KeyWordFound = Buffer->LocateKeyWord(Etext, Elength);
	}

	lFrame = MainData->Frames;
	for (long iframe=0; iframe<MainData->NumFrames; iframe++) {
		lFrame->IRCPt = iframe + 1;
		lFrame = lFrame->NextFrame;
	}
	MainData->cFrame = MainData->Frames;
	MainData->CurrentFrame = 1;
	return 1;
}	/*OpenGAMESSDRC*/
#ifndef __wxBuild__
void MolDisplayWin::CloseFile(void) {
	if (fileRefNum) {
		FSClose(fileRefNum);
		fileRefNum = 0;		//Make sure this file isn't closed again
		FlushVol(NULL, fileFSSpec.vRefNum);	//Flush all file information to disk
	}
}
void MolDisplayWin::ImportVecGroup(FSSpec * target) {
	if (MainData->Basis) {
			short vreftemp;
		OSErr myerr = FSpOpenDF(target, fsRdPerm, &vreftemp);	/* Open the data fork */
		if (myerr != noErr) {
			MessageAlert("\pError opening the requested file!");
			return;		// abort if there is an error opening the file
		}
			ImportVecDlg * temp = NULL;
		try {
			temp = new ImportVecDlg(this, vreftemp);
		}
		catch (MemoryError) {
			MessageAlert("\pOut of memory! Be careful!");
		}
		catch (DataError) {
			MessageAlert("\pInvalid data encountered! Beware of bad data already read into frames.");
		}
		catch (FileError) {
			MessageAlert("\pUnexpected End of File encountered!");
		}
		catch (...) {
			if (temp) delete temp;
			MessageAlert("\pSorry. Out of memory. Unable to complete the import.");
			FSClose(vreftemp);
		}
	}
}
void MolDisplayWin::WritePICT(BufferFile * Buffer) {
	PicHandle	tempPict = CreatePICT(360, false);
	if (tempPict) {
			//The first 512 bytes are reservered for App use, so just write out 512 zeros
			char junk=0;
		long dataSize = GetHandleSize((Handle) tempPict);
		if (dataSize > 10) {
			for (int i=0; i<512; i++) Buffer->Write(&junk, 1);
			HLock((Handle) tempPict);
			Buffer->Write((Ptr) *tempPict, dataSize);
		} else {
			MessageAlert("\pError creating PICT! Try increasing MacMolPlt's memory partition and retry.");
		}
		KillPicture(tempPict);
	}
}
void MolDisplayWin::ExportGAMESS(BufferFile * Buffer, bool AllFrames) {
	Frame * lFrame = MainData->Frames;
	long NumFrames = MainData->NumFrames;
	if (!AllFrames) {
		lFrame = MainData->cFrame;
		NumFrames = 1;
	}
		long		iatom;
		Str255		PointGroup, AtomLabel;
		char		text[kMaxLineLength];
		bool		C1Sym=false;
		
	if (MainData->InputOptions) {
		iatom = MainData->InputOptions->Data->GetPointGroup(PointGroup, false);
		C1Sym = (iatom == C1);
		if ((iatom>3)&&(iatom<11)) {
			sprintf((char *) &(PointGroup[PointGroup[0]+1]), " %d",
				MainData->InputOptions->Data->GetPointGroupOrder());
		}
	} else {
		strcpy((char *) &(PointGroup[1]), "C1");
		C1Sym = true;
	}
	for (long i=0; i<NumFrames; i++) {
		sprintf(text, " $DATA\rComments go here: Frame # %ld\r%s\r", 
			i, &(PointGroup[1]));
		Buffer->PutText(text);
		if (!C1Sym) {	//Add a blank line unless the Point Group is C1
			sprintf(text, "\r");
			Buffer->PutText(text);
		}
		for (iatom=0; iatom<lFrame->NumAtoms; iatom++) {
			Prefs->GetAtomLabel(lFrame->Atoms[iatom].GetType()-1, AtomLabel);
			AtomLabel[AtomLabel[0]+1] = 0;
			sprintf(text, "%s   %5.1f  %10.5f  %10.5f  %10.5f\r",
				(char *) &(AtomLabel[1]), (float) (lFrame->Atoms[iatom].Type), 
				lFrame->Atoms[iatom].Position.x, lFrame->Atoms[iatom].Position.y,
				lFrame->Atoms[iatom].Position.z);
			Buffer->PutText(text);
		}
		sprintf(text, " $END\r");
		Buffer->PutText(text);

		lFrame = lFrame->NextFrame;
		if (!lFrame) break;
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
	Buffer->PutText("\r");

	Frame * lFrame = MainData->Frames;
	float	UnitFactor = 1.0;
	if (lEOpts->GetDisplayUnits() == kKCalPerMole) UnitFactor = kHartreeTokCalPMol;
	long NumFrames = MainData->NumFrames;
	if (!AllFrames) {
		lFrame = MainData->cFrame;
		NumFrames = 1;
	}
	for (long i=0; i<NumFrames; i++) {
		sprintf(text, "%f", lFrame->time);
		Buffer->PutText(text);
		if (PlotTE) {
			sprintf(text, "\t%f", (lFrame->Energy-lEOpts->GetY1Zero())*UnitFactor);
			Buffer->PutText(text);
		}
		if (PlotMPE) {
			sprintf(text, "\t%f", (lFrame->MP2Energy-lEOpts->GetY1Zero())*UnitFactor);
			Buffer->PutText(text);
		}
		if (PlotKE) {
			sprintf(text, "\t%f", (lFrame->KE-lEOpts->GetY2Zero())*UnitFactor);
			Buffer->PutText(text);
		}
		if (PlotPE) {
			sprintf(text, "\t%f", (lFrame->Energy - lFrame->KE - lEOpts->GetY1Zero())*UnitFactor);
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
		Buffer->PutText("\r");

		lFrame = lFrame->NextFrame;
		if (!lFrame) break;
	}
}
void MolDisplayWin::WriteFrequencies(BufferFile * Buffer) {
	char	text[kMaxLineLength];
	Frame * lFrame;
	lFrame = MainData->cFrame;

	if (!lFrame->Vibs) return;
//		Atom * lAtoms = lFrame->Atoms;
		char * lFreq = lFrame->Vibs->Frequencies, Line[kMaxLineLength];
		long	fPos = 0;
		float *	lInten=NULL;
	if (lFrame->Vibs->Intensities) lInten = lFrame->Vibs->Intensities;
	Buffer->PutText("Frequency\tIntensity\r");
	for (long i=0; i<lFrame->Vibs->GetNumModes(); i++) {
		strncpy(Line, &(lFreq[fPos+1]), lFreq[fPos]);
		Line[lFreq[fPos]] = 0;
		fPos += 1 + lFreq[fPos];
		sprintf(text, "%s", Line);
		Buffer->PutText(text);
		if (lInten) {
			sprintf(text, "\t%f", lInten[i]);
			Buffer->PutText(text);
		}
		Buffer->PutText("\r");
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
		Atom * lAtoms = lFrame->Atoms;
		char * lFreq = lFrame->Vibs->Frequencies, FreqLabel[kMaxLineLength];
		long	fPos = 0, i, iatm;
		for (i=0; i<lFrame->Vibs->CurrentMode; i++) {
			fPos += 1 + lFreq[fPos];
		}	//Store the frequency label for later use
		strncpy(FreqLabel, &(lFreq[fPos+1]), lFreq[fPos]);
		FreqLabel[lFreq[fPos]] = 0;
		long cmode = (lFrame->NumAtoms)*(lFrame->Vibs->CurrentMode);
		CPoint3D * ModeOffset = new CPoint3D[lFrame->NumAtoms];
		CPoint3D * tempAtoms = new CPoint3D[lFrame->NumAtoms];
		if (!ModeOffset || !tempAtoms) return;
		float	VectorScale = Prefs->GetVectorScale();
//		long AnimationSpeed = Prefs->GetAnimationSpeed();
		float offsetFactor = 1.0/40.0;
		for (iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
			tempAtoms[iatm] = lAtoms[iatm].Position;
			ModeOffset[iatm] = lFrame->Vibs->NormMode[iatm+cmode];
			ModeOffset[iatm] *= VectorScale;
		}
		for (i=0; i<20; i++) {
			if ((i==5)||(i==15)) {
				offsetFactor *= -1.0;
			}
			for (iatm=0; iatm<(lFrame->NumAtoms); iatm++) {
				tempAtoms[iatm].x += offsetFactor*(ModeOffset[iatm].x);
				tempAtoms[iatm].y += offsetFactor*(ModeOffset[iatm].y);
				tempAtoms[iatm].z += offsetFactor*(ModeOffset[iatm].z);
			}	//Now punch out the XYZ frame
			sprintf(text, "%ld\r", lFrame->NumAtoms);
			Buffer->PutText(text);
			sprintf(text, "Frequency %s Frame %ld\r", FreqLabel, i+1);
			Buffer->PutText(text);
			for (long j=0; j<lFrame->NumAtoms; j++) {
					Str255	Label;
				Prefs->GetAtomLabel(lAtoms[j].Type-1, Label);
				Label[Label[0]+1]=0;
				sprintf(text, "%s %.4f %.4f %.4f", &(Label[1]), tempAtoms[j].x,
					tempAtoms[j].y, tempAtoms[j].z);
				Buffer->PutText(text);
				CPoint3D * lMode = &(lFrame->Vibs->NormMode[(j + cmode)]);
				sprintf(text," %.4f %.4f %.4f", lMode->x, lMode->y, lMode->z);
				Buffer->PutText(text);

				Buffer->PutText("\r");
			}
		}
		delete [] tempAtoms;
		delete [] ModeOffset;
	} else if (AllModes) {
		if (!lFrame->Vibs) return;
			Atom * lAtoms = lFrame->Atoms;
			char * lFreq = lFrame->Vibs->Frequencies, Line[kMaxLineLength];
			long	fPos = 0;
		for (long i=0; i<lFrame->Vibs->GetNumModes(); i++) {
			sprintf(text, "%ld\r", lFrame->NumAtoms);
			Buffer->PutText(text);
			strncpy(Line, &(lFreq[fPos+1]), lFreq[fPos]);
			Line[lFreq[fPos]] = 0;
			fPos += 1 + lFreq[fPos];
			sprintf(text, "Frequency %s\r", Line);
			Buffer->PutText(text);
			for (long j=0; j<lFrame->NumAtoms; j++) {
					Str255	Label;
				Prefs->GetAtomLabel(lAtoms[j].Type-1, Label);
				Label[Label[0]+1]=0;
				sprintf(text, "%s %.4f %.4f %.4f", &(Label[1]), lAtoms[j].Position.x,
					lAtoms[j].Position.y, lAtoms[j].Position.z);
				Buffer->PutText(text);
				long cmode = (lFrame->NumAtoms)*i;
				CPoint3D * lMode = &(lFrame->Vibs->NormMode[(j + cmode)]);
				sprintf(text," %.4f %.4f %.4f", lMode->x, lMode->y, lMode->z);
				Buffer->PutText(text);

				Buffer->PutText("\r");
			}
		}
	} else {	
		for (long i=0; i<NumFrames; i++) {
				Atom * lAtoms = lFrame->Atoms;
			sprintf(text, "%ld\r", lFrame->NumAtoms);
			Buffer->PutText(text);
			sprintf(text, "Frame %ld\r", i+1);
			Buffer->PutText(text);
			for (long j=0; j<lFrame->NumAtoms; j++) {
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
				Buffer->PutText("\r");
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
	Buffer->PutText("\r");
		//2nd line may contain various program data
	Buffer->PutText("  MacMolPlt                       ");
	if (lFrame->Energy != 0.0) {
		sprintf(Line, "%12.5f", lFrame->Energy);
		Buffer->PutText(Line);
	}
	Buffer->PutText("\r\r");	//finish off 2nd line and leave 3rd line blank
	sprintf(Line, "%3ld%3ld  0  0  0  0  0  0  0  0  0 v2000\r",
		lFrame->GetNumAtoms(), lFrame->GetNumBonds());
	Buffer->PutText(Line);
		Atom * lAtoms = lFrame->Atoms;
		long i;
	for (i=0; i<lFrame->GetNumAtoms(); i++) {
			Str255	Label;
		Prefs->GetAtomLabel(lAtoms[i].GetType()-1, Label);
		Label[Label[0]+1]=0;
		sprintf(Line, "%10.4f%10.4f%10.4f %-3s 0  0  0  0  0  0  0  0  0  0  0  0\r",
			lAtoms[i].Position.x, lAtoms[i].Position.y,
			lAtoms[i].Position.z, &(Label[1]));
		Buffer->PutText(Line);
	}
	if (lFrame->GetNumBonds() > 0) {
		Bond * lBonds = lFrame->Bonds;
		for (i=0; i<lFrame->GetNumBonds(); i++) {
			int	type = lBonds[i].Order;
			if (type>3 || type<=0) type = 1;
			sprintf(Line, "%3ld%3ld%3d  0  0  0  0\r", lBonds[i].Atom1+1,
				lBonds[i].Atom2+1, type);
			Buffer->PutText(Line);
		}
	}
		//Final line needs to be end line to terminate properties block
	Buffer->PutText("M  END\r");
}
#endif
void General2DSurface::ReadGrid(const bool Square, const bool UseMult, const double & MultValue) {
#ifdef __wxBuild__
    wxString filename = wxFileSelector(wxT("Choose a file containing the surface data."));
    //We are looking for $ VEC groups. Scan to see how many are there. If more than 1 the user will
    //have to choose.
	FILE * myfile = NULL;
    if (!filename.empty()) {
        myfile = fopen(filename.mb_str(wxConvUTF8), "r");
        if (myfile == NULL) {
            MessageAlert("Unable to open the selected file!");
			return;
        }
	} else
		return;
#else
	//First prompt the user for the file
	FSSpec				inFile;
	short				fileRefNum=0;
	OSErr				myerr;

	if (!GetTextFileName(&inFile)) return;

// Attempt to open the data fork of the selected file
	myerr = FSpOpenDF(&inFile, fsRdPerm, &fileRefNum);
	if (myerr != noErr) fileRefNum = 0;
	if (!fileRefNum) return;	//no file found/chosen
#endif
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
#ifdef __wxBuild__
		Buffer = new BufferFile(myfile, false);
#else
		Buffer = new BufferFile(fileRefNum, false);
#endif
		Buffer->GetLine(Line);
		if (FirstFile) SetLabel(Line);
		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%ld", &LinePos);
		if ((scanerr != 1)||(LinePos<=0))
			throw DataError(4);
		if (FirstFile) NumGridPoints = LinePos;
		else if (LinePos != NumGridPoints) throw DataError(7);
		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%f%f%f", &(tempPt.x), &(tempPt.y), &(tempPt.z));
		if (scanerr != 3) throw DataError(2);
		if (FirstFile) Origin = tempPt;
		else if ((fabs(tempPt.x) < fabs(100*(tempPt.x-Origin.x)))||
				(fabs(tempPt.y) < fabs(100*(tempPt.y-Origin.y)))||
				(fabs(tempPt.z) < fabs(100*(tempPt.z-Origin.z)))) 
					throw DataError(8);
		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%f%f%f", &(tempPt.x), &(tempPt.y), &(tempPt.z));
		if (scanerr != 3) throw DataError(5);
		if (FirstFile) XInc = tempPt;
		else if ((fabs(tempPt.x) < fabs(100*(tempPt.x-XInc.x)))||
				(fabs(tempPt.y) < fabs(100*(tempPt.y-XInc.y)))||
				(fabs(tempPt.z) < fabs(100*(tempPt.z-XInc.z)))) 
					throw DataError(9);
		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%f%f%f", &(tempPt.x), &(tempPt.y), &(tempPt.z));
		if (scanerr != 3) throw DataError(6);
		if (FirstFile) YInc = tempPt;
		else if ((fabs(tempPt.x) < fabs(100*(tempPt.x-YInc.x)))||
				(fabs(tempPt.y) < fabs(100*(tempPt.y-YInc.y)))||
				(fabs(tempPt.z) < fabs(100*(tempPt.z-YInc.z)))) 
					throw DataError(10);
			//allocate memory for the grid
		long TotalPoints = NumGridPoints*NumGridPoints;
		if (FirstFile) {
			AllocateGrid(TotalPoints);
		}
		if (!Grid) throw MemoryError();
		float * lGrid;
#ifdef UseHandles
		HLock(Grid);
		lGrid = (float *) *Grid;
#else
		lGrid = Grid;
#endif
		if (FirstFile) for (LinePos=0; LinePos<TotalPoints; LinePos++)
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
			n++;
		}
#ifdef UseHandles
		HUnlock(Grid);
#endif

		success = true;
	}
	catch (DataError Error) { Error.WriteError();}
	catch (FileError Error) { Error.WriteError();}
	catch (...) {
		success = false;
	}
	if (!success && FirstFile) {	//invalid data or out of memory, etc
		if (Label) {delete [] Label; Label = NULL;}
		FreeGrid();
	}
	if (Buffer) delete Buffer;
#ifdef __wxBuild__
	fclose(myfile);
#else
	FSClose(fileRefNum);
#endif
}
void General3DSurface::ReadGrid(const bool Square, const bool UseValue, const double & MultValue) {
	//First prompt the user for the file
#ifdef __wxBuild__
    wxString filename = wxFileSelector(wxT("Choose a file containing the surface data."));
    //We are looking for $ VEC groups. Scan to see how many are there. If more than 1 the user will
    //have to choose.
	FILE * myfile = NULL;
    if (!filename.empty()) {
        myfile = fopen(filename.mb_str(wxConvUTF8), "r");
        if (myfile == NULL) {
            MessageAlert("Unable to open the selected file!");
			return;
        }
	} else
		return;
#else
	FSSpec				inFile;
	short				fileRefNum=0;
	OSErr				myerr;

	if (!GetTextFileName(&inFile)) return;
	
	// Attempt to open the data fork of the selected file
	myerr = FSpOpenDF(&inFile, fsRdPerm, &fileRefNum);
	if (myerr != noErr) fileRefNum = 0;
	if (!fileRefNum) return;	//no file found/chosen
#endif	

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
#ifdef __wxBuild__
		Buffer = new BufferFile(myfile, false);
#else
		Buffer = new BufferFile(fileRefNum, false);
#endif
		Buffer->GetLine(Line);
		if (FirstFile) SetLabel(Line);	//grab the label only if this is the first data file

		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%ld%ld%ld", &LineLength, &LinePos, &tempL);
		if ((scanerr != 3)||(LineLength<=0)||(LinePos<=0)||(tempL<=0))
			throw DataError(1);
		if (FirstFile) {
			NumXGridPoints = LineLength;
			NumYGridPoints = LinePos;
			NumZGridPoints = tempL;
		} else if ((NumXGridPoints!=LineLength)||(NumYGridPoints!=LinePos)||
			(NumZGridPoints!=tempL)) throw DataError(7);

		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%f%f%f", &(tempPt.x), &(tempPt.y), &(tempPt.z));
		if (scanerr != 3) throw DataError(2);
		if (FirstFile) Origin = tempPt;
		else if ((fabs(tempPt.x) < fabs(100*(tempPt.x-Origin.x)))||
				(fabs(tempPt.y) < fabs(100*(tempPt.y-Origin.y)))||
				(fabs(tempPt.z) < fabs(100*(tempPt.z-Origin.z)))) 
					throw DataError(8);

		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "%f%f%f", &(tempPt.x), &(tempPt.y), &(tempPt.z));
		if (scanerr != 3) throw DataError(3);
		if (FirstFile) {
			XGridInc = tempPt.x;
			YGridInc = tempPt.y;
			ZGridInc = tempPt.z;
		} else if ((fabs(tempPt.x) < fabs(100*(tempPt.x-XGridInc)))||
				(fabs(tempPt.y) < fabs(100*(tempPt.y-YGridInc)))||
				(fabs(tempPt.z) < fabs(100*(tempPt.z-ZGridInc)))) 
					throw DataError(11);
			//allocate memory for the grid (if needed)
		long TotalPoints = NumXGridPoints*NumYGridPoints*NumZGridPoints;
		if (FirstFile) AllocateGrid(TotalPoints);
		if (!Grid) throw MemoryError();
			float * lGrid;
#ifdef UseHandles
		HLock(Grid);
		lGrid = (float *) *Grid;
#else
		lGrid = Grid;
#endif
		if (FirstFile) for (LinePos=0; LinePos<TotalPoints; LinePos++)
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
			n++;
		}
#ifdef UseHandles
		HUnlock(Grid);
#endif

		success = true;
	}
	catch (DataError Error) { Error.WriteError();}
	catch (FileError Error) { Error.WriteError();}
	catch (...) {
		success = false;
	}
	if (!success && FirstFile) {	//invalid data or out of memory, etc
		if (Label) {delete [] Label; Label = NULL;}
		if (Grid) FreeGrid();
	}
	if (Buffer) delete Buffer;
#ifdef __wxBuild__
	fclose(myfile);
#else
	FSClose(fileRefNum);
#endif
}
long LocateKeyWord(const char *Buffer, const char * KeyWord, long length, long bytecount)
{	long	test=0, pos=-1;

	while (!test) {
		for (pos++; ((Buffer[pos] != KeyWord[0])||(Buffer[pos+1] != KeyWord[1]))&&(pos<bytecount)&&
			 (Buffer[pos]!='\0'); pos++) ;
		if (pos>=bytecount) return -1;
		if (Buffer[pos]=='\0') return -1;
		test = 2;
		while ((Buffer[pos+test] == KeyWord[test])&&(test<length)) test++;
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
	
	for (iatm=0; iatm<numExpected; iatm++) {
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
	unsigned char Label[kMaxLineLength];

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
			} else return -iscanerr;
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
				case 'g':
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
				case 'f':
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
				case 'b':
					result = 105;
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
					if (label[2]=='B' || label[2]=='b')
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
