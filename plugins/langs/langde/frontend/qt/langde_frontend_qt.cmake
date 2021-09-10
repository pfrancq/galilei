SET(langde_frontend_qt_Qt_INCLUDES )

SET(langde_frontend_qt_UI_INCLUDES)

SET(langde_frontend_qt_TARGET_SOURCES langde_qt.cpp)

FOREACH(file ${langde_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langde_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langde_frontend_qt_UI_INCLUDES})

FOREACH(file ${langde_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langde_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langde_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})