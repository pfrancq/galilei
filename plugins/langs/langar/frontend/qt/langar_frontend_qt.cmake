SET(langar_frontend_qt_Qt_INCLUDES )

SET(langar_frontend_qt_UI_INCLUDES)

SET(langar_frontend_qt_TARGET_SOURCES langar_qt.cpp)

FOREACH(file ${langar_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langar_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langar_frontend_qt_UI_INCLUDES})

FOREACH(file ${langar_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langar_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langar_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})