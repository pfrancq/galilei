SET(langes_frontend_qt_Qt_INCLUDES )

SET(langes_frontend_qt_UI_INCLUDES)

SET(langes_frontend_qt_TARGET_SOURCES langes_qt.cpp)

FOREACH(file ${langes_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langes_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langes_frontend_qt_UI_INCLUDES})

FOREACH(file ${langes_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langes_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langes_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})