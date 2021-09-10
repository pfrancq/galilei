PROJECT(stringtokenizer)
INCLUDE_DIRECTORIES(.)

SET(SRC gstringtokenizer.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(stringtokenizer SHARED ${SRC})
    ADD_SOURCE_FILE(stringtokenizer frontend stringtokenizer_INCLUDES)
    ADD_FRONTEND(stringtokenizer SHARED 2 2 2.0.0 lib "r/galilei" stringtokenizer_INCLUDES)
    SET_TARGET_PROPERTIES(stringtokenizer PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(stringtokenizer galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(stringtokenizer SHARED ${SRC})
    ADD_SOURCE_FILE(stringtokenizer frontend stringtokenizer_INCLUDES)
    ADD_FRONTEND(stringtokenizer SHARED 2 2 2.0.0 lib "r/galilei" stringtokenizer_INCLUDES)
    TARGET_LINK_LIBRARIES(stringtokenizer galilei roptimization rcore rmath dl)
ENDIF(WIN32)
