IF(KDE4_FOUND)
    SET(gravitation_topic_AVAILABLE_FRONTENDS kde)
    SET(gravitation_topic_kde_DEPENDENCY rcorekde ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(gravitation_topic_frontend gravitation_topic kde gravitation_topic_INCLUDES)
ENDIF(KDE4_FOUND)
