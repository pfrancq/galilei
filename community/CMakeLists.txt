PROJECT(gravitation_community)
INCLUDE_DIRECTORIES(.)

SET(SRC gravitation_community.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(gravitation_community SHARED ${SRC})
    ADD_SOURCE_FILE(gravitation_community frontend gravitation_INCLUDES)
    ADD_FRONTEND(gravitation_community SHARED 2 2 2.0.0 lib "r/galilei" gravitation_INCLUDES)
    SET_TARGET_PROPERTIES(gravitation_community PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(gravitation_community galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(gravitation_community SHARED ${SRC})
    ADD_SOURCE_FILE(gravitation_community frontend gravitation_INCLUDES)
    ADD_FRONTEND(gravitation_community SHARED 2 2 2.0.0 lib "r/galilei" gravitation_INCLUDES)
    TARGET_LINK_LIBRARIES(gravitation_community galilei roptimization rcore rmath dl)
ENDIF(WIN32)
