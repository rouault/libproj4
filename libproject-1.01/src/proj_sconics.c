/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_sconics.c,v 5.3 2009/05/22 14:20:50 gie Exp gie $";
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
	double	n; \
	double	rho_c; \
	double	rho_0; \
	double	sig; \
	double	c1, c2; \
	enum {EULER, MURD1, MURD2, MURD3, PCONIC, TISSOT, \
		VITK1, BRAUNSC} type;
#define PROJ_LIB__
#include	<project.h>
#define EPS10	1.e-10
#define EPS 1e-10
#define BCOS30 0.86602540378443864676372317075 // cos(30deg) = sqrt(3)/2
#define B30DEG 0.52359877559829887307710723 // 30deg
PROJ_HEAD(tissot, "Tissot")
	"\ntype: Conic\nmode: sphere\n"
	"lat_1= lat_2= standard parallels (required and not equal\n";
PROJ_HEAD(murd1, "Murdoch I")
	"\ntype: Conic\nmode: sphere\n"
	"lat_1= lat_2= standard parallels (required and not equal\n";
PROJ_HEAD(murd2, "Murdoch II")
	"\ntype: Conic\nmode: sphere\n"
	"lat_1= lat_2= standard parallels (required and not equal\n";
PROJ_HEAD(murd3, "Murdoch III")
	"\ntype: Conic\nmode: sphere\n"
	"lat_1= lat_2= standard parallels (required and not equal\n";
PROJ_HEAD(euler, "Euler")
	"\ntype: Conic\nmode: sphere\n"
	"lat_1= lat_2= standard parallels (required and not equal\n";
PROJ_HEAD(pconic, "Perspective Conic")
	"\ntype: Conic\nmode: sphere\n"
	"lat_1= lat_2= standard parallels (required and not equal\n";
PROJ_HEAD(vitk1, "Vitkovsky I")
	"\ntype: Conic\nmode: sphere\n"
	"lat_1= lat_2= standard parallels (required and not equal\n";
PROJ_HEAD(BraunSC, "Braun Stereographic conic")
	"\ntype: Conic\nmode: sphere\n"
	"standard parallel fixed at latitude 30N\n";
/* get common factors for simple conics */
	static int
phi12(PROJ *P, double *del) {
	double p1, p2;
	int err = 0;

	if (P->type != BRAUNSC) {
		if ((proj_param(P->params, "rlat_1", &p1) != 1) ||
			(proj_param(P->params, "rlat_2", &p2) != 1))
			err = -41;
		else {
			*del = 0.5 * (p2 - p1);
			P->sig = 0.5 * (p2 + p1);
			err = (fabs(P->sig) < EPS) ? -42 : 0;
		}
	}
	return err;
}
FORWARD(s_forward); /* spheroid */
	double rho;

	switch (P->type) {
	case EULER:
	case MURD1:
	case MURD3:
		rho = P->rho_c - lp.phi;
		break;
	case TISSOT:
		rho = sqrt((P->rho_c - 2 * sin(lp.phi))/P->n);
		break;
	case VITK1:
	case MURD2:
		rho = P->rho_c + tan(P->sig - lp.phi);
		break;
	case PCONIC:
		rho = P->c2 * (P->c1 - tan(lp.phi - P->sig));
		break;
	case BRAUNSC:
		rho = 3.*(BCOS30 - 0.5 * tan(0.5*(lp.phi + B30DEG)));
		break;
	default:
		rho = P->rho_c - lp.phi;
		break;
	}
	xy.x = rho * sin( lp.lam *= P->n );
	xy.y = P->rho_0 - rho * cos(lp.lam);
	return (xy);
}
INVERSE(s_inverse); /* ellipsoid & spheroid */
	double rho;

	rho = hypot(xy.x, xy.y = P->rho_0 - xy.y);
	if (P->n < 0.) {
		rho = - rho;
		xy.x = - xy.x;
		xy.y = - xy.y;
	}
	lp.lam = atan2(xy.x, xy.y) / P->n;
	switch (P->type) {
	case EULER:
	case MURD1:
	case MURD3:
		lp.phi = P->rho_c - rho;
		break;
	case TISSOT:
		lp.phi = proj_asin(0.5 * (P->rho_c - P->n * rho * rho));
		break;
	case PCONIC:
		lp.phi = atan(P->c1 - rho / P->c2) + P->sig;
		break;
	case VITK1:
	case MURD2:
		lp.phi = P->sig - atan(rho - P->rho_c);
		break;
	case BRAUNSC:
		lp.phi = 2 * atan(2 * (BCOS30 - rho / 3)) - B30DEG;
		break;
	default:
		lp.phi = P->rho_c - rho;
	}
	return (lp);
}
FREEUP; if (P) free(P); }
	static PROJ *
setup(PROJ *P) {
	double del, cs;
	int i;

	if ((i = phi12(P, &del)))
		E_ERROR(i);
	switch (P->type) {
	case TISSOT:
		P->n = sin(P->sig);
		cs = cos(del);
		P->rho_c = P->n / cs + cs / P->n;
		P->rho_0 = sqrt((P->rho_c - 2 * sin(P->phi0))/P->n);
		break;
	case MURD1:
		P->rho_c = sin(del)/(del * tan(P->sig)) + P->sig;
		P->rho_0 = P->rho_c - P->phi0;
		P->n = sin(P->sig);
		break;
	case VITK1:
		P->n = (cs = tan(del)) * sin(P->sig) / del;
		P->rho_c = sqrt(cos(del))/ cs;
		P->rho_0 = P->rho_c + tan(P->sig - P->phi0);
		break;
	case MURD2:
		P->rho_c = (cs = sqrt(cos(del))) / tan(P->sig);
		P->rho_0 = P->rho_c + tan(P->sig - P->phi0);
		P->n = sin(P->sig) * cs;
		break;
	case MURD3:
		P->rho_c = del / (tan(P->sig) * tan(del)) + P->sig;
		P->rho_0 = P->rho_c - P->phi0;
		P->n = sin(P->sig) * sin(del) * tan(del) / (del * del);
		break;
	case EULER:
		P->n = sin(P->sig) * sin(del) / del;
		del *= 0.5;
		P->rho_c = del / (tan(del) * tan(P->sig)) + P->sig;	
		P->rho_0 = P->rho_c - P->phi0;
		break;
	case PCONIC:
		P->n = sin(P->sig);
		P->c2 = cos(del);
		P->c1 = 1./tan(P->sig);
		if (fabs(del = P->phi0 - P->sig) - EPS10 >= M_PI_2)
			E_ERROR(-43);
		P->rho_0 = P->c2 * (P->c1 - tan(del));
		break;
	case BRAUNSC:
		P->n = 0.5;
		P->rho_0 = 3.*(BCOS30 - 0.5 * tan(0.5*(P->phi0 + B30DEG)));
	}
	P->inv = s_inverse;
	P->fwd = s_forward;
	P->es = 0;
	return (P);
}
ENTRY0(euler) P->type = EULER; ENDENTRY(setup(P))
ENTRY0(tissot) P->type = TISSOT; ENDENTRY(setup(P))
ENTRY0(murd1) P->type = MURD1; ENDENTRY(setup(P))
ENTRY0(murd2) P->type = MURD2; ENDENTRY(setup(P))
ENTRY0(murd3) P->type = MURD3; ENDENTRY(setup(P))
ENTRY0(pconic) P->type = PCONIC; ENDENTRY(setup(P))
ENTRY0(vitk1) P->type = VITK1; ENDENTRY(setup(P))
ENTRY0(BraunSC) P->type = BRAUNSC; ENDENTRY(setup(P))
/*
** $Log: proj_sconics.c,v $
** Revision 5.3  2009/05/22 14:20:50  gie
** corrected several of the projections.
** ,
** ,
**
** Revision 5.2  2009/05/19 19:27:50  gie
** corrected comment lines about lat-2 to lat_2 and
** missing factor in perspective conic forward formula
**
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
