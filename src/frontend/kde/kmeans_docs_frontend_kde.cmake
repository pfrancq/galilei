SET(kmeans_docs_frontend_kde_UI_INCLUDES config.ui)

SET(kmeans_docs_frontend_kde_TARGET_SOURCES kmeansdocs_kde.cpp)

FOREACH(file ${kmeans_docs_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(kmeans_docs_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${kmeans_docs_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
