SET(feedback_frontend_kde_UI_INCLUDES config.ui)

SET(feedback_frontend_kde_TARGET_SOURCES feedback_kde.cpp)

FOREACH(file ${feedback_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(feedback_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${feedback_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
