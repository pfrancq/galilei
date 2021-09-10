SET(gravitation_community_frontend_qt_Qt_INCLUDES)

SET(gravitation_community_frontend_qt_UI_INCLUDES config.ui)

SET(gravitation_community_frontend_qt_TARGET_SOURCES gravitation_community_qt.cpp)

FOREACH(file ${gravitation_community_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(gravitation_community_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${gravitation_community_frontend_qt_UI_INCLUDES})

FOREACH(file ${gravitation_community_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(gravitation_community_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${gravitation_community_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


