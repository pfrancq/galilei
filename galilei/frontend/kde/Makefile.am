INCLUDES = -I$(top_srcdir) $(all_includes)
pkglib_LTLIBRARIES =  libkde_galilei.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =

SUBDIRS =

libkde_galilei_la_METASOURCES = AUTO

galileiincludedir = $(includedir)/frontend/kde
galileiinclude_HEADERS = qgprofile.h  qgdocxml.h qgsubprofiles.h \
		qlistviewitemtype.h qggroupshistory.h gfiltermanagerkde.h

libkde_galilei_la_SOURCES =  qgprofile.cpp qgdocxml.cpp  \
				qgsubprofiles.cpp qlistviewitemtype.cpp qggroupshistory.cpp \
				gfiltermanagerkde.cpp


minipicsdir = $(kde_icondir)/hicolor/16x16/actions/
minipics_DATA = xml_attribute.png xml_cdata_b.png xml_cdata.png xml_comment_b.png nokonqueror.png \
		xml_comment.png xml_contents_b.png xml_contents.png xml_element_b.png \
		xml_element.png xml_procinstr_b.png xml_procinstr.png xml_text_b.png xml_text.png
