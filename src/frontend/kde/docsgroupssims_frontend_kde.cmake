SET(docsgroupssims_frontend_kde_UI_INCLUDES specific.ui)

SET(docsgroupssims_frontend_kde_TARGET_SOURCES genericsims_kde.cpp docsgroupssims_kde.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

FOREACH(file ${docsgroupssims_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
	KDE4_ADD_UI_FILES(docsgroupssims_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${docsgroupssims_frontend_kde_UI_INCLUDES})


