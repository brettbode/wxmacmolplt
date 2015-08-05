/*
 *  (c) 2004-2008 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* MoleculeData.h

	class MoleculeData organizes data on the molecule involved in each window/file.

	Created from other files, BMB, 4/1998
*/

#ifndef __MoleculeData__
#define __MoleculeData__
#ifndef __MyTypes__
#include "MyTypes.h"
#endif

typedef class AtomTypeList AtomTypeList;
typedef class WindowData WindowData;
typedef class MolDisplayWin MolDisplayWin;

/**
 * The main data container for Molecule information.
 * This class encapsulates all of the chemical information for a file including
 * basis set, geometries, GAMESS options, etc.
 */
class MoleculeData {
		friend class MolDisplayWin;
		friend class CoordWin;
		friend class EnergyPlotDialog;
		friend class FrequenciesDialog;
		friend class Surf2DBase;
		friend class Surf3DBase;
		friend class MpGLCanvas;
		friend class FrameSnapShot;
		friend class DataGroup;
	private:
		std::vector<Annotation *> Annotations;	///< Set of annotations
		int constrain_anno_id;
		CPoint3D	*RotCoords;				///< The currently displayed, rotated coordinates in 2D mode
		Frame *		cFrame;					///< pointer to the currently drawn frame
		Frame *		Frames;					///< pointer to the first frame
		Internals *	IntCoords;				///< The set of internal coordinate definitions
		BasisSet *	Basis;					///< The full basis set used in a computation

		/** efrags is a hash from fragment name to a structure containing
		 * fragment text and parsed coordinates.  There's only one entry for
		 * each different fragment type.  FragmentNames is used to map fragment
		 * instances to their fragment type.  There's one entry per instance of
		 * each fragment type.  Use an atom's fragment number to index into
		 * FragmentNames, and use the string there to index into efrags.
		 */
		std::map<std::string, EFrag> efrags;
		std::vector<std::string> FragmentNames;	///< Effective Fragment name for each fragment (FRAGNAME)
		std::vector<long> FMOFragmentIds;	///< Vector containing the FMO fragment mapping for each atom

		char *		Description;			///< Simple one line label
		long		CurrentFrame;			///< Number of the current frame
		long		NumFrames;				///< Current number of Frames
		long		MaxAtoms;				///< The maximum number of atoms in any frame
		float		MaxSize;				///< The maximum side length of any frame
		float		WindowSize;				///< The Window size in the molecule coordinate space (ie  or Bohrs) user adjustable to change the window scaling
		Matrix4D	TotalRotation;			///< Rotation matrix for displaying the molecule
		CPoint3D    Centroid;
		CPoint3D    MolCentroid;
		InputData *	InputOptions;			///< GAMESS run configuration information
		
		/** Flag for normal mode display & MO display
		 *	bit 1 for showing normal modes
		 *	bit 2 for showing special atoms
		 *	4 & 5 are obsolete, 6 indicates 2D plane display
		 *	bit 7 for showing the axis's
		 *	bit 4 for plotting KE rather than Total E
		 *	bit 5 for plotting PE
		 */
		char		DrawMode;
		char		DrawLabels;				///< Flag for label drawing (both #'s and atomic labels) bit3 deactivates labeling H atoms
	public:
		MolDisplayWin *MolWin;
		MoleculeData(MolDisplayWin *MolWin);
		~MoleculeData(void);
		long ReadInitialFragmentCoords(BufferFile * Buffer);
		void ReadFragmentCoordinates(BufferFile * Buffer, long NumFragmentAtoms);
		void ParseMOPACZMatrix(BufferFile * Buffer, const long & nAtoms, WinPrefs * Prefs);
		void ParseZMatrix(BufferFile * Buffer, const long & nAtoms, WinPrefs * Prefs);
		void ParseGAMESSUKZMatrix(BufferFile * Buffer, WinPrefs * Prefs);
#ifndef __wxBuild__
		void ConvertMainWinData1(BufferFile *Buffer, long size);
		void ConvertMainWinData14(BufferFile * Buffer, long length);
		long UnPackOldData(BufferFile *Buffer);
		long UnPackData(BufferFile * Buffer);
		void ReadMORec10(BufferFile * Buffer, long length);
		long ReadMORec48(BufferFile *Buffer, const long & NumBasisFuncs, const long & ByteCount);
#endif
		void ReadRunInfoRec(BufferFile *Buffer, long length);
		void ReadRunTitle(BufferFile *Buffer, long length);
		void ReadBasisOptions(BufferFile * Buffer);
		void ReadControlOptions(BufferFile * Buffer);
		long ParseTinkerCoordinates(BufferFile * Buffer);
		long WriteCMLFile(BufferFile * Buffer, WinPrefs * Prefs, WindowData * wData, bool allFrames, bool AllData);
		/**
		 * Parse CML text in the provided buffer
		 * @param Buffer The buffer containing the CML text
		 * @param Prefs The window preferences
		 * @param wData The extra window data (sizes, etc)
		 * @param p A progress indicator in case the operation takes too much time
		 * @param readPrefs Should window preferences be read from the CML text (if present)
		 * @param UseAutoBond Should the SetBonds routine be called on each frame
		 */
		long OpenCMLFile(BufferFile * Buffer, WinPrefs * Prefs, WindowData * wData, Progress * p, bool readPrefs, bool UseAutoBond=true);
		inline float GetMoleculeSize(void) {return WindowSize;};
		inline void SetMoleculeSize(float newVal) {if (newVal > 0.0) WindowSize = newVal;};
		void GetModelCenter(CPoint3D * center);
		void SetModelCenter(CPoint3D * center);
		void GetModelRotation(float * Psi, float * Phi, float * Theta);
		void SetModelRotation(float Psi, float Phi, float Theta);
	//	inline Boolean GetFrameMode(void) {return (DrawMode & (1<<3));};
		AtomTypeList * GetAtomTypes(void);
		bool SurfaceExportPossible(void);
		void ParseGAMESSBasisSet(BufferFile * Buffer);
		long ParseECPotentials(BufferFile * Buffer);
		/**
		 * Parse the INDAT array of fragment ids for FMO.
		 @param Buffer the file buffer stream
		 @param NumAtoms target atom number
		 @param EndOfGroup the file position to use as a limit for the parsing
		 */
		bool ParseFMOIds(BufferFile * Buffer, const long & NumAtoms, const wxFileOffset & EndOfGroup);
		/**
		 * Write the FMO array to XML
		 * @param parent
		 */
		void WriteFMOIdsToXML(XMLElement * parent);
		/**
		 * Read the FMO Id array from XML
		 * @param item XML element
		 */
		void ReadFMOIdsFromXML(XMLElement * item);
		/**
		 * Write the INDAT array of fragment ids for FMO out to the buffer in the input file format.
		 @param Buffer the file buffer stream
		 */
		void WriteINDAT(BufferFile * Buffer);
		inline BasisSet * GetBasisSet(void) const {return Basis;};
		long GetNumBasisFunctions(void) const;
		inline long GetNumFrames(void) {return NumFrames;};
		/**
		 * Does the current data include enough information to plot an orbital?
		 */
		bool OrbSurfacePossible(void) const;
		/**
		 * Determine whether it is possible to compute a total electron density with the
		 * current data. This requires having a set of orbitals with a known occupation.
		 */
		bool TotalDensityPossible(void) const;
		/**
		 * Determine whether it is possible to compute a molecular electrostatic potential with the
		 * current data. This requires having a set of orbitals with a known occupation and knowing
		 * the nuclear charge for each atom (potentially not known for ECP/MCPs).
		 */
		bool MEPCalculationPossible(void) const;
		void ResetRotation(void);
		void CenterModelWindow(void);
		void InvertMode(void);
		void UnitConversion(bool AngToBohr);
		void FlipRotation(short theItem);
		/**
		 * Rotate/translate the reference coordinates into the principle orientation.
		 * @param Prefs The current window preferences
		 * @param precision The tolerance for slop in the coordinates (ex 1.0D-5)
		 */
		void RotateToPrincipleOrientation(WinPrefs * Prefs, double precision=1.0E-4);
		/**
		 * Determine the set of point groups satisfied by the current set of coordinates.
		 * @param pgFlags One flag per point group indicating whether the coordinates satisfy the point group.
		 * @param Prefs The current window preferences
		 * @param tolerance how tight should the operator tolerances be
		 */
		void DeterminePointGroup(bool * pgFlags, WinPrefs * Prefs, double tolerance);
		/**
		 * Compute the principle orientation of the current set of coordinates.
		 * The return value indicates whether the current coordinates satisfy the current point group.
		 * @param result The rotation/translation to convert the coordinates to the princ. orientation.
		 * @param translation vector translating the molecule such that the center of mass is at the origin.
		 * @param Prefs The current window preferences
		 * @param precision The tolerance for slop in the coordinates (ex 1.0D-5)
		 */
		bool DeterminePrincipleOrientation(Matrix4D result, CPoint3D& translation, WinPrefs * Prefs, double precision) const;
		void GenerateSymmetryDependentAtoms(bool do_warnings = true);
		/**
		 * Flag the symmetry unique atoms.
		 * The coordinates are left unchanged, just the symmetry unique flag on the atoms
		 * may be changed. A failure in the result would indicate a problem with the coordinates
		 * or that the point group is not corrent.
		 * @param tolerance The maximum distance between atoms that will be considered a match.
		 */
		bool GenerateSymmetryUniqueAtoms(double tolerance);
		/**
		 * Cleanup the coordinates to more tightly match the selected point group.
		 * The routine assumes the symmetry unique atoms are already correctly marked.
		 * This routine should not add or remove atoms, but will change the positions slightly.
		 */
		void SymmetrizeCoordinates(bool selected_only = false);
		bool SetScreenPlane(CPoint3D *Points);
		void LinearLeastSquaresFit(Progress * lProgress);
		void CreateLLM(long NumPts, WinPrefs * Prefs);
		void CreateInternalLLM(long NumPts, WinPrefs * Prefs);
// routines to get/set the current/active frame
		/// Return the index of the active frame.
		inline long GetCurrentFrame(void) {return CurrentFrame;};	
		/// Return a pointer to the active frame.
		inline Frame* GetCurrentFramePtr(void) {return cFrame;};
		/// Return a pointer to the first frame in a series.
		inline Frame * GetFirstFrame(void) {return Frames;};
		/// Increment the frame to the next one
		void AdvanceFrame(void);
		/**
		 * Change the active frame to the one indexed by FrameNum. If the index is invalid the call does nothing.
		 * @param FrameNum the index of the desired Frame.
		 */
		void SetCurrentFrame(long FrameNum);
// routines to add/delete a frame or increase the memory allocations
		/** Add a new frame to the list after the current/active frame. Preallocate memory for the specified number of
		 * atoms and bonds. A pointer to the new frame is returned and also becomes the active frame.
		 * @param NumAtoms Desired length of the Atom array
		 * @param NumBonds Desired length of the Bonds array
		 */
		Frame * AddFrame(long NumAtoms, long NumBonds);
		/** Add a new frame to the series ordered based on the provided XPosition. If successful, a pointer to the
		 * new frame is returned and the new frame becomes the active frame. The call will fail if there is an
		 * existing frame with the same XPosition value in which case the returned value will be NULL and the
		 * state unchanged.
		 * @param XPosition A value ordering the new frame in the existing series.
		 */
		Frame * LocateNewFrame(float XPosition);
		/** Delete the current/active frame. The next frame will become the active frame, if deleting the last
		 * frame, the previous frame will become active. If there is only one frame nothing is changed.
		 */
		void DeleteFrame(void);
		/** Change the memory allocation for the atoms and bonds. Any existing data is preservered so you can't
		 * set the allocation smaller than what is currently in use. Returns false if memory allocation fails.
		 * @param NumAtoms Desired length of the Atom array
		 * @param NumBonds Desired length of the Bonds array
		 */
		bool SetupFrameMemory(long NumAtoms, long NumBonds);

		inline bool DrawHLabels(void) const {return ((DrawLabels & 4) == 0);};
		inline void SetHLabelMode(bool State) {DrawLabels = (DrawLabels & 0xFB) + (State ? 0: 4);};
		bool ModeVisible(void) const;
		void SetDescription(char * NewLabel);
		inline bool SetSpecialAtomDrawMode(bool State) {if (DrawMode&2) DrawMode-=2; if (State) DrawMode+=2; return State;};
		inline bool GetSpecialAtomDrawMode(void) const {return ((DrawMode & 2)!=0);};
		inline bool SetDrawMode(bool State) {if (DrawMode&1) DrawMode--; if (State) DrawMode++; return State;};
		inline bool GetDrawMode(void) const {return (DrawMode & 1);};
		inline bool ShowAxis(void) const {return ((DrawMode & 0x40) != 0);};
		inline void SetShowAxis(bool newState) {DrawMode = (DrawMode & 0xBF) + (newState ? 0x40 : 0);};
		long GetNumBonds(void) const;
		long GetNumElectrons(void) const;
		short GetMultiplicity(void) const;
		inline long GetMaximumAtomCount(void) const {return MaxAtoms;};
		void WriteInputFile(MolDisplayWin * owner);
		InputData * GetInputData(void);
		InputData * SetInputData(InputData * NewData);
		/**
		 * Add a new atom with type AtomType and located at Position
		 * @param AtomType The type of the new atom
		 * @param Position x,y,z coordinates for the new atom
		 * @param updateGlobal Update the global arrays and structures (needed at some point)
		 * @param index Insert the atom in the atom list at the specified index (-1 for the end)
		 */
		void NewAtom(long AtomType, const CPoint3D & Position, bool updateGlobal=true, long index=-1);
		/**
		 * Add a new atom as a copy of atom
		 * @param atom The atom to copy
		 * @param updateGlobal Update the global arrays and structures (needed at some point)
		 * @param index Insert the atom in the atom list at the specified index (-1 for the end)
		 * @param pos Optionally set the position of the new atom to pos.
		 */
		void NewAtom(const mpAtom& atom, bool updateGlobal=true, long index=-1, const CPoint3D *pos = NULL);
		/**
		 * Updates internal data structures to make sure they are consistent with the current atom count.
		 */
		void AtomAdded(void);
		/**
			Make the current rotated view the reference orientation.
		 */
		void StickCoordinates(void);
		void InitializeInternals(void);
		/**
			Accessor for the internal coordinates class.
		 */
		inline Internals * GetInternalCoordinates(void) const {return IntCoords;};
		/**
			Delete an atom.
			The return value is the index of the next atom in the list. This routine may remove
			more than one atom. For example in the case of effective fragments the entire fragment
			must be removed.
			@param AtomNum The index of the atom to delete.
			@param allFrames If true the atom with the same index will be removed from all frames.
		 */
		long DeleteAtom(long AtomNum, bool allFrames=false);
		/**
			Reorders the atom list.
			@param index1 The initial index of the atom
			@param index2 The final index of the atom
		 */
		void ReorderAtomList(long index1, long index2);
		/**
			Is the provided atom index valid for the current frame?
			@param AtomNum The atom index to test
		 */
		bool ValidAtom(long AtomNum);
		/**
			Returns the FRAGNAME for the indicated effective fragment.
			@param index The index of the fragment.
		 */
		const char * GetFragmentName(unsigned long index) const;
		void PruneUnusedFragments();
		void GetRotationMatrix(Matrix4D copy);
		const Matrix4D& GetRotationMatrix() const;
		int GetAnnotationCount(void) const {return Annotations.size();};
		void DeleteAllAnnotations(void);
		void ConstrainToAnnotation(int anno_id) {
			constrain_anno_id = anno_id;
		}
		int GetConstrainAnnotation(void) {
			return constrain_anno_id;
		}
		void RemoveAnnotationConstraint(void) { constrain_anno_id = -1; }
		void ExportPOV(BufferFile *Buffer, WinPrefs *Prefs);
		/**
		 Create an FMO fragment list
		 @param NumMolecules The number of non-bonded molecules to group together in the same FMO fragment
		 @param newFragmentation The resulting fragmentation vector
		 */
		long CreateFMOFragmentation(const int & NumMolecules, std::vector<long> & newFragmentation);
		/**
		 Returns the FMO fragment number for the selected atom
		 @param AtomId the index of the desired atom
		 */
		long GetFMOFragmentId(const long & AtomId) const;
		/**
		 Set the FragmentId for atom index AtomId. This routine will initialize the fragment id
		 array if it is empty.
		 @param AtomId index of the atom to target
		 @param FragmentId the new fragment id for the targeted atom
		 */
		void SetFMOFragmentId(const long & AtomId, const long & FragmentId);
};

#endif
