SET(email_frontend_qt_Qt_INCLUDES )

SET(email_frontend_qt_UI_INCLUDES)

SET(email_frontend_qt_TARGET_SOURCES email_qt.cpp)

FOREACH(file ${email_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(email_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${email_frontend_qt_UI_INCLUDES})

FOREACH(file ${email_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(email_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${email_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
