/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_wag3.c,v 5.2 2009/05/23 14:49:36 gie Exp gie $";
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
#define PROJ_PARMS__ \
	double	C_x;
#define PROJ_LIB__
# include	<project.h>
PROJ_HEAD(wag3, "Wagner III")
	"\ntype: Pseudocylindrical\nmode: sphere\n"
	"lat_ts=(0) latitude of true scale\n";
#define TWOTHIRD 0.6666666666666666666667
FORWARD(s_forward); /* spheroid */
	xy.x = P->C_x * lp.lam * cos(TWOTHIRD * lp.phi);
	xy.y = lp.phi;
	return (xy);
}
INVERSE(s_inverse); /* spheroid */
	lp.phi = xy.y;
	lp.lam = xy.x / (P->C_x * cos(TWOTHIRD * lp.phi));
	return (lp);
}
FREEUP; if (P) free(P); }
ENTRY0(wag3)
	double ts = 0.;

	(void)proj_param(P->params, "rlat_ts", &ts);
	P->C_x = cos(ts) / cos(2.*ts/3.);
	P->es = 0.; P->inv = s_inverse; P->fwd = s_forward;
ENDENTRY(P)
/*
** $Log: proj_wag3.c,v $
** Revision 5.2  2009/05/23 14:49:36  gie
** proper initialization procedures.
**
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
