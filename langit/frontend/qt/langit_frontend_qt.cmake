SET(langfr_frontend_qt_Qt_INCLUDES )

SET(langit_frontend_qt_UI_INCLUDES)

SET(langit_frontend_qt_TARGET_SOURCES langit_qt.cpp)

FOREACH(file ${langit_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langit_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langit_frontend_qt_UI_INCLUDES})

FOREACH(file ${langit_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langit_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langit_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})