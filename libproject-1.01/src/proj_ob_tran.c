/*
** Copyright (c) 2003, 2005, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_ob_tran.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
	struct PROJconsts *link; \
	void *en; \
	double costh, sinth; \
	int	rot;
#define PROJ_LIB__
#include	<project.h>
#include	<string.h>
PROJ_HEAD(ob_tran, "General Oblique Transformation")
	"\ntype: General oblique\nmode: sphere\n"
"to_proj= plus parameters for projection\n"
"o_lat_p= o_lon_p= (new pole) or\n"
"o_alpha= o_lon_c= o_lat_c= or\n"
"o_lon_1= o_lat_1= o_lon_2= o_lat_2=\n";
#define TOL 1e-10
FORWARD(forward);

	xy = P->link->fwd(proj_translate(lp, P->en), P->link);
	if (xy.x != HUGE && P->rot) {
		double tmp = xy.x;

		xy.x = xy.x * P->costh - xy.y * P->sinth;
		xy.y = tmp * P->sinth + xy.y * P->costh;;
	}
	return (xy);
}
INVERSE(inverse);

	if (P->rot) {
		double tmp = xy.x;

		xy.x = xy.x * P->costh + xy.y * P->sinth;
		xy.y = tmp * - P->sinth + xy.y * P->costh;;
	}
	lp = P->link->inv(xy, P->link);
	if (lp.lam != HUGE_VAL)
		lp = proj_inv_translate(lp, P->en);
	return (lp);
}
FREEUP;
	if (P) {
		if (P->link)
			(*(P->link->pfree))(P->link);
		if (P->en) free(P->en);
		free(P);
	}
}
ENTRY1(ob_tran, link)
	int i;
	double phip, lamp, theta;
	const char *name, *s;

	/* get name of projection to be translated */
	if (proj_param(P->params, "so_proj", &name) != 1)
		E_ERROR(-26);
	for (i = 0; (s = proj_list[i].id) && proj_casecmp(name, s) ; ++i) ;
	if (!s || !(P->link = (*proj_list[i].proj)(0))) E_ERROR(-37);
	/* copy existing header into new */
	P->es = 0.; /* force to spherical */
	P->link->params = P->params;
	P->link->over = P->over;
	P->link->a = P->a;
	P->link->es = P->es;
	P->link->ra = P->ra;
	P->link->lam0 = P->lam0;
	P->link->phi0 = P->phi0;
	P->link->x0 = P->x0;
	P->link->y0 = P->y0;
	P->link->k0 = P->k0;
	/* force spherical earth */
	P->link->one_es = P->link->rone_es = 1.;
	P->link->es = P->link->e = 0.;
	if (!(P->link = proj_list[i].proj(P->link))) {
		freeup(P);
		return 0;
	}
	if ((P->rot = (proj_param(P->params, "rrot", &theta) == 1)) &&
				(theta != 0.)) {
		P->costh = cos(theta);
		P->sinth = sin(-theta);
	}
	if (proj_param(P->params, "to_alpha", 0) == 1) {
		double lamc, phic, alpha;

		(void) proj_param(P->params, "ro_lon_c", &lamc);
		(void) proj_param(P->params, "ro_lat_c", &phic);
		(void) proj_param(P->params, "ro_alpha", &alpha);
		if (fabs(fabs(phic) - M_PI_2) <= TOL)
			E_ERROR(-32);
		lamp = proj_atan2(-cos(alpha), -sin(alpha) * sin(phic));
		P->lam0 = lamc + proj_atan2(-cos(alpha), -sin(alpha) * sin(phic)) - M_PI;
		phip = fabs(proj_asin(cos(phic) * sin(alpha)));
		lamp = P->link->lam0;
	} else if (proj_param(P->params, "ro_lat_p", &lamp) == 1)
		/* specified new pole */
		phip = proj_param(P->params, "ro_lat_p", &phip);
	else { /* specified new "equator" points */
		double lam1, lam2, phi1, phi2, con;

		(void) proj_param(P->params, "ro_lon_1", &lam1);
		(void) proj_param(P->params, "ro_lat_1", &phi1);
		(void) proj_param(P->params, "ro_lon_2", &lam2);
		(void) proj_param(P->params, "ro_lat_2", &phi2);
		if (fabs(phi1 - phi2) <= TOL ||
			(con = fabs(phi1)) <= TOL ||
			fabs(con - M_PI_2) <= TOL ||
			fabs(fabs(phi2) - M_PI_2) <= TOL) E_ERROR(-33);
		lamp = atan2(cos(phi1) * sin(phi2) * cos(lam1) -
			sin(phi1) * cos(phi2) * cos(lam2),
			sin(phi1) * cos(phi2) * sin(lam2) -
			cos(phi1) * sin(phi2) * sin(lam1));
		phip = fabs(atan(-cos(lamp - lam1) / tan(phi1)));
		P->lam0 = lamp;
		lamp = P->link->lam0;
	}
	P->fwd = forward;
	P->inv = P->link->inv ? inverse : 0;
	if (!(P->en = proj_translate_ini(phip, lamp))) E_ERROR_0;
ENDENTRY(P)
/*
** $Log: proj_ob_tran.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
