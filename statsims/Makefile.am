INCLUDES = $(all_includes) -I$(top_srcdir) -I$(R_LIB) -I$(GALILEI_LIB) -I$(QTDIR)/include
lib_LTLIBRARIES =  ../libstatssims.la ../libstatssims_dlg.la
___libstatssims_la_LDFLAGS = -module -avoid-version
___libstatssims_la_SOURCES = gstatssims.cpp ggroupevaluatedoc.cpp ggroupsevaluate.cpp \
	gstatprofdoc.cpp gstatsimdocgrp.cpp gstatsimid.cpp gstatsimsubprof.cpp  \
	ggroupevaluate.cpp ggroupevaluatesubprof.cpp gstatsimdoc.cpp \
	gstatsimdocprof.cpp  gstatsimprofgrp.cpp
___libstatssims_la_LIBADD=$(GALILEI_LIB)/groups/libgalilei_groups.la
___libstatssims_dlg_la_LDFLAGS = -module -avoid-version
___libstatssims_dlg_la_SOURCES = dlgconfig_qt.ui gstatssims_kde.cpp
___libstatssims_dlg_la_LIBADD=$(GALILEI_LIB)/groups/libgalilei_groups.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =

