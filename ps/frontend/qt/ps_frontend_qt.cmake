SET(ps_frontend_qt_Qt_INCLUDES )

SET(ps_frontend_qt_UI_INCLUDES)

SET(ps_frontend_qt_TARGET_SOURCES ps_qt.cpp)

FOREACH(file ${ps_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(ps_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${ps_frontend_qt_UI_INCLUDES})

FOREACH(file ${ps_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(ps_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${ps_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
