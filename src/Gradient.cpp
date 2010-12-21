/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	Gradient.cpp

	Class to organize data about the gradient at a particular geometry

	BMB - 6/1998
	Fixed parse bug where the RMS value was missed if the cartesian gradiant is missing - BMB 4/2002
*/
#include "Globals.h"
#include "MyTypes.h"
#include "Gradient.h"
#include "BFiles.h"

GradientData::GradientData(void) {
	RMS = -1.0f;	//init to an impossible value for the rms
	Maximum = 0.0f;
	CartesianGradient = NULL;
	CartAllocation = 0;
}

GradientData::~GradientData(void) {
	if (CartesianGradient) delete [] CartesianGradient;
}
#ifndef __wxBuild__
long GradientData::GetSize(BufferFile * Buffer) {
	Boolean	cState = Buffer->GetOutput();
	Buffer->SetOutput(false);
	long length = Write(Buffer);
	Buffer->SetOutput(cState);
	return length;
}
GradientData * GradientData::Read(BufferFile * Buffer) {
	long code;
	long total = Buffer->Read((Ptr) &code, sizeof(long));
	if (code != 1) return NULL;
	
	GradientData * lGrad = new GradientData;
	if (lGrad) {
		total += Buffer->Read((Ptr) &lGrad->RMS, sizeof(float));
		total += Buffer->Read((Ptr) &lGrad->Maximum, sizeof(float));
		if (Buffer->BytesLeftInBlock()>0) {
				long length;
			total += Buffer->Read((Ptr) &code, sizeof(long));
			total += Buffer->Read((Ptr) &length, sizeof(long));
			if (code == 2) {
				long NumAtoms = length / sizeof(CPoint3D);
				lGrad->CartesianGradient = new CPoint3D[NumAtoms];
				if (lGrad->CartesianGradient) {
					lGrad->CartAllocation = NumAtoms;
					total+=Buffer->Read((Ptr) lGrad->CartesianGradient, length);
				}
			}
		}
	}
	return lGrad;
}
long GradientData::Write(BufferFile * Buffer) {
	long total, code;
	
	code = 1;
	total = Buffer->Write((Ptr) &code, sizeof(long));
	total += Buffer->Write((Ptr) &RMS, sizeof(float));
	total += Buffer->Write((Ptr) &Maximum, sizeof(float));
	if (CartAllocation) {
		code = 2;
		total += Buffer->Write((Ptr) &code, sizeof(long));
		long length = CartAllocation*sizeof(CPoint3D);
		total += Buffer->Write((Ptr) &length, sizeof(long));
		total += Buffer->Write((Ptr) CartesianGradient, length);
	}
	return total;
}
#endif
//Buffer should be positioned at the nserch line
bool GradientData::ParseGAMESSGradient(BufferFile * Buffer, long NumAtoms,
		wxFileOffset SearchLength, bool Style) {
	bool result = false;
	char	Line[kMaxLineLength], token[kMaxLineLength];
	float	nucCharge;
	long	scanerr, iline, test;

	wxFileOffset	StartPosition = Buffer->GetFilePos();
	//read in the complete cartesian gradient
	CartesianGradient = new CPoint3D[NumAtoms];
	CartAllocation = NumAtoms;
	if (CartesianGradient) {
		result = true;
		if (Style) {	//new style gradient output sans actual coordinates
			Buffer->SkipnLines(7);
			for (iline=0; iline<NumAtoms; iline++) {
				Buffer->GetLine(Line);
				scanerr = sscanf(Line, "%ld %s %f %f %f %f", &test, token, &nucCharge,
					&(CartesianGradient[iline].x), &(CartesianGradient[iline].y),
					&(CartesianGradient[iline].z));
				if ((scanerr != 6)||(test!=(iline+1))) {
					delete [] CartesianGradient;
					CartesianGradient = NULL;
					CartAllocation = 0;
					result = false;
					Buffer->SetFilePos(StartPosition);
					break;
				}
			}
		} else {		//old style, both coords and gradient on each line
			float	xc, yc, zc;
			Buffer->SkipnLines(9);
			for (iline=0; iline<NumAtoms; iline++) {
				Buffer->GetLine(Line);
				scanerr = sscanf(Line, "%ld %s %f %f %f %f %f %f %f", &test, token,
					&nucCharge, &xc, &yc, &zc, &(CartesianGradient[iline].x),
					&(CartesianGradient[iline].y), &(CartesianGradient[iline].z));
				if ((scanerr != 9)||(test!=(iline+1))) {
					delete [] CartesianGradient;
					CartesianGradient = NULL;
					CartAllocation = 0;
					result = false;
					Buffer->SetFilePos(StartPosition);
					break;
				}
			}
		}
	}

	//read in the maximum and rms gradient
	if (Buffer->LocateKeyWord("MAXIMUM GRADIENT", 16, SearchLength)) {
		Buffer->GetLine(Line);
		scanerr = sscanf(Line, "MAXIMUM GRADIENT = %f RMS GRADIENT = %f", &Maximum, &RMS);
		if (scanerr == 2) result = true;
	}

	return result;
}
