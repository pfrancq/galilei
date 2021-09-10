PROJECT(promrank)
INCLUDE_DIRECTORIES(.)


SET(SRC
	gpromrank.cpp
	gprom.cpp
)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(promrank SHARED ${SRC})
    ADD_SOURCE_FILE(promrank frontend xmlengine_INCLUDES)
    ADD_FRONTEND(promrank SHARED 2 2 2.0.0 lib "r/galilei" promrank_INCLUDES)
    SET_TARGET_PROPERTIES(promrank PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(promrank galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(promrank SHARED ${SRC})
    ADD_SOURCE_FILE(promrank frontend xmlengine_INCLUDES)
    ADD_FRONTEND(promrank SHARED 2 2 2.0.0 lib "r/galilei" promrank_INCLUDES)
    TARGET_LINK_LIBRARIES(promrank galilei roptimization rcore rmath dl)
	 TARGET_LINK_LIBRARIES(promrankqt roptimizationqt)
ENDIF(WIN32)

