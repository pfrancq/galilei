PROJECT(langfr)
INCLUDE_DIRECTORIES(.)

SET(SRC langfr.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(langfr SHARED ${SRC})
    ADD_SOURCE_FILE(langfr frontend langfr_INCLUDES)
    ADD_FRONTEND(langfr SHARED 2 2 2.0.0 lib "r/galilei" langfr_INCLUDES)
    SET_TARGET_PROPERTIES(langfr PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langfr galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(langfr SHARED ${SRC})
    ADD_SOURCE_FILE(langfr frontend langfr_INCLUDES)
    ADD_FRONTEND(langfr SHARED 2 2 2.0.0 lib "r/galilei" langfr_INCLUDES)
    TARGET_LINK_LIBRARIES(langfr galilei roptimization rcore rmath dl)
ENDIF(WIN32)
