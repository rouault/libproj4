/*
** Copyright (c) 2008, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_gdinverse.c,v 5.6 2009/05/17 19:41:47 gie Exp gie $";
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
/** @file proj_gdinverse.c 
 * @brief general inverse projection support
 * @author Gerald I. Evenden
 */
#include "config.h"
#include <stdlib.h>

#if HAVE_LIBGSL

#include <gsl/gsl_vector.h>
#include <gsl/gsl_multiroots.h>
#include <project.h>
#define LOOP_LIMIT 10

struct p_gsl { PROJ *P; double x, y; };
typedef struct p_gsl GP;
	static int
func_f(const gsl_vector *x, void *p, gsl_vector *f) {
//func_f(const gsl_vector *x, GP *p, gsl_vector *f) {
	PROJ_LP lp;

	lp.lam = gsl_vector_get(x,0);
	lp.phi = gsl_vector_get(x,1);
	PROJ_XY xy = (*((GP *)p)->P->fwd)(lp, ((GP *)p)->P);
	gsl_vector_set(f, 0, xy.x - ((GP *)p)->x);
	gsl_vector_set(f, 1, xy.y - ((GP *)p)->y);
	return GSL_SUCCESS;
}
	static int
func_df(const gsl_vector * x, void *p, gsl_matrix *J) {
	struct PROJ_DERIVS der;
	PROJ_LP lp;

	lp.lam = gsl_vector_get(x,0);
	lp.phi = gsl_vector_get(x,1);
	(*((GP *)p)->P->derivs)(((GP *)p)->P, lp, &der);
	gsl_matrix_set(J, 0, 0, der.x_l);
	gsl_matrix_set(J, 0, 1, der.x_p);
	gsl_matrix_set(J, 1, 0, der.y_l);
	gsl_matrix_set(J, 1, 1, der.y_p);
	return GSL_SUCCESS;
}
	static int
func_fdf(const gsl_vector * x, void *p, gsl_vector *f,  gsl_matrix *J) {
	func_f(x, p, f);
	func_df(x, p, J);
	return GSL_SUCCESS;
}
/* @brief determine projection inverse
 * @param[in] P projection control structure
 * @param est estimated geographic answer
 * @param[in] xy Cartesian location
 * @param[in] tol tolerance
 * @return ==0 if successful
 */
	int
proj_gdinverse(PROJ *P, PROJ_LP *est, PROJ_XY xy, double tol) {
	const gsl_multiroot_fdfsolver_type *T2;
	const gsl_multiroot_fsolver_type *T1;
	gsl_multiroot_fdfsolver *s2;
	gsl_multiroot_fsolver *s1;
	int status;
	size_t i, iter = 0;
	const size_t n = 2;
	GP p;
	int ders = P->derivs != NULL;
	gsl_multiroot_function_fdf f2 = {&func_f, &func_df, &func_fdf, n, &p};
	gsl_multiroot_function f1 = {&func_f, n, &p};

	p.P = P;
	p.x = xy.x;
	p.y = xy.y;
	gsl_vector *x = gsl_vector_alloc(n);
	gsl_vector_set(x, 0, est->lam);
	gsl_vector_set(x, 1, est->phi);
	if (ders) {
		T2 = gsl_multiroot_fdfsolver_gnewton;
		s2 = gsl_multiroot_fdfsolver_alloc(T2, n);
		gsl_multiroot_fdfsolver_set(s2, &f2, x);
	} else {
		T1 = gsl_multiroot_fsolver_hybrids;
		s1 = gsl_multiroot_fsolver_alloc(T1, n);
		gsl_multiroot_fsolver_set(s1, &f1, x);
	}
	do {
		++iter;
		status = ders ? gsl_multiroot_fdfsolver_iterate(s2):
						gsl_multiroot_fsolver_iterate(s1);
		if (status)
			break;
		status = gsl_multiroot_test_residual( ders?s2->f : s1->f, tol);
	} while (status == GSL_CONTINUE && iter < LOOP_LIMIT);
	if (ders) {
		est->lam = gsl_vector_get(s2->x,0);
		est->phi = gsl_vector_get(s2->x,1);
		gsl_multiroot_fdfsolver_free(s2);
	} else {
		est->lam = gsl_vector_get(s1->x,0);
		est->phi = gsl_vector_get(s1->x,1);
		gsl_multiroot_fsolver_free(s1);
	}
	gsl_vector_free(x);
	return 0;
}
#else // end LIBGSL true
/* the following is present to resolve any funky linking and to
 * ensure the system fails if gdinverse is called when the GSL flag
 * is not set.
*/
#include <stdio.h>
#include <project.h>
	int
proj_gdinverse(PROJ *P, PROJ_LP *est, PROJ_XY xy, double tol) {
	fprintf(stderr,"libproj4: entered gdinverse---no GSL avail. See admin\n");
	exit(-1);
	return -1;
}
#endif // end LIBGSL condition
/*
 * $Log:
 */
