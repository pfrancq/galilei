IF(KDE3_FOUND)
    SET(gravitation_topic_AVAILABLE_FRONTENDS kde)
    SET(gravitation_topic_kde_DEPENDENCY rcorekde ${QT_AND_KDECORE_LIBS})
    ADD_FRONTEND_SOURCE_FILE(gravitation_topic_frontend gravitation_topic kde gravitation_topic_INCLUDES)
ENDIF(KDE3_FOUND)
