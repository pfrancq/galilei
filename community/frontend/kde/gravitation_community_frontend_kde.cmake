SET(gravitation_community_frontend_kde_UI_INCLUDES config.ui)

SET(gravitation_community_frontend_kde_TARGET_SOURCES gravitation_community_kde.cpp)

FOREACH(file ${gravitation_community_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(gravitation_community_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${gravitation_community_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
