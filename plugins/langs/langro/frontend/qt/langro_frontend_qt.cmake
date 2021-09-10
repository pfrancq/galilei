SET(langro_frontend_qt_Qt_INCLUDES )

SET(langro_frontend_qt_UI_INCLUDES)

SET(langro_frontend_qt_TARGET_SOURCES langro_qt.cpp)

FOREACH(file ${langro_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langro_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langro_frontend_qt_UI_INCLUDES})

FOREACH(file ${langro_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langro_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langro_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})