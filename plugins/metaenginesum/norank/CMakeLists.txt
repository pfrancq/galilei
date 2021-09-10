PROJECT(norank)
INCLUDE_DIRECTORIES(.)

SET(SRC gnorank.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(norank SHARED ${SRC})
    ADD_SOURCE_FILE(norank frontend norank_INCLUDES)
    ADD_FRONTEND(norank SHARED 2 2 2.0.0 lib "r/galilei" norank_INCLUDES)
    SET_TARGET_PROPERTIES(norank PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(norank galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(norank SHARED ${SRC})
    ADD_SOURCE_FILE(norank frontend norank_INCLUDES)
    ADD_FRONTEND(norank SHARED 2 2 2.0.0 lib "r/galilei" norank_INCLUDES)
    TARGET_LINK_LIBRARIES(norank galilei roptimization rcore rmath dl)
ENDIF(WIN32)
