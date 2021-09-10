SET(subslevel_frontend_qt_Qt_INCLUDES )

SET(subslevel_frontend_qt_UI_INCLUDES config.ui)

SET(subslevel_frontend_qt_TARGET_SOURCES subslevel_qt.cpp)

FOREACH(file ${subslevel_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(subslevel_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${subslevel_frontend_qt_UI_INCLUDES})

FOREACH(file ${subslevel_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(subslevel_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${subslevel_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
