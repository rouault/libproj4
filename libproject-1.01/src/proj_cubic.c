/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_cubic.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
/*lp.phi * (
** Permission is hereby granted, fre of charge, to any person obtaining
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
	double xf; \
	double c1, c3, c5;
#define PROJ_LIB__
#include	<project.h>

PROJ_HEAD(kh_sh, "Kharchenko-Shabanaova")
	"\ntype: Cylindrical\nmode: sphere\n";
PROJ_HEAD(pav_cyl, "Pavlov's")
	"\ntype: Cylindrical\nmode: sphere\n";
PROJ_HEAD(tobler_1, "Tobler's alternate 1")
	"\ntype: Cylindrical\nmode: sphere\n";
PROJ_HEAD(urm_3, "Urmayev III")
	"\ntype: Cylindrical\nmode: sphere\n";
PROJ_HEAD(tobler_2, "Tobler's alternate 2")
	"\ntype: Cylindrical\nmode: sphere\n";
PROJ_HEAD(urm_2, "Urmayev II")
	"\ntype: Cylindrical\nmode: sphere\n";

FORWARD(s_forward); /* spheroid */
	double phi2;

	phi2 = lp.phi * lp.phi;
	xy.x = P->xf * lp.lam;
	xy.y = P->c5 ? P->c1 + phi2 * (P->c3 + phi2 * P->c5)
		: P->c1 + phi2 * P->c3;
	xy.y *= lp.phi;
	return (xy);
}
FREEUP; if (P) free(P); }
	static PROJ *
setup(PROJ *P) {
	P->es = 0.;
	P->inv = 0;
	P->fwd = s_forward;
	return P;
}
ENTRY0(kh_sh)
	P->xf = 0.984807753012208;
	P->c1 = 0.99;
	P->c3 = 0.0026263;
	P->c5 = 0.10734;
ENDENTRY(setup(P))
ENTRY0(pav_cyl)
	P->xf = 1.;
	P->c1 = 1.;
	P->c3 = -0.0510333333333;
	P->c5 = -0.00534;
ENDENTRY(setup(P))
ENTRY0(tobler_2)
	P->xf = 1.;
	P->c1 = 1.;
	P->c3 = 0.166666666667;
	P->c5 = 0.0416666666667;
ENDENTRY(setup(P))
ENTRY0(urm_3)
	P->xf = 1.;
	P->c1 = 0.9281;
	P->c3 = 0.371433333333333333;
	P->c5 = 0.;
ENDENTRY(setup(P))
ENTRY0(urm_2)
	P->xf = 1.;
	P->c1 = 1.;
	P->c3 = 0.1275561329783;
	P->c5 = 0.0133641090422587;
ENDENTRY(setup(P))
ENTRY0(tobler_1)
	P->xf = 1.;
	P->c1 = 1.;
	P->c3 = 0.16666666666667;
	P->c5 = 0.;
ENDENTRY(setup(P))
/*
** $Log: proj_cubic.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
