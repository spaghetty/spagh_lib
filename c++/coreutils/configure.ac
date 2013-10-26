#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([coreutils++], [0.01], [spaghettyATgmail.com])
m4_include([config/boost_base.m4])
m4_include([config/boost_programoptions.m4])
AM_INIT_AUTOMAKE([foreign])

AC_CONFIG_SRCDIR([src/tail.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AX_BOOST_BASE(1.53)
AX_BOOST_PROGRAM_OPTIONS

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES([
	Makefile
	src/Makefile
])

AC_OUTPUT
