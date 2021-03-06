PROJECT(galilei)
INCLUDE_DIRECTORIES(.)

ADD_SOURCE_FILE(galilei knowledge galilei_INCLUDES)
ADD_SOURCE_FILE(galilei docs galilei_INCLUDES)
ADD_SOURCE_FILE(galilei engines galilei_INCLUDES)
ADD_SOURCE_FILE(galilei profiles galilei_INCLUDES)
ADD_SOURCE_FILE(galilei groups galilei_INCLUDES)
ADD_SOURCE_FILE(galilei sessions galilei_INCLUDES)
ADD_SOURCE_FILE(galilei plugins galilei_INCLUDES)
ADD_SOURCE_FILE(galilei tools galilei_INCLUDES)
ADD_SOURCE_FILE(galilei frontend galilei_INCLUDES)

INCLUDE_DIRECTORIES(${galilei_INCLUDES})

IF(WIN32)
    ADD_LIBRARY(galilei STATIC ${galilei_TARGET_SOURCES})
    ADD_FRONTEND(galilei STATIC 2 2 2.0.0 lib "r/galilei" galilei_INCLUDES)
    SET_TARGET_PROPERTIES(galilei PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(galilei roptimization rmath rcore)
    INSTALL(TARGETS galilei ARCHIVE DESTINATION lib)
ELSE(WIN32)
    ADD_LIBRARY(galilei SHARED ${galilei_TARGET_SOURCES})   
    ADD_FRONTEND(galilei SHARED 2 2 2.0.0 lib "r/galilei" galilei_INCLUDES)
    TARGET_LINK_LIBRARIES(galilei roptimization rcore rmath dl)
    SET_TARGET_PROPERTIES(galilei PROPERTIES LT_VERSION_CURRENT 2
                                               VERSION 2.0.0
                                               SOVERSION 2)
    CREATE_LIBTOOL_FILE(galilei /lib)
    INSTALL(TARGETS galilei LIBRARY DESTINATION lib)
ENDIF(WIN32)

IF(rcmake-verbose)
    PRINT_LIST_WITH_MESSAGE("Will install the following header files for galilei:" galilei_INST_INCLUDES)
ENDIF(rcmake-verbose)

INSTALL(FILES ${galilei_INST_INCLUDES} DESTINATION include/r/galilei)