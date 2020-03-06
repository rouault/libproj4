/*
** Copyright (c) 2003, 2006   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_cea.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
	double qp; \
	double *apa;
#define PROJ_LIB__
# include	<project.h>
PROJ_HEAD(cea, "Equal Area Cylindrical")
    "\ntype: cylindrical\nmode: sphere, ellipsoid\n"
	"lat_ts=(lat_0=(0)) if lat true scale omitted, lat_0 used\n"
	"y origin always at lat=0\nk_0=0 set internally by lat_ts\n";
# define EPS	1e-10
FORWARD(e_forward); /* ellipsoid */
	xy.x = P->k0 * lp.lam;
	xy.y = .5 * proj_qsfn(lp.phi, P->apa) / P->k0;
	return (xy);
}
FORWARD(s_forward); /* spheroid */
	xy.x = P->k0 * lp.lam;
	xy.y = sin(lp.phi) / P->k0;
	return (xy);
}
INVERSE(e_inverse); /* ellipsoid */
	lp.phi = proj_auth_inv(asin( 2. * xy.y * P->k0 / P->qp), P->apa);
	lp.lam = xy.x / P->k0;
	return (lp);
}
INVERSE(s_inverse); /* spheroid */
	double t;

	if ((t = fabs(xy.y *= P->k0)) - EPS <= 1.) {
		if (t >= 1.)
			lp.phi = xy.y < 0. ? -M_PI_2 : M_PI_2;
		else
			lp.phi = asin(xy.y);
		lp.lam = xy.x / P->k0;
	} else I_ERROR;
	return (lp);
}
FREEUP;
	if (P) {
		if (P->apa)
			free(P->apa);
		free(P);
	}
}
ENTRY1(cea, apa)
	double t;

	(void)proj_param(P->params, "rlat_ts", &P->phi0);
	if ((P->k0 = cos(t)) < 0.) E_ERROR(-24)
	if (P->es) {
		t = sin(t);
		P->k0 /= sqrt(1. - P->es * t * t);
		if (!(P->apa = proj_auth_ini(P->es, &t))) E_ERROR_0;
		P->qp = proj_qsfn(M_PI_2, P->apa);
		P->inv = e_inverse;
		P->fwd = e_forward;
	} else {
		P->inv = s_inverse;
		P->fwd = s_forward;
	}
ENDENTRY(P)
/*
** $Log: proj_cea.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
