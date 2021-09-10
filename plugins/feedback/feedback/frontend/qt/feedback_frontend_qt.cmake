SET(feedback_frontend_qt_Qt_INCLUDES )

SET(feedback_frontend_qt_UI_INCLUDES config.ui)

SET(feedback_frontend_qt_TARGET_SOURCES feedback_qt.cpp)

FOREACH(file ${feedback_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(feedback_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${feedback_frontend_qt_UI_INCLUDES})

FOREACH(file ${feedback_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(feedback_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${feedback_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})