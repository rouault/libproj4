/*
** libproj -- library of cartographic projections
**
** Copyright (c) 2003, 2006   Gerald I. Evenden
*/
static const char
LIBPROJ_ID[] = "$Id: proj_fwd.c,v 5.5 2009/05/06 18:46:51 gie Exp gie $";
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
/** @file proj_fwd.c
 * @brief forward projection
 * @author Gerald I. Evenden
 */
#include "config.h"
#include <project.h>
#include <errno.h>
#if HAVE_FLOAT_H
#include <float.h>
#else
#define DBL_EPSILON 1e-14
#endif
// give things a little sloppyness
# define EPS (DBL_EPSILON * 3)
/**
 * @brief compute the forward projection
 * @param[in] lp geographic coordinates
 * @param[in] P  projection control
 * @return Cartesian coordinates
 *
 * If the projection fails the returned Cartesian coordinates are
 * set to HUGE_VAL and proj_errno will be non-zero.
 */
	PROJ_XY /* forward projection entry */
proj_fwd(PROJ_LP lp, PROJ *P) {
	PROJ_XY xy;
	double t;

	/* check for forward and latitude or longitude overange */
	if ((t = fabs(lp.phi) - M_PI_2) > EPS) {
//	if ((t = fabs(fabs(lp.phi)-M_PI_2)) < EPS) {
		xy.x = xy.y = HUGE_VAL;
		proj_errno = -14;
	} else { /* proceed with projection */
		errno = proj_errno = 0;
		if (fabs(t) <= EPS) // if close to Pi, force all the way
			lp.phi = copysign(M_PI_2, lp.phi);
		/* dropping geocentric option
		else if (P->geoc)
			lp.phi = atan(P->rone_es * tan(lp.phi));
		*/
		lp.lam -= P->netlam0;	// compute net longitude shift
		if (!P->over)
			lp.lam = proj_adjlon(lp.lam); /* adjust del longitude */
		xy = (*P->fwd)(lp, P); /* project */
		if (proj_errno || (proj_errno = errno) ||
			(xy.x == HUGE_VAL) || (xy.y ==HUGE_VAL) )
			xy.x = xy.y = HUGE_VAL;
		/* adjust for major axis and easting/northings */
		else {
			xy.x = P->fr_meter * (P->a * xy.x + P->x0);
			xy.y = P->fr_meter * (P->a * xy.y + P->y0);
		}
	}
	return xy;
}
/* Revision log:
** $Log: proj_fwd.c,v $
** Revision 5.5  2009/05/06 18:46:51  gie
** updates for Doxygen
**
** Revision 5.4  2009/05/05 23:40:30  gie
** restored config.h header
**
** Revision 5.3  2009/05/03 01:00:29  gie
** removed config.h reference
**
** Revision 5.2  2009/05/01 17:54:48  gie
** added conditional compile
**
** Revision 5.1  2009/04/30 20:47:47  gie
** *** empty log message ***
**
*/
