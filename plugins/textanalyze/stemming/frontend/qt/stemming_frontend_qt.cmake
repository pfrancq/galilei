SET(stemmingl_frontend_qt_Qt_INCLUDES )

SET(stemming_frontend_qt_UI_INCLUDES )

SET(stemming_frontend_qt_TARGET_SOURCES gstemming_qt.cpp)

FOREACH(file ${stemming_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(stemming_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${stemming_frontend_qt_UI_INCLUDES})

FOREACH(file ${stemming_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(stemmingl_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${stemming_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
