SET(langru_frontend_qt_Qt_INCLUDES )

SET(langru_frontend_qt_UI_INCLUDES)

SET(langru_frontend_qt_TARGET_SOURCES langru_qt.cpp)

FOREACH(file ${langru_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langru_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langru_frontend_qt_UI_INCLUDES})

FOREACH(file ${langru_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langru_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langru_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})