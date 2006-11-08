AUTOMAKE_OPTIONS        = foreign

SUBDIRS = goo fofi xpdf . $(FRONTEND)

INCLUDES = $(all_includes) -I$(top_srcdir) -I.. -Igoo -Ixpdf
AM_CFLAGS = -DHAVE_CONFIG_H
AM_CXXFLAGS = -DHAVE_CONFIG_H
AM_FFLAGS =


filterslib_LTLIBRARIES =  libpdf.la
libpdf_la_LDFLAGS = -module -avoid-version
libpdf_la_SOURCES = gfilterpdf.cpp  TextBufOutputDev.cc
libpdf_la_INCLUDES = gfilterpdf.cpp  TextBufOutputDev.cc

EXTRA_DIST= README

libpdf_la_LIBADD= goo/libgoo.la fofi/libfofi.la xpdf/libxpdf.la


