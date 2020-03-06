/*
** Copyright (c) 2003 2005, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_strerrno.c,v 5.2 2009/05/06 18:46:51 gie Exp gie $";
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
/** @file  proj_strerrno.c
 * @brief list of projection system proj_errno values
 * @author Gerald I. Evenden
*/
#include <project.h>
#include <string.h>
#define MAX_LEN 160
static char temp[MAX_LEN + 1];
extern const PROJ_ERR_LIST proj_err_list[];
/** @brief list projection error message
 * @param[in] err projection error number
 * @return string of error message
 */
	const char *
proj_strerrno(int err) {

	if (err > 0)
		return strerror(err);
	else {
		int i, n;

		for (i = 0; proj_err_list[i].errnum < 0 &&
					(proj_err_list[i].errnum != err); ++i) ;
		if ((n = strlen(proj_err_list[i].name)) > MAX_LEN) n = MAX_LEN;
		strncpy(temp, proj_err_list[i].name, n);
		temp[n] = '\0';
		return temp;
	}
}
/*
** $Log: proj_strerrno.c,v $
** Revision 5.2  2009/05/06 18:46:51  gie
** updates for Doxygen
**
** Revision 5.1  2009/04/30 20:47:47  gie
** *** empty log message ***
**
*/
