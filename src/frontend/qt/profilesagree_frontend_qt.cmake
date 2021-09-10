SET(profilesagree_frontend_qt_Qt_INCLUDES)

SET(profilesagree_frontend_qt_UI_INCLUDES)

SET(profilesagree_frontend_qt_TARGET_SOURCES profilesagree_qt.cpp)

FOREACH(file ${profilesagree_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(profilesagree_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${profilesagree_frontend_qt_UI_INCLUDES})

FOREACH(file ${profilesagree_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(profilesagree_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${profilesagree_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})