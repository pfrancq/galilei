SET(parser_frontend_qt_Qt_INCLUDES )

SET(parser_frontend_qt_UI_INCLUDES)

SET(parser_frontend_qt_TARGET_SOURCES wparser_qt.cpp)

FOREACH(file ${parser_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(parser_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${parser_frontend_qt_UI_INCLUDES})

FOREACH(file ${parser_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(parser_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${parser_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
