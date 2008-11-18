SET(gmysql_frontend_kde_UI_INCLUDES config.ui)

SET(gmysql_frontend_kde_TARGET_SOURCES gmysql_kde.cpp)

FOREACH(file ${gmysql_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(gmysql_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${gmysql_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
