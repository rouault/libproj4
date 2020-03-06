#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([geodesic], 1.0, [geraldi.evenden@gmail.com])
AC_CONFIG_SRCDIR([src/geodesic.c])
AC_CONFIG_HEADER([src/config.h])
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CC_C99
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([m],[cos])
AC_CHECK_LIB([project],[proj_param])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([project.h math.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
#AC_FUNC_MALLOC
AC_FUNC_STRTOD
#AC_CHECK_FUNCS([sqrt proj_casecmp strchr strerror proj_ncasecmp])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
