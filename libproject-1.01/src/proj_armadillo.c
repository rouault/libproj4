/*
** Copyright (c) 2005, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_armadillo.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
#define YA 0.2011637612698801744949951686
#define YB 0.9396926207859083840541092773
#define YC 0.3420201433256687330440996146
#define TA 0.3639702342662023613510478827
#define PROJ_LIB__
# include	<project.h>
PROJ_HEAD(arma, "Armadillo")
	"\ntype: Orthoapsidal (coined by originator\nmode: sphere\n"
	"No inverse\n";
FORWARD(s_forward); /* spheroid */
	double cp, cl;

	if (lp.phi >= - atan((cl = cos(lp.lam *= 0.5))/TA) ) {
		xy.x = (1. + (cp = cos(lp.phi))) * sin(lp.lam);
		xy.y = YA + sin(lp.phi) * YB - (1. + cp) * YC * cl;
	} else
		F_ERROR;
	return (xy);
}
FREEUP; if (P) free(P); }
ENTRY0(arma) P->es = 0.; P->fwd = s_forward; ENDENTRY(P)
/*
** $Log: proj_armadillo.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
