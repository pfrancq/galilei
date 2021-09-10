SET(sgga_docs_frontend_qt_Qt_INCLUDES sggaconfig_qt.h)

SET(sgga_docs_frontend_qt_UI_INCLUDES config.ui)

SET(sgga_docs_frontend_qt_TARGET_SOURCES sggagroupdocs_qt.cpp sggaconfig_qt.cpp)

FOREACH(file ${sgga_docs_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(sgga_docs_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${sgga_docs_frontend_qt_UI_INCLUDES})

FOREACH(file ${sgga_docs_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(sgga_docs_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${sgga_docs_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

