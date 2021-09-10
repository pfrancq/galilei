SET(docsprofilessims_frontend_qt_Qt_INCLUDES genericsims_qt.h)

SET(docsprofilessims_frontend_qt_UI_INCLUDES specific.ui)

SET(docsprofilessims_frontend_qt_TARGET_SOURCES docsprofilessims_qt.cpp genericsims_qt.cpp)

FOREACH(file ${docsprofilessims_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(docsprofilessims_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${docsprofilessims_frontend_qt_UI_INCLUDES})

FOREACH(file ${docsprofilessims_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(docsprofilessims_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${docsprofilessims_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})