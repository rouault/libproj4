/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_kocc.c,v 5.2 2009/05/23 14:49:36 gie Exp gie $";
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
	void	*en, *en2; \
	double phi1; \
	double phit; \
	double n, kRF; \
	int czech;
#define PROJ_LIB__
#include	<project.h>
PROJ_HEAD(kocc, "Krovak Oblique Conformal Conic")
	"\ntype: Oblique Conic\nmode: ellipse\n"
	"options are complex and manual reference required reading\n"
	"czech(=f) correct for x-y coordinate orientation\n"
	"lat_t= latitude of translated pole\n"
	"lat_1= latitude of tangency point\n";

FORWARD(e_forward); /* ellipsoid */
	double rho, theta;

	lp = proj_translate(proj_gauss(lp, P->en), P->en2);
	rho = P->kRF / pow(tan(.5 * lp.phi + M_PI_4), P->n);
	theta = P->n * lp.lam;
	if (P->czech) {	/* Czech grid mode */
		xy.x = rho * cos(theta);
		xy.y = - rho * sin(theta);
	} else { /* proper math mode */
		xy.x = rho * sin(theta);
		xy.y = - rho * cos(theta);
	}
	return (xy);
}
INVERSE(e_inverse); /* ellipsoid */
	double x, y, rho, theta;

	if (P->czech) {
		x = -xy.y;
		y = -xy.x;
	} else {
		x = xy.x;
		y = xy.y;
	}
	rho = hypot(x, y);
	if (P->n < 0) rho = - rho;
	theta = atan2(x, -y);
	lp.phi = 2. * atan(pow(P->kRF / rho, 1./P->n)) - M_PI_2;
	lp.lam = theta / P->n;
	return (proj_inv_gauss(proj_inv_translate(lp, P->en2), P->en));
}
FREEUP; if (P) {
	if (P->en) free(P->en);
	if (P->en2) free(P->en2);
	free(P);
	}
}
ENTRY0(kocc)
	double Rc, chi;

	P->czech = 0;
	(void)proj_param(P->params, "tczech", &P->czech);
	if ((proj_param(P->params, "rlat_1", &P->phi1) != 1) ||
		(proj_param(P->params, "rlat_t", &P->phit) != 1) )
		E_ERROR(-51);
	if (!(P->en = proj_gauss_ini(P->e, P->phi0, &chi, &Rc))) E_ERROR_0;
	if (!(P->en2 = proj_translate_ini(M_PI + P->phit, 0.))) E_ERROR_0;
	P->n = sin(P->phi1);
	P->kRF = P->k0 * Rc * cos(P->phi1) *
		pow(tan(0.5 * P->phi1 + M_PI_4) , P->n) / P->n;
	P->inv = e_inverse;
	P->fwd = e_forward;
ENDENTRY(P)
/*
** $Log: proj_kocc.c,v $
** Revision 5.2  2009/05/23 14:49:36  gie
** proper initialization procedures.
**
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
