/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	SurfaceBase.cpp

	Class member functions implementing the base functionality for MacMolPlt
	2D and 3D surfaces.

	Split out of other source files - January 1998 by Brett Bode
	Added AdjustSurfaceNormals routine - December 2000 BMB
	Changed surface normal code to average over adjacent triangles - June 2001 BMB
*/


/* member functions for my base surface class */
#include "Globals.h"
#include "GlobalExceptions.h"
#include "MoleculeData.h"
#include "Frame.h"
#include "SurfaceTypes.h"
#include "Math3D.h"
#include "Prefs.h"
#include "Progress.h"
#include <string.h>

void SortContourPoints(long *Points, unsigned char index, long *VertexList, long * NumTriangles);
void NormalizeNormalArray(CPoint3D * ContourNorm, long &NumVertices);
void ComputeTriangleNormals(CPoint3D * Contour, CPoint3D *ContourNorm, long * VertexList, long &NumTriangles);


Surface::Surface(void)
	: OrbSet(-1) {
	ID = 0;		//Set this to something unique
	NextSurface = NULL;
	Label = NULL;
	Visible = false;
	has_default_label = true;
}
Surface::~Surface(void) {
	if (Label) delete[] Label;
}
bool Surface::Needs2DPlane(void) const {return false;}
void Surface::RotateEvent(MoleculeData * /*MainData*/) {
}
char * Surface::GetLabel(void) {
	/* char *ltext = NULL; */
	/* if (Label) { */
		/* long textLength = strlen(Label)+1; */
		/* ltext = new char[textLength]; */
		/* strcpy(ltext, Label); */
	/* } */
	return Label;
}
void Surface::SetLabel(const char * NewLabel) {
	if (Label) {delete [] Label; Label = NULL;}
	if (NewLabel) {
		long length = strlen(NewLabel) + 1;
		Label = new char[length];
		if (Label) strcpy(Label, NewLabel);
		has_default_label = false;
	} else {
		has_default_label = true;
	}
}
void Surface::Draw2D(MoleculeData * /*lData*/, long /*hoffset*/, long /*voffset*/, float /*scale*/) {
	//No default Drawing
}
void Surface::Update(MoleculeData * /*lData*/) {
}
void Surface::Export(BufferFile * Buffer, exportFileType eft) {
	Buffer->Write("Export not supported for the chosen surface type", true);
}
void Surface::RotateSurface(Matrix4D /*RotationMatrix*/) {
}
/**
  * Exports surface data to our text file type	
  * This can effectively only be called by Surf3DBase objects
  * @param Grid3D
  * @param nx number of grid points on the X axis
  * @param ny number of grid points on the Y axis
  * @param nz number of grid points on the Z axis
  * @param Origin origin of the 3D cartesian grid
  * @param XInc increment in the X direction 
  * @param YInc increment in the Y direction 
  * @param ZInc increment in the Z direction 
  * @param Label provided text label for this surface
  * @param Buffer A BufferFileObject which the .txt file is buffered into
  * to make parsing the file easier.  See the BufferFile object for valid
  * BufferFile operations.
*/
void Surface::Export3D(const float * Grid3D, long nx, long ny, long nz, const CPoint3D * Origin,
		float XInc, float YInc, float ZInc, const char * Label, BufferFile * Buffer) const {
	char Line[kMaxLineLength];
	if (!Grid3D) return;
		//punch out the provided surface label
	Buffer->WriteLine(Label, true);
		//Write out the number of grid points in each direction
	sprintf(Line, "%ld %ld %ld   //nx ny nz", nx,ny,nz);
	Buffer->WriteLine(Line, true);	// true means add a system-appropriate EOL
	sprintf(Line, "%g %g %g   //Origin of the 3D grid", Origin->x, Origin->y, Origin->z);
	Buffer->WriteLine(Line, true);
	sprintf(Line, "%g %g %g   //x increment, y inc, z inc/ grid(x(y(z)))", XInc, YInc, ZInc);
	Buffer->WriteLine(Line, true);
		//now write out the grid
		long	ix,iy,iz,igrid=0,ival=0;
	for (ix=0; ix < nx; ix++) {
		for (iy=0; iy < ny; iy++) {
			for (iz=0; iz < nz; iz++) {
				sprintf(Line, "%g ", Grid3D[igrid]);
				Buffer->PutText(Line);
				ival++;
				if (ival>=5) {
					Buffer->WriteLine("", true);
					ival=0;
				}
				igrid++;
			}
		}
	}
	if (ival) 	Buffer->WriteLine("", true);
}

/**
  * Exports surface data to CCP4 file type	
  * This can effectively only be called by Surf3DBase objects
  * @param Grid3D
  * @param nx number of grid points on the X axis
  * @param ny number of grid points on the Y axis
  * @param nz number of grid points on the Z axis
  * @param Origin origin of the 3D cartesian grid
  * @param XInc increment in the X direction 
  * @param YInc increment in the Y direction 
  * @param ZInc increment in the Z direction 
  * @param Label provided text label for this surface
  * @param Buffer A BufferFileObject which the CCP4 file is buffered into
  * to make parsing the file easier.  See the BufferFile object for valid
  * BufferFile operations.
*/
void Surface::Export3DCCP4(const float * Grid3D, long nx, long ny, long nz, const CPoint3D * Origin,
		float XInc, float YInc, float ZInc, const char * Label, BufferFile * Buffer) const {

}

/**
  * Exports surface data to CNS electron density map file type	
  * This can effectively only be called by Surf3DBase objects
  * @param Grid3D
  * @param nx number of grid points on the X axis
  * @param ny number of grid points on the Y axis
  * @param nz number of grid points on the Z axis
  * @param Origin origin of the 3D cartesian grid
  * @param XInc increment in the X direction 
  * @param YInc increment in the Y direction 
  * @param ZInc increment in the Z direction 
  * @param Label provided text label for this surface
  * @param Buffer A BufferFileObject which the CNS file is buffered into
  * to make parsing the file easier.  See the BufferFile object for valid
  * BufferFile operations.
*/
void Surface::Export3DCNS(const float * Grid3D, long nx, long ny, long nz, const CPoint3D * Origin,
		float XInc, float YInc, float ZInc, const char * Label, BufferFile * Buffer) const {
	if(!Grid3D) return;			// we'll only export if this is a 3D surface
	
	char Line[kMaxLineLength];
	wxDateTime CurTime = wxDateTime::Now();
	wxString UserName = wxGetUserId();
	if(!Grid3D) return;
	
	Buffer->WriteLine("\n\t2 ", true);		// !NTITLE
	sprintf(Line, "REMARKS CNS electron density map created by wxMacMolPlt"); 
	Buffer->WriteLine(Line, true);
	sprintf(Line, "REMARKS user: %s\tdate: %s", 
		(const char *)UserName.ToAscii(),					// username that ran program
		(const char *)CurTime.Format(wxT("%c")).ToAscii());	// standard time format	
	Buffer->WriteLine(Line, true);

	// TODO print out formatted data here
}

/**
 * Exports surface data to our text file type	
 * This can effectively only be called by Surf2DBase objects
 * @param Grid2D 2D Grid of data
 * @param NumPoints number of grid points in both the x and y direction
 * @param Origin origin of the 3D cartesian grid
 * @param XInc increment vector in the X direction 
 * @param YInc increment vector in the Y direction 
 * @param Label provided text label for this surface
 * @param Buffer A BufferFileObject which the .txt file is buffered into
 * to make parsing the file easier.  See the BufferFile object for valid
 * BufferFile operations.
 */
void Surface::Export2D(const float * Grid2D, long NumPoints, const CPoint3D * Origin,
	const CPoint3D *XInc, const CPoint3D *YInc, const char * Label, BufferFile * Buffer) const {
		char Line[kMaxLineLength];
	if (!Grid2D) return;
		//punch out the provided surface label
	Buffer->WriteLine(Label, true);
		//Write out the number of grid points in each direction
	sprintf(Line, "%ld    //# grid points", NumPoints);
	Buffer->WriteLine(Line, true);	//true means add a line feed
	sprintf(Line, "%g %g %g   //Origin of the 2D grid", Origin->x, Origin->y, Origin->z);
	Buffer->WriteLine(Line, true);
	sprintf(Line, "%g %g %g   //X inc vector", XInc->x, XInc->y, XInc->z);
	Buffer->WriteLine(Line, true);
	sprintf(Line, "%g %g %g   //Y inc vector", YInc->x, YInc->y, YInc->z);
	Buffer->WriteLine(Line, true);
		//now write out the grid
		long	ix,iy,igrid=0,ival=0;
	for (ix=0; ix < NumPoints; ix++) {
		for (iy=0; iy < NumPoints; iy++) {
			sprintf(Line, "%g ", Grid2D[igrid]);
			Buffer->PutText(Line);
			ival++;
			if (ival>=5) {
				Buffer->WriteLine("", true);
				ival=0;
			}
			igrid++;
		}
	}
	if (ival) Buffer->WriteLine("", true);
}

Surf1DBase::Surf1DBase(WinPrefs *Prefs) {
	Grid = NULL;
	GridAllocation = 0;
	SurfaceOptions * lOpts = Prefs->GetSurfaceOptions();
	NumGridPoints = lOpts->GetNumGridPoints();
	MaxContourValue = lOpts->GetMaxContour();
	SurfOptions = 0;
	SetContourBothPosNeg(true);
	lOpts->GetPosColor(&PosColor);
	lOpts->GetNegColor(&NegColor);
	Start = CPoint3D(0.0f, 0.0f, 0.0f);
	End = CPoint3D(1.0f, 0.0f, 0.0f);
	Scale = 1.0f;
}

Surf1DBase::Surf1DBase() {
	Grid = NULL;
	GridAllocation = 0;
	SetContourBothPosNeg(true);
	Start = CPoint3D(0.0f, 0.0f, 0.0f);
	End = CPoint3D(1.0f, 0.0f, 0.0f);
	Scale = 1.0f;
	MaxContourValue = 100.0f;
}

Surf1DBase::~Surf1DBase() {
	FreeGrid();
}

void Surf1DBase::SetNumGridPoints(long newNum) {
	if (newNum > 1) NumGridPoints = newNum;
}

/* member functions for the base 2D surface class */

Surf2DBase::Surf2DBase(WinPrefs * Prefs) {
	Grid = NULL;
	GridAllocation = 0;
	Origin.x = Origin.y = Origin.z = 0.0;
	XInc.x = 0.1f; XInc.y=XInc.z=0.0;
	YInc.x=YInc.z=0.0; YInc.y=0.1f;

	SurfaceOptions * lOpts = Prefs->GetSurfaceOptions();

	NumGridPoints = lOpts->GetNumGridPoints();
	NumContours = lOpts->GetNumContours();
	MaxContourValue = lOpts->GetMaxContour();
	SurfOptions = 0;
	if (lOpts->GetShowZeroContour()) SurfOptions ++;
	SurfOptions += 2;	//default to using the screen plane
	SetContourBothPosNeg(true);
	lOpts->GetPosColor(&PosColor);
	lOpts->GetNegColor(&NegColor);
}
Surf2DBase::Surf2DBase(void) {
	Grid = NULL;
	GridAllocation = 0;
	SetContourBothPosNeg(true);
}
Surf2DBase::~Surf2DBase(void) {
	FreeGrid();
}
void Surf2DBase::SetNumGridPoints(long newNum) {
	long OldNum = NumGridPoints;
	if (newNum>1) NumGridPoints=newNum;
	if (!NeedsUpdateDuringRotation()) {	//for fixed planes update the x and y incs for the new
										//# of grid points
		XInc.x *= OldNum;
		XInc.y *= OldNum;
		XInc.z *= OldNum;
		YInc.x *= OldNum;
		YInc.y *= OldNum;
		YInc.z *= OldNum;
		XInc.x /= NumGridPoints;
		XInc.y /= NumGridPoints;
		XInc.z /= NumGridPoints;
		YInc.x /= NumGridPoints;
		YInc.y /= NumGridPoints;
		YInc.z /= NumGridPoints;
	}
}
bool Surf2DBase::Needs2DPlane(void) const {
	bool result = false;
	if (Visible) result = true;
	return result;
}
void Surf2DBase::RotateEvent(MoleculeData * MainData) {
	if ((SurfOptions&2)!=0) {
		if (Visible)
			Update(MainData);
		else
			FreeGrid();
	}
}
void Surf2DBase::RotateSurface(Matrix4D RotationMatrix) {
		CPoint3D temp;
	Rotate3DPt(RotationMatrix, Origin, &temp);
	Origin = temp;
	Rotate3DOffset(RotationMatrix, XInc, &temp);
	XInc = temp;
	Rotate3DOffset(RotationMatrix, YInc, &temp);
	YInc = temp;
}

void Surf1DBase::Export(BufferFile * Buffer, exportFileType /*eft*/) {
	char Line[kMaxLineLength];
	if (!Grid) return;
	//punch out the provided surface label
	Buffer->WriteLine(Label, true);
	//Write out the number of grid points
	sprintf(Line, "%ld   //# points", NumGridPoints);
	Buffer->WriteLine(Line, true);	// true means add a system-appropriate EOL
	sprintf(Line, "%g %g %g   //Origin of the Line", Start.x, Start.y, Start.z);
	Buffer->WriteLine(Line, true);
	sprintf(Line, "%g %g %g   //End of the Line", End.x, End.y, End.z);
	Buffer->WriteLine(Line, true);
	//now write out the grid
	long	ix, ival=0;
	for (ix=0; ix < NumGridPoints; ix++) {
		sprintf(Line, "%g ", Grid[ix]);
		Buffer->PutText(Line);
		ival++;
		if (ival>=5) {	//5 values per line
			Buffer->WriteLine("", true);
			ival=0;
		}
	}
	if (ival) 	Buffer->WriteLine("", true);
}

void Surf2DBase::Export(BufferFile * Buffer, exportFileType eft) {
	float * lGrid;
	char * label = GetLabel();
	lGrid = Grid;
	Surface::Export2D(lGrid, NumGridPoints, &Origin, &XInc, &YInc, label, Buffer);
	/* if (label) delete [] label; */
}
void Surf2DBase::Draw2D(MoleculeData * lData, long hoffset, long voffset, float scale) {
	if (Visible) {
			//Update the grid if needed, then contour and display
		if (!Grid) Update(lData);
		if (Grid) Contour2DGrid(lData, hoffset, voffset, scale);
	}
}
//Setup the 2D plane to the current screen plane
void Surf2DBase::SetPlaneToScreenPlane(MoleculeData * MainData) {
	Matrix4D	RotInverse;
	InverseMatrix(MainData->TotalRotation, RotInverse);

	CPoint3D	TestPoint, NewPoint1, NewPoint2;
	TestPoint.x = -0.55f*MainData->WindowSize;
	TestPoint.y=-0.55f*MainData->WindowSize; TestPoint.z=0.0;
	BackRotate3DPt(RotInverse, TestPoint, &NewPoint1);
	TestPoint.x = 0.55f*MainData->WindowSize;
	TestPoint.y=-0.55f*MainData->WindowSize;
	BackRotate3DPt(RotInverse, TestPoint, &NewPoint2);
//	Origin.x = NewPoint1.x*kAng2BohrConversion;
//	Origin.y = NewPoint1.y*kAng2BohrConversion;
//	Origin.z = NewPoint1.z*kAng2BohrConversion;
	Origin.x = NewPoint1.x + MainData->Centroid.x;
	Origin.y = NewPoint1.y + MainData->Centroid.y;
	Origin.z = NewPoint1.z + MainData->Centroid.z;
	long NumPoints = NumGridPoints-1;	//Subtract one such that the window will be spanned by NumPoints
//	XInc.x = (NewPoint2.x-NewPoint1.x)*kAng2BohrConversion/NumPoints;
//	XInc.y = (NewPoint2.y-NewPoint1.y)*kAng2BohrConversion/NumPoints;
//	XInc.z = (NewPoint2.z-NewPoint1.z)*kAng2BohrConversion/NumPoints;
	XInc.x = (NewPoint2.x-NewPoint1.x)/NumPoints;
	XInc.y = (NewPoint2.y-NewPoint1.y)/NumPoints;
	XInc.z = (NewPoint2.z-NewPoint1.z)/NumPoints;
	TestPoint.x = -0.55f*MainData->WindowSize;
	TestPoint.y=0.55f*MainData->WindowSize;
	BackRotate3DPt(RotInverse, TestPoint, &NewPoint2);
//	YInc.x = (NewPoint2.x-NewPoint1.x)*kAng2BohrConversion/NumPoints;
//	YInc.y = (NewPoint2.y-NewPoint1.y)*kAng2BohrConversion/NumPoints;
//	YInc.z = (NewPoint2.z-NewPoint1.z)*kAng2BohrConversion/NumPoints;
	YInc.x = (NewPoint2.x-NewPoint1.x)/NumPoints;
	YInc.y = (NewPoint2.y-NewPoint1.y)/NumPoints;
	YInc.z = (NewPoint2.z-NewPoint1.z)/NumPoints;
}
void Surf2DBase::Contour2DGrid(MoleculeData * lData, long hoffset, long voffset, float scale) {
	if (!GridAvailable()) return;
//For now just ifdef out the entire routine for the wxWidgets build
#ifndef __wxBuild__
	PenSize(1,1);		//Make sure the pen size is appropriate for contours
		RGBColor	savecolor, LtGrayColor={43000,43000,43000};
	GetForeColor(&savecolor);	//Save the current fore color

		//Scan the Grid producing the contours
	float	TestPoint1, TestPoint2, TestPoint3, TestPoint4, XGridValue, YGridValue, ZGridValue;
	CPoint3D	Contour[4];
	Point		StartPt, EndPt;
	bool		HasPoint[4];

	long NumPoints = NumGridPoints;
	float * lGrid;
	lGrid = Grid;

	CPoint3D	XGridMin, XGridInc, YGridInc, OutPt;
	XGridMin = Origin;
//	XGridMin *= kBohr2AngConversion;
	XGridInc = XInc;
//	XGridInc *= kBohr2AngConversion;
	YGridInc = YInc;
//	YGridInc *= kBohr2AngConversion;

	Rotate3DPt(lData->TotalRotation, XGridMin, &OutPt);
	XGridMin = OutPt;
	Rotate3DOffset(lData->TotalRotation, XGridInc, &OutPt);
	XGridInc = OutPt;
	Rotate3DOffset(lData->TotalRotation, YGridInc, &OutPt);
	YGridInc = OutPt;
	
	float ContourValueInc = MaxContourValue/(NumContours+1);
	long NumPosContours = (long) (fabs(GridMax)/ContourValueInc + 1);
	long NumNegContours = (long) (fabs(GridMin)/ContourValueInc + 1);
	bool Dashing = GetDashLine();
	
	float ContourValue = 0.0;
	long n;
		//Go up to NumContours+1 to allow for the zero contour
	for (long iContour=0; iContour<=NumContours; iContour++) {
		for (int pass=0; pass<2; pass++) {
			if (iContour==0) {	//0 value contour
				RGBForeColor(&LtGrayColor);
				pass++;	//only need one 0 contour!
					//Plot zero value contour only if requested
				if (!(SurfOptions&1)) continue;
			} else {
				ContourValue *= -1.0;
				if (pass==0) {
					if (!ContourBothPosNeg()) continue;
					if (iContour > NumNegContours) continue;
					RGBForeColor(&NegColor);
				} else {
					if (iContour > NumPosContours) continue;
					RGBForeColor(&PosColor);
				}
			}
			n=NumGridPoints;
			for (long i=1; i<NumPoints; i++) {
				XGridValue = XGridMin.x + i*XGridInc.x;
				YGridValue = XGridMin.y + i*XGridInc.y;
				ZGridValue = XGridMin.z + i*XGridInc.z;
				for (long j=1; j<NumPoints; j++) {
					XGridValue += YGridInc.x;
					YGridValue += YGridInc.y;
					ZGridValue += YGridInc.z;
					n++;
					for (int i=0; i<4; i++) HasPoint[i]=false;
					
					TestPoint1 = lGrid[n]-ContourValue;
					TestPoint2 = lGrid[n-1]-ContourValue;
					TestPoint3 = lGrid[n-NumPoints]-ContourValue;
					TestPoint4 = lGrid[n-1-NumPoints]-ContourValue;

					if ((TestPoint1*TestPoint2)<0.0) {
						HasPoint[0]=true;
						Contour[0].x = XGridValue - YGridInc.x*(TestPoint1/(TestPoint1-TestPoint2));
						Contour[0].y = YGridValue - YGridInc.y*(TestPoint1/(TestPoint1-TestPoint2));
						Contour[0].z = ZGridValue - YGridInc.z*(TestPoint1/(TestPoint1-TestPoint2));
					}
					if ((TestPoint1*TestPoint3)<0.0) {
						HasPoint[1]=true;
						Contour[1].x = XGridValue - XGridInc.x*(TestPoint1/(TestPoint1-TestPoint3));
						Contour[1].y = YGridValue - XGridInc.y*(TestPoint1/(TestPoint1-TestPoint3));
						Contour[1].z = ZGridValue - XGridInc.z*(TestPoint1/(TestPoint1-TestPoint3));
					}
					if ((TestPoint2*TestPoint4)<0.0) {
						HasPoint[2]=true;
						Contour[2].x = XGridValue-YGridInc.x - XGridInc.x*(TestPoint2/(TestPoint2-TestPoint4));
						Contour[2].y = YGridValue-YGridInc.y - XGridInc.y*(TestPoint2/(TestPoint2-TestPoint4));
						Contour[2].z = ZGridValue-YGridInc.z - XGridInc.z*(TestPoint2/(TestPoint2-TestPoint4));
					}
					if ((TestPoint3*TestPoint4)<0.0) {
						HasPoint[3]=true;
						Contour[3].x = XGridValue-XGridInc.x - YGridInc.x*(TestPoint3/(TestPoint3-TestPoint4));
						Contour[3].y = YGridValue-XGridInc.y - YGridInc.y*(TestPoint3/(TestPoint3-TestPoint4));
						Contour[3].z = ZGridValue-XGridInc.z - YGridInc.z*(TestPoint3/(TestPoint3-TestPoint4));
					}
					
					if (HasPoint[0]) {
						StartPt.h = (long) (scale * Contour[0].x) + hoffset;
						StartPt.v = (long) (-scale * Contour[0].y) + voffset;
						if (HasPoint[1] || HasPoint[2]) {
							if (HasPoint[1]) {
								EndPt.h = (long) (scale * Contour[1].x) + hoffset;
								EndPt.v = (long) (-scale * Contour[1].y) + voffset;
								if ((pass==0)&&Dashing) {
									EndPt.h = StartPt.h + (EndPt.h-StartPt.h)/2;
									EndPt.v = StartPt.v + (EndPt.v-StartPt.v)/2;
								}
								MoveTo(StartPt.h, StartPt.v);
								LineTo(EndPt.h, EndPt.v);
							}
							if (HasPoint[2]) {
								EndPt.h = (long) (scale * Contour[2].x) + hoffset;
								EndPt.v = (long) (-scale * Contour[2].y) + voffset;
								if ((pass==0)&&Dashing) {
									EndPt.h = StartPt.h + (EndPt.h-StartPt.h)/2;
									EndPt.v = StartPt.v + (EndPt.v-StartPt.v)/2;
								}
								MoveTo(StartPt.h, StartPt.v);
								LineTo(EndPt.h, EndPt.v);
							}
						} else if (HasPoint[3]) {
							EndPt.h = (long) (scale * Contour[3].x) + hoffset;
							EndPt.v = (long) (-scale * Contour[3].y) + voffset;
							if ((pass==0)&&Dashing) {
								EndPt.h = StartPt.h + (EndPt.h-StartPt.h)/2;
								EndPt.v = StartPt.v + (EndPt.v-StartPt.v)/2;
							}
							MoveTo(StartPt.h, StartPt.v);
							LineTo(EndPt.h, EndPt.v);
						}
					}
					if (HasPoint[1]) {
						if (HasPoint[3]) {
							StartPt.h = (long) (scale * Contour[1].x) + hoffset;
							StartPt.v = (long) (-scale * Contour[1].y) + voffset;
							EndPt.h = (long) (scale * Contour[3].x) + hoffset;
							EndPt.v = (long) (-scale * Contour[3].y) + voffset;
							if ((pass==0)&&Dashing) {
								EndPt.h = StartPt.h + (EndPt.h-StartPt.h)/2;
								EndPt.v = StartPt.v + (EndPt.v-StartPt.v)/2;
							}
							MoveTo(StartPt.h, StartPt.v);
							LineTo(EndPt.h, EndPt.v);
						} else if (HasPoint[2] && !HasPoint[0]) {
							StartPt.h = (long) (scale * Contour[1].x) + hoffset;
							StartPt.v = (long) (-scale * Contour[1].y) + voffset;
							EndPt.h = (long) (scale * Contour[2].x) + hoffset;
							EndPt.v = (long) (-scale * Contour[2].y) + voffset;
							if ((pass==0)&&Dashing) {
								EndPt.h = StartPt.h + (EndPt.h-StartPt.h)/2;
								EndPt.v = StartPt.v + (EndPt.v-StartPt.v)/2;
							}
							MoveTo(StartPt.h, StartPt.v);
							LineTo(EndPt.h, EndPt.v);
						}
					}
					if (HasPoint[2]&&HasPoint[3]) {
						StartPt.h = (long) (scale * Contour[2].x) + hoffset;
						StartPt.v = (long) (-scale * Contour[2].y) + voffset;
						EndPt.h = (long) (scale * Contour[3].x) + hoffset;
						EndPt.v = (long) (-scale * Contour[3].y) + voffset;
						if ((pass==0)&&Dashing) {
							EndPt.h = StartPt.h + (EndPt.h-StartPt.h)/2;
							EndPt.v = StartPt.v + (EndPt.v-StartPt.v)/2;
						}
						MoveTo(StartPt.h, StartPt.v);
						LineTo(EndPt.h, EndPt.v);
					}
				}
				n++;
			}
		}
		ContourValue += ContourValueInc;
	}
	RGBForeColor(&savecolor);
#endif	//end of wxBuild ifdef
}

long Surf2DBase::ExportPOV(MoleculeData *MainData, WinPrefs *Prefs,
						   BufferFile *Buffer) {
	// Scan the Grid producing the contours
	float TestPoint1, TestPoint2, TestPoint3, TestPoint4, XGridValue, YGridValue, ZGridValue;
	CPoint3D Contour[4];
	Boolean HasPoint[4];
	wxString tmpStr;

	long NumPoints = NumGridPoints;
	float * lGrid = Grid;
	
	CPoint3D	XGridMin, XGridInc, YGridInc;
	XGridMin = Origin;
	XGridInc = XInc;
	YGridInc = YInc;

	float ContourValueInc = MaxContourValue/(NumContours+1);
	long NumPosContours = (long)(fabs(GridMax)/ContourValueInc) + 1;
	long NumNegContours = (long)(fabs(GridMin)/ContourValueInc) + 1;
	
	CPoint3D lineStart, lineEnd;
	float ContourValue = 0.0;
	float lineWidth = Prefs->GetQD3DLineWidth();
	long n;

	if (lineWidth < 1e-6f) {
		lineWidth = 0.005f;
	}

	float color[3];
	color[0] = color[1] = color[2] = 0.65f;
	
	// Go up to NumContours+1 to allow for the zero contour
	for (long iContour = 0; iContour <= NumContours; iContour++) {
		for (int pass = 0; pass < 2; pass++) {
			if (iContour==0) {	//0 value contour
				pass++;	//only need one 0 contour!
				//Plot zero value contour only if requested
				if (!(SurfOptions&1)) continue;
			} else {
				ContourValue *= -1.0;
				if (pass==0) {
					if (!ContourBothPosNeg()) continue;
					if (iContour > NumNegContours) continue;
					color[0] = NegColor.red / 65535.0f;
					color[1] = NegColor.green / 65535.0f;
					color[2] = NegColor.blue / 65535.0f;
				} else {
					if (iContour > NumPosContours) continue;
					color[0] = PosColor.red / 65535.0f;
					color[1] = PosColor.green / 65535.0f;
					color[2] = PosColor.blue / 65535.0f;
				}
			}
			n=NumGridPoints;
			for (long i=1; i<NumPoints; i++) {
				XGridValue = XGridMin.x + i*XGridInc.x;
				YGridValue = XGridMin.y + i*XGridInc.y;
				ZGridValue = XGridMin.z + i*XGridInc.z;
				for (long j=1; j<NumPoints; j++) {
					XGridValue += YGridInc.x;
					YGridValue += YGridInc.y;
					ZGridValue += YGridInc.z;
					n++;
					for (int i=0; i<4; i++) HasPoint[i]=false;
					
					TestPoint1 = lGrid[n]-ContourValue;
					TestPoint2 = lGrid[n-1]-ContourValue;
					TestPoint3 = lGrid[n-NumPoints]-ContourValue;
					TestPoint4 = lGrid[n-1-NumPoints]-ContourValue;

					if ((TestPoint1*TestPoint2)<0.0) {
						HasPoint[0]=true;
						Contour[0].x = XGridValue - YGridInc.x*(TestPoint1/(TestPoint1-TestPoint2));
						Contour[0].y = YGridValue - YGridInc.y*(TestPoint1/(TestPoint1-TestPoint2));
						Contour[0].z = ZGridValue - YGridInc.z*(TestPoint1/(TestPoint1-TestPoint2));
					}
					if ((TestPoint1*TestPoint3)<0.0) {
						HasPoint[1]=true;
						Contour[1].x = XGridValue - XGridInc.x*(TestPoint1/(TestPoint1-TestPoint3));
						Contour[1].y = YGridValue - XGridInc.y*(TestPoint1/(TestPoint1-TestPoint3));
						Contour[1].z = ZGridValue - XGridInc.z*(TestPoint1/(TestPoint1-TestPoint3));
					}
					if ((TestPoint2*TestPoint4)<0.0) {
						HasPoint[2]=true;
						Contour[2].x = XGridValue-YGridInc.x - XGridInc.x*(TestPoint2/(TestPoint2-TestPoint4));
						Contour[2].y = YGridValue-YGridInc.y - XGridInc.y*(TestPoint2/(TestPoint2-TestPoint4));
						Contour[2].z = ZGridValue-YGridInc.z - XGridInc.z*(TestPoint2/(TestPoint2-TestPoint4));
					}
					if ((TestPoint3*TestPoint4)<0.0) {
						HasPoint[3]=true;
						Contour[3].x = XGridValue-XGridInc.x - YGridInc.x*(TestPoint3/(TestPoint3-TestPoint4));
						Contour[3].y = YGridValue-XGridInc.y - YGridInc.y*(TestPoint3/(TestPoint3-TestPoint4));
						Contour[3].z = ZGridValue-XGridInc.z - YGridInc.z*(TestPoint3/(TestPoint3-TestPoint4));
					}
					
					if (HasPoint[0]) {
						lineStart = Contour[0];
						if (HasPoint[1] || HasPoint[2]) {
							if (HasPoint[1]) {
								lineEnd = Contour[1];
								tmpStr.Printf(wxT("cylinder {\n")
											  wxT("   <%f, %f, %f>, <%f, %f, %f>, %f\n")
											  wxT("   texture {\n")
											  wxT("      pigment {color rgb<%f, %f, %f>}\n")
											  wxT("      finish {SurfaceFinish}\n")
											  wxT("   }\n")
											  wxT("}\n"),
											  lineStart.x, lineStart.y, lineStart.z,
											  lineEnd.x, lineEnd.y, lineEnd.z, lineWidth,
											  color[0], color[1], color[2]);
								Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
							}
							if (HasPoint[2]) {
								lineEnd = Contour[2];
								tmpStr.Printf(wxT("cylinder {\n")
											  wxT("   <%f, %f, %f>, <%f, %f, %f>, %f\n")
											  wxT("   texture {\n")
											  wxT("      pigment {color rgb<%f, %f, %f>}\n")
											  wxT("      finish {SurfaceFinish}\n")
											  wxT("   }\n")
											  wxT("}\n"),
											  lineStart.x, lineStart.y, lineStart.z,
											  lineEnd.x, lineEnd.y, lineEnd.z, lineWidth,
											  color[0], color[1], color[2]);
								Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
							}
						} else if (HasPoint[3]) {
							lineEnd = Contour[3];
							tmpStr.Printf(wxT("cylinder {\n")
										  wxT("   <%f, %f, %f>, <%f, %f, %f>, %f\n")
										  wxT("   texture {\n")
										  wxT("      pigment {color rgb<%f, %f, %f>}\n")
										  wxT("      finish {SurfaceFinish}\n")
										  wxT("   }\n")
										  wxT("}\n"),
										  lineStart.x, lineStart.y, lineStart.z,
										  lineEnd.x, lineEnd.y, lineEnd.z, lineWidth,
										  color[0], color[1], color[2]);
							Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
						}
					}
					if (HasPoint[1]) {
						if (HasPoint[3]) {
							lineStart = Contour[1];
							lineEnd = Contour[3];
							tmpStr.Printf(wxT("cylinder {\n")
										  wxT("   <%f, %f, %f>, <%f, %f, %f>, %f\n")
										  wxT("   texture {\n")
										  wxT("      pigment {color rgb<%f, %f, %f>}\n")
										  wxT("      finish {SurfaceFinish}\n")
										  wxT("   }\n")
										  wxT("}\n"),
										  lineStart.x, lineStart.y, lineStart.z,
										  lineEnd.x, lineEnd.y, lineEnd.z, lineWidth,
										  color[0], color[1], color[2]);
							Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
						} else if (HasPoint[2] && !HasPoint[0]) {
							lineStart = Contour[1];
							lineEnd = Contour[2];
							tmpStr.Printf(wxT("cylinder {\n")
										  wxT("   <%f, %f, %f>, <%f, %f, %f>, %f\n")
										  wxT("   texture {\n")
										  wxT("      pigment {color rgb<%f, %f, %f>}\n")
										  wxT("      finish {SurfaceFinish}\n")
										  wxT("   }\n")
										  wxT("}\n"),
										  lineStart.x, lineStart.y, lineStart.z,
										  lineEnd.x, lineEnd.y, lineEnd.z, lineWidth,
										  color[0], color[1], color[2]);
							Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
						}
					}
					if (HasPoint[2]&&HasPoint[3]) {
						lineStart = Contour[2];
						lineEnd = Contour[3];
						tmpStr.Printf(wxT("cylinder {\n")
									  wxT("   <%f, %f, %f>, <%f, %f, %f>, %f\n")
									  wxT("   texture {\n")
									  wxT("      pigment {color rgb<%f, %f, %f>}\n")
									  wxT("      finish {SurfaceFinish}\n")
									  wxT("   }\n")
									  wxT("}\n"),
									  lineStart.x, lineStart.y, lineStart.z,
									  lineEnd.x, lineEnd.y, lineEnd.z, lineWidth,
									  color[0], color[1], color[2]);
						Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
					}
				}
				n++;
			}
		}
		ContourValue += ContourValueInc;
	}

	return 0;

}

/* member functions for the base 3D surface class */
Surf3DBase::Surf3DBase(WinPrefs * Prefs) {
	ContourHndl = NULL;
	VertexList = NULL;
	List = NULL;
	Grid = NULL;
	SurfaceNormals = NULL;
	GridMin = GridMax = 0;
	GridAllocation = ContourAllocation = ListAllocation = SNormAllocation = VertexAllocation = 0;
	NumVertices = NumPosContourTriangles = NumNegContourTriangles = 0;

	SurfaceOptions * lOpts = Prefs->GetSurfaceOptions();

	NumGridPoints = lOpts->GetNumGridPoints();
	NumXGridPoints = NumYGridPoints = NumZGridPoints = NumGridPoints;
	Origin.x = Origin.y = Origin.z = 0.0;
	XGridInc = YGridInc = ZGridInc = 0.1f;
	Mode = 2+4;	//init to a wireframe mode and contouring +/- values
	lOpts->GetPosColor(&PosColor);
	lOpts->GetNegColor(&NegColor);
		//Init the transparency color to opaque
	Transparency = 0;
	ContourValue = 0.1f;
	GridSize = lOpts->GetGridSize();
}
Surf3DBase::Surf3DBase(void) : Surface() {
	ContourHndl = NULL;
	List = NULL;
	Grid = NULL;
	SurfaceNormals = NULL;
	VertexList = NULL;
	GridAllocation = ContourAllocation = ListAllocation = SNormAllocation = VertexAllocation = 0;
	NumVertices = NumPosContourTriangles = NumNegContourTriangles = 0;
	GridMin = GridMax = 0;
	Mode = 0;
}
Surf3DBase::~Surf3DBase(void) {
	FreeContour();
	FreeGrid();
}
void Surf3DBase::Export(BufferFile * Buffer, exportFileType eft) {
	float * lGrid;
	lGrid = Grid;
	char * label = GetLabel();
	// our text file type
	if (eft == 0) {
		Surface::Export3D(lGrid, NumXGridPoints, NumYGridPoints, NumZGridPoints, &Origin,
			XGridInc, YGridInc, ZGridInc, label, Buffer);
	}
	// ccp4 file
	else if (eft == CCP4FILE) {
		Surface::Export3DCCP4(lGrid, NumXGridPoints, NumYGridPoints, NumZGridPoints, &Origin,
			XGridInc, YGridInc, ZGridInc, label, Buffer);
	}
	// cns file
	else if (eft == CNSFILE) {
		Surface::Export3DCNS(lGrid, NumXGridPoints, NumYGridPoints, NumZGridPoints, &Origin,
			XGridInc, YGridInc, ZGridInc, label, Buffer);
	}
		
	/* if (label) delete [] label; */
}
//Call to define the 3D volume (meaning the origin, x,y,z increments and
//the number of grid points along each axis
void Surf3DBase::SetupGridParameters(Frame * lFrame) {
	if (!GetFixGrid()) {
			//Find mins, maxs for this frame
		float	XGridMin, XGridMax, YGridMin, YGridMax, ZGridMin, ZGridMax,
				XGridSize, YGridSize, ZGridSize;
		XGridMin = YGridMin = ZGridMin = 1.0e10;
		XGridMax = YGridMax = ZGridMax = -XGridMin;
		for (int iatom=0; iatom<lFrame->NumAtoms; iatom++) {
			if (lFrame->Atoms[iatom].IsSIMOMMAtom() || lFrame->Atoms[iatom].IsEffectiveFragment())
				continue;	//Don't include MM type atoms in the surface volume since they lack basis functions
			XGridMin = MIN(XGridMin, lFrame->Atoms[iatom].Position.x*kAng2BohrConversion);
			XGridMax = MAX(XGridMax, lFrame->Atoms[iatom].Position.x*kAng2BohrConversion);
			YGridMin = MIN(YGridMin, lFrame->Atoms[iatom].Position.y*kAng2BohrConversion);
			YGridMax = MAX(YGridMax, lFrame->Atoms[iatom].Position.y*kAng2BohrConversion);
			ZGridMin = MIN(ZGridMin, lFrame->Atoms[iatom].Position.z*kAng2BohrConversion);
			ZGridMax = MAX(ZGridMax, lFrame->Atoms[iatom].Position.z*kAng2BohrConversion);
		}
		XGridSize = XGridMax-XGridMin;
		YGridSize = YGridMax-YGridMin;
		ZGridSize = ZGridMax-ZGridMin;
			//Make sure the molecule is not planar in any direction
		if (XGridSize < 2.0) {
			XGridMin -= (2.0-XGridSize)*0.5;
			XGridMax += (2.0-XGridSize)*0.5;
			XGridSize = 2.0;
		}
		if (YGridSize < 2.0) {
			YGridMin -= (2.0-YGridSize)*0.5;
			YGridMax += (2.0-YGridSize)*0.5;
			YGridSize = 2.0;
		}
		if (ZGridSize < 2.0) {
			ZGridMin -= (2.0-ZGridSize)*0.5;
			ZGridMax += (2.0-ZGridSize)*0.5;
			ZGridSize = 2.0;
		}
		XGridMin -= GridSize*XGridSize;
		XGridMax += GridSize*XGridSize;
		YGridMin -= GridSize*YGridSize;
		YGridMax += GridSize*YGridSize;
		ZGridMin -= GridSize*ZGridSize;
		ZGridMax += GridSize*ZGridSize;
		Origin.x = XGridMin;
		Origin.y = YGridMin;
		Origin.z = ZGridMin;
		XGridSize = ((XGridMax-XGridMin)+(YGridMax-YGridMin)+(ZGridMax-ZGridMin))/(3*(NumGridPoints-1));
			//Set the # of grid points in each direction to obtain equal increments along each direction
		NumXGridPoints = (long) ((XGridMax-XGridMin)/XGridSize);
		NumYGridPoints = (long) ((YGridMax-YGridMin)/XGridSize);
		NumZGridPoints = (long) ((ZGridMax-ZGridMin)/XGridSize);
		XGridInc = (XGridMax-XGridMin)/NumXGridPoints;
		YGridInc = (YGridMax-YGridMin)/NumYGridPoints;
		ZGridInc = (ZGridMax-ZGridMin)/NumZGridPoints;
		NumXGridPoints++;
		NumYGridPoints++;
		NumZGridPoints++;
	} else {
			//Convert the grid values to Bohrs
		Origin *= kAng2BohrConversion;
		XGridInc *= kAng2BohrConversion;
		YGridInc *= kAng2BohrConversion;
		ZGridInc *= kAng2BohrConversion;
	}
}
void Surf3DBase::AllocateGrid(long NumPoints) {
	if (NumPoints > 0) {
		Grid = new float[NumPoints];
		if (Grid) GridAllocation = NumPoints;
	} else {
		if (Grid) delete Grid;
		Grid = NULL;
		GridAllocation = 0;
	}
};
bool Surf3DBase::AllocateContour(long NumPoints) {
	ContourHndl = new CPoint3D[NumPoints];
	VertexList = new long[6*NumPoints];
	if (ContourHndl) ContourAllocation = NumPoints;
	if (VertexList) VertexAllocation = 6*NumPoints;
	return ((ContourHndl != NULL)&&(VertexList != NULL));
};
bool Surf3DBase::AllocateContour(long NumVertices, long NumTriangles) {
	ContourHndl = new CPoint3D[NumVertices];
	VertexList = new long[3*NumTriangles];
	if (ContourHndl) ContourAllocation = NumVertices;
	if (VertexList) VertexAllocation = 3*NumTriangles;
	return ((ContourHndl != NULL)&&(VertexList != NULL));
};
void Surf3DBase::AllocateList(long NumPoints) {
	if (NumPoints <= 0 ) NumPoints = ContourAllocation;
	List = new float[NumPoints];
	if (List) ListAllocation = NumPoints;
};
void Surf3DBase::AllocateNormals(long NumPoints) {
	if (NumPoints <= 0 ) NumPoints = ContourAllocation;
	SurfaceNormals = new CPoint3D[NumPoints];
	if (SurfaceNormals) SNormAllocation = NumPoints;
};
//The grid and contour are not rotated, just cleared. Thus the
//surface must be manually updated later.
void Surf3DBase::RotateSurface(Matrix4D /*RotationMatrix*/) {
	if (ContourHndl) FreeContour();
	if (Grid) FreeGrid();
}
void Surf3DBase::Contour3DGrid(Progress * lProgress) {
	if (!GridAvailable()) return;	//Grid must be already read in before contouring is possible
	float	*Grid3D;
	long	iXPt, iYPt, iZPt, NumTriangles;
	bool	HasPoint[12];
//	UseSurfaceNormals(true);//temporary
//	bool ComputeNormals = UseSurfaceNormals();
	bool ComputeNormals = true;
	CPoint3D	*Contour, *ContourNorm = NULL;
	long	Points[12], * PreviousYZz, * CurrentYZz, *PreviousYZy, *CurrentYZy, *PreviousZ, *CurrentZ, *tp,
			v0, v1, v2, v3, v4, v5, v6, v7;
	float	SideLength;
		//Check for and dispose of previous contours
	if (ContourHndl) FreeContour();
	NumVertices = 0;
	NumPosContourTriangles=0;
	NumNegContourTriangles=0;

		//Attempt to allocate memory for the 3D contour
	long MaxContourPts = NumXGridPoints*NumYGridPoints;
	if (ContourBothPosNeg()) MaxContourPts *= 2;
	if (!AllocateContour(MaxContourPts)) return;	//return if memory allocation failed
	if (ComputeNormals) AllocateNormals(MaxContourPts);

	if (ComputeNormals && (SurfaceNormals == NULL)) return;
		//Allocate memory for local arrays
	PreviousYZz = new long[NumYGridPoints*NumZGridPoints];
	CurrentYZz = new long[NumYGridPoints*NumZGridPoints];
	PreviousYZy = new long[NumYGridPoints*NumZGridPoints];
	CurrentYZy = new long[NumYGridPoints*NumZGridPoints];
	PreviousZ = new long[NumZGridPoints];
	CurrentZ = new long[NumZGridPoints];
	if (!PreviousYZz || !CurrentYZz || !PreviousYZy || !CurrentYZy
			|| !PreviousZ || !CurrentZ) {
		FreeContour();
		if (PreviousYZz) delete [] PreviousYZz;
		if (CurrentYZz) delete [] CurrentYZz;
		if (PreviousYZy) delete [] PreviousYZy;
		if (CurrentYZy) delete [] CurrentYZy;
		if (PreviousZ) delete [] PreviousZ;
		if (CurrentZ) delete [] CurrentZ;
		return;
	}

	Contour = ContourHndl;
	if (ComputeNormals) ContourNorm = SurfaceNormals;
	Grid3D = Grid;

	float TestPoint[8];
	
	float lContourValue = ContourValue;

	long n, nContourTriangles=0;
	for (int iround=0; iround<2; iround++) {	//Do it twice, once + and once -
		n = 0;
		float XGridValue = Origin.x;
		for (iXPt=0; iXPt<(NumXGridPoints-1); iXPt++) {
				//Give up the CPU and check for cancels
			if (!lProgress->UpdateProgress(((iround*NumXGridPoints)+iXPt)*100/(2*NumXGridPoints))) {	//User canceled so clean things up and abort
				goto Exit;	//I don't really allow cancels here, just stop at the current point
			}
			float YGridValue = Origin.y;
			for (iYPt=0; iYPt<(NumYGridPoints-1); iYPt++) {
				float ZGridValue = Origin.z;
				for (iZPt=0; iZPt<(NumZGridPoints-1); iZPt++) {
						//make sure there is enough room for the maximum number of vertices from
						//this cube in the vertexarrays
					if ((NumVertices+12)>=ContourAllocation) {
						MaxContourPts = ContourAllocation + NumGridPoints*NumGridPoints;
						CPoint3D * tempC = new CPoint3D[MaxContourPts];
						if (tempC) {
							for (long i=0; i<NumVertices; i++)
								tempC[i] = Contour[i];
							delete [] ContourHndl;
							ContourHndl = Contour = tempC;
							ContourAllocation = MaxContourPts;
						}
						if (ComputeNormals) {
							tempC = new CPoint3D[MaxContourPts];
							if (tempC) {
								for (long i=0; i<NumVertices; i++)
									tempC[i] = ContourNorm[i];
								delete [] SurfaceNormals;
								SurfaceNormals = ContourNorm = tempC;
								SNormAllocation = MaxContourPts;
							} else ContourAllocation = SNormAllocation;	//increase failed so set the contour size back
						}
						if ((NumVertices+12)>=ContourAllocation)
							goto Exit;	//Allocation failed so bail
					}
					for (int i=0; i<12; i++) HasPoint[i]=false;
					v0 = n;
					v1 = n+1;
					v2 = n+1+NumZGridPoints;
					v3 = n+NumZGridPoints;
					v4 = n+NumYGridPoints*NumZGridPoints;
					v5 = n+1+NumYGridPoints*NumZGridPoints;
					v6 = n+1+(1+NumYGridPoints)*NumZGridPoints;
					v7 = n+(1+NumYGridPoints)*NumZGridPoints;
						//Calculate the test values
					TestPoint[0] = Grid3D[n] - lContourValue;
					TestPoint[1] = Grid3D[n+1] - lContourValue;
					TestPoint[2] = Grid3D[n+1+NumZGridPoints] - lContourValue;
					TestPoint[3] = Grid3D[n+NumZGridPoints] - lContourValue;
					TestPoint[4] = Grid3D[n+NumYGridPoints*NumZGridPoints] - lContourValue;
					TestPoint[5] = Grid3D[n+1+NumYGridPoints*NumZGridPoints] - lContourValue;
					TestPoint[6] = Grid3D[n+1+(1+NumYGridPoints)*NumZGridPoints] - lContourValue;
					TestPoint[7] = Grid3D[n+(1+NumYGridPoints)*NumZGridPoints] - lContourValue;
					unsigned char	index;
					index = 0;
						//setup the triangulation index
					for (int ivertex=0; ivertex<8; ivertex++)
						if (TestPoint[ivertex] > 0.0) index += (1<<ivertex);
						//calculate gradient at each vertex
						//Setup the edge point list
					if (iXPt == 0) {
						if (iYPt == 0) {
							if ((TestPoint[0]*TestPoint[1])<0.0) {
								Contour[NumVertices].x = XGridValue;
								Contour[NumVertices].y = YGridValue;
								Contour[NumVertices].z = ZGridValue+ZGridInc*TestPoint[0]/
																(TestPoint[0]-TestPoint[1]);
								PreviousYZz[iZPt+iYPt*NumZGridPoints] = NumVertices;
								ContourNorm[NumVertices].x = ContourNorm[NumVertices].y = ContourNorm[NumVertices].z = 0.0;
								NumVertices ++;
							}
						}
						if ((TestPoint[1]*TestPoint[2])<0.0) {
							Contour[NumVertices].x = XGridValue;
							Contour[NumVertices].y = YGridValue+YGridInc*(TestPoint[1])/
															(TestPoint[1]-TestPoint[2]);
							Contour[NumVertices].z = ZGridValue+ZGridInc;
							PreviousYZy[iZPt+1+iYPt*NumZGridPoints] = NumVertices;
							ContourNorm[NumVertices].x = ContourNorm[NumVertices].y = ContourNorm[NumVertices].z = 0.0;
							NumVertices ++;
						}
						if ((TestPoint[2]*TestPoint[3])<0.0) {
							Contour[NumVertices].x = XGridValue;
							Contour[NumVertices].y = YGridValue+YGridInc;
							Contour[NumVertices].z = ZGridValue+ZGridInc*(TestPoint[3])/
															(TestPoint[3]-TestPoint[2]);
							PreviousYZz[iZPt+(iYPt+1)*NumZGridPoints] = NumVertices;
							ContourNorm[NumVertices].x = ContourNorm[NumVertices].y = ContourNorm[NumVertices].z = 0.0;
							NumVertices ++;
						}
						if (iZPt == 0) {
							if ((TestPoint[0]*TestPoint[3])<0.0) {
								Contour[NumVertices].x = XGridValue;
								Contour[NumVertices].y = YGridValue+YGridInc*TestPoint[0]/
																(TestPoint[0]-TestPoint[3]);
								Contour[NumVertices].z = ZGridValue;
								PreviousYZy[iZPt+iYPt*NumZGridPoints] = NumVertices;
								ContourNorm[NumVertices].x = ContourNorm[NumVertices].y = ContourNorm[NumVertices].z = 0.0;
								NumVertices ++;
							}
						}
					}
					if (iYPt == 0) {
						if ((TestPoint[4]*TestPoint[5])<0.0) {
							Contour[NumVertices].x = XGridValue+XGridInc;
							Contour[NumVertices].y = YGridValue;
							Contour[NumVertices].z = ZGridValue+ZGridInc*TestPoint[4]/
															(TestPoint[4]-TestPoint[5]);
							CurrentYZz[iZPt+iYPt*NumZGridPoints] = NumVertices;
							ContourNorm[NumVertices].x = ContourNorm[NumVertices].y = ContourNorm[NumVertices].z = 0.0;
							NumVertices ++;
						}
						if (iZPt == 0) {
							if ((TestPoint[0]*TestPoint[4])<0.0) {
								Contour[NumVertices].x = XGridValue+XGridInc*TestPoint[0]/
																(TestPoint[0]-TestPoint[4]);
								Contour[NumVertices].y = YGridValue;
								Contour[NumVertices].z = ZGridValue;
								PreviousZ[iZPt] = NumVertices;
								ContourNorm[NumVertices].x = ContourNorm[NumVertices].y = ContourNorm[NumVertices].z = 0.0;
								NumVertices ++;
							}
						}
						if ((TestPoint[1]*TestPoint[5])<0.0) {
							Contour[NumVertices].x = XGridValue+XGridInc*TestPoint[1]/
															(TestPoint[1]-TestPoint[5]);
							Contour[NumVertices].y = YGridValue;
							Contour[NumVertices].z = ZGridValue+ZGridInc;
							PreviousZ[iZPt+1] = NumVertices;
							ContourNorm[NumVertices].x = ContourNorm[NumVertices].y = ContourNorm[NumVertices].z = 0.0;
							NumVertices ++;
						}
					}
					if (iZPt == 0) {
						if ((TestPoint[4]*TestPoint[7])<0.0) {
							Contour[NumVertices].x = XGridValue+XGridInc;
							Contour[NumVertices].y = YGridValue+YGridInc*TestPoint[4]/
															(TestPoint[4]-TestPoint[7]);
							Contour[NumVertices].z = ZGridValue;
							CurrentYZy[iZPt+iYPt*NumZGridPoints] = NumVertices;
							ContourNorm[NumVertices].x = ContourNorm[NumVertices].y = ContourNorm[NumVertices].z = 0.0;
							NumVertices ++;
						}
						if ((TestPoint[3]*TestPoint[7])<0.0) {
							Contour[NumVertices].x = XGridValue+XGridInc*TestPoint[3]/
															(TestPoint[3]-TestPoint[7]);
							Contour[NumVertices].y = YGridValue+YGridInc;
							Contour[NumVertices].z = ZGridValue;
							CurrentZ[iZPt] = NumVertices;
							ContourNorm[NumVertices].x = ContourNorm[NumVertices].y = ContourNorm[NumVertices].z = 0.0;
							NumVertices ++;
						}
					}
					if ((TestPoint[0]*TestPoint[1])<0.0)
						Points[0] = PreviousYZz[iZPt+iYPt*NumZGridPoints];
					if ((TestPoint[1]*TestPoint[2])<0.0)
						Points[1] = PreviousYZy[iZPt+1+iYPt*NumZGridPoints];
					if ((TestPoint[2]*TestPoint[3])<0.0)
						Points[2] = PreviousYZz[iZPt+(iYPt+1)*NumZGridPoints];
					if ((TestPoint[0]*TestPoint[3])<0.0)
						Points[3] = PreviousYZy[iZPt+iYPt*NumZGridPoints];
					if ((TestPoint[4]*TestPoint[5])<0.0)
						Points[4] = CurrentYZz[iZPt+iYPt*NumZGridPoints];
					if ((TestPoint[0]*TestPoint[4])<0.0)
						Points[8] = PreviousZ[iZPt];
					if ((TestPoint[1]*TestPoint[5])<0.0)
						Points[9] = PreviousZ[iZPt+1];
					if ((TestPoint[4]*TestPoint[7])<0.0)
						Points[7] = CurrentYZy[iZPt+iYPt*NumZGridPoints];
					if ((TestPoint[3]*TestPoint[7])<0.0)
						Points[10] = CurrentZ[iZPt];

					if ((TestPoint[5]*TestPoint[6])<0.0) {
						SideLength = TestPoint[5]/(TestPoint[5]-TestPoint[6]);
						Contour[NumVertices].x = XGridValue+XGridInc;
						Contour[NumVertices].y = YGridValue+YGridInc*SideLength;
						Contour[NumVertices].z = ZGridValue+ZGridInc;
						CurrentYZy[iZPt+1+iYPt*NumZGridPoints] = NumVertices;
						Points[5] = NumVertices;
						ContourNorm[NumVertices].x = ContourNorm[NumVertices].y = ContourNorm[NumVertices].z = 0.0;
						NumVertices ++;
					}
					if ((TestPoint[6]*TestPoint[7])<0.0) {
						SideLength = TestPoint[7]/(TestPoint[7]-TestPoint[6]);
						Contour[NumVertices].x = XGridValue+XGridInc;
						Contour[NumVertices].y = YGridValue+YGridInc;
						Contour[NumVertices].z = ZGridValue+ZGridInc*SideLength;
						CurrentYZz[iZPt+(iYPt+1)*NumZGridPoints] = NumVertices;
						Points[6] = NumVertices;
						ContourNorm[NumVertices].x = ContourNorm[NumVertices].y = ContourNorm[NumVertices].z = 0.0;
						NumVertices ++;
					}
					if ((TestPoint[2]*TestPoint[6])<0.0) {
						SideLength = TestPoint[2]/(TestPoint[2]-TestPoint[6]);
						Contour[NumVertices].x = XGridValue+XGridInc*SideLength;
						Contour[NumVertices].y = YGridValue+YGridInc;
						Contour[NumVertices].z = ZGridValue+ZGridInc;
						CurrentZ[iZPt+1] = NumVertices;
						Points[11] = NumVertices;
						ContourNorm[NumVertices].x = ContourNorm[NumVertices].y = ContourNorm[NumVertices].z = 0.0;
						NumVertices ++;
					}
					if ((index>0)&&(index<255)) {
							//Check on memory and attempt to add if needed
						if ((3*nContourTriangles+30)>=VertexAllocation) {
							long NewNumVertexes = VertexAllocation + NumGridPoints*NumGridPoints;
							long * tempV = new long[NewNumVertexes];
							if (tempV) {
								for (long i=0; i<(3*nContourTriangles); i++)
									tempV[i] = VertexList[i];
								delete [] VertexList;
								VertexList = tempV;
								VertexAllocation = NewNumVertexes;
							}
							if ((3*nContourTriangles+30)>=VertexAllocation)
								goto Exit;	//Allocation failed so bail
						}
						SortContourPoints(Points, index, &(VertexList[3*nContourTriangles]), &NumTriangles);
						if (ComputeNormals)
							ComputeTriangleNormals(Contour, ContourNorm, &(VertexList[3*nContourTriangles]), NumTriangles);
						nContourTriangles += NumTriangles;
					}
					n++;
					ZGridValue += ZGridInc;
				}
				tp = PreviousZ;
				PreviousZ = CurrentZ;
				CurrentZ = tp;
				n++;
				YGridValue += YGridInc;
			}
			tp = PreviousYZz;
			PreviousYZz = CurrentYZz;
			CurrentYZz = tp;
			tp = PreviousYZy;
			PreviousYZy = CurrentYZy;
			CurrentYZy = tp;
			n+=NumZGridPoints;
			XGridValue += XGridInc;
		}
		lContourValue *= -1.0;
		if (iround==0) NumPosContourTriangles = nContourTriangles;
		else NumNegContourTriangles = (nContourTriangles) - NumPosContourTriangles;
		if (!ContourBothPosNeg()) break;
	}
Exit:
	if (ComputeNormals) NormalizeNormalArray(ContourNorm, NumVertices);
	if (PreviousYZz) delete [] PreviousYZz;	//free local storage
	if (CurrentYZz) delete [] CurrentYZz;
	if (PreviousYZy) delete [] PreviousYZy;
	if (CurrentYZy) delete [] CurrentYZy;
	if (PreviousZ) delete [] PreviousZ;
	if (CurrentZ) delete [] CurrentZ;
	return;
}
void NormalizeNormalArray(CPoint3D * ContourNorm, long &NumVertices)
{
	for (long i=0; i<NumVertices; i++) {
		CPoint3D * v = &(ContourNorm[i]);
		float length = (float) sqrt (v->x * v->x + v->y * v->y + v->z * v->z);
		if (length > 0.0) {
			v->x /= length;
			v->y /= length;
			v->z /= length;
	    } else {
	    	v->x = v->y = v->z = 0;
		}
	}
}
void ComputeTriangleNormals(CPoint3D * Contour, CPoint3D *ContourNorm, long * VertexList, long &NumTriangles)
{
		//Computes a normal to each triangle and adds it to the normal for each triangle vertex
		//The result must be normalized later
	for (long i=0; i<NumTriangles; i++) {
		long v1 = VertexList[3*i];
		long v2 = VertexList[3*i+1];
		long v3 = VertexList[3*i+2];
		
		float qx = Contour[v2].x - Contour[v1].x;
		float qy = Contour[v2].y - Contour[v1].y;
		float qz = Contour[v2].z - Contour[v1].z;
		float px = Contour[v3].x - Contour[v1].x;
		float py = Contour[v3].y - Contour[v1].y;
		float pz = Contour[v3].z - Contour[v1].z;
		float vx = -(py*qz - pz*qy);
		float vy = -(pz*qx - px*qz);
		float vz = -(px*qy - py*qx);

		float len = (float) (1.0/sqrt( vx*vx + vy*vy + vz*vz ));
		vx *= len;
		vy *= len;
		vz *= len;
		
		ContourNorm[v1].x += vx;
		ContourNorm[v1].y += vy;
		ContourNorm[v1].z += vz;
		ContourNorm[v2].x += vx;
		ContourNorm[v2].y += vy;
		ContourNorm[v2].z += vz;
		ContourNorm[v3].x += vx;
		ContourNorm[v3].y += vy;
		ContourNorm[v3].z += vz;
	}
}
void Surf3DBase::AdjustSurfaceNormals(void) {
	unsigned long	NumTriangles, TestPt, NumHits, HitArray[30];
	CPoint3D	AveVec;
	CPoint3D *	ContourNorm;
	
	if (!VertexList || !SurfaceNormals) return;
	ContourNorm = SurfaceNormals;

//	for (unsigned int pass=0; pass < 2; pass++) {
//		if (pass == 0) {
//			start = 0;
//			NumTriangles = NumPosContourTriangles;
//		} else { 
//			NumTriangles += NumNegContourTriangles;
//		}
//		NumTriangles = NumPosContourTriangles + NumNegContourTriangles;
		unsigned long start=0;
		NumTriangles = NumPosContourTriangles;
		for (TestPt=0; TestPt<NumVertices; TestPt++) {
			NumHits = 1;
			AveVec = ContourNorm[TestPt];
			HitArray[0] = TestPt;
			for (unsigned long i=0; i<NumTriangles; i++) {
				if ((VertexList[3*i] == TestPt) || (VertexList[3*i+1] == TestPt)||
					(VertexList[3*i+2] == TestPt)) {	//This triangle contains the test point
					unsigned long	PtA, PtB, PtC;
					PtA = VertexList[3*i];
					PtB = VertexList[3*i + 1];
					PtC = VertexList[3*i + 2];
					for (unsigned long j=0; j<NumHits; j++) {
						if (HitArray[j] == PtA) PtA = 0;
						if (HitArray[j] == PtB) PtB = 0;
						if (HitArray[j] == PtC) PtC = 0;
					}
					if (PtA > 0) {
						AveVec += ContourNorm[PtA];
						HitArray[NumHits] = PtA;
						NumHits++;
					}
					if (PtB > 0) {
						AveVec += ContourNorm[PtB];
						HitArray[NumHits] = PtB;
						NumHits++;
					}
					if (PtC > 0) {
						AveVec += ContourNorm[PtC];
						HitArray[NumHits] = PtC;
						NumHits++;
					}
					if (NumHits >= 28) break; //we've reached the maximum num to average over in the HitArray
				}
			}
			if (NumHits > 1) {
				AveVec.x /= NumHits;
				AveVec.y /= NumHits;
				AveVec.z /= NumHits;
				ContourNorm[TestPt] = AveVec;
			} else {	//switch to negative triangle set
				TestPt--;
				start = NumPosContourTriangles;
				NumTriangles += NumNegContourTriangles;
			}
		}
//	}
}

/* ------------------------------------------------------------------------- */

long General3DSurface::ExportPOV(MoleculeData *MainData, WinPrefs *Prefs,
								 BufferFile *Buffer) {

	long result=0;
	if (Visible) {
		if (ContourHndl && VertexList) {
			if (UseSurfaceNormals() && SurfaceNormals) {
				result = ExportPOVSurface(ContourHndl, SurfaceNormals,
										  VertexList, NumPosContourTriangles,
										  &PosColor, NULL, NULL, 1.0, MainData,
										  Buffer);
				if ((Mode & 4) && NumNegContourTriangles > 0)
					result += ExportPOVSurface(ContourHndl,
											   SurfaceNormals,
											   &VertexList[3 * NumPosContourTriangles],
											   NumNegContourTriangles,
											   &NegColor, NULL, NULL, 1.0,
											   MainData, Buffer);
			} else {
				result = ExportPOVSurface(ContourHndl, NULL, VertexList,
										  NumPosContourTriangles, &PosColor,
										  NULL, NULL, 1.0, MainData, Buffer);
				if ((Mode & 4) && NumNegContourTriangles > 0)
					result += ExportPOVSurface(ContourHndl, NULL,
											   &VertexList[3 * NumPosContourTriangles],
											   NumNegContourTriangles,
											   &NegColor, NULL, NULL, 1.0,
											   MainData, Buffer);
			}
		}
	}

	return result;

}

/* ------------------------------------------------------------------------- */

long TEDensity3DSurface::ExportPOV(MoleculeData *MainData, WinPrefs *Prefs,
								   BufferFile *Buffer) {

	long result = 0;
	if (Visible) {
		if (ContourHndl && VertexList) {
			if ((UseSurfaceNormals())&&SurfaceNormals) {
				result = ExportPOVSurface(ContourHndl, SurfaceNormals, VertexList,
										  NumPosContourTriangles,
										  &PosColor, List, &NegColor, MaxMEPValue,
										  MainData, Buffer);
			} else {
				result = ExportPOVSurface(ContourHndl, NULL, VertexList,
										  NumPosContourTriangles,
										  &PosColor, List, &NegColor, MaxMEPValue,
										  MainData, Buffer);
			}
		}
	}

	return result;

}

/* ------------------------------------------------------------------------- */

long Orb3DSurface::ExportPOV(MoleculeData *MainData, WinPrefs *Prefs,
							 BufferFile *Buffer) {

	long result=0;
	if (Visible && PlotOrb >=0) {
		if (ContourHndl && VertexList) {
			if ((UseSurfaceNormals())&&SurfaceNormals) {
				result = ExportPOVSurface(ContourHndl,
										  SurfaceNormals, VertexList, NumPosContourTriangles,
										  &PosColor, NULL, NULL, 1.0, MainData, Buffer);
				result += ExportPOVSurface(ContourHndl,
										   SurfaceNormals,
										   &(VertexList[3*NumPosContourTriangles]),
										   NumNegContourTriangles, &NegColor,
										   NULL, NULL, 1.0, MainData, Buffer);
			} else {
				result = ExportPOVSurface(ContourHndl, NULL, VertexList,
										  NumPosContourTriangles,
										  &PosColor, NULL, NULL, 1.0, MainData, Buffer);
				result += ExportPOVSurface(ContourHndl,
										   NULL, &(VertexList[3*NumPosContourTriangles]),
										   NumNegContourTriangles, &NegColor, NULL, NULL,
										   1.0, MainData, Buffer);
			}
		}
	}

	return result;

}

/* ------------------------------------------------------------------------- */

long MEP3DSurface::ExportPOV(MoleculeData *MainData, WinPrefs *Prefs,
							 BufferFile *Buffer) {

	long result=0;
	if (Visible) {
		if (ContourHndl && VertexList) {
			if (UseSurfaceNormals() && SurfaceNormals) {
				result = ExportPOVSurface(ContourHndl, SurfaceNormals,
										  VertexList, NumPosContourTriangles,
										  &PosColor, NULL, NULL, 1.0, MainData,
										  Buffer);
				result += ExportPOVSurface(ContourHndl, SurfaceNormals,
										   &(VertexList[3*NumPosContourTriangles]), 
										   NumNegContourTriangles, &NegColor, NULL,
										   NULL, 1.0, MainData, Buffer);
			} else {
				result = ExportPOVSurface(ContourHndl, NULL, VertexList, NumPosContourTriangles,
										  &PosColor, NULL, NULL, 1.0, MainData, Buffer);
				result += ExportPOVSurface(ContourHndl,
										   NULL, &(VertexList[3*NumPosContourTriangles]),
										   NumNegContourTriangles, &NegColor, NULL, NULL,
										   1.0, MainData, Buffer);
			}
		}
	}

	return result;

}

/* ------------------------------------------------------------------------- */

long Surf3DBase::ExportPOVSurface(CPoint3D *Vertices, CPoint3D *Normals,
								  long *vList, long NumTriangles,
								  RGBColor *SurfaceColor, float *SurfaceValue,
								  RGBColor *NColor, float MaxSurfaceValue,
								  MoleculeData *MainData, BufferFile *Buffer) {

	if (!NumTriangles) {
		return 0;
	}

	long v1, v2, v3, result = 0;
	float alpha = 1.0, red, green, blue, xnorm, ynorm, znorm;
	wxString tmpStr;

	red = (float) SurfaceColor->red / 65536.0f;
	green = (float) SurfaceColor->green / 65536.0f;
	blue = (float) SurfaceColor->blue / 65536.0f;
	red = MIN(red, 1.0);
	blue = MIN(blue, 1.0);
	green = MIN(green, 1.0);
	red = MAX(red, 0.0);
	blue = MAX(blue, 0.0);
	green = MAX(green, 0.0);
	long *VertexList = vList;
	myGLTriangle *transpTri;

	transpTri = new myGLTriangle[NumTriangles];

	if (isTransparent()) {
		alpha = (100 - Transparency) / 100.0f;
		result = NumTriangles;
	}

	for (long itri = 0; itri < NumTriangles; itri++) {
		v1 = VertexList[3 * itri];
		v2 = VertexList[3 * itri + 1];
		v3 = VertexList[3 * itri + 2];
		
		// Triangle 1
		if (Normals) {
			xnorm = Normals[v1].x;
			ynorm = Normals[v1].y;
			znorm = Normals[v1].z;
		} else {	//compute a simple triangle normal for all three vertices
			float qx = Vertices[v2].x - Vertices[v1].x;
			float qy = Vertices[v2].y - Vertices[v1].y;
			float qz = Vertices[v2].z - Vertices[v1].z;
			float px = Vertices[v3].x - Vertices[v1].x;
			float py = Vertices[v3].y - Vertices[v1].y;
			float pz = Vertices[v3].z - Vertices[v1].z;
			xnorm = -(py*qz - pz*qy);
			ynorm = -(pz*qx - px*qz);
			znorm = -(px*qy - py*qx);

			float len = (float) (1.0 / sqrt(xnorm * xnorm + ynorm * ynorm + znorm * znorm));
			xnorm *= len;
			ynorm *= len;
			znorm *= len;
		}

		if (SurfaceValue) {
			float temp = SurfaceValue[v1];
			temp /= MaxSurfaceValue;
			SetSurfaceColor(temp, SurfaceColor, NColor, red, green, blue);
		}

		transpTri[itri].v1 = Vertices[v1];
		transpTri[itri].n1.x = xnorm;
		transpTri[itri].n1.y = ynorm;
		transpTri[itri].n1.z = znorm;
		transpTri[itri].r1 = red;
		transpTri[itri].g1 = green;
		transpTri[itri].b1 = blue;
		transpTri[itri].a1 = alpha;

		// Triangle 2
		if (Normals) {
			xnorm = Normals[v2].x;
			ynorm = Normals[v2].y;
			znorm = Normals[v2].z;
		}

		if (SurfaceValue) {
			float temp = SurfaceValue[v2];
			temp /= MaxSurfaceValue;
			SetSurfaceColor(temp, SurfaceColor, NColor, red, green, blue);
		}

		transpTri[itri].v2 = Vertices[v2];
		transpTri[itri].n2.x = xnorm;
		transpTri[itri].n2.y = ynorm;
		transpTri[itri].n2.z = znorm;
		transpTri[itri].r2 = red;
		transpTri[itri].g2 = green;
		transpTri[itri].b2 = blue;
		transpTri[itri].a2 = alpha;
		
		// Triangle 3
		if (Normals) {
			xnorm = Normals[v3].x;
			ynorm = Normals[v3].y;
			znorm = Normals[v3].z;
		}

		if (SurfaceValue) {
			float temp = SurfaceValue[v3];
			temp /= MaxSurfaceValue;
			SetSurfaceColor(temp, SurfaceColor, NColor, red, green, blue);
		}

		transpTri[itri].v3 = Vertices[v3];
		transpTri[itri].n3.x = xnorm;
		transpTri[itri].n3.y = ynorm;
		transpTri[itri].n3.z = znorm;
		transpTri[itri].r3 = red;
		transpTri[itri].g3 = green;
		transpTri[itri].b3 = blue;
		transpTri[itri].a3 = alpha;
	}

	Buffer->PutText("mesh2 {\n");
	Buffer->PutText("\tvertex_vectors {\n");
	tmpStr.Printf(wxT("\t\t3 * %d"), NumTriangles);
	Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
	for (long i = 0; i < NumTriangles; i++) {
		tmpStr.Printf(wxT(",\n\t\t<%f, %f, %f>, <%f, %f, %f>, <%f, %f, %f>"),
					  transpTri[i].v1.x, transpTri[i].v1.y, transpTri[i].v1.z,
					  transpTri[i].v2.x, transpTri[i].v2.y, transpTri[i].v2.z,
					  transpTri[i].v3.x, transpTri[i].v3.y, transpTri[i].v3.z);
		Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
	}
	Buffer->PutText("\n\t}\n\n");
	Buffer->PutText("\tnormal_vectors {\n");
	tmpStr.Printf(wxT("\t\t3 * %d"), NumTriangles);
	Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
	for (long i = 0; i < NumTriangles; i++) {
		tmpStr.Printf(wxT(",\n\t\t<%f, %f, %f>, <%f, %f, %f>, <%f, %f, %f>"),
					  transpTri[i].n1.x, transpTri[i].n1.y, transpTri[i].n1.z,
					  transpTri[i].n2.x, transpTri[i].n2.y, transpTri[i].n2.z,
					  transpTri[i].n3.x, transpTri[i].n3.y, transpTri[i].n3.z);
		Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
	}
	Buffer->PutText("\n\t}\n\n");
	Buffer->PutText("\ttexture_list {\n");
	tmpStr.Printf(wxT("\t\t3 * %d"), NumTriangles);
	Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
	for (long i = 0; i < NumTriangles; i++) {
		tmpStr.Printf(wxT(",\n\t\ttexture {pigment {color rgbt <%f, %f, %f, %f>} finish{SurfaceFinish}},")
					  wxT("\n\t\ttexture {pigment {color rgbt <%f, %f, %f, %f>} finish{SurfaceFinish}},")
					  wxT("\n\t\ttexture {pigment {color rgbt <%f, %f, %f, %f>} finish{SurfaceFinish}}"),
					  transpTri[i].r1, transpTri[i].g1, transpTri[i].b1, 1.0f - transpTri[i].a1,
					  transpTri[i].r2, transpTri[i].g2, transpTri[i].b2, 1.0f - transpTri[i].a2,
					  transpTri[i].r3, transpTri[i].g3, transpTri[i].b3, 1.0f - transpTri[i].a3);
		Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
	}
	Buffer->PutText("\n\t}\n\n");
	Buffer->PutText("\tface_indices {\n");
	tmpStr.Printf(wxT("\t\t%d"), NumTriangles);
	Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
	for (long i = 0; i < NumTriangles * 3; i += 3) {
		tmpStr.Printf(wxT(",\n\t\t<%d, %d, %d>, %d, %d, %d"),
					  i, i + 1, i + 2, i, i + 1, i + 2);
		Buffer->PutText(tmpStr.mb_str(wxConvUTF8));
	}
	Buffer->PutText("\n\t}\n");
	Buffer->PutText("}\n");

	delete[] transpTri;

	return result;

}

/* ------------------------------------------------------------------------- */

//Routine to sort a set of points into a set of triangles
	//Definition of the triangle look-up table
typedef struct {
    int    nverts;
    int    verts[8];
    int    nedges;
    int    edges[12];
    int    npolys;
    int    polys[30];
    } CELL_ENTRY;
#include "cell_table.h"

void SortContourPoints(long *Points, unsigned char index, long *VertexList, long * NumTriangles) {
	*NumTriangles = cell_table[index].npolys;
	for (int i=0; i<*NumTriangles; i++) {
		VertexList[3*i] = Points[cell_table[index].polys[3*i]-1];
		VertexList[3*i+1] = Points[cell_table[index].polys[3*i+1]-1];
		VertexList[3*i+2] = Points[cell_table[index].polys[3*i+2]-1];
	}
}
/*
					if ((TestPoint[0]*TestPoint[1])<0.0) {
						Points[0].x = XGridValue;
						Points[0].y = YGridValue;
						Points[0].z = ZGridValue+ZGridInc*TestPoint[0]/
														(TestPoint[0]-TestPoint[1]);
						Normals[0].x = Grid3D[n] - Grid3D[n+NumYGridPoints*NumZGridPoints];
						Normals[0].y = Grid3D[n] - Grid3D[n+NumZGridPoints];
						Normals[0].z = Grid3D[n] - Grid3D[n+1];
						Normalize3D(&(Normals[0]));
					}
					if ((TestPoint[1]*TestPoint[2])<0.0) {
						Points[1].x = XGridValue;
						Points[1].y = YGridValue+YGridInc*(TestPoint[1])/
														(TestPoint[1]-TestPoint[2]);
						Points[1].z = ZGridValue+ZGridInc;
						Normals[1].x = Grid3D[n+1] - Grid3D[n+NumYGridPoints*NumZGridPoints+1];
						Normals[1].y = Grid3D[n+1] - Grid3D[n+NumZGridPoints+1];
						Normals[1].z = Grid3D[n+1] - Grid3D[n];
						Normalize3D(&(Normals[1]));
					}
					if ((TestPoint[2]*TestPoint[3])<0.0) {
						Points[2].x = XGridValue;
						Points[2].y = YGridValue+YGridInc;
						Points[2].z = ZGridValue+ZGridInc*(TestPoint[3])/
														(TestPoint[3]-TestPoint[2]);
						Normals[2].x = Grid3D[n+NumZGridPoints+1] - Grid3D[n+1+(1+NumYGridPoints)*NumZGridPoints];
						Normals[2].y = Grid3D[n+NumZGridPoints+1] - Grid3D[n+1];
						Normals[2].z = Grid3D[n+NumZGridPoints+1] - Grid3D[n+NumZGridPoints];
						Normalize3D(&(Normals[2]));
					}
					if ((TestPoint[0]*TestPoint[3])<0.0) {
						Points[3].x = XGridValue;
						Points[3].y = YGridValue+YGridInc*TestPoint[0]/
														(TestPoint[0]-TestPoint[3]);
						Points[3].z = ZGridValue;
						Normals[3].x = Grid3D[n+NumZGridPoints] - Grid3D[n+(1+NumYGridPoints)*NumZGridPoints];
						Normals[3].y = Grid3D[n+NumZGridPoints] - Grid3D[n];
						Normals[3].z = Grid3D[n+NumZGridPoints] - Grid3D[n+NumZGridPoints+1];
						Normalize3D(&(Normals[3]));
					}
					if ((TestPoint[4]*TestPoint[5])<0.0) {
						Points[4].x = XGridValue+XGridInc;
						Points[4].y = YGridValue;
						Points[4].z = ZGridValue+ZGridInc*TestPoint[4]/
														(TestPoint[4]-TestPoint[5]);
						Normals[4].x = Grid3D[n+NumYGridPoints*NumZGridPoints] - Grid3D[n];
						Normals[4].y = Grid3D[n+NumYGridPoints*NumZGridPoints] - Grid3D[n+(1+NumYGridPoints)*NumZGridPoints];
						Normals[4].z = Grid3D[n+NumYGridPoints*NumZGridPoints] - Grid3D[n+NumYGridPoints*NumZGridPoints+1];
						Normalize3D(&(Normals[4]));
					}
					if ((TestPoint[5]*TestPoint[6])<0.0) {
						Points[5].x = XGridValue+XGridInc;
						Points[5].y = YGridValue+YGridInc*TestPoint[5]/
														(TestPoint[5]-TestPoint[6]);
						Points[5].z = ZGridValue+ZGridInc;
						Normals[5].x = Grid3D[n+NumYGridPoints*NumZGridPoints+1] - Grid3D[n+1];
						Normals[5].y = Grid3D[n+NumYGridPoints*NumZGridPoints+1] - Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1];
						Normals[5].z = Grid3D[n+NumYGridPoints*NumZGridPoints+1] - Grid3D[n+NumYGridPoints*NumZGridPoints];
						Normalize3D(&(Normals[5]));
					}
					if ((TestPoint[6]*TestPoint[7])<0.0) {
						Points[6].x = XGridValue+XGridInc;
						Points[6].y = YGridValue+YGridInc;
						Points[6].z = ZGridValue+ZGridInc*TestPoint[7]/
														(TestPoint[7]-TestPoint[6]);
						Normals[6].x = Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1] - Grid3D[n+NumZGridPoints+1];
						Normals[6].y = Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1] - Grid3D[n+NumYGridPoints*NumZGridPoints+1];
						Normals[6].z = Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1] - Grid3D[n+(1+NumYGridPoints)*NumZGridPoints];
						Normalize3D(&(Normals[6]));
					}
					if ((TestPoint[4]*TestPoint[7])<0.0) {
						Points[7].x = XGridValue+XGridInc;
						Points[7].y = YGridValue+YGridInc*TestPoint[4]/
														(TestPoint[4]-TestPoint[7]);
						Points[7].z = ZGridValue;
						Normals[7].x = Grid3D[n+(1+NumYGridPoints)*NumZGridPoints] - Grid3D[n+NumZGridPoints];
						Normals[7].y = Grid3D[n+(1+NumYGridPoints)*NumZGridPoints] - Grid3D[n+NumYGridPoints*NumZGridPoints];
						Normals[7].z = Grid3D[n+(1+NumYGridPoints)*NumZGridPoints] - Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1];
						Normalize3D(&(Normals[7]));
					}
					if ((TestPoint[0]*TestPoint[4])<0.0) {
						Points[8].x = XGridValue+XGridInc*TestPoint[0]/
														(TestPoint[0]-TestPoint[4]);
						Points[8].y = YGridValue;
						Points[8].z = ZGridValue;
						Normals[8] = Normals[4];
					}
					if ((TestPoint[1]*TestPoint[5])<0.0) {
						Points[9].x = XGridValue+XGridInc*TestPoint[1]/
														(TestPoint[1]-TestPoint[5]);
						Points[9].y = YGridValue;
						Points[9].z = ZGridValue+ZGridInc;
						Normals[9] = Normals[5];
					}
					if ((TestPoint[3]*TestPoint[7])<0.0) {
						Points[10].x = XGridValue+XGridInc*TestPoint[3]/
														(TestPoint[3]-TestPoint[7]);
						Points[10].y = YGridValue+YGridInc;
						Points[10].z = ZGridValue;
						Normals[10] = Normals[7];
					}
					if ((TestPoint[2]*TestPoint[6])<0.0) {
						Points[11].x = XGridValue+XGridInc*TestPoint[2]/
														(TestPoint[2]-TestPoint[6]);
						Points[11].y = YGridValue+YGridInc;
						Points[11].z = ZGridValue+ZGridInc;
						Normals[11] = Normals[6];
					}
					if ((index>0)&&(index<255)) {
							//Check on memory and attempt to add if needed
						if ((nContourPoints+30)>=MaxContourPts) {
							MaxContourPts += NumGridPoints*NumGridPoints;
							CPoint3D * tempC = new CPoint3D[MaxContourPts];
							if (tempC) {
								for (long i=0; i<nContourPoints; i++)
									tempC[i] = Contour[i];
								delete [] ContourHndl;
								ContourHndl = Contour = tempC;
								ContourAllocation = MaxContourPts;
							} else MaxContourPts = ContourAllocation;
							needs work here
							if ((nContourPoints+30)>=MaxContourPts)
								goto Exit;	//Allocation failed so bail
						}
							//calculate gradient at each vertex
						Normals[0].x = Grid3D[n] - Grid3D[n+NumYGridPoints*NumZGridPoints];
						Normals[0].y = Grid3D[n] - Grid3D[n+NumZGridPoints];
						Normals[0].z = Grid3D[n] - Grid3D[n+1];
						if (fabs(Grid3D[n]) > fabs(Grid3D[n+NumYGridPoints*NumZGridPoints])) Normals[0].x *= -1.0;
						if (fabs(Grid3D[n]) > fabs(Grid3D[n+NumZGridPoints])) Normals[0].y *= -1.0;
						if (fabs(Grid3D[n]) > fabs(Grid3D[n+1])) Normals[0].z *= -1.0;
						if (XGridValue < 0.0) Normals[0].x *= -1.0;
						if (YGridValue < 0.0) Normals[0].y *= -1.0;
						if (ZGridValue < 0.0) Normals[0].z *= -1.0;
						Normalize3D(&(Normals[0]));
						Normals[1].x = Grid3D[n+1] - Grid3D[n+NumYGridPoints*NumZGridPoints+1];
						Normals[1].y = Grid3D[n+1] - Grid3D[n+NumZGridPoints+1];
						Normals[1].z = Grid3D[n+1] - Grid3D[n];
						if (fabs(Grid3D[n+1]) > fabs(Grid3D[n+NumYGridPoints*NumZGridPoints+1])) Normals[1].x *= -1.0;
						if (fabs(Grid3D[n+1]) > fabs(Grid3D[n+NumZGridPoints+1])) Normals[1].y *= -1.0;
						if (fabs(Grid3D[n+1]) > fabs(Grid3D[n])) Normals[1].z *= -1.0;
						if (XGridValue < 0.0) Normals[1].x *= -1.0;
						if (YGridValue < 0.0) Normals[1].y *= -1.0;
						if (ZGridValue < 0.0) Normals[1].z *= -1.0;
						Normalize3D(&(Normals[1]));
						Normals[2].x = Grid3D[n+NumZGridPoints+1] - Grid3D[n+1+(1+NumYGridPoints)*NumZGridPoints];
						Normals[2].y = Grid3D[n+NumZGridPoints+1] - Grid3D[n+1];
						Normals[2].z = Grid3D[n+NumZGridPoints+1] - Grid3D[n+NumZGridPoints];
						if (fabs(Grid3D[n+NumZGridPoints+1]) > fabs(Grid3D[n+1+(1+NumYGridPoints)*NumZGridPoints])) Normals[2].x *= -1.0;
						if (fabs(Grid3D[n+NumZGridPoints+1]) > fabs(Grid3D[n+1])) Normals[2].y *= -1.0;
						if (fabs(Grid3D[n+NumZGridPoints+1]) > fabs(Grid3D[n+NumZGridPoints])) Normals[2].z *= -1.0;
						if (XGridValue < 0.0) Normals[2].x *= -1.0;
						if (YGridValue < 0.0) Normals[2].y *= -1.0;
						if (ZGridValue < 0.0) Normals[2].z *= -1.0;
						Normalize3D(&(Normals[2]));
						Normals[3].x = Grid3D[n+NumZGridPoints] - Grid3D[n+(1+NumYGridPoints)*NumZGridPoints];
						Normals[3].y = Grid3D[n+NumZGridPoints] - Grid3D[n];
						Normals[3].z = Grid3D[n+NumZGridPoints] - Grid3D[n+NumZGridPoints+1];
						if (fabs(Grid3D[n+NumZGridPoints]) > fabs(Grid3D[n+(1+NumYGridPoints)*NumZGridPoints])) Normals[3].x *= -1.0;
						if (fabs(Grid3D[n+NumZGridPoints]) > fabs(Grid3D[n])) Normals[3].y *= -1.0;
						if (fabs(Grid3D[n+NumZGridPoints]) > fabs(Grid3D[n+NumZGridPoints+1])) Normals[3].z *= -1.0;
						if (XGridValue < 0.0) Normals[3].x *= -1.0;
						if (YGridValue < 0.0) Normals[3].y *= -1.0;
						if (ZGridValue < 0.0) Normals[3].z *= -1.0;
						Normalize3D(&(Normals[3]));
						Normals[4].x = Grid3D[n+NumYGridPoints*NumZGridPoints] - Grid3D[n];
						Normals[4].y = Grid3D[n+NumYGridPoints*NumZGridPoints] - Grid3D[n+(1+NumYGridPoints)*NumZGridPoints];
						Normals[4].z = Grid3D[n+NumYGridPoints*NumZGridPoints] - Grid3D[n+NumYGridPoints*NumZGridPoints+1];
						if (fabs(Grid3D[n+NumYGridPoints*NumZGridPoints]) > fabs(Grid3D[n])) Normals[4].x *= -1.0;
						if (fabs(Grid3D[n+NumYGridPoints*NumZGridPoints]) > fabs(Grid3D[n+(1+NumYGridPoints)*NumZGridPoints])) Nont[0]*TestPoint[3])<0.0) {
					bs(Grid3D[n+NumYGridPoints*NumZGridPoints]) > fabs(Grid3D[n+NumYGridPoints*NumZGridPoints+1])) Normals[4].z *= -1.0;
						if (XGridValue < 0.0) Normals[4].x *= -1.0;
						if (YGridValue < 0.0) Normals[4].y *= -1.0;
						if (ZGridValue < 0.0) Normals[4].z *= -1.0;
						Normalize3D(&(Normals[4]));
						Normals[5].x = Grid3D[n+NumYGridPoints*NumZGridPoints+1] - Grid3D[n+1];
						Normals[5].y = Grid3D[n+NumYGridPoints*NumZGridPoints+1] - Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1];
						Normals[5].z = Grid3D[n+NumYGridPoints*NumZGridPoints+1] - Grid3D[n+NumYGridPoints*NumZGridPoints];
						if (fabs(Grid3D[n+NumYGridPoints*NumZGridPoints+1]) > fabs(Grid3D[n+1])) Normals[5].x *= -1.0;
						if (fabs(Grid3D[n+NumYGridPoints*NumZGridPoints+1]) > fabs(Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1])) Normals[5].y *= -1.0;
						if (fabs(Grid3D[n+NumYGridPoints*NumZGridPoints+1]) > fabs(Grid3D[n+NumYGridPoints*NumZGridPoints])) Normals[5].z *= -1.0;
						if (XGridValue < 0.0) Normals[5].x *= -1.0;
						if (YGridValue < 0.0) Normals[5].y *= -1.0;
						if (ZGridValue < 0.0) Normals[5].z *= -1.0;
						Normalize3D(&(Normals[5]));
						Normals[6].x = Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1] - Grid3D[n+NumZGridPoints+1];
						Normals[6].y = Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1] - Grid3D[n+NumYGridPoints*NumZGridPoints+1];
						Normals[6].z = Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1] - Grid3D[n+(1+NumYGridPoints)*NumZGridPoints];
						if (fabs(Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1]) > fabs(Grid3D[n+NumZGridPoints+1])) Normals[6].x *= -1.0;
						if (fabs(Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1]) > fabs(Grid3D[n+NumYGridPoints*NumZGridPoints+1])) Normals[6].y *= -1.0;
						if (fabs(Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1]) > fabs(Grid3D[n+(1+NumYGridPoints)*NumZGridPoints])) Normals[6].z *= -1.0;
						if (XGridValue < 0.0) Normals[6].x *= -1.0;
						if (YGridValue < 0.0) Normals[6].y *= -1.0;
						if (ZGridValue < 0.0) Normals[6].z *= -1.0;
						Normalize3D(&(Normals[6]));
						Normals[7].x = Grid3D[n+(1+NumYGridPoints)*NumZGridPoints] - Grid3D[n+NumZGridPoints];
						Normals[7].y = Grid3D[n+(1+NumYGridPoints)*NumZGridPoints] - Grid3D[n+NumYGridPoints*NumZGridPoints];
						Normals[7].z = Grid3D[n+(1+NumYGridPoints)*NumZGridPoints] - Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1];
						if (fabs(Grid3D[n+(1+NumYGridPoints)*NumZGridPoints]) > fabs(Grid3D[n+NumZGridPoints])) Normals[7].x *= -1.0;
						if (fabs(Grid3D[n+(1+NumYGridPoints)*NumZGridPoints]) > fabs(Grid3D[n+NumYGridPoints*NumZGridPoints])) Normals[7].y *= -1.0;
						if (fabs(Grid3D[n+(1+NumYGridPoints)*NumZGridPoints]) > fabs(Grid3D[n+(1+NumYGridPoints)*NumZGridPoints+1])) Normals[7].z *= -1.0;
						if (XGridValue < 0.0) Normals[7].x *= -1.0;
						if (YGridValue < 0.0) Normals[7].y *= -1.0;
						if (ZGridValue < 0.0) Normals[7].z *= -1.0;
						Normalize3D(&(Normals[7]));
						Normals[8] = Normals[4];
						Normals[9] = Normals[5];
						Normals[10] = Normals[7];
						Normals[11] = Normals[6];

*/

