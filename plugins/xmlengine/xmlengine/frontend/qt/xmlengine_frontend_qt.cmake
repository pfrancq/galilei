SET(xmlengine_frontend_qt_Qt_INCLUDES )

SET(xmlengine_frontend_qt_UI_INCLUDES genginexml.ui)

SET(xmlengine_frontend_qt_TARGET_SOURCES genginexml_qt.cpp)

FOREACH(file ${xmlengine_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(xmlengine_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${xmlengine_frontend_qt_UI_INCLUDES})

FOREACH(file ${xmlengine_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(xmlengine_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${xmlengine_frontend_qt_Qt_INCLUDES})