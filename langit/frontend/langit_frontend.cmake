IF(KDE4_FOUND)
    SET(langit_AVAILABLE_FRONTENDS kde)
    SET(langit_kde_DEPENDENCY rcorekde ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(langit_frontend langit kde langit_INCLUDES)
ENDIF(KDE4_FOUND)
