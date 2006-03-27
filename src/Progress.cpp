/*
 * (c) 2006 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * Progress.cpp
 *
 * Created:       10-31-2006  Steven Schulteis
 * Last Modified: 3-26-2006	  Brett Bode
***************************************/

//These provide the basics of what is needed. Cancel hasn't been tested!

#include "Globals.h"
#include "Progress.h"

Progress::Progress(void) {
	BaseValue = 0;
	ResetTimes();
	NextTime = RunTime;
	ScaleFactor = 1.0;
	SavedPercentDone = 0;
	progDlg = NULL;
}

Progress::~Progress(void) {
	if (progDlg) {
		progDlg->Destroy();
	}
}

bool Progress::UpdateProgress(float Percent) {
	bool result = true;
	long PercentDone = BaseValue + (long)(Percent * ScaleFactor);
	long ctime = timer.Time();
		//This check is probably not really needed, but it keeps us from calling
		//the update function (which call Yield) too frequently.
	if (progDlg && (ctime > NextTime)) {
		result = progDlg->Update(PercentDone, tempLabel);
		NextTime = ctime + SleepTime;
	} else if (RunTime<ctime) {
		progDlg = new wxProgressDialog(wxT("progress"), tempLabel, 100, NULL, 
									   wxPD_APP_MODAL | wxPD_CAN_ABORT);
		result = progDlg->Update(PercentDone, tempLabel);
		NextTime = ctime + SleepTime;
	}
	SavedPercentDone = PercentDone;
    return result;
}
void Progress::ChangeText(const char *newText) {
	wxString str(newText, wxConvUTF8);
	tempLabel = str;
}

void Progress::ResetTimes(void) {
	RunTime = 200;
	SleepTime = 10;
}

