SET(langhu_frontend_qt_Qt_INCLUDES )

SET(langhu_frontend_qt_UI_INCLUDES)

SET(langhu_frontend_qt_TARGET_SOURCES langhu_qt.cpp)

FOREACH(file ${langhu_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langhu_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langhu_frontend_qt_UI_INCLUDES})

FOREACH(file ${langhu_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langhu_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langhu_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})