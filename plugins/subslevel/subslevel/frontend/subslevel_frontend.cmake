IF(QT4_FOUND)
    SET(subslevel_AVAILABLE_FRONTENDS qt)
    SET(subslevel_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(subslevel_frontend subslevel qt subslevel_INCLUDES)
ENDIF(QT4_FOUND)
