PROJECT(langno)
INCLUDE_DIRECTORIES(.)

SET(SRC langno.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../libstemmer/include)

IF(WIN32)
    ADD_LIBRARY(langno SHARED ${SRC})
    ADD_SOURCE_FILE(langno frontend langno_INCLUDES)
    ADD_FRONTEND(langno SHARED 2 2 2.0.0 lib "r/galilei" langno_INCLUDES)
    SET_TARGET_PROPERTIES(langno PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langno galilei roptimization rmath rcore libstemmer)
ELSE(WIN32)
    ADD_LIBRARY(langno SHARED ${SRC})
    ADD_SOURCE_FILE(langno frontend langno_INCLUDES)
    ADD_FRONTEND(langno SHARED 2 2 2.0.0 lib "r/galilei" langno_INCLUDES)
    TARGET_LINK_LIBRARIES(langno galilei roptimization rcore rmath dl libstemmer)
ENDIF(WIN32)
