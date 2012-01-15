SET(html_frontend_kde_UI_INCLUDES config.ui)

SET(html_frontend_kde_TARGET_SOURCES html_kde.cpp)

FOREACH(file ${html_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(html_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${html_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
