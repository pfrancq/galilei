####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = kgalileicenter

## INCLUDES were found outside kdevelop specific part

kgalileicenter_SOURCES = qlanguages.ui kviewr.cpp kviewprg.cpp qpluginsdlg.cpp kgalileicenter_init.cpp kgalileicenter_dlg.cpp kviewga.cpp kviewstems.cpp qsessionprogress.cpp qdocsoptionsdlg.ui qconnectmysql.ui kviewusers.cpp kviewthgroups.cpp kviewstat.cpp kviewprofile.cpp kviewgroups.cpp kviewgroup.cpp kviewdocs.cpp kviewdoc.cpp kview.cpp kdoc.cpp kgalileicenter.cpp main.cpp 
kgalileicenter_LDADD   = ./galilei/docs/libdocs.a ./galilei/libgalilei.a ./galilei/filters/libfilters.a ./galilei/groups/libgroups.a ./galilei/infos/libinfos.a ./galilei/langs/liblangs.a ./galilei/profiles/libprofiles.a ./galilei/sessions/libsessions.a ./galilei/urlmanagers/liburlmanagers.a ./r/rstd/librstd.a ./r/rio/librio.a ./r/rxml/librxml.a ./r/rtimedate/librtimedate.a ./r/rinter/librinter.a ./r/rmysql/librmysql.a ./qtwidgets/rga/librga.a ./qtwidgets/galilei/libgalilei.a ./r/rga/librga.a ./r/rmath/librmath.a ./r/rgga/librgga.a ./r/rpromethee/librpromethee.a ./qtwidgets/rpromethee/librpromethee.a /usr/lib/libmysqlclient.so  ${GCENTER}/galilei/sessions/libsessions.a  ${GCENTER}/galilei/profiles/libprofiles.a ${GCENTER}/galilei/langs/liblangs.a ${GCENTER}/galilei/docs/libdocs.a ${GCENTER}/galilei/groups/libgroups.a ${GCENTER}/r/rinter/librinter.a ${GCENTER}/r/rxml/librxml.a ${GCENTER}/r/rio/librio.a ${GCENTER}/galilei/infos/libinfos.a ${GCENTER}/qtwidgets/rga/librga.a ${GCENTER}/r/rga/librga.a ${GCENTER}/galilei/filters/libfilters.a ${GCENTER}/r/rmysql/librmysql.a ${GCENTER}/r/rstd/librstd.a ${GCENTER}/r/rtimedate/librtimedate.a ${GCENTER}/r/rpromethee/librpromethee.a $(LIB_KFILE) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT) $(LIBSOCKET)

SUBDIRS = galilei r qtwidgets 

EXTRA_DIST = main.cpp kgalileicenter.cpp kgalileicenter.h kgalileicenterui.rc kgalileicenter.desktop lo32-app-kgalileicenter.png lo16-app-kgalileicenter.png kdoc.cpp kdoc.h kview.cpp kview.h kviewdoc.cpp kviewdoc.h kviewdocs.cpp kviewdocs.h kviewgroup.cpp kviewgroup.h kviewgroups.cpp kviewgroups.h kviewprofile.cpp kviewprofile.h kviewstat.cpp kviewstat.h kviewthgroups.cpp kviewthgroups.h kviewusers.cpp kviewusers.h qconnectmysql.ui qdocsoptionsdlg.ui qsessionprogress.cpp qsessionprogress.h kviewstems.cpp kviewstems.h kviewga.cpp kviewga.h kgalileicenter_dlg.cpp kgalileicenter_init.cpp qpluginsdlg.cpp qpluginsdlg.h kviewprg.cpp kviewprg.h kviewr.cpp kviewr.h qlanguages.ui 

install-data-local:
	$(mkinstalldirs) $$(kde_appsdir)/Applications/
	$(INSTALL_DATA) $(srcdir)/kgalileicenter.desktop $$(kde_appsdir)/Applications/kgalileicenter.desktop
	$(mkinstalldirs) $$(kde_icondir)/locolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/lo32-app-kgalileicenter.png $$(kde_icondir)/locolor/32x32/apps/kgalileicenter.png
	$(mkinstalldirs) $$(kde_icondir)/locolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/lo16-app-kgalileicenter.png $$(kde_icondir)/locolor/16x16/apps/kgalileicenter.png

uninstall-local:
	-rm -f $$(kde_appsdir)/Applications/kgalileicenter.desktop
	-rm -f $$(kde_icondir)/locolor/32x32/apps/kgalileicenter.png
	-rm -f $$(kde_icondir)/locolor/16x16/apps/kgalileicenter.png

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
INCLUDES= $(all_includes)

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

