IF(QT4_FOUND)
    SET(langnl_AVAILABLE_FRONTENDS qt)
    SET(langnl_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(langnl_frontend langnl qt langnl_INCLUDES)
ENDIF(QT4_FOUND)