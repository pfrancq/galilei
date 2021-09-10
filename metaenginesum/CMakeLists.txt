PROJECT(metaenginesum)
INCLUDE_DIRECTORIES(.)

SET(SRC gmetaenginesum.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(metaenginesum SHARED ${SRC})
    ADD_SOURCE_FILE(metaenginesum frontend metaenginesum_INCLUDES)
    ADD_FRONTEND(metaenginesum SHARED 2 2 2.0.0 lib "r/galilei" metaenginesum_INCLUDES)
    SET_TARGET_PROPERTIES(metaenginesum PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(metaenginesum galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(metaenginesum SHARED ${SRC})
    ADD_SOURCE_FILE(metaenginesum frontend metaenginesum_INCLUDES)
    ADD_FRONTEND(metaenginesum SHARED 2 2 2.0.0 lib "r/galilei" metaenginesum_INCLUDES)
    TARGET_LINK_LIBRARIES(metaenginesum galilei roptimization rcore rmath dl)
ENDIF(WIN32)
