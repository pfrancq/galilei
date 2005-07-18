## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS        = foreign

SUBDIRS                 = infos docs profiles groups engines historic sessions . frontend 

CPPFLAGS                = -I$(top_srcdir)/infos -I$(top_srcdir)/docs -I$(top_srcdir)/profiles -I$(top_srcdir)/groups -I$(top_srcdir)/engines -I$(top_srcdir)/historic -I$(top_srcdir)/sessions

#include_HEADERS         = error.h

lib_LTLIBRARIES         = libgalilei.la 
libgalilei_la_SOURCES     = 
libgalilei_la_LDFLAGS     = -no-undefined -version-info 2:0:0
libgalilei_la_LIBADD      = infos/libgalilei_infos.la docs/libgalilei_docs.la profiles/libgalilei_profiles.la \
	groups/libgalilei_groups.la engines/libgalilei_engines.la historic/libgalilei_historic.la sessions/libgalilei_sessions.la
