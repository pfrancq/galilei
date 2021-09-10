SET(profilesdisagree_frontend_qt_Qt_INCLUDES)

SET(profilesdisagree_frontend_qt_UI_INCLUDES)

SET(profilesdisagree_frontend_qt_TARGET_SOURCES profilesdisagree_qt.cpp)

FOREACH(file ${profilesdisagree_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(profilesdisagree_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${profilesdisagree_frontend_qt_UI_INCLUDES})

FOREACH(file ${profilesdisagree_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(profilesdisagree_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${profilesdisagree_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
