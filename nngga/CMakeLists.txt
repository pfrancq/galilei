PROJECT(nngga)
INCLUDE_DIRECTORIES(.)

SET(COMMON_SRC nnggagroup.cpp nnggachromo.cpp nnggainst.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    # Static libraries
    ADD_LIBRARY(nngga STATIC ${COMMON_SRC})
    SET_TARGET_PROPERTIES(nngga PROPERTIES COMPILE_FLAGS -fPIC)

    # Create plug-ins
    ADD_LIBRARY(nngga_profiles SHARED nnggagroupprofiles.cpp)
    ADD_SOURCE_FILE(nngga_profiles frontend nngga_INCLUDES)
    ADD_FRONTEND(nngga_profiles SHARED 2 2 2.0.0 lib "r/galilei" nngga_profiles_INCLUDES)
    SET_TARGET_PROPERTIES(nngga_profiles PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(nngga_profiles galilei roptimization rcore rmath nngga)
    ADD_LIBRARY(nngga_docs SHARED nnggagroupdocs.cpp)
    ADD_SOURCE_FILE(nngga_docs frontend nngga_INCLUDES)
    ADD_FRONTEND(nngga_docs SHARED 2 2 2.0.0 lib "r/galilei" nngga_docs_INCLUDES)
    SET_TARGET_PROPERTIES(nngga_docs PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(nngga_docs galilei roptimization rcore rmath nngga)

ELSE(WIN32)
    # Static libraries
    ADD_LIBRARY(nngga STATIC ${COMMON_SRC})
    SET_TARGET_PROPERTIES(nngga PROPERTIES COMPILE_FLAGS -fPIC)

    # Create plug-ins
    ADD_LIBRARY(nngga_profiles SHARED nnggagroupprofiles.cpp)
    ADD_SOURCE_FILE(nngga_profiles frontend nngga_INCLUDES)
    ADD_FRONTEND(nngga_profiles SHARED 2 2 2.0.0 lib "r/galilei" nngga_profiles_INCLUDES)
    TARGET_LINK_LIBRARIES(nngga_profiles galilei roptimization rcore rmath nngga dl)
    ADD_LIBRARY(nngga_docs SHARED nnggagroupdocs.cpp)
    ADD_SOURCE_FILE(nngga_docs frontend nngga_INCLUDES)
    ADD_FRONTEND(nngga_docs SHARED 2 2 2.0.0 lib "r/galilei" nngga_docs_INCLUDES)
    TARGET_LINK_LIBRARIES(nngga_docs galilei roptimization rcore rmath nngga dl)
ENDIF(WIN32)
