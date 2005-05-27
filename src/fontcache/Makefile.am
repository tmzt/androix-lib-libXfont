if XFONT_FONTCACHE
INCLUDES =                                \
	-I${top_srcdir}/include/X11/fonts
lib_LTLIBRARIES = libfontcache.la
libfontcache_la_CFLAGS = $(FONTCACHEPROTO_CFLAGS)
libfontcache_la_SOURCES = fontcache.c fontcacheint.h fcqueue.h
fontdir = $(includedir)/X11/fonts
endif
