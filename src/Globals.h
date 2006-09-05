/*
 *  (c) 2004 Iowa State University
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
#define wxMacMolPlt_VERSION "6.2"
/*
#define MacintoshBuild		//define to build for Mac PPC or 68K
#if __GNUC__
	//unix/os X build
#include <Carbon/Carbon.h>
#define Use_NIBs
#ifndef powerc
#define powerc
#endif

#define CarbonBuild
#define UseNavServices
//#ifdef PM_USE_SESSION_APIS
//#undef PM_USE_SESSION_APIS
//#endif
//#define PM_USE_SESSION_APIS 0	//Turn off printing session APIs for now.
#else
#define PM_OLDAPI
#endif
*/

#if TARGET_API_MAC_CARBON
#define CarbonBuild		//define to build PPC carbon app
#ifdef PM_USE_SESSION_APIS
#undef PM_USE_SESSION_APIS
#endif
#define PM_USE_SESSION_APIS 0	//Turn off printing session APIs for now.
#endif
//#define RhapsodyBuild		//define to build for Rhapsody (any platform)

#ifdef MacintoshBuild

#define UseMacIO		//Use FS routines instead of StdC calls
#define UseHandles		//Use Mac Handles instead of plain Ptrs

#ifdef powerc	//only enable QuickDraw3D related routines on PPC's
#define UseNavServices
#ifndef CarbonBuild	/*Standard File package not supported on carbon*/
#define QuickDraw3D
#define QuickDraw3DHeaders
#define UseStandardFilePackage
#endif
#endif

#ifdef CarbonBuild	/*Carbon requires new menu routines only available in 8.5 and higher*/

	//enable the following for a Quesa build
//#define QuickDraw3D
//#define Quesa
//#define QuesaHeaders

	//If we are not using QuickDraw3D use OpenGL (default on Carbon)
#ifndef QuickDraw3D
#define UseOpenGL
#endif

#define myDisableMenuItem( menu, item ) DisableMenuItem( menu, item )
#define myEnableMenuItem( menu, item ) EnableMenuItem( menu, item )
inline void SetCursorToArrow(void) {
	Cursor	arrow;	
	SetCursor(GetQDGlobalsArrow(&arrow));
}
inline void myValidWindowRect(WindowRef theWindow, const Rect * theRect) {
	ValidWindowRect(theWindow, theRect);
}
inline void myInValidWindowRect(WindowRef theWindow, const Rect * theRect) {
	InvalWindowRect(theWindow, theRect);
}
inline void SpinCursor(int ) {}
#else /*non-Carbon function definitions*/
#include <CursorCtl.h>  //Only needed for pre-carbon builds
#define myDisableMenuItem( menu, item ) DisableItem( menu, item )
#define myEnableMenuItem( menu, item ) EnableItem( menu, item )
inline void SetCursorToArrow(void) {
	SetCursor (&qd.arrow);
}
inline void myValidWindowRect(WindowPtr , const Rect * theRect) {
	ValidRect(theRect);
}
inline void myInValidWindowRect(WindowPtr , const Rect * theRect) {
	InvalRect(theRect);
}
#ifndef powerc
inline DialogPtr GetDialogFromWindow(WindowPtr a) { return a;}
inline void SetPortDialogPort(DialogPtr theDialog) {SetPort(theDialog);}
#endif
inline Pattern * GetQDGlobalsBlack(Pattern * black) {*black = qd.black; return black;}
inline WindowPtr GetControlOwner(ControlRef thecontrol) {return ((**thecontrol).contrlOwner);}
inline Rect * GetControlBounds(ControlRef theControl, Rect * bounds) {*bounds = (**theControl).contrlRect; return bounds;}
inline PixMapHandle GetPortPixMap(CGrafPtr thePort) {return thePort->portPixMap;}
#endif

//#define DEBUG_IT	// Uncomment this flag to enable debug code
#ifdef DEBUG_IT
#define FailMessage(cond, s)								\
	if (cond) {												\
		DebugStr ((ConstStr255Param)"\p"#s);				\
	}														
#else
#define FailMessage(cond, s)								\
	((void)	0)
#endif// DEBUG_IT

#ifdef DEBUG_IT
#define DebugMessage(s)			DebugStr ((ConstStr255Param)"\p"#s)
#else
#define DebugMessage(s)			((void)	0)
#endif// DEBUG_IT
#pragma options align=power		//This shouldn't be needed, but CW9 seemed to need it for 68K

/* Define HiWrd and LoWrd macros for efficiency. */
#define HiWrd(aLong)	(((aLong) >> 16) & 0xFFFF)
#define LoWrd(aLong)	((aLong) & 0xFFFF)
//#ifndef fabs
//#define fabs(aaFloat)	(((aaFloat)>=0.0) ? (aaFloat) : -(aaFloat))
//#endif
#define max(x, y)		(((x)>(y)) ? (x) : (y))
#define MAX(x, y)		(((x)>(y)) ? (x) : (y))
#define min(x, y)		(((x)>(y)) ? (y) : (x))
#define MIN(x, y)		(((x)>(y)) ? (y) : (x))
#define fsign(x)		(((x)<0.0) ? -1.0 : 1.0)

//#define kMaxOpenDocuments	20			//Max possible open documents
#define kMinDocDim			150			//Min size (height and width) of the main display
#define kBarHeight			15			//Scroll bar height (or width)
#define	rDitherPatterns		0			//Dither patterns rsrc #
#define kIStringID			128
#define kerrstrings			129			//Indexed strings for error messages
#define kMenuStringsID		130			//indexed strings for menu items
#define kConfirmStrings		131			//ind. strings for confirmation dialogs
#define rMessageAlert		130
#define kConfirmationAlert	131
#define kUncaughtAlert		132
#define kYesOrNoAlert		133
#define kDataErrMsgs		134
#define kSaveWarningAlert	134
#define kGenLDEFID			200			//General use LDEF resource ID
/* Structure used for dithering patterns */
	typedef struct PatList {
		short	patListSize;	/* Size */
		Pattern	patList[1];		/* Pointer to varible size arrary */
	} **PatListHandle;

//Special global routines
void MessageAlert (Str255 message);
bool ConfirmationDialog(short ResID, short itemNum);
void MessageAlertAndQuit (Str255 message);
bool YesOrNoDialog(short ResID, short itemNum);
void MessageAlertByID(const short ResID, const short itemNum);
void ShowAboutMeDialog(void);
Boolean FrontApp(void);

#endif	//End of Mac definitions!

#ifdef RhapsodyBuild

#define BlockMoveData(source, target, nBytes)	memcpy(target, source, nBytes)

struct RGBColor {
	unsigned short 					red;						/*magnitude of red component*/
	unsigned short 					green;						/*magnitude of green component*/
	unsigned short 					blue;						/*magnitude of blue component*/
};
typedef struct RGBColor RGBColor;

#endif	//End of Rhapsody definitions!

#ifdef __wxBuild__
//When building on a Mac the following types will get picked up from system headers
#ifdef __WXMAC__
#include <wx/glcanvas.h>
#else
struct RGBColor {
	unsigned short 					red;						/*magnitude of red component*/
	unsigned short 					green;						/*magnitude of green component*/
	unsigned short 					blue;						/*magnitude of blue component*/
};
struct Point { short h; short v; };
#endif

#ifdef WIN32
#define snprintf _snprintf
#define strcasecmp strcmp
#endif

#define UseOpenGL
typedef char * Ptr;
typedef unsigned char Boolean;
typedef unsigned char Str255 [256];
#endif

// These are here because the ifdefs are broken right now.
#define max(x, y)		(((x)>(y)) ? (x) : (y))
#define MAX(x, y)		(((x)>(y)) ? (x) : (y))
#define min(x, y)		(((x)>(y)) ? (y) : (x))
#define MIN(x, y)		(((x)>(y)) ? (y) : (x))

//#define ChipmanCode	1	//Define to include the Chipman solvent surface code

void MessageAlert (const char * message);

#define kMaxOpenDocuments	20			//Max possible open documents
//make kMaxOpenDocuments a global definition

//Useful constants
#define	kBohr2AngConversion	0.52917724924	/*Bohrs to Angstroms*/
#define kAng2BohrConversion	1.889725988		/*Angstroms to Bohrs*/
#define kDebyeToAU			0.39342724704	/*Debyes to AUs for charge dipoles*/
#define kRadToDegree		57.29577951		//Radians to Degrees conversion factor
#define kPi					3.141592654
#define	kHartreeTokCalPMol	627.5098		//Convert Hartree to kCal/mole
#define kCosErrorTolerance	1.0001			//Dihedral tolerance for converting
#define kZeroTolerance		1.0e-7			//to/from internal coords

#endif
