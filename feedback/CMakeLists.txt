PROJECT(feedback)
INCLUDE_DIRECTORIES(.)

SET(SRC feedback.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(feedback SHARED ${SRC})
    ADD_SOURCE_FILE(feedback frontend feedback_INCLUDES)
    ADD_FRONTEND(feedback SHARED 2 2 2.0.0 lib "r/galilei" feedback_INCLUDES)
    SET_TARGET_PROPERTIES(feedback PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(feedback galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(feedback SHARED ${SRC})
    ADD_SOURCE_FILE(feedback frontend feedback_INCLUDES)
    ADD_FRONTEND(feedback SHARED 2 2 2.0.0 lib "r/galilei" feedback_INCLUDES)
    TARGET_LINK_LIBRARIES(feedback galilei roptimization rcore rmath dl)
ENDIF(WIN32)
