PROJECT(xmlengine)
INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(${QT_QT3SUPPORT_INCLUDE_DIR})
ADD_DEFINITIONS(-DQT3_SUPPORT)
ADD_DEFINITIONS(${QT_DEFINITIONS})

SET(SRC
	genginexml.cpp
	gquerynode.cpp
	gquery.cpp
	gresnode.cpp
	gresnodes.cpp
	gqueryres.cpp
	gprom.cpp
)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(xmlengine STATIC ${SRC})
    SET_TARGET_PROPERTIES(xmlengine PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(xmlengine galilei roptimization rmath rcore ${QT_LIBRARIES} ${QT_QT3SUPPORT_LIBRARY})


ELSE(WIN32)
    ADD_LIBRARY(xmlengine SHARED ${SRC})
    ADD_SOURCE_FILE(xmlengine frontend xmlengine_INCLUDES)
    ADD_FRONTEND(xmlengine 2 2 2.0.0 lib "r/galilei" xmlengine_INCLUDES)
    TARGET_LINK_LIBRARIES(xmlengine galilei roptimization rcore rmath dl ${QT_LIBRARIES} ${QT_QT3SUPPORT_LIBRARY})

ENDIF(WIN32)

