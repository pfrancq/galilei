PROJECT(gca)
INCLUDE_DIRECTORIES(.)

SET(COMMON_SRC ggcaobj.cpp ggcachromo.cpp ggcagroup.cpp ggcainst.cpp ggca.cpp ggcaheuristic.cpp ggcaprom.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(gca STATIC ggcagrouping.cpp ${COMMON_SRC)
    SET_TARGET_PROPERTIES(galilei PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(textanalyse galilei roptimization rmath rcore)
#    INSTALL(TARGETS galilei ARCHIVE DESTINATION lib)
ELSE(WIN32)
    ADD_LIBRARY(gca SHARED ggcagrouping.cpp ${COMMON_SRC})
    ADD_SOURCE_FILE(gca frontend gca_INCLUDES)
    ADD_FRONTEND(gca 2 2 2.0.0 lib "r/galilei" gca_INCLUDES)
    TARGET_LINK_LIBRARIES(gca galilei roptimization rcore rmath)
    ADD_LIBRARY(gda SHARED ggdagrouping.cpp ${COMMON_SRC})
    ADD_SOURCE_FILE(gda frontend gda_INCLUDES)
   ADD_FRONTEND(gda 2 2 2.0.0 lib "r/galilei" gda_INCLUDES)
    TARGET_LINK_LIBRARIES(gda galilei roptimization rcore rmath)

#    SET_TARGET_PROPERTIES(galilei PROPERTIES LT_VERSION_CURRENT 2
#                                               VERSION 2.0.0
#                                               SOVERSION 2)
#    CREATE_LIBTOOL_FILE(galilei /lib)
#    INSTALL(TARGETS galilei LIBRARY DESTINATION lib)
ENDIF(WIN32)

#IF(rcmake-verbose)
#    PRINT_LIST_WITH_MESSAGE("Will install the following header files for galilei:" galilei_INST_INCLUDES)
#ENDIF(rcmake-verbose)

#INSTALL(FILES ${galilei_INST_INCLUDES} DESTINATION include/r/galilei)