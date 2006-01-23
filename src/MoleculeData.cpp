/***************************************
 * MoleculeData.cpp
 *
 * Created:       04-??-1998  BMB (Created from other files)
 * Last Modified: 01-23-2006  Steven Schulteis
 ***************************************/

/*  This file was created from a copy of MoleculeData.cpp from the old
 *  MacMolPlt source.
 *
 *  License info from original file:
 *
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

#include "Globals.h"
#include "GlobalExceptions.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "Internals.h"
#include "BasisSet.h"
#include "Math3D.h"
#include "InputData.h"
#include "VirtualSphere.h"
#include "Progress.h"
#include "Prefs.h"
#include <string.h>
#include <new>

MoleculeData::MoleculeData(void) {
	RotCoords = NULL;
	zBuffer = NULL;
	cFrame = Frames = new Frame;
	CurrentFrame=1;
	NumFrames = 1;
	IntCoords = NULL;
	Basis = NULL;
	Description = NULL;
	MaxAtoms = 0;
	MaxSize=0.5;
	WindowSize=1.0;
	InitRotationMatrix(TotalRotation);
	InputOptions = NULL;
	DrawMode = 0;
	DrawLabels = 0;
}
MoleculeData::~MoleculeData(void) {
	while (Frames) {
		cFrame = Frames->GetNextFrame();
		delete Frames;
		Frames = cFrame;
	}
	if (RotCoords) delete [] RotCoords;
	if (zBuffer) delete [] zBuffer;
	if (Description) delete [] Description;
	if (IntCoords) delete IntCoords;
	if (Basis) delete Basis;
	if (InputOptions) delete InputOptions;
}
Boolean MoleculeData::SetupFrameMemory(long NumAtoms, long NumBonds) {
	if (NumAtoms < cFrame->AtomAllocation) NumAtoms = cFrame->AtomAllocation;
	if (NumBonds < cFrame->BondAllocation) NumBonds = cFrame->BondAllocation;
	if ((NumAtoms == cFrame->AtomAllocation)&&
		(NumBonds == cFrame->BondAllocation)) return true;//no need to resize
	Atom * tempAtoms=NULL;
	Bond * tempBonds=NULL;
	try {
		tempAtoms = new Atom[NumAtoms];
		tempBonds = new Bond[NumBonds];
	}
	catch (std::bad_alloc) {
		if (tempAtoms) delete [] tempAtoms;
		if (tempBonds) delete [] tempBonds;
		return false;
	}
//	if (!tempAtoms || !tempBonds) {
//		if (tempAtoms) delete [] tempAtoms;
//		if (tempBonds) delete [] tempBonds;
//		return false;
//	}
	BlockMoveData(cFrame->Atoms, tempAtoms, cFrame->NumAtoms*sizeof(Atom));
	BlockMoveData(cFrame->Bonds, tempBonds, cFrame->NumBonds*sizeof(Bond));
	if (cFrame->Atoms) delete [] cFrame->Atoms;	//delete old arrays, if any
	if (cFrame->Bonds) delete [] cFrame->Bonds;
	cFrame->Atoms = tempAtoms;
	cFrame->Bonds = tempBonds;
	cFrame->AtomAllocation = NumAtoms;
	cFrame->BondAllocation = NumBonds;
	if (NumAtoms > MaxAtoms) {	//Need to extend the size of the zbuffer arrays
		if (RotCoords) delete [] RotCoords;
		if (zBuffer) delete [] zBuffer;
		try {
			RotCoords = new CPoint3D[NumAtoms];
			zBuffer = new long[NumAtoms];
		}
		catch (std::bad_alloc) {
			if (RotCoords) delete [] RotCoords;
			if (zBuffer) delete [] zBuffer;	//Attempt to restore the zBuffer
			RotCoords = new CPoint3D[MaxAtoms];
			zBuffer = new long[MaxAtoms];
			return false;
		}
//		if (!RotCoords || !zBuffer) {
//			if (RotCoords) delete [] RotCoords;
//			if (zBuffer) delete [] zBuffer;	//Attempt to restore the zBuffer
//			RotCoords = new CPoint3D[MaxAtoms];
//			zBuffer = new long[MaxAtoms];
//			return false;
//		}
		MaxAtoms = NumAtoms;
	}
	return true;
}
/*Call to position a frame in the frame list according to the xcoord (time or IRC value)
  routine returns a new empty frame ptr in the frame list*/
Frame * MoleculeData::LocateNewFrame(float XPosition)
{	Frame * lNewFrame;

	Frame * lFrame = cFrame;
	if (XPosition < lFrame->time) {
		while (XPosition < lFrame->time) {
			if (lFrame->PreviousFrame)
				lFrame = lFrame->PreviousFrame;
			else
				break;
		}
		if (lFrame->time < XPosition) lFrame = lFrame->NextFrame;
		if ((lFrame->time-XPosition) < 1.0E-6) return NULL; /*duplicate point*/
		lNewFrame = new Frame;
		if (lNewFrame == NULL) throw MemoryError();
		lNewFrame->PreviousFrame = lFrame->PreviousFrame;
		lFrame->PreviousFrame = lNewFrame;
		lNewFrame->NextFrame = lFrame;
		if (lNewFrame->PreviousFrame) lNewFrame->PreviousFrame->NextFrame = lNewFrame;
	} else {
		while (XPosition > lFrame->time) {
			if (lFrame->NextFrame)
				lFrame = lFrame->NextFrame;
			else
				break;
		}
		if (lFrame->time > XPosition) lFrame = lFrame->PreviousFrame;
		if ((XPosition-lFrame->time) < 1.0E-6) return NULL; /*duplicate point*/
		lNewFrame = new Frame;
		if (lNewFrame == NULL) throw MemoryError();
		lNewFrame->NextFrame = lFrame->NextFrame;
		lFrame->NextFrame = lNewFrame;
		lNewFrame->PreviousFrame = lFrame;
		if (lNewFrame->NextFrame) lNewFrame->NextFrame->PreviousFrame = lNewFrame;
	}
	if (lNewFrame->PreviousFrame == NULL) Frames = lNewFrame;
	cFrame = lNewFrame;
	lNewFrame->time = XPosition;
	lFrame = Frames;
	for (CurrentFrame=1; lFrame!=lNewFrame; CurrentFrame++)
		lFrame = lFrame->NextFrame;
	NumFrames ++;
	return lNewFrame;
} /*LocateNewFrame*/
Frame * MoleculeData::AddFrame(long NumAtoms, long NumBonds) {
	Frame * temp = new Frame;
	if (!temp) throw MemoryError();
	if (cFrame->NextFrame) {
		cFrame->NextFrame->PreviousFrame = temp;
		temp->NextFrame = cFrame->NextFrame;
	}
	temp->PreviousFrame = cFrame;
	cFrame->NextFrame = temp;
	cFrame = temp;
	
	if (!SetupFrameMemory(NumAtoms, NumBonds)) throw MemoryError();
	NumFrames++;
	CurrentFrame++;
	return temp;
}
void MoleculeData::DeleteFrame(void) {
	if (!cFrame->NextFrame && !cFrame->PreviousFrame) return;
	Frame * temp = cFrame;
	if (cFrame->NextFrame) {
		cFrame->NextFrame->PreviousFrame = cFrame->PreviousFrame;
		if (cFrame->PreviousFrame)
			cFrame->PreviousFrame->NextFrame = cFrame->NextFrame;
		cFrame = cFrame->NextFrame;
	} else {
		cFrame->PreviousFrame->NextFrame=NULL;
		cFrame = cFrame->PreviousFrame;
		CurrentFrame--;
	}
	if (temp == Frames) Frames = temp->NextFrame;
	delete temp;
	NumFrames--;
}
void MoleculeData::GetRotationMatrix(Matrix4D copy) {
	CopyMatrix(TotalRotation, copy);
}

//routine calculates the cartesian center of the molecule and offsets the 
//coordinate system of the window to match the moleculer center.
void MoleculeData::CenterModelWindow(void) {
	float	XMin, XMax, YMin, YMax, ZMin, ZMax;
//first set the Min and Max to big and small values
	XMin=YMin=ZMin=1.0E10;
	XMax=YMax=ZMax=-1.0E10;

//Start at the beginning of the frame list so that the center for the entire
//frame list is generated.	
	Frame *	lFrame = Frames;
//Now find the min and max in each direction	
	while (lFrame) {
		for (long iatom=0; iatom<lFrame->NumAtoms; iatom++) {
			XMin = MIN(XMin, lFrame->Atoms[iatom].Position.x);
			XMax = MAX(XMax, lFrame->Atoms[iatom].Position.x);
			YMin = MIN(YMin, lFrame->Atoms[iatom].Position.y);
			YMax = MAX(YMax, lFrame->Atoms[iatom].Position.y);
			ZMin = MIN(ZMin, lFrame->Atoms[iatom].Position.z);
			ZMax = MAX(ZMax, lFrame->Atoms[iatom].Position.z);
		}
		lFrame = lFrame->GetNextFrame();
	}
//The center is now just half of the min plus the max
	CPoint3D Center, NewCenter;
	Center.x = (XMax+XMin)*0.5;
	Center.y = (YMax+YMin)*0.5;
	Center.z = (ZMax+ZMin)*0.5;
//Recompute the maximum width of the molecule
	MaxSize = MAX((XMax-XMin), (YMax-YMin));
	MaxSize = MAX(MaxSize, (ZMax-ZMin));
	
	TotalRotation[3][0] =
	TotalRotation[3][1] =
	TotalRotation[3][2] = 0.0;

	Rotate3DPt(TotalRotation, Center, &NewCenter);
//Now offset the screen coordinate system in the opposite direction
//as the center. This has the effect of lining up the screen coordinate
//center with the molecular center.
	TotalRotation[3][0] = -NewCenter.x;
	TotalRotation[3][1] = -NewCenter.y;
	TotalRotation[3][2] = -NewCenter.z;
}	/*CenterModelWindow*/
void MoleculeData::ResetRotation(void) {
	if (cFrame->NumAtoms > MaxAtoms) {
		if (RotCoords != NULL) {
			delete [] RotCoords;
			RotCoords = NULL;
		}
		RotCoords = new CPoint3D[cFrame->NumAtoms];
		if (zBuffer != NULL) {
			delete [] zBuffer;
			zBuffer = NULL;
		}
		zBuffer = new long[cFrame->NumAtoms];
		MaxAtoms = cFrame->NumAtoms;
	}
	for (long i=0; i<cFrame->NumAtoms; i++) {
		Rotate3DPt(TotalRotation, (cFrame->Atoms[i].Position), &(RotCoords[i]));
		zBuffer[i] = i;
	}
		//Now sort the Z Buffer
	SortzBuffer(RotCoords, zBuffer, cFrame->NumAtoms);
}
// Stick the coordinates by moving the rotated coordinates to the file coordinates
// being careful to invalidate any orbital info
void MoleculeData::StickCoordinates(void) {
	if (cFrame->Orbs.size() > 0) {	// rotate any MO vectors and update the surface list
		std::vector<OrbitalRec *>::const_iterator OrbSet = cFrame->Orbs.begin();
		while (OrbSet != cFrame->Orbs.end()) {
			(*OrbSet)->RotateVectors(TotalRotation, Basis, cFrame->NumAtoms);
			OrbSet++;
		}
			Surface * lSurface = cFrame->SurfaceList;
		while (lSurface) {
			lSurface->RotateSurface(TotalRotation);
			lSurface = lSurface->GetNextSurface();
		}
	}
		// rotate any frequencies
	if (cFrame->Vibs) {
		CPoint3D *	Modes = cFrame->Vibs->NormMode, NMode;
		for (long imode=0; imode < cFrame->Vibs->NumModes; imode++) {
			long cmode = imode*cFrame->NumAtoms;
			for (long iatom=0; iatom<cFrame->NumAtoms; iatom++) {
				Rotate3DOffset(TotalRotation, Modes[iatom+cmode], &NMode);
				Modes[iatom+cmode] = NMode;
			}
		}
	}
		long i;
	for (i=0; i<cFrame->NumAtoms; i++)
		Rotate3DPt(TotalRotation, cFrame->Atoms[i].Position, &(RotCoords[i]));
	for (i=0; i<cFrame->NumAtoms; i++) {
		cFrame->Atoms[i].Position = RotCoords[i];
	}
	InitRotationMatrix(TotalRotation);
}
void MoleculeData::NewAtom(void) {
	if (cFrame->NumAtoms>=cFrame->AtomAllocation) {
		Atom * temp = new Atom[cFrame->NumAtoms+10];
		if (temp) {
			BlockMoveData(cFrame->Atoms, temp, cFrame->NumAtoms*sizeof(Atom));
			if (cFrame->Atoms) delete [] cFrame->Atoms;
			cFrame->Atoms = temp;
			cFrame->AtomAllocation += 10;
		}
	}
	if (cFrame->NumAtoms<cFrame->AtomAllocation) {
		cFrame->Atoms[cFrame->NumAtoms].Type = 1;
		cFrame->Atoms[cFrame->NumAtoms].Position.x = 0;
		cFrame->Atoms[cFrame->NumAtoms].Position.y = 0;
		cFrame->Atoms[cFrame->NumAtoms].Position.z = 0;
		cFrame->Atoms[cFrame->NumAtoms].Highlite = 0;
		cFrame->NumAtoms++;
			//Delete any orbitals and normal modes
		if (cFrame->Vibs) {
			delete cFrame->Vibs;
			cFrame->Vibs = NULL;
		}
		cFrame->DeleteOrbitals();
		while (cFrame->SurfaceList) cFrame->DeleteSurface(0);
	}
	if (cFrame->AtomAllocation>MaxAtoms) {
		if (RotCoords) delete [] RotCoords;
		if (zBuffer) delete [] zBuffer;
		RotCoords = new CPoint3D[cFrame->AtomAllocation];
		zBuffer = new long[cFrame->AtomAllocation];
		if (!RotCoords || !zBuffer) throw MemoryError();
		MaxAtoms = cFrame->AtomAllocation;
	}
	ResetRotation();
}
void MoleculeData::SetDescription(char * NewLabel) {
	if (Description) delete [] Description;
	long LineLength = strlen(NewLabel);
	Description = new char[LineLength+1];
	if (Description) strcpy(Description, NewLabel);
}
void MoleculeData::InvertMode(void) {
	if (!cFrame->Vibs) return;
	
	CPoint3D * Freq = &(cFrame->Vibs->NormMode[cFrame->NumAtoms*cFrame->Vibs->CurrentMode]);
	for (long iatm=0; iatm<cFrame->NumAtoms; iatm++) {
		Freq[iatm] *= -1.0;
	}
}	/*InvertMode*/
void MoleculeData::UnitConversion(Boolean AtoB) {
	float		factor;
//Choose the correct factor
	if (AtoB) factor = kBohr2AngConversion;
	else factor = kAng2BohrConversion;
	
	Frame * lFrame = Frames;
//Now scale the permenant set of coordinates (for all frames)
	while (lFrame) {
		for (long iatm=0; iatm < lFrame->NumAtoms; iatm++) {
			lFrame->Atoms[iatm].Position *= factor;
		}
		lFrame = lFrame->NextFrame;
	}
	TotalRotation[3][0] *= factor;	//scale the origin offset so things stay centered
	TotalRotation[3][1] *= factor;
	TotalRotation[3][2] *= factor;
//Finally update the window scaling sizes so things are still drawn the same
	MaxSize *= factor;
	WindowSize *= factor;
} /*UnitConversion*/
//Rotate the molecule by 180 degrees about either the horizontal or vertical axis
//Pass in 0 to flip about the horizontal axis, 1 for vertical
void MoleculeData::FlipRotation(short theItem) {
	Point			p={0,0}, q={10,10}, sphereCenter={10,10};
	long			sphereRadius=10;
	Matrix4D		rotationMatrix, tempcopyMatrix;

//180 degree rotation is desired, but if the two vectors are colinear then the
//rotation will not have a well defined axis so use two 90 degree rotations instead
	if (theItem == 0) {	//flipHitem
		p.v = 10;
	} else {	//theItem == 1 (flipVitem)
		p.h = 10;
	}
	VirtualSphereQD3D (p, q, sphereCenter, sphereRadius, rotationMatrix, TotalRotation);

	CPoint3D	InitialTrans, FinalTrans;
//First back rotate the translation to get the inital translation
	FinalTrans.x = TotalRotation[3][0];
	FinalTrans.y = TotalRotation[3][1];
	FinalTrans.z = TotalRotation[3][2];
	BackRotate3DOffset(TotalRotation, &FinalTrans, &InitialTrans);
/*	InitialTrans.x = ((TotalRotation[3][0])*TotalRotation[0][0] +
		( TotalRotation[3][1])*TotalRotation[0][1] +
		( TotalRotation[3][2])*TotalRotation[0][2]);
	InitialTrans.y = ((TotalRotation[3][0])*TotalRotation[1][0] +
		( TotalRotation[3][1])*TotalRotation[1][1] +
		( TotalRotation[3][2])*TotalRotation[1][2]);
	InitialTrans.z = ((TotalRotation[3][0])*TotalRotation[2][0] +
		( TotalRotation[3][1])*TotalRotation[2][1] +
		( TotalRotation[3][2])*TotalRotation[2][2]);*/
//Now zero out the translation part of the matrix
	TotalRotation[3][0] = TotalRotation[3][1] =
	TotalRotation[3][2] = 0.0;
/* Concatenate the new rotation with the current rotation */
//Mulitply twice since the rotate generated is 90 degrees and we want 180
	MultiplyMatrix (rotationMatrix, TotalRotation, tempcopyMatrix);
	CopyMatrix (tempcopyMatrix, TotalRotation);
	MultiplyMatrix (rotationMatrix, TotalRotation, tempcopyMatrix);
	CopyMatrix (tempcopyMatrix, TotalRotation);
//Now rotate the translation to the new orientation
	Rotate3DPt(TotalRotation, InitialTrans, &FinalTrans);
	TotalRotation[3][0] = FinalTrans.x;
	TotalRotation[3][1] = FinalTrans.y;
	TotalRotation[3][2] = FinalTrans.z;
}	/*FlipRotation*/
// Sets the plane of the screen to that defined by the three points provided
Boolean MoleculeData::SetScreenPlane(CPoint3D *Points) {
	CPoint3D	Vector1, Vector2, Vector3;

	Vector1.x = Points[1].x - Points[0].x;
	Vector1.y = Points[1].y - Points[0].y;
	Vector1.z = Points[1].z - Points[0].z;
	Vector2.x = Points[2].x - Points[0].x;
	Vector2.y = Points[2].y - Points[0].y;
	Vector2.z = Points[2].z - Points[0].z;
	
	float length = Vector1.Magnitude();
	if (length <= 0.0) return false;	//2 points with the same coordinates
	Vector1.x /= length;
	Vector1.y /= length;
	Vector1.z /= length;
	length = Vector2.Magnitude();
	if (length <= 0.0) return false;
	Vector2.x /= length;
	Vector2.y /= length;
	Vector2.z /= length;
	float V1DotV2 = DotProduct3D(&Vector2, &Vector1);
		//Make sure the vectors are not parallel or anti-parallel
	if (fabs(V1DotV2) >= 1.0) return false;
	Vector2.x -= V1DotV2*Vector1.x;
	Vector2.y -= V1DotV2*Vector1.y;
	Vector2.z -= V1DotV2*Vector1.z;
	Normalize3D(&Vector2);
	CrossProduct3D (&Vector1, &Vector2, &Vector3);
	Normalize3D(&Vector3);
	
	TotalRotation[0][0] = Vector1.x;
	TotalRotation[1][0] = Vector1.y;
	TotalRotation[2][0] = Vector1.z;
	TotalRotation[0][3] = 0.0;
	TotalRotation[0][1] = Vector2.x;
	TotalRotation[1][1] = Vector2.y;
	TotalRotation[2][1] = Vector2.z;
	TotalRotation[1][3] = 0.0;
	TotalRotation[0][2] = Vector3.x;
	TotalRotation[1][2] = Vector3.y;
	TotalRotation[2][2] = Vector3.z;
	TotalRotation[2][3] = TotalRotation[3][0] = TotalRotation[3][1] = TotalRotation[3][2] = 0.0;
	TotalRotation[3][3] = 1.0;
		//Rotation the first Point and use for the translation
	Rotate3DPt(TotalRotation, Points[0], &Vector1);
	TotalRotation[3][0] = -Vector1.x;
	TotalRotation[3][1] = -Vector1.y;
	TotalRotation[3][2] = -Vector1.z;
	
	ResetRotation();
	return true;
}
//Linear Least Squares fit:  minimize the sum of squares of the differences between each
//frame. This will "fix" a series of frames to give a nice, smooth animation.
void MoleculeData::LinearLeastSquaresFit(Progress * lProgress) {
	long		MinFrameNum=CurrentFrame;

	Frame * lFrame = Frames;
	Frame * l2Frame = lFrame->NextFrame;
	lProgress->ChangeText("Minimizing frame motions...");

//First run through the frame list and make sure that each frame has the same number
//and order for the atoms. If not this routine will not work correctly!
	while (l2Frame) {
		if (lFrame->NumAtoms != l2Frame->NumAtoms) goto FrameIncorrect;
		for (long iatom=0; iatom<lFrame->NumAtoms; iatom++) {
			if (lFrame->Atoms[iatom].Type != l2Frame->Atoms[iatom].Type) goto FrameIncorrect;
		}
		l2Frame = l2Frame->NextFrame;
	}
//Ok set of frames so get started fitting
	l2Frame = lFrame->NextFrame;
	Matrix4D	FitMatrix, TempRotMat;
	float		SquaresValue, NewSquareValue, RotAngle, TransAmount;
	long		iOptAtoms;
		//Save the current frame and rotation
	Frame * SavedFrame;
	SavedFrame = cFrame;
	Matrix4D SavedRotation;
	::CopyMatrix(TotalRotation, SavedRotation);
	while (l2Frame) {
		InitRotationMatrix(FitMatrix);		//zero out the fit matrix to start with...
//Now move the first atom of the second frame to the same position as the first frame.
//This is done by first moving it to the origin, then putting a translation in the rotation
//matrix to the desired location.
		FitMatrix[3][0] = -l2Frame->Atoms[0].Position.x;
		FitMatrix[3][1] = -l2Frame->Atoms[0].Position.y;
		FitMatrix[3][2] = -l2Frame->Atoms[0].Position.z;
		for (long iatom=0; iatom<lFrame->NumAtoms; iatom++) {
			l2Frame->Atoms[iatom].Position.x += FitMatrix[3][0];
			l2Frame->Atoms[iatom].Position.y += FitMatrix[3][1];
			l2Frame->Atoms[iatom].Position.z += FitMatrix[3][2];
		}
		FitMatrix[3][0] = lFrame->Atoms[0].Position.x;
		FitMatrix[3][1] = lFrame->Atoms[0].Position.y;
		FitMatrix[3][2] = lFrame->Atoms[0].Position.z;
		Boolean	Done;
		for (long ipass=0; ipass<4; ipass++) {
			if (ipass<3) {
				RotAngle = 10.0;	TransAmount=0.1;
				if (ipass == 0) iOptAtoms = MIN(2, lFrame->NumAtoms);
				else if (ipass == 1) iOptAtoms = MIN(3, lFrame->NumAtoms);
				else iOptAtoms = lFrame->NumAtoms;
				for (int ii=0; ii<iOptAtoms; ii++)
					Rotate3DPt(FitMatrix, l2Frame->Atoms[ii].Position, &(RotCoords[ii]));
				SquaresValue = CalculateSquaresValue(iOptAtoms, lFrame->Atoms, RotCoords);
			}
			Done = false;
			while (!Done) {
				Done = true;
				Boolean RotDone = false;
				for (long jpass=0; jpass<2; jpass++) {
					while (!RotDone) {
						RotDone = true;
						::CopyMatrix (FitMatrix, TempRotMat);
						for (long ii=0; ii<3; ii++) {
							ApplyRotation(TempRotMat, ii, RotAngle);
							for (int i=0; i<iOptAtoms; i++)
								Rotate3DPt(TempRotMat, l2Frame->Atoms[i].Position, &(RotCoords[i]));
							NewSquareValue = CalculateSquaresValue(iOptAtoms, lFrame->Atoms, RotCoords);
							if (NewSquareValue<SquaresValue) {
								RotDone = Done = false;
								SquaresValue = NewSquareValue;
								CopyMatrix (TempRotMat, FitMatrix);
							} else {
								ApplyRotation(TempRotMat, ii, -2.0*RotAngle);
								for (int i=0; i<iOptAtoms; i++)
									Rotate3DPt(TempRotMat, l2Frame->Atoms[i].Position, &(RotCoords[i]));
								NewSquareValue = CalculateSquaresValue(iOptAtoms, lFrame->Atoms, RotCoords);
								if (NewSquareValue<SquaresValue) {
									RotDone = Done = false;
									SquaresValue = NewSquareValue;
									CopyMatrix (TempRotMat, FitMatrix);
								} else {
									CopyMatrix (FitMatrix, TempRotMat);
								}
							}
						}
		//"clean up" the rotation matrix make the rotation part orthogonal and magnitude 1
						OrthogonalizeRotationMatrix (FitMatrix);
					}
					RotAngle *= 0.1;
				}
				if (ipass > 2) {	//Only rotate for the first two passes
					Boolean TransDone = false;
					while (!TransDone) {
						TransDone = true;
						for (long ii=0; ii<3; ii++) {
							FitMatrix[3][ii] += TransAmount;
							for (int i=0; i<iOptAtoms; i++)
								Rotate3DPt(FitMatrix, l2Frame->Atoms[i].Position, &(RotCoords[i]));
							NewSquareValue = CalculateSquaresValue(iOptAtoms, lFrame->Atoms, RotCoords);
							if (NewSquareValue<SquaresValue) {
								TransDone = Done = false;
								SquaresValue = NewSquareValue;
							} else {
								FitMatrix[3][ii] -= 2.0*TransAmount;
								for (int i=0; i<iOptAtoms; i++)
									Rotate3DPt(FitMatrix, l2Frame->Atoms[i].Position, &(RotCoords[i]));
								NewSquareValue = CalculateSquaresValue(iOptAtoms, lFrame->Atoms, RotCoords);
								if (NewSquareValue<SquaresValue) {
									TransDone = Done = false;
									SquaresValue = NewSquareValue;
								} else {
									FitMatrix[3][ii] += TransAmount;
								}
							}
						}
						if (!lProgress->UpdateProgress(MinFrameNum/(NumFrames-CurrentFrame)))
							goto CleanUpAndExit;
					}
				}
			}
			TransAmount *= 0.1;
		}
//Done with this set of frames. Call StickCoordinates to copy over the coords, and
//update any MO vectors, surfaces, and normal mode information.
		cFrame = l2Frame;
		::CopyMatrix(FitMatrix, TotalRotation);
		StickCoordinates();

		lFrame = l2Frame;
		l2Frame = l2Frame->NextFrame;
		MinFrameNum++;
	}
CleanUpAndExit:		//Clean up the RotCoords Array...
	cFrame = SavedFrame;
	::CopyMatrix(SavedRotation, TotalRotation);
	ResetRotation();
#ifdef CarbonBuild
	Cursor	arrow;	
	SetCursor(GetQDGlobalsArrow(&arrow));
#else
	SetCursor(&qd.arrow);	//reset the cursor to the standard arrow
#endif
	return;	//Correct return
FrameIncorrect:		//Incorrect frame list for this routine: throw up an error and exit
	Str255	errmsg;
	GetIndString(errmsg, kerrstrings, 33);
	MessageAlert(errmsg);
}	/*LinearLeastSquaresFit*/
Boolean MoleculeData::OrbSurfacePossible(void) {
	Boolean result = false;
	if (Basis) result = true;
	return result;
}
Boolean MoleculeData::TotalDensityPossible(void) {
	Boolean result = false;
	if (Basis) {
		if (cFrame->Orbs.size() > 0) {//Currently TE density is based on eigenvectors only
			std::vector<OrbitalRec *>::const_iterator OrbSet = cFrame->Orbs.begin();
			while (OrbSet != cFrame->Orbs.end()) {
				if ((*OrbSet)->TotalDensityPossible()) {	//Is this a good enough test?
					result = true;
					break;
				}
				OrbSet++;
			}
		}
	}
	return result;
}
//Read in a general basis set from a GAMESS log file
void MoleculeData::ParseGAMESSBasisSet(BufferFile * Buffer) {

	Basis = BasisSet::ParseGAMESSBasisSet(Buffer, cFrame->NumAtoms);
	if (Basis) {	//Setup the default nuclear charge array. This will be changed if
					//ECP's are used.
			long iatom;
		for (iatom=0; iatom<cFrame->NumAtoms; iatom++) {
			Basis->NuclearCharge[iatom] = cFrame->Atoms[iatom].GetNuclearCharge();
			if (Basis->NuclearCharge[iatom] == 115)	//dummy atom/no charge
				Basis->NuclearCharge[iatom] = 0;
		}
	}
}	/*ParseGAMESSBasisSet*/
void MoleculeData::GetModelCenter(CPoint3D * center) {
		CPoint3D	temp;
	temp.x = -TotalRotation[3][0];
	temp.y = -TotalRotation[3][1];
	temp.z = -TotalRotation[3][2];
	BackRotate3DOffset(TotalRotation, &temp, center);
}
void MoleculeData::SetModelCenter(CPoint3D * center) {
		CPoint3D	temp;
	Rotate3DOffset(TotalRotation, *center, &temp);
	TotalRotation[3][0] = -temp.x;
	TotalRotation[3][1] = -temp.y;
	TotalRotation[3][2] = -temp.z;
}
void MoleculeData::GetModelRotation(float * Psi, float * Phi, float * Theta) {
	MatrixToEulerAngles(TotalRotation, Psi, Phi, Theta);
}
void MoleculeData::SetModelRotation(float Psi, float Phi, float Theta) {
	CPoint3D	Center;
	GetModelCenter(&Center);	//Retrieve off the old center
	EulerAnglesToMatrix(TotalRotation, Psi, Phi, Theta);
	SetModelCenter(&Center);
}
void MoleculeData::CreateLLM(long NumPts, WinPrefs * Prefs) {
	Frame *	NewFrame, * NewFrame2;
	
	Frame * lFrame = cFrame;
	Frame * lEndFrame = lFrame->NextFrame;
	long NumAtoms = lFrame->NumAtoms;
	long memNeeded = (NumPts+1)*(sizeof(Frame)+NumAtoms*sizeof(Atom))+5000;
	if (MaxBlock() < memNeeded) return;	//roughly determine if sufficient memory is available
	
	CPoint3D * offset = new CPoint3D[NumAtoms];
	if (!offset) throw MemoryError();
	NumPts++;
	for (long i=0; i<NumAtoms; i++) {
		offset[i].x = (lEndFrame->Atoms[i].Position.x-lFrame->Atoms[i].Position.x)/NumPts;
		offset[i].y = (lEndFrame->Atoms[i].Position.y-lFrame->Atoms[i].Position.y)/NumPts;
		offset[i].z = (lEndFrame->Atoms[i].Position.z-lFrame->Atoms[i].Position.z)/NumPts;
	}
	NumPts--;
	NewFrame2 = lFrame;
	for (long counter=0; counter<NumPts; counter++) {
		NewFrame = AddFrame(NumAtoms, lFrame->NumBonds);
		for (long iatm=0; iatm<NumAtoms; iatm++) {
			NewFrame->Atoms[iatm].Type = lFrame->Atoms[iatm].Type;
			NewFrame->Atoms[iatm].Highlite = 0;
			NewFrame->Atoms[iatm].Position.x = NewFrame2->Atoms[iatm].Position.x + offset[iatm].x;
			NewFrame->Atoms[iatm].Position.y = NewFrame2->Atoms[iatm].Position.y + offset[iatm].y;
			NewFrame->Atoms[iatm].Position.z = NewFrame2->Atoms[iatm].Position.z + offset[iatm].z;
		}
		NewFrame->NumAtoms = NumAtoms;
		if (Prefs->GetAutoBond())
			NewFrame->SetBonds(Prefs, false);
		NewFrame2 = NewFrame;
	}
	delete [] offset;
} /*CreateLLM*/

//CreateInternalLLM creates a linear least motion path based on the internal
//coordinates of the two frames. In general this will result is a lower energy
//path than a cartesian based LLM path, since it prevents bonds from being
//stretched or squashed.
void MoleculeData::CreateInternalLLM(long NumPts, WinPrefs * Prefs) {
	Frame *	NewFrame, * NewFrame2;
	
	Frame * lFrame = cFrame;
	Frame * lEndFrame = lFrame->NextFrame;
	long NumAtoms = lFrame->NumAtoms;
	long SavedFrameNum = CurrentFrame;

	if (NumAtoms <= 2) {	//No point to using internals when there are less than 3 atoms!
		CreateLLM(NumPts, Prefs);
		return;
	}
	MOPacInternals * lInternals = IntCoords->GetMOPacStyle();
	if (!lInternals) return;
	float * OffsetValues = new float[3*NumAtoms];
	float * StartValues = new float[3*NumAtoms];
	if (!OffsetValues || !StartValues) throw MemoryError();
		//Make sure the internals are up to date for the first frame
	lInternals->CartesiansToInternals(this);
		long i, part;
	for (i=1; i<NumAtoms; i++)
		for (part=0; part<3; part++)
			StartValues[3*i+part] = lInternals->GetValue(i, part);
	cFrame = lEndFrame;
	lInternals->CartesiansToInternals(this);
	for (i=0; i<9; i++) OffsetValues[i] = 0.0;
	NumPts++;
	for (i=1; i<NumAtoms; i++) {
		OffsetValues[3*i] = (lInternals->GetValue(i, 0) - StartValues[3*i])/NumPts;
		if (i>=2) {
			OffsetValues[3*i+1] = (lInternals->GetValue(i, 1) - StartValues[3*i+1])/NumPts;
			if (i>=3) {	//dihedrals are tricky. We must take into account that they run from
					//180 to -180 and the shorest rotation might be through 180.
				OffsetValues[3*i+2] = lInternals->GetValue(i, 2) - StartValues[3*i+2];
				if (OffsetValues[3*i+2] > 180.0) OffsetValues[3*i+2] = OffsetValues[3*i+2] - 360.0;
				else if (OffsetValues[3*i+2] < -180.0) OffsetValues[3*i+2] = 360 - OffsetValues[3*i+2];
				OffsetValues[3*i+2] /= NumPts;
			}
		}
	}
	NumPts--;
	cFrame = lFrame;
	NewFrame2 = lFrame;
	for (long counter=0; counter<NumPts; counter++) {
			//add a new frame and copy the coordinates from the previous frame
		NewFrame = AddFrame(NumAtoms, lFrame->NumBonds);
		for (long iatm=0; iatm<NumAtoms; iatm++) {
			NewFrame->Atoms[iatm].Type = lFrame->Atoms[iatm].Type;
			NewFrame->Atoms[iatm].Highlite = 0;
			NewFrame->Atoms[iatm].Position.x = NewFrame2->Atoms[iatm].Position.x;
			NewFrame->Atoms[iatm].Position.y = NewFrame2->Atoms[iatm].Position.y;
			NewFrame->Atoms[iatm].Position.z = NewFrame2->Atoms[iatm].Position.z;
		}
		NewFrame->NumAtoms = NumAtoms;
			//Now set the internals to their proper values
		cFrame = NewFrame;
		for (i=1; i<NumAtoms; i++) {
			StartValues[3*i] += OffsetValues[3*i];
			lInternals->SetValue(i, 0, StartValues[3*i]);
			if (i>=2) {
				StartValues[3*i+1] += OffsetValues[3*i+1];
				if (StartValues[3*i+1] < 0) StartValues[3*i+1] = 0.0;
				if (StartValues[3*i+1] > 180.0) StartValues[3*i+1] = 180.0;
				lInternals->SetValue(i, 1, StartValues[3*i+1]);
				if (i>=3) {
					StartValues[3*i+2] += OffsetValues[3*i+2];
					if (StartValues[3*i+2] < -180.0) StartValues[3*i+2] += 360.0;;
					if (StartValues[3*i+2] > 180.0) StartValues[3*i+2] -= 360.0;
					lInternals->SetValue(i, 2, StartValues[3*i+2]);
				}
			}
		}
			//Now convert the internals to cartesians so they are saved.
		lInternals->InternalsToCartesians(this, Prefs, 0);

		if (Prefs->GetAutoBond())
			NewFrame->SetBonds(Prefs, false);
		NewFrame2 = NewFrame;
	}
	cFrame = lFrame;	//reset the current frame pointer
	CurrentFrame = SavedFrameNum;
	delete [] OffsetValues;
	delete [] StartValues;
	Progress * lProg = new Progress;
	if (lProg) {
		LinearLeastSquaresFit(lProg);
		delete lProg;
	}
}

void MoleculeData::AdvanceFrame(void) {
	if (cFrame->NextFrame) cFrame = cFrame->NextFrame;
}
void MoleculeData::SetCurrentFrame(long FrameNum) {
	if ((FrameNum>NumFrames)||(FrameNum<1)) return;
	if (FrameNum < CurrentFrame) {
		cFrame = Frames;
		CurrentFrame = 1;
	}
	while (CurrentFrame < FrameNum) {
		cFrame = cFrame->NextFrame;
		CurrentFrame++;
	}
}
Boolean MoleculeData::SurfaceExportPossible(void) {
	return cFrame->SurfaceExportPossible();
}
Boolean MoleculeData::ModeVisible(void) {
	return ((cFrame->Vibs)&&GetDrawMode());
}
long MoleculeData::GetNumBonds(void) {
	return cFrame->GetNumBonds();
}
void MoleculeData::DeleteAtom(long AtomNum) {
	cFrame->DeleteAtom(AtomNum);
	ResetRotation();
}
Boolean MoleculeData::ValidAtom(long AtomNum) {
	return ((AtomNum>=0)&&(AtomNum<cFrame->NumAtoms));
}
AtomTypeList * MoleculeData::GetAtomTypes(void) {return cFrame->GetAtomTypes();}

