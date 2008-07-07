PROJECT(langno)
INCLUDE_DIRECTORIES(.)

SET(SRC langno.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../Stemming_Library)

IF(WIN32)
    ADD_LIBRARY(langno STATIC ${SRC})
    SET_TARGET_PROPERTIES(langno PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langno galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(langno SHARED ${SRC})
    ADD_SOURCE_FILE(langno frontend langno_INCLUDES)
    ADD_FRONTEND(langno 2 2 2.0.0 lib "r/galilei" langno_INCLUDES)
    TARGET_LINK_LIBRARIES(langno galilei roptimization rcore rmath)
ENDIF(WIN32)
