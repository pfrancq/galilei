SET(langpt_frontend_qt_Qt_INCLUDES )

SET(langpt_frontend_qt_UI_INCLUDES)

SET(langpt_frontend_qt_TARGET_SOURCES langpt_qt.cpp)

FOREACH(file ${langpt_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langpt_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langpt_frontend_qt_UI_INCLUDES})

FOREACH(file ${langpt_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langpt_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langpt_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})