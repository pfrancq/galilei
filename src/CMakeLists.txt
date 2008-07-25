PROJECT(multi-space)
INCLUDE_DIRECTORIES(.)

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
    ADD_LIBRARY(genericsim STATIC genericsims.cpp)
    SET_TARGET_PROPERTIES(genericsim PROPERTIES COMPILE_FLAGS -fPIC)

    # Create plug-ins
  SET(simplugins profilessims docssims docsprofilessims docsgroupssims profilesgroupssims)
  FOREACH(simplugin ${simplugins})
    ADD_LIBRARY(${simplugin} SHARED ${simplugin}.cpp)
    ADD_SOURCE_FILE(${simplugin} frontend ${simplugin}_INCLUDES)
    ADD_FRONTEND(${simplugin} 2 2 2.0.0 lib "r/galilei" gcap_rofiles_INCLUDES)
    TARGET_LINK_LIBRARIES(${simplugin} galilei roptimization rcore rmath genericsim)
  ENDFOREACH(simplugin ${simplugins})

  SET(plugins profilesagree profilesdisagree docsagree docsdisagree)
  FOREACH(plugin ${plugins})
    ADD_LIBRARY(${plugin} SHARED ${plugin}.cpp)
    ADD_SOURCE_FILE(${plugin} frontend ${plugin}_INCLUDES)
    ADD_FRONTEND(${plugin} 2 2 2.0.0 lib "r/galilei" gcap_rofiles_INCLUDES)
    TARGET_LINK_LIBRARIES(${plugin} galilei roptimization rcore rmath)
  ENDFOREACH(plugin ${plugins})

ENDIF(WIN32)
