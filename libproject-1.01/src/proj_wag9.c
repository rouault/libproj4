/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_wag9.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
#define CM 0.77777777777777777777777777778
#define CN 0.27777777777777777777777777778
#define CX 3.6
#define CY 1.285714285714285714285714285
#define PROJ_LIB__
#include	<project.h>
PROJ_HEAD(wag9, "Wagner IX")
	"\ntype: Modified Azimuthal\nmode: sphere\n"
	"\nNo inverse\n";
FORWARD(s_forward); /* sphere */
	double cosdel, cosalph;

	if ((xy.y = proj_acos(cosdel = cos(lp.phi *= CM) * cos(CN * lp.lam))) == 0.)
		xy.x = xy.y = 0;
	else {
		cosalph = sin(lp.phi) / sqrt(fabs(1. - cosdel * cosdel));
		xy.x = CX * xy.y * sqrt(fabs(1. - cosalph * cosalph));
		if (lp.lam < 0.)
			xy.x = -xy.x;
		xy.y *= CY * cosalph;
	}
	return (xy);
}
FREEUP; if (P) free(P); }
ENTRY0(wag9) P->fwd = s_forward; P->inv = 0; P->es = 0.; ENDENTRY(P)
/*
** $Log: proj_wag9.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
