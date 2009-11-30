IF(KDE4_FOUND)
    SET(textanalyze_AVAILABLE_FRONTENDS kde)
    SET(textanalyze_kde_DEPENDENCY rcorekde ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(textanalyze_frontend textanalyze kde textanalyze_INCLUDES)
ENDIF(KDE4_FOUND)

