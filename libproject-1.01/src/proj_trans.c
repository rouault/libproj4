/*
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_trans.c,v 5.2 2009/05/06 18:46:51 gie Exp gie $";
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
/** @file proj_trans.c
 * @brief Alternative transendental functions
 * @author Gerald I. Evenden
 *
 * For checked near domain range problems
*/
#include <project.h>
#define ONE_TOL	 1.00000000000001
#define TOL	0.000000001
#define ATOL 1e-50
/** @brief arc sine function
 * @param[in] v radian angle
 * @return arc sine(v)
 */
	double
proj_asin(double v) {
	double av;

	if ((av = fabs(v)) >= 1.) {
		if (av > ONE_TOL)
			proj_errno = -19;
		return (v < 0. ? -M_PI_2 : M_PI_2);
	}
	return asin(v);
}
/** @brief arc cosine function
 * @param[in] v radian angle
 * @return arc cosine(v0
 */
	double
proj_acos(double v) {
	double av;

	if ((av = fabs(v)) >= 1.) {
		if (av > ONE_TOL)
			proj_errno = -19;
		return (v < 0. ? M_PI : 0.);
	}
	return acos(v);
}
/** @brief square root
 * @param[in] v argument
 * @return square root of v
 *
 * Just assume anything less than 0 is 0.  A bit dangerous but usually
 * this problem only occurs with *very* small negative arguments.
 */
	double
proj_sqrt(double v) { return ((v <= 0) ? 0. : sqrt(v)); }
/** @brief arc tangent(n/d)
 * @param[in] n numerator
 * @param[in] d denominator
 * @return arc tangent(n/d)
 *
 * Problem is when both approach zero---so we force 0 result
 */
	double
proj_atan2(double n, double d) {
	return ((fabs(n) < ATOL && fabs(d) < ATOL) ? 0. : atan2(n,d));
}
/*
** $Log: proj_trans.c,v $
** Revision 5.2  2009/05/06 18:46:51  gie
** updates for Doxygen
**
** Revision 5.1  2009/04/30 20:47:47  gie
** *** empty log message ***
**
*/
