INCLUDES = $(all_includes) -I$(top_srcdir) -I$(R_LIB) -I$(GALILEI_LIB) -I$(QTDIR)/include
lib_LTLIBRARIES =  ../libsugs.la ../libsugs_dlg.la
___libsugs_la_LDFLAGS = -module -avoid-version
___libsugs_la_SOURCES = gsugs.cpp
___libsugs_la_LIBADD=$(GALILEI_LIB)/groups/libgalilei_groups.la \
					$(GALILEI_LIB)/docs/libgalilei_docs.la \
					$(GALILEI_LIB)/infos/libgalilei_infos.la \
					$(GALILEI_LIB)/profiles/libgalilei_profiles.la \
					$(GALILEI_LIB)/sessions/libgalilei_sessions.la \
					$(R_LIB)/rstd/librstd.la $(R_LIB)/rprg/librprg.la
___libsugs_dlg_la_LDFLAGS = -module -avoid-version
___libsugs_dlg_la_SOURCES = gsugs_kde.cpp
___libsugs_dlg_la_LIBADD=$(GALILEI_LIB)/groups/libgalilei_groups.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =
