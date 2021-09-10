SET(thesaurus_frontend_qt_Qt_INCLUDES )

SET(thesaurus_frontend_qt_UI_INCLUDES config.ui)

SET(thesaurus_frontend_qt_TARGET_SOURCES thesaurus_qt.cpp)

FOREACH(file ${thesaurus_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(thesaurus_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${thesaurus_frontend_qt_UI_INCLUDES})

FOREACH(file ${thesaurus_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(thesaurus_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${thesaurus_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

