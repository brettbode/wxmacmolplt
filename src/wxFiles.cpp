/*
 *  (c) 2006 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* Routines to manage the open/save file interfaces

	Created by Brett Bode 3/2006
*/

#include "Globals.h"
#include "GlobalExceptions.h"
#include "Progress.h"
#include "MoleculeData.h"
#include "BFiles.h"
#include "myFiles.h"
#include "InputData.h"
#include "Prefs.h"

extern WinPrefs *	gPreferences;

	//Prompt for a filename and then write out a valid input file for GAMESS
long InputData::WriteInputFile(MoleculeData * lData, WinPrefs * Prefs) {

	return 1;
}

	//Prompt for the dat file name and return a buffer to it
BufferFile * OpenDatFile(void) {
	BufferFile * Buffer = NULL;

	return Buffer;
}
