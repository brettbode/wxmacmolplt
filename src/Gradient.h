/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/* Gradient.h

	class to organize gradient data for a given geometry

	BMB 6/1998
*/

#ifndef __Gradient__
#define __Gradient__
typedef class XMLElement XMLElement;

class GradientData {
	private:
		float		RMS;
		float		Maximum;
		CPoint3D *	CartesianGradient;
		long		CartAllocation;
	public:
		GradientData(void);
		~GradientData(void);
		float GetRMS(void) const {return RMS;};
		float GetMaximum(void) const {return Maximum;};
		void SetRMS(float v) {RMS = v;};
		void SetMaximum(float v) {Maximum = v;};
		bool ParseGAMESSGradient(BufferFile * Buffer, long NumAtoms, wxFileOffset SearchLength,
			int Style);
		/* Retrive the gradient vector for theAtom, returns true if successful
		 * @param theAtom the atom to pull from the array
		 * @param GradientVector upon return contains the gradient vector for theAtom
		 */
		bool RetrieveAtomGradient(long theAtom, CPoint3D & GradientVector) const;
		/* Test to see if the gradient vector is available for display.
		 * @param NumAtoms the number of atoms for the coordinates to confirm the gradient vector is the same length
		 */
		bool GradientVectorAvailable(long NumAtoms) const {return ((CartesianGradient!=NULL)&&(CartAllocation==NumAtoms));};
#ifndef __wxBuild__
		long GetSize(BufferFile * Buffer);
		long Write(BufferFile * Buffer);
		static GradientData * Read(BufferFile * Buffer);
#endif
		void WriteXML(XMLElement * parent);
		void ReadXML(XMLElement * gradXML);
};

#endif
