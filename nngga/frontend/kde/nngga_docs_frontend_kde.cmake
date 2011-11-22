SET(nngga_docs_frontend_kde_UI_INCLUDES config.ui)

SET(nngga_docs_frontend_kde_TARGET_SOURCES nnggaconfig_kde.cpp nnggagroupdocs_kde.cpp)

FOREACH(file ${nngga_docs_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(nngga_docs_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${nngga_docs_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
