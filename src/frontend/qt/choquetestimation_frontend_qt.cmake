SET(choquetestimation_frontend_qt_Qt_INCLUDES)

SET(choquetestimation_frontend_qt_UI_INCLUDES)

SET(choquetestimation_frontend_qt_TARGET_SOURCES choquetestimation_qt.cpp)

FOREACH(file ${choquetestimation_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(choquetestimation_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${choquetestimation_frontend_qt_UI_INCLUDES})

FOREACH(file ${choquetestimation_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(choquetestimation_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${choquetestimation_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

