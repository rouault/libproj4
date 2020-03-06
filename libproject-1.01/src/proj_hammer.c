/*
** Copyright (c) 2003, 2006, 2008, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_hammer.c,v 5.4 2009/05/17 19:41:47 gie Exp gie $";
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
#include "config.h"
#define PROJ_PARMS__ \
	int mode; \
	double w; \
	double sp, cp, slw, clw, d, qd; \
	double q3, q3rw, q3w; \
	double xm, ym;
#define PROJ_LIB__
# include	<project.h>
#define SQRT2 1.4142135623730950488016887242096980786
#define TOL 1e-10
#define MODE_HAMMER 1
#define MODE_ECK_GREIF 2
#define MODE_GENERAL 0
PROJ_HEAD(hammer, "Hammer")
	"\ntype: Miscellaneous\nmode: sphere\n"
	"W=(0.5) true Hammer projection when omitted\n"; 
PROJ_HEAD(eck_greif, "Eckert-Greifendorff")
	"\ntype: Miscellaneous\nmode: sphere\n";
FORWARD(s_forward); /* spheroid */
	P->sp = sin(lp.phi);
	P->cp = cos(lp.phi);
	P->slw = sin(lp.lam *= P->w);
	P->clw = cos(lp.lam);
	P->qd = 1. / sqrt(P->d = 1 + P->cp * P->clw);

	xy.x = P->xm * P->qd * P->cp * P->slw;
	xy.y = P->ym * P->qd * P->sp;
	return (xy);
}
DERIVS(s_derivs);
	double od = 1. + P->d, den = P->qd / P->d;
	der->x_l = den * P->q3 * P->cp * (P->cp + od * P->clw);
	der->x_p = den * P->q3rw * od * P->sp * P->slw;
	der->y_l = den * P->q3w * P->cp * P->sp * P->slw;
	der->y_p = den * P->q3 * (P->clw + od * P->cp);
}
#if HAVE_LIBGSL
#define MAXy 1.4142135623731
#define MAXy2 2
INVERSE(s_inverse); /* spheroid */
	int negx = xy.x < 0., negy = xy.y < 0.;

	xy.x = fabs(xy.x);
	xy.y = fabs(xy.y);
	if (xy.x < TOL && xy.y < TOL)
		lp.phi = lp.lam = 0.;
	else {
		if (P->mode == MODE_HAMMER) {
			double r;
			lp.phi = (0.0059352*xy.y+(1.114-0.16684*xy.x))*xy.y
				+(-0.011799*xy.x+0.04837)*xy.x-0.03175;
			if ((r = MAXy2 - lp.phi * lp.phi) > 0.0) {
				lp.lam = xy.x * MAXy / sqrt(r);
				if (lp.lam > 2.82843)
					lp.lam = 2.82843;
			} else
				lp.lam = 1.570796;
		} else {
			lp.phi = (0.093931*xy.y+(0.97418-0.042348*xy.x))*xy.y
				+(-0.0010565*xy.x+0.0078493i)*xy.x-0.0031801;
			lp.lam = ((3.629*xy.y+(1.2425*xy.x-5.8619))*xy.y
				+((-0.063661*xy.x-0.32196)*xy.x+2.1456))*xy.y+((0.0083212*xy.x
				-0.01121)*xy.x+1.0081)*xy.x-0.058104;
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
#endif // end LIBGSL condition
FREEUP if (P) free(P); }
	static PROJ *
setup(PROJ *P) {
	P->es = 0.;
	P->fwd = s_forward;
	P->derivs = s_derivs;
#if HAVE_LIBGSL
	if (P->mode != MODE_GENERAL)
		P->inv = s_inverse;
#endif // end LIBGSL condition
	P->xm = SQRT2 / P->w;
	P->ym = SQRT2;
	P->q3 = SQRT2/2;
	P->q3w = P->w / SQRT2;
	P->q3rw = - SQRT2/(2.*P->w);
	return P;
}
ENTRY0(hammer)
	if (proj_param(P->params, "dW", &P->w) == 1) {
		if (fabs(P->w) <= 0.)
			E_ERROR(-27);
		P->mode = MODE_GENERAL;
	} else {
		P->mode = MODE_HAMMER;
		P->w = .5;
	}
ENDENTRY(setup(P))
ENTRY0(eck_greif)
	P->mode = MODE_ECK_GREIF;
	P->w = 0.25;
ENDENTRY(setup(P))
/*
** $Log: proj_hammer.c,v $
** Revision 5.4  2009/05/17 19:41:47  gie
** changes test to HAVE_LIBGSL
**
** Revision 5.3  2009/05/06 14:04:00  gie
** added config.h
**
** Revision 5.2  2009/05/03 00:17:18  gie
** changed to HAVE_GSL
**
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
