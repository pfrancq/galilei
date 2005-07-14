INCLUDES = -I$(top_srcdir) $(all_includes) $(INCLUDES_GALILEI)
noinst_LTLIBRARIES =  libgalilei_groups.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =
libgalilei_groups_la_LIBADD = /usr/lib/libmysqlclient.la $(LIB_R)
libgalilei_groups_la_SOURCES = gsubjects.cpp ggroup.cpp ggroupcalc.cpp \
	ggrouping.cpp ggroups.cpp gsubject.cpp gpostgroup.cpp
groupsincludedir = $(includedir)/galilei
groupsinclude_HEADERS = ggroupcalc.h ggroups.h gpostgroup.h gsubject.h ggroup.h \
	ggrouping.h gsubjects.h

