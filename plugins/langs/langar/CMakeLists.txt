PROJECT(langar)
INCLUDE_DIRECTORIES(.)

SET(SRC langar.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(langar SHARED ${SRC})
    ADD_SOURCE_FILE(langar frontend langar_INCLUDES)
    ADD_FRONTEND(langar SHARED 2 2 2.0.0 lib "r/galilei" langar_INCLUDES)
    SET_TARGET_PROPERTIES(langar PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langar galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(langar SHARED ${SRC})
    ADD_SOURCE_FILE(langar frontend langar_INCLUDES)
    ADD_FRONTEND(langar SHARED 2 2 2.0.0 lib "r/galilei" langar_INCLUDES)
    TARGET_LINK_LIBRARIES(langar galilei roptimization rcore rmath dl)
ENDIF(WIN32)
