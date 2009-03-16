SET(xmlengine_frontend_kde_UI_INCLUDES genginexml.ui)
SET(xmlengine_frontend_kde_MOC_INCLUDES genginexml_kde.h)

SET(xmlengine_frontend_kde_TARGET_SOURCES genginexml_kde.cpp)

FOREACH(file ${xmlengine_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(xmlengine_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${xmlengine_frontend_kde_UI_INCLUDES})

FOREACH(file ${xmlengine_frontend_kde_MOC_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_MOC_HEADERS(xmlengine_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${xmlengine_frontend_kde_MOC_INCLUDES})


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
