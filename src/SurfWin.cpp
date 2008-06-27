/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	SurfClasses.cpp
	Classes related to the Surfaces window and its daughter panes.
	
	Split off of mywindows.cpp	9-97	BMB
	Changed Gen 2D and 3D surface panes to simply fire the color picker to the center
	of the main monitor. This avoids it being chopped off the edge of the screen. Already
	done for all other surface types.	3/1999	BMB
	Added seperate flag to hold the MO Energy versus Orb Occupation. 8/2000 BMB
	Added check on the active edit field in TED3D so the update button activates better. 8/2000 BMB
	Modified UPP calls for Carbon. 3/2001 BMB
	Use PaintRect instead of EraseRect to make List backgrounds white again. 6/2001 BMB
	Added SetDialogDefaultItem call. 11/2001 BMB
	Added ability to specify an arbitrary multiplier in general surfaces. 4/2002 BMB
	Stripped out all the Mac stuff for the wxBuild. 3/2006 BMB
*/
#include "Globals.h"
#include "GlobalExceptions.h"
#include "Progress.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "BasisSet.h"
#include "SurfaceTypes.h"
#include <string.h>

void Orb3DSurface::Update(MoleculeData * MainData) {
	if (Visible) {
			Progress * lProgress = new Progress();
		if (lProgress) {
			lProgress->ChangeText("Calculating 3D Grid...");
			lProgress->SetScaleFactor(0.9);
			CalculateMOGrid(MainData, lProgress);
			lProgress->ChangeText("Contouring 3D Grid...");
			lProgress->SetBaseValue(90);
			lProgress->SetScaleFactor(0.1);
			Contour3DGrid(lProgress);
			delete lProgress;
		}
	}
}
void TEDensity3DSurface::Update(MoleculeData * MainData) {
	if (Visible) {
			Progress * lProgress = new Progress();
		if (lProgress) {
				float MEPScale = 1.0;
			if (ColorByValue()) MEPScale = 0.5;
			lProgress->ChangeText("Calculating 3D Grid...");
			lProgress->SetScaleFactor(0.9*MEPScale);
			CalculateMOGrid(MainData, lProgress);
			lProgress->ChangeText("Contouring 3D Grid...");
			lProgress->SetBaseValue((long)(90*MEPScale));
			lProgress->SetScaleFactor(0.1*MEPScale);
			Contour3DGrid(lProgress);
			if (ColorByValue() && MainData->MEPCalculationPossible()) {
				lProgress->ChangeText("Calculating MEP values...");
				lProgress->SetBaseValue(50);
				lProgress->SetScaleFactor(0.5);
				CalculateSurfaceValues(MainData, lProgress);
			}
			delete lProgress;
		}
	}
}
void MEP2DSurface::Update(MoleculeData * MainData) {
	if (Visible) {
		if (GetRotate2DMap()) SetPlaneToScreenPlane(MainData);
			Progress * lProgress = new Progress();
		if (lProgress) {
			lProgress->ChangeText("Calculating 2D Grid...");
			CalculateMOGrid(MainData, lProgress);
			delete lProgress;
		}
	}
}
void MEP3DSurface::Update(MoleculeData * MainData) {
	if (Visible) {
			Progress * lProgress = new Progress();
		if (lProgress) {
			lProgress->ChangeText("Calculating 3D Grid...");
			lProgress->SetScaleFactor(0.95);
			CalculateMEPGrid(MainData, lProgress);
			lProgress->ChangeText("Contouring 3D Grid...");
			lProgress->SetBaseValue(95);
			lProgress->SetScaleFactor(0.05);
			Contour3DGrid(lProgress);
			delete lProgress;
		}
	}
}

