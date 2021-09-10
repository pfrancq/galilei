SET(gravitation_topic_frontend_qt_Qt_INCLUDES)

SET(gravitation_topic_frontend_qt_UI_INCLUDES config.ui)

SET(gravitation_topic_frontend_qt_TARGET_SOURCES gravitation_topic_qt.cpp)

FOREACH(file ${gravitation_topic_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(gravitation_topic_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${gravitation_topic_frontend_qt_UI_INCLUDES})

FOREACH(file ${gravitation_topic_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(gravitation_topic_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${gravitation_topic_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})