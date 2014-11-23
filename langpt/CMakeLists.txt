PROJECT(langpt)
INCLUDE_DIRECTORIES(.)

SET(SRC langpt.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../libstemmer/include)

IF(WIN32)
    ADD_LIBRARY(langpt SHARED ${SRC})
    ADD_SOURCE_FILE(langpt frontend langpt_INCLUDES)
    ADD_FRONTEND(langpt SHARED 2 2 2.0.0 lib "r/galilei" langpt_INCLUDES)
    SET_TARGET_PROPERTIES(langpt PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langpt galilei roptimization rmath rcore libstemmer)
ELSE(WIN32)
    ADD_LIBRARY(langpt SHARED ${SRC})
    ADD_SOURCE_FILE(langpt frontend langpt_INCLUDES)
    ADD_FRONTEND(langpt SHARED 2 2 2.0.0 lib "r/galilei" langpt_INCLUDES)
    TARGET_LINK_LIBRARIES(langpt galilei roptimization rcore rmath dl libstemmer)
ENDIF(WIN32)
