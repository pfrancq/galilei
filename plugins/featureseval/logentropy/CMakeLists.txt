PROJECT(logentropy)
INCLUDE_DIRECTORIES(.)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)

	# Create plug-ins
	ADD_LIBRARY(logentropy SHARED logentropy.cpp)
	ADD_SOURCE_FILE(logentropy frontend logentropy_INCLUDES)
	ADD_FRONTEND(logentropy SHARED 2 2 2.0.0 lib "r/galilei" logentropy_INCLUDES)
	SET_TARGET_PROPERTIES(logentropy PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
	TARGET_LINK_LIBRARIES(logentropy galilei roptimization rcore rmath)

ELSE(WIN32)

	# Create plug-ins
	ADD_LIBRARY(logentropy SHARED logentropy.cpp)
	ADD_SOURCE_FILE(logentropy frontend logentropy_INCLUDES)
	ADD_FRONTEND(logentropy SHARED 2 2 2.0.0 lib "r/galilei" logentropy_INCLUDES)
	TARGET_LINK_LIBRARIES(logentropy galilei roptimization rcore rmath dl)

ENDIF(WIN32)
