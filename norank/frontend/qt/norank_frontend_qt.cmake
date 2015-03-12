SET(norank_frontend_qt_Qt_INCLUDES gnorank_qt.h)

SET(norank_frontend_qt_UI_INCLUDES config.ui)

SET(norank_frontend_qt_TARGET_SOURCES gnorank_qt.cpp)

FOREACH(file ${norank_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(norank_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${norank_frontend_qt_UI_INCLUDES})

FOREACH(file ${norank_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(norank_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${norank_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})


