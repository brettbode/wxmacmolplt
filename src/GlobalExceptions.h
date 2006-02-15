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

typedef bool Boolean;

class MemoryError {
	private:
		char *	ErrorMessage;
	public:
		MemoryError(void) {};
		MemoryError(const char *Message);
};
class DataError {
	public:
		long ErrNum;
		Boolean ErrorSet(void) {return ((ErrNum>0)?1:0);}
		void SetError(long num) {ErrNum = num;}
		DataError(void) {ErrNum = -1;};
		DataError(long num) {ErrNum = num;};
		void WriteError(void);
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
