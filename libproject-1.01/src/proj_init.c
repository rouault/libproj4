/*
**  library of cartographic projections
**
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
LIBPROJ_ID[] = "$Id: proj_init.c,v 5.5 2009/05/23 14:46:38 gie Exp gie $";
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
/** @file proj_init.c
 * @brief projection initialization and closure
 * @author Gerald I. Evenden
*/
#define PROJ_LIB__
#include <project.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
/** @brief projection initialization
 * @param[in] argc number of argument strings in argv
 * @param[in] argv array of argument strings
 * @return projection control sturcture pointer, NULL if failure
 */
	PROJ *
proj_init(int argc, char *argv[]) {
	const char *s, *name;
	PROJ *(*proj)(PROJ *);
	PROJ_PARAM_ITEM *curr = 0, *start = 0;
	int i, first = 1;
	PROJ *PIN = 0;

	errno = proj_errno = 0;
	/* first: put arguments into internal linked list */
	if (argc <= 0) { proj_errno = -1; goto bum_call; }
	for (i = 0; i < argc; ++i) {
		const char *ss = argv[i];
		do { // this handles multiple tokens in string
			while (isspace(*ss)) // skip over white space
				++ss;
			if (*ss) { // if something there add it to list
				if (first) {
					start = curr = proj_mkparam(ss, &ss);
					first = 0;
				} else
					curr = curr->next = proj_mkparam(ss, &ss);
			}
		} while (*ss);
	}
	if (proj_errno) goto bum_call;  // only errno would be from malloc in mkparam
	/* find projection selection */
	if (!proj_param(start, "sproj", &name))
		{ proj_errno = -4; goto bum_call; }
	for (i = 0; (s = proj_list[i].id) && proj_casecmp(name, s) ; ++i) ;
	if (!s) { proj_errno = -5; goto bum_call; }
	proj = proj_list[i].proj;
	/* allocate projection structure */
	if (!(PIN = (*proj)(0))) goto bum_call;
	PIN->params = start;
	/* set basic defaults */
	PIN->a = 1.; // unit sphere
	PIN->e = PIN->es = PIN->primer = PIN->lam0 = PIN->phi0 =
		PIN->x0 = PIN->y0 = 0.;
	PIN->k0 = PIN->to_meter = PIN->fr_meter = 1.;
	/* set ellipsoid/sphere parameters */
	if (proj_ell_set(start, &PIN->a, &PIN->es)) goto bum_call;
	PIN->e = sqrt(PIN->es);
	PIN->ra = 1. / PIN->a;
	PIN->one_es = 1. - PIN->es;
	if (PIN->one_es == 0.) { proj_errno = -6; goto bum_call; }
	PIN->rone_es = 1./PIN->one_es;
	/* over-ranging flag */
	PIN->over = proj_param(start, "bover", 0);
	/* alternate prime meridian */
	if (proj_param(start, "spms", &name) == 1 ) {
		if (isalpha(*name)) {
			const struct PROJ_PRIMES_LIST *fd = proj_pm_list;
			/* then must be identifier and in pm list */
			while ((fd->name != NULL) && (proj_casecmp(fd->name, name) != 0))
				++fd;
			if (fd->name == NULL) {
				proj_errno = -50;
				goto bum_call;
			}
			name = fd->del_lon;
		}
		(PIN->primer) = proj_dms2rad(name, NULL);
	}
	/* central meridian */
	(void) proj_param(start, "rlon_0", &(PIN->lam0));
	/* central latitude */
	(void) proj_param(start, "rlat_0", &(PIN->phi0));
	/* false easting and northing */
	(void) proj_param(start, "dx_0", &(PIN->x0));
	(void) proj_param(start, "dy_0", &(PIN->y0));
	/* general scaling factor */
	if (proj_param(start, "tk_0", 0))
		(void) proj_param(start, "dk_0", &(PIN->k0));
	else if (proj_param(start, "tk", 0))
		(void) proj_param(start, "dk", &(PIN->k0));
	else
		PIN->k0 = 1.;
	if (PIN->k0 <= 0.) {
		proj_errno = -31;
		goto bum_call;
	}
	/* set units */
	s = 0;
	if (proj_param(start, "sunits", &name)) { 
		for (i = 0; (s = proj_units[i].id) && proj_casecmp(name, s) ; ++i) ;
		if (!s) { proj_errno = -7; goto bum_call; }
		s = proj_units[i].to_meter;
	}
	if (s || proj_param(start, "sto_meter", &s)) {
		PIN->to_meter = strtod(s, (char **)&s);
		if (*s == '/') /* ratio number */
			PIN->to_meter /= strtod(++s, 0);
		PIN->fr_meter = 1. / PIN->to_meter;
	} else
		PIN->to_meter = PIN->fr_meter = 1.;
	/* projection specific initialization */
	if (!(PIN = (*proj)(PIN)) || errno || proj_errno) {
bum_call: /* cleanup error return */
		if (!proj_errno)
			proj_errno = errno;
		if (PIN)
			proj_free(PIN);
		else
			for ( ; start; start = curr) {
				curr = start->next;
				free(start);
			}
		PIN = 0;
	} else
		/* compute net longitude offset */
		PIN->netlam0 = PIN->lam0 + PIN->primer;

	return PIN;
}
/** @brief projection closure --- frees control space
 * @param[in] P projection control sturcture
 */
	void
proj_free(PROJ *P) {
	if (P) {
		PROJ_PARAM_ITEM *t = P->params, *n;

		/* free parameter list elements */
		for (t = P->params; t; t = n) {
			n = t->next;
			free(t);
		}
		/* free projection parameters */
		P->pfree(P);
	}
}
/** @brief single string projection initialization
 * @param[in] str character string of initialization parameters
 * @return projection control structure
 */
	PROJ *
proj_initstr(const char *str) {
	const char * plist[1];
	plist[0] = str;
	return(proj_init(1, plist));
}
/* Revision log
** $Log: proj_init.c,v $
** Revision 5.5  2009/05/23 14:46:38  gie
** initialization of *all* PIN primary values.
**
** Revision 5.4  2009/05/20 15:06:05  gie
** corrected for caseless test for projection name
** and units.
**
** Revision 5.3  2009/05/17 15:08:58  gie
** added pms option
**
** Revision 5.2  2009/05/06 18:46:51  gie
** updates for Doxygen
**
** Revision 5.1  2009/04/30 20:47:47  gie
** *** empty log message ***
**
*/
