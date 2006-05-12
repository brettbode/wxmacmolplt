/*
 *  (c) 2004 Iowa State University
 *      see the LICENSE file in the top level directory
 */

/*	RysPolynomial.h
	Adapted version of the Rys polynomial code in GAMESS.

	converted from GAMESS FORTRAN source 9-1997 BMB

	Routines:

*/

#ifndef _RYS_POLYNOMIAL_H_
#define _RYS_POLYNOMIAL_H_

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

typedef struct Root {
	double	x;
	double	root[9];
	double	weight[9];
	long	nRoots;
} Root;

void Root123(Root * ldata);
void Root4(Root * ldata);
void Root5(Root * ldata);
void Root6(Root * ldata);
void RysFun(double * x, long n, double * ff);
void RysNod(double * a, double * rt, long k);
void RysSmt(double * c, double * s, long n);

#ifdef __cplusplus
}
#endif  /*  __cplusplus  */

#endif /* #ifndef _RYS_POLYNOMIAL_H_ */

