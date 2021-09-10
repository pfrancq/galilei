SET(html_frontend_qt_Qt_INCLUDES )

SET(html_frontend_qt_UI_INCLUDES config.ui)

SET(html_frontend_qt_TARGET_SOURCES html_qt.cpp)

FOREACH(file ${html_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(html_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${html_frontend_qt_UI_INCLUDES})

FOREACH(file ${html_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(html_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${html_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
