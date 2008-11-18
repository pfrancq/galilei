SET(gravitation_topic_frontend_kde_UI_INCLUDES config.ui)

SET(gravitation_topic_frontend_kde_TARGET_SOURCES gravitation_topic_kde.cpp)

FOREACH(file ${gravitation_topic_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(gravitation_topic_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${gravitation_topic_frontend_kde_UI_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
