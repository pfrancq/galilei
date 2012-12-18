PROJECT(multi-space)
INCLUDE_DIRECTORIES(.)


# Create a generic library with the computational methods
IF(WIN32)
    ADD_LIBRARY(computesims STATIC gsimplugin.cpp gcomputesimcos.cpp gcomputesimmeta.cpp gcomputesimlink.cpp)
    SET_TARGET_PROPERTIES(computesims galilei roptimization rcore rmath PROPERTIES COMPILE_FLAGS -fPIC)

ELSE(WIN32)

    ADD_LIBRARY(computesims SHARED gsimplugin.cpp gcomputesimcos.cpp gcomputesimmeta.cpp gcomputesimlink.cpp)
    TARGET_LINK_LIBRARIES(computesims galilei roptimization rcore rmath dl)

ENDIF(WIN32)



INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)

    # Create plug-ins
    SET(simplugins profilessims docssims docsprofilessims docsgroupssims profilesgroupssims)
    FOREACH(simplugin ${simplugins})
        ADD_LIBRARY(${simplugin} STATIC ${simplugin}.cpp)
        SET_TARGET_PROPERTIES(${simplugin} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
        TARGET_LINK_LIBRARIES(${simplugin} computesims galilei roptimization rcore rmath)
    ENDFOREACH(simplugin ${simplugins})

  SET(plugins profilesagree profilesdisagree docsagree docsdisagree)
  FOREACH(plugin ${plugins})
        ADD_LIBRARY(${plugin} STATIC ${plugin}.cpp)
        SET_TARGET_PROPERTIES(${plugin} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
        TARGET_LINK_LIBRARIES(${plugin} galilei roptimization rcore rmath)
  ENDFOREACH(plugin ${plugins})

ELSE(WIN32)

    # Create plug-ins
  SET(simplugins profilessims docssims docsprofilessims docsgroupssims profilesgroupssims)
  FOREACH(simplugin ${simplugins})
    ADD_LIBRARY(${simplugin} SHARED ${simplugin}.cpp)
    ADD_SOURCE_FILE(${simplugin} frontend ${simplugin}_INCLUDES)
    ADD_FRONTEND(${simplugin} 2 2 2.0.0 lib "r/galilei" ${simplugin}_INCLUDES)
    TARGET_LINK_LIBRARIES(${simplugin} computesims galilei roptimization rcore rmath dl)
  ENDFOREACH(simplugin ${simplugins})

  SET(plugins profilesagree profilesdisagree docsagree docsdisagree)
  FOREACH(plugin ${plugins})
    ADD_LIBRARY(${plugin} SHARED ${plugin}.cpp)
    ADD_SOURCE_FILE(${plugin} frontend ${plugin}_INCLUDES)
    ADD_FRONTEND(${plugin} 2 2 2.0.0 lib "r/galilei" ${plugin}_INCLUDES)
    TARGET_LINK_LIBRARIES(${plugin} galilei roptimization rcore rmath dl)
  ENDFOREACH(plugin ${plugins})

ENDIF(WIN32)
