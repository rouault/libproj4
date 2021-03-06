/*
** Copyright (c) 2009   Gerald I. Evenden
*/
static const char
 PROJ_ID[] = "$Id: proj_ginverse.c,v 5.1 2009/04/30 20:47:47 gie Exp gie $";
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
/** @file proj_ginverse.c
 * @brief compute the inverse geodesic
 * @author Gerald I. Evenden
 */
#include <math.h>
#include <project.h>
/**
 * @brief Inverse geodesic computation - elliptical earth
 *  @param A pointer to geodesic line stucture
 *  @return 0 if OK else !=0
 * @author Gerald I. Evenden
 *
 * Determines the geodesic distance between A->pt1 and A-pt2
 * and the forward azimuth and back azimuths for each point.
 *
 * Preferred entry for geodesic computations.
 */
int proj_ginverse(PROJ_LINE * A) {
    if (A->pt1 == NULL || A->pt2 == NULL || A->E == NULL)
	return 1;
    if (A->E->f)		/* ellipsoid earth */
	proj_in_inv(A);
    else			/* spherical earth */
	proj_sp_inv(A);
    return 0;
}

/*
 * $Log: proj_ginverse.c,v $
 * Revision 5.1  2009/04/30 20:47:47  gie
 * *** empty log message ***
 *
*/
