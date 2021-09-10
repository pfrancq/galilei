SET(rtf_frontend_qt_Qt_INCLUDES )

SET(rtf_frontend_qt_UI_INCLUDES)

SET(rtf_frontend_qt_TARGET_SOURCES rtf_qt.cpp)

FOREACH(file ${rtf_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(rtf_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${rtf_frontend_qt_UI_INCLUDES})

FOREACH(file ${rtf_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(rtf_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${rtf_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
