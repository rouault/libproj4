/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_loxim.c,v 5.2 2009/05/23 14:49:36 gie Exp gie $";
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
	double phi1; \
	double cosphi1; \
	double tanphi1;
#define PROJ_LIB__
#include	<project.h>
PROJ_HEAD(loxim, "Loximuthal")
	"\ntype: Pseudocylindrical\nmode: sphere\n"
	"lat_1=(0) loxodrome origin at lon_0\n";
#define EPS	1e-8
FORWARD(s_forward); /* spheroid */
	xy.y = lp.phi - P->phi1;
	if (fabs(xy.y) < EPS)
		xy.x = lp.lam * P->cosphi1;
	else {
		xy.x = M_PI_4 + 0.5 * lp.phi;
		if (fabs(xy.x) < EPS || fabs(fabs(xy.x) - M_PI_2) < EPS)
			xy.x = 0.;
		else
			xy.x = lp.lam * xy.y / log( tan(xy.x) / P->tanphi1 );
	}
	return (xy);
}
INVERSE(s_inverse); /* spheroid */
	lp.phi = xy.y + P->phi1;
	if (fabs(xy.y) < EPS)
		lp.lam = xy.x / P->cosphi1;
	else
		if (fabs( lp.lam = M_PI_4 + 0.5 * lp.phi ) < EPS ||
			fabs(fabs(lp.lam) - M_PI_2) < EPS)
			lp.lam = 0.;
		else
			lp.lam = xy.x * log( tan(lp.lam) / P->tanphi1 ) / xy.y ;
	return (lp);
}
FREEUP; if (P) free(P); }
ENTRY0(loxim);
	P->phi1 = 0.;

	(void)proj_param(P->params, "rlat_1", &P->phi1);
	if ((P->cosphi1 = cos(P->phi1)) < EPS) E_ERROR(-22);
	P->tanphi1 = tan(M_PI_4 + 0.5 * P->phi1);
	P->inv = s_inverse; P->fwd = s_forward;
	P->es = 0.;
ENDENTRY(P)
/*
** $Log: proj_loxim.c,v $
** Revision 5.2  2009/05/23 14:49:36  gie
** proper initialization procedures.
**
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
