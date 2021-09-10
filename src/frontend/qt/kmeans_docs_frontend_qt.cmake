SET(kmeans_docs_frontend_qt_Qt_INCLUDES)

SET(kmeans_docs_frontend_qt_UI_INCLUDES config.ui)

SET(kmeans_docs_frontend_qt_TARGET_SOURCES kmeansdocs_qt.cpp)

FOREACH(file ${kmeans_docs_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(kmeans_docs_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${kmeans_docs_frontend_qt_UI_INCLUDES})

FOREACH(file ${kmeans_docs_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(kmeans_docs_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${kmeans_docs_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

