/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_maurer.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
PROJ_HEAD(maurer, "Maurer")
	"\ntype: Cylindrical\nmode: sphere\n";
FORWARD(s_forward); /* spheroid */

	xy.x = lp.lam * (M_PI - 2. * lp.phi) / M_PI ;
	xy.y = lp.phi;
	return (xy);
}
INVERSE(s_inverse);
	lp.phi = xy.y;
	lp.lam = M_PI * xy.x / (M_PI - 2. * lp.phi);
	return (lp);
}
FREEUP; if (P) free(P); }
ENTRY0(maurer)
	P->es = 0.;
	P->inv = s_inverse;
	P->fwd = s_forward;
ENDENTRY(P)
/*
** $Log: proj_maurer.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
