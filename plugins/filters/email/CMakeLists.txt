PROJECT(email)
INCLUDE_DIRECTORIES(.)

SET(SRC email.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(email SHARED ${SRC})
    ADD_SOURCE_FILE(email frontend email_INCLUDES)
    ADD_FRONTEND(email SHARED 2 2 2.0.0 lib "r/galilei" email_INCLUDES)
    SET_TARGET_PROPERTIES(email PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(email galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(email SHARED ${SRC})
    ADD_SOURCE_FILE(email frontend email_INCLUDES)
    ADD_FRONTEND(email SHARED 2 2 2.0.0 lib "r/galilei" email_INCLUDES)
    TARGET_LINK_LIBRARIES(email galilei roptimization rcore rmath dl)
ENDIF(WIN32)
