PROJECT(kmeans)
INCLUDE_DIRECTORIES(.)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

IF(WIN32)

	# Create plug-ins
    ADD_LIBRARY(kmeans_docs SHARED groupdocs.cpp)
    ADD_SOURCE_FILE(kmeans_docs frontend kmeans_docs_INCLUDES)
    ADD_FRONTEND(kmeans_docs SHARED 2 2 2.0.0 lib "r/galilei" kmeans_docs_INCLUDES)
    SET_TARGET_PROPERTIES(kmeans_docs PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(kmeans_docs galilei roptimization rcore rmath)
    ADD_LIBRARY(kmeans_profiles SHARED groupprofiles.cpp)
    ADD_SOURCE_FILE(kmeans_profiles frontend kmeans_profiles_INCLUDES)
    ADD_FRONTEND(kmeans_profiles SHARED 2 2 2.0.0 lib "r/galilei" kmeans_profiles_INCLUDES)
    SET_TARGET_PROPERTIES(kmeans_profiles PROPERTIES LINK_FLAGS "/NODEFAULTLIB:MSVCRTD;MSVCRT;LIBCMT;LIBCMTD")
    TARGET_LINK_LIBRARIES(kmeans_profiles galilei roptimization rcore rmath)

ELSE(WIN32)

    # Create plug-ins
    ADD_LIBRARY(kmeans_docs SHARED groupdocs.cpp)
    ADD_SOURCE_FILE(kmeans_docs frontend kmeans_docs_INCLUDES)
    ADD_FRONTEND(kmeans_docs SHARED 2 2 2.0.0 lib "r/galilei" kmeans_docs_INCLUDES)
    TARGET_LINK_LIBRARIES(kmeans_docs galilei roptimization rcore rmath dl)
    ADD_LIBRARY(kmeans_profiles SHARED groupprofiles.cpp)
    ADD_SOURCE_FILE(kmeans_profiles frontend kmeans_profiles_INCLUDES)
    ADD_FRONTEND(kmeans_profiles SHARED 2 2 2.0.0 lib "r/galilei" kmeans_profiles_INCLUDES)
    TARGET_LINK_LIBRARIES(kmeans_profiles galilei roptimization rcore rmath dl)

ENDIF(WIN32)
