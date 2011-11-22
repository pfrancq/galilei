IF(KDE4_FOUND)
    SET(gstringindexer_AVAILABLE_FRONTENDS kde)
    SET(gstringindexer_kde_DEPENDENCY rcorekde ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(gstringindexer_frontend gstringindexer kde gstringindexer_INCLUDES)
ENDIF(KDE4_FOUND)

