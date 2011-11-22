SET(sgga_docs_frontend_kde_UI_INCLUDES config.ui)

SET(sgga_docs_frontend_kde_TARGET_SOURCES sggaconfig_kde.cpp sggagroupdocs_kde.cpp)

FOREACH(file ${sgga_docs_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(sgga_docs_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${sgga_docs_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
