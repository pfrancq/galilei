SET(docsagree_frontend_qt_Qt_INCLUDES)

SET(docsagree_frontend_qt_UI_INCLUDES)

SET(docsagree_frontend_qt_TARGET_SOURCES docsagree_qt.cpp)

FOREACH(file ${docsagree_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(docsagree_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${docsagree_frontend_qt_UI_INCLUDES})

FOREACH(file ${docsagree_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(docsagree_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${docsagree_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})