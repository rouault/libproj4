/*
** Copyright (c) 2009   Gerald I. Evenden
**
** "$Id: geodesic.h,v 2.3 2009/03/19 20:06:13 gie Exp gie $";
**
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
/** @file geodesic.h
 * @brief program geodesic header file
 */
#define P1GOOD 1		//!< @brief point 1 established - arcstate bit flag
#define P2GOOD 2		//!< @brief point 2 established - arcstate bit flag
#define COMPD 4			//!< @brief arc computed - arcstate bit flag
#define EARTHSET 8		//!< @brief ellipse set - arcstate bit flag
#define VECTOR 16		//!< @brief vector mode - arcstate bit flag
#define OP_FORWARD 2		//!< @brief display forward - last_op bit flag
#define OP_NONE 0		// no last operation on arc
#define OP_INVERSE 1		// inverse was last operation on arc
#define MAX_FMT 50		//!< @brief maximum output format work line (for 2 values)
#define MAXLINE 1000		//!< @brief maximut input line length to be read
#define PRNT_DUMP 1		//!< @brief dump or flat line list output mode -- pmode
#define PRNT_DMS 2		//!< @brief use DMS format on angular output -- pmode
#define PRNT_LAT_LON 4		//!< @brief output geographic in lat lon order -- pmode
#define PRNT_NO_HGT 8		//!< @brief do not print height field -- pmode
PROJ_PT_LPH point1,		//!< @brief p1 value
 point2;			//!< @brief p2 value
PROJ_ELLIPS earth;		//!< @brief ellipsoid value
/* area for compile initialized data */
#ifdef GEODESIC_MAIN
char dist_fmt[MAX_FMT] = { "%.3f" };
char out_fmt[MAX_FMT] = { "%4dd%02d'%08.5f\"%c" };
int display = PRNT_DMS;
FILE *out_file = NULL, *datain = NULL;
PROJ_LINE arc = { &point1, 0., &point2, 0., -1., &earth };
#else
extern char dist_fmt[MAX_FMT];	//!< @brief format for distance value
extern char out_fmt[MAX_FMT];	//!<  @brief format for DMS values
extern PROJ_LINE arc;		//!<  @brief geodesic arc
extern int display;		//<! @brief display options
FILE *out_file,			//!< @brief alternate input
*datain;			//!< @brief alternate output
#endif
int arcstate;			//!< @brief status of arc structure information
int last_op;			//!< @brief last operation on arc
// prototypes for global functions
int set_ellps(PROJ_PARAM_ITEM *);
int out_open(PROJ_PARAM_ITEM *);
void show_ellps(void);
int node(PROJ_PARAM_ITEM *);
int vector(PROJ_PARAM_ITEM *);
int do_batch(PROJ_PARAM_ITEM *);
int in_open(PROJ_PARAM_ITEM *);
int set_dump(PROJ_PARAM_ITEM *);
void show_arc(PROJ_LINE *, int, int, FILE *);
int aline(PROJ_PARAM_ITEM *);
int circle(PROJ_PARAM_ITEM *);
/*
 * $Log: geodesic.h,v $
 * Revision 2.3  2009/03/19 20:06:13  gie
 * updated for libproject and indent format
 *
 * Revision 2.2  2009/02/26 20:58:58  gie
 * corrected some spelling lapses
 *
 * Revision 2.1  2009/02/25 17:27:56  gie
 * *** empty log message ***
 *
 * Revision 1.1  2009/02/25 16:46:18  gie
 * Initial revision
 *
 * Revision 1.2  2009/01/08 01:48:05  gie
 * prep for release
 *
 * Revision 1.1  2008/12/07 18:48:11  gie
 * Initial revision
 *
 */
