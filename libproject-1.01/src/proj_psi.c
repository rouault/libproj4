/*
** Copyright (c) 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_psi.c,v 5.2 2009/05/06 18:46:51 gie Exp gie $";
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
/** @file proj_psi.c
 * @brief isometric latitude routines
 * @author Gerald I. Evenden
 */
#include	<project.h>
#define MAX_ITER 11
#define EPS 1e-14
/** @brief isometric latitude
 * @param[in] phi latitude
 * @param[in] sphi sine(latitude)
 * @param[in] e eccentricity
 * @return isometric latitude
 */
	double
proj_psi(double phi, double sphi, double e) {
	double esp = e * sphi;

	return log(tan(M_PI_4 + 0.5 * phi) * pow((1. - esp)/(1. + esp), 0.5 * e));
}
/** @brief inverse isometric latitude
 * @param[in] psi isometric latitude
 * @param[in] e eccentricity
 * @return geographic latitude
 */
	double
proj_apsi(double psi, double e) {
	double esp, phi, phi0, he = e * 0.5, exp_psi = exp(psi);
	int i = MAX_ITER;

	phi0 = 2. * atan(exp_psi) - M_PI_2;
	while (--i) {
		esp = e * sin(phi0);
		phi = 2. * atan(pow((1. + esp)/(1. - esp), he) * exp_psi) - M_PI_2;
		phi0 = phi;
	} while ((fabs(phi0 - phi) > EPS) && --i);
	// if (!i) runaway
	return phi;
}
/*
** $Log: proj_psi.c,v $
** Revision 5.2  2009/05/06 18:46:51  gie
** updates for Doxygen
**
** Revision 5.1  2009/04/30 20:47:47  gie
** *** empty log message ***
**
*/
