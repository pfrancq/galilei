IF(KDE4_FOUND)
    SET(if_AVAILABLE_FRONTENDS kde)
    SET(if_kde_DEPENDENCY rcorekde galileikde galileikde ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(if_frontend if kde if_INCLUDES)
ENDIF(KDE4_FOUND)
