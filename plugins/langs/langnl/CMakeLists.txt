PROJECT(langnl)
INCLUDE_DIRECTORIES(.)

SET(SRC langnl.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(langnl SHARED ${SRC})
    ADD_SOURCE_FILE(langnl frontend langnl_INCLUDES)
    ADD_FRONTEND(langnl SHARED 2 2 2.0.0 lib "r/galilei" langnl_INCLUDES)
    SET_TARGET_PROPERTIES(langnl PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langnl galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(langnl SHARED ${SRC})
    ADD_SOURCE_FILE(langnl frontend langnl_INCLUDES)
    ADD_FRONTEND(langnl SHARED 2 2 2.0.0 lib "r/galilei" langnl_INCLUDES)
    TARGET_LINK_LIBRARIES(langnl galilei roptimization rcore rmath dl)
ENDIF(WIN32)
