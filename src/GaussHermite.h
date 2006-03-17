/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	Gauss-Hermite quadrature for one-electron integrals
	Adapted from GAMESS in 10-1997, BMB
*/

#ifndef __GaussHermite__
#define __GaussHermite__

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

typedef struct GaussHermiteData {
	double	H1;
	double	W1;
	double	H2[2];
	double	W2[2];
	double	H3[3];
	double	W3[3];
	double	H4[4];
	double	W4[4];
	double	H5[5];
	double	W5[5];
	double	H6[6];
	double	W6[6];
	double	H7[7];
	double	W7[7];
} GaussHermiteData;

typedef struct GaussHermiteIntegral {
	double	xIntegral;
	double	yIntegral;
	double	zIntegral;
	double	T;
	double	x0, y0, z0;
	double	xi, yi, zi;
	double	xj, yj, zj;
	long	NumI, NumJ;
} GaussHermiteIntegral;


void InitGaussHermite(GaussHermiteData * ldata);
void DoGaussHermite(GaussHermiteData * ldata, GaussHermiteIntegral * lInt);

#ifdef __cplusplus
}
#endif  /*  __cplusplus  */

#endif
