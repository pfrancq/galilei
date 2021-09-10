SET(gstringindexer_frontend_qt_Qt_INCLUDES )

SET(gstringindexer_frontend_qt_UI_INCLUDES config.ui)

SET(gstringindexer_frontend_qt_TARGET_SOURCES gstringindexer_qt.cpp)

FOREACH(file ${gstringindexer_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(gstringindexer_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${gstringindexer_frontend_qt_UI_INCLUDES})

FOREACH(file ${gstringindexer_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(gstringindexer_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${gstringindexer_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
