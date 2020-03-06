/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_tpeqd.c,v 5.2 2009/05/23 14:49:36 gie Exp gie $";
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
	double cp1, sp1, cp2, sp2, ccs, cs, sc, r2z0, z02, dlam2; \
	double hz0, thz0, rhshz0, ca, sa, lp, lamc;
#define PROJ_LIB__
#include	<project.h>
PROJ_HEAD(tpeqd, "Two Point Equidistant")
	"\ntype: Miscellaneous\nmode: sphere\n"
	"lat_1= lon_1= lat_2= lon_2= coordinates of two points\n";
FORWARD(s_forward); /* sphere */
	double t, z1, z2, dl1, dl2, sp, cp;

	sp = sin(lp.phi);
	cp = cos(lp.phi);
	z1 = proj_acos(P->sp1 * sp + P->cp1 * cp * cos(dl1 = lp.lam + P->dlam2));
	z2 = proj_acos(P->sp2 * sp + P->cp2 * cp * cos(dl2 = lp.lam - P->dlam2));
	z1 *= z1;
	z2 *= z2;
	xy.x = P->r2z0 * (t = z1 - z2);
	t = P->z02 - t;
	xy.y = P->r2z0 * proj_sqrt(4. * P->z02 * z2 - t * t);
	if ((P->ccs * sp - cp * (P->cs * sin(dl1) - P->sc * sin(dl2))) < 0.)
		xy.y = -xy.y;
	return xy;
}
INVERSE(s_inverse); /* sphere */
	double cz1, cz2, s, d, cp, sp;

	cz1 = cos(hypot(xy.y, xy.x + P->hz0));
	cz2 = cos(hypot(xy.y, xy.x - P->hz0));
	s = cz1 + cz2;
	d = cz1 - cz2;
	lp.lam = - atan2(d, (s * P->thz0));
	lp.phi = proj_acos(hypot(P->thz0 * s, d) * P->rhshz0);
	if ( xy.y < 0. )
		lp.phi = - lp.phi;
	/* lam--phi now in system relative to P1--P2 base equator */
	sp = sin(lp.phi);
	cp = cos(lp.phi);
	lp.phi = proj_asin(P->sa * sp + P->ca * cp * (s = cos(lp.lam -= P->lp)));
	lp.lam = atan2(cp * sin(lp.lam), P->sa * cp * s - P->ca * sp) + P->lamc;
	return lp;
}
FREEUP; if (P) free(P); }
ENTRY0(tpeqd)
	double lam_1=0., lam_2=0., phi_1=0., phi_2=0., A12=0., pp;

	/* get control point locations */
	(void) proj_param(P->params, "rlat_1", &phi_1);
	(void) proj_param(P->params, "rlon_1", &lam_1);
	(void) proj_param(P->params, "rlat_2", &phi_2);
	(void) proj_param(P->params, "rlon_2", &lam_2);
	if (phi_1 == phi_2 && lam_1 == lam_2) E_ERROR(-25);
	P->lam0 = proj_adjlon(0.5 * (lam_1 + lam_2));
	P->dlam2 = proj_adjlon(lam_2 - lam_1);
	P->cp1 = cos(phi_1);
	P->cp2 = cos(phi_2);
	P->sp1 = sin(phi_1);
	P->sp2 = sin(phi_2);
	P->cs = P->cp1 * P->sp2;
	P->sc = P->sp1 * P->cp2;
	P->ccs = P->cp1 * P->cp2 * sin(P->dlam2);
	P->z02 = proj_acos(P->sp1 * P->sp2 + P->cp1 * P->cp2 * cos(P->dlam2));
	P->hz0 = .5 * P->z02;
	A12 = atan2(P->cp2 * sin(P->dlam2),
		P->cp1 * P->sp2 - P->sp1 * P->cp2 * cos(P->dlam2));
	P->ca = cos(pp = proj_asin(P->cp1 * sin(A12)));
	P->sa = sin(pp);
	P->lp = proj_adjlon(atan2(P->cp1 * cos(A12), P->sp1) - P->hz0);
	P->dlam2 *= .5;
	P->lamc = M_PI_2 - atan2(sin(A12) * P->sp1, cos(A12)) - P->dlam2;
	P->thz0 = tan(P->hz0);
	P->rhshz0 = .5 / sin(P->hz0);
	P->r2z0 = 0.5 / P->z02;
	P->z02 *= P->z02;
	P->inv = s_inverse; P->fwd = s_forward;
	P->es = 0.;
ENDENTRY(P)
/*
** $Log: proj_tpeqd.c,v $
** Revision 5.2  2009/05/23 14:49:36  gie
** proper initialization procedures.
**
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
