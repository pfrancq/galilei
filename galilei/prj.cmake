#normaly all def in CMakeLists.txt
PROJECT(galilei CXX)

R_LOAD_DEPENDENCY("R" "R_LIB" "r")

OPTION(disable-qt "Disable QT4 support" OFF)


#--------------------------------
#search for Qt4
IF(disable-qt)
	MESSAGE(STATUS "Qt4 support disabled")
ELSE(disable-qt)
	FIND_PACKAGE(Qt4 REQUIRED)
	IF(QT4_FOUND)
		ADD_DEFINITIONS(${QT_DEFINITIONS})
		INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )
		MESSAGE(STATUS "Qt4 support enabled.")
	ELSE(QT4_FOUND)
		MESSAGE(FATAL_ERROR "QT4 can't be found as requested")
	ENDIF(QT4_FOUND)
ENDIF(disable-qt)

SET(SUB_PROJECT galilei)
ADD_SUBDIRECTORY(galilei)

###file to install
##keep this macro for galilei based project in a standard place
INSTALL(FILES galileimacro.cmake DESTINATION share/r)
##keep it for compatibility with auto tools...
INSTALL(FILES libgalilei.m4 DESTINATION share/aclocal)
