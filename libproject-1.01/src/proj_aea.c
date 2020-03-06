/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_aea.c,v 5.2 2009/05/23 14:49:36 gie Exp gie $";
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
# define EPS10	1.e-10
# define TOL7	1.e-7
#define PROJ_PARMS__ \
	double	ec; \
	double	n; \
	double	c; \
	double	dd; \
	double	n2; \
	double  qp; \
	double	rho0; \
	double	rho; \
	double	phi1; \
	double	phi2; \
	void	*en; \
	void	*apa; \
	int		ellips;
#define PROJ_LIB__
# include	<project.h>
PROJ_HEAD(aea, "Albers Equal Area")
	"\ntype: conic\nmode: sphere, ellipse\n"
	"lat_1= lat_2= standard parallels\n"
	"phi_0=(0)\n";
PROJ_HEAD(leac, "Lambert Equal Area Conic")
	"\ntype: conic\nmode: sphere, ellipse\n"
	"tlat_1=  parallel\n"
	"south(=F) polar location of second parallel\n"
	"phi_0=(0)\n";
/* determine latitude angle phi-1 */
# define N_ITER 15
# define EPSILON 1.0e-7
# define TOL 1.0e-10
FORWARD(e_forward); /* ellipsoid & spheroid */
	if ((P->rho = P->c - (P->ellips ? P->n * proj_qsfn(lp.phi, P->apa) :
			P->n2 * sin(lp.phi))) < 0.) F_ERROR
	P->rho = P->dd * sqrt(P->rho);
	xy.x = P->rho * sin( lp.lam *= P->n );
	xy.y = P->rho0 - P->rho * cos(lp.lam);
	return (xy);
}
INVERSE(e_inverse) /* ellipsoid & spheroid */;
	if ((P->rho = hypot(xy.x, xy.y = P->rho0 - xy.y))) {
		if (P->n < 0.) {
			P->rho = -P->rho;
			xy.x = -xy.x;
			xy.y = -xy.y;
		}
		lp.phi =  P->rho / P->dd;
		if (P->ellips) {
			lp.phi = (P->c - lp.phi * lp.phi) / P->n;
			lp.phi = proj_auth_inv(asin(lp.phi/P->qp), P->apa);
		} else if (fabs(lp.phi = (P->c - lp.phi * lp.phi) / P->n2) <= 1.)
			lp.phi = asin(lp.phi);
		else
			lp.phi = lp.phi < 0. ? -M_PI_2 : M_PI_2;
		lp.lam = atan2(xy.x, xy.y) / P->n;
	} else {
		lp.lam = 0.;
		lp.phi = P->n > 0. ? M_PI_2 : - M_PI_2;
	}
	return (lp);
}
FREEUP;
	if (P) {
		if (P->en) free(P->en);
		if (P->apa) free(P->apa);
		free(P);
	}
}
	static PROJ *
setup(PROJ *P) {
	double cosphi, sinphi, t;
	int secant;

	if (fabs(P->phi1 + P->phi2) < EPS10) E_ERROR(-21);
	P->n = sinphi = sin(P->phi1);
	cosphi = cos(P->phi1);
	secant = fabs(P->phi1 - P->phi2) >= EPS10;
	if ((P->ellips = P->es > 0.)) {
		double ml1, m1;

		if (!((P->en = proj_mdist_ini(P->es)))) E_ERROR_0;
		if (!((P->apa = proj_auth_ini(P->es, &t)))) E_ERROR_0;
		m1 = proj_msfn(sinphi, cosphi, P->es);
		ml1 = proj_qsfn(P->phi1, P->apa);
		if (secant) { /* secant cone */
			double ml2, m2;

			sinphi = sin(P->phi2);
			cosphi = cos(P->phi2);
			m2 = proj_msfn(sinphi, cosphi, P->es);
			ml2 = proj_qsfn(P->phi2, P->apa);
			P->n = (m1 * m1 - m2 * m2) / (ml2 - ml1);
		}
		P->ec = 1. - .5 * P->one_es * log((1. - P->e) /
			(1. + P->e)) / P->e;
		P->c = m1 * m1 + P->n * ml1;
		P->dd = 1. / P->n;
		P->rho0 = P->dd * sqrt(P->c - P->n * proj_qsfn(P->phi0, P->apa));
		P->qp = proj_qsfn(M_PI_2, P->apa);
	} else {
		if (secant) P->n = .5 * (P->n + sin(P->phi2));
		P->n2 = P->n + P->n;
		P->c = cosphi * cosphi + P->n2 * sinphi;
		P->dd = 1. / P->n;
		P->rho0 = P->dd * sqrt(P->c - P->n2 * sin(P->phi0));
	}
	P->inv = e_inverse; P->fwd = e_forward;
	return P;
}
ENTRY2(aea, en, apa)
	P->phi1 = P->phi2 = 0.;
	(void)proj_param(P->params, "rlat_1", &P->phi1);
	(void)proj_param(P->params, "rlat_2", &P->phi2);
ENDENTRY(setup(P))
ENTRY2(leac, en, apa)
	int south = 0;
	P->phi1 = 0.;
	(void)proj_param(P->params, "rlat_1", &P->phi1);
	(void)proj_param(P->params, "bsouth", &south);
	P->phi1 = south ? - M_PI_2: M_PI_2;
ENDENTRY(setup(P))
/*
** $Log: proj_aea.c,v $
** Revision 5.2  2009/05/23 14:49:36  gie
** proper initialization procedures.
**
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
