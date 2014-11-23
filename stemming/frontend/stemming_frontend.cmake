IF(KDE4_FOUND)
    SET(stemming_AVAILABLE_FRONTENDS kde)
    SET(stemming_kde_DEPENDENCY rcoreqt ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(stemming_frontend stemming kde stemming_INCLUDES)
ENDIF(KDE4_FOUND)

