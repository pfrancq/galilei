IF(QT4_FOUND)
    SET(logentropy_AVAILABLE_FRONTENDS qt)
    SET(logentropy_qt_DEPENDENCY rcoreqt galileiqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(logentropy_frontend logentropy qt logentropy_INCLUDES)
ENDIF(QT4_FOUND)
