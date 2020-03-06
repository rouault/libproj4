/*
** Copyright (c) 2003, 2005, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_translate.c,v 5.2 2009/05/06 18:46:51 gie Exp gie $";
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
/** @file  proj_translate.c
 * @brief general axis translation on a sphere
 * @author Gerald I. Evenden
 */
#define PROJ_LIB__
#include <project.h>

struct TRANS {
	double cosal, sinal, beta; \
	int mode;
};
#define TOL 1e-12
#define EN ((struct TRANS *)en)
/** @brief translate geographic coordinates
 * @param[in] in geographic coordinates
 * @param[in] en translation constants stucture
 * @return translated geographic coordinates
 */
	PROJ_LP
proj_translate(PROJ_LP in, const void *en) {
	PROJ_LP out;
	double cp, sp, cl;

	cp = cos(in.phi);
	cl = cos(in.lam);
	switch (EN->mode) {
	case 0:
		sp = sin(in.phi);
		out.phi = proj_asin(EN->sinal * sp - EN->cosal * cp * cl);
		out.lam = proj_adjlon(
			atan2(cp * sin(in.lam) , (EN->sinal * cp * cl + EN->cosal * sp))
			+ EN->beta);
		break;
	case 1:
		out.phi = proj_asin(- cp * sin(in.lam));
		out.lam = atan2(-cl, tan(in.phi));
		break;
	case 2:
	default:
		out.phi = proj_asin(- cp * sin(in.lam));
		out.lam = atan2(tan(in.phi), cl);
		break;
	}	
	return (out);
}
/** @brief inverse translate geographic coordinates
 * @param[in] in geographic coordinates
 * @param[in] en translation constants stucture
 * @return inverse translated geographic coordinates
 */
	PROJ_LP
proj_inv_translate(PROJ_LP in, const void *en) {
	PROJ_LP out;
	double cp, sp, cl, sl;

	cp = cos(in.phi);
	sp = sin(in.phi);
	switch (EN->mode) {
	case 0:
		cl = cos(in.lam -= EN->beta);
		sl = sin(in.lam);
		out.phi = proj_asin(EN->sinal * sp + EN->cosal * cp * cl);
		out.lam = atan2(cp * sl, EN->sinal * cp * cl - EN->cosal * sp);
		break;
	case 1:
		cl = cos(in.lam);
		sl = sin(in.lam);
		out.phi = proj_asin(cp * cl);
		out.lam = atan2(sl * cp,  -sp) + M_PI_2;
		break;
	case 2:
	default:
		cl = sin(in.lam);
		sl = cos(in.lam);
		out.phi = proj_asin(cp * cl);
		out.lam = atan2(sl * cp, sp) - M_PI_2;
		break;
	}
	return (out);
}
/** @brief generate geographic coordinate constants
 * @param[in] alpha
 * @param[in] beta
 * @return pointer of translation constant stucture
 *
 * See manual for details
 */
	void *
proj_translate_ini(double alpha, double beta) {
	struct TRANS *en;

	if ((en = (struct TRANS *)malloc(sizeof(struct TRANS))) == NULL)
		return (NULL);
	EN->mode = 0;
	if ( fabs(alpha) < TOL ) {
		if ( (fabs(beta) < TOL) )
			EN->mode = 1;
		else if ( (fabs(beta - M_PI_2) < TOL) )
			EN->mode = 2;
	}
	if (EN->mode == 0) {
		EN->sinal = sin(alpha);
		EN->cosal = cos(alpha);
		EN->beta = beta;
	}
	return ((void *)en);
}
/*
** $Log: proj_translate.c,v $
** Revision 5.2  2009/05/06 18:46:51  gie
** updates for Doxygen
**
** Revision 5.1  2009/04/30 20:47:47  gie
** *** empty log message ***
**
*/
