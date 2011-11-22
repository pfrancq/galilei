PROJECT(sgga)
INCLUDE_DIRECTORIES(.)

SET(COMMON_SRC sggagroup.cpp sggachromo.cpp sggainst.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    # Static libraries
    ADD_LIBRARY(sgga STATIC ${COMMON_SRC})
    SET_TARGET_PROPERTIES(sgga PROPERTIES COMPILE_FLAGS -fPIC)

    # Create plug-ins
    ADD_LIBRARY(sgga_profiles STATIC sggagroupprofiles.cpp)
    SET_TARGET_PROPERTIES(sgga_profiles PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(sgga_profiles galilei roptimization rcore rmath sgga)
    ADD_LIBRARY(sgga_docs STATIC sggagroupdocs.cpp)
    SET_TARGET_PROPERTIES(sgga_docs PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(sgga_docs galilei roptimization rcore rmath sgga)

ELSE(WIN32)
    # Static libraries
    ADD_LIBRARY(sgga STATIC ${COMMON_SRC})
    SET_TARGET_PROPERTIES(sgga PROPERTIES COMPILE_FLAGS -fPIC)

    # Create plug-ins
    ADD_LIBRARY(sgga_profiles SHARED sggagroupprofiles.cpp)
    ADD_SOURCE_FILE(sgga_profiles frontend sgga_INCLUDES)
    ADD_FRONTEND(sgga_profiles 2 2 2.0.0 lib "r/galilei" sgga_profiles_INCLUDES)
    TARGET_LINK_LIBRARIES(sgga_profiles galilei roptimization rcore rmath sgga dl)
    ADD_LIBRARY(sgga_docs SHARED sggagroupdocs.cpp)
    ADD_SOURCE_FILE(sgga_docs frontend gda_INCLUDES)
    ADD_FRONTEND(sgga_docs 2 2 2.0.0 lib "r/galilei" sgga_docs_INCLUDES)
    TARGET_LINK_LIBRARIES(sgga_docs galilei roptimization rcore rmath sgga dl)
ENDIF(WIN32)
