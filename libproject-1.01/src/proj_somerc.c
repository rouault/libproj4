/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_somerc.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
	double	kR; \
	void *en, *en2;
#define PROJ_LIB__
#include	<project.h>
PROJ_HEAD(somerc, "Swiss. Obl. Mercator")
	"\ntype: Oblique Cylindrical\nmode: ellipse\n"
	"For CH1903 grid system--=Gauss transform/translation\n";
#define EPS	1.e-10
#define NITER 6
FORWARD(e_forward);

	lp = proj_translate(proj_gauss(lp, P->en), P->en2);
	xy.x = P->kR * lp.lam;
	xy.y = P->kR * log(tan(M_PI_4 + 0.5 * lp.phi));
	return (xy);
}
INVERSE(e_inverse); /* ellipsoid & spheroid */

	lp.phi = 2. * (atan(exp(xy.y / P->kR)) - M_PI_4);
	lp.lam = xy.x / P->kR;
	return (proj_inv_gauss(proj_inv_translate(lp, P->en2), P->en));
}
FREEUP; if (P){
	if (P->en) free(P->en);
	if (P->en2) free(P->en2);
	free(P);
	}
}
ENTRY2(somerc, en, en2)
	double phip0, Rc;

	if (!(P->en = proj_gauss_ini(P->e, P->phi0, &phip0, &Rc))) E_ERROR_0;
	if (!(P->en2 = proj_translate_ini(M_PI_2 - phip0, 0.))) E_ERROR_0;
	P->kR = P->k0 * Rc;
	P->inv = e_inverse;
	P->fwd = e_forward;
ENDENTRY(P)
/*
** $Log: proj_somerc.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/