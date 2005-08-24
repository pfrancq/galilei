INCLUDES = -I$(top_srcdir)  $(all_includes) $(INCLUDES_GALILEI)
noinst_LTLIBRARIES =  libgalilei_docs.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =
libgalilei_docs_la_LIBADD = $(LIB_R)
libgalilei_docs_la_SOURCES = gbalancedlinks.cpp gdoc.cpp gsugs.cpp \
	glinkcalc.cpp glink.cpp gdocanalyse.cpp gdocxml.cpp glinks.cpp gpostdoc.cpp \
	gfilter.cpp gfiltermanagercurl.cpp

docsincludedir = $(includedir)/galilei
docsinclude_HEADERS = gbalancedlinks.h gdocanalyse.h gdocxml.h glinkcalc.h \
	glinks.h gdoc.h gsugs.h glink.h gpostdoc.h gfilter.h gfiltermanagercurl.h
