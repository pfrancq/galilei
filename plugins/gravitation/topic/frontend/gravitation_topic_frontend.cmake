IF(QT4_FOUND)
    SET(gravitation_topic_AVAILABLE_FRONTENDS qt)
    SET(gravitation_topic_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(gravitation_topic_frontend gravitation_topic qt gravitation_topic_INCLUDES)
ENDIF(QT4_FOUND)
