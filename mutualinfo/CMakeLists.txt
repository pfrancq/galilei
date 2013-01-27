PROJECT(mutualinfo)
INCLUDE_DIRECTORIES(.)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)

	# Create plug-ins
	ADD_LIBRARY(mutualinfo STATIC mutualinfo.cpp)
	SET_TARGET_PROPERTIES(mutualinfo PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
	TARGET_LINK_LIBRARIES(mutualinfo galilei roptimization rcore rmath)

ELSE(WIN32)

	# Create plug-ins
	ADD_LIBRARY(mutualinfo SHARED mutualinfo.cpp)
	ADD_SOURCE_FILE(mutualinfo frontend mutualinfo_INCLUDES)
	ADD_FRONTEND(mutualinfo 2 2 2.0.0 lib "r/galilei" mutualinfo_INCLUDES)
	TARGET_LINK_LIBRARIES(mutualinfo galilei roptimization rcore rmath dl)

ENDIF(WIN32)
