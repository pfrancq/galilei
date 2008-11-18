SET(textanalyse_frontend_kde_UI_INCLUDES config.ui)

SET(textanalyse_frontend_kde_TARGET_SOURCES gtextanalyse_kde.cpp)

FOREACH(file ${textanalyse_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(textanalyse_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${textanalyse_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
