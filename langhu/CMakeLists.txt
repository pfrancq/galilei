PROJECT(langhu)
INCLUDE_DIRECTORIES(.)

SET(SRC langhu.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../libstemmer/include)

IF(WIN32)
    ADD_LIBRARY(langhu SHARED ${SRC})
    ADD_SOURCE_FILE(langhu frontend langhu_INCLUDES)
    ADD_FRONTEND(langhu SHARED 2 2 2.0.0 lib "r/galilei" langhu_INCLUDES)
    SET_TARGET_PROPERTIES(langhu PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langhu galilei roptimization rmath rcore libstemmer)
ELSE(WIN32)
    ADD_LIBRARY(langhu SHARED ${SRC})
    ADD_SOURCE_FILE(langhu frontend langhu_INCLUDES)
    ADD_FRONTEND(langhu SHARED 2 2 2.0.0 lib "r/galilei" langhu_INCLUDES)
    TARGET_LINK_LIBRARIES(langhu galilei roptimization rcore rmath dl libstemmer)
ENDIF(WIN32)
