# 
#  $Id$
# 
#  Copyright © 2003 Keith Packard, Noah Levitt
# 
#  Permission to use, copy, modify, distribute, and sell this software and its
#  documentation for any purpose is hereby granted without fee, provided that
#  the above copyright notice appear in all copies and that both that
#  copyright notice and this permission notice appear in supporting
#  documentation, and that the name of Keith Packard not be used in
#  advertising or publicity pertaining to distribution of the software without
#  specific, written prior permission.  Keith Packard makes no
#  representations about the suitability of this software for any purpose.  It
#  is provided "as is" without express or implied warranty.
# 
#  KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
#  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
#  EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
#  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
#  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
#  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
#  PERFORMANCE OF THIS SOFTWARE.

lib_LTLIBRARIES = libXfont.la

if XFONT_FONTCACHE
FONTCACHE_DIR = src/fontcache
FONTCACHE_LIB = src/fontcache/libfontcache.la
endif

if XFONT_FONTFILE
FONTFILE_DIR = src/fontfile
FONTFILE_LIB = src/fontfile/libfontfile.la
endif

if XFONT_FREETYPE
FREETYPE_DIR = src/FreeType
FREETYPE_LIB = src/FreeType/libft.la
endif

if XFONT_BITMAP
BITMAP_DIR = src/bitmap
BITMAP_LIB = src/bitmap/libbitmap.la
endif

if XFONT_BUILTINS
BUILTINS_DIR = src/builtins
BUILTINS_LIB = src/builtins/libbuiltins.la
endif

if XFONT_FC
FC_DIR = src/fc
FC_LIB = src/fc/libfc.la
endif

if XFONT_TYPE1
TYPE1_DIR = src/Type1
TYPE1_LIB = src/Type1/libtype1.la
endif

if XFONT_SPEEDO
SPEEDO_DIR = src/Speedo
SPEEDO_LIB = src/Speedo/libspeedo.la
endif

UTIL_DIR = src/util
UTIL_LIB = src/util/libutil.la

STUBS_LIB = src/stubs/libstubs.la
STUBS_DIR = src/stubs

SUBDIRS=$(FONTFILE_DIR) $(FREETYPE_DIR) $(BITMAP_DIR) \
	$(BUILTINS_DIR) $(FC_DIR) $(UTIL_DIR) $(STUBS_DIR) $(FONTCACHE_DIR) \
	$(TYPE1_DIR) $(SPEEDO_DIR)

libXfont_la_LIBADD = $(FREETYPE_LIB) $(BITMAP_LIB) $(BUILTINS_LIB) \
	$(FC_LIB) $(FONTFILE_LIB) $(UTIL_LIB) $(STUBS_LIB) \
	$(FREETYPE_LIBS) $(Z_LIBS) $(MATH_LIBS) $(FONTCACHE_LIB) \
	$(TYPE_1_LIB) $(SPEEDO_LIB)

libXfont_la_SOURCES =

#
# Shared library version info.  This is not the same as the package version
#
libXfont_la_LDFLAGS = -version-number 1:4:1

libXfontincludedir = $(includedir)/X11/fonts
libXfontinclude_HEADERS = \
	include/X11/fonts/bitmap.h   \
	include/X11/fonts/bufio.h    \
	include/X11/fonts/fntfil.h   \
	include/X11/fonts/fntfilio.h \
	include/X11/fonts/fntfilst.h \
	include/X11/fonts/fontconf.h \
	include/X11/fonts/fontencc.h \
	include/X11/fonts/fontmisc.h \
	include/X11/fonts/fontmod.h  \
	include/X11/fonts/fontshow.h \
	include/X11/fonts/fontutil.h \
	include/X11/fonts/fontxlfd.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xfont.pc

EXTRA_DIST = xfont.pc.in autogen.sh include/X11/fonts/fontconf.h.in
