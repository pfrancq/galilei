PROJECT(stemming)
INCLUDE_DIRECTORIES(.)

SET(SRC gstemming.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(stemming SHARED ${SRC})
    ADD_SOURCE_FILE(stemming frontend stemming_INCLUDES)
    ADD_FRONTEND(stemming SHARED 2 2 2.0.0 lib "r/galilei" stemming_INCLUDES)
    SET_TARGET_PROPERTIES(stemming PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(stemming galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(stemming SHARED ${SRC})
    ADD_SOURCE_FILE(stemming frontend stemming_INCLUDES)
    ADD_FRONTEND(stemming SHARED 2 2 2.0.0 lib "r/galilei" stemming_INCLUDES)
    TARGET_LINK_LIBRARIES(stemming galilei roptimization rcore rmath dl)
ENDIF(WIN32)
