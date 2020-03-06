/*
** Copyright (c) 2003, 2006, 2008, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_aitoff.c,v 5.4 2009/05/17 19:41:47 gie Exp gie $";
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
	double	cosphi1; \
	int		mode;
#define PROJ_LIB__
#include "config.h"
#include	<project.h>
#define		TOL	1.e-9
#define MODE_AITOFF 0
#define MODE_WINKEL_TRIPEL 1
#define MODE_BARTH 2
#define MODE_GENERAL 3
PROJ_HEAD(aitoff, "Aitoff")
	"\ntype: Modified Azimuthal\nmode: sphere\n";
PROJ_HEAD(wintri, "Winkel Tripel")
	"\ntype: Modified Azimuthal\nmode: sphere\n"
	"lat_1=(50d28)\n";
PROJ_HEAD(barth, "Bartholomew")
	"\ntype: Modified Azimuthal\nmode: sphere\n"
	"(lat_1=40d fixed)\n";
FORWARD(s_forward); /* spheroid */
	double c, d;

	if (((d = proj_acos(cos(lp.phi) * cos(0.5 * lp.lam))))) { /* basic Aitoff */
		c = sin(lp.phi)/sin(d);
		xy.x = 2. * d * sqrt(fabs(1. - c * c));
		if (lp.lam < 0.)
			xy.x = -xy.x;
		xy.y = d * c;
	} else
		xy.x = xy.y = 0.;
	if (P->mode) { /* Winkel Tripel, Bartholomew, etc. */
		xy.x = (xy.x + lp.lam * P->cosphi1) * 0.5;
		xy.y = (xy.y + lp.phi) * 0.5;
	}
	return xy;
}
DERIVS(s_derivs);
	double sl2, s2p, rC, drC3, sl, sp;
	double	d, cp, cl2, cpcl2;

	d = proj_acos(cpcl2=((cp=cos(lp.phi)) * (cl2=cos(0.5 * lp.lam))));
	rC = 1./(1. - cpcl2 * cpcl2);
	drC3 = d*rC * sqrt(rC);
	sl2 = sin(0.5*lp.lam);
	sp = sin(lp.phi);
	sl = sin(lp.lam);
	s2p = sin(2.*lp.phi);
	der->x_l = rC * cp * cp * sl2 * sl2 +
			drC3 * cpcl2 * sp * sp;
	der->x_p = 0.5 * rC * sl * s2p -
			2. * drC3 * sp * sl2;
	der->y_l = 0.25 * (rC * s2p * sl2 -
			drC3 * sp * sl * cp * cp);
	der->y_p = rC * sp * sp * cl2 +
			drC3 * cp * (1. - cl2 * cl2);
	if (P->mode != MODE_AITOFF) {
		der->x_l = 0.5 * (der->x_l + P->cosphi1);
		der->x_p *= 0.5;
		der->y_l *= 0.5;
		der->y_p = 0.5 * (der->y_p + 1.);
	}
	return(0);
}
#if HAVE_LIBGSL
INVERSE(s_inverse); /* spheroid */
	int negx = xy.x < 0., negy = xy.y < 0.;

	xy.x = fabs(xy.x);
	xy.y = fabs(xy.y);
	if (xy.x < TOL && xy.y < TOL)
		lp.phi = lp.lam = 0.;
	else {
		if (P->mode == MODE_AITOFF) {
			double r;
			lp.phi=(-0.083557*xy.y+(1.1495-0.15666*xy.x))*xy.y+
				(-0.013258*xy.x+0.050367)*xy.x-0.034144;
			if ((r = 2.4674 - xy.y * xy.y) > TOL) {
				lp.lam = xy.x * 1.570796 / sqrt(r);
				if (lp.lam > M_PI)
					lp.lam = M_PI;
			} else
				lp.lam = 1.570796;
		} else if (P->mode == MODE_WINKEL_TRIPEL) {
			lp.phi = (0.030779*xy.y+(0.97772-0.07118*xy.x))*xy.y+
				(-0.022258*xy.x+0.050243)*xy.x-0.0079936;
			lp.lam = ((0.95883*xy.y+(0.81597*xy.x-2.0631))*xy.y+
				((-0.11825*xy.x-0.2113)*xy.x+1.0087))*xy.y+ ((0.023893*xy.x-
				0.048077)*xy.x+1.2454)*xy.x-0.047809;
		} else if (P->mode == MODE_BARTH) {
			lp.phi = (0.037265*xy.y+(0.96681-0.060926*xy.x))*xy.y+
				(-0.019451*xy.x+0.045983)*xy.x-0.0059598;
			lp.lam = ((0.79504*xy.y+(0.66796*xy.x-1.7303))*xy.y+
				((-0.08181*xy.x-0.1881)*xy.x+0.86145))*xy.y+
				((0.012063*xy.x-0.020416)*xy.x+1.1409)*xy.x-0.041698;
		}
		if (proj_gdinverse(P, &lp, xy, TOL))
			lp.phi = lp.lam = HUGE_VAL;
		else {
			if (negx) lp.lam = -lp.lam;
			if (negy) lp.phi = -lp.phi;
		}
	}
	return (lp);
}
#endif // end of LIBGSL condition
FREEUP; if (P) free(P); }
	static PROJ *
setup(PROJ *P) {
	P->fwd = s_forward;
	P->derivs = s_derivs;
	P->es = 0.;
#if HAVE_LIBGSL
	if (P->mode != MODE_GENERAL)
		P->inv = s_inverse;
#endif // end LIBGSL test
	return P;
}
ENTRY0(aitoff)
	P->mode = MODE_AITOFF;
ENDENTRY(setup(P))
ENTRY0(wintri)
	double lat_1;

	if (proj_param(P->params, "tlat_1", &lat_1) == 1) { /* general form */
		P->mode = MODE_GENERAL;
		if ((P->cosphi1 = cos(lat_1)) <= 0.)
			E_ERROR(-22)
	} else { /* 50d28' or acos(2/pi) Winkel Tripel */
		P->mode = MODE_WINKEL_TRIPEL;
		P->cosphi1 = 0.636619772367581343;
	}
ENDENTRY(setup(P))
ENTRY0(barth)
	P->mode = MODE_BARTH;
	P->cosphi1 = 0.69813170079773183076947630; /* 40 degrees */
ENDENTRY(setup(P))
/*
** $Log: proj_aitoff.c,v $
** Revision 5.4  2009/05/17 19:41:47  gie
** changes test to HAVE_LIBGSL
**
** Revision 5.3  2009/05/05 23:40:30  gie
** restored config.h header
**
** Revision 5.2  2009/05/03 00:17:18  gie
** changed to HAVE_GSL
**
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
