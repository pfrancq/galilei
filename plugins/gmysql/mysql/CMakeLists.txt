PROJECT(gmysql)
INCLUDE_DIRECTORIES(.)

SET(SRC gmysql.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(gmysql SHARED ${SRC})
    ADD_SOURCE_FILE(gmysql frontend gmysql_INCLUDES)
    ADD_FRONTEND(gmysql SHARED 2 2 2.0.0 lib "r/galilei" gmysql_INCLUDES)
    SET_TARGET_PROPERTIES(gmysql PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(gmysql galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(gmysql SHARED ${SRC})
    ADD_SOURCE_FILE(gmysql frontend gmysql_INCLUDES)
    ADD_FRONTEND(gmysql SHARED 2 2 2.0.0 lib "r/galilei" gmysql_INCLUDES)
    TARGET_LINK_LIBRARIES(gmysql galilei roptimization rcore rmath dl)
ENDIF(WIN32)
