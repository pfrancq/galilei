SET(langsv_frontend_qt_Qt_INCLUDES )

SET(langsv_frontend_qt_UI_INCLUDES)

SET(langsv_frontend_qt_TARGET_SOURCES langsv_qt.cpp)

FOREACH(file ${langsv_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(langsv_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langsv_frontend_qt_UI_INCLUDES})

FOREACH(file ${langsv_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(langsv_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${langsv_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})