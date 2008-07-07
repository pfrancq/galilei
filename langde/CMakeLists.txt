PROJECT(langde)
INCLUDE_DIRECTORIES(.)

SET(SRC langde.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../Stemming_Library)

IF(WIN32)
    ADD_LIBRARY(langde STATIC ${SRC})
    SET_TARGET_PROPERTIES(langde PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langde galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(langde SHARED ${SRC})
    ADD_SOURCE_FILE(langde frontend langde_INCLUDES)
    ADD_FRONTEND(langde 2 2 2.0.0 lib "r/galilei" langde_INCLUDES)
    TARGET_LINK_LIBRARIES(langde galilei roptimization rcore rmath)
ENDIF(WIN32)
