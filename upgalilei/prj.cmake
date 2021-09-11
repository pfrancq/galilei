#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project,
# - ...

#normaly all def in CMakeLists.txt
PROJECT(upgalilei CXX)

R_LOAD_DEPENDENCY("R" "R_LIB" "r")
R_LOAD_DEPENDENCY("GALILEI" "GALILEI_LIB" "galilei")

SET(SUB_PROJECT upgalilei)
ADD_SUBDIRECTORY(src)
