PROJECT(langfi)
INCLUDE_DIRECTORIES(.)

SET(SRC langfi.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../libstemmer/include)

IF(WIN32)
    ADD_LIBRARY(langfi SHARED ${SRC})
    ADD_SOURCE_FILE(langfi frontend langfi_INCLUDES)
    ADD_FRONTEND(langfi SHARED 2 2 2.0.0 lib "r/galilei" langfi_INCLUDES)
    SET_TARGET_PROPERTIES(langfi PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langfi galilei roptimization rmath rcore libstemmer)
ELSE(WIN32)
    ADD_LIBRARY(langfi SHARED ${SRC})
    ADD_SOURCE_FILE(langfi frontend langfi_INCLUDES)
    ADD_FRONTEND(langfi SHARED 2 2 2.0.0 lib "r/galilei" langfi_INCLUDES)
    TARGET_LINK_LIBRARIES(langfi galilei roptimization rcore rmath dl libstemmer)
ENDIF(WIN32)
