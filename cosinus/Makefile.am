INCLUDES = $(all_includes) -I$(top_srcdir)
lib_LTLIBRARIES =  libprofilessimscosinus.la \
		libprofilesagree.la \
		libprofilesdisagree.la \
		libprofilesdocssimscosinus.la \
		libgroupsdocssimscosinus.la \
		libprofilesgroupssimscosinus.la     \
		libdocsagree.la \
		libdocsdisagree.la \
		libdocssimscosinus.la

libprofilessimscosinus_la_LDFLAGS = -module -avoid-version
libprofilessimscosinus_la_SOURCES = ggenericsims.cpp gsubprofilessims.cpp
libprofilessimscosinus_la_LIBADD=

libprofilesagree_la_LDFLAGS = -module -avoid-version
libprofilesagree_la_SOURCES = gprofilesagree.cpp
libprofilesagree_la_LIBADD=

libprofilesdisagree_la_LDFLAGS = -module -avoid-version
libprofilesdisagree_la_SOURCES = gprofilesdisagree.cpp
libprofilesdisagree_la_LIBADD=

libdocssimscosinus_la_LDFLAGS = -module -avoid-version
libdocssimscosinus_la_SOURCES = ggenericsims.cpp gdocssims.cpp
libdocssimscosinus_la_LIBADD=

libdocsagree_la_LDFLAGS = -module -avoid-version
libdocsagree_la_SOURCES = gdocsagree.cpp
libdocsagree_la_LIBADD=

libdocsdisagree_la_LDFLAGS = -module -avoid-version
libdocsdisagree_la_SOURCES = gdocsdisagree.cpp
libdosdisagree_la_LIBADD=

libprofilesdocssimscosinus_la_LDFLAGS = -module -avoid-version
libprofilesdocssimscosinus_la_SOURCES =  ggenericsims.cpp gsubprofilesdocssims.cpp
libprofilesdocssimscosinus_la_LIBADD=

libgroupsdocssimscosinus_la_LDFLAGS = -module -avoid-version
libgroupsdocssimscosinus_la_SOURCES = ggenericsims.cpp ggroupsdocssims.cpp
libgroupsdocssimscosinus_la_LIBADD=

libprofilesgroupssimscosinus_la_LDFLAGS = -module -avoid-version
libprofilesgroupssimscosinus_la_SOURCES = ggenericsims.cpp gsubprofilesgroupssims.cpp
libprofilesgroupssimscosinus_la_LIBADD=

SUBDIRS = $(FRONTEND)


