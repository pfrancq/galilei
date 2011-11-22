PROJECT(stopwords)
INCLUDE_DIRECTORIES(.)

SET(SRC gstopwords.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)
    ADD_LIBRARY(stopwords STATIC ${SRC})
    SET_TARGET_PROPERTIES(stopwords PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(stopwords galilei roptimization rmath rcore)
ELSE(WIN32)
    ADD_LIBRARY(stopwords SHARED ${SRC})
    ADD_SOURCE_FILE(stopwords frontend stopwords_INCLUDES)
    ADD_FRONTEND(stopwords 2 2 2.0.0 lib "r/galilei" stopwords_INCLUDES)
    TARGET_LINK_LIBRARIES(stopwords galilei roptimization rcore rmath dl)
ENDIF(WIN32)
