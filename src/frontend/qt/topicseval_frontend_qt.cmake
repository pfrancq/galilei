SET(topicseval_frontend_qt_Qt_INCLUDES )

SET(topicseval_frontend_qt_UI_INCLUDES )

SET(topicseval_frontend_qt_TARGET_SOURCES topicseval_qt.cpp)

FOREACH(file ${topicseval_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(topicseval_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${topicseval_frontend_qt_UI_INCLUDES})

FOREACH(file ${topicseval_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(topicseval_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${topicseval_frontend_qt_Qt_INCLUDES})