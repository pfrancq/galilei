IF(KDE4_FOUND)
    SET(featureseval_AVAILABLE_FRONTENDS kde)
    SET(featureseval_kde_DEPENDENCY rcorekde galileikde galileikde ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
    ADD_FRONTEND_SOURCE_FILE(featureseval_frontend featureseval kde featureseval_INCLUDES)
ENDIF(KDE4_FOUND)
