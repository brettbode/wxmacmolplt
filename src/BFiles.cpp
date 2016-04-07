/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥
	BFiles.cpp

	Classes to read/write a MacMolPlt binary file

	Brett Bode - 6/96
	Adapted for Std C IO for Rhapsody - 2/98 BMB
	Added block read length restrictions - 10/98 BMB
	Avoid potential NULL filename dereference in GetFileType - 12/2002 BMB
	Use EOLchar for line feeds in WriteLine - 12/2002 BMB
  ¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥¥*/
#include "Globals.h"
#include "BFiles.h"
#include "GlobalExceptions.h"
#include "MyTypes.h"
#include "Prefs.h"
#include <string.h>

extern WinPrefs *	gPreferences;

/**
 * This function checks buffer to see if it contains the keyword.  If so,
 * a value > 0 is returned.  Otherwise, -1 is returned.
 * @param buffer The buffer to scan through.
 * @param keyin The keyword to search for. Must be uppercase.
 * @param numbyte The number of characters in the keyword.
 */
long FindKeyWord(const char *buffer, const char keyin[], long numbyte) {
	long	check;

#if defined(__WXDEBUG__)
//	char * keyword = new char[numbyte+1];
//	strncpy(keyword, keyin, numbyte);
//	keyword[numbyte] = '\0';
//	for (check=0; check<numbyte; check++) 				/* Make sure the keyword is uppercase */
//		if ((keyword[check]>96) && (keyword[check]<123)) keyword[check] -= 32;
	for (check=0; check<numbyte; check++)
		if ((keyin[check]>96) && (keyin[check]<123)) {
			wxLogMessage(_("Error: keyword search on lower case keyword"));
		}
#endif
	long	pos=0;
	long result = -1;
	while (buffer[pos]) {
		check = 0;
		bool good=true;
		while ((check<numbyte) && good) {
			good=false;
			if ((buffer[pos+check]>='a')&&(buffer[pos+check]<='z')) {
				if (buffer[pos+check]-32 == keyin[check]) {
					good=true;
					check++;
				}
			} else {
				if (buffer[pos+check] == keyin[check]) {
					good=true;
					check++;
				}
			}
		}
		if (check == numbyte) {
			result = pos;
			break;
		}
		pos++;
	}
//	delete [] keyword;
	return result;
} /* FindKeyword */

long ReadStringKeyword(const char * Line, const char * Keyword, char * Value) {
	long Pos = LocateForValue(Line, Keyword);
	if (Pos > -1) {
		Pos = sscanf(&(Line[Pos]), "%s", Value);
	}
	return (Pos == 1);
}
long ReadLongKeyword(const char * Line, const char * Keyword, long * Value) {
	long Pos = LocateForValue(Line, Keyword);
	if (Pos > -1) {
		Pos = sscanf(&(Line[Pos]), "%ld", Value);
	}
	return (Pos == 1);
}
long ReadFloatKeyword(const char * Line, const char * Keyword, float * Value) {
	long Pos = LocateForValue(Line, Keyword);
	if (Pos > -1) {
		Pos = sscanf(&(Line[Pos]), "%f", Value);
	}
	return (Pos == 1);
}
long ReadDoubleKeyword(const char * Line, const char * Keyword, double & Value) {
	long Pos = LocateForValue(Line, Keyword);
	if (Pos > -1) {
		Pos = sscanf(&(Line[Pos]), "%lf", &Value);
	}
	return (Pos == 1);
}
long ReadBooleanKeyword(const char * Line, const char * Keyword, bool * Value) {
	long Pos = LocateForValue(Line, Keyword);
	if (Pos > -1) {
			char token[kMaxLineLength];
		Pos = sscanf(&(Line[Pos]), "%s", token);
		if (Pos == 1) {
			*Value = false;
			if (! strcasecmp(token, ".t.") ||
				! strcasecmp(token, ".true.")) *Value = true;
		}
	}
	return (Pos == 1);
}
//This routine should find a keyword on the provided line (if it is there) and
//return the position just after the '=' sign. 
long LocateForValue(const char * Line, const char * KeyWord) {
	long Pos, Length;
	Length = strlen(KeyWord);
	Pos = FindKeyWord(Line, KeyWord, Length);
	if (Pos > -1) {
		if ((Pos == 0) || (Line[Pos-1] == ' ') || (Line[Pos-1] == '	')) {
			Pos += Length;
			if ((Line[Pos] == '=')||(Line[Pos]==' ')||(Line[Pos]=='	')) {
				while ((Line[Pos] != '=')&&(Line[Pos] != 0)) {
					if ((Line[Pos]!=' ')&&(Line[Pos]!='	')) break;
					Pos++;
				}
				if (Line[Pos] != '=') Pos = -1;
				else Pos++;
			} else Pos = -1;
		} else Pos = -1;
	}
	return Pos;
}
/** Convert old fortran style exponents using D or Q into c style e or E.
 * The routine simply replaces all 'd's, 'D's, 'q's and 'Q's with 'e'.
 * @param Line A null-terminated string that will be modified in place.
 */
void ConvertExponentStyle(char * Line) {
	int i=0;
	while ((Line[i] != '\0')&&(i<kMaxLineLength)) {
		if ((tolower(Line[i]) == 'd')||(tolower(Line[i]) == 'q'))
			Line[i] = 'e';
		i++;
	}
}

BufferFile::BufferFile(FILE * TargetFileRef, bool Write) {
//Initialize the class data
	Buffer = NULL;
	BlockLengths = new wxFileOffset[10];
	if (!BlockLengths) throw MemoryError();
	BlockArrayAllocation = 10;
	BlockLengths[0] = 0;
	BlockCount = 1;
	ByteCount = BufferStart = BufferPos = LinePos = 0;
	ColsPerLine = 80;
	DoIt = true;
	if (gPreferences->NativeEOLChar()) {
#if defined(CarbonBuild) | defined(__wxBuild__)
		EOLchar = '\n';
#else
		EOLchar = '\r';
#endif
	} else {
#if defined(CarbonBuild) | defined(__wxBuild__)
		EOLchar = '\r';
#else
		EOLchar = '\n';
#endif
	}
	
//check out the file
	myFile = new wxFFile(TargetFileRef);
	if (Write) {
		IOType = 1;
		BufferSize = kBufferSize;
	} else {
		IOType = 0;
		ByteCount = myFile->Length();
		if (ByteCount <= 0) {
			wxLogMessage(_("The requested file is empty."));
			throw DataError();
		}
//Allocate the Buffer
		BufferSize = (long) MIN(kBufferSize, ByteCount);
		BlockLengths[0] = ByteCount;
	}
	if (BufferSize <= 0) throw MemoryError("Invalid buffer size!");
	Buffer = new char[BufferSize];
	if (!Buffer) throw MemoryError("Unable to allocate file buffer");
//Got the Buffer so read in the first block
	AdvanceBuffer();
}
BufferFile::BufferFile(char * Data, long DataSize)
{
	BufferStart = BufferPos = 0;
	BufferSize = DataSize;		//Obviously Data should be >= to DataSize or there will be trouble!
	myFile = NULL;
	IOType = -1;	//No file associated with this buffer
					//and the buffer should not be freed when this object is destroyed
	DoIt = true;
	if (gPreferences->NativeEOLChar()) {
#if defined(CarbonBuild) | defined(__wxBuild__)
		EOLchar = '\n';
#else
		EOLchar = '\r';
#endif
	} else {
#if defined(CarbonBuild) | defined(__wxBuild__)
		EOLchar = '\r';
#else
		EOLchar = '\n';
#endif
	}
	LinePos = 0;
	ColsPerLine = 80;
	Buffer = Data;
	ByteCount = DataSize;
	BlockCount = 1;
	BlockLengths = new wxFileOffset[10];
	if (!BlockLengths) {
		throw MemoryError();
	}
	BlockLengths[0]=ByteCount;
	BlockArrayAllocation = 10;
}
BufferFile::~BufferFile(void) {
	if (IOType == 1) AdvanceBuffer();
	if (Buffer && (IOType != -1)) {	//Dispose the file buffer (if I allocated it)
		delete [] Buffer;
	}
	if (BlockLengths) delete [] BlockLengths;
}
void BufferFile::CloseFile(void) {
	myFile->Close();
	delete this;
}
	//Error cleanup routine. Call only to discard buffers.
void BufferFile::AbnormalCleanup(void) {
	IOType = 0;
	BufferPos = 0;
}
void BufferFile::SetFilePos(wxFileOffset NewPos) {
	if ((NewPos<0)||(NewPos>ByteCount)) throw FileError(eofErr);
	if ((NewPos>=BufferStart)&&(NewPos<(BufferStart+BufferSize)))
		BufferPos = (size_t) (NewPos - BufferStart);
	else {
		BufferPos = 0;
		BufferStart = NewPos;	//Set the beginning of the buffer to the New pos
		AdvanceBuffer();		//Now cause the buffer to be read in from disk
	}
}
// GetFileType will attempt to determine the type of text file we are dealing
// with (ex: GAMESS log, GAMESS IRC ...) return Unknown when matching fails
// the argument is really a pascal string
TextFileType BufferFile::GetFileType(const char * fileName) {
	wxFileOffset EntryPos = GetFilePos(), FileSize=ByteCount;
	TextFileType	Type=kUnknown;

	ByteCount = BufferSize;
		//Check file extention first
	if (fileName != NULL) {
		int len = strlen(fileName);
		if (len > 4) {
			const char * temp = &(fileName[len-4]);
			if (-1<FindKeyWord(temp, ".XYZ", 4)) Type = kXYZType;
			else if (-1<FindKeyWord(temp, ".PDB", 4)) Type = kPDBType;
			else if (-1<FindKeyWord(temp, ".INP", 4)) Type = kGAMESSInputType;
			else if (-1<FindKeyWord(temp, ".TRJ", 4)) Type = kGAMESSTRJType;
			else if (-1<FindKeyWord(temp, ".CML", 4)) Type = CMLFile;
			else if (-1<FindKeyWord(temp, ".MKL", 4)) Type = MolekelFile;
			else if (-1<FindKeyWord(temp, ".ARC", 4)) Type = MOPACarcFile;
			else if (-1<FindKeyWord(temp, ".MOP", 4)) Type = MOPACmopFile;
		}
	}
	
	if ((GetFilePos() < kMaxLineLength) && LocateKeyWord("[MOLDEN FORMAT]", 14, kMaxLineLength))
		Type = MolDenFile;
	while ((ByteCount <= FileSize)&&(Type == kUnknown)) {
		if (LocateKeyWord("GAMESS VERSION", 14, -1))
			Type = kGAMESSlogType;
		else if (LocateKeyWord("===== IRC DATA PACKET", 21, -1))
			Type = kGAMESSTRJType;
		else if (LocateKeyWord("===== DRC DATA PACKET", 21, -1))
			Type = kGAMESSTRJType;
		else if (LocateKeyWord("===== MD DATA PACKET", 20, -1))
			Type = kGAMESSTRJType;
		else if (LocateKeyWord("IRC INFORMATION PACKET", 22, -1))
			Type = kGAMESSIRCType;
		else if (LocateKeyWord("VELOCITY (BOHR/FS)", 18, -1))
			Type = kGAMESSDRCType;
		else if (LocateKeyWord("GAMESS (US) VERSION", 19, -1))
			Type = kGAMESSlogType;
		else if ((GetFilePos() < kMaxLineLength) && LocateKeyWord("NATOMS", 6, kMaxLineLength))
			Type = kMolType;
		else if (LocateKeyWord("COMPND", 6, -1))
			Type = kPDBType;
		else if (FindGroup("DATA")||FindGroup("CONTRL")) {
			Type = kGAMESSInputType;
			//technically molecule is lower case, but LocateKeyword requires upper case
		} else if (LocateKeyWord("<MOLECULE>", 10, -1))
			Type = CMLFile;
		else if (GetFilePos() < kMaxLineLength) {
			try {
					char line[kMaxLineLength];
				SkipnLines(3);
				GetLine(line);
					long t1, t2;
				int scanerr = sscanf(line, "%3ld%3ld", &t1, &t2);
				if ((scanerr==2)&&((t1>0)&&(t2>=0))) Type = kMDLMolFile;
				else {
					SetFilePos(EntryPos);
					GetLine(line);
					long temp;
					scanerr = sscanf(line, "%ld", &temp);
					if ((scanerr==1)&&(temp>0)) Type = kXYZType;
				}
			}
			catch (FileError) {
			}
			SetFilePos(EntryPos);
		}
		if (ByteCount >= FileSize) break;
		ByteCount += BufferSize;
		if (ByteCount > FileSize) ByteCount = FileSize;
	}

	ByteCount = FileSize;	//return buffer to its original state before returning
	SetFilePos(EntryPos);
	return Type;
}

/**
 * This function scans the buffered file for the group specified by 
 * GroupName.  In the GAMESS file format, groups start with a line
 * containing " $GroupName" and end with " $END".  If the group is found,
 * the file pointer is set to point at the " $GroupName" line.  Otherwise,
 * the file pointer remains unchanged.
 */
wxFileOffset BufferFile::FindGroup(const char * GroupName) {
	wxFileOffset result = 0, InitialPos, LineStartPos;
	char Line[kMaxLineLength];

	InitialPos = GetFilePos();
	while (!result && (GetFilePos() < GetFileLength())) {
		LineStartPos = GetFilePos();
		GetLine(Line);
		if (Line[0]==' ' && Line[1] == '$') {	//possible group name
			if (0 == FindKeyWord(&(Line[2]), GroupName, strlen(GroupName)))
				result = 1;
		}
	}
	if (!result) SetFilePos(InitialPos);	//reset the initial pos if no group was found
	else SetFilePos(LineStartPos);	//Push the line back into the buffer
	return result;
}

/** Search the file for the keywords FINAL and ENERGY on the same line until found, EOF,
 * or the limit is reached. Useful for GAMESS log files.
 * Returns true or false, the file position upon exit will be the start of "FINAL",
 * or the starting position if the keyword is not found.
 * @param Limit (optional) The file position limit (in bytes) to limit the search. -1 will
 *							search to the end of the file.
 */
bool BufferFile::LocateFinalEnergy(wxFileOffset Limit) {
	bool result = false;
	char LineText[kMaxLineLength];
	
	wxFileOffset StartPos = GetFilePos();
	
	while (LocateKeyWord("FINAL", 5, Limit)) {
		wxFileOffset FINALPos = GetFilePos();
		GetLine(LineText);
		int LinePos = FindKeyWord(LineText, "ENERGY", 6);
		if (LinePos > -1) {	//Found energy on the same line
			SetFilePos(FINALPos);
			result = true;
			break;
		}
	}
	if (!result) SetFilePos(StartPos);
	return result;
}

void BufferFile::AdvanceBuffer(void) {
	if (IOType == 1) {	//Write mode
		long BytesToWrite = MIN(BufferSize, BufferPos);
		if (BytesToWrite > 0) {
			if (!myFile) throw FileError(0);
			myFile->Seek(BufferStart);
			long written = myFile->Write(Buffer, BytesToWrite);
			if (written != BytesToWrite) throw FileError();
			ByteCount = BufferStart += BufferPos;
			BufferPos = 0;
		}
	} else {	//Read mode
		long BytesToRead = (long) MIN(BufferSize, (ByteCount-(BufferStart+BufferPos)));
		if (BytesToRead > 0) {
			if (!myFile) throw FileError(0);
			myFile->Seek(BufferStart+BufferPos);
			long read = myFile->Read(Buffer, BytesToRead);
			if (read != BytesToRead) throw FileError();
			BufferStart += BufferPos;
			BufferPos = 0;
		}
	}
}
long BufferFile::Read(Ptr Target, long NumBytes) {
	long BytesRead=0, BlockSize;
	if ((NumBytes+BufferStart+BufferPos)>ByteCount) throw FileError(eofErr);
	if ((NumBytes+BufferStart+BufferPos)>BlockLengths[0]) throw DataError();
	while (BytesRead<NumBytes) {
		if (BufferPos>=BufferSize) AdvanceBuffer();
		BlockSize = MIN((NumBytes-BytesRead), (BufferSize-BufferPos));
		memcpy(&(Target[BytesRead]), &(Buffer[BufferPos]), BlockSize);
		BytesRead += BlockSize;
		BufferPos += BlockSize;
	}
	return BytesRead;
}

/**
 * This function reads a single line from the buffer. It reads up until
 * the newline, copying all characters into Line.  The newline is not
 * copied into Line and the file pointer is set one character past it.
 * @param Line A preallocated buffer into which the line from the file is copied.
 * @return The number of characters copied.
 */
long BufferFile::GetLine(char * Line)
{	long	LineChars=0;
	if (BufferPos>=BufferSize) AdvanceBuffer();	//Just in case...
	if ((BufferStart+BufferPos)>=ByteCount) throw FileError(eofErr);	//End of file reached
			//advance until a line feed or return is found
	while ((Buffer[BufferPos] != 13)&&(Buffer[BufferPos] != 10)) {
		Line[LineChars] = Buffer[BufferPos];
		LineChars++;
		BufferPos ++;
		if ((BufferStart+BufferPos)>=ByteCount) break;	//End of file reached, assume EOL
		if (LineChars>=(kMaxLineLength-2)) { //Make sure we don't overrun the Line buffer
			wxLogMessage(_("Warning: the maximum line length has been exceeded, skipping the rest of a very long line!"));
			while ((Buffer[BufferPos] != 13)&&(Buffer[BufferPos] != 10)) {
				BufferPos ++;
				if ((BufferStart+BufferPos)>=ByteCount) break;	//End of file reached, assume EOL
				if (BufferPos>=BufferSize) AdvanceBuffer();
			}
			break;
		}
		if (BufferPos>=BufferSize) AdvanceBuffer();
	}
	if ((BufferPos+BufferStart)<ByteCount) {
		if (BufferPos >= BufferSize) AdvanceBuffer();
		if ((Buffer[BufferPos] == 13)||(Buffer[BufferPos] == 10)) {
			BufferPos++;
			if (Buffer[BufferPos-1]==13) {
				if (BufferPos >= BufferSize) AdvanceBuffer();
				if (Buffer[BufferPos]==10) BufferPos++;
			}
		}
	}
	Line[LineChars] = 0;	//Make it a proper NULL terminated string
	return LineChars;	//Return the number of characters read (not including the NULL)
}
long BufferFile::PutText(const char * Text)	//This requires a NULL terminated string, but the nil is
{	long	TextChars=0;				//not copied
	while (Text[TextChars]) {
		if (BufferPos>=BufferSize) AdvanceBuffer();	// Flush out the buffer
		Buffer[BufferPos] = Text[TextChars];
		TextChars++;
		BufferPos ++;
	}
	return TextChars;	//Return the number of characters written
}
void BufferFile::BackupnLines(long nBack) {
	for (long nBacked=0; nBacked<=nBack; nBacked++) {
		if ((BufferStart+BufferPos)<=0) break;	//Beginning of file reached
		if (BufferPos == 0) {
			wxFileOffset cPos = GetFilePos();
			BufferStart = MAX(0, (BufferStart-BufferSize-1)); 
			AdvanceBuffer();
			BufferPos = (long) (cPos-BufferStart);	//Set pos to end of buffer
		}
		while ((Buffer[BufferPos] != 13)&&(Buffer[BufferPos] != 10)) {
			BufferPos --;
			if ((BufferStart+BufferPos)==0) break;	//Beginning of file reached
			if (BufferPos == 0) {	//Back up one buffer block
				wxFileOffset cPos = GetFilePos();
				BufferStart = MAX(0, (BufferStart-BufferSize-1));
				AdvanceBuffer();
				BufferPos = (long) (cPos-BufferStart-1);	//Set pos to end of buffer
				if (BufferPos<0) BufferPos = 0;	//Don't think this is possible, but just to be safe.
			}
		}
		if ((Buffer[BufferPos]==10)&&(Buffer[BufferPos-1]==13)) BufferPos--;
		BufferPos --;
	}
	if (BufferPos>0) BufferPos += 2;
	if (BufferPos <0) BufferPos = 0;
}
void BufferFile::SkipnLines(long nSkip) {
	for (long nSkipped=0; nSkipped<nSkip; nSkipped++) {	//advance until a line feed or return is found
		if ((BufferStart+BufferPos)>=ByteCount) throw FileError(eofErr);	//End of file reached
		if (BufferPos>=BufferSize) AdvanceBuffer();	//Just in case...
		while ((Buffer[BufferPos] != 13)&&(Buffer[BufferPos] != 10)) {
			BufferPos ++;
			if ((BufferStart+BufferPos)>=ByteCount) break;	//End of file reached, assume EOL
			if (BufferPos>=BufferSize) AdvanceBuffer();
		}
		if ((Buffer[BufferPos]==13)&&(Buffer[BufferPos+1]==10)) BufferPos++;
		BufferPos ++;	//Advance to the first character of the next line
	}
}
bool IsBlank(const char * Line) {
	bool result = true;
	long length = strlen(Line);
	for (long i=0; i<length; i++) {
		if (Line[i]>32) {
			result = false;
			break;
		}
	}
	
	return result;
}
wxFileOffset BufferFile::FindBlankLine(void) {
	wxFileOffset Start = BufferStart + BufferPos, LineStart;
	wxFileOffset BlankLinePos=-1;
	bool	done=false;
	
	while (!done) {
		done = true;
		LineStart = BufferStart+BufferPos;
		if (BufferStart + BufferPos >= ByteCount)	// No blank lines found
			break;
		if (BufferPos>=BufferSize) AdvanceBuffer();	//Just in case...
		for (; (Buffer[BufferPos] != 13)&&(Buffer[BufferPos] != 10); ) {
			if (Buffer[BufferPos] > 32) done = false;	//Is it a printable char?
			BufferPos++;
			if ((BufferStart+BufferPos)>=ByteCount) break;	//End of file reached, assume EOL
			if (BufferPos>=BufferSize) AdvanceBuffer();
		}
		if ((Buffer[BufferPos]==13)&&(Buffer[BufferPos+1]==10)) BufferPos++;
		if (done) BlankLinePos = LineStart;
		BufferPos ++;
	}
		//return the buffer position to where it was when this function was called
	SetFilePos(Start);
	return BlankLinePos;
}
//Search the file for the specified keyword until found, EOF, or the limit is
//reached Returns true or false, the file position upon exit will be the start
//of the keyword, or the starting position if the keyword is not found.
bool BufferFile::LocateKeyWord(const char Keyword[], long NumByte, wxFileOffset Limit) {
	wxFileOffset OldPosition = GetFilePos();
	char	LineText[kMaxLineLength + 1];
	bool	KeyWordFound=false;
	
	if (Limit < 0) Limit = ByteCount;
	if (Limit > ByteCount) Limit = ByteCount;
	while (!KeyWordFound) {
		if ((BufferStart+BufferPos+1) >= Limit) {	//We've hit the end of the file without
			SetFilePos(OldPosition);				//finding the keyword
			break;
		}
		wxFileOffset lineStartPos = GetFilePos();
		GetLine(LineText);
		long Start = FindKeyWord(LineText, Keyword, NumByte);
		if (Start > -1) {	//Found it!
			KeyWordFound = true;
			OldPosition = lineStartPos + Start;
			SetFilePos(OldPosition);
		}
	}
	return KeyWordFound;
}
/** Search the file for the keywords provided in the vector of strings.
 * returns the int of the string/int pair or -1 if none are found. The buffer is left
 * at the start of the found keyword or the starting position if none are found.
 */
int BufferFile::LocateKeyWord(const std::vector<std::pair<std::string, int> > & keywords, int NumKeywords, wxFileOffset Limit) {
	int result=-1;
	if (NumKeywords < 0) NumKeywords = keywords.size();
	wxFileOffset OldPosition = GetFilePos();
	char	LineText[kMaxLineLength + 1];
	bool	KeyWordFound=false;
	
	if (Limit < 0) Limit = ByteCount;
	if (Limit > ByteCount) Limit = ByteCount;
	 while (!KeyWordFound) {
		 if ((BufferStart+BufferPos+1) >= Limit) {	//We've hit the end of the file without
			 SetFilePos(OldPosition);				//finding the keyword
			 break;
		 }
		 wxFileOffset lineStartPos = GetFilePos();
		 GetLine(LineText);
		 for (int ik=0; ik < keywords.size(); ik++) {
			 long NumByte = keywords[ik].first.size();
			 long Start = FindKeyWord(LineText, keywords[ik].first.c_str(), NumByte);
			 if (Start > -1) {	//Found it!
				 KeyWordFound = true;
				 OldPosition = lineStartPos + Start;
				 SetFilePos(OldPosition);
				 result = keywords[ik].second;
				 break;
			 }
		 }
	 }
	 
	return result;
}

long BufferFile::GetNumLines(long size) {
	wxFileOffset	StartPos = BufferStart + BufferPos;
	wxFileOffset	EndPos=size + StartPos;
	if (EndPos>ByteCount) EndPos = ByteCount;	//really an error, but I will be leanient here
	if (size == -1) EndPos = ByteCount;		//-1 causes entire file to be scanned
	if (EndPos<=StartPos) return 0;			//No characters, so no lines...
	
	long NumLines=1;
	while ((BufferStart+BufferPos) < EndPos) {
		if (BufferPos>=BufferSize) AdvanceBuffer();
		if ((Buffer[BufferPos] == 13)||(Buffer[BufferPos]==10)) NumLines++;
		if ((Buffer[BufferPos]==13)&&(Buffer[BufferPos+1]==10)) BufferPos++;
		
		BufferPos++;
	}
	BufferPos--;
	if ((Buffer[BufferPos] == 13)||(Buffer[BufferPos]==10)) NumLines--;
	SetFilePos(StartPos);
	return NumLines;
} /*GetNumLines*/
wxFileOffset BufferFile::BufferSkip(wxFileOffset NumBytes) {
	if ((NumBytes+BufferStart+BufferPos)>ByteCount) throw FileError(eofErr);
	if ((NumBytes+BufferStart+BufferPos)>BlockLengths[0]) throw DataError();
	if (((NumBytes+BufferPos)<BufferSize)||(IOType==-1)) BufferPos += (long) NumBytes;
	else {
		BufferStart += BufferPos + NumBytes;
		BufferPos = 0;
		AdvanceBuffer();
	}
	return NumBytes;
}
long BufferFile::Write(const char * Source, long NumBytes) {
	if (DoIt) {	//If DoIt is false then no writing will be attempted
		long BytesWritten = 0, BlockSize;

		while (BytesWritten<NumBytes) {
			if (BufferPos>=BufferSize) AdvanceBuffer();
			BlockSize = MIN((NumBytes-BytesWritten), (BufferSize-BufferPos));
			memcpy(&(Buffer[BufferPos]), &(Source[BytesWritten]), BlockSize);
			BytesWritten += BlockSize;
			BufferPos += BlockSize;
		}
	}
	return NumBytes;
}
long BufferFile::WriteLine(const char *text, bool NewLine) {
	long nchar = strlen(text);
	if (DoIt) {
		if ((nchar+BufferPos)>(BufferSize-10)) {//Make sure there is room in the buffer
			AdvanceBuffer();					//for the text + any possible EOL chars
		}
		if (((nchar+LinePos)>ColsPerLine)&&(nchar<ColsPerLine)) {
				//Wrap line and indent the next line, but don't wrap a single huge line
			sprintf(&(Buffer[BufferPos]), "%c    ", EOLchar);
			BufferPos+=5;
			LinePos=4;
		}
		for (long i=0; i<nchar; i++) {
			Buffer[BufferPos+i] = text[i];
		}
		BufferPos += nchar;
		LinePos += nchar;
		if (NewLine) {	//User requested a newline (no blanks for the next line)
						//Tack on an EOL char as set when the Buffer was initialized.
			Buffer[BufferPos] = EOLchar;
			BufferPos ++;
			LinePos = 0;
		}
	}
	return nchar;
}
bool BufferFile::SetOutput(bool State) {
	DoIt = State;
	return DoIt;
}
bool BufferFile::GetOutput(void) { return DoIt; }
//Setup a new restricted length read block. Length is
//assumed to be the length from the current position to the
//end of the block.
void BufferFile::SetBlockLength(long length) {
	wxFileOffset BlockEndPosition = length + GetFilePos();
		//The new end position cannot be longer than the current end point
	if (BlockEndPosition > BlockLengths[0]) throw DataError();
	if (BlockCount >= BlockArrayAllocation) {
		wxFileOffset * temp = new wxFileOffset[BlockArrayAllocation+10];
		if (!temp) throw MemoryError();
		memcpy((Ptr) temp, (Ptr) BlockLengths, BlockArrayAllocation*sizeof(wxFileOffset));
		delete [] BlockLengths;
		BlockLengths = temp;
		BlockArrayAllocation += 10;
	}
	for (long i=BlockCount; i>0; i--) {
		BlockLengths[i] = BlockLengths[i-1];
	}
	BlockLengths[0] = BlockEndPosition;
	BlockCount++;
}
void BufferFile::FinishBlock(void) {
	SetFilePos(BlockLengths[0]);
	if (BlockCount>1) {
		for (long i=0; i<BlockCount-1; i++)
			BlockLengths[i] = BlockLengths[i+1];
		BlockCount--;
	}
}
bool BufferFile::ReadLongValue(long & target, wxFileOffset Limit) {
	bool result = false;
	char work[kMaxLineLength];
	if (GetFilePos() >= GetFileLength()) return false;	//already at the EOF
	if (Limit < 0) Limit = GetFileLength();
	
	wxFileOffset SaveStart = GetFilePos();
	//Advance over the whitespace (non-printable chars, space and commas
	do {
		if ((1 != Read(work, 1))||(GetFilePos() >= Limit)) {
			SetFilePos(SaveStart);
			return false;
		}
	} while ((work[0]<=' ')&&(work[0]!=','));
	
	//work[0] now has the first char
	int count=0;
	while (((work[count]>='0')&&(work[count]<='9'))||(work[count]=='-')) {
		++count;
		if ((1 != Read(&(work[count]), 1))||(GetFilePos() >= Limit)||
			(count>(kMaxLineLength-2)))
			break;
	}
		//backup over the non #.
	if (GetFilePos() < Limit) SetFilePos(GetFilePos()-1);
	if (count > 0) {
		work[count] = '\0';
		char * test;
		target = strtol(work, &test, 0);
		if ((test!=work)&&(test[0]=='\0')) {
			result = true;
		}
	}
	if (!result) SetFilePos(SaveStart);
	
	return result;
}

#ifdef _MSC_VER
//functions Visual studio lacks...
int strcasecmp(const char * a, const char * b) {
	int pos = 0, result = -1;
	while ((a[pos] != '\0')&&(b[pos]!='\0')) {
		if (toupper(a[pos]) != toupper(b[pos])) break;
		pos ++;
	}
	if ((a[pos] == '\0')&&(b[pos]=='\0')) result = 0;	//strings matched
	return result;
}
int strncasecmp(const char * a, const char * b, int max) {
	int pos = 0, result = -1;
	while ((a[pos] != '\0')&&(b[pos]!='\0')&&(pos<max)) {
		if (toupper(a[pos]) != toupper(b[pos])) break;
		pos ++;
	}
	if ((a[pos] == '\0')&&(b[pos]=='\0')) result = 0;	//strings matched
	return result;
}
#endif
