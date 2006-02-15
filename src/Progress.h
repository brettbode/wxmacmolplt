/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	Progress.h

	Simple platform independant interface to a progress indicator
	This is really just a pass through interface which will call
	routines native to each platform as appropriate

	Created 4/1998 BMB
*/

#ifndef __Progress__
#define __Progress__

class Progress {
	private:
		char *	localDialog;	//placeholder for the platform dependant data
	public:
		Progress(void);
		~Progress(void);
		Boolean UpdateProgress(float PercentDone);
		void ChangeText(char * newText);
		void SetBaseValue(long NewVal);
		void SetScaleFactor(float NewVal);
		void SetSleepTime(long NewTime);
		void SetRunTime(long NewTime);
		void ResetTimes(void);
};

#endif
