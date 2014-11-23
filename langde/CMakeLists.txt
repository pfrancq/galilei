PROJECT(langde)
INCLUDE_DIRECTORIES(.)

SET(SRC langde.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../libstemmer/include)

IF(WIN32)
    ADD_LIBRARY(langde SHARED ${SRC})
    ADD_SOURCE_FILE(langde frontend langde_INCLUDES)
    ADD_FRONTEND(langde SHARED 2 2 2.0.0 lib "r/galilei" langde_INCLUDES)
    SET_TARGET_PROPERTIES(langde PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langde galilei roptimization rmath rcore libstemmer)
ELSE(WIN32)
    ADD_LIBRARY(langde SHARED ${SRC})
    ADD_SOURCE_FILE(langde frontend langde_INCLUDES)
    ADD_FRONTEND(langde SHARED 2 2 2.0.0 lib "r/galilei" langde_INCLUDES)
    TARGET_LINK_LIBRARIES(langde galilei roptimization rcore rmath dl libstemmer)
ENDIF(WIN32)
