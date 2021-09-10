SET(wikipedia_frontend_qt_Qt_INCLUDES wikipedia_qt.h)

SET(wikipedia_frontend_qt_UI_INCLUDES config.ui)

SET(wikipedia_frontend_qt_TARGET_SOURCES wikipedia_qt.cpp)

FOREACH(file ${wikipedia_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(wikipedia_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${wikipedia_frontend_qt_UI_INCLUDES})

FOREACH(file ${wikipedia_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(wikipedia_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${wikipedia_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
