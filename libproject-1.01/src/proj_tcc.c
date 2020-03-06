/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_tcc.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
	double ap;
#define EPS10 1.e-10
#define PROJ_LIB__
#include	<project.h>
PROJ_HEAD(tcc, "Transverse Central Cylindrical")
	"\ntype: Transverse Cylindrical\nmode: sphere\nNo inverse\n";
FORWARD(s_forward); /* spheroid */
	double b, bt;

	b = cos(lp.phi) * sin(lp.lam);
	if ((bt = 1. - b * b) < EPS10) F_ERROR;
	xy.x = b / sqrt(bt);
	xy.y = atan2(tan(lp.phi) , cos(lp.lam));
	return (xy);
}
FREEUP; if (P) free(P); }
ENTRY0(tcc) P->es = 0.; P->fwd = s_forward; ENDENTRY(P)
/*
** $Log: proj_tcc.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
