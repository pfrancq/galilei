INCLUDES = $(all_includes) -I$(top_srcdir) -I$(R_LIB) -I$(GALILEI_LIB) -I$(QTDIR)/include -I.. -Igoo -Ixps
lib_LTLIBRARIES =  ../libps.la ../libps_dlg.la
___libps_la_LDFLAGS = -module -avoid-version
___libps_la_SOURCES = gfilterps.cpp bundle.cpp ptotdll.cpp
___libps_la_LIBADD=$(GALILEI_LIB)/docs/libgalilei_docs.la
___libps_dlg_la_LDFLAGS = -module -avoid-version
___libps_dlg_la_SOURCES = gfilterps_kde.cpp
___libps_dlg_la_LIBADD=$(GALILEI_LIB)/docs/libgalilei_docs.la
AM_CFLAGS = -DHAVE_CONFIG_H
AM_CXXFLAGS = -DHAVE_CONFIG_H
AM_FFLAGS =
SUBDIRS =

