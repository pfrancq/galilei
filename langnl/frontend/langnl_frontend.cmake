IF(KDE4_FOUND)
    SET(langnl_AVAILABLE_FRONTENDS kde)
    SET(langnl_kde_DEPENDENCY rcorekde ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(langnl_frontend langnl kde langnl_INCLUDES)
ENDIF(KDE4_FOUND)
