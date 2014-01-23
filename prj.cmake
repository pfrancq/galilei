#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project,
# - ...

#normaly all def in CMakeLists.txt
PROJECT(featureseval CXX)

R_LOAD_DEPENDENCY("R" "R_LIB" "r")
R_LOAD_DEPENDENCY("GALILEI" "GALILEI_LIB" "galilei")

#LINK_DIRECTORIES(${EXTRA_LIB_DIRECTORY})

OPTION(disable-kde "Disable KDE 4 support" OFF)
OPTION(disable-logentropy "Disable Log/Entropy support" OFF)
OPTION(disable-tfidf "Disable Tf/Idf support" OFF)

#search for kde4
IF(disable-kde)
	MESSAGE(STATUS "KDE4 support disabled")
ELSE(disable-kde)
	FIND_PACKAGE(KDE4 REQUIRED)
	FIND_PACKAGE(Qt4)
	IF(KDE4_FOUND)
	    INCLUDE (KDE4Defaults)
		ADD_DEFINITIONS(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
		INCLUDE_DIRECTORIES( ${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )
		MESSAGE(STATUS "KDE4 support enabled")
	ELSE(KDE4_FOUND)
		MESSAGE(FATAL_ERROR "KDE4 can't be found as requested")
	ENDIF(KDE4_FOUND)
ENDIF(disable-kde)

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
