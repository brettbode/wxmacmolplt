/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	SurfaceTypes.h
	Class definitions for specific surface types

	Pulled out of MyTypes 9/97 BMB
	Added spherical harmonics flags 2/99 BMB
	Added seperate flag for MO energy versus orb occupation. 8/2000 BMB
	Added OpenGL version of the Draw3D call. 6/2001 BMB
*/
#ifndef __SurfaceTypes__
#define __SurfaceTypes__
#ifndef __MyTypes__
#include "MyTypes.h"
#endif
#include <iostream>

class OrbSurfBase {
	protected:
		long		PlotOrb;
		long		TargetSet;
		long		Options;//bits 0-4 for orb options, bit 5 for phase, bit 6 for spherical harmonics
							//bit 7 for Energy or Occ #
							//bit 0 - AO's, bit 1 - EigenVectors, bit 2 - Local orbs, bit 3 - oriented
							//local orbitals, bit 4 - selects alpha or beta orbitals
	public:
		OrbSurfBase(void) {PlotOrb=-1; TargetSet = -1; Options=0;};
		inline long GetTargetOrb(void) const {return PlotOrb;};
		inline void SetTargetOrb(long NewTarget) {PlotOrb = NewTarget;};
		inline long GetTargetSet(void) const { return TargetSet;};
		inline void SetTargetSet(long target) {TargetSet = target;};
		inline bool UseAOs(void) const {return (Options & 1);};
		inline void UseAOs(bool newval) {Options = (Options & 0xFFFFFFFE) + (newval ? 1 : 0);};
		inline bool UseBetaSet(void) const {return ((Options & 16) != 0);};
		inline void UseBetaSet(bool newval) {Options = (Options & 0xFFFFFFEF) + (newval ? 16 : 0);};
		inline long GetOptions(void) const {return (Options & 0x1F);};
		inline void SetOptions(long NewVal) {Options = (NewVal & 0x1F) + (Options & 0xFFFFFFE0);};
		inline bool GetPhaseChange(void) const {return ((Options & 32) != 0);};
		inline void SetPhaseChange(bool newState) {Options = (Options & 0xFFFFFFDF) + (newState ? 32 : 0);};
		inline bool UseSphericalHarmonics(void) const {return ((Options & 64) != 0);};
		inline void UseSphericalHarmonics(bool newState) {Options = (Options & 0xFFFFFFBF) + (newState ? 64 : 0);};
		inline bool GetOrbOccDisplay(void) const {return ((Options & 128) != 0);};
		inline void SetOrbOccDisplay(bool newState) {Options = (Options & 0xFFFFFF7F) + (newState ? 128 : 0);};
		void ReduceOrbitalVector(const mpAtom * const Atoms, std::vector<CPoint3D> & reducedAtomPos, std::vector<short> & shellsPerAtom, std::vector<float> & reducedVector, const float * const MOVector) const;
		virtual void WriteXML(XMLElement * parent) const;
		virtual void ReadXML(XMLElement * parent);
};

class Surf1DBase : public Surface {
	protected:
		float *		Grid;
		long		GridAllocation;
		long		NumGridPoints;
		RGBColor	PosColor;
		RGBColor	NegColor;
		float		MaxContourValue;
		float       Scale;
		float		GridMax, GridMin;
		
		// bit field:
		// 	  bit 0 -> both + and - contours
		long		SurfOptions;
	public:
		Surf1DBase(void);
		Surf1DBase(WinPrefs *);
		virtual ~Surf1DBase(void);	
		inline short GetDimensions(void) const {return 1;};
		inline void GetPosColor(RGBColor *temp) const {*temp=PosColor;};
		inline void SetPosColor(const RGBColor *newColor) {PosColor=*newColor;};
		inline void GetNegColor(RGBColor *temp) const {*temp=NegColor;};
		inline void SetNegColor(const RGBColor *newColor) {NegColor=*newColor;};
		inline long GetNumGridPoints(void) const {return NumGridPoints;};
		void SetNumGridPoints(long newNum);
		inline bool GridAvailable(void) const {return (Grid!=NULL);};
		virtual bool ExportPossible(void) const {return GridAvailable();};
		/**
		 * Exports surface data to our text file type	
		 * @param Buffer A BufferFileObject
		 * @param eft File type selector (currently unused)
		 */
		virtual void Export(BufferFile * Buffer, exportFileType eft);
		inline bool ContourBothPosNeg(void) const {return (SurfOptions & 1) != 0;};
		inline void SetContourBothPosNeg(bool NewVal) { SurfOptions = (SurfOptions & ~(1 << 1)) | (((int) NewVal) << 1); }
		inline float GetMaxValue(void) const { return MaxContourValue; }
		inline void SetMaxValue(float newVal) {MaxContourValue=newVal;};
		inline float GetScale(void) const { return Scale; }
		inline void SetScale(float newVal) {Scale=newVal;};
		void Write1DXML(XMLElement *parent, bool writeGrid) const;
		void Read1DXML(XMLElement *sxml);
		long Draw3DGL(MoleculeData * lData, WinPrefs * Prefs, myGLTriangle *,
					  unsigned int shader_program = 0);
		inline void FreeGrid(void) {
			if (Grid) {
				delete [] Grid;
				Grid=NULL;
			}
			GridAllocation = 0;
		};
		void AllocateGrid(long NumPoints) {
			if (NumPoints > 0) {
				Grid = new float[NumPoints];
				if (Grid) GridAllocation = NumPoints;
			} else {
				if (Grid) delete Grid;
				Grid = NULL;
				GridAllocation = 0;
			}
		};
		CPoint3D Start;
		CPoint3D End;
};

class Surf2DBase : public Surface {
	protected:
		float *		Grid;
		long		GridAllocation;
		CPoint3D	Origin;
		CPoint3D	XInc;
		CPoint3D	YInc;
		float		GridMax, GridMin;
		long		NumGridPoints;
		RGBColor	PosColor;
		RGBColor	NegColor;
		long		NumContours;
		float		MaxContourValue;
		long		SurfOptions;	//bit field: 1 for zero contour, 2 for Screen Plane, 3 for dashed contours,
									//4 for contouring both +/- contours, 5 for show plane
	public:
		Surf2DBase(WinPrefs * Prefs);
		Surf2DBase(void);
		virtual ~Surf2DBase(void);
		inline short GetDimensions(void) const {return 2;};
		inline bool GetShowZeroContour(void) const {return (SurfOptions&1);};
		inline void SetShowZeroContour(bool state) {if (SurfOptions&1) SurfOptions--; if (state) SurfOptions++;};
		inline bool GetRotate2DMap(void) const {return ((SurfOptions&2)!=0);};
		inline void SetRotate2DMap(bool state) {if (SurfOptions&2) SurfOptions-=2; if (state) SurfOptions+=2;};
		inline bool GetDashLine(void) const {return ((SurfOptions&4)!=0);};
		inline void SetDashLine(bool NewVal) {SurfOptions = (SurfOptions & 0xFFFFFFFB) + (NewVal ? 4 : 0);};
		inline bool ContourBothPosNeg(void) const {return ((SurfOptions & 8)!=0);};
		inline void SetContourBothPosNeg(bool NewVal) {SurfOptions = (SurfOptions & 0xFFFFFFF7) + (NewVal ? 8 : 0);};
		inline bool ShowPlottingPlane(void) const {return ((SurfOptions & 16)!=0);};
		inline void ShowPlottingPlane(bool NewVal) {SurfOptions = (SurfOptions & 0xFFFFFFEF) + (NewVal ? 16 : 0);};
		inline long GetSurfOptions(void) const {return SurfOptions;};
		inline void GetPosColor(RGBColor *temp) const {*temp=PosColor;};
		inline void SetPosColor(const RGBColor *newColor) {PosColor=*newColor;};
		inline void GetNegColor(RGBColor *temp) const {*temp=NegColor;};
		inline void SetNegColor(const RGBColor *newColor) {NegColor=*newColor;};
		inline void GetOrigin(CPoint3D * a) const {*a = Origin;};
		inline void GetYIncrement(CPoint3D * a) const {*a = YInc;};
		inline void GetXIncrement(CPoint3D * a) const {*a = XInc;};
		inline void SetOrigin(CPoint3D * a) {Origin = *a;};
		inline void SetXIncrement(CPoint3D * a) {XInc = *a;};
		inline void SetYIncrement(CPoint3D * a) {YInc = *a;};
		inline long GetNumGridPoints(void) const {return NumGridPoints;};
		void SetNumGridPoints(long newNum);
		inline long GetNumContours(void) const {return NumContours;};
		inline void SetNumContours(long newNum) {if (newNum>0) NumContours=newNum;};
		inline float GetMaxValue(void) const {return MaxContourValue;};
		inline void SetMaxValue(float newVal) {if (newVal>0.0) MaxContourValue=newVal;};
		void SetPlaneToScreenPlane(MoleculeData * MainData);
		virtual bool Needs2DPlane(void) const;
		virtual bool NeedsUpdateDuringRotation(void) const {return ((SurfOptions&2)!=0);};
		virtual void RotateEvent(MoleculeData * MainData);
		virtual void RotateSurface(Matrix4D RotationMatrix);
		inline bool GridAvailable(void) const {return (Grid!=NULL);};
		virtual bool ExportPossible(void) const {return GridAvailable();};
		virtual void Export(BufferFile * Buffer, exportFileType eft);
		void Write2DXML(XMLElement * parent, bool writeGrid) const;
		void Read2DXML(XMLElement * sxml);
		inline void FreeGrid(void) {
			if (Grid) {
				delete [] Grid;
				Grid=NULL; GridAllocation = 0;
			}
		};
		void AllocateGrid(long NumPoints) {
			if (NumPoints > 0) {
				Grid = new float[NumPoints];
				if (Grid) GridAllocation = NumPoints;
			} else {
				if (Grid) delete Grid;
				Grid = NULL;
				GridAllocation = 0;
			}
		};
		virtual void Draw2D(MoleculeData * lData, long hoffset, long voffset, float scale);
		void Contour2DGrid(MoleculeData * lData, long hoffset, long voffset, float scale);
		virtual long Draw3DGL(MoleculeData * lData, WinPrefs * Prefs, myGLTriangle *, unsigned int shader_program = 0);
		void Contour2DGrid3DGL(MoleculeData * lData, WinPrefs * Prefs);
		virtual long ExportPOV(MoleculeData *lData, WinPrefs *Prefs, BufferFile *Buffer);
};
class Surf3DBase : public Surface {
	protected:
		CPoint3D *	ContourHndl;	//Actual vertex
		long *		VertexList;	//Triangle vertex list
		float *		List;
		float *		Grid;
		CPoint3D *	SurfaceNormals;
		long		GridAllocation;
		long		ContourAllocation;
		long		VertexAllocation;
		long		ListAllocation;
		long		SNormAllocation;
		long		NumVertices;
		long		NumPosContourTriangles;
		long		NumNegContourTriangles;
		float		GridMax;		//Maximum magnitude in the 3D grid
		float		GridMin;
		CPoint3D	Origin;
		float		XGridInc;
		float		YGridInc;
		float		ZGridInc;
		long		NumXGridPoints;
		long		NumYGridPoints;
		long		NumZGridPoints;
		long		NumGridPoints;	//Dimension of 3D grid (cubic)
		long		Mode;			//1 for solid, 2 for wireframe, 4 produce +/- contours,
									//8 color surface by value,
									//16 to use preset values for grid
									//32 to compute and use surface normals
									//64 method to colorize surface (set to use +/- colors)
									//128 invert color mapping
		RGBColor	PosColor;
		RGBColor	NegColor;
		int			Transparency;		//Transparency color
		float		ContourValue;
		float		GridSize;
		float		MaxMEPValue;	//Maximum surface value to map to a color
	public:
		Surf3DBase(WinPrefs * Prefs);
		Surf3DBase(void);
		virtual ~Surf3DBase(void);
		inline void SetMaxSurfaceValue(float NewVal) {MaxMEPValue = NewVal;};
		inline float GetMaxSurfaceValue(void) const {return MaxMEPValue;};
		inline short GetDimensions(void) const {return 3;};
		inline float GetContourValue(void) const {return ContourValue;};
		inline float GetGridSize(void) const {return GridSize;};
		inline void SetContourValue(float newValue) {if (newValue>0) ContourValue = newValue;};
		inline void SetGridSize(float newSize) {if (newSize>0) GridSize = newSize;};
		inline long GetNumGridPoints(void) const {return NumGridPoints;};
		inline void SetNumGridPoints(long newNum) {if (newNum>9) NumGridPoints = newNum;};
		inline bool SolidSurface(void) const {return ((Mode & 1)!=0);};
		inline void SolidSurface(bool state) {Mode = (Mode & 0xFFFFFFFC) + (state?1:2);};
		inline bool WireFrameSurface(void) const {return ((Mode & 2)!=0);};
		inline void WireFrameSurface(bool state) {Mode = (Mode & 0xFFFFFFFC) + (state?2:1);};
		inline bool GetFixGrid(void) const {return ((Mode & 16)!=0);};
		inline void SetFixGrid(bool State) {Mode = (Mode & 0xFFFFFFEF) + (State?16:0);};
		inline bool ColorByValue(void) const {return ((Mode & 8) != 0);};
		inline void SetColorByValue(bool State) {Mode = (Mode & 0xFFFFFFF7) + (State?8:0);};
		inline bool ContourBothPosNeg(void) const {return ((Mode & 4)!=0);};
		inline void SetContourBothPosNeg(bool state) {Mode = (Mode & 0xFFFFFFFB) + (state?4:0);};
		inline void UseSurfaceNormals(bool state) {Mode = (Mode & 0xFFFFFFDF) + (state?32:0);};
		inline bool UseSurfaceNormals(void) const {return ((Mode & 32)!=0);};
		inline void UseRGBColoration(bool state) {Mode = (Mode & 0xFFFFFFBF) + (state?0:64);};
		inline bool UseRGBColoration(void) const {return ((Mode & 64)==0);};
		inline void InvertRGBColoration(bool state) {Mode = (Mode & 0xFFFFFF7F) + (state?128:0);};
		inline bool InvertRGBColoration(void) const {return ((Mode & 128)!=0);};
		inline void GetPosColor(RGBColor *temp) const {*temp=PosColor;};
		inline void SetPosColor(RGBColor *newColor) {PosColor=*newColor;};
		inline void GetNegColor(RGBColor *temp) const {*temp=NegColor;};
		inline void SetNegColor(RGBColor *newColor) {NegColor=*newColor;};
		inline int GetTransparency() const {return Transparency;};
		inline void SetTransparency(int newColor) {Transparency=newColor;};
		virtual bool isTransparent(void) const {return (SolidSurface()&& Transparency > 0);};
		inline float GetGridMax(void) const {return GridMax;};
		inline float GetGridMin(void) const {return GridMin;};
		inline long GetNumXGridPoints(void) const {return NumXGridPoints;};
		inline void SetNumXGridPoints(long NewNum) {if (NewNum > 0) NumXGridPoints = NewNum;};
		inline long GetNumYGridPoints(void) const {return NumYGridPoints;};
		inline void SetNumYGridPoints(long NewNum) {if (NewNum > 0) NumYGridPoints = NewNum;};
		inline long GetNumZGridPoints(void) const {return NumZGridPoints;};
		inline void SetNumZGridPoints(long NewNum) {if (NewNum > 0) NumZGridPoints = NewNum;};
		inline void GetOrigin(CPoint3D * a) const {*a = Origin;};
		inline void SetOrigin(CPoint3D * a) {Origin = *a;};
		inline float GetXGridInc(void) const {return XGridInc;};
		inline float GetYGridInc(void) const {return YGridInc;};
		inline float GetZGridInc(void) const {return ZGridInc;};
		inline void SetXGridInc(float NewVal) {XGridInc = NewVal;};
		inline void SetYGridInc(float NewVal) {YGridInc = NewVal;};
		inline void SetZGridInc(float NewVal) {ZGridInc = NewVal;};
		inline bool GridAvailable(void) const {return (Grid!=NULL);};
		inline void FreeGrid(void) {
			if (Grid) {
				delete [] Grid;
				Grid=NULL; GridAllocation = 0;
			}
		};
		inline void FreeContour(void) {
			if (ContourHndl) {
				delete [] ContourHndl;
				ContourHndl=NULL; ContourAllocation = 0;
			}
			if (VertexList) {
				delete [] VertexList;
				VertexList=NULL;
				VertexAllocation = 0;
			}
			if (List) FreeList();
			if (SurfaceNormals) FreeNormals();
		};
		inline void FreeList(void) {
			if (List) {
				delete [] List;
				List=NULL; ListAllocation = 0;
			}
		};
		inline void FreeNormals(void) {
			if (SurfaceNormals) {
				delete [] SurfaceNormals;
				SurfaceNormals=NULL; SNormAllocation = 0;
			}
		};
		void AllocateGrid(long NumPoints);
		bool AllocateContour(long NumPoints);
		bool AllocateContour(long NumVertices, long NumTriangles);
		void AllocateList(long NumPoints);
		void AllocateNormals(long NumPoints);
		inline bool ContourAvail(void) const {return (ContourHndl != NULL);};
		virtual bool ExportPossible(void) const {return GridAvailable();};
		virtual void Export(BufferFile * Buffer, exportFileType eft);
		virtual void RotateSurface(Matrix4D RotationMatrix);
		void SetSurfaceColor(const float & surfaceValue, const RGBColor * pColor, const RGBColor * nColor, float & red, float & green, float & blue) const; //Pass the value/Max, does not need to between 1 and -1
		long CreateSolidSurface(CPoint3D * Vertices, CPoint3D * Normals, long * VertexList,
								long NumTriangles, RGBColor * SurfaceColor, float * SurfaceValue,
								RGBColor * NegColor, float MaxSurfaceValue, MoleculeData *, myGLTriangle *);
		
		void CreateWireSurface(CPoint3D * Vertices, CPoint3D * Normals, long * VertexList,
							   long NumTriangles, RGBColor * SurfaceColor, float * SurfaceValue,
							   RGBColor * NegColor, float MaxSurfaceValue, MoleculeData *, const WinPrefs *);
		long CreateWireFrameSurfaceWithLines(CPoint3D * Vertices, long * vList,
								long NumTriangles, RGBColor * SurfaceColor, float * SurfaceValue,
								RGBColor * NColor, float MaxSurfaceValue, MoleculeData * );
			virtual long getTriangleCount(void) const {return (NumPosContourTriangles+NumNegContourTriangles);};
		/* virtual long ExportPOV(MoleculeData *lData, WinPrefs *Prefs, BufferFile *Buffer) { */
		/* } */
		long ExportPOVSurface(CPoint3D *Vertices, CPoint3D *Normals, long *VertexList,
							  long NumTriangles, RGBColor *SurfaceColor, float *SurfaceValue,
							  RGBColor *NegColor, float MaxSurfaceValue, MoleculeData *MainData,
							  BufferFile *Buffer);
		void SetupGridParameters(Frame * lFrame);
		void Contour3DGrid(Progress * lProgress);
		void AdjustSurfaceNormals(void);
		void Write3DXML(XMLElement * parent, bool writeGrid) const;
		void Read3DXML(XMLElement * parent);
		void CalculateSurfaceValues(MoleculeData * MainData, Progress * progress);
		void CalculateSurfaceValuesGrid(MoleculeData * MainData, Progress * progress,
										AODensity * TotalDensity, long start, long end,
										GaussHermiteData * GHData, float * grid,
										CPoint3D * Contour, long * PercentDone, bool MPTask);
};

class General2DSurface : public Surf2DBase {
	public:
		General2DSurface(WinPrefs * Prefs);
		General2DSurface(XMLElement * x);
		virtual char * GetLabel(void);
		virtual SurfaceType GetSurfaceType(void) const {return kGeneral2DSurface;};
		//pick and read a file containing the 2D grid
		void ReadGrid(const bool Square, const bool UseMult, const double & MultValue);
		virtual void WriteXML(XMLElement * parent) const;
		virtual bool Needs2DPlane(void) const {bool result=false; if (Visible&&Grid) result=true; return result;};
};

class General3DSurface : public Surf3DBase {
	public:
		General3DSurface(WinPrefs * Prefs);
		General3DSurface(XMLElement * x);
		virtual char * GetLabel(void);
		virtual SurfaceType GetSurfaceType(void) const {return kGeneral3DSurface;};
			//pick and read a file containing the 3D grid
		void ReadGrid(const bool Square, const bool UseValue, const double & MultValue);
		virtual long Draw3DGL(MoleculeData * lData, WinPrefs * Prefs, myGLTriangle *, unsigned int shader_program = 0);
		virtual long getTriangleCount(void) const;
		virtual long ExportPOV(MoleculeData *lData, WinPrefs *Prefs, BufferFile *Buffer);
		virtual void WriteXML(XMLElement * parent) const;
		inline void GetMaxColor(RGBColor *temp) const {*temp=PosColor;};
		inline void SetMaxColor(const RGBColor *newColor) {PosColor=*newColor;};
		inline void GetMinColor(RGBColor *temp) const {*temp=NegColor;};
		inline void SetMinColor(const RGBColor *newColor) {NegColor=*newColor;};
};
class TEDensity3DSurface : public Surf3DBase {
	private:
	public:
		TEDensity3DSurface(WinPrefs * Prefs);
		TEDensity3DSurface(XMLElement * x);
		virtual char * GetLabel(void);
		virtual SurfaceType GetSurfaceType(void) const {return kTotalDensity3D;};
		void UpdateData(TEDensity3DSurface * target);
		virtual void Update(MoleculeData * MainData);
		virtual long Draw3DGL(MoleculeData * lData, WinPrefs * Prefs, myGLTriangle *, unsigned int shader_program = 0);
		virtual long ExportPOV(MoleculeData *lData, WinPrefs *Prefs, BufferFile *Buffer);
		virtual void WriteXML(XMLElement * parent) const;
		void CalculateMOGrid(MoleculeData * MainData, Progress * progress);
		float CalculateGrid(long xStart, long xEnd, mpAtom * Atoms, BasisSet * Basis,
			float * Vectors, float * VectorsB, float * OccupancyA, float * OccupancyB, float * AOVector,
			long NumOccupiedAlphaOrbs, long NumOccupiedBetaOrbs, long NumAtoms, Progress * progress,
			long * PercentDone, bool MPTask);
};
class TEDensity2DSurface : public Surf2DBase {
	private:
	public:
		TEDensity2DSurface(WinPrefs * Prefs);
		TEDensity2DSurface(TEDensity2DSurface * target);
		TEDensity2DSurface(XMLElement * x);
		virtual char * GetLabel(void);
		virtual SurfaceType GetSurfaceType(void) const {return kTotalDensity2D;};
		virtual void WriteXML(XMLElement * parent) const;
		void CalculateMOGrid(MoleculeData * MainData, Progress * lProgress);
		virtual void Update(MoleculeData * MainData);
		void UpdateData(TEDensity2DSurface * target);
};

class TEDensity1DSurface : public Surf1DBase {
	public:
		TEDensity1DSurface(WinPrefs * Prefs);
		TEDensity1DSurface(TEDensity1DSurface * target);
		TEDensity1DSurface(XMLElement * x);
		virtual char *GetLabel(void);
		virtual SurfaceType GetSurfaceType(void) const { return kTotalDensity1D; }
		virtual void WriteXML(XMLElement *parent) const;
		void CalculateMOGrid(MoleculeData *MainData, Progress *lProgress);
		virtual void Update(MoleculeData *MainData);
		void UpdateData(TEDensity1DSurface *target);
};

class MEP2DSurface : public Surf2DBase {
	private:
	public:
		MEP2DSurface(WinPrefs * Prefs);
		MEP2DSurface(MEP2DSurface * target);
		MEP2DSurface(XMLElement * x);
		virtual char * GetLabel(void);
		virtual SurfaceType GetSurfaceType(void) const {return kMEP2D;};
		virtual void WriteXML(XMLElement * parent) const;
		void CalculateMOGrid(MoleculeData * MainData, Progress * lProgress);
		virtual void Update(MoleculeData * MainData);
		void UpdateData(MEP2DSurface * target);
};
class MEP3DSurface : public Surf3DBase {
	private:
	public:
		MEP3DSurface(WinPrefs * Prefs);
		MEP3DSurface(XMLElement * x);
		virtual char * GetLabel(void);
		virtual SurfaceType GetSurfaceType(void) const {return kMEP3D;};
		void UpdateData(MEP3DSurface * target);
		virtual void Update(MoleculeData * MainData);
		virtual long Draw3DGL(MoleculeData * lData, WinPrefs * Prefs, myGLTriangle *, unsigned int shader_program = 0);
		virtual long ExportPOV(MoleculeData *lData, WinPrefs *Prefs, BufferFile *Buffer);
		virtual void WriteXML(XMLElement * parent) const;
		void CalculateMEPGrid(MoleculeData * MainData, Progress * progress);
		float CalculateGrid(long xStart, long xEnd, Frame * lFrame, BasisSet * Basis,
			AODensity * TotalAODensity, GaussHermiteData * GHData,
			Progress * progress, long * PercentDone, bool MPTask);
};
class Orb3DSurface : public Surf3DBase, public OrbSurfBase {
	public:
		Orb3DSurface(WinPrefs * Prefs);
		Orb3DSurface(Orb3DSurface * target);
		Orb3DSurface(XMLElement * s);
		virtual char * GetLabel(void);
		virtual SurfaceType GetSurfaceType(void) const {return kOrb3DType;};
		virtual void Update(MoleculeData * MainData);
		virtual long Draw3DGL(MoleculeData * lData, WinPrefs * Prefs, myGLTriangle *, unsigned int shader_program = 0);
		virtual long ExportPOV(MoleculeData *lData, WinPrefs *Prefs, BufferFile *Buffer);
		virtual void WriteXML(XMLElement * parent) const;
		void CalculateMOGrid(MoleculeData * MainData, Progress * lProgress);
		float CalculateGrid(long xStart, long xEnd, const mpAtom * const Atoms, const BasisSet * const Basis,
			const float * const MOVector, long NumAtoms, Progress * lProgress, long * PercentDone,
							const std::vector<int> * const atomScreen, const std::vector<int> * const shellScreen, bool MPTask);
	float CalculateGridStreamlined(long xStart, long xEnd, const std::vector<CPoint3D> & atomList, const BasisSet * const Basis,
						const std::vector<float> & reducedVector, long NumAtoms, Progress * lProgress, long * PercentDone,
								   const std::vector<int> * const atomScreen, const std::vector<int> * const shellScreen, 	const std::vector<int> & shellTypes, const std::vector<int> & shellIndex, const std::vector<short> & shellsPerAtom, bool MPTask);
		void UpdateData(Orb3DSurface * Original);
};
class Orb2DSurface : public Surf2DBase, public OrbSurfBase {
	public:
		Orb2DSurface(WinPrefs * Prefs);
		Orb2DSurface(Orb2DSurface * Original);
		Orb2DSurface(XMLElement * x);
		virtual char * GetLabel(void);
		virtual SurfaceType GetSurfaceType(void) const {return kOrb2DType;};
		virtual void WriteXML(XMLElement * parent) const;
		void CalculateMOGrid(MoleculeData * MainData, Progress * lProgress);
		void UpdateData(Orb2DSurface * Original);
		virtual void Update(MoleculeData * MainData);
		virtual bool Needs2DPlane(void) const;
};
#endif
