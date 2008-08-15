/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*
 Math3D.h

 3D math routines

 Modified and added to for MacMolPlt by Brett Bode 8-94
 Added Euler angle routines and separated out math routines 4/98

 Portions by:
   Author: Michael Chen, Human Interface Group / ATG
   Copyright © 1991-1993 Apple Computer, Inc.  All rights reserved.
   Part of Virtual Sphere Sample Code Release v1.1

*/

#ifndef	__MATH3D__
#define __MATH3D__

void InitRotationMatrix(Matrix4D RotationM);
void Rotate3DPt(const Matrix4D rotationMatrix, const CPoint3D & incoord, CPoint3D *outcoord);
void Rotate3DOffset(const Matrix4D rotationMatrix, const CPoint3D & incoord, CPoint3D *outcoord);
void SortzBuffer(const CPoint3D coord[], long zBuffer[], long natoms);
void BackRotate3DOffset(const Matrix4D rotMatrix, const CPoint3D * in, CPoint3D * out);
void MatrixToEulerAngles(Matrix4D rotMatrix, float * phi, float * psi, float * theta);
void EulerAnglesToMatrix(Matrix4D rotMatrix, float phi, float psi, float theta);
void CopyMatrix (const Matrix4D fromMatrix, Matrix4D toMatrix);
void CrossProduct3D (const CPoint3D *a, const CPoint3D *b, CPoint3D *aCrossB);
float DotProduct3D (const CPoint3D *a, const CPoint3D *b);
float DeterminantMatrix(const Matrix4D A);
void InverseMatrix(const Matrix4D A, Matrix4D AInverse);
void MultiplyMatrix (Matrix4D a, Matrix4D b, Matrix4D aTimesB);
void Normalize3D (CPoint3D *v);
void OrthogonalizeRotationMatrix(Matrix4D a);
void SetRotationMatrix (Matrix4D rotationMatrix, const CPoint3D *op, const CPoint3D *oq);
void SetPlaneRotation(Matrix4D rotationMatrix, const CPoint3D & op, const CPoint3D & oq);
void BackRotate3DPt(Matrix4D rotationMatrix, CPoint3D incoord, CPoint3D *outcoord);
void UnitCrossProduct3D (const CPoint3D *a, const CPoint3D *b, CPoint3D *aCrossB);

void SymmetricJacobiDiagonalization(double * SymMatrix, double * EigenVectors,
	double * EigenValues, long NumVectors, long Dimension);
void SortEigenValues(double * EigenVectors, double * EigenValues, long Dimension);
void JacobiDiagonalization(double * Matrix, double * EigenVectors, long Dimension,
	long NumVectors, long Start, long End);
double EPSLON(double x);
void ProjectToPlane(const CPoint3D& normal, const CPoint3D& origin, CPoint3D& pt);
void RotateAroundAxis(Matrix4D m, const CPoint3D& axis, float degrees);
void OrthoVector(const CPoint3D& base_vec, CPoint3D& ortho_vec);

#endif /*__MATH3D__*/
