PROJECT(multi-space)
INCLUDE_DIRECTORIES(.)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    # Static libraries
    ADD_LIBRARY(genericsim STATIC genericsims.cpp)
    SET_TARGET_PROPERTIES(genericsim PROPERTIES COMPILE_FLAGS -fPIC)

    # Create plug-ins
    SET(simplugins profilessims docssims docsprofilessims docsgroupssims profilesgroupssims)
    FOREACH(simplugin ${simplugins})
        ADD_LIBRARY(${simplugin} STATIC ${simplugin}.cpp)
        SET_TARGET_PROPERTIES(${simplugin} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
        TARGET_LINK_LIBRARIES(${simplugin} galilei roptimization rcore rmath genericsim)
    ENDFOREACH(simplugin ${simplugins})

ELSE(WIN32)

    # Static libraries
    ADD_LIBRARY(genericsim STATIC genericsims.cpp)
    SET_TARGET_PROPERTIES(genericsim PROPERTIES COMPILE_FLAGS -fPIC)

    # Create plug-ins
  SET(simplugins profilessims docssims docsprofilessims docsgroupssims profilesgroupssims)
  FOREACH(simplugin ${simplugins})
    ADD_LIBRARY(${simplugin} SHARED ${simplugin}.cpp)
    ADD_SOURCE_FILE(${simplugin} frontend ${simplugin}_INCLUDES)
    ADD_FRONTEND(${simplugin} 2 2 2.0.0 lib "r/galilei" ${simplugin}_INCLUDES)
    TARGET_LINK_LIBRARIES(${simplugin} galilei roptimization rcore rmath genericsim dl)
  ENDFOREACH(simplugin ${simplugins})

  SET(plugins profilesagree profilesdisagree docsagree docsdisagree)
  FOREACH(plugin ${plugins})
    ADD_LIBRARY(${plugin} SHARED ${plugin}.cpp)
    ADD_SOURCE_FILE(${plugin} frontend ${plugin}_INCLUDES)
    ADD_FRONTEND(${plugin} 2 2 2.0.0 lib "r/galilei" ${plugin}_INCLUDES)
    TARGET_LINK_LIBRARIES(${plugin} galilei roptimization rcore rmath dl)
  ENDFOREACH(plugin ${plugins})

ENDIF(WIN32)
