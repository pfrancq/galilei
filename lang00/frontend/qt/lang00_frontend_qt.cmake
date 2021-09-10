SET(lang00_frontend_qt_Qt_INCLUDES )

SET(lang00_frontend_qt_UI_INCLUDES)

SET(lang00_frontend_qt_TARGET_SOURCES lang00_qt.cpp)

FOREACH(file ${lang00_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(lang00_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${lang00_frontend_qt_UI_INCLUDES})

FOREACH(file ${lang00_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(lang00_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${lang00_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})