/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_putp4p.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
	double	C_x, C_y;
#define PROJ_LIB__
# include	<project.h>
PROJ_HEAD(putp4p, "Putnins P4'")
	"\ntype: Pseudocylindrical\nmode: sphere\n";
PROJ_HEAD(weren, "Werenskiold I")
	"\ntype: Pseudocylindrical\nmode: sphere\n";
FORWARD(s_forward); /* spheroid */
	lp.phi = proj_asin(0.883883476 * sin(lp.phi));
	xy.x = P->C_x * lp.lam * cos(lp.phi);
	xy.x /= cos(lp.phi *= 0.333333333333333);
	xy.y = P->C_y * sin(lp.phi);
	return (xy);
}
INVERSE(s_inverse); /* spheroid */
	lp.phi = proj_asin(xy.y / P->C_y);
	lp.lam = xy.x * cos(lp.phi) / P->C_x;
	lp.phi *= 3.;
	lp.lam /= cos(lp.phi);
	lp.phi = proj_asin(1.13137085 * sin(lp.phi));
	return (lp);
}
FREEUP; if (P) free(P); }
	static PROJ *
setup(PROJ *P) {
	P->es = 0.; P->inv = s_inverse; P->fwd = s_forward;
	return P;
}
ENTRY0(putp4p) P->C_x = 0.874038744; P->C_y = 3.883251825; ENDENTRY(setup(P))
ENTRY0(weren) P->C_x = 1.; P->C_y = 4.442882938; ENDENTRY(setup(P))
/*
** $Log: proj_putp4p.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
