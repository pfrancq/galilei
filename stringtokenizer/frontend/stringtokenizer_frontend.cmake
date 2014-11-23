IF(KDE4_FOUND)
    SET(stringtokenizer_AVAILABLE_FRONTENDS kde)
    SET(stringtokenizer_kde_DEPENDENCY rcoreqt ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(stringtokenizer_frontend stringtokenizer kde stringtokenizer_INCLUDES)
ENDIF(KDE4_FOUND)

