#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project,
# - ...

#normaly all def in CMakeLists.txt
PROJECT(filters CXX)

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

#path for libwv2
FIND_LIBRARY(LIBWV2_LIB_PATH wv2 ${libwv2_LIB_SEARCH_PATH})
FIND_PATH(LIBWV2_INCLUDE_PATH wv2/parser.h ${libwv2_INCLUDE_SEARCH_PATH})
IF(rcmake-verbose)
	MESSAGE(STATUS "libwv2 library path configured: lib ${libwv2_LIB_SEARCH_PATH} includes ${libwv2_INCLUDE_SEARCH_PATH}")
ENDIF(rcmake-verbose)
IF(LIBWV2_LIB_PATH)
	IF(LIBWV2_INCLUDE_PATH)
		INCLUDE_DIRECTORIES(${LIBWV2_INCLUDE_PATH})
		MESSAGE(STATUS "libwv2 library successfully configured: lib ${LIBWV2_LIB_PATH} includes ${LIBWV2_INCLUDE_PATH}")
	ELSE(LIBWV2_INCLUDE_PATH)
		MESSAGE(FATAL_ERROR "libwv2 header files cannot be found.")
	ENDIF(LIBWV2_INCLUDE_PATH)
ELSE(LIBWV2_LIB_PATH)
	MESSAGE(FATAL_ERROR "libwv2 library cannot be found.")
ENDIF(LIBWV2_LIB_PATH)

SET(plugins txt email html pdf ps rtf xml) # msdoc
FOREACH(plugin ${plugins})
    SET(SUB_PROJECT ${plugin})
    ADD_SUBDIRECTORY(${plugin})
ENDFOREACH(plugin ${plugins})

