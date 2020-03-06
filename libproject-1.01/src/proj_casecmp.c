/*
** libgeodesic -- library of geodesic functions
**
** Copyright (c) 2009   Gerald I. Evenden
*/
/** @file proj_casecmp.c
 * @brief alternative to POSIX strcasecmp
 * @author Gerald I. Evenden
 */
#include <project.h>
static const char
LIBGEOD_ID[] = "$Id: proj_casecmp.c,v 5.2 2009/05/06 18:46:51 gie Exp gie $";
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
/**
 * @brief case independent string comparison of n characters
 * @param s1 pointer to null terminated string 1
 * @param s2 pointer to null terminated string 2
 * @return 0 s1==s2, <0 when s1<s2 and >0 when s1>s2
 *
 * This is a clone of POSIX procedure that may not be available
 * on some systems.
 */
#include <ctype.h>
int proj_casecmp(const char *s1, const char *s2) {
    int ret;

    while (((ret = tolower(*s1) - tolower(*s2)) == 0) && *s1++)
	++s2;
    return ret;
}
/*
 * $Log: proj_casecmp.c,v $
 * Revision 5.2  2009/05/06 18:46:51  gie
 * updates for Doxygen
 *
 * Revision 5.1  2009/04/30 20:47:47  gie
 * *** empty log message ***
 *
*/
