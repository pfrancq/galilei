PROJECT(textanalyse)
INCLUDE_DIRECTORIES(.)

SET(SRC gtextanalyse.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(textanalyse STATIC ${SRC})
    SET_TARGET_PROPERTIES(textanalyse PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(textanalyse galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(textanalyse SHARED ${SRC})
    ADD_SOURCE_FILE(textanalyse frontend textanalyse_INCLUDES)
    ADD_FRONTEND(textanalyse 2 2 2.0.0 lib "r/galilei" textanalyse_INCLUDES)
    TARGET_LINK_LIBRARIES(textanalyse galilei roptimization rcore rmath)
ENDIF(WIN32)
