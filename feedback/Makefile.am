INCLUDES = $(all_includes) -I$(top_srcdir) -I$(R_LIB) -I$(GALILEI_LIB) -I$(QTDIR)/include
lib_LTLIBRARIES =  ../libfdbk.la ../libfdbk_dlg.la
___libfdbk_la_LDFLAGS = -module -avoid-version
___libfdbk_la_SOURCES = gprofilecalcfeedback.cpp
___libfdbk_la_LIBADD=$(GALILEI_LIB)/profiles/libgalilei_profiles.la
___libfdbk_dlg_la_LDFLAGS = -module -avoid-version
___libfdbk_dlg_la_SOURCES = dlgconfig_qt.ui gprofilecalcfeedback_kde.cpp
___libfdbk_dlg_la_LIBADD=$(GALILEI_LIB)/profiles/libgalilei_profiles.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =

