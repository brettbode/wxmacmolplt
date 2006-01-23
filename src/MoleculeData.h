/***************************************
 * MoleculeData.h
 *
 * MoleculeData class organizes data on the molecule involved in each
 * window/file.
 *
 * Created:       04-??-1998  BMB (Created from other files)
 * Last Modified: 01-19-2006  Steven Schulteis
 ***************************************/

/*  This file was created from a copy of MoleculeData.h from the old
 *  MacMolPlt source.
 *
 *  License info from original file:
 *
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

#ifndef __MoleculeData__
#define __MoleculeData__

#include "MyTypes.h"

typedef class AtomTypeList AtomTypeList;

class MoleculeData {
        friend class MolDisplayWin;
        friend class CoordWin;
        friend class EnergyPlotWin;
        friend class Surf2DBase;
        friend class Surf3DBase;
    private:
        CPoint3D    *RotCoords;             // The currently displayed, rotated coordinates
        long        *zBuffer;               // the sorting order for RotCoords
        Frame *     cFrame;                 // pointer to the currently drawn frame
        Frame *     Frames;                 // pointer to the first frame
        Internals * IntCoords;
        BasisSet *  Basis;
        char *      Description;            // Simple one line label
        long        CurrentFrame;           // Number of the current frame
        long        NumFrames;              // Current number of Frames
        long        MaxAtoms;               // The maximum number of atoms in any frame
        float       MaxSize;                // The maximum side length of any frame
        float       WindowSize;             // The Window size in the molecule coordinate space (ie  or Bohrs) user adjustable to change the window scaling
        Matrix4D    TotalRotation;          // Rotation matrix for displaying the molecule
        InputData * InputOptions;           // Run information
        char        DrawMode;               // Flag for normal mode display & MO display
                                            /*  bit 1 for showing normal modes
                                                bit 2 for showing special atoms
                                                4 & 5 are obsolete, 6 indicates 2D plane display
                                                bit 7 for showing the axis's
                                                bit 4 for plotting KE rather than Total E
                                                bit 5 for plotting PE */
        char        DrawLabels;             // Flag for label drawing (both #'s and atomic labels) bit3 deactivates labeling H atoms

    public:
        // Commented out inline kewords until the original intention behind
        // their use is determined.  It should be implied in all of these cases
        // anyway.
        MoleculeData();
        ~MoleculeData(void);
        long UnPackOldData(BufferFile *Buffer);
        long ReadInitialFragmentCoords(BufferFile * Buffer);
        void ReadFragmentCoordinates(BufferFile * Buffer, long NumFragmentAtoms);
        void ConvertMainWinData1(BufferFile *Buffer, long size);
        void ConvertMainWinData14(BufferFile * Buffer, long length);
        void ReadRunInfoRec(BufferFile *Buffer, long length);
        void ReadRunTitle(BufferFile *Buffer, long length);
        void ReadMORec10(BufferFile * Buffer, long length);
        long ReadMORec48(BufferFile *Buffer, const long & NumBasisFuncs, const long & ByteCount);
        void ReadBasisOptions(BufferFile * Buffer);
        void ReadControlOptions(BufferFile * Buffer);
        /* inline */ float GetMoleculeSize(void) {return WindowSize;};
        /* inline */ void SetMoleculeSize(float newVal) {if (newVal > 0.0) WindowSize = newVal;};
        void GetModelCenter(CPoint3D * center);
        void SetModelCenter(CPoint3D * center);
        void GetModelRotation(float * Psi, float * Phi, float * Theta);
        void SetModelRotation(float Psi, float Phi, float Theta);
        void AdvanceFrame(void);
        void SetCurrentFrame(long FrameNum);
        long PackData(short Amount, BufferFile * Buffer);
        long UnPackData(BufferFile * Buffer);
        /* inline */ Boolean GetFrameMode(void) {return (DrawMode & (1<<3));};
        AtomTypeList * GetAtomTypes(void);
        Boolean SurfaceExportPossible(void);
        /* inline */ long GetCurrentFrame(void) {return CurrentFrame;};
        /* inline */ Frame* GetCurrentFramePtr(void) {return cFrame;};
        /* inline */ Frame * GetFirstFrame(void) {return Frames;};
        void ParseGAMESSBasisSet(BufferFile * Buffer);
        long ParseECPotentials(BufferFile * Buffer);
        /* inline */ BasisSet * GetBasisSet(void) {return Basis;};
        /* inline */ long GetNumFrames(void) {return NumFrames;};
        Boolean OrbSurfacePossible(void);
        Boolean TotalDensityPossible(void);
        void ResetRotation(void);
        void CenterModelWindow(void);
        void InvertMode(void);
        void UnitConversion(Boolean AngToBohr);
        void FlipRotation(short theItem);
        Boolean SetScreenPlane(CPoint3D *Points);
        void LinearLeastSquaresFit(Progress * lProgress);
        void CreateLLM(long NumPts, WinPrefs * Prefs);
        void CreateInternalLLM(long NumPts, WinPrefs * Prefs);
        Frame * AddFrame(long NumAtoms, long NumBonds);
        void DeleteFrame(void);
        Boolean SetupFrameMemory(long NumAtoms, long NumBonds);
        Frame * LocateNewFrame(float XPosition);
        /* inline */ Boolean DrawAtomLabels(void) {return (DrawLabels & 1);};
        /* inline */ Boolean DrawHLabels(void) {return ((DrawLabels & 4) == 0);};
        /* inline */ void SetHLabelMode(Boolean State) {DrawLabels = (DrawLabels & 0xFB) + (State ? 0: 4);};
        /* inline */ void SetAtomLabelDrawMode(Boolean State) {if (DrawLabels&1) DrawLabels--; if (State) DrawLabels++;};
        /* inline */ Boolean DrawAtomNumbers(void) {return ((DrawLabels & 2)!=0);};
        /* inline */ void SetAtomNumbersDrawMode(Boolean State) {if (DrawLabels&2) DrawLabels-=2; if (State) DrawLabels+=2;};
        Boolean ModeVisible(void);
        void SetDescription(char * NewLabel);
        /* inline */ Boolean SetSpecialAtomDrawMode(Boolean State) {if (DrawMode&2) DrawMode-=2; if (State) DrawMode+=2; return State;};
        /* inline */ Boolean GetSpecialAtomDrawMode(void) {return ((DrawMode & 2)!=0);};
        /* inline */ Boolean SetDrawMode(Boolean State) {if (DrawMode&1) DrawMode--; if (State) DrawMode++; return State;};
        /* inline */ Boolean GetDrawMode(void) {return (DrawMode & 1);};
        /* inline */ Boolean ShowAxis(void) {return ((DrawMode & 0x40) != 0);};
        /* inline */ void SetShowAxis(Boolean newState) {DrawMode = (DrawMode & 0xBF) + (newState ? 0x40 : 0);};
        long GetNumBonds(void);
        long GetNumElectrons(void);
        short GetMultiplicity(void);
        void WriteInputFile(WinPrefs * Prefs);
        InputData * GetInputData(void);
        InputData * SetInputData(InputData * NewData);
        void NewAtom(void);
        void StickCoordinates(void);
        /* inline */ Internals * GetInternalCoordinates(void) {return IntCoords;};
        void DeleteAtom(long AtomNum);
        Boolean ValidAtom(long AtomNum);
        void GetRotationMatrix(Matrix4D copy);
};

#endif

