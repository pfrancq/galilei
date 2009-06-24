PROJECT(langtr)
INCLUDE_DIRECTORIES(.)

SET(SRC langtr.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../libstemmer/include)

IF(WIN32)
    ADD_LIBRARY(langtr STATIC ${SRC})
    SET_TARGET_PROPERTIES(langtr PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langtr galilei roptimization rmath rcore libstemmer)
ELSE(WIN32)
    ADD_LIBRARY(langtr SHARED ${SRC})
    ADD_SOURCE_FILE(langtr frontend langtr_INCLUDES)
    ADD_FRONTEND(langtr 2 2 2.0.0 lib "r/galilei" langtr_INCLUDES)
    TARGET_LINK_LIBRARIES(langtr galilei roptimization rcore rmath dl libstemmer)
ENDIF(WIN32)
