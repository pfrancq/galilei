PROJECT(featureseval)
INCLUDE_DIRECTORIES(.)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)

	# Create plug-ins
	ADD_LIBRARY(featureseval STATIC featureseval.cpp)
	SET_TARGET_PROPERTIES(featureseval PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
	TARGET_LINK_LIBRARIES(featureseval galilei roptimization rcore rmath)

ELSE(WIN32)

	# Create plug-ins
	ADD_LIBRARY(featureseval SHARED featureseval.cpp)
	ADD_SOURCE_FILE(featureseval frontend featureseval_INCLUDES)
	ADD_FRONTEND(featureseval 2 2 2.0.0 lib "r/galilei" featureseval_INCLUDES)
	TARGET_LINK_LIBRARIES(featureseval galilei roptimization rcore rmath dl)

ENDIF(WIN32)
