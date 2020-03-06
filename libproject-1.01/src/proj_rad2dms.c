/*
** Copyright (c) 2006, 2009   Gerald I. Evenden
*/
static const char
 PROJ_ID[] = "$Id: proj_rad2dms.c,v 5.2 2009/05/06 18:46:51 gie Exp gie $";
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
/** @file proj_rad2dms.c
 * @brief conversion of binary radians to Deg-Min-Sec character string
 * @author Gerald I. Evenden
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/** @brief locates field definitions within format
 * @param[in] str pointer to format string
 * @param[out] type pointer to int format type code
 * @param[out] prec pointer to int precision of format type
 *
 * find token of the form "%[<ignore>][.prec][type=d|f]"
 */
static const char *		/* find %[n[.m]]{d|f} format fields */
fmt_token(const char *str, int *type, int *prec) {
    *prec = 0;
    *type = 'X';
    while (*str && *str != '%')
	++str;			/* look for % field */
    if (*str) {			/* if not at end of line */
	++str;
	str += strspn(str, " +-0123456789");	/* skip over width section */
	if (*str) {		/* not at end of string */
	    if (*str == '.') {	/* has n.m field */
		++str;
		*prec = strtol(str, (char **)&str, 10);
	    }
	    *type = *str++;
	}
    }
    return str;
}

/**
 * @brief radian to DMS output format
 * @param[out] fld pointer to character array to contain formated string
 * @param[in] wf length of formatted string
 * @param[in] fmt pointer to C style format string \n
 * Example for DMS would be "%dd%02d'%07.4f\"%c" for fixed width
 * output ("%dd%d'%.4f\"%c" for variable width). For GRASS colon
 * format: "%d:%d:%.4f%c" is typical.
 * @param[in] deg output data value in radians to be formated
 * @param[in] sign pointer to two char string with post fix sign\n
 * Typical geographic usage would be "NS" for latitude and "EW"
 * for longitude.
 * The first character of sign is for a positive value of
 * data and the second character is for negative data.\n
 * If format does not have a "c" field then a prefix "-" sign
 * is used if the the data value is negative.
 * @return returns the number of formatted characters
 *
 * If an error is detected in the formatting process then a string
 * containing "**" is returned.
 */
int
proj_rad2dms(char *fld, size_t wf, const char *fmt, double deg,
	     const char *sign) {
    const char *p = fmt;
    int type, prec, ret, neg = deg < 0.;

    /* input "deg" is really radians so ... */
    deg *= 180./M_PI;
    p = fmt_token(p, &type, &prec);	// look at first format conversion
    if (type == 'f' || type == 'g' || type == 'e') {	// simple fraction type
	p = fmt_token(p, &type, &prec);	// check for suffix sign
	ret = type == 'c' ? snprintf(fld, wf, fmt, fabs(deg), sign[neg]) :
	    snprintf(fld, wf, fmt, deg);
    } else if (type == 'd') {	// minutes (maybe seconds
	double min, sec, nf;

	p = fmt_token(p, &type, &prec);
	deg = fabs(deg);
	if (type == 'f') {	/* DM format, fractional minutes */
	    nf = pow(10., prec);
	    deg = floor(deg * 60. * nf + 0.5);
	    min = fmod((deg /= nf), 60.);
	    deg = floor(deg / 60.);
	    p = fmt_token(p, &type, &prec);
	    ret = type == 'c' ?
		snprintf(fld, wf, fmt, (int)deg, min, sign[neg]) :
		snprintf(fld, wf, fmt, (int)(neg ? -deg : deg), min);
	} else if (type == 'd') {	/* full DMS output */
	    p = fmt_token(p, &type, &prec);
	    nf = pow(10., prec);
	    deg = floor(deg * 3600. * nf + 0.5);
	    sec = fmod(deg / nf, 60.);
	    deg = floor(deg / (nf * 60.));
	    min = fmod(deg, 60.);
	    deg /= 60.;
	    p = fmt_token(p, &type, &prec);
	    ret = type == 'c' ?
		snprintf(fld, wf, fmt, (int)deg, (int)min, sec, sign[neg]) :
		snprintf(fld, wf, fmt, (int)(neg ? -deg : deg), (int)min, sec);
	} else {
	    ret = snprintf(fld, wf, "**");
	}
    } else {
	ret = snprintf(fld, wf, "**");
    }
    return (ret);
}

/*
 * $Log: proj_rad2dms.c,v $
 * Revision 5.2  2009/05/06 18:46:51  gie
 * updates for Doxygen
 *
 * Revision 5.1  2009/04/30 20:47:47  gie
 * *** empty log message ***
 *
*/
