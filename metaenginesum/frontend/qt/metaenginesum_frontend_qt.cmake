SET(metaenginesum_frontend_qt_Qt_INCLUDES gmetaenginesum_qt.h)

SET(metaenginesum_frontend_qt_UI_INCLUDES config.ui)

SET(metaenginesum_frontend_qt_TARGET_SOURCES gmetaenginesum_qt.cpp)

FOREACH(file ${metaenginesum_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(metaenginesum_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${metaenginesum_frontend_qt_UI_INCLUDES})

FOREACH(file ${metaenginesum_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(metaenginesum_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${metaenginesum_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

