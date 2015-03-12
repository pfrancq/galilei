SET(docsdisagree_frontend_qt_Qt_INCLUDES)

SET(docsdisagree_frontend_qt_UI_INCLUDES)

SET(docsdisagree_frontend_qt_TARGET_SOURCES docsdisagree_qt.cpp)

FOREACH(file ${docsdisagree_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(docsdisagree_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${docsdisagree_frontend_qt_UI_INCLUDES})

FOREACH(file ${docsdisagree_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(docsdisagree_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${docsdisagree_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})