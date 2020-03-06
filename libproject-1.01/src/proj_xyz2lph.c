/*
** Copyright (c) 2009   Gerald I. Evenden
*/
static const char
 PROJ_ID[] = "$Id: proj_xyz2lph.c,v 5.1 2009/04/30 20:47:47 gie Exp gie $";
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
/** @file proj_xyz2lph.c
 * @brief convert xyz to lat-lon-h
 */
#include <stdlib.h>
#include <math.h>
#include <project.h>
#define TOL 1e-13
#define MAXLOOP 11
/**
 * @brief Convert Cartesian XYZ coordinates to geographic lon/lat/height 
 * @param[in] P1 pointer to XYZ coordinate structure
 * @param[out] P2 pointer to geographic coordinate structure
 * @param[in] ell pointer to ellipsoid constants
 * @return 0 if successful else return -1 if too many iterations
 */
int proj_xyz2lph(PROJ_PT_XYZ * P1, PROJ_PT_LPH * P2, PROJ_ELLIPS * ell) {
    double p, en, ae2, clat, tgla, slat, tglax;
    int count = MAXLOOP, err = 0;

    ae2 = ell->a * ell->es;
    p = hypot(P1->x, P1->y);
    tgla = P1->z / p / (1. - ell->es);
    do {
	tglax = tgla;
	tgla = P1->z / (p - ae2 / sqrt(ell->one_es * tgla * tgla + 1.));
    } while ((fabs(tgla - tglax) > TOL) && --count);
    if (count > 0) {		/* convergence achieved */
	P2->phi = atan(tgla);
	slat = sin(P2->phi);
	clat = cos(P2->phi);
	en = ell->a / sqrt(1. - ell->es * slat * slat);
	if (abs(P2->phi) <= .7854)
	    P2->h = p / clat - en;
	else
	    P2->h = P1->z / slat - en + ell->es * en;
	P2->lam = atan2(P1->y, P1->x);
    } else			/* too many iterations */
	err = -1;
    return err;
}

/*
 * $Log: proj_xyz2lph.c,v $
 * Revision 5.1  2009/04/30 20:47:47  gie
 * *** empty log message ***
 *
*/
