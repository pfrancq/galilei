INCLUDES = $(all_includes) -I$(top_srcdir)
lib_LTLIBRARIES =  libsubprofilessimscosinus.la \
		libprofilesagree.la \
		libprofilesdisagree.la \
		libsubprofilesdocssimscosinus.la \
		libgroupsdocssimscosinus.la \
		libsubprofilesgroupssimscosinus.la     \
		libdocsagree.la \
		libdocsdisagree.la \
		libdocssimscosinus.la

libsubprofilessimscosinus_la_LDFLAGS = -module -avoid-version
libsubprofilessimscosinus_la_SOURCES = gsubprofilessims.cpp
libsubprofilessimscosinus_la_LIBADD=

libprofilesagree_la_LDFLAGS = -module -avoid-version
libprofilesagree_la_SOURCES = gprofilesagree.cpp
libprofilesagree_la_LIBADD=

libprofilesdisagree_la_LDFLAGS = -module -avoid-version
libprofilesdisagree_la_SOURCES = gprofilesdisagree.cpp
libprofilesdisagree_la_LIBADD=

libdocssimscosinus_la_LDFLAGS = -module -avoid-version
libdocssimscosinus_la_SOURCES = gdocssims.cpp
libdocssimscosinus_la_LIBADD=

libdocsagree_la_LDFLAGS = -module -avoid-version
libdocsagree_la_SOURCES = gdocsagree.cpp
libdocsagree_la_LIBADD=

libdocsdisagree_la_LDFLAGS = -module -avoid-version
libdocsdisagree_la_SOURCES = gdocsdisagree.cpp
libdosdisagree_la_LIBADD=

libsubprofilesdocssimscosinus_la_LDFLAGS = -module -avoid-version
libsubprofilesdocssimscosinus_la_SOURCES =  ggenericsims.cpp gsubprofilesdocssims.cpp
libsubprofilesdocssimscosinus_la_LIBADD=

libgroupsdocssimscosinus_la_LDFLAGS = -module -avoid-version
libgroupsdocssimscosinus_la_SOURCES = ggenericsims.cpp ggroupsdocssims.cpp
libgroupsdocssimscosinus_la_LIBADD=

libsubprofilesgroupssimscosinus_la_LDFLAGS = -module -avoid-version
libsubprofilesgroupssimscosinus_la_SOURCES = ggenericsims.cpp gsubprofilesgroupssims.cpp
libsubprofilesgroupssimscosinus_la_LIBADD=

SUBDIRS = $(FRONTEND)


