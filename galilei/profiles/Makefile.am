INCLUDES = -I$(top_srcdir) $(all_includes) $(INCLUDES_GALILEI)
noinst_LTLIBRARIES =  libgalilei_profiles.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =
libgalilei_profiles_la_LIBADD = /usr/lib/libmysqlclient.la $(LIB_R)
libgalilei_profiles_la_SOURCES = gprofilecalc.cpp  \
	gsubprofile.cpp gusers.cpp gprofile.cpp gpostprofile.cpp  guser.cpp \
	gpreprofile.cpp
profilesincludedir = $(includedir)/galilei
profilesinclude_HEADERS = gprofilecalc.h gusers.h gprofile.h \
	 gsubprofile.h guser.h gsubprofileproxy.h \
	gsubprofileproxymem.h gprofileproxy.h gprofileproxymem.h gpostprofile.h gpreprofile.h

