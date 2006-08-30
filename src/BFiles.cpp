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

long FindKeyWord(const char *buffer, const char keyin[], long numbyte) {
	long	check;
	
	char * keyword = new char[numbyte+1];
	strncpy(keyword, keyin, numbyte);
	keyword[numbyte] = '\0';
	for (check=0; check<numbyte; check++) 				/* Make sure the keyword is uppercase */
		if ((keyword[check]>96) && (keyword[check]<123)) keyword[check] -= 32;
	long	pos=0;
	long result = -1;
	while (buffer[pos]) {
		check = 0;
		while (((buffer[pos+check] == keyword[check])||(buffer[pos+check]-32 == keyword[check]))&&
			(check < numbyte)) check++;
		if (check == numbyte) {
			result = pos;
			break;
		}
		pos++;
	}
	delete [] keyword;
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
long ReadBooleanKeyword(const char * Line, const char * Keyword, bool * Value) {
	long Pos = LocateForValue(Line, Keyword);
	if (Pos > -1) {
			char token[kMaxLineLength];
		Pos = sscanf(&(Line[Pos]), "%s", token);
		if (Pos == 1) {
			*Value = false;
			if ((token[0] == 't')||(token[1] == 't')) *Value = true;
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

#ifdef UseMacIO
BufferFile::BufferFile(short TargetFileRef, bool Write)
#else
BufferFile::BufferFile(FILE * TargetFileRef, bool Write)
#endif
{
//Initialize the class data
	Buffer = NULL;
	BlockLengths = new long[10];
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
#ifdef UseMacIO
	FileRefNum = TargetFileRef;
#else
	FilePtr = TargetFileRef;
#endif
	if (Write) {
		IOType = 1;
		BufferSize = kBufferSize;
	} else {
		IOType = 0;
#ifdef UseMacIO
		GetEOF(FileRefNum, &ByteCount);				/* Find out how many bytes are in the file */
#else
		fseek(FilePtr, 0, SEEK_END);
		ByteCount = ftell(FilePtr);
		fseek(FilePtr, 0, SEEK_SET);
#endif
		if (ByteCount <= 0) throw DataError(24);
//Allocate the Buffer
		BufferSize = MIN(kBufferSize, ByteCount);
		BlockLengths[0] = ByteCount;
	}
	if (BufferSize <= 0) throw MemoryError("Invalid buffer size!");
#ifdef UseHandles
		OSErr		myerr;
	Buffer = TempNewHandle(BufferSize, &myerr);
	if (!Buffer)
		Buffer = NewHandle(BufferSize);
#else
	Buffer = (Ptr) new char[BufferSize];
#endif
	if (!Buffer) throw MemoryError("Unable to allocate file buffer");
//Got the Buffer so read in the first block
	AdvanceBuffer();
}
#ifdef UseHandles
BufferFile::BufferFile(Handle Data, long DataSize)
#else
BufferFile::BufferFile(Ptr Data, long DataSize)
#endif
{
	BufferStart = BufferPos = 0;
	BufferSize = DataSize;		//Obviously Data should be >= to DataSize or there will be trouble!
#ifdef UseMacIO
	FileRefNum = 0;
#else
	FilePtr = NULL;
#endif
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
	BlockLengths = new long[10];
	if (!BlockLengths) throw MemoryError();
	BlockLengths[0]=ByteCount;
	BlockArrayAllocation = 10;
}
BufferFile::~BufferFile(void) {
	if (IOType == 1) AdvanceBuffer();
	if (Buffer && (IOType != -1)) {	//Dispose the file buffer (if I allocated it)
#ifdef UseHandles
		DisposeHandle(Buffer);
#else
		delete [] Buffer;
#endif
	}
	if (BlockLengths) delete [] BlockLengths;
}
void BufferFile::CloseFile(void) {
#ifdef UseMacIO
		short		tempRefNum = FileRefNum;
#else
		FILE *		tempPtr = FilePtr;
#endif
	delete this;

#ifdef UseMacIO
	FSClose(tempRefNum);
#else
	fclose(tempPtr);
#endif
}
	//Error cleanup routine. Call only to discard buffers.
void BufferFile::AbnormalCleanup(void) {
	IOType = 0;
	BufferPos = 0;
}
long BufferFile::GetFilePos(void) {return BufferStart+BufferPos;}
void BufferFile::SetFilePos(long NewPos) {
	if ((NewPos<0)||(NewPos>ByteCount)) throw FileError(eofErr);
	if ((NewPos>=BufferStart)&&(NewPos<(BufferStart+BufferSize)))
		BufferPos = NewPos - BufferStart;
	else {
		BufferPos = 0;
		BufferStart = NewPos;	//Set the beginning of the buffer to the New pos
		AdvanceBuffer();		//Now cause the buffer to be read in from disk
	}
}
// GetFileType will attempt to determine the type of text file we are dealing
// with (ex: GAMESS log, GAMESS IRC ...) return Unknown when matching fails
// the arguement is really a pascal string
TextFileType BufferFile::GetFileType(const char * fileName) {
	long EntryPos = GetFilePos(), FileSize=ByteCount;
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
			else if (-1<FindKeyWord(temp, ".CML", 4)) Type = CMLFile;
		}
	}
	if ((GetFilePos() < kMaxLineLength) && LocateKeyWord("[MOLDEN FORMAT]", 14, kMaxLineLength))
		Type = MolDenFile;
	while ((ByteCount <= FileSize)&&(Type == kUnknown)) {
		if (LocateKeyWord("GAMESS VERSION", 14, -1))
			Type = kGAMESSlogType;
		else if (LocateKeyWord("IRC INFORMATION PACKET", 22, -1))
			Type = kGAMESSIRCType;
		else if (LocateKeyWord("VELOCITY (BOHR/FS)", 18, -1))
			Type = kGAMESSDRCType;
		else if ((GetFilePos() < kMaxLineLength) && LocateKeyWord("NATOMS", 6, kMaxLineLength))
			Type = kMolType;
		else if (LocateKeyWord("COMPND", 6, -1))
			Type = kPDBType;
		else if (FindGroup("DATA")||FindGroup("CONTRL")) {
			Type = kGAMESSInputType;
		} else if (LocateKeyWord("molecule", 8, -1))
			Type = CMLFile;
		else if (GetFilePos() < kMaxLineLength) {
			try {
					char line[kMaxLineLength];
				SkipnLines(3);
				GetLine(line);
					long t1, t2;
				short scanerr = sscanf(line, "%3ld%3ld", &t1, &t2);
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
long BufferFile::FindGroup(char * GroupName) {
	long result = 0, InitialPos, LineStartPos;
	char Line[kMaxLineLength];

	InitialPos = GetFilePos();
	while (!result && (GetFilePos() < GetFileLength())) {
		LineStartPos = GetFilePos();
		GetLine(Line);
		if (Line[0]==' ' && Line[1] == '$') {	//possible group name
			if (0 == FindKeyWord(&(Line[2]), GroupName, strlen(GroupName)))
				result = true;
		}
	}
	if (!result) SetFilePos(InitialPos);	//reset the initial pos if no group was found
	else SetFilePos(LineStartPos);	//Push the line back into the buffer
	return result;
}
void BufferFile::AdvanceBuffer(void) {
#ifdef UseMacIO
	if (!FileRefNum) throw FileError(0);
#else
	if (!FilePtr) throw FileError(0);
#endif
	if (IOType == 1) {	//Write mode
		long BytesToWrite = MIN(BufferSize, BufferPos);
		if (BytesToWrite > 0) {
#ifdef UseMacIO
			SetFPos(FileRefNum, fsFromStart, BufferStart);
			OSErr myerr = FSWrite(FileRefNum, &BytesToWrite, *Buffer);
			if (myerr != noErr) throw FileError(myerr);
#else
			fseek(FilePtr, BufferStart, SEEK_SET);
			long written = fwrite(Buffer, 1, BytesToWrite, FilePtr);
			if (written != BytesToWrite) throw FileError();
#endif
			ByteCount = BufferStart += BufferPos;
			BufferPos = 0;
		}
	} else {	//Read mode
		long BytesToRead = MIN(BufferSize, (ByteCount-(BufferStart+BufferPos)));
		if (BytesToRead >= 0) {
#ifdef UseMacIO
			SetFPos(FileRefNum, fsFromStart, (BufferStart+BufferPos));
			OSErr myerr = FSRead(FileRefNum, &BytesToRead, *Buffer);
			if (myerr != noErr) throw FileError(myerr);
#else
			fseek(FilePtr, (BufferStart+BufferPos), SEEK_SET);
			long read = fread(Buffer, 1, BytesToRead, FilePtr);
			if (read != BytesToRead) throw FileError();
#endif
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
#ifdef UseHandles
		BlockMoveData(&((*Buffer)[BufferPos]), &(Target[BytesRead]), BlockSize);
#else
		memcpy(&(Target[BytesRead]), &(Buffer[BufferPos]), BlockSize);
#endif
		BytesRead += BlockSize;
		BufferPos += BlockSize;
	}
	return BytesRead;
}
long BufferFile::GetLine(char * Line)
{	long	LineChars=0;
	if (BufferPos>=BufferSize) AdvanceBuffer();	//Just in case...
	if ((BufferStart+BufferPos)>=ByteCount) throw FileError(eofErr);	//End of file reached
			//advance until a line feed or return is found
#ifdef UseHandles
	while (((*Buffer)[BufferPos] != 13)&&((*Buffer)[BufferPos] != 10)) {
		Line[LineChars] = (*Buffer)[BufferPos];
#else
	while ((Buffer[BufferPos] != 13)&&(Buffer[BufferPos] != 10)) {
		Line[LineChars] = Buffer[BufferPos];
#endif
		LineChars++;
		BufferPos ++;
		if ((BufferStart+BufferPos)>=ByteCount) break;	//End of file reached, assume EOL
		if (LineChars>=(kMaxLineLength-2)) break;	//Make sure we don't overrun the Line buffer
		if (BufferPos>=BufferSize) AdvanceBuffer();
	}
#ifdef UseHandles
	if (((BufferPos+BufferStart+1)<ByteCount)&&
		(((*Buffer)[BufferPos] == 13)||((*Buffer)[BufferPos] == 10))) BufferPos++;
#else
	if (((BufferPos+BufferStart)<ByteCount)&&
		((Buffer[BufferPos] == 13)||(Buffer[BufferPos] == 10))) {
		if ((Buffer[BufferPos]==13)&&(Buffer[BufferPos+1]==10)) BufferPos++;
		BufferPos++;
	}
#endif
	Line[LineChars] = 0;	//Make it a proper NULL terminated string
	return LineChars;	//Return the number of characters read (not including the NULL)
}
long BufferFile::PutText(const char * Text)	//This requires a NULL terminated string, but the nil is
{	long	TextChars=0;				//not copied
	while (Text[TextChars]) {
		if (BufferPos>=BufferSize) AdvanceBuffer();	// Flush out the buffer
#ifdef UseHandles
		(*Buffer)[BufferPos] = Text[TextChars];
#else
		Buffer[BufferPos] = Text[TextChars];
#endif
		TextChars++;
		BufferPos ++;
	}
	return TextChars;	//Return the number of characters written
}
void BufferFile::BackupnLines(long nBack) {
	for (long nBacked=0; nBacked<=nBack; nBacked++) {
		if ((BufferStart+BufferPos)<=0) break;	//Beginning of file reached
		if (BufferPos == 0) { BufferStart = MAX(0, (BufferStart-BufferSize)); AdvanceBuffer();}
#ifdef UseHandles
		while (((*Buffer)[BufferPos] != 13)&&((*Buffer)[BufferPos] != 10)) {
#else
		while ((Buffer[BufferPos] != 13)&&(Buffer[BufferPos] != 10)) {
#endif
			BufferPos --;
			if ((BufferStart+BufferPos)==0) break;	//Beginning of file reached
			if (BufferPos == 0) {	//Back up one buffer block
				BufferStart = MAX(0, (BufferStart-BufferSize));
				AdvanceBuffer();
				BufferPos = BufferSize;	//Set pos to end of buffer
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
#ifdef UseHandles
		while (((*Buffer)[BufferPos] != 13)&&((*Buffer)[BufferPos] != 10)) {
#else
		while ((Buffer[BufferPos] != 13)&&(Buffer[BufferPos] != 10)) {
#endif
			BufferPos ++;
			if ((BufferStart+BufferPos)>=ByteCount) break;	//End of file reached, assume EOL
			if (BufferPos>=BufferSize) AdvanceBuffer();
		}
		if ((Buffer[BufferPos]==13)&&(Buffer[BufferPos+1]==10)) BufferPos++;
		BufferPos ++;	//Advance to the first character of the next line
	}
}
long BufferFile::FindBlankLine(void) {
	long Start = BufferStart + BufferPos, LineStart;
	long BlankLinePos=-1;
	bool	done=false;
	
	while (!done) {
		done = true;
		LineStart = BufferStart+BufferPos;
		if (BufferStart + BufferPos >= ByteCount)	// No blank lines found
			break;
		if (BufferPos>=BufferSize) AdvanceBuffer();	//Just in case...
#ifdef UseHandles
		for (; ((*Buffer)[BufferPos] != 13)&&((*Buffer)[BufferPos] != 10); ) {
			if ((*Buffer)[BufferPos] > 32) done = false;	//Is it a printable char?
#else
		for (; (Buffer[BufferPos] != 13)&&(Buffer[BufferPos] != 10); ) {
			if (Buffer[BufferPos] > 32) done = false;	//Is it a printable char?
#endif
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
//Search the file for the specified keyword until found, EOF, or the limit is reached
//Returns true or false, the file position upon exit will be the start of the keyword,
//or the starting position if the keyword is not found.
bool BufferFile::LocateKeyWord(const char Keyword[], long NumByte, long Limit) {
	long OldPosition = GetFilePos();
	char	LineText[kMaxLineLength + 1];
	bool	KeyWordFound=false;
	
	if (Limit < 0) Limit = ByteCount;
	if (Limit > ByteCount) Limit = ByteCount;
	while (!KeyWordFound) {
		if ((BufferStart+BufferPos+1) >= Limit) {	//We've hit the end of the file without
			SetFilePos(OldPosition);				//finding the keyword
			break;
		}
		long lineStartPos = GetFilePos();
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
long BufferFile::GetNumLines(long size) {
	long	StartPos = BufferStart + BufferPos;
	long	EndPos=size + StartPos;
	if (EndPos>ByteCount) EndPos = ByteCount;	//really an error, but I will be leanient here
	if (size == -1) EndPos = ByteCount;		//-1 causes entire file to be scanned
	if (EndPos<=StartPos) return 0;			//No characters, so no lines...
	
	long NumLines=1;
	while ((BufferStart+BufferPos) < EndPos) {
		if (BufferPos>=BufferSize) AdvanceBuffer();
#ifdef UseHandles
		if (((*Buffer)[BufferPos] == 13)||((*Buffer)[BufferPos]==10)) NumLines++;
#else
		if ((Buffer[BufferPos] == 13)||(Buffer[BufferPos]==10)) NumLines++;
		if ((Buffer[BufferPos]==13)&&(Buffer[BufferPos+1]==10)) BufferPos++;
#endif
		
		BufferPos++;
	}
	BufferPos--;
#ifdef UseHandles
	if (((*Buffer)[BufferPos] == 13)||((*Buffer)[BufferPos]==10)) NumLines--;
#else
	if ((Buffer[BufferPos] == 13)||(Buffer[BufferPos]==10)) NumLines--;
#endif
	SetFilePos(StartPos);
	return NumLines;
} /*GetNumLines*/
long BufferFile::BufferSkip(long NumBytes) {
	if ((NumBytes+BufferStart+BufferPos)>ByteCount) throw FileError(eofErr);
	if ((NumBytes+BufferStart+BufferPos)>BlockLengths[0]) throw DataError();
	if (((NumBytes+BufferPos)<BufferSize)||(IOType==-1)) BufferPos += NumBytes;
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
#ifdef UseHandles
			BlockMoveData(&(Source[BytesWritten]), &((*Buffer)[BufferPos]), BlockSize);
#else
			memcpy(&(Buffer[BufferPos]), &(Source[BytesWritten]), BlockSize);
#endif
			BytesWritten += BlockSize;
			BufferPos += BlockSize;
		}
	}
	return NumBytes;
}
long BufferFile::WriteLine(Ptr text, bool NewLine) {
	long nchar = strlen(text);
	if (DoIt) {
		if ((nchar+BufferPos)>(BufferSize-10)) {//Make sure there is room in the buffer
			AdvanceBuffer();					//for the text + any possible EOL chars
		}
		if (((nchar+LinePos)>ColsPerLine)&&(nchar<ColsPerLine)) {
				//Wrap line and indent the next line, but don't wrap a single huge line
#ifdef UseHandles
			sprintf(&((*Buffer)[BufferPos]), "%c    ", EOLchar);
#else
			sprintf(&(Buffer[BufferPos]), "%c    ", EOLchar);
#endif
			BufferPos+=5;
			LinePos=4;
		}
		for (long i=0; i<nchar; i++) {
#ifdef UseHandles
			(*Buffer)[BufferPos+i] = text[i];
#else
			Buffer[BufferPos+i] = text[i];
#endif
		}
		BufferPos += nchar;
		LinePos += nchar;
		if (NewLine) {	//User requested a newline (no blanks for the next line)
						//Using Macintosh style '\r' EOL char. UNIX uses '\n'
						//but they should get converted by an FTP program normally
#ifdef UseHandles
			(*Buffer)[BufferPos] = EOLchar;
#else
			Buffer[BufferPos] = EOLchar;
#endif
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
	long BlockEndPosition = length + GetFilePos();
		//The new end position cannot be longer than the current end point
	if (BlockEndPosition > BlockLengths[0]) throw DataError();
	if (BlockCount >= BlockArrayAllocation) {
		long * temp = new long[BlockArrayAllocation+10];
		if (!temp) throw MemoryError();
#ifdef UseHandles
		BlockMoveData((Ptr) BlockLengths, (Ptr) temp, BlockArrayAllocation*sizeof(long));
#else
		memcpy((Ptr) temp, (Ptr) BlockLengths, BlockArrayAllocation*sizeof(long));
#endif
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
