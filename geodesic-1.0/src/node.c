/*
** Copyright (c) 2008   Gerald I. Evenden
*/
static const char
 PROJ_ID[] = "$Id: node.c,v 2.3 2009/04/03 18:25:37 gie Exp gie $";
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
/** @file node.c
 * @brief support functions for geodesic program
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <project.h>
#include "geodesic.h"
/** @brief compute forward geodesic
 * @param list pointer to parameter list
 * @return 0 if no errors detected, else ...
 * 
 * Procedure processes input "lon lat length azimuth" as vector from
 * p1 to determine the location of point p2.
 * */
int vector(PROJ_PARAM_ITEM * list) {
    char *help =
	"vector length azimuth\n\n"
	"determine arc point p2 at the end of a geodesic line of length\n"
	"'length' from p1 and at the 'azimuth'.  Also known as a 'forward'\n"
	"geodesic problem.  The back azimuth from p2 is also determined\n\n"
	"If 'redo' is argument and p1 and azimuth and distance present\n"
	"(usually as result of previous p2 operation) 'vector' will\n"
	"recompute forward.  Principle use is as a check operation\n\n";
    int err;

    if (proj_param(list, "bhelp", 0) == 1) {
	fputs(help, stdout);
	return 0;
    }
    arcstate &= ~(VECTOR + P2GOOD);
    if (!(arcstate & P1GOOD)) {
	fprintf(stderr, "vector requires that p1 node previously made\n");
	arcstate &= ~COMPD;
	return -1;
    }
    if (proj_param(list, "bredo", 0) == 1) {
	// compute forward from az12, S
	if (!(arcstate & COMPD) || !(arcstate & P1GOOD)) {
	    fprintf(stderr, "p1 not set or arc not computed\n");
	    arcstate &= ~COMPD;
	    return -1;
	}
    } else if (((err = proj_param(list, "d", &arc.S)) <= 0) ||
	       ((err = proj_param(list, "r", &arc.az12)) <= 0)) {
	fprintf(stderr, "distance or azimuth missing or in error\n");
	return -1;
    }
    arcstate |= VECTOR;
    if (arcstate & EARTHSET) {
	proj_gforward(&arc);
	arcstate |= COMPD + P2GOOD;
	last_op = OP_FORWARD;
	show_arc(&arc, arcstate, display, stdout);
    }
    return 0;
}

/** @brief compute goedesic distances and azimuths for point pairs
 * @param list pointer to parameter list
 * @return 0 if no errors detected, else ...
 * */
#define MAXLINE 1000
int do_batch(PROJ_PARAM_ITEM * list) {
    char *help =
	"\nbatch [inverse | forward] [lat_lon]\n\n"
	"where either the 'inverse' or 'forward' mode must be selected.\n\n"
	"Input for the inverse mode consists of lines of two lon-lat pairs\n\n"
	"Input for the forward mode consists of a lon-lat pair, distance and\n"
	"azimuth\n\n"
	"lon-lat order may be changed to lat-lon with the \"lat_lon\" option.\n"
	"Input may be from a \"datin <file>\".\n\n"
	"Processing in this mode is terminated at an <end_of_file> condition\n"
	"or encountering a \"#eod\" at the beginning of the input data line.\n\n"
	"Note: order of output geographic coordinates determined by \"display\"\n"
	"settings\n\n";
    char line[MAXLINE + 1], *s, *r;
    int lat_lon, i, forw, cnt;
    double val[4];
    PROJ_PT_LPH p1, p2;
    PROJ_LINE loc_arc = { &p1, 0., &p2, 0., 0., 0 };

    if (proj_param(list, "bhelp", 0) == 1) {
	fputs(help, stdout);
	return 0;
    }
    if (((forw = proj_param(list, "bforward", 0)) +	// a bit devious
	 proj_param(list, "binverse", 0)) != 1) {
	fprintf(stderr, "'forward' or 'inverse' must be specified\n");
	return -1;
    }
    cnt = forw ? 2 : 4;
    if (!(arcstate & EARTHSET)) {
	fprintf(stderr, "ellipsoid not set\n");
	exit(-1);
    } else
	loc_arc.E = &earth;
    lat_lon = proj_param(list, "blat_lon", 0);
    while ((s = fgets(line, MAXLINE, datain)) != NULL) {
	if (proj_ncasecmp(line, "#eod", 4) == 0)
	    break;
	if (*line == '#') {	// comment line
	    fputs(line, out_file);
	    continue;
	}
	for (i = 0; i < cnt; ++i) {
	    val[i] = proj_dms2rad(s, &r);
	    if (s == r) {
		fprintf(stderr, "problems reading data on line:\n%s\n", line);
		break;
	    }
	    s = r;
	}
	if (i < cnt)		// error reading line;
	    continue;
	if (lat_lon == 1) {	// flip lon/lat order
	    p1.lam = val[1];
	    p1.phi = val[0];
	    if (!forw) {
		p2.lam = val[3];
		p2.phi = val[2];
	    }
	} else {
	    p1.lam = val[0];
	    p1.phi = val[1];
	    if (!forw) {
		p2.lam = val[2];
		p2.phi = val[3];
	    }
	}
	if (forw) {
	    loc_arc.S = strtod(s, &r);
	    if (s != r) {
		s = r;
		loc_arc.az12 = proj_dms2rad(s, &r);
	    }
	    if (s == r) {
		fprintf(stderr, "problems reading length or azimuth\n");
		continue;
	    }
	    proj_gforward(&loc_arc);
	} else
	    proj_ginverse(&loc_arc);
	show_arc(&loc_arc, EARTHSET + COMPD + P1GOOD + P2GOOD,
		 display, out_file);
    }
    return 0;
}

/** @brief set node (point) value
 * @param list pointer to parameter list
 * @return 0 if no errors detected, else ...
 * */
int node(PROJ_PARAM_ITEM * list) {
    char *help =
	"p[1|2] longitude latitude\n\n"
	"establish the p1 or p2 coordinates of the arc end points\n"
	"when both p1 and p2 are given the inverse geodesic distance\n"
	"and forward and back azimuths will be computed and displayed.\n"
	"Optionally a third parameter, height above the ellipsoid can\n"
	"be included.\n\n"
	"If 'redo' option for either option and last operation a forward\n"
	"computation then recompute as a inverse computation\n\n"
	"If 'reverse' option and both points present then reverse points\n"
	"and recompute as inverse\n\n";
    double v;
    PROJ_PT_LPH pt;
    int made_1, i, err;

    if (proj_param(list, "bhelp", 0) == 1) {
	fputs(help, stdout);
	return 0;
    }
    if (proj_param(list, "bredo", 0) == 1) {
	// only "redo" if last operation a 'forward'
	if (last_op != OP_FORWARD)
	    return 0;		// ignore
    } else if (proj_param(list, "breverse", 0) == 1) {	// reverse points
	if ((arcstate & P1GOOD) && (arcstate & P2GOOD)) {
	    pt = *arc.pt1;
	    *arc.pt1 = *arc.pt2;
	    *arc.pt2 = pt;
	} else if (arcstate & P1GOOD) {
	    *arc.pt2 = *arc.pt1;
	    arcstate |= P2GOOD;
	    arcstate &= ~P1GOOD;
	    return 0;
	} else if (arcstate & P2GOOD) {
	    *arc.pt1 = *arc.pt2;
	    arcstate |= P1GOOD;
	    arcstate &= ~P2GOOD;
	    return 0;
	} else			// nothing there
	    return 0;
    } else {			// set a point
	made_1 = (list->param[1] == '1');
	arcstate &= ~(COMPD + (made_1 ? P1GOOD : P2GOOD));
	for (i = 2; i > 0; --i) {
	    if ((err = proj_param(list, "r", &v)) != 1) {
		fprintf(stderr, "coordinate missing or in error (%d)\n", err);
		return -1;
	    }
	    if (i == 2)
		pt.lam = v;
	    else
		pt.phi = v;
	}
	pt.h = 0.;
	(void)proj_param(list, "d", &pt.h);
	if (made_1) {
	    *arc.pt1 = pt;
	    arcstate |= P1GOOD;
	} else {
	    *arc.pt2 = pt;
	    arcstate |= P2GOOD;
	}
    }
    if ((arcstate & P1GOOD) && (arcstate & P2GOOD) && (arcstate & EARTHSET)) {
	proj_ginverse(&arc);
	arcstate |= COMPD;
	last_op = OP_INVERSE;
	show_arc(&arc, arcstate, display, stdout);
    } else {
	arcstate &= ~COMPD;
	last_op = OP_NONE;
    }
    return 0;
}

/** @brief print point structure data
 * @param out file pointer
 * @param no point number
 * @param val pointer to point structure
 * @param pmode display mode
 * */
static void print_point(FILE * out, int no, PROJ_PT_LPH * val, int pmode) {
    char slat[MAX_FMT], slon[MAX_FMT];

    if (PRNT_DMS & pmode) {
	proj_rad2dms(slon, MAX_FMT, out_fmt, val->lam, "EW");
	proj_rad2dms(slat, MAX_FMT, out_fmt, val->phi, "NS");
    } else {
	sprintf(slon, "%.14g", (180. / M_PI) * val->lam);
	sprintf(slat, "%.14g", (180. / M_PI) * val->phi);
    }
    if (!(PRNT_DUMP & pmode)) {
	fprintf(out, "   Point %d\n", no);
	if (PRNT_LAT_LON & pmode)
	    fprintf(out, "      Lat: %s\n      Lon: %s\n", slat, slon);
	else
	    fprintf(out, "      Lon: %s\n      Lat: %s\n", slon, slat);
	fprintf(out, "      Hgt:   %.6f\n", val->h);
    } else if (PRNT_LAT_LON & pmode) {
	fputs(slat, out);
	fputc('\t', out);
	fputs(slon, out);
    } else {
	fputs(slon, out);
	fputc('\t', out);
	fputs(slat, out);
    }
}

/** @brief print arc structure data
 * @param A arc structure pointer
 * @param state status flags for A elements
 * @param pmode display flags
 * @param out output file pointer
 */
void show_arc(PROJ_LINE * A, int state, int pmode, FILE * out) {
    const char *azi_fmt = { "%dd%d'%.7f\"" };
    char line[MAX_FMT + 1];

    if (!state)
	printf("ARC empty\n");
    else if (pmode & PRNT_DUMP) {
	if (state & P1GOOD)
	    print_point(out, 1, A->pt1, pmode);
	else
	    fputc('*', out);
	fputc('\t', out);
	if (state & P2GOOD)
	    print_point(out, 2, A->pt2, pmode);
	else
	    fputc('*', out);
	if (state & COMPD) {
	    fputc('\t', out);
	    if (pmode & PRNT_DMS) {
		proj_rad2dms(line, MAX_FMT, azi_fmt, A->az12, NULL);
		fputc('\t', out);
		fputs(line, out);
		proj_rad2dms(line, MAX_FMT, azi_fmt, A->az21, NULL);
		fputc('\t', out);
		fputs(line, out);
	    } else
		fprintf(out, "%.14g\t%.14g",
			(180. / M_PI) * A->az12, (180. / M_PI) * A->az21);
	    fputc('\t', out);
	    fprintf(out, dist_fmt, A->S);
	    fputc('\n', out);
	} else
	    fputs("\t*\t*\t*\n", out);
    } else {
	if (state & P1GOOD)
	    print_point(out, 1, A->pt1, pmode);
	if (state & P2GOOD)
	    print_point(out, 2, A->pt2, pmode);
	if (state & COMPD) {
	    if (pmode & PRNT_DMS)
		proj_rad2dms(line, MAX_FMT, azi_fmt, A->az12, NULL);
	    else
		sprintf(line, "%.14g", A->az12 * (180. / M_PI));
	    printf("   Azimuth p1->p2: %s\n", line);
	    printf("   Distance: ");
	    printf(dist_fmt, A->S);
	    if (pmode & PRNT_DMS)
		proj_rad2dms(line, MAX_FMT, azi_fmt, A->az21, NULL);
	    else
		sprintf(line, "%.14g", A->az21 * (180. / M_PI));
	    printf("\n   Azimuth p2->p1: %s\n", line);
	}
    }
}

/*
 * $Log: node.c,v $
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
 * Revision 1.2  2009/01/08 01:47:49  gie
 * prep for release
 *
 * Revision 1.1  2008/12/07 18:48:11  gie
 * Initial revision
 *
 */
