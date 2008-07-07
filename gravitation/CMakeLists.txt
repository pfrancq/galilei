PROJECT(gravitation)
INCLUDE_DIRECTORIES(.)

SET(SRC gravitation.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(gravitation STATIC ${SRC})
    SET_TARGET_PROPERTIES(gravitation PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(gravitation galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(gravitation SHARED ${SRC})
    ADD_SOURCE_FILE(gravitation frontend gravitation_INCLUDES)
    ADD_FRONTEND(gravitation 2 2 2.0.0 lib "r/galilei" gravitation_INCLUDES)
    TARGET_LINK_LIBRARIES(gravitation galilei roptimization rcore rmath)
ENDIF(WIN32)
