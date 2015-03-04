PROJECT(promrank)
INCLUDE_DIRECTORIES(.)
ADD_DEFINITIONS(${QT_DEFINITIONS})

SET(SRC
	gpromrank.cpp
 #	genginexml.cpp
#	gquerynode.cpp
#	gquery.cpp
 #	gresnode.cpp
 #	gresnodes.cpp
#	gqueryres.cpp
	#gprom.cpp
)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(promrank SHARED ${SRC})
    ADD_SOURCE_FILE(promrank frontend xmlengine_INCLUDES)
    ADD_FRONTEND(xmlengine SHARED 2 2 2.0.0 lib "r/galilei" xmlengine_INCLUDES)
    SET_TARGET_PROPERTIES(promrank PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(promrank galilei roptimization rmath rcore ${QT_LIBRARIES})
ELSE(WIN32)
    ADD_LIBRARY(promrank SHARED ${SRC})
    ADD_SOURCE_FILE(promrank frontend xmlengine_INCLUDES)
    ADD_FRONTEND(promrank SHARED 2 2 2.0.0 lib "r/galilei" xmlengine_INCLUDES)
    TARGET_LINK_LIBRARIES(promrank galilei roptimization rcore rmath dl ${QT_LIBRARIES})
ENDIF(WIN32)

