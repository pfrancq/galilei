# set the include path for X, qt and KDE
INCLUDES = -I$(GALILEI_LIB) -I$(R_LIB) -I$(QTWIDGETS_LIB) $(all_includes)

# these are the headers for your project
noinst_HEADERS =

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

KDE_ICON = upgalilei

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = upgalilei

# the application source, library search path, and link libraries
upgalilei_SOURCES = main.cpp
upgalilei_LDFLAGS = $(KDE_RPATH) $(all_libraries)
upgalilei_LDADD = $(QTWIDGETS_LIB)/rpromethee/libkde_rpromethee.la $(QTWIDGETS_LIB)/galilei/libkde_galilei.la $(GALILEI_LIB)/postgroups/libgalilei_postgroups.la /usr/lib/libmysqlclient.la $(GALILEI_LIB)/tests/libgalilei_tests.la $(GALILEI_LIB)/sessions/libgalilei_sessions.la $(GALILEI_LIB)/profiles/libgalilei_profiles.la $(GALILEI_LIB)/langs/libgalilei_langs.la $(GALILEI_LIB)/infos/libgalilei_infos.la $(GALILEI_LIB)/groups/libgalilei_groups.la $(GALILEI_LIB)/historic/libgalilei_historic.la $(GALILEI_LIB)/filters/libgalilei_filters.la $(GALILEI_LIB)/docs/libgalilei_docs.la $(R_LIB)/rgga/librgga.la $(R_LIB)/rpromethee/librpromethee.la $(R_LIB)/rstd/librstd.la $(R_LIB)/rga/librga.la $(R_LIB)/rdb/librdb.la $(LIB_KFILE) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT) $(LIBSOCKET)

