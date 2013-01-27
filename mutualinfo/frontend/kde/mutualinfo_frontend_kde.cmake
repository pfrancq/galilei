SET(mutualinfo_frontend_kde_UI_INCLUDES config.ui)

SET(mutualinfo_frontend_kde_TARGET_SOURCES mutualinfo_kde.cpp)

FOREACH(file ${mutualinfo_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(mutualinfo_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${mutualinfo_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
