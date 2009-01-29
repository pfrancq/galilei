PROJECT(xmlengine)
INCLUDE_DIRECTORIES(.)

SET(SRC
   xnode.cpp
   genginexml.cpp
   xanalyser.cpp
   xanalysexmlfile.cpp
   xnodesql.cpp
   xtag.cpp
   xwordsql.cpp
   xxmlfile.cpp
   xquery.cpp
   xquerynode.cpp
   xqueryres.cpp
   xnodeset.cpp
   xresult.cpp
   xgeneral.cpp
   xstuff.cpp
   xsqlcmd.cpp
   gxmlranking.cpp
   gxmlprom.cpp
   gxmlir.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(xmlengine STATIC ${SRC})
    SET_TARGET_PROPERTIES(xmlengine PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(xmlengine galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(xmlengine SHARED ${SRC})
    ADD_SOURCE_FILE(xmlengine frontend xmlengine_INCLUDES)
    ADD_FRONTEND(xmlengine 2 2 2.0.0 lib "r/galilei" xmlengine_INCLUDES)
    TARGET_LINK_LIBRARIES(xmlengine galilei roptimization rcore rmath dl)
ENDIF(WIN32)
