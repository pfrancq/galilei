SET(promrank_frontend_kde_UI_INCLUDES gpromrank.ui)
SET(promrank_frontend_kde_MOC_INCLUDES gpromrank_kde.h)

SET(promrank_frontend_kde_TARGET_SOURCES gpromrank_kde.cpp)

FOREACH(file ${promrank_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(promrank_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${promrank_frontend_kde_UI_INCLUDES})

FOREACH(file ${promrank_frontend_kde_MOC_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_MOC_HEADERS(promrank_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${promrank_frontend_kde_MOC_INCLUDES})


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
