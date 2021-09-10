SET(msdoc_frontend_qt_Qt_INCLUDES )

SET(msdoc_frontend_qt_UI_INCLUDES)

SET(msdoc_frontend_qt_TARGET_SOURCES msdoc_qt.cpp)

FOREACH(file ${msdoc_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(msdoc_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${msdoc_frontend_qt_UI_INCLUDES})

FOREACH(file ${msdoc_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(msdoc_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${msdoc_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
