/*
** Copyright (c) 2008   Gerald I. Evenden
*/
static const char
 PROJ_ID[] = "$Id: lines.c,v 2.3 2009/04/03 18:25:37 gie Exp gie $";
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
/**
 * @file lines.c
 * @brief support functions for geodesic program
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <project.h>
#include "geodesic.h"
/**
 * @brief procedure to open and close ancillary output file
 * @param list pointer to linked list of parameters */
int out_open(PROJ_PARAM_ITEM * list) {
    const char *help =
	"\noutput <file_name> [mode=[a|w]]\n\n"
	"Open output file to recieve output from line and circle options.\n"
	"If \"mode=a\" is specified then the output is appended to\n"
	"existing file.  \"mode=w\" or no mode spec will overwrite existing\n"
	"file\n" "\nTo close an output file \"output close\"\n\n";

    if (proj_param(list, "bhelp", 0) == 1)
	fputs(help, stdout);
    else if (proj_param(list, "bclose", 0) == 1) {
	if (out_file != stdout)
	    fclose(out_file);
	out_file = stdout;
    } else {
	int err;
	FILE *tmp;
	const char *name, *mode;

	if (proj_param(list, "smode", &mode) != 1)
	    mode = "w";
	else if (proj_casecmp(mode, "w") && proj_casecmp(mode, "a")) {
	    fprintf(stderr, "<%s> is improper mode type: a or w\n", mode);
	    return -1;
	}
	if ((err = proj_param(list, "s", &name)) != 1) {
	    fprintf(stderr, "file name not given\n");
	    return -1;
	}
	if ((tmp = fopen(name, mode)) == NULL) {
	    fprintf(stderr, "Opening (%s) failed: \"%s\"\n",
		    name, strerror(errno));
	    return -1;
	}
	if (out_file != stdout)
	    fclose(out_file);
	out_file = tmp;
    }
    return 0;
}

/**
 * @brief open input file for "vector" or "inverse"
 * @param list pointer to linked argument list
 */
int in_open(PROJ_PARAM_ITEM * list) {
    char *help =
	"\ninput will open <input> file for processing by inverse or forward\n"
	"commands.\n" "To close the input file: input close.\n\n";
    if (proj_param(list, "bhelp", 0) == 1)
	fputs(help, stdout);
    else if (proj_param(list, "bclose", 0) == 1) {
	if (datain != stdin)
	    fclose(datain);
	datain = stdin;
    } else {
	const char *name;

	if ((proj_param(list, "s", &name)) != 1) {
	    fprintf(stderr, "file name not given\n");
	    return -1;
	}
	if ((datain = fopen(name, "r")) == NULL) {
	    fprintf(stderr, "Opening (%s) failed: \"%s\"\n",
		    name, strerror(errno));
	    return -1;
	}
    }
    return 0;
}

/** @brief intermediate geodesic line point generator
 * @param list inked argument list */
int aline(PROJ_PARAM_ITEM * list) {
    const char *help =
	"line [ n=<n> | del=<m> ]\n\n"
	"compute intermediate points along the geodetic arc between\n"
	"arc node p1 and p2. If \"n=<integer>\" then divide the interval\n"
	"into <integer> intermediate points else else if \"del=<m>\" then\n"
	"divide line into points approximately <m> meters apart.\n\n";
    int n, err;
    double f;

    if (proj_param(list, "bhelp", 0) == 1) {
	fputs(help, stdout);
	return 0;
    }
    if ((err = proj_param(list, "in", &n)) != 0) {
	if ((err < 0) || (n <= 0)) {
	    fprintf(stderr, "either loopup err(%d) or n (%d) <= 0\n", err, n);
	    return (-1);
	}
	f = arc.S / n;
    } else if ((err = proj_param(list, "ddel", &f)) != 0) {
	if ((err < 0) || (f <= 0.)) {
	    fprintf(stderr, "either loopup err(%d) or f (%g) <= 0\n", err, f);
	    return (-1);
	}
	n = (arc.S / f) + 0.5;
    } else {
	fprintf(stderr, "neither 'n' nor 'del' specified\n");
	return (-1);
    }
    f = arc.S / n;
    {				/* generation of geodesic line */
	PROJ_LINE larc = arc;	/* copy over to preserve original values */
	PROJ_PT_LPH lpt2;
	int i;

	larc.pt2 = &lpt2;
	/* print initial point */
	fprintf(out_file, "\n%.8f\t%.8f\n",
		arc.pt1->lam * (180. / M_PI), arc.pt1->phi * (180. / M_PI));
	larc.S = 0.;
	for (i = 1; i < n; ++i) {	/* print intermediate points */
	    larc.S += f;
	    proj_gforward(&larc);
	    fprintf(out_file, "%.8f\t%.8f\n",
		    lpt2.lam * (180. / M_PI), lpt2.phi * (180. / M_PI));
	}
	/* print final point */
	fprintf(out_file, "%.8f\t%.8f\n",
		arc.pt2->lam * (180. / M_PI), arc.pt2->phi * (180. / M_PI));
    }
    return 0;
}

#define TOL 1e-12
/** @brief intermediate geodesic circle point generator
 * @param list linked argument list */
int circle(PROJ_PARAM_ITEM * list) {
    const char *help =
	"circle azimuth delta_azi\n\n"
	"computes points along a circular arc centered at arc\n"
	"structure node p1 and extending from arc node p2 in a\n"
	"clockwise direction through an angle define by 'azimuth'\n"
	"and divided by intermediate points at approximately 'az'\n"
	"angular separation.\n"
	"'circle 360 10' describes a circle of points at 10 degree\n"
	"'delta_az' interval\n\n";
    double azi2, delaz, tarc;
    int nseg, err;

    if (proj_param(list, "bhelp", 0) == 1) {
	fputs(help, stdout);
	return 0;
    }
    if (((err = proj_param(list, "r", &azi2) <= 0)) ||
	((err = proj_param(list, "r", &delaz)) <= 0)) {
	fprintf(stderr, "either azimuth or delta angle absent or err:%d\n",
		err);
	return -1;
    }
    if (azi2 < 0.)
	azi2 += (2 * M_PI);
    if (fabs(azi2 - arc.az12) < TOL)	/* full circle */
	tarc = (2 * M_PI);
    else
	tarc = azi2 - arc.az12;
    nseg = tarc / delaz + 0.5;
    delaz = tarc / nseg;
    {				/* generate arc or circle */
	PROJ_LINE larc = arc;
	PROJ_PT_LPH lpt2;
	int i;

	larc.pt2 = &lpt2;
	/* print initial point */
	fprintf(out_file, "\n%.8f\t%.8f\n",
		arc.pt2->lam * (180. / M_PI), arc.pt2->phi * (180. / M_PI));
	for (i = 0; i < nseg; ++i) {
	    larc.az12 += delaz;
	    if (larc.az12 >= (2 * M_PI))
		larc.az12 -= (2 * M_PI);
	    proj_gforward(&larc);
	    fprintf(out_file, "%.8f\t%.8f\n",
		    lpt2.lam * (180. / M_PI), lpt2.phi * (180. / M_PI));
	}
    }
    return 0;
}

/*
 * $Log: lines.c,v $
 * Revision 2.3  2009/04/03 18:25:37  gie
 * *** empty log message ***
 *
 * Revision 2.2  2009/03/19 20:06:13  gie
 * updated for libproject and indent format
 *
 * Revision 2.1  2009/02/25 17:27:56  gie
 * *** empty log message ***
 *
 * Revision 1.1  2009/02/25 16:45:19  gie
 * Initial revision
 *
 * Revision 1.2  2009/01/08 01:47:42  gie
 * prep for release
 *
 * Revision 1.1  2008/12/07 18:48:11  gie
 * Initial revision
 *
 */
