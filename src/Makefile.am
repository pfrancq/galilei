# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project
noinst_HEADERS =

# let automoc handle all of the meta source files (moc)
# METASOURCES = AUTO


#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = upgalilei

# the application source, library search path, and link libraries
upgalilei_SOURCES = main.cpp
upgalilei_LDFLAGS = -export-dynamic $(KDE_RPATH) $(all_libraries) -dlopen self
upgalilei_LDADD = /usr/lib/libmysqlclient.la $(UPGALILEI_ADD) \
	$(LIB_KFILE) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT) $(LIBSOCKET) \
	-lcurl -lltdl

# $(LIB_KFILE) $(LIB_KDEUI) $(LIB_KDECORE) $(LIB_QT) $(LIBSOCKET) $(KDE_RPATH) $(all_libraries)

self:
	touch $@
