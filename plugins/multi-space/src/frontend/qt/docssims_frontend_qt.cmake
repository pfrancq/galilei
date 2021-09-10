SET(docssims_frontend_qt_Qt_INCLUDES genericsims_qt.h)

SET(docssims_frontend_qt_UI_INCLUDES specific.ui)

SET(docssims_frontend_qt_TARGET_SOURCES docssims_qt.cpp genericsims_qt.cpp)

FOREACH(file ${docssims_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(docssims_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${docssims_frontend_qt_UI_INCLUDES})

FOREACH(file ${docssims_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(docssims_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${docssims_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

