SET(promrank_frontend_qt_Qt_INCLUDES)

SET(promrank_frontend_qt_UI_INCLUDES gpromrank.ui)

SET(promrank_frontend_qt_TARGET_SOURCES gpromrank_qt.cpp)

FOREACH(file ${promrank_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(promrank_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${promrank_frontend_qt_UI_INCLUDES})

FOREACH(file ${promrank_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(promrank_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${promrank_frontend_qt_Qt_INCLUDES})