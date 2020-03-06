/*
** Copyright (c) 2009   Gerald I. Evenden
*/
static const char
 PROG_ID[] = "$Id: proj_dms2rad.c,v 5.1 2009/04/30 20:47:47 gie Exp gie $";
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
/** @file proj_dms2rad.c
 * @brief conversion of Deg-Min-Sec character string to binary radians
 * @author Gerald I. Evenden
 */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
/**
 * @brief dms to radian input deformater
 * @param[in] is  is pointer to input string containing dms input field
 * @param[out] rs if not NULL, rs is set to point to next character after dms
 * @return return radian value of input string
 *
 * If an error is detected in input stream the return value is
 * HUGE_VAL.  If either no input dms data found or error detected
 * the return value of rs points to the same location as is
 *
 * This procedure will process both the DMS (12d15'33.123") type of field
 * associated with the proj system or the colon delimited type
 * associated with GRASS.  Of course "12.3456" will be interpreted
 * as degrees input.
 *
 * For DMS or colon types, all fields but the last may \a not have a
 * fractional component: 22:22.2:33.124 is in error.  Leading fields
 * may be empty: 5'44.55" is interpreted as 5 minutes, 44.55 seconds.
 * In the case of colon input, the leading colons must be supplied:
 * :5:44.55.  The minutes and seconds fields must \a always be less
 * than 60.
 *
 * -/+ may be used as prefix sign or one of the characters NSEW (upper or
 *  lower case) as post fix sign (S, W negative).
 *
 *  A simple floating point value with a suffix of R or r will
 *  be treated as radian input.
 */
double proj_dms2rad(const char *is, char **rs) {
    int pos, nl = 0, n;
    double val = HUGE_VAL, v[3];
    const double vmul[] = {
	0.017453292519943295769236907684886127134,
	0.00029088820866572159615394846141476878557,
	0.0000048481368110953599358991410235794797595
    };
    const char *sym = "NnEeSsWw";
    char *s = (char *)is;
    char *ns;

    while (*s && isspace(*s))
	++s;			/* clear beginning */
    if (*s == '+') {		/* prefix sign */
	pos = 1;
	++s;
    } else if (*s == '-') {
	pos = -1;
	++s;
    } else			/* no prefix sign */
	pos = 0;
    if (strpbrk(s, "rR") != NULL) {	/* radian input */
	if ((val = strtod(s, &ns)) == HUGE_VAL) {
	    if (rs != NULL)
		*rs = (char *)is;
	    return val;
	}
	if (s == ns) {		/* null input */
	    if (rs != NULL)
		*rs = ns;
	    return HUGE_VAL;
	}
	s = ns;
	nl = -1;
    } else if (strchr(s, ':') != NULL) {	/* colon mode */
	int OK;
	nl = -2;
	for (n = 0; n < 3; ++n) {
	    if ((v[nl = n] = strtod(s, &ns)) == HUGE_VAL) {
		if (rs != NULL)
		    *rs = (char *)is;
		return val;
	    }
	    if ((OK = (*(s = ns) != ':')))
		break;;
	    ++s;
	}
	if (!OK || nl == -2) {
	    if (rs != NULL)
		*rs = (char *)is;
	    return HUGE_VAL;
	}
    } else {			/* assumed to be dms mode */
	double vr;
	int quit = 0;

	nl = -3;
	for (n = 0; n < 3; ++n)
	    v[n] = 0.;
	for (n = 0; n < 3; n = nl + 1) {
	    if ((vr = strtod(s, &ns)) == HUGE_VAL) {
		if (rs != NULL)
		    *rs = ns;
		return HUGE_VAL;
	    }
	    if (s == ns)	/* no number */
		break;
	    s = ns;
	    switch (*s) {
	    case 'D':
	    case 'd':
		v[nl = 0] = vr;
		break;
	    case '\'':
		v[nl = 1] = vr;
		break;
	    case '"':
		v[nl = 2] = vr;
		break;
	    default:
		quit = 1;
		v[n] = vr;
	    }
	    if (quit) {
		nl = n;
		break;
	    }
	    if (nl < n) {
		if (rs != NULL)
		    *rs = (char *)is;
		return HUGE_VAL;
	    }
	    ++s;
	}
    }
    if (nl >= 0) {		/* convert to radian number */
	double iv, f;
	for (val = 0., n = 0; n <= nl; ++n) {
	    if (n > 0) {	/* check over 59 or frac and not last */
		f = modf(v[n], &iv);
		if ((iv > 59.0) || ((n < nl) && (f != 0.))) {
		    if (rs != NULL)
			*rs = (char *)is;
		    return HUGE_VAL;
		}
		if (n != nl)
		    v[n] = iv;
	    }
	    val += v[n] * vmul[n];
	}
    }
    if (nl == -1) {		/* next char should be radian tag */
	if ((*s == 'r') || (*s == 'R'))
	    ++s;
	else {			/* must be radian suffix here */
	    if (rs != NULL)
		*rs = (char *)is;
	    return HUGE_VAL;
	}
    }
    if (pos) {			/* prefix sign */
	if (pos < 0)
	    val = -val;
    } else if (*s) {		/* postfix sign */
	if ((ns = strchr(sym, *s)) != NULL) {
	    if ((ns - sym) >= 4)
		val = -val;
	    ++s;
	}
    }
    if (rs != NULL)
	*rs = s;
    return val;
}
