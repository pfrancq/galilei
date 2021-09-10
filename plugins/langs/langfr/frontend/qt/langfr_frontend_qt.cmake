SET(langfr_frontend_qt_Qt_INCLUDES )

SET(langfr_frontend_qt_UI_INCLUDES)

SET(langfr_frontend_qt_TARGET_SOURCES langfr_qt.cpp)

FOREACH(file ${langfr_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langfr_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langfr_frontend_qt_UI_INCLUDES})

FOREACH(file ${langfr_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langfr_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langfr_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})