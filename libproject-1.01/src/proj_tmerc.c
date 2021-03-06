/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_tmerc.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
PROJ_HEAD(tmerc, "Transverse Mercator")
	"\ntype: Transverse Cylindrical\nmode: sphere, ellipse\n";
PROJ_HEAD(utm, "Universal Transverse Mercator (UTM)")
	"\ntype: Transverse Cylindrical\nmode: ellipse\n"
	"longitude in controled zones: lat_0 adjusted to poper zone CM\n"
	"or may be selected by zone number: zone=(1-60)\n"
	"south(=F) for region south of equatore\n"
	"ellipsoid must be used and x_0 & y_0 preset\n";
#define EPS10	1.e-10
#define aks0	P->esp
#define aks5	P->ml0
#define FC1 1.
#define FC2 .5
#define FC3 .16666666666666666666
#define FC4 .08333333333333333333
#define FC5 .05
#define FC6 .03333333333333333333
#define FC7 .02380952380952380952
#define FC8 .01785714285714285714
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
		FC5 * als * (5. + t * (t - 18.) + n * (14. - 58. * t +
		 n * (13. - 64. * t + n * (4. - 24 * t)))
		+ FC7 * als * (61. + t * ( t * (179. - t) - 479. ) )
		)));
	xy.y = P->k0 * (proj_mdist(lp.phi, sinphi, cosphi, P->en) - P->ml0 +
		sinphi * al * lp.lam * FC2 * ( 1. +
		FC4 * als * (5. - t + n * (9. + 4. * n) +
		FC6 * als * (61. + t * (t - 58.) + n * (270. - 330 * t +
		 n * (445. - 680. * t + n * (324. - 600. * t + n * (88. - 192. * t))))
		+ FC8 * als * (1385. + t * ( t * (543. - t) - 3111.) )
		))));
	return (xy);
}
FORWARD(s_forward); /* sphere */
	double b, cosphi;

	b = (cosphi = cos(lp.phi)) * sin(lp.lam);
	if (fabs(fabs(b) - 1.) <= EPS10) F_ERROR;
	xy.x = aks5 * log((1. + b) / (1. - b));
	if ((b = fabs( xy.y = cosphi * cos(lp.lam) / sqrt(1. - b * b) )) >= 1.) {
		if ((b - 1.) > EPS10) F_ERROR
		else xy.y = 0.;
	} else
		xy.y = acos(xy.y);
	if (lp.phi < 0.) xy.y = -xy.y;
	xy.y = aks0 * (xy.y - P->phi0);
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
			ds * FC4 * (5. + t * (3. - 9. *  n) + n * (1. - 4 * n) -
			ds * FC6*(61. + t*(90. - n*(252. + 90.*t) +
				45.*t) + n*(46. + n*(-3. + t*(-66. + 225.*t) +
				n*(100. + 84.*t + n*(88. - 192.*t))))
		   - ds * FC8 * (1385. + t * (3633. + t * (4095. + 1574. * t)) )
			)));
		lp.lam = d*(FC1 -
			ds*FC3*( 1. + 2.*t + n -
			ds*FC5*(5. + t*(28. + 8.*n + 24.*t) + n * (6. + n*(- 3. +
			   4.*t + n*(-4. + 24.*t)))
		   - ds * FC7 * (61. + t * (662. + t * (1320. + 720. * t)) )
		))) / cosphi;
	}
	return (lp);
}
INVERSE(s_inverse); /* sphere */
	double D = xy.y / aks0 + P->phi0, xp = xy.x / aks0;

	lp.phi = asin(sin(D) / cosh(xp));
	lp.lam = atan2(sinh(xp), cos(D));
	return (lp);
}
FREEUP;
	if (P) {
		if (P->en)
			free(P->en);
		free(P);
	}
}
	static PROJ *
setup(PROJ *P) { /* general initialization */
	if (P->es) {
		if (!((P->en = proj_mdist_ini(P->es))))
			E_ERROR_0;
		P->ml0 = proj_mdist(P->phi0, sin(P->phi0), cos(P->phi0), P->en);
		P->esp = P->es / (1. - P->es);
		P->inv = e_inverse;
		P->fwd = e_forward;
	} else {
		aks0 = P->k0;
		aks5 = .5 * aks0;
		P->inv = s_inverse;
		P->fwd = s_forward;
	}
	return P;
}
ENTRY1(tmerc, en)
ENDENTRY(setup(P))
ENTRY1(utm, en)
	int zone;

	if (!P->es) E_ERROR(-34);
	P->y0 = proj_param(P->params, "bsouth", 0) == 1 ? 10000000. : 0.;
	P->x0 = 500000.;
	if (proj_param(P->params, "izone", &zone) == 1) // zone input ?
		if (zone > 0 && zone <= 60) // check zone range
			--zone;
		else
			E_ERROR(-35)
	else /* nearest central meridian input */
		if (((zone = (int)floor((proj_adjlon(P->lam0) + M_PI) * 30. / M_PI))) < 0)
			zone = 0;
		else if (zone >= 60)
			zone = 59;
	P->lam0 = (zone + .5) * M_PI / 30. - M_PI;
	P->k0 = 0.9996;
	P->phi0 = 0.;
ENDENTRY(setup(P))
/*
** $Log: proj_tmerc.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
