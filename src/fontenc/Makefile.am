FONTDIR=${libdir}/X11/fonts
FONTENCDEFS = -DFONT_ENCODINGS_DIRECTORY=\"$(FONTDIR)/encodings/encodings.dir\"

INCLUDES =			\
	-I${top_srcdir}/include/X11/fonts \
	$(FONTENCDEFS)		

lib_LTLIBRARIES = libfontenc.la

libfontenc_la_SOURCES =					\
	$(top_srcdir)/src/fontfile/encparse.c		\
	$(top_srcdir)/src/fontfile/fontenc.c

libfontenc_la_LIBADD = @X_LIBS@
