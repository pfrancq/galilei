PROJECT(subslevel)
INCLUDE_DIRECTORIES(.)

SET(SRC subslevel.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(subslevel SHARED ${SRC})
    ADD_SOURCE_FILE(subslevel frontend subslevel_INCLUDES)
    ADD_FRONTEND(subslevel SHARED 2 2 2.0.0 lib "r/galilei" subslevel_INCLUDES)
    SET_TARGET_PROPERTIES(subslevel PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(subslevel galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(subslevel SHARED ${SRC})
    ADD_SOURCE_FILE(subslevel frontend subslevel_INCLUDES)
    ADD_FRONTEND(subslevel SHARED 2 2 2.0.0 lib "r/galilei" subslevel_INCLUDES)
    TARGET_LINK_LIBRARIES(subslevel galilei roptimization rcore rmath dl)
ENDIF(WIN32)
