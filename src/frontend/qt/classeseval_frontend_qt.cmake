SET(classeseval_frontend_kde_TARGET_SOURCES classeseval_kde.cpp)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET(classeseval_frontend_qt_Qt_INCLUDES )

SET(classeseval_frontend_qt_UI_INCLUDES)

SET(classeseval_frontend_qt_TARGET_SOURCES classeseval_qt.cpp)

FOREACH(file ${classeseval_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(classeseval_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${classeseval_frontend_qt_UI_INCLUDES})

FOREACH(file ${classeseval_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(classeseval_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${classeseval_frontend_qt_Qt_INCLUDES})