/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

#ifndef __MYFILES__
#define __MYFILES__
#ifndef __BFILE__
#include "BFiles.h"
#endif
#ifndef __MyTypes__
#include "MyTypes.h"
#endif

#define kOpenDLOG			2001		//Custom open dialog

void OpenFile(void);
void OpenTEXTFile(FSSpec *myFile, long flip, float offset, long nSkip);
void OpenMolPFile(FSSpec *myFile);
BufferFile * OpenDatFile(void);
Boolean GetTextFileName(FSSpec *outFile);
long LocateKeyWord(const char *Buffer, const char * KeyWord, long length, long bytecount);
long ParseGLogLine(BufferFile * Buffer, Frame * lFrame, long numExpected, long Type, float *maxsize);
long ParseCartLine(char *Line, long *atomtype, CPoint3D *coord, CPoint3D *offset, long Mode);
long SetAtomType(const unsigned char *TestLabel);

#endif
