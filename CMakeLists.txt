cmake_minimum_required(VERSION 2.6)

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

IF(CMAKE_COMPILER_IS_GNUCC)
  MESSAGE(STATUS "Using GNU C Compiler, set -fmessage-length to 0 (Eclipse support)")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
    MESSAGE(STATUS "Using GNU C++ Compiler, set -fmessage-length to 0 (Eclipse support)")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

OPTION(rcmake-verbose "Enable verbose output during cmake construction" OFF)

IF(UNIX)
    ADD_DEFINITIONS("-fexceptions")
    MESSAGE(STATUS "C++ exceptions will be re-enabled (since KDE4 detection disable them)")
    ADD_DEFINITIONS("-pedantic")
    ADD_DEFINITIONS("-Wno-long-long")
    ADD_DEFINITIONS("-DPIC")
ELSE(UNIX)
    ADD_DEFINITIONS("/D_RCORE_DLLAPI_")
ENDIF(UNIX)

INCLUDE(rfinder.cmake)
INCLUDE(prj.cmake)
