INCLUDES = $(all_includes) -I$(top_srcdir) -I$(R_LIB) -I$(GALILEI_LIB) -I$(QTDIR)/include
lib_LTLIBRARIES =  ../libmsdoc.la ../libmsdoc_dlg.la
___libmsdoc_la_LDFLAGS = -module -avoid-version
___libmsdoc_la_SOURCES = gfiltermsdoc.cpp
___libmsdoc_la_LIBADD=$(GALILEI_LIB)/filters/libgalilei_filters.la -lwv2

___libmsdoc_dlg_la_LDFLAGS = -module -avoid-version
___libmsdoc_dlg_la_SOURCES = gfiltermsdoc_kde.cpp
___libmsdoc_dlg_la_LIBADD=$(GALILEI_LIB)/filters/libgalilei_filters.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =

