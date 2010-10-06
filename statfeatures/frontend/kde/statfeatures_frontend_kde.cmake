SET(statfeatures_frontend_kde_UI_INCLUDES config.ui)

SET(statfeatures_frontend_kde_TARGET_SOURCES statfeatures_kde.cpp)

FOREACH(file ${statfeatures_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(statfeatures_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${statfeatures_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
