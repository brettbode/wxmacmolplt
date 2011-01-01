/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

#include <iostream>
#include "Globals.h"
#include "Progress.h"
#include "MyTypes.h"
#include "Frame.h"
#include "MoleculeData.h"
#include "BasisSet.h"
#include "GlobalExceptions.h"
#include "SurfaceTypes.h"
//#include "ChipmanSurface.h"
#include "Math3D.h"
#include "VirtualSphere.h"
#include "InputData.h"
#include "Prefs.h"
#include <string.h>
#include <stdio.h>
#include <sstream>
#include "CML.h"
#include "XML.hpp"
#include "MMPPrefs.h"

void OrthogonalizeMatrix(Matrix4D a);

//Applies a rotation of the specified number of degrees to the specified axis.
//Axis=(0 for x, 1 for y, 2 for z)
void ApplyRotation(Matrix4D RotMat, long Axis, float AngleDegrees) {
	CPoint3D	InitialTrans;
//First back rotate the translation to get the inital translation
	InitialTrans.x = RotMat[3][0];
	InitialTrans.y = RotMat[3][1];
	InitialTrans.z = RotMat[3][2];
//Now zero out the translation part of the matrix
	RotMat[3][0] = RotMat[3][1] = RotMat[3][2] = 0.0f;
//Now calculate the rotation
	float AngleRads = AngleDegrees * kPi/180.0;
	float cosAngle = cos(AngleRads);
	float sinAngle = sin(AngleRads);
	Matrix4D	TempRot, TempCopyMat;
	for (long ii=0; ii<4; ii++) {
		TempRot[ii][ii] = 1.0f;
		for (long jj=0; jj<ii; jj++) {
			TempRot[ii][jj] = 0.0f;
			TempRot[jj][ii] = 0.0f;
		}
	}
	switch (Axis) {
		case 0:	//rotate about the x-axis
			TempRot[1][1] = cosAngle;
			TempRot[2][2] = cosAngle;
			TempRot[1][2] = -sinAngle;
			TempRot[2][1] = sinAngle;
		break;
		case 1:	//rotate about the y-axis
			TempRot[0][0] = cosAngle;
			TempRot[2][2] = cosAngle;
			TempRot[0][2] = sinAngle;
			TempRot[2][0] = -sinAngle;
		break;
		case 2:	//rotate about the z-axis
			TempRot[0][0] = cosAngle;
			TempRot[1][1] = cosAngle;
			TempRot[0][1] = -sinAngle;
			TempRot[1][0] = sinAngle;
		break;
	}
	MultiplyMatrix (TempRot, RotMat, TempCopyMat);
	CopyMatrix (TempCopyMat, RotMat);
//Now rotate the translation to the new orientation
	RotMat[3][0] = InitialTrans.x;
	RotMat[3][1] = InitialTrans.y;
	RotMat[3][2] = InitialTrans.z;
}	/*Apply Rotation*/
void CalculateCenterOfMass(mpAtom * AtomList, long NumAtoms, float * AtomMasses, CPoint3D * Center) {
	float TotalMass=0.0f;
	Center->x = Center->y = Center->z = 0.0f;
	for (long i=0; i<NumAtoms; i++) {
		float mass = AtomMasses[AtomList[i].GetType() - 1];
		mass *= mass;	//Masses are stored as the square root
		TotalMass += mass;
		Center->x += mass * AtomList[i].Position.x;
		Center->y += mass * AtomList[i].Position.y;
		Center->z += mass * AtomList[i].Position.z;
	}
	Center->x /= TotalMass;
	Center->y /= TotalMass;
	Center->z /= TotalMass;
}
void MinimizeDifferences(mpAtom * FixedAtoms, mpAtom * targetAtoms, long NumAtoms,
		WinPrefs * Prefs, long NumOptAtoms) {
	Matrix4D	FitMatrix, TempRotMat;
	float		SquaresValue, NewSquareValue, RotAngle;
	long		iOptAtoms, i;
	CPoint3D *	RotCoords = new CPoint3D[NumAtoms];
	if (!RotCoords) return;

	InitRotationMatrix(FitMatrix);		//zero out the fit matrix to start with...
		//Move the center of mass of the target to the same point as the fixed set
	
		CPoint3D	Center1, Center2;
	CalculateCenterOfMass(FixedAtoms, NumOptAtoms, Prefs->GetAtomMassLoc(), &Center1);
	CalculateCenterOfMass(targetAtoms, NumOptAtoms, Prefs->GetAtomMassLoc(), &Center2);
	Center1.x -= Center2.x;
	Center1.y -= Center2.y;
	Center1.z -= Center2.z;	//Apply the center of mass translation
	for (i=0; i<NumAtoms; i++) {
		targetAtoms[i].Position.x += Center1.x;
		targetAtoms[i].Position.y += Center1.y;
		targetAtoms[i].Position.z += Center1.z;
	}
	Boolean	Done;
	for (long ipass=0; ipass<4; ipass++) {
		if (ipass<3) {
			RotAngle = 10.0;
			if (ipass == 0) iOptAtoms = MIN(2, NumAtoms);
			else if (ipass == 1) iOptAtoms = MIN(3, NumAtoms);
			else iOptAtoms = NumOptAtoms;
			for (i=0; i<iOptAtoms; i++)
				Rotate3DPt(FitMatrix, targetAtoms[i].Position, &(RotCoords[i]));
			SquaresValue = CalculateSquaresValue(iOptAtoms, FixedAtoms, RotCoords);
		}
		Done = false;
		while (!Done) {
			Done = true;
			Boolean RotDone = false;
			for (long jpass=0; jpass<2; jpass++) {
				while (!RotDone) {
					RotDone = true;
					CopyMatrix (FitMatrix, TempRotMat);
					for (long ii=0; ii<3; ii++) {
						ApplyRotation(TempRotMat, ii, RotAngle);
						for (i=0; i<iOptAtoms; i++)
							Rotate3DPt(TempRotMat, targetAtoms[i].Position, &(RotCoords[i]));
						NewSquareValue = CalculateSquaresValue(iOptAtoms, FixedAtoms, RotCoords);
						if (NewSquareValue<SquaresValue) {
							RotDone = Done = false;
							SquaresValue = NewSquareValue;
							CopyMatrix (TempRotMat, FitMatrix);
						} else {
							ApplyRotation(TempRotMat, ii, -2.0*RotAngle);
							for (i=0; i<iOptAtoms; i++)
								Rotate3DPt(TempRotMat, targetAtoms[i].Position, &(RotCoords[i]));
							NewSquareValue = CalculateSquaresValue(iOptAtoms, FixedAtoms, RotCoords);
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
				RotAngle *= 0.1f;
			}
		}
	}
//Done with this set of frames. Copy the RotCoords over on to the FileCoords array to make them permenant.

	for (i=0; i<NumAtoms; i++)
		Rotate3DPt(FitMatrix, targetAtoms[i].Position, &(RotCoords[i]));
	for (i=0; i<NumAtoms; i++)
		targetAtoms[i].Position = RotCoords[i];
	delete [] RotCoords;
}	/*MinimizeDifferences*/

SymmetryOps::SymmetryOps(GAMESSPointGroup pg, short pgOrder) {
	//setup a series of 3x3 transformation matrices that will map source 
	//points to any symmetry dependent copy. Note that in general this results
	//in more matrices than there are symmetry operators
	Matrix4D work;

	//All point groups have an identity operator so toss one on here.
	InitRotationMatrix(work);
	AddMatrix(work);
	if (pg == GAMESS_C1) return;	//That's it for C1
	
	double pi2 = 2.0*acos(-1.0);

	//Start with the rotation around the primary axis (everything except C1, CI, and CS)
	if ((pg >= GAMESS_CNH)&&(pg<=GAMESS_DN) && (pgOrder > 0)) {
		for (int i=1; i<pgOrder; i++) {
			double alpha = i*pi2/((double) pgOrder);
			double cosa = cos(alpha);
			double sina = sin(alpha);
			operations.push_back(cosa);
			operations.push_back(-sina);
			operations.push_back(0.0);
			operations.push_back(sina);
			operations.push_back(cosa);
			operations.push_back(0.0);
			operations.push_back(0.0);
			operations.push_back(0.0);
			operations.push_back(1.0);
		}
		if (pg == GAMESS_CN) return; //Done with Cn
		if ((pg == GAMESS_CNH) || (pg == GAMESS_DNH)) {
			//Add the sigma-H plane (z=0) by repeating the rotations
			//and inverting the z coordinate
			int count = operations.size()/9;
			for (int i=0; i<count; i++) {
				operations.push_back(operations[9*i]);
				operations.push_back(operations[9*i+1]);
				operations.push_back(operations[9*i+2]);
				operations.push_back(operations[9*i+3]);
				operations.push_back(operations[9*i+4]);
				operations.push_back(operations[9*i+5]);
				operations.push_back(operations[9*i+6]);
				operations.push_back(operations[9*i+7]);
				operations.push_back(-operations[9*i+8]);
			}
			if (pg == GAMESS_CNH) return; //Done with CNH
		}
		if ((pg >= GAMESS_DND) && (pg <= GAMESS_DN)) {
			//Dn, DnH and DnD have a two fold axis as the x axis
			int count = operations.size()/9;
			for (int i=0; i<count; i++) {
				operations.push_back(operations[9*i]);
				operations.push_back(-operations[9*i+1]);
				operations.push_back(-operations[9*i+2]);
				operations.push_back(operations[9*i+3]);
				operations.push_back(-operations[9*i+4]);
				operations.push_back(-operations[9*i+5]);
				operations.push_back(operations[9*i+6]);
				operations.push_back(-operations[9*i+7]);
				operations.push_back(-operations[9*i+8]);
			}
			if (pg == GAMESS_DND) {
				//DnD has a sigma-D plane with equation
				//sin(2*pi/4*pgorder)*x-cos(2*pi/4*n)*y=0
				double beta = pi2/(2.0*((double) pgOrder));
				double cosb = cos(beta);
				double sinb = sin(beta);
				//replicate all the preceding operations with an x/y rotation
				int count = operations.size()/9;
				for (int i=0; i<count; i++) {
					operations.push_back(cosb*operations[9*i]+sinb*operations[9*i+1]);
					operations.push_back(-(-sinb*operations[9*i]+cosb*operations[9*i+1]));
					operations.push_back(operations[9*i+2]);
					operations.push_back(cosb*operations[9*i+3]+sinb*operations[9*i+4]);
					operations.push_back(-(-sinb*operations[9*i+3]+cosb*operations[9*i+4]));
					operations.push_back(operations[9*i+5]);
					operations.push_back(cosb*operations[9*i+6]+sinb*operations[9*i+7]);
					operations.push_back(-(-sinb*operations[9*i+6]+cosb*operations[9*i+7]));
					operations.push_back(operations[9*i+8]);
				}
			}
			//Done with Dn, DnH, and DnD
			return;
		}
		if (pg == GAMESS_CNV) {
			//sigma V plane is XZ plane
			int count = operations.size()/9;
			for (int i=0; i<count; i++) {
				operations.push_back(operations[9*i]);
				operations.push_back(-operations[9*i+1]);
				operations.push_back(operations[9*i+2]);
				operations.push_back(operations[9*i+3]);
				operations.push_back(-operations[9*i+4]);
				operations.push_back(operations[9*i+5]);
				operations.push_back(operations[9*i+6]);
				operations.push_back(-operations[9*i+7]);
				operations.push_back(operations[9*i+8]);
			}
			//Done with CNV
			return;
		}
	}
	if (pg == GAMESS_CS) {
		//X-Y plane
		InitRotationMatrix(work);
		work[2][2] = -1.0f;
		AddMatrix(work);
	} else if (pg == GAMESS_CI) {
		InitRotationMatrix(work);
		work[0][0] = work[1][1] = work[2][2] = -1.0f;
		AddMatrix(work);
	} else if (pg == GAMESS_S2N) {
		//The X-Y plane is the plane for the improper rotation
		double beta = pi2/(2.0*((double) pgOrder));
		double cosb = cos(beta);
		double sinb = sin(beta);
		int count = operations.size()/9;
		for (int i=0; i<count; i++) {
			operations.push_back(cosb*operations[9*i]+sinb*operations[9*i+1]);
			operations.push_back(-sinb*operations[9*i]+cosb*operations[9*i+1]);
			operations.push_back(-operations[9*i+2]);
			operations.push_back(cosb*operations[9*i+3]+sinb*operations[9*i+4]);
			operations.push_back(-sinb*operations[9*i+3]+cosb*operations[9*i+4]);
			operations.push_back(-operations[9*i+5]);
			operations.push_back(cosb*operations[9*i+6]+sinb*operations[9*i+7]);
			operations.push_back(-sinb*operations[9*i+6]+cosb*operations[9*i+7]);
			operations.push_back(-operations[9*i+8]);
		}
	} else { //T and O related groups
		//Here we are taking care of the common subgroup T
		InitRotationMatrix(work);
		work[1][1] = work[2][2] = -1.0f;
		AddMatrix(work);
		work[1][1] = 1.0f;
		work[0][0] = work[2][2] = -1.0f;
		AddMatrix(work);
		work[0][0] = work[1][1] = -1.0f;
		work[2][2] = 1.0f;
		AddMatrix(work);
		for (int i=0; i<8; i++) {
			operations.push_back(operations[9*i+6]);
			operations.push_back(operations[9*i+7]);
			operations.push_back(operations[9*i+8]);
			operations.push_back(operations[9*i]);
			operations.push_back(operations[9*i+1]);
			operations.push_back(operations[9*i+2]);
			operations.push_back(operations[9*i+3]);
			operations.push_back(operations[9*i+4]);
			operations.push_back(operations[9*i+5]);
		}
		//Done with T continue on for others
		if (pg == GAMESS_TH) {
			//TH is finished off with a direct product with CI
			int count = operations.size();
			for (int i=0; i<count; i++) {
				operations.push_back( - operations[i]);
			}
		} else if ((pg == GAMESS_TD)||(pg == GAMESS_OH)||(pg == GAMESS_O)) {
			//Octahedral groups are a direct product of T and a C4 rotation
			//TD group is direct product of T and a reflection plane (X=Y)
			double sign = -1.0;
			if (pg == GAMESS_TD) sign = 1.0;
			for (int i=0; i<12; i++) {
				operations.push_back(operations[9*i+3]*sign);
				operations.push_back(operations[9*i+4]*sign);
				operations.push_back(operations[9*i+5]*sign);
				operations.push_back(operations[9*i]);
				operations.push_back(operations[9*i+1]);
				operations.push_back(operations[9*i+2]);
				operations.push_back(operations[9*i+6]);
				operations.push_back(operations[9*i+7]);
				operations.push_back(operations[9*i+8]);
			}
			if (pg == GAMESS_OH) {
				//OH is a direct product of O and CI
				int count = operations.size();
				for (int i=0; i<count; i++) {
					operations.push_back( - operations[i]);
				}
			}
		}
	}
}
void SymmetryOps::AddMatrix(const Matrix4D work) {
	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			operations.push_back(work[i][j]);
		}
	}
}
void SymmetryOps::ApplyOperator(const CPoint3D & source, CPoint3D & dest, long theOp) const {
	if ((theOp >= 0)&&(theOp < (operations.size()/9))) {
		long base = theOp*9;
		dest.x = ((source.x)*operations[base] +
				( source.y)*operations[base+3] +
				( source.z)*operations[base+6]);
		dest.y = ((source.x)*operations[base+1] +
				( source.y)*operations[base+4] +
				( source.z)*operations[base+7]);
		dest.z = ((source.x)*operations[base+2] +
				( source.y)*operations[base+5] +
				( source.z)*operations[base+8]);
	}
}

VibRec::VibRec(const long & NumVibs, const long & NumAtoms) {
	init();
	Setup(NumVibs, NumAtoms);
}
VibRec::VibRec(void) {
	init();
}
VibRec::~VibRec(void) {
}
void VibRec::init(void) {
	NumModes = 0;
	CurrentMode = 0;
}
void VibRec::Setup(const long & NumVibs, const long & NumAtoms) {
	if (NumVibs>0) {
		NormMode.reserve(NumAtoms*NumVibs);
		Intensities.reserve(NumVibs);
		Frequencies.reserve(NumVibs);
		if (NumVibs == 1)	//Read in the single mode string
			Frequencies.push_back(std::string("single mode"));
		//init the intensities all to 1
		for (long i=0; i<NumVibs; i++) Intensities.push_back(1.0);
		CPoint3D temp;
		temp.x = temp.y = temp.z = 0.0f;
		for (long i=0; i<(NumAtoms*NumVibs); i++) {
			NormMode.push_back(temp);
		}
		NumModes = NumVibs;
	}
}
bool VibRec::Resize(long NumAtoms) {	//reduce the allocation to the current # of modes
	NormMode.resize(NumModes*NumAtoms);
	Frequencies.resize(NumModes);
	Intensities.resize(NumModes);
	return true;
}
float VibRec::GetIntensity(long Mode) const {
	float result = 1.0f;	//default to 1.0
	if ((Mode>=0)&&(Mode<Intensities.size())) result = Intensities[Mode];
	return result;
}
float VibRec::GetRamanIntensity(long Mode) const {
	float result = 0.0f;
	if ((Mode>=0)&&(Mode<RamanIntensity.size())) result = RamanIntensity[Mode];
	return result;
}
float VibRec::GetFrequency(long Mode) const {
	float result = 0.0f;
	if ((Mode >= 0) && (Mode < Frequencies.size())) {
		sscanf(Frequencies[Mode].c_str(), "%f", &result);
		if (((Frequencies[Mode])[Frequencies[Mode].length()-1] == 'i') ||
			((Frequencies[Mode])[Frequencies[Mode].length()-1] == 'I'))
			result *= -1.0f;
	}
	return result;
}

bool Orb2DSurface::Needs2DPlane(void) const {
	bool result = false;
	if (Visible && (PlotOrb>=0)) result = true;
	return result;
}

void Orb2DSurface::Update(MoleculeData * MainData) {
	if (Visible && (PlotOrb>=0)) {
		Progress * lProgress = new Progress;
		if (lProgress) {
			lProgress->ChangeText("Calculating MO Grid...");
			if (GetRotate2DMap()) SetPlaneToScreenPlane(MainData);
			CalculateMOGrid(MainData, lProgress);
			delete lProgress;
		}
	}
}
void Orb2DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kOrb2DType));
	
	Surf2DBase::Write2DXML(sElem, false);
	OrbSurfBase::WriteXML(sElem);
}
Orb2DSurface::Orb2DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_2DSurface:
					Surf2DBase::Read2DXML(child);
					break;
				case MMP_OrbSurfBase:
					OrbSurfBase::ReadXML(child);
					break;
			}
		}
		child = child->getNextChild();
	}
}
void Orb3DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kOrb3DType));
	
	Surf3DBase::Write3DXML(sElem, false);
	OrbSurfBase::WriteXML(sElem);
}
Orb3DSurface::Orb3DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_3DSurface:
					Surf3DBase::Read3DXML(child);
					break;
				case MMP_OrbSurfBase:
					OrbSurfBase::ReadXML(child);
					break;
			}
		}
		child = child->getNextChild();
	}
}
void General2DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kGeneral2DSurface));
	
	Surf2DBase::Write2DXML(sElem, true);
}
General2DSurface::General2DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_2DSurface:
					Surf2DBase::Read2DXML(child);
					break;
			}
		}
		child = child->getNextChild();
	}
}
void General3DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kGeneral3DSurface));
	
	Surf3DBase::Write3DXML(sElem, true);
}

General3DSurface::General3DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_3DSurface:
					Surf3DBase::Read3DXML(child);
					break;
			}
		}
		child = child->getNextChild();
	}
}

void TEDensity1DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kTotalDensity1D));
	
	Surf1DBase::Write1DXML(sElem, false);
	if (OrbSet >= 0) {
		std::ostringstream b;
		b << OrbSet;
		sElem->addChildElement(CML_convert(MMP_OrbSurfTargetSet), b.str().c_str());
	}
}

TEDensity1DSurface::TEDensity1DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_1DSurface:
					Surf1DBase::Read1DXML(child);
					break;
				case MMP_OrbSurfTargetSet:
				{
					long tl;
					if (child->getLongValue(tl))
						OrbSet = tl;
				}
			}
		}
		child = child->getNextChild();
	}
}

void TEDensity2DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kTotalDensity2D));
	
	Surf2DBase::Write2DXML(sElem, false);
	if (OrbSet >= 0) {
		std::ostringstream b;
		b << OrbSet;
		sElem->addChildElement(CML_convert(MMP_OrbSurfTargetSet), b.str().c_str());
	}
}

TEDensity2DSurface::TEDensity2DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_2DSurface:
					Surf2DBase::Read2DXML(child);
					break;
				case MMP_OrbSurfTargetSet:
				{
					long tl;
					if (child->getLongValue(tl))
						OrbSet = tl;
				}
			}
		}
		child = child->getNextChild();
	}
}

void TEDensity3DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kTotalDensity3D));
	
	Surf3DBase::Write3DXML(sElem, false);
	{
		std::ostringstream b;
		b << MaxMEPValue;
		sElem->addChildElement(CML_convert(MMP_SurfMaxMEPValue), b.str().c_str());
	}
	if (OrbSet >= 0) {
		std::ostringstream b;
		b << OrbSet;
		sElem->addChildElement(CML_convert(MMP_OrbSurfTargetSet), b.str().c_str());
	}
}
TEDensity3DSurface::TEDensity3DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_3DSurface:
					Surf3DBase::Read3DXML(child);
					break;
				case MMP_SurfMaxMEPValue:
				{
					double tf;
					if (child->getDoubleValue(tf))
						MaxMEPValue = tf;
				}
				case MMP_OrbSurfTargetSet:
				{
					long tl;
					if (child->getLongValue(tl))
						OrbSet = tl;
				}
					break;
			}
		}
		child = child->getNextChild();
	}
}
void MEP2DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kMEP2D));
	
	Surf2DBase::Write2DXML(sElem, true);
	if (OrbSet >= 0) {
		std::ostringstream b;
		b << OrbSet;
		sElem->addChildElement(CML_convert(MMP_OrbSurfTargetSet), b.str().c_str());
	}
}
MEP2DSurface::MEP2DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_2DSurface:
					Surf2DBase::Read2DXML(child);
					break;
				case MMP_OrbSurfTargetSet:
				{
					long tl;
					if (child->getLongValue(tl))
						OrbSet = tl;
				}
			}
		}
		child = child->getNextChild();
	}
}
void MEP3DSurface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_SurfaceDescription));
	sElem->addAttribute(CML_convert(dataTypeAttr), SurfaceTypeToText(kMEP3D));
	
	Surf3DBase::Write3DXML(sElem, true);
	if (OrbSet >= 0) {
		std::ostringstream b;
		b << OrbSet;
		sElem->addChildElement(CML_convert(MMP_OrbSurfTargetSet), b.str().c_str());
	}
}
MEP3DSurface::MEP3DSurface(XMLElement * sxml) {
	XMLElement * child = sxml->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_3DSurface:
					Surf3DBase::Read3DXML(child);
					break;
				case MMP_OrbSurfTargetSet:
				{
					long tl;
					if (child->getLongValue(tl))
						OrbSet = tl;
				}
					break;
			}
		}
		child = child->getNextChild();
	}
}

void Surf1DBase::Write1DXML(XMLElement *parent, bool writeGrid) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_1DSurface));
	
	Surface::WriteXML(sElem);
	
	char line[kMaxLineLength];

	snprintf(line, kMaxLineLength, "%f", GridMax);
	sElem->addChildElement(CML_convert(MMP_SurfGridMax), line);
	snprintf(line, kMaxLineLength, "%f", GridMin);
	sElem->addChildElement(CML_convert(MMP_SurfGridMin), line);
	snprintf(line, kMaxLineLength, "%ld", NumGridPoints);
	sElem->addChildElement(CML_convert(MMP_SurfNumGridPoints), line);
	snprintf(line, kMaxLineLength, "%f", MaxContourValue);
	sElem->addChildElement(CML_convert(MMP_SurfMaxContourValue), line);

	XMLElement * color = sElem->addChildElement(CML_convert(MMP_SurfPosColor));
	snprintf(line, kMaxLineLength, "%f", (PosColor.red/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), line);
	snprintf(line, kMaxLineLength, "%f", (PosColor.green/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), line);
	snprintf(line, kMaxLineLength, "%f", (PosColor.blue/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), line);

	color = sElem->addChildElement(CML_convert(MMP_SurfNegColor));
	snprintf(line, kMaxLineLength, "%f", (NegColor.red/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), line);
	snprintf(line, kMaxLineLength, "%f", (NegColor.green/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), line);
	snprintf(line, kMaxLineLength, "%f", (NegColor.blue/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), line);

	sElem->addChildElement(CML_convert(MMP_SurfPosNegContours), (ContourBothPosNeg()?trueXML:falseXML));

	XMLElement *endpoint = sElem->addChildElement(CML_convert(MMP_SurfEndpoint1));
	snprintf(line, kMaxLineLength, "%f", Start.x);
	endpoint->addAttribute(CML_convert(X3Attr), line);
	snprintf(line, kMaxLineLength, "%f", Start.y);
	endpoint->addAttribute(CML_convert(Y3Attr), line);
	snprintf(line, kMaxLineLength, "%f", Start.z);
	endpoint->addAttribute(CML_convert(Z3Attr), line);

	endpoint = sElem->addChildElement(CML_convert(MMP_SurfEndpoint2));
	snprintf(line, kMaxLineLength, "%f", End.x);
	endpoint->addAttribute(CML_convert(X3Attr), line);
	snprintf(line, kMaxLineLength, "%f", End.y);
	endpoint->addAttribute(CML_convert(Y3Attr), line);
	snprintf(line, kMaxLineLength, "%f", End.z);
	endpoint->addAttribute(CML_convert(Z3Attr), line);
	
	if (writeGrid && Grid) {
		std::ostringstream buf;
		long len = NumGridPoints*NumGridPoints;
		for (int i=0; i<len; i++)
			buf << Grid[i] << " ";
		sElem->addChildElement(CML_convert(MMP_SurfGrid), buf.str().c_str());
	}
}

void Surf1DBase::Read1DXML(XMLElement *parent) {
	XMLElement * child = parent->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			float tf;
			double td;
			long tl;
			switch (type) {
				case MMP_BaseSurface:
					Surface::ReadXML(child);
					break;
				case MMP_SurfGridMax:
					if (child->getDoubleValue(td))
						GridMax = td;
					break;
				case MMP_SurfGridMin:
					if (child->getDoubleValue(td))
						GridMin = td;
					break;
				case MMP_SurfNumGridPoints:
					if (child->getLongValue(tl))
						NumGridPoints = tl;
					break;
				case MMP_SurfMaxContourValue:
					if (child->getDoubleValue(td))
						MaxContourValue = td;
					break;
				case MMP_SurfPosColor:
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), tf))
						PosColor.red = (unsigned short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), tf))
						PosColor.green = (unsigned short)(tf*65535.0);
						if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), tf))
							PosColor.blue = (unsigned short)(tf*65535.0);
							break;
				case MMP_SurfNegColor:
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), tf))
						NegColor.red = (unsigned short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), tf))
						NegColor.green = (unsigned short)(tf*65535.0);
						if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), tf))
							NegColor.blue = (unsigned short)(tf*65535.0);
							break;
				case MMP_SurfEndpoint1:
					if (child->getAttributeValue(CML_convert(X3Attr), tf))
						Start.x = tf;
					if (child->getAttributeValue(CML_convert(Y3Attr), tf))
						Start.y = tf;
					if (child->getAttributeValue(CML_convert(Z3Attr), tf))
						Start.z = tf;
				case MMP_SurfEndpoint2:
					if (child->getAttributeValue(CML_convert(X3Attr), tf))
						End.x = tf;
					if (child->getAttributeValue(CML_convert(Y3Attr), tf))
						End.y = tf;
					if (child->getAttributeValue(CML_convert(Z3Attr), tf))
						End.z = tf;
				case MMP_SurfGrid:
					if (NumGridPoints > 0) {
						long count = NumGridPoints*NumGridPoints;
						AllocateGrid(count);
						long test = child->getFloatArray(count, Grid);
						if (test != count) FreeGrid();
					}
					break;
			}
		}
		child = child->getNextChild();
	}
}

void Surf2DBase::Write2DXML(XMLElement * parent, bool writeGrid) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_2DSurface));
	
	Surface::WriteXML(sElem);
	
	char line[kMaxLineLength];
	XMLElement * orig = sElem->addChildElement(CML_convert(MMP_SurfOrigin));
	snprintf(line, kMaxLineLength, "%f", Origin.x);
	orig->addAttribute(CML_convert(X3Attr), line);
	snprintf(line, kMaxLineLength, "%f", Origin.y);
	orig->addAttribute(CML_convert(Y3Attr), line);
	snprintf(line, kMaxLineLength, "%f", Origin.z);
	orig->addAttribute(CML_convert(Z3Attr), line);

	XMLElement * xi = sElem->addChildElement(CML_convert(MMP_SurfXInc));
	snprintf(line, kMaxLineLength, "%f", XInc.x);
	xi->addAttribute(CML_convert(X3Attr), line);
	snprintf(line, kMaxLineLength, "%f", XInc.y);
	xi->addAttribute(CML_convert(Y3Attr), line);
	snprintf(line, kMaxLineLength, "%f", XInc.z);
	xi->addAttribute(CML_convert(Z3Attr), line);

	XMLElement * yi = sElem->addChildElement(CML_convert(MMP_SurfYInc));
	snprintf(line, kMaxLineLength, "%f", YInc.x);
	yi->addAttribute(CML_convert(X3Attr), line);
	snprintf(line, kMaxLineLength, "%f", YInc.y);
	yi->addAttribute(CML_convert(Y3Attr), line);
	snprintf(line, kMaxLineLength, "%f", YInc.z);
	yi->addAttribute(CML_convert(Z3Attr), line);
		
	snprintf(line, kMaxLineLength, "%f", GridMax);
	sElem->addChildElement(CML_convert(MMP_SurfGridMax), line);
	snprintf(line, kMaxLineLength, "%f", GridMin);
	sElem->addChildElement(CML_convert(MMP_SurfGridMin), line);
	snprintf(line, kMaxLineLength, "%ld", NumGridPoints);
	sElem->addChildElement(CML_convert(MMP_SurfNumGridPoints), line);
	snprintf(line, kMaxLineLength, "%ld", NumContours);
	sElem->addChildElement(CML_convert(MMP_SurfNumContours), line);
	snprintf(line, kMaxLineLength, "%f", MaxContourValue);
	sElem->addChildElement(CML_convert(MMP_SurfMaxContourValue), line);

	XMLElement * color = sElem->addChildElement(CML_convert(MMP_SurfPosColor));
	snprintf(line, kMaxLineLength, "%f", (PosColor.red/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), line);
	snprintf(line, kMaxLineLength, "%f", (PosColor.green/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), line);
	snprintf(line, kMaxLineLength, "%f", (PosColor.blue/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), line);

	color = sElem->addChildElement(CML_convert(MMP_SurfNegColor));
	snprintf(line, kMaxLineLength, "%f", (NegColor.red/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), line);
	snprintf(line, kMaxLineLength, "%f", (NegColor.green/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), line);
	snprintf(line, kMaxLineLength, "%f", (NegColor.blue/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), line);
	
	sElem->addChildElement(CML_convert(MMP_SurfShowZeroContour), (GetShowZeroContour()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfScreenPlane), (GetRotate2DMap()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfDashedContours), (GetDashLine()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfPosNegContours), (ContourBothPosNeg()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_DisplayPlane), (ShowPlottingPlane()?trueXML:falseXML));
	
	if (writeGrid && Grid) {
		std::ostringstream buf;
		long len = NumGridPoints*NumGridPoints;
		for (int i=0; i<len; i++)
			buf << Grid[i] << " ";
		sElem->addChildElement(CML_convert(MMP_SurfGrid), buf.str().c_str());
	}
}
void Surf2DBase::Read2DXML(XMLElement * parent) {
	XMLElement * child = parent->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			float tf;
			double td;
			long tl;
			bool tb;
			switch (type) {
				case MMP_BaseSurface:
					Surface::ReadXML(child);
					break;
				case MMP_SurfOrigin:
					if (child->getAttributeValue(CML_convert(X3Attr), tf))
						Origin.x = tf;
					if (child->getAttributeValue(CML_convert(Y3Attr), tf))
						Origin.y = tf;
					if (child->getAttributeValue(CML_convert(Z3Attr), tf))
						Origin.z = tf;
					break;
				case MMP_SurfXInc:
					if (child->getAttributeValue(CML_convert(X3Attr), tf))
						XInc.x = tf;
					if (child->getAttributeValue(CML_convert(Y3Attr), tf))
						XInc.y = tf;
					if (child->getAttributeValue(CML_convert(Z3Attr), tf))
						XInc.z = tf;
					break;
				case MMP_SurfYInc:
					if (child->getAttributeValue(CML_convert(X3Attr), tf))
						YInc.x = tf;
					if (child->getAttributeValue(CML_convert(Y3Attr), tf))
						YInc.y = tf;
					if (child->getAttributeValue(CML_convert(Z3Attr), tf))
						YInc.z = tf;
					break;
				case MMP_SurfGridMax:
					if (child->getDoubleValue(td))
						GridMax = td;
					break;
				case MMP_SurfGridMin:
					if (child->getDoubleValue(td))
						GridMin = td;
					break;
				case MMP_SurfNumGridPoints:
					if (child->getLongValue(tl))
						NumGridPoints = tl;
					break;
				case MMP_SurfNumContours:
					if (child->getLongValue(tl))
						NumContours = tl;
					break;
				case MMP_SurfMaxContourValue:
					if (child->getDoubleValue(td))
						MaxContourValue = td;
					break;
				case MMP_SurfPosColor:
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), tf))
						PosColor.red = (unsigned short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), tf))
						PosColor.green = (unsigned short)(tf*65535.0);
						if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), tf))
							PosColor.blue = (unsigned short)(tf*65535.0);
							break;
				case MMP_SurfNegColor:
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), tf))
						NegColor.red = (unsigned short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), tf))
						NegColor.green = (unsigned short)(tf*65535.0);
						if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), tf))
							NegColor.blue = (unsigned short)(tf*65535.0);
							break;
				case MMP_SurfShowZeroContour:
					if (child->getBoolValue(tb))
						SetShowZeroContour(tb);
					break;
				case MMP_SurfScreenPlane:
					if (child->getBoolValue(tb))
						SetRotate2DMap(tb);
					break;
				case MMP_SurfDashedContours:
					if (child->getBoolValue(tb))
						SetDashLine(tb);
					break;
				case MMP_SurfContourPosNeg:
					if (child->getBoolValue(tb))
						SetContourBothPosNeg(tb);
					break;
				case MMP_DisplayPlane:
					if (child->getBoolValue(tb))
						ShowPlottingPlane(tb);
					break;
				case MMP_SurfGrid:
					if (NumGridPoints > 0) {
						long count = NumGridPoints*NumGridPoints;
						AllocateGrid(count);
						long test = child->getFloatArray(count, Grid);
						if (test != count) FreeGrid();
					}
					break;
			}
		}
		child = child->getNextChild();
	}
}

void Surf3DBase::Write3DXML(XMLElement * parent, bool writeGrid) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_3DSurface));
	
	Surface::WriteXML(sElem);
	
	char line[kMaxLineLength];
	XMLElement * orig = sElem->addChildElement(CML_convert(MMP_SurfOrigin));
	snprintf(line, kMaxLineLength, "%f", Origin.x);
	orig->addAttribute(CML_convert(X3Attr), line);
	snprintf(line, kMaxLineLength, "%f", Origin.y);
	orig->addAttribute(CML_convert(Y3Attr), line);
	snprintf(line, kMaxLineLength, "%f", Origin.z);
	orig->addAttribute(CML_convert(Z3Attr), line);
	
	snprintf(line, kMaxLineLength, "%f", GridMax);
	sElem->addChildElement(CML_convert(MMP_SurfGridMax), line);
	snprintf(line, kMaxLineLength, "%f", GridMin);
	sElem->addChildElement(CML_convert(MMP_SurfGridMin), line);
	snprintf(line, kMaxLineLength, "%f", XGridInc);
	sElem->addChildElement(CML_convert(MMP_SurfXInc), line);
	snprintf(line, kMaxLineLength, "%f", YGridInc);
	sElem->addChildElement(CML_convert(MMP_SurfYInc), line);
	snprintf(line, kMaxLineLength, "%f", ZGridInc);
	sElem->addChildElement(CML_convert(MMP_SurfZInc), line);

	snprintf(line, kMaxLineLength, "%ld", NumXGridPoints);
	sElem->addChildElement(CML_convert(MMP_SurfNumXGridPoints), line);
	snprintf(line, kMaxLineLength, "%ld", NumYGridPoints);
	sElem->addChildElement(CML_convert(MMP_SurfNumYGridPoints), line);
	snprintf(line, kMaxLineLength, "%ld", NumZGridPoints);
	sElem->addChildElement(CML_convert(MMP_SurfNumZGridPoints), line);
	snprintf(line, kMaxLineLength, "%ld", NumGridPoints);
	sElem->addChildElement(CML_convert(MMP_SurfNumGridPoints), line);

	XMLElement * color = sElem->addChildElement(CML_convert(MMP_SurfPosColor));
	snprintf(line, kMaxLineLength, "%f", (PosColor.red/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), line);
	snprintf(line, kMaxLineLength, "%f", (PosColor.green/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), line);
	snprintf(line, kMaxLineLength, "%f", (PosColor.blue/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), line);
	
	color = sElem->addChildElement(CML_convert(MMP_SurfNegColor));
	snprintf(line, kMaxLineLength, "%f", (NegColor.red/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), line);
	snprintf(line, kMaxLineLength, "%f", (NegColor.green/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), line);
	snprintf(line, kMaxLineLength, "%f", (NegColor.blue/65535.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), line);

	color = sElem->addChildElement(CML_convert(MMP_SurfTransColor));
	snprintf(line, kMaxLineLength, "%f", (Transparency/100.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorRed), line);
	snprintf(line, kMaxLineLength, "%f", (Transparency/100.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorGreen), line);
	snprintf(line, kMaxLineLength, "%f", (Transparency/100.0));
	color->addAttribute(MMPPref_convert(MMPPref_ColorBlue), line);

	snprintf(line, kMaxLineLength, "%f", ContourValue);
	sElem->addChildElement(CML_convert(MMP_SurfContourValue), line);
	snprintf(line, kMaxLineLength, "%f", GridSize);
	sElem->addChildElement(CML_convert(MMP_SurfGridSize), line);

	sElem->addChildElement(CML_convert(MMP_SurfSolidSurface), (SolidSurface()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfColorizeSurface), (ColorByValue()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfUseSurfaceNormals), (UseSurfaceNormals()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfUseRGBColor), (UseRGBColoration()?trueXML:falseXML));
	if (InvertRGBColoration()) sElem->addChildElement(CML_convert(MMP_SurfInvertRGBColor), 
													  (InvertRGBColoration()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfFixedGrid), (GetFixGrid()?trueXML:falseXML));
	sElem->addChildElement(CML_convert(MMP_SurfContourPosNeg), (ContourBothPosNeg()?trueXML:falseXML));

	if (writeGrid && Grid) {
		std::ostringstream buf;
		long len = NumGridPoints*NumGridPoints*NumGridPoints;
		for (int i=0; i<len; i++)
			buf << Grid[i] << " ";
		sElem->addChildElement(CML_convert(MMP_SurfGrid), buf.str().c_str());
	}
	if (ContourHndl) {
		snprintf(line, kMaxLineLength, "%ld", NumVertices);
		sElem->addChildElement(CML_convert(MMP_SurfNumVertices), line);
		snprintf(line, kMaxLineLength, "%ld", NumPosContourTriangles);
		sElem->addChildElement(CML_convert(MMP_SurfNumPosTris), line);
		snprintf(line, kMaxLineLength, "%ld", NumNegContourTriangles);
		sElem->addChildElement(CML_convert(MMP_SurfNumNegTris), line);
		std::ostringstream buf, vbuf;
		long length = 3*(NumPosContourTriangles + NumNegContourTriangles);
		for (int i=0; i<NumVertices; i++)
			buf << ContourHndl[i].x << " " << ContourHndl[i].y << " "
				<< ContourHndl[i].z << " ";
		for (int j=0; j<length; j++)
			vbuf << VertexList[j] << " ";
		sElem->addChildElement(CML_convert(MMP_SurfVertices), buf.str().c_str());
		sElem->addChildElement(CML_convert(MMP_SurfVerticesList), vbuf.str().c_str());
		if (SurfaceNormals) {
			std::ostringstream sbuf;
			 for (int i=0; i<NumVertices; i++)
				 sbuf << SurfaceNormals[i].x << " " << SurfaceNormals[i].y << " "
					 << SurfaceNormals[i].z << " ";
			 sElem->addChildElement(CML_convert(MMP_SurfNormals), sbuf.str().c_str());
		}
		if (List) {
			std::ostringstream lbuf;
			for (int i=0; i<NumVertices; i++)
				lbuf << List[i] << " ";
			sElem->addChildElement(CML_convert(MMP_SurfSurfaceValues), lbuf.str().c_str());
		}
	}
}
void Surf3DBase::Read3DXML(XMLElement * parent) {
	XMLElement * child = parent->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			float tf;
			double td;
			long tl;
			bool tb;
			switch (type) {
				case MMP_BaseSurface:
					Surface::ReadXML(child);
					break;
				case MMP_SurfOrigin:
					if (child->getAttributeValue(CML_convert(X3Attr), tf))
						Origin.x = tf;
					if (child->getAttributeValue(CML_convert(Y3Attr), tf))
						Origin.y = tf;
					if (child->getAttributeValue(CML_convert(Z3Attr), tf))
						Origin.z = tf;
					break;
				case MMP_SurfGridMax:
					if (child->getDoubleValue(td))
						GridMax = td;
					break;
				case MMP_SurfGridMin:
					if (child->getDoubleValue(td))
						GridMin = td;
					break;
				case MMP_SurfXInc:
					if (child->getDoubleValue(td))
						XGridInc = td;
					break;
				case MMP_SurfYInc:
					if (child->getDoubleValue(td))
						YGridInc = td;
					break;
				case MMP_SurfZInc:
					if (child->getDoubleValue(td))
						ZGridInc = td;
					break;
				case MMP_SurfNumXGridPoints:
					if (child->getLongValue(tl))
						NumXGridPoints = tl;
					break;
				case MMP_SurfNumYGridPoints:
					if (child->getLongValue(tl))
						NumYGridPoints = tl;
					break;
				case MMP_SurfNumZGridPoints:
					if (child->getLongValue(tl))
						NumZGridPoints = tl;
					break;
				case MMP_SurfNumGridPoints:
					if (child->getLongValue(tl))
						NumGridPoints = tl;
					break;
				case MMP_SurfPosColor:
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), tf))
						PosColor.red = (unsigned short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), tf))
						PosColor.green = (unsigned short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), tf))
						PosColor.blue = (unsigned short)(tf*65535.0);
					break;
				case MMP_SurfNegColor:
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), tf))
						NegColor.red = (unsigned short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorGreen), tf))
						NegColor.green = (unsigned short)(tf*65535.0);
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorBlue), tf))
						NegColor.blue = (unsigned short)(tf*65535.0);
					break;
				case MMP_SurfTransColor:
					if (child->getAttributeValue(MMPPref_convert(MMPPref_ColorRed), tf)) {
						Transparency = (int) (tf * 100.0f);
					}
					break;
				case MMP_SurfContourValue:
					if (child->getDoubleValue(td))
						ContourValue = td;
					break;
				case MMP_SurfGridSize:
					if (child->getDoubleValue(td))
						GridSize = td;
					break;
				case MMP_SurfSolidSurface:
					if (child->getBoolValue(tb))
						SolidSurface(tb);
					break;
				case MMP_SurfColorizeSurface:
					if (child->getBoolValue(tb))
						SetColorByValue(tb);
					break;
				case MMP_SurfUseSurfaceNormals:
					if (child->getBoolValue(tb))
						UseSurfaceNormals(tb);
					break;
				case MMP_SurfUseRGBColor:
					if (child->getBoolValue(tb))
						UseRGBColoration(tb);
					break;
				case MMP_SurfInvertRGBColor:
					if (child->getBoolValue(tb))
						InvertRGBColoration(tb);
					break;
				case MMP_SurfFixedGrid:
					if (child->getBoolValue(tb))
						SetFixGrid(tb);
					break;
				case MMP_SurfContourPosNeg:
					if (child->getBoolValue(tb))
						SetContourBothPosNeg(tb);
					break;
				case MMP_SurfGrid:
					if (NumGridPoints > 0) {
						long count = NumGridPoints*NumGridPoints*NumGridPoints;
						AllocateGrid(count);
						long test = child->getFloatArray(count, Grid);
						if (test != count) FreeGrid();
					}
					break;
				case MMP_SurfNumVertices:
					if (child->getLongValue(tl))
						NumVertices = tl;
					break;
				case MMP_SurfNumPosTris:
					if (child->getLongValue(tl))
						NumPosContourTriangles = tl;
					break;
				case MMP_SurfNumNegTris:
					if (child->getLongValue(tl))
						NumNegContourTriangles = tl;
					break;
				case MMP_SurfVertices:
					if (NumVertices > 0) {
						AllocateContour(NumVertices, NumPosContourTriangles+NumNegContourTriangles);
						long test = child->getFloatArray(3*NumVertices, (float *) ContourHndl);
						if (test != (3*NumVertices)) FreeContour();
					}
					break;
				case MMP_SurfVerticesList:
					if (VertexAllocation > 0) {
						long length = 3*(NumPosContourTriangles + NumNegContourTriangles);
						long test = child->getLongArray(length, VertexList);
						if (test != length) FreeContour();
					}
					break;
				case MMP_SurfNormals:
					if (NumVertices > 0) {
						AllocateNormals(NumVertices);
						long test = child->getFloatArray(NumVertices*3, (float *) SurfaceNormals);
						if (test != 3*NumVertices) FreeNormals();
					}
					break;
				case MMP_SurfSurfaceValues:
					if (NumVertices > 0) {
						AllocateList(NumVertices);
						long test = child->getFloatArray(NumVertices, List);
						if (test != NumVertices) FreeList();
					}
					break;
			}
		}
		child = child->getNextChild();
	}
}
void OrbSurfBase::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_OrbSurfBase));

	if (PlotOrb >= 0) {
		std::ostringstream b;
		b << PlotOrb;
		sElem->addChildElement(CML_convert(MMP_OrbSurfPlotOrb), b.str().c_str());
	}
	if (TargetSet >= 0) {
		std::ostringstream b;
		b << TargetSet;
		sElem->addChildElement(CML_convert(MMP_OrbSurfTargetSet), b.str().c_str());
	}
	if (UseAOs()) sElem->addChildElement(CML_convert(MMP_OrbSurfUseAOs), trueXML);
	if (UseBetaSet()) sElem->addChildElement(CML_convert(MMP_OrbSurfUseBetaOrbs), trueXML);
	if (GetPhaseChange()) sElem->addChildElement(CML_convert(MMP_OrbSurfPhaseChage), trueXML);
	if (UseSphericalHarmonics()) sElem->addChildElement(CML_convert(MMP_OrbSurfSpherical), trueXML);
	if (GetOrbOccDisplay()) sElem->addChildElement(CML_convert(MMP_OrbSurfOccNums), trueXML);
}
void OrbSurfBase::ReadXML(XMLElement * parent) {
	XMLElement * child = parent->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			long tl;
			bool tb;
			switch (type) {
				case MMP_OrbSurfPlotOrb:
					if (child->getLongValue(tl))
						PlotOrb = tl;
					break;
				case MMP_OrbSurfTargetSet:
					if (child->getLongValue(tl))
						TargetSet = tl;
					break;
				case MMP_OrbSurfUseAOs:
					if (child->getBoolValue(tb))
						UseAOs(tb);
					break;
				case MMP_OrbSurfUseBetaOrbs:
					if (child->getBoolValue(tb))
						UseBetaSet(tb);
					break;
				case MMP_OrbSurfPhaseChage:
					if (child->getBoolValue(tb))
						SetPhaseChange(tb);
					break;
				case MMP_OrbSurfSpherical:
					if (child->getBoolValue(tb))
						UseSphericalHarmonics(tb);
					break;
				case MMP_OrbSurfOccNums:
					if (child->getBoolValue(tb))
						SetOrbOccDisplay(tb);
					break;
			}
		}
		child = child->getNextChild();
	}
}
void Surface::WriteXML(XMLElement * parent) const {
	XMLElement * sElem = parent->addChildElement(CML_convert(MMP_BaseSurface));
	//id, label, visible
	sElem->addAttribute(CML_convert(MMP_VisibleAttr), (Visible?trueXML:falseXML));
	std::ostringstream b;
	b << ID;
	sElem->addChildElement(CML_convert(MMP_SurfaceId), b.str().c_str());
	if (Label) sElem->addChildElement(CML_convert(MMP_SurfaceLabel), Label);
}
bool Surface::ReadXML(XMLElement * parent) {
	bool vis;
	if (parent->getAttributeValue(CML_convert(MMP_VisibleAttr), vis))
		Visible = vis;
	XMLElement * child = parent->getFirstChild();
	while (child) {
		MMP_SurfaceNameSpace type;
		if (CML_convert(child->getName(), type)) {
			switch (type) {
				case MMP_SurfaceId:
				{
					long temp;
					if (child->getLongValue(temp))
						ID = temp;
				}
					break;
				case MMP_SurfaceLabel:
				{
					const char * c = child->getValue();
					if (c) SetLabel(c);
				}
					break;
			}
		}
		child = child->getNextChild();
	}
	return true;
}
Surface * Surface::ReadSurface(XMLElement * parent) {
	Surface * result = NULL;
	const char * surftype = parent->getAttributeValue(CML_convert(dataTypeAttr));
	try {
		if (surftype) {
			SurfaceType t;
			if (TextToSurfaceType(surftype, t)) {
				switch (t) {
					case kOrb3DType:
						result = new Orb3DSurface(parent);
						break;
					case kOrb2DType:
						result = new Orb2DSurface(parent);
						break;
					case kTotalDensity3D:
						result = new TEDensity3DSurface(parent);
						break;
					case kTotalDensity2D:
						result = new TEDensity2DSurface(parent);
						break;
					case kTotalDensity1D:
						result = new TEDensity1DSurface(parent);
						break;
					case kMEP2D:
						result = new MEP2DSurface(parent);
						break;
					case kMEP3D:
						result = new MEP3DSurface(parent);
						break;
					case kSolventType3D:
						break;
					case kGeneral3DSurface:
						result = new General3DSurface(parent);
						break;
					case kGeneral2DSurface:
						result = new General2DSurface(parent);
						break;
				}
			}
		}
	}
	catch (...) {
		if (result) {
			delete result;
			result = NULL;
		}
	}
	return result;
}

MEP2DSurface::MEP2DSurface(WinPrefs * Prefs) : Surf2DBase(Prefs) {
	OrbSet = -1;
}
MEP2DSurface::MEP2DSurface(MEP2DSurface * target) {
	ID = target->GetSurfaceID();
	Visible = target->GetVisibility();
	NumGridPoints = target->GetNumGridPoints();
	NumContours = target->GetNumContours();
	MaxContourValue = target->GetMaxValue();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	SurfOptions = target->GetSurfOptions();
	target->GetOrigin(&Origin);
	target->GetXIncrement(&XInc);
	target->GetYIncrement(&YInc);
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	OrbSet = target->getTargetOrbSet();
	NextSurface = NULL;
	GridAllocation = 0;
	Grid = NULL;
}

void MEP2DSurface::UpdateData(MEP2DSurface * target) {
	Visible = target->GetVisibility();
	NumGridPoints = target->GetNumGridPoints();
	NumContours = target->GetNumContours();
	MaxContourValue = target->GetMaxValue();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	SurfOptions = target->GetSurfOptions();
	target->GetOrigin(&Origin);
	target->GetXIncrement(&XInc);
	target->GetYIncrement(&YInc);
	if (Label) {delete [] Label; Label = NULL;}
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	OrbSet = target->getTargetOrbSet();
}
char * MEP2DSurface::GetLabel(void) {
	if (DefaultLabel()) {
		if (Label) delete[] Label;
		Label = new char[100];
		int nchar;
		sprintf(Label, "2D Molecular Electrostatic Potential%n", &nchar);
		if (Visible) sprintf(&(Label[nchar]), " Visible");
	}
	return Surface::GetLabel();
}

MEP3DSurface::MEP3DSurface(WinPrefs * Prefs) : Surf3DBase(Prefs) {
	OrbSet = -1;
	SetContourBothPosNeg(true);
}

char * MEP3DSurface::GetLabel(void) {
	if (DefaultLabel()) {
		if (Label) delete[] Label;
		Label = new char[100];
		int nchar;
		sprintf(Label, "3D Molecular Electrostatic Potential%n", &nchar);
		if (Visible) sprintf(&(Label[nchar]), " Visible");
	}
	return Surface::GetLabel();
}
void MEP3DSurface::UpdateData(MEP3DSurface * target) {
	ID = target->GetSurfaceID();
	Visible = target->GetVisibility();
	if (Label) {delete [] Label; Label = NULL;}
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	SolidSurface(target->SolidSurface());
	SetContourBothPosNeg(target->ContourBothPosNeg());
	SetColorByValue(target->ColorByValue());
	SetFixGrid(target->GetFixGrid());
	UseSurfaceNormals(target->UseSurfaceNormals());
	NumGridPoints = target->GetNumGridPoints();
	ContourValue = target->GetContourValue();
	GridSize = target->GetGridSize();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	Transparency = target->GetTransparency();
	OrbSet = target->getTargetOrbSet();
}

Orb2DSurface::Orb2DSurface(WinPrefs * Prefs) : Surf2DBase(Prefs) {
}
Orb2DSurface::Orb2DSurface(Orb2DSurface * target) {
	ID = target->GetSurfaceID();
	Visible = target->GetVisibility();
	PlotOrb = target->GetTargetOrb();
	Options = target->GetOptions();
	NumGridPoints = target->GetNumGridPoints();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	NumContours = target->GetNumContours();
	MaxContourValue = target->GetMaxValue();
	SurfOptions = target->GetSurfOptions();
	target->GetOrigin(&Origin);
	target->GetXIncrement(&XInc);
	target->GetYIncrement(&YInc);
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	NextSurface = NULL;
	Grid = NULL;
	GridAllocation = 0;
}


void Orb2DSurface::UpdateData(Orb2DSurface * target) {
	Visible = target->GetVisibility();
	PlotOrb = target->GetTargetOrb();
	Options = target->GetOptions();
	NumGridPoints = target->GetNumGridPoints();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	NumContours = target->GetNumContours();
	MaxContourValue = target->GetMaxValue();
	SurfOptions = target->GetSurfOptions();
	target->GetOrigin(&Origin);
	target->GetXIncrement(&XInc);
	target->GetYIncrement(&YInc);
	if (Label) {delete [] Label; Label = NULL;}
	if (!target->DefaultLabel())
		Label = target->GetLabel();
}
char * Orb2DSurface::GetLabel(void) {
	if (DefaultLabel()) {
		if (Label) delete[] Label;
		Label = new char[100];
		int nchar, nchar2=0;
		sprintf(Label, "2D Orbital%n", &nchar);
		if (Options&1) sprintf(&(Label[nchar]), " AO #%n",&nchar2);
		else if (Options&2) sprintf(&(Label[nchar]), " MO #%n",&nchar2);
		else if (Options&4) sprintf(&(Label[nchar]), " LMO #%n", &nchar2);
		nchar += nchar2;
		if (PlotOrb >= 0) {
			sprintf(&(Label[nchar]), " %ld%n", PlotOrb+1, &nchar2);
			nchar += nchar2;
		}
		if (Visible) sprintf(&(Label[nchar]), " Visible");
	}
	return Surface::GetLabel();
}
Orb3DSurface::Orb3DSurface(WinPrefs * Prefs) : Surf3DBase(Prefs) {
	SetContourBothPosNeg(true);
}
Orb3DSurface::Orb3DSurface(Orb3DSurface * target) : Surf3DBase() {
	ID = target->GetSurfaceID();
	Visible = target->GetVisibility();
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	PlotOrb = target->GetTargetOrb();
	Options = target->GetOptions();
	SolidSurface(target->SolidSurface());
	SetContourBothPosNeg(target->ContourBothPosNeg());
	SetColorByValue(target->ColorByValue());
	SetFixGrid(target->GetFixGrid());
	UseSurfaceNormals(target->UseSurfaceNormals());
	NumGridPoints = target->GetNumGridPoints();
	ContourValue = target->GetContourValue();
	GridSize = target->GetGridSize();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	Transparency = target->GetTransparency();
}
void Orb3DSurface::UpdateData(Orb3DSurface * target) {
	Visible = target->GetVisibility();
	PlotOrb = target->GetTargetOrb();
	Options = target->GetOptions();
	SolidSurface(target->SolidSurface());
	SetContourBothPosNeg(target->ContourBothPosNeg());
	SetColorByValue(target->ColorByValue());
	SetFixGrid(target->GetFixGrid());
	UseSurfaceNormals(target->UseSurfaceNormals());
	NumGridPoints = target->GetNumGridPoints();
	ContourValue = target->GetContourValue();
	GridSize = target->GetGridSize();
	target->GetPosColor(&PosColor);
	target->GetNegColor(&NegColor);
	Transparency = target->GetTransparency();
	if (Label) {delete [] Label; Label = NULL;}
	if (!target->DefaultLabel())
		Label = target->GetLabel();
}
char * Orb3DSurface::GetLabel(void) {
	if (DefaultLabel()) {
		if (Label) delete[] Label;
		Label = new char[100];
		int nchar, nchar2=0;
		sprintf(Label, "3D Orbital%n", &nchar);
		if (Options&1) sprintf(&(Label[nchar]), " AO #%n",&nchar2);
		else if (Options&2) sprintf(&(Label[nchar]), " MO #%n",&nchar2);
		else if (Options&4) sprintf(&(Label[nchar]), " LMO #%n", &nchar2);
		nchar += nchar2;
		if (PlotOrb >= 0) {
			sprintf(&(Label[nchar]), " %ld%n", PlotOrb+1, &nchar2);
			nchar += nchar2;
		}
		if (Visible) sprintf(&(Label[nchar]), " Visible");
	}
	return Surface::GetLabel();
}
General3DSurface::General3DSurface(WinPrefs * Prefs) : Surf3DBase(Prefs) {
}
char * General3DSurface::GetLabel(void) {
	if (DefaultLabel()) {
		if (Label) delete[] Label;
		Label = new char[100];
		int nchar;
		sprintf(Label, "General 3D surface%n", &nchar);
		if (Visible) sprintf(&(Label[nchar]), " Visible");
	}
	return Surface::GetLabel();
}
General2DSurface::General2DSurface(WinPrefs * Prefs) : Surf2DBase(Prefs) {
}
char * General2DSurface::GetLabel(void) {
	if (DefaultLabel()) {
		if (Label) delete[] Label;
		Label = new char[100];
		int nchar;
		sprintf(Label, "General 2D surface%n", &nchar);
		if (Visible) sprintf(&(Label[nchar]), " Visible");
	}
	return Surface::GetLabel();
}

TEDensity3DSurface::TEDensity3DSurface(WinPrefs * Prefs) : Surf3DBase(Prefs) {
	MaxMEPValue = 0.1f;
	OrbSet = -1;
	SetContourBothPosNeg(false);
}

char * TEDensity3DSurface::GetLabel(void) {
	if (DefaultLabel()) {
		if (Label) delete[] Label;
		Label = new char[100];
		int nchar;
		sprintf(Label, "3D Total Electron Density%n", &nchar);
		if (Visible) sprintf(&(Label[nchar]), " Visible");
	}
	return Surface::GetLabel();
}

void TEDensity3DSurface::UpdateData(TEDensity3DSurface * target) {
	ID = target->GetSurfaceID();
	Visible = target->GetVisibility();
	if (Label) {delete [] Label; Label = NULL;}
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	SolidSurface(target->SolidSurface());
	SetContourBothPosNeg(target->ContourBothPosNeg());
	SetColorByValue(target->ColorByValue());
	SetFixGrid(target->GetFixGrid());
	UseSurfaceNormals(target->UseSurfaceNormals());
	NumGridPoints = target->GetNumGridPoints();
	ContourValue = target->GetContourValue();
	GridSize = target->GetGridSize();
	target->GetPosColor(&PosColor);
	Transparency = target->GetTransparency();
	OrbSet = target->getTargetOrbSet();
}

TEDensity2DSurface::TEDensity2DSurface(TEDensity2DSurface * target) {
	ID = target->GetSurfaceID();
	Visible = target->GetVisibility();
	NumGridPoints = target->GetNumGridPoints();
	NumContours = target->GetNumContours();
	MaxContourValue = target->GetMaxValue();
	target->GetPosColor(&PosColor);
	SurfOptions = target->GetSurfOptions();
	target->GetOrigin(&Origin);
	target->GetXIncrement(&XInc);
	target->GetYIncrement(&YInc);
	OrbSet = target->getTargetOrbSet();
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	NextSurface = NULL;
	Grid = NULL;
	GridAllocation = 0;
}

TEDensity2DSurface::TEDensity2DSurface(WinPrefs * Prefs) : Surf2DBase(Prefs) {
	SetContourBothPosNeg(false);
	OrbSet = -1;
}

char * TEDensity2DSurface::GetLabel(void) {
	if (DefaultLabel()) {
		if (Label) delete[] Label;
		Label = new char[100];
		int nchar;
		sprintf(Label, "2D Total Electron Density%n", &nchar);
		if (Visible) sprintf(&(Label[nchar]), " Visible");
	}
	return Surface::GetLabel();
}

void TEDensity2DSurface::Update(MoleculeData * MainData) {
	if (Visible) {
		Progress * lProgress = new Progress;
		if (lProgress) {
			lProgress->ChangeText("Calculating TED Grid...");
			if (GetRotate2DMap()) SetPlaneToScreenPlane(MainData);
			CalculateMOGrid(MainData, lProgress);
			delete lProgress;
		}
	}
}

void TEDensity2DSurface::UpdateData(TEDensity2DSurface * target) {
	Visible = target->GetVisibility();
	NumGridPoints = target->GetNumGridPoints();
	NumContours = target->GetNumContours();
	MaxContourValue = target->GetMaxValue();
	target->GetPosColor(&PosColor);
	SurfOptions = target->GetSurfOptions();
	target->GetOrigin(&Origin);
	target->GetXIncrement(&XInc);
	target->GetYIncrement(&YInc);
	target->setTargetOrbSet(OrbSet);
	if (Label) {delete[] Label; Label = NULL;}
	if (!target->DefaultLabel())
		Label = target->GetLabel();
}

TEDensity1DSurface::TEDensity1DSurface(WinPrefs * Prefs) :
	Surf1DBase(Prefs) {
	OrbSet = -1;
	SetContourBothPosNeg(false);
}

char *TEDensity1DSurface::GetLabel() {
	if (DefaultLabel()) {
		if (Label) delete[] Label;
		Label = new char[100];
		int nchar;
		sprintf(Label, "1D Total Electron Density%n", &nchar);
		if (Visible) sprintf(&(Label[nchar]), " Visible");
	}
	return Surface::GetLabel();
}

void TEDensity1DSurface::Update(MoleculeData * MainData) {
	if (Visible) {
		Progress * lProgress = new Progress;
		if (lProgress) {
			lProgress->ChangeText("Calculating TED Grid...");
			CalculateMOGrid(MainData, lProgress);
			delete lProgress;
		}
	}
}

void TEDensity1DSurface::UpdateData(TEDensity1DSurface *target) {
	Visible = target->GetVisibility();
	NumGridPoints = target->GetNumGridPoints();
	/* NumContours = target->GetNumContours(); */
	MaxContourValue = target->GetMaxValue();
	target->GetPosColor(&PosColor);
	/* SurfOptions = target->GetSurfOptions(); */
	/* target->GetOrigin(&Origin); */
	/* target->GetXIncrement(&XInc); */
	/* target->GetYIncrement(&YInc); */
	target->setTargetOrbSet(OrbSet);
	if (Label) {delete[] Label; Label = NULL;}
	if (!target->DefaultLabel())
		Label = target->GetLabel();
}

TEDensity1DSurface::TEDensity1DSurface(TEDensity1DSurface *target) {
	ID = target->GetSurfaceID();
	Visible = target->GetVisibility();
	NumGridPoints = target->GetNumGridPoints();
	MaxContourValue = target->GetMaxValue();
	target->GetPosColor(&PosColor);
	/* SurfOptions = target->GetSurfOptions(); */
	/* target->GetOrigin(&Origin); */
	/* target->GetXIncrement(&XInc); */
	/* target->GetYIncrement(&YInc); */
	OrbSet = target->getTargetOrbSet();
	if (!target->DefaultLabel())
		Label = target->GetLabel();
	NextSurface = NULL;
	Grid = NULL;
	GridAllocation = 0;
}

//Calculate the sum of the squares of the change in position of each atom between
//CoordSetA and CoordSetB
float CalculateSquaresValue(long NumOptAtoms, mpAtom CoordSetA[], CPoint3D CoordSet[]) {
	float result=0.0f, x, y, z;
	for (long iatom=0; iatom<NumOptAtoms; iatom++) {
		x = (CoordSetA[iatom].Position.x - CoordSet[iatom].x);
		y = (CoordSetA[iatom].Position.y - CoordSet[iatom].y);
		z = (CoordSetA[iatom].Position.z - CoordSet[iatom].z);
		result += x*x + y*y + z*z;
	}
	return result;
}	/*CalculateSquaresValue*/
Surface * Frame::GetSurface(long SurfaceNum) {
	Surface * result = SurfaceList;
	for (long i=0; (i<SurfaceNum)&&result; i++) result = result->GetNextSurface();
	return result;
}
Surface * Frame::GetSurfaceWithID(long SurfaceID) {
	Surface * result = SurfaceList;
	while (result) {
		if (result->GetSurfaceID() == SurfaceID) break;
		result = result->GetNextSurface();
	}
	return result;
}
void Frame::DeleteSurface(long SurfaceNum) {
	Surface * previous=NULL, * target=SurfaceList;
	for (long i=0; (i<SurfaceNum)&&target; i++) {previous=target; target=target->GetNextSurface();}
	if (target) {
		if (previous) previous->SetNextSurface(target->GetNextSurface());
		else SurfaceList = target->GetNextSurface();
		delete target;
	}
}
void Frame::DeleteSurfaceWithID(long SurfaceID) {
	Surface * previous=NULL, * target=SurfaceList;
	while (target) {
		if (target->GetSurfaceID() == SurfaceID) break;
		previous = target;
		target=target->GetNextSurface();
	}
	if (target) {
		if (previous) previous->SetNextSurface(target->GetNextSurface());
		else SurfaceList = target->GetNextSurface();
		delete target;
	}
}
void Frame::AppendSurface(Surface * NewSurface) {
	if (SurfaceList) {
		Surface * temp = SurfaceList;
		while (temp->NextSurface) temp = temp->NextSurface;
		temp->SetNextSurface(NewSurface);
	} else SurfaceList = NewSurface;
}
long Frame::GetNumSurfaces(void) {
	long result = 0;
	Surface * temp = SurfaceList;
	while (temp) {result++; temp = temp->GetNextSurface();}
	return result;
}
bool Frame::SurfaceExportPossible(void) {
	bool result = false;
	Surface * temp = SurfaceList;
	while (temp) {result = (result || temp->ExportPossible()); temp = temp->GetNextSurface();}
	return result;
}
MemoryError::MemoryError(const char *Message) {
	long length = strlen(Message);
	ErrorMessage = new char [length];
	strcpy(ErrorMessage, Message);
};

