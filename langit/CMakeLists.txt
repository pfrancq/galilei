PROJECT(langit)
INCLUDE_DIRECTORIES(.)

SET(SRC langit.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../libstemmer/include)

IF(WIN32)
    ADD_LIBRARY(langit STATIC ${SRC})
    SET_TARGET_PROPERTIES(langit PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langit galilei roptimization rmath rcore libstemmer)
ELSE(WIN32)
    ADD_LIBRARY(langit SHARED ${SRC})
    ADD_SOURCE_FILE(langit frontend langit_INCLUDES)
    ADD_FRONTEND(langit 2 2 2.0.0 lib "r/galilei" langit_INCLUDES)
    TARGET_LINK_LIBRARIES(langit galilei roptimization rcore rmath dl libstemmer)
ENDIF(WIN32)
