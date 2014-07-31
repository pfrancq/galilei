PROJECT(parser)
INCLUDE_DIRECTORIES(.)

SET(SRC wparser.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(parser STATIC ${SRC})
    SET_TARGET_PROPERTIES(parser PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(parser galilei roptimization rmath rcore wikitoken)
ELSE(WIN32)
    ADD_LIBRARY(parser SHARED ${SRC})
    ADD_SOURCE_FILE(parser frontend parser_INCLUDES)
    ADD_FRONTEND(parser 2 2 2.0.0 lib "r/galilei" exportdocs_INCLUDES)
    TARGET_LINK_LIBRARIES(parser galilei roptimization rcore rmath wikitoken dl)
ENDIF(WIN32)
