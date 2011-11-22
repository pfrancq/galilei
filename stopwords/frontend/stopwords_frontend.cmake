IF(KDE4_FOUND)
    SET(stopwords_AVAILABLE_FRONTENDS kde)
    SET(stopwords_kde_DEPENDENCY rcorekde ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(stopwords_frontend stopwords kde stopwords_INCLUDES)
ENDIF(KDE4_FOUND)

