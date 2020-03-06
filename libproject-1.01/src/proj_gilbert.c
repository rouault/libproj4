/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_gilbert.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
#define DEFAULT_LAT (5.*M_PI/180)
#define PROJ_LIB__
#define PROJ_PARMS__ \
	double cp1, sp1;
# include	<project.h>
PROJ_HEAD(gilbert, "Gilbert Two World Perspective")
	"\ntype: Cylindrical\nmode: sphere"
	"lat_1=(5) latitude of persp. azimuth for oblique case\n"
	"No inverse\n";
	static double
phiprime(double phi) { return (proj_asin(tan(0.5 * phi))); }
FORWARD(s_forward); /* spheroid */
	double sp, cp, cl;

	lp.phi = phiprime(lp.phi);
	sp = sin(lp.phi);
	cp = cos(lp.phi);
	cl = cos(lp.lam *= 0.5);
	if ((P->sp1*sp + P->cp1*cp*cl) >= 0.) {
		xy.x = cp * sin(lp.lam);
		xy.y = P->cp1*sp - P->sp1 * cp * cl;
	} else
		F_ERROR;
	return (xy);
}
FREEUP; if (P) free(P); }
ENTRY0(gilbert)
	double lat1;

	if (proj_param(P->params, "rlat_1", &lat1) != 1)
		lat1 = DEFAULT_LAT;
	P->cp1 = phiprime(lat1);
	P->sp1 = sin(P->cp1);
	P->cp1 = cos(P->cp1);
	P->es = 0.;
	P->fwd = s_forward;
ENDENTRY(P)
/*
** $Log: proj_gilbert.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
