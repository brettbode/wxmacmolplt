/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	Progress.h

	This implements a platform independant progress dialog interface.
	It is modeled on the original Mac version created 4/1998 BMB. The 
    wxWidgets version was created 3/2006 by BMB.
*/

#ifndef __Progress__
#define __Progress__
#include <wx/stopwatch.h>
#include <wx/progdlg.h>

class Progress {
private:
	long			NextTime;
	long			BaseValue;
	long			RunTime;
	long			SleepTime;
	long			SavedPercentDone;
	float			ScaleFactor;
	wxStopWatch		timer;
	wxString		tempLabel;
	wxProgressDialog * progDlg;
public:
		Progress(void);
		~Progress(void);
		void InitWindow(void);
		bool UpdateProgress(float PercentDone);
		void ChangeText(const char * newText);
		inline void SetBaseValue(long NewVal) {BaseValue = NewVal;};
		inline void SetScaleFactor(float NewVal) {ScaleFactor = NewVal;};
		inline void SetSleepTime(long NewTime) {SleepTime = NewTime;};
		inline void SetRunTime(long NewTime) {RunTime = NewTime;};
		void ResetTimes(void);
};

#endif
