SET(metaenginesum_frontend_kde_UI_INCLUDES dlgconfig_qt.ui)

SET(sugs_frontend_kde_TARGET_SOURCES gmetaenginesum_kde.cpp)

FOREACH(file ${metaenginesum_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(metaenginesum_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${metaenginesum_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
