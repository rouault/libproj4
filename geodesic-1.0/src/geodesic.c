/**
 * @file geodesic.c
 * @brief main entry and support
 */
/*
** Copyright (c) 2009   Gerald I. Evenden
*/
static const char
 PROJ_ID[] = "$Id: geodesic.c,v 2.6 2009/05/16 23:59:43 gie Exp gie $";
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <project.h>
#define GEODESIC_MAIN		//!< @brief tag for compile global initializations
#include "geodesic.h"
#include "geodesic_cmd.h"
#define COMMENT '#'		//!< @brief comment character - terminates line scan
static PROJ_PARAM_ITEM *list_base;	//!< @brief base address of linked argument list
static struct
gengetopt_args_info args_info;	//!< @brief link to command line stuff
static
char *prompt = "geod: ";	//!< @brief prompt string
static int show(PROJ_PARAM_ITEM *);
static int quitter(PROJ_PARAM_ITEM *);
static int set_display(PROJ_PARAM_ITEM *);
/** @brief list of user commands */
static struct cmd_table {
    const char *id;	/**< command string */
    int (*func) (PROJ_PARAM_ITEM *);	/**< ptr. to function to be executed */
    const char *desc;	  /**< description string of function */
} *list, elem[] = {
    {
    "earth", set_ellps, "set earth shape parameters"}, {
    "show", show, "display [arc|earth] structure contents"}, {
    "p1", node, "set point 1 [lon lat]"}, {
    "p2", node, "set point 2 [lon lat]"}, {
    "batch", do_batch, "batch process single line data"}, {
    "display", set_display, "control display of output"}, {
    "output", out_open, "open output file"}, {
    "input", in_open, "open input file"}, {
    "vector", vector, "set vector [distance azimuth"}, {
    "line", aline, "compute geodesic line"}, {
    "circle", circle, "compute circle or arc [2nd azi. del azi.]"}, {
    "quit", quitter, "exit program"}, {
    "help", NULL, "show this list"}, {
    NULL, NULL, NULL}
};

/** @brief fetch input command line and do checks
 * @param prompt pointer to prompt string
 * @return pointer to command line segment.
 *
 * Reads input from stdin and checks for COMMENTS (comments).
 * Checks that maximum length of line does noe exceed MAXLINE.
 * */
static char *readline(const char *prompt) {
    static char line[MAXLINE + 1];
    char *input = line;
    int c, n = MAXLINE;

    if (prompt != NULL)
	fputs(prompt, stdout);
    *line = '\0';
    while ((c = fgetc(stdin)) != EOF) {
	if (c == '\n') {
	    *input = '\0';
	    break;
	}
	if (c == COMMENT) {	/* comment, flush remainder of line */
	    int d;
	    *input = '\0';
	    while ((d = fgetc(stdin)) != EOF && d != '\n') ;
	    break;
	}
	if (n-- > 0)
	    *input++ = c;
	else if (n == -1)
	    fprintf(stderr, "input line longer than %d char encountered\n",
		    MAXLINE);
    }
    return ((c == EOF) ? NULL : line);
}

/** @brief program completion procedure
 * @param start option list pointer
 *
 * Procedure to cleanup operations and quit.
 */
static int quitter(PROJ_PARAM_ITEM * start) {
    const char *help =
	"Not much help needed here.  Enter this command and the program\n"
	"terminates with a G'day mate!\n";

    if ((start != NULL) && (proj_param(start, "bhelp", 0) == 1)) {
	fputs(help, stdout);
    } else {
	if (prompt != NULL)
	    fputs("geodesic at completion.   G'day mate!\n", stdout);
	proj_free_param(list_base);	/* OK if already NULL */
	cmdline_parser_free(&args_info);
	exit(0);
    }
    return 0;
}

/** @brief display parser to determine either arc or earth to be displayed
 * @param start argument linked list */
static int show(PROJ_PARAM_ITEM * start) {
    const char *help =
	"\n\"show [arc|earth]\" displays the contents of theses sturctures.\n"
	"This option is often performed automatically at the completion of\n"
	"other operations that alter the contents of either of these structures.\n\n"
	"Additional options for 'arc' are\n"
	"  \"dump[=f]\" to control pretty display or single line dump\n"
	"  \"dms[=f]\" to control DMS or decimal degree display\n"
	"  \"lat_lon[=f]\" to control order of longitude latitude\n"
	"The '=f' of '=F' turns a currently active option off\n\n";

    if ((start->next == 0) || (proj_param(start, "bhelp", 0) == 1))
	fputs(help, stdout);
    else {
	if (proj_param(start, "bearth", 0) == 1)
	    show_ellps();
	if (proj_param(start, "barc", 0) == 1) {
	    int pmode = display, ret;

	    if (proj_param(start, "bdms", &ret) == 1) {
		if (ret)
		    pmode |= PRNT_DMS;
		else
		    pmode &= ~PRNT_DMS;
	    }
	    if (proj_param(start, "bdump", &ret) == 1) {
		if (ret)
		    pmode |= PRNT_DUMP;
		else
		    pmode &= ~PRNT_DUMP;
	    }
	    if (proj_param(start, "blat_lon", &ret) == 1) {
		if (ret)
		    pmode |= PRNT_LAT_LON;
		else
		    pmode &= ~PRNT_LAT_LON;
	    }
	    show_arc(&arc, arcstate, pmode, stdout);
	}
    }
    return 0;
}

/** @brief set/unset dump mode from command input
 * @param list pointer to argument linked list
 * Acts only in toggle mode */
static int set_display(PROJ_PARAM_ITEM * list) {
    const char *help =
	"\nToggles the various display modes of the output of arc display\n\n"
	"The conditions that may be controlled are:\n"
	"   \"dump\" - single line output vs. the labelled, multi-line display\n"
	"   \"lat_lon\" - print lat-lon versus lon_lat\n"
	"   \"dms\" - print xxdxx'xx.xx\" otherwise decimal degrees\n"
	"   \"prec=n\" - set precision of lat/lon/distance\n"
	"If a '=f' follows the first 3 options then the option is turned off,\n"
	"otherwise the state of the option is turned on.\n\n"
	"Default values are dump=off, lat_lon=off, dms=on, prec=3\n\n";
    int ret;

    if (proj_param(list, "bhelp", 0) == 1)
	fputs(help, stdout);
    else {
	if (proj_param(list, "bdms", &ret) == 1) {
	    if (ret)
		display |= PRNT_DMS;
	    else
		display &= ~PRNT_DMS;
	}
	if (proj_param(list, "bdump", &ret) == 1) {
	    if (ret)
		display |= PRNT_DUMP;
	    else
		display &= ~PRNT_DUMP;
	}
	if (proj_param(list, "blat_lon", &ret) == 1) {
	    if (ret)
		display |= PRNT_LAT_LON;
	    else
		display &= ~PRNT_LAT_LON;
	}
	if (proj_param(list, "iprec", &ret) == 1) {
	    if (ret <= 0) {
		fprintf(stderr, "prec value (%d) <= 0\n", ret);
		return -3;
	    }
	    sprintf(dist_fmt, "%%.%df", ret);
	    sprintf(out_fmt, "%%4dd%%02d'%%0%d.%df\"%%c", ret + 5, ret + 2);
	}
    }
    return 0;
}

/**
 * @brief check list for unused options and print
 * @param list option linked list
 *
 * Somewhat after the fact diagnosis but better than nothing
 */
static void check_list(PROJ_PARAM_ITEM * list) {
    while (list != NULL) {
	if (list->used == 0)
	    fprintf(stderr, "parameter <%s> not used\n", list->param);
	list = list->next;
    }
}

/** @brief separate command line into argument groups
 * @param line input data line
 */
static void do_commands(char *line) {
    const char *help =
	"\nFor more detailed assistance type \"<command> help\" for\n"
	"more detailed information on that command.\n";
    char *next, *firstone;
    const char *opr = line;
    PROJ_PARAM_ITEM *curr;
    int first = 1;

    if (*line == '\0')
	return;
    do {			/* breakdown line into command groups separated by ';' */
	if ((next = strchr(line, ';')) != NULL)
	    *next++ = '\0';
	opr = line;
	do {			// this handles multiple tokens in string
	    while (isspace(*opr))	// skip over white space
		++opr;
	    if (*opr) {		// if something there add it to list
		if (first) {
		    list_base = curr = proj_mkparam(opr, &opr);
		    (void)proj_param(list_base, "s", &firstone);
		    first = 0;
		} else
		    curr = curr->next = proj_mkparam(opr, &opr);
	    }
	} while (*opr);
	if (proj_ncasecmp(firstone, "help", 4) == 0) {	// was 1st "help"
	    for (list = elem; list->id != NULL; ++list)
		printf("command: %s -- %s\n", list->id, list->desc);
	    fputs(help, stdout);
	} else {
	    for (list = elem; (list->id != NULL) &&
		 (proj_casecmp(list->id, firstone) != 0); ++list) ;
	    if (list->id != NULL) {
		list->func(list_base);
		/* free list */
		check_list(list_base);
		list_base = proj_free_param(list_base);
	    } else
		fprintf(stderr, "invalid operation: %s\n", opr);
	}
    } while ((line = next) != NULL);
}

/** @brief main entry
 * @param argc number of argument strings
 * @param argv array of argument strings
 */
int main(int argc, char *argv[]) {
    int nc;
    char *line;

    if (cmdline_parser(argc, argv, &args_info) != 0)
	exit(1);
    /* check is prompt to be suppressed */
    if (args_info.no_prompt_given)
	prompt = NULL;
    else {
	cmdline_parser_print_version();
	fputs("Free software with X11 license\n", stdout);
    }
    arc.pt1 = &point1;
    arc.pt2 = &point2;
    arc.E = &earth;
    out_file = stdout;
    datain = stdin;
    // skip over pre -- stuff
    for (nc = 1; nc < argc && (strncmp("--", argv[nc], 2) != 0); ++nc) ;
    // do runline commands
    for (++nc; nc < argc; ++nc)
	do_commands(argv[nc]);
    // do "sysin" commands
    while ((line = readline(prompt)) != NULL)
	do_commands(line);
    // done
    quitter(NULL);
}

/*
 * $Log: geodesic.c,v $
 * Revision 2.6  2009/05/16 23:59:43  gie
 * several changes due to dropping use of strtok
 *
 * Revision 2.5  2009/04/03 18:25:37  gie
 * *** empty log message ***
 *
 * Revision 2.4  2009/03/19 20:06:13  gie
 * updated for libproject and indent format
 *
 * Revision 2.3  2009/02/26 20:58:58  gie
 * corrected some spelling lapses
 *
 * Revision 2.2  2009/02/25 20:19:09  gie
 * error monitoring of unused parameters
 *
 * Revision 2.1  2009/02/25 17:27:56  gie
 * *** empty log message ***
 *
 * Revision 1.1  2009/02/25 16:45:19  gie
 * Initial revision
 *
 * Revision 1.3  2009/01/09 21:34:45  gie
 * update command line closing
 *
 * Revision 1.2  2009/01/08 01:46:11  gie
 * prep for release
 *
 * Revision 1.1  2008/12/07 18:47:07  gie
 * Initial revision
 *
 */
