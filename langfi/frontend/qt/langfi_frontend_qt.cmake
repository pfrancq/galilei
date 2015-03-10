SET(langfi_frontend_qt_Qt_INCLUDES )

SET(langfi_frontend_qt_UI_INCLUDES)

SET(langfi_frontend_qt_TARGET_SOURCES langfi_qt.cpp)

FOREACH(file ${langfi_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langfi_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langfi_frontend_qt_UI_INCLUDES})

FOREACH(file ${langfi_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langfi_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langfi_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})