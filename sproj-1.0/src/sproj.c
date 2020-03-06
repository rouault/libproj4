/**
 * @file sproj.c
 * @brief elementary cartographic projection program
 */
/*
** Copyright (c) 2009   Gerald I. Evenden
*/
	static const char
RCS_ID[] = "$Id: sproj.c,v 1.4 2009/05/16 20:39:28 gie Exp gie $";
/*
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** " Software "), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED " AS IS ", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <stdlib.h>
#include <stdio.h>
#include <project.h>
#include "sproj_cmd.h"
#define MAXFIELD 50 //!< maximum size of formatted item
#define MAXLINE 1000 //!< maximum length of input line
#define MAXERRORS 20 //!< maximum conversion errors before muting
static FILE *fileout = NULL,	//!< output file RFE
	*filein = NULL;	//!< input file RFE
static const char *outformat;	//!< output format
#define	inverse args_info.inverse_given
#define revin args_info.revin_given
#define revout args_info.revout_given
static struct gengetopt_args_info args_info;
static PROJ *PC = NULL;
static int errlimit = MAXERRORS;
/** @brief clean up and quit procedure
 * @param code error code if != 0
 *
 * Exit with -1 it code != 0 else normal exit 0 code
 */
	static void
endsproj(int code) {
	if (code) // print error codes
		fprintf(stderr, "code: %d, %s\n", code, proj_strerrno(code));
	if (PC != NULL) // close projection
		proj_free(PC);
	cmdline_parser_free(&args_info);
	exit(code ? -1 : 0);
}
/** @brief process data procedure loop
 */
	static void
process(void) {
	char field[2][MAXFIELD], // output fields
		line[MAXLINE];	// input data line
	PROJ_XY xy;
	PROJ_LP lp;

	while (fgets(line, MAXLINE, filein) != NULL) {
		if (*line == '#') {  // pass on comment lines
			fputs(line, fileout);
			continue;
		}
		{ 	double v0, v1;	// gather input and do projection operation
			char *s;
			if (inverse) {
				v0 = strtod(line, &s);
				v1 = strtod(s, &s);
				if (revin) {
					xy.x = v1;
					xy.y = v0;
				} else {
					xy.x = v0;
					xy.y = v1;
				}
				lp = proj_inv(xy, PC);
			} else {
				v0 = proj_dms2rad(line, &s);
				v1 = proj_dms2rad(s, &s);
				if (revin) {
					lp.lam = v1;
					lp.phi = v0;
				} else {
					lp.lam = v0;
					lp.phi = v1;
				}
				xy = proj_fwd(lp, PC);
			}
		}
		if (proj_errno) {
			fputs("*\t*\n", fileout);
			if (errlimit-- < 0)
				endsproj(proj_errno);
			else if (proj_errno)
				fprintf(stderr, "err: %d, %s\n", proj_errno,
					proj_strerrno(proj_errno));
			continue;
		}
		// output phase
		if (inverse) {
			proj_rad2dms(field[0], MAXFIELD, outformat, lp.lam, "EW");
			proj_rad2dms(field[1], MAXFIELD, outformat, lp.phi, "NS");
		} else {
			snprintf(field[0], MAXFIELD, outformat, xy.x);
			snprintf(field[1], MAXFIELD, outformat, xy.y);
		}
		fputs(field[revout?1:0], fileout);
		fputc('\t', fileout);
		fputs(field[revout?0:1], fileout);
		fputc('\n', fileout);
	}
}
/** @brief detailed list of projections or selected projection
 */
	static void
projlist(char *name) {
	const struct PROJ_LIST *lp = proj_list;
	int noredo;

	if ((noredo = (name != NULL)))
		while ((lp->id != NULL) && proj_casecmp(name, lp->id) != 0) ++lp;
	for ( ; lp->id != NULL; ++lp) {
		printf("%s : %s\n", lp->id, *lp->descr);
		if (noredo)
			break;
	}
}
/** @brief process various listing options
 */
	static void
dolists(void) {
	switch (args_info.list_arg) {
	case list_arg_proj:
		{ const struct PROJ_LIST *lp;
			const char *str;
			int c;

			for (lp = proj_list ; lp->id ; ++lp) {
				(void)printf("%s : ", lp->id);
				str = *lp->descr;
				while ((c = *str++) && c != '\n')
					putchar(c);
				putchar('\n');
			}
		} break;
	case list_arg_ellps:
		{	const struct PROJ_ELLPS_LIST *le;

			for (le = proj_ellps; le->id ; ++le)
				(void)printf("%9s %-16s %-16s %s\n",
						le->id, le->major, le->ell, le->name);
		} break;
	case list_arg_pms:
		{	const struct PROJ_PRIMES_LIST *lp;

			for (lp = proj_pm_list; lp->name != NULL; ++lp)
				(void)printf("%12s : %s\n", lp->name, lp->del_lon);
		} break;
	case list_arg_units:
		{	const struct PROJ_UNITS *lu;

			for (lu = proj_units; lu->id ; ++lu)
				(void)printf("%12s %-20s %s\n",
						lu->id, lu->to_meter, lu->name);
		} break;
	default:
		fputs("catastrophic program error in lists", stderr);
		exit(1);
	}
}
/** @brief main entry
 */
	int
main(int argc, char **argv) {

	if (cmdline_parser(argc, argv, &args_info) !=  0) {
		fprintf(stderr, "getopt open failure\n");
		exit(1);
	}
	if (args_info.list_given) // check for list option
		dolists();
	else if (args_info.longproj_given) // long projection list
		projlist(args_info.longproj_arg);
	else { // all remaining options do data conversion
		if (args_info.format_given) // use given format
			outformat = args_info.format_arg;
		else if (inverse) // else assign reasonable format
			outformat = "%dd%02d'%06.3f\"%c";
		else // dependent upon operation
			outformat = "%.4f";
		if ((PC = proj_init(args_info.inputs_num, args_info.inputs))==NULL) {
			fprintf(stderr, "Projection initialization failed\n");
			endsproj(proj_errno);
		}
		filein = stdin;
		fileout = stdout;
		process();
	}
	endsproj(0); // cleanup and quit
}
/*
 * $Log: sproj.c,v $
 * Revision 1.4  2009/05/16 20:39:28  gie
 * corrected doxygen line
 *
 * Revision 1.3  2009/05/16 14:54:14  gie
 * get the damn rcs fixed!!
 *
 * Revision 1.2  2009/05/16 14:51:35  gie
 * correct rcs annotation
 *
 */
