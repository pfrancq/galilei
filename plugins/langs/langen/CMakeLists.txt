PROJECT(langen)
INCLUDE_DIRECTORIES(.)

SET(SRC langen.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(langen SHARED ${SRC})
    ADD_SOURCE_FILE(langen frontend langen_INCLUDES)
    ADD_FRONTEND(langen SHARED 2 2 2.0.0 lib "r/galilei" langen_INCLUDES)
    SET_TARGET_PROPERTIES(langen PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langen galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(langen SHARED ${SRC})
    ADD_SOURCE_FILE(langen frontend langen_INCLUDES)
    ADD_FRONTEND(langen SHARED 2 2 2.0.0 lib "r/galilei" langen_INCLUDES)
    TARGET_LINK_LIBRARIES(langen galilei roptimization rcore rmath dl)
ENDIF(WIN32)
