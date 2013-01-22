SET(profilesgroupssims_frontend_kde_UI_INCLUDES specific.ui)

SET(profilesgroupssims_frontend_kde_TARGET_SOURCES genericsims_kde.cpp profilesgroupssims_kde.cpp)

FOREACH(file ${profilesgroupssims_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(profilesgroupssims_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${profilesgroupssims_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
