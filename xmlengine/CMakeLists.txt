PROJECT(xmlengine)
INCLUDE_DIRECTORIES(.)

SET(SRC
	genginexml.cpp
	gquerynode.cpp
	gquery.cpp
	gqueryres.cpp
	gprom.cpp
)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(xmlengine SHARED ${SRC})
    ADD_SOURCE_FILE(xmlengine frontend xmlengine_INCLUDES)
    ADD_FRONTEND(xmlengine SHARED 2 2 2.0.0 lib "r/galilei" xmlengine_INCLUDES)
    SET_TARGET_PROPERTIES(xmlengine PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(xmlengine galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(xmlengine SHARED ${SRC})
    ADD_SOURCE_FILE(xmlengine frontend xmlengine_INCLUDES)
    ADD_FRONTEND(xmlengine SHARED 2 2 2.0.0 lib "r/galilei" xmlengine_INCLUDES)
    TARGET_LINK_LIBRARIES(xmlengine galilei roptimization rcore rmath dl)
ENDIF(WIN32)

