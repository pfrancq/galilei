INCLUDES = -I$(top_srcdir) $(all_includes) $(INCLUDES_GALILEI)
noinst_LTLIBRARIES =  libgalilei_infos.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =
libgalilei_infos_la_LIBADD = /usr/lib/libmysqlclient.la $(LIB_R)
libgalilei_infos_la_SOURCES = ginfo.cpp gweightinfos.cpp gweightinfo.cpp \
	gdata.cpp gdict.cpp glang.cpp gword.cpp gwordlist.cpp ginfolist.cpp
infosincludedir = $(includedir)/galilei
infosinclude_HEADERS = ginfo.h gweightinfos.h gweightinfo.h gdata.h gdict.h \
	glang.h gword.h gwordlist.h ginfolist.h glangproxy.h glangproxymem.h
