INCLUDES = $(all_includes) -I$(top_srcdir) -I$(R_LIB) -I$(GALILEI_LIB) -I$(QTDIR)/include
lib_LTLIBRARIES =  ../libgravitation.la ../libgravitation_dlg.la
___libgravitation_la_LDFLAGS = -module -avoid-version
___libgravitation_la_SOURCES = ggroupcalcgravitation.cpp
___libgravitation_la_LIBADD=$(GALILEI_LIB)/groups/libgalilei_groups.la
___libgravitation_dlg_la_LDFLAGS = -module -avoid-version
___libgravitation_dlg_la_SOURCES = dlgconfig_qt.ui ggroupcalcgravitation_kde.cpp
___libgravitation_dlg_la_LIBADD=$(GALILEI_LIB)/groups/libgalilei_groups.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =

