SET(profilessims_frontend_kde_UI_INCLUDES specific.ui)

SET(profilessims_frontend_kde_TARGET_SOURCES genericsims_kde.cpp profilessims_kde.cpp)

FOREACH(file ${profilessims_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(profilessims_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${profilessims_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
