#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project,
# - ...

#normaly all def in CMakeLists.txt
PROJECT(featureseval CXX)

R_LOAD_DEPENDENCY("R" "R_LIB" "r")
R_LOAD_DEPENDENCY("GALILEI" "GALILEI_LIB" "galilei")

OPTION(disable-logentropy "Disable Log/Entropy support" OFF)
OPTION(disable-tfidf "Disable Tf/Idf support" OFF)
OPTION(disable-qt "Disable QT4 support" OFF)


#--------------------------------
#search for Qt4
IF(disable-qt)
	MESSAGE(STATUS "Qt4 support disabled")
ELSE(disable-qt)
	FIND_PACKAGE(Qt4 REQUIRED)
	IF(QT4_FOUND)
		ADD_DEFINITIONS(${QT_DEFINITIONS})
		INCLUDE(${QT_USE_FILE})
		INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )
		MESSAGE(STATUS "Qt4 support enabled.")
	ELSE(QT4_FOUND)
		MESSAGE(FATAL_ERROR "QT4 can't be found as requested")
	ENDIF(QT4_FOUND)
ENDIF(disable-qt)

SET(SUB_PROJECT featureseval)

# Add Tf/Idf if necessary
IF(disable-tfidf)
	MESSAGE(STATUS "Tf/Idf support disabled")
ELSE(disable-tfidf)
	MESSAGE(STATUS "Tf/Idf support enabled")
	ADD_SUBDIRECTORY(if)
ENDIF(disable-tfidf)

# Add Log/Entropy if necessary
IF(disable-logentropy)
	MESSAGE(STATUS "Log/Entropy support disabled")
ELSE(disable-logentropy)
	MESSAGE(STATUS "Log/Entropy support enabled")
	ADD_SUBDIRECTORY(logentropy)
ENDIF(disable-logentropy)
