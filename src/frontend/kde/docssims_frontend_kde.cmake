SET(docssims_frontend_kde_UI_INCLUDES specific.ui)

SET(docssims_frontend_kde_TARGET_SOURCES genericsims_kde.cpp docssims_kde.cpp)

FOREACH(file ${docssims_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(docssims_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${docssims_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
