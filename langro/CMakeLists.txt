PROJECT(langro)
INCLUDE_DIRECTORIES(.)

SET(SRC langro.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../libstemmer/include)

IF(WIN32)
    ADD_LIBRARY(langro STATIC ${SRC})
    SET_TARGET_PROPERTIES(langro PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langro galilei roptimization rmath rcore libstemmer)
ELSE(WIN32)
    ADD_LIBRARY(langro SHARED ${SRC})
    ADD_SOURCE_FILE(langro frontend langro_INCLUDES)
    ADD_FRONTEND(langro 2 2 2.0.0 lib "r/galilei" langro_INCLUDES)
    TARGET_LINK_LIBRARIES(langro galilei roptimization rcore rmath dl libstemmer)
ENDIF(WIN32)
