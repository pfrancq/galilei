PROJECT(textanalyse)
INCLUDE_DIRECTORIES(.)

SET(SRC gtextanalyse.cpp xmlparser.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(textanalyse STATIC ${SRC})
    SET_TARGET_PROPERTIES(galilei PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(textanalyse galilei roptimization rmath rcore)
#    INSTALL(TARGETS galilei ARCHIVE DESTINATION lib)
ELSE(WIN32)
    ADD_LIBRARY(textanalyse SHARED ${SRC})
    ADD_SOURCE_FILE(textanalyse frontend textanalyse_INCLUDES)
   ADD_FRONTEND(textanalyse 2 2 2.0.0 lib "r/galilei" textanalyse_INCLUDES)
    TARGET_LINK_LIBRARIES(textanalyse galilei roptimization rcore rmath)
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