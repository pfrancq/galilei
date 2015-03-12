SET(statfeatures_frontend_qt_Qt_INCLUDES statfeatures_qt.h)

SET(statfeatures_frontend_qt_UI_INCLUDES config.ui)

SET(statfeatures_frontend_qt_TARGET_SOURCES statfeatures_qt.cpp)

FOREACH(file ${statfeatures_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(statfeatures_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${statfeatures_frontend_qt_UI_INCLUDES})

FOREACH(file ${statfeatures_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(statfeatures_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${statfeatures_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
