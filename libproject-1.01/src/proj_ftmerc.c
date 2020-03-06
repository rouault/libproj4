/*
** Copyright (c) 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_ftmerc.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
	double	rho0; \
	double	fc[5], ic[5];
#define PROJ_LIB__
#include	<project.h>
#include <complex.h>
PROJ_HEAD(ftmerc, "French Transverse Mercator")
	"\ntype: Transverse Cylindrical\nmode: ellipse\n";

/* forward series constants */
#define FC00 1.
#define FC02 -0.25
#define FC04 -0.046875
#define FC06 -0.01953125
#define FC08 -0.01068115234375
#define FC22 0.125
#define FC24 -0.01041666666666666666666666667
#define FC26 -0.0087890625
#define FC28 -0.004888237847222222222222222222
#define FC44 0.01692708333333333333333333333
#define FC46 0.0033203125
#define FC48 -0.0004218207465277777777777777778
#define FC66 0.003971354166666666666666666667
#define FC68 0.002090308779761904761904761905
#define FC88 0.001200382293216765873015873016
/* inverse series constants */
#define IC00 1.
#define IC02 -0.25
#define IC04 -0.046875
#define IC06 -0.01953125
#define IC08 -0.01068115234375
#define IC22 0.125
#define IC24 0.02083333333333333333333333333
#define IC26 0.00341796875
#define IC28 0.00001627604166666666666666666667
#define IC44 0.001302083333333333333333333333
#define IC46 0.00234375
#define IC48 0.001516384548611111111111111111
#define IC66 0.0005533854166666666666666666667
#define IC68 0.0006580171130952380952380952381
#define IC88 0.0001064966595362103174603174603
	static void
fs_init(double *s, double es) {
	double t = es * es;
	s[0] = FC00 + es * (FC02 + es * (FC04 + es * (FC06 + es * FC08)));
	s[1] = es * (FC22 + es * (FC24 + es * (FC26 + es * FC28)));
	s[2] = t * (FC44 + es * (FC46 + es * FC48));
	t *= es;
	s[3] = t * (FC66 + es * FC68);
	s[4] = t * es * FC88;
}
	static void
is_init(double *s, double es) {
	double t = es * es;
	s[0] = IC00 + es * (IC02 + es * (IC04 + es * (IC06 + es * IC08)));
	s[1] = es * (IC22 + es * (IC24 + es * (IC26 + es * IC28)));
	s[2] = t * (IC44 + es * (IC46 + es * IC48));
	t *= es;
	s[3] = t * (IC66 + es * IC68);
	s[4] = t * es * IC88;
}
	static double _Complex
cevals(double _Complex z, double *c) {
	double _Complex zp;
	int i;

	zp = z * *c;
	for (i = 2; i <= 8; i += 2)
		zp += *++c * csin(i * z);
	return zp;
}
	static double _Complex
icevals(double _Complex z, double *c) {
	double _Complex zp;
	int i;

	zp = z /= *c;
	for (i = 2; i <= 8; i += 2)
		zp -= *++c * csin(i * z);
	return zp;
}

FORWARD(e_forward); /* ellipsoid */
	double psi, beta, psi_s;
	double _Complex z;

	psi = proj_psi(lp.phi, sin(lp.phi), P->e);
	beta = asin(sin(lp.lam) / cosh(psi));
	psi_s = log(tan(M_PI_4 + 0.5 * beta));
	z = (atan(sinh(psi) / cos(lp.lam)))+ psi_s * I;
	z = cevals(z, P->fc);
	xy.x = cimag(z) * P->k0;
	xy.y = (creal(z) - P->rho0) * P->k0;
	return (xy);
}
INVERSE(e_inverse); /* ellipsoid */
	double _Complex z;
	double L, Ls, psi;

	xy.x /= P->k0;
	xy.y = xy.y / P->k0 + P->rho0;
	z = xy.y + xy.x * I;
	z = icevals(z, P->ic);
	L = creal(z);
	Ls = cimag(z);
	lp.lam = atan(sinh(Ls) / cos(L));
	psi = asin(sin(L) / cosh(Ls));
	L = log(tan(M_PI_4 + 0.5 * psi));
	lp.phi = proj_apsi(L, P->e);
	return (lp);
}
FREEUP;
	if (P) free(P);
}
ENTRY0(ftmerc)
	void *en;
	
	fs_init(P->fc, P->es);
	is_init(P->ic, P->es);
	en = proj_mdist_ini(P->es);
	if (en) {
		P->rho0 = proj_mdist(P->phi0, sin(P->phi0), cos(P->phi0), en);
		free(en);
	}
	P->fwd = e_forward;
	P->inv = e_inverse;
ENDENTRY(P)
/*
** $Log: proj_ftmerc.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
