PROJECT(statsims)
INCLUDE_DIRECTORIES(.)

SET(SRC gstatprofdoc.cpp statsims.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(statsims STATIC ${SRC})
    SET_TARGET_PROPERTIES(statsims PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(statsims galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(statsims SHARED ${SRC})
    ADD_SOURCE_FILE(statsims frontend statsims_INCLUDES)
    ADD_FRONTEND(statsims 2 2 2.0.0 lib "r/galilei" statsims_INCLUDES)
    TARGET_LINK_LIBRARIES(statsims galilei roptimization rcore rmath dl)
ENDIF(WIN32)
