INCLUDES = -I$(top_srcdir)  $(all_includes)
pkglib_LTLIBRARIES =  libgalilei_sessions.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =
libgalilei_sessions_la_SOURCES = gsession.cpp gsessionprg.cpp  \
		gslot.cpp gslotlog.cpp  gparam.cpp gparams.cpp \
		gstorage.cpp gstoragemysql.cpp \
		gconfig.cpp gstatscalc.cpp gstatscalcmanager.cpp gmixidealgroups.cpp
sessionsincludedir = $(includedir)/galilei/sessions
sessionsinclude_HEADERS = gsession.h gsessionprg.h  \
		 galilei.h \
		 gstorage.h gstoragemysql.h \
		 gslot.h gslotlog.h gparam.h gparams.h \
		 gplugin.h gconfig.h gstatscalc.h gstatscalcmanager.h gmixidealgroups.h
