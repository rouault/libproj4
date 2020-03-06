/*
** Copyright (c) 2008   Gerald I. Evenden
*/
static const char
 PROJ_ID[] = "$Id: set_ellps.c,v 2.2 2009/03/19 20:06:13 gie Exp gie $";
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
/** @file set_ellps.c
 * @brief support function for geodesic program
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <project.h>
#include "geodesic.h"
/* translate arguments into proj compatible selection of
 * ellipsoid selection.
 */
/** @brief load ellipsoid parameters
 * @param curr pointer to parameter linked list
 */
int set_ellps(PROJ_PARAM_ITEM * curr) {
    const char *help =
	"earth <ellipse parameters>\n\n"
	"This system uses the libproj4 method of selection of earth shape\n"
	"shape paramters.  For standard elliptic parameters select with the\n"
	"'ellps=name' (for current list enter 'earth list') or set each factor\n"
	"with:\n"
	"a=<major axis>\n"
	"b=<minor axis>\n"
	"rf=<reciprocal flattening (1/f)>\n"
	"f=<flattening>\n"
	"es=<eccentricity squared>\n"
	"R=<sphere radius>\n"
	"The following options can determine appropriate radius from ellipses:\n"
	"R_V radius of sphere of equivalent volume\n"
	"R_A radius of sphere of equivalent surface area\n"
	"R_a radius of arithmetic mean of a and b\n"
	"R_g radius of geometric mean of a and b\n"
	"R_h radius of harmonic mean of a and b\n"
	"R_lat_g=<lat> radius of geometric mean at latitude 'lat'\n";

    arc.E = NULL;		/* reset ellips pointer in arc structure */
    if (proj_param(curr, "bhelp", 0) == 1)
	fputs(help, stdout);
    else if (proj_param(curr, "blist", 0) == 1) {
	const struct PROJ_ELLPS_LIST *le;
	for (le = proj_ellps; le->id; ++le)
	    printf("%9s %-16s %-16s %s\n",
		   le->id, le->major, le->ell, le->name);
    } else {
	int err;
	if ((err = proj_ell_set(curr, &earth.a, &earth.es)) != 0) {
	    fprintf(stderr, "failure on ellipse setup\n");
	    return -1;
	}
	earth.f = earth.es != 0. ? 1. - sqrt(earth.one_es = 1. - earth.es) : 0.;
	/* free parameter list elements */
	arcstate &= ~COMPD;
	arcstate |= EARTHSET;
	arc.E = &earth;
	if (arcstate & P1GOOD) {
	    if (arcstate & P2GOOD)
		proj_ginverse(&arc);
	    else if (arcstate & VECTOR)
		proj_gforward(&arc);
	    arcstate |= COMPD;
	    show_arc(&arc, arcstate, PRNT_DMS, stdout);
	}
    }
    return 0;
}

/**
 * @brief display ellipsoid parameter
 */
void show_ellps(void) {
    if (earth.a <= 0.)
	printf("earth parameters not set\n");
    else if (earth.f == 0.)
	printf("    Spherical earth of radius: %.4f\n", earth.a);
    else
	printf("    Ellipse major axis: %.4f, 1./f = %.7f\n",
	       earth.a, 1. / earth.f);
}
