#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project, 
# - ...

#normaly all def in CMakeLists.txt
PROJECT(galilei CXX)

R_LOAD_DEPENDENCY("R" "R_LIB" "r")

#LINK_DIRECTORIES(${EXTRA_LIB_DIRECTORY})

OPTION(disable-kde "Disable KDE 3 support" OFF)

#search for kde3
IF(disable-kde)
	MESSAGE(STATUS "KDE3 support disabled")
ELSE(disable-kde)
	FIND_PACKAGE(KDE3 REQUIRED)
	IF(KDE3_FOUND)
		ADD_DEFINITIONS(${QT_DEFINITIONS} ${KDE3_DEFINITIONS})
		INCLUDE_DIRECTORIES( ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )
		MESSAGE(STATUS "KDE3 support enabled.")
	ELSE(KDE3_FOUND)
		MESSAGE(FATAL_ERROR "KDE3 can't be found as requested")
	ENDIF(KDE3_FOUND)
ENDIF(disable-kde)

SET(SUB_PROJECT galilei)
ADD_SUBDIRECTORY(galilei)

###file to install
##keep this macro for galilei based project in a standard place
INSTALL(FILES galileimacro.cmake DESTINATION share/r)
##keep it for compatibility with auto tools...
INSTALL(FILES libgalilei.m4 DESTINATION share/aclocal)
