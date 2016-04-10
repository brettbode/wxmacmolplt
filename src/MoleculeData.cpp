/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* MoleculeData.cpp

	class MoleculeData organizes all information on the molecule for each window/file

	Created from other files, BMB, 4/1998
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
#include "myFiles.h"
#include <string.h>
#include <new>
#include <map>
#if defined(WIN32)
#undef AddAtom
#endif

MoleculeData::MoleculeData(MolDisplayWin *MolWin) {
	this->MolWin = MolWin;
	RotCoords = NULL;
	cFrame = Frames = new Frame(MolWin);
	CurrentFrame=1;
	NumFrames = 1;
	IntCoords = NULL;
	Basis = NULL;
	Description = NULL;
	MaxAtoms = 0;
	MaxSize=0.5;
	WindowSize=3.0;
	InitRotationMatrix(TotalRotation);
	InputOptions = NULL;
	DrawMode = 0;
	DrawLabels = 0;
	constrain_anno_id = -1;
}
MoleculeData::~MoleculeData(void) {
	DeleteAllAnnotations();
	while (Frames) {
		cFrame = Frames->GetNextFrame();
		delete Frames;
		Frames = cFrame;
	}
	if (RotCoords) delete [] RotCoords;
	if (Description) delete [] Description;
	if (IntCoords) delete IntCoords;
	if (Basis) delete Basis;
	if (InputOptions) delete InputOptions;
}
bool MoleculeData::SetupFrameMemory(long NumAtoms, long NumBonds) {
	if (NumAtoms < cFrame->AtomAllocation) NumAtoms = cFrame->AtomAllocation;
	if (NumBonds < cFrame->BondAllocation) NumBonds = cFrame->BondAllocation;
	if ((NumAtoms == cFrame->AtomAllocation)&&
		(NumBonds == cFrame->BondAllocation)) return true;//no need to resize
	mpAtom * tempAtoms=NULL;
	Bond * tempBonds=NULL;
	try {
		tempAtoms = new mpAtom[NumAtoms];
		tempBonds = new Bond[NumBonds];
	}
	catch (std::bad_alloc) {
		if (tempAtoms) delete [] tempAtoms;
		if (tempBonds) delete [] tempBonds;
		return false;
	}
	memcpy(tempAtoms, cFrame->Atoms, cFrame->NumAtoms*sizeof(mpAtom));
	memcpy(tempBonds, cFrame->Bonds, cFrame->NumBonds*sizeof(Bond));
	if (cFrame->Atoms) delete [] cFrame->Atoms;	//delete old arrays, if any
	if (cFrame->Bonds) delete [] cFrame->Bonds;
	cFrame->Atoms = tempAtoms;
	cFrame->Bonds = tempBonds;
	cFrame->AtomAllocation = NumAtoms;
	cFrame->BondAllocation = NumBonds;
	if (NumAtoms > MaxAtoms) {	//Need to extend the size of the zbuffer arrays
		if (RotCoords) delete [] RotCoords;
		try {
			RotCoords = new CPoint3D[NumAtoms];
		}
		catch (std::bad_alloc) {
			if (RotCoords) delete [] RotCoords;
			RotCoords = new CPoint3D[MaxAtoms];
			return false;
		}
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
		lNewFrame = new Frame(MolWin);
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
		lNewFrame = new Frame(MolWin);
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
	Frame * temp = new Frame(MolWin);
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

const Matrix4D& MoleculeData::GetRotationMatrix() const {
	return TotalRotation;
}

#include <iostream>
//routine calculates the cartesian center of the molecule and offsets the 
//coordinate system of the window to match the moleculer center.
void MoleculeData::CenterModelWindow(void) {
	float	XMin, XMax, YMin, YMax, ZMin, ZMax;
//first set the Min and Max to big and small values
	XMin=YMin=ZMin=1.0e10;
	XMax=YMax=ZMax=-1.0e10;

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
	Centroid.x = (XMax+XMin)*0.5;
	Centroid.y = (YMax+YMin)*0.5;
	Centroid.z = (ZMax+ZMin)*0.5;
//Recompute the maximum width of the molecule
	MaxSize = MAX((XMax-XMin), (YMax-YMin));
	MaxSize = MAX(MaxSize, (ZMax-ZMin));
	MolCentroid = Centroid;
	
}	/*CenterModelWindow*/

void MoleculeData::ResetRotation(void) {
	if ((cFrame->NumAtoms > MaxAtoms)||(RotCoords==NULL)) {
		if (RotCoords != NULL) {
			delete [] RotCoords;
			RotCoords = NULL;
		}
		RotCoords = new CPoint3D[cFrame->NumAtoms];
		MaxAtoms = cFrame->NumAtoms;
	}
	for (long i=0; i<cFrame->NumAtoms; i++) {
		Rotate3DPt(TotalRotation, (cFrame->Atoms[i].Position), &(RotCoords[i]));
	}
		//Now sort the Z Buffer
//	SortzBuffer(RotCoords, zBuffer, cFrame->NumAtoms);
}
void MoleculeData::InitializeInternals(void) {
	if (!IntCoords) IntCoords = new Internals;
}
// Stick the coordinates by moving the rotated coordinates to the file coordinates
// being careful to invalidate any orbital info
void MoleculeData::StickCoordinates(void) {
	//Clean up the rotation matrix before making permenant changes
	OrthogonalizeRotationMatrix(TotalRotation);
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
		std::vector<CPoint3D> & Modes = cFrame->Vibs->NormMode;
		CPoint3D NMode;
		for (long imode=0; imode < cFrame->Vibs->NumModes; imode++) {
			long cmode = imode*cFrame->NumAtoms;
			for (long iatom=0; iatom<cFrame->NumAtoms; iatom++) {
				Rotate3DOffset(TotalRotation, Modes[iatom+cmode], &NMode);
				Modes[iatom+cmode] = NMode;
			}
		}
	}
		long i;
	for (i=0; i<cFrame->NumAtoms; i++) {
		Rotate3DPt(TotalRotation, cFrame->Atoms[i].Position - Centroid,
				   &(RotCoords[i]));
	}
	for (i=0; i<cFrame->NumAtoms; i++) {
		cFrame->Atoms[i].Position = RotCoords[i];
	}
	Centroid = CPoint3D(0.0f, 0.0f, 0.0f);
	InitRotationMatrix(TotalRotation);
}

void MoleculeData::NewAtom(const mpAtom& atom, bool updateGlobal, long index, const CPoint3D *pos) {
	cFrame->AddAtom(atom, index, pos);

	// Adjust annotations that connect higher-numbered atoms.
	if (index >= 0 && index < cFrame->NumAtoms - 1) {
		std::vector<Annotation *>::iterator anno;
		for (anno = Annotations.begin(); anno != Annotations.end(); ++anno) {
			(*anno)->adjustIds(index - 1, 1);
		}
	}

	if (IntCoords) {
		MOPacInternals * mInts = IntCoords->GetMOPacStyle();
		if (mInts) mInts->AppendAtom(this);
	}
	if (updateGlobal) AtomAdded();
}

void MoleculeData::NewAtom(long AtomType, const CPoint3D & AtomPosition, bool updateGlobal, long index) {
	cFrame->AddAtom(AtomType, AtomPosition, index);

	// Adjust annotations that connect higher-numbered atoms.
	if (index >= 0 && index < cFrame->NumAtoms - 1) {
		std::vector<Annotation *>::iterator anno;
		for (anno = Annotations.begin(); anno != Annotations.end(); ++anno) {
			(*anno)->adjustIds(index - 1, 1);
		}
	}

	if (IntCoords) {
		MOPacInternals * mInts = IntCoords->GetMOPacStyle();
		if (mInts) mInts->AppendAtom(this);
	}
	if (updateGlobal) AtomAdded();
}

void MoleculeData::AtomAdded(void) {
	if (cFrame->AtomAllocation>MaxAtoms) {
		if (RotCoords) delete [] RotCoords;
		RotCoords = new CPoint3D[cFrame->AtomAllocation];
		if (!RotCoords) throw MemoryError();
		MaxAtoms = cFrame->AtomAllocation;
	}
	float	XMin, XMax, YMin, YMax, ZMin, ZMax;
	//first set the Min and Max to big and small values
	XMin=YMin=ZMin=1.0E10;
	XMax=YMax=ZMax=-1.0E10;
	
	for (long iatom=0; iatom<cFrame->NumAtoms; iatom++) {
		XMin = MIN(XMin, cFrame->Atoms[iatom].Position.x);
		XMax = MAX(XMax, cFrame->Atoms[iatom].Position.x);
		YMin = MIN(YMin, cFrame->Atoms[iatom].Position.y);
		YMax = MAX(YMax, cFrame->Atoms[iatom].Position.y);
		ZMin = MIN(ZMin, cFrame->Atoms[iatom].Position.z);
		ZMax = MAX(ZMax, cFrame->Atoms[iatom].Position.z);
	}
	//Recompute the maximum width of the molecule
	MaxSize = MAX(MaxSize, (XMax-XMin));
	MaxSize = MAX(MaxSize, (YMax-YMin));
	MaxSize = MAX(MaxSize, (ZMax-ZMin));
	
	while (FMOFragmentIds.size() < cFrame->GetNumAtoms())
		FMOFragmentIds.push_back(1);	//initialize the fragment id for the new atoms

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
void MoleculeData::UnitConversion(bool AtoB) {
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
	wxPoint			p(0,0), q(10,10), sphereCenter(10,10);
	long			sphereRadius=10;
	Matrix4D		rotationMatrix, tempcopyMatrix;

//180 degree rotation is desired, but if the two vectors are colinear then the
//rotation will not have a well defined axis so use two 90 degree rotations instead
	if (theItem == 0) {	//flipHitem
		p.y = 10;
	} else {	//theItem == 1 (flipVitem)
		p.x = 10;
	}
	VirtualSphereQD3D (p, q, sphereCenter, sphereRadius, rotationMatrix, TotalRotation);
	float *m = (float *) rotationMatrix;
	std::cout << "m[#](# in 0,16): " << std::endl
		<< m[0] << ", " << m[1] << ", " << m[2] << ", " << m[3] << ", " <<
										   std::endl
		<< m[4] << ", " << m[5] << ", " << m[6] << ", " << m[7] << ", " <<
										   std::endl
		<< m[8] << ", " << m[9] << ", " << m[10] << ", " << m[11] << ", " <<
										   std::endl
		<< m[12] << ", " << m[13] << ", " << m[14] << ", " << m[15] << std::endl;

/* Concatenate the new rotation with the current rotation */
//Mulitply twice since the rotate generated is 90 degrees and we want 180
	MultiplyMatrix (rotationMatrix, TotalRotation, tempcopyMatrix);
	CopyMatrix (tempcopyMatrix, TotalRotation);
	MultiplyMatrix (rotationMatrix, TotalRotation, tempcopyMatrix);
	CopyMatrix (tempcopyMatrix, TotalRotation);
}	/*FlipRotation*/
// Sets the plane of the screen to that defined by the three points provided
bool MoleculeData::SetScreenPlane(CPoint3D *Points) {
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
	/* Rotate3DPt(TotalRotation, Points[0], &Vector1); */
	/* Centroid = Vector1 * -1.0f; */
	
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
		bool	Done;
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
				bool RotDone = false;
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
					bool TransDone = false;
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
	return;	//Correct return
FrameIncorrect:		//Incorrect frame list for this routine: throw up an error and exit
	MessageAlert("Sorry each frame must have the same number and order of atoms in order to fit an animation path.");
}	/*LinearLeastSquaresFit*/
bool MoleculeData::OrbSurfacePossible(void) const {
	bool result = false;
	if (Basis) result = true;
	return result;
}
bool MoleculeData::TotalDensityPossible(void) const {
	bool result = false;
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
bool MoleculeData::MEPCalculationPossible(void) const {
	bool result = false;
	if (Basis) {
		result = Basis->AreNuclearChargesValid();
		if (result) result = TotalDensityPossible();
	}
	return result;
}
//Read in a general basis set from a GAMESS log file
void MoleculeData::ParseGAMESSBasisSet(BufferFile * Buffer) {

	Basis = BasisSet::ParseGAMESSBasisSet(Buffer, cFrame->NumAtoms, cFrame->Atoms);
	if (Basis) {	//Setup the default nuclear charge array. This will be changed if
					//ECP's are used.
			long iatom;
		for (iatom=0; iatom<cFrame->NumAtoms; iatom++) {
			Basis->NuclearCharge[iatom] = cFrame->Atoms[iatom].GetNuclearCharge();
			if (Basis->NuclearCharge[iatom] == 115)	//dummy atom/no charge
				Basis->NuclearCharge[iatom] = 0;
		}
		Basis->NuclearChargesAreValid(true);
	}
}	/*ParseGAMESSBasisSet*/
bool MoleculeData::ParseFMOIds(BufferFile * Buffer, const long & AtomCount, const wxFileOffset & EndOfGroup) {
	if (Buffer->LocateKeyWord("INDAT", 5, EndOfGroup)) {
		Buffer->LocateKeyWord("=", 1, EndOfGroup);
		Buffer->BufferSkip(1);
		
		//Prep the fragment mapping array so we can detect 
		if (FMOFragmentIds.size() > 0) {
			//hmmm this array is already setup? must be an error?? Consider this as non-fatal
			wxLogMessage(_("Starting to parse INDAT array, but already have existing data!"));
		}
		FMOFragmentIds.reserve(AtomCount);
		for (long i=0; i<AtomCount; i++) FMOFragmentIds.push_back(0);

		// read in the ids here
		//This array comes in two forms, but either way it specifes how atoms are divided into fragments
		long tempL;
		if (!Buffer->ReadLongValue(tempL, EndOfGroup)) {
			wxLogMessage(_("Error: Unable to locate the first integer for the INDAT array. Skipping Fragment Ids."));
			return false;
		}

		if (tempL == 0) { //Style b, atoms grouped by fragment
			InputOptions->FMO.UseFragmentINDAT(true);
			for (long ifragment=0; ifragment<InputOptions->FMO.GetNumberFragments(); ifragment++) {
				long lastatom=0;
				do {
					if (!Buffer->ReadLongValue(tempL, EndOfGroup)) {
						wxString msg;
						msg.Printf(_("Error: Unable to parse atom Id for fragment %d. Skipping the remainder of the array."),
								   ifragment+1);
						wxLogMessage(msg);
						return false;
					}
					if ((tempL > 0)&&(tempL<=AtomCount)) {
						FMOFragmentIds[tempL-1] = ifragment+1;
						lastatom = tempL;
					} else if (tempL<0) {
						if (lastatom==0) {
							wxString msg;
							msg.Printf(_("Error: Encountered %d not preceeded by a valid atom id. Skipping rest of INDAT."),
									   tempL);
							wxLogMessage(msg);
							return false;
						}
						tempL *= -1;
						if ((tempL <= (lastatom))||(tempL > AtomCount)) {
							wxString msg;
							msg.Printf(_("Error: Encountered %d preceeded by %d. Skipping invalid data."),
									   -1*tempL, lastatom);
							wxLogMessage(msg);
						} else {
							for (long jatom=lastatom; jatom < tempL; jatom++) {
								FMOFragmentIds[jatom] = ifragment+1;
							}
						}
					}
				} while (tempL > 0);
			}
		} else {	//Style a, array of fragment ids, one per atom
			//read in each id, test it, and store it
			InputOptions->FMO.UseFragmentINDAT(false);
			
			for (long iatm=0; iatm<AtomCount; iatm++) {
				if (iatm>0) {	//skip the 1st pass since we already have it from above
					if (!Buffer->ReadLongValue(tempL, EndOfGroup)) {
						wxString msg;
						msg.Printf(_("Error: Unable to parse fragment Id for atom %d. Skipping the remainder of the array."),
								   iatm+1);
						wxLogMessage(msg);
						return false;
					}
				}
				if ((tempL < 1)||(tempL > InputOptions->FMO.GetNumberFragments())) {
					wxString msg;
					msg.Printf(_("Warning: Invalid fragment id assigned to atom %d, value %d."), iatm+1, tempL);
					wxLogMessage(msg);
				} else
					FMOFragmentIds[iatm] = tempL;
			}
		}
		
		// Now check the array to see if all atoms were assigned to a fragment
		for (int i=0; i<FMOFragmentIds.size(); i++) {
			if (FMOFragmentIds[i] < 1) {
				wxString msg;
				msg.Printf(_("Warning: Atom %d is not assigned to a fragment."), i);
				wxLogMessage(msg);
			}
		}
	}
	return true;
}
//Parse a gaussian style zmatrix as would be formated in GAMESS input
void MoleculeData::ParseZMatrix(BufferFile * Buffer, const long & nAtoms, WinPrefs * Prefs) {
	if (!IntCoords) {
		IntCoords = new Internals;
	}
	MOPacInternals * mInts = IntCoords->GetMOPacStyle();
	if (!mInts) {
		IntCoords->CreateMOPacInternals(3*nAtoms);
		mInts = IntCoords->GetMOPacStyle();
	}
	float unitConversion = 1.0;
	if (InputOptions && InputOptions->Data->GetUnits()) unitConversion = kBohr2AngConversion;
	int iline=0;
	long StartPos = Buffer->GetFilePos();
	Buffer->LocateKeyWord("$END", 4);
	long EndPos = Buffer->GetFilePos();
	Buffer->SetFilePos(StartPos);
	//First jump past the atoms and build a map of any string=value pairs (if any)
	std::map<std::string, double> valueMap;
	long bPos = Buffer->FindBlankLine();
	if ((bPos > 0)&&(bPos < EndPos)) {
		Buffer->SetFilePos(bPos);
		Buffer->SkipnLines(1);
		while (Buffer->GetFilePos() < EndPos) {
			char		token[kMaxLineLength], Line[kMaxLineLength];
			double		value;
			Buffer->GetLine(Line);
			char * last=NULL;
			int foo=0;	//Since VS lacks strtok_r we'll make do with this
			while ((Line[foo] != '\0')&&(foo < kMaxLineLength)) {
				if (Line[foo] == '=') {
					Line[foo]='\0';
					last = &(Line[foo+1]);
					break;
				}
				foo++;
			}
		//	if (strtok_r(Line, "=", &last) == NULL) break;
			if (last == NULL) break;
			int readCount = sscanf(Line, "%s", token);
			readCount += sscanf(last, "%lf", &value);
			if (readCount!=2) break;
			std::pair<std::string, double> myVal(token, value);
			std::pair<std::map<std::string, double>::iterator, bool> p = valueMap.insert(myVal);
			if (! p.second) {	//We have hit a duplicate value
				MessageAlert("Duplicate keys detected in the value list");
			}
		}
		Buffer->SetFilePos(StartPos);
	}
	
	while ((Buffer->GetFilePos() < EndPos)&&(iline<nAtoms)) {
		CPoint3D	pos = CPoint3D(0.0f, 0.0f, 0.0f);	//This is just a placeholder
		char		token[kMaxLineLength], Line[kMaxLineLength], bondText[kMaxLineLength],
					angleText[kMaxLineLength], dihedralText[kMaxLineLength];
		float		bondLength, bondAngle, bondDihedral;
		long		AtomType;
		int			con1, con2, con3;
		Buffer->GetLine(Line);
		int readCount = sscanf(Line, "%s %d %s %d %s %d %s", token, &con1, bondText, &con2, &angleText, &con3,
							   &dihedralText);
		if (readCount < 1) break;	//failed to parse anything??
		AtomType = SetAtomType((unsigned char *) token);
		if (AtomType < 0) break;
		cFrame->AddAtom(AtomType, pos);
		if (iline > 0) {
			if (readCount < 2) break;
			if (sscanf(bondText, "%f", &bondLength) != 1) {
				//attempt to lookup the value in the map
				char * s = bondText;
				double flip = 1.0;
				if (bondText[0] == '-') {
					s = &(bondText[1]);
					flip = -1.0;
				}
				std::map<std::string, double>::iterator p = valueMap.find(s);
				if (p != valueMap.end()) {
					bondLength = flip * (*p).second;
				} else break;	//missing key value
			}
			if (iline >= 2) {
				if (readCount < 4) break;
				if (sscanf(angleText, "%f", &bondAngle) != 1) {
					//attempt to lookup the value in the map
					char * s = angleText;
					double flip = 1.0;
					if (angleText[0] == '-') {
						s = &(angleText[1]);
						flip = -1.0;
					}
					std::map<std::string, double>::iterator p = valueMap.find(s);
					if (p != valueMap.end()) {
						bondAngle = flip * (*p).second;
					} else break;	//missing key value
				}
				if (iline >= 3) {
					if (readCount < 6) break;
					if (sscanf(dihedralText, "%f", &bondDihedral) != 1) {
						//attempt to lookup the value in the map
						char * s = dihedralText;
						double flip = 1.0;
						if (dihedralText[0] == '-') {
							s = &(dihedralText[1]);
							flip = -1.0;
						}
						std::map<std::string, double>::iterator p = valueMap.find(s);
						if (p != valueMap.end()) {
							bondDihedral = flip * (*p).second;
						} else break;	//missing key value
					}
				}
			}
			if (bondLength < 0.0) break;
			con1--;
			con2--;
			con3--;
			if (con1 >= iline) break;
			mInts->AddInternalCoordinate(iline, con1, 0, bondLength*unitConversion);
			if (iline > 1) {
				if (con2 >= iline) break;
				mInts->AddInternalCoordinate(iline, con2, 1, bondAngle);
				if (iline > 2) {
					if (con3 >= iline) break;
					mInts->AddInternalCoordinate(iline, con3, 2, bondDihedral);
				}
			}
		}
		iline++;
	}
	//if we punted after the AddAtom call delete off the atom without internal coordinate information
	if (iline > cFrame->NumAtoms) cFrame->DeleteAtom(iline-1);
	//Now convert the set of internals into cartesians
	mInts->InternalsToCartesians(this, Prefs, 0);
}
void MoleculeData::ParseGAMESSUKZMatrix(BufferFile * Buffer, WinPrefs * Prefs) {
	//In order to support concatinated sets of coords we will use a local set of internals if a preexisting
	//set is present
	//First determine the potential number of atoms
	char	Line[kMaxLineLength];
	Buffer->GetLine(Line);
	if (FindKeyWord(Line, "ZMAT", 4) < 0) return;	//Not positioned at the start of the zmatrix block
	float	unitConversion = kBohr2AngConversion;	//The default units are AU
	if (FindKeyWord(Line, "ANGS", 4) > 0) unitConversion = 1.0;
	long BlockStart = Buffer->GetFilePos();
	if (! Buffer->LocateKeyWord("END", 3, -1)) return;	//end of block marker not found
	long EndPos = Buffer->GetFilePos();
	Buffer->SetFilePos(BlockStart);
	long LastAtomPos = EndPos;
	if (Buffer->LocateKeyWord("VARIABLES", 9, EndPos)) {
		LastAtomPos = Buffer->GetFilePos();
		Buffer->SetFilePos(BlockStart);
	}
	if (Buffer->LocateKeyWord("CONSTANTS", 9, EndPos)) {
		if (Buffer->GetFilePos() < LastAtomPos)
			LastAtomPos = Buffer->GetFilePos();
		Buffer->SetFilePos(BlockStart);
	}
	long atmCount = Buffer->GetNumLines(LastAtomPos);
	
	bool internalsAreLocal=false;
	if (!IntCoords) {
		IntCoords = new Internals;
	}
	MOPacInternals * mInts = IntCoords->GetMOPacStyle();
	if (!mInts) {
		IntCoords->CreateMOPacInternals(3*atmCount);
		mInts = IntCoords->GetMOPacStyle();
	} else {
		internalsAreLocal = true;
		mInts = new MOPacInternals(3*atmCount);
	}
	//First jump past the atoms and build a map of any string=value pairs (if any)
	std::map<std::string, double> valueMap;
	if (Buffer->LocateKeyWord("VARIABLES", 9, EndPos)) {
		Buffer->SkipnLines(1);
		while (Buffer->GetFilePos() < EndPos) {
			char		token[kMaxLineLength];
			double		value;
			Buffer->GetLine(Line);
			int readCount = sscanf(Line, "%s %lf", token, &value);
			if (readCount!=2) break;
			std::pair<std::string, double> myVal(token, value);
			std::pair<std::map<std::string, double>::iterator, bool> p = valueMap.insert(myVal);
			if (! myVal.second) {	//We have hit a duplicate value
				MessageAlert("Duplicate keys detected in the value list");
			}
		}
		Buffer->SetFilePos(BlockStart);
	}
	if (Buffer->LocateKeyWord("CONSTANTS", 9, EndPos)) {
		Buffer->SkipnLines(1);
		while (Buffer->GetFilePos() < EndPos) {
			char		token[kMaxLineLength];
			double		value;
			Buffer->GetLine(Line);
			int readCount = sscanf(Line, "%s %lf", token, &value);
			if (readCount!=2) break;
			std::pair<std::string, double> myVal(token, value);
			std::pair<std::map<std::string, double>::iterator, bool> p = valueMap.insert(myVal);
			if (! myVal.second) {	//We have hit a duplicate value
				MessageAlert("Duplicate keys detected in the value list");
			}
		}
		Buffer->SetFilePos(BlockStart);
	}
	
	int iline=0;
	while ((Buffer->GetFilePos() < LastAtomPos)&&(iline<atmCount)) {
		CPoint3D	pos = CPoint3D(0.0f, 0.0f, 0.0f);	//This is just a placeholder
		char		token[kMaxLineLength], bondText[kMaxLineLength],
			angleText[kMaxLineLength], dihedralText[kMaxLineLength];
		float		bondLength, bondAngle, bondDihedral;
		long		AtomType;
		int			con1, con2, con3;
		Buffer->GetLine(Line);
		int readCount = sscanf(Line, "%s %d %s %d %s %d %s", token, &con1, bondText, &con2, &angleText, &con3,
							   &dihedralText);
		if (readCount < 1) break;	//failed to parse anything??
		AtomType = SetAtomType((unsigned char *) token);
		if (AtomType < 0) break;
		cFrame->AddAtom(AtomType, pos);
		if (iline > 0) {
			if (readCount < 2) break;
			if (sscanf(bondText, "%f", &bondLength) != 1) {
				//attempt to lookup the value in the map
				char * s = bondText;
				double flip = 1.0;
				if (bondText[0] == '-') {
					s = &(bondText[1]);
					flip = -1.0;
				}
				std::map<std::string, double>::iterator p = valueMap.find(s);
				if (p != valueMap.end()) {
					bondLength = flip * (*p).second;
				} else break;	//missing key value
			}
			if (iline >= 2) {
				if (readCount < 4) break;
				if (sscanf(angleText, "%f", &bondAngle) != 1) {
					//attempt to lookup the value in the map
					char * s = angleText;
					double flip = 1.0;
					if (angleText[0] == '-') {
						s = &(angleText[1]);
						flip = -1.0;
					}
					std::map<std::string, double>::iterator p = valueMap.find(s);
					if (p != valueMap.end()) {
						bondAngle = flip * (*p).second;
					} else break;	//missing key value
				}
				if (iline >= 3) {
					if (readCount < 6) break;
					if (sscanf(dihedralText, "%f", &bondDihedral) != 1) {
						//attempt to lookup the value in the map
						char * s = dihedralText;
						double flip = 1.0;
						if (dihedralText[0] == '-') {
							s = &(dihedralText[1]);
							flip = -1.0;
						}
						std::map<std::string, double>::iterator p = valueMap.find(s);
						if (p != valueMap.end()) {
							bondDihedral = flip * (*p).second;
						} else break;	//missing key value
					}
				}
			}
			if (bondLength < 0.0) break;
			con1--;
			con2--;
			con3--;
			if (con1 >= iline) break;
			mInts->AddInternalCoordinate(iline, con1, 0, bondLength);
			if (iline > 1) {
				if (con2 >= iline) break;
				mInts->AddInternalCoordinate(iline, con2, 1, bondAngle);
				if (iline > 2) {
					if (con3 >= iline) break;
					mInts->AddInternalCoordinate(iline, con3, 2, bondDihedral);
				}
			}
		}
		iline++;
	}
	//if we punted after the AddAtom call delete off the atom without internal coordinate information
	if (iline > cFrame->NumAtoms) cFrame->DeleteAtom(iline-1);
	//Now convert the set of internals into cartesians
	mInts->InternalsToCartesians(this, Prefs, 0);
	if (internalsAreLocal) delete mInts;
	Buffer->SetFilePos(EndPos);
	Buffer->SkipnLines(1);
}
void MoleculeData::ParseMOPACZMatrix(BufferFile * Buffer, const long & nAtoms, WinPrefs * Prefs) {
	if (!IntCoords) {
		IntCoords = new Internals;
	}
	MOPacInternals * mInts = IntCoords->GetMOPacStyle();
	if (!mInts) {
		IntCoords->CreateMOPacInternals(3*nAtoms);
		mInts = IntCoords->GetMOPacStyle();
	}
	float unitConversion = 1.0;
	if (InputOptions && InputOptions->Data->GetUnits()) unitConversion = kBohr2AngConversion;
	int iline=0;
	long EndPos = Buffer->GetFileSize();
	while ((Buffer->GetFilePos() < EndPos)&&(iline<nAtoms)) {
		CPoint3D	pos = CPoint3D(0.0f, 0.0f, 0.0f);	//This is just a placeholder
		char		token[kMaxLineLength], Line[kMaxLineLength];
		float		bondLength, bondAngle, bondDihedral;
		long		AtomType;
		int			j1, j2, j3, con1, con2, con3;
		Buffer->GetLine(Line);
		int readCount = sscanf(Line, "%s %f %d %f %d %f %d %d %d %d", token, &bondLength, &j1, &bondAngle, &j2,
			   &bondDihedral, &j3, &con1, &con2, &con3);
		if (readCount < 1) break;	//failed to parse anything??
		AtomType = SetAtomType((unsigned char *) token);
		cFrame->AddAtom(AtomType, pos);
		if (iline > 0) {
			if (readCount < 2) break;
			if (iline == 1) {	//the second atom will specify only the bond length
				con1 = 1;
			} else {
				if (iline == 2) {	//For the third atom the connectivity is optional
					if ((readCount >= 5)&&(readCount <=7)) {
						con1 = 2;
						con2 = 1;	//The default allows the connections to be assumed
						if (readCount >= 6) {
							con1 = (int) bondDihedral;
							con2 = j3;
						}
					} else break;	//invalid line
				}
			}
			if (bondLength < 0.0) break;
			con1--;
			con2--;
			con3--;
			if (con1 >= iline) break;
			mInts->AddInternalCoordinate(iline, con1, 0, bondLength*unitConversion);
			if (iline > 1) {
				mInts->AddInternalCoordinate(iline, con2, 1, bondAngle);
				if (iline > 2)
					mInts->AddInternalCoordinate(iline, con3, 2, bondDihedral);
			}
		}
		iline++;
	}
		//if we punted after the AddAtom call delete off the atom without internal coordinate information
	if (iline > cFrame->NumAtoms) cFrame->DeleteAtom(iline-1);
	//Now convert the set of internals into cartesians
	mInts->InternalsToCartesians(this, Prefs, 0);
}
void MoleculeData::GetModelCenter(CPoint3D * center) {
	*center = Centroid;
}
void MoleculeData::SetModelCenter(CPoint3D * center) {
	Centroid = *center;
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
void MoleculeData::RotateToPrincipleOrientation(WinPrefs * Prefs, double precision) {
	if (!DeterminePrincipleOrientation(TotalRotation, Centroid, Prefs, precision)) {
		MessageAlert("Unable to determine the proper symmetry adapted rotation. This"
					 " may mean your selected point group is incorrect.");
	}
}
void MoleculeData::DeterminePointGroup(bool * pgFlags, WinPrefs * Prefs, double precision) {
	pgFlags[0] = true;
	for (int i=0; i<kNumSymmetryPointGroups; i++) pgFlags[i] = false;
	//order of the flags
	//c1, cs, ci, c2h-c8h (3-9), c2v-c8v (10-16), c2-c8 (17-23), s2-s8 (24-26),
	//D2d-d8d (27-33), d2h-d8h (34-40), d2-d8 (41-47), Td, Th, T, Oh, O
	int PrincAxisOrder = 1;
	Matrix4D temp;
	CPoint3D ptmp;
	
	GAMESSPointGroup savedpg = GAMESS_C1;
	long savedpgOrder = 1;
	if (InputOptions) {
		savedpg = InputOptions->Data->GetPointGroup();
		savedpgOrder = InputOptions->Data->GetPointGroupOrder();
	} else {
		InputOptions = new InputData;
	}
	//First determine the rotation axis orders and store in Cn
	for (int i=2; i<=8; i++) {
		InputOptions->Data->SetPointGroup(GAMESS_CN);
		InputOptions->Data->SetPointGroupOrder(i);
		if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
			pgFlags[17 - 2 + i] = true;
			PrincAxisOrder = i;
			InputOptions->Data->SetPointGroup(GAMESS_CNH);
			if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
				pgFlags[3 - 2 + i] = true;
			}
			InputOptions->Data->SetPointGroup(GAMESS_CNV);
			if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
				pgFlags[10 - 2 + i] = true;
			}
			InputOptions->Data->SetPointGroup(GAMESS_DN);
			if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
				pgFlags[41 - 2 + i] = true;
				InputOptions->Data->SetPointGroup(GAMESS_DND);
				if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
					pgFlags[27 - 2 + i] = true;
				}
				InputOptions->Data->SetPointGroup(GAMESS_DNH);
				if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
					pgFlags[34 - 2 + i] = true;
				}
			}
			if (i < 5) {
				InputOptions->Data->SetPointGroup(GAMESS_S2N);
				InputOptions->Data->SetPointGroupOrder(i-1);
				if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
					pgFlags[24 - 2 + i] = true;
				}
			}
		}
	}
	if (PrincAxisOrder > 1) {
		InputOptions->Data->SetPointGroup(GAMESS_T);
		if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
			pgFlags[50] = true;
			InputOptions->Data->SetPointGroup(GAMESS_O);
			if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
				pgFlags[52] = true;
				InputOptions->Data->SetPointGroup(GAMESS_OH);
				if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
					pgFlags[51] = true;
				}
			}
			InputOptions->Data->SetPointGroup(GAMESS_TD);
			if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
				pgFlags[48] = true;
			}
			InputOptions->Data->SetPointGroup(GAMESS_TH);
			if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
				pgFlags[49] = true;
			}
		}
	}
	InputOptions->Data->SetPointGroup(GAMESS_CS);
	if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
		pgFlags[1] = true;
	}
	InputOptions->Data->SetPointGroup(GAMESS_CI);
	if (DeterminePrincipleOrientation(temp, ptmp, Prefs, precision)) {
		pgFlags[2] = true;
	}
	//restore the entry point group
	InputOptions->Data->SetPointGroup(savedpg);
	InputOptions->Data->SetPointGroupOrder(savedpgOrder);
}
bool MoleculeData::DeterminePrincipleOrientation(Matrix4D result,
												 CPoint3D& translation,
												 WinPrefs * Prefs,
												 double precision) const {
	//Setup the rotation matrix to present the molecule in the priciple orientation
	
	InitRotationMatrix(result);
	//First compute and offset to the center of mass
	CPoint3D centerOfMass;
	CalculateCenterOfMass(cFrame->Atoms, cFrame->GetNumAtoms(),
						  Prefs->GetAtomMassLoc(), &centerOfMass);
	translation = centerOfMass;
	centerOfMass *= -1.0;
	CPoint3D rotatedCenterOfMass = centerOfMass;
	
	//Compute the moment of interia tensor
	double xx=0.0, xy=0.0, yy=0.0, xz=0.0, yz=0.0, zz=0.0;
	for (int i=0; i<cFrame->GetNumAtoms(); i++) {
		double atmMass = Prefs->GetAtomMass(cFrame->Atoms[i].GetType()-1);
		double xc = cFrame->Atoms[i].Position.x + centerOfMass.x;
		double yc = cFrame->Atoms[i].Position.y + centerOfMass.y;
		double zc = cFrame->Atoms[i].Position.z + centerOfMass.z;
		xx += atmMass*yc*yc + atmMass*zc*zc;
		yy += atmMass*xc*xc + atmMass*zc*zc;
		zz += atmMass*xc*xc + atmMass*yc*yc;
		xy -= atmMass*xc*yc;
		xz -= atmMass*xc*zc;
		yz -= atmMass*yc*zc;
	}
	if ((fabs(xy)>1.0e-8)||(fabs(xz)>1.0e-8)||(fabs(yz)>1.0e-8)) {
		//Diagonalize the moment of interia tensor to yield the
		//rotation into the principle axis.
		double tri[6];
		tri[0] = xx;
		tri[1] = xy;
		tri[2] = yy;
		tri[3] = xz;
		tri[4] = yz;
		tri[5] = zz;
		double rot[9], eig[3];
		SymmetricJacobiDiagonalization(tri, rot, eig, 3, 3);
		for (int ii=0; ii<3; ii++) {
			for (int j=0; j<3; j++) {
				result[ii][j] = rot[3*ii + j];
			}
		}
		float det = DeterminantMatrix(result);
		//The determinate should be positive, multiple by -1 if not
		//I think this is to ensure the rotation is right-handed
		if (det < 0.0) {
			for (int ii=0; ii<3; ii++) {
				result[ii][0] *= -1.0;
			}
		}
		Rotate3DOffset(result,centerOfMass,&rotatedCenterOfMass);
	}

	for (long i=0; i<cFrame->GetNumAtoms(); i++) {
		CPoint3D temp = (cFrame->Atoms[i].Position) + centerOfMass;
		Rotate3DPt(result, temp, &(RotCoords[i]));
	}
	//generate the moments of inertia
	xx = yy = zz = xy = xz = yz = 0.0;
	for (int i=0; i<cFrame->GetNumAtoms(); i++) {
		double atmMass = Prefs->GetAtomMass(cFrame->Atoms[i].GetType()-1);
		double xc = RotCoords[i].x;
		double yc = RotCoords[i].y;
		double zc = RotCoords[i].z;
		xx += atmMass*xc*xc;
		yy += atmMass*yc*yc;
		zz += atmMass*zc*zc;
		xy += atmMass*xc*yc;
		xz += atmMass*xc*zc;
		yz += atmMass*yc*zc;
	}
	if ((xx>0.1)&&(fabs(xx - yy)<1.0e-4)&&(fabs(zz-xx)>1.0e-4)) {
		//The x and y axis do not appear to be unique, Try to rotate an atom onto the x axis
		//loop through and look for atoms not on the z-axis. If there is already an atom on the
		//x or y axis we can skip as well. If the z axis is not unique we skip as well
		bool needRot = true;
		for (int i=0; i<cFrame->GetNumAtoms(); i++) {
			if (fabs(RotCoords[i].x)>1.0e-4) {
				if (fabs(RotCoords[i].y)<1.0e-4) {
					needRot = false;	//Atom already on y-axis
					break;
				}
			} else if (fabs(RotCoords[i].y)>1.0e-4) {
				needRot	= false;	//Atom already on x-axis
				break;
			}
		}
		if (needRot) {
			//ok pick the first atom off of the z-axis
			for (int i=0; i<cFrame->GetNumAtoms(); i++) {
				if ((fabs(RotCoords[i].x)>1.0e-4)||(fabs(RotCoords[i].y)>1.0e-4)) {
					double theta = atan(RotCoords[i].y/RotCoords[i].x);
					double costh = cos(-theta);
					double sinth = sin(-theta);
					Matrix4D rotation;
					InitRotationMatrix(rotation);
					rotation[0][0] = costh;
					rotation[0][1] = sinth;
					rotation[1][0] = -sinth;
					rotation[1][1] = costh;
					Matrix4D temp;
					MultiplyMatrix(result,rotation,temp);
					CopyMatrix(temp,result);
					break;
				}
			}
		}
	}

	//To complete the determination of the principle axis we must apply all of the symmetry
	//operators to each of the permutations of the axis. When we find a set that results in an
	//unchanged set of coordinates we are done.
	GAMESSPointGroup pg = GAMESS_C1;
	long pgOrder = 1;
	if (InputOptions) {
		pg = InputOptions->Data->GetPointGroup();
		pgOrder = InputOptions->Data->GetPointGroupOrder();
	}
	bool success = true;
	//If we are in C1 symmetry we are done.
	if (pg != GAMESS_C1) {
		
		for (long i=0; i<cFrame->GetNumAtoms(); i++) {
			CPoint3D temp = (cFrame->Atoms[i].Position) + centerOfMass;
			Rotate3DPt(result, temp, &(RotCoords[i]));
		}

		SymmetryOps symOps(pg, pgOrder);
		Matrix4D permuteMatrix;
		for (int pass=0; pass<6; pass++) {
			switch (pass) {
				case 0:	//default axis
					InitRotationMatrix(permuteMatrix);
					break;
				case 1: //x, z, y
					permuteMatrix[1][1] = permuteMatrix[2][2] = 0.0;
					permuteMatrix[1][2] = -1.0;
					permuteMatrix[2][1] = 1.0;
					break;
				case 2: //y, x, z
					permuteMatrix[0][0] = permuteMatrix[1][2] = permuteMatrix[2][1] = 0.0;
					permuteMatrix[0][1] = -1.0;
					permuteMatrix[1][0] = permuteMatrix[2][2] = 1.0;
					break;
				case 3: //y, z, x
					permuteMatrix[0][1] = permuteMatrix[2][2] = 0.0;
					permuteMatrix[0][2] = permuteMatrix[2][1] = 1.0;
					break;
				case 4: //z, x, y
					permuteMatrix[0][2] = permuteMatrix[1][0] = permuteMatrix[2][1] = 0.0;
					permuteMatrix[0][2] = permuteMatrix[1][0] = permuteMatrix[2][1] = 1.0;
					break;
				case 5: //z, y, x
					permuteMatrix[1][0] = permuteMatrix[2][1] = 0.0;
					permuteMatrix[2][0] = -1.0;
					permuteMatrix[1][1] = 1.0;
					break;
			}
			success = true;
			for (int iOp=0; iOp<symOps.getOperationCount(); iOp++) {
				bool SymMatch = true;
				for (int atm=0; atm<cFrame->GetNumAtoms(); atm++) {
					CPoint3D test;
					Rotate3DPt(permuteMatrix, RotCoords[atm], &test);
					CPoint3D result;
					symOps.ApplyOperator(test, result, iOp);
					bool match = false;
					for (int testatom=0; testatom<cFrame->GetNumAtoms(); testatom++) {
						if (cFrame->Atoms[atm].GetType() != cFrame->Atoms[testatom].GetType())
							continue;
						CPoint3D testpt;
						Rotate3DPt(permuteMatrix, RotCoords[testatom], &testpt);
						CPoint3D offset = result - testpt;
							//test the difference in position. They should be quite close!
						if (offset.Magnitude() < precision) {
							match = true;
							break;
						}
					}
					if (!match) { //No matching atom so this permutation fails
						SymMatch = false;
						break;
					}
				}
				if (!SymMatch) {
					success = false;
					break;
				}
			}
			if (success) { //we are done
				//combine the permutation with the rotation matrix
				Matrix4D temp;
				MultiplyMatrix(result,permuteMatrix,temp);
				CopyMatrix(temp,result);
				Rotate3DOffset(result,centerOfMass,&rotatedCenterOfMass);
				break;
			}
		}
	}
	return success;
}
void MoleculeData::GenerateSymmetryDependentAtoms(bool do_warnings) {
	// The input coordinates (in the current frame) must contain the symmetry
	// unique atoms in the proper symmetry adapted reference frame. This
	// routine will generate the symmetry dependent atoms.
	GAMESSPointGroup pg = GAMESS_C1;
	long pgOrder = 1;
	if (InputOptions) {
		pg = InputOptions->Data->GetPointGroup();
		pgOrder = InputOptions->Data->GetPointGroupOrder();
	}
	bool conflicts = false;
	bool closeAtoms = false;
	//If we are in C1 symmetry we are done.
	if ((pg != GAMESS_C1)&&(cFrame->GetNumAtoms()>0)) {
		SymmetryOps symOps(pg, pgOrder);
		//loop over the symmetry operations
		for (int atm=0; atm<cFrame->GetNumAtoms(); atm++) {
			for (int iOp=0; iOp<symOps.getOperationCount(); iOp++) {
				if (cFrame->Atoms[atm].IsSymmetryUnique()) {
					//Apply the operator to each atom
					CPoint3D result;
					symOps.ApplyOperator(cFrame->Atoms[atm].Position, result, iOp);
					bool match = false;
					for (int testatom=0; testatom<cFrame->GetNumAtoms(); testatom++) {
						//Now loop over the atoms to see if this generates a new atom
						//or if there are any conflicts
						CPoint3D offset = result - cFrame->Atoms[testatom].Position;
						float dist = offset.Magnitude();
						if (dist < 1.0e-3) {	//this cutoff might need tuning
							//If we have a match the atoms had better be the same type
							if (cFrame->Atoms[atm].GetType() != cFrame->Atoms[testatom].GetType())
								conflicts = true;
							match = true;
						} else if (dist < 0.2) {
							//GAMESS would consider this an error
							closeAtoms = true;
							match = true;
						}
					}
					if (!match) {
						//In order to match the order of atoms that GAMESS generates we need
						//to insert the generated atom before the input one.
						/* NewAtom(cFrame->Atoms[atm].GetType(), result, atm); */
						NewAtom(cFrame->Atoms[atm], true, atm, &result);
						cFrame->Atoms[atm].IsSymmetryUnique(false);
						cFrame->SetAtomSelection(atm, false);
						atm++;
					}
				}
			}
		}
	}

	if (do_warnings) {
		if (conflicts) {
			MessageAlert("Found conflicts during generation of symmetry dependent coordinates. Your starting coordinates are probably incorrect for the chosen symmetry point group.");
		}
		if (closeAtoms) {
			MessageAlert("Atoms closer than 0.2 Angstroms have been removed. Your coordinates may be incorrect for the chosen symmetry point group.");
		}
	}
}

bool MoleculeData::GenerateSymmetryUniqueAtoms(double tolerance) {
	//On input the coordinates should contain the full set of atoms in the
	//correct orientation for the selected symmetry point group. The
	//coordinates will not be changed, just "marked" as symmetry unique or not.
	//If a symmetry operation generates a new atom an error is generated and
	//the operation fails.  GAMESS likes the symmetry unique atom last. So if
	//an atom can be generated by an atom later in the list we consider it
	//symmetry dependent.
	GAMESSPointGroup pg = GAMESS_C1;
	long pgOrder = 1;
	if (InputOptions) {
		pg = InputOptions->Data->GetPointGroup();
		pgOrder = InputOptions->Data->GetPointGroupOrder();
	}
	//Clear the unique flags
	for (int i=0; i<cFrame->GetNumAtoms(); i++) {
		cFrame->Atoms[i].IsSymmetryUnique(false);
	}
	bool conflicts = false;
	bool closeAtoms = false;
	SymmetryOps symOps(pg, pgOrder);
	//loop over the symmetry operations
	for (int atm=0; atm<cFrame->GetNumAtoms(); atm++) {
		bool match = false;
		for (int iOp=0; iOp<symOps.getOperationCount(); iOp++) {
			//Apply the operator to each atom
			CPoint3D result;
			symOps.ApplyOperator(cFrame->Atoms[atm].Position, result, iOp);
			for (int testatom=atm+1; testatom<cFrame->GetNumAtoms(); testatom++) {
				//Now loop over the atoms to see if this generates a new atom
				//or if there are any conflicts
				CPoint3D offset = result - cFrame->Atoms[testatom].Position;
				float dist = offset.Magnitude();
				if (dist < tolerance) {	//this cutoff might need tuning
										//If we have a match the atoms had better be the same type
					if (cFrame->Atoms[atm].GetType() != cFrame->Atoms[testatom].GetType())
						conflicts = true;
					match = true;
					break;
				} else if (dist < 0.2) {
					//GAMESS would consider this an error
					closeAtoms = true;
					match = true;
					break;
				}
			}
			if (match) {
				break;
			}
		}
		if (match == false) cFrame->Atoms[atm].IsSymmetryUnique(true);
	}
//	if (conflicts) {
//		MessageAlert("Found conflicts during generation of symmetry dependent coordinates. Your starting coordinates are probably incorrect for the chosen symmetry point group.");
//	}
//	if (closeAtoms) {
//		MessageAlert("Atoms closer than 0.2 Angstroms have been removed. Your coordinates may be incorrect for the chosen symmetry point group.");
//	}
	return (!conflicts);
}
void MoleculeData::SymmetrizeCoordinates(bool selected_only) {
	//The purpose of this routine is to remove the "slop" in the coordinates such that they 
	//more tightly meet the selected point group.
	GAMESSPointGroup pg = GAMESS_C1;
	long pgOrder = 1;
	if (InputOptions) {
		pg = InputOptions->Data->GetPointGroup();
		pgOrder = InputOptions->Data->GetPointGroupOrder();
	}
	if (pg == GAMESS_C1) return; //there is nothing to be done for C1
	SymmetryOps symOps(pg, pgOrder);
	//My strategy is to loop over the symmetry unique atoms, apply each operator. 
	//If the resulting operation generates an atom "close" to the original atom add it
	//to a vector sum. After all of the operators are applied divid the sum by the number of
	//images. Next pass through the symmetry dependent atoms and adjust their position to
	//match the newly generated coordinates
	for (int atm=0; atm<cFrame->GetNumAtoms(); atm++) {
		if (cFrame->Atoms[atm].IsSymmetryUnique() &&
			(!selected_only || cFrame->GetAtomSelection(atm))) {
			CPoint3D sum;
			int matchcount = 0;
			for (int iOp=0; iOp<symOps.getOperationCount(); iOp++) {
				CPoint3D result;
				//Apply the operator to each atom
				symOps.ApplyOperator(cFrame->Atoms[atm].Position, result, iOp);
				CPoint3D offset = result - cFrame->Atoms[atm].Position;
				float dist = offset.Magnitude();
				if (dist < 0.3) {	//this cutoff might need tuning
					sum += result;
					matchcount++;
				}
			}
			if (matchcount > 1) {
				//update the symmetry unique coordinate
				cFrame->Atoms[atm].Position = sum / (float) matchcount;
				//and now update symmetry dependent atoms
				for (int iOp=0; iOp<symOps.getOperationCount(); iOp++) {
					CPoint3D result;
					//Apply the operator to each atom
					symOps.ApplyOperator(cFrame->Atoms[atm].Position, result, iOp);
					for (int testatom=0; testatom<cFrame->GetNumAtoms(); testatom++) {
						if (testatom == atm || cFrame->GetAtomType(testatom) != cFrame->GetAtomType(atm)) continue;
						//Now loop over the atoms to see if this generates a new atom
						//or if there are any conflicts
						CPoint3D offset = result - cFrame->Atoms[testatom].Position;
						float dist = offset.Magnitude();
						if (dist < 0.3) {	//this cutoff might need tuning
							cFrame->Atoms[testatom].Position = result;
						}
					}
				}
			}
		}
	}
}
void MoleculeData::CreateLLM(long NumPts, WinPrefs * Prefs) {
	Frame *	NewFrame, * NewFrame2;
	
	Frame * lFrame = cFrame;
	Frame * lEndFrame = lFrame->NextFrame;
	//saniety check
	if ((lFrame == NULL)||(lEndFrame == NULL)||(NumPts<1)) return;
	long NumAtoms = lFrame->NumAtoms;
	
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
			NewFrame->Atoms[iatm].Position.x = NewFrame2->Atoms[iatm].Position.x + offset[iatm].x;
			NewFrame->Atoms[iatm].Position.y = NewFrame2->Atoms[iatm].Position.y + offset[iatm].y;
			NewFrame->Atoms[iatm].Position.z = NewFrame2->Atoms[iatm].Position.z + offset[iatm].z;
		}
		NewFrame->NumAtoms = NumAtoms;
		if (Prefs->GetAutoBond()) {
			Progress lProg;
			NewFrame->SetBonds(Prefs, false, &lProg);
		}
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
	//saniety check
	if ((lFrame == NULL)||(lEndFrame == NULL)||(NumPts<1)) return;

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
	Progress * lProg = new Progress;
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
				else if (OffsetValues[3*i+2] < -180.0) OffsetValues[3*i+2] = 360 + OffsetValues[3*i+2];
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
			NewFrame->SetBonds(Prefs, false, lProg);
		NewFrame2 = NewFrame;
	}
	cFrame = lFrame;	//reset the current frame pointer
	CurrentFrame = SavedFrameNum;
	delete [] OffsetValues;
	delete [] StartValues;
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
bool MoleculeData::SurfaceExportPossible(void) {
	return cFrame->SurfaceExportPossible();
}
bool MoleculeData::ModeVisible(void) const {
	return ((cFrame->Vibs)&&GetDrawMode());
}
long MoleculeData::GetNumBonds(void) const {
	return cFrame->GetNumBonds();
}
long MoleculeData::DeleteAtom(long AtomNum, bool allFrames) {
	long offset = AtomNum;
	int fragId = -1;
	if (cFrame->Atoms[AtomNum].IsEffectiveFragment()) fragId = cFrame->Atoms[AtomNum].GetFragmentNumber();
	if (allFrames) {
		Frame * lFrame = Frames;
		while (lFrame) {
			lFrame->DeleteAtom(AtomNum);
			lFrame = lFrame->NextFrame;
		}
	} else
		cFrame->DeleteAtom(AtomNum);
	if (IntCoords) {
		MOPacInternals * mInts = IntCoords->GetMOPacStyle();
		if (mInts) mInts->DeleteAtom(this, AtomNum);
	}
	std::vector<Annotation *>::iterator anno;
	anno = Annotations.begin();
	for (anno = Annotations.begin(); anno != Annotations.end(); ) {
		if ((*anno)->containsAtom(AtomNum)) {
			delete (*anno);
			anno = Annotations.erase(anno);
		} else {
			(*anno)->adjustIds(AtomNum, -1);
			anno++;
		}
	}
	
	if (fragId > 0) { //recurse to delete the other atoms in the same fragment
		for (long i=0; i<cFrame->NumAtoms; i++) {
			if (cFrame->Atoms[i].IsEffectiveFragment() &&
				cFrame->Atoms[i].GetFragmentNumber() == fragId) {
				//turn off fragment bit to prevent multilevel recursion
				cFrame->Atoms[i].IsEffectiveFragment(false);
				DeleteAtom(i, allFrames);
				--i;
			}
		}
		//remove the fragment name from the name list
		std::vector<std::string>::iterator iter = FragmentNames.begin();
		for (int i=1; i<fragId; i++) ++iter;
		FragmentNames.erase(iter);
		offset = 0;	//Have the caller rescan the whole list to be safe.
	}
	if (FMOFragmentIds.size() > AtomNum) FMOFragmentIds.erase(FMOFragmentIds.begin() + AtomNum);

	ResetRotation();
	return offset;
}
void MoleculeData::ReorderAtomList(long index1, long targetindex) {
	//Sanity check the move
	if ((index1 < 0)||(targetindex < 0)||(index1 >= cFrame->NumAtoms)||
		(targetindex >= cFrame->NumAtoms)||(index1 == targetindex)) return;
	//This should really remove basis set, orbitals, etc unless they are also updated
	//Move the atom
	mpAtom temp;
	if (targetindex > index1) {
		temp = cFrame->Atoms[index1];
		for (long i=index1; i<targetindex; i++) {
			cFrame->Atoms[i] = cFrame->Atoms[i+1];
		}
		cFrame->Atoms[targetindex] = temp;
		//update the bonds list
		for (long i=0; i<cFrame->NumBonds; i++) {
			if (cFrame->Bonds[i].Atom1 == index1)
				cFrame->Bonds[i].Atom1 = targetindex;
			else if ((cFrame->Bonds[i].Atom1 > index1)&&(cFrame->Bonds[i].Atom1 <= targetindex))
				cFrame->Bonds[i].Atom1 --;
			if (cFrame->Bonds[i].Atom2 == index1)
				cFrame->Bonds[i].Atom2 = targetindex;
			else if ((cFrame->Bonds[i].Atom2 > index1)&&(cFrame->Bonds[i].Atom2 <= targetindex))
				cFrame->Bonds[i].Atom2 --;
		}
	} else {
		temp = cFrame->Atoms[index1];
		for (long i=index1; i>targetindex; i--) {
			cFrame->Atoms[i] = cFrame->Atoms[i-1];
		}
		cFrame->Atoms[targetindex] = temp;
		//update the bonds list
		for (long i=0; i<cFrame->NumBonds; i++) {
			if (cFrame->Bonds[i].Atom1 == index1)
				cFrame->Bonds[i].Atom1 = targetindex;
			else if ((cFrame->Bonds[i].Atom1 < index1)&&(cFrame->Bonds[i].Atom1 >= targetindex))
				cFrame->Bonds[i].Atom1 ++;
			if (cFrame->Bonds[i].Atom2 == index1)
				cFrame->Bonds[i].Atom2 = targetindex;
			else if ((cFrame->Bonds[i].Atom2 < index1)&&(cFrame->Bonds[i].Atom2 >= targetindex))
				cFrame->Bonds[i].Atom2 ++;
		}
	}
	//update the internals list
	if (IntCoords) {
		IntCoords->ChangeAtomIndex(this, index1, targetindex);
	}
}
bool MoleculeData::ValidAtom(long AtomNum) {
	return ((AtomNum>=0)&&(AtomNum<cFrame->NumAtoms));
}
AtomTypeList * MoleculeData::GetAtomTypes(void) {return cFrame->GetAtomTypes();}
long MoleculeData::GetNumBasisFunctions(void) const {
	return ((Basis!=NULL)?Basis->NumFuncs : 0);
}
void MoleculeData::DeleteAllAnnotations(void) {
	std::vector<Annotation *>::const_iterator anno;
	anno = Annotations.begin();
	for (anno = Annotations.begin(); anno != Annotations.end(); ) {
		delete (*anno);
		anno++;
	}
	Annotations.clear();
}

const char * MoleculeData::GetFragmentName(unsigned long index) const {
	if (index < FragmentNames.size())
		return FragmentNames[index].c_str();
	else
		return "H2ORHF";
}

void MoleculeData::PruneUnusedFragments() {

	bool found_instance;
	std::map<std::string, EFrag>::iterator frag;
	std::vector<std::string>::const_iterator fragname;

	for (frag = efrags.begin(); frag != efrags.end(); /* BLANK */) {
		found_instance = false;
		for (fragname = FragmentNames.begin();
			 !found_instance && fragname != FragmentNames.end();
			 ++fragname) {
			if (frag->first.compare(*fragname) == 0) {
				found_instance = true;
			}
		}
		if (!found_instance) {
			efrags.erase(frag++);
		} else {
			++frag;
		}
	}

}

long MoleculeData::CreateFMOFragmentation(const int & NumMolecules, std::vector<long> & newFragmentation) {
	//Create an FMO fragment list by breaking up the system into non-bonded pieces (H-bonds are ignored)
	
	long result = 0;
	//Start by initializing the vector with invalid fragment numbers
	newFragmentation.clear();
	newFragmentation.reserve(cFrame->GetNumAtoms());
	for (long i=0; i<cFrame->GetNumAtoms(); i++) {
		newFragmentation.push_back(-1);
	}
	
	long NextId = 1;
	long atomNum = 0;
	while (atomNum < cFrame->GetNumAtoms()) {
		if (newFragmentation[atomNum] < 1) {
			newFragmentation[atomNum] = NextId;
			//starting a new fragment
			//a recursive function would be simpler to write and should normally be fine, but if somebody runs this
			//on a system with a large molecule that could be rather abusive on the stack
			//Scan through the bonds list and check both ends of the bond for the active fragment Id. If one
			//atom is tagged then the other needs to be. The process is done when a further pass through the list 
			//does not produce any more tags. This seems a bit brute force and is horrible on memory accesses, but
			//it is probably fast enough for the very limited number of times it be executed.
			bool done=false;
			while (!done) {
				done = true;
				for (long ib=0; ib<cFrame->GetNumBonds(); ++ib) {
					if (newFragmentation[cFrame->Bonds[ib].Atom1] == NextId) {
						if (newFragmentation[cFrame->Bonds[ib].Atom2] != NextId) {
							newFragmentation[cFrame->Bonds[ib].Atom2] = NextId;
							done = false;
						}
					} else if (newFragmentation[cFrame->Bonds[ib].Atom2] == NextId) {
						if (newFragmentation[cFrame->Bonds[ib].Atom1] != NextId) {
							newFragmentation[cFrame->Bonds[ib].Atom1] = NextId;
							done = false;
						}
					}
				}
			}
			++NextId;
		}
		++atomNum;
	}
	result = NextId - 1;
	//Ok we should now have the non-bonded pieces separated. If the user wants more than one non-bonded piece in
	//each fragment coalesce them now.
	if (NumMolecules > 1) {
		--NextId;	//this leaves NextId containing the starting number of fragments
		//I think it is probably best to combine fragments that are closest to each other.
		//It's probably best to consider the center of each fragment for the distance computation.
		std::vector<CPoint3D> fragCenters;
		fragCenters.reserve(NextId);
		for (long ifrag=0; ifrag<NextId; ++ifrag) {
			fragCenters.push_back(CPoint3D(0,0,0));
			long atmcount;
			for (long iatom=0; iatom<cFrame->GetNumAtoms(); ++iatom) {
				if (newFragmentation[iatom] == (ifrag+1)) {
					fragCenters[ifrag].x += cFrame->Atoms[iatom].Position.x;
					fragCenters[ifrag].y += cFrame->Atoms[iatom].Position.y;
					fragCenters[ifrag].z += cFrame->Atoms[iatom].Position.z;
					atmcount++;
				}
			}
			fragCenters[ifrag].x /= (float) atmcount;
			fragCenters[ifrag].y /= (float) atmcount;
			fragCenters[ifrag].z /= (float) atmcount;
		}
		
		for (long ifrag=0; ifrag<NextId; ifrag++) {
			for (int i=1; i<NumMolecules; i++) {
				if ((ifrag+1) == NextId) break;	//we've ran out of fragments, the last one will have to be short
				CPoint3D offset;
				offset.x = fragCenters[ifrag].x-fragCenters[ifrag+1].x;
				offset.y = fragCenters[ifrag].y-fragCenters[ifrag+1].y;
				offset.z = fragCenters[ifrag].z-fragCenters[ifrag+1].z;
				float distance = offset.x * offset.x + offset.y * offset.y + offset.z * offset.z;
				
				long targetId = ifrag+1;
				for (long jfrag=ifrag+2; jfrag<NextId; jfrag++) {
					offset.x = fragCenters[ifrag].x-fragCenters[jfrag].x;
					offset.y = fragCenters[ifrag].y-fragCenters[jfrag].y;
					offset.z = fragCenters[ifrag].z-fragCenters[jfrag].z;
					float testDistance = offset.x * offset.x + offset.y * offset.y + offset.z * offset.z;
					if (testDistance < distance) {
						distance = testDistance;
						targetId = jfrag;
					}
				}
				//Since we are removing a fragment we need to pull down the Ids higher up
				//one thing I'm not doing is recomputing the fragment center so the next addition will 
				//reference the original fragment only
				for (long atomi=0; atomi<cFrame->GetNumAtoms(); ++atomi) {
					if (newFragmentation[atomi] == (targetId+1)) newFragmentation[atomi] = (ifrag+1);
					if (newFragmentation[atomi] > (targetId+1)) newFragmentation[atomi]--;
				}
				fragCenters.erase(fragCenters.begin() + targetId);
				NextId--;
			}
		}
		result = NextId;
	}
	return result;
}
long MoleculeData::GetFMOFragmentId(const long & AtomId) const {
	if ((AtomId >= 0) && (AtomId < FMOFragmentIds.size())) {
		return FMOFragmentIds[AtomId];
	}
	return -1;
}
void MoleculeData::SetFMOFragmentId(const long & AtomId, const long & FragId) {
	if ((AtomId<0)||(AtomId>=cFrame->GetNumAtoms())) return;	//invalid atom
	if ((FragId<1)||(FragId > InputOptions->FMO.GetNumberFragments())) return; //invalid fragment id
	//Is the fragment id array setup?
	while (FMOFragmentIds.size() < cFrame->GetNumAtoms()) {
		FMOFragmentIds.push_back(1);	//simply initialize atoms to fragment 1
	}
	FMOFragmentIds[AtomId] = FragId;
}

void MoleculeData::ExportPOV(BufferFile *Buffer, WinPrefs *Prefs) {

	Frame *lFrame = GetCurrentFramePtr();
	mpAtom *lAtoms = lFrame->Atoms;

	long NumAtoms = lFrame->NumAtoms;
	float AtomScale = Prefs->GetAtomScale();
	long curAtomType;
	RGBColor * AtomColor;
	float red, green, blue;
	char tmpStr[500];

	Buffer->PutText("#include \"transforms.inc\"\n\n");

	sprintf(tmpStr,
			"camera {\n"
			"\tlocation <0, 0, 0>\n"
			"\tsky <0, 1, 0>\n"
			"\tlook_at <0, 0, -1>\n"
			"}\n\n");
	Buffer->PutText(tmpStr);

	sprintf(tmpStr,
			"light_source {\n"
			"\t<6, 6, 12>, rgb <1, 1, 1>\n"
			"}\n\n");
	Buffer->PutText(tmpStr);

	sprintf(tmpStr,
			"light_source {\n"
			"\t<-6, 6, 12>, rgb <1, 1, 1>\n"
			"}\n\n");
	Buffer->PutText(tmpStr);

	sprintf(tmpStr,
			"background {\n"
			"\trgb <1, 1, 1>\n"
			"}\n\n");
	Buffer->PutText(tmpStr);

	Buffer->PutText("#declare AtomBondFinish = finish {specular 0.95 roughness 0.005}\n");
	Buffer->PutText("#declare SurfaceFinish = finish {specular 0.95 roughness 0.001}\n\n");

	Buffer->PutText("union {\n");
	for (long iatom = 0; iatom < NumAtoms; iatom++) {
		if (lAtoms[iatom].GetInvisibility()) continue;

		curAtomType = lAtoms[iatom].GetType() - 1;
		AtomColor = Prefs->GetAtomColorLoc(curAtomType);
		red = AtomColor->red / 65536.0;
		green = AtomColor->green / 65536.0;
		blue = AtomColor->blue / 65536.0;

		float radius = AtomScale*Prefs->GetAtomSize(curAtomType);
		Buffer->PutText("sphere {\n");
		sprintf(tmpStr, "\t<%f, %f, %f>, %f\n",
				lAtoms[iatom].Position.x,
				lAtoms[iatom].Position.y,
				lAtoms[iatom].Position.z, radius);
		Buffer->PutText(tmpStr);
		sprintf(tmpStr, "\ttexture {\n"
				"\t\tpigment {color rgb <%f, %f, %f>}\n"
				"\t\tfinish {AtomBondFinish}\n"
				"\t}\n", red, green, blue);
		Buffer->PutText(tmpStr);
		Buffer->PutText("}\n\n");
	}

	Bond *lBonds = lFrame->Bonds;
	long NumBonds = lFrame->NumBonds;
	double BondSize = Prefs->GetQD3DBondWidth();
	for (long ibond = 0; ibond < NumBonds; ibond++) {
		const mpAtom& atom1 = lAtoms[lBonds[ibond].Atom1];
		const mpAtom& atom2 = lAtoms[lBonds[ibond].Atom2];

		CPoint3D offset = atom2.Position - atom1.Position;
		double length = offset.Magnitude();

		double radius1 = AtomScale * Prefs->GetAtomSize(atom1.GetType() - 1);
		double radius2 = AtomScale * Prefs->GetAtomSize(atom2.GetType() - 1);
		double percent1 = radius1 / length;
		double percent2 = radius2 / length;
		double centerPercent = 0.5 + 0.5*(percent1-percent2);
		CPoint3D halfway = atom1.Position + offset * centerPercent;

		AtomColor = Prefs->GetAtomColorLoc(atom1.GetType() - 1);
		red = AtomColor->red / 65536.0;
		green = AtomColor->green / 65536.0;
		blue = AtomColor->blue / 65536.0;

		Buffer->PutText("cylinder {\n");
		sprintf(tmpStr, "\t<%f, %f, %f>, <%f, %f, %f>, %f\n",
				atom1.Position.x, atom1.Position.y, atom1.Position.z,
				halfway.x, halfway.y, halfway.z, BondSize);
		Buffer->PutText(tmpStr);
		sprintf(tmpStr,
				"\ttexture {\n"
				"\t\tpigment {color rgb <%f, %f, %f>}\n"
				"\t\tfinish {AtomBondFinish}\n"
				"\t}\n",
				red, green, blue);
		Buffer->PutText(tmpStr);
		Buffer->PutText("}\n\n");

		AtomColor = Prefs->GetAtomColorLoc(atom2.GetType() - 1);
		red = AtomColor->red / 65536.0;
		green = AtomColor->green / 65536.0;
		blue = AtomColor->blue / 65536.0;

		Buffer->PutText("cylinder {\n");
		sprintf(tmpStr, "\t<%f, %f, %f>, <%f, %f, %f> %f\n",
				atom2.Position.x, atom2.Position.y, atom2.Position.z,
				halfway.x, halfway.y, halfway.z, BondSize);
		Buffer->PutText(tmpStr);
		sprintf(tmpStr, "\ttexture {\n"
				"\t\tpigment {color rgb <%f, %f, %f>}\n"
				"\t\tfinish {AtomBondFinish}\n"
				"\t}\n", red, green, blue);
		Buffer->PutText(tmpStr);
		Buffer->PutText("}\n\n");
	}

	// Export any surfaces.
	Surface *lSurface = lFrame->SurfaceList;
	while (lSurface) {
		Buffer->PutText("// ");
		Buffer->PutText(lSurface->GetLabel());
		Buffer->PutText("\n");
		lSurface->ExportPOV(this, Prefs, Buffer);
		lSurface = lSurface->GetNextSurface();
	}

	// Now, transform scene to mimic current rotation and translation.
	float *m = (float *) TotalRotation;
	sprintf(tmpStr,
			"\n\tmatrix <%f, %f, %f,"
			"\n\t        %f, %f, %f,"
			"\n\t        %f, %f, %f,"
			"\n\t        %f, %f, %f>\n",
			m[0], m[1], m[2], m[4], m[5], m[6],
			m[8], m[9], m[10], m[12], m[13], m[14]);
	Buffer->PutText(tmpStr);
	Buffer->PutText("\n\tscale <-1, 1, 1>\n");
	sprintf(tmpStr, "\n\ttranslate <0, 0, %f>\n", -WindowSize);
	Buffer->PutText(tmpStr);
	Buffer->PutText("}\n\n");

	if (Prefs->ShowAtomicSymbolLabels()) {
		CPoint3D text_pos;
		wxString atomic_symbol;
		unsigned int i;

		for (i = 0; i < kMaxAtomTypes; i++) {
			Prefs->GetAtomLabel(i, atomic_symbol);
			sprintf(tmpStr,
					"#declare Atom_%03d = "
					"   text {"
					"      ttf \"timrom.ttf\", \"%s\", 0.01, 0"
					"   }\n\n",
					i, (const char *) atomic_symbol.mb_str(wxConvUTF8));
			Buffer->PutText(tmpStr);
		}

		Buffer->PutText("union {\n");
		float radius;
		for (long iatom = 0; iatom < NumAtoms; iatom++) {
			if (lAtoms[iatom].GetInvisibility()) continue;

			Rotate3DPt(TotalRotation, lAtoms[iatom].Position, &text_pos);
			curAtomType = lAtoms[iatom].GetType() - 1;
			radius = AtomScale * Prefs->GetAtomSize(curAtomType);

			AtomColor = Prefs->GetAtomColorLoc(curAtomType);
			red = AtomColor->red / 65536.0;
			green = AtomColor->green / 65536.0;
			blue = AtomColor->blue / 65536.0;

			sprintf(tmpStr,
					"object {\n"
					"   Atom_%03d\n"
					"   Center_Trans(Atom_%03d, x + y)\n"
					"   scale <0.25, 0.25, 1.0>\n"
					"   translate <%f, %f, %f>\n"
					"   no_shadow\n"
					"   pigment { color rgb <%f, %f, %f> }\n"
					"}\n\n", curAtomType, curAtomType,
					text_pos.x, text_pos.y, text_pos.z + radius,
					1.0f - red, 1.0f - green, 1.0f - blue);
			Buffer->PutText(tmpStr);
		}
		Buffer->PutText("\n\tscale <-1, 1, 1>\n");
		sprintf(tmpStr, "\n\ttranslate <0, 0, %f>\n", -WindowSize);
		Buffer->PutText(tmpStr);
		Buffer->PutText("}\n");
	}

}
