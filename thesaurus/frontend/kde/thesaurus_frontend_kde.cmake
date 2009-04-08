SET(thesaurus_frontend_kde_UI_INCLUDES config.ui)

SET(thesaurus_frontend_kde_TARGET_SOURCES thesaurus_kde.cpp)

FOREACH(file ${thesaurus_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(thesaurus_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${thesaurus_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
