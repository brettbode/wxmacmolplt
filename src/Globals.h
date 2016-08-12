/*
 *  (c) 2004-2011 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* Global Definitions:	Written by Brett Bode - January 1998

	Machine specific data type and compiler directives should be
	placed in this file to be included in every other source file.

	Created by Brett Bode  July 1994
	many additions and changes up to version 1.2 in January 1995
	Major revamping of the master data structure for multiple frames January 1995
	Most stuff moved elsewhere for the major rewrite done in September 1996
	Macintosh (PPC and 68K) directives generalized for Rhapsody 1/1998 BMB
	
*/

#ifndef _GlobalDefs_
#define _GlobalDefs_

//This needs to get put into the build system...
#define __wxBuild__
#define wxMacMolPlt_VERSION "7.7"

//activate the following line to use the interactive editor
// please add CXXFLAGS=-DENABLE_INTERACTIVE_MODE to your configure step rather
// than changing this file!
#ifndef ENABLE_INTERACTIVE_MODE
#define ENABLE_INTERACTIVE_MODE
#endif
//Activate to allow quicktime movie export on Mac
//#define __MAC_USE_QUICKTIME__

#ifdef __wxBuild__
#include "mmp_gl.h"
//When building on a Mac the following types will get picked up from system headers
//prior to 2.9 WXOSX_CARBON wasn't defined, but is implied. It seems easier to define it than
//complicate the checks elsewhere.
#if !wxCHECK_VERSION(2, 9, 0)
#ifdef __WXMAC__
#define __WXOSX_CARBON__
#endif
#endif
#ifndef __WXOSX_CARBON__
struct RGBColor {
	unsigned short 					red;						/*magnitude of red component*/
	unsigned short 					green;						/*magnitude of green component*/
	unsigned short 					blue;						/*magnitude of blue component*/
};
#endif

#ifdef _MSC_VER
#define snprintf _snprintf
//VS apparently lacks these functions
int strcasecmp(const char * a, const char * b);
int strncasecmp(const char * a, const char * b, int l);
#endif

typedef char * Ptr;
typedef unsigned char Boolean;
typedef unsigned char Str255 [256];
#endif

// These are here because the ifdefs are broken right now.
#define MAX(x, y)		(((x)>(y)) ? (x) : (y))
#define MIN(x, y)		(((x)>(y)) ? (y) : (x))

//#define ChipmanCode	1	//Define to include the Chipman solvent surface code

void MessageAlert (const char * message);

//Useful constants
#define	kBohr2AngConversion	0.52917724924	/*Bohrs to Angstroms*/
#define kAng2BohrConversion	1.889725988		/*Angstroms to Bohrs*/
#define kDebyeToAU			0.39342724704	/*Debyes to AUs for charge dipoles*/
#define kRadToDegree		57.29577951		//Radians to Degrees conversion factor
#define kPi					3.141592654
#define	kHartreeTokCalPMol	627.5098		//Convert Hartree to kCal/mole
#define kCosErrorTolerance	1.0001			//Dihedral tolerance for converting
#define kZeroTolerance		1.0e-7			//to/from internal coords

typedef enum {
	MMP_NULL,
	MMP_ATOM,
	MMP_BOND,
	MMP_ANNOTATION,
	MMP_1DSURFACE
} MMP_object_t;

#endif
