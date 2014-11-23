PROJECT(langdk)
INCLUDE_DIRECTORIES(.)

SET(SRC langdk.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../libstemmer/include)

IF(WIN32)
    ADD_LIBRARY(langdk SHARED ${SRC})
    ADD_SOURCE_FILE(langdk frontend langdk_INCLUDES)
    ADD_FRONTEND(langdk SHARED 2 2 2.0.0 lib "r/galilei" langdk_INCLUDES)
    SET_TARGET_PROPERTIES(langdk PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langdk galilei roptimization rmath rcore libstemmer)
ELSE(WIN32)
    ADD_LIBRARY(langdk SHARED ${SRC})
    ADD_SOURCE_FILE(langdk frontend langdk_INCLUDES)
    ADD_FRONTEND(langdk SHARED 2 2 2.0.0 lib "r/galilei" langdk_INCLUDES)
    TARGET_LINK_LIBRARIES(langdk galilei roptimization rcore rmath dl libstemmer)
ENDIF(WIN32)
