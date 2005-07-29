INCLUDES = -I$(top_srcdir)  $(all_includes) $(INCLUDES_GALILEI)
noinst_LTLIBRARIES =  libgalilei_engines.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =
libgalilei_engines_la_LIBADD = $(LIB_R)
libgalilei_engines_la_SOURCES = genginedoc.cpp granking.cpp gengine.cpp \
	gmetaengine.cpp gwordoccurs.cpp gindexer.cpp

enginesincludedir = $(includedir)/galilei
enginesinclude_HEADERS = genginedoc.h granking.h gengine.h gmetaengine.h \
	gwordoccurs.h gindexer.h
