PROJECT(thesaurus)
INCLUDE_DIRECTORIES(.)

SET(SRC thesaurus.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(thesaurus STATIC ${SRC})
    SET_TARGET_PROPERTIES(thesaurus PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(thesaurus galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(thesaurus SHARED ${SRC})
    ADD_SOURCE_FILE(thesaurus frontend subslevel_INCLUDES)
    ADD_FRONTEND(thesaurus 2 2 2.0.0 lib "r/galilei" subslevel_INCLUDES)
    TARGET_LINK_LIBRARIES(thesaurus galilei roptimization rcore rmath dl)
ENDIF(WIN32)
