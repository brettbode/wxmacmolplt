/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	Gauss-Hermite quadrature for one-electron integrals
	Adapted from GAMESS in 10-1997, BMB
*/

#include "GaussHermite.h"


//Initialize the set of roots and weights
void InitGaussHermite(GaussHermiteData * ldata) {
	ldata->H1 = 0.0;
	ldata->W1 = 1.77245385090552;
	ldata->H2[0] = -.707106781186548;
	ldata->H2[1] = 0.707106781186548;
	ldata->W2[0] = .8862269254528;
	ldata->W2[1] = .8862269254528;
	ldata->H3[0] = -1.22474487139159;
	ldata->H3[1] = 0.0;
	ldata->H3[2] = 1.22474487139159;
	ldata->W3[0] = .2954089751509;
	ldata->W3[1] = 1.181635900604;
	ldata->W3[2] = .2954089751509;
	ldata->H4[0] = -1.65068012388578;
	ldata->H4[1] = -.524647623275290;
	ldata->H4[2] = .524647623275290;
	ldata->H4[3] = 1.65068012388578;
	ldata->W4[0] = 8.131283544725e-2;
	ldata->W4[1] = 8.049140900055e-1;
	ldata->W4[2] = 8.049140900055e-1;
	ldata->W4[3] = 8.131283544725e-2;
	ldata->H5[0] = -2.02018287045609;
	ldata->H5[1] = -.958572464613819;
	ldata->H5[2] = 0.0;
	ldata->H5[3] = .958572464613819;
	ldata->H5[4] = 2.02018287045609;
	ldata->W5[0] = 1.995324205905e-2;
	ldata->W5[1] = 3.936193231522e-1;
	ldata->W5[2] = 9.453087204829e-1;
	ldata->W5[3] = 3.936193231522e-1;
	ldata->W5[4] = 1.995324205905e-2;
	ldata->H6[0] = -2.350604973674;
	ldata->H6[1] = -1.335849074014;
	ldata->H6[2] = -0.436077411928;
	ldata->H6[3] = 0.436077411928;
	ldata->H6[4] = 1.335849074014;
	ldata->H6[5] = 2.350604973674;
	ldata->W6[0] = 4.530009905509e-3;
	ldata->W6[1] = 1.570673203229e-1;
	ldata->W6[2] = 7.246295952244e-1;
	ldata->W6[3] = 7.246295952244e-1;
	ldata->W6[4] = 1.570673203229e-1;
	ldata->W6[5] = 4.530009905509e-3;
	ldata->H7[0] = -2.651961356835233;
	ldata->H7[1] = -1.673551628767471;
	ldata->H7[2] = -0.816287882858965;
	ldata->H7[3] = 0.0;
	ldata->H7[4] = 0.816287882858965;
	ldata->H7[5] = 1.673551628767471;
	ldata->H7[6] = 2.651961356835233;
	ldata->W7[0] = 9.717812450995e-4;
	ldata->W7[1] = 5.451558281913e-2;
	ldata->W7[2] = 4.256072526101e-1;
	ldata->W7[3] = 8.102646175568e-1;
	ldata->W7[4] = 4.256072526101e-1;
	ldata->W7[5] = 5.451558281913e-2;
	ldata->W7[6] = 9.717812450995e-4;
}

void DoGaussHermite(GaussHermiteData * ldata, GaussHermiteIntegral * lInt) {
	long	NumPts, iPt, ni, nj;
	double	Px, Py, Pz, temp, Ptx, Pty, Ptz, Ax, Ay, Az, Bx, By, Bz;
	double	* H, * W;

	lInt->xIntegral = lInt->yIntegral = lInt->zIntegral = 0.0;
	NumPts = (lInt->NumI + lInt->NumJ)/2;
	switch (NumPts+1) {
		case 1:
			H = &(ldata->H1);
			W = &(ldata->W1);
		break;
		case 2:
			H = ldata->H2;
			W = ldata->W2;
		break;
		case 3:
			H = ldata->H3;
			W = ldata->W3;
		break;
		case 4:
			H = ldata->H4;
			W = ldata->W4;
		break;
		case 5:
			H = ldata->H5;
			W = ldata->W5;
		break;
		case 6:
			H = ldata->H6;
			W = ldata->W6;
		break;
		case 7:
			H = ldata->H7;
			W = ldata->W7;
		break;
	}
	for (iPt=0; iPt<=NumPts; iPt++) {
		Px = Py = Pz = W[iPt];
		temp = H[iPt]*lInt->T;
		Ptx = temp + lInt->x0;
		Pty = temp + lInt->y0;
		Ptz = temp + lInt->z0;
		Ax = Ptx - lInt->xi;
		Ay = Pty - lInt->yi;
		Az = Ptz - lInt->zi;
		Bx = Ptx - lInt->xj;
		By = Pty - lInt->yj;
		Bz = Ptz - lInt->zj;
		for (ni=0; ni<lInt->NumI; ni++) {
			Px *= Ax;
			Py *= Ay;
			Pz *= Az;
		}
		for (nj=0; nj<lInt->NumJ; nj++) {
			Px *= Bx;
			Py *= By;
			Pz *= Bz;
		}
		lInt->xIntegral += Px;
		lInt->yIntegral += Py;
		lInt->zIntegral += Pz;
	}
}
