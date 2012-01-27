SET(metaenginesum_frontend_kde_UI_INCLUDES config.ui)

SET(metaenginesum_frontend_kde_TARGET_SOURCES gmetaenginesum_kde.cpp)

KDE4_HANDLE_AUTOMOC(metaenginesum_frontend_kde_MOCFILE frontend/kde/gmetaenginesum_kde.cpp)

FOREACH(file ${metaenginesum_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(metaenginesum_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${metaenginesum_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
