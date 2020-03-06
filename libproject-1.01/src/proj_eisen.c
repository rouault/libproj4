/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_eisen.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
#define CON 5.828427124746190097603377448
#define PROJ_LIB__
# include	<project.h>
PROJ_HEAD(eisen, "Eisenlohr")
	"\ntype: Miscellaneous\nmode: sphere\n"
	"No inverse\n";
FORWARD(s_forward); /* spheroid */
	double s1, c1, q, p, t, c, v;

	s1 = sin(c1 = 0.5 * lp.lam);
	c1 = cos(c1);
	q = cos(t = 0.5 * lp.phi);
	t = sin(t) / (q + sqrt(2. * cos(lp.phi)) * c1);
	c = sqrt(2./(1 + t*t));
	p = sqrt(0.5 * cos(lp.phi));
	v = sqrt((q + p *(c1 + s1))/(q + p*(c1 - s1)));
	xy.x = CON * (-2 * log(v) + c * (v - 1/v));
	xy.y = CON * (-2 * atan(t) + c*t*(v + 1/v));
	return (xy);
}
FREEUP; if (P) free(P); }
ENTRY0(eisen) P->es = 0.; P->fwd = s_forward; ENDENTRY(P)
/*
** $Log: proj_eisen.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
