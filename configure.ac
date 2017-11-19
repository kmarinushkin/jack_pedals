#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([jack_pedals], [1.0], [k.marinushkin@gmail.com])
AM_INIT_AUTOMAKE([foreign subdir-objects])
LT_INIT
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Check pkg-config.
PKG_CHECK_MODULES([GTK], [gtk+-3.0], [],
		  [AC_MSG_ERROR([gtk3 package not found])])

# Check for programs after packages are checked.
AC_CHECK_PROG([GLIB_COMPILE_RESOURCES], [glib-compile-resources],
	      [glib-compile-resources],
	      [AC_MSG_ERROR([executable glib-compile-resources not found])])

# Checks for libraries.
AC_CHECK_LIB([:libjack.so.0], [jack_client_open], [],
	     [AC_MSG_ERROR([jack library not found])])

# Checks for header files.
AC_CHECK_HEADERS([jack/jack.h], [], [AC_MSG_ERROR([jack headers not found])])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([atexit])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
