#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project,
# - ...

#normaly all def in CMakeLists.txt
PROJECT(gravitation CXX)

R_LOAD_DEPENDENCY("R" "R_LIB" "r")
R_LOAD_DEPENDENCY("GALILEI" "GALILEI_LIB" "galilei")

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

SET(SUB_PROJECT gravitation_community)
ADD_SUBDIRECTORY(community)

SET(SUB_PROJECT gravitation_topic)
ADD_SUBDIRECTORY(topic)
