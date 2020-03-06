/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_gauss.c,v 5.2 2009/05/06 18:46:51 gie Exp gie $";
/*
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/** @file proj_gauss.c
 * @brief reduction to Gaussian sphere
 * @author Gerald I. Evenden
 */
#define PROJ_LIB__
#include <project.h>

#define MAX_ITER 20

struct GAUSS {
	double C;
	double K;
	double e;
	double ratexp;
};
#define EN ((struct GAUSS *)en)
#define DEL_TOL 1e-14
	static double
srat(double esinp, double exp) {
	return(pow((1.-esinp)/(1.+esinp), exp));
}
/** @brief constant struct initialization for Gausian sphere
 * @param[in] e eccentricity
 * @param[in] phi0 central latitude
 * @param[out] chi returned factor
 * @param[out] rc returned sphere radius
 * @return pointer to constant structure
 */
	void *
proj_gauss_ini(double e, double phi0, double *chi, double *rc) {
	double sphi, cphi, es;
	struct GAUSS *en;

	if ((en = (struct GAUSS *)malloc(sizeof(struct GAUSS))) == NULL)
		return (NULL);
	es = e * e;
	EN->e = e;
	sphi = sin(phi0);
	cphi = cos(phi0);  cphi *= cphi;
	*rc = sqrt(1. - es) / (1. - es * sphi * sphi);
	EN->C = sqrt(1. + es * cphi * cphi / (1. - es));
	*chi = asin(sphi / EN->C);
	EN->ratexp = 0.5 * EN->C * e;
	EN->K = tan(.5 * *chi + M_PI_4) / (
		pow(tan(.5 * phi0 + M_PI_4), EN->C) *
		srat(EN->e * sphi, EN->ratexp)  );
	return ((void *)en);
}
/** @brief determine geographic location on Gaussian sphere
 * @param[in] ellipsoid geographic coordinates
 * @param[in] gaussian constant structure
 * @return sphere geographic coordinates
 */
	PROJ_LP
proj_gauss(PROJ_LP elp, const void *en) {
	PROJ_LP slp;

	slp.phi = 2. * atan( EN->K *
		pow(tan(.5 * elp.phi + M_PI_4), EN->C) *
		srat(EN->e * sin(elp.phi), EN->ratexp) ) - M_PI_2;
	slp.lam = EN->C * (elp.lam);
	return(slp);
}
/** @brief determine ellipsoid coordinates from sphere coordinates
 * @param[in] sphere geographic coordinates
 * @param[in] gaussian constant structure
 * @return ellipsoid geographic coordinates
 */
	PROJ_LP
proj_inv_gauss(PROJ_LP slp, const void *en) {
	PROJ_LP elp;
	double num;
	int i;

	elp.lam = slp.lam / EN->C;
	num = pow(tan(.5 * slp.phi + M_PI_4)/EN->K, 1./EN->C);
	for (i = MAX_ITER; i; --i) {
		elp.phi = 2. * atan(num * srat(EN->e * sin(slp.phi), -.5 * EN->e))
			- M_PI_2;
		if (fabs(elp.phi - slp.phi) < DEL_TOL) break;
			slp.phi = elp.phi;
	}	
	/* convergence failed */
	if (!i)
		proj_errno = -17;
	return (elp);
}
/* Revision Log:
** $Log: proj_gauss.c,v $
** Revision 5.2  2009/05/06 18:46:51  gie
** updates for Doxygen
**
** Revision 5.1  2009/04/30 20:47:47  gie
** *** empty log message ***
**
*/
