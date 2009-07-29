PROJECT(clustering-evaluation)
INCLUDE_DIRECTORIES(.)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)

    # Create plug-ins
    SET(simplugins topicseval communitieseval classeseval)
    FOREACH(simplugin ${simplugins})
        ADD_LIBRARY(${simplugin} STATIC ${simplugin}.cpp)
        SET_TARGET_PROPERTIES(${simplugin} PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
        TARGET_LINK_LIBRARIES(${simplugin} galilei roptimization rcore rmath)
    ENDFOREACH(simplugin ${simplugins})

ELSE(WIN32)

    # Create plug-ins
  SET(plugins topicseval communitieseval classeseval)
  FOREACH(plugin ${plugins})
    ADD_LIBRARY(${plugin} SHARED ${plugin}.cpp)
    ADD_SOURCE_FILE(${plugin} frontend ${plugin}_INCLUDES)
    ADD_FRONTEND(${plugin} 2 2 2.0.0 lib "r/galilei" ${plugin}_INCLUDES)
    TARGET_LINK_LIBRARIES(${plugin} galilei roptimization rcore rmath dl)
  ENDFOREACH(plugin ${plugins})

ENDIF(WIN32)
