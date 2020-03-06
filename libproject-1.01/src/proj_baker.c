/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_baker.c,v 5.1 2009/04/30 00:35:56 gie Exp gie $";
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
#define C2SQ2 2.828427124746190097603377448
PROJ_HEAD(baker, "Baker Dinomic")
	"\ntype: Miscellaneous\nmode: sphere\n"
	"No inverse\n";
FORWARD(s_forward); /* spheroid */
	double aphi = fabs(lp.phi);
	
	if (aphi < M_PI_4) {
		xy.x = P->k0 * lp.lam;
		xy.y = P->k0 * log(tan(M_PI_4 + .5 * lp.phi));
	} else {
		xy.x = lp.lam * cos(aphi) * (C2SQ2 - 1./sin(aphi));
		xy.y = -log(tan(0.5*aphi)) + C2SQ2 * (aphi - M_PI_4);
		if (lp.phi < 0.)
			xy.y = -xy.y;
	}
	return (xy);
}
FREEUP; if (P) free(P); }
ENTRY0(baker)
	P->es = 0.; P->fwd = s_forward;
ENDENTRY(P)
/*
** $Log: proj_baker.c,v $
** Revision 5.1  2009/04/30 00:35:56  gie
** *** empty log message ***
**
*/
