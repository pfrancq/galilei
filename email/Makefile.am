INCLUDES = $(all_includes) -I$(top_srcdir) -I$(R_LIB) -I$(GALILEI_LIB) -I$(QTDIR)/include
lib_LTLIBRARIES =  ../libemail.la ../libemail_dlg.la
___libemail_la_LDFLAGS = -module -avoid-version
___libemail_la_SOURCES = gfilteremail.cpp
___libemail_la_LIBADD=$(GALILEI_LIB)/filters/libgalilei_filters.la
___libemail_dlg_la_LDFLAGS = -module -avoid-version
___libemail_dlg_la_SOURCES = dlgconfig_qt.ui gfilteremail_kde.cpp
___libemail_dlg_la_LIBADD=$(GALILEI_LIB)/filters/libgalilei_filters.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =

