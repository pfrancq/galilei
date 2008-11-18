PROJECT(langse)
INCLUDE_DIRECTORIES(.)

SET(SRC langse.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../Stemming_Library)

IF(WIN32)
    ADD_LIBRARY(langse STATIC ${SRC})
    SET_TARGET_PROPERTIES(langse PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langse galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(langse SHARED ${SRC})
    ADD_SOURCE_FILE(langse frontend langse_INCLUDES)
    ADD_FRONTEND(langse 2 2 2.0.0 lib "r/galilei" langse_INCLUDES)
    TARGET_LINK_LIBRARIES(langse galilei roptimization rcore rmath dl)
ENDIF(WIN32)
