####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = kgalileicenter

## INCLUDES were found outside kdevelop specific part

kgalileicenter_SOURCES = kviewstorechromos.cpp qhistorydlg.ui kviewhistory.cpp qfilldatabase.ui qcreatedatabase.ui qmixidealconfig.ui qviewchromos.ui kviewquery.cpp kviewchromos.cpp kviewevaluategroup.cpp kviewgroupsevaluation.cpp qlanguages.ui kviewr.cpp kviewprg.cpp qpluginsdlg.cpp kgalileicenter_init.cpp kgalileicenter_dlg.cpp kviewga.cpp kviewstems.cpp qsessionprogress.cpp qdocsoptionsdlg.ui qconnectmysql.ui kviewusers.cpp kviewthgroups.cpp kviewstat.cpp kviewprofile.cpp kviewgroups.cpp kviewgroup.cpp kviewdocs.cpp kviewdoc.cpp kview.cpp kdoc.cpp kgalileicenter.cpp main.cpp
kgalileicenter_LDADD = $(GALILEI_LIB)/postgroups/libgalilei_postgroups.la /usr/lib/libmysqlclient.la $(QWLIB)/galilei/libkde_galilei.la $(QWLIB)/rpromethee/libkde_rpromethee.la $(QWLIB)/rga/libkde_rga.la $(GALILEI_LIB)/tests/libgalilei_tests.la $(GALILEI_LIB)/sessions/libgalilei_sessions.la $(GALILEI_LIB)/profiles/libgalilei_profiles.la $(GALILEI_LIB)/langs/libgalilei_langs.la $(GALILEI_LIB)/infos/libgalilei_infos.la $(GALILEI_LIB)/groups/libgalilei_groups.la $(GALILEI_LIB)/historic/libgalilei_historic.la $(GALILEI_LIB)/filters/libgalilei_filters.la $(GALILEI_LIB)/docs/libgalilei_docs.la $(R_LIB)/rgga/librgga.la $(R_LIB)/rpromethee/librpromethee.la $(R_LIB)/rstd/librstd.la $(R_LIB)/rga/librga.la $(R_LIB)/rdb/librdb.la $(LIB_KFILE) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT) $(LIBSOCKET)

SUBDIRS = 

EXTRA_DIST = main.cpp kgalileicenter.cpp kgalileicenter.h kgalileicenterui.rc kgalileicenter.desktop lo32-app-kgalileicenter.png lo16-app-kgalileicenter.png kdoc.cpp kdoc.h kview.cpp kview.h kviewdoc.cpp kviewdoc.h kviewdocs.cpp kviewdocs.h kviewgroup.cpp kviewgroup.h kviewgroups.cpp kviewgroups.h kviewprofile.cpp kviewprofile.h kviewstat.cpp kviewstat.h kviewthgroups.cpp kviewthgroups.h kviewusers.cpp kviewusers.h qconnectmysql.ui qdocsoptionsdlg.ui qsessionprogress.cpp qsessionprogress.h kviewstems.cpp kviewstems.h kviewga.cpp kviewga.h kgalileicenter_dlg.cpp kgalileicenter_init.cpp qpluginsdlg.cpp qpluginsdlg.h kviewprg.cpp kviewprg.h kviewr.cpp kviewr.h qlanguages.ui kviewgroupsevaluation.cpp kviewgroupsevaluation.h kviewevaluategroup.cpp kviewevaluategroup.h kviewchromos.cpp kviewchromos.h kviewquery.cpp kviewquery.h qviewchromos.ui qmixidealconfig.ui qcreatedatabase.ui qcreatedatabase.ui.h qfilldatabase.ui hi16-app-kgalileicenter.png hi32-app-kgalileicenter.png kviewhistory.cpp kviewhistory.h qhistorydlg.ui kviewstorechromos.cpp kviewstorechromos.h 

install-data-local:
	$(mkinstalldirs) $$(kde_appsdir)/Applications/
	$(INSTALL_DATA) $(srcdir)/kgalileicenter.desktop $$(kde_appsdir)/Applications/kgalileicenter.desktop
	$(mkinstalldirs) $$(kde_icondir)/locolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/lo32-app-kgalileicenter.png $$(kde_icondir)/locolor/32x32/apps/kgalileicenter.png
	$(mkinstalldirs) $$(kde_icondir)/locolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/lo16-app-kgalileicenter.png $$(kde_icondir)/locolor/16x16/apps/kgalileicenter.png
	$(mkinstalldirs) $$(kde_icondir)/hicolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/hi16-app-kgalileicenter.png $$(kde_icondir)/hicolor/16x16/apps/kgalileicenter.png
	$(mkinstalldirs) $$(kde_icondir)/hicolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/hi32-app-kgalileicenter.png $$(kde_icondir)/hicolor/32x32/apps/kgalileicenter.png

uninstall-local:
	-rm -f $$(kde_appsdir)/Applications/kgalileicenter.desktop
	-rm -f $$(kde_icondir)/locolor/32x32/apps/kgalileicenter.png
	-rm -f $$(kde_icondir)/locolor/16x16/apps/kgalileicenter.png
	-rm -f $$(kde_icondir)/hicolor/16x16/apps/kgalileicenter.png
	-rm -f $$(kde_icondir)/hicolor/32x32/apps/kgalileicenter.png

####### kdevelop will overwrite this part!!! (end)############
# this 10 paths are KDE specific. Use them:
# kde_htmldir       Where your docs should go to. (contains lang subdirs)
# kde_appsdir       Where your application file (.kdelnk) should go to. 
# kde_icondir       Where your icon should go to.
# kde_minidir       Where your mini icon should go to.
# kde_datadir       Where you install application data. (Use a subdir)
# kde_locale        Where translation files should go to.(contains lang subdirs)
# kde_cgidir        Where cgi-bin executables should go to.
# kde_confdir       Where config files should go to.
# kde_mimedir       Where mimetypes should go to.
# kde_toolbardir    Where general toolbar icons should go to.
# kde_wallpaperdir  Where general wallpapers should go to.

# set the include path for X, qt and KDE
INCLUDES = -I$(GALILEI_LIB) -I$(R_LIB) -I$(QWLIB) $(all_includes)

METASOURCES = AUTO

# the library search path. 
kgalileicenter_LDFLAGS = $(all_libraries) $(KDE_RPATH)

rcdir = $(kde_datadir)/kgalileicenter
rc_DATA = kgalileicenterui.rc

messages: rc.cpp
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) $$LIST -o $(podir)/kgalileicenter.pot; \
	fi

AM_CFLAGS = 
AM_CXXFLAGS = 
AM_FFLAGS = 
