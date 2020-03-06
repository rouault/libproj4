/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_lsat.c,v 5.2 2009/05/23 14:49:36 gie Exp gie $";
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
/* based upon Snyder and Linck, USGS-NMD */
#define PROJ_PARMS__ \
    double a2, a4, b, c1, c3; \
    double q, t, u, w, p22, sa, ca, xj, rlm, rlm2;
#define PROJ_LIB__
#include	<project.h>
PROJ_HEAD(lsat, "Space oblique for LANDSAT")
	"\ntype: Cylindrical\nmode: ellipse\n"
	"lsat= index\npath= index\n";
#define TOL 1e-7
	static void
seraz0(double lam, double mult, PROJ *P) {
    double sdsq, h, s, fc, sd, sq, d__1;

    lam *= (M_PI/180);
    sd = sin(lam);
    sdsq = sd * sd;
    s = P->p22 * P->sa * cos(lam) * sqrt((1. + P->t * sdsq) / ((
	    1. + P->w * sdsq) * (1. + P->q * sdsq)));
    d__1 = 1. + P->q * sdsq;
    h = sqrt((1. + P->q * sdsq) / (1. + P->w * sdsq)) * ((1. + 
	    P->w * sdsq) / (d__1 * d__1) - P->p22 * P->ca);
    sq = sqrt(P->xj * P->xj + s * s);
    P->b += fc = mult * (h * P->xj - s * s) / sq;
    P->a2 += fc * cos(lam + lam);
    P->a4 += fc * cos(lam * 4.);
    fc = mult * s * (h + P->xj) / sq;
    P->c1 += fc * cos(lam);
    P->c3 += fc * cos(lam * 3.);
}
FORWARD(e_forward); /* ellipsoid */
    int l, nn;
    double lamt, xlam, sdsq, c, d, s, lamdp, phidp, lampp, tanph,
		lamtp, cl, sd, sp, fac, sav, tanphi;

	if (lp.phi > M_PI_2)
	    lp.phi = M_PI_2;
	else if (lp.phi < -M_PI_2)
	    lp.phi = -M_PI_2;
	lampp = lp.phi >= 0. ? M_PI_2 : (3*M_PI_2);
	tanphi = tan(lp.phi);
	for (nn = 0;;) {
		sav = lampp;
		lamtp = lp.lam + P->p22 * lampp;
		cl = cos(lamtp);
		if (fabs(cl) < TOL)
		    lamtp -= TOL;
		fac = lampp - sin(lampp) * (cl < 0. ? -M_PI_2 : M_PI_2);
		for (l = 50; l; --l) {
			lamt = lp.lam + P->p22 * sav;
			if (fabs(c = cos(lamt)) < TOL)
			    lamt -= TOL;
			xlam = (P->one_es * tanphi * P->sa + sin(lamt) * P->ca) / c;
			lamdp = atan(xlam) + fac;
			if (fabs(fabs(sav) - fabs(lamdp)) < TOL)
			    break;
			sav = lamdp;
		}
		if (!l || ++nn >= 3 || (lamdp > P->rlm && lamdp < P->rlm2))
			break;
		if (lamdp <= P->rlm)
		    lampp = (5*M_PI_2);
		else if (lamdp >= P->rlm2)
		    lampp = M_PI_2;
	}
	if (l) {
		sp = sin(lp.phi);
		phidp = proj_asin((P->one_es * P->ca * sp - P->sa * cos(lp.phi) * 
			sin(lamt)) / sqrt(1. - P->es * sp * sp));
		tanph = log(tan(M_PI_4 + .5 * phidp));
		sd = sin(lamdp);
		sdsq = sd * sd;
		s = P->p22 * P->sa * cos(lamdp) * sqrt((1. + P->t * sdsq)
			 / ((1. + P->w * sdsq) * (1. + P->q * sdsq)));
		d = sqrt(P->xj * P->xj + s * s);
		xy.x = P->b * lamdp + P->a2 * sin(2. * lamdp) + P->a4 *
			sin(lamdp * 4.) - tanph * s / d;
		xy.y = P->c1 * sd + P->c3 * sin(lamdp * 3.) + tanph * P->xj / d;
	} else
		xy.x = xy.y = HUGE_VAL;
	return xy;
}
INVERSE(e_inverse); /* ellipsoid */
    int nn;
    double lamt, sdsq, s, lamdp, phidp, sppsq, dd, sd, sl, fac, scl, sav, spp;

	lamdp = xy.x / P->b;
	nn = 50;
	do {
		sav = lamdp;
		sd = sin(lamdp);
		sdsq = sd * sd;
		s = P->p22 * P->sa * cos(lamdp) * sqrt((1. + P->t * sdsq)
			 / ((1. + P->w * sdsq) * (1. + P->q * sdsq)));
		lamdp = xy.x + xy.y * s / P->xj - P->a2 * sin(
			2. * lamdp) - P->a4 * sin(lamdp * 4.) - s / P->xj * (
			P->c1 * sin(lamdp) + P->c3 * sin(lamdp * 3.));
		lamdp /= P->b;
	} while (fabs(lamdp - sav) >= TOL && --nn);
	sl = sin(lamdp);
	fac = exp(sqrt(1. + s * s / P->xj / P->xj) * (xy.y - 
		P->c1 * sl - P->c3 * sin(lamdp * 3.)));
	phidp = 2. * (atan(fac) - M_PI_4);
	dd = sl * sl;
	if (fabs(cos(lamdp)) < TOL)
	    lamdp -= TOL;
	spp = sin(phidp);
	sppsq = spp * spp;
	lamt = atan(((1. - sppsq * P->rone_es) * tan(lamdp) * 
		P->ca - spp * P->sa * sqrt((1. + P->q * dd) * (
		1. - sppsq) - sppsq * P->u) / cos(lamdp)) / (1. - sppsq 
		* (1. + P->u)));
	sl = lamt >= 0. ? 1. : -1.;
	scl = cos(lamdp) >= 0. ? 1. : -1;
	lamt -= M_PI_2 * (1. - scl) * sl;
	lp.lam = lamt - P->p22 * lamdp;
	if (fabs(P->sa) < TOL)
	    lp.phi = proj_asin(spp / sqrt(P->one_es * P->one_es + P->es * sppsq));
	else
		lp.phi = atan((tan(lamdp) * cos(lamt) - P->ca * sin(lamt)) /
			(P->one_es * P->sa));
	return lp;
}
FREEUP; if (P) free(P); }
ENTRY0(lsat)
    int land = 0, path = 0;
    double lam, alf, esc, ess;

	(void)proj_param(P->params, "ilsat", &land);
	if (land <= 0 || land > 5) E_ERROR(-28);
	(void)proj_param(P->params, "ipath", &path);
	if (path <= 0 || path > (land <= 3 ? 251 : 233)) E_ERROR(-29);
	if (land <= 3) {
		P->lam0 = (M_PI/180) * 128.87 - (2*M_PI) / 251. * path;
	    P->p22 = 103.2669323;
	    alf = (M_PI/180) * 99.092;
	} else {
		P->lam0 = (M_PI/180) * 129.3 - (2*M_PI) / 233. * path;
	    P->p22 = 98.8841202;
	    alf = (M_PI/180) * 98.2;
	}
	P->p22 /= 1440.;
	P->sa = sin(alf);
	P->ca = cos(alf);
	if (fabs(P->ca) < 1e-9)
	    P->ca = 1e-9;
	esc = P->es * P->ca * P->ca;
	ess = P->es * P->sa * P->sa;
	P->w = (1. - esc) * P->rone_es;
	P->w = P->w * P->w - 1.;
	P->q = ess * P->rone_es;
	P->t = ess * (2. - P->es) * P->rone_es * P->rone_es;
	P->u = esc * P->rone_es;
	P->xj = P->one_es * P->one_es * P->one_es;
	P->rlm = M_PI * (1. / 248. + .5161290322580645);
	P->rlm2 = P->rlm + (2*M_PI);
    P->a2 = P->a4 = P->b = P->c1 = P->c3 = 0.;
	seraz0(0., 1., P);
	for (lam = 9.; lam <= 81.0001; lam += 18.)
	    seraz0(lam, 4., P);
	for (lam = 18; lam <= 72.0001; lam += 18.)
	    seraz0(lam, 2., P);
	seraz0(90., 1., P);
	P->a2 /= 30.;
	P->a4 /= 60.;
	P->b /= 30.;
	P->c1 /= 15.;
	P->c3 /= 45.;
	P->inv = e_inverse; P->fwd = e_forward;
ENDENTRY(P)
/*
** $Log: proj_lsat.c,v $
** Revision 5.2  2009/05/23 14:49:36  gie
** proper initialization procedures.
**
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
