SET(gstringindexer_frontend_kde_UI_INCLUDES config.ui)

SET(gstringindexer_frontend_kde_TARGET_SOURCES gstringindexer_kde.cpp)

FOREACH(file ${gstringindexer_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(gstringindexer_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${gstringindexer_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})