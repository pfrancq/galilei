SET(subslevel_frontend_kde_UI_INCLUDES config.ui)

SET(subslevel_frontend_kde_TARGET_SOURCES subslevel_kde.cpp)

FOREACH(file ${subslevel_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(subslevel_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${subslevel_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
