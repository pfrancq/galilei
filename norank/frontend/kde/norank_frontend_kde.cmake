SET(norank_frontend_kde_UI_INCLUDES config.ui)

SET(norank_frontend_kde_TARGET_SOURCES gnorank_kde.cpp)

KDE4_HANDLE_AUTOMOC(norank_frontend_kde_MOCFILE frontend/kde/gnorank_kde.cpp)

FOREACH(file ${norank_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(norank_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${norank_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
