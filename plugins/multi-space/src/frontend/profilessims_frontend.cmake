IF(QT4_FOUND)
    SET(profilessims_AVAILABLE_FRONTENDS qt)
    SET(profilessims_qt_DEPENDENCY rcoreqt galileiqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(profilessims_frontend profilessims qt profilessims_INCLUDES)
ENDIF(QT4_FOUND)
