IF(KDE3_FOUND)
    SET(docsdisagree_AVAILABLE_FRONTENDS kde)
    SET(docsdisagree_kde_DEPENDENCY rcorekde ${QT_AND_KDECORE_LIBS})
    ADD_FRONTEND_SOURCE_FILE(docsdisagree_frontend docsdisagree kde docsdisagree_INCLUDES)
ENDIF(KDE3_FOUND)
