#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project,
# - ...

#normaly all def in CMakeLists.txt
PROJECT(multi-space CXX)

R_LOAD_DEPENDENCY("R" "R_LIB" "r")
R_LOAD_DEPENDENCY("GALILEI" "GALILEI_LIB" "galilei")


#LINK_DIRECTORIES(${EXTRA_LIB_DIRECTORY})

#search for kde4
OPTION(disable-kde "Disable KDE 4 support" OFF)
IF(disable-kde)
	MESSAGE(STATUS "KDE4 support disabled")
ELSE(disable-kde)
	FIND_PACKAGE(KDE4 REQUIRED)
	FIND_PACKAGE(Qt4)
	IF(KDE4_FOUND)
	    INCLUDE (KDE4Defaults)
		ADD_DEFINITIONS(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
		INCLUDE_DIRECTORIES( ${KDE4_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )
		MESSAGE(STATUS "KDE4 support enabled.")
	ELSE(KDE4_FOUND)
		MESSAGE(FATAL_ERROR "KDE4 can't be found as requested")
	ENDIF(KDE4_FOUND)
ENDIF(disable-kde)

#search for gecodechoquet
OPTION(disable-choquet "Disable Choquet's parameters evaluation" OFF)
IF(disable-choquet)
	MESSAGE(STATUS "Choquet's parameters evaluation disabled")
ELSE(disable-choquet)
	R_LOAD_DEPENDENCY("GECODECHOQUET" "GECODECHOQUET_LIB" "gecodechoquet")
ENDIF(disable-choquet)

SET(SUB_PROJECT multi-space)
ADD_SUBDIRECTORY(src)
