/***************************************
 * Geometries.h
 *
 * CPoint3D and other specific geometry classes.
 *
 * Created:       04-??-1998  BMB
 * Last Modified: 01-23-2006  Steven Schulteis
 ***************************************/

/*  This file was created from a copy of Geometries.h from the old MacMolPlt
 *  source.
 *
 *  License info from original file:
 *
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

#ifndef __MyGeometries__
#define __MyGeometries__

#include <math.h>

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
        CPoint3D operator*(float factor) {
            CPoint3D result;
            result.x = x * factor;
            result.y = y * factor;
            result.z = z * factor;
            return result;
        }
        inline float Magnitude(void) {return sqrt(x*x+y*y+z*z);};
};

typedef float Matrix3D[3][3]; 
typedef float Matrix4D[4][4];

#endif

