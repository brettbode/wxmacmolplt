/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

#ifndef __Progress__
#define __Progress__
#include <wx/stopwatch.h>
#include <wx/progdlg.h>

/// The default initial delay (in ms) between object create and the display of the dialog
#define kInitialProgressDisplayDelay	200
/// The default minimum time (in ms) between actual dialog update calls
#define kProgessDialogSleepTime	10
/**
 * The Progress class implements a light weight progress indicator. In practice
 * it is a thin wrapper on top of the wxProgress class. The main difference is that
 * it does not immediately display (or even create) the wx dialog. That is because
 * the operations being tracked vary widely in their duration so this wrapper
 * delays the dialog display until two seconds pass and then only updates the dialog
 * at a reasonable interval.
 * The dialog is only updated when UpdateProgress is called. Most uses will simply 
 * create the class object, use ChangeText occasionaly and UpdateProgress in an
 * outer loop.
 */
class Progress {
private:
	long			NextTime;			///< The next time the dialog will be updated.
	long			BaseValue;			///< The offset for the input percentage.
	long			RunTime;			///< The initial dialog display delay time (ms)
	long			SleepTime;			///< The delay between calls to update the dialog.
	float			ScaleFactor;		///< Scalefactor for the input percentage.
	wxStopWatch		timer;				///< Internal timer to track the delays
	wxString		tempLabel;			///< The text in the dialog will be set to this the next time it is updated.
	wxProgressDialog * progDlg;			///< The actual wx dialog.
public:
	/// Setup the class, the dialog is not actually created until the initial delay time passes and UpdateProgress is called.
	Progress(void);
	/// The destructor removes the progress dialog, if needed.
	~Progress(void);
	/**
	 * UpdateProgress adjusts the progress indicator to the specified value,
	 * updates the text label and checks to see if the user has canceled the operation.
	 * The wxDialog is not created until after an initial delay and the dialog is
	 * not updated more frequently than sleepTime. The effective percentage can be altered
	 * through the SetBaseValue and SetScaleFactor functions.
	 * @param PercentDone A value between 0 and 100 to use for the progress indicator
	 */
	bool UpdateProgress(float PercentDone);
	/**
	 * ChangeText alters the text displayed in the progress dialog. Note the change
	 * to the dialog does not actually occur until the next call to UpdateProgress.
	 * @param newText The desired new message for the dialog
	 */
	void ChangeText(const char * newText);
	/**
	 * Offsets the percent by the base value. Should be used with SetScaleFactor.
	 * @param NewVal the new base offset for the total percentage.
	 */
	inline void SetBaseValue(long NewVal) {BaseValue = NewVal;};
	/**
	 * Setup a scale factor to multiple future inputs to UpdateProgress.
	 * The displayed percent is then BaseValue + ScaleFactor*inputPercent
	 * @param NewVal the new scale factor for the input percentage.
	 */
	inline void SetScaleFactor(float NewVal) {ScaleFactor = NewVal;};
	/**
	 * Set the minimum time between actual dialog updates. The value helps
	 * prevent spamming the dialog with updates and slowing everything down.
	 * @param NewTime the new delay in ms
	 */
	inline void SetSleepTime(long NewTime) {SleepTime = NewTime;};
	/**
	 * Set the minimum time after class creation to actually create and display the
	 * progress dialog. This avoids flashing up a progress dialog for a short time.
	 * Once the dialog has been displayed this setting has no effect.
	 * @param NewTime the new delay in ms
	 */
	inline void SetRunTime(long NewTime) {RunTime = NewTime;};
	/// Reset the initial and sleep delays to their default values.
	void ResetTimes(void);
};

#endif
