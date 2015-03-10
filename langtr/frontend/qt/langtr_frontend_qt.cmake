SET(langtr_frontend_qt_Qt_INCLUDES )

SET(langtr_frontend_qt_UI_INCLUDES)

SET(langtr_frontend_qt_TARGET_SOURCES langtr_qt.cpp)

FOREACH(file ${langtr_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langtr_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langtr_frontend_qt_UI_INCLUDES})

FOREACH(file ${langtr_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langtr_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langtr_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})