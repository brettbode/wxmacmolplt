/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	GlobalExceptions.h

	Declarations of the classes which might be 'thrown' and caught within
	MacMolPlt.
	
	Brett Bode 4/1998
	BMB 1/2001 Added UserCancel class to handle user aborts of long operations
*/

//Global exceptions types

#ifndef __GLOBAL_EXCEPTIONS_H__
#define __GLOBAL_EXCEPTIONS_H__

//The following are for non-Mac systems
#if defined(__wxBuild__) && !defined(__WXOSX_CARBON__)
typedef long OSErr;
const int noErr = 0;
const int eofErr = -39;
#endif

class MemoryError {
	private:
		char *	ErrorMessage;
	public:
		MemoryError(void) {};
		MemoryError(const char *Message);
};
class DataError {
	public:
		DataError(void) {};
};
class FileError {
	public:
		OSErr	Error;
		FileError(void) {
			Error = noErr;
		};
		FileError(OSErr	err) {
			Error = err;
		};
		void WriteError(void);
};
class UserCancel {
	public:
		UserCancel(void) {};
};

#endif

