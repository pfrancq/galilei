SET(xml_frontend_kde_UI_INCLUDES config.ui)

SET(xml_frontend_kde_TARGET_SOURCES gxml_kde.cpp)

KDE4_HANDLE_AUTOMOC(xml_frontend_kde_MOCFILE frontend/kde/gxml_kde.cpp)

FOREACH(file ${xml_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(xml_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${xml_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
