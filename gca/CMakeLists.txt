PROJECT(gca)
INCLUDE_DIRECTORIES(.)

SET(COMMON_SRC  gcagroup.cpp gcachromo.cpp gcainst.cpp gca.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    # Static libraries
    ADD_LIBRARY(gca STATIC ${COMMON_SRC})
    SET_TARGET_PROPERTIES(gca PROPERTIES COMPILE_FLAGS -fPIC)

    # Create plug-ins
    ADD_LIBRARY(gca_profiles STATIC gcagroupprofiles.cpp)
    SET_TARGET_PROPERTIES(gca_profiles PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(gca_profiles galilei roptimization rcore rmath gca)
    ADD_LIBRARY(gca_docs STATIC gcagroupdocs.cpp)
    SET_TARGET_PROPERTIES(gca_docs PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(gca_docs galilei roptimization rcore rmath gca)
    
ELSE(WIN32)
    # Static libraries
    ADD_LIBRARY(gca STATIC ${COMMON_SRC})
    SET_TARGET_PROPERTIES(gca PROPERTIES COMPILE_FLAGS -fPIC)

    # Create plug-ins
    ADD_LIBRARY(gca_profiles SHARED gcagroupprofiles.cpp)
    ADD_SOURCE_FILE(gca_profiles frontend gca_INCLUDES)
    ADD_FRONTEND(gca_profiles 2 2 2.0.0 lib "r/galilei" gcap_rofiles_INCLUDES)
    TARGET_LINK_LIBRARIES(gca_profiles galilei roptimization rcore rmath gca dl)
    ADD_LIBRARY(gca_docs SHARED gcagroupdocs.cpp)
    ADD_SOURCE_FILE(gca_docs frontend gda_INCLUDES)
    ADD_FRONTEND(gca_docs 2 2 2.0.0 lib "r/galilei" gca_docs_INCLUDES)
    TARGET_LINK_LIBRARIES(gca_docs galilei roptimization rcore rmath gca dl)
ENDIF(WIN32)
