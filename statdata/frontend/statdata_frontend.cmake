IF(KDE4_FOUND)
    SET(statdata_AVAILABLE_FRONTENDS kde)
    SET(statdata_kde_DEPENDENCY rcorekde ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(statdata_frontend statdata kde statdata_INCLUDES)
ENDIF(KDE4_FOUND)
