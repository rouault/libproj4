/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_lcc.c,v 5.2 2009/05/23 14:49:36 gie Exp gie $";
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
#define PROJ_PARMS__ \
	double	phi1; \
	double	phi2; \
	double	n, belgiuma; \
	double	rho; \
	double	rho0; \
	double	c; \
	int		ellips, westo;
#define PROJ_LIB__
#include	<project.h>
/* Belgium correction factor 29.2985 in radians */
#define BELGIUMA 0.0001420431363598774030819409832
PROJ_HEAD(lcc, "Lambert Conformal Conic")
	"\ntype: Conic\nmode: sphere, ellipse\n"
	"lat_1= and lat_2= for secant cone\n"
	"lat_1=, k_0= for tangent cone\n"
	"westo(=F) belgium(=F) apply Belgium correction\n";
# define EPS10	1.e-10
FORWARD(e_forward); /* ellipsoid & spheroid */
	double theta;

	if (fabs(fabs(lp.phi) - M_PI_2) < EPS10) {
		if ((lp.phi * P->n) <= 0.) F_ERROR;
		P->rho = 0.;
		}
	else
		P->rho = P->c * (P->ellips ? pow(proj_tsfn(lp.phi, sin(lp.phi),
			P->e), P->n) : pow(tan(M_PI_4 + .5 * lp.phi), -P->n));
	theta = P->n * lp.lam - P->belgiuma;
	xy.x = P->k0 * (P->rho * sin( theta ) );
	if (P->westo) xy.x = -xy.x;
	xy.y = P->k0 * (P->rho0 - P->rho * cos( theta ) );
	return (xy);
}
INVERSE(e_inverse); /* ellipsoid & spheroid */
	xy.x /= P->k0;
	if (P->westo) xy.x = -xy.x;
	xy.y /= P->k0;
	if ((P->rho = hypot(xy.x, xy.y = P->rho0 - xy.y))) {
		if (P->n < 0.) {
			P->rho = -P->rho;
			xy.x = -xy.x;
			xy.y = -xy.y;
		}
		if (P->ellips) {
			if ((lp.phi = proj_phi2(pow(P->rho / P->c, 1./P->n), P->e))
				== HUGE_VAL)
				I_ERROR;
		} else
			lp.phi = 2. * atan(pow(P->c / P->rho, 1./P->n)) - M_PI_2;
		lp.lam = (atan2(xy.x, xy.y) + P->belgiuma) / P->n;
	} else {
		lp.lam = 0.;
		lp.phi = P->n > 0. ? M_PI_2 : - M_PI_2;
	}
	return (lp);
}
SPECIAL(fac) {
	if (fabs(fabs(lp.phi) - M_PI_2) < EPS10) {
		if ((lp.phi * P->n) <= 0.) return;
		P->rho = 0.;
	} else
		P->rho = P->c * (P->ellips ? pow(proj_tsfn(lp.phi, sin(lp.phi),
			P->e), P->n) : pow(tan(M_PI_4 + .5 * lp.phi), -P->n));
	fac->code |= IS_ANAL_HK + IS_ANAL_CONV;
	fac->k = fac->h = P->k0 * P->n * P->rho /
		proj_msfn(sin(lp.phi), cos(lp.phi), P->es);
	fac->conv = P->n * lp.lam;
}
FREEUP; if (P) free(P); }
ENTRY0(lcc)
	double cosphi, sinphi;
	int secant;

	P->phi1 = 0.;
	P->westo = 0;
	(void)proj_param(P->params, "rlat_1", &P->phi1);
	if (proj_param(P->params, "tlat_2", &P->phi2) != 1) {
		P->phi2 = P->phi1; // force tangent mode if phi2 not spec'd
		if (proj_param(P->params, "tlat_0", 0) != 1)
			P->phi0 = P->phi1;
	}
	(void)proj_param(P->params, "twesto", &P->westo);
	P->belgiuma = proj_param(P->params, "tbelgium", 0) == 1 ?  BELGIUMA : 0.;
	// cylinder test
	if (fabs(P->phi1 + P->phi2) < EPS10) E_ERROR(-21);
	P->n = sinphi = sin(P->phi1);
	cosphi = cos(P->phi1);
	// secant mode test
	secant = fabs(P->phi1 - P->phi2) >= EPS10;
	if ((P->ellips = (P->es != 0.))) {
		double ml1, m1;

		P->e = sqrt(P->es);
		m1 = proj_msfn(sinphi, cosphi, P->es);
		ml1 = proj_tsfn(P->phi1, sinphi, P->e);
		if (secant) { /* secant cone */
			P->n = log(m1 /
			   proj_msfn(sinphi = sin(P->phi2), cos(P->phi2), P->es));
			P->n /= log(ml1 / proj_tsfn(P->phi2, sinphi, P->e));
		}
		P->c = (P->rho0 = m1 * pow(ml1, -P->n) / P->n);
		P->rho0 *= (fabs(fabs(P->phi0) - M_PI_2) < EPS10) ? 0. :
			pow(proj_tsfn(P->phi0, sin(P->phi0), P->e), P->n);
	} else {
		if (secant)
			P->n = log(cosphi / cos(P->phi2)) /
			   log(tan(M_PI_4 + .5 * P->phi2) /
			   tan(M_PI_4 + .5 * P->phi1));
		P->c = cosphi * pow(tan(M_PI_4 + .5 * P->phi1), P->n) / P->n;
		P->rho0 = (fabs(fabs(P->phi0) - M_PI_2) < EPS10) ? 0. :
			P->c * pow(tan(M_PI_4 + .5 * P->phi0), -P->n);
	}
	P->inv = e_inverse;
	P->fwd = e_forward;
	P->spc = fac;
ENDENTRY(P)
/*
** $Log: proj_lcc.c,v $
** Revision 5.2  2009/05/23 14:49:36  gie
** proper initialization procedures.
**
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
