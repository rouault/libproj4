/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_nocol.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
PROJ_HEAD(nicol, "Nicolosi Globular")
	"\ntype: Globular\nmode: sphere\n";
#define EPS	1e-10
FORWARD(s_forward); /* spheroid */
	if (fabs(lp.lam) < EPS) {
		xy.x = 0;
		xy.y = lp.phi;
	} else if (fabs(lp.phi) < EPS) {
		xy.x = lp.lam;
		xy.y = 0.;
	} else if (fabs(fabs(lp.lam) - M_PI_2) < EPS) {
		xy.x = lp.lam * cos(lp.phi);
		xy.y = M_PI_2 * sin(lp.phi);
	} else if (fabs(fabs(lp.phi) - M_PI_2) < EPS) {
		xy.x = 0;
		xy.y = lp.phi;
	} else {
		double tb, c, d, m, n, r2, sp;

		tb = M_PI_2 / lp.lam - lp.lam / M_PI_2;
		c = lp.phi / M_PI_2;
		d = (1 - c * c)/((sp = sin(lp.phi)) - c);
		r2 = tb / d;
		r2 *= r2;
		m = (tb * sp / d - 0.5 * tb)/(1. + r2);
		n = (sp / r2 + 0.5 * d)/(1. + 1./r2);
		xy.x = cos(lp.phi);
		xy.x = sqrt(m * m + xy.x * xy.x / (1. + r2));
		xy.x = M_PI_2 * ( m + (lp.lam < 0. ? -xy.x : xy.x));
		xy.y = sqrt(n * n - (sp * sp / r2 + d * sp - 1.) /
			(1. + 1./r2));
		xy.y = M_PI_2 * ( n + (lp.phi < 0. ? xy.y : -xy.y ));
	}
	return (xy);
}
FREEUP; if (P) free(P); }
ENTRY0(nicol) P->es = 0.; P->fwd = s_forward; ENDENTRY(P)
/*
** $Log: proj_nocol.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
