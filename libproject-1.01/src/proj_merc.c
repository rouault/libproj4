/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_merc.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
#define PROJ_LIB__
#include	<project.h>
PROJ_HEAD(merc, "Mercator")
	"\ntype: Cylindrical\nmode: sphere, ellipse\n"
	"lat_ts=(0) latitude of true scale\n";
#define EPS10 1.e-10
FORWARD(e_forward); /* ellipsoid */
	if (fabs(fabs(lp.phi) - M_PI_2) <= EPS10) F_ERROR;
	xy.x = P->k0 * lp.lam;
	xy.y = - P->k0 * log(proj_tsfn(lp.phi, sin(lp.phi), P->e));
	return (xy);
}
FORWARD(s_forward); /* spheroid */
	if (fabs(fabs(lp.phi) - M_PI_2) <= EPS10) F_ERROR;
	xy.x = P->k0 * lp.lam;
	xy.y = P->k0 * log(tan(M_PI_4 + .5 * lp.phi));
	return (xy);
}
INVERSE(e_inverse); /* ellipsoid */
	if ((lp.phi = proj_phi2(exp(- xy.y / P->k0), P->e)) == HUGE_VAL) I_ERROR;
	lp.lam = xy.x / P->k0;
	return (lp);
}
INVERSE(s_inverse); /* spheroid */
	lp.phi = M_PI_2 - 2. * atan(exp(-xy.y / P->k0));
	lp.lam = xy.x / P->k0;
	return (lp);
}
FREEUP; if (P) free(P); }
ENTRY0(merc)
	double phits;

	if ((proj_param(P->params, "rlat_ts", &phits) == 1) &&
		((phits = fabs(phits)) >= M_PI_2))
		E_ERROR(-24);
	if (P->es) { /* ellipsoid */
		if (phits > 0.)
			P->k0 = proj_msfn(sin(phits), cos(phits), P->es);
		P->inv = e_inverse;
		P->fwd = e_forward;
	} else { /* sphere */
		if (phits > 0.)
			P->k0 = cos(phits);
		P->inv = s_inverse;
		P->fwd = s_forward;
	}
ENDENTRY(P)
/*
** $Log: proj_merc.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
