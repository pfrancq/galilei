IF(QT4_FOUND)
    SET(classeseval_AVAILABLE_FRONTENDS qt)
    SET(classeseval_qt_DEPENDENCY rcoreqt galileiqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(classeseval_frontend classeseval qt classeseval_INCLUDES)
ENDIF(QT4_FOUND)
