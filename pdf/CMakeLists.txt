PROJECT(pdf)
INCLUDE_DIRECTORIES(. goo fofi xpdf)

SET(SRC pdf.cpp
    TextBufOutputDev.cc
    goo/GHash.cc
	goo/GList.cc
	goo/GString.cc
	goo/gfile.cc
	goo/gmem.c
	goo/gmempp.cc
	fofi/FoFiBase.cc
	fofi/FoFiEncodings.cc
	fofi/FoFiTrueType.cc
	fofi/FoFiType1.cc
	fofi/FoFiType1C.cc
    xpdf/Array.cc
    xpdf/BuiltinFont.cc
    xpdf/BuiltinFontTables.cc
    xpdf/CMap.cc
    xpdf/Catalog.cc
    xpdf/CharCodeToUnicode.cc
    xpdf/Decrypt.cc
    xpdf/Dict.cc
    xpdf/Error.cc
    xpdf/FontEncodingTables.cc
    xpdf/Function.cc
    xpdf/Gfx.cc
    xpdf/GfxFont.cc
    xpdf/GfxState.cc
    xpdf/GlobalParams.cc
    xpdf/Lexer.cc
    xpdf/Link.cc
    xpdf/NameToCharCode.cc
    xpdf/Object.cc
    xpdf/Outline.cc
    xpdf/OutputDev.cc
    xpdf/PDFDoc.cc
    xpdf/PDFDocEncoding.cc
    xpdf/Page.cc
    xpdf/Parser.cc
    xpdf/SFont.cc
    xpdf/Stream.cc
    xpdf/UnicodeMap.cc
    xpdf/Annot.cc
    xpdf/PSTokenizer.cc
    xpdf/XRef.cc
    xpdf/UnicodeTypeTable.cc
    xpdf/SecurityHandler.cc
    xpdf/JPXStream.cc
    xpdf/JBIG2Stream.cc
    xpdf/JArithmeticDecoder.cc	
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(pdf STATIC ${SRC})
    SET_TARGET_PROPERTIES(pdf PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(pdf galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(pdf SHARED ${SRC})
    ADD_SOURCE_FILE(pdf frontend pdf_INCLUDES)
    ADD_FRONTEND(pdf 2 2 2.0.0 lib "r/galilei" pdf_INCLUDES)
    TARGET_LINK_LIBRARIES(pdf galilei roptimization rcore rmath)
ENDIF(WIN32)
