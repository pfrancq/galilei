IF(KDE3_FOUND)	
    SET(galilei_AVAILABLE_FRONTENDS kde)
    SET(galilei_kde_DEPENDENCY rcorekde ${QT_AND_KDECORE_LIBS})
    ADD_FRONTEND_SOURCE_FILE(galilei_frontend galilei kde galilei_INCLUDES)
ENDIF(KDE3_FOUND)
  