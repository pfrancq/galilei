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

FIND_PACKAGE(KDE3 REQUIRED)
IF(KDE3_FOUND)
	ADD_DEFINITIONS(${QT_DEFINITIONS} ${KDE3_DEFINITIONS})
	INCLUDE_DIRECTORIES( ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )
	MESSAGE(STATUS "KDE3 support enabled.")
ELSE(KDE3_FOUND)
	MESSAGE(FATAL_ERROR "KDE3 can't be found as requested")
ENDIF(KDE3_FOUND)

SET(SUB_PROJECT multi-sapce)
ADD_SUBDIRECTORY(src)
