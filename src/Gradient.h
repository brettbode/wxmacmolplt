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
		float GetRMS(void) {return RMS;};
		float GetMaximum(void) {return Maximum;};
		bool ParseGAMESSGradient(BufferFile * Buffer, long NumAtoms, long SearchLength,
			bool Style);
#ifndef __wxBuild__
		long GetSize(BufferFile * Buffer);
		long Write(BufferFile * Buffer);
		static GradientData * Read(BufferFile * Buffer);
#endif
		void WriteXML(XMLElement * parent);
		void ReadXML(XMLElement * gradXML);
};

#endif
