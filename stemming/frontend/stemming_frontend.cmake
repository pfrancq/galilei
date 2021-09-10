IF(QT4_FOUND)
    SET(stemming_AVAILABLE_FRONTENDS qt)
    SET(stemming_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(stemming_frontend stemming qt stemming_INCLUDES)
ENDIF(QT4_FOUND)

