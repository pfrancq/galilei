SET(if_frontend_kde_TARGET_SOURCES if_kde.cpp)

SET(if_frontend_qt_Qt_INCLUDES )

SET(if_frontend_qt_UI_INCLUDES)

SET(if_frontend_qt_TARGET_SOURCES if_qt.cpp)

FOREACH(file ${if_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(if_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${if_frontend_qt_UI_INCLUDES})

FOREACH(file ${if_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(if_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${if_frontend_qt_Qt_INCLUDES})

