IF(QT4_FOUND)
    SET(statsims_AVAILABLE_FRONTENDS qt)
    SET(statsims_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(statsims_frontend statsims qt statsims_INCLUDES)
ENDIF(QT4_FOUND)