PROJECT(langfi)
INCLUDE_DIRECTORIES(.)

SET(SRC langfi.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR} ../Stemming_Library)

IF(WIN32)
    ADD_LIBRARY(langfi STATIC ${SRC})
    SET_TARGET_PROPERTIES(langfi PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(langfi galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(langfi SHARED ${SRC})
    ADD_SOURCE_FILE(langfi frontend langfi_INCLUDES)
    ADD_FRONTEND(langfi 2 2 2.0.0 lib "r/galilei" langfi_INCLUDES)
    TARGET_LINK_LIBRARIES(langfi galilei roptimization rcore rmath dl)
ENDIF(WIN32)
