/*
** Copyright (c) 2003, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_phi2.c,v 5.2 2009/05/06 18:46:51 gie Exp gie $";
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
/** @file proj_phi2.c
 * @brief determine latitude angle phi-2
 * @author Gerald I. Evenden
 */
#include <project.h>

#define TOL 1.0e-10
#define N_ITER 15
/** @brief determine latitude phi2
 * @param[in] ts ??
 * @param[in] e eccentricity
 * @return latitude
 *
 * See manual for detail information.
 */
	double
proj_phi2(double ts, double e) {
	double eccnth, Phi, con, dphi;
	int i;

	eccnth = .5 * e;
	Phi = M_PI_2 - 2. * atan (ts);
	i = N_ITER;
	do {
		con = e * sin (Phi);
		dphi = M_PI_2 - 2. * atan (ts * pow((1. - con) /
		   (1. + con), eccnth)) - Phi;
		Phi += dphi;
	} while ( fabs(dphi) > TOL && --i);
	if (i <= 0)
		proj_errno = -18;
	return Phi;
}
/*
** $Log: proj_phi2.c,v $
** Revision 5.2  2009/05/06 18:46:51  gie
** updates for Doxygen
**
** Revision 5.1  2009/04/30 20:47:47  gie
** *** empty log message ***
**
*/
