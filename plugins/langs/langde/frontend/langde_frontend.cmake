IF(QT4_FOUND)
    SET(langde_AVAILABLE_FRONTENDS qt)
    SET(langde_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(langde_frontend langde qt langde_INCLUDES)
ENDIF(QT4_FOUND)
