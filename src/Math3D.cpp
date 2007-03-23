/*
 Math3D.cpp

 3D math routines

 Modified and added to for MacMolPlt by Brett Bode 8-94
 Added Euler angle routines and separated out math routines 4/98

 Portions by:
   Author: Michael Chen, Human Interface Group / ATG
   Copyright � 1991-1993 Apple Computer, Inc.  All rights reserved.
   Part of Virtual Sphere Sample Code Release v1.1

*/
#include "Globals.h"
#include "Geometries.h"
#include "Math3D.h"
#include "GlobalExceptions.h"

void InitRotationMatrix(Matrix4D RotationM)
{
//	Init to the identity matrix
	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++)
			RotationM[i][j] = 0.0;
		RotationM[i][i] = 1.0;
	}
}

/*-------------------------------------
 Rotate3DPt						   

 Simple routine to rotate a 3D point 
-------------------------------------*/
void Rotate3DPt(Matrix4D rotationMatrix, CPoint3D incoord, CPoint3D *outcoord)
{
	(*outcoord).x = ((incoord.x)*rotationMatrix[0][0] +
				    ( incoord.y)*rotationMatrix[1][0] +
				    ( incoord.z)*rotationMatrix[2][0])
				    + rotationMatrix[3][0];
	(*outcoord).y = ((incoord.x)*rotationMatrix[0][1] +
				    ( incoord.y)*rotationMatrix[1][1] +
				    ( incoord.z)*rotationMatrix[2][1])
				     + rotationMatrix[3][1];
	(*outcoord).z = ((incoord.x)*rotationMatrix[0][2] +
				    ( incoord.y)*rotationMatrix[1][2] +
				    ( incoord.z)*rotationMatrix[2][2])
				     + rotationMatrix[3][2];
	return ;
}
/*-------------------------------------
 Rotate3DOffset						   

 Simple routine to rotate a 3D offset ignoring the translation part of the rotation Matrix
 Useful for the normal mode vectors which are just offset from the atom center. Thus the
 translation is already taken into account in the atom center. 
-------------------------------------*/
void Rotate3DOffset(Matrix4D rotationMatrix, CPoint3D incoord, CPoint3D *outcoord)
{
	(*outcoord).x = ((incoord.x)*rotationMatrix[0][0] +
				    ( incoord.y)*rotationMatrix[1][0] +
				    ( incoord.z)*rotationMatrix[2][0]);
	(*outcoord).y = ((incoord.x)*rotationMatrix[0][1] +
				    ( incoord.y)*rotationMatrix[1][1] +
				    ( incoord.z)*rotationMatrix[2][1]);
	(*outcoord).z = ((incoord.x)*rotationMatrix[0][2] +
				    ( incoord.y)*rotationMatrix[1][2] +
				    ( incoord.z)*rotationMatrix[2][2]);
}
void SortzBuffer(CPoint3D coord[], long zBuffer[], long natoms)	{
	int			itemp, iatm;
	bool		done=false;

	do {
		done = true;
		for (iatm=0; iatm<(natoms-1); iatm++) {
			if ((coord[zBuffer[iatm]]).z>(coord[zBuffer[iatm+1]]).z) {
				itemp = zBuffer[iatm];
				zBuffer[iatm] = zBuffer[iatm+1];
				zBuffer[iatm+1] = itemp;
				done = false;
			}
		}
		natoms--;			// The max is now at the end so we don't need to check it again
	} while (!done);
} /* SortzBuffer */
void BackRotate3DOffset(Matrix4D rotMatrix, CPoint3D * in, CPoint3D * out) {
	out->x = (in->x * rotMatrix[0][0] + in->y * rotMatrix[0][1] + in->z * rotMatrix[0][2]);
	out->y = (in->x * rotMatrix[1][0] + in->y * rotMatrix[1][1] + in->z * rotMatrix[1][2]);
	out->z = (in->x * rotMatrix[2][0] + in->y * rotMatrix[2][1] + in->z * rotMatrix[2][2]);
}
//Code  converted from :
//  METHOD: NOTES OF  J. APPLEQUIST, 10/16/79 (JA-H-146-148).
//  REFERENCE: H. GOLDSTEIN, "CLASSICAL MECHANICS," P. 107.
void MatrixToEulerAngles(Matrix4D R, float * phi, float * psi, float * theta) {
	if ((1.0-fabs(R[2][2]))>1.0e-5) { //if theta is not 0 or 180
		double sinThetaSquared = 1.0 - R[2][2] * R[2][2];
			//use formulas for sin/cos (psi +/- phi) to get psi and phi
		double cSum = -(R[1][2]*R[2][1] + R[0][2]*R[2][0])/sinThetaSquared;
		double sSum = (R[0][2]*R[2][1] - R[1][2]*R[2][0])/sinThetaSquared;
		double cDiff = (-R[1][2]*R[2][1] + R[0][2]*R[2][0])/sinThetaSquared;
		double sDiff = (R[0][2]*R[2][1] + R[1][2]*R[2][0])/sinThetaSquared;

		if (cSum > 1.0) cSum = 1.0;
		else if (cSum < -1.0) cSum = -1.0;

		double PhiPlusPsi = acos(cSum);
		if (sin(PhiPlusPsi)*sSum < 0.0) PhiPlusPsi *= -1.0;

		if (cDiff > 1.0) cDiff = 1.0;
		else if (cDiff < -1.0) cDiff = -1.0;

		double PhiMinusPsi = acos(cDiff);
		if (sin(PhiMinusPsi)*sDiff < 0.0) PhiMinusPsi *= -1.0;

		*psi = (PhiPlusPsi + PhiMinusPsi) * 0.5;
		*phi = (PhiPlusPsi - PhiMinusPsi) * 0.5;
	} else {	//otherwise we really only have 2 angles
		if (R[0][0] > 1.0) R[0][0] = 1.0;
		else if (R[0][0] < -1.0) R[0][0] = -1.0;

		*psi = acos(R[0][0]);
		if (sin(*psi)*R[1][0] < 0.0) *psi *= -1.0;
		*phi = 0.0;
	}
		double sin2;
	if (fabs(*psi)-1.0e-5 <= 0.0)
		sin2 = -R[1][2]/cos(*psi);
	else
		sin2 = R[0][2]/sin(*psi);

	if (R[2][2] > 1.0) R[2][2] = 1.0;
	else if (R[2][2] < -1.0) R[2][2] = -1.0;

	*theta = acos(R[2][2]);
	if (sin(*theta)*sin2 < 0.0) *theta *= -1.0;

	double RadToDegree = 180.0/acos(-1.0);
	*phi *= RadToDegree;
	*psi *= RadToDegree;
	*theta *= RadToDegree;
}
//reference: "Foundations of Mathematical Physics" by Sadri Hassani pg 32.
void EulerAnglesToMatrix(Matrix4D rotMatrix, float phi, float psi, float theta) {
	double degreeToRad = acos(-1.0)/180.0;
	double cosPhi = cos(phi*degreeToRad);
	double sinPhi = sin(phi*degreeToRad);
	double cosPsi = cos(psi*degreeToRad);
	double sinPsi = sin(psi*degreeToRad);
	double cosTheta = cos(theta*degreeToRad);
	double sinTheta = sin(theta*degreeToRad);

	rotMatrix[0][0] = cosPsi*cosPhi-sinPsi*cosTheta*sinPhi;
	rotMatrix[0][1] = -cosPsi*sinPhi-sinPsi*cosTheta*cosPhi;
	rotMatrix[0][2] = sinPsi*sinTheta;
	rotMatrix[1][0] = sinPsi*cosPhi+cosPsi*cosTheta*sinPhi;
	rotMatrix[1][1] = -sinPsi*sinPhi+cosPsi*cosTheta*cosPhi;
	rotMatrix[1][2] = -cosPsi*sinTheta;
	rotMatrix[2][0] = sinTheta*sinPhi;
	rotMatrix[2][1] = sinTheta*cosPhi;
	rotMatrix[2][2] = cosTheta;
}


/*=================================================================================================
 CopyMatrix
-------------------------------------------------------------------------------------------------*/
typedef struct {		/* Struct to make matrix copying more efficient */
	Matrix4D a;
} MatrixAsStruct;

void CopyMatrix (Matrix4D fromMatrix, Matrix4D toMatrix)
{
	* (MatrixAsStruct *) toMatrix = * (MatrixAsStruct *) fromMatrix;
}

/*=================================================================================================
 CrossProduct3D

 Returns the right-handed cross-product of a and b in c
-------------------------------------------------------------------------------------------------*/
void CrossProduct3D (const CPoint3D *a, const CPoint3D *b, CPoint3D *aCrossB)
{
	aCrossB->x = a->y * b->z - a->z * b->y;
	aCrossB->y = a->z * b->x - a->x * b->z;
	aCrossB->z = a->x * b->y - a->y * b->x;
}

/*=================================================================================================
 DotProduct3D

 Returns the dot-product of a and b
-------------------------------------------------------------------------------------------------*/
float DotProduct3D (const CPoint3D *a, const CPoint3D *b)
{
	return (a->x*b->x + a->y*b->y + a->z*b->z);
}

float DeterminantMatrix(Matrix4D A) {
	return A[0][0]*(A[1][1]*A[2][2] - A[1][2]*A[2][1]) - A[0][1]*(A[1][0]*A[2][2] - A[1][2]*A[2][0])
					+ A[0][2]*(A[1][0]*A[2][1] - A[1][1]*A[2][0]);
}

void InverseMatrix(Matrix4D A, Matrix4D AInverse) {
	float DetA = DeterminantMatrix(A);
	
	AInverse[0][0] = (A[1][1]*A[2][2]-A[1][2]*A[2][1])/DetA;
	AInverse[0][1] = (A[0][2]*A[2][1]-A[0][1]*A[2][2])/DetA;
	AInverse[0][2] = (A[0][1]*A[1][2]-A[0][2]*A[1][1])/DetA;
	AInverse[1][0] = (A[1][2]*A[2][0]-A[1][0]*A[2][2])/DetA;
	AInverse[1][1] = (A[0][0]*A[2][2]-A[0][2]*A[2][0])/DetA;
	AInverse[1][2] = (A[0][2]*A[1][0]-A[0][0]*A[1][2])/DetA;
	AInverse[2][0] = (A[1][0]*A[2][1]-A[1][1]*A[2][0])/DetA;
	AInverse[2][1] = (A[0][1]*A[2][0]-A[0][0]*A[2][1])/DetA;
	AInverse[2][2] = (A[0][0]*A[1][1]-A[0][1]*A[1][0])/DetA;
	AInverse[0][3] = AInverse[1][3] = AInverse[2][3] = 0.0;
	AInverse[3][0] = -1.0*A[3][0];
	AInverse[3][1] = -1.0*A[3][1];
	AInverse[3][2] = -1.0*A[3][2];
	AInverse[3][3] = A[3][3];
}
/*=================================================================================================
 MultiplyMatrix

 Matrix multiplication c = a x b 
-------------------------------------------------------------------------------------------------*/
void MultiplyMatrix (Matrix4D a, Matrix4D b, Matrix4D aTimesB)
{
	long	 i, j, k;

	for (i=3; i>=0; i--) {
		for (j=3; j>=0; j--) {
			float sum = 0.0;
			for (k=3; k>=0; k--) {
				sum += a[i][k] * b[k][j];
			}
			aTimesB[i][j]= sum;
		}
	}
}

/*=================================================================================================
 Normalize3D

 Returns the normalized vector
-------------------------------------------------------------------------------------------------*/
void Normalize3D (CPoint3D *v)
{
    double length;

    length = sqrt ((double) (v->x * v->x + v->y * v->y + v->z * v->z));
	if (length > 0.0) {
		v->x /= length;
		v->y /= length;
		v->z /= length;
    } else {
		/* Vector is zero.  Probably should set some error */
		v->x = v->y = v->z = 0;
/*		DebugMessage ("\p Normalize3D: zero vector!");*/
	}
}

/*=================================================================================================
 SetRotationMatrix

 Computes a rotation matrix that would map (rotate) vectors op onto oq.
 The rotation is about an axis perpendicular to op and oq.
 Note this routine won't work if op or oq are zero vectors, or if they
 are parallel or antiparallel to each other.  Note the implementation below
 is easy to read but is not done in the most efficient way.  It can be
 sped up by reusing some of the temporary results.

 Modification of Michael Pique's formula in 
 Graphics Gems Vol. 1.  Andrew Glassner, Ed.  Addison-Wesley.
-------------------------------------------------------------------------------------------------*/
void SetRotationMatrix (Matrix4D rotationMatrix, const CPoint3D *op, const CPoint3D *oq)
{
	float		s, c, t;
	CPoint3D	a;
	
	#define ax	(-(a.x))
	#define ay	(-(a.y))
	#define az	(-(a.z))
	#define ax2	(ax * ax)
	#define ay2	(ay * ay)
	#define az2	(az * az)

	CrossProduct3D (op, oq, &a);
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
/*	rotationMatrix[0][0] = t*ax2+c;
	rotationMatrix[0][1] = t*ax*ay+s*az;
	rotationMatrix[0][2] = t*ax*az-s*ay;

	rotationMatrix[1][0] = t*ax*ay-s*az;
	rotationMatrix[1][1] = t*ay2+c;
	rotationMatrix[1][2] = t*ay*az+s*ax;

	rotationMatrix[2][0] = t*ax*az+s*ay;
	rotationMatrix[2][1] = t*ay*az-s*ax;
	rotationMatrix[2][2] = t*az2+c;*/

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
//Generate a rotation matrix to rotate the x-y plane to the plane defined by vectors op and oq
void SetPlaneRotation(Matrix4D rotationMatrix, const CPoint3D & op, const CPoint3D & oq) {
	CPoint3D	Vector1, Vector2, Vector3;
	Matrix4D	A;
	
	Vector1 = op;
	Vector2 = oq;
	
	float length = Vector1.Magnitude();
	if (length <= 0.0) return;	//2 points with the same coordinates
	Vector1.x /= length;
	Vector1.y /= length;
	Vector1.z /= length;
	length = Vector2.Magnitude();
	if (length <= 0.0) return;
	Vector2.x /= length;
	Vector2.y /= length;
	Vector2.z /= length;
	float V1DotV2 = DotProduct3D(&Vector2, &Vector1);
	//Make sure the vectors are not parallel or anti-parallel
	if (fabs(V1DotV2) >= 1.0) return;
	Vector2.x -= V1DotV2*Vector1.x;
	Vector2.y -= V1DotV2*Vector1.y;
	Vector2.z -= V1DotV2*Vector1.z;
	Normalize3D(&Vector2);
	CrossProduct3D (&Vector1, &Vector2, &Vector3);
	Normalize3D(&Vector3);
	
	A[0][0] = Vector1.x;
	A[1][0] = Vector1.y;
	A[2][0] = Vector1.z;
	A[0][3] = 0.0;
	A[0][1] = Vector2.x;
	A[1][1] = Vector2.y;
	A[2][1] = Vector2.z;
	A[1][3] = 0.0;
	A[0][2] = Vector3.x;
	A[1][2] = Vector3.y;
	A[2][2] = Vector3.z;
	A[2][3] = A[3][0] = A[3][1] = A[3][2] = 0.0;
	A[3][3] = 1.0;
	
	InverseMatrix(A, rotationMatrix);
}
void BackRotate3DPt(Matrix4D rotationMatrix, CPoint3D incoord, CPoint3D *outcoord)
{
	incoord.x += rotationMatrix[3][0];
	incoord.y += rotationMatrix[3][1];
	incoord.z += rotationMatrix[3][2];
	(*outcoord).x = ((incoord.x)*rotationMatrix[0][0] +
				    ( incoord.y)*rotationMatrix[1][0] +
				    ( incoord.z)*rotationMatrix[2][0]);
	(*outcoord).y = ((incoord.x)*rotationMatrix[0][1] +
				    ( incoord.y)*rotationMatrix[1][1] +
				    ( incoord.z)*rotationMatrix[2][1]);
	(*outcoord).z = ((incoord.x)*rotationMatrix[0][2] +
				    ( incoord.y)*rotationMatrix[1][2] +
				    ( incoord.z)*rotationMatrix[2][2]);
	return ;
}
/*=========================================================================
 * OrthogonalizeRotationMatrix
 *
 * Orthogonalizes a pure rotation matrix (this is not checked!).
 * Assumes the "y-axis" (2nd row) of the matrix is "correct"
 * and make 1st and 3rd row orthogonal to it.
 * Assumes matrix is already close to orthogonal.
 *-------------------------------------------------------------------------*/
void OrthogonalizeRotationMatrix (Matrix4D matrix)
{
    CPoint3D xAxis, yAxis, zAxis;

    yAxis.x = matrix[1][0];
    yAxis.y = matrix[1][1];
    yAxis.z = matrix[1][2];
    zAxis.x = matrix[2][0];
    zAxis.y = matrix[2][1];
    zAxis.z = matrix[2][2];

	Normalize3D (&yAxis);
	UnitCrossProduct3D  (&yAxis, &zAxis, &xAxis);
	UnitCrossProduct3D  (&xAxis, &yAxis, &zAxis);

    matrix[0][0] = xAxis.x;
    matrix[0][1] = xAxis.y;
    matrix[0][2] = xAxis.z;

    matrix[1][0] = yAxis.x;
    matrix[1][1] = yAxis.y;
    matrix[1][2] = yAxis.z;

    matrix[2][0] = zAxis.x;
    matrix[2][1] = zAxis.y;
    matrix[2][2] = zAxis.z;
}

/*=================================================================================================
 UnitCrossProduct3D

 Returns the normalized right-handed cross product of a and b in c
-------------------------------------------------------------------------------------------------*/
void UnitCrossProduct3D (const CPoint3D *a, const CPoint3D *b, CPoint3D *aCrossB)
{
	CrossProduct3D (a, b, aCrossB);
	Normalize3D (aCrossB);
}

void SymmetricJacobiDiagonalization(double * SymMatrix, double * EigenVectors,
	double * EigenValues, long NumVectors, long Dimension) {
	long i, MLength;
	
	MLength = NumVectors * Dimension;
	for (i=0; i<MLength; i++) EigenVectors[i] = 0.0;
	for (i=0; i<NumVectors; i++) EigenVectors[i+i*Dimension] = 1.0;
	
	JacobiDiagonalization(SymMatrix, EigenVectors, Dimension, NumVectors, 0,
		Dimension);
	for (i=0; i<NumVectors; i++)	//Copy over the eigenvalues
		EigenValues[i] = SymMatrix[(i+i*i)/2];
	
	SortEigenValues(EigenVectors, EigenValues, Dimension);
}
//Matrix is a triangular matrix of dimension Dimension which will be diagonalized
//in place. The results are stored in EigenVectors and EigenValues. NumVectors is the
//number of eigenvectors to solve for and start indicates the first vector to consider
//for rotation, end is the last vector to consider for rotation.
void JacobiDiagonalization(double * Matrix, double * EigenVectors, long Dimension,
	long /*NumVectors*/, long Start, long End) {

	const double root2=sqrt((double)2.0);
	long ieaa=0, ieab=0, i;
	double tt=0.0;
	double * big = new double[Dimension];
	long * jbig = new long[Dimension];

	if (!big || !jbig) throw MemoryError();

	double eps = 64.0*EPSLON(1.0);

// loop over the columns (k) of the triangular matrix to determine the largest
// off-diagonal elements in row(i)
	for (i=0; i<Dimension; i++) {
		big[i] = 0.0;
		jbig[i] = 0;
		if ((i>0)&&(i>=Start)) {
			long ii = (i*(i+1))/2;
			long j = MIN(i, End);
			for (long k=0; k<j; k++) {
				if (fabs(big[i]) < fabs(Matrix[ii+k])) {
					big[i] = Matrix[ii+k];
					jbig[i] = k;
				}
			}
		}
	}
// 2x2 Jacobi iterations begin here
	long MaxIt = MAX((Dimension*Dimension+Dimension)/2, 500);
	long Iter = 0;
		double	SD;
		long	jj, j, it;
	while (true) {
		Iter++;
			//Find the smallest diagonal element
		SD = 1.05;
		jj = -1;
		for (j=0; j<Dimension; j++) {
			jj += j + 1;
			SD = MIN(SD, fabs(Matrix[jj]));
		}
		double test = MAX(eps, (1.0e-12)*MAX(SD, 3.0e-06));
			//Find the largest off-diagonal element
		double T = 0.0;
		long I1 = MAX(1, Start);
		long IB = I1;
		for (i=I1; i<Dimension; i++) {
			if (T<fabs(big[i])) {
				T = fabs(big[i]);
				IB = i;
			}
		}
			//Test for convergance, then determine rotation
		if (T<test) break;

		if (Iter > MaxIt) {
			delete [] big;
			delete [] jbig;
			throw DataError();
		}

		long IA = jbig[IB];
		long IAA = IA*(IA+1)/2;
		long IBB = IB*(IB+1)/2;
		double Dif = Matrix[IAA+IA]-Matrix[IBB+IB];
		double SX, CX, T2X2, T2X25;
		if (fabs(Dif)>((4.0e-16)*T)) {
			T2X2 = big[IB]/Dif;
			T2X25 = T2X2*T2X2;
			if (T2X25 > 2.0e-16) {
				if (T2X25 > 8.0e-09) {
					if (T2X25 > 3.0e-06) {
						T = 0.25/sqrt(0.25 + T2X25);
						CX = sqrt(0.5 + T);
						SX = sqrt(0.5-T)*((T2X2>=0.0)?1.0:-1.0);
					} else {
						CX = 1.0 + T2X25*(T2X25*1.375 - 0.5);
						SX = T2X2*(1.0 + T2X25*(T2X25*3.875 - 1.5));
					}
				} else {
					SX = T2X2*(1.0 - 1.5*T2X25);
					CX = 1.0 - 0.5*T2X25;
				}
			} else {
				CX = 1.0;
				SX = T2X2;
			}
		} else {
			SX = CX = root2;
		}
		long IEAR = IAA;
		long IEBR = IBB;
		for (long ir=0; ir<Dimension; ir++) {
			T = Matrix[IEAR]*SX;
			Matrix[IEAR] = Matrix[IEAR]*CX + Matrix[IEBR]*SX;
			Matrix[IEBR] = T - Matrix[IEBR]*CX;
			if ((ir-IA)>=0) {
				if ((ir-IA)==0) {
					tt = Matrix[IEBR];
					ieaa = IEAR;
					ieab = IEBR;
					Matrix[IEBR] = big[IB];
					IEAR += ir;
					if (jbig[ir] == 0) goto l220;
				} else {
					T = Matrix[IEAR];
					it = IA;
					IEAR += ir;
					if ((ir-IB)==0) {
						Matrix[ieaa] = Matrix[ieaa]*CX + Matrix[ieab]*SX;
						Matrix[ieab] = tt*CX + Matrix[IEBR]*SX;
						Matrix[IEBR] = tt*SX - Matrix[IEBR]*CX;
						IEBR += ir;
					} else {
						if ((ir-IB)>0) {
							if ((fabs(T) < fabs(Matrix[IEBR]))&&
								(IB <= End)) {
								T = Matrix[IEBR];
								it = IB;
							}
							IEBR += ir;
						}
						if (fabs(T) >= fabs(big[ir])) {
							big[ir] = T;
							jbig[ir] = it;
							goto l220;
						}
						if ((IA != jbig[ir]) && (IB != jbig[ir])) goto l220;
					}
				}
				long kq = IEAR-ir-IA+1;
				big[ir] = 0.0;
				long ir1 = MIN(ir, End);
				for (i=0; i<ir1; i++) {
					long k = kq+i;
					if (fabs(big[ir]) < fabs(Matrix[k])) {
						big[ir] = Matrix[k];
						jbig[ir] = i;
					}
				}
			}
l220:		IEAR++;
			IEBR++;
		}
		for (i=0; i<Dimension; i++) {
			double T1 = EigenVectors[IA+i]*CX + EigenVectors[IB+i]*SX;
			double T2 = EigenVectors[IA+i]*SX - EigenVectors[IB+i]*CX;
			EigenVectors[IA+i] = T1;
			EigenVectors[IB+i] = T2;
		}
	}
	
	delete [] big;
	delete [] jbig;
}

void SortEigenValues(double * EigenVectors, double * EigenValues, long Dimension) {
		long i, j, jj;
	for (i=0; i<Dimension; i++) {
		jj = i;
		for (j=i; j<Dimension; j++) {
			if (EigenValues[j] < EigenValues[jj]) jj = j;
		}
		if (jj != i) {
			double tempValue = EigenValues[jj];
			EigenValues[jj] = EigenValues[i];
			EigenValues[i] = tempValue;
			for (j=0; j<Dimension; j++) {
				tempValue = EigenVectors[j+jj*Dimension];
				EigenVectors[j+jj*Dimension] = EigenVectors[j+i*Dimension];
				EigenVectors[j+i*Dimension] = tempValue;
			}
		}
	}
}
double EPSLON(double x) {
/*C*
C*    AUTHORS -
C*       THIS ROUTINE WAS TAKEN FROM EISPACK EDITION 3 DATED 4/6/83
C*       THIS VERSION IS BY S. T. ELBERT, AMES LABORATORY-USDOE NOV 1986
C*
C*    PURPOSE -
C*       ESTIMATE UNIT ROUNDOFF IN QUANTITIES OF SIZE X.
C*
C*    ON ENTRY -
C*       X      - WORKING PRECISION REAL
C*                VALUES TO FIND EPSLON FOR
C*
C*    ON EXIT -
C*       EPSLON - WORKING PRECISION REAL
C*                SMALLEST POSITIVE VALUE SUCH THAT X+EPSLON .NE. ZERO
C*
C*    QUALIFICATIONS -
C*       THIS ROUTINE SHOULD PERFORM PROPERLY ON ALL SYSTEMS
C*       SATISFYING THE FOLLOWING TWO ASSUMPTIONS,
C*          1.  THE BASE USED IN REPRESENTING FLOATING POINT
C*              NUMBERS IS NOT A POWER OF THREE.
C*          2.  THE QUANTITY  A  IN STATEMENT 10 IS REPRESENTED TO
C*              THE ACCURACY USED IN FLOATING POINT VARIABLES
C*              THAT ARE STORED IN MEMORY.
C*       THE STATEMENT NUMBER 10 AND THE GO TO 10 ARE INTENDED TO
C*       FORCE OPTIMIZING COMPILERS TO GENERATE CODE SATISFYING
C*       ASSUMPTION 2.
C*       UNDER THESE ASSUMPTIONS, IT SHOULD BE TRUE THAT,
C*              A  IS NOT EXACTLY EQUAL TO FOUR-THIRDS,
C*              B  HAS A ZERO FOR ITS LAST BIT OR DIGIT,
C*              C  IS NOT EXACTLY EQUAL TO ONE,
C*              EPS  MEASURES THE SEPARATION OF 1.0 FROM
C*                   THE NEXT LARGER FLOATING POINT NUMBER.
C*       THE DEVELOPERS OF EISPACK WOULD APPRECIATE BEING INFORMED
C*       ABOUT ANY SYSTEMS WHERE THESE ASSUMPTIONS DO NOT HOLD.
C*
C*    DIFFERENCES FROM EISPACK 3 -
C*       USE IS MADE OF PARAMETER STATEMENTS AND INTRINSIC FUNCTIONS
C*       --NO EXECUTEABLE CODE CHANGES--
C*
C*    NOTE -
C*       QUESTIONS AND COMMENTS CONCERNING EISPACK SHOULD BE DIRECTED TO
C*       B. S. GARBOW, APPLIED MATH. DIVISION, ARGONNE NATIONAL LAB.
C
*/
	double A,B,C,EPS, result;
	const double zero=0, one=1.0, three=3.0, four=4.0;

	A = four/three;
	do {
		B = A - one;
		C = B + B + B;
		EPS = fabs(C - one);
	} while (EPS == zero);
	result = EPS*fabs(x);

	return result;
}

