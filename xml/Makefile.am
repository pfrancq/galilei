INCLUDES = $(all_includes) -I$(top_srcdir) -I$(R_LIB) -I$(GALILEI_LIB) -I$(QTDIR)/include
lib_LTLIBRARIES =  ../libxml.la ../libxml_dlg.la
___libxml_la_LDFLAGS = -module -avoid-version
___libxml_la_SOURCES = gfilterxml.cpp codetochar.cpp
___libxml_la_LIBADD=$(GALILEI_LIB)/filters/libgalilei_filters.la
___libxml_dlg_la_LDFLAGS = -module -avoid-version
___libxml_dlg_la_SOURCES = gfilterxml_kde.cpp
___libxml_dlg_la_LIBADD=$(GALILEI_LIB)/filters/libgalilei_filters.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =

