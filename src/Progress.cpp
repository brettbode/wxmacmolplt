/*
 * (c) 2006 Iowa State University
 *     see the LICENSE file in the top level directory
 */

#include "Globals.h"
#include "Progress.h"

Progress::Progress(void) {
	BaseValue = 0;
	ResetTimes();
	NextTime = RunTime;
	ScaleFactor = 1.0;
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
	if (progDlg) {
		//This check is probably not really needed, but it keeps us from calling
		//the update function (which calls Yield) too frequently.
		if (ctime > NextTime) {
			result = progDlg->Update(PercentDone, tempLabel);
			NextTime = ctime + SleepTime;
		}
	} else if (RunTime<ctime) {
		progDlg = new wxProgressDialog(wxT("progress"), tempLabel, 100, NULL, 
									   wxPD_APP_MODAL | wxPD_CAN_ABORT |
									   wxPD_AUTO_HIDE);
		result = progDlg->Update(PercentDone, tempLabel);
		NextTime = ctime + SleepTime;
	}
    return result;
}
void Progress::ChangeText(const char *newText) {
	wxString str(newText, wxConvUTF8);
	tempLabel = str;
}

void Progress::ResetTimes(void) {
	RunTime = kInitialProgressDisplayDelay;
	SleepTime = kProgessDialogSleepTime;
}

