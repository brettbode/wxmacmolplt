/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	Geometries.h	****	CPoint3D and other specific geomtry classes.

Created for other header files 4-98	BMB
*/
#ifndef __MyGeometries__
#define __MyGeometries__

#include <string.h>
#include <math.h>

#include <ostream>

class Matrix2D {
		friend std::ostream& operator<<(std::ostream& stream, const Matrix2D& m);
	public:
		Matrix2D();
		Matrix2D(const int nrows, const int ncols);
		Matrix2D(const int nrows, const int ncols, const float *data);
		Matrix2D(const Matrix2D& src);
		~Matrix2D();

		const Matrix2D& operator=(const Matrix2D& m);

		void Init(const int nrows, const int ncols, const float *data);
		Matrix2D Transpose(void) const;

		float *data;
		int nrows;
		int ncols;

	private:
};

class CPoint3D {
	public:
		float x;
		float y;
		float z;
		CPoint3D operator+=(CPoint3D pt) {
			x += pt.x;
			y += pt.y;
			z += pt.z;
			return *this;
		}
		CPoint3D operator-=(CPoint3D pt) {
			x -= pt.x;
			y -= pt.y;
			z -= pt.z;
			return *this;
		}
		CPoint3D operator*=(float fval) {
			x *= fval;
			y *= fval;
			z *= fval;
			return *this;
		}
		CPoint3D operator*(float factor) const {
			CPoint3D result;
			result.x = x * factor;
			result.y = y * factor;
			result.z = z * factor;
			return result;
		}
		CPoint3D operator-(const CPoint3D & pt) const {
			CPoint3D result;
			result.x = x - pt.x;
			result.y = y - pt.y;
			result.z = z - pt.z;
			return result;
		}
		CPoint3D operator+(const CPoint3D & pt) const {
			CPoint3D result;
			result.x = x + pt.x;
			result.y = y + pt.y;
			result.z = z + pt.z;
			return result;
		}
		inline float Magnitude(void) {return sqrt(x*x+y*y+z*z);};
};

typedef float Matrix3D[3][3]; 
typedef float Matrix4D[4][4];



#endif
