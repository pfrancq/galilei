SET(profilesgroupssims_frontend_qt_Qt_INCLUDES genericsims_qt.h)

SET(profilesgroupssims_frontend_qt_UI_INCLUDES specific.ui)

SET(profilesgroupssims_frontend_qt_TARGET_SOURCES profilesgroupssims_qt.cpp genericsims_qt.cpp)

FOREACH(file ${profilesgroupssims_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(profilesgroupssims_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${profilesgroupssims_frontend_qt_UI_INCLUDES})

FOREACH(file ${profilesgroupssims_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(profilesgroupssims_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${profilesgroupssims_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

