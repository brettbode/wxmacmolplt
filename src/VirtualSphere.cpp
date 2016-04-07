/*еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее
   VirtualSphere.c

   Implements the Virtual Sphere algorithm for 3D rotation using a 2D input device.
   See paper "A Study in Interactive 3-D Rotation Using 2-D Control Devices" by
   Michael Chen, S. Joy Mountford and Abigail Sellen published in the ACM Siggraph '88
   proceedings (Volume 22, Number 4, August 1988) for more detail.  The code here
   provides a much simpler implementation than that described in the paper.

   Author: Michael Chen, Human Interface Group / ATG
   Copyright й 1991-1993 Apple Computer, Inc.  All rights reserved.

   Part of Virtual Sphere Sample Code Release v1.1
  еееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееееее*/
#include "Globals.h"
#include "Geometries.h"	//Needed for a couple of structure and type definition
#include "VirtualSphere.h"
#include "Math3D.h"

/* Local routine */
static void PointOnUnitSphere (const wxPoint &	p,
							   const wxPoint &	cueCenter,
							   long		cueRadius,
							   CPoint3D	*v);
void Rotate3DPt(Matrix4D rotationMatrix, CPoint3D incoord, CPoint3D *outcoord);
void SetRotation4DMatrix (Matrix4D ModelRotation, Matrix4D rotationMatrix, const CPoint3D *op, const CPoint3D *oq);

/*=================================================================================================
 VirtualSphere

 Determine the axis and angle of rotation from the last 2 locations of the mouse
 relative to the Virtual Sphere cue circle.  
-------------------------------------------------------------------------------------------------*/
void VirtualSphere (const wxPoint &	p,	
					const wxPoint &	q,
					const wxPoint &	cueCenter,
					long		cueRadius,
					Matrix4D rotationMatrix)
{	CPoint3D		op, oq;

	/* Project mouse points to 3-D points on the +z hemisphere of a unit
	 * sphere. */
	PointOnUnitSphere (p, cueCenter, cueRadius, &op);
	PointOnUnitSphere (q, cueCenter, cueRadius, &oq);

	/* Consider the two projected points as vectors from the center of the 
	 * unit sphere. Compute the rotation matrix that will transform vector
	 * op to oq. */
	SetRotationMatrix (rotationMatrix, &op, &oq);
}
void VirtualSphereQD3D (const wxPoint &	p,	
						const wxPoint &	q,
						const wxPoint &	cueCenter,
						long	cueRadius,
						Matrix4D rotationMatrix,
						Matrix4D ModelRotation)
{	CPoint3D		op, oq;

	/* Project mouse points to 3-D points on the +z hemisphere of a unit
	 * sphere. */
	PointOnUnitSphere (p, cueCenter, cueRadius, &op);
	PointOnUnitSphere (q, cueCenter, cueRadius, &oq);

	/* Consider the two projected points as vectors from the center of the 
	 * unit sphere. Compute the rotation matrix that will transform vector
	 * op to oq. */
	SetRotation4DMatrix (ModelRotation, rotationMatrix, &op, &oq);
}

void SetRotation4DMatrix (Matrix4D ModelRotation, Matrix4D rotationMatrix, const CPoint3D *op, const CPoint3D *oq)
{
	float		s, c, t;
	CPoint3D	a, b;
	
	#define ax	(-(a.x))
	#define ay	(-(a.y))
	#define az	(-(a.z))
	#define ax2	(ax * ax)
	#define ay2	(ay * ay)
	#define az2	(az * az)

	CrossProduct3D (op, oq, &b);
//Back rotate the rotation vector into the eye coordinate system such that the rotation
//appears to be about the screen axis
	a.x = 	((b.x)*ModelRotation[0][0] +
			( b.y)*ModelRotation[0][1] +
			( b.z)*ModelRotation[0][2]);
	a.y =	((b.x)*ModelRotation[1][0] +
			( b.y)*ModelRotation[1][1] +
			( b.z)*ModelRotation[1][2]);
	a.z =	((b.x)*ModelRotation[2][0] +
			( b.y)*ModelRotation[2][1] +
			( b.z)*ModelRotation[2][2]);
	s = a.Magnitude();
	c = DotProduct3D (op, oq);
	t = 1 - c;

	if (s > 0) {
		a.x /= s;
		a.y /= s;
		a.z /= s;
	}

	rotationMatrix[0][0] = t*ax2+c;
	rotationMatrix[1][0] = t*ax*ay+s*az;
	rotationMatrix[2][0] = t*ax*az-s*ay;

	rotationMatrix[0][1] = t*ax*ay-s*az;
	rotationMatrix[1][1] = t*ay2+c;
	rotationMatrix[2][1] = t*ay*az+s*ax;

	rotationMatrix[0][2] = t*ax*az+s*ay;
	rotationMatrix[1][2] = t*ay*az-s*ax;
	rotationMatrix[2][2] = t*az2+c;

	rotationMatrix[0][3] = rotationMatrix[1][3] = rotationMatrix[2][3] = 
	rotationMatrix[3][0] = rotationMatrix[3][1] = rotationMatrix[3][2] = 0.0;
	rotationMatrix[3][3] = 1.0;

	#undef ax
	#undef ay
	#undef az
	#undef ax2
	#undef ay2
	#undef az2
}

/*=================================================================================================
 PointOnUnitSphere

 Project a 2D point on a circle to a 3D point on the +z hemisphere of a unit sphere.
 If the 2D point is outside the circle, it is first mapped to the nearest point on
 the circle before projection.
 Orthographic projection is used, though technically the field of view of the camera
 should be taken into account.  However, the discrepancy is neglegible.
-------------------------------------------------------------------------------------------------*/
static void PointOnUnitSphere (const wxPoint &	p,
							   const wxPoint &	cueCenter,
							   long		cueRadius,
							   CPoint3D	*v)
{	float	length;
	float	lengthSquared;
	
	/* Turn the mouse points into vectors relative to the center of the circle
	 * and normalize them.  Note we need to flip the y value since the 3D coordinate
	 * has positive y going up. */
	v->x = (float)  (p.x - cueCenter.x) / (float) cueRadius;
//	v->y = (float)  (p.v - cueCenter.v) / (float) cueRadius;
	v->y = (float)  (cueCenter.y - p.y) / (float) cueRadius;

	lengthSquared = v->x*v->x + v->y*v->y;
	
	/* Project the point onto the sphere, assuming orthographic projection.
	 * Points beyond the virtual sphere are normalized onto 
	 * edge of the sphere (where z = 0). */
	if (lengthSquared < 1.0) {
		v->z = (float) sqrt (1.0 - lengthSquared);
	} else {
		length = (float) sqrt (lengthSquared);
		v->x /= length;
		v->y /= length;
		v->z = 0.0;
	}
}
