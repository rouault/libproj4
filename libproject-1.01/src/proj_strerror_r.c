/*
** Copyright (c) 2005, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_strerror_r.c,v 5.4 2009/05/17 19:27:40 gie Exp gie $";
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
/** @file  proj_strerror_r.c
 * @brief return error string
 * @author Gerald I. Evenden
 */
#include "config.h"
#include <project.h>
#include <stdio.h>
#include <string.h>
extern const PROJ_ERR_LIST proj_err_list[];
/* @brief error string
 * @param[in] err error number
 * @param[out] buf character buffer for ourput string
 * @param[in] len character length of buffer
 * @return error description string
 *
 * The procedure is thread safe as long as the library was
 * compiled with HAVE_THREADS option.
 */
	int
proj_strerror_r(int err, char *buf, int len) {
		
	if (err > 0) {
#if HAVE_LIBPTHREAD
		return strerror_r(err, buf, len);
#else // end LIBPTHREAD true
		return (snprintf(buf, len, "sys. error no: %d, (no strerror_r)",
						err) == EOF? -1: 0);
#endif // end LIBPTHREAD check
	} else {
		int i, n;

		for (i = 0; proj_err_list[i].errnum < 0 &&
					(proj_err_list[i].errnum != err); ++i) ;
		n = strlen(proj_err_list[i].name) + 1;
		if (n > len) n = len;
		strncpy(buf, proj_err_list[i].name, n);
		buf[n] = '\0';
		return proj_err_list[i].errnum ? 0 : -1;
	}
}
/*
** $Log: proj_strerror_r.c,v $
** Revision 5.4  2009/05/17 19:27:40  gie
** changed conditional test name
**
** Revision 5.3  2009/05/06 18:46:51  gie
** updates for Doxygen
**
** Revision 5.2  2009/05/05 23:40:30  gie
** restored config.h header
**
** Revision 5.1  2009/04/30 20:47:47  gie
** *** empty log message ***
**
*/
