INCLUDES = $(all_includes) -I$(top_srcdir) -I.. -Igoo -Ixpdf
filterslib_LTLIBRARIES =  libpdf.la
libpdf_la_LDFLAGS = -module -avoid-version
libpdf_la_SOURCES = gfilterpdf.cpp \
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

EXTRA_DIST=	goo/GHash.h \
	goo/GList.h \
	goo/GString.h \
	goo/GMutex.h \
	goo/gfile.h \
	goo/gtypes.h \
	goo/gmem.h \
	xpdf/Array.h \
	xpdf/BuiltinFont.h \
	xpdf/BuiltinFontTables.h \
	xpdf/CMap.h \
	xpdf/Catalog.h \
	xpdf/CharCodeToUnicode.h \
	xpdf/Decrypt.h \
	xpdf/Dict.h \
	xpdf/Error.h \
	xpdf/FontEncodingTables.h \
	xpdf/FontFile.h \
	xpdf/Function.h \
	xpdf/Gfx.h \
	xpdf/GfxFont.h \
	xpdf/GfxState.h \
	xpdf/GlobalParams.h \
	xpdf/JBIG2Stream.h \
	xpdf/Lexer.h \
	xpdf/Link.h \
	xpdf/NameToCharCode.h \
	xpdf/Object.h \
	xpdf/Outline.h \
	xpdf/OutputDev.h \
	xpdf/PDFDoc.h \
	xpdf/PDFDocEncoding.h \
	xpdf/Page.h \
	xpdf/Parser.h \
	xpdf/SFont.h \
	xpdf/Stream.h \
	xpdf/T1Font.h \
	xpdf/TTFont.h \
	xpdf/TextOutputDev.h \
	TextBufOutputDev.h \
	xpdf/UnicodeMap.h \
	xpdf/Annot.h \
	xpdf/PSTokenizer.h \
	xpdf/XRef.h \
	xpdf/CharTypes.h \
	xpdf/config.h \
	xpdf/CompactFontTables.h \
	xpdf/NameToUnicodeTable.h \
	xpdf/UnicodeMapTables.h \
	xpdf/DisplayFontTable.h \
	xpdf/UTF8.h \
	xpdf/Stream-CCITT.h \
	xpdf/ErrorCodes.h

___libpdf_la_LIBADD=
AM_CFLAGS = -DHAVE_CONFIG_H
AM_CXXFLAGS = -DHAVE_CONFIG_H
AM_FFLAGS =
SUBDIRS =$(FRONTEND)

