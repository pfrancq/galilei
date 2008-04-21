INCLUDES = -I$(top_srcdir) $(all_includes) $(INCLUDES_GALILEI)

AUTOMAKE_OPTIONS        = foreign

SUBDIRS =

lib_LTLIBRARIES         = libgalileikde.la

libgalileikde_la_METASOURCES = AUTO

galileikdeincludedir = $(includedir)/galilei
galileikdeinclude_HEADERS = qgprofile.h  qgdocxml.h qgmeasure2elementsdlg.h \
		qlistviewitemtype.h qggroupshistory.h
libgalileikde_la_LIBADD = ../../libgalilei.la $(LIB_KIO) $(LIBKDE_R)
libgalileikde_la_LDFLAGS     = -no-undefined -version-info 2:0:0
libgalileikde_la_SOURCES =  qgprofile.cpp qgdocxml.cpp  qgmeasure2elementsdlg.cpp dlgmeasure2elements.ui \
				qlistviewitemtype.cpp qggroupshistory.cpp


minipicsdir = $(kde_icondir)/hicolor/16x16/actions/
minipics_DATA = xml_attribute.png xml_cdata_b.png xml_cdata.png xml_comment_b.png nokonqueror.png \
		xml_comment.png xml_contents_b.png xml_contents.png xml_element_b.png \
		xml_element.png xml_procinstr_b.png xml_procinstr.png xml_text_b.png xml_text.png
