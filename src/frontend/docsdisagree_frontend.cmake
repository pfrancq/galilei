IF(KDE4_FOUND)
    SET(docsdisagree_AVAILABLE_FRONTENDS kde)
    SET(docsdisagree_kde_DEPENDENCY rcoreqt galileiqt ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(docsdisagree_frontend docsdisagree kde docsdisagree_INCLUDES)
ENDIF(KDE4_FOUND)
