INCLUDES = $(all_includes) -I$(top_srcdir) -I.. -Igoo -Ixpdf
filterslib_LTLIBRARIES =  ../libpdf.la
___libpdf_la_LDFLAGS = -module -avoid-version
___libpdf_la_SOURCES = gfilterpdf.cpp \
	goo/GHash.cc \
	goo/GList.cc \
	goo/GString.cc \
	goo/gfile.cc \
	goo/gmem.c \
	goo/gmempp.cc \
	xpdf/Array.cc \
	xpdf/BuiltinFont.cc \
	xpdf/BuiltinFontTables.cc \
	xpdf/CMap.cc \
	xpdf/Catalog.cc \
	xpdf/CharCodeToUnicode.cc \
	xpdf/Decrypt.cc \
	xpdf/Dict.cc \
	xpdf/Error.cc \
	xpdf/FontEncodingTables.cc \
	xpdf/FontFile.cc \
	xpdf/Function.cc \
	xpdf/Gfx.cc \
	xpdf/GfxFont.cc \
	xpdf/GfxState.cc \
	xpdf/GlobalParams.cc \
	xpdf/JBIG2Stream.cc \
	xpdf/Lexer.cc \
	xpdf/Link.cc \
	xpdf/NameToCharCode.cc \
	xpdf/Object.cc \
	xpdf/Outline.cc \
	xpdf/OutputDev.cc \
	xpdf/PDFDoc.cc \
	xpdf/PDFDocEncoding.cc \
	xpdf/Page.cc \
	xpdf/Parser.cc \
	xpdf/SFont.cc \
	xpdf/Stream.cc \
	xpdf/T1Font.cc \
	xpdf/TTFont.cc \
	xpdf/TextOutputDev.cc \
	TextBufOutputDev.cc \
	xpdf/UnicodeMap.cc \
	xpdf/Annot.cc \
	xpdf/PSTokenizer.cc \
	xpdf/XRef.cc
___libpdf_la_LIBADD=
AM_CFLAGS = -DHAVE_CONFIG_H
AM_CXXFLAGS = -DHAVE_CONFIG_H
AM_FFLAGS =
SUBDIRS =$(FRONTEND)

