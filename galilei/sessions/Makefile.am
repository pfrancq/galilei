INCLUDES = -I$(top_srcdir) $(all_includes) $(INCLUDES_GALILEI)
noinst_LTLIBRARIES =  libgalilei_sessions.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =
libgalilei_sessions_la_LIBADD = $(LIB_R)
libgalilei_sessions_la_SOURCES = gsession.cpp gsessionprg.cpp gslot.cpp \
	gslotlog.cpp gstorage.cpp gstatscalc.cpp gplugin.hh gpluginmanager.cpp \
	gpluginmanager.h gpluginmanager.hh gsignalhandler.cpp galilei.cpp gmeasure.cpp \
	ggalileiapp.cpp glangdata.cpp
sessionsincludedir = $(includedir)/galilei
sessionsinclude_HEADERS = gsession.h gsessionprg.h galilei.h gstorage.h \
	gslot.h gslotlog.h gplugin.h gstatscalc.h \
	gplugin.hh gpluginmanager.h gpluginmanager.hh gsignalhandler.h \
	gmeasure.h ggalileiapp.h glangdata.h gmeasurefile.h gmeasurefile.hh

