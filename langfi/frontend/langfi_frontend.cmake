IF(KDE4_FOUND)
    SET(langfi_AVAILABLE_FRONTENDS kde)
    SET(langfi_kde_DEPENDENCY rcorekde ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(langfi_frontend langfi kde langfi_INCLUDES)
ENDIF(KDE4_FOUND)
