SET(stopwords_frontend_qt_Qt_INCLUDES )

SET(stopwords_frontend_qt_UI_INCLUDES config.ui)

SET(stopwords_frontend_qt_TARGET_SOURCES gstopwords_qt.cpp)

FOREACH(file ${stopwords_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(stopwords_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${stopwords_frontend_qt_UI_INCLUDES})

FOREACH(file ${stopwords_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(stopwords_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${stopwords_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
