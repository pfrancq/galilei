SET(langnl_frontend_qt_Qt_INCLUDES )

SET(langnl_frontend_qt_UI_INCLUDES)

SET(langnl_frontend_qt_TARGET_SOURCES langnl_qt.cpp)

FOREACH(file ${langnl_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langnl_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langnl_frontend_qt_UI_INCLUDES})

FOREACH(file ${langnl_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langnl_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langnl_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})