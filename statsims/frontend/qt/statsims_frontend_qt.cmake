SET(statsims_frontend_qt_Qt_INCLUDES statsims_qt.h)

SET(statsims_frontend_qt_UI_INCLUDES config.ui)

SET(statsims_frontend_qt_TARGET_SOURCES statsims_qt.cpp)

FOREACH(file ${statsims_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(statsims_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${statsims_frontend_qt_UI_INCLUDES})

FOREACH(file ${statsims_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(statsims_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${statsims_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
