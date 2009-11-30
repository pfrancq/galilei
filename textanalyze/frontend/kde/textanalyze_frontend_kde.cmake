SET(textanalyze_frontend_kde_UI_INCLUDES config.ui)

SET(textanalyze_frontend_kde_TARGET_SOURCES gtextanalyze_kde.cpp)

KDE4_HANDLE_AUTOMOC(textanalyze_frontend_kde_MOCFILE frontend/kde/gtextanalyze_kde.cpp)

FOREACH(file ${textanalyze_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(textanalyze_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${textanalyze_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
