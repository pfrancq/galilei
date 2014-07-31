SET(wikipedia_frontend_kde_UI_INCLUDES config.ui)

SET(wikipedia_frontend_kde_TARGET_SOURCES wikipedia_kde.cpp)

FOREACH(file ${wikipedia_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(wikipedia_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${wikipedia_frontend_kde_UI_INCLUDES})


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
