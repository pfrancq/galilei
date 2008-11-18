SET(sugs_frontend_kde_UI_INCLUDES config.ui)

SET(sugs_frontend_kde_TARGET_SOURCES sugs_kde.cpp)

FOREACH(file ${sugs_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(sugs_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${sugs_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
