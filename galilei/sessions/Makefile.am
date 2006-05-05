INCLUDES = -I$(top_srcdir) $(all_includes) $(INCLUDES_GALILEI)
noinst_LTLIBRARIES =  libgalilei_sessions.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =
libgalilei_sessions_la_LIBADD = $(LIB_R)
libgalilei_sessions_la_SOURCES = gsession.cpp gsessionprg.cpp gslot.cpp \
		gslotlog.cpp gparam.cpp gparams.cpp gstorage.cpp gconfig.cpp 	gstatscalc.cpp \
	gplugin.cpp gplugin.hh gpluginmanager.cpp gpluginmanager.h 	gpluginmanager.hh \
	gpluginmanagers.cpp gpluginmanagers.h gsignalhandler.cpp galilei.cpp 	gmeasure.cpp \
	ggalileiapp.cpp
sessionsincludedir = $(includedir)/galilei
sessionsinclude_HEADERS = gsession.h gsessionprg.h galilei.h gstorage.h \
	gslot.h gslotlog.h gparam.h gparams.h gplugin.h gconfig.h gstatscalc.h \
	gplugin.hh gpluginmanager.h gpluginmanager.hh gpluginmanagers.h gsignalhandler.h \
	gmeasure.h


noinst_HEADERS = ggalileiapp.h
