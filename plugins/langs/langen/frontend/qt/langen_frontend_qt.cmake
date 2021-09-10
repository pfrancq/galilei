SET(langen_frontend_qt_Qt_INCLUDES )

SET(langen_frontend_qt_UI_INCLUDES)

SET(langen_frontend_qt_TARGET_SOURCES langen_qt.cpp)

FOREACH(file ${langen_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langen_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langen_frontend_qt_UI_INCLUDES})

FOREACH(file ${langen_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langen_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langen_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})