SET(langdk_frontend_qt_Qt_INCLUDES )

SET(langdk_frontend_qt_UI_INCLUDES)

SET(langdk_frontend_qt_TARGET_SOURCES langdk_qt.cpp)

FOREACH(file ${langdk_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langdk_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langdk_frontend_qt_UI_INCLUDES})

FOREACH(file ${langdk_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langdk_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langdk_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})