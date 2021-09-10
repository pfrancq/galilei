PROJECT(gstringindexer)
INCLUDE_DIRECTORIES(.)

SET(SRC gstringindexer.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(gstringindexer SHARED ${SRC})
    ADD_SOURCE_FILE(gstringindexer frontend gstringindexer_INCLUDES)
    ADD_FRONTEND(gstringindexer SHARED 2 2 2.0.0 lib "r/galilei" gstringindexer_INCLUDES)
    SET_TARGET_PROPERTIES(gstringindexer PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(gstringindexer galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(gstringindexer SHARED ${SRC})
    ADD_SOURCE_FILE(gstringindexer frontend gstringindexer_INCLUDES)
    ADD_FRONTEND(gstringindexer SHARED 2 2 2.0.0 lib "r/galilei" gstringindexer_INCLUDES)
    TARGET_LINK_LIBRARIES(gstringindexer galilei roptimization rcore rmath dl)
ENDIF(WIN32)
