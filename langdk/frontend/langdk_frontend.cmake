IF(KDE4_FOUND)
    SET(langdk_AVAILABLE_FRONTENDS kde)
    SET(langdk_kde_DEPENDENCY rcoreqt ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(langdk_frontend langdk kde langdk_INCLUDES)
ENDIF(KDE4_FOUND)
