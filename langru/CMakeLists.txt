PROJECT(langru)
INCLUDE_DIRECTORIES(.)

SET(SRC langru.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../libstemmer/include)

IF(WIN32)
    ADD_LIBRARY(langru STATIC ${SRC})
    SET_TARGET_PROPERTIES(langru PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langru galilei roptimization rmath rcore libstemmer)
ELSE(WIN32)
    ADD_LIBRARY(langru SHARED ${SRC})
    ADD_SOURCE_FILE(langru frontend langru_INCLUDES)
    ADD_FRONTEND(langru 2 2 2.0.0 lib "r/galilei" langru_INCLUDES)
    TARGET_LINK_LIBRARIES(langru galilei roptimization rcore rmath dl libstemmer)
ENDIF(WIN32)
