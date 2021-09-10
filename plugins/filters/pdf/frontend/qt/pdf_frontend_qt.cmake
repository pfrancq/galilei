SET(pdf_frontend_qt_Qt_INCLUDES )

SET(pdf_frontend_qt_UI_INCLUDES)

SET(pdf_frontend_qt_TARGET_SOURCES pdf_qt.cpp)

FOREACH(file ${pdf_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(pdf_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${pdf_frontend_qt_UI_INCLUDES})

FOREACH(file ${pdf_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(pdf_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${pdf_frontend_qt_Qt_INCLUDES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
