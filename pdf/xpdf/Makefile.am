INCLUDES = $(all_includes) -I$(top_srcdir) -I.. -I../goo -I../fofi

AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =

noinst_LTLIBRARIES =  libxpdf.la
libxpdf_la_LDFLAGS = -module -avoid-version
libxpdf_la_SOURCES =	Array.cc BuiltinFont.cc BuiltinFontTables.cc CMap.cc Catalog.cc CharCodeToUnicode.cc \
			Decrypt.cc Dict.cc Error.cc FontEncodingTables.cc Function.cc Gfx.cc GfxFont.cc 	GfxState.cc GlobalParams.cc \
			Lexer.cc Link.cc NameToCharCode.cc Object.cc Outline.cc OutputDev.cc PDFDoc.cc \
			PDFDocEncoding.cc Page.cc Parser.cc SFont.cc Stream.cc UnicodeMap.cc \
			Annot.cc PSTokenizer.cc XRef.cc UnicodeTypeTable.cc SecurityHandler.cc JPXStream.cc JBIG2Stream.cc JArithmeticDecoder.cc

EXTRA_DIST= Array.h BuiltinFont.h BuiltinFontTables.h CMap.h Catalog.h CharCodeToUnicode.h Decrypt.h \
			Dict.h Error.h FontEncodingTables.h FontFile.h Function.h Gfx.h GfxFont.h GfxState.h GlobalParams.h \
			Lexer.h Link.h NameToCharCode.h Object.h Outline.h OutputDev.h PDFDoc.h PDFDocEncoding.h \
			Page.h Parser.h SFont.h Stream.h UnicodeMap.h Annot.h PSTokenizer.h XRef.h CharTypes.h \
			config.h CompactFontTables.h NameToUnicodeTable.h UnicodeMapTables.h DisplayFontTable.h UTF8.h \
			Stream-CCITT.h ErrorCodes.h UnicodeTypeTable.h PDFCore.h SecurityHandler.h JPXStream.h JBIG2Stream.h JArithmeticDecoder.h


