PROJECT(textanalyze)
INCLUDE_DIRECTORIES(.)

SET(SRC gtextanalyze.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(textanalyze STATIC ${SRC})
    SET_TARGET_PROPERTIES(textanalyze PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(textanalyze galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(textanalyze SHARED ${SRC})
    ADD_SOURCE_FILE(textanalyze frontend textanalyze_INCLUDES)
    ADD_FRONTEND(textanalyze 2 2 2.0.0 lib "r/galilei" textanalyze_INCLUDES)
    TARGET_LINK_LIBRARIES(textanalyze galilei roptimization rcore rmath dl)
ENDIF(WIN32)
