SET(docsgroupssims_frontend_qt_Qt_INCLUDES genericsims_qt.h)

SET(docsgroupssims_frontend_qt_UI_INCLUDES specific.ui)

SET(docsgroupssims_frontend_qt_TARGET_SOURCES docsgroupssims_qt.cpp genericsims_qt.cpp)

FOREACH(file ${docsgroupssims_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(docsgroupssims_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${docsgroupssims_frontend_qt_UI_INCLUDES})

FOREACH(file ${docsgroupssims_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(docsgroupssims_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${docsgroupssims_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

