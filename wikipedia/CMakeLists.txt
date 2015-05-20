PROJECT(wikipedia)
INCLUDE_DIRECTORIES(.)

SET(SRC wikipedia.cpp wikidumparticles.cpp wikidumphistory.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(wikipedia SHARED ${SRC})
    ADD_SOURCE_FILE(wikipedia frontend wikipedia_INCLUDES)
    ADD_FRONTEND(wikipedia SHARED 2 2 2.0.0 lib "r/galilei" wikipedia_INCLUDES)
    SET_TARGET_PROPERTIES(wikipedia PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(wikipedia galilei roptimization rmath rcore wikitoken)
ELSE(WIN32)
    ADD_LIBRARY(wikipedia SHARED ${SRC})
    ADD_SOURCE_FILE(wikipedia frontend wikipedia_INCLUDES)
    ADD_FRONTEND(wikipedia SHARED 2 2 2.0.0 lib "r/galilei" wikipedia_INCLUDES)
    TARGET_LINK_LIBRARIES(wikipedia galilei roptimization rcore rmath wikitoken dl)
ENDIF(WIN32)
