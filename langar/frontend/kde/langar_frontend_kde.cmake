SET(langar_frontend_kde_UI_INCLUDES config.ui)

SET(langar_frontend_kde_TARGET_SOURCES langar_kde.cpp)

FOREACH(file ${langar_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(langar_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${langar_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
