dnl Process this file with autoconf to produce a configure script.
dnl
dnl This file is free software; as a special exception the author gives
dnl unlimited permission to copy and/or distribute it, with or without
dnl modifications, as long as this notice is preserved.
dnl
dnl This program is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
dnl implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

AC_INIT([scoreboard], [0.2.1], [scoreboard@naver.com], [scoreboard],
	[http://github.com/unipro/scoreboard])

dnl Must come before AM_INIT_AUTOMAKE.
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror])

dnl Minimum Autoconf version required.
AC_PREREQ(2.60)

dnl Where to generate output; srcdir location.
AC_CONFIG_HEADERS([config.h:config.in])
AC_CONFIG_SRCDIR([wx/main.cpp])

dnl Checks for programs.
AC_ISC_POSIX
AM_PROG_AR
AC_PROG_CXX
AC_PROG_RANLIB

dnl Determine the operating system
AC_MSG_CHECKING([uname -s for detecting host operating system])
OSNAME=`uname -s`
case "$OSNAME" in
     MINGW*)
        dnl MinGW
        AC_MSG_RESULT(MinGW)
	win32=true
        ;;
     *)
	AC_MSG_RESULT(*NIX)
	win32=false
        ;;
esac
AM_CONDITIONAL([HAVE_WINDRES], [test x$win32 = xtrue])


dnl Defines C++ compile flags
CXXFLAGS="$CXXFLAGS -Wall -Wundef -Wno-ctor-dtor-privacy -g -O0"

AC_CHECK_PROG(wxconfig,wx-config,`which wx-config`)
if test x$wxconfig = x ; then
   AC_MSG_ERROR([
	Cannot find wx-config tool.
   	Are development packages of wxWidgets installed?
	])
fi
CPPFLAGS="$CPPFLAGS `wx-config --cxxflags`"
LIBS="$LIBS `wx-config --libs`"

dnl Checks for SDL
AC_CHECK_PROG(sdlconfig,sdl2-config,`which sdl2-config`)
if test x$sdlconfig = x ; then
   AC_MSG_ERROR([[Cannot find the sdl-config tool. Are development packages of SDL installed?]])
fi
CPPFLAGS="$CPPFLAGS `sdl2-config --cflags`"
LIBS="$LIBS `sdl2-config --libs`"

dnl Output
AC_CONFIG_FILES([Makefile CppUnitLite/Makefile model/Makefile test/Makefile wx/Makefile])
AC_OUTPUT
