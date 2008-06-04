INCLUDES = $(all_includes) -I$(top_srcdir)
lib_LTLIBRARIES =  libprofilesmssims.la \
		libprofilesagree.la \
		libprofilesdisagree.la \
		libdocsprofilesmssims.la \
		libdocsgroupsmssims.la \
		libprofilesgroupsmssims.la     \
		libdocsagree.la \
		libdocsdisagree.la \
		libdocsmssims.la

libprofilesmssims_la_LDFLAGS = -module -avoid-version
libprofilesmssims_la_SOURCES = ggenericsims.cpp gprofilessims.cpp
libprofilesmssims_la_LIBADD=

libprofilesagree_la_LDFLAGS = -module -avoid-version
libprofilesagree_la_SOURCES = gprofilesagree.cpp
libprofilesagree_la_LIBADD=

libprofilesdisagree_la_LDFLAGS = -module -avoid-version
libprofilesdisagree_la_SOURCES = gprofilesdisagree.cpp
libprofilesdisagree_la_LIBADD=

libdocsmssims_la_LDFLAGS = -module -avoid-version
libdocsmssims_la_SOURCES = ggenericsims.cpp gdocssims.cpp
libdocsmssims_la_LIBADD=

libdocsagree_la_LDFLAGS = -module -avoid-version
libdocsagree_la_SOURCES = gdocsagree.cpp
libdocsagree_la_LIBADD=

libdocsdisagree_la_LDFLAGS = -module -avoid-version
libdocsdisagree_la_SOURCES = gdocsdisagree.cpp
libdocsdisagree_la_LIBADD=

libdocsprofilesmssims_la_LDFLAGS = -module -avoid-version
libdocsprofilesmssims_la_SOURCES =  ggenericsims.cpp gdocsprofilessims.cpp
libdocsprofilesmssims_la_LIBADD=

libdocsgroupsmssims_la_LDFLAGS = -module -avoid-version
libdocsgroupsmssims_la_SOURCES = ggenericsims.cpp gdocsgroupssims.cpp
libdocsgroupsmssims_la_LIBADD=

libprofilesgroupsmssims_la_LDFLAGS = -module -avoid-version
libprofilesgroupsmssims_la_SOURCES = ggenericsims.cpp gprofilesgroupssims.cpp
libprofilesgroupsmssims_la_LIBADD=

SUBDIRS = $(FRONTEND)


