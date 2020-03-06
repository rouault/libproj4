/*
** Copyright (c) 2008, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_gbtmerc.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
	double	esp; \
	double	ml0; \
	void	*en;
#define PROJ_LIB__
#include	<project.h>
PROJ_HEAD(gbtmerc, "Gauss-Boaga Transverse Mercator")
	"\ntype: Transverse Cylinder\nmode: ellipse\n";
#define EPS10	1.e-10
#define aks0	P->esp
#define aks5	P->ml0
#define FC1 1.
#define FC2 .5
#define FC3 .16666666666666666666
#define FC4 .08333333333333333333
#define FC5 .05
#define FC6 .03333333333333333333
FORWARD(e_forward); /* ellipse */
	double al, als, n, cosphi, sinphi, t;

	sinphi = sin(lp.phi); cosphi = cos(lp.phi);
	t = fabs(cosphi) > 1e-10 ? sinphi/cosphi : 0.;
	t *= t;
	al = cosphi * lp.lam;
	als = al * al;
	al /= sqrt(1. - P->es * sinphi * sinphi);
	n = P->esp * cosphi * cosphi;
	xy.x = P->k0 * al * (FC1 +
		FC3 * als * (1. - t + n +
		FC5 * als * (5. + t * (t - 18.) +
			n * (14. - 58. * t))));
	xy.y = P->k0*(proj_mdist(lp.phi, sinphi, cosphi, P->en) -
		P->ml0 + sinphi * al * lp.lam * FC2 * ( 1. +
		FC4 * als * (5. - t + n * (9. + 4. * n) +
		FC6 * als * (61. + t * (t - 58.) +
			n * (270. - 330 * t)))));
	return (xy);
}
INVERSE(e_inverse); /* ellipsoid */
	double n, con, cosphi, d, ds, sinphi, t;

	lp.phi = proj_inv_mdist(P->ml0 + xy.y / P->k0, P->en);
	if (fabs(lp.phi) >= M_PI_2) {
		lp.phi = xy.y < 0. ? -M_PI_2 : M_PI_2;
		lp.lam = 0.;
	} else {
		sinphi = sin(lp.phi);
		cosphi = cos(lp.phi);
		t = fabs(cosphi) > 1e-10 ? sinphi/cosphi : 0.;
		n = P->esp * cosphi * cosphi;
		d = xy.x * sqrt(con = 1. - P->es * sinphi * sinphi) / P->k0;
		con *= t;
		t *= t;
		ds = d * d;
		lp.phi -= (con * ds / (1.-P->es)) * FC2 * (1. -
			ds * FC4 * (5. + t * (3. - 9. *  n) +
				n * (1. - 4 * n) -
			ds * FC6*(61. + t*(90. - n*252. +
				45.*t) + n*(46. - 3.0 * n))));
		lp.lam = d*(FC1 -
			ds*FC3*( 1. + 2.*t + n -
			ds*FC5*(5. + t*(28. + 8.*n + 24.*t) +
				n * (6. - 3.0 )))) / cosphi;
	}
	return (lp);
}
FREEUP;
	if (P) {
		if (P->en)
			free(P->en);
		free(P);
	}
}
ENTRY0(gbtmerc) /* general initialization */

	if (P->es) {
		if (!((P->en = proj_mdist_ini(P->es))))
			E_ERROR_0;
		P->ml0 = proj_mdist(P->phi0, sin(P->phi0), cos(P->phi0), P->en);
		P->esp = P->es / (1. - P->es);
		P->inv = e_inverse;
		P->fwd = e_forward;
	} else E_ERROR(-34);
ENDENTRY(P)
/*
** $Log: proj_gbtmerc.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
