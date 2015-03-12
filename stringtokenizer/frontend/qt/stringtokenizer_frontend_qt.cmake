SET(stringtokenizer_frontend_qt_Qt_INCLUDES )

SET(stringtokenizer_frontend_qt_UI_INCLUDES config.ui)

SET(stringtokenizer_frontend_qt_TARGET_SOURCES gstringtokenizer_qt.cpp)

FOREACH(file ${stringtokenizer_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(stringtokenizer_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${stringtokenizer_frontend_qt_UI_INCLUDES})

FOREACH(file ${stringtokenizer_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(stringtokenizer_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${stringtokenizer_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
