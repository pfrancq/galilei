INCLUDES = -I$(top_srcdir) -I$(R_LIB)
lib_LTLIBRARIES =  libgalilei_sessions.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =
libgalilei_sessions_la_SOURCES = gsession.cpp gsessionprg.cpp  \
		gsessionmysql.cpp gslot.cpp gslotlog.cpp  gparam.cpp gparams.cpp \
		gconfig.cpp gstatscalc.cpp gstatscalcmanager.cpp gmixidealgroups.cpp
sessionsincludedir = $(includedir)/galilei/sessions
sessionsinclude_HEADERS = gsession.h gsessionprg.h  \
		 gsessionmysql.h galilei.h \
		 gslot.h gslotlog.h gparam.h gparams.h \
		 gplugin.h gconfig.h gstatscalc.h gstatscalcmanager.h gmixidealgroups.h
