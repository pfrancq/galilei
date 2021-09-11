#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project,
# - ...

#normaly all def in CMakeLists.txt
PROJECT(qgalilei CXX)

R_LOAD_DEPENDENCY("R" "R_LIB" "r")
R_LOAD_DEPENDENCY("GALILEI" "GALILEI_LIB" "galilei")

FIND_PACKAGE(Qt4 REQUIRED)

IF(MINGW)
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mwindows")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows")
ENDIF(MINGW)


SET(SUB_PROJECT qgalilei)
ADD_SUBDIRECTORY(qgalilei)
