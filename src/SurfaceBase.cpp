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


Surface::Surface(void) {
	ID = 0;		//Set this to something unique
	NextSurface = NULL;
	Label = NULL;
	Visible = false;
}
Surface::~Surface(void) {
	if (Label) delete [] Label;
}
bool Surface::Needs2DPlane(void) const {return false;}
void Surface::RotateEvent(MoleculeData * /*MainData*/) {
}
char * Surface::GetLabel(void) const {
		char * ltext = NULL;
	if (Label) {
		long textLength = strlen(Label)+1;
		ltext = new char[textLength];
		strcpy(ltext, Label);
	}
	return ltext;
}
long Surface::GetSize(BufferFile * Buffer) {
	bool	cState = Buffer->GetOutput();
	Buffer->SetOutput(false);
	long length = Write(Buffer);
	Buffer->SetOutput(cState);
	return length;
}
void Surface::SetLabel(const char * NewLabel) {
	if (Label) {delete [] Label; Label = NULL;}
	if (NewLabel) {
		long length = strlen(NewLabel) + 1;
		Label = new char[length];
		if (Label) strcpy(Label, NewLabel);
	}
}
void Surface::Draw2D(MoleculeData * /*lData*/, long /*hoffset*/, long /*voffset*/, float /*scale*/) {
	//No default Drawing
}
void Surface::Update(MoleculeData * /*lData*/) {
}
void Surface::Export(BufferFile * Buffer) const {
	Buffer->Write("Export not supported for the chosen surface type", true);
}
void Surface::RotateSurface(Matrix4D /*RotationMatrix*/) {
}
void Surface::Export3D(const float * Grid3D, long nx, long ny, long nz, const CPoint3D * Origin,
	float XInc, float YInc, float ZInc, const char * Label, BufferFile * Buffer) const {
		char Line[kMaxLineLength];
	if (!Grid3D) return;
		//punch out the provided surface label
	Buffer->PutText(Label);
	Buffer->PutText("\r");
		//Write out the number of grid points in each direction
	sprintf(Line, "%d %d %d   //nx ny nz\r", nx,ny,nz);
	Buffer->PutText(Line);	//true means add a line feed
	sprintf(Line, "%g %g %g   //Origin of the 3D grid\r", Origin->x, Origin->y, Origin->z);
	Buffer->PutText(Line);
	sprintf(Line, "%g %g %g   //x increment, y inc, z inc/ grid(x(y(z)))\r", XInc, YInc, ZInc);
	Buffer->PutText(Line);
		//now write out the grid
		long	ix,iy,iz,igrid=0,ival=0;
	for (ix=0; ix < nx; ix++) {
		for (iy=0; iy < ny; iy++) {
			for (iz=0; iz < nz; iz++) {
				sprintf(Line, "%g ", Grid3D[igrid]);
				Buffer->PutText(Line);
				ival++;
				if (ival>=5) {
					Buffer->PutText("\r");
					ival=0;
				}
				igrid++;
			}
		}
	}
	if (ival) 	Buffer->PutText("\r");
}
void Surface::Export2D(const float * Grid2D, long NumPoints, const CPoint3D * Origin,
	const CPoint3D *XInc, const CPoint3D *YInc, const char * Label, BufferFile * Buffer) const {
		char Line[kMaxLineLength];
	if (!Grid2D) return;
		//punch out the provided surface label
	Buffer->PutText(Label);
	Buffer->PutText("\r");
		//Write out the number of grid points in each direction
	sprintf(Line, "%d    //# grid points\r", NumPoints);
	Buffer->PutText(Line);	//true means add a line feed
	sprintf(Line, "%g %g %g   //Origin of the 2D grid\r", Origin->x, Origin->y, Origin->z);
	Buffer->PutText(Line);
	sprintf(Line, "%g %g %g   //X inc vector\r", XInc->x, XInc->y, XInc->z);
	Buffer->PutText(Line);
	sprintf(Line, "%g %g %g   //Y inc vector\r", YInc->x, YInc->y, YInc->z);
	Buffer->PutText(Line);
		//now write out the grid
		long	ix,iy,igrid=0,ival=0;
	for (ix=0; ix < NumPoints; ix++) {
		for (iy=0; iy < NumPoints; iy++) {
			sprintf(Line, "%g ", Grid2D[igrid]);
			Buffer->PutText(Line);
			ival++;
			if (ival>=5) {
				Buffer->PutText("\r");
				ival=0;
			}
			igrid++;
		}
	}
	if (ival) Buffer->PutText("\r");
}
long Surface::GetSizeofSurface(BufferFile * Buffer) {
	bool	cState = Buffer->GetOutput();
	Buffer->SetOutput(false);
	long length = WriteSurface(Buffer);
	Buffer->SetOutput(cState);
	return length;
}
long Surface::WriteSurface(BufferFile * Buffer) {
	long code, length, total;

	code = 100;
	total = Buffer->Write((Ptr) &code, sizeof(long));
	
	total += Buffer->Write((Ptr) &ID, sizeof(long));
	total += Buffer->Write((Ptr) &Visible, sizeof(Boolean));
	if (Label) {
		code = 101;
		total += Buffer->Write((Ptr) &code, sizeof(long));
		length = strlen(Label)+1;
		total += Buffer->Write((Ptr) &length, sizeof(long));
		total += Buffer->Write((Ptr) Label, length);
	}
	return total;
}
long Surface::Read(BufferFile * Buffer, long ObjectLength) {
	long code, length, total;

	total = Buffer->Read((Ptr) &code, sizeof(long));
	if (code != 100) return total;

	total += Buffer->Read((Ptr) &ID, sizeof(long));
	total += Buffer->Read((Ptr) &Visible, sizeof(Boolean));
	if (total < ObjectLength) {
		total += Buffer->Read((Ptr) &code, sizeof(long));
		if (code != 101) return total;
		total += Buffer->Read((Ptr) &length, sizeof(long));
		Label = new char[length];
		if (Label)
			total += Buffer->Read((Ptr) Label, length);
	}
	return total;
}
long OrbSurfBase::GetSizeofSurface(BufferFile * Buffer) const {
	bool	cState = Buffer->GetOutput();
	Buffer->SetOutput(false);
	long length = OrbSurfBase::Write(Buffer);
	Buffer->SetOutput(cState);
	return length;
}
long OrbSurfBase::Write(BufferFile * Buffer) const {
	long length, total=0;

	length = 2;
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += Buffer->Write((Ptr) &PlotOrb, sizeof(long));
	total += Buffer->Write((Ptr) &TargetSet, sizeof(long));
	total += Buffer->Write((Ptr) &Options, sizeof(long));
	return total;
}
long OrbSurfBase::Read(BufferFile * Buffer, long ObjectLength) {
	long code, total=0;

	total += Buffer->Read((Ptr) &code, sizeof(long));
	if ((code != 1)&&(code != 2)) return total;	//Only difference in 1 vs 2 is the addition of TargetSet
	if ((code==1) && (ObjectLength != 3*sizeof(long))) return total;
	if ((code==2) && (ObjectLength != 4*sizeof(long))) return total;
	total += Buffer->Read((Ptr) &PlotOrb, sizeof(long));
	if (code != 1) total += Buffer->Read((Ptr) &TargetSet, sizeof(long));
	total += Buffer->Read((Ptr) &Options, sizeof(long));
	if (code==1) {	//can we interpert Options into TargetSet?
		if (Options & 2) TargetSet = 0;
	}
	return total;
}

/* member functions for the base 2D surface class */

Surf2DBase::Surf2DBase(WinPrefs * Prefs) {
	Grid = NULL;
	GridAllocation = 0;
	Origin.x = Origin.y = Origin.z = 0.0;
	XInc.x = 0.1; XInc.y=XInc.z=0.0;
	YInc.x=YInc.z=0.0; YInc.y=0.1;

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
void Surf2DBase::Export(BufferFile * Buffer) const {
	float * lGrid;
	char * label = GetLabel();
#ifdef UseHandles
	HLock(Grid);
	lGrid = (float *) *Grid;
#else
	lGrid = Grid;
#endif
	Surface::Export2D(lGrid, NumGridPoints, &Origin, &XInc, &YInc, label, Buffer);
	if (label) delete [] label;
#ifdef UseHandles
	HUnlock(Grid);
#endif
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
	TestPoint.x = -0.5*MainData->WindowSize;
	TestPoint.y=-0.5*MainData->WindowSize; TestPoint.z=0.0;
	BackRotate3DPt(RotInverse, TestPoint, &NewPoint1);
	TestPoint.x = 0.5*MainData->WindowSize;
	TestPoint.y=-0.5*MainData->WindowSize;
	BackRotate3DPt(RotInverse, TestPoint, &NewPoint2);
//	Origin.x = NewPoint1.x*kAng2BohrConversion;
//	Origin.y = NewPoint1.y*kAng2BohrConversion;
//	Origin.z = NewPoint1.z*kAng2BohrConversion;
	Origin.x = NewPoint1.x;
	Origin.y = NewPoint1.y;
	Origin.z = NewPoint1.z;
	long NumPoints = NumGridPoints-1;	//Subtract one such that the window will be spanned by NumPoints
//	XInc.x = (NewPoint2.x-NewPoint1.x)*kAng2BohrConversion/NumPoints;
//	XInc.y = (NewPoint2.y-NewPoint1.y)*kAng2BohrConversion/NumPoints;
//	XInc.z = (NewPoint2.z-NewPoint1.z)*kAng2BohrConversion/NumPoints;
	XInc.x = (NewPoint2.x-NewPoint1.x)/NumPoints;
	XInc.y = (NewPoint2.y-NewPoint1.y)/NumPoints;
	XInc.z = (NewPoint2.z-NewPoint1.z)/NumPoints;
	TestPoint.x = -0.5*MainData->WindowSize;
	TestPoint.y=0.5*MainData->WindowSize;
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
#ifdef UseHandles
	HLock(Grid);
	lGrid = (float *) *Grid;
#else
	lGrid = Grid;
#endif

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
#ifdef UseHandles
	HUnlock(Grid);
#endif
	RGBForeColor(&savecolor);
#endif	//end of wxBuild ifdef
}

/* member functions for the base 3D surface class */

Surf3DBase::Surf3DBase(WinPrefs * Prefs) {
	ContourHndl = NULL;
#ifdef UseHandles
	VertexHndl = NULL;
#else
	VertexList = NULL;
#endif
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
	XGridInc = YGridInc = ZGridInc = 0.1;
	Mode = 2+4;	//init to a wireframe mode and contouring +/- values
	lOpts->GetPosColor(&PosColor);
	lOpts->GetNegColor(&NegColor);
		//Init the transparency color to opaque
	TranspColor.red = TranspColor.green = TranspColor.blue = 65535;
	ContourValue = 0.1;
	GridSize = lOpts->GetGridSize();
}
Surf3DBase::Surf3DBase(void) : Surface() {
	ContourHndl = NULL;
	List = NULL;
	Grid = NULL;
	SurfaceNormals = NULL;
#ifdef UseHandles
	VertexHndl = NULL;
#else
	VertexList = NULL;
#endif
	GridAllocation = ContourAllocation = ListAllocation = SNormAllocation = VertexAllocation = 0;
	NumVertices = NumPosContourTriangles = NumNegContourTriangles = 0;
	GridMin = GridMax = 0;
	Mode = 0;
}
Surf3DBase::~Surf3DBase(void) {
	FreeContour();
	FreeGrid();
}
void Surf3DBase::Export(BufferFile * Buffer) const {
	float * lGrid;
#ifdef UseHandles
	HLock(Grid);
	lGrid = (float *) *Grid;
#else
	lGrid = Grid;
#endif
	char * label = GetLabel();
	Surface::Export3D(lGrid, NumXGridPoints, NumYGridPoints, NumZGridPoints, &Origin,
		XGridInc, YGridInc, ZGridInc, label, Buffer);
	if (label) delete [] label;
#ifdef UseHandles
	HUnlock(Grid);
#endif
}
long Surf3DBase::Read3D(BufferFile * Buffer, long ObjectLength) {
	long code, length, total=0;

	while (total < ObjectLength) {
		total += Buffer->Read((Ptr) &code, sizeof(long));
		switch (code) {
			case 1:	//Base surface items
				total += Buffer->Read((Ptr) &length, sizeof(long));
				code = Buffer->GetFilePos();
				total += Surface::Read(Buffer, length);
				Buffer->SetFilePos(code + length);
			break;
			case 2:
				total += Buffer->Read((Ptr) &GridMax, sizeof(float));
				total += Buffer->Read((Ptr) &GridMin, sizeof(float));
				total += Buffer->Read((Ptr) &Origin, sizeof(CPoint3D));
				total += Buffer->Read((Ptr) &XGridInc, sizeof(float));
				total += Buffer->Read((Ptr) &YGridInc, sizeof(float));
				total += Buffer->Read((Ptr) &ZGridInc, sizeof(float));
				total += Buffer->Read((Ptr) &NumXGridPoints, sizeof(long));
				total += Buffer->Read((Ptr) &NumYGridPoints, sizeof(long));
				total += Buffer->Read((Ptr) &NumZGridPoints, sizeof(long));
				total += Buffer->Read((Ptr) &NumGridPoints, sizeof(long));
				total += Buffer->Read((Ptr) &Mode, sizeof(long));
				total += Buffer->Read((Ptr) &PosColor, sizeof(RGBColor));
				total += Buffer->Read((Ptr) &NegColor, sizeof(RGBColor));
				total += Buffer->Read((Ptr) &TranspColor, sizeof(RGBColor));
				total += Buffer->Read((Ptr) &ContourValue, sizeof(float));
				total += Buffer->Read((Ptr) &GridSize, sizeof(float));
			break;
			case 3:
				length = GridAllocation * sizeof(float);
				total += Buffer->Read((Ptr) &length, sizeof(long));
				AllocateGrid(length/sizeof(float));
				if (Grid) {
#ifdef UseHandles
					HLock(Grid);
					total += Buffer->Read((Ptr) *Grid, length);
					HUnlock(Grid);
#else
					total += Buffer->Read((Ptr) Grid, length);
#endif
				} else Buffer->BufferSkip(length);
			break;
			case 4:
				total += Buffer->Read((Ptr) &NumVertices, sizeof(long));
				total += Buffer->Read((Ptr) &NumPosContourTriangles, sizeof(long));
				total += Buffer->Read((Ptr) &NumNegContourTriangles, sizeof(long));
				if (AllocateContour(NumVertices, NumPosContourTriangles+NumNegContourTriangles)) {
#ifdef UseHandles
					HLock(ContourHndl);
					total += Buffer->Read((Ptr) *ContourHndl, NumVertices*sizeof(CPoint3D));
					HUnlock(ContourHndl);
					HLock(VertexHndl);
					length = 3*(NumPosContourTriangles + NumNegContourTriangles)*sizeof(long);
					total += Buffer->Read((Ptr) *VertexHndl, length);
					HUnlock(VertexHndl);
#else
					total += Buffer->Read((Ptr) ContourHndl, NumVertices*sizeof(CPoint3D));
					length = 3*(NumPosContourTriangles + NumNegContourTriangles)*sizeof(long);
					total += Buffer->Read((Ptr) VertexList, length);
#endif
				} else Buffer->BufferSkip(NumVertices*sizeof(CPoint3D) +
					3*(NumPosContourTriangles + NumNegContourTriangles)*sizeof(long));
			break;
			case 5:
				total += Buffer->Read((Ptr) &length, sizeof(long));
				if (length != (NumVertices * sizeof(CPoint3D))) throw DataError();
				AllocateNormals(NumVertices);
				if (SurfaceNormals) {
#ifdef UseHandles
					HLock(SurfaceNormals);
					total += Buffer->Read((Ptr) *SurfaceNormals, length);
					HUnlock(SurfaceNormals);
#else
					total += Buffer->Read((Ptr) SurfaceNormals, length);
#endif
				} else Buffer->BufferSkip(length);
			break;
			case 6:
				total += Buffer->Read((Ptr) &length, sizeof(long));
				if (length != (NumVertices*sizeof(float))) throw DataError();
				AllocateList(NumVertices);
				if (List) {
#ifdef UseHandles
					HLock(List);
					total += Buffer->Read((Ptr) *List, length);
					HUnlock(List);
#else
					total += Buffer->Read((Ptr) List, length);
#endif
				} else Buffer->BufferSkip(length);
			break;
		}
	}
	return total;
}
long Surf3DBase::GetSizeofSurface(BufferFile * Buffer, bool WriteGrid) {
	bool	cState = Buffer->GetOutput();
	Buffer->SetOutput(false);
	long length = Write3D(Buffer, WriteGrid);
	Buffer->SetOutput(cState);
	return length;
}
long Surf3DBase::Write3D(BufferFile * Buffer, bool WriteGrid) {
	long code, length, total=0;

	length = 1;	//first write out the base class
	total += Buffer->Write((Ptr) &length, sizeof(long));
	length = Surface::GetSizeofSurface(Buffer);
	total += Buffer->Write((Ptr) &length, sizeof(long));
	total += WriteSurface(Buffer);

	code = 2;	//now surf3d parameters
	total += Buffer->Write((Ptr) &code, sizeof(long));
	total += Buffer->Write((Ptr) &GridMax, sizeof(float));
	total += Buffer->Write((Ptr) &GridMin, sizeof(float));
	total += Buffer->Write((Ptr) &Origin, sizeof(CPoint3D));
	total += Buffer->Write((Ptr) &XGridInc, sizeof(float));
	total += Buffer->Write((Ptr) &YGridInc, sizeof(float));
	total += Buffer->Write((Ptr) &ZGridInc, sizeof(float));
	total += Buffer->Write((Ptr) &NumXGridPoints, sizeof(long));
	total += Buffer->Write((Ptr) &NumYGridPoints, sizeof(long));
	total += Buffer->Write((Ptr) &NumZGridPoints, sizeof(long));
	total += Buffer->Write((Ptr) &NumGridPoints, sizeof(long));
	total += Buffer->Write((Ptr) &Mode, sizeof(long));
	total += Buffer->Write((Ptr) &PosColor, sizeof(RGBColor));
	total += Buffer->Write((Ptr) &NegColor, sizeof(RGBColor));
	total += Buffer->Write((Ptr) &TranspColor, sizeof(RGBColor));
	total += Buffer->Write((Ptr) &ContourValue, sizeof(float));
	total += Buffer->Write((Ptr) &GridSize, sizeof(float));
		//Now for contours and grids
	if (Grid && WriteGrid) {
		code = 3;
		total += Buffer->Write((Ptr) &code, sizeof(long));
		length = GridAllocation * sizeof(float);
		total += Buffer->Write((Ptr) &length, sizeof(long));
#ifdef UseHandles
		HLock(Grid);
		total += Buffer->Write((Ptr) *Grid, length);
		HUnlock(Grid);
#else
		total += Buffer->Write((Ptr) Grid, length);
#endif
	}
	if (ContourHndl) {
		code = 4;
		total += Buffer->Write((Ptr) &code, sizeof(long));
			//the # of vertices and triangles is only applicable if a contour is being saved
		total += Buffer->Write((Ptr) &NumVertices, sizeof(long));
		total += Buffer->Write((Ptr) &NumPosContourTriangles, sizeof(long));
		total += Buffer->Write((Ptr) &NumNegContourTriangles, sizeof(long));
#ifdef UseHandles
		HLock(ContourHndl);
		total += Buffer->Write((Ptr) *ContourHndl, NumVertices*sizeof(CPoint3D));
		HUnlock(ContourHndl);
		HLock(VertexHndl);
		length = 3*(NumPosContourTriangles + NumNegContourTriangles)*sizeof(long);
		total += Buffer->Write((Ptr) *VertexHndl, length);
		HUnlock(VertexHndl);
#else
		total += Buffer->Write((Ptr) ContourHndl, NumVertices*sizeof(CPoint3D));
		length = 3*(NumPosContourTriangles + NumNegContourTriangles)*sizeof(long);
		total += Buffer->Write((Ptr) VertexList, length);
#endif
		if (SurfaceNormals) {
			code = 5;
			total += Buffer->Write((Ptr) &code, sizeof(long));
			length = NumVertices * sizeof(CPoint3D);
			total += Buffer->Write((Ptr) &length, sizeof(long));
	#ifdef UseHandles
			HLock(SurfaceNormals);
			total += Buffer->Write((Ptr) *SurfaceNormals, length);
			HUnlock(SurfaceNormals);
	#else
			total += Buffer->Write((Ptr) SurfaceNormals, length);
	#endif
		}
		if (List) {
			code = 6;
			total += Buffer->Write((Ptr) &code, sizeof(long));
			length = NumVertices * sizeof(float);
			total += Buffer->Write((Ptr) &length, sizeof(long));
	#ifdef UseHandles
			HLock(List);
			total += Buffer->Write((Ptr) *List, length);
			HUnlock(List);
	#else
			total += Buffer->Write((Ptr) List, length);
	#endif
		}
	}
	return total;
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
#ifdef UseHandles	//The main point of using handles here is to use temp memory out of the system heap
	OSErr	myErr;
	Grid = TempNewHandle(NumPoints*sizeof(float), &myErr);
	if (Grid == NULL) {	//Try getting memory out of my heap
		Grid = NewHandle(NumPoints*sizeof(float));
	}
#else
	Grid = new float[NumPoints];
#endif
	if (Grid) GridAllocation = NumPoints;
};
bool Surf3DBase::AllocateContour(long NumPoints) {
#ifdef UseHandles	//The main point of using handles here is to use temp memory out of the system heap
	OSErr	myErr;
	ContourHndl = TempNewHandle(NumPoints*sizeof(CPoint3D), &myErr);
	if (ContourHndl == NULL) {	//Try getting memory out of my heap
		ContourHndl = NewHandle(NumPoints*sizeof(CPoint3D));
	}
	VertexHndl = TempNewHandle(6*NumPoints*sizeof(long), &myErr);
	if (VertexHndl == NULL) {	//Try getting memory out of my heap
		VertexHndl = NewHandle(6*NumPoints*sizeof(long));
	}
	if (ContourHndl) ContourAllocation = NumPoints;
	if (VertexHndl) VertexAllocation = 6*NumPoints;
	return ((ContourHndl != NULL)&&(VertexHndl != NULL));
#else
	ContourHndl = new CPoint3D[NumPoints];
	VertexList = new long[6*NumPoints];
	if (ContourHndl) ContourAllocation = NumPoints;
	if (VertexList) VertexAllocation = 6*NumPoints;
	return ((ContourHndl != NULL)&&(VertexList != NULL));
#endif
};
bool Surf3DBase::AllocateContour(long NumVertices, long NumTriangles) {
#ifdef UseHandles	//The main point of using handles here is to use temp memory out of the system heap
	OSErr	myErr;
	ContourHndl = TempNewHandle(NumVertices*sizeof(CPoint3D), &myErr);
	if (ContourHndl == NULL) {	//Try getting memory out of my heap
		ContourHndl = NewHandle(NumVertices*sizeof(CPoint3D));
	}
	VertexHndl = TempNewHandle(3*NumTriangles*sizeof(long), &myErr);
	if (VertexHndl == NULL) {	//Try getting memory out of my heap
		VertexHndl = NewHandle(3*NumTriangles*sizeof(long));
	}
	if (ContourHndl) ContourAllocation = NumVertices;
	if (VertexHndl) VertexAllocation = 3*NumTriangles;
	return ((ContourHndl != NULL)&&(VertexHndl != NULL));
#else
	ContourHndl = new CPoint3D[NumVertices];
	VertexList = new long[3*NumTriangles];
	if (ContourHndl) ContourAllocation = NumVertices;
	if (VertexList) VertexAllocation = 3*NumTriangles;
	return ((ContourHndl != NULL)&&(VertexList != NULL));
#endif
};
void Surf3DBase::AllocateList(long NumPoints) {
	if (NumPoints <= 0 ) NumPoints = ContourAllocation;
#ifdef UseHandles	//The main point of using handles here is to use temp memory out of the system heap
	OSErr	myErr;
	List = TempNewHandle(NumPoints*sizeof(float), &myErr);
	if (List == NULL) {	//Try getting memory out of my heap
		List = NewHandle(NumPoints*sizeof(float));
	}
#else
	List = new float[NumPoints];
#endif
	if (List) ListAllocation = NumPoints;
};
void Surf3DBase::AllocateNormals(long NumPoints) {
	if (NumPoints <= 0 ) NumPoints = ContourAllocation;
#ifdef UseHandles	//The main point of using handles here is to use temp memory out of the system heap
	OSErr	myErr;
	SurfaceNormals = TempNewHandle(NumPoints*sizeof(CPoint3D), &myErr);
	if (SurfaceNormals == NULL) {	//Try getting memory out of my heap
		SurfaceNormals = NewHandle(NumPoints*sizeof(CPoint3D));
	}
#else
	SurfaceNormals = new CPoint3D[NumPoints];
#endif
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

#ifdef UseHandles
	HLock(ContourHndl);
	Contour = (CPoint3D *) *ContourHndl;
	HLock(VertexHndl);
	long * VertexList = (long *) *VertexHndl;
	if (ComputeNormals) {
		HLock(SurfaceNormals);
		ContourNorm = (CPoint3D *) *SurfaceNormals;
	}
	HLock(Grid);
	Grid3D = (float *) *Grid;
#else
	Contour = ContourHndl;
	if (ComputeNormals) ContourNorm = SurfaceNormals;
	Grid3D = Grid;
#endif

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
#ifdef UseHandles
						HUnlock(ContourHndl);	//unlock to make it easier to move blocks of memory
						SetHandleSize(ContourHndl, MaxContourPts*sizeof(CPoint3D));//Attempt to resize the handle
						HLock(ContourHndl);
						Contour = (CPoint3D *) *ContourHndl;
						if (ComputeNormals) {
							HUnlock(SurfaceNormals);	//unlock to make it easier to move blocks of memory
							SetHandleSize(SurfaceNormals, MaxContourPts*sizeof(CPoint3D));//Attempt to resize the handle
							HLock(SurfaceNormals);
							ContourNorm = (CPoint3D *) *SurfaceNormals;
							ContourAllocation = (GetHandleSize(SurfaceNormals)/sizeof(CPoint3D));
							SNormAllocation = ContourAllocation;
						} else
							ContourAllocation = (GetHandleSize(ContourHndl)/sizeof(CPoint3D));
#else
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
#endif
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
#ifdef UseHandles
							HUnlock(VertexHndl);	//unlock to make it easier to move blocks of memory
							SetHandleSize(VertexHndl, NewNumVertexes*sizeof(long));//Attempt to resize the handle
							HLock(VertexHndl);
							VertexList = (long *) *VertexHndl;
							VertexAllocation = (GetHandleSize(VertexHndl)/sizeof(long));
#else
							long * tempV = new long[NewNumVertexes];
							if (tempV) {
								for (long i=0; i<(3*nContourTriangles); i++)
									tempV[i] = VertexList[i];
								delete [] VertexList;
								VertexList = tempV;
								VertexAllocation = NewNumVertexes;
							}
#endif
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
#ifdef UseHandles
	HUnlock(Grid);	
	HUnlock(ContourHndl);
	HUnlock(VertexHndl);
	if (ComputeNormals) HUnlock(SurfaceNormals);
		//Trim down any extra memory on the contour handles
	SetHandleSize(ContourHndl, NumVertices*sizeof(CPoint3D));
	SetHandleSize(VertexHndl, 3*nContourTriangles*sizeof(long));
	if (ComputeNormals) SetHandleSize(SurfaceNormals, NumVertices*sizeof(CPoint3D));
#endif
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
		float length = sqrt (v->x * v->x + v->y * v->y + v->z * v->z);
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

		float len = 1.0/sqrt( vx*vx + vy*vy + vz*vz );
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
	
#ifdef UseHandles
	if (!VertexHndl || !SurfaceNormals) return;
	HLock(VertexHndl);
	long * VertexList = (long *) *VertexHndl;
	HLock(SurfaceNormals);
	ContourNorm = (CPoint3D *) *SurfaceNormals;
#else
	if (!VertexList || !SurfaceNormals) return;
	ContourNorm = SurfaceNormals;
#endif

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
#ifdef UseHandles
	HUnlock(VertexHndl);
	HUnlock(SurfaceNormals);
#endif
}

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
#ifdef UseHandles
							HUnlock(ContourHndl);	//unlock to make it easier to move blocks of memory
							SetHandleSize(ContourHndl, MaxContourPts*sizeof(CPoint3D));//Attempt to resize the handle
							HLock(ContourHndl);
							Contour = (CPoint3D *) *ContourHndl;
							HUnlock(SurfaceNormals);	//unlock to make it easier to move blocks of memory
							SetHandleSize(SurfaceNormals, MaxContourPts*sizeof(CPoint3D));//Attempt to resize the handle
							HLock(SurfaceNormals);
							ContourNorm = (CPoint3D *) *SurfaceNormals;
							MaxContourPts = (GetHandleSize(SurfaceNormals)/sizeof(CPoint3D));
#else
							CPoint3D * tempC = new CPoint3D[MaxContourPts];
							if (tempC) {
								for (long i=0; i<nContourPoints; i++)
									tempC[i] = Contour[i];
								delete [] ContourHndl;
								ContourHndl = Contour = tempC;
								ContourAllocation = MaxContourPts;
							} else MaxContourPts = ContourAllocation;
							needs work here
#endif
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
						if (fabs(Grid3D[n+NumYGridPoints*NumZGridPoints]) > fabs(Grid3D[n+(1+NumYGridPoints)*NumZGridPoints])) Normals[4].y *= -1.0;
						if (fabs(Grid3D[n+NumYGridPoints*NumZGridPoints]) > fabs(Grid3D[n+NumYGridPoints*NumZGridPoints+1])) Normals[4].z *= -1.0;
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