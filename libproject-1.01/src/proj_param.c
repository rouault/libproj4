/*
** Copyright (c) 2003, 2006, 2008, 2009  Gerald I. Evenden
*/
static const char
 PROJ_ID[] = "$Id: proj_param.c,v 5.2 2009/05/06 18:46:51 gie Exp gie $";
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
/** @file proj_param.c
 * @brief Interprets keyword parameter lists and returns values.
 * @author Gerald I. Evenden
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <project.h>
extern int errno;
/**
 * @brief enters a token string into a linked list
 * @param[in] str pointer to a string to be copied into a linked list
 * @param[out] newstr pointer to updates str pointing to end of token + 1
 * @return pointer to new link list structure and update string pointer
 */
PROJ_PARAM_ITEM *proj_mkparam(const char *str, const char **newstr) {
    PROJ_PARAM_ITEM *New;
    const char *s;
    size_t cnt = 0;

    if (*str == '+') 	// '+' prefix for historical reasons 
	++str;
    s = str;
    while (*s && !isspace(*s)) ++s; // look for end of token
    cnt = s - str;
    if ((New = (PROJ_PARAM_ITEM *)
	 malloc(sizeof(PROJ_PARAM_ITEM) + cnt + 1))) {
	New->used = 0;
	New->next = NULL;
	(void)strncpy(New->param, str, cnt);
	New->param[cnt] = '\0';
    }
    if (newstr) // only update if 'asked'
	*newstr = s;
    return New;
}

/** @brief delete linked parameter list
 * @return NULL
 *
 * Not much to say about the obvious.
 */
PROJ_PARAM_ITEM *proj_free_param(PROJ_PARAM_ITEM * p) {
    PROJ_PARAM_ITEM *pn;

    while (p != NULL) {
	pn = p->next;
	free(p);
	p = pn;
    }
    return NULL;
}

/**
 * @brief lookup keyword in list and extract data
 * @param[in] pl pointer to linked parameter list
 * @param[in] opt pointer to keyword prefixed with return type
 * @param[out] value pointer to value from found list item
 * @return =1 if keyword record found and value successfully decoded
 *
 * \li \c return =0 if keyword not found
 * \li \c return <0 if problems decoding information
 *
 * The prefix character of the key determines the treatment of the
 * \c value pointer with following options:\n
 * \li \c 'i<keyword>=' pointer to type int -- integer input
 * \li \c 't<keyword>' value ignored -- is something there? func return has it
 * \li \c 'd<keyword>=' pointer to type double -- double float input
 * \li \c 's<keyword>=' pointer to type char * -- character string
 * \li \c 'r<keyword>=' pointer to type double -- radian input from DMS
 * \li \c 'b<keyword>=' pointer to type int -- [T|1|<absent>] =1 else [F|0] = 0
 * \li \c 'p<keyword>' pointer to type PARAM_ITEM * -- item found
 *
 * For 'i', 'd', 'r' and 's' types and a return of <=0 then the location
 * pointed to by '*value' is unaltered.
 *
 * If the prefix type letter "itdsrbp" is not present, a return code of
 * -99 will be returned (value unaltered).
 *
 * For numeric values, if errno!=0 after a numeric conversion then the
 * return code is -2.  Note: errno is reset at entry to function regardless
 * of operation.
 *
 * If <keyword> absent look for the first item where 'used'==0 and treat
 * the param field the same as info after an '<keyword>=' in the manner 
 * described by the operation letter prefixing <keyword>. The 'used'
 * field will be or'ed with 1 except in the cases of t and p.
 *
 * If one of the above prefixes is not used the procedure catastophically
 * fails.
 *
 * If the looked for keyword is not in the list then a nominally 0 and
 * argument is set to 0.
 *
 */
int proj_param(PROJ_PARAM_ITEM * pl, const char *opt, void *value) {
    int type, ret = 0;
    int ivalue = 0;		// to keep pedantic quiet
    double dvalue = 0;;
    unsigned l;

    errno = 0;			// clear system error flag
    if (!strchr("itbdsrp", type = *opt++))
	return (-99);		// bad call, abort
    /* simple linear lookup */
    l = strlen(opt);
    if (l > 0)			/* search for keyword */
	while (pl && !(!proj_ncasecmp(pl->param, opt, l) &&
		       (!pl->param[l] || pl->param[l] == '=')))
	    pl = pl->next;
    else			/* search for first "unread" */
	while (pl && (pl->used != 0))
	    pl = pl->next;
    if (type == 't')
	ret = (pl != 0);	// end of list = 0 else 1 (something there)
    else if (type == 'p') {
	if (value)
	    *(PROJ_PARAM_ITEM **) value = pl;	// 0 if nothing there
	ret = (pl != 0);	// same as last pl test
    } else if (pl) {		// something there and lets process it
	char *dummy;
	opt = pl->param + l;
	if (*opt == '=')
	    ++opt;
	switch (type) {		// select type of process
	case 'i':		/* integer input */
	    if (value)
		ivalue = strtol(opt, &dummy, 0);
	    goto numcheck;
	case 'd':		/* simple real input */
	    if (value)
		dvalue = strtod(opt, &dummy);
	    goto numcheck;
	case 'r':		/* degrees input */
	    if (value)
		dvalue = proj_dms2rad(opt, &dummy);
	  numcheck:		// check possible bummer when converting numbers
	    if (errno)		// conversion error
		ret = -2;
	    else if ((dummy == opt) && (l == 0))	// unlikely occurance
		ret = 0;
	    else {
		ret = 1;
		if (type == 'i')
		    *(int *)value = ivalue;
		else
		    *(double *)value = dvalue;
	    }
	    break;
	case 's':		/* char string */
	    if (value && (*opt != '\0'))
		*(char **)value = (char *)opt;
	    ret = 1;
	    break;
	case 'b':		/* boolean */
	    ret = 1;
	    switch (*opt) {
	    case 'F':
	    case 'f':
		if (value)
		    *(int *)value = 0;
		break;
	    case '\0':
	    case 'T':
	    case 't':
		if (value)
		    *(int *)value = 1;
		break;
	    default:
		if (value)
		    *(int *)value = 0;
		break;
	    }
	    break;
	}
    } else {			// keyword entry not found
	ret = 0;
    }
    if (ret > 0)
	pl->used |= 1;
    return ret;
}

/*
** $Log: proj_param.c,v $
** Revision 5.2  2009/05/06 18:46:51  gie
** updates for Doxygen
**
** Revision 5.1  2009/04/30 20:47:47  gie
** *** empty log message ***
**
*/
