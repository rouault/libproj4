/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_semiconf.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
#define PROJ_LIB__
# include	<project.h>
#define RPI 0.1591549430918953357688837633
#define ALT 0.99989
#define TEST 1.5564
PROJ_HEAD(near_con, "Semiconformal Pseudocylindrical")
	"\ntype: Pseudocylindrical\nmode: sphere\n"
	"No inverse\n";
FORWARD(s_forward); /* spheroid */
	double p, theta;

	p = lp.phi < -TEST ? -ALT : lp.phi > TEST ? ALT : sin(lp.phi);
	theta = RPI * log((1. + p)/(1. - p));
	xy.x = lp.lam * cos(theta);
	xy.y = M_PI * sin(theta);
	return (xy);
}
FREEUP; if (P) free(P); }
ENTRY0(near_con) P->es = 0.; P->inv = 0; P->fwd = s_forward; ENDENTRY(P)
/*
** $Log: proj_semiconf.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
