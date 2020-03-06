/*
** Copyright (c) 2009   Gerald I. Evenden
*/
static const char
 PROJ_ID[] = "$Id: proj_lph2xyz.c,v 5.1 2009/04/30 20:47:47 gie Exp gie $";
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
/** @file proj_lph2xyz.c
 * @brief conversion of lat-lon-h to xyz
 */
#include <stdlib.h>
#include <math.h>
#include <project.h>
/**
 * @brief Convert geographic lon/lat/height to Cartesian XYZ coordinates 
 * @param[in] P1 pointer to geographic coordinate structure
 * @param[out] P2 pointer to XYZ coordinate structure
 * @param[in] ell pointer to ellipsoid constants
 */
void proj_lph2xyz(PROJ_PT_LPH * P1, PROJ_PT_XYZ * P2, PROJ_ELLIPS * ell) {
    double en, clat, slat;

    slat = sin(P1->phi);
    clat = cos(P1->phi);
    en = ell->a / sqrt(1. - ell->es * slat * slat);
    P2->x = (en + P1->h) * clat * cos(P1->lam);
    P2->y = (en + P1->h) * clat * sin(P1->lam);
    P2->z = (en * ell->one_es + P1->h) * slat;
}
