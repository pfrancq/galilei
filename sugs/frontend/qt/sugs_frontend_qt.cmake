SET(sugs_frontend_qt_Qt_INCLUDES )

SET(sugs_frontend_qt_UI_INCLUDES config.ui)

SET(sugs_frontend_qt_TARGET_SOURCES sugs_qt.cpp)

FOREACH(file ${sugs_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(sugs_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${sugs_frontend_qt_UI_INCLUDES})

FOREACH(file ${sugs_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(sugs_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${sugs_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
