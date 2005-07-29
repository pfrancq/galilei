####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = kgalileicenter

## INCLUDES were found outside kdevelop specific part

kgalileicenter_SOURCES = qchooseparent.ui qhistorydlg.ui qfilldatabase.ui \
		qcreatedatabase.ui qmixidealconfig.ui qexportmatrixdlg.ui qplugins.ui \
		qsessionoptions.ui qconnectmysql.ui qchoosemap.ui qsommapconfig.ui qsimulationdlg.ui \
		kviewhistory.cpp kviewprg.cpp \
		kgalileicenter_init.cpp kgalileicenter_dlg.cpp kviewstems.cpp 	qsessionprogress.cpp \
	 kviewusers.cpp kviewthgroups.cpp 	kviewprofile.cpp kviewgroups.cpp \
	kviewgroup.cpp kviewdocs.cpp kviewdoc.cpp kviewstats.cpp 	kview.cpp kdoc.cpp \
	kviewsom.cpp ksommap.cpp kviewmetaengine.cpp kgalileicenter.cpp main.cpp

kgalileicenter_LDFLAGS=$(all_libraries) -export-dynamic $(KDE_RPATH)
kgalileicenter_LDADD = $(LIB_R) $(LIB_GALILEI) $(LIBKDE_GALILEI) \
	$(LIB_KFILE) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT) $(LIBSOCKET)

SUBDIRS =

EXTRA_DIST = main.cpp kgalileicenter.cpp kgalileicenter.h kgalileicenterui.rc kgalileicenter.desktop \
	lo32-app-kgalileicenter.png lo16-app-kgalileicenter.png kdoc.cpp kdoc.h kview.cpp kview.h \
	kviewdoc.cpp kviewdoc.h kviewdocs.cpp kviewdocs.h kviewgroup.cpp kviewgroup.h \
	kviewgroups.cpp kviewgroups.h kviewprofile.cpp kviewprofile.h \
	kviewthgroups.cpp kviewthgroups.h kviewusers.cpp kviewusers.h qconnectmysql.ui \
	qsessionprogress.cpp qsessionprogress.h kviewstems.cpp kviewstems.h kviewmetaengine.h\
	kgalileicenter_dlg.cpp kgalileicenter_init.cpp \
	kviewprg.cpp kviewprg.h \
	qmixidealconfig.ui\
	qcreatedatabase.ui qcreatedatabase.ui.h qfilldatabase.ui\
	hi16-app-kgalileicenter.png hi32-app-kgalileicenter.png kviewhistory.cpp kviewhistory.h \
	qhistorydlg.ui kviewstats.h

KDE_ICON = kgalileicenter konquerorHub konquerorAutho

install-data-local:
	$(mkinstalldirs) /usr/share/galilei/sql/
	$(INSTALL_DATA) $(srcdir)/sql/*.sql /usr/share/galilei/sql/

# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

METASOURCES = AUTO

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


