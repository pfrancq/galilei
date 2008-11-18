IF(KDE4_FOUND)	
    SET(galilei_AVAILABLE_FRONTENDS kde)
    SET(galilei_kde_DEPENDENCY rcorekde ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} )
    ADD_FRONTEND_SOURCE_FILE(galilei_frontend galilei kde galilei_INCLUDES)
ENDIF(KDE4_FOUND)
  