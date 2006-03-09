/*
 * (c) 2005 Iowa State University
 *     see the LICENSE file in the top level directory
 */

/***************************************
 * Progress.cpp
 *
 * Created:       10-31-2006  Steven Schulteis
 * Last Modified: 02-15-2006  Steven Schulteis
***************************************/

// This file currently provides no-op implementations of the functions defined
// in Progress.h.  This is just to get things to compile for now.
// TODO:  Write actual implementations for these functions.

#include "Progress.h"

Progress::Progress(void) {
}

Progress::~Progress(void) {
}

bool Progress::UpdateProgress(float PercentDone) {
    return true;
}

void Progress::ChangeText(char *newText) {
}

void Progress::SetBaseValue(long NewVal) {
}

void Progress::SetScaleFactor(float NewVal) {
}

void Progress::SetSleepTime(long NewTime) {
}

void Progress::SetRunTime(long NewTime) {
}

void Progress::ResetTimes(void) {
}

