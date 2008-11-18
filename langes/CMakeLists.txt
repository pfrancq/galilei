PROJECT(langes)
INCLUDE_DIRECTORIES(.)

SET(SRC langes.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../Stemming_Library)

IF(WIN32)
    ADD_LIBRARY(langes STATIC ${SRC})
    SET_TARGET_PROPERTIES(langes PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langes galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(langes SHARED ${SRC})
    ADD_SOURCE_FILE(langes frontend langes_INCLUDES)
    ADD_FRONTEND(langes 2 2 2.0.0 lib "r/galilei" langes_INCLUDES)
    TARGET_LINK_LIBRARIES(langes galilei roptimization rcore rmath dl)
ENDIF(WIN32)
