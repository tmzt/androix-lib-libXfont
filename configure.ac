dnl 
dnl  $Id$
dnl 
dnl  Copyright © 2003 Keith Packard
dnl 
dnl  Permission to use, copy, modify, distribute, and sell this software and its
dnl  documentation for any purpose is hereby granted without fee, provided that
dnl  the above copyright notice appear in all copies and that both that
dnl  copyright notice and this permission notice appear in supporting
dnl  documentation, and that the name of Keith Packard not be used in
dnl  advertising or publicity pertaining to distribution of the software without
dnl  specific, written prior permission.  Keith Packard makes no
dnl  representations about the suitability of this software for any purpose.  It
dnl  is provided "as is" without express or implied warranty.
dnl 
dnl  KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
dnl  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
dnl  EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
dnl  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
dnl  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
dnl  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
dnl  PERFORMANCE OF THIS SOFTWARE.
dnl
dnl Process this file with autoconf to create configure.

AC_PREREQ([2.57])

AC_INIT([libXfont],
	1.4.3,
	[keithp@keithp.com],
	libXfont)
dnl
dnl This is the package version number, not the shared library
dnl version.  This same version number must appear in Xfont.h
dnl Yes, it is a pain to synchronize version numbers.  Unfortunately, it's
dnl not possible to extract the version number here from Xfont.h
dnl
AM_INIT_AUTOMAKE([foreign dist-bzip2])
AM_CONFIG_HEADER(config.h)
AC_CONFIG_HEADERS([include/X11/fonts/fontconf.h])

# Check for progs
AC_PROG_CC
AC_PROG_LIBTOOL
dnl If the first PKG_CHECK_MODULES appears inside a conditional, pkg-config
dnl must first be located explicitly.
PKG_PROG_PKG_CONFIG

# Look for headers
AC_CHECK_HEADERS([endian.h poll.h sys/poll.h])
AC_CHECK_FUNCS([poll])

#
# select libraries to include
#

AC_ARG_ENABLE(freetype, [ --enable-freetype ],[XFONT_FREETYPE=$enableval],[XFONT_FREETYPE=no])
AM_CONDITIONAL(XFONT_FREETYPE, [test x$XFONT_FREETYPE = xyes])
if test x$XFONT_FREETYPE = xyes; then
	AC_DEFINE(XFONT_FREETYPE,1,[Support FreeType fonts])
	AC_ARG_WITH(freetype-config, [  --with-freetype-config=PROG   Use FreeType configuration program PROG], freetype_config=$withval, freetype_config=yes)
	
	if test "$freetype_config" = "yes"; then 
		AC_PATH_PROG(ft_config,freetype-config,no)
		if test "$ft_config" = "no"; then
			AC_MSG_ERROR([You must have freetype installed; see http://www.freetype.org/])
		fi
	else
		ft_config="$freetype_config"
	fi
	
	FREETYPE_CFLAGS="`$ft_config --cflags`"
	FREETYPE_LIBS="`$ft_config --libs`"
	
else
	FREETYPE_CFLAGS=""
	FREETYPE_LIBS=""
fi
AC_SUBST(FREETYPE_LIBS)
AC_SUBST(FREETYPE_CFLAGS)

AC_ARG_ENABLE(fontcache, [ --disable-fontcache ],[XFONT_FONTCACHE=$enableval],[XFONT_FONTCACHE=yes])
AM_CONDITIONAL(XFONT_FONTCACHE, [test "x$XFONT_FONTCACHE" = xyes])
if test "x$XFONT_FONTCACHE" = xyes; then
	AC_DEFINE(XFONT_FONTCACHE,1,[Support the font caching extension])
	PKG_CHECK_MODULES(FONTCACHEPROTO, fontcacheproto)
else
	FONTCACHEPROTO_CFLAGS=""
fi
AC_SUBST(FONTCACHEPROTO_CFLAGS)

AC_ARG_ENABLE(fc, [ --disable-fc ],[XFONT_FC=$enableval],[XFONT_FC=yes])
AM_CONDITIONAL(XFONT_FC, [test "x$XFONT_FC" = xyes])
if test "x$XFONT_FC" = xyes; then
	AC_DEFINE(XFONT_FC,1,[Support the X Font Services Protocol])
fi

AC_ARG_ENABLE(builtins, [ --disable-builtins ], [XFONT_BUILTINS=$enableval], [XFONT_BUILTINS=yes])
AM_CONDITIONAL(XFONT_BUILTINS, [test "x$XFONT_BUILTINS" = xyes ])
if test "x$XFONT_BUILTINS" = xyes; then
	AC_DEFINE(XFONT_BUILTINS,1,[Support built-in fonts])
fi

AC_ARG_ENABLE(bitmap, [ --disable-bitmap ], [XFONT_BITMAP=$enableval], [XFONT_BITMAP=yes])
AM_CONDITIONAL(XFONT_BITMAP, [test "x$XFONT_BITMAP" = xyes ])
if test "x$XFONT_BITMAP" = xyes; then
	AC_DEFINE(XFONT_BITMAP,1,[Support bitmap font files])
fi

AC_ARG_ENABLE(type1, [ --disable-type1 ], [XFONT_TYPE1=$enableval], [XFONT_TYPE1=yes])
AM_CONDITIONAL(XFONT_TYPE1, [test "x$XFONT_TYPE1" = xyes ])
if test "x$XFONT_TYPE1" = xyes; then
	AC_DEFINE(XFONT_TYPE1,1,[Support Type 1 font files])
fi

AC_ARG_ENABLE(speedo, [ --disable-speedo ], [XFONT_SPEEDO=$enableval], [XFONT_SPEEDO=no])
AM_CONDITIONAL(XFONT_SPEEDO, [test "x$XFONT_SPEEDO" = xyes ])
if test "x$XFONT_SPEEDO" = xyes; then
	AC_DEFINE(XFONT_SPEEDO,1,[Support Type 1 font files])
fi

#AC_ARG_ENABLE(poll, [ --disable-poll ], [USE_POLL=$enableval], [USE_POLL=$ac_cv_func_poll])
#AM_CONDITIONAL(USE_POLL, [test "x$USE_POLL" = xyes ])
#if test "x$USE_POLL" = xyes; then
#        AC_DEFINE(USE_POLL, 1, [Use poll instead of select])
#fi

if test "x$XFONT_BUILTINS" = xyes -o "x$XFONT_BITMAP" = xyes -o "x$XFONT_FREETYPE" = xyes; then
	XFONT_FONTFILE=yes
else
	XFONT_FONTFILE=no
fi
AM_CONDITIONAL(XFONT_FONTFILE, [test "x$XFONT_FONTFILE" = xyes])
if test "x$XFONT_FONTFILE" = xyes; then
	AC_DEFINE(XFONT_FONTFILE,,[Support fonts in files])
fi

AC_CHECK_LIB(z, gzopen, [Z_LIBS=-lz
AC_SUBST(Z_LIBS)], AC_MSG_ERROR([*** zlib is required]))

AC_DEFINE(PCFFORMAT,,[Support PCF format fonts])
AC_DEFINE(X_GZIP_FONT_COMPRESSION,1,[Support gzip fonts])
X_GZIP_FONT_COMPRESSION=1
AC_SUBST(X_GZIP_FONT_COMPRESSION)

AC_CHECK_LIB(m, hypot, [MATH_LIBS=-lm
AC_SUBST(MATH_LIBS)], AC_MSG_ERROR([*** libm is required]))

PKG_CHECK_MODULES(XFONT, xproto xtrans fontsproto fontenc)

# Transport selection macro from xtrans.m4
XTRANS_CONNECTION_FLAGS

if test "x$GCC" = "xyes"; then
	GCC_WARNINGS="-Wall -Wpointer-arith -Wstrict-prototypes \
	-Wmissing-prototypes -Wmissing-declarations \
	-Wnested-externs -fno-strict-aliasing"
	CFLAGS="$GCC_WARNINGS $CFLAGS"
fi

CFLAGS="$CFLAGS $XFONT_CFLAGS"
LIBS="$LIBS $XFONT_LIBS"

AC_OUTPUT([Makefile
	   src/Makefile
	   src/fontcache/Makefile
	   src/fontfile/Makefile
	   src/FreeType/Makefile
	   src/builtins/Makefile
	   src/bitmap/Makefile
	   src/Type1/Makefile
	   src/Speedo/Makefile
	   src/fc/Makefile
	   src/util/Makefile
	   src/stubs/Makefile
           xfont.pc])
