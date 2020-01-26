#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT([libbptree], [0.1], [])
AM_INIT_AUTOMAKE([foreign])
LT_INIT
AC_CONFIG_SRCDIR([src/bptree.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_SEARCH_LIBS([ceil], [m])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile
                 examples/Makefile
                 tests/Makefile])
AC_OUTPUT
