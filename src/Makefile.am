# set the include path found by configure
INCLUDES= $(all_includes)

bin_PROGRAMS = upgalilei

# the application source, library search path, and link libraries
upgalilei_SOURCES = main.cpp
upgalilei_LDFLAGS = -export-dynamic $(all_libraries) 
upgalilei_LDADD = /usr/lib/libmysqlclient.la $(LIB_R) $(LIB_GALILEI)

EXTRA_DIST = galilei.conf 

install-data-local:
	$(mkinstalldirs) /etc/galilei/
	$(INSTALL_DATA) $(srcdir)/galilei.conf /etc/galilei/galilei.conf

