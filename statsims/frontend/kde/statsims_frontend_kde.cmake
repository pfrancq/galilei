SET(statsims_frontend_kde_UI_INCLUDES config.ui)

SET(statsims_frontend_kde_TARGET_SOURCES statsims_kde.cpp)

FOREACH(file ${statsims_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(statsims_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${statsims_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
