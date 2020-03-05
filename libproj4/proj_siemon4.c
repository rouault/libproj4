/*
** libproj -- library of cartographic projections
**
** Copyright (c) 2008   Gerald I. Evenden
*/
static const char
LIBPROJ_ID[] = "$Id: proj_siemon4.c,v 3.2 2008/09/21 17:57:15 gie Exp $";
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
#define PROJ_PARMS__
#define PROJ_LIB__
#include	<lib_proj.h>
#define Cx 0.9488499966575886907221825528895435250 /* 1/sqrt(Pi/(2*sqrt(2))) */
#define Cy 2.1078147305108118175915186523697798558 /* 2/Cx */
PROJ_HEAD(siemon4, "Siemon IV") "\n\tPCyl Sph";
FORWARD(s_forward); /* spheroid */
	double hphi = 0.5 * lp.phi;

	xy.x = Cx * lp.lam * cos(lp.phi)/cos(hphi);
	xy.y = Cy * sin(hphi);
	return (xy);
}
INVERSE(s_inverse); /* spheroid */
	double hphi;

	hphi = proj_asin(xy.y/Cy);
	lp.lam = xy.x * cos(hphi) / (Cx * cos(lp.phi = 2. * hphi));
	return (lp);
}
FREEUP; if (P) free(P); }
ENTRY0(siemon4); P->inv = s_inverse; P->fwd = s_forward; P->es = 0.;
ENDENTRY(P)
/*
** $Log: proj_siemon4.c,v $
** Revision 3.2  2008/09/21 17:57:15  gie
** clean out some leftovers
**
** Revision 3.1  2008/09/21 17:52:47  gie
** Initial installation
**
*/
