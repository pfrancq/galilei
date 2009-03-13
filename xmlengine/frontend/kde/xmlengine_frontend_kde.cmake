SET(xmlengine_frontend_kde_UI_INCLUDES genginexml.ui)

SET(xmlengine_frontend_kde_TARGET_SOURCES genginexml_kde.cpp)

FOREACH(file ${xmlengine_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(xmlengine_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${xmlengine_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
