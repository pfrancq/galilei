PROJECT(langsv)
INCLUDE_DIRECTORIES(.)

SET(SRC langsv.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../libstemmer/include)

IF(WIN32)
    ADD_LIBRARY(langsv SHARED ${SRC})
    ADD_SOURCE_FILE(langsv frontend langsv_INCLUDES)
    ADD_FRONTEND(langsv SHARED 2 2 2.0.0 lib "r/galilei" langsv_INCLUDES)
    SET_TARGET_PROPERTIES(langsv PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langsv galilei roptimization rmath rcore libstemmer)
ELSE(WIN32)
    ADD_LIBRARY(langsv SHARED ${SRC})
    ADD_SOURCE_FILE(langsv frontend langsv_INCLUDES)
    ADD_FRONTEND(langsv SHARED 2 2 2.0.0 lib "r/galilei" langsv_INCLUDES)
    TARGET_LINK_LIBRARIES(langsv galilei roptimization rcore rmath dl libstemmer)
ENDIF(WIN32)
