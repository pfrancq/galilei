PROJECT(statdata)
INCLUDE_DIRECTORIES(.)

SET(SRC statdata.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(statdata STATIC ${SRC})
    SET_TARGET_PROPERTIES(statdata PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(statdata galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(statdata SHARED ${SRC})
    ADD_SOURCE_FILE(statdata frontend statdata_INCLUDES)
    ADD_FRONTEND(statdata 2 2 2.0.0 lib "r/galilei" statdata_INCLUDES)
    TARGET_LINK_LIBRARIES(statdata galilei roptimization rcore rmath dl)
ENDIF(WIN32)
