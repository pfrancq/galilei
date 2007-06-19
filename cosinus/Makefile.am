INCLUDES = $(all_includes) -I$(top_srcdir)
lib_LTLIBRARIES =  libsubprofilessimscosinus.la \
		libprofilesagree.la \
		libprofilesdisagree.la \
		libsubprofilesdocssimscosinus.la \
		libgroupsdocssimscosinus.la \
		libsubprofilesgroupssimscosinus.la

libsubprofilessimscosinus_la_LDFLAGS = -module -avoid-version
libsubprofilessimscosinus_la_SOURCES = gsubprofilessims.cpp
libsubprofilessimscosinus_la_LIBADD=

libprofilesagree_la_LDFLAGS = -module -avoid-version
libprofilesagree_la_SOURCES = gprofilesgeneric.cpp gprofilesagree.cpp
libprofilesagree_la_LIBADD=

libprofilesdisagree_la_LDFLAGS = -module -avoid-version
libprofilesdisagree_la_SOURCES = gprofilesgeneric.cpp gprofilesdisagree.cpp
libprofilesdisagree_la_LIBADD=

libsubprofilesdocssimscosinus_la_LDFLAGS = -module -avoid-version
libsubprofilesdocssimscosinus_la_SOURCES = ggenericsims.cpp gsubprofilesdocssims.cpp
libsubprofilesdocssimscosinus_la_LIBADD=

libgroupsdocssimscosinus_la_LDFLAGS = -module -avoid-version
libgroupsdocssimscosinus_la_SOURCES = ggenericsims.cpp ggroupsdocssims.cpp
libgroupsdocssimscosinus_la_LIBADD=

libsubprofilesgroupssimscosinus_la_LDFLAGS = -module -avoid-version
libsubprofilesgroupssimscosinus_la_SOURCES = ggenericsims.cpp gsubprofilesgroupssims.cpp
libsubprofilesgroupssimscosinus_la_LIBADD=

SUBDIRS = $(FRONTEND)


