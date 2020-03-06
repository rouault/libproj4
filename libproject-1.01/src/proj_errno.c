/*
** libproj -- library of cartographic projections
**
** Copyright (c) 2003, 2006, 2009   Gerald I. Evenden
*/
static const char
RCS_ID[] = "$Id: proj_errno.c,v 5.5 2009/05/17 19:27:40 gie Exp gie $";
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
/** @file  proj_errno.c
 * @brief error handler
 * @author Gerald I. Evenden
 */
#include "config.h"
#if HAVE_LIBPTHREAD
#include <pthread.h>
/* this makes project library thread friendly */
	static pthread_once_t
key_once = PTHREAD_ONCE_INIT;
	static pthread_key_t
key;
	static void
destroy(void *buf) { free(buf); }
	static void
create(void) { pthread_key_create(&key, destroy); }
/** @brief procedure to return pointer to error value
 * @return pointer to error number location
 */
	int *
proj_errno_loc(void) {
	void *p;

	pthread_once(&key_once, create);
	if (!(p = pthread_getspecific(key))) {
		p = (void *)malloc(sizeof(int));
		pthread_setspecific(key, p);
	}
	return (int *)p;
}
#else // end LIBPTHREAD true test
/* threads unfriendly */
	static int
proj_errno;
	int *
proj_errno_loc(void) { return &proj_errno; }
#endif // end LIBPTHREAD conditional
/*
** $Log: proj_errno.c,v $
** Revision 5.5  2009/05/17 19:27:40  gie
** changed conditional test name
**
** Revision 5.4  2009/05/06 18:46:51  gie
** updates for Doxygen
**
** Revision 5.3  2009/05/05 23:40:30  gie
** restored config.h header
**
** Revision 5.2  2009/05/03 00:13:24  gie
** change HAVE_PTHREADS id
**
** Revision 5.1  2009/04/30 20:47:47  gie
** *** empty log message ***
**
*/
