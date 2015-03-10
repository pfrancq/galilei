SET(gmysql_frontend_qt_Qt_INCLUDES )

SET(gmysql_frontend_qt_UI_INCLUDES config.ui)

SET(gmysql_frontend_qt_TARGET_SOURCES gmysql_qt.cpp)

FOREACH(file ${gmysql_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(gmysql_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${gmysql_frontend_qt_UI_INCLUDES})

FOREACH(file ${gmysql_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(gmysql_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${gmysql_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
