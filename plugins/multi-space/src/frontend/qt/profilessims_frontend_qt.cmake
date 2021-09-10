SET(profilessims_frontend_qt_Qt_INCLUDES genericsims_qt.h)

SET(profilessims_frontend_qt_UI_INCLUDES specific.ui)

SET(profilessims_frontend_qt_TARGET_SOURCES profilessims_qt.cpp genericsims_qt.cpp)

FOREACH(file ${profilessims_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(profilessims_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${profilessims_frontend_qt_UI_INCLUDES})

FOREACH(file ${profilessims_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(profilessims_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${profilessims_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


