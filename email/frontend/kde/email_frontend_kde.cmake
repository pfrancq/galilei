SET(email_frontend_kde_UI_INCLUDES config.ui)

SET(email_frontend_kde_TARGET_SOURCES email_kde.cpp)

FOREACH(file ${email_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(email_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${email_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
