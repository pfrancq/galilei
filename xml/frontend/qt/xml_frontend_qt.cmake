SET(xml_frontend_qt_Qt_INCLUDES gxml_qt.h)

SET(xml_frontend_qt_UI_INCLUDES config.ui)

SET(xml_frontend_qt_TARGET_SOURCES gxml_qt.cpp)

FOREACH(file ${xml_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(xml_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${xml_frontend_qt_UI_INCLUDES})

FOREACH(file ${xml_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(xml_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${xml_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
