IF(KDE4_FOUND)
    SET(langes_AVAILABLE_FRONTENDS kde)
    SET(langes_kde_DEPENDENCY rcorekde ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(langes_frontend langes kde langes_INCLUDES)
ENDIF(KDE4_FOUND)
