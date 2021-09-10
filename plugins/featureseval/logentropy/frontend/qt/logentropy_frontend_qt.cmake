SET(logentropy_frontend_qt_Qt_INCLUDES )

SET(logentropy_frontend_qt_UI_INCLUDES)

SET(logentropy_frontend_qt_TARGET_SOURCES logentropy_qt.cpp)

FOREACH(file ${logentropy_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(gmysql_logentropy_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${gmysql_logentropy_qt_UI_INCLUDES})

FOREACH(file ${gmysql_logentropy_qt_Qt_INCLUDES})
   SET(REALFILE logentropy/qt/${file})
   QT4_WRAP_CPP(gmysql_logentropy_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${gmysql_logentropy_qt_Qt_INCLUDES})

