INCLUDES = $(all_includes) -I$(top_srcdir)
pkglib_LTLIBRARIES =  libgca.la libgda.la

libgca_la_LDFLAGS = -module -avoid-version
libgca_la_SOURCES = gcagroupprofiles.cpp gcaobj.cpp gcachromo.cpp gcagroup.cpp \
	gcainst.cpp gca.cpp gcaheuristic.cpp gcaprom.cpp
libgca_la_LIBADD=

libgda_la_LDFLAGS = -module -avoid-version
libgda_la_SOURCES = gcagroupdocs.cpp gcaobj.cpp gcachromo.cpp gcagroup.cpp gcainst.cpp gca.cpp gcaheuristic.cpp gcaprom.cpp
libgda_la_LIBADD=

AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS = . frontend

