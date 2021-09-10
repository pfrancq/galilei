SET(txt_frontend_qt_Qt_INCLUDES )

SET(txt_frontend_qt_UI_INCLUDES)

SET(txt_frontend_qt_TARGET_SOURCES txt_qt.cpp)

FOREACH(file ${txt_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(txt_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${txt_frontend_qt_UI_INCLUDES})

FOREACH(file ${txt_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(txt_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${txt_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
