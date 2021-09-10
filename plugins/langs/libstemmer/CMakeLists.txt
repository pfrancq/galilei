PROJECT(libstemmer)
INCLUDE_DIRECTORIES(.)

SET(SRC
    src_c/stem_UTF_8_danish.c 
    src_c/stem_UTF_8_dutch.c 
    src_c/stem_UTF_8_english.c 
    src_c/stem_UTF_8_finnish.c 
    src_c/stem_UTF_8_french.c 
    src_c/stem_UTF_8_german.c 
    src_c/stem_UTF_8_hungarian.c 
    src_c/stem_UTF_8_italian.c 
    src_c/stem_UTF_8_norwegian.c 
    src_c/stem_UTF_8_porter.c 
    src_c/stem_UTF_8_portuguese.c 
    src_c/stem_UTF_8_romanian.c 
    src_c/stem_UTF_8_russian.c 
    src_c/stem_UTF_8_spanish.c 
    src_c/stem_UTF_8_swedish.c 
    src_c/stem_UTF_8_turkish.c 
    runtime/api.c 
    runtime/utilities.c 
    libstemmer/libstemmer_utf8.c    
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(libstemmer STATIC ${SRC})
    SET_TARGET_PROPERTIES(libstemmer PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(libstemmer)
ELSE(WIN32)
    ADD_LIBRARY(libstemmer STATIC ${SRC})
    TARGET_LINK_LIBRARIES(libstemmer dl)
ENDIF(WIN32)
