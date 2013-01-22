SET(docsprofilessims_frontend_kde_UI_INCLUDES specific.ui)

SET(docsprofilessims_frontend_kde_TARGET_SOURCES genericsims_kde.cpp docsprofilessims_kde.cpp)

FOREACH(file ${docsprofilessims_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(docsprofilessims_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${docsprofilessims_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
