INCLUDES = $(all_includes) -I$(top_srcdir)
filterslib_LTLIBRARIES =  libps.la
libps_la_LDFLAGS = -module -avoid-version
libps_la_SOURCES = gfilterps.cpp bundle.cpp ptotdll.cpp
libps_la_LIBADD=
AM_CFLAGS = -DHAVE_CONFIG_H
AM_CXXFLAGS = -DHAVE_CONFIG_H
AM_FFLAGS =
SUBDIRS =$(FRONTEND)

