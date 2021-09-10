SET(langno_frontend_qt_Qt_INCLUDES )

SET(langno_frontend_qt_UI_INCLUDES)

SET(langno_frontend_qt_TARGET_SOURCES langno_qt.cpp)

FOREACH(file ${langno_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langno_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langno_frontend_qt_UI_INCLUDES})

FOREACH(file ${langno_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langno_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langno_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})