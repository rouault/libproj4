/*
** libproj -- library of cartographic projections
**
** Copyright (c) 2003, 2006, 2008   Gerald I. Evenden
*/
static const char
LIBPROJ_ID[] = "$Id: proj_hammer.c,v 3.3 2008/11/06 18:53:56 gie Exp $";
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
	int mode; \
	double w; \
	double sp, cp, slw, clw, d, qd; \
	double q3, q3rw, q3w; \
	double xm, ym;
#define PROJ_LIB__
# include	<lib_proj.h>
#define SQRT2 1.4142135623730950488016887242096980786
#define TOL 1e-10
#define MODE_HAMMER 1
#define MODE_ECK_GREIF 2
#define MODE_GENERAL 0
PROJ_HEAD(hammer, "Hammer")
	"\n\tMisc Sph, \n\tW=";
PROJ_HEAD(eck_greif, "Eckert-Greifendorff")
	"\n\tMisc Sph, \n";
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
#ifdef PROJ_HAVE_GSL
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
#endif
FREEUP if (P) free(P); }
	static PROJ *
setup(PROJ *P) {
	P->es = 0.;
	P->fwd = s_forward;
	P->derivs = s_derivs;
#ifdef PROJ_HAVE_GSL
	if (P->mode != MODE_GENERAL)
		P->inv = s_inverse;
#endif
	P->xm = SQRT2 / P->w;
	P->ym = SQRT2;
	P->q3 = SQRT2/2;
	P->q3w = P->w / SQRT2;
	P->q3rw = - SQRT2/(2.*P->w);
	return P;
}
ENTRY0(hammer)
	if (proj_param(P->params, "tW").i) {
		if ((P->w = fabs(proj_param(P->params, "dW").f)) <= 0.) E_ERROR(-27);
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
** Revision 3.3  2008/11/06 18:53:56  gie
** improvements of inverse procedures
**
** Revision 3.2  2008/10/12 16:27:40  gie
** added dinverse call for numeric inverse projection capability
**
** Revision 3.1  2006/01/11 01:38:18  gie
** Initial
**
*/
