PROJECT(gravitation_topic)
INCLUDE_DIRECTORIES(.)

SET(SRC gravitation_topic.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(gravitation_topic STATIC ${SRC})
    SET_TARGET_PROPERTIES(gravitation_topic PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(gravitation_topic galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(gravitation_topic SHARED ${SRC})
    ADD_SOURCE_FILE(gravitation_topic frontend gravitation_INCLUDES)
    ADD_FRONTEND(gravitation_topic 2 2 2.0.0 lib "r/galilei" gravitation_INCLUDES)
    TARGET_LINK_LIBRARIES(gravitation_topic galilei roptimization rcore rmath dl)
ENDIF(WIN32)
