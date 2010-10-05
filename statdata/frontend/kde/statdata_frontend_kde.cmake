SET(statdata_frontend_kde_UI_INCLUDES config.ui)

SET(statdata_frontend_kde_TARGET_SOURCES statdata_kde.cpp)

FOREACH(file ${statdata_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(statdata_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${statdata_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
