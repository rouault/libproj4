/*
** Copyright (c) 2009   Gerald I. Evenden
*/
static const char
 PROG_ID[] = "$Id: proj_primes.c,v 5.1 2009/05/15 17:14:14 gie Exp gie $";
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
/** @file proj_primes.c
 * @brief Defines list of common prime meridians
 *
 * Extracted and clensed from Frank Warmerdam's effort in proj-4.6.1
 *
 * At the moment, values not verified.
 */
#define PROJ_PRIMES_LIST_FLAG
#include <project.h>
struct PROJ_PRIMES_LIST proj_pm_list[] = {
    {"Athens",    "23d42'58.815\"E"},
    {"Bern",      "7d26'22.5\"E"},
    {"Bogota",    "74d04'51.3\"W"},
    {"Brussels",  "4d22'4.71\"E"},
    {"Ferro",     "17d40'W"},
    {"Greenwich", "0dE"},
    {"Jakarta",   "106d48'27.79\"E"},
    {"Lisbon",    "9d07'54.862\"W"},
    {"Madrid",    "3d41'16.58\"W"},
    {"Oslo",      "10d43'22.5\"E"},
    {"Paris",     "2d20'14.025\"E"},
    {"Rome",      "12d27'8.4\"E"},
    {"Stockholm", "18d3'29.8\"E"},
    {NULL, NULL}
};

/*
 * $Log: proj_primes.c,v $
 * Revision 5.1  2009/05/15 17:14:14  gie
 * initial
 *
*/
