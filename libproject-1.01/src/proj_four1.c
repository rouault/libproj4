/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_four1.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
#define C 2.467401100272339654708622749
#define CN1 1.467401100272339654708622749
#define TOL 1e-9
#define PROJ_LIB__
# include	<project.h>
PROJ_HEAD(four1, "Fournier Globular I")
	"\ntype: Miscellaneoud\nmode: sphere\nNo inverse\n";
FORWARD(s_forward); /* spheroid */
	double s, p, a;

	if (fabs(lp.lam) < TOL || fabs(fabs(lp.phi) - M_PI_2) < TOL) {
			/* lon == 0 || |phi| == pi/2 */
		xy.x = 0;
		xy.y = lp.phi;
	} else if (fabs(lp.phi) < TOL) { /* lat == 0 */
		xy.x = lp.lam;
		xy.y = 0.;
	} else {
		s = sin(lp.phi);
		if (fabs(fabs(lp.lam)-M_PI_2) < TOL) {
			xy.x = lp.lam * cos(lp.phi);
			xy.y = M_PI_2 * s;
		} else {
			p = fabs(M_PI * s);
			s = (C - lp.phi * lp.phi)/(p - 2.*fabs(lp.phi));
			a = lp.lam * lp.lam / C - 1.;
			xy.y = fabs((sqrt(s*s-a*(C-p*s-lp.lam*lp.lam)) - s)/a);
			if (lp.phi < 0)
				xy.y = -xy.y;
			xy.x = lp.lam * sqrt(1. - xy.y * xy.y / C);
		}
	}
	return (xy);
}
FREEUP; if (P) free(P); }
ENTRY0(four1) P->es = 0.; P->fwd = s_forward; ENDENTRY(P)
/*
** $Log: proj_four1.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
