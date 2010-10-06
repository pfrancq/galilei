PROJECT(statfeatures)
INCLUDE_DIRECTORIES(.)

SET(SRC statfeatures.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(statfeatures STATIC ${SRC})
    SET_TARGET_PROPERTIES(statfeatures PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(statfeatures galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(statfeatures SHARED ${SRC})
    ADD_SOURCE_FILE(statfeatures frontend statfeatures_INCLUDES)
    ADD_FRONTEND(statfeatures 2 2 2.0.0 lib "r/galilei" statfeatures_INCLUDES)
    TARGET_LINK_LIBRARIES(statfeatures galilei roptimization rcore rmath dl)
ENDIF(WIN32)
