/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_winks.c,v 5.2 2009/05/23 14:49:36 gie Exp gie $";
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
	double	cosphi1;
#define PROJ_LIB__
# include	<project.h>
PROJ_HEAD(winks, "Winkel II-Snyder")
	"\ntype: Pseudocylindrical--=combo of Mollweide and Equirectangular\n"
	"mode: sphere\n"
	"lat_1=(58d20') controls std. parallels and width of flat pole\n"
	"No inverse\n";
#define MAX_ITER    10
#define LOOP_TOL    1e-7
FORWARD(s_forward); /* spheroid */
	double k, V;
	int i;

	xy.y = lp.phi * (2/M_PI);
	k = M_PI * sin(lp.phi);
	lp.phi *= 1.8;
	for (i = MAX_ITER; i ; --i) {
		lp.phi -= V = (lp.phi + sin(lp.phi) - k) /
			(1. + cos(lp.phi));
		if (fabs(V) < LOOP_TOL)
			break;
	}
	if (!i)
		lp.phi = (lp.phi < 0.) ? -M_PI_2 : M_PI_2;
	else
		lp.phi *= 0.5;
	xy.x = 0.5 * lp.lam * (cos(lp.phi) + P->cosphi1);
	xy.y = M_PI_4 * (sin(lp.phi) + xy.y);
	return (xy);
}
FREEUP; if (P) free(P); }
ENTRY0(winks)
	double phi = 0.;

	if (proj_param(P->params, "rlat_1", &phi) != 1)
		phi = (50.+28./60)*M_PI/180;
	P->cosphi1 = cos(phi);
	P->es = 0.; P->inv = 0; P->fwd = s_forward;
ENDENTRY(P)
/*
** $Log: proj_winks.c,v $
** Revision 5.2  2009/05/23 14:49:36  gie
** proper initialization procedures.
**
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
