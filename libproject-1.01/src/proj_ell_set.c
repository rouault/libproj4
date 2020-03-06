/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
 PROG_ID[] = "$Id: proj_ell_set.c,v 5.2 2009/05/23 14:44:31 gie Exp gie $";
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
/** @file proj_ell_set.c
 * @brief decodes parameters and established earth's ellipsoid constants
 */
#include <math.h>
#include <project.h>
/** @brief set ellipsoidal parameters
 * @param[in] pl pointer to linked parameter list
 * @param[out] a  pointer to earth's semi-major axis or radius
 * @param[out] es pointer to earth's eccentricity
 * @return NULL if no errors detected else pointer to error description
 *
 * This procedure provides a variety of options to return ellipsoid
 * parameters.
 *
 * The parameter \c R=nnnn takes precedence and will set the
 * value of \a a to the radius value of 'nnn'.  All other options
 * are ignored.
 *
 * In the case of an ellipsoid, the semi-major axis must always
 * be specified with \c a=nnn and the ellipsoid defined by any
 * one of the following options:
 *
 * \li \c b=<nnn> the semi-minor axis
 * \li \c rf=<nnn> the reciprocal flattening (most common)
 * \li \c f=<nnn> flattening
 * \li \c es=<nnn> eccentricity squared
 *
 * Ellipsoid values can also be secured from an list of standard
 * ellipsoids by using the option \c ellps=keyword.  If the keyword
 * is found in the internal list then ellipsoid constants are used
 * for that entry.
 * 
 * In some cases it is desired to use a spherical earth with a
 * radius derived from the ellipsoid parameters by one of the following
 * additional options:
 *
 * \li \c R_V spherical earth with volume equivalent to ellipsoid
 * \li \c R_A spherical earth with surface area equivalent to ellipsoid
 * \li \c R_a radius from mean of a and b
 * \li \c T_g radius from geometric mean of a and b
 * \li \c R_h radius from harmonic mean of a and b
 * \li \c R_lat_g=<nnn> radius of geometric mean of lat <nnn>
 */
int proj_ell_set(PROJ_PARAM_ITEM * pl, double *a, double *es) {
    int i = 0, err, ea = 0, eg = 0;
    double b = 0., e, tmp;
    const char *name;

    /* check for varying forms of ellipsoid input */
    *a = *es = 0.;
    /* R takes precedence */
    if (((err = proj_param(pl, "dR", a)) != 0) ||
	((ea = proj_param(pl, "da", a)))) {
	if (err < 0)
	    return err;
	if (ea < 0)
	    return ea;
	if (*a <= 0.)
	    return -3;
	if (err == 1)
	    return 0;		// if R given, nothing more to do.
    } else {			// neither R nor a given, hope for "ellps="
	/* check if ellps present and temporarily append its values to pl */
	if ((err = proj_param(pl, "sellps", &name)) == 1) {
	    PROJ_PARAM_ITEM *curr = pl;
	    char *s;

	    while (curr->next != NULL)
		curr = curr->next;
	    for (i = 0; (s = proj_ellps[i].id) && proj_casecmp(name, s); ++i) ;
	    if (!s)
		return -3;
	    curr = curr->next = proj_mkparam(proj_ellps[i].major, 0);
	    curr = curr->next = proj_mkparam(proj_ellps[i].ell, 0);
	} else if (err == 0) { // ellps not present
	    *a = 1; // default to unit radius
	    *es = 0.;
	    return 0;
	} else
	    return -3;	// no hope
	/* set major axis, not done yet */
	if (((err = proj_param(pl, "da", a)) != 1) || (*a == 0.))
	    return (err < 0 ? err : -3);	// give up
    }
    /* various eccentricity input option */
    if ((err = proj_param(pl, "des", es)) != 0) {	// eccentricity squared
	if (err < 0)
	    return err;
	else if (*es < 0.)
	    return -3;
	else if (*es == 0.)
	    return 0;
    } else if ((err = proj_param(pl, "de", &e)) != 0) {	// eccentricity
	if (err < 0)
	    return err;
	else if (*es == 0.)
	    return 0;
	*es = e * e;
    } else if ((err = proj_param(pl, "drf", es)) != 0) { // recip flattening
	if (err < 0)
	    return err;
	if (*es <= 0.)
	    return -3;
	*es = 1. / *es;
	*es = *es * (2. - *es);
    } else if ((err = proj_param(pl, "df", es)) != 0) {	// flattening
	if (err < 0)
	    return err;
	if (*es < 0.)
	    return -3;
	if (*es == 0.)
	    return 0;
	*es = *es * (2. - *es);
    } else if ((err = proj_param(pl, "db", &b)) != 0) {	// minor axis
	if (err < 0)
	    return err;
	if (b <= 0.)
	    return -3;
	*es = 1. - (b * b) / (*a * *a);
    }
    if (!b)
	b = *a * sqrt(1. - *es);
    /* following options turn ellipsoid into equivalent sphere */
    if (proj_param(pl, "bR_A", 0) == 1) {	// sphere--area of ellipsoid
	*a *=
	    1. - *es * ((1. / 6.) + *es * ((17. / 360.) + *es * (67. / 3024.)));
	*es = 0.;
    } else if (proj_param(pl, "bR_V", 0) == 1) {	// vol. of ellipsoid
	*a *= 1. - *es * ((1. / 6.) + *es * ((5. / 72.) + *es * (55. / 1296.)));
	*es = 0.;
    } else if (proj_param(pl, "bR_a", 0) == 1) {	// arithmetic mean
	*a = .5 * (*a + b);
	*es = 0.;
    } else if (proj_param(pl, "bR_g", 0) == 1) {	// geometric mean
	*a = sqrt(*a * b);
	*es = 0.;
    } else if (proj_param(pl, "bR_h", 0) == 1) {	// harmonic mean
	*a = 2. * *a * b / (*a + b);
	*es = 0.;
    } else if (((ea = proj_param(pl, "rR_lat_a", &tmp)) != 0) ||
	       ((eg = proj_param(pl, "rR_lat_g", &tmp)) != 0)) {
	/* ea=1: arith mean | eg=1 geometric mean at latitude */
	if (ea < 0)
	    return ea;
	if (eg < 0)
	    return eg;
	if (fabs(tmp) > M_PI_2)
	    return -3;
	tmp = 1. - *es * tmp * tmp;
	*a *= i ? .5 * (1. - *es + tmp) / (tmp * sqrt(tmp)) :
	    sqrt(1. - *es) / tmp;
	*es = 0.;
    }
    return 0;
}

/*
** $Log: proj_ell_set.c,v $
** Revision 5.2  2009/05/23 14:44:31  gie
** if no initialization input then unit sphere is default
**
** Revision 5.1  2009/04/30 20:47:47  gie
** *** empty log message ***
**
*/
