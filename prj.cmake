#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project,
# - ...

#normaly all def in CMakeLists.txt
PROJECT(multi-space CXX)

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


#search for gecodechoquet
OPTION(disable-choquet "Disable Choquet's parameters evaluation" OFF)
IF(disable-choquet)
	MESSAGE(STATUS "Choquet's parameters evaluation disabled")
ELSE(disable-choquet)
	R_LOAD_DEPENDENCY("GECODECHOQUET" "GECODECHOQUET_LIB" "gecodechoquet")
ENDIF(disable-choquet)

SET(SUB_PROJECT multi-space)
ADD_SUBDIRECTORY(src)
