IF(KDE4_FOUND)
    SET(textanalyse_AVAILABLE_FRONTENDS kde)
    SET(textanalyse_kde_DEPENDENCY rcorekde ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(textanalyse_frontend textanalyse kde textanalyse_INCLUDES)
ENDIF(KDE4_FOUND)

